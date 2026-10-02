#!/usr/bin/env python3
"""Prepare restored opponent photos at 4x, then reduce to 2x palette art (requires Pillow)."""

import argparse
from collections import Counter, deque
import hashlib
from io import BytesIO
import json
from pathlib import Path
import runpy
from statistics import median
import struct

from PIL import Image, ImageFilter, __version__ as PILLOW_VERSION


ROOT = Path(__file__).resolve().parents[2]
PHOTO = (2, 2, 76, 81)
NUMBER = (66, 4, 72, 13)
WORKING_SCALE = 4
SCALE = 2
BACKGROUND_TOLERANCE = 16
BACKGROUND_BUCKET_SIZE = 8
BACKGROUND_CORE_TOLERANCE = 6
BACKGROUND_HOLE_MIN_PIXELS = 16
BACKGROUND_HOLE_CORE_FRACTION = 0.8
RGB_CHANNELS = 3
MASK_SELECTED = 255
MEDIAN_FILTER_SIZE = 3
DEFAULT_RESAMPLING = "bilinear"
RESAMPLING_FILTERS = {
    "bilinear": Image.Resampling.BILINEAR,
    "nearest": Image.Resampling.NEAREST,
}
PROCESSING_FILENAME = "processing.json"


def digest(data):
    return hashlib.sha256(data).hexdigest()


def background_mask(photo):
    """Find the new photo's flat backdrop without imposing the old silhouette."""
    width, height = photo.size
    colors = list(photo.getdata())
    edge = [colors[x] for x in range(width)]
    edge += [colors[y * width + x] for y in range(height) for x in (0, width - 1)]
    buckets = Counter(tuple(channel // BACKGROUND_BUCKET_SIZE for channel in color) for color in edge)
    dominant = buckets.most_common(1)[0][0]
    samples = [color for color in edge
               if tuple(channel // BACKGROUND_BUCKET_SIZE for channel in color) == dominant]
    backdrop = tuple(round(median(color[channel] for color in samples))
                     for channel in range(RGB_CHANNELS))
    distances = [max(abs(a - b) for a, b in zip(color, backdrop)) for color in colors]
    candidates = bytearray(distance <= BACKGROUND_TOLERANCE for distance in distances)
    selected = bytearray(len(colors))
    enclosed_pixels = 0
    for start in range(len(colors)):
        if not candidates[start]:
            continue
        candidates[start] = 0
        queue = deque([start])
        component = []
        touches_edge = False
        while queue:
            point = queue.popleft()
            component.append(point)
            y, x = divmod(point, width)
            touches_edge |= x in (0, width - 1) or y in (0, height - 1)
            for ny in range(max(0, y - 1), min(height, y + 2)):
                for nx in range(max(0, x - 1), min(width, x + 2)):
                    neighbor = ny * width + nx
                    if candidates[neighbor]:
                        candidates[neighbor] = 0
                        queue.append(neighbor)
        # Enclosed backdrop holes must have a near-exact, flat-color core.
        # This avoids swallowing similarly colored clothing or highlights.
        close = sum(distances[point] <= BACKGROUND_CORE_TOLERANCE for point in component)
        enclosed = (len(component) >= BACKGROUND_HOLE_MIN_PIXELS
                    and close >= len(component) * BACKGROUND_HOLE_CORE_FRACTION)
        if touches_edge or enclosed:
            for point in component:
                selected[point] = MASK_SELECTED
            if not touches_edge:
                enclosed_pixels += len(component)
    mask = Image.frombytes("L", photo.size, bytes(selected))
    return mask, backdrop, enclosed_pixels


def tile_with_photo(original, photo, scale):
    output = original.resize((original.width * scale, original.height * scale),
                             Image.Resampling.NEAREST)
    if photo.mode == "RGB":
        output = output.convert("RGB")
    output.paste(photo, (PHOTO[0] * scale, PHOTO[1] * scale))
    # The authored number and its entire background rectangle remain exact.
    digit_size = ((NUMBER[2] - NUMBER[0]) * scale, (NUMBER[3] - NUMBER[1]) * scale)
    digit = original.crop(NUMBER).resize(digit_size, Image.Resampling.NEAREST)
    output.paste(digit.convert(output.mode) if output.mode == "RGB" else digit,
                 (NUMBER[0] * scale, NUMBER[1] * scale))
    return output


def load_processing(path, filenames):
    """Validate optional per-image resizing filters in a source archive."""
    processing = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(processing, dict):
        raise ValueError(f"Processing configuration must map image filenames to filters: {path}")
    for filename, resampling in processing.items():
        if filename not in filenames:
            raise ValueError(f"Unknown opponent image in processing configuration: {filename}")
        if not isinstance(resampling, str) or resampling not in RESAMPLING_FILTERS:
            allowed = ", ".join(RESAMPLING_FILTERS)
            raise ValueError(f"Unsupported resampling for {filename}: expected {allowed}")
    return processing


def restore(original, generated, resampling=DEFAULT_RESAMPLING):
    photo = original.crop(PHOTO)
    background = Counter(photo.getdata()).most_common(1)[0][0]
    palette = original.getpalette()
    background_rgb = tuple(palette[background * RGB_CHANNELS:(background + 1) * RGB_CHANNELS])
    working_size = (photo.width * WORKING_SCALE, photo.height * WORKING_SCALE)
    resize_filter = RESAMPLING_FILTERS[resampling]
    working = generated.convert("RGB").resize(working_size, resize_filter)
    # A one-working-pixel median removes isolated specks at one quarter of an
    # original pixel, before reduction. It does not reintroduce source noise.
    working = working.filter(ImageFilter.MedianFilter(MEDIAN_FILTER_SIZE))
    mask, estimated_background, enclosed = background_mask(working)
    working.paste(background_rgb, mask=mask)
    reduced_size = (photo.width * SCALE, photo.height * SCALE)
    reduced = working.resize(reduced_size, resize_filter)
    reduced_mask = mask.resize(reduced_size, resize_filter)
    solid = reduced_mask.point(lambda value: MASK_SELECTED if value == MASK_SELECTED else 0)
    reduced.paste(background_rgb, mask=solid)
    palette_image = Image.new("P", (1, 1))
    palette_image.putpalette(palette)
    reduced = reduced.quantize(palette=palette_image, dither=Image.Dither.NONE)
    reduced.paste(background, mask=solid)
    output = tile_with_photo(original, reduced, SCALE)
    working_tile = tile_with_photo(original, working, WORKING_SCALE)
    stats = {
        "background_palette_index": background,
        "background_rgb": list(background_rgb),
        "estimated_generated_background_rgb": list(estimated_background),
        "working_background_pixels": sum(value == MASK_SELECTED for value in mask.getdata()),
        "working_enclosed_background_pixels": enclosed,
        "final_solid_background_pixels": sum(value == MASK_SELECTED for value in solid.getdata()),
    }
    return output, working_tile, stats


def generate(repository, game_directory, generated_directory, processing_config=None):
    extractor = runpy.run_path(str(repository / "tools/scripts/extract-opponent-portraits.py"))
    originals = extractor["generate"](repository, game_directory)
    filenames = [f"opp{number}.png" for number in range(1, len(extractor["OPPONENTS"]) + 1)]
    default_config = generated_directory.parent / PROCESSING_FILENAME
    config_path = processing_config or default_config
    processing = (load_processing(config_path, filenames)
                  if processing_config is not None or config_path.exists() else {})
    provenance = json.loads(originals["manifest.json"])
    encode = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))["indexed_png"]
    manifest = {
        "generator": "tools/scripts/prepare-original-opponent-upscales.py",
        "detail_generation": "built-in imagegen, fresh photographs from original photo crops only",
        "prompts": "docs/opponents/game-regeneration-prompts.json",
        "pillow_version": PILLOW_VERSION,
        "width": extractor["PORTRAIT_SIZE"][0] * SCALE,
        "height": extractor["PORTRAIT_SIZE"][1] * SCALE,
        "original_width": extractor["PORTRAIT_SIZE"][0],
        "original_height": extractor["PORTRAIT_SIZE"][1],
        "working_width": extractor["PORTRAIT_SIZE"][0] * WORKING_SCALE,
        "working_height": extractor["PORTRAIT_SIZE"][1] * WORKING_SCALE,
        "working_scale": WORKING_SCALE,
        "scale": SCALE,
        "photo_rectangle": {"x": PHOTO[0] * SCALE, "y": PHOTO[1] * SCALE,
                            "width": (PHOTO[2] - PHOTO[0]) * SCALE,
                            "height": (PHOTO[3] - PHOTO[1]) * SCALE},
        "palette_source": provenance["palette_source"],
        "palette_source_sha256": provenance["palette_source_sha256"],
        "palette_rgb_sha256": provenance["palette_rgb_sha256"],
        "palette_conversion": provenance["palette_conversion"],
        "background_max_channel_distance": BACKGROUND_TOLERANCE,
        "processing": [
            "Resize fresh original-only photographic restorations to 296 x 316 "
            "using each image's recorded resampling filter",
            "Apply a gentle 3 x 3 median at 4x scale to remove isolated pixel noise",
            "Normalize connected flat backdrops and verified near-exact enclosed color holes",
            "Reduce the cleaned 4x photo and backdrop mask to half size, 148 x 158, "
            "using the same recorded resampling filter",
            "Quantize to the exact original game palette without dithering or added grain",
            "Preserve the original frame and entire number rectangle by exact 2x duplication",
        ],
        "images": [],
    }
    outputs = {}
    working_outputs = {}
    for number, filename in enumerate(filenames, 1):
        resampling = processing.get(filename, DEFAULT_RESAMPLING)
        generated_path = generated_directory / filename
        with Image.open(BytesIO(originals[filename])) as original:
            with Image.open(generated_path) as restored:
                output, working_tile, stats = restore(original, restored, resampling)
            palette = bytes(original.getpalette())
        pixels = output.tobytes()
        data = encode(*output.size, pixels, palette)
        working_buffer = BytesIO()
        working_tile.save(working_buffer, format="PNG")
        working_data = working_buffer.getvalue()
        outputs[filename] = data
        working_outputs[filename] = working_data
        manifest["images"].append({
            "file": filename,
            "opponent": number,
            "original": f"SDOSEL.PVS:opp{number}",
            "resampling": resampling,
            "original_png_sha256": digest(originals[filename]),
            "generated_restoration_sha256": digest(generated_path.read_bytes()),
            "working_png_sha256": digest(working_data),
            **stats,
            "sha256": digest(data),
            "pixels_sha256": digest(pixels),
        })
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs, working_outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--generated-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--working-directory", type=Path)
    parser.add_argument("--processing-config", type=Path,
                        help="Image-to-filter JSON; defaults to processing.json beside the source folder")
    parser.add_argument("--check", action="store_true", help="Recompute and compare without writing")
    args = parser.parse_args()
    game = args.game_directory or args.repository_root / "stunts"
    sources = args.repository_root / "docs/opponents/game-sources"
    generated = args.generated_directory or sources / "full-resolution"
    destination = args.output_directory or args.repository_root / "assets/opponents/game"
    working = args.working_directory or (
        generated / "4x" if args.generated_directory is not None else sources / "working-4x")
    try:
        directories = [path.resolve() for path in (generated, destination, working)]
        if len(set(directories)) != len(directories):
            raise ValueError("Generated, working and output directories must differ")
        outputs, working_outputs = generate(args.repository_root, game, generated,
                                            args.processing_config)
        for directory, files in ((destination, outputs), (working, working_outputs)):
            if not args.check:
                directory.mkdir(parents=True, exist_ok=True)
            for name, data in files.items():
                path = directory / name
                if args.check:
                    if path.read_bytes() != data:
                        raise ValueError(f"Restored portrait differs: {path}")
                else:
                    path.write_bytes(data)
        action = "Verified" if args.check else "Prepared"
        print(f"{action} six restored 160 x 166 palette tiles in {destination}")
        print(f"4x working tiles: {working}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
