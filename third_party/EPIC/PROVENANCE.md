# E.P.I.C.

Source record for `E.P.I.C.c4f` and its nested `E.P.I.C.c4d` definition pack.
This directory is excluded from `content.zip`; it documents the locally
archived inputs without changing their payload bytes. The repository-wide
derivative policy and patch ledger are in [`../PROVENANCE.md`](../PROVENANCE.md).

| Input | SHA-256 | Hash scope |
|---|---|---|
| `E.P.I.C.c4f` | `432165d0b7df4191ac2d21bf21d5a5c1bb9e82087288e6e08e2376980869b7fe` | Recorded for the maintainer-supplied directory snapshot; the original container artifact is unavailable, so this is not independently reproducible from an archive. |
| `E.P.I.C.c4d` | `3ed243a408bd204b94b7a96a0f556ddc5c595e874d3236dc42fb33013055a488` | Packed source file; matches `E.P.I.C.c4f/E.P.I.C.c4d`. |

The packs were supplied from the maintainers' local archive. Their embedded
description identifies the project as **Extreme Pack Increasing Complexity**
and points to ClonkForge project 1138.

Credits carried by the definition pack:

- Designers: Zagabar and Wertilq
- Main scripter: Zagabar; scripting assistance: Wertilq
- Graphics: Elwebbaro, with Wertilq, Zagabar and Vaosk
- Beta testing: Fred, Vaosk, Elwebbaro, Enkay, Dominator, Firecrow, Cvan,
  Jafem, RG's, Wertilq and Zagabar
- Additional credit: the authors of Druckwellenkampf's item-holding script and
  Seapack's telescope

## Shape on disk

`E.P.I.C.c4f` is exploded into a directory so `cargo xtask scenario-sweep` can
discover its five fully exploded `.c4s` children. `E.P.I.C.c4d` remains
byte-identical and packed inside that folder. Keeping the definition beside the
only scenario pack that names it makes the mod self-contained and avoids
another unrelated definition in the data root.
