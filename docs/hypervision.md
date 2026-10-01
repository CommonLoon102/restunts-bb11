# HyperVision

HyperVision is the enhanced renderer for SDL3 builds, including native
desktop platforms, 32-bit DOS, and WebAssembly. It replaces SDL3 SuperSight while
retaining its three internal resolutions, quality controls, visual interpolation,
skyboxes, ghosts, and shadows. The performance target is a 2 GHz Core 2 Duo.
That target is a hardware requirement to evaluate, not a measured FPS result.

F12 selects Auto or Off; Shift+F12 cycles Full, High, Medium, and Low. The preferred
startup options are `hv:full`, `hv:high`, `hv:medium`, and `hv:low`; the corresponding
`ss:` options remain aliases. Off uses the classic renderer. The Open Watcom
16-bit DOS build retains its existing SuperSight renderer and controls.

Enhanced horizon artwork is enabled at 640x400 and 1280x800. The 320x200 mode
uses the original horizon strips and disables car shadows. Banked and inverted
views work at every HyperVision resolution. Native-resolution panorama caches
replace repeated strip lookup while rotating the horizon. Enhanced artwork stays
cached across resolution changes. Classic mode and missing enhanced assets also
use the original horizon strips.

## Visibility architecture

The enhanced path separates scene preparation from software visibility and
shading. It submits projected polygons and material information without relying
on a back-to-front scene or primitive sort. The classic path retains the
original ordering and rendering rules.

The software backend resolves visibility along screen-space spans. A planar
polygon's inverse depth varies linearly across a scanline. Overlapping spans can
therefore be compared over intervals, splitting at depth crossings to identify
the nearer surface. Planar commands cache their depth equation for reuse across
scanlines; warped polygons retain edge interpolation. Intersecting surfaces do
not need a single global painter order; the visible surface can change within
the overlap.

The pipeline is:

1. Select scene geometry using the active quality preset and visibility mask.
   Transform and clip it into projected polygon/material commands.
2. Convert opaque polygon coverage into scanline spans and resolve their
   inverse-depth intersections. Submission order does not determine which opaque
   surface is visible.
3. Resolve solid decals and generated line/point coverage against those supports.
   An attached decal can retain its supporting surface's depth even when its
   authored paint lies slightly behind that surface. The flat-ground approximation
   clips generated coverage where there is no explicit supporting surface.
4. Shade the surviving solid intervals and write their depth values once.
   Covered opaque fragments avoid repeated shading and per-fragment depth tests.
5. Draw patterned surfaces with the depth checks needed for their individual
   covered samples. A grille hole cannot discard the surface behind it.
6. Use the resulting depth information for effects that require it, including
   shadow receivers, then compose the completed frame for presentation.

This is an analytic visibility renderer, not a painter's algorithm. It still
keeps depth information and performs per-sample checks where a material or effect
needs them. Visibility bookkeeping, crossings, depth writes, composition, and
special materials all have CPU costs; eliminating opaque overdraw alone does
not establish a whole-frame speedup.

The renderer permits small differences in clipping, edge coverage, depth ties,
and material appearance compared with SuperSight. The original image is useful
for visual review, but exact SuperSight pixel hashes are not the acceptance
criterion for the new enhanced path. The classic path retains its separate
pixel-parity requirements.

## Scene and backend boundary

The command boundary describes projected geometry, material selection, and depth
independently of the backend. CPU spans remain the default. Desktop builds also
include a Vulkan backend selected with F10; F10 again returns to CPU rendering.
The preset, current quality, and internal resolution stay the same when switching.
For a sustained comparison, lock a preset with Shift+F12 and use the second F11
mode to view average render time. VSync can cap displayed FPS even when rendering
gets faster; the timing line excludes that wait.
From classic mode, F10 first enables HyperVision with Vulkan. Initialization
failure leaves the current renderer and quality unchanged.

Vulkan uses SDL3's GPU API with the Vulkan driver explicitly selected. Projected
polygons are triangulated and rasterized with hardware depth testing. Concave
contours use ear clipping; self-crossing or touching contours are split into
even-odd trapezoids before GPU rasterization. Indexed color, coverage, full-width
surface family IDs, and physical inverse depth use
separate offscreen attachments. Completed support metadata is copied before
attached details sample it, avoiding simultaneous sampling and writing of the
same attachment. Patterns discard uncovered fragments; alternate materials
retain both palette colors.

Projection, enhanced horizons, shadow effects, sprite copies, and final framebuffer
composition remain on the CPU. A synchronized readback imports covered Vulkan
samples and depth into the existing companion buffers, preserving uncovered
artwork and the legacy interface. Resources and transfer buffers are retained
between frames. This allows the two visibility backends to share the same game
and presentation path, but the transfer and wait can outweigh GPU rasterization
savings. Benchmarks must include that cost.

Failed Vulkan submissions fall back to the retained CPU commands for the same
frame. Legacy incremental raster calls continue to use CPU depth tests; ordinary
game scenes submit a complete Vulkan batch. Backend switches happen between
joined frames and never change authoritative simulation state. Render-time history
resets on a switch, including automatic fallback.

Configure with `-DRESTUNTS_VULKAN=OFF` to omit the Vulkan implementation. Desktop
builds enable it by default; DOS and WebAssembly use CPU rendering. No Vulkan SDK
is needed to build the game because compiled SPIR-V is included. Regenerate it
with `tools/scripts/generate-hypervision-shaders.py`, glslang 15.1.0, and
`spirv-val`; `--check` verifies the checked-in header. Set
`RESTUNTS_VULKAN_DEBUG=1` to enable driver validation when its layers are installed.
The runtime requires a Vulkan driver compatible with SDL3; software Vulkan
drivers can also be selected explicitly with F10 for comparison.

## Physics, interpolation, and presentation

Authoritative simulation, input sampling, and replay recording remain at their
original 10 or 20 Hz rate. HyperVision targets 60 presented frames per second by
interpolating completed physics states using the existing timing rules. Rendered
positions, rotations, suspension, cameras, and ghosts share the visual sample
time. Interpolated values must never become the next authoritative physics state
or enter recorded replay data.

Late rendering skips overdue visual samples without predicting beyond the newest
completed state. Seeking, pausing, rewinding, and camera changes reset the visual
history. Switching renderer, resolution, quality, worker count, or VSync must not
change an old replay's simulated result.

The original renderer also leaves values that later physics consumes. Those
compatibility effects remain represented explicitly by the game; replacing the
visible enhanced draw must not discard or overwrite them. See the existing
[renderer parity notes](renderer-parity.md#renderer-values-consumed-by-physics)
for the historical behavior and its regression coverage.

Desktop VSync remains enabled by default. `RESTUNTS_VSYNC=0` disables it and
`RESTUNTS_VSYNC=1` enables it explicitly on the supported desktop SDL3 platforms.
Display synchronization waits are excluded from automatic quality decisions.
The 32-bit DOS and browser builds retain their platform presentation mechanisms.
VSync does not alter physics timing or create a different replay format.

## Performance measurement

Measure the actual renderer using identical scene states, camera paths,
resolutions, build settings, and hardware. The supported internal resolutions
remain 1280x800, 640x400, and 320x200. Use locked presets during comparisons so
automatic quality changes do not make a slower run appear faster by reducing
the workload. Compare equivalent geometry and visual features at each resolution.

Useful coverage includes cockpit and external cameras, dense and sparse tracks,
crossing geometry, shadows, patterned surfaces, ghosts, and moving views. Include
both the 10 Hz and 20 Hz replay schedules in compatibility testing. Keep classic
pixel parity and authoritative replay-state comparisons as separate checks from
enhanced visual review.

For repeatable CPU measurements:

- Use Release builds with the same compiler flags and instruction-set baseline.
  Record the CPU, operating system, build revision, affinity, and worker count.
- Warm up resources and caches before collecting measurements. Alternate baseline
  and candidate batches to reduce bias from CPU frequency and host load.
- Report elapsed frame time and total process CPU time separately. Worker CPU time
  belongs in the process total, even when wall time improves.
- Disable VSync and frame pacing for uncapped rendering comparisons. Measure
  display synchronization separately during a normal interactive run.
- State which phases each harness includes: scene preparation, visibility,
  shading, shadows, composition, upload, display, physics, audio, and UI. A focused
  span benchmark cannot establish end-to-end game FPS.
- Report several runs and their variation. Inspect representative enhanced frames
  for missing objects, bad intersections, incorrect holes, shadows, and clipping.

A virtual-machine or modern-host result is useful comparative evidence but does
not demonstrate 60 FPS on a Core 2 Duo. Confirm the target on the intended machine
before making a hardware FPS claim. Record regressions and workload limitations
alongside improvements.

### Running the benchmark

In a native Linux, BSD, or macOS Release build configured with
`RESTUNTS_BUILD_TESTS=ON`, build the optional target explicitly:

```sh
cmake --build out/sdl3-linux-x64 --target benchmark-hypervision --parallel 2
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy RESTUNTS_RENDER_WORKERS=0 \
    out/sdl3-linux-x64/benchmark-hypervision --data-dir stunts DEFAULT 120
```

Replace the build directory and replay name as needed. The final argument is the
number of measured frames per case; omit it for 120. Each case warms up for eight
frames, then reports mean process CPU time and mean, median, and 95th-percentile
elapsed time in milliseconds per frame. Cases use cockpit and external cameras
at replay ticks 0, 80, and 160 (or a shorter replay's final tick), at all three
resolutions with full-track geometry held fixed. Lower resolutions here isolate
raster cost; they do not select the reduced visibility masks of the normal presets.
The output reports the actual background worker count. Repeat with
`RESTUNTS_RENDER_WORKERS=1` to assess the second Core 2 Duo core.

This batch harness includes scene rendering, framebuffer copies, and checks that
rendering preserves simulation state. Add `--compose` to include final ARGB
framebuffer composition. Display upload, VSync, audio playback, frame pacing,
and physics advancement remain outside the timed loop. It does not measure
complete interactive FPS. The target is excluded from ordinary builds and is
not a CTest timing gate.

Use `--backend cpu` or `--backend vulkan` to require a particular backend. Vulkan
measurements include triangulation, submission, rasterization, synchronized
readback, and import; any fallback fails the run. Use the same `--compose` setting
for both backends. On a headless Linux host, Vulkan needs a video driver such as
`offscreen`; SDL's `dummy` video driver cannot load Vulkan:

```sh
SDL_VIDEODRIVER=offscreen SDL_AUDIODRIVER=dummy RESTUNTS_RENDER_WORKERS=0 \
    out/sdl3-linux-x64/benchmark-hypervision --data-dir stunts \
    --backend vulkan --compose DEFAULT 120
```

The output identifies the device, driver, hardware-acceleration status, and
composition setting. Disable validation layers during timing; run them separately
for correctness checks.

### Workers

Existing desktop render workers remain opt-in: `RESTUNTS_RENDER_WORKERS=0` is the
default, values `1` through `7` request background workers, and `auto` chooses a
count from the detected logical CPUs. Small workloads can remain serial. DOS
renders serially; worker creation failure retains the available-worker or serial
fallback.

Compare serial execution with one background worker on the two-core target.
Retain parallel work only when its complete-frame elapsed-time benefit outweighs
scheduling, synchronization, and extra CPU cost. Results from the previous
SuperSight worker implementation are historical evidence, not a performance
claim for HyperVision.

### Results

A paired Linux Release comparison on 2026-10-01 used GCC 13.3 (`-O3`, the same
instruction-set baseline), a KVM guest with three virtual CPUs reporting an
Intel Core i5-4690 at 3.50 GHz, and no CPU affinity pinning. It used the DEFAULT
replay at ticks 0, 80, and 160, with cockpit and external views and full-track
geometry at each resolution. Each case warmed up before measuring 60 frames.
Two runs of each configuration alternated in this order: SuperSight serial,
HyperVision serial, HyperVision with one worker, then the reverse order.

Mean process CPU milliseconds per frame, averaged across the six scenes and
both runs:

| Resolution | SuperSight, serial | HyperVision, serial | CPU reduction | HyperVision, one worker |
| --- | ---: | ---: | ---: | ---: |
| 320x200 | 3.003 | 2.862 | 4.7% | 2.851 |
| 640x400 | 4.592 | 3.908 | 14.9% | 3.948 |
| 1280x800 | 9.222 | 6.926 | 24.9% | 7.064 |

The two HyperVision serial run means ranged from 2.752 to 2.973 ms at 320x200,
3.770 to 4.046 ms at 640x400, and 6.744 to 7.107 ms at 1280x800. Individual scenes
and elapsed-time tails varied with guest scheduling; some low-resolution cases
regressed. These measurements preceded the switch back to original horizon
artwork at 320x200: that row measures HyperVision with enhanced horizons against
SuperSight with original strips. Higher-resolution results are unaffected.

One background worker reduced mean elapsed time from 7.163 to 6.277 ms at
1280x800 (12.4%), with slightly higher process CPU time. At 640x400 elapsed time
changed from 3.984 to 3.652 ms; at 320x200 the difference was small, 2.928 to
2.875 ms. Workers therefore remain opt-in, with a serial default.

These measurements include scene rendering, shadows, frame copies, and replay
state checks; they exclude display upload/composition, VSync, audio, pacing,
and physics advancement. They establish a renderer CPU improvement on this
host, not interactive FPS or 60 FPS on the Core 2 Duo target. No target machine
was available for this change. The old renderer library was preserved before
implementation from revision `246d11fd4155567b3f8e3e475809b2b9e8b42fef`; its game
and SDL3 sources match the pre-change HEAD `91049fac6`. The benchmark source is
available in
[`benchmark-hypervision.c`](../src/restunts/benchmarks/benchmark-hypervision.c).
The [SuperSight measurements](supersight-performance.md) remain the record of
previous experiments.

### CPU optimization follow-up

The follow-up compares against the first HyperVision commit, `492d4770`, with
original horizons at 320x200 in both builds. The same host, compiler, replay,
18 scene/resolution cases, and 60-frame batches were used. Each worker setting
(0, 1, 2) was measured in baseline/candidate order and then in reverse order.

| Resolution | First HyperVision CPU ms | Optimized CPU ms | Reduction |
| --- | ---: | ---: | ---: |
| 320x200 | 2.761 | 2.358 | 14.6% |
| 640x400 | 3.994 | 3.396 | 15.0% |
| 1280x800 | 7.310 | 6.835 | 6.5% |

The changes batch complete skybox rows, emit opaque indexed spans without
repeating overlay checks for every cell, cache planar inverse-depth equations,
remove unused primitive mean-depth work, and reject wholly offscreen primitives
against frustum planes before projection. Skybox rows can also use the existing worker
pool. Jobs own disjoint logical rows and combine overlay-retirement counts only
after joining; unusual aliased sprite rows retain the serial fallback.

Shape preparation remains serial: its shared transform and queue state would
require substantial restructuring, while its measured cost was much smaller
than shading, skybox output, and shadows. Shadow traversal also remains serial.

At 1280x800 the optimized serial elapsed mean was 7.299 ms, versus 7.116 ms with
one worker and 7.531 ms with two workers. Process CPU rose from 6.835 ms to
7.279 and 7.539 ms. Scheduling variation is too large to establish a consistent
worker benefit here, so the default remains serial. All 18 captured views match
byte-for-byte between zero and two workers. The high-resolution serial elapsed
comparison (7.333 to 7.299 ms) is also within this VM's timing noise; the table
reports process CPU time, not a guaranteed FPS increase.

### CPU versus Vulkan

The final comparison used the same Linux Release executable and host described
above, with Vulkan provided by **software llvmpipe**, LLVM 20.1.2 (256 bits),
Mesa 25.2.8. No physical Vulkan GPU was available. Both backends used SDL's
`offscreen` video driver, zero application render workers, `--compose`, and a
frozen UI clock. Validation layers, VSync, and pacing were disabled. The order
was CPU, Vulkan, Vulkan, CPU, with eight warmup frames and 60 measured frames
for each of the 18 cases. All 72 case records completed without fallback or crash.

Mean milliseconds per frame across both runs and six scenes at each resolution:

| Resolution | CPU elapsed | Vulkan elapsed | CPU process time | Vulkan process time |
| --- | ---: | ---: | ---: | ---: |
| 320x200 | 2.366 | 10.497 | 2.321 | 13.200 |
| 640x400 | 3.594 | 15.692 | 3.559 | 20.162 |
| 1280x800 | 6.859 | 29.394 | 6.780 | 40.096 |

Software Vulkan is substantially slower on this host. GPU submission, wait,
readback, and import are included, as are CPU scene preparation, horizons,
shadows, frame copies, composition, and state checks. Display upload and physics
advancement are excluded. These results do not predict performance on a physical
GPU or a Core 2 Duo; CPU remains the default. Unlike the earlier CPU optimization
table, this comparison includes final composition, so the two tables have
different measurement scopes.

A candidate using interpolated hardware depth for earlier rejection passed the
small GPU fixtures but produced extensive road and grass striping in real scenes.
It was rejected before timing; the shipped shaders retain explicit fragment-depth
calculation for consistent coplanar surfaces.

## Validation

`test-hypervision-vulkan` runs actual GPU commands. CTest skips it when no usable
Vulkan driver is available; `--require-vulkan` makes that an error. The replay
harness also rejects any unexpected CPU fallback:

```sh
SDL_VIDEODRIVER=offscreen out/sdl3-linux-x64/test-hypervision-vulkan --require-vulkan
python3 tools/scripts/test-render-replay.py \
    out/sdl3-linux-x64/test-render-replay stunts --backend vulkan
python3 tools/scripts/test-render-replay.py \
    out/sdl3-linux-x64/test-render-replay stunts --backend vulkan --toggle-backends-only
```

The native Linux Release build and optional benchmark build pass. All 24 CTest
suites pass, including actual Vulkan execution with no skipped suites, host
regressions, projection/visibility, worker consistency, horizon artwork, inverted
sampling, track previews, interpolation, and presentation controls.

The replay harness compares complete authoritative state and RNG values with
classic rendering, HyperVision CPU or Vulkan, repeated F12 switching, and
adaptive quality changes. The checked ranges produce identical records:

| Replay | Physics rate | Ticks checked | State/RNG records |
| --- | ---: | ---: | ---: |
| DEFAULT | 10 Hz | 0–240 | 241 |
| DEFCRSH | 20 Hz | 0–312, complete replay | 313 |
| 0A0A | 20 Hz | 0–240 | 241 |
| HARDLAND | 20 Hz | 0–356, complete replay | 357 |
| SHAKING | 20 Hz | 0–1265, complete replay | 1266 |

Extra interpolated frames cover ordinary playback, a hard landing, and a loop
exit without changing physics. Actual F10 CPU/Vulkan transitions also match
classic rendering for DEFAULT ticks 0–60 and HARDLAND ticks 0–330, including
interpolated hard-landing samples. This coverage does not claim full playback of
DEFAULT or 0A0A. Eighteen ordinary and eighteen inverted CPU/Vulkan frame pairs
were compared; the largest pixel difference was under 0.05%. Enhanced horizons
were visually confirmed in inverted 640x400 and 1280x800 views.

The Vulkan fixtures pass Khronos validation for crossing depth, background
surfaces, concave and self-crossing contours, patterned and alternate colors,
attached decals, full-width surface families, partial overlay preservation,
shadows, resizing, and deliberate CPU fallback followed by Vulkan recovery.
Real scene smoke tests also exit cleanly with validation enabled. Normal and
fatal shutdown paths release the renderer before loaded driver exit handlers.

One plain Release SHAKING run with F12 switching exited with SIGSEGV after its
rendering loop completed. The cause remains unknown: the same case subsequently
exited cleanly in a plain Release run, under the debugger, with Vulkan validation,
and with renderer AddressSanitizer instrumentation. All retained repeat outputs
match the classic state/RNG records. This is an unresolved intermittent cleanup
failure, not a confirmed fix or an observed replay desynchronization.

The CPU visibility backend, geometry suites, Vulkan backend, and companion-buffer
import pass AddressSanitizer and UndefinedBehaviorSanitizer with leak detection.
A separate 64-vertex crossing-contour stress test exercises tessellation bounds.
The long SHAKING toggle run also passes renderer AddressSanitizer and leak checks.
Source formatting, EditorConfig, CRLF, and whitespace checks pass. Windows, DOS,
and WebAssembly cross-builds were not run in this environment.
