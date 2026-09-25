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
