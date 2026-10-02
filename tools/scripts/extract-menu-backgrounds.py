#!/usr/bin/env python3
"""Extract the original main-menu, showroom and opponent VGA backgrounds losslessly.

Exports default to the ignored out/menus/original directory. The showroom
resource contains the upper painted scene only; the game draws its lower
controls, car preview, and optional opponent portrait separately. The opponent
background ends above its buttons; its portrait and biography are also separate.
"""

import argparse
import hashlib
import json
from pathlib import Path
import runpy
import struct


ROOT = Path(__file__).resolve().parents[2]
BACKGROUNDS = (
    ("main", "SDMSEL.PVS", "scrn", (320, 200)),
    ("showroom", "SDCSEL.PVS", "stop", (320, 103)),
    ("opponent", "SDOSEL.PVS", "scrn", (320, 182)),
)
PALETTE_SOURCE = "SDMAIN.PVS"
PALETTE_RESOURCE = "!pal"
SHAPE_GEOMETRY_FORMAT = "<HHhhHH"
SHAPE_FLAGS_OFFSET = struct.calcsize(SHAPE_GEOMETRY_FORMAT)
SHAPE_HEADER_SIZE = SHAPE_FLAGS_OFFSET + 4


def digest(data):
    return hashlib.sha256(data).hexdigest()


def generate(repository, directory):
    helpers = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))
    unpack = runpy.run_path(
        str(repository / "tools/scripts/generate-owoot-road-geometry.py")
    )["unpack"]
    resources = helpers["resource_shapes"]
    packed_palette = (directory / PALETTE_SOURCE).read_bytes()
    palette_shape = resources(unpack(packed_palette))[PALETTE_RESOURCE]
    if len(palette_shape) != SHAPE_HEADER_SIZE + helpers["VGA_PALETTE_SIZE"]:
        raise ValueError("Expected a 768-byte VGA palette after its shape header")
    palette_vga = palette_shape[SHAPE_HEADER_SIZE:]
    palette_rgb = bytes(
        (value & helpers["VGA_COMPONENT_MAX"]) * helpers["RGB_COMPONENT_MAX"]
        // helpers["VGA_COMPONENT_MAX"]
        for value in palette_vga
    )
    outputs = {}
    manifest = {
        "palette_source": f"{PALETTE_SOURCE}:{PALETTE_RESOURCE}",
        "palette_source_sha256": digest(packed_palette),
        "palette_vga_sha256": digest(palette_vga),
        "palette_rgb_sha256": digest(palette_rgb),
        "palette_conversion": "(component & 63) * 255 // 63",
        "backgrounds": [],
    }
    for name, source_name, resource_id, expected_size in BACKGROUNDS:
        packed_source = (directory / source_name).read_bytes()
        unpacked_source = unpack(packed_source)
        shape = resources(unpacked_source)[resource_id]
        width, height, pixels = helpers["bitmap"](shape)
        if (width, height) != expected_size:
            raise ValueError(
                f"Unexpected dimensions for {source_name}:{resource_id}: {width}x{height}"
            )
        _, _, anchor_x, anchor_y, position_x, position_y = struct.unpack_from(
            SHAPE_GEOMETRY_FORMAT, shape
        )
        filename = f"{name}.png"
        outputs[filename] = helpers["indexed_png"](width, height, pixels, palette_rgb)
        manifest["backgrounds"].append({
            "name": name,
            "source": source_name,
            "resource": resource_id,
            "file": filename,
            "width": width,
            "height": height,
            "anchor_x": anchor_x,
            "anchor_y": anchor_y,
            "position_x": position_x,
            "position_y": position_y,
            "plane_flags": list(shape[SHAPE_FLAGS_OFFSET:SHAPE_HEADER_SIZE]),
            "header_hex": shape[:SHAPE_HEADER_SIZE].hex(),
            "source_sha256": digest(packed_source),
            "unpacked_source_sha256": digest(unpacked_source),
            "shape_sha256": digest(shape),
            "pixels_sha256": digest(pixels),
            "png_sha256": digest(outputs[filename]),
        })
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--check", action="store_true", help="Verify outputs without writing")
    args = parser.parse_args()
    game_directory = args.game_directory or args.repository_root / "stunts"
    output_directory = args.output_directory or args.repository_root / "out/menus/original"
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
        print(f"{action} {len(BACKGROUNDS)} menu backgrounds and metadata in {output_directory}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
