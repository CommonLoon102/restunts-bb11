# Enhanced menu backgrounds

The enhanced main menu, car showroom and opponent-selection backgrounds are
faithful photorealistic pixel-art restorations made with the built-in imagegen
tool from the original game images.
The runtime PNGs use the exact `SDMAIN.PVS:!pal` palette without dithering.

| Background | Original resource | Original | 4x working image | Final game image |
| --- | --- | --- | --- | --- |
| [Main menu](../../assets/menus/main.png) | `SDMSEL.PVS:scrn` | 320 x 200 | 1280 x 800 | 640 x 400 |
| [Showroom](../../assets/menus/showroom.png) | `SDCSEL.PVS:stop` | 320 x 103 | 1280 x 412 | 640 x 206 |
| [Opponent selection](../../assets/menus/opponent.png) | `SDOSEL.PVS:scrn` | 320 x 182 | 1280 x 728 | 640 x 364 |

The main menu retains the original scene, signs, labels and clickable layout.
The showroom asset contains only the empty upper stage. The game draws the
rotating car, shadow, optional opponent portrait, and lower controls separately.
The opponent-selection background preserves the clipboard, empty biography paper,
stopwatch and desk objects. Its portrait, biography text and bottom button row
remain separate game elements. The portrait's metal clip is still a separate
overlay; enhanced mode samples its artwork from the matching background region
while preserving the original transparency mask, so the clip meets the background
without a seam. Classic mode retains the original clip artwork. No VGA display
aspect correction is baked into the images.

SDL3 builds use these backgrounds when HyperVision or SuperSight enables the
enhanced renderer, including when F12 changes mode while a menu is open.
Classic mode uses the original game images. Missing, invalid, or incorrectly
sized replacement PNGs fall through to the next asset location; if none loads,
the original background remains visible. Original game resource files are not
modified. The original 16-bit DOS build keeps its original artwork.

## Extraction and preparation

With the original Broderbund Stunts 1.1 files in `stunts/`, extract the indexed
references and their source hashes into the ignored `out/menus/original/`:

```sh
python3 tools/scripts/extract-menu-backgrounds.py
python3 tools/scripts/extract-menu-backgrounds.py --check
```

The [exact generation prompts](generation-prompts.json) preserve the intended
composition, materials, labels and exclusions. The unmodified generated images
are retained in [`sources/full-resolution/`](sources/full-resolution/).
Imagegen returned larger images; preparation normalizes them to exactly four
times the original width and height using nearest neighbor, retaining those
working images in [`sources/working-4x/`](sources/working-4x/). Optional RGBA
correction patches are applied at these exact working dimensions before
reduction. Transparent pixels retain the generated image unchanged.

The working images are then reduced with **nearest neighbor** to half their
width and height: from 16 times the original pixel count to 4 times. Palette
mapping follows reduction, without dithering, blur, sharpening or other filters.
The [asset manifest](../../assets/menus/manifest.json) records dimensions,
processing, and source, working and final hashes.

With Pillow installed, regenerate or verify both working and game images:

```sh
python3 tools/scripts/prepare-menu-backgrounds.py
python3 tools/scripts/prepare-menu-backgrounds.py --check
```

Optional patches live in `sources/patches/`, with this `manifest.json` schema:

```json
{
  "images": {
    "main.png": {
      "base_working_pixels_sha256": "SHA-256 of normalized RGB pixel bytes",
      "patch_file": "main-corrections.png",
      "patch_sha256": "SHA-256 of correction PNG file bytes"
    }
  }
}
```

Each entry names one full-canvas RGBA PNG within the patch directory, using the
working dimensions in the table above. Any background may have an entry; omit an
entry to preserve that generated image. The base hash is computed after
converting the generated source to RGB and normalizing to working dimensions,
before applying a patch. Preparation verifies both hashes, RGBA mode and exact
dimensions, then alpha-composites the patch. The asset manifest records the patch
path and hashes for each corrected image. The archived working image includes
these corrections, while the full-resolution generated source remains unchanged.

The default patch manifest is used only with `sources/full-resolution/` input.
A custom `--generated-directory` does not inherit these corrections. To apply
patches to custom input, supply `--patch-directory PATH` explicitly; its manifest
must match that input's normalized pixel hashes. A missing or incompatible
explicit patch manifest fails preparation. An absent default manifest leaves
all images unpatched.

Normal builds need only the final PNGs, without Pillow or imagegen access.
At runtime, nearest sampling keeps the authored 2x pixels crisp at each render
scale. The loader searches `menus/` in the game directory, beside the executable,
then the configured source asset directory. Native packages install the images
in `bin/menus/`; browser builds embed them in `/assets/menus/` and include them
in the relink package. The generated masters and working images are not shipped.
