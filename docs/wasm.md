# Browser / WebAssembly build

The SDL3 platform also builds with Emscripten. The output is one offline
`restunts.html`: JavaScript, WebAssembly, enhanced skyboxes, and enhanced opponent
portraits are embedded. **No original game data is included**, including cars,
tracks, replays, sounds, fonts, or menus. The build does not require a `stunts/`
directory. The redistributable package and relink kit also exclude that data.

## Build

Install the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html),
CMake 3.25+, and Ninja. Emscripten 6.0.10 is the tested SDK version. Activate its
environment, then run from the repository root:

```sh
. /path/to/emsdk/emsdk_env.sh
emcmake cmake -S . -B out/sdl3-wasm -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build out/sdl3-wasm
```

Use `emsdk_env.bat` in a Windows command prompt. This build uses the same pinned
SDL3 source as native builds. Native dump tools and native regressions are not
built for the browser. No pthreads, shared memory, server headers, CDN, or network
requests are needed at runtime. Emscripten's single-file output and embedded
artwork avoid requests for neighboring `.wasm`, `.js`, or `.data` files.

## Play from a website or offline

You can open a hosted `restunts.html` directly in your browser; saving a copy
is optional. For offline play, save the HTML file anywhere on your device and
open it in your browser. `file://` is supported, so a local web server is not
needed. The HTML's location does not determine where the game looks for data.

1. Have your own Broderbund Stunts 1.1 game files in a folder on your device.
2. Click **Choose game folder** and select the folder containing those files.
   Supported browsers remember it for this page and reopen it when permission
   is still granted. Otherwise, click **Allow folder access** to approve access
   again. Use **Change game folder** to select a different folder.
3. Click **Start game**. This user gesture enables browser audio. **Skip intro**
   is optional. Click the canvas to focus the game keyboard.

**Your game files and saves stay on your device and are never uploaded**, even
when you play from a website. Selecting a folder gives the game local access to
its files; it does not send them to the hosting server.

Only immediate game files are loaded; subfolders, executables, and the HTML are
ignored. Filenames are resolved case-insensitively. Identical files differing
only in case are accepted; conflicting contents report an error. The selected
folder must contain the original game resources, including `MAIN.RES` (or
`MAIN.PRE`), `FONTDEF.FNT`, and `FONTN.FNT`. Other missing resources are reported by
the game. Files are read locally and are never uploaded.

On **Start game**, the game checks the version text in `MISC.RES` (preferred) or
`MISC.PRE`. It requires Broderbund Stunts 1.1 (12 Feb. 1991), while allowing
modifications to other resources. A missing or incompatible version displays an
error on the page; reload and choose the correct game folder before starting
again.

## Saving and browser support

Chrome and Edge support a folder picker with **read/write** permission. After
that permission is granted, game saves are written directly into the selected
folder, preserving existing filename casing. The game waits for each write to
finish before treating the file as closed. Disk failures are reported to the
game and on the page. Deletions are also propagated. Closing or reloading a tab
while a save is in progress can interrupt it; wait until the page reports the
save is complete.

The selected directory handle is stored in IndexedDB for this HTML's location;
original game data is not stored in that database. On reopening, the game checks
read/write permission and asks for approval through **Allow folder access** only
when needed. Clearing browser storage, moving the HTML, or browser restrictions
can require selecting the folder again. If storage is unavailable, folder
selection and saving still work for the current session.

Firefox, Safari, and other browsers without that API use a read-only directory
picker and require selecting the folder each time. The page shows **Export saves** and **Import saves, tracks, or replays**
only on this fallback path (or after a direct-write failure). Export downloads a
JSON bundle of changed/new tracks, replays, high scores, and configuration files;
it does not copy unchanged original resources. Select the game folder and import
that bundle before starting the next session. Imported files are available to
the game in memory; this fallback does not overwrite the selected disk folder.
Deleting files in fallback mode affects the current session only. Landscape
cache files are regenerated rather than exported.

Browser support for directory writes is described in
[Chrome's File System Access documentation](https://developer.chrome.com/docs/capabilities/web-apis/file-system-access)
and [MDN's directory picker reference](https://developer.mozilla.org/en-US/docs/Web/API/Window/showDirectoryPicker).
Both paths work offline. Browser settings and policies may restrict folder
access. Audio needs a user gesture; browser shortcuts can take precedence over
some function keys. Use the page's **Fullscreen** button for fullscreen mode.
With the canvas focused, **F12** toggles HyperVision Auto and **Shift+F12** cycles
fixed **Full → High → Medium → Low** presets. **F11** or **Shift+F11** toggles
the FPS and render-time display on or off. Use Shift+F11 to avoid the browser
fullscreen shortcut assigned to F11.

The browser presents frames through SDL3's WebGL renderer and uses WebAudio
output. Scene rendering runs on the CPU and is serial;
`RESTUNTS_RENDER_WORKERS` does not enable threads. Asyncify preserves the existing
nested game/menu loops. Browser waits share one scheduler: explicit waits also
satisfy the bounded input/presentation yield interval, and SDL's additional
implicit sleeps are disabled. Each wait resumes through a `MessageChannel` task
so repeated short waits do not accumulate the browser's nested-timer minimum.
The existing game clock still targets 60 visual FPS in HyperVision; physics and
replay formats use the same game code as native builds. This does not require
an Emscripten main-loop callback or change the native VSync setting.
Background tabs may still be throttled by the browser.

## Distribution and rebuilding the audio library

```sh
cmake --install out/sdl3-wasm --prefix out/package-wasm --component Runtime
```

The installed HTML runs by itself, but redistribute the complete package to
include the required dependency notices, licenses, and Nuked OPL2 relinking
materials. Original game files must be supplied separately by each player.
Do not copy them into the package.

The browser links Nuked OPL2 Lite statically. The package includes its exact
source and a standalone relink kit under `share/restunts/wasm-relink/`, containing
the compiled application/SDL objects, enhanced artwork, HTML shell, and build
instructions. Recipients can modify and rebuild Nuked and relink the HTML
without the game's source or original game data. Use the SDK version recorded
in the kit. See its `wasm-relink.md` and `THIRD-PARTY-NOTICES.txt` for details.

## Verification

The pacing regression builds a small WebAssembly fixture with the real Asyncify
wait implementation, then runs it in Playwright without original game data.
It checks that input polling shares explicit waits, repeated short waits resume
through browser message tasks without nesting timers, and waits complete even
when animation callbacks are unavailable. Activate the Emscripten SDK and install
Playwright as described below, then run:

```sh
NODE_PATH="$PWD/out/wasm-test-tools/node_modules" node tools/scripts/test-wasm-pacing.js \
    --sdl-include out/sdl3-wasm/_deps/sdl3-src/include
```

Use `--browser firefox` for Firefox. `--emcc` accepts an explicit compiler path,
and `--sdl-include` names the SDL3 public include directory from a configured build.

The actual WebAssembly file I/O regression checks asynchronous commit ordering
and failure propagation with Node.js (included in the SDK):

```sh
emcc -O2 -Wno-pointer-sign -DRESTUNTS_SDL3 -DRESTUNTS_HEADLESS \
    -sASYNCIFY -sASYNCIFY_STACK_SIZE=65536 -sENVIRONMENT=node -sEXIT_RUNTIME=1 -sASSERTIONS=1 \
    src/restunts/tests/test-wasm-fileio.c src/restunts/platform/sdl3/file.c \
    src/restunts/c/fileio.c -o out/test-wasm-fileio.js
node out/test-wasm-fileio.js
```

The browser smoke test uses Node.js and Playwright with Chromium installed:

```sh
npm install --prefix out/wasm-test-tools playwright
out/wasm-test-tools/node_modules/.bin/playwright install chromium firefox
NODE_PATH="$PWD/out/wasm-test-tools/node_modules" node tools/scripts/test-wasm.js \
    --html out/sdl3-wasm/restunts.html --data-dir stunts
```

Add `--browser firefox --output out/wasm-browser-test/firefox-results` to run
the same smoke test in Firefox. Chromium and Firefox have both been tested.
Add `--idle-only` to check that the main-menu idle demo starts and keeps playing;
this waits for the normal idle timeout and requires `DEFAULT.RPL` in the supplied folder.

The game data path is a local test input only; it is not embedded or packaged.
The test opens the HTML with `file://`, rejects runtime network dependencies,
checks that original resources are absent until a folder is selected, exercises
keyboard gameplay and fallback save round trips, and checks direct-write
behavior using test directory adapters. Remembered-folder checks exercise real
IndexedDB across reloads with simulated permission approval, denial, and retry.
The real native directory picker and permission dialogs still require a manual
browser check.
