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
the Creative Commons Attribution-NonCommercial grant those packs are published
under, not by the MIT licence that covers the engine source.

`ClonkMars.*` and `EkeReloaded.*` are classic third-party packs included under
separate, explicitly granted permission, and `MetalMagic.c4d`,
`MetalMagicExtra.c4d` and `Melees.c4f/Queron3.c4s` come from CCAN. Their terms
are **not** the CC BY-NC grant above. Establish the position for yourself
before redistributing any of them.

`Collection.c4f` and the packs it depends on — `BlacksCMC-Waffenmod.c4d`,
`Clinfinity.c4d`, `ClonkPartyRes.c4d`, `ClonkPartyRes2.c4d`, `Clonkrix.c4d`,
`EkeBot.c4d`, `Explorer.c4d`, `GIDL_Race.c4d`, `Golems.*`,
`INEXSzenarioObjekte.c4d`, `InExantros.C4D`, `Jetbelt.c4d`, `KdD_Magie_Pack.c4d`,
`KnightMelees.c4d`, `KnightSkirmishs.c4d`, `ModernCombat.*`, `PhysGun.c4d`,
`Ropepack.c4d`, `RopepackRemake.c4d`, `RufDerWipfe.c4d`, `WesternBalancing.c4d`,
`WesternBugfixes.c4d`, `ZombieWars.c4d`, `Zombieland.c4d`, `Zombiesounds.c4d` —
come from Newton's **Ultimate Clonk Compilation v3.0** (CCAN entry 6383,
published 2025-03-14, SHA-256
`f6fce16b6bb4c9e150ba239876568a84187ab0c3e2ad45619d11fdb5e08214c3`). Its own
readme is explicit that the works remain the property of their individual
authors, so these terms are **not** the CC BY-NC grant above either. What was
taken and what was deliberately left behind is recorded in
`third_party/UltimateClonkCompilation/`.

The compilation's engine binaries, its bundled retail packs, and its Clonk 4 and
Clonk Planet trees are **not** here: this repository already carries the base
packs from `legacyclonk/content`, and replacing them would move every group
checksum a non-clonk-rs peer computes.

## Usage

The engine repository pins this repository as a submodule:

```sh
git clone --recurse-submodules https://github.com/clonk-org/clonk-rs.git
```

For an existing clone, `git submodule update --init --recursive`.
