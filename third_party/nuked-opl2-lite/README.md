# Vendored Nuked OPL2 Lite

Source: https://github.com/nukeykt/Nuked-OPL2-Lite
Commit: f3209b087da9117a5cd0e6db2fb8bea4df4e181a
Version: 0.9 beta
License: LGPL-2.1-or-later (see LICENSE and the source headers).
Copyright (C) 2026 Nuke.YKT.

Local changes, 2026-09-21, beyond CRLF line endings:

- Replace six signed left shifts with equivalent multiplication in output
  crushing and rhythm mixing, avoiding undefined behavior for negative samples.
- Declare the public DLL exports/imports in `opl2.h`; retain upstream structure
  layouts and calling conventions.
- Add this documentation and a standalone CMake shared-library build script.

All SDL3 desktop builds use this library, dynamically linked as
`libnuked-opl2.so` on Linux or `nuked-opl2.dll` on Windows. DOS writes the hardware
OPL2 chip and does not include the library.

Buffered register writes preserve same-tick key-off/key-on transitions;
immediate writes can collapse them. Initialization writes can be immediate.
The upstream stream generator resamples YM3812 output to 44.1 kHz mono PCM,
retaining its state between 100 Hz sequencer ticks.

## Rebuilding and replacing the library

The distributed `share/restunts/nuked-opl2-lite/` directory contains the exact
library source and this standalone build, including all local changes. It needs
only CMake 3.25+ and a C compiler; SDL, game sources and network access are not
needed. `nuked-build-info.txt` in the package records its original build settings.
`build-context/` preserves the parent project's CMake file, desktop toolchains
and build-information template used for the original integrated build. These
are reference snapshots; use the standalone `CMakeLists.txt` beside `opl2.c`
for the library-only rebuild described below.

Copy that source directory to a writable location and run from the copy:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This produces `build/libnuked-opl2.so` on Linux. For 32-bit Linux using GCC on an
x64 host, install the multilib compiler and development runtime, then use:

```sh
cmake -S . -B build-x86 -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS=-m32 -DCMAKE_SHARED_LINKER_FLAGS=-m32 \
    -DNUKED_OPL2_SSE2=OFF
cmake --build build-x86
```

Use `NUKED_OPL2_SSE2=OFF` to match a Linux x86 package built without SSE/SSE2/AVX.
The default is ON; x64 has SSE2 as part of its architecture. The library must
match the application's 32-bit or 64-bit architecture and system ABI.

Cross-build a Windows library with MinGW-w64 from Linux:

```sh
cmake -S . -B build-win64 -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc
cmake --build build-win64
```

Use `i686-w64-mingw32-gcc` and a separate build directory for Windows x86.
Alternatively, build directly in a MinGW-w64 shell on Windows using the first
pair of commands. These builds target Windows 7 APIs and bundle the GCC runtime.
Single-configuration generators place the DLL in the build directory;
multi-configuration generators may put it under `Release/`.

Close the game, keep a backup, and replace the packaged `lib/libnuked-opl2.so`
on Linux or `bin/nuked-opl2.dll` on Windows with your rebuilt file. For a source
tree build, the library lives beside `restunts` instead. No executable relinking
is required. You can modify the implementation while preserving the public
function names, calling conventions and the layouts in `opl2.h`; the application
allocates the public `opl2_chip` structure. Changes to that ABI require rebuilding
the application against the changed header too.

The LGPL permits modified versions subject to its terms. Preserve the copyright,
license and warranty notices, and add dated notices of your further changes.
