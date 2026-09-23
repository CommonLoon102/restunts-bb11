# Opponent portraits

The six photorealistic opponent photos are stored in
[`assets/opponents/`](../../assets/opponents/). They were generated with the
built-in imagegen tool from the corresponding original `SDOSEL.PVS` portraits,
using the `SDMAIN.PVS:!pal` palette. These full-resolution RGB PNGs are the
masters. The game now prefers the restored 2x comparison set in
[`assets/opponents/game2/`](../../assets/opponents/game2/). The palette-matched
photo set in [`assets/opponents/game/`](../../assets/opponents/game/) remains
available as a fallback for comparison. The original game resources remain
unchanged. Each asset folder has a manifest recording its dimensions and hashes.

| File | Opponent | Original shape |
| --- | --- | --- |
| `opp1.png` | Squealin' Bernie Rubber | `SDOSEL.PVS:opp1` |
| `opp2.png` | Herr Otto Partz | `SDOSEL.PVS:opp2` |
| `opp3.png` | Smokin' Joe Stallin | `SDOSEL.PVS:opp3` |
| `opp4.png` | Cherry Chassis | `SDOSEL.PVS:opp4` |
| `opp5.png` | Helen Wheels | `SDOSEL.PVS:opp5` |
| `opp6.png` | Skid Vicious | `SDOSEL.PVS:opp6` |

SDL3 builds display the enhanced photos only while SuperSight is enabled with
**F12**. F12 also switches the artwork while the opponent menu is open. If an
individual replacement cannot be loaded, that opponent keeps the original
portrait. Turning SuperSight off restores the original artwork for all six.
Clock and Ghost retain their original illustration.

The original portraits occupy 80 x 83 VGA pixels. The replacement photos are displayed
in the original portrait frame with approximately 4:5 proportions, accounting
for the game's vertical VGA pixel correction. The original white
border, numbered labels, and clipboard artwork remain part of the game UI;
the master and `game` files contain full-bleed photographs. The `game2`
files also preserve the original tile border and number, which the loader crops
out before displaying the photo interior beneath the original UI overlays.

The [generation prompts](generation-prompts.txt) record the exact request for
each photo. Each generation used only its matching original portrait as the
reference. Otto's final photo uses a [monocle correction](otto-monocle-correction.txt)
to restore the single eyepiece held by his raised hand, its retaining cord,
and his open lightweight jacket over a nearly black purple T-shirt. The
cord disappears under the jacket. Joe's [sunglasses correction](joe-sunglasses-correction.txt)
uses sharply squared upper corners, a heavy black frame, and one bulky
central bridge. Skid's [fresh regeneration](skid-original-regeneration.txt)
uses only his original game portrait as its visual reference, with the eyewear
and expression requests consolidated into the prompt. This replaces the earlier
sequence of edits to recover clean photographic detail. The first fresh
regeneration was retained at the user's request, then received a localized
eyewear correction: rounder, deeper brown lenses with a thin black upper rim
and no rim below the temple attachments.
No fallback CLI or API key was used.

## Original reference extraction

With the original game files in `stunts/`, export lossless indexed references
and verify them with:

```sh
python3 tools/scripts/extract-opponent-portraits.py
python3 tools/scripts/extract-opponent-portraits.py --check
```

The standard-library extractor reuses the existing Stunts resource decoder and
writes to `out/opponents/original/`. It does not overwrite the enhanced photos.
The export manifest includes shape headers, original pixel hashes, and the
exact number masks used to verify preservation.

## Restored original comparison set (`game2`)

Each `game2/oppN.png` is **160 x 166** pixels, exactly twice the width and height
of the original 80 x 83 tile (four times as many pixels). The photo interior is
148 x 158 at `(4,4)`. This set uses fresh reconstructions from the original game
portraits; the full-resolution masters and the `game` set remain unchanged.

One built-in imagegen call per opponent reconstructs a coherent photograph from
only that opponent's original 74 x 79 photo crop, preserving its pose, expression,
clothes and accessories while resolving missing detail. The number is removed
from the reference and restored exactly afterward. The
[exact prompts and source paths](game2-generation-prompts.json) record all six
initial calls and subsequent full-resolution corrections. Original pixel
speckles and digitization noise are excluded from the restoration. The base
photographs use only original game portraits as references; requested corrections
edit those retained full-resolution photographs before the same reduction.
Bernie's folded shirt collars are green. Otto has a gold monocle with a visible
single cord hanging vertically beside his cheek and curving over his shoulder,
an unzipped lightweight jacket over a black T-shirt, and closed puckered kiss lips.
The user selected this shoulder-cord version as the final portrait.

Preparation then follows the requested larger-to-smaller workflow:

1. Bilinear resize the reconstructed photograph into a 320 x 332 working tile,
   four times the original width and height, with a 296 x 316 photo interior.
2. Apply a gentle 3 x 3 median to the photo at that working size to remove isolated
   specks while retaining facial features and intentional clothing patterns.
3. Fill the connected flat backdrop with its exact original palette color.
   Closely matching enclosed backdrop regions, such as Otto's arm gap, are also
   filled. Masks follow the new photograph's silhouette.
4. Bilinear reduce the photo to half size, then map it to the exact game palette
   without dithering, added grain or sharpening.
5. Restore the original border and complete number rectangle as exact 2x copies.

The [game2 manifest](../../assets/opponents/game2/manifest.json) records source,
working-image and final hashes, background colors, dimensions and processing.
The [preserved source gallery](game2-sources/README.md) links all six selected
full-resolution photographs, 4x working tiles, and earlier correction variants.
These files are stored under `docs/opponents/game2-sources/` for future reference.
With Pillow and original game data, rebuild or verify the final copies using:

```sh
python3 tools/scripts/prepare-original-opponent-upscales.py
python3 tools/scripts/prepare-original-opponent-upscales.py --check
```

The script reads `docs/opponents/game2-sources/full-resolution/` and prepares
the 4x RGB tiles in `docs/opponents/game2-sources/working-4x/`. The preserved
images match their original generated files byte for byte; their hashes and
source paths are recorded alongside them. The build and installed game need
only the final six indexed PNGs, without Pillow or imagegen access.

## Game-ready photo copies (`game`)

The renderer uses a 74 x 79 VGA-pixel photo interior at four samples per axis,
so each prepared PNG is exactly **296 x 316** pixels. Preparation uses Pillow
(tested with 10.2.0) and the existing original-resource decoder:

1. Bilinear downscale the RGB master to 296 x 316.
2. Bilinear reduce to 148 x 158 for a light pixel treatment.
3. Map to the exact 256-entry `SDMAIN.PVS:!pal` palette without dithering.
4. Expand by nearest neighbor to 296 x 316, forming 2 x 2 sample blocks.

The blocks are half the width and height of an original VGA pixel in the
enhanced renderer. This keeps facial detail while matching the game's palette
and pixel style. The PNGs are indexed and fully opaque; the original number,
white border and clipboard remain separate overlays. No vertical aspect
correction is baked in: the game's final display handles it.

After changing a master photo, regenerate and verify all six copies with:

```sh
python3 -m pip install Pillow
python3 tools/scripts/prepare-opponent-portraits.py
python3 tools/scripts/prepare-opponent-portraits.py --check
```

The script reads `stunts/SDMAIN.PVS` and leaves the masters unchanged. `--check`
compares regenerated bytes and provenance with the saved copies without writing.
The manifest records the Pillow version and exact processing steps. Normal game
builds and runtime do not require Pillow.

## Runtime paths

The loader searches these artwork tiers in order: `game2/oppN.png`,
`game/oppN.png`, then full-resolution `oppN.png`. Within each tier it checks
`opponents/` in the game data directory, beside the executable, then this
checkout's `assets/opponents/` directory. Missing, invalid or incorrectly sized
files fall through to the next usable tier; if none exists, the original game
portrait remains visible. F12 still gates all replacements.

`game2` tiles must be 160 x 166. Their `(4,4,148,158)` interior is expanded by
nearest neighbor into the 296 x 316 enhanced photo area, preserving every palette
color. `game` files must be 296 x 316 and are used without resizing. The menu
continues to draw its original number, border and clipboard over either set.

CMake runtime installation copies both prepared sets to `bin/opponents/game2/`
and `bin/opponents/game/`, and masters to `bin/opponents/`. All are optional;
no extra original game resource files are required.
