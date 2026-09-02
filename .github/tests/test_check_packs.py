#!/usr/bin/env python3
"""Tests for the pack manifest reader and its checks against a fixture tree."""

from __future__ import annotations

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import packs  # noqa: E402


def manifest(*entries: str) -> str:
    return "\n".join(
        [
            "[origins.clonk-rs]",
            'description = "own"',
            "[origins.legacyclonk]",
            'terms = "CC BY-NC"',
            "[origins.ucc]",
            'terms = "authors"',
            "",
            *entries,
        ]
    )


class PackManifestTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary_directory = tempfile.TemporaryDirectory()
        self.addCleanup(temporary_directory.cleanup)
        self.root = Path(temporary_directory.name)
        subprocess.run(["git", "init", "--quiet", str(self.root)], check=True)
        self.write("Version.txt", "4.9.11.1\r\n")
        self.write("Objects.c4d/Version.txt", "4.9.11.1\r\n")
        self.write("Objects.c4d/DefCore.txt", "objects")

    def write(self, relative_path: str, contents: str | bytes) -> None:
        path = self.root / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        if isinstance(contents, bytes):
            path.write_bytes(contents)
        else:
            path.write_bytes(contents.encode("latin-1"))
        subprocess.run(["git", "-C", str(self.root), "add", relative_path], check=True)

    def problems(self) -> list[str]:
        return packs.check(self.root)

    def assert_problem(self, *fragments: str) -> None:
        problems = self.problems()
        joined = "\n".join(problems).casefold()
        self.assertTrue(problems, "expected a problem, found none")
        for fragment in fragments:
            self.assertIn(fragment.casefold(), joined)

    def base_manifest(self) -> None:
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
            ),
        )

    def test_a_consistent_tree_has_no_problems(self) -> None:
        self.base_manifest()

        self.assertEqual(self.problems(), [])

    def test_a_missing_manifest_is_a_problem(self) -> None:
        self.assert_problem("packs.toml", "missing")

    def test_a_tracked_root_pack_must_be_listed(self) -> None:
        self.base_manifest()
        self.write("Stray.c4d/DefCore.txt", "stray")

        self.assert_problem("Stray.c4d", "not listed")

    def test_a_listed_entry_must_exist(self) -> None:
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                '[packs."Ghost.c4d"]',
                'origin = "legacyclonk"',
            ),
        )

        self.assert_problem("Ghost.c4d", "does not exist")

    def test_an_origin_must_be_declared(self) -> None:
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "nowhere"',
            ),
        )

        self.assert_problem("Objects.c4d", "origin", "nowhere")

    def test_a_preserved_entry_needs_its_binary_attribute(self) -> None:
        self.write("Import.c4d/Version.txt", "1.0")
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                '[packs."Import.c4d"]',
                'origin = "ucc"',
                'bytes = "preserve"',
            ),
        )

        self.assert_problem("Import.c4d", "binary")

    def test_a_binary_attribute_needs_a_preserved_entry(self) -> None:
        self.base_manifest()
        self.write(".gitattributes", "/Objects.c4d/** binary\n")

        self.assert_problem("Objects.c4d", "binary", "preserve")

    def test_a_normalized_pack_carries_the_project_version(self) -> None:
        self.base_manifest()
        self.write("Objects.c4d/Version.txt", "4.9.10.0\r\n")

        self.assert_problem("Objects.c4d", "Version.txt", "project version")

    def test_a_preserved_pack_keeps_its_upstream_version(self) -> None:
        self.write("Import.c4d/Version.txt", "1.0")
        self.write(".gitattributes", "/Import.c4d/** binary\n")
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                '[packs."Import.c4d"]',
                'origin = "ucc"',
                'bytes = "preserve"',
                'upstream_version = "2.0"',
            ),
        )

        self.assert_problem("Import.c4d", "upstream_version", "2.0")

    def test_localization_issues_must_be_qualified(self) -> None:
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                'localization = ["#78"]',
            ),
        )

        self.assert_problem("Objects.c4d", "clonk-org/clonk-rs-content#N")

    def test_nested_definitions_live_beside_their_scenarios(self) -> None:
        self.write("Import.c4f/Fun.c4f/Res.c4d", b"packed")
        self.write("Res.c4d", b"duplicate")
        self.write(
            ".gitattributes",
            "/Import.c4f/** binary\n/Res.c4d binary\n",
        )
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                '[packs."Import.c4f"]',
                'origin = "ucc"',
                'bytes = "preserve"',
                'nested_definitions = ["Fun.c4f/Res.c4d"]',
                '[packs."Res.c4d"]',
                'origin = "ucc"',
                'bytes = "preserve"',
            ),
        )

        self.assert_problem("Res.c4d", "nested", "data root")

    def test_queries_answer_from_the_manifest(self) -> None:
        self.write("Import.c4f/Fun.c4f/Res.c4d", b"packed")
        self.write("Import.c4f/Version.txt", "2.0")
        self.write("Base.c4f/Queron.c4s/Script.c", "script")
        self.write("Base.c4f/Version.txt", "4.9.11.1\r\n")
        self.write(
            ".gitattributes",
            "/Import.c4f/** binary\n/Import.c4f/Fun.c4f/Res.c4d binary\n"
            "/Base.c4f/Queron.c4s/** binary\n",
        )
        self.write(
            ".github/packs.toml",
            manifest(
                '[packs."Version.txt"]',
                'origin = "clonk-rs"',
                '[packs."Objects.c4d"]',
                'origin = "legacyclonk"',
                '[packs."Base.c4f"]',
                'origin = "legacyclonk"',
                '[packs."Base.c4f/Queron.c4s"]',
                'origin = "ucc"',
                'bytes = "preserve"',
                'localization = ["clonk-org/clonk-rs-content#68"]',
                '[packs."Import.c4f"]',
                'origin = "ucc"',
                'bytes = "preserve"',
                'nested_definitions = ["Fun.c4f/Res.c4d"]',
            ),
        )
        loaded = packs.Manifest.load(self.root)

        self.assertEqual(self.problems(), [])
        self.assertEqual(
            loaded.root_entries(),
            ["Version.txt", "Objects.c4d", "Base.c4f", "Import.c4f"],
        )
        self.assertEqual(loaded.own_version_packs(), ["Import.c4f"])
        self.assertEqual(
            loaded.preserved_paths(),
            ["Base.c4f/Queron.c4s", "Import.c4f", "Import.c4f/Fun.c4f/Res.c4d"],
        )
        self.assertEqual(loaded.preserved_roots(), ["Base.c4f/Queron.c4s", "Import.c4f"])
        self.assertEqual(loaded.nested_definitions(), ["Import.c4f/Fun.c4f/Res.c4d"])
        self.assertEqual(
            loaded.localization_pending(),
            {"Base.c4f/Queron.c4s": ("clonk-org/clonk-rs-content#68",)},
        )

    def test_the_command_line_prints_one_path_per_line(self) -> None:
        self.base_manifest()

        result = subprocess.run(
            [sys.executable, str(packs.__file__), "--root", str(self.root), "roots"],
            check=True,
            stdout=subprocess.PIPE,
            text=True,
        )

        self.assertEqual(result.stdout, "Version.txt\nObjects.c4d\n")

    def test_the_command_line_check_fails_on_a_problem(self) -> None:
        result = subprocess.run(
            [sys.executable, str(packs.__file__), "--root", str(self.root), "check"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("packs.toml", result.stderr)


if __name__ == "__main__":
    unittest.main()
