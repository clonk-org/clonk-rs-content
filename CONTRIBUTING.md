# Contributing

This repository is the game data for [Clonk Rust](https://github.com/clonk-org/clonk-rs),
consumed as its `content/` submodule. Most of it is twenty-year-old Clonk
content, and several of the rules below exist because breaking them is silent —
every check stays green and the damage shows up in someone's install.

The packaging tool ships only the entries `packs.toml` lists, so the tooling
under `tools/`, the checks under `tests/` and this file live at the root like
any repository's, beside the packs.

## Every change costs a download

`content.zip` is content-addressed. Clonk Rust records its SHA-256 in the update
manifest and a client re-downloads only when the digest changes. Any change to a
file under a listed pack — or to `packs.toml`, `.gitattributes` or the packer —
moves that digest and makes every client fetch the entire archive again.

So batch content changes rather than landing five one-line fixes separately, and
expect the `Validate` job to tell you which files it saw as client-facing.

## Every pack is described once

`packs.toml` lists every entry of the data root with its origin, its
byte policy, its localization status and the private definitions nested inside
it. `set_version.sh`, the localization audit and the layout test read their
lists from it, and `python3 tools/packs.py check` fails when it disagrees
with the tracked tree or with `.gitattributes`.

So adding a pack means adding an entry there, naming an `[origins.<name>]`
table that records where it came from and on what terms. An import whose bytes
must not move gets `bytes = "preserve"`, a matching `binary` rule in
`.gitattributes`, its `upstream_version`, and a ledger under `third_party/`.

## Third-party working trees are maintained derivatives

`.gitattributes` marks every import `packs.toml` preserves `binary`: eleven
directory trees, from `ClonkMars.c4d` to `ModernCombat.c4f`, and the five
packed dependency files at the data root.

The attribute controls Git's checkout and diff handling. It stops automatic eol
normalisation from rewriting resource bytes and moving the group checksums a
non-clonk-rs peer computes. Normalising Metal & Magic rewrote roughly half its
bytes on the first attempt, which is why the rule is there. It does **not** mean
the current working trees are unmodified upstream archives: project history
already contains localization, script, asset, layout, and curation changes.

Treat them as maintained derivatives with byte-safe checkout. Their source
snapshots and project changes are recorded in
[`third_party/PROVENANCE.md`](../third_party/PROVENANCE.md). An intentional
change must:

- preserve the upstream author, credit, copyright, and licence notices;
- identify the source archive and hash, or say explicitly when only an imported
  Git tree is available;
- add a ledger entry naming the changed paths, purpose, pull request, and source
  baseline; and
- preserve the pack's encoding and script substitutions, with focused tests for
  the behavior being changed.

This repository policy does not create or expand permission to modify or
redistribute a work. Public availability, prior redistribution, and an earlier
project patch are not substitutes for licence terms or author permission. If a
change's basis is not recorded, obtain an upstream replacement or leave the
import unchanged and report the blocker.

Many entries inside these imports are *packed* C4Groups rather than
directories. **Do not unpack them for consistency with the rest of the tree** —
and note `grep` cannot see inside them, so a search will under-report call sites
and translations.

## Line endings and encoding

- Content is checked out CRLF (`* text=auto eol=crlf`). Everything under
  `.github/` is LF.
- Text assets are US-ASCII and ISO-8859-1. **Not UTF-8.** Editors re-encode
  Latin-1 on save without asking; the file still parses and the game still runs,
  and the only symptom is an umlaut rendering as two characters. CI rejects a
  changed asset that arrives as UTF-8.
- No new NUL bytes. Fifteen assets carry a stray trailing one already; that is a
  known backlog, not a licence to add more.

## Localization

Classic Clonk names English resources `US`. Maintained content follows these
rules:

- every nonempty `DescDE.txt`/`.rtf`, `RankDE.txt`, and `StringTblDE.txt` has a
  nonempty English counterpart;
- every nonempty German string-table key has an English value with the same
  ordered `%` format directives and the same `{{ICON}}` tokens;
- `Names.txt` and `Title.txt` use explicit `DE:`/`US:` lines, without duplicate
  locale tags;
- player-visible script text belongs in paired string tables, not as a
  German-only literal; and
- localization filenames use their canonical casing (`StringTbl`, `Desc`,
  `Rank`, `Names`, and `Title`).

`check_localizations.py` enforces the structural rules and catches a curated set
of high-confidence German leftovers. It is not a substitute for reading new
English prose for meaning and fluency. Imported working trees with known
backlogs carry a `localization` list of qualified issue references in
`packs.toml`. Remove each reference when its linked work is complete;
an entry with none left is audited. The `binary` attribute alone never excludes
a path from the audit.

## Extension casing is insignificant — leave it alone

130 paths use an uppercase extension: 102 `.WAV`, 14 `.C4D`, 13 `.TXT`, one `.C`.
This looks like something to tidy. It is not.

Both engines match these case-insensitively, deliberately, because the classic
one did:

- definitions — `is_definition_file` in `clonk-engine` uses
  `eq_ignore_ascii_case`, mirroring C++ `SEqualNoCase(GetExtension(szFilename), "c4d")`;
- sounds — `sound_sample_available` lowercases both the request and the sample
  name, mirroring `C4SoundSystem::PrepareFilename` and StdFile's
  ASCII-insensitive `WildcardMatch`.

So `LightningShot.C4D` loads exactly like `LightningShot.c4d`, on Linux as much
as anywhere else. Renaming all 130 would move the archive digest — a re-download
for every install — churn imported paths for no behavior change, and buy
nothing. Case-only renames also need a two-step dance to register on macOS and
Windows.

## Versions

Run `./set_version.sh <VERSION>` rather than editing `Version.txt` by hand. It
skips the third-party packs, which carry their own upstream versions
(`1.5 [Spirit]`, `1.7`, `3.1b`) and must keep them: every entry `packs.toml`
marks `bytes = "preserve"`.

## What ships is what the manifest lists

`tools/pack-content` builds `content.zip` from the data-root entries in
`packs.toml` and nothing else. There is no deny list: a file at the
root that is not a listed pack does not ship, whatever it is called, and a pack
that is not listed fails `packs.py check` before it can be forgotten.

The engine repository's installer (`xtask package` in `clonk-org/clonk-rs`)
copies the same entries from the same manifest, so the two cannot disagree
about which files exist. Until that change lands there, its copy still uses
the old deny list, which produces the same set for the current tree — so do
not add a root entry that is neither a pack nor on that list before it does.

## Running the checks

The `Validate` job runs on every pull request and is required to merge. Locally:

```sh
cd tools/pack-content && cargo test --locked && cargo fmt --check && cargo clippy --locked --all-targets -- -D warnings
cd ../.. && tests/test_set_version.sh && tests/test_shared_bases_scenarios.sh && tests/test_content_layout.sh
python3 tools/packs.py check
python3 -m unittest discover -v -s tests -p 'test_check_*.py'
python3 tests/check_localizations.py
python3 tests/check_text_assets.py origin/main
```

To build the archive the way CI does — note it refuses a dirty worktree:

```sh
cargo build --release --locked --manifest-path tools/pack-content/Cargo.toml
./tools/pack-content/target/release/pack-content content.zip
```

## Commits and pull requests

- Conventional Commits, no scope, subject line only:
  `fix: strip the stray NUL byte from FoggyCliffs Scenario.txt`.
- `main` merges by **squash**, so the *pull request title* becomes the commit
  subject. Retitle it if `--fill` took the branch slug.
- Put `Fixes #<n>` in the pull request body, not in a commit — the subject-only
  rule leaves no room for a footer.
- Keep structural changes (renames, moves, reformatting) in separate commits
  from behavioural ones, and land structural first.
