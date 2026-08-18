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
//! moving and every install would re-fetch 225 MB. The engine repository
//! therefore stopped building this archive and now references the one published
//! here, beside the files it is made of.
//!
//! # What determinism costs
//!
//! Every axis that could vary between runs or runners is pinned rather than
//! inherited from the environment:
//!
//! * **Membership** — the Git index, not the working tree, so an untracked
//!   scratch file cannot enter the archive.
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

/// Repository entries that are infrastructure rather than game data.
///
/// This list is mirrored by `is_runtime_package_path` in the engine
/// repository's packaging tool, which applies it when it copies this submodule
/// into an installer. The two must agree, or a file would ship in the installer
/// and never reach a client that updates in place — or the reverse.
///
/// `the_exclusion_list_matches_the_engine_repositorys_copy` pins these entries
/// so that changing them here fails rather than diverging silently.
const NON_CONTENT_ENTRIES: [&str; 6] = [
    ".git",
    ".github",
    ".gitignore",
    ".gitattributes",
    ".editorconfig",
    ".DS_Store",
];

/// Repository files that are infrastructure, matched **only at the root**.
///
/// These are ordinary names a game pack could legitimately contain, so they
/// cannot join `NON_CONTENT_ENTRIES` — that list matches every path segment, and
/// a definition shipping its own `README.md` would silently vanish from the
/// archive and from installers. Anchoring them here keeps the deny rule to the
/// one directory where these names mean packaging infrastructure.
///
/// `Version.txt` is deliberately absent: the engine reads the root one, and 28
/// more live inside packs.
const NON_CONTENT_ROOT_ENTRIES: [&str; 4] =
    ["Makefile", "README.md", "set_version.sh", "third_party"];

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
    let files = content_files(&root)?;
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

/// Every tracked file that is game data, as repository-relative zip names.
///
/// Symlinks are dropped rather than followed: a client extracts this archive
/// into its own data directory, and a link is either a duplicate of a file that
/// is already in here or a path escape.
fn content_files(root: &Path) -> Result<Vec<String>> {
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
    Ok(tracked
        .into_iter()
        .filter(|path| is_content_path(path))
        .filter(|path| {
            std::fs::symlink_metadata(root.join(path))
                .map(|metadata| metadata.is_file())
                .unwrap_or(false)
        })
        .collect())
}

/// Whether a repository-relative path is game data a client should receive.
fn is_content_path(path: &str) -> bool {
    if path.is_empty() {
        return false;
    }
    let mut segments = path.split('/');
    let Some(root) = segments.next() else {
        return false;
    };
    if NON_CONTENT_ROOT_ENTRIES.contains(&root) {
        return false;
    }
    std::iter::once(root)
        .chain(segments)
        .all(|segment| !segment.is_empty() && !NON_CONTENT_ENTRIES.contains(&segment))
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

    /// Pins the exclusion list itself, not just its effect.
    ///
    /// `NON_CONTENT_ENTRIES` is mirrored by `is_runtime_package_path` in the
    /// engine repository (`xtask/src/main.rs`), which applies it when copying
    /// this submodule into an installer. Nothing can check the other repository
    /// from here, so this fails on any edit instead: the two must be changed
    /// together, and a diff that touches only one side is the bug this catches.
    ///
    /// If this test fails because the list genuinely changed, update
    /// `is_runtime_package_path` in `clonk-org/clonk-rs` in the same breath.
    /// A file excluded on one side and not the other ships in the installer and
    /// never reaches a client that updates in place, or the reverse.
    #[test]
    fn the_exclusion_list_matches_the_engine_repositorys_copy() {
        assert_eq!(
            NON_CONTENT_ENTRIES,
            [
                ".git",
                ".github",
                ".gitignore",
                ".gitattributes",
                ".editorconfig",
                ".DS_Store",
            ],
            "NON_CONTENT_ENTRIES changed; update is_runtime_package_path in \
             clonk-org/clonk-rs (xtask/src/main.rs) to match"
        );
        assert_eq!(
            NON_CONTENT_ROOT_ENTRIES,
            ["Makefile", "README.md", "set_version.sh", "third_party"],
            "NON_CONTENT_ROOT_ENTRIES changed; update is_runtime_package_path in \
             clonk-org/clonk-rs (xtask/src/main.rs) to match"
        );
    }

    /// The root-anchored entries must not reach into packs.
    ///
    /// This is the whole reason they are a separate list: `NON_CONTENT_ENTRIES`
    /// matches every segment, so putting `README.md` there would delete a
    /// definition's own readme from the archive without a word.
    #[test]
    fn root_infrastructure_names_are_still_content_inside_a_pack() {
        for path in [
            "Objects.c4d/README.md",
            "Worlds.c4f/Gold.c4s/Makefile",
            "Knights.c4d/Crew.c4d/set_version.sh",
            "Hazard.c4f/third_party/notes.txt",
        ] {
            assert!(
                is_content_path(path),
                "{path:?} is game data; only the repository root carries infrastructure"
            );
        }
    }

    #[test]
    fn repository_infrastructure_is_not_content() {
        for path in [
            ".github/workflows/release.yml",
            ".github/pack-content/src/main.rs",
            ".gitignore",
            ".gitattributes",
            "Objects.c4d/.DS_Store",
            "Makefile",
            "README.md",
            "set_version.sh",
            "third_party/Hazard/readme.txt",
            "",
        ] {
            assert!(!is_content_path(path), "{path:?} must not ship to clients");
        }
    }

    #[test]
    fn game_data_is_content() {
        for path in [
            "Objects.c4d/Clonk.c4d/DefCore.txt",
            "Worlds.c4f/Gold.c4s/Scenario.txt",
            "Version.txt",
        ] {
            assert!(is_content_path(path), "{path:?} must ship to clients");
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
