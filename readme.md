# Restunts - The Stunts reverse engineering project

https://wiki.stunts.hu/wiki/Restunts

Main repository: https://github.com/4d-stunts/restunts

## Repository contents:
	docs
		Various technical docs related to (re)stunts itself.

	src\restunts
		Project directory containing disassembly, ported c and makefiles to
		produce various executables based on the (re)stunts code.

	stunts
		Broderbund Stunts 1.1, the game.

	tools
		Contains setup scripts for Open Watcom 2 and bundled Windows build and
		testing tools.


### Contents of src\restunts:

	src\restunts\asmorig
		Contains asm code from the disassembled original exe.

	src\restunts\c
		Contains c functions ported from the disassembly.

	src\restunts\dos
		Makefile to build restunts for DOS.

	src\restunts\platform\dos
		DOS specific code.

	src\restunts\repldump
		Tool based on the original game code, loads replays and dumps the game
		state contents at each frame in a file for further analysis.

	src\restunts\pixldump
		Replay-driven renderer test tool. It renders every frame incrementally
		and hashes the 320x200 camera framebuffer at frame 0 and every subsequent frame.


## Running the game

Run `restunts.exe` in DOSBox or DOSBox-X with `core=dynamic` and `cycles=max`.
Mount `stunts/` directly as a DOS drive in the emulator.

For DOSBox-X, use these recommended settings in the `[dos]` section of your
configuration file:

```ini
[dos]
# DOS version (DOSBox-X only)
ver=7.1

# Put the shell into upper memory (DOSBox-X only)
shellhigh=true

# Enable long file name support (DOSBox-X only)
lfn=true

# Disable DOSBox-X's low-memory padding to free conventional memory.
# Original dump tools need this space when loading large custom dashboards.
minimum mcb free=1
```

### SuperSight and FPS display

Press **F12** while driving or viewing a replay to toggle SuperSight. It extends
visibility from the original 23 candidate tiles to 110 and uses detailed models
where capacity permits. Crowded scenes reduce distant detail and then draw
fewer distant tiles. Nearby frames reuse the last suitable quality level, with
periodic full-detail checks and fresh checks after camera changes or replay seeks.
SuperSight allows up to 592 primitives with a 13 KiB
rendering buffer and respects the graphics menu's scenery setting. Switching
it off restores the original draw distance, detail policy, and rendering limits.
The enhancement is based on Alberto Marnetto's
[SuperSight](https://marnetto.net/2025/02/20/broderbund-stunts-1).

Press **F11** to toggle a frame-rate counter in the top-left corner. It measures
presented frames over approximately one second and rounds down, for example
`20 FPS`. Values below 20 are red; values of 20 or higher are green.

Both features start off and retain their selected state until toggled again or
the game exits. They work in all driving and replay cameras, including opponent
and ghost views and paused replays. Holding either key toggles only once.

### Race against a ghost

In **Opponent**, choose **Clock**, then **Ghost** to select a replay. The game
uses the replay's track and changes the description to **Race against a Ghost.**
Your car and transmission stay selected, and you can change them before driving.
The recorded player's car follows the replay as a silent, black grille ghost;
it cannot collide with your car or change the track. At the end of the replay,
it stays in its final position. Restarting or rewinding keeps it synchronized
with your race time. Press **T** to switch between your view and the ghost's
view; **F1** through **F4** select the camera mode for either car.

Choose **Clock** again to remove the ghost. Loading a different track, changing
its layout in the editor, or selecting an AI opponent also clears the ghost.
Canceling the replay picker keeps the previous selection. The ghost selection
lasts for the current game session; saved replays retain the ordinary replay
format and do not include the ghost.

Preparing the ghost reconstructs the selected replay before driving and caches
its positions in a temporary `GHxxxxxx.TMP` file in the game directory. The game
removes this file when the selection is cleared or the game exits normally.
As with ordinary replay playback, use the physics switches used to record it.

### Rewind while driving

Hold **Q** during a race to rewind, then release it to continue driving from
that point. The race stays visible without opening the replay controls or a
menu. Rewind speed doubles after holding Q for ten seconds and stops at the
start of the available recording. Releasing Q replaces the later recording
with your new driving; the run is marked as modified, like continuing from a
replay, and does not qualify for the normal high-score table.

### Optional parameters

Run `restunts.exe /nointro` to skip the startup intro and open the main menu
immediately after initialization. The intro remains available when leaving the
main menu. This switch can be combined with the existing startup options.

Run `restunts.exe /pg:off` to correct the original power gear bug, including
anti-power gear and the loss of aerodynamic deceleration while accelerating.
Original physics remain the default for replay compatibility. `/pg:on` explicitly
selects the original behavior. These switches are case-insensitive; if both are
supplied, the last one wins. They can be combined with `/nointro` and other
startup options.

Run `restunts.exe /lc:off` to disable Legacy Collision and restore the earlier
32-bit signed interpolation for wheel collisions with walls and track planes.
Legacy collision behavior remains the default for replay compatibility; `/lc:on`
explicitly selects it. These switches are case-insensitive, and the last one
wins. They can be combined with `/pg:off`, `/nointro`, and other startup options.
With `/lc:off`, collisions also check the wheel's movement through the underside
clearance of elevated track surfaces, preventing fast cars from skipping the
collision zone behind a loop. The impact is checked against the actual surface
footprint, preserving clear passages underneath and beside it. Surfaces at both
ends of the movement are checked, including the surface a wheel just left when
moving onto the ground or another segment. Car edges between the wheels are
also checked against finite walls, catching impacts with the start of a ramp
side wall that individual wheel paths can miss. Slalom stones use their full
finite bounds for these checks, including wheel movement that crosses an entire
stone between frames. The car stops at the first contact.
With `/lc:off`, collision-induced sideways heading offsets and opponent spin
also decay fully to zero in either direction, preventing a permanent steering
bias after contact. Legacy mode retains the original negative-rounding behavior.
Renderer clipping retains its original arithmetic.

The ported physics dump tool accepts the same switches after the replay name:
`repldump.exe 0681 /pg:off /lc:off`. Replays do not store these options, so use
the same physics settings for recording and playback. Original assembly
executables and the renderer dump tools retain their existing interfaces.

### OWOOT driving rules

Run `restunts.exe /owoot` to require at least part of one player wheel to remain
on or above the road. The check uses the car model's tire geometry, including
steering, suspension, pitch, and roll, and is independent of camera position,
zoom, and detail level. Red-white rumble strips on large corners count as grass.
Driving beneath an elevated road does not count as being above it.

Stunts must be followed along their intended route: the switch also checks
ordered progress through loops, corkscrews, slaloms, and other stunt elements.
An airborne car may cross the single off-road tile between aligned ramps or
bridges. The whole gap tile is exempt. The exception requires a launch from
the connected approach and ends at the receiving road; it does not permit
arbitrary flights over grass. The
`r*` OWOOT replay corpus contains only single-tile gaps between connected
elevated road ends, including gaps occupied by scenery. Flight may continue
over the receiving road for additional tiles.

A violation triggers the normal crash immediately. Only the player is checked,
including during replay playback. Rewinding restores the OWOOT progress along
with the car state. The switch is case-insensitive and can be combined with
`/pg:off`, `/lc:off`, and `/nointro`; it is disabled by default.

The physics dump tool also accepts `repldump.exe <replay> /owoot`. Replays do not
store the switch, so launch with the same settings for recording and playback.
See [OWOOT validation notes](docs/owoot-validation.md) for replay audit findings.

With `/owoot`, `repldump.exe R0019.RPL /owoot` also writes `R0019.owo`, containing
exactly `pass` or `fail`. Passing requires finishing the race without an OWOOT
violation; ordinary crashes and unfinished replays fail too. The result starts
as `fail` and changes to `pass` only after successful processing. File-writing
or replay-loading errors return a nonzero exit code; check that code as well
when validating a batch. Without `/owoot`, no `.owo` file is created or updated.

### Needle colours

The ported game includes the [needle colour mod](https://wiki.stunts.hu/wiki/Needle_colour_mod).
Each car selects its dashboard needle colours through the Red #5 word
(labelled "needle colour" in CarWorks), at offset `0xAE` within its `simd`
resource. The low byte sets the speedometer colour. The high byte sets the
tachometer colour; when it is zero, the tachometer uses the low byte too.
For example, `0x040F` selects palette index 15 for the speedometer and 4 for
the tachometer. The stock value `0x0010` keeps both needles white, and
`0x0000` makes both black. Digital speedometer digits are unaffected.


## How to build

The DOS compiler, assembler, and linker are Open Watcom 2, pinned to the official
[2026-09-01 build](https://github.com/open-watcom/open-watcom-v2/releases/tag/2026-09-01-Build).
The setup scripts verify its SHA256 and install into ignored `tools/watcom/`.
The shared release pin is `tools/scripts/open-watcom.conf`. WCC, WASM, WLINK,
and GNU Make 4.3 or newer run natively on Linux or Windows. The standard build
does not require Wine or DOSBox. Python 3.9 or newer is required for the
`*-original` targets, which prepare assembler-compatible copies of the
preserved original sources.

Git is required for the ported game builds. The Options menu shows
`Chocolate Stunts` and `Version <githash> (Mmm dd yyyy)`, using the first seven
characters of the checkout's commit and the compilation date (for example,
`Version d37a5c8 (Sep 18 2026)`). Incremental builds refresh both the commit
and compilation date.

### On Windows

1. Install the toolchain from PowerShell at the repository root (Windows 10/11
   `tar.exe` and PowerShell are required):

   ```powershell
   powershell -ExecutionPolicy Bypass -File tools\scripts\install-open-watcom.ps1
   ```

2. In cmd.exe at the repository root, run:

   ```text
   cd src\restunts
   setpath
   make restunts repldump pixldump
   ```

3. To build the original game and both dump tools from source as
   `stunts/restunto.exe`, `stunts/repldumo.exe`, and `stunts/pixldumo.exe`,
   install Python 3.9 or newer and run in the same cmd.exe window:

   ```text
   make restunts-original repldump-original pixldump-original
   ```

### On Linux (x86-64)

1. Install GNU Make 4.3 or newer, Bash, curl, tar, xz, and
   coreutils. Run the setup script from the repository root:

   ```sh
   tools/scripts/install-open-watcom.sh
   ```

2. Build with native GNU Make:

   ```sh
   make -C src/restunts restunts repldump pixldump
   ```

3. To build the original game and both dump tools from source as
   `stunts/restunto.exe`, `stunts/repldumo.exe`, and `stunts/pixldumo.exe`,
   run from the repository root after installing Python 3.9 or newer:

   ```sh
   make -C src/restunts restunts-original repldump-original pixldump-original
   ```

The makefiles use `python3` on Linux and `python` on Windows; override
`PYTHON` if needed.

### On both platforms

Build outputs are copied to `stunts/`.

The supported targets are:

| Target | Result |
| --- | --- |
| `restunts` | Builds `restunts.exe` from the ported C game and DOS platform layer. |
| `restunts-original` | Assembles the original game and links `restunto.exe` with WLINK. |
| `repldump` | Builds the C physics dump tool, `repldump.exe`. |
| `pixldump` | Builds the C renderer dump tool, `pixldump.exe`. |
| `repldump-original` | Builds `repldumo.exe` from the original assembly and physics dump wrapper. |
| `pixldump-original` | Builds `pixldumo.exe` from the original assembly and renderer dump wrapper. |
| `test-dos-platform` | Builds the DOS platform ABI test, `tests/build/watcom/<configuration>/DOSPLAT.EXE`. |
| `test-race-memory` | Builds the full-game race memory test, `tests/build/watcom/<configuration>/RACEMEM.EXE`. |
| `clean` | Removes generated build objects and candidate executables. |

The `*-original` dump targets assemble the original game code with WASM,
compile the dump wrappers with WCC, and link them with WLINK. The resulting
executables are rebuilt development tools. CI uses the independent Borland
`repldumo.exe` and `pixldumo.exe` preserved under
[tools/oracles/borland](tools/oracles/borland/README.md), whose SHA256 hashes
are recorded alongside them. The archived renderer captures every frame
incrementally. Source builds never replace the archived files.

The original game assembly under `src/restunts/asmorig/` is preserved
unchanged. The WASM build runs
[the source adapter](tools/scripts/prepare-wasm-original.py) to prepare
compatible copies under `asmorig/build/watcom/<configuration>/wasm/source/`.
All three `*-original` targets reuse these objects. The `restunto.exe` link
response file reproduces the original segment order, disables automatic segment packing,
and preserves the original 8000-byte stack. The old empty `segments.obj`
layout helper is replaced by WLINK ordering directives; its ASM source is
retained unchanged.

See [the assembler guide](docs/assembler.md) for generated-source handling,
original-code layout requirements, and object/executable comparison commands.

`makerepldump.bat` builds both games and both physics dump tools;
`makepixldump.bat` builds both renderer dump tools. Both stop on build errors.

`pixldump-original` builds `stunts/pixldumo.exe` from the original game assembly
and the current capture wrapper. Local `pixelcheck.sh` runs use the renderer
reference in `stunts/`. `validate-toolchain.py` takes it from
`--candidate-directory`, which defaults to `stunts/`, and uses the archived
physics oracle. CI copies both archived Borland oracles into its isolated
test directory after copying the source-built executables.

### pixldump parameters

pixldump and pixldumo use the same mandatory parameters. The number of
parameters selects the output mode:

```text
pixldump.exe <replay> <camera> <target>
pixldump.exe <replay> <camera> <target> <frame>

pixldumo.exe <replay> <camera> <target>
pixldumo.exe <replay> <camera> <target> <frame>
```

| Parameter | Accepted values | Description |
| --- | --- | --- |
| `replay` | Replay base name, `.rpl`, or `.RPL` filename | Replay to render. The extension may be omitted. |
| `camera` | `1`, `2`, `3`, or `4` | Selects F1, F2, F3, or F4 respectively. |
| `target` | `0` or `1` | Selects the player (`0`) or opponent (`1`). Opponent mode requires a replay containing an opponent. |
| `frame` | `0` through `65535` | Exact frame to render. It must not exceed the replay's final frame. Supplying it selects BMP mode. |

With three parameters, the tools generate the normal hash dump. pixldumo writes
`<replay>.PDO` and pixldump writes `<replay>.PDD`. The first line is `PIXLDUMP 2`,
terminated by CRLF, identifying capture with incremental redraws on every frame.
Each subsequent CRLF-terminated row
contains the decimal frame number, one space, and the MurmurHash3_x86_32 of the
raw 64,000-byte Mode 13h framebuffer, with seed 0. The hash is eight lowercase
hexadecimal digits, most significant digit first, including leading zeroes.
The tools hash every frame from frame 0 through the replay's final frame.
Old full-redraw, MD5, and five-frame sampled dumps are incompatible; the
regression runner rejects their missing version header or missing frames and
regenerates them automatically.

With four parameters, the tools generate only the requested 320x200 indexed BMP
using the game's VGA palette. The filename includes the camera, target, frame,
and renderer:

```text
<replay>.<camera>.<target>.<frame>.PDO.bmp
<replay>.<camera>.<target>.<frame>.PDD.bmp
```

For example, `default.2.0.5.PDD.bmp` is frame 5 from the ported renderer, using
the F2 helicopter camera and following the player.

Examples:

```text
REM Generate a player/F2 hash dump.
pixldump.exe default.rpl 2 0

REM Generate an opponent/F4 hash dump with the original renderer.
pixldumo.exe default.rpl 4 1

REM Generate a player/F1 BMP at frame 5.
pixldump.exe default.rpl 1 0 5
```

pixldump must reproduce the original renderer, including its bugs. In particular,
polygon depth averages use unsigned division for non-power-of-two vertex counts
even when near-plane clipping retains a negative depth sum. This can put a grille
behind opaque surfaces, as in `0027.rpl`, camera 2, player, frame 665. Preserve
this behavior in the C port; the original engine in `asmorig` remains the oracle.

Opponent route selection also preserves original resource overreads. Track tile
IDs index the opponent's `sped` data beyond its 16 speed entries, into following
resource chunks and bytes left by decompression. The route loader reconstructs
these bytes in a fresh, zero-initialized allocation, retaining the original
compressed-source placement and enough owned tail storage for every byte-sized
index. Ordinary cached resources have discarded this tail and cannot supply it.
Clamping costs at the declared resource size changes route choices: in
`0034.rpl`, it selects the alternate branch and first diverges at frame 943.
Do not replace the reconstruction with either zero costs or reads from adjacent
allocations. Host tests cover decoder residue and poisoned memory; the golden
replays cover the resulting opponent behavior.

The C renderer also preserves the original sphere bounding-box writes used by
crash explosions and the renderer stack values reused by stopped-wheel physics.
The pixel-dump wrapper supplies the archived caller context, deriving addresses
from the DOS load segment, decoded arguments, and resource allocations. See
[renderer parity notes](docs/renderer-parity.md) for the assembly evidence and
regression coverage.

Both modes render every frame from frame 0 with incremental redraws, maximum
graphical detail, and hidden dashboard and replay controls. BMP capture retains
the preceding framebuffer history up to the requested frame. Invalid arguments
are rejected before an output file is created. The
complete output path, including its generated suffix, must fit in 127
characters. BMP filenames require DOS long-filename support; the supplied
`tools/scripts/dosbox.proc.conf` enables it for DOSBox-X.

### pixelcheck parameters

On Linux, `pixelcheck.sh` builds or reuses both executables, runs them with the
same replay, camera, target, and optional BMP frame, and compares the resulting
files:

```text
tools/scripts/pixelcheck.sh <replay-file> <rebuild> <camera> <target>
tools/scripts/pixelcheck.sh <replay-file> <rebuild> <camera> <target> <frame>
```

| Parameter | Accepted values | Description |
| --- | --- | --- |
| `replay-file` | Replay filename under `stunts/` | Replay passed to both executables. |
| `rebuild` | `true` or `false` | `true` rebuilds both executables first; `false` reuses the existing executables in `stunts/`. |
| `camera` | `1`, `2`, `3`, or `4` | Camera passed to both executables: F1, F2, F3, or F4 respectively. |
| `target` | `0` or `1` | Player (`0`) or opponent (`1`) passed to both executables. |
| `frame` | `0` through `65535` | Present in the BMP form only. It selects the exact frame compared by both executables. |

Examples:

```text
# Rebuild both executables and compare player/F2 hash dumps.
tools/scripts/pixelcheck.sh 0610.rpl true 2 0

# Reuse existing executables and compare opponent/F4 hash dumps.
tools/scripts/pixelcheck.sh 0610.rpl false 4 1

# Reuse existing executables and compare player/F1 BMPs at frame 5.
tools/scripts/pixelcheck.sh 0610.rpl false 1 0 5
```

Set the `PIXELDUMP_TIMEOUT_SECONDS` environment variable to a positive integer
to override the default 120-second timeout for each DOSBox run.


## C coding style

Project `.c` and `.h` files under `src/` use tabs with a width of four columns,
a 100-column target, K&R braces (function opening braces on their own line),
and a required braced body for every `if`, `else`, `for`, `while`, and `do`.
Conventional `else if` chains are allowed. Empty loops also need braces. Keep
CRLF line endings, as required by `.gitattributes`.

C sources use the C99 features supported by Open Watcom (`-zastd=c99`).
Declare local variables close to their first use, combining the declaration
and first assignment when possible. Keep declarations in the enclosing scope
when values are shared across branches or loops, and preserve initialization
order and object lifetime.

Two standard tools check the style directly:

- [editorconfig-checker](https://github.com/editorconfig-checker/editorconfig-checker)
  reads the whitespace rules in `.editorconfig`. Its Python package installs
  the `ec` command. `.editorconfig-checker.json` limits discovery to `src/`
  and permits alignment spaces after indentation tabs. Only C/H files have
  EditorConfig rules.
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html) reads C layout
  and brace rules from `.clang-format`. Keep its whitespace settings consistent
  with `.editorconfig`. Include order is preserved.

Install the pinned tools once in a Python virtual environment:

```sh
python3 -m venv .venv
# Linux/macOS:
. .venv/bin/activate
# Windows cmd.exe: .venv\Scripts\activate.bat
python -m pip install -r tools/scripts/requirements-format.txt
```

From the repository root, check against `.editorconfig` with one command:

```sh
ec
```

Check C formatting too (Bash or Git Bash):

```sh
git ls-files -z 'src/*.c' 'src/*.h' | xargs -0 -r clang-format --dry-run --Werror
```

Format all tracked project C/H files:

```sh
git ls-files -z 'src/*.c' 'src/*.h' | xargs -0 -r clang-format -i
```

For an individual file, use `clang-format -i path/to/file.c`. Add new files to
Git before running the commands based on `git ls-files`. CI runs both checks
for pull requests and before releases. No project-specific formatter or checker
script is needed.

Braces are required even where clang-format cannot insert them automatically,
including macro bodies, empty loops, and bodies spanning preprocessor
branches. Review those cases manually; a successful clang-format check does
not prove that those cases have braces.

## C# coding style

The regression application and its tests under `tools/scripts/dumpsrv` use four-space
indentation, opening braces on their own line, braced control-flow bodies, a
100-column target, and CRLF line endings. `.editorconfig` defines these rules.
Use the .NET 10 SDK to check or apply formatting:

```sh
dotnet format tools/scripts/dumpsrv/dumpsrv.slnx --verify-no-changes
dotnet format tools/scripts/dumpsrv/dumpsrv.slnx
```

## Complexity audit

See [the complexity report](docs/complexity.md) for measurements, completed
refactors and audit results.


## CI replay validation

CI runs in five phases, each requiring the previous phase to pass:

1. C/H formatting, C# regression service tests, host regression tests, and shard
   planning run in parallel.
2. Build the executables, including the DOS timer/cleanup checks.
3. Run all physics replay shards and validate their coverage.
4. Run the renderer replay shards and validate coverage separately for each camera.
5. Publish a combined physics/renderer report for each camera as
   `partitions_all-cam<camera>-target<target>`.

A failed phase skips the later phases. Physics and renderer phase diagnostics
remain available in their individual artifacts if replay validation fails.

Pull requests and releases build the game, physics dump tools, and both
renderer dump tools. CI compares the full golden replay set for physics and
rendering by default, comparing pixldump `.PDD` files
against pixldumo `.PDO` files. The `cameras` input is a nonempty JSON array
of unique camera IDs from 1 through 4, defaulting to `[2]`. For example,
`cameras: '[1,2,3,4]'` runs a separate **Renderer replays** job for each camera.
Invalid IDs and duplicates fail validation before the build. The `target`
input selects player (`0`, the default) or opponent (`1`). Both inputs are
available in the manual **PR validation** and **Release** workflows and passed
to **Build and validate**, which calls **Replay tests** for each camera.
Push and pull-request runs use the fallback values in `pr-validation.yml`.
Physics runs once, and every camera uses the same shard plan. A failed camera
does not cancel the other camera jobs. The final reports require every
camera to pass. Each report combines the shared physics diagnostics with only
that camera's renderer diagnostics. Opponent rendering selects only replays
containing an opponent.

Each CI shard verifies the archived checksums and uses the preserved Borland
dump executables to generate missing references. Renderer shards download
`PDOs-cam<camera>-target<target>.zip` for the selected camera and target.
Physics shards download `BINs.zip` from oracle release `v1.0.4`. Oracle
downloads run for both targets. Unavailable or failed cache downloads produce
a warning and allow testing to continue. Missing or invalid references are
generated locally. Renderer cache metadata records the camera and target so
a changed view cannot reuse an incompatible reference.

The C# application in `tools/scripts/dumpsrv` runs these comparisons on Linux, Windows,
and GitHub Actions. Its HTTP service, direct runner, and report merger share the
same engine. See the [service and runner guide](tools/scripts/dumpsrv/README.md)
for publishing, service parameters, client options, and local execution.

Set `renderer-test-percentage` (an integer from 1 to 100) when manually
starting **PR validation** or **Release** to change renderer coverage. Calls
to the reusable `build-and-validate.yml` workflow can set the same input;
pull request events use 100%. The HTTP service retains its separate 100% default
for `RendererTestPercentage`.

Every top-level `.rpl` file is considered, regardless of filename structure.
Physics uses the full corpus. With `target: 1`, renderer selection first
filters for a nonzero opponent type in the replay header. Renderer sampling
then uses the eligible, ordinal-sorted list before shard balancing; the
percentage applies to that list. With `target: 0`, all replays remain eligible.
The CI planning job reads recorded tick counts from replay headers and
balances physics and renderer shards separately, targeting totals
within ±2% of each phase's average. It publishes `shard-plan.json` as the
`replay-shard-plan` artifact. Replay jobs, oracle extraction, and coverage checks
all consume that same plan and validate its target; the C# application selects
its lists by shard ID. A corpus with no opponents has empty renderer shards
when `target: 1`, while physics still covers every replay.
Workers within a shard still receive round-robin lists whose replay counts
differ by at most one. Sampling is independent of shard and worker counts.
CI validates completed replay identities from the JSON shard results, so missing
or duplicate coverage, processing errors, and byte mismatches fail validation.

The reusable workflow defaults to 20 shards with 5 workers each, 120 seconds
per physics execution, and 980 seconds per renderer execution. It checks C/H
formatting, host regressions, and C# tests and formatting before building with
native Open Watcom 2. The build phase then checks the DOS platform ABI, dump
timer masking, and failure cleanup. Run the C# checks locally with the .NET 10 SDK:

```sh
python3 tools/scripts/test-plan-replay-shards.py
dotnet test tools/scripts/dumpsrv/dumpsrv.slnx --configuration Release
dotnet format tools/scripts/dumpsrv/dumpsrv.slnx --verify-no-changes
```

### Compiler migration validation

For a deterministic comparison of 100 evenly spaced golden replays in both
physics and rendering, build `repldump`, `pixldump`, and `pixldump-original`, then run:

```sh
python3 tools/scripts/validate-toolchain.py --output out/watcom-validation
```

Use a new output directory. This verifies the archived Borland checksums,
uses the archived physics oracle and freshly built incremental renderer reference,
records SHA-256 fingerprints of executables and inputs, and generates fresh
outputs in an isolated DOS directory. The shared C# runner checks complete
per-frame physics data and camera-2/player framebuffer hashes byte for byte.
See [the oracle guide](tools/oracles/borland/README.md) for coverage, timeout,
and cache options. Run the platform ABI check separately:

```sh
make -C src/restunts test-dos-platform
python3 tools/scripts/run-dos-platform-test.py
```

Check the conventional-memory budget with a complete race, including the
player dashboard and both 320x200 VGA render pages:

```bash
make -C src/restunts test-race-memory
python3 tools/scripts/run-race-memory-test.py --dosbox dosbox
python3 tools/scripts/run-race-memory-test.py --dosbox dosbox-x
```

This regression loads DIA3 for the player and CSIL for the opponent on
`DEFAULT.TRK`, draws with SuperSight off, on, then off again, and reports the
remaining conventional memory. It checks page isolation and presentation, changes
gears and gauges, and reloads the race to check drawing and resource release.
It also requires that the conventional-memory race framebuffer is absent. Run it
in both DOSBox and DOSBox-X; the latter has a smaller conventional-memory budget. The bundled game
and custom car resources are copied to an isolated directory. The test links
all normal game objects; its small test entry makes its memory budget slightly
stricter than the game.


## Build options

### Assembler

All assembly targets use Open Watcom WASM from the same pinned installation
as WCC and WLINK, with 8086 code generation. `-zcm=tasm` selects WASM's
built-in compatibility mode for the preserved assembly syntax; it does not
require the Turbo Assembler tools.

### Compiler, linker, and debugging symbols

All DOS C targets use Open Watcom 2 WCC and WLINK. `LINKER=wlink` is the only
supported linker setting. `setpath.bat` puts the pinned Watcom tools before
bundled utilities on PATH and sets `WATCOM` and `INCLUDE` accordingly.

Shared flags live in `src/restunts/watcom.mk`: 8086 instructions, the medium
memory model (far code and near data), the stack-based C calling convention,
signed `char`, and byte-packed structures. These settings preserve the
original game's 16-bit data layout and assembly interfaces. The custom DOS
startup initializes the stack and BSS; compiler stack probes are disabled.
The runtime libraries and headers come from the same pinned Watcom release.
Portable game C uses size optimization (`-os`). The DOS platform layer and
startup are compiled without optimization (`-od`) because the pinned compiler
can incorrectly merge branches around inline assembly interrupt calls.
DOS resource pointers are explicitly normalized to a paragraph plus an offset
below 16 bytes before they reach fixed-segment sprite code.
[Watcom huge-pointer arithmetic](https://github.com/open-watcom/open-watcom-v2/blob/2026-09-01-Build/bld/clib/cgsupp/a/pia.asm)
preserves larger offsets; normalization retains the Borland representation
and prevents bitmap reads from wrapping at a 64 KiB boundary.

Use `CONFIG=debug` to request Watcom C debug information. C optimization is
disabled except for the original renderer wrapper described below:

```text
make CONFIG=debug restunts
```

For `pixldump-original`, `pixldump.c` and `murmur3.c` retain size optimization
(`-os`) and use line debugging (`-d1`) without local-variable information.
This preserves the release code generation and stack layout required by the
original rendering code. Other objects use their usual debug flags.

WLINK writes Watcom debug information; debug builds also request WASM line
information.

## The toolchain

| Purpose | Active tool |
| --- | --- |
| 16-bit DOS C compilation | Open Watcom 2 `binnt/wcc.exe` or native Linux `binl64/wcc` |
| 16-bit DOS linking | Open Watcom 2 `binnt/wlink.exe` or native Linux `binl64/wlink` |
| C headers and runtime | Open Watcom 2 `h/` and `lib286/` |
| Assembly | Open Watcom 2 `binnt/wasm.exe` or native Linux `binl64/wasm` |
| Build orchestration | GNU Make 4.3 or newer (bundled 4.4.1 on Windows) |
| Original-source preparation | Python 3.9 or newer, for the `*-original` targets with WASM |
| Running and testing | DOSBox / DOSBox-X |

Current makefiles select Watcom executables by their full installation paths.
Open Watcom supplies all C headers and runtime libraries. Regression oracles
retain their original Borland-built machine code.

## Debugging restunts.exe

`CONFIG=debug` builds Watcom debug information and writes linker map files
beside the executables. Use a debugger that supports Watcom's format.
