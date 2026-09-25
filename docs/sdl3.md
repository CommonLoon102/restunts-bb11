# SDL3 builds

The SDL3 platform in `src/restunts/platform/sdl3/` builds the game (`restunts`),
physics replay dumper (`repldump`), and renderer dumper (`pixldump`) for Windows,
Linux, and 32-bit DOS. The existing Open Watcom 16-bit DOS build remains available
through `make -C src/restunts restunts repldump pixldump`; its platform code stays
under `src/restunts/platform/dos/`.

## Dependencies and supported build targets

CMake 3.25+, a C99 GCC-compatible compiler (plus MinGW C++ on Windows), a build
tool, Git, and network access are required. CMake fetches and verifies SDL3 upstream commit
[`015489c672f24feed28c2aa2cdd6176df95329f3`](https://github.com/libsdl-org/SDL/tree/015489c672f24feed28c2aa2cdd6176df95329f3).
That revision includes SDL's DOS backend and the gameport/Sound Blaster timing fix
used by the [reference DOS port](https://github.com/murphy666/stuntsengine/pull/3).
All three platforms use the same revision; no separate SDL fork is required.

| Target | Compiler and baseline |
| --- | --- |
| Linux x64 | GCC, Debian 12 build baseline. |
| Linux x86 | GCC multilib or an i386 environment, Debian 12 build baseline. |
| Windows x64 and x86 | MinGW-w64; Windows 7 API target (`_WIN32_WINNT=0x0601`). |
| DOS | DJGPP GCC 12.2.0, 32-bit DPMI executable; VGA and a DPMI host. |

The baseline describes build settings. Successful builds and automated tests do
not establish runtime compatibility with every old OS or CPU. Windows 7, older
non-SSE2 processors, and physical DOS hardware need testing on those systems.
SDL's DOS minimum is i386 with 4 MB RAM; the game's actual memory and performance
requirements can be higher. See [SDL's DOS notes](https://github.com/libsdl-org/SDL/blob/015489c672f24feed28c2aa2cdd6176df95329f3/docs/README-dos.md).

## Linux

On Debian 12 x64, install the compiler, build tools, and common desktop drivers:

```sh
sudo apt-get update
sudo apt-get install build-essential cmake ninja-build git curl ca-certificates pkg-config \
    libasound2-dev libpulse-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
    libxi-dev libxfixes-dev libxss-dev libxtst-dev libwayland-dev libxkbcommon-dev libudev-dev \
    libdrm-dev libgbm-dev libegl1-mesa-dev libgl1-mesa-dev
cmake -S . -B out/sdl3-linux-x64 -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build out/sdl3-linux-x64
ctest --test-dir out/sdl3-linux-x64 --output-on-failure
```

For x86 on an x64 Debian host, enable i386 packages and install `gcc-multilib`
and the corresponding `:i386` development libraries. Use a separate build tree:

```sh
cmake -S . -B out/sdl3-linux-x86 -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/linux-x86.cmake
cmake --build out/sdl3-linux-x86
```

Append `-DRESTUNTS_SSE2=OFF` for a 32-bit x86 build without SSE/SSE2/AVX code in
the game, Nuked OPL2 Lite, or bundled SDL. Use another fresh build tree when changing CPU options.
The same option is available for Windows x86; x64 requires SSE2. A build without
SSE2 still uses the toolchain's x86 instruction-set baseline and system runtime;
it is not an 8086 executable. The DOS toolchain selects i386 and disables SSE2
by default.

`-DRESTUNTS_SYSTEM_SDL=ON` uses an installed SDL 3.4+ CMake package instead of the
pinned source. This option is incompatible with `RESTUNTS_SSE2=OFF`, because
CMake cannot control the instruction set of a prebuilt SDL library.

## Windows with MinGW-w64

Cross-compile from Linux after installing `mingw-w64`. SDL is built statically,
so the bundled-SDL build does not require a separate `SDL3.dll`:

```sh
cmake -S . -B out/sdl3-windows-x64 -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-x64.cmake
cmake --build out/sdl3-windows-x64
cmake -S . -B out/sdl3-windows-x86 -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-x86.cmake
cmake --build out/sdl3-windows-x86
```

The outputs are `restunts.exe`, `repldump.exe`, and `pixldump.exe` in their build
directory, together with the required `nuked-opl2.dll`. Native Windows builds can use a MinGW-w64 environment with CMake and
Ninja; select its GCC compiler directly instead of a Linux cross toolchain file.
MSVC is not a supported compiler for this port.

## DOS with DJGPP

The pinned Linux x64 cross-toolchain is
[build-djgpp v3.4 / GCC 12.2.0](https://github.com/andrewwutw/build-djgpp/releases/download/v3.4/djgpp-linux64-gcc1220.tar.bz2).
On Linux, install `libfl2` as well (DJGPP binutils uses `libfl.so.2`).
Download and verify it before extracting:

```sh
mkdir -p out/toolchains/djgpp
curl -fL https://github.com/andrewwutw/build-djgpp/releases/download/v3.4/djgpp-linux64-gcc1220.tar.bz2 \
    -o out/toolchains/djgpp-linux64-gcc1220.tar.bz2
printf '%s  %s\n' 8464f17017d6ab1b2bb2df4ed82357b5bf692e6e2b7fee37e315638f3d505f00 \
    out/toolchains/djgpp-linux64-gcc1220.tar.bz2 | sha256sum -c -
tar -xjf out/toolchains/djgpp-linux64-gcc1220.tar.bz2 -C out/toolchains/djgpp --strip-components=1
export PATH="$PWD/out/toolchains/djgpp/bin:$PATH"
cmake -S . -B out/sdl3-dos -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/djgpp.cmake -DRESTUNTS_BUILD_TESTS=OFF
cmake --build out/sdl3-dos
```

This creates 32-bit DOS `.exe` files. Place a compatible DPMI host such as
[`CWSDPMI.EXE`](https://sandmann.dotster.com/cwsdpmi/) alongside them when the
DOS environment does not supply one. CI artifacts do not bundle CWSDPMI or the
game's data files.

The DOS video path uses indexed VGA 320x200 with SuperSight off. Enabling
SuperSight with F12 renders the 3D scene at 1280x800 and selects a VESA mode
that can display it with the original 4:3 aspect ratio (normally 1280x1024,
with a 1280x960 image and black borders). Indexed modes are preferred; true-colour
VESA modes are supported too. If no sufficiently large mode is available, SDL
scales the 1280x800 rendering to the largest available viewport and logs a warning.
Disabling SuperSight restores Mode 13h. High-resolution rendering requires more
RAM and processing power than the original mode.

Audio writes the real
AdLib-compatible OPL2 chip at port `388h`, which DOSBox also emulates. An AdLib
or compatible Sound Blaster FM device is needed for sound. If audio initialization
fails, the native game reports a warning and continues silently. Desktop builds
synthesize the same FM registers through
[Nuked OPL2 Lite](https://github.com/nukeykt/Nuked-OPL2-Lite), licensed under
LGPL-2.1-or-later, and send PCM to SDL. The library is dynamically linked and
replaceable. The driver retains AD15's octave-crossing pitch-bend table and
signed rounding. Sustained engine
voices with a half-rate carrier use equivalent integer operator multipliers
and a halved base pitch. This preserves the carrier frequency while preventing
low-RPM rounding from permanently changing the oscillators' relative phase.
The adjustment applies only to compatible continuous FM instruments; musical
notes and instruments with pitch-dependent envelopes, vibrato, key scaling or
multiplier controllers retain their original setup. Nuked runtime writes are
buffered to preserve same-tick key-off/key-on transitions.

In DOSBox/DOSBox-X use `core=dynamic`, `cycles=max`, and `aspect=true`. Aspect
correction displays 320x200 VGA pixels at their intended 4:3 shape. Mount a
directory containing the executable, data, and DPMI host, then run `RESTUNTS.EXE`. Stop an
automated DOSBox process with SIGKILL; graceful termination can open a modal
confirmation dialog. Real gameport joystick behavior requires hardware or a
configured DOSBox joystick; the reference port did not establish that coverage.

## Running and controls

The source checkout contains only a placeholder under `stunts/`; building does
not fetch game data. Copy Broderbund Stunts 1.1 resources into `stunts/`, or extract
the [BB11 archive used by the existing DOS CI](https://github.com/CommonLoon102/restunts4d-oracles/releases/download/v1.0.1/BB11.zip)
there before running the game or the audio regression. The archive must place
files such as `ADSKIDMS.VCE` and `DEFAULT.RPL` directly in that folder.

Resources are read from the current directory, or from `--data-dir` when it is
the first argument. Existing game and dump parameters follow it unchanged:

```sh
out/sdl3-linux-x64/restunts --data-dir stunts /nointro
out/sdl3-linux-x64/repldump --data-dir stunts DEFAULT.RPL
out/sdl3-linux-x64/pixldump --data-dir stunts DEFAULT.RPL 2 0 5
```

Windows uses the corresponding `.exe` names. Dump outputs and saved game data
are written in the selected data directory, so it must be writable. Keep the
original game resources and replay/car additions together there.

On Linux and Windows, the interactive game defaults to serial rendering with
zero background render workers and leaves CPU affinity and process priority
unchanged. The OS can schedule threads across the allowed CPUs.
DOS and dump tools do not alter affinity or priority.

`RESTUNTS_CPU_AFFINITY=off` is the default and retains inherited affinity. Set it
to an allowed zero-based logical CPU number to pin the process explicitly, or
use `auto` to pin to the OS-selected CPU at startup if allowed, otherwise the
first allowed CPU. Windows CPU numbers are relative to the current processor group.
`RESTUNTS_HIGH_PRIORITY=0` is the default and retains inherited priority. Set it
to `1` to request above-normal priority (Windows) or nice `-5` (Linux) before SDL
initialization, while preserving higher inherited priority. Linux may deny the
increase without an appropriate `RLIMIT_NICE` or `CAP_SYS_NICE`; a diagnostic is
printed and the game continues.
`RESTUNTS_RENDER_WORKERS=auto` enables automatic parallel rendering, or use a
count from `0` through `7`. Keep affinity set to `off` when comparing multiple
workers so they can use all allowed CPUs. See the
[scheduling and worker settings](../readme.md#supersight-and-fps-display) for
Linux and PowerShell examples. These settings do not reserve a core or guarantee
a frame rate.

Desktop windows apply the VGA vertical 6:5 pixel-aspect correction: the original
320x200 framebuffer fills a 4:3 image. Nearest-neighbour scaling preserves sharp
pixel edges, and resizing adds black borders to retain that aspect. Renderer dump
images retain the original 320x200 pixel data for parity checks. With SuperSight
on, the 3D scene and both car-selection previews render at 1280x800. Their geometry
is projected and rasterized at the higher resolution; menu artwork, dashboard,
and replay controls keep their original pixel detail. F12 works in both car
selection screens and the track preview as well as driving and replay views.
Enhanced driving and replay scenes use [AI-refined skybox PNGs](skyboxes/README.md)
at four times the original width and height. Track selector previews use these same PNGs
when SuperSight is active. Runtime installs include the required
`bin/skyboxes/` directory; retain it beside the executable when packaging.
Missing or invalid textures fall back to the original artwork. Menus support
keyboard, mouse, and an SDL joystick. Existing driving and replay controls remain
available:

- **Up/Down** accelerate and brake; **Left/Right** steer.
- **F1–F4** select cockpit, follow, custom, and trackside cameras.
- **Alt+Enter** toggles desktop fullscreen, preserving the 4:3 image and restoring
  the previous window size when leaving fullscreen. Keypad Enter also works.
- **F11** toggles the frame-rate counter, including in both car-selection
  screens; **F12** toggles SuperSight.
- Hold **Q** to rewind a live race; release it to resume from that point.
- **T** switches between the player and opponent or selected ghost view.
- **Escape** leaves driving or replay playback; closing the desktop window exits
  the game and removes its temporary ghost cache.

See the [gameplay notes](../readme.md#supersight-and-fps-display) for SuperSight,
ghost selection, and rewind behavior.

The SDL3 build supports AdLib music and effects. Other original executable DOS
sound drivers (MT-32, PC speaker, and Sound Blaster sampled-speech paths) are not
ported. Original DOS16 builds retain their existing driver selection. Native
sequencing remains on the main thread; SDL only consumes generated PCM, so an
audio callback cannot race resource loading or freeing.

## Validation and CI

The `SDL3 builds` workflow builds the game and both dump tools on all targets,
using Debian 12 for the Linux release baseline. Its Linux matrix includes x86
with SSE2 disabled. Linux tests cover the platform layer and AdLib synthesis
from a shipped instrument, plus existing host regressions. Audio tests check
audible PCM, pitch, engine frequency, volume, modulation, key-off, native
engine-definition pointers, unavailable-device fallback, and batch-mode cleanup.
Windows CI runs platform, scheduling, worker lifecycle, file I/O, input, audio,
and dump regressions on Windows Server 2022; Windows 7 runtime compatibility
still needs verification on that OS.
The shared **PR validation** and **Release** workflows run the complete physics
corpus and configurable renderer coverage for the selected platforms. Their
`platforms` input is a nonempty JSON array of unique names from `dos` and
`sdl3`, defaulting to `["dos","sdl3"]`. Use `platforms: '["sdl3"]'` for Linux
x64 SDL3 only, `platforms: '["dos"]'` for DOS only, or
`platforms: '["dos","sdl3"]'` for both. Like `cameras`, this input is available
in the manual workflows and the reusable **Build and validate** workflow.
Tests and reports run only for selected platforms. Unselected builds are
skipped, except **Release** always builds the DOS executables it publishes.

Both platforms use identical shard plans and archived Borland references. The
`cameras`, `target`, and `renderer-test-percentage` inputs apply to both; setting
the percentage to 100 tests every eligible renderer replay. SDL3 shard results
and reports have an `sdl3-` artifact prefix and fail on mismatches, execution
errors, or incomplete coverage. These dump comparisons exercise physics and
framebuffer rendering; interactive display, input, and audio have separate
platform regressions.

The separate **SDL3 builds** workflow also retains a small Linux x64 sample.
For an isolated local comparison against freshly generated DOS physics and
renderer oracles, run:

```sh
python3 tools/scripts/validate-native.py --build-directory out/sdl3-linux-x64 \
    --output out/native-validation --count 3
```

This requires DOSBox and a fresh output directory. It checks preserved oracle
checksums, uses `core=dynamic` and `cycles=max`, and kills DOSBox after each
capture. Full replay-corpus validation and physical hardware checks are separate
from the build matrix; a successful sample is not evidence that all replays,
controllers, or sound hardware have been exercised.

CI archives contain an installed package: executables in `bin/`, Nuked's shared
library in `lib/` on Linux or `bin/` on Windows, dependency license notices, and
the exact Nuked source and rebuild instructions. Windows test archives carry
the same library, source and notices. DOS packages omit Nuked. These are build
artifacts, not automatic GitHub Releases or deployments.

## Packaging and Nuked's license

Create a redistributable directory from a completed build:

```sh
cmake --install out/sdl3-linux-x64 --prefix out/package-linux-x64 --component Runtime
out/package-linux-x64/bin/restunts --data-dir "$PWD/stunts" /nointro
```

Distribute the complete directory, including `THIRD-PARTY-NOTICES.txt` and
`share/`. Linux executables find the library relative to their installed location,
so the package can be moved; Windows loads the DLL beside the executable.
Do not distribute a desktop executable alone. `--component Tests` installs a
separate test package with the same license and source material.

Nuked's full LGPL-2.1 license is in
`share/licenses/restunts/Nuked-OPL2-LICENSE`; its grant allows later versions too.
The exact library sources, local modification notices, standalone build script,
build settings, and parent build-control snapshots are in
`share/restunts/nuked-opl2-lite/`. The `README.md` there explains how to rebuild
and substitute the shared library without relinking the application.
`THIRD-PARTY-NOTICES.txt` in the package root includes the required permission
for own-use modification and reverse engineering to debug library modifications.
These terms do not relicense unrelated code or game assets.

The desktop game's second intro screen acknowledges Nuked, and
`restunts --licenses` prints the notice and package locations without needing
game resources or a display. The package regression verifies relocation and
replacement with a modified library built solely from the shipped source:

```sh
python3 tools/scripts/test-nuked-package.py --build-directory out/sdl3-linux-x64
```

## Capturing a live audio problem

Desktop builds can record the complete OPL register stream independently of
race replays. This captures synthesis history that an `.rpl` file does not
contain, including engine starts, RPM changes and the restart on replay seeking.
Tracing is off unless `RESTUNTS_AUDIO_TRACE` names an output file. Each game
launch overwrites that file; choose a new name to preserve an earlier recording.

From the repository root on Linux:

```sh
RESTUNTS_AUDIO_TRACE="$PWD/engine.trace" \
  ./out/sdl3-linux-x64/restunts --data-dir ./stunts /nointro
```

In PowerShell, set `$env:RESTUNTS_AUDIO_TRACE = "$PWD\engine.trace"` before
launching the game. Reproduce the changed engine tone, then use replay seeking
to restore it and let the restored sound play briefly. Note the approximate
race times of both events, quit normally, and retain the `.trace` file.
Unset the variable afterward to disable recording. Recording flushes once per
second; normal shutdown writes the final sample position. A disk error disables
tracing while the game keeps running.

Render the recorded synthesizer output without the game or SDL:

```sh
python3 tools/scripts/render-opl-trace.py engine.trace engine.wav
```

The renderer needs Python 3.9+ and a C compiler (`cc` by default, or `--cc`). It
builds a temporary helper from the vendored Nuked sources and replays the
recorded write timing. The WAV contains all generated PCM; SDL queue clears are
recorded as markers but are not cut from the WAV. Device latency, resampling by
the audio device and hardware playback faults are outside the trace. Keep the
original trace so it can be re-rendered after changes to the audio driver.
