#!/usr/bin/env python3
"""Prepare game-sized, lightly pixelated opponent photos in the original Stunts palette.

Requires Pillow. The high-resolution source portraits are never overwritten.
"""

import argparse
import hashlib
import json
from pathlib import Path
import runpy
import struct

from PIL import Image, __version__ as PILLOW_VERSION


ROOT = Path(__file__).resolve().parents[2]
# The 74 x 79 VGA-pixel photo interior has four samples per axis in SuperSight.
PHOTO_SIZE = (74, 79)
SCALE = 4
OUTPUT_SIZE = tuple(value * SCALE for value in PHOTO_SIZE)
PIXEL_SIZE = 2
OPPONENT_COUNT = 6


def digest(data):
    return hashlib.sha256(data).hexdigest()


def generate(repository, game_directory, source_directory):
    helpers = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))
    unpack = runpy.run_path(
        str(repository / "tools/scripts/generate-owoot-road-geometry.py")
    )["unpack"]
    packed_palette = (game_directory / "SDMAIN.PVS").read_bytes()
    palette_shape = helpers["resource_shapes"](unpack(packed_palette))["!pal"]
    header_size = helpers["SHAPE_HEADER_SIZE"]
    if len(palette_shape) != header_size + helpers["VGA_PALETTE_SIZE"]:
        raise ValueError("Expected a 768-byte VGA palette after its 16-byte header")
    palette_vga = palette_shape[header_size:]
    vga_max = helpers["VGA_COMPONENT_MAX"]
    palette_rgb = bytes((value & vga_max) * helpers["RGB_COMPONENT_MAX"] // vga_max
                        for value in palette_vga)
    palette_image = Image.new("P", (1, 1))
    palette_image.putpalette(palette_rgb)
    block_size = tuple(value // PIXEL_SIZE for value in OUTPUT_SIZE)
    manifest = {
        "generator": "tools/scripts/prepare-opponent-portraits.py",
        "pillow_version": PILLOW_VERSION,
        "palette_source": "SDMAIN.PVS:!pal",
        "palette_source_sha256": digest(packed_palette),
        "palette_vga_sha256": digest(palette_vga),
        "palette_rgb_sha256": digest(palette_rgb),
        "palette_conversion": "(component & 63) * 255 // 63",
        "width": OUTPUT_SIZE[0],
        "height": OUTPUT_SIZE[1],
        "pixel_block_size": PIXEL_SIZE,
        "processing": [
            "RGB bilinear downscale to 296 x 316 renderer samples",
            "RGB bilinear reduction to 148 x 158 for subtle pixelation",
            "Quantize to the original 256-entry game palette without dithering",
            "Nearest-neighbor expansion to 296 x 316 (2 x 2 sample blocks)",
        ],
        "images": [],
    }
    outputs = {}
    for number in range(1, OPPONENT_COUNT + 1):
        filename = f"opp{number}.png"
        source = source_directory / filename
        with Image.open(source) as original:
            source_size = original.size
            # All filtering occurs in RGB: filtering a P image would use nearest.
            resized = original.convert("RGB").resize(OUTPUT_SIZE, Image.Resampling.BILINEAR)
        softened = resized.resize(block_size, Image.Resampling.BILINEAR)
        indexed = softened.quantize(palette=palette_image, dither=Image.Dither.NONE)
        prepared = indexed.resize(OUTPUT_SIZE, Image.Resampling.NEAREST)
        pixels = prepared.tobytes()
        # Use the shared encoder to retain every palette entry, with no transparency.
        data = helpers["indexed_png"](*OUTPUT_SIZE, pixels, palette_rgb)
        outputs[filename] = data
        manifest["images"].append({
            "file": filename,
            "opponent": number,
            "source": f"../{filename}",
            "source_width": source_size[0],
            "source_height": source_size[1],
            "source_sha256": digest(source.read_bytes()),
            "sha256": digest(data),
            "pixels_sha256": digest(pixels),
            "palette_colors_used": len(set(pixels)),
        })
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--source-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--check", action="store_true", help="Verify existing outputs without writing")
    args = parser.parse_args()
    game_directory = args.game_directory or args.repository_root / "stunts"
    source_directory = args.source_directory or args.repository_root / "assets/opponents"
    output_directory = args.output_directory or source_directory / "game"
    try:
        if source_directory.resolve() == output_directory.resolve():
            raise ValueError("Output directory must differ from the high-resolution source directory")
        outputs = generate(args.repository_root, game_directory, source_directory)
        if not args.check:
            output_directory.mkdir(parents=True, exist_ok=True)
        for name, data in outputs.items():
            path = output_directory / name
            if args.check:
                if path.read_bytes() != data:
                    raise ValueError(f"Prepared portrait differs: {path}; regenerate the copies")
            else:
                path.write_bytes(data)
        action = "Verified" if args.check else "Prepared"
        print(f"{action} 6 palette-matched 296 x 316 portraits in {output_directory}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
