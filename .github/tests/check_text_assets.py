#!/usr/bin/env python3
"""Guards the encoding of game text assets against silent drift.

The corpus is US-ASCII and ISO-8859-1, as classic Clonk content is. Editors and
tooling re-encode ISO-8859-1 to UTF-8 on save without asking; the file still
parses, the game still runs, every other check stays green, and the only symptom
is a German umlaut rendering as two characters somewhere nobody looks.

This compares each *changed* asset against its previous version rather than
auditing the tree, for two reasons. Seven in-group files are already UTF-8, and
converting them back is not obviously right — one of them lives inside
`ClonkMars.c4d`, which is redistributed as an exact copy of the pack as
received. And some carry a stray trailing NUL. Both are pre-existing backlogs; the
point here is to stop the pile growing.

Usage: check_text_assets.py [<base-ref>]     (default: origin/main)

Lives under .github/ so it does not ship in content.zip.
"""

import subprocess
import sys

# The text formats the engine reads out of group directories.
EXTENSIONS = (".txt", ".c", ".c4m")

# Only files inside a Clonk group are game content. This keeps the rule off
# repository documentation such as `README.md` and
# `third_party/Hazard/readme.txt`, which are project-authored and legitimately
# UTF-8.
GROUP_SUFFIXES = (".c4d", ".c4f", ".c4g", ".c4s", ".c4p")


def is_game_text_asset(path: str) -> bool:
    if not path.lower().endswith(EXTENSIONS):
        return False
    return any(
        segment.lower().endswith(GROUP_SUFFIXES) for segment in path.split("/")[:-1]
    )


def classify(blob: bytes) -> str:
    """ascii, utf8 or eight-bit.

    `utf8` means "contains non-ASCII and decodes as UTF-8" — the state a
    re-encode lands in. Pure ASCII is valid UTF-8 too, and is reported
    separately because it is the state most of the corpus is in.
    """
    if all(byte < 0x80 for byte in blob):
        return "ascii"
    try:
        blob.decode("utf-8")
    except UnicodeDecodeError:
        return "eight-bit"
    return "utf8"


def git(*args: str) -> bytes:
    return subprocess.run(
        ["git", *args], check=True, stdout=subprocess.PIPE
    ).stdout


def previous_version(ref: str, path: str) -> bytes | None:
    result = subprocess.run(
        ["git", "show", f"{ref}:{path}"], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL
    )
    return result.stdout if result.returncode == 0 else None


def changed_paths(base: str) -> list[str]:
    """Paths differing from `base`, unquoted.

    `-z` is not optional here. Seventy paths in this repository contain non-ASCII
    characters, and git's default output would render them C-quoted —
    `"Hazard.c4f/.../\\303\\234berladungen.c4d/..."` — which no longer ends in
    `.txt`, so every one of them would be skipped silently. A check that quietly
    ignores the German directories is worse than no check.
    """
    raw = git("-c", "core.quotePath=false", "diff", "--name-only", "-z", base, "HEAD")
    return [entry.decode("utf-8") for entry in raw.split(b"\0") if entry]


def main() -> int:
    base = sys.argv[1] if len(sys.argv) > 1 else "origin/main"

    assets = [path for path in changed_paths(base) if is_game_text_asset(path)]
    if not assets:
        print("no game text assets changed")
        return 0

    problems = []
    for path in assets:
        new = previous_version("HEAD", path)
        if new is None:
            continue  # deleted
        old = previous_version(base, path)

        new_class = classify(new)
        old_class = classify(old) if old is not None else None

        # The drift this exists to catch: a Latin-1 or ASCII asset arriving as
        # UTF-8. Files that were already UTF-8 are grandfathered.
        if new_class == "utf8" and old_class != "utf8":
            origin = "a new file" if old_class is None else f"was {old_class}"
            problems.append(
                f"{path}: {origin}, now UTF-8. Clonk text assets are ISO-8859-1; "
                f"an editor has probably re-encoded it on save."
            )

        # No *new* NUL bytes. Fifteen assets already carry a stray trailing one,
        # so this deliberately compares against the previous version rather than
        # rejecting outright.
        if 0 in new and (old is None or 0 not in old):
            problems.append(f"{path}: gained a NUL byte, which makes it binary to Git.")

    for problem in problems:
        print(f"  {problem}", file=sys.stderr)

    if problems:
        print(f"\n{len(problems)} text-asset problem(s)", file=sys.stderr)
        return 1

    print(f"ok {len(assets)} changed game text asset(s), no encoding drift")
    return 0


if __name__ == "__main__":
    sys.exit(main())
