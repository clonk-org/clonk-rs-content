#!/usr/bin/env python3
"""Check that maintained game text has complete English localization.

``binary`` in ``.gitattributes`` controls checkout bytes. It does not decide
which content this audit maintains. Which imports may carry that attribute, and
which are temporarily excluded behind a linked issue, is recorded per pack in
``packs.toml``.
"""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import subprocess
import sys


REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))
import group_entry  # noqa: E402
from packs import Manifest  # noqa: E402

MANIFEST = Manifest.load(REPO_ROOT)

# Classic Clonk calls English ``US`` in localized asset names.
LOCALIZED_FILE_PAIRS = {
    "descde.txt": "DescUS.txt",
    "descde.rtf": "DescUS.rtf",
    "rankde.txt": "RankUS.txt",
    "stringtblde.txt": "StringTblUS.txt",
}

CANONICAL_LOCALIZED_NAMES = {
    name.casefold(): name
    for name in (
        "DescDE.txt",
        "DescUS.txt",
        "DescDE.rtf",
        "DescUS.rtf",
        "RankDE.txt",
        "RankUS.txt",
        "StringTblDE.txt",
        "StringTblUS.txt",
        "Names.txt",
        "Title.txt",
    )
}

# The imports allowed to use ``binary`` checkout handling in ``.gitattributes``:
# every entry packs.toml marks ``bytes = "preserve"``. Keeping the boundary in
# the manifest prevents that attribute from spreading silently into maintained
# base content.
BINARY_IMPORTS = tuple(Path(path) for path in MANIFEST.preserved_roots())
GROUP_SUFFIXES = (".c4d", ".c4f", ".c4g", ".c4s")

# Keep the complete directive: width, flags, precision and conversion case all
# affect C4Script Format output. Literal %% is intentionally ignored.
FORMAT_TOKEN = re.compile(
    r"%(?:%|[-+#0]*(?:\d+|\*)?(?:\.(?:\d+|\*))?[hlLzjt]*[diuoxXfFeEgGaAcspn])"
)
ICON_TOKEN = re.compile(r"\{\{[A-Za-z0-9_]{4}\}\}")
LOCALE_LINE = re.compile(r"^([A-Za-z]{2}):(.*)$")
WORD = re.compile(r"[A-Za-zÀ-ÿ]+")
RTF_HEX = re.compile(r"\\'([0-9A-Fa-f]{2})")
RTF_CONTROL = re.compile(r"\\[A-Za-z]+-?\d* ?")

# High-confidence German leftovers found in English values during the corpus
# audit. Ambiguous words such as ``die`` and proper names stay allowed.
GERMAN_RESIDUE_WORDS = {
    "belegen",
    "besitzer",
    "dekoration",
    "für",
    "gefunden",
    "glas",
    "hier",
    "hilfe",
    "ist",
    "kann",
    "keine",
    "nicht",
    "objekt",
    "objekte",
    "projektor",
    "rohr",
    "schussmodus",
    "spielstand",
    "typ",
    "und",
    "verfügbar",
}

RESOURCE_REFERENCE = re.compile(r"\$[^$]+\$|\{\{[A-Za-z0-9_]{4}\}\}")
PLAYER_TEXT_ARGUMENTS = {
    "AddMenuItem": (0, 6),
    "CreateMenu": (4,),
    "Message": (0,),
    "PlayerMessage": (1,),
    "SetScoreboardData": (2,),
}

# Arguments that name something the engine looks up: a variable, a function,
# an effect, an action or a menu command. C4ScriptHost::MakeScript replaces a
# string-table placeholder from the table of the player's own language
# (C4ScriptHost.cpp:56-57, 66-75), so a placeholder there names something
# different in each language (clonk-org/clonk-rs-content#149).
NAME_ARGUMENTS = {
    "AddEffect": (0,),
    "AddMenuItem": (1,),
    "Call": (0,),
    "ChangeEffect": (0,),
    "CheckEffect": (0,),
    "DefinitionCall": (1,),
    "EffectCall": (2,),
    "GameCall": (0,),
    "GameCallEx": (0,),
    "GetEffect": (0,),
    "GetEffectCount": (0,),
    "GlobalN": (0,),
    "LocalN": (0,),
    "ObjectCall": (1,),
    "ObjectSetAction": (1,),
    "PrivateCall": (1,),
    "ProtectedCall": (1,),
    "RemoveEffect": (0,),
    "ScheduleCall": (1,),
    "SetAction": (0,),
}
PLACEHOLDER_ARGUMENT = re.compile(r'"\$[^"$]+\$"')


def git(*args: str, input_bytes: bytes | None = None) -> bytes:
    return subprocess.run(
        ["git", *args],
        cwd=REPO_ROOT,
        check=True,
        input=input_bytes,
        stdout=subprocess.PIPE,
    ).stdout


def repository_files() -> list[Path]:
    """Return tracked and untracked, non-ignored files in the worktree."""
    raw = git("ls-files", "-z", "--cached", "--others", "--exclude-standard")
    paths = [
        Path(entry.decode("utf-8"))
        for entry in raw.split(b"\0")
        if entry
    ]
    return sorted(path for path in paths if (REPO_ROOT / path).is_file())


def attributed_binary(paths: list[Path]) -> set[Path]:
    if not paths:
        return set()

    raw = git(
        "check-attr",
        "-z",
        "binary",
        "--stdin",
        input_bytes=b"\0".join(path.as_posix().encode("utf-8") for path in paths)
        + b"\0",
    ).split(b"\0")
    return {
        Path(raw[index].decode("utf-8"))
        for index in range(0, len(raw) - 2, 3)
        if raw[index + 2] == b"set"
    }


def approved_binary_import(path: Path) -> bool:
    return any(path == root or root in path.parents for root in BINARY_IMPORTS)


def binary_attribute_problems(paths: list[Path], attributed: set[Path]) -> list[str]:
    approved = {path for path in paths if approved_binary_import(path)}
    problems = [
        f"{path}: unapproved binary attribute would hide maintained content"
        for path in sorted(attributed - approved)
    ]
    problems.extend(
        f"{path}: approved binary import is missing its binary attribute"
        for path in sorted(approved - attributed)
    )
    return problems


def localization_pending(paths: list[Path]) -> tuple[set[Path], list[str]]:
    roots = [Path(root) for root in MANIFEST.localization_pending()]
    pending = {
        path
        for path in paths
        if any(path == root or root in path.parents for root in roots)
    }
    return pending, list(MANIFEST.problems)


def is_group_content(path: Path) -> bool:
    return any(part.casefold().endswith(GROUP_SUFFIXES) for part in path.parts)


def canonical_filename_problems(paths: list[Path], excluded: set[Path]) -> list[str]:
    problems = []
    for path in paths:
        canonical = CANONICAL_LOCALIZED_NAMES.get(path.name.casefold())
        if path in excluded or not is_group_content(path) or canonical is None:
            continue
        if path.name != canonical:
            problems.append(f"{path}: use canonical localization name {canonical}")
    return problems


def missing_english_files(paths: list[Path], excluded: set[Path]) -> list[str]:
    available = {path.as_posix().casefold() for path in paths}
    problems = []

    for path in paths:
        english_name = LOCALIZED_FILE_PAIRS.get(path.name.casefold())
        if path in excluded or not is_group_content(path) or english_name is None:
            continue

        english_path = path.with_name(english_name)
        if english_path.as_posix().casefold() not in available:
            problems.append(f"{english_path}: missing English counterpart to {path}")
            continue

        if asset_nonempty(path) and not asset_nonempty(english_path):
            problems.append(f"{english_path}: English counterpart to {path} is empty")

    return problems


def asset_nonempty(path: Path) -> bool:
    return bool((REPO_ROOT / path).read_bytes().strip(b"\x00\t\r\n "))


def string_table(path: Path) -> tuple[dict[str, str], list[str]]:
    text = (REPO_ROOT / path).read_bytes().decode("latin-1")
    entries = {}
    problems = []

    # C4LangStringTable ends a line only at "\n" or "\r"
    # (C4LangStringTable.cpp:54-59); str.splitlines would also split at
    # bytes such as 0x85, the ellipsis of the shipped charset.
    lines = text.replace("\r\n", "\n").replace("\r", "\n").split("\n")
    for line_number, line in enumerate(lines, 1):
        stripped = line.strip()
        if not stripped or stripped.startswith(("#", ";", "//")):
            continue
        if "=" not in line:
            problems.append(f"{path}:{line_number}: malformed string-table line")
            continue
        key, value = line.split("=", 1)
        key = key.strip()
        if not key:
            problems.append(f"{path}:{line_number}: empty string-table key")
            continue
        if key in entries:
            problems.append(f"{path}:{line_number}: duplicate string-table key {key}")
            continue
        entries[key] = value.strip()

    return entries, problems


def substitution_tokens(value: str) -> tuple[list[str], Counter[str]]:
    return (
        [match.group(0) for match in FORMAT_TOKEN.finditer(value) if match.group(0) != "%%"],
        Counter(ICON_TOKEN.findall(value)),
    )


def string_table_problems(paths: list[Path], excluded: set[Path]) -> list[str]:
    by_name = {path.as_posix().casefold(): path for path in paths}
    problems = []
    maintained_tables = [
        path
        for path in paths
        if path not in excluded
        and is_group_content(path)
        and path.name.casefold() in ("stringtblde.txt", "stringtblus.txt")
    ]
    parsed = {}

    for path in maintained_tables:
        entries, parse_problems = string_table(path)
        parsed[path] = entries
        problems.extend(parse_problems)

    for german_path in maintained_tables:
        if german_path.name.casefold() != "stringtblde.txt":
            continue

        english_path = by_name.get(
            german_path.with_name("StringTblUS.txt").as_posix().casefold()
        )
        if english_path is None:
            continue  # Reported by missing_english_files.

        german = parsed[german_path]
        english = parsed[english_path]

        for key in sorted(german.keys() - english.keys()):
            problems.append(f"{english_path}: missing English key {key}")
        for key in sorted(german.keys() & english.keys()):
            if german[key] and not english[key]:
                problems.append(f"{english_path}: empty English value for {key}")
            if substitution_tokens(german[key]) != substitution_tokens(english[key]):
                problems.append(
                    f"{english_path}: substitution tokens for {key} do not match German"
                )

    return problems


def is_crew_name_list(path: Path) -> bool:
    """A scenario's Names.txt, which C4Game loads as its crew name list
    (C4CFN_Names, C4Game.cpp:3307-3308) rather than as localized names."""
    return path.name.casefold() == "names.txt" and path.parent.name.casefold().endswith(".c4s")


def localized_metadata_problems(paths: list[Path], excluded: set[Path]) -> list[str]:
    problems = []

    for path in paths:
        if (
            path in excluded
            or not is_group_content(path)
            or path.name.casefold() not in ("names.txt", "title.txt")
            or is_crew_name_list(path)
        ):
            continue

        text = (REPO_ROOT / path).read_bytes().decode("latin-1").strip("\x00\r\n ")
        locales = {}
        for line_number, line in enumerate(text.splitlines(), 1):
            stripped = line.strip()
            if not stripped or stripped.startswith(("#", ";", "//")):
                continue
            match = LOCALE_LINE.fullmatch(line)
            if match is None:
                problems.append(f"{path}:{line_number}: malformed localized metadata")
                continue
            locale = match.group(1).upper()
            if locale in locales:
                problems.append(
                    f"{path}:{line_number}: duplicate localized metadata locale {locale}"
                )
                continue
            locales[locale] = match.group(2).strip()

        if text and not locales:
            problems.append(f"{path}: text is not tagged with DE:/US: locales")
            continue
        if locales.get("DE") and not locales.get("US"):
            problems.append(f"{path}: German text has no English US: value")

    return problems


def english_values(path: Path) -> list[str]:
    name = path.name.casefold()
    if name == "stringtblus.txt":
        entries, _ = string_table(path)
        return list(entries.values())

    text = (REPO_ROOT / path).read_bytes().decode("latin-1")
    if name == "descus.rtf":
        text = RTF_HEX.sub(lambda match: chr(int(match.group(1), 16)), text)
        text = RTF_CONTROL.sub(" ", text)
        return [text.replace("{", " ").replace("}", " ")]
    if name in ("descus.txt", "rankus.txt"):
        return [text]
    if name in ("names.txt", "title.txt"):
        return [
            match.group(2)
            for line in text.splitlines()
            if (match := LOCALE_LINE.fullmatch(line))
            if match.group(1).upper() == "US"
        ]
    return []


def german_residue_problems(paths: list[Path], excluded: set[Path]) -> list[str]:
    problems = []

    for path in paths:
        if path in excluded or not is_group_content(path):
            continue
        words = {
            word.casefold()
            for value in english_values(path)
            for word in WORD.findall(value)
        }
        residue = sorted(words & GERMAN_RESIDUE_WORDS)
        if residue:
            problems.append(f"{path}: German word(s) in English text: {', '.join(residue)}")

    return problems


def string_end(text: str, quote: int) -> int:
    index = quote + 1
    while index < len(text):
        if text[index] == "\\":
            index += 2
            continue
        if text[index] == '"':
            return index + 1
        index += 1
    return len(text)


def comment_end(text: str, index: int) -> int | None:
    if text.startswith("//", index):
        newline = text.find("\n", index + 2)
        return len(text) if newline < 0 else newline + 1
    if text.startswith("/*", index):
        close = text.find("*/", index + 2)
        return len(text) if close < 0 else close + 2
    return None


def call_arguments(text: str, opening_parenthesis: int) -> tuple[list[str], int] | None:
    arguments = []
    argument_start = opening_parenthesis + 1
    stack = []
    index = argument_start
    pairs = {"(": ")", "[": "]", "{": "}"}

    while index < len(text):
        if text[index] == '"':
            index = string_end(text, index)
            continue
        if (end := comment_end(text, index)) is not None:
            index = end
            continue

        character = text[index]
        if character in pairs:
            stack.append(pairs[character])
        elif stack and character == stack[-1]:
            stack.pop()
        elif character == ")" and not stack:
            arguments.append(text[argument_start:index])
            return arguments, index + 1
        elif character == "," and not stack:
            arguments.append(text[argument_start:index])
            argument_start = index + 1
        index += 1

    return None


def calls(text: str, functions: dict[str, tuple[int, ...]]):
    """Each call of one of `functions` in C4Script `text`: its name, its
    argument sources and the line it starts on."""
    index = 0
    while index < len(text):
        if text[index] == '"':
            index = string_end(text, index)
            continue
        if (end := comment_end(text, index)) is not None:
            index = end
            continue
        if not (text[index].isalpha() or text[index] == "_"):
            index += 1
            continue

        start = index
        index += 1
        while index < len(text) and (text[index].isalnum() or text[index] == "_"):
            index += 1
        name = text[start:index]
        if name not in functions:
            continue

        opening = index
        while opening < len(text) and text[opening].isspace():
            opening += 1
        if opening >= len(text) or text[opening] != "(":
            continue
        if (parsed := call_arguments(text, opening)) is not None:
            arguments, _ = parsed
            yield name, arguments, text.count("\n", 0, start) + 1


def quoted_strings(text: str):
    index = 0
    while index < len(text):
        if (end := comment_end(text, index)) is not None:
            index = end
            continue
        if text[index] != '"':
            index += 1
            continue

        end = string_end(text, index)
        value = text[index + 1 : max(index + 1, end - 1)]
        yield re.sub(r"\\(.)", r"\1", value)
        index = end


def hardcoded_player_text_problems(paths: list[Path], excluded: set[Path]) -> list[str]:
    problems = []

    for path in paths:
        if path in excluded or not is_group_content(path) or path.suffix.casefold() != ".c":
            continue

        text = (REPO_ROOT / path).read_bytes().decode("latin-1")
        for name, arguments, line_number in calls(text, PLAYER_TEXT_ARGUMENTS):
            for argument_index in PLAYER_TEXT_ARGUMENTS[name]:
                if argument_index >= len(arguments):
                    continue
                for value in quoted_strings(arguments[argument_index]):
                    visible = RESOURCE_REFERENCE.sub("", value)
                    words = {word.casefold() for word in WORD.findall(visible)}
                    residue = sorted(words & GERMAN_RESIDUE_WORDS)
                    if residue:
                        problems.append(
                            f"{path}:{line_number}: hard-coded German player text: "
                            f"{', '.join(residue)}"
                        )

    return problems


def packed_scripts(image: bytes, location: str):
    """Each script in a packed group image, with its path inside the group."""
    entries, data_start = group_entry.records(image)
    for entry in entries:
        name = entry["name"].decode("latin-1")
        begin = data_start + entry["offset"]
        data = image[begin : begin + entry["size"]]
        if entry["child"]:
            yield from packed_scripts(data, f"{location}/{name}")
        elif name.casefold().endswith(".c"):
            yield f"{location}/{name}", data


def scripts(paths: list[Path], excluded: set[Path]):
    """Each maintained C4Script source, plain or inside a packed group, as
    (location, text)."""
    for path in paths:
        if path in excluded or not is_group_content(path):
            continue
        if path.suffix.casefold() == ".c":
            yield str(path), (REPO_ROOT / path).read_bytes().decode("latin-1")
        elif path.suffix.casefold() in GROUP_SUFFIXES:
            raw = (REPO_ROOT / path).read_bytes()
            if raw[:2] != group_entry.MAGIC:
                continue
            for location, data in packed_scripts(group_entry.unpack(raw), str(path)):
                yield location, data.decode("latin-1")


def placeholder_name_problems(paths: list[Path]) -> list[str]:
    """Pending localization excuses missing English, not a placeholder that
    already makes each language run different code, so every script counts."""
    problems = []

    for path, text in scripts(paths, set()):
        for name, arguments, line_number in calls(text, NAME_ARGUMENTS):
            for argument_index in NAME_ARGUMENTS[name]:
                if argument_index >= len(arguments):
                    continue
                argument = arguments[argument_index].strip()
                if PLACEHOLDER_ARGUMENT.fullmatch(argument):
                    problems.append(
                        f"{path}:{line_number}: {name} names something with "
                        f"string-table placeholder {argument}, which differs per language"
                    )

    return problems


def localization_source_count(paths: list[Path], excluded: set[Path]) -> int:
    return sum(
        path not in excluded
        and is_group_content(path)
        and path.name.casefold()
        in {*LOCALIZED_FILE_PAIRS, "names.txt", "title.txt"}
        for path in paths
    )


def has_localization_sources(paths: list[Path]) -> bool:
    return any(
        is_group_content(path)
        and path.name.casefold()
        in {*LOCALIZED_FILE_PAIRS, "names.txt", "title.txt"}
        for path in paths
    )


def main() -> int:
    paths = repository_files()
    binary = attributed_binary(paths)
    pending, pending_problems = localization_pending(paths)
    source_count = localization_source_count(paths, pending)
    problems = (
        binary_attribute_problems(paths, binary)
        + pending_problems
        + canonical_filename_problems(paths, pending)
        + missing_english_files(paths, pending)
        + string_table_problems(paths, pending)
        + localized_metadata_problems(paths, pending)
        + german_residue_problems(paths, pending)
        + hardcoded_player_text_problems(paths, pending)
        + placeholder_name_problems(paths)
    )
    if not has_localization_sources(paths):
        problems.append("no localization sources found")

    for problem in problems:
        print(f"  {problem}", file=sys.stderr)

    if problems:
        print(f"\n{len(problems)} localization problem(s)", file=sys.stderr)
        return 1

    print(f"ok {source_count} maintained localization sources audited")
    return 0


if __name__ == "__main__":
    sys.exit(main())
