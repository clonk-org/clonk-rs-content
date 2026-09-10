# Third-party derivative provenance

This is the policy and patch ledger for imported working trees. `binary` in
`.gitattributes` controls Git checkout and line-ending behavior. It is not a
licence, ownership assertion, provenance record, exact-copy claim, or
localization-audit exemption.

A scope may be called an exact copy only when a named source artifact, its
source hash, the exact path and packing boundary, and the redistribution basis
are recorded. Exploding, selecting, deduplicating, nesting, renaming,
translating, fixing, or repacking makes the result a maintained derivative or
curated import. Do not label a parent tree exact when it contains changed
children.

Public availability, an author field, a source URL, a checksum, a historical
commit, or prior project redistribution does not by itself grant permission to
redistribute or modify a work. Unknown terms remain unknown. This ledger
records evidence; it does not create or expand rights.

The local maintenance decisions are recorded separately in
[RIGHTS.md](RIGHTS.md) and `packs.toml`. Assumed permission does not replace
the evidence classifications below.

## Source and rights ledger

`unknown` means the repository does not retain enough evidence to make the
claim. Git tree IDs identify imported project bytes, not external source
archives.

| Scope | Classification | Source artifact and source hash | Authors and notices | Recorded basis and rights status | Imported project baseline | Localization |
|---|---|---|---|---|---|---|
| `ClonkMars.c4d`, `ClonkMars.c4f` | maintained derivative | external archive and SHA-256: `unknown` | embedded `Author.txt`, `Credits.txt`, and `Copyright.txt` | repository history records project-specific exact-copy redistribution permission; the record was deleted by [#41](https://github.com/clonk-org/clonk-rs-content/pull/41); derivative permission: `unknown` | commit [`c90bafba`](https://github.com/clonk-org/clonk-rs-content/commit/c90bafbaeafa22ed0fb2269613f29de6432b5786); trees `ef9a9256f7155c21a1c712aa6d1b5b78080385c1`, `851de1ffa5e7d708dd7521bc7752a39a75e82338` | [#78](https://github.com/clonk-org/clonk-rs-content/issues/78) |
| `EkeReloaded.c4d`, `EkeReloaded.c4f` | maintained derivative | external archive and SHA-256: `unknown` | embedded `Author.txt` and pack credits | repository history records project-specific exact-copy redistribution permission; the record was deleted by [#41](https://github.com/clonk-org/clonk-rs-content/pull/41); derivative permission: `unknown` | commit [`c90bafba`](https://github.com/clonk-org/clonk-rs-content/commit/c90bafbaeafa22ed0fb2269613f29de6432b5786); trees `228dba3135e5d715a7a7e4f5668b29f85179de16`, `efdf7a71cc7a9eac7cb9ddd9ba419c8eb3dce953` | [#74](https://github.com/clonk-org/clonk-rs-content/issues/74) |
| `Melees.c4f/Queron3.c4s` | maintained derivative | CCAN entry 5737; artifact name and SHA-256: `unknown` | `Author.txt` and the script's author/version line | historical source record describes a public reuse note conditioned on preserving authorship; exact permission scope: `unknown` | commit [`d34d3855`](https://github.com/clonk-org/clonk-rs-content/commit/d34d385591134ce6c262b8c9ed53faaa6229cc6b); tree `b6c1018e143005c50a6456be45c35354f7fbd233`; baseline already localized | parent [#68](https://github.com/clonk-org/clonk-rs-content/issues/68), child [#72](https://github.com/clonk-org/clonk-rs-content/issues/72) |
| `MetalMagic.c4d`, `MetalMagicExtra.c4d` | maintained derivative | CCAN entry 4068, `MM3_1c.zip`; SHA-256: `unknown` | embedded author and description files | the CCAN entry recorded no redistribution terms; [#5](https://github.com/clonk-org/clonk-rs-content/issues/5) closed not planned when the written requirement was deleted, not when rights were resolved | commit [`d34d3855`](https://github.com/clonk-org/clonk-rs-content/commit/d34d385591134ce6c262b8c9ed53faaa6229cc6b); trees `7420fa6f84109863781943f2cd1355952f026a55`, `756a3d628ec3fe948347d1da65d423b4a83f2016`; baseline already localized | [#72](https://github.com/clonk-org/clonk-rs-content/issues/72) |
| `Collection.c4f`, Golems, Modern Combat, and retained compilation dependencies | curated import | `UltimateClonkCompilation-v3.0.zip`; SHA-256 `f6fce16b6bb4c9e150ba239876568a84187ab0c3e2ad45619d11fdb5e08214c3`; full source record in [`UltimateClonkCompilation/PROVENANCE.md`](UltimateClonkCompilation/PROVENANCE.md) | compilation `Authors.txt` plus notices inside individual works | compilation readme leaves works with their individual authors; blanket redistribution and derivative terms: `unknown` | commit [`3ecc43c7`](https://github.com/clonk-org/clonk-rs-content/commit/3ecc43c791eb9d7c1c4bb8f1c038ea2fd6a95f70); Collection tree `e11d9419be40c227be237a2655cf52bc0fe69da4` | [#68](https://github.com/clonk-org/clonk-rs-content/issues/68)-[#77](https://github.com/clonk-org/clonk-rs-content/issues/77) |
| `Collection.c4f/Fun.c4f/ClonkPartyRemake8-clonk-rs-localized.c4s` | maintained derivative | CCAN entry 6111, `Clonk_Party_Remake.zip`; archive SHA-256 `1cebf848fee9611d4fa90c6f7c5b78a8c6a5d07701ea3735137d12c65d882586`; original packed scenario SHA-256 `103577bbe25fa3773923eff62dd0de1a37bc00a0e29561e3fc168c1687a425a0` | CCAN description and scenario resources retain the original remake credits; packaged `ClonkRsLocalization.txt` records the derivative and source | the CCAN entry says the ground-up remake may be freely copied or modified and asks changed versions to provide a version history and a new filename; both are included in this derivative | commit [`b9214caf`](https://github.com/clonk-org/clonk-rs-content/commit/b9214cafb46ac1fd82293d1fe9f2f78a48831a47); original packed scenario is byte-identical to CCAN entry 6111 | [#58](https://github.com/clonk-org/clonk-rs-content/issues/58) |
| `E.P.I.C.c4f` | curated import | maintainer archive; packed `E.P.I.C.c4d` SHA-256 `3ed243a408bd204b94b7a96a0f556ddc5c595e874d3236dc42fb33013055a488`; original scenario-container artifact and SHA-256: `unknown`; details in [`EPIC/PROVENANCE.md`](EPIC/PROVENANCE.md) | embedded credits name Zagabar, Wertilq, Elwebbaro, and other contributors | redistribution and derivative terms: `unknown` | commit [`b9214caf`](https://github.com/clonk-org/clonk-rs-content/commit/b9214cafb46ac1fd82293d1fe9f2f78a48831a47); exploded tree `266844cedfa6afb4eb7de03332d640daedb82c36` | [#78](https://github.com/clonk-org/clonk-rs-content/issues/78) |

## Project patch ledger

| Commit | Scope | Source baseline | Project-authored change |
|---|---|---|---|
| [`2cb2a7bd`](https://github.com/clonk-org/clonk-rs-content/commit/2cb2a7bdd4053f093c0192f3dddbaa0acd850b41) | ClonkMars, Eke Reloaded | `c90bafba` imported trees above | Added and repaired English player-visible text and metadata. |
| [`d34d3855`](https://github.com/clonk-org/clonk-rs-content/commit/d34d385591134ce6c262b8c9ed53faaa6229cc6b) | Queron 3, Metal & Magic | external unmodified archive hashes `unknown` | Imported working trees with project-added English localization. |
| [`963e8cb4`](https://github.com/clonk-org/clonk-rs-content/commit/963e8cb4458b220fc4b8ca18be7ae6ced75f2f5a) | MetalMagicExtra | `d34d3855` tree | Replaced Troll crew sheets with renders from the original rigs. |
| [`3ffa183d`](https://github.com/clonk-org/clonk-rs-content/commit/3ffa183d4d70e1cf5c69996ff110a5971a2f10bf) | Eke Reloaded | `c90bafba` tree plus `2cb2a7bd` | Changed HarpoonRace relaunch-message behavior. |
| [`201c4a24`](https://github.com/clonk-org/clonk-rs-content/commit/201c4a24edf17d322ffcd16ef7c79491189aa95a), reverted by [`e82d6d27`](https://github.com/clonk-org/clonk-rs-content/commit/e82d6d275741dca4d520de6f51c21e87767d6b78) | ClonkMars, Eke Reloaded, Metal & Magic | then-current working trees | Super-resolved preview art, then reverted it; no net effect remains. |
| [`2772c602`](https://github.com/clonk-org/clonk-rs-content/commit/2772c602a0142fcf50879a44bb64d6005ecbc7ab) | ClonkMars | then-current working tree | Removed a stale logo override. |
| [`3ecc43c7`](https://github.com/clonk-org/clonk-rs-content/commit/3ecc43c791eb9d7c1c4bb8f1c038ea2fd6a95f70) | Ultimate Clonk Compilation | v3.0 archive above | Selected dependencies and exploded scenario containers for repository discovery. |
| [`b9214caf`](https://github.com/clonk-org/clonk-rs-content/commit/b9214cafb46ac1fd82293d1fe9f2f78a48831a47) | Collection, E.P.I.C. | compilation archive and maintainer E.P.I.C. snapshot above | Moved private definitions, removed duplicates and unused packs, and added exploded E.P.I.C. scenarios. |
| [clonk-org/clonk-rs-content#58](https://github.com/clonk-org/clonk-rs-content/issues/58) | Collection Fun Clonk Party Remake | CCAN entry 6111 artifact and original packed scenario recorded above | Added reviewed English runtime localization, renamed the derivative, and packaged its source and version history while retaining credits, substitutions, encoding, and nested pack boundaries. |

For every future derivative change, append a row naming its pull request or
commit, affected scope, source baseline, exact purpose, and preserved notices.
If it comes from a newer upstream artifact, also update the source ledger with
the artifact filename, source URL or reference, version, SHA-256 and hash scope.

The `localization` lists in `packs.toml` are temporary audit debt, not a rights or
modification allowlist. Removing a row makes that scope subject to the complete
localization audit regardless of its `binary` attribute.
