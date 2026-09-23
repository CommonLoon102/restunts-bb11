#!/usr/bin/env python3
"""Extract the six original opponent portraits and provenance using only Python's standard library."""

import argparse
import hashlib
import json
from pathlib import Path
import runpy
import struct


ROOT = Path(__file__).resolve().parents[2]
OPPONENTS = (
    ("Squealin' Bernie Rubber", "bernie-rubber", 23),
    ("Herr Otto Partz", "otto-partz", 52),
    ("Smokin' Joe Stallin", "joe-stallin", 29),
    ("Cherry Chassis", "cherry-chassis", 24),
    ("Helen Wheels", "helen-wheels", 24),
    ("Skid Vicious", "skid-vicious", 34),
)
DIGIT_RECT = (66, 4, 6, 9)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def shape_metadata(data):
    width, height, anchor_x, anchor_y, x, y = struct.unpack_from("<HHhhHH", data)
    return {
        "width": width,
        "height": height,
        "anchor_x": anchor_x,
        "anchor_y": anchor_y,
        "position_x": x,
        "position_y": y,
        "plane_flags": list(data[12:16]),
        "header_hex": data[:16].hex(),
        "shape_sha256": digest(data),
    }


def generate(repository, directory):
    # Share the checked resource, VGA ordering, and PNG helpers with skyboxes.
    helpers = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))
    unpack = runpy.run_path(
        str(repository / "tools/scripts/generate-owoot-road-geometry.py")
    )["unpack"]
    resources = helpers["resource_shapes"]
    bitmap = helpers["bitmap"]
    indexed_png = helpers["indexed_png"]

    packed_palette = (directory / "SDMAIN.PVS").read_bytes()
    palette_shape = resources(unpack(packed_palette))["!pal"]
    if len(palette_shape) != 784:
        raise ValueError("Expected a 768-byte VGA palette after its 16-byte header")
    palette_vga = palette_shape[16:]
    palette_rgb = bytes((value & 63) * 255 // 63 for value in palette_vga)
    packed_source = (directory / "SDOSEL.PVS").read_bytes()
    unpacked_source = unpack(packed_source)
    shapes = resources(unpacked_source)
    outputs = {}
    manifest = {
        "source": "SDOSEL.PVS",
        "source_sha256": digest(packed_source),
        "unpacked_source_sha256": digest(unpacked_source),
        "palette_source": "SDMAIN.PVS:!pal",
        "palette_source_sha256": digest(packed_palette),
        "palette_vga_sha256": digest(palette_vga),
        "palette_rgb_sha256": digest(palette_rgb),
        "palette_conversion": "(component & 63) * 255 // 63",
        "photo_interior": {"x": 2, "y": 2, "width": 74, "height": 79},
        "white_border_palette_index": 15,
        "black_right_columns": [78, 79],
        "digit_palette_index": 39,
        "digit_rgb": list(palette_rgb[39 * 3:40 * 3]),
        "digit_search_rectangle": dict(zip(("x", "y", "width", "height"), DIGIT_RECT)),
        "opponents": [],
    }
    for number, (name, slug, age) in enumerate(OPPONENTS, 1):
        shape_id = f"opp{number}"
        shape = shapes[shape_id]
        width, height, pixels = bitmap(shape)
        if (width, height) != (80, 83):
            raise ValueError(f"Unexpected dimensions for {shape_id}: {width}x{height}")
        filename = f"{shape_id}.png"
        outputs[filename] = indexed_png(width, height, pixels, palette_rgb)
        digit_pixels = [
            [x, y] for y in range(DIGIT_RECT[1], DIGIT_RECT[1] + DIGIT_RECT[3])
            for x in range(DIGIT_RECT[0], DIGIT_RECT[0] + DIGIT_RECT[2])
            if pixels[y * width + x] == 39
        ]
        manifest["opponents"].append({
            "number": number,
            "name": name,
            "slug": slug,
            "age": age,
            "resource": shape_id,
            "file": filename,
            **shape_metadata(shape),
            "pixels_sha256": digest(pixels),
            "png_sha256": digest(outputs[filename]),
            "digit_pixels": digit_pixels,
        })
    clip = shapes["clip"]
    width, height, pixels = bitmap(clip)
    outputs["clip.png"] = indexed_png(width, height, pixels, palette_rgb)
    manifest["clip"] = {
        "resource": "clip",
        "file": "clip.png",
        **shape_metadata(clip),
        "transparent_palette_index": 255,
        "pixels_sha256": digest(pixels),
        "png_sha256": digest(outputs["clip.png"]),
    }
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--check", action="store_true", help="Verify existing outputs without writing")
    args = parser.parse_args()
    game_directory = args.game_directory or args.repository_root / "stunts"
    output_directory = args.output_directory or args.repository_root / "out/opponents/original"
    try:
        outputs = generate(args.repository_root, game_directory)
        if not args.check:
            output_directory.mkdir(parents=True, exist_ok=True)
        for name, data in outputs.items():
            path = output_directory / name
            if args.check:
                if path.read_bytes() != data:
                    raise ValueError(f"Extraction differs: {path}")
            else:
                path.write_bytes(data)
        action = "Verified" if args.check else "Extracted"
        print(f"{action} 6 opponent portraits, clipboard overlay, and metadata in {output_directory}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
