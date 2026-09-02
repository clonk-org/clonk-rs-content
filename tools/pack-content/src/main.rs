//! Builds `content.zip`, the archive a clonk-rs client downloads when it
//! updates its game data.
//!
//! # Why the builder lives here
//!
//! There must be exactly **one** producer of these bytes. The archive is
//! content-addressed — clonk-rs records its SHA-256 in the update manifest and
//! a client re-downloads only when that digest changes — so two independent
//! "deterministic zip" implementations would have to agree byte for byte
//! forever. The day they drifted, the digest would move without the content
//! moving and every install would re-fetch the entire archive. The engine
//! repository therefore stopped building this archive and now references the
//! one published here, beside the files it is made of.
//!
//! # What determinism costs
//!
//! Every axis that could vary between runs or runners is pinned rather than
//! inherited from the environment:
//!
//! * **Membership** — the data-root entries `packs.toml` lists, read
//!   from the Git index rather than the working tree, so an untracked scratch
//!   file cannot enter the archive and neither can a tracked one that nobody
//!   described. There is no deny list to keep in step with the engine
//!   repository: its installer copies the same manifest's entries.
//! * **Order** — entries sorted by their name inside the zip, not by directory
//!   iteration order.
//! * **Timestamps** — the 1980 zip epoch. `FileOptions::default()` reads the
//!   wall clock when the `zip` crate's `time` feature is enabled, which feature
//!   unification could turn on at any time.
//! * **Modes** — a fixed 0o644. Nothing in the content tree is an executable,
//!   and a runner whose umask differed would otherwise produce a different
//!   archive from identical files.
//! * **Compression** — Deflate, spelled out, through the pure-Rust backend the
//!   lockfile pins.
//!
//! The archive is also **prefix-free**: entries begin `Objects.c4d/…`, not
//! `content/Objects.c4d/…`, because a client unpacks it into whichever
//! directory its platform keeps game data in.

use anyhow::{bail, Context, Result};
use std::path::{Path, PathBuf};
use std::process::Command;
use zip::write::FileOptions;
use zip::{CompressionMethod, ZipWriter};

/// The published asset name. Fixed rather than digest-named: the release tag
/// already names the commit, and a stable name keeps the download URL
/// predictable.
const ARCHIVE_NAME: &str = "content.zip";

/// Where the manifest lives, relative to the repository root.
///
/// The engine repository's installer reads the same file from the same place
/// (`content/packs.toml` from its point of view), so the two producers of a
/// player's data root agree on membership by construction.
const MANIFEST_PATH: &str = "packs.toml";

fn main() -> Result<()> {
    let mut arguments = std::env::args().skip(1);
    let output = arguments
        .next()
        .map(PathBuf::from)
        .unwrap_or_else(|| PathBuf::from(ARCHIVE_NAME));
    if let Some(unexpected) = arguments.next() {
        bail!("unexpected argument `{unexpected}`; usage: pack-content [<output.zip>]");
    }

    let root = repository_root()?;
    refuse_a_dirty_worktree(&root)?;
    let manifest = root.join(MANIFEST_PATH);
    let roots = data_root_entries(
        &std::fs::read_to_string(&manifest)
            .with_context(|| format!("failed to read {}", manifest.display()))?,
    )
    .with_context(|| format!("{} does not describe the data root", manifest.display()))?;
    let files = content_files(&root, &roots)?;
    if files.is_empty() {
        bail!(
            "no content files are tracked in {}; refusing to publish an empty archive",
            root.display()
        );
    }

    write_deterministic_zip(&output, &root, &files)?;

    let digest = hex_digest(&sha256_file(&output)?);
    let sidecar = output.with_extension("sha256");
    // `sha256sum -c` format, so the sidecar is checkable with the tool everyone
    // already has rather than only by this program.
    let name = output
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_else(|| ARCHIVE_NAME.to_string());
    std::fs::write(&sidecar, format!("{digest}  {name}\n"))
        .with_context(|| format!("failed to write {}", sidecar.display()))?;

    println!("{digest}  {name}");
    println!("{} files", files.len());
    Ok(())
}

fn repository_root() -> Result<PathBuf> {
    let output = Command::new("git")
        .args(["rev-parse", "--show-toplevel"])
        .output()
        .context("failed to run `git rev-parse --show-toplevel`")?;
    if !output.status.success() {
        bail!("`git rev-parse --show-toplevel` failed; run this inside the content repository");
    }
    let text = String::from_utf8(output.stdout).context("git printed a non-UTF-8 path")?;
    Ok(PathBuf::from(text.trim_end()))
}

/// Refuses to package a tree whose tracked files differ from the index.
///
/// The release is tagged with a commit, so an archive built from modified
/// working-tree bytes would carry a digest that commit can never reproduce.
fn refuse_a_dirty_worktree(root: &Path) -> Result<()> {
    let status = Command::new("git")
        .arg("-C")
        .arg(root)
        .args(["diff", "--quiet", "--no-ext-diff"])
        .status()
        .context("failed to inspect the working tree")?;
    match status.code() {
        Some(0) => Ok(()),
        Some(1) => bail!(
            "tracked files differ from the Git index; commit or discard those changes before \
             packaging, or the published digest will describe no commit"
        ),
        other => bail!("`git diff` failed with status {other:?}"),
    }
}

/// The data-root entries the manifest describes, sorted.
///
/// A key that reaches inside a pack (`Melees.c4f/Queron3.c4s`) records policy
/// for a nested tree the enclosing entry already ships, so it is not a root.
fn data_root_entries(manifest: &str) -> Result<Vec<String>> {
    let document: toml::Table = manifest.parse().context("packs.toml is not valid TOML")?;
    let mut roots: Vec<String> = document
        .get("packs")
        .and_then(toml::Value::as_table)
        .map(|packs| {
            packs
                .keys()
                .filter(|key| !key.contains('/'))
                .cloned()
                .collect()
        })
        .unwrap_or_default();
    if roots.is_empty() {
        bail!("packs.toml lists no packs; refusing to publish an empty archive");
    }
    roots.sort();
    Ok(roots)
}

/// Every tracked file that is game data, as repository-relative zip names.
///
/// Symlinks are dropped rather than followed: a client extracts this archive
/// into its own data directory, and a link is either a duplicate of a file that
/// is already in here or a path escape.
///
/// Every listed entry must contribute at least one file: a manifest entry with
/// nothing tracked under it is a typo or a deletion the manifest has not caught
/// up with, and either would otherwise publish an archive silently missing a
/// pack.
fn content_files(root: &Path, roots: &[String]) -> Result<Vec<String>> {
    let output = Command::new("git")
        .arg("-C")
        .arg(root)
        .args(["ls-files", "-z"])
        .output()
        .context("failed to list tracked files")?;
    if !output.status.success() {
        bail!(
            "`git ls-files` failed with status {:?}",
            output.status.code()
        );
    }

    let tracked = output
        .stdout
        .split(|byte| *byte == 0)
        .filter(|raw| !raw.is_empty())
        .map(|raw| {
            std::str::from_utf8(raw)
                .map(str::to_string)
                .context("a tracked path is not UTF-8")
        })
        .collect::<Result<Vec<_>>>()?;

    // Deliberately left in index order: `write_deterministic_zip` imposes the
    // archive's own, so there is exactly one place that decides it.
    let files: Vec<String> = tracked
        .into_iter()
        .filter(|path| is_content_path(path, roots))
        .filter(|path| {
            std::fs::symlink_metadata(root.join(path))
                .map(|metadata| metadata.is_file())
                .unwrap_or(false)
        })
        .collect();

    let empty: Vec<&String> = roots
        .iter()
        .filter(|entry| {
            !files
                .iter()
                .any(|path| root_entry_of(path) == entry.as_str())
        })
        .collect();
    if !empty.is_empty() {
        bail!(
            "packs.toml lists {} but no tracked file lies under {}",
            empty
                .iter()
                .map(|entry| format!("`{entry}`"))
                .collect::<Vec<_>>()
                .join(", "),
            if empty.len() == 1 { "it" } else { "them" }
        );
    }
    Ok(files)
}

/// Whether a repository-relative path is game data a client should receive:
/// whether its data-root entry is one the manifest lists.
fn is_content_path(path: &str, roots: &[String]) -> bool {
    roots
        .binary_search_by(|entry| entry.as_str().cmp(root_entry_of(path)))
        .is_ok()
}

fn root_entry_of(path: &str) -> &str {
    path.split('/').next().unwrap_or_default()
}

/// Writes a byte-reproducible zip of `files`, read from `root`.
///
/// The caller's order is not trusted: entry order is part of the bytes, so it
/// is decided here rather than wherever the file list happened to come from.
fn write_deterministic_zip(archive_path: &Path, root: &Path, files: &[String]) -> Result<()> {
    let mut files = files.to_vec();
    files.sort();

    let file = std::fs::File::create(archive_path)
        .with_context(|| format!("unable to create archive {}", archive_path.display()))?;
    let mut zip = ZipWriter::new(std::io::BufWriter::new(file));

    // `FileOptions::default()` reads the wall clock when `zip`'s `time` feature
    // is enabled, which any dependency could turn on through feature
    // unification.
    let options = FileOptions::default()
        .compression_method(CompressionMethod::Deflated)
        .last_modified_time(zip::DateTime::default())
        // Nothing in the content tree is executable, so this is a constant
        // rather than a lookup — and a constant cannot vary with a runner's
        // umask or with a filesystem that does not carry a mode at all.
        .unix_permissions(0o644);

    for name in &files {
        zip.start_file(name, options)
            .with_context(|| format!("failed to start zip entry {name}"))?;
        let mut source = std::fs::File::open(root.join(name))
            .with_context(|| format!("failed to open tracked file {name}"))?;
        std::io::copy(&mut source, &mut zip)
            .with_context(|| format!("failed to compress {name}"))?;
    }

    // No directory entries at all: they carry nothing a client needs and would
    // be one more thing to keep identical between runs.
    zip.finish().context("failed to finish the archive")?;
    Ok(())
}

fn sha256_file(path: &Path) -> Result<Vec<u8>> {
    use std::io::Read;

    let mut file = std::fs::File::open(path)
        .with_context(|| format!("failed to open {} for hashing", path.display()))?;
    let mut context = ring::digest::Context::new(&ring::digest::SHA256);
    let mut buffer = vec![0u8; 64 * 1024];
    loop {
        let read = file
            .read(&mut buffer)
            .with_context(|| format!("failed to read {}", path.display()))?;
        if read == 0 {
            break;
        }
        context.update(&buffer[..read]);
    }
    Ok(context.finish().as_ref().to_vec())
}

fn hex_digest(digest: &[u8]) -> String {
    digest.iter().map(|byte| format!("{byte:02x}")).collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::Read;

    const MANIFEST: &str = r#"
[origins.legacyclonk]
terms = "CC BY-NC"

[packs."Version.txt"]
origin = "clonk-rs"

[packs."Objects.c4d"]
origin = "legacyclonk"

[packs."Melees.c4f"]
origin = "legacyclonk"

[packs."Melees.c4f/Queron3.c4s"]
origin = "ccan"
bytes = "preserve"

[packs."Golems.c4d"]
origin = "ucc"
bytes = "preserve"
"#;

    /// The manifest's root entries are the whole answer to "what ships": an
    /// entry that reaches inside a pack describes policy, not membership.
    #[test]
    fn the_manifest_names_the_data_root_entries() {
        let roots = data_root_entries(MANIFEST).expect("parse manifest");
        assert_eq!(
            roots,
            ["Golems.c4d", "Melees.c4f", "Objects.c4d", "Version.txt"]
        );
    }

    #[test]
    fn a_manifest_without_packs_is_refused() {
        let error = data_root_entries("[origins.x]\nterms = \"t\"\n").unwrap_err();
        assert!(error.to_string().contains("no packs"), "{error}");
    }

    /// Only what the manifest lists ships, so infrastructure at the root needs
    /// no deny list — and cannot be forgotten on one.
    #[test]
    fn only_listed_root_entries_are_content() {
        let roots = data_root_entries(MANIFEST).expect("parse manifest");
        for path in [
            "Objects.c4d/Clonk.c4d/DefCore.txt",
            "Melees.c4f/Queron3.c4s/Scenario.txt",
            "Golems.c4d",
            "Version.txt",
        ] {
            assert!(
                is_content_path(path, &roots),
                "{path:?} must ship to clients"
            );
        }
        for path in [
            ".github/workflows/release.yml",
            "tools/pack-content/src/main.rs",
            ".gitignore",
            ".gitattributes",
            "Makefile",
            "README.md",
            "CONTRIBUTING.md",
            "packs.toml",
            "set_version.sh",
            "third_party/Hazard/readme.txt",
            "Stray.c4d/DefCore.txt",
            "Objects.c4d.bak",
            "",
        ] {
            assert!(
                !is_content_path(path, &roots),
                "{path:?} must not ship to clients"
            );
        }
    }

    /// Infrastructure names inside a pack are game data: the rule is about
    /// the root entry, never about a file's own name.
    #[test]
    fn infrastructure_names_inside_a_pack_are_still_content() {
        let roots = data_root_entries(MANIFEST).expect("parse manifest");
        for path in [
            "Objects.c4d/README.md",
            "Objects.c4d/.gitignore",
            "Melees.c4f/Gold.c4s/Makefile",
            "Objects.c4d/Crew.c4d/set_version.sh",
            "Melees.c4f/third_party/notes.txt",
        ] {
            assert!(is_content_path(path, &roots), "{path:?} is game data");
        }
    }

    /// Writes a small tree and returns its root.
    fn fixture() -> tempfile::TempDir {
        let temp = tempfile::TempDir::new().expect("temporary tree");
        for (path, body) in [
            ("Objects.c4d/DefCore.txt", "objects"),
            ("Music.c4g/Track.ogg", "music"),
            ("Version.txt", "version"),
        ] {
            let full = temp.path().join(path);
            std::fs::create_dir_all(full.parent().expect("parent")).expect("create");
            std::fs::write(full, body).expect("write");
        }
        temp
    }

    fn names() -> Vec<String> {
        vec![
            "Objects.c4d/DefCore.txt".to_string(),
            "Music.c4g/Track.ogg".to_string(),
            "Version.txt".to_string(),
        ]
    }

    fn pack(root: &Path, into: &Path, files: &[String]) {
        write_deterministic_zip(into, root, files).expect("write archive");
    }

    #[test]
    fn two_runs_of_the_same_tree_produce_identical_bytes() {
        // The property the whole component store rests on. Anything that leaked
        // the clock, the umask or directory iteration order in would fail here.
        let tree = fixture();
        let out = tempfile::TempDir::new().expect("output");
        let mut files = names();
        pack(tree.path(), &out.path().join("first.zip"), &files);
        // Deliberately offered in a different order: the archive sorts its own
        // entries rather than trusting the caller's.
        files.reverse();
        pack(tree.path(), &out.path().join("second.zip"), &files);

        assert_ne!(
            files,
            names(),
            "the second run must have been offered a different order"
        );
        assert_eq!(
            sha256_file(&out.path().join("first.zip")).expect("digest"),
            sha256_file(&out.path().join("second.zip")).expect("digest"),
            "the archive must not depend on the order files are offered in"
        );
    }

    #[test]
    fn entries_are_prefix_free_sorted_and_carry_no_directories() {
        let tree = fixture();
        let out = tempfile::TempDir::new().expect("output");
        let archive = out.path().join("content.zip");
        let mut files = names();
        files.sort();
        pack(tree.path(), &archive, &files);

        let mut zip = zip::ZipArchive::new(std::fs::File::open(&archive).expect("open"))
            .expect("read archive");
        let entries: Vec<String> = (0..zip.len())
            .map(|index| zip.by_index(index).expect("entry").name().to_string())
            .collect();
        assert_eq!(
            entries,
            [
                "Music.c4g/Track.ogg",
                "Objects.c4d/DefCore.txt",
                "Version.txt"
            ]
        );
        assert!(
            entries.iter().all(|name| !name.ends_with('/')),
            "directory entries would be one more thing to keep identical"
        );
    }

    #[test]
    fn every_entry_carries_the_zip_epoch_and_a_fixed_mode() {
        let tree = fixture();
        let out = tempfile::TempDir::new().expect("output");
        let archive = out.path().join("content.zip");
        pack(tree.path(), &archive, &names());

        let mut zip = zip::ZipArchive::new(std::fs::File::open(&archive).expect("open"))
            .expect("read archive");
        for index in 0..zip.len() {
            let entry = zip.by_index(index).expect("entry");
            assert_eq!(entry.last_modified().year(), 1980, "{}", entry.name());
            // The reader ORs the file-type bits back in, so only the
            // permission bits are ours to assert.
            assert_eq!(
                entry.unix_mode().map(|mode| mode & 0o7777),
                Some(0o644),
                "{}",
                entry.name()
            );
            assert_eq!(entry.compression(), CompressionMethod::Deflated);
        }
    }

    #[test]
    fn the_archive_round_trips_the_bytes_it_was_given() {
        let tree = fixture();
        let out = tempfile::TempDir::new().expect("output");
        let archive = out.path().join("content.zip");
        pack(tree.path(), &archive, &names());

        let mut zip = zip::ZipArchive::new(std::fs::File::open(&archive).expect("open"))
            .expect("read archive");
        let mut body = String::new();
        zip.by_name("Objects.c4d/DefCore.txt")
            .expect("entry")
            .read_to_string(&mut body)
            .expect("read entry");
        assert_eq!(body, "objects");
    }
}
