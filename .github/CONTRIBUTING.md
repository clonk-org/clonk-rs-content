# Contributing

This repository is the game data for [Clonk Rust](https://github.com/clonk-org/clonk-rs),
consumed as its `content/` submodule. Most of it is twenty-year-old Clonk
content, and several of the rules below exist because breaking them is silent —
every check stays green and the damage shows up in someone's install.

Kept in `.github/` deliberately: the repository root is game data as far as the
packaging tool is concerned, so a file there ships to every player inside
`content.zip`.

## Every change costs a download

`content.zip` is content-addressed. Clonk Rust records its SHA-256 in the update
manifest and a client re-downloads only when the digest changes — the archive is
around 253 MB. Any change to a tracked file that is not under `.github/` moves
that digest.

So batch content changes rather than landing five one-line fixes separately, and
expect the `Validate` job to tell you which files it saw as client-facing.

## Packs you must not touch casually

`.gitattributes` marks these `binary`, and
[`THIRD_PARTY_GAME_CONTENT.md`](../THIRD_PARTY_GAME_CONTENT.md) records why:

- `ClonkMars.c4d`, `ClonkMars.c4f`
- `EkeReloaded.c4d`, `EkeReloaded.c4f`
- `MetalMagic.c4d`, `MetalMagicExtra.c4d`
- `Melees.c4f/Queron3.c4s`

They are redistributed as exact copies. Any eol normalisation or reformatting
changes their resource bytes and the group checksums a non-clonk-rs peer
computes. Normalising Metal & Magic rewrote roughly half its bytes on the first
attempt, which is why the `binary` rule is there.

103 files inside the ClonkMars and Eke Reloaded packs are *packed* C4Groups
rather than directories. **Do not unpack them for consistency with the rest of
the tree** — and note `grep` cannot see inside them, so a search will
under-report call sites.

## Line endings and encoding

- Content is checked out CRLF (`* text=auto eol=crlf`). Everything under
  `.github/` is LF.
- Text assets are US-ASCII and ISO-8859-1. **Not UTF-8.** Editors re-encode
  Latin-1 on save without asking; the file still parses and the game still runs,
  and the only symptom is an umlaut rendering as two characters. CI rejects a
  changed asset that arrives as UTF-8.
- No new NUL bytes. Fifteen assets carry a stray trailing one already; that is a
  known backlog, not a licence to add more.

## Versions

Run `./set_version.sh <VERSION>` rather than editing `Version.txt` by hand. It
skips the third-party packs, which carry their own upstream versions
(`1.5 [Spirit]`, `1.7`, `3.1b`) and must keep them.

## The exclusion list is mirrored

`NON_CONTENT_ENTRIES` in `.github/pack-content/src/main.rs` decides what stays
out of `content.zip`. The same list exists as `is_runtime_package_path` in
`clonk-org/clonk-rs` (`xtask/src/main.rs`), which applies it when building
installers. **Change both or neither** — otherwise an install and an in-place
update disagree about which files exist. A test fails if you change one.

## Running the checks

The `Validate` job runs on every pull request and is required to merge. Locally:

```sh
cd .github/pack-content && cargo test --locked && cargo clippy --locked --all-targets -- -D warnings
cd ../.. && .github/tests/test_set_version.sh
python3 .github/tests/check_text_assets.py origin/main
```

To build the archive the way CI does — note it refuses a dirty worktree:

```sh
cargo build --release --locked --manifest-path .github/pack-content/Cargo.toml
./.github/pack-content/target/release/pack-content content.zip
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
