#!/usr/bin/env python3
"""Bind local rights decisions and definition dependencies to resource bytes."""

from __future__ import annotations
import re
import hashlib
import argparse
import json
from pathlib import Path
import subprocess
import sys
import tomllib

REPORT = "third_party/rights-inventory.json"
KNOWN_MISSING = "third_party/known-missing-definitions.json"
BEGIN = "# BEGIN INVENTORY DEPENDENCIES"
END = "# END INVENTORY DEPENDENCIES"


def scope_snapshots(root, scopes, files):
    result = {
        path: {
            key: value
            for key, value in scope.items()
            if key in ("rights", "license", "reason", "evidence", "origin")
        }
        for path, scope in scopes.items()
    }
    digests = {path: hashlib.sha256() for path in scopes}
    for record in result.values():
        record.update(file_count=0, bytes=0)
    for path in sorted(files):
        scope = scope_for(path, scopes)
        if scope is None:
            continue
        with (root / path).open("rb") as stream:
            digest = hashlib.file_digest(stream, "sha256").digest()
        digests[scope].update(path.encode("utf-8") + b"\0" + digest)
        result[scope]["file_count"] += 1
        result[scope]["bytes"] += (root / path).stat().st_size
    for path, digest in digests.items():
        result[path]["sha256"] = digest.hexdigest()
    return result


def scope_for(path, scopes):
    folded = path.casefold()
    return max(
        (
            scope
            for scope in scopes
            if folded == scope.casefold() or folded.startswith(scope.casefold() + "/")
        ),
        key=len,
        default=None,
    )


def resolve_dependencies(rows: list[dict]) -> dict:
    groups = {row["path"].casefold(): row["path"] for row in rows}
    dependencies = {}
    for row in rows:
        if not row["path"].casefold().endswith(".c4s"):
            continue
        text = next(
            (
                text
                for name, text in row["texts"].items()
                if name.casefold() == "scenario.txt"
            ),
            "",
        )
        required = []
        for reference in re.findall(
            r"^\s*Definition\d+\s*=\s*([^\r\n]*)", text, re.M | re.I
        ):
            reference = reference.strip().replace("\\", "/")
            if not reference:
                continue
            parents = row["path"].split("/")
            candidates = [
                "/".join(parents[:index] + [reference])
                for index in range(len(parents), -1, -1)
            ]
            resolved = next(
                (
                    groups[path.casefold()]
                    for path in candidates
                    if path.casefold() in groups
                ),
                None,
            )
            required.append({"reference": reference, "resolved": resolved})
        dependencies[row["path"]] = required
    return dependencies


def physical_path(root, path):
    parts = path.split("/")
    for length in range(1, len(parts) + 1):
        candidate = "/".join(parts[:length])
        if (root / candidate).is_file():
            return candidate
    if (root / path).is_dir():
        return path
    raise ValueError(f"no physical content for {path}")


def physical_dependencies(root, references):
    result = {}
    for scenario, dependencies in references.items():
        consumer = physical_path(root, scenario)
        required = result.setdefault(consumer, set())
        required.update(
            physical_path(root, item["resolved"])
            for item in dependencies
            if item["resolved"]
        )
    return {
        path: sorted(required - {path})
        for path, required in sorted(result.items())
        if required - {path}
    }


def repository_files(root):
    output = subprocess.check_output(
        [
            "git",
            "-C",
            str(root),
            "ls-files",
            "--cached",
            "--others",
            "--exclude-standard",
            "-z",
        ]
    )
    return sorted(
        {
            raw.decode()
            for raw in output.split(b"\0")
            if raw
            and (root / raw.decode()).is_file()
            and not (root / raw.decode()).is_symlink()
        }
    )


def write_json(path, value):
    path.write_text(
        json.dumps(value, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )


def missing_references(references):
    return [
        {"scenario": path, "reference": item["reference"]}
        for path, items in sorted(references.items())
        for item in items
        if item["resolved"] is None
    ]


def record(root, scanner):
    manifest_path = root / "packs.toml"
    manifest = tomllib.loads(manifest_path.read_text(encoding="utf-8"))
    scopes = manifest["packs"]
    before = scope_snapshots(root, scopes, repository_files(root))
    # Run the reader ourselves so a stale exported scan cannot bless new bytes.
    raw = subprocess.check_output([str(Path(scanner).resolve()), str(root)])
    rows = json.loads(raw)
    after = scope_snapshots(root, scopes, repository_files(root))
    if before != after:
        raise ValueError("resource bytes changed during the scan; retry")
    unscoped = [row["path"] for row in rows if scope_for(row["path"], scopes) is None]
    if unscoped:
        raise ValueError(f"groups lack a rights decision: {unscoped}")
    references = resolve_dependencies(rows)
    missing = missing_references(references)
    known = json.loads((root / KNOWN_MISSING).read_text(encoding="utf-8"))["references"]
    new = [item for item in missing if item not in known]
    if new:
        raise ValueError(
            f"new unresolved definitions need an explicit record in {KNOWN_MISSING}: {new}"
        )
    dependencies = physical_dependencies(root, references)
    notices = [
        {
            "path": row["path"] + "/" + name,
            "decoded_text_sha256": hashlib.sha256(text.encode("utf-8")).hexdigest(),
        }
        for row in rows
        for name, text in sorted(row["texts"].items())
        if name.casefold() not in ("scenario.txt", "folder.txt", "defcore.txt")
    ]
    report = {
        "schema": 1,
        "baseline_content_commit": "9a01c8f55f0fbdccfa2dcf3a67e3cfcfcac7c009",
        "reader_baseline_engine_commit": "e3d35afcff3411d9c7d8152686ebec87faafd9c7",
        "reader_sha256": hashlib.sha256(Path(scanner).read_bytes()).hexdigest(),
        "scopes": after,
        "groups": [row["path"] for row in rows],
        "notices": notices,
        "scenarios": references,
        "dependencies": dependencies,
        "unresolved": missing,
    }
    block = (
        BEGIN
        + "\n[distribution.dependencies]\n"
        + "".join(
            f"{json.dumps(path)} = {json.dumps(required)}\n"
            for path, required in dependencies.items()
        )
        + END
    )
    text = manifest_path.read_text(encoding="utf-8")
    text = (
        re.sub(
            re.escape(BEGIN) + r".*?" + re.escape(END),
            lambda _: block,
            text,
            flags=re.S,
        )
        if BEGIN in text
        else text.rstrip() + "\n\n" + block + "\n"
    )
    manifest_path.write_text(text, encoding="utf-8")
    write_json(root / REPORT, report)
    print(
        f"recorded {len(scopes)} scopes, {len(rows)} groups, {len(references)} scenarios, {len(notices)} notices; {len(missing)} known unresolved references"
    )


def check(root):
    manifest = tomllib.loads((root / "packs.toml").read_text(encoding="utf-8"))
    report = json.loads((root / REPORT).read_text(encoding="utf-8"))
    current = scope_snapshots(root, manifest["packs"], repository_files(root))
    if report.get("schema") != 1 or current != report["scopes"]:
        raise ValueError(
            "resource bytes or rights decisions changed; refresh the inventory with a fresh scan"
        )
    if manifest["distribution"].get("dependencies", {}) != report["dependencies"]:
        raise ValueError("packaging dependencies differ from the recorded inventory")
    print(
        f"ok {len(current)} rights scopes match resource bytes and packaging dependencies"
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--root", type=Path, default=Path(__file__).resolve().parents[1]
    )
    commands = parser.add_subparsers(dest="command", required=True)
    commands.add_parser("record").add_argument(
        "--scanner", required=True, help="clonk-resources rights_inventory executable"
    )
    commands.add_parser("check")
    args = parser.parse_args()
    try:
        if args.command == "record":
            record(args.root.resolve(), args.scanner)
        else:
            check(args.root.resolve())
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        print(error, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
