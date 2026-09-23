#!/usr/bin/env python3
"""Extract the five original VGA skyboxes as lossless indexed PNGs.

Each scene contains four horizon images, in scen/sce2/sce3/sce4 order.
The accompanying panorama joins them at their common bottom edge, exactly
as the renderer does. PNG palette entries use the SDL renderer's conversion
from six-bit VGA components to eight-bit RGB. No image library is required.

Exports default to the ignored out/skyboxes/original directory.
Run with --check to verify existing exports and metadata without writing.
"""

import argparse
import hashlib
import json
from pathlib import Path
import runpy
import struct
import zlib


ROOT = Path(__file__).resolve().parents[2]
SCENES = ("desert", "tropical", "alpine", "city", "country")
SHAPES = ("scen", "sce2", "sce3", "sce4")
SKY_COLOR = 116


def digest(data):
    return hashlib.sha256(data).hexdigest()


def resource_shapes(data):
    if len(data) < 6 or struct.unpack_from("<I", data)[0] != len(data):
        raise ValueError("Invalid resource size")
    count = struct.unpack_from("<H", data, 4)[0]
    base = 6 + count * 8
    if base > len(data):
        raise ValueError("Truncated resource table")
    entries = []
    for index in range(count):
        name = data[6 + index * 4:10 + index * 4].decode("ascii")
        offset = base + struct.unpack_from("<I", data, 6 + count * 4 + index * 4)[0]
        if offset < base or offset + 16 > len(data):
            raise ValueError(f"Invalid resource offset: {name}")
        entries.append((offset, name))
    entries.sort()
    result = {}
    for index, (offset, name) in enumerate(entries):
        end = entries[index + 1][0] if index + 1 < len(entries) else len(data)
        if name in result:
            raise ValueError(f"Duplicate resource name: {name}")
        result[name] = data[offset:end]
    return result


def bitmap(data):
    width, height = struct.unpack_from("<HH", data)
    if width == 0 or height == 0 or len(data) < 16 + width * height:
        raise ValueError("Truncated or empty VGA bitmap")
    pixels = data[16:16 + width * height]
    if data[15] & 240:
        raise ValueError("Planar VGA bitmap is not supported")
    flip = data[14] >> 4
    if flip > 3:
        raise ValueError(f"Unknown VGA bitmap ordering: {flip}")
    if flip:
        output = bytearray(width * height)
        for y in range(height):
            for x in range(width):
                if flip == 1:
                    source = x * height + y
                elif flip == 2:
                    source = x * height + (y // 2 if y % 2 == 0 else (height + y) // 2)
                elif y % 2 == 0:
                    source = x * ((height + 1) // 2) + y // 2
                else:
                    source = width * ((height + 1) // 2) + x * (height // 2) + y // 2
                output[y * width + x] = pixels[source]
        pixels = bytes(output)
    return width, height, pixels


def png_chunk(kind, payload):
    return (struct.pack(">I", len(payload)) + kind + payload
            + struct.pack(">I", zlib.crc32(kind + payload)))


def indexed_png(width, height, pixels, palette):
    rows = b"".join(b"\0" + pixels[y * width:(y + 1) * width] for y in range(height))
    return (b"\x89PNG\r\n\x1a\n"
            + png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 3, 0, 0, 0))
            + png_chunk(b"PLTE", palette)
            + png_chunk(b"IDAT", zlib.compress(rows, 9))
            + png_chunk(b"IEND", b""))


def generate(directory):
    # Share the existing Stunts RLE/VLE decoder with the geometry extractor.
    unpack = runpy.run_path(str(ROOT / "tools/scripts/generate-owoot-road-geometry.py"))["unpack"]
    source_palette = (directory / "SDMAIN.PVS").read_bytes()
    palette_shape = resource_shapes(unpack(source_palette))["!pal"]
    if len(palette_shape) != 784:
        raise ValueError("Expected a 768-byte VGA palette after the 16-byte shape header")
    palette_vga = palette_shape[16:]
    palette_rgb = bytes((value & 63) * 255 // 63 for value in palette_vga)
    outputs = {}
    manifest = {
        "palette_source": "SDMAIN.PVS:!pal",
        "palette_source_sha256": digest(source_palette),
        "palette_vga_sha256": digest(palette_vga),
        "palette_rgb_sha256": digest(palette_rgb),
        "palette_conversion": "(component & 63) * 255 // 63",
        "sky_palette_index": SKY_COLOR,
        "panorama_width": 1024,
        "scenes": [],
    }
    for selector, scene in enumerate(SCENES):
        source_name = scene.upper() + ".PVS"
        packed = (directory / source_name).read_bytes()
        resources = resource_shapes(unpack(packed))
        images = [bitmap(resources[name]) for name in SHAPES]
        panorama_height = max(image[1] for image in images)
        if [image[0] for image in images] != [320, 192, 320, 192]:
            raise ValueError(f"Unexpected panorama image widths: {source_name}")
        panorama = bytearray([SKY_COLOR]) * (1024 * panorama_height)
        entry = {
            "name": scene,
            "selector": selector,
            "source": source_name,
            "source_sha256": digest(packed),
            "panorama": f"{scene}.png",
            "width": 1024,
            "height": panorama_height,
            "images": [],
        }
        x = 0
        for shape_name, (width, height, pixels) in zip(SHAPES, images):
            filename = f"{scene}-{shape_name}.png"
            outputs[filename] = indexed_png(width, height, pixels, palette_rgb)
            y = panorama_height - height
            for row in range(height):
                offset = (y + row) * 1024 + x
                panorama[offset:offset + width] = pixels[row * width:(row + 1) * width]
            entry["images"].append({
                "resource": shape_name,
                "file": filename,
                "width": width,
                "height": height,
                "scenery_pixels": sum(value != SKY_COLOR for value in pixels),
                "panorama_x": x,
                "panorama_y": y,
                "header_hex": resources[shape_name][:16].hex(),
                "pixels_sha256": digest(pixels),
                "png_sha256": digest(outputs[filename]),
            })
            x += width
        outputs[entry["panorama"]] = indexed_png(1024, panorama_height, panorama, palette_rgb)
        entry["panorama_pixels_sha256"] = digest(panorama)
        entry["panorama_png_sha256"] = digest(outputs[entry["panorama"]])
        manifest["scenes"].append(entry)
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--game-directory", type=Path, default=ROOT / "stunts")
    parser.add_argument("--output-directory", type=Path, default=ROOT / "out/skyboxes/original")
    parser.add_argument("--check", action="store_true", help="Verify outputs without writing")
    args = parser.parse_args()
    try:
        outputs = generate(args.game_directory)
        if not args.check:
            args.output_directory.mkdir(parents=True, exist_ok=True)
        for name, data in outputs.items():
            path = args.output_directory / name
            if args.check:
                if path.read_bytes() != data:
                    raise ValueError(f"Extraction differs: {path}")
            else:
                path.write_bytes(data)
        action = "Verified" if args.check else "Extracted"
        print(f"{action} 20 original skybox images, 5 panoramas, and metadata in "
              f"{args.output_directory}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
