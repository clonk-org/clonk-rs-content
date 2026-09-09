#!/usr/bin/env python3
"""The pack manifest: what each entry of the data root is, described once.

``packs.toml`` lists every directory and file at the repository root that a
player receives, with the policies that used to be scattered across
``.gitattributes``, ``set_version.sh``, the localization checker and the
layout test. Those consumers read it through this module; ``check`` compares
it against the tracked tree so the two cannot drift apart silently.

Library use::

    manifest = Manifest.load(repository_root)

Command line, one path per line, for the shell scripts::

    packs.py [--root DIR] roots|own-version|preserved|preserved-roots|
                          nested-definitions|pending|check
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
import re
import subprocess
import sys
import tomllib


MANIFEST = Path("packs.toml")
CONTENT_ISSUE = re.compile(r"clonk-org/clonk-rs-content#[0-9]+")
BYTE_POLICIES = ("normalize", "preserve")
GROUP_SUFFIXES = (".c4d", ".c4f", ".c4g", ".c4s")
PACK_FIELDS = frozenset(
    {
        "origin",
        "bytes",
        "upstream_version",
        "localization",
        "nested_definitions",
        "rights",
        "license",
        "reason",
        "evidence",
    }
)


@dataclass(frozen=True)
class Pack:
    path: str
    origin: str
    bytes: str
    upstream_version: str | None
    localization: tuple[str, ...]
    nested_definitions: tuple[str, ...]

    @property
    def preserved(self) -> bool:
        return self.bytes == "preserve"

    @property
    def is_root_entry(self) -> bool:
        return "/" not in self.path

    def nested_definition_paths(self) -> list[str]:
        return [f"{self.path}/{nested}" for nested in self.nested_definitions]


@dataclass(frozen=True)
class Manifest:
    origins: dict[str, dict]
    packs: tuple[Pack, ...]
    problems: tuple[str, ...]

    @classmethod
    def load(cls, root: Path) -> Manifest:
        path = root / MANIFEST
        if not path.is_file():
            return cls({}, (), (f"{MANIFEST}: missing",))
        try:
            document = tomllib.loads(path.read_text(encoding="utf-8"))
        except tomllib.TOMLDecodeError as error:
            return cls({}, (), (f"{MANIFEST}: {error}",))
        origins = document.get("origins", {})
        packs, problems = _read_packs(document.get("packs", {}), origins)
        unknown = sorted(set(document) - {"origins", "packs", "distribution"})
        problems.extend(f"{MANIFEST}: unknown table [{key}]" for key in unknown)
        problems.extend(_distribution_problems(root, document))
        return cls(origins, tuple(packs), tuple(problems))

    def root_entries(self) -> list[str]:
        return [pack.path for pack in self.packs if pack.is_root_entry]

    def own_version_packs(self) -> list[str]:
        return [
            pack.path for pack in self.packs if pack.is_root_entry and pack.preserved
        ]

    def preserved_roots(self) -> list[str]:
        return [pack.path for pack in self.packs if pack.preserved]

    def preserved_paths(self) -> list[str]:
        return [
            path
            for pack in self.packs
            for path in ([pack.path] if pack.preserved else [])
            + pack.nested_definition_paths()
        ]

    def nested_definitions(self) -> list[str]:
        return [path for pack in self.packs for path in pack.nested_definition_paths()]

    def localization_pending(self) -> dict[str, tuple[str, ...]]:
        return {
            pack.path: pack.localization for pack in self.packs if pack.localization
        }


def _safe_path(path) -> bool:
    return (
        isinstance(path, str)
        and bool(path)
        and not any(char in path for char in "\\:\0")
        and all(part not in ("", ".", "..") for part in path.split("/"))
    )


def _distribution_problems(root: Path, document: dict) -> list[str]:
    scopes = document.get("packs", {})
    if "distribution" not in document and not any(
        "rights" in fields for fields in scopes.values()
    ):
        return []  # Historical manifests remain readable by version/localization tools.
    policy = document.get("distribution", {})
    problems = []
    if not isinstance(policy, dict) or policy.get("version") != 1:
        return ["distribution: missing or unsupported policy version"]
    unknown = set(policy) - {"version", "notice", "dependencies"}
    if unknown:
        problems.append(f"distribution: unknown fields {sorted(unknown)}")
    notice = policy.get("notice")
    if not _safe_path(notice) or not (root / notice).is_file():
        problems.append("distribution: notice must be a present relative file")
    for path, fields in scopes.items():
        rights = fields.get("rights")
        if rights not in ("licensed", "assumed", "excluded"):
            problems.append(f"distribution: {path}: missing or unknown rights decision")
        key = "license" if rights == "licensed" else "reason"
        value = fields.get(key)
        if not isinstance(value, str) or not value.strip():
            problems.append(f"distribution: {path}: missing {key}")
        evidence = fields.get("evidence", [])
        if not isinstance(evidence, list) or (rights != "excluded" and not evidence):
            problems.append(f"distribution: {path}: needs evidence files")
        elif any(
            not _safe_path(item) or not (root / item).is_file() for item in evidence
        ):
            problems.append(
                f"distribution: {path}: evidence must name present relative files"
            )
    dependencies = policy.get("dependencies", {})
    if not isinstance(dependencies, dict):
        problems.append("distribution: dependencies must be a table")
    else:
        for path, required in dependencies.items():
            if (
                not _safe_path(path)
                or not isinstance(required, list)
                or any(not _safe_path(item) for item in required)
            ):
                problems.append(f"distribution: unsafe dependency paths for {path}")
    return problems


def _read_packs(table: dict, origins: dict) -> tuple[list[Pack], list[str]]:
    packs: list[Pack] = []
    problems: list[str] = []
    for path, fields in table.items():
        problems.extend(_path_problems(path, fields, origins))
        packs.append(
            Pack(
                path=path,
                origin=str(fields.get("origin", "")),
                bytes=str(fields.get("bytes", "normalize")),
                upstream_version=fields.get("upstream_version"),
                localization=tuple(fields.get("localization", ())),
                nested_definitions=tuple(fields.get("nested_definitions", ())),
            )
        )
    return packs, problems


def _path_problems(path: str, fields: dict, origins: dict) -> list[str]:
    problems = []
    if not _is_relative_group_path(path):
        problems.append(f"{path}: must be a relative Clonk group path or Version.txt")
    unknown = sorted(set(fields) - PACK_FIELDS)
    if unknown:
        problems.append(f"{path}: unknown field(s) {', '.join(unknown)}")
    origin = fields.get("origin")
    if origin not in origins:
        problems.append(f"{path}: origin {origin!r} is not declared in [origins]")
    if fields.get("bytes", "normalize") not in BYTE_POLICIES:
        problems.append(f"{path}: bytes must be one of {', '.join(BYTE_POLICIES)}")
    references = fields.get("localization", ())
    if any(CONTENT_ISSUE.fullmatch(str(reference)) is None for reference in references):
        problems.append(
            f"{path}: localization must use qualified clonk-org/clonk-rs-content#N references"
        )
    if any(
        not _is_relative_group_path(str(nested)) or nested == ""
        for nested in fields.get("nested_definitions", ())
    ):
        problems.append(
            f"{path}: nested_definitions must be relative Clonk group paths"
        )
    return problems


def _is_relative_group_path(path: str) -> bool:
    parts = path.split("/")
    return (
        bool(path)
        and not path.startswith("/")
        and all(part not in ("", ".", "..") for part in parts)
        and (path == "Version.txt" or parts[-1].casefold().endswith(GROUP_SUFFIXES))
    )


def check(root: Path) -> list[str]:
    """Every way the manifest and the tracked tree can disagree."""
    manifest = Manifest.load(root)
    if manifest.problems and not manifest.packs:
        return list(manifest.problems)
    return (
        list(manifest.problems)
        + _unlisted_root_packs(root, manifest)
        + _missing_entries(root, manifest)
        + _binary_attribute_problems(root, manifest)
        + _version_problems(root, manifest)
        + _nested_definition_problems(root, manifest)
    )


def _tracked_root_entries(root: Path) -> list[str]:
    output = subprocess.run(
        ["git", "-C", str(root), "ls-files", "-z"],
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    seen: dict[str, None] = {}
    for raw in output.split(b"\0"):
        if raw:
            seen.setdefault(raw.decode("utf-8").split("/", 1)[0], None)
    return list(seen)


def _unlisted_root_packs(root: Path, manifest: Manifest) -> list[str]:
    listed = set(manifest.root_entries())
    return [
        f"{entry}: tracked at the data root but not listed in {MANIFEST}"
        for entry in _tracked_root_entries(root)
        if entry not in listed and _is_relative_group_path(entry)
    ]


def _missing_entries(root: Path, manifest: Manifest) -> list[str]:
    paths = [pack.path for pack in manifest.packs] + manifest.nested_definitions()
    return [
        f"{path}: listed in {MANIFEST} but does not exist"
        for path in paths
        if not (root / path).exists()
    ]


def _binary_rules(root: Path) -> set[str]:
    attributes = root / ".gitattributes"
    if not attributes.is_file():
        return set()
    rules = set()
    for line in attributes.read_text(encoding="utf-8").splitlines():
        fields = line.split()
        if len(fields) >= 2 and not line.startswith("#") and "binary" in fields[1:]:
            rules.add(fields[0].removeprefix("/").removesuffix("/**"))
    return rules


def _binary_attribute_problems(root: Path, manifest: Manifest) -> list[str]:
    rules = _binary_rules(root)
    preserved = set(manifest.preserved_paths())
    return [
        f'{path}: bytes = "preserve" but .gitattributes has no binary rule for it'
        for path in sorted(preserved - rules)
    ] + [
        f"{path}: .gitattributes marks it binary but {MANIFEST} does not preserve it"
        for path in sorted(rules - preserved)
    ]


def _version_problems(root: Path, manifest: Manifest) -> list[str]:
    project_version = (
        (root / "Version.txt").read_bytes() if (root / "Version.txt").is_file() else b""
    )
    problems = []
    for pack in manifest.packs:
        version_file = root / pack.path / "Version.txt"
        if pack.path == "Version.txt" or not version_file.is_file():
            continue
        actual = version_file.read_bytes()
        if not pack.preserved and pack.is_root_entry and actual != project_version:
            problems.append(
                f"{pack.path}/Version.txt: does not carry the project version"
            )
        if pack.preserved and pack.upstream_version is not None:
            if actual != pack.upstream_version.encode("latin-1"):
                problems.append(
                    f"{pack.path}/Version.txt: does not match upstream_version "
                    f"{pack.upstream_version!r}"
                )
    return problems


def _nested_definition_problems(root: Path, manifest: Manifest) -> list[str]:
    return [
        f"{Path(nested).name}: nested definition {nested} is duplicated at the data root"
        for nested in manifest.nested_definitions()
        if (root / Path(nested).name).exists()
    ]


QUERIES = {
    "roots": Manifest.root_entries,
    "own-version": Manifest.own_version_packs,
    "preserved": Manifest.preserved_paths,
    "preserved-roots": Manifest.preserved_roots,
    "nested-definitions": Manifest.nested_definitions,
}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="repository root (default: the checkout this script lives in)",
    )
    parser.add_argument(
        "query", choices=[*QUERIES, "pending", "check", "archive-input-roots"]
    )
    arguments = parser.parse_args(argv)

    if arguments.query == "check":
        problems = check(arguments.root)
        for problem in problems:
            print(f"  {problem}", file=sys.stderr)
        if problems:
            print(f"\n{len(problems)} pack manifest problem(s)", file=sys.stderr)
            return 1
        print(
            f"ok {len(Manifest.load(arguments.root).packs)} packs described by {MANIFEST}"
        )
        return 0

    manifest = Manifest.load(arguments.root)
    if manifest.problems:
        for problem in manifest.problems:
            print(f"  {problem}", file=sys.stderr)
        return 1
    if arguments.query == "pending":
        for path, references in manifest.localization_pending().items():
            print(f"{path}\t{' '.join(references)}")
        return 0
    if arguments.query == "archive-input-roots":
        document = tomllib.loads(
            (arguments.root / MANIFEST).read_text(encoding="utf-8")
        )
        paths = manifest.root_entries()
        notice = document.get("distribution", {}).get("notice")
        if notice:
            paths.append(notice)
        for fields in document["packs"].values():
            paths.extend(fields.get("evidence", []))
        print("\n".join(sorted({path.split("/", 1)[0] for path in paths})))
        return 0
    for path in QUERIES[arguments.query](manifest):
        print(path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
