#!/usr/bin/env python3
"""Change one entry inside a packed C4Group without rewriting the rest of it.

Many imported entries are packed groups, and CONTRIBUTING.md says not to unpack
them. A group tool that rebuilds the file restamps its times, and the port's
c4group cannot write into a nested child group at all. This edits the raw group
image instead: the entry's name or data, its size and CRC, the offsets of the
entries after it, and the size and CRC of every enclosing child entry. Nothing
else in the image moves, and `compare` shows that.

A packed group is a gzip stream whose magic is 1e 8c instead of 1f 8b. The
image is a 204-byte header (scrambled: bytes i and i+2 swapped for every third
i, then XOR 0xED; the entry count is a little-endian int at offset 36), then one
316-byte record per entry, then the entry data. A child group is stored
uncompressed, as its own image, in place.

Record fields used here: name[260] at 0, child flag at 264, size at 268, offset
at 276, time at 280, CRC state at 284 and CRC at 285. State 2 is a current CRC:

  file entry   crc32(crc32(0, data), name), or 0 for an empty file
  child group  XOR of the child's own entry CRCs

State 1 is an older CRC of a file's data alone, without its name (C4GECS_Old).

Before it changes anything the editor proves that model against the stored
value of every state-2 or state-1 record on the path and refuses if one does
not reproduce, then writes the new CRC in the record's own state. State 0
records carry no CRC (older writers; the engine calculates it on demand) and
are left as they are. A child group in state 1, or any other state, is refused.

`add` appends a new file entry to a group: its record after the group's last
record and its data after the group's last data, so every existing entry keeps
its offset. The new record takes the newest time of the entries beside it and
their CRC state, 2 or 0; beside mixed or legacy states it is refused, as is a
name that is already an entry.

  group_entry.py replace <group> <Child.c4d/.../Entry> <old-bytes-file> <new-bytes-file>
  group_entry.py rename  <group> <Child.c4d/.../Entry> <NewName>
  group_entry.py add     <group> <Child.c4d/.../NewEntry> <data-file>
  group_entry.py cat     <group> <Child.c4d/.../Entry> <output-file>
  group_entry.py compare <original-group> <edited-group>
"""

from __future__ import annotations

import gzip
import struct
import sys
import zlib
from pathlib import Path
from typing import Callable

HEADER = 204
RECORD = 316
NAME = 260
MAGIC = b"\x1e\x8c"
GZIP_MAGIC = b"\x1f\x8b"

Change = Callable[[bytes, bytes], "tuple[bytes, bytes]"]


class GroupEntryError(Exception):
    """The group is not shaped the way this tool can edit safely."""


def unpack(raw: bytes) -> bytes:
    if raw[:2] != MAGIC:
        raise GroupEntryError("not a packed group")
    return gzip.decompress(GZIP_MAGIC + raw[2:])


def pack(image: bytes, member_header: bytes) -> bytes:
    """Compress `image`, keeping the ten-byte gzip member header the file had."""
    if member_header[2:4] != b"\x08\x00":
        raise GroupEntryError("unexpected gzip member header")
    packed = bytearray(gzip.compress(image, compresslevel=9, mtime=0))
    packed[:10] = member_header
    return bytes(packed)


def unscramble(header: bytes) -> bytes:
    data = bytearray(header)
    for index in range(0, len(data) - 2, 3):
        data[index], data[index + 2] = data[index + 2], data[index]
    return bytes(value ^ 0xED for value in data)


def records(image: bytes) -> tuple[list[dict], int]:
    count = struct.unpack_from("<i", unscramble(image[:HEADER]), 36)[0]
    found = []
    for index in range(count):
        start = HEADER + index * RECORD
        record = image[start : start + RECORD]
        child, size, _unused, offset = struct.unpack_from("<4i", record, 264)
        found.append(
            {
                "start": start,
                "name": record[:NAME].split(b"\0", 1)[0],
                "child": child,
                "size": size,
                "offset": offset,
                "crc_state": record[284],
                "crc": struct.unpack_from("<I", record, 285)[0],
                "rest": record[NAME:276] + b"\0\0\0\0" + record[280:],
            }
        )
    return found, HEADER + count * RECORD


def file_crc(data: bytes, name: bytes) -> int:
    return zlib.crc32(name, zlib.crc32(data)) & 0xFFFFFFFF if data else 0


def legacy_file_crc(data: bytes) -> int:
    """C4GECS_Old: the data's CRC alone, which C4Group::CalcCRC32 extends by the
    name only when it upgrades the record (C4Group.cpp:2444-2516)."""
    return zlib.crc32(data) & 0xFFFFFFFF if data else 0


def contents_crc(entries: list[dict]) -> int:
    crc = 0
    for entry in entries:
        if entry["crc_state"] != 2:
            raise GroupEntryError(f"{entry['name']!r} carries no current CRC to fold")
        crc ^= entry["crc"]
    return crc


def edit_entry(image: bytes, parts: list[str], change: Change, *, group: bool = False) -> bytes:
    """Apply `change(name, data) -> (name, data)` to the entry at `parts`: a
    file, or with `group` the image of a child group."""
    entries, data_start = records(image)
    wanted = parts[0].encode("latin-1").lower()
    matches = [entry for entry in entries if entry["name"].lower() == wanted]
    if len(matches) != 1:
        raise GroupEntryError(f"{parts[0]!r}: {len(matches)} entries")
    target = matches[0]
    if target["crc_state"] not in (0, 1, 2):
        raise GroupEntryError(f"{parts[0]!r}: CRC state {target['crc_state']}")
    checked = target["crc_state"] == 2
    legacy = target["crc_state"] == 1
    begin = data_start + target["offset"]
    end = begin + target["size"]
    data = image[begin:end]

    if len(parts) == 1 and not group:
        if target["child"]:
            raise GroupEntryError(f"{parts[0]!r} is a child group")
        if checked and file_crc(data, target["name"]) != target["crc"]:
            raise GroupEntryError("file CRC model does not hold")
        if legacy and legacy_file_crc(data) != target["crc"]:
            raise GroupEntryError("legacy file CRC model does not hold")
        name, replaced = change(target["name"], data)
        if len(name) >= NAME or b"\0" in name or not name:
            raise GroupEntryError(f"unusable entry name {name!r}")
        if any(e is not target and e["name"].lower() == name.lower() for e in entries):
            raise GroupEntryError(f"{name!r} is already an entry")
        if checked:
            crc = file_crc(replaced, name)
        elif legacy:
            crc = legacy_file_crc(replaced)
        else:
            crc = target["crc"]
    else:
        if not target["child"]:
            raise GroupEntryError(f"{parts[0]!r} is not a child group")
        if legacy:
            raise GroupEntryError(f"{parts[0]!r}: a legacy CRC on a child group")
        if checked and contents_crc(records(data)[0]) != target["crc"]:
            raise GroupEntryError("child CRC model does not hold")
        if len(parts) == 1:
            name, replaced = change(target["name"], data)
        else:
            name = target["name"]
            replaced = edit_entry(data, parts[1:], change, group=group)
        crc = contents_crc(records(replaced)[0]) if checked else target["crc"]

    delta = len(replaced) - len(data)
    out = bytearray(image[:begin] + replaced + image[end:])
    out[target["start"] : target["start"] + NAME] = name.ljust(NAME, b"\0")
    struct.pack_into("<i", out, target["start"] + 268, len(replaced))
    struct.pack_into("<I", out, target["start"] + 285, crc)
    for entry in entries:
        if entry["offset"] > target["offset"]:
            struct.pack_into("<i", out, entry["start"] + 276, entry["offset"] + delta)
    return bytes(out)


def replacing(old: bytes, new: bytes) -> Change:
    def change(name: bytes, data: bytes) -> tuple[bytes, bytes]:
        if data.count(old) != 1:
            raise GroupEntryError(f"old bytes occur {data.count(old)} times")
        return name, data.replace(old, new)

    return change


def renaming(new_name: str) -> Change:
    return lambda _name, data: (new_name.encode("latin-1"), data)


def read_entry(image: bytes, parts: list[str]) -> bytes:
    captured: list[bytes] = []

    def capture(name: bytes, data: bytes) -> tuple[bytes, bytes]:
        captured.append(data)
        return name, data

    edit_entry(image, parts, capture)
    return captured[0]


def appended(image: bytes, name: str, data: bytes) -> bytes:
    """`image` with a new file entry after its last record and after all of its
    data, so every existing entry keeps its offset. The record takes the newest
    time and the CRC convention of the entries beside it."""
    entries, data_start = records(image)
    encoded = name.encode("latin-1")
    if len(encoded) >= NAME or b"\0" in encoded or not encoded:
        raise GroupEntryError(f"unusable entry name {encoded!r}")
    if any(entry["name"].lower() == encoded.lower() for entry in entries):
        raise GroupEntryError(f"{encoded!r} is already an entry")
    states = {entry["crc_state"] for entry in entries} or {2}
    if len(states) != 1 or not states <= {0, 2}:
        raise GroupEntryError(f"cannot follow the CRC states {sorted(states)} beside {encoded!r}")
    state = states.pop()

    record = bytearray(RECORD)
    record[: len(encoded)] = encoded
    struct.pack_into("<4i", record, 264, 0, len(data), 0, len(image) - data_start)
    times = (struct.unpack_from("<I", image, entry["start"] + 280)[0] for entry in entries)
    struct.pack_into("<I", record, 280, max(times, default=0))
    record[284] = state
    struct.pack_into("<I", record, 285, file_crc(data, encoded) if state == 2 else 0)
    header = bytearray(unscramble(image[:HEADER]))
    struct.pack_into("<i", header, 36, len(entries) + 1)
    return unscramble(bytes(header)) + image[HEADER:data_start] + bytes(record) + image[data_start:] + data


def add_entry(image: bytes, parts: list[str], data: bytes) -> bytes:
    """Add a file entry: `parts` names the child groups down to it, then the
    new entry's name."""
    if len(parts) == 1:
        return appended(image, parts[0], data)
    return edit_entry(
        image,
        parts[:-1],
        lambda name, group: (name, appended(group, parts[-1], data)),
        group=True,
    )


def compare(first: bytes, second: bytes, where: str) -> list[str]:
    """Every difference between two images, entry by entry, by position. Entries
    appended after all of the first image's entries are reported as added."""
    report = []
    if first[:HEADER] != second[:HEADER]:
        report.append(f"{where}: group header differs")
    (left, first_start), (right, second_start) = records(first), records(second)
    if len(left) != len(right):
        prefix = [entry["name"] for entry in right[: len(left)]]
        if len(right) < len(left) or prefix != [entry["name"] for entry in left]:
            return report + [f"{where}: {len(left)} entries became {len(right)}"]
    for a, b in zip(left, right):
        path = f"{where}/{b['name'].decode('latin-1')}"
        if a["name"] != b["name"]:
            report.append(
                f"{where}: entry {a['name'].decode('latin-1')} is now {b['name'].decode('latin-1')}"
            )
        if a["rest"] != b["rest"]:
            spans = {"size": (268, 272), "time": (280, 284), "crc": (285, 289)}
            fields = [
                label
                for label, (lo, hi) in spans.items()
                if a["rest"][lo - NAME : hi - NAME] != b["rest"][lo - NAME : hi - NAME]
            ]
            other = any(
                a["rest"][i] != b["rest"][i]
                for i in range(len(a["rest"]))
                if not any(lo <= i + NAME < hi for lo, hi in spans.values())
            )
            report.append(
                f"{path}: record differs in {fields}{' and OTHER BYTES' if other else ''}"
            )
        data_a = first[first_start + a["offset"] : first_start + a["offset"] + a["size"]]
        data_b = second[second_start + b["offset"] : second_start + b["offset"] + b["size"]]
        if data_a != data_b:
            if a["child"]:
                report.extend(compare(data_a, data_b, path))
            else:
                report.append(f"{path}: data differs, {len(data_a)} -> {len(data_b)} bytes")
    report.extend(f"{where}: entry {e['name'].decode('latin-1')} added" for e in right[len(left) :])
    return report


def rewrite(path: Path, transform: Callable[[bytes], bytes]) -> str:
    raw = path.read_bytes()
    image = unpack(raw)
    edited = transform(image)
    packed = pack(edited, raw[:10])
    path.write_bytes(packed)
    return f"{path}: image {len(image)} -> {len(edited)} bytes, file {len(raw)} -> {len(packed)} bytes"


def editing(inner: str, change: Change) -> Callable[[bytes], bytes]:
    return lambda image: edit_entry(image, inner.split("/"), change)


def main(argv: list[str]) -> int:
    try:
        command, *args = argv
        if command == "replace" and len(args) == 4:
            old, new = Path(args[2]).read_bytes(), Path(args[3]).read_bytes()
            print(rewrite(Path(args[0]), editing(args[1], replacing(old, new))))
        elif command == "rename" and len(args) == 3:
            print(rewrite(Path(args[0]), editing(args[1], renaming(args[2]))))
        elif command == "add" and len(args) == 3:
            data = Path(args[2]).read_bytes()
            print(rewrite(Path(args[0]), lambda image: add_entry(image, args[1].split("/"), data)))
        elif command == "cat" and len(args) == 3:
            data = read_entry(unpack(Path(args[0]).read_bytes()), args[1].split("/"))
            Path(args[2]).write_bytes(data)
            print(len(data))
        elif command == "compare" and len(args) == 2:
            first, second = (unpack(Path(arg).read_bytes()) for arg in args)
            report = compare(first, second, Path(args[1]).name)
            print("\n".join(report) if report else "identical")
        else:
            raise ValueError
    except ValueError:
        print(__doc__, file=sys.stderr)
        return 2
    except GroupEntryError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
