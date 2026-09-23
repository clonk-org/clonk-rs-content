#!/usr/bin/env python3
"""Every scenario names its definition modules by their path from the data root.

`C4GameResList::Load` opens each `[Definitions]` name with `C4Group::Open`,
which resolves a relative path against the working directory only, and a name
that does not resolve fails the start with IDS_PRC_DEFNOTFOUND
(C4GameParameters.cpp:199-207, C4Group.cpp:660-677). clonk-rs additionally
searches a scenario's enclosing folders, so a name that only resolves there
still starts in clonk-rs but not in the C++ engine (clonk-org/clonk-rs-content#123).
"""

from __future__ import annotations

import os
from pathlib import Path
import re
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import group_entry  # noqa: E402

DEFINITIONS = re.compile(r"\[Definitions\](.*?)(?:\r?\n\[|\Z)", re.S)
MODULE = re.compile(r"^Definition\d+=(.*?)\r?$", re.M)


def child(directory: Path, name: str) -> Path | None:
    """`name` below `directory`, matched without regard to case as on the reference platform."""
    try:
        names = {entry.lower(): entry for entry in os.listdir(directory)}
    except (NotADirectoryError, FileNotFoundError):
        return None
    real = names.get(name.lower())
    return directory / real if real else None


def group_contains(image: bytes, parts: list[str]) -> bool:
    entries, start = group_entry.records(image)
    for entry in entries:
        if entry["name"].decode("latin-1").lower() != parts[0].lower():
            continue
        if len(parts) == 1:
            return True
        if not entry["child"]:
            return False
        begin = start + entry["offset"]
        return group_contains(image[begin : begin + entry["size"]], parts[1:])
    return False


def resolves(relative: str) -> bool:
    parts = [part for part in relative.replace("\\", "/").split("/") if part]
    current = ROOT
    for index, part in enumerate(parts):
        found = child(current, part)
        if found is None:
            return False
        if found.is_file() and index + 1 < len(parts):
            return group_contains(group_entry.unpack(found.read_bytes()), parts[index + 1 :])
        current = found
    return True


def scenario_text(scenario: Path) -> str | None:
    if scenario.is_dir():
        path = child(scenario, "Scenario.txt")
        return path.read_bytes().decode("latin-1") if path else None
    image = group_entry.unpack(scenario.read_bytes())
    entries, start = group_entry.records(image)
    for entry in entries:
        if entry["name"].decode("latin-1").lower() == "scenario.txt":
            begin = start + entry["offset"]
            return image[begin : begin + entry["size"]].decode("latin-1")
    return None


def scenarios():
    for dirpath, dirnames, filenames in os.walk(ROOT):
        dirnames[:] = [d for d in dirnames if not d.startswith(".")]
        for name in dirnames + filenames:
            if name.lower().endswith(".c4s"):
                yield Path(dirpath) / name
        dirnames[:] = [d for d in dirnames if not d.lower().endswith(".c4s")]


class ScenarioDefinitionTests(unittest.TestCase):
    def test_every_definition_module_resolves_from_the_data_root(self) -> None:
        unresolved = []
        for scenario in sorted(scenarios()):
            text = scenario_text(scenario)
            section = DEFINITIONS.search(text or "")
            if not section:
                continue
            for module in MODULE.findall(section.group(1)):
                if module.strip() and not resolves(module.strip()):
                    unresolved.append(f"{scenario.relative_to(ROOT)}: {module.strip()}")
        self.assertEqual(unresolved, [])


if __name__ == "__main__":
    unittest.main()
