#!/usr/bin/env python3
"""Compare DOS MZ load images or the emitted contents of bounded OMF objects.

OMF record definitions follow Open Watcom's pcobj.h and omfreloc.c:
https://github.com/open-watcom/open-watcom-v2/blob/master/bld/watcom/h/pcobj.h
https://github.com/open-watcom/open-watcom-v2/blob/master/bld/wl/c/omfreloc.c

OMF comparison ignores record partitioning, name-table indices, comments and
debug line/type records. It retains segment layout, initialized bytes, public
and external names, groups, and resolved FIXUPP threads. It is deliberately
strict about different instruction encodings and embedded relocation addends;
equal linked MZ images remain the stronger final check. Unsupported records,
iterated-data relocations, and malformed inputs fail instead of being ignored.
Conflicting overlapping data is also rejected, including compiler -d2 debug
sections that use this representation; compare linked MZ files in that case.
"""

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import sys


MAX_SEGMENT = 16 * 1024 * 1024
OMF_THEADR = 0x80
OMF_LHEADR = 0x82
OMF_MODEND = 0x8A
OMF_EXTDEF = 0x8C
OMF_PUBDEF = 0x90
OMF_COMENT = 0x88
OMF_TYPDEF = 0x8E
OMF_LINNUM = 0x94
OMF_LINNUM32 = OMF_LINNUM + 1
OMF_LNAMES = 0x96
OMF_SEGDEF = 0x98
OMF_GRPDEF = 0x9A
OMF_FIXUPP = 0x9C
OMF_LEDATA = 0xA0
OMF_LIDATA = 0xA2
OMF_LEXTDEF = 0xB4
OMF_LPUBDEF = 0xB6
OMF_GROUP_SEGMENT = 0xFF
OMF_INDEX_WIDE = 0x80
OMF_INDEX_HIGH_MASK = 0x7F
OMF_MAX_ITERATION_DEPTH = 32
OMF_REFERENCE_SEGMENT = 0
OMF_REFERENCE_GROUP = 1
OMF_REFERENCE_EXTERNAL = 2
OMF_REFERENCE_ABSOLUTE = 3
OMF_REFERENCE_LOCATION = 4
OMF_REFERENCE_TARGET = 5
OMF_REFERENCE_NONE = 6
OMF_FRAME_THREAD_FLAG = 0x80
OMF_TARGET_THREAD_FLAG = 0x08
OMF_NO_DISPLACEMENT_FLAG = 0x04
OMF_FRAME_METHOD_SHIFT = 4
OMF_FRAME_METHOD_MASK = 7
OMF_TARGET_METHOD_MASK = 3
OMF_THREAD_INDEX_MASK = 3
OMF_FIXUP_FLAG = 0x80
OMF_SEGMENT_RELATIVE_FLAG = 0x40
OMF_FRAME_THREAD_DEFINITION_FLAG = 0x40
OMF_FIXUP_LOCATION_HIGH_MASK = 3
OMF_FIXUP_KIND_SHIFT = 2
OMF_FIXUP_KIND_MASK = 15
OMF_THREAD_METHOD_SHIFT = 2
OMF_SEGMENT_ALIGNMENT_SHIFT = 5
OMF_BIG_SEGMENT_FLAG = 2
OMF_WIDE_RECORD_FLAG = 1
OMF_MODULE_START_FLAG = 0x40
OMF_MODULE_MAIN_FLAG = 0x80
OMF_MODULE_LOGICAL_START_FLAG = 1
BYTE_BITS = 8
BYTE_MASK = 0xFF
MZ_HEADER_SIZE = struct.calcsize("<14H")
MZ_PAGE_SIZE = 512
MZ_PARAGRAPH_SIZE = 16
MZ_RELOCATION_SIZE = struct.calcsize("<HH")
MZ_MAX_LOAD_IMAGE = 1024 * 1024
DIFFERING_BYTE_LIMIT = 16
FIXUP_WIDTHS = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def require(condition, message):
    if not condition:
        raise ValueError(message)


class Reader:
    def __init__(self, data):
        self.data = data
        self.offset = 0

    def take(self, size):
        require(size >= 0 and self.offset + size <= len(self.data), "Truncated record")
        result = self.data[self.offset:self.offset + size]
        self.offset += size
        return result

    def number(self, size=1):
        return int.from_bytes(self.take(size), "little")

    def index(self):
        first = self.number()
        return ((first & OMF_INDEX_HIGH_MASK) << BYTE_BITS) | self.number() if first & OMF_INDEX_WIDE else first

    def name(self):
        return self.take(self.number()).decode("latin1")

    def remaining(self):
        return len(self.data) - self.offset


def parse_mz(path):
    data = path.read_bytes()
    require(len(data) >= MZ_HEADER_SIZE and data[:2] == b"MZ", f"Not a DOS MZ executable: {path}")
    words = struct.unpack_from("<14H", data)
    _, last_page, pages, count, paragraphs, minimum, maximum, ss, sp, checksum, ip, cs, table, overlay = words
    require(pages > 0 and last_page < MZ_PAGE_SIZE, "Invalid MZ page counts")
    declared_size = (pages - 1) * MZ_PAGE_SIZE + (last_page or MZ_PAGE_SIZE)
    header_size = paragraphs * MZ_PARAGRAPH_SIZE
    require(MZ_HEADER_SIZE <= header_size <= declared_size <= len(data), "Invalid MZ image bounds")
    require(MZ_HEADER_SIZE <= table and table + count * MZ_RELOCATION_SIZE <= header_size, "Invalid MZ relocation table")
    image = data[header_size:declared_size]
    require(len(image) <= MZ_MAX_LOAD_IMAGE, "MZ load image exceeds real-mode comparator bound")
    relocations = []
    relocation_entries = []
    for index in range(count):
        offset, segment = struct.unpack_from("<HH", data, table + MZ_RELOCATION_SIZE * index)
        address = segment * MZ_PARAGRAPH_SIZE + offset
        require(address + 2 <= len(image), "MZ relocation lies outside loaded image")
        relocations.append(address)
        relocation_entries.append((offset, segment))
    return {
        "file_sha256": sha(data),
        "file_bytes": len(data),
        "header_bytes": header_size,
        "checksum": checksum,
        "trailer_bytes": len(data) - declared_size,
        "trailer_sha256": sha(data[declared_size:]),
        "loader": {"minimum_paragraphs": minimum, "maximum_paragraphs": maximum,
                   "ss": ss, "sp": sp, "cs": cs, "ip": ip, "overlay": overlay},
        "image": image,
        "relocations": relocations,
        "relocation_entries": relocation_entries,
    }


def mz_summary(parsed):
    result = {key: value for key, value in parsed.items() if key != "image"}
    result["image_bytes"] = len(parsed["image"])
    result["image_sha256"] = sha(parsed["image"])
    return result


def differing_bytes(left, right, limit=DIFFERING_BYTE_LIMIT):
    result = []
    for offset in range(max(len(left), len(right))):
        a = left[offset] if offset < len(left) else None
        b = right[offset] if offset < len(right) else None
        if a != b:
            result.append({"offset": offset, "left": a, "right": b})
            if len(result) == limit:
                break
    return result


def compare_mz(left, right):
    order_equal = left["relocation_entries"] == right["relocation_entries"]
    multiset_equal = Counter(left["relocations"]) == Counter(right["relocations"])
    entries_equal = Counter(left["relocation_entries"]) == Counter(right["relocation_entries"])
    locations = set(left["relocations"]) | set(right["relocations"])
    overlap = any(address + 1 in locations for address in locations)
    # Additions to disjoint words commute. Duplicate entries are retained by
    # the multiset and add the load segment repeatedly. Partially overlapping
    # words require the original order because carries can change the result.
    # Also retain each raw offset:segment pair. Alternative aliases of the
    # same linear location can behave differently near segment/A20 wrap.
    relocation_equal = entries_equal and (order_equal or (multiset_equal and not overlap))
    image_equal = left["image"] == right["image"]
    loader_equal = left["loader"] == right["loader"]
    return {
        "equivalent": image_equal and loader_equal and relocation_equal,
        "file_bytes_equal": left["file_sha256"] == right["file_sha256"],
        "load_image_equal": image_equal,
        "loader_fields_equal": loader_equal,
        "relocation_order_equal": order_equal,
        "relocation_multiset_equal": multiset_equal,
        "relocation_entry_multiset_equal": entries_equal,
        "partially_overlapping_relocations": overlap,
        "relocation_semantics_equal": relocation_equal,
        "first_image_differences": differing_bytes(left["image"], right["image"]),
        "left": mz_summary(left),
        "right": mz_summary(right),
    }


class OMF:
    def __init__(self, ignore_case=False):
        self.ignore_case = ignore_case
        self.names = [None]
        self.segments = [None]
        self.groups = [None]
        self.externals = [None]
        self.publics = []
        self.fixups = []
        self.frame_threads = {}
        self.target_threads = {}
        self.last_data = None
        self.comments = Counter()
        self.record_counts = Counter()
        self.module_end = None
        self.total_segment_bytes = 0

    def canonical_name(self, name):
        if self.ignore_case:
            return name.translate(str.maketrans("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
        return name

    def lookup(self, table, index):
        require(0 < index < len(table), f"Invalid OMF index {index}")
        return table[index]

    def segment_key(self, index):
        segment = self.lookup(self.segments, index)
        return (segment["name"], segment["class"], segment["overlay"])

    def reference(self, reader, method):
        if method == OMF_REFERENCE_SEGMENT:
            return ("segment", self.segment_key(reader.index()))
        if method == OMF_REFERENCE_GROUP:
            return ("group", self.lookup(self.groups, reader.index())["name"])
        if method == OMF_REFERENCE_EXTERNAL:
            return ("external", self.lookup(self.externals, reader.index()))
        if method == OMF_REFERENCE_ABSOLUTE:
            return ("absolute", reader.number(2))
        if method in (OMF_REFERENCE_LOCATION, OMF_REFERENCE_TARGET, OMF_REFERENCE_NONE):
            return ("location", "target", "none")[method - OMF_REFERENCE_LOCATION],
        raise ValueError(f"Unsupported OMF reference method {method}")

    def fixdata(self, reader, wide):
        flags = reader.number()
        if flags & OMF_FRAME_THREAD_FLAG:
            key = (flags >> OMF_FRAME_METHOD_SHIFT) & OMF_THREAD_INDEX_MASK
            require(key in self.frame_threads, "Undefined OMF frame thread")
            frame = self.frame_threads[key]
        else:
            frame = self.reference(reader, (flags >> OMF_FRAME_METHOD_SHIFT) & OMF_FRAME_METHOD_MASK)
        if flags & OMF_TARGET_THREAD_FLAG:
            key = flags & OMF_THREAD_INDEX_MASK
            require(key in self.target_threads, "Undefined OMF target thread")
            target = self.target_threads[key]
        else:
            target = self.reference(reader, flags & OMF_TARGET_METHOD_MASK)
        displacement = 0 if flags & OMF_NO_DISPLACEMENT_FLAG else reader.number(4 if wide else 2)
        if frame == ("target",):
            frame = target
        return frame, target, displacement

    def iterated(self, reader, wide, depth=0):
        require(depth < OMF_MAX_ITERATION_DEPTH, "Excessively nested LIDATA")
        repeat = reader.number(4 if wide else 2)
        blocks = reader.number(2)
        if blocks:
            parts = []
            length = 0
            for _ in range(blocks):
                part = self.iterated(reader, wide, depth + 1)
                length += len(part)
                require(length <= MAX_SEGMENT, "Oversized LIDATA block")
                parts.append(part)
            unit = b"".join(parts)
        else:
            unit = reader.take(reader.number())
        require(len(unit) * repeat <= MAX_SEGMENT, "Oversized LIDATA expansion")
        return unit * repeat

    def read_record(self, record_type, payload):
        self.record_counts[f"{record_type:02x}"] += 1
        reader = Reader(payload)
        wide = bool(record_type & OMF_WIDE_RECORD_FLAG)
        base = record_type & ~OMF_WIDE_RECORD_FLAG
        if record_type in (OMF_THEADR, OMF_LHEADR, OMF_TYPDEF, OMF_LINNUM, OMF_LINNUM32):
            return  # Module source names and debug type/line records.
        if record_type == OMF_COMENT:
            reader.number()
            self.comments[f"{reader.number():02x}"] += 1
            return  # Report their classes; this is an emitted-object comparator.
        if record_type == OMF_LNAMES:
            while reader.remaining():
                self.names.append(self.canonical_name(reader.name()))
        elif base == OMF_SEGDEF:
            attributes = reader.number()
            absolute = [reader.number(2), reader.number()] if attributes >> OMF_SEGMENT_ALIGNMENT_SHIFT == 0 else None
            length = reader.number(4 if wide else 2)
            if attributes & OMF_BIG_SEGMENT_FLAG:
                length = 1 << (32 if wide else 16)
            require(length <= MAX_SEGMENT, "OMF segment exceeds comparator bound")
            self.total_segment_bytes += length
            require(self.total_segment_bytes <= MAX_SEGMENT, "OMF module exceeds comparator bound")
            name, segment_class, overlay = [self.lookup(self.names, reader.index()) for _ in range(3)]
            self.segments.append({"name": name, "class": segment_class, "overlay": overlay,
                                  "attributes": attributes, "absolute": absolute,
                                  "length": length, "data": bytearray(length),
                                  "initialized": bytearray(length)})
        elif record_type == OMF_GRPDEF:
            name = self.lookup(self.names, reader.index())
            members = []
            while reader.remaining():
                require(reader.number() == OMF_GROUP_SEGMENT, "Unsupported OMF group component")
                members.append(self.segment_key(reader.index()))
            self.groups.append({"name": name, "segments": members})
        elif record_type in (OMF_EXTDEF, OMF_LEXTDEF):
            while reader.remaining():
                name = self.canonical_name(reader.name())
                reader.index()  # Debug type index.
                self.externals.append(("local" if record_type == OMF_LEXTDEF else "global", name))
        elif base in (OMF_PUBDEF, OMF_LPUBDEF):
            group = reader.index()
            segment = reader.index()
            frame = reader.number(2) if segment == 0 else None
            group_name = self.lookup(self.groups, group)["name"] if group else None
            segment_name = self.segment_key(segment) if segment else None
            while reader.remaining():
                name = self.canonical_name(reader.name())
                offset = reader.number(4 if wide else 2)
                reader.index()
                self.publics.append({"name": name, "local": base == OMF_LPUBDEF, "group": group_name,
                                     "segment": segment_name, "frame": frame, "offset": offset})
        elif base in (OMF_LEDATA, OMF_LIDATA):
            index = reader.index()
            offset = reader.number(4 if wide else 2)
            segment = self.lookup(self.segments, index)
            if base == OMF_LIDATA:
                pieces = []
                total = 0
                while reader.remaining():
                    piece = self.iterated(reader, wide)
                    total += len(piece)
                    require(total <= MAX_SEGMENT, "Oversized LIDATA record")
                    pieces.append(piece)
                data = b"".join(pieces)
            else:
                data = reader.take(reader.remaining())
            require(offset + len(data) <= segment["length"], "OMF data exceeds segment")
            for position, value in enumerate(data, offset):
                require(not segment["initialized"][position] or segment["data"][position] == value,
                        "Conflicting overlapping OMF data")
                segment["data"][position] = value
                segment["initialized"][position] = 1
            self.last_data = (index, offset, len(data), base == OMF_LIDATA)
        elif base == OMF_FIXUPP:
            while reader.remaining():
                first = reader.number()
                if first & OMF_FIXUP_FLAG:
                    require(self.last_data is not None, "FIXUPP without preceding data")
                    segment, start, length, iterated = self.last_data
                    require(not iterated, "Relocations within LIDATA are unsupported")
                    location = ((first & OMF_FIXUP_LOCATION_HIGH_MASK) << BYTE_BITS) | reader.number()
                    kind = (first >> OMF_FIXUP_KIND_SHIFT) & OMF_FIXUP_KIND_MASK
                    require(kind in FIXUP_WIDTHS and location + FIXUP_WIDTHS[kind] <= length,
                            "Unsupported or out-of-bounds OMF fixup location")
                    frame, target, displacement = self.fixdata(reader, wide)
                    if frame == ("location",):
                        frame = ("segment", self.segment_key(segment))
                    self.fixups.append({"segment": self.segment_key(segment),
                                        "offset": start + location, "kind": kind,
                                        "segment_relative": bool(first & OMF_SEGMENT_RELATIVE_FLAG), "frame": frame,
                                        "target": target, "displacement": displacement})
                elif first & OMF_FRAME_THREAD_DEFINITION_FLAG:
                    self.frame_threads[first & OMF_THREAD_INDEX_MASK] = self.reference(reader, (first >> OMF_THREAD_METHOD_SHIFT) & OMF_FRAME_METHOD_MASK)
                else:
                    self.target_threads[first & OMF_THREAD_INDEX_MASK] = self.reference(reader, (first >> OMF_THREAD_METHOD_SHIFT) & OMF_TARGET_METHOD_MASK)
        elif base == OMF_MODEND:
            flags = reader.number()
            require(not (flags & OMF_MODULE_START_FLAG) or flags & OMF_MODULE_LOGICAL_START_FLAG, "Physical MODEND start address unsupported")
            start = self.fixdata(reader, wide) if flags & OMF_MODULE_START_FLAG else None
            self.module_end = {"main": bool(flags & OMF_MODULE_MAIN_FLAG), "start": start}
        else:
            raise ValueError(f"Unsupported OMF record 0x{record_type:02x}")
        require(reader.remaining() == 0, f"Unparsed bytes in OMF record 0x{record_type:02x}")

    def normalized(self):
        segments = []
        keys = []
        for index, segment in enumerate(self.segments[1:], 1):
            key = self.segment_key(index)
            require(key not in keys, f"Duplicate segment identity: {key}")
            keys.append(key)
            segments.append({k: bytes(v).hex() if k in ("data", "initialized") else v
                             for k, v in segment.items()})
        sort = lambda values: sorted(values, key=lambda value: json.dumps(value, sort_keys=True))
        occupied = set()
        overlapping = False
        for fixup in self.fixups:
            locations = {(fixup["segment"], fixup["offset"] + byte)
                         for byte in range(FIXUP_WIDTHS[fixup["kind"]])}
            overlapping |= bool(occupied & locations)
            occupied.update(locations)
        return {"segments": sort(segments), "segment_order": keys,
                "groups": sort(self.groups[1:]),
                "externals": sort(self.externals[1:]), "publics": sort(self.publics),
                "fixups": self.fixups if overlapping else sort(self.fixups),
                "overlapping_fixups": overlapping, "module_end": self.module_end}


def parse_omf(path, ignore_case=False):
    data = path.read_bytes()
    reader = Reader(data)
    module = OMF(ignore_case)
    while reader.remaining():
        require(module.module_end is None, "Trailing bytes after OMF MODEND")
        start = reader.offset
        record_type = reader.number()
        size = reader.number(2)
        require(size >= 1, "Empty OMF record")
        payload = reader.take(size)
        # OMF explicitly allows a zero checksum byte to mean not supplied.
        require(payload[-1] == 0 or sum(data[start:reader.offset]) & BYTE_MASK == 0,
                "Invalid OMF record checksum")
        module.read_record(record_type, payload[:-1])
    require(module.module_end is not None, "Missing OMF MODEND")
    return {"file_sha256": sha(data), "file_bytes": len(data),
            "ascii_names_ignore_case": ignore_case,
            "records": dict(module.record_counts), "ignored_comment_classes": dict(module.comments),
            "normalized": module.normalized()}


def compare_omf(left, right):
    differences = []
    for field in left["normalized"]:
        if left["normalized"][field] != right["normalized"][field]:
            differences.append(field)
    segment_differences = []
    def by_name(parsed):
        return {(s["name"], s["class"], s["overlay"]): s
                for s in parsed["normalized"]["segments"]}
    a, b = by_name(left), by_name(right)
    for key in sorted(a.keys() & b.keys()):
        if a[key] != b[key]:
            segment_differences.append({"segment": key,
                "left_length": a[key]["length"], "right_length": b[key]["length"],
                "first_data_differences": differing_bytes(bytes.fromhex(a[key]["data"]),
                                                          bytes.fromhex(b[key]["data"]))})
    return {"equivalent": not differences, "comparison_scope": "emitted OMF; comments/debug ignored",
            "different_fields": differences, "segment_differences": segment_differences,
            "left_only_segments": sorted(a.keys() - b.keys()),
            "right_only_segments": sorted(b.keys() - a.keys()),
            "left": left, "right": right}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("kind", choices=("mz", "omf"))
    parser.add_argument("left", type=Path)
    parser.add_argument("right", type=Path)
    parser.add_argument("--output", type=Path, help="Write full JSON, including normalized OMF data")
    parser.add_argument("--ignore-case", action="store_true",
                        help="OMF only: compare ASCII names case-insensitively; retain exact data bytes")
    args = parser.parse_args()
    if args.ignore_case and args.kind != "omf":
        parser.error("--ignore-case is only valid for OMF comparisons")
    try:
        if args.kind == "mz":
            result = compare_mz(parse_mz(args.left), parse_mz(args.right))
        else:
            result = compare_omf(parse_omf(args.left, args.ignore_case),
                                 parse_omf(args.right, args.ignore_case))
    except (ValueError, OSError, struct.error) as error:
        print(f"Comparison failed: {error}", file=sys.stderr)
        return 2
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2) + "\n")
    brief = {key: value for key, value in result.items() if key not in ("left", "right")}
    print(json.dumps(brief, indent=2))
    return 0 if result["equivalent"] else 1


if __name__ == "__main__":
    sys.exit(main())
