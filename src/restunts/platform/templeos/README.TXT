Restunts for TempleOS / TinkerOS
==============================

The HolyC port uses the shared DOS game code, native 64-bit pointers, and
explicit legacy integer conversions. It has no SDL, OPL, PNG, or Ogg dependency.

Display and original assets
---------------------------

The internal 3D resolution is 640x400: projection and rasterization both use
the finer pixel grid. The 8:5 image is centered on the 640x480 display, with
40-pixel black bars above and below. Original bitmap artwork, fonts, and UI
retain their source detail at a uniform 2x scale. Mouse coordinates, clipping,
and saved backgrounds follow the same layout. DOS resource sizes and stored
offsets retain their original widths.

The first sixteen VGA palette entries drive the hardware's sixteen colors.
Original bitmap resources containing !eg0 supply a 256-entry table of packed
EGA color pairs; the renderer retains these pairs through sprite composition.
The checkerboard phase is a port presentation choice, not a verified byte-exact
reconstruction of the EGA executable's dithering phase. Resources without that
table use TemplePaletteNearest: minimum weighted squared RGB6 distance, with
red/green/blue weights 30/59/11 and the lowest index winning a tie. Transparency
and AND/OR masks operate before color conversion. Palette changes rebuild the
lookup table and update the hardware palette so the original fades still work.

3D geometry uses the original EGA driver's 129 primary colors, secondary colors,
pattern types, and pattern masks, installed by MATERIAL.HC after game-data
initialization. These tables must not be replaced by SDMAIN's !eg0 bitmap map:
several VGA blue material shades map to purple through that map. Bitmap resources
keep their own !eg0 pairs, while original UI lines fill complete 2x2 source pixels.
Native 3D lines retain their one-pixel thickness at 640x400.

Material-table provenance: Broderbund Stunts 1.1 EGA.CMN + EGA.COD, resource
unpacked, concatenated, then EXEPACK decoded. The decoded image SHA256 is
90841d1ac8aa27d47c94af74911a36b0ed4baf03db5f9500121da5965b8d93df.
The four 129-word little-endian tables start at image offsets 0x356e4 (primary),
0x357e6 (pattern type), 0x358e8 (pattern bits), and 0x359ea (secondary).
The pointer block at 0x35aec confirms these tables relative to data base 0x30650.

The PC speaker is the only audio device. The native backend uses the original
PC15 note periods, engine pitch conversion, resource envelopes, and sequencer.
TempleOS's IRQ0 system timer is not replaced. Keyboard and mouse are
supported; there is no TempleOS joystick backend.

Stock TempleOS installation CD
------------------------------

For the player guide, see docs/templeos.md in the repository. The stock package
contains a native RedSea RESTUNTS.ISO with INSTALL.HC and PLAY.HC, plus the game
sources and an empty DATA directory. Original game resources are not included.
Mount it as a CD in the existing TempleOS VM.
The installer copies and verifies a new C:/Restunts directory; it never replaces
an existing installation. tools/PACKAGE.PY builds the CD using stock TempleOS.
The package includes MAKE_DATA_CD.PY to transfer each player's own game files
on a separate private RedSea CD. See docs/templeos.md for both steps.

Loading in the guest
--------------------

Copy this directory's .HC/.HH files, including generated/, into C:/PORT and
the original Broderbund Stunts 1.1 game data into C:/DATA. Extensions must stay
uppercase. Use a writable data directory for tracks, replays, and configuration.
The game verifies the MISC resource version exactly as the DOS build does.

In a fresh TempleOS task:

    Cd("C:/PORT");
    #include "LOAD.HC";
    Restunts("C:/DATA");

Restunts("C:/DATA", TRUE) skips the intro. RUN.HC loads and starts the default
directory. COMPILE.HC loads without starting. Ctrl+Q opens the original exit
dialog; select Yes to return to the TempleOS prompt. Cleanup restores the
task's display settings and the system palette.

Preparing files on the host
---------------------------

From the repository root, prepare a new directory (inputs are never renamed):

    python3 src/restunts/platform/templeos/tools/PREPARE.PY \
        --data /path/to/original/game --output /path/to/new/package

The package contains PORT/ and DATA/. Case collisions with different contents
are rejected. Existing output directories are rejected to protect prior saves.
Copy those two folders to a guest disk. Do not mount a disk on the host while
the guest is using it. tools/VM.PY builds its own disposable FAT32 disk instead;
see tools/VM.TXT for the tested QEMU workflow and dependency installation.

Source and validation
---------------------

Handwritten native services live in this directory. generated/ is produced by
tools/TRANSLATE.PY from the repository's DOS full-game source list. Regenerate
after changing shared C code; do not hand-edit generated files. The translator
requires GCC for preprocessing, pycparser 2.22, and clang-format 18.1.8.
For example, from the repository root on a host with Python venv support:

    python3 -m venv /tmp/restunts-templeos-env
    /tmp/restunts-templeos-env/bin/pip install \
        -r src/restunts/platform/templeos/tools/REQUIREMENTS.TXT
    /tmp/restunts-templeos-env/bin/python \
        src/restunts/platform/templeos/tools/TRANSLATE.PY \
        --formatter /tmp/restunts-templeos-env/bin/clang-format

Add --check to verify regeneration without writing. See TRANSLATE.PY --help
for other options. Generated numeric literals come from the shared C constants;
maintain those named constants in the C source rather than editing generated code.

HolyC has different expression precedence, no ternary/continue/typedef, and
narrow integer casts do not implement C truncation. Boolean values outside
conditions also need explicit C short-circuit evaluation. The translator lowers
these constructs, initializes C static storage, preserves packed resource
layouts, and uses separate native pointer storage where DOS segmented layouts
cannot hold 64-bit pointers.

Run the guest checks with tools/VM.PY and a fresh disposable disk for each entry:

    COMPILE.HC        Compile the complete port without starting the game.
    TEST_NATIVE.HC    File persistence, input scaling, and timer callbacks.
    TEST_RENDERER.HC  Palette, native raster, masks, backgrounds, video cleanup.
    TEST_AUDIO.HC     Speaker note/engine/bend periods and PCM task cleanup.
    TEST_AUDIO_CORE.HC Native callbacks in the original packed audio structures.
    TEST_GRAPHICS.HC  EGA materials, generated graphics, layouts, and sprite storage.
    TEST_RESOURCES.HC Original-resource loading and native-buffer file round trips.
    TEST_REPLAY.HC    Compare 120 physics frames with an independent DOS oracle.

The replay check needs --data pointing to the original game directory, including
its unchanged DEFAULT.RPL. Tests create temporary files in the disposable guest;
they should not run against an existing game disk. tools/VM.TXT gives commands,
oracle provenance, and diagnostic paths. tools/TEST_TRANSLATE.PY builds a host
C oracle and matching HolyC language-semantics fixture; tools/TEST_RENDERER.PY
checks EGA materials and raster behavior with address/undefined-behavior sanitizers.
Host checks complement actual HolyC compilation and guest runtime tests.

Reference projects and documentation
------------------------------------

https://github.com/Church-of-Templeos/TOOM
https://www.templeos.info/Wb/Doc/HolyC.DD.HTML
https://www.templeos.info/Wb/Doc/Requirements.DD.HTML

Original game data is not duplicated in this source directory.
