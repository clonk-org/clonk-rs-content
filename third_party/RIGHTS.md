# Maintenance decisions and local rights inventory

This records the maintainer's September 9, 2026 decision for
clonk-org/clonk-rs-content#94. The inventory was prepared locally without
contacting rights holders or requesting additional permission. These changes
are submitted for review; existing release archives remain unchanged.

## What the statuses mean

`packs.toml` separates three distribution decisions:

- `licensed`: a recorded grant supports modification and redistribution under
  its terms. The `license` and `evidence` fields identify that grant.
- `assumed`: the maintainer has directed that the content remain in the local
  candidate and be available for maintenance despite incomplete or restrictive
  recorded terms. The `reason` and `evidence` fields identify this decision.
  This is not a verified permission, a licence, or a relicensing.
- `excluded`: the scope does not enter the candidate. A narrower scope overrides
  a broader scope. An exclusion inside a packed file must be resolved at the
  packing boundary; the tools refuse to ship that file unchanged.

The maintainer stated: "If current records don't establish permission to modify
them, we can assume we do have permission as these were all obtained from
ccan.de." The inventory records that assumption without treating a download
source as a copyright grant. The existing provenance descriptions remain the
source of truth about acquisition: in particular E.P.I.C. was supplied from a
maintainer archive, and UCC was imported as a compilation. Neither is relabeled
as a directly verified CCAN release.

Future changes within a recorded scope need ordinary code/content review and
updated inventory evidence, not a new author-contact step. Preserve original
credits, licence notices, encodings, packed boundaries, and version/change
records. New imports need an explicit scope decision before packaging.

## Recorded evidence

| Scope | Recorded terms | Local candidate decision |
|---|---|---|
| Official packs other than Hazard | Their `COPYING` files grant CC BY-NC 4.0; this covers scripting and media. Nested imports retain their separate decisions. | Licensed maintenance and noncommercial redistribution with the required notices. |
| `Hazard.c4d`, `Hazard.c4f` | Their `COPYING` files state CC BY-NC **3.0**, also recorded in `third_party/Hazard/readme.txt`. | Licensed maintenance under that version, not an invented 4.0 upgrade. |
| Clonk Party Remake localized scenario | `third_party/PROVENANCE.md` and the packed `ClonkRsLocalization.txt` record CCAN entry 6111's copy/modify grant and the requested filename and change history. | Licensed scenario; its external ClonkPartyRes dependencies remain separately assumed. |
| ClonkMars and Eke Reloaded | `THIRD_PARTY_GAME_CONTENT.md` at content commit `47496762` records project-specific redistribution of exact copies. The current working trees include later edits. ClonkMars' preserved `Copyright.txt` restricts reuse of complete scripts and media. | Assumed modification permission by the maintainer; the historical exact-copy grant is not represented as a derivative grant. |
| Queron 3 | The same historical document records a public reuse note requiring retention of the script's author/version line. The original note and its full scope have not been independently recovered in this local audit. | Assumed; MetalMagic and MetalMagicExtra have separate assumed decisions. |
| UCC, MetalMagic, E.P.I.C. and other imported dependencies | Authors, attribution, source references and some asset-specific notices are present. They do not establish a blanket modification grant in the available records. | Assumed, retaining all notices and the recorded acquisition sources. |

The UCC compiler's ownership statement and credits remain available under
`third_party/UltimateClonkCompilation/`. Embedded notices also identify
third-party music and sound authors. These assets are included in the scan;
the assumption does not establish rights held by those other contributors.

## Reproducible local inventory

The inventory starts at content commit
`9a01c8f55f0fbdccfa2dcf3a67e3cfcfcac7c009`. The reader uses clonk-rs's existing
`Group` implementation, including recursive packed-group access; it does not
unpack or rewrite game assets. Its starting engine commit is
`e3d35afcff3411d9c7d8152686ebec87faafd9c7`.

`rights-inventory.json` records every group and scenario, notice locations and
hashes, scope decisions, and scenario definition references. Scope digests bind
the report to the actual game-resource bytes. These digests describe the local
checkout, including its line endings; they are not hashes of original external
archives. External artifact hashes remain in the existing provenance ledger.

From the engine worktree, build the read-only scanner:

```sh
cargo build -p clonk-resources --example rights_inventory
```

From the content worktree, run a fresh scan and record it, or check the existing inventory:

```sh
uv run --python 3.13 python tools/rights_inventory.py record --scanner /path/to/engine/target/debug/examples/rights_inventory
uv run --python 3.13 python tools/rights_inventory.py check
```

Definition resolution in the inventory is a static, case-insensitive search
through the scenario's ancestors and data root. The packaging dependency map
protects the physical files containing resolved definitions, including packed
containers. It is not proof of runtime script dependency discovery or gameplay
compatibility. Engine parity and scenario checks remain separate.

The baseline has 13 unresolved explicit references to
`MetalMagic.c4f/Misc.c4d`: `Adventure.c4s` and twelve MissionsHarkon scenarios.
They are the existing defect tracked by clonk-org/clonk-rs-content#80. The
inventory records each reference as unresolved; it neither invents a replacement
nor marks the scenarios playable. New unresolved references must be reported
explicitly in `known-missing-definitions.json` when refreshing the inventory.
The recorder rejects new gaps without that record and refuses a scan if
resource bytes change while it is running. CI checks the snapshot before
packaging. Notice hashes cover the decoded Windows-1252 text encoded as UTF-8;
scope hashes cover the physical file bytes, including packed archives.

## Publication boundary

The local candidate includes assumed scopes at the maintainer's direction.
Successful packaging validates that recorded decision, dependency membership,
and required notices; it does not certify that all third-party permissions have
been established. No historical artifact was removed or rewritten by this work.
Decisions about future publication and historical copies remain separate from
this local implementation.
