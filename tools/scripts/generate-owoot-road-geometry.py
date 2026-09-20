#!/usr/bin/env python3
"""Extract OWOOT road triangles from the original GAME1/2.P3S resources.

Run from any directory; optional --game-directory and --output override the
repository defaults. Only road paint materials (19, 22, 23) are extracted.
Paint variants share vertices, so these also cover dirt and ice. Kerbs (127,
128), railings, buildings, lane markings, and highway medians are excluded.
The output is checked in so DOS and headless builds do not load render shapes.
"""

import argparse
import hashlib
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]
# Scene overlays are included with their base model. Finish/slalom have a road
# overlay; the overpass also has the perpendicular lower road. Model 1's hill
# variants use separate model 36 so their sloping surface heights are retained.
MODEL_SHAPES = {
    0: ("road",), 1: ("road",), 2: ("turn",), 3: ("stur",),
    4: ("chi1",), 5: ("chi2",), 6: ("offl",), 7: ("offr",),
    8: ("sofl",), 9: ("sofr",), 10: ("gwro",), 11: ("wroa",),
    12: ("inte",), 16: ("ramp",), 17: ("sram",), 18: ("elrd",),
    19: ("elsp",), 20: ("selr",), 21: ("sest",), 22: ("elsp",),
    23: ("lban",), 24: ("rban",), 25: ("bank",), 26: ("btur",),
    27: ("loop", "loo1"), 28: ("tun2",), 29: ("spip",),
    30: ("pipe", "pip2"), 31: ("hpip", "pip2"),
    32: ("lco0", "lco1"), 33: ("rco0", "rco1"),
    34: ("road",), 35: ("vcor",), 36: ("rdup",),
}
ROAD_MATERIALS = {19, 22, 23}


def vle(data):
    size = int.from_bytes(data[1:4], "little")
    depth = data[4] & 127
    counts = data[5:5 + depth]
    alphabet = iter(data[5 + depth:5 + depth + sum(counts)])
    codes = {}
    code = 0
    for width, count in enumerate(counts, 1):
        for _ in range(count):
            codes[width, code] = next(alphabet)
            code += 1
        code *= 2
    stream = data[5 + depth + sum(counts):]
    output = bytearray()
    code = width = previous = 0
    for byte in stream:
        for bit in range(7, -1, -1):
            code = code * 2 + ((byte >> bit) & 1)
            width += 1
            if (width, code) not in codes:
                continue
            value = codes[width, code]
            if data[4] & 128:
                value = (value + previous) & 255
            output.append(value)
            previous = value
            code = width = 0
            if len(output) == size:
                return bytes(output)
    raise ValueError("truncated VLE resource")


def rle(data):
    size = int.from_bytes(data[1:4], "little")
    source_size = int.from_bytes(data[4:7], "little")
    escape_count = data[8] & 127
    escapes = data[9:9 + escape_count]
    source = data[9 + escape_count:]
    if data[8] <= 128:
        sequence = bytearray()
        cursor = 0
        while cursor < source_size:
            value = source[cursor]
            cursor += 1
            if value == escapes[1]:
                end = source.index(value, cursor)
                sequence.extend(source[cursor:end] * source[end + 1])
                cursor = end + 2
            else:
                sequence.append(value)
        source = sequence
    lookup = {value: index + 1 for index, value in enumerate(escapes)}
    output = bytearray()
    cursor = 0
    while len(output) < size:
        value = source[cursor]
        cursor += 1
        kind = lookup.get(value, 0)
        if kind:
            if kind == 1:
                count = source[cursor]
                cursor += 1
            elif kind == 3:
                count = int.from_bytes(source[cursor:cursor + 2], "little")
                cursor += 2
            else:
                count = kind - 1
            value = source[cursor]
            cursor += 1
            output.extend(bytes((value,)) * count)
        else:
            output.append(value)
    return bytes(output[:size])


def unpack(data):
    passes = data[0] & 127 if data[0] & 128 else 1
    if data[0] & 128:
        data = data[4:]
    for _ in range(passes):
        if data[0] == 1:
            data = rle(data)
        elif data[0] == 2:
            data = vle(data)
        else:
            raise ValueError("unsupported resource compression")
    return data


def shapes(data):
    count = struct.unpack_from("<H", data, 4)[0]
    base = 6 + count * 8
    result = {}
    for index in range(count):
        name = data[6 + 4 * index:10 + 4 * index].decode("ascii")
        start = base + struct.unpack_from("<I", data, 6 + 4 * count + 4 * index)[0]
        vertex_count, primitive_count, paint_count = data[start:start + 3]
        vertices = [struct.unpack_from("<hhh", data, start + 4 + 6 * i)
                    for i in range(vertex_count)]
        cursor = start + 4 + 6 * vertex_count + 8 * primitive_count
        polygons = []
        for _ in range(primitive_count):
            kind = data[cursor]
            material = data[cursor + 2]
            length = kind if kind <= 10 else (2 if kind == 11 else 6)
            indices = data[cursor + 2 + paint_count:cursor + 2 + paint_count + length]
            if 3 <= kind <= 10 and material in ROAD_MATERIALS:
                polygons.append([vertices[i] for i in indices])
            cursor += 2 + paint_count + length
        result[name] = polygons
    return result


def cross(a, b, c):
    return (b[0] - a[0]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[0] - a[0])


def triangles(polygon):
    # Triangulate concave split-road polygons without adding their grassy bays.
    points = polygon[:]
    area = sum(a[0] * b[2] - a[2] * b[0]
               for a, b in zip(points, points[1:] + points[:1]))
    if area == 0:
        return [(points[0], points[i], points[i + 1]) for i in range(1, len(points) - 1)]
    sign = 1 if area > 0 else -1
    result = []
    while len(points) > 3:
        for i in range(len(points)):
            a, b, c = points[i - 1], points[i], points[(i + 1) % len(points)]
            if cross(a, b, c) * sign <= 0:
                continue
            others = [v for j, v in enumerate(points)
                      if j not in ((i - 1) % len(points), i, (i + 1) % len(points))]
            if any(all(cross(x, y, v) * sign >= 0
                       for x, y in ((a, b), (b, c), (c, a))) for v in others):
                continue
            result.append((a, b, c))
            del points[i]
            break
        else:
            raise ValueError(f"cannot triangulate road polygon: {polygon}")
    return result + [tuple(points)]


def generate(directory):
    bank = {}
    hashes = []
    for name in ("GAME1.P3S", "GAME2.P3S"):
        data = (directory / name).read_bytes()
        hashes.append(f" * {name} SHA256 {hashlib.sha256(data).hexdigest()}")
        bank.update(shapes(unpack(data)))
    all_triangles = []
    model_ranges = []
    for model in range(37):
        start = len(all_triangles)
        polygons = [p for name in MODEL_SHAPES.get(model, ()) for p in bank[name]]
        if model == 22:
            polygons += [[(-z, y, x) for x, y, z in p] for p in bank["road"]]
        for polygon in polygons:
            all_triangles.extend(triangles(polygon))
        model_ranges.append((start, len(all_triangles) - start))
    lines = ["/* Generated by tools/scripts/generate-owoot-road-geometry.py.",
             " * Road surface triangles retain resource coordinates and heights.",
             *hashes, " * Do not edit the numeric table by hand. */", "",
             "static const struct OWOOT_ROAD_TRIANGLE far owoot_road_triangles[] = {"]
    for model, (start, count) in enumerate(model_ranges):
        if count:
            lines.append(f"\t/* Physical model {model}: {', '.join(MODEL_SHAPES[model])}. */")
        for triangle in all_triangles[start:start + count]:
            vertices = ", ".join("{" + ", ".join(map(str, vertex)) + "}" for vertex in triangle)
            lines.append("\t{{" + vertices + "}},")
    lines += ["};", "", "static const struct OWOOT_ROAD_MODEL owoot_road_models[] = {"]
    for start, count in model_ranges:
        lines.append(f"\t{{{start}, {count}}},")
    lines += ["};", ""]
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--game-directory", type=Path, default=ROOT / "stunts")
    parser.add_argument("--output", type=Path, default=ROOT / "src/restunts/c/owoot_road_data.h")
    parser.add_argument("--clang-format", default="clang-format")
    args = parser.parse_args()
    args.output.write_bytes(generate(args.game_directory).replace("\n", "\r\n").encode())
    subprocess.run([args.clang_format, f"--style=file:{ROOT / '.clang-format'}",
                    "-i", str(args.output)], check=True, cwd=ROOT)


if __name__ == "__main__":
    main()
