# SuperSight CPU optimization measurements

These measurements guide the SDL3 renderer changes made on 2026-09-25.
Resolution remains 1280 x 800. Skybox artwork and sampling quality are preserved.

## Method and limits

Tests use the local Linux VM, a Release build, serial rendering, fixed replay
states/cameras, warmup, and alternating baseline/candidate runs. CPU time and
elapsed time are reported separately. The scene harness includes rendering and
final ARGB composition, but excludes authoritative physics, dashboard/UI work,
SDL texture upload, display, audio, and frame pacing. Results are measurements of
these workloads, not a guarantee of 60 FPS on other machines.

Host scheduling and CPU frequency varied enough to reverse some comparisons.
A lower whole-frame time alone does not justify an optimization when its focused
CPU measurement regresses. Exact output hashes constrain lossless changes;
intentional geometry changes have separate coverage and error tests.

## Polygon drawing: 46a37af5

Complex polygons use active-edge scan conversion; triangles and quads keep their
short direct path. Solid spans dispatch once before their sample loop. An earlier
solid-fill variant was retried after the host workload was reduced, then refined
to avoid penalizing patterned spans.

Targeted production draws used approximately 10.1% less CPU for spheres, 5.2% for
wheels, 2.7% for small quads, and 8.5% for a mixed case, with identical pixels.
The short three-scene aggregate changed from 17.646 to 17.795 ms of CPU time per
frame, so it did not establish an overall frame-rate improvement.

## Direct texture composition: 959170c0

Compose directly into the locked SDL texture instead of assembling an intermediate
framebuffer and then copying or expanding it. Existing surface/readback APIs remain.
All final pixels matched in 20 comparison batches, including palette fades,
indexed fallback, black shadows, and a translucent full-color fixture.

| Workload | Before CPU ms | After CPU ms |
| --- | ---: | ---: |
| Indexed composition | 2.464 | 1.680 |
| Shadow composition | 4.063 | 1.639 |
| Full-color composition fixture | 4.823 | 2.258 |
| Cockpit scene including composition | 14.260 | 13.502 |
| External scene including shadows/composition | 17.692 | 15.532 |

## Projected roundness: 7b197f99

Wheels and spheres use 8, 16, 32, or 64 perimeter segments according to projected
size. Conservative axis bounds limit the geometric outline change to less than
0.25 high-resolution pixel. Nearby and background shapes retain 64 segments.

| Projected diameter | Sphere CPU reduction | Wheel CPU reduction |
| --- | ---: | ---: |
| 4 pixels | 58% | 69% |
| 16 pixels | 34% | 52% |
| 64 pixels | 15% | 26% |

Changed small-shape pixels stayed beside existing silhouette/material edges.
Both 320-pixel fixtures remained byte-identical. Large-wheel timing reversed on
retry, so it establishes neither an improvement nor a regression.

## Shadow simplification: evaluated, not retained

Two alternatives were implemented and tested with automatic overload detection,
recovery headroom, and backoff after failed recovery attempts. Both preserved
per-pixel receiver/depth checks and retained full sampling for tiny shadows.

Sharing one opacity across valid same-surface 2 x 2 screen blocks added enough
bookkeeping to outweigh the saved filtering. Removing division/modulo from its
write loop left the start scene approximately flat and the midpoint shadow pass
32.2% slower. This implementation was removed.

Nearest sampling of the existing cached model silhouette was much smaller, but
separate-process measurements remained noisy. A final within-process test used
320 identical frames per external view, with 80 full/nearest/nearest/full groups.
Controller changes were outside the measured shadow work.

| Shadow pass | Full CPU ms | Nearest CPU ms | Paired delta 95% interval, ms |
| --- | ---: | ---: | ---: |
| Start external view | 1.4117 | 1.3971 | -0.0964 to +0.0687 |
| Midpoint external view | 1.2703 | 1.2064 | -0.1992 to +0.0570 |

Only 44 and 45 of the 80 paired groups improved, respectively. Both intervals
include slowdowns; unrelated frame work drifted by comparable amounts. These
results do not establish a useful saving, so automatic shadow quality reduction
and its controller were removed. The existing full-quality shadows remain.

All experimental normal-detail frames matched the preceding commit exactly, and
indexed pixels and depth arrays remained unchanged. Tests also covered excluded
receivers, height/depth boundaries, clipped and tiny shadows, model-less fallback,
and the controller's overload/recovery/reset behavior before the experiment was
removed. No FPS or quality setting was added for these rejected alternatives.

## Tile and shape preparation

Shapes rejected by the existing screen-bounds check skip directional visibility
preparation. Accepted shapes also skip that work when their existing distance
gate cannot require a visibility sector. The projection, clipping, and visibility
rules themselves remain the same.

SuperSight builds a coordinate-to-sorted-index table while copying its 900-tile
lookahead list. Multi-tile coverage and car-wheel placement reuse this table
instead of repeatedly scanning the list. The table is rebuilt for each camera
view, so there is no cross-frame cache to invalidate after editing/loading a
track. Classic lookahead and wrapped signed-byte camera offsets retain the
original scans. Marker eligibility, wheel ties, and tile ordering are preserved.

All 12 moving-view comparisons matched legacy, indexed, and final ARGB pixels
across 96 frames per version, including five camera modes, a rolled cockpit,
two replays, and classic controls. Whole-scene CPU timing was inconclusive.

A focused comparison used the actual preceding and candidate code, including
per-frame sorting, lookup construction, and both cars' wheel searches across
16 changing camera poses. All four paired comparisons improved and produced
identical tile/car checksums.

| Preparation workload | Before CPU microseconds | After CPU microseconds |
| --- | ---: | ---: |
| Sparse single-tile map | 35.58 | 33.31 |
| Dense map with 225 four-tile elements | 124.48 | 33.10 |
| One offscreen shape transform | 0.573 | 0.546 |
| One behind-camera shape transform | 0.463 | 0.419 |

These are preparation costs, not complete frame times. The dense fixture shows
the benefit of avoiding repeated scans; sparse-map savings are much smaller.

## Level skybox sampling

An exactly level horizon reuses wrapped source columns across rows and source
row pointers across columns. The original arithmetic and texel selection are
preserved, including native artwork, original-resolution fallback, missing
strips, clipping, altitude, and inverted views. Banked views retain the general
renderer. Artwork, resolution, and sampling quality do not change.

Twelve new full-image oracle cases use independent integer coordinates to check
half-pixel horizons, inverted normals, altitude, clipping, missing/narrow strips,
and one-unit banks that must continue through the general renderer. All pass,
and the legacy framebuffer remains unchanged.

Nine before/after fixtures matched byte for byte. Textured level fixtures
contained 538,880 artwork pixels, so they exercised source sampling extensively.

| Skybox workload | Before CPU ms | After CPU ms |
| --- | ---: | ---: |
| Level, enhanced artwork | 4.975 | 0.722 |
| Level, original artwork | 5.566 | 0.724 |
| Inverted, enhanced artwork | 4.476 | 0.754 |
| Banked control | 5.675 | 5.721 |
| Solid lowest-detail control | 0.578 | 0.387 |

The level artwork cases used 83-87% less CPU; the general banked path was within
observed timing drift. Three complete scene comparisons also matched output,
but their timing changed in opposite directions (including a faster banked
control), so they do not establish a complete-frame improvement.
