# Ultimate Clonk Compilation v3.0

Source of `Collection.c4f` and the dependency packs listed in the repository
README. Kept under `third_party/`, which `NON_CONTENT_ROOT_ENTRIES` in
`.github/pack-content/src/main.rs` excludes, so this record is auditable in the
repository without shipping inside `content.zip`.

| | |
|---|---|
| Author / compiler | Newton (`newton@westnordost.de`) |
| CCAN entry | <https://ccan.de/cgi-bin/ccan/ccan-view.pl?a=view&i=6383> |
| Archive | `https://www.westnordost.de/clonk/UltimateClonkCompilation-v3.0.zip` |
| Published | 2025-03-14 |
| Size | 1,263,708,249 bytes |
| SHA-256 | `f6fce16b6bb4c9e150ba239876568a84187ab0c3e2ad45619d11fdb5e08214c3` |
| Discussion | <https://forum.clonkspot.org/t/ultimate-clonk-collection/1514> |

`Readme.txt` and `Authors.txt` beside this file are the compilation's own. Their
text is unedited, but line endings follow this repository's `eol=crlf` default
like the rest of `third_party/`: they are documentation, not group payload, so
nothing checksums their bytes. The readme states that the works remain the
property of their respective authors and that the retail Clonk content is
© RedWolf Design.

## How the collection was selected

Newton downloaded everything released across CCAN (~2000), Clonk Center (~140),
Treffpunkt Clonk (~200), the Clonk History Project (~150), some league scenarios
(~50) and the Clonk 4 Complete Collection (~150); pre-sorted to roughly the top
30%; then played every remaining scenario and judged it against written
criteria — a defined ending and playable to it (no sandboxes), fun, still looks
good without nostalgia, best of its genre only, and no grinding. Each folder
keeps a `Comments.txt` recording why entries were included or cut; those files
ship inside `Collection.c4f`.

## What was taken

- `Clonk Rage/Collection/` — 249 files, the curated ~10%, as
  `Collection.c4f/` with its eleven genre folders given the `.c4f` extension the
  engine classifies scenario folders by.
- 28 dependency packs from `Clonk Rage/` that this repository did not already
  carry.

## What was deliberately left behind

- The `Clonk 4/` and `Clonk Planet/` trees — a different engine generation.
- Every executable and library: `Clonk.exe`, `clonk`, `clonk64`, `c4group*`,
  `Editor.exe`, `c4author.exe`, `uninst.exe`, `Planet.exe`, `*.pdb`,
  `fmod.dll` (proprietary FMOD) and the dgVoodoo `D3D8/D3D9/D3DImm/DDraw.dll`
  wrappers.
- The bundled retail packs — `Objects.c4d`, `Material.c4g`, `Music.c4g`,
  `Sound.c4g`, `Knights.*`, `Western.*`, `Fantasy.*`, `FarWorlds.*`, `Hazard.*`,
  `Melees.c4f`, `Races.c4f`, `Tutorial.c4f`, `Worlds.c4f`, `Missions.c4f`. This
  repository already carries these from `legacyclonk/content`; replacing them
  with the retail variants would move every group checksum a non-clonk-rs peer
  computes, and a different `Material.c4g` would reorder material indices, which
  is determinism-critical.
- `System.c4g` and `Graphics.c4g` — the port authors its own under `planet/`,
  and `planet/System.c4g` is the marker the engine locates its install root by.
- `Newton.c4p`, a player file.
- `Clonk.log`, `LGPL.txt`, `OpenSSL.txt` and the two license texts this
  repository already removed deliberately.
- `Extra.c4g`. `Extra.c4g` is the *classic global override group*, and the
  engine requires it to resolve inside `planet/`:
  `mapped_classic_extra_group_path` (`clonk-app/src/main_parts/assets.rs:1883-1902`)
  hard-errors on a copy found anywhere else, which took out GUI bootstrap —
  every font and GUI surface — across 389 tests. The name is reserved, so a
  content-root copy is not an option, and renaming it would be worse: the
  material-overload lookup keys on that exact name
  (`main_parts/resources.rs:800-813`), so a renamed group would be 104 MB the
  engine never reads. Its contents were additive presentation only — music and
  loader art overlaid onto ClonkMars, Fantasy, GIDL_Race, Hazard, Knights,
  Metal & Magic and Western, all of which keep the music and loaders they
  already ship. No definition or scenario depended on it.

## Shape on disk

Packed `.c4d` packs are kept packed: a packed group returns its stored header
order, which is host-stable and is what C++ `C4MaterialMap::Load` consumes,
where an unpacked directory returns unsorted `readdir` order.

`.c4f` packs are exploded into directories instead. `cargo xtask scenario-sweep`
walks the filesystem, so a packed `.c4f` would hide every scenario inside it
from the sweep the compatibility profile's evidence rests on.
`Collection.c4f/Adventures.c4f/MissionsHarkon.c4f` had a second reason: at
105 MB packed it exceeds GitHub's per-file push limit.

Everything added is marked `binary` in `.gitattributes` so that no end-of-line
normalisation rewrites bytes this project does not own.

## Known content defects

`Collection.c4f/Settling.c4f/RufDerWipfeRE.c4f/Others.c4f/ModernBattle.c4s/Teams.txt`
carries two team icons spelled `Portrait:CLN2:Soldier1` and
`Portrait:CLN2:Soldier2` — a single colon where the portrait grammar wants two.
Classic Clonk rejects them as well: `C4Portrait::EvaluatePortraitString`
(`C4DefGraphics.cpp:578-603`) falls into its bare-name branch and returns the
default ID, and `C4Game::DrawTextSpecImage` passed `C4ID_None` as that default,
so it bails at `if (idPortrait == C4ID_None) return false`
(`C4Game.cpp:4314-4315`). The icons draw in neither engine. Left as received.
