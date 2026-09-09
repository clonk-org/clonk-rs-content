# Clonk Rust game content

Game data for [Clonk Rust](https://github.com/clonk-org/clonk-rs), consumed as
the `content/` submodule of the engine repository. This directory is the
engine's data root (`General.ExePath` in classic terms), so every definition and
scenario pack the game should offer must live here.

## Origin

Each entry of this tree is described once in `packs.toml`: where it
came from, its rights decision, whether its bytes may be normalised, and which
localization work is still open against it. The tooling reads that file; this section is the prose.

The base packs — `Objects.c4d`, `Worlds.c4f`, `Fantasy.*`, `FarWorlds.*`,
`Hazard.*`, `Knights.*`, `Western.*`, `Material.c4g`, `Music.c4g`, `Sound.c4g`,
`Melees.c4f`, `Missions.c4f`, `Races.c4f`, `Tutorial.c4f` — originate from
[legacyclonk/content](https://github.com/legacyclonk/content) and are covered by
the Creative Commons Attribution-NonCommercial grant those packs are published
under: CC BY-NC 4.0 except Hazard, which uses 3.0. The engine has its own MIT
licence.

`ClonkMars.*` and `EkeReloaded.*` have historical exact-copy redistribution
records. `MetalMagic.c4d`, `MetalMagicExtra.c4d` and
`Melees.c4f/Queron3.c4s` come from CCAN. Modification of these current trees is
included under the maintainer assumption in [the rights inventory](third_party/RIGHTS.md).
That decision preserves their recorded terms and does not turn archive
availability into a verified modification grant.

`Collection.c4f`, the private definition packs nested beside its scenarios,
and the dependencies shared across scenario folders — `Golems.*`,
`ModernCombat.*`, `RopepackRemake.c4d`, `WesternBalancing.c4d` and
`WesternBugfixes.c4d` — come from Newton's **Ultimate Clonk Compilation v3.0**
(CCAN entry 6383, published 2025-03-14, SHA-256
`f6fce16b6bb4c9e150ba239876568a84187ab0c3e2ad45619d11fdb5e08214c3`). Its own
readme is explicit that the works remain the property of their individual
authors, so these terms are **not** the CC BY-NC grant above either. What was
taken and what was deliberately left behind is recorded in
`third_party/UltimateClonkCompilation/`.

`E.P.I.C.c4f` and its nested `E.P.I.C.c4d` definition pack were supplied from
the maintainers' archived copy. The payload credits Zagabar, Wertilq,
Elwebbaro and its other contributors; its source hashes and complete credits are
recorded in `third_party/EPIC/`.

The imported working-tree classifications, known rights evidence, and project
patch history are recorded in `third_party/PROVENANCE.md`. That record does not
create or expand permission to redistribute or modify a work.

The compilation's engine binaries, its bundled retail packs, and its Clonk 4 and
Clonk Planet trees are **not** here: this repository already carries the base
packs from `legacyclonk/content`, and replacing them would move every group
checksum a non-clonk-rs peer computes.

## Local maintenance and packaging

[The rights inventory](third_party/RIGHTS.md) records the one-time review and
how to refresh it after content changes. All current imports remain selected
for local maintenance. `licensed` identifies a recorded grant; `assumed`
identifies the maintainer's decision where the available record is incomplete.
No per-change author-contact step is required by this local policy.

The package tool reads these decisions from `packs.toml`, applies narrower
scopes first, and includes [CONTENT-NOTICES.md](CONTENT-NOTICES.md) and each
selected scope's evidence. It refuses missing dependencies and a nested
exclusion that would otherwise remain inside a shipped packed archive.
`excluded` is available for future scope decisions; no current pack is removed.

## Usage

The engine repository pins this repository as a submodule:

```sh
git clone --recurse-submodules https://github.com/clonk-org/clonk-rs.git
```

For an existing clone, `git submodule update --init --recursive`.
