# Vulkan renderer

Windows and Linux SDL3 builds provide an optional Vulkan renderer selected with
**F10**. It keeps SuperSight's 1280x800 3D resolution, complete track draw distance,
shadow and contact-shading settings, enhanced artwork, and 60 FPS presentation
schedule. Physics and replay recording keep their original timing.

| Current renderer | F10, when Vulkan is available | F12 | Shift+F12 |
| --- | --- | --- | --- |
| Classic | Vulkan | CPU without shadows | CPU with shadows |
| CPU without shadows | Vulkan | Classic | CPU with shadows |
| CPU with shadows | Vulkan | CPU without shadows | Classic |
| Vulkan | Classic | CPU without shadows | CPU with shadows |

Vulkan always includes shadows. Both CPU modes retain the same enhanced resolution,
draw distance, and artwork. Disabling CPU shadows skips their frame capture and
shading; cached static lighting remains available for an immediate switch back.

The original renderer is selected at startup. F10 has no effect when the startup
probe fails. The shortcuts work while driving, in replays, the nighttime intro,
both car-selection screens, the opponent menu, and the track preview. F10 retains
its category shortcut in the track editor. DOS builds continue to use their
existing renderers.

## Hardware and startup detection

A working Vulkan graphics driver is required. SDL's Vulkan GPU backend requires
Vulkan 1.0 and its documented device features and extensions; see the
[SDL GPU requirements](https://wiki.libsdl.org/SDL3/CategoryGPU#system-requirements).
The game probes support once after video initialization by creating a Vulkan GPU
device and its compute pipelines. Finding a Vulkan loader library alone does not
enable F10. Failed initialization leaves the original and CPU SuperSight renderers
available.

Windows 11 and Ubuntu 24 with a GTX 970 and Haswell CPU are the intended target
configuration. Actual availability depends on the installed driver and device
exposed to the process. A virtual machine may expose only a virtual display
adapter, even when its host has a capable GPU. Software Vulkan devices are excluded
by default because running these shaders on the CPU does not provide GPU offload.

The game uses C and SDL3's
[GPU API](https://wiki.libsdl.org/SDL3/SDL_CreateGPUDevice), explicitly selecting
its Vulkan driver. No C++ renderer, ray-tracing hardware, or Vulkan SDK installation
is needed to run the game. Compiled SPIR-V shaders are included in the build.

## Work moved to the GPU

The CPU retains scene traversal, projection, clipping, and pixel-coverage span
generation. Those spans preserve the existing renderer's drawing order, edge
rules, palette materials, grille patterns, and attached-surface depth behavior.
Vulkan compute shaders perform per-pixel rasterization and depth testing, followed
by shadow and soft contact shading. The work performed by the CPU raster workers
is therefore executed on the GPU for Vulkan batches.

Static lighting still uses the existing track bake and `.LMP` cache. Lossless LZ4
compression reduces cache file sizes without changing CPU or GPU lighting. Old
uncompressed caches are upgraded after a successful load, without rebaking. Its
lighting maps and receiver data are uploaded when their revision changes, then
reused across frames. Dynamic shadow data follows the animated geometry. GPU buffers and
transfer buffers are retained and grown as needed instead of being recreated for
every frame.

The completed scene is read back into the existing high-resolution sprite surface.
That preserves dashboard and replay-control clipping, menu composition, palette
handling, and the original sprite-copy behavior. Failed Vulkan batches leave the
target untouched so the CPU renderer can draw the complete batch.

Vulkan selects the nearest shadow receiver independently for each sample. Small
shading differences can appear where surfaces meet; CPU SuperSight retains its
existing receiver-hint behavior.

Readback and synchronization have a cost. This implementation reduces CPU pixel
and shadow work, but still performs CPU geometry preparation and transfers the
result before presentation. Scene complexity, driver overhead, transfer bandwidth,
and the selected camera can therefore affect the gain. Retaining the requested
quality does not guarantee 60 FPS on every device or track. Performance on the
GTX 970 must be measured on that hardware; software Vulkan results are useful
for correctness, not for predicting its frame rate.

## Building and diagnostics

Vulkan is enabled in desktop builds by default. To build without it:

```sh
cmake -S . -B out/sdl3-linux-x64 -G Ninja -DCMAKE_BUILD_TYPE=Release -DRESTUNTS_VULKAN=OFF
cmake --build out/sdl3-linux-x64 --parallel 2
```

| Environment variable | Effect when set to `1` before launching |
| --- | --- |
| `RESTUNTS_VULKAN_DISABLE` | Skip the Vulkan startup probe; F10 remains inactive. |
| `RESTUNTS_VULKAN_DEBUG` | Request Vulkan validation through SDL's debug mode. |
| `RESTUNTS_VULKAN_SOFTWARE` | Allow a software Vulkan device for diagnostic testing. |

Validation requires installed Vulkan validation layers. Debug validation and
software-device overrides should be disabled for hardware performance comparisons.
For example, on Linux:

```sh
RESTUNTS_VULKAN_DEBUG=1 ./out/sdl3-linux-x64/restunts --data-dir stunts /nointro
```

Normal builds use the included shader binaries and do not run a shader compiler.
After changing shader source, regenerate the embedded header with glslangValidator:

```sh
python3 tools/scripts/generate-vulkan-shaders.py --compiler /path/to/glslangValidator
python3 tools/scripts/generate-vulkan-shaders.py --compiler /path/to/glslangValidator --check
```

The included shaders were generated with glslang 15.1.0, targeting Vulkan 1.0.
The check mode verifies that regenerated shader data matches the included header;
use the same compiler version for a reproducible byte-for-byte check.

## Verification and performance comparisons

Host regressions cover every F10/F12/Shift+F12 transition, repeated-key suppression,
unsupported-device no-ops, and switching during menus, intro playback, and paused
replays. The `shape3d-vulkan` regression compares coverage, materials, depth behavior,
and shadows against the CPU renderer and reports a skip if no compatible device
is available. Run the GPU comparison with validation enabled:

```sh
RESTUNTS_VULKAN_DEBUG=1 ctest --test-dir out/sdl3-linux-x64 -R shape3d-vulkan --output-on-failure
```

Also set `RESTUNTS_VULKAN_SOFTWARE=1` to exercise these shaders through a software
Vulkan device when hardware Vulkan is unavailable. Keep a successful software
comparison separate from hardware performance results.

For a useful hardware comparison, use a release build, load the same replay and
camera, and let the lighting cache finish loading before measuring. Compare F10
and Shift+F12 with the same scenery, dashboard, and replay-control settings to
include shadows in both renderers. Measure F12 separately for CPU rendering without
shadows. F11 shows presented FPS; sustained frame times over demanding track
sections are needed to assess stable 60 FPS. Repeat with cockpit, follow, trackside, opponent, and ghost
views because their visible geometry differs. Keep the GPU driver, worker-count
override, and power settings consistent between runs.
