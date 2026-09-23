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
import zlib

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

    def legacy_group(self, data: bytes, crc: int) -> bytes:
        """A one-file group whose record carries C4GECS_Old: the data's CRC without its name."""
        legacy = bytearray(image([("Scenario.txt", data)], crc_state=0))
        first_record = group_entry.HEADER
        legacy[first_record + 284] = 1
        struct.pack_into("<I", legacy, first_record + 285, crc)
        return bytes(legacy)

    def test_a_legacy_crc_is_rewritten_over_the_new_data_alone(self) -> None:
        # C4GECS_Old keeps the CRC of the data alone. C4Group::CalcCRC32 folds
        # the name in when it upgrades the record (C4Group.cpp:2444-2516).
        before, after = b"Definition2=Explorer.c4d\r\n", b"Definition2=Puzzles.c4f\\Explorer.c4d\r\n"
        legacy = self.legacy_group(before, zlib.crc32(before))

        edited = group_entry.edit_entry(
            legacy, ["Scenario.txt"], group_entry.replacing(b"=Explorer", b"=Puzzles.c4f\\Explorer")
        )

        record = group_entry.records(edited)[0][0]
        self.assertEqual(record["crc_state"], 1)
        self.assertEqual(record["crc"], zlib.crc32(after))

    def test_a_legacy_crc_the_model_cannot_reproduce_stops_the_edit(self) -> None:
        legacy = self.legacy_group(b"Definition2=Explorer.c4d\r\n", 0xDEADBEEF)

        with self.assertRaisesRegex(group_entry.GroupEntryError, "legacy file CRC model"):
            group_entry.edit_entry(legacy, ["Scenario.txt"], group_entry.renaming("S.txt"))

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

    def test_an_added_file_is_what_a_fresh_build_appends(self) -> None:
        edited = group_entry.add_entry(
            nested(), ["Helper.c4d", "Rule.c4d", "DescUS.txt"], b"A rule.\r\n"
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
                            [
                                ("DefCore.txt", b"id=RULE\r\n"),
                                ("Script.c", b"a();\r\nb();\r\n"),
                                ("DescUS.txt", b"A rule.\r\n"),
                            ],
                        ),
                        ("After.txt", b"after"),
                    ],
                ),
                ("Title.txt", b"US:Title\r\n"),
            ]
        )
        self.assertEqual(edited, rebuilt)
        self.assertEqual(
            group_entry.read_entry(edited, ["Helper.c4d", "Rule.c4d", "DescUS.txt"]),
            b"A rule.\r\n",
        )

    def test_a_file_is_added_to_the_top_group_too(self) -> None:
        edited = group_entry.add_entry(nested(), ["StringTblUS.txt"], b"Key=Value\r\n")

        self.assertEqual(group_entry.read_entry(edited, ["StringTblUS.txt"]), b"Key=Value\r\n")
        self.assertEqual(group_entry.read_entry(edited, ["Helper.c4d", "After.txt"]), b"after")
        self.assertEqual(
            group_entry.compare(nested(), edited, "g"),
            ["g: group header differs", "g: entry StringTblUS.txt added"],
        )

    def test_an_added_file_reports_every_group_on_its_path(self) -> None:
        edited = group_entry.add_entry(
            nested(), ["Helper.c4d", "Rule.c4d", "DescUS.txt"], b"A rule.\r\n"
        )

        self.assertEqual(
            group_entry.compare(nested(), edited, "g"),
            [
                "g/Helper.c4d: record differs in ['size', 'crc']",
                "g/Helper.c4d/Rule.c4d: record differs in ['size', 'crc']",
                "g/Helper.c4d/Rule.c4d: group header differs",
                "g/Helper.c4d/Rule.c4d: entry DescUS.txt added",
            ],
        )

    def test_an_added_file_keeps_the_crc_convention_of_its_group(self) -> None:
        edited = group_entry.add_entry(
            nested(crc_state=0), ["Helper.c4d", "Rule.c4d", "DescUS.txt"], b"A rule.\r\n"
        )

        rule = self.child(self.child(edited, "Helper.c4d"), "Rule.c4d")
        added = group_entry.records(rule)[0][-1]
        self.assertEqual((added["name"], added["crc_state"], added["crc"]), (b"DescUS.txt", 0, 0))

    def child(self, group: bytes, name: str) -> bytes:
        """The image of the child group `name` inside `group`."""
        entries, start = group_entry.records(group)
        entry = next(entry for entry in entries if entry["name"] == name.encode("latin-1"))
        return group[start + entry["offset"] : start + entry["offset"] + entry["size"]]

    def test_an_addition_that_is_not_exactly_one_new_file_is_refused(self) -> None:
        for parts, message in [
            (["Helper.c4d", "Rule.c4d", "SCRIPT.C"], "already an entry"),
            (["Title.txt", "Inner.txt"], "is not a child group"),
            (["Missing.c4d", "Inner.txt"], "0 entries"),
            (["Helper.c4d", ""], "unusable entry name"),
        ]:
            with self.subTest(message=message):
                with self.assertRaisesRegex(group_entry.GroupEntryError, message):
                    group_entry.add_entry(nested(), parts, b"data")

    def mixed(self, script: bytes) -> bytes:
        """A group whose child carries a current CRC over entries that carry
        none (state 0) or only their data's (state 1), as older writers left
        them; the child's CRC is what C4Group::CalcCRC32 folds for them."""
        entries = [("DefCore.txt", b"id=OLDY\r\n"), ("Script.c", script), ("Names.txt", b"DE:Alt\r\n")]
        child = bytearray(image(entries, crc_state=0))
        names = group_entry.records(bytes(child))[0]
        # Names.txt keeps the older data-only CRC.
        child[names[2]["start"] + 284] = 1
        struct.pack_into("<I", child, names[2]["start"] + 285, zlib.crc32(b"DE:Alt\r\n"))
        folded = 0
        for name, data in entries:
            folded ^= group_entry.file_crc(data, name.encode("latin-1"))
        outer = bytearray(image([("Title.txt", b"US:Title\r\n"), ("Old.c4d", [])]))
        # Swap the empty placeholder child for the prepared one.
        records, start = group_entry.records(bytes(outer))
        placeholder = records[1]
        body = bytes(outer[: start + placeholder["offset"]]) + bytes(child)
        body = bytearray(body)
        struct.pack_into("<i", body, placeholder["start"] + 268, len(child))
        struct.pack_into("<I", body, placeholder["start"] + 285, folded)
        return bytes(body)

    def test_a_child_folds_the_crcs_its_older_entries_do_not_carry(self) -> None:
        edited = group_entry.edit_entry(
            self.mixed(b"a();\r\n"), ["Old.c4d", "Script.c"], group_entry.replacing(b"a();", b"b();")
        )

        self.assertEqual(edited, self.mixed(b"b();\r\n"))

    def test_a_child_whose_folded_crc_does_not_reproduce_is_still_refused(self) -> None:
        broken = bytearray(self.mixed(b"a();\r\n"))
        child = group_entry.records(bytes(broken))[0][1]
        struct.pack_into("<I", broken, child["start"] + 285, 0xDEADBEEF)

        with self.assertRaisesRegex(group_entry.GroupEntryError, "child CRC model"):
            group_entry.edit_entry(bytes(broken), ["Old.c4d", "Script.c"], group_entry.renaming("S.c"))

    def test_a_file_added_beside_older_entries_carries_a_current_crc(self) -> None:
        edited = group_entry.add_entry(self.mixed(b"a();\r\n"), ["Old.c4d", "DescUS.txt"], b"Old.\r\n")

        added = group_entry.records(self.child(edited, "Old.c4d"))[0][-1]
        self.assertEqual(
            (added["name"], added["crc_state"], added["crc"]),
            (b"DescUS.txt", 2, group_entry.file_crc(b"Old.\r\n", b"DescUS.txt")),
        )
        self.assertEqual(
            group_entry.records(edited)[0][1]["crc"],
            group_entry.image_crc(self.child(edited, "Old.c4d")),
        )

    def test_the_add_command_writes_the_new_entry_into_the_packed_file(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "Pack.c4s"
            path.write_bytes(group_entry.pack(nested(), MEMBER_HEADER))
            data = Path(directory) / "DescUS.txt"
            data.write_bytes(b"A rule.\r\n")

            output = io.StringIO()
            with contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
                self.assertEqual(
                    group_entry.main(
                        ["add", str(path), "Helper.c4d/Rule.c4d/DescUS.txt", str(data)]
                    ),
                    0,
                )
                self.assertEqual(
                    group_entry.main(
                        ["add", str(path), "Helper.c4d/Rule.c4d/DescUS.txt", str(data)]
                    ),
                    1,
                )
            self.assertEqual(path.read_bytes()[:10], MEMBER_HEADER)
            self.assertEqual(
                group_entry.read_entry(
                    group_entry.unpack(path.read_bytes()), ["Helper.c4d", "Rule.c4d", "DescUS.txt"]
                ),
                b"A rule.\r\n",
            )
            self.assertIn("is already an entry", output.getvalue())

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
