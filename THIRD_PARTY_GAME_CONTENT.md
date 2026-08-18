# Bundled third-party game content

The maintainer has obtained explicit permission to commit and publicly
redistribute the exact copies of the following classic Clonk game packs that
are present in the Clonk Rust project's repositories:

- `EkeReloaded.c4d` and `EkeReloaded.c4f`, version 1.7, credited by the packs
  to Stephan Bednarzik (Mirosch).
- `ClonkMars.c4d` and `ClonkMars.c4f`, version 1.5 “Spirit”, credited by the
  packs to Luchs and Nachtfalter.

The named authorship files are preserved in both definition and scenario
packs. The complete ClonkMars contributor and third-party asset
acknowledgements are preserved in `ClonkMars.c4d/Credits.txt`. Additional
component-specific author notices remain in their original locations
throughout both packs.

The packs' original notices, including `ClonkMars.c4f/Copyright.txt`, remain
part of the copies. These packs are not distributed under the ISC license that
applies to the Clonk Rust source, nor are they included in the CC BY-NC grant
that covers the rest of this repository's content (see
`clonk_content_license.txt`). Their inclusion is based on the explicit
permission recorded here.

That permission is limited to committing and publicly redistributing these
exact copies in the Clonk Rust project's repositories — the engine repository
and this game-content repository — and in the release archives built from
them. For those distributions only, the specific permission authorizes that
activity notwithstanding more restrictive language in the preserved notices,
including `ClonkMars.c4f/Copyright.txt`. It does not amend or replace those
notices, is not a relicensing, and neither claims nor grants broader rights in
the packs or their individual assets. No attribution or license files were
inserted into the classic group directories, so documenting the permission
does not change their resource bytes or network checksums.

### These packs contain packed groups, and must stay that way

103 files inside `ClonkMars.*` and `EkeReloaded.*` are *packed* C4Groups
rather than directories — 56 in `ClonkMars.c4d`, 1 in `ClonkMars.c4f`, 25 in
`EkeReloaded.c4d` and 21 in `EkeReloaded.c4f`. For example
`ClonkMars.c4d/Helpers.c4d/Explosion.c4d/Blast.c4d` is a single binary blob,
while definitions elsewhere in the same pack are unpacked directories of
`DefCore.txt`, `Script.c` and `Graphics.png`.

This is how the packs were received, and it is deliberate. Unpacking them would
rewrite the resource bytes and the group checksums this section is careful to
preserve, so **do not unpack them for consistency with the rest of the tree.**

Two consequences worth knowing before searching this repository:

- `grep` does not see their scripts. A search for a C4Script function will
  report fewer call sites than exist, and `grep -a` does not help, because the
  contents are compressed rather than merely binary.
- `git diff` shows `Binary files differ` for any change to one of them.

Use the `c4group` tool to inspect one; explode a copy outside the repository
rather than in place.

## Community packs added from CCAN

**No permission has been obtained from the authors for the two packs in this
section.** They are recorded separately from the packs above for that reason,
and the difference is deliberate: nothing here should be read as extending the
explicit permission documented above.

- `Melees.c4f/Queron3.c4s`, version 3.41, credited by the pack's `Author.txt`
  to Samuel Beer (Miniwipf). Retrieved 2026-07-27 from CCAN entry 5737
  (`https://ccan.de/cgi-bin/ccan/ccan-view.pl?a=view&i=5737`). That entry
  carries a public note from the author to developers stating that the
  scenario's contents may be reused, asking only that the line in the script
  naming the author and version be left in place. That line —
  `// script v3.91 by miniwipf` in `Script.c` — is preserved unmodified, as is
  `Author.txt`. The note is a public statement on the archive entry, not a
  permission granted to this project, and its scope has not been verified with
  the author.
- `MetalMagic.c4d` and `MetalMagicExtra.c4d`, version 3.1b/3.1c, credited by
  `MetalMagic.c4d/Author.txt` to Merten Ehmig, David Dormagen and Richard
  Gerum. Retrieved 2026-07-27 from CCAN entry 4068
  (`https://ccan.de/cgi-bin/ccan/ccan-view.pl?a=view&i=4068`). **That entry
  states no redistribution terms of any kind.** These two definition packs are
  present only because `Queron3.c4s/Scenario.txt` names them in `Definition4`
  and `Definition5`; without them the scenario cannot resolve its definitions
  and will not load. Only those two required packs were taken from the
  `MM3_1c.zip` download — `MetalMagic.c4f` and `Adventure.c4s`, the pack's own
  scenarios, were not copied.

Both packs are freely downloadable from CCAN, but free availability is not a
redistribution grant. Neither pack is covered by the ISC license that applies
to the Clonk Rust source, nor by the CC BY-NC grant in
`clonk_content_license.txt`. Before this content repository is made public, or
included in any release archive, the redistribution position for
`MetalMagic.c4d`/`MetalMagicExtra.c4d` in particular should be settled with
the authors.

The packs' own `Author.txt`, `Version.txt` and description files are preserved
as downloaded. The only modifications are added English localization files —
`StringTblUS.txt`, `DescUS.txt`/`DescUS.rtf`, `US:` name lines — and the
replacement of a few hardcoded German display strings with `$Key$` lookups
resolved from those tables. No German text was removed: every German string
remains reachable through the packs' `StringTblDE.txt` and `Desc*DE*` files.
