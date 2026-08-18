# Clonk Rust game content

Game data for [Clonk Rust](https://github.com/clonk-org/clonk-rs), consumed as
the `content/` submodule of the engine repository. This directory is the
engine's data root (`General.ExePath` in classic terms), so every definition and
scenario pack the game should offer must live here.

## Origin

The base packs — `Objects.c4d`, `Worlds.c4f`, `Fantasy.*`, `FarWorlds.*`,
`Hazard.*`, `Knights.*`, `Western.*`, `Material.c4g`, `Music.c4g`, `Sound.c4g`,
`Melees.c4f`, `Missions.c4f`, `Races.c4f`, `Tutorial.c4f` — originate from
[legacyclonk/content](https://github.com/legacyclonk/content) and are covered by
the CC BY-NC grant in [`clonk_content_license.txt`](clonk_content_license.txt).

`ClonkMars.*` and `EkeReloaded.*` are classic third-party packs included under
separate, explicitly granted permission, and `MetalMagic.c4d`,
`MetalMagicExtra.c4d` and `Melees.c4f/Queron3.c4s` come from CCAN. Their terms
are **not** the CC BY-NC grant above. Establish the position for yourself
before redistributing any of them.

## Usage

The engine repository pins this repository as a submodule:

```sh
git clone --recurse-submodules https://github.com/clonk-org/clonk-rs.git
```

For an existing clone, `git submodule update --init --recursive`.
