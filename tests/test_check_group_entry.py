#!/usr/bin/env python3
"""Tests for the in-place packed-group entry editor, on synthetic groups."""

from __future__ import annotations

import contextlib
import io
from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

import group_entry  # noqa: E402

MEMBER_HEADER = b"\x1e\x8c\x08\x00\x00\x00\x00\x00\x00\x0b"


def image(entries: list[tuple[str, object]], crc_state: int = 2) -> bytes:
    """A group image. An entry's payload is bytes, or a list for a child group."""
    header = bytearray(group_entry.HEADER)
    header[:25] = b"RedWolf Design GrpFolder\0"
    struct.pack_into("<i", header, 36, len(entries))
    table, data = bytearray(), bytearray()
    for name, payload in entries:
        child = isinstance(payload, list)
        body = image(payload, crc_state) if child else payload
        encoded = name.encode("latin-1")
        if crc_state != 2:
            crc = 0
        elif child:
            crc = group_entry.contents_crc(group_entry.records(body)[0])
        else:
            crc = group_entry.file_crc(body, encoded)
        record = bytearray(group_entry.RECORD)
        record[: len(encoded)] = encoded
        struct.pack_into("<4i", record, 264, int(child), len(body), 0, len(data))
        struct.pack_into("<i", record, 280, 1_400_000_000)
        record[284] = crc_state
        struct.pack_into("<I", record, 285, crc)
        table += record
        data += body
    # The scramble is its own inverse.
    return group_entry.unscramble(bytes(header)) + bytes(table) + bytes(data)


def nested(crc_state: int = 2) -> bytes:
    return image(
        [
            ("Scenario.txt", b"[Head]\r\n"),
            (
                "Helper.c4d",
                [
                    ("Names.txt", b"DE:Helfer\r\n"),
                    ("Rule.c4d", [("DefCore.txt", b"id=RULE\r\n"), ("Script.c", b"a();\r\nb();\r\n")]),
                    ("After.txt", b"after"),
                ],
            ),
            ("Title.txt", b"US:Title\r\n"),
        ],
        crc_state,
    )


class GroupEntryTests(unittest.TestCase):
    def test_a_nested_replacement_touches_only_the_path_to_the_entry(self) -> None:
        original = nested()
        edited = group_entry.edit_entry(
            original,
            ["Helper.c4d", "Rule.c4d", "Script.c"],
            group_entry.replacing(b"b();\r\n", b"if (c) b();\r\n"),
        )

        self.assertEqual(
            group_entry.compare(original, edited, "g"),
            [
                "g/Helper.c4d: record differs in ['size', 'crc']",
                "g/Helper.c4d/Rule.c4d: record differs in ['size', 'crc']",
                "g/Helper.c4d/Rule.c4d/Script.c: record differs in ['size', 'crc']",
                "g/Helper.c4d/Rule.c4d/Script.c: data differs, 12 -> 19 bytes",
            ],
        )
        # The entries after the resized one still read back whole.
        self.assertEqual(group_entry.read_entry(edited, ["Helper.c4d", "After.txt"]), b"after")
        self.assertEqual(group_entry.read_entry(edited, ["Title.txt"]), b"US:Title\r\n")
        self.assertEqual(
            group_entry.read_entry(edited, ["Helper.c4d", "Rule.c4d", "Script.c"]),
            b"a();\r\nif (c) b();\r\n",
        )

    def test_the_rewritten_crcs_are_the_ones_a_fresh_build_computes(self) -> None:
        edited = group_entry.edit_entry(
            nested(),
            ["Helper.c4d", "Rule.c4d", "Script.c"],
            group_entry.replacing(b"b();\r\n", b"if (c) b();\r\n"),
        )
        rebuilt = image(
            [
                ("Scenario.txt", b"[Head]\r\n"),
                (
                    "Helper.c4d",
                    [
                        ("Names.txt", b"DE:Helfer\r\n"),
                        (
                            "Rule.c4d",
                            [("DefCore.txt", b"id=RULE\r\n"), ("Script.c", b"a();\r\nif (c) b();\r\n")],
                        ),
                        ("After.txt", b"after"),
                    ],
                ),
                ("Title.txt", b"US:Title\r\n"),
            ]
        )
        self.assertEqual(edited, rebuilt)

    def test_a_rename_keeps_the_data_and_recomputes_the_name_bound_crc(self) -> None:
        original = nested()
        edited = group_entry.edit_entry(
            original, ["Helper.c4d", "Rule.c4d", "Script.c"], group_entry.renaming("ScriptUS.c")
        )

        self.assertEqual(
            group_entry.compare(original, edited, "g"),
            [
                "g/Helper.c4d: record differs in ['crc']",
                "g/Helper.c4d/Rule.c4d: record differs in ['crc']",
                "g/Helper.c4d/Rule.c4d: entry Script.c is now ScriptUS.c",
                "g/Helper.c4d/Rule.c4d/ScriptUS.c: record differs in ['crc']",
            ],
        )
        self.assertEqual(
            group_entry.read_entry(edited, ["Helper.c4d", "Rule.c4d", "scriptus.c"]),
            b"a();\r\nb();\r\n",
        )

    def test_records_without_a_crc_are_left_without_one(self) -> None:
        original = nested(crc_state=0)
        edited = group_entry.edit_entry(
            original, ["Helper.c4d", "Rule.c4d", "Script.c"], group_entry.renaming("ScriptDE.c")
        )

        self.assertEqual(
            group_entry.compare(original, edited, "g"),
            ["g/Helper.c4d/Rule.c4d: entry Script.c is now ScriptDE.c"],
        )

    def test_a_stored_crc_the_model_cannot_reproduce_stops_the_edit(self) -> None:
        broken = bytearray(nested())
        first_record = group_entry.HEADER
        struct.pack_into("<I", broken, first_record + 285, 0xDEADBEEF)

        with self.assertRaisesRegex(group_entry.GroupEntryError, "file CRC model"):
            group_entry.edit_entry(bytes(broken), ["Scenario.txt"], group_entry.renaming("S.txt"))

    def test_an_edit_that_is_not_exactly_one_thing_is_refused(self) -> None:
        for parts, change, message in [
            (["Missing.txt"], group_entry.renaming("X.txt"), "0 entries"),
            (["Helper.c4d"], group_entry.renaming("X.c4d"), "is a child group"),
            (["Title.txt", "Inner.txt"], group_entry.renaming("X.txt"), "is not a child group"),
            (["Title.txt"], group_entry.renaming("Scenario.txt"), "already an entry"),
            (["Title.txt"], group_entry.replacing(b"absent", b"x"), "occur 0 times"),
            (
                ["Helper.c4d", "Rule.c4d", "Script.c"],
                group_entry.replacing(b"();", b"x"),
                "occur 2 times",
            ),
        ]:
            with self.subTest(message=message):
                with self.assertRaisesRegex(group_entry.GroupEntryError, message):
                    group_entry.edit_entry(nested(), parts, change)

    def test_the_command_rewrites_a_packed_file_and_keeps_its_member_header(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "Pack.c4s"
            path.write_bytes(group_entry.pack(nested(), MEMBER_HEADER))
            copy = Path(directory) / "Copy.c4s"
            copy.write_bytes(path.read_bytes())

            output = io.StringIO()
            with contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
                self.assertEqual(
                    group_entry.main(
                        ["rename", str(path), "Helper.c4d/Rule.c4d/Script.c", "Script2.c"]
                    ),
                    0,
                )
                self.assertEqual(path.read_bytes()[:10], MEMBER_HEADER)
                self.assertEqual(group_entry.main(["compare", str(copy), str(path)]), 0)
                self.assertEqual(group_entry.main(["rename", str(path), "Nope.txt", "X.txt"]), 1)
                self.assertEqual(group_entry.main(["rename", str(path)]), 2)
            self.assertIn("entry Script.c is now Script2.c", output.getvalue())
            self.assertIn("error: 'Nope.txt': 0 entries", output.getvalue())


if __name__ == "__main__":
    unittest.main()
