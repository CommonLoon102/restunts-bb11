#!/usr/bin/env python3
"""Prepare restored menu backgrounds at 4x, then nearest-neighbor reduce to 2x."""

import argparse
import hashlib
from io import BytesIO
import json
from pathlib import Path
import runpy
import struct

from PIL import Image, __version__ as PILLOW_VERSION


ROOT = Path(__file__).resolve().parents[2]
WORKING_SCALE = 4
FINAL_SCALE = 2


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load_patches(patch_directory, filenames):
    if patch_directory is None:
        return {}
    manifest = json.loads((patch_directory / "manifest.json").read_text(encoding="utf-8"))
    if not isinstance(manifest, dict) or not isinstance(manifest.get("images"), dict):
        raise ValueError("Patch manifest must contain an images object")
    patches = manifest["images"]
    for filename, patch in patches.items():
        if filename not in filenames:
            raise ValueError(f"Unknown menu background in patch manifest: {filename}")
        fields = ("base_working_pixels_sha256", "patch_file", "patch_sha256")
        if not isinstance(patch, dict) or any(not isinstance(patch.get(field), str)
                                              for field in fields):
            raise ValueError(f"Invalid patch entry: {filename}")
        if not patch["patch_file"] or Path(patch["patch_file"]).name != patch["patch_file"]:
            raise ValueError(f"Patch file must be a filename within the patch directory: {filename}")
    return patches


def apply_patch(working, patch_directory, patch, repository):
    if digest(working.tobytes()) != patch["base_working_pixels_sha256"]:
        raise ValueError(f"Patch base working pixels differ: {patch['patch_file']}")
    patch_path = patch_directory / patch["patch_file"]
    patch_data = patch_path.read_bytes()
    if digest(patch_data) != patch["patch_sha256"]:
        raise ValueError(f"Patch file hash differs: {patch_path}")
    with Image.open(BytesIO(patch_data)) as overlay:
        if overlay.format != "PNG" or overlay.mode != "RGBA" or overlay.size != working.size:
            raise ValueError(f"Patch must be an RGBA PNG at working dimensions {working.size}: {patch_path}")
        working = Image.alpha_composite(working.convert("RGBA"), overlay).convert("RGB")
    try:
        provenance_path = patch_path.resolve().relative_to(repository.resolve()).as_posix()
    except ValueError:
        provenance_path = str(patch_path.resolve())
    provenance = dict(patch, patch_file=provenance_path)
    return working, provenance


def generate(repository, game_directory, generated_directory, patch_directory=None):
    extractor = runpy.run_path(str(repository / "tools/scripts/extract-menu-backgrounds.py"))
    originals = extractor["generate"](repository, game_directory)
    provenance = json.loads(originals["manifest.json"])
    patches = load_patches(patch_directory, {image["file"] for image in provenance["backgrounds"]})
    encode = runpy.run_path(str(repository / "tools/scripts/extract-skyboxes.py"))["indexed_png"]
    manifest = {
        "generator": "tools/scripts/prepare-menu-backgrounds.py",
        "detail_generation": "built-in imagegen, faithful photorealistic pixel-art restoration",
        "prompts": "docs/menus/generation-prompts.json",
        "pillow_version": PILLOW_VERSION,
        "working_scale": WORKING_SCALE,
        "final_scale": FINAL_SCALE,
        "palette_source": provenance["palette_source"],
        "palette_rgb_sha256": provenance["palette_rgb_sha256"],
        "palette_conversion": provenance["palette_conversion"],
        "processing": [
            "Nearest-neighbor normalize generated restoration to 4x original width and height",
            "Nearest-neighbor reduce to 2x original width and height (one quarter pixel count)",
            "Map to the exact original game palette without dithering or filtering",
        ],
        "images": [],
    }
    if patches:
        manifest["processing"].insert(1, "Alpha-composite verified RGBA corrections at exact working dimensions")
    outputs = {}
    working_outputs = {}
    for reference in provenance["backgrounds"]:
        filename = reference["file"]
        source = generated_directory / filename
        with Image.open(BytesIO(originals[filename])) as original:
            original_size = original.size
            palette = bytes(original.getpalette())
        working_size = tuple(dimension * WORKING_SCALE for dimension in original_size)
        final_size = tuple(dimension * FINAL_SCALE for dimension in original_size)
        with Image.open(source) as generated:
            generated_size = generated.size
            working = generated.convert("RGB").resize(working_size, Image.Resampling.NEAREST)
        patch_provenance = None
        if filename in patches:
            working, patch_provenance = apply_patch(working, patch_directory, patches[filename], repository)
        reduced = working.resize(final_size, Image.Resampling.NEAREST)
        palette_image = Image.new("P", (1, 1))
        palette_image.putpalette(palette)
        reduced = reduced.quantize(palette=palette_image, dither=Image.Dither.NONE)
        pixels = reduced.tobytes()
        output_data = encode(*final_size, pixels, palette)
        working_buffer = BytesIO()
        working.save(working_buffer, format="PNG")
        working_data = working_buffer.getvalue()
        outputs[filename] = output_data
        working_outputs[filename] = working_data
        manifest["images"].append({
            "file": filename,
            "original": f"{reference['source']}:{reference['resource']}",
            "original_size": original_size,
            "generated_size": generated_size,
            "working_size": working_size,
            "final_size": final_size,
            "original_png_sha256": digest(originals[filename]),
            "generated_restoration_sha256": digest(source.read_bytes()),
            "working_png_sha256": digest(working_data),
            "sha256": digest(output_data),
            "pixels_sha256": digest(pixels),
        })
        if patch_provenance is not None:
            manifest["images"][-1]["patch"] = patch_provenance
    outputs["manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode("utf-8")
    return outputs, working_outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository-root", type=Path, default=ROOT)
    parser.add_argument("--game-directory", type=Path)
    parser.add_argument("--generated-directory", type=Path)
    parser.add_argument("--patch-directory", type=Path,
                        help="Directory with manifest.json and optional working-size RGBA corrections")
    parser.add_argument("--working-directory", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--check", action="store_true", help="Recompute and compare without writing")
    args = parser.parse_args()
    game = args.game_directory or args.repository_root / "stunts"
    sources = args.repository_root / "docs/menus/sources"
    generated = args.generated_directory or sources / "full-resolution"
    patches = args.patch_directory
    if patches is None and generated.resolve() == (sources / "full-resolution").resolve():
        if (sources / "patches/manifest.json").is_file():
            patches = sources / "patches"
    working = args.working_directory or sources / "working-4x"
    destination = args.output_directory or args.repository_root / "assets/menus"
    try:
        directories = [path.resolve() for path in (generated, working, destination)]
        if patches is not None:
            directories.append(patches.resolve())
        if len(set(directories)) != len(directories):
            raise ValueError("Generated, working, output and patch directories must differ")
        outputs, working_outputs = generate(args.repository_root, game, generated, patches)
        for directory, files in ((destination, outputs), (working, working_outputs)):
            if not args.check:
                directory.mkdir(parents=True, exist_ok=True)
            for name, data in files.items():
                path = directory / name
                if args.check:
                    if path.read_bytes() != data:
                        raise ValueError(f"Restored menu background differs: {path}")
                else:
                    path.write_bytes(data)
        action = "Verified" if args.check else "Prepared"
        print(f"{action} {len(working_outputs)} restored 2x menu backgrounds in {destination}")
        print(f"4x working images: {working}")
    except (OSError, ValueError, KeyError, IndexError, struct.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
