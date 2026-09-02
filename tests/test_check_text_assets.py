#!/usr/bin/env python3
"""Integration tests for the changed-text encoding audit."""

from __future__ import annotations

from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


CHECKER_SOURCE = Path(__file__).with_name("check_text_assets.py")


class TextAssetCheckerTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary_directory = tempfile.TemporaryDirectory()
        self.addCleanup(temporary_directory.cleanup)

        self.repository = Path(temporary_directory.name) / "repository"
        self.repository.mkdir()
        subprocess.run(
            ["git", "init", "--quiet", str(self.repository)],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        subprocess.run(
            ["git", "config", "user.email", "test@example.invalid"],
            cwd=self.repository,
            check=True,
        )
        subprocess.run(
            ["git", "config", "user.name", "Text Asset Test"],
            cwd=self.repository,
            check=True,
        )

        checker = self.repository / "tests/check_text_assets.py"
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

    def commit(self, subject: str) -> None:
        subprocess.run(["git", "add", "."], cwd=self.repository, check=True)
        subprocess.run(
            ["git", "commit", "--quiet", "-m", subject],
            cwd=self.repository,
            check=True,
        )

    def test_changed_binary_import_is_checked_for_encoding_drift(self) -> None:
        path = "ClonkMars.c4d/Object.c4d/DescDE.txt"
        self.write(".gitattributes", "/ClonkMars.c4d/** binary\n")
        self.write(path, b"Gr\xfc\xdfe")
        self.commit("base")

        self.write(path, "Grüße")
        self.commit("change")
        result = subprocess.run(
            [sys.executable, str(self.checker), "HEAD^"],
            cwd=self.repository,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding="utf-8",
            errors="replace",
        )

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("now UTF-8", result.stderr)


if __name__ == "__main__":
    unittest.main()
