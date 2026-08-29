#!/usr/bin/env python3
"""Integration tests for the maintained-content localization audit."""

from __future__ import annotations

from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


CHECKER_SOURCE = Path(__file__).with_name("check_localizations.py")


class LocalizationCheckerTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary_directory = tempfile.TemporaryDirectory()
        self.addCleanup(temporary_directory.cleanup)

        self.temporary_root = Path(temporary_directory.name)
        self.repository = self.temporary_root / "repository"
        self.outside_directory = self.temporary_root / "outside"
        self.repository.mkdir()
        self.outside_directory.mkdir()

        subprocess.run(
            ["git", "init", "--quiet", str(self.repository)],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )

        checker = self.repository / ".github/tests/check_localizations.py"
        checker.parent.mkdir(parents=True)
        shutil.copyfile(CHECKER_SOURCE, checker)
        self.checker = checker

    def write(self, relative_path: str, contents: bytes | str) -> None:
        path = self.repository / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        if isinstance(contents, bytes):
            path.write_bytes(contents)
        else:
            path.write_text(contents, encoding="utf-8")

    def run_checker(self, *, outside_repository: bool = False):
        working_directory = (
            self.outside_directory if outside_repository else self.repository
        )
        return subprocess.run(
            [sys.executable, str(self.checker)],
            cwd=working_directory,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding="utf-8",
            errors="replace",
        )

    def assert_checker_passes(self, result) -> None:
        self.assertEqual(
            result.returncode,
            0,
            msg=f"checker failed:\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}",
        )

    def assert_checker_fails(self, result, *diagnostic_fragments: str) -> None:
        self.assertNotEqual(
            result.returncode,
            0,
            msg=f"checker unexpectedly passed:\n{result.stdout}",
        )
        output = f"{result.stdout}\n{result.stderr}".casefold()
        for fragment in diagnostic_fragments:
            self.assertIn(fragment.casefold(), output)

    def test_invocation_outside_repository_still_audits_repository(self) -> None:
        self.write("Maintained.c4d/Object.c4d/DescDE.txt", "Beschreibung")

        result = self.run_checker(outside_repository=True)

        self.assert_checker_fails(result, "DescUS.txt", "missing English counterpart")

    def test_maintained_missing_description_fails(self) -> None:
        self.write("Maintained.c4d/Object.c4d/DescDE.txt", "Beschreibung")

        result = self.run_checker()

        self.assert_checker_fails(result, "DescUS.txt", "missing English counterpart")

    def test_pending_import_missing_description_is_skipped(self) -> None:
        self.write(".gitattributes", "/ClonkMars.c4d/** binary\n")
        self.write(
            ".github/localization-pending.txt",
            "ClonkMars.c4d\tclonk-org/clonk-rs-content#78\n",
        )
        self.write("ClonkMars.c4d/Object.c4d/DescDE.txt", "Beschreibung")

        result = self.run_checker()

        self.assert_checker_passes(result)

    def test_binary_import_without_pending_entry_is_audited(self) -> None:
        self.write(".gitattributes", "/ClonkMars.c4d/** binary\n")
        self.write("ClonkMars.c4d/Object.c4d/DescDE.txt", "Beschreibung")

        result = self.run_checker()

        self.assert_checker_fails(result, "DescUS.txt", "missing English counterpart")

    def test_pending_scope_without_binary_attribute_is_skipped(self) -> None:
        self.write(
            ".github/localization-pending.txt",
            "Maintained.c4d\tclonk-org/clonk-rs-content#78\n",
        )
        self.write("Maintained.c4d/Object.c4d/DescDE.txt", "Beschreibung")

        result = self.run_checker()

        self.assert_checker_passes(result)

    def test_pending_entry_requires_qualified_issue(self) -> None:
        self.write(
            ".github/localization-pending.txt",
            "Maintained.c4d\t#78\n",
        )
        self.write("Maintained.c4d/Object.c4d/DescUS.txt", "Description")

        result = self.run_checker()

        self.assert_checker_fails(
            result, "localization-pending.txt", "use qualified", "#N"
        )

    def test_approved_binary_root_must_retain_its_attribute(self) -> None:
        self.write("ClonkMars.c4d/Object.c4d/DescDE.txt", "Beschreibung")
        self.write("ClonkMars.c4d/Object.c4d/DescUS.txt", "Description")

        result = self.run_checker()

        self.assert_checker_fails(result, "ClonkMars.c4d", "missing", "binary")

    def test_unapproved_binary_classification_fails(self) -> None:
        self.write(".gitattributes", "/Maintained.c4d/** binary\n")
        self.write("Maintained.c4d/Object.c4d/DescDE.txt", "Beschreibung")
        self.write("Maintained.c4d/Object.c4d/DescUS.txt", "Description")

        result = self.run_checker()

        self.assert_checker_fails(result, "Maintained.c4d", "binary", "approved")

    def test_whitespace_only_english_counterpart_fails(self) -> None:
        self.write("Maintained.c4d/Object.c4d/DescDE.txt", "Beschreibung")
        self.write("Maintained.c4d/Object.c4d/DescUS.txt", " \t\r\n")

        result = self.run_checker()

        self.assert_checker_fails(result, "DescUS.txt", "empty")

    def test_string_table_missing_english_key_fails(self) -> None:
        self.write(
            "Maintained.c4d/StringTblDE.txt",
            "Greeting=Hallo\nFarewell=Auf Wiedersehen\n",
        )
        self.write("Maintained.c4d/StringTblUS.txt", "Greeting=Hello\n")

        result = self.run_checker()

        self.assert_checker_fails(result, "StringTblUS.txt", "missing English key Farewell")

    def test_string_table_empty_english_value_fails(self) -> None:
        self.write("Maintained.c4d/StringTblDE.txt", "Greeting=Hallo\n")
        self.write("Maintained.c4d/StringTblUS.txt", "Greeting= \t\n")

        result = self.run_checker()

        self.assert_checker_fails(result, "StringTblUS.txt", "empty English value for Greeting")

    def test_string_table_placeholder_mismatch_fails(self) -> None:
        self.write(
            "Maintained.c4d/StringTblDE.txt",
            "Status=%s hat %02d {{CLNK}}\n",
        )
        self.write(
            "Maintained.c4d/StringTblUS.txt",
            "Status=%s has %02d {{WIPF}}\n",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "StringTblUS.txt", "substitution tokens for Status")

    def test_string_table_placeholder_width_and_case_are_exact(self) -> None:
        self.write(
            "Maintained.c4d/StringTblDE.txt",
            "Count=%02d\nColor=%X\n",
        )
        self.write(
            "Maintained.c4d/StringTblUS.txt",
            "Count=%d\nColor=%x\n",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "substitution tokens for Count", "Color")

    def test_literal_percentage_after_placeholder_is_not_a_directive(self) -> None:
        self.write(
            "Maintained.c4d/StringTblDE.txt",
            "Progress=%d% des Weges\n",
        )
        self.write(
            "Maintained.c4d/StringTblUS.txt",
            "Progress=%d% of the way\n",
        )

        result = self.run_checker()

        self.assert_checker_passes(result)

    def test_english_only_string_key_is_allowed(self) -> None:
        self.write("Maintained.c4d/StringTblDE.txt", "Greeting=Hallo\n")
        self.write(
            "Maintained.c4d/StringTblUS.txt",
            "Greeting=Hello\nExtra=English-only fallback\n",
        )

        result = self.run_checker()

        self.assert_checker_passes(result)

    def test_latin1_nul_and_unusual_path_names_are_safe(self) -> None:
        directory = "Päck mit Leerzeichen.c4d/Objekt\nmit Umbruch.c4d"
        self.write(f"{directory}/DescDE.txt", b"Gr\xfc\xdfe\x00\r\n")
        self.write(f"{directory}/DescUS.txt", b"Large\x00\r\n")

        result = self.run_checker()

        self.assert_checker_passes(result)

    def test_duplicate_german_string_key_fails(self) -> None:
        self.write(
            "Maintained.c4d/StringTblDE.txt",
            "Greeting=Hallo\nGreeting=Guten Tag\n",
        )
        self.write("Maintained.c4d/StringTblUS.txt", "Greeting=Hello\n")

        result = self.run_checker()

        self.assert_checker_fails(result, "StringTblDE.txt", "duplicate", "Greeting")

    def test_duplicate_english_string_key_fails(self) -> None:
        self.write("Maintained.c4d/StringTblDE.txt", "Greeting=Hallo\n")
        self.write(
            "Maintained.c4d/StringTblUS.txt",
            "Greeting=Hello\nGreeting=Hi\n",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "StringTblUS.txt", "duplicate", "Greeting")

    def test_duplicate_metadata_locale_fails(self) -> None:
        self.write(
            "Maintained.c4d/Object.c4d/Names.txt",
            "DE:Objekt\nUS:Object\nUS:Item\n",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "Names.txt", "duplicate", "US")

    def test_malformed_metadata_line_fails(self) -> None:
        self.write(
            "Maintained.c4d/Object.c4d/Names.txt",
            "DE:Objekt\nUS:Object\nthis line has no locale tag\n",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "Names.txt", "malformed")

    def test_localization_asset_names_use_canonical_case(self) -> None:
        self.write("Maintained.c4d/StringtblDE.txt", "Greeting=Hallo\n")
        self.write("Maintained.c4d/StringTblUS.txt", "Greeting=Hello\n")

        result = self.run_checker()

        self.assert_checker_fails(result, "StringtblDE.txt", "canonical")

    def test_rank_english_text_is_checked_for_german_residue(self) -> None:
        self.write("Maintained.c4d/RankDE.txt", "Dekoration")
        self.write("Maintained.c4d/RankUS.txt", "Dekoration")

        result = self.run_checker()

        self.assert_checker_fails(result, "RankUS.txt", "German word")

    def test_rtf_escapes_do_not_hide_german_residue(self) -> None:
        self.write("Maintained.c4d/DescDE.rtf", b"{\\rtf1 Beschreibung}")
        self.write(
            "Maintained.c4d/DescUS.rtf",
            b"{\\rtf1 Keine Objekte verf\\'fcgbar}",
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "DescUS.rtf", "German word")

    def test_multiline_spaced_player_text_call_is_checked(self) -> None:
        self.write("Maintained.c4d/DescDE.txt", "Beschreibung")
        self.write("Maintained.c4d/DescUS.txt", "Description")
        self.write(
            "Maintained.c4d/Script.c",
            'Message (\n  "Keine \\"Objekte\\" gefunden",\n  this()\n);\n',
        )

        result = self.run_checker()

        self.assert_checker_fails(result, "Script.c", "hard-coded German player text")

    def test_nonvisible_call_arguments_do_not_trigger_player_text_check(self) -> None:
        self.write("Maintained.c4d/DescDE.txt", "Beschreibung")
        self.write("Maintained.c4d/DescUS.txt", "Description")
        self.write(
            "Maintained.c4d/Script.c",
            'AddMenuItem("$Label$", "Call(\\"Keine Objekte gefunden\\")", '
            'ICON, this(), 0, 0, "$Description$");\n',
        )

        result = self.run_checker()

        self.assert_checker_passes(result)


if __name__ == "__main__":
    unittest.main()
