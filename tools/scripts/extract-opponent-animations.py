#!/usr/bin/env python3
"""Extract original opponent win/lose animation frames as lossless indexed PNGs.

Exports default to out/opponent-animations/<opponent>/<win|lose>/opNN.png.
Win and lose describe the opponent's result. Each bitmap is exported once,
in resource ID order, at its original size and with the game's VGA palette.
The frames are opaque rectangles; no scaling or display aspect correction
is applied. The manifest records source hashes and original shape headers.

Only Python's standard library is required. Run from any directory, or use
--game-directory and --output-directory to override the repository defaults.
Run with --check to verify existing exports and metadata without writing.
"""

import argparse
import json
from pathlib import Path
import re
import runpy
import struct


ROOT = Path(__file__).resolve().parents[2]
OUTCOMES = ("win", "lose")
FRAME_RESOURCE_PATTERN = re.compile(r"op[0-9]{2}")
PALETTE_FILENAME = "SDMAIN.PVS"
PALETTE_RESOURCE = "!pal"


def generate(repository, directory):
    # Reuse the same resource decoder, VGA ordering, palette conversion,
    # opponent names, and shape metadata as the existing reference exports.
    helpers = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))
    portraits = runpy.run_path(str(repository / "tools/scripts/extract-opponent-portraits.py"))
    unpack = runpy.run_path(
        str(repository / "tools/scripts/generate-owoot-road-geometry.py")
    )["unpack"]
    resources = helpers["resource_shapes"]
    bitmap = helpers["bitmap"]
    indexed_png = helpers["indexed_png"]
    digest = helpers["digest"]
    shape_metadata = portraits["shape_metadata"]

    packed_palette = (directory / PALETTE_FILENAME).read_bytes()
    palette_shape = resources(unpack(packed_palette))[PALETTE_RESOURCE]
    header_size = helpers["SHAPE_HEADER_SIZE"]
    if len(palette_shape) != header_size + helpers["VGA_PALETTE_SIZE"]:
        raise ValueError("Expected a 768-byte VGA palette after its 16-byte header")
    palette_vga = palette_shape[header_size:]
    vga_max = helpers["VGA_COMPONENT_MAX"]
    palette_rgb = bytes((value & vga_max) * helpers["RGB_COMPONENT_MAX"] // vga_max
                        for value in palette_vga)
    outputs = {}
    manifest = {
        "palette_source": f"{PALETTE_FILENAME}:{PALETTE_RESOURCE}",
        "palette_source_sha256": digest(packed_palette),
        "palette_vga_sha256": digest(palette_vga),
        "palette_rgb_sha256": digest(palette_rgb),
        "palette_conversion": "(component & 63) * 255 // 63",
        "outcome_perspective": "opponent",
        "frame_order": "Resource ID order; each bitmap is exported once, not playback repeats",
        "opponents": [],
    }
    for number, (name, slug, _age) in enumerate(portraits["OPPONENTS"], 1):
        opponent = {"number": number, "name": name, "slug": slug, "animations": {}}
        for outcome in OUTCOMES:
            source_name = f"OPP{number}{outcome.upper()}.PVS"
            packed_source = (directory / source_name).read_bytes()
            unpacked_source = unpack(packed_source)
            shapes = resources(unpacked_source)
            frame_ids = sorted(name for name in shapes if FRAME_RESOURCE_PATTERN.fullmatch(name))
            if not frame_ids:
                raise ValueError(f"No opponent animation frames found in {source_name}")
            animation = {
                "source": source_name,
                "source_sha256": digest(packed_source),
                "unpacked_source_sha256": digest(unpacked_source),
                "non_frame_resources": sorted(set(shapes) - set(frame_ids)),
                "frames": [],
            }
            for frame_id in frame_ids:
                shape = shapes[frame_id]
                width, height, pixels = bitmap(shape)
                filename = f"{slug}/{outcome}/{frame_id}.png"
                outputs[filename] = indexed_png(width, height, pixels, palette_rgb)
                animation["frames"].append({
                    "resource": frame_id,
                    "file": filename,
                    **shape_metadata(shape),
                    "pixels_sha256": digest(pixels),
                    "png_sha256": digest(outputs[filename]),
                })
            opponent["animations"][outcome] = animation
        manifest["opponents"].append(opponent)
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--check", action="store_true",
                        help="Verify existing outputs without writing")
    args = parser.parse_args()
    game_directory = args.game_directory or args.repository_root / "stunts"
    output_directory = args.output_directory or args.repository_root / "out/opponent-animations"
    try:
        outputs = generate(args.repository_root, game_directory)
        for name, data in outputs.items():
            path = output_directory / name
            if args.check:
                if path.read_bytes() != data:
                    raise ValueError(f"Extraction differs: {path}")
            else:
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
        frame_count = sum(name.endswith(".png") for name in outputs)
        action = "Verified" if args.check else "Extracted"
        print(f"{action} {frame_count} opponent animation frames and metadata in "
              f"{output_directory}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
