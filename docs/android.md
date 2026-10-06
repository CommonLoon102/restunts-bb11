# Android port

The Android APKs run in landscape. `android-armv7` contains 32-bit ARM
(`armeabi-v7a`) libraries and supports Android 5.0 (API 21) or newer, including
the Samsung Galaxy S5 SM-G900F. `android-arm64` contains 64-bit ARM (`arm64-v8a`)
libraries and supports Android 5.0 (API 21) or newer on phones and Android TVs
running a 64-bit ARM Android system. Each APK contains only its target
architecture. Some TVs with 64-bit CPUs run 32-bit Android; those need the
ARMv7 APK. Android 4.x is below the supported SDL3/native-toolchain minimum.

## Game files

The APK does not contain original Stunts game data. Use **Choose game folder**
to select the folder containing your Brøderbund Stunts 1.1 (12 Feb. 1991) game
files. This is the single visible folder for original resources, custom cars,
opponents, graphics, tracks, replays, high scores and any other game files.
The choice is remembered. Copy custom content into this folder with a file
manager, then press **Start game** again to load it.

For **Import Stunts ZIP**, first select the ZIP. After validation, Android's
folder picker asks you to create or select **Documents/Chocolate Stunts** (or
another destination). The app extracts the game into that exact folder and
remembers it. Only the selected game folder receives persistent read/write
access; no grant for all of Documents or the whole storage is needed. ZIPs may
contain a parent directory around the game files; its game root is extracted
without that wrapper. Choose an empty destination for different game data:
ZIP import preserves existing files and refuses to replace different contents.
An interrupted extraction can leave completed files in the destination; retrying
the same ZIP reuses identical copies and finishes missing files.

Both folder selection and ZIP import require `MAIN.RES` (or `MAIN.PRE`),
`FONTDEF.FNT` and `FONTN.FNT`, and validate the `gver` resource in `MISC.RES`, or
`MISC.PRE` when no unpacked resource exists. The accepted value is exactly
`Version 1.1 (Feb 12 1991)`, matching the common SDL3 startup check. A present
but invalid `MISC.RES` does not fall back to `MISC.PRE`. Other game resources
are not compared, so custom content remains supported. Validation also runs
before every start; bad or missing data does not replace the last working cache.
The game reports other missing resources during loading.

No network or broad storage permission is declared. Access follows the
[Android Storage Access Framework](https://developer.android.com/training/data-storage/shared/documents-files),
using the system picker's grant for one folder. Prefer a normal shared folder
such as Documents rather than `Android/data`, which newer Android versions
restrict. The document provider must support creating, renaming and deleting
files so failed replacements can preserve previous data. Game content is limited
to 4096 files/subfolders and 256 MiB in total, including custom videos.

The native game uses a private working cache because document-provider URIs
cannot be opened as normal filesystem paths. Every **Start game** refreshes
that cache from the selected folder, including externally edited, added and
deleted files and empty subfolders. This cache is managed automatically; users only manage the one
selected public game folder. Bundled optional port artwork/music and launcher
preferences remain in app storage, with custom content in the game folder
available to the existing resource loaders.

Completed game writes and screenshots are copied to the selected game folder
immediately. Failed copies retain a durable pending copy outside the replaceable
cache and show an explanation; **Start game** retries them before refreshing.
If the shared file was also edited, the app preserves both versions and reports
the conflict. Rename the shared version to keep it, then retry to publish the
pending game write under its original name. Files in the chosen folder survive
app uninstallation; after reinstalling, choose that same game folder. Private
preferences, cached data and unexported pending writes are removed by clearing
app storage or uninstalling.

The options row above **Start game** contains **New MIDI** for replacement
music, **Show FPS** for the FPS/render-time display, and **HyperVision** with
**Off**, **Auto**, **Full**, **High**, **Medium**, and **Low** presets. Start game
applies all three selections; they are remembered for later launches. The
defaults use original AdLib music, hide FPS, and leave HyperVision off.

## Touch controls

With **Keyboard** input selected in Options, transparent circles appear at the
screen edges. Left/right steer; upper/lower right accelerate/brake. They support
simultaneous held touches. Moving outside a circle releases that control.
The steering cluster has no inset from the usable left edge; display cutouts
and system bars are respected.

During a live race, bottom-left **Q** holds the existing rewind function. **Shift
up/down** appear above/below steering only with manual gears selected. Each tap
sends one gear-change request; holding does not repeatedly shift. Q and shifting
are hidden in menus and replays. The four arrows navigate menus and replay
controls; **ESC** is at the upper left. **C** is at the upper right and **T**
is directly below it. They send one C/T key press per tap, cycling the camera
and followed car exactly like the keyboard shortcuts. Tap the game buttons
to activate choices.

All existing car/opponent and replay toolbar buttons accept direct touch,
including the camera icon, camera adjustment controls, and replay timeline.
Drag or tap the timeline to seek. In replay mode, swipe left/right above the
toolbar to skip backward/forward ten seconds, clamped to the recording's ends.
Seeking pauses playback. Text fields open the Android software keyboard.
Text-entry dialogs appear at the top so names and paths remain visible while
editing highscore names, track/replay filenames, or file-picker directories.

## Android TV

Both APKs declare touchscreen support optional and provide a TV launcher entry,
icon and banner, following [Android TV setup](https://developer.android.com/training/tv/start/start).
Use the APK matching the TV's Android ABI. Import still uses Android's document
picker; install a compatible document picker if the TV does not provide one.
An unavailable picker displays an explanation instead of crashing.

Use an external keyboard or compatible controller for racing. The TV remote
navigates the import screen with its D-pad and center/select; Back leaves it.
In the game, basic remotes send arrows, Enter and Escape, and stay separate
from the racing joystick. This uses SDL's
[remote keyboard input setting](https://wiki.libsdl.org/SDL3/SDL_HINT_TV_REMOTE_AS_JOYSTICK).

An external keyboard works with **Keyboard** selected: arrows drive, A/Z shift,
Q rewinds, Enter selects and Escape leaves racing/replay. For a controller,
select **Joystick** and calibrate in Options. The existing joystick controls
use the left stick X/Y axes for steering and acceleration/braking and the
first two buttons for shifting. This port does not yet map an opened Android
gamepad's D-pad buttons or shoulder/trigger controls. A separate TV remote or
keyboard can continue to navigate menus. Remote-only racing is not a supported
setup; controller guidance follows
[Android TV controller requirements](https://developer.android.com/training/tv/get-started/controllers).

## Build

Install Java 17, Python 3.12+, Android SDK platform 35 and build tools 35.0.0,
Android NDK 28.2.13676358, and SDK CMake 3.31.6. Set `JAVA_HOME` and
`ANDROID_HOME` (or `ANDROID_SDK_ROOT`) to your toolchain locations. Then run:

```sh
bash tools/scripts/build-android.sh android-armv7
bash tools/scripts/build-android.sh android-arm64
```

Omit the target (or use `all`) to build both APKs.

The helper downloads and verifies the repository's pinned SDL source, uses its
matching Android Java glue and Gradle wrapper, and builds the shared native
game through the root CMake project. Gradle 8.12 and Android Gradle Plugin 8.7.3
are pinned. SDK/NDK/CMake versions and the per-ABI minimum APIs are shared with CI in
`android/toolchain.properties`. The results are:

- `android/app/build/outputs/apk/armv7/debug/app-armv7-debug.apk`
- `android/app/build/outputs/apk/arm64/debug/app-arm64-debug.apk`

When the checkout lives on a VM shared folder, put native archives and Gradle
outputs on a local disk to avoid filesystem stalls:

```sh
bash tools/scripts/build-android.sh android-armv7 \
    -PrestuntsAndroidBuildRoot=/tmp/restunts-android-build
```

The APK then appears at
`/tmp/restunts-android-build/app/build/outputs/apk/armv7/debug/app-armv7-debug.apk`.

Install with `adb install -r PATH_TO_APK`.
This development APK uses a debug signing key and optimized native code with
debug symbols. For distribution, open `android/`
in Android Studio after running the helper and configure your own release key.
No signing keys are stored in the repository.

## CI packages

PR validation and Release both call the shared package workflow. Its Android
matrix builds `android-armv7` and `android-arm64` independently and uploads
`packages-android-armv7` and `packages-android-arm64`. Each artifact contains
`restunts-android-<cpu>.zip` and its SHA-256 sidecar. The ZIP includes the
installable `bin/restunts.apk`, these instructions, dependency licenses, and
the exact Nuked OPL2 library sources/build settings. Release verifies and
attests both archives with the other platform packages before publishing.
Original Stunts data is never downloaded by the Android build or bundled.

Each job tests the folder/ZIP importer, version validation, launcher options and complete-folder synchronization,
checks the APK signature and 16 KB
alignment, verifies each target's minimum SDK (21 for both ARMv7 and ARM64)
and common target SDK, and checks that all native libraries
match the selected ABI. Package verification also checks required enhancement
assets/licenses and rejects embedded original game resources. CI builds do
not replace installation, performance, audio, controller, or lifecycle testing
on a real Galaxy S5, phone, or TV. Both APKs include a classic launcher
icon for Android 5-7 and a JAR/v1 signature for pre-Android 7 installation;
newer systems can use their adaptive icons and newer signature schemes.

CI APKs use the Android debug signing key generated on that runner. A later CI
run may have a different key, so Android can require uninstalling the old APK
first, which removes private preferences and unexported writes but leaves the selected public folder. Use one persistent private
release key for builds intended to update an installed copy.

## Replacing Nuked OPL2

The CI ZIP includes `share/restunts/nuked-opl2-lite/` with the exact library
source and its standalone CMake build. Build it with the pinned NDK toolchain,
`ANDROID_PLATFORM=android-21` for either `armeabi-v7a` or `arm64-v8a`.
`nuked-build-info.txt` records the original compiler and flags.
Replace `lib/<abi>/libnuked-opl2.so` inside a copy of the APK with the rebuilt
shared library, then use SDK `zipalign -P 16` and `apksigner` to align and sign
the modified APK with your own key. The library must retain the public ABI in
`opl2.h`. Re-signing with a different key requires uninstalling the original
app before installing the replacement. No game executable relinking is needed.

## Verification

The importer, version validation, launcher options and folder synchronization can be tested without Android:

```sh
javac -d out/android-import-tests \
    android/app/src/main/java/org/restunts/android/GameDataImport.java \
    android/app/src/main/java/org/restunts/android/GameDataVersion.java \
    android/app/src/main/java/org/restunts/android/LaunchOptions.java \
    android/app/src/main/java/org/restunts/android/GameDataSync.java \
    android/tests/GameDataImportTest.java android/tests/GameDataVersionTest.java \
    android/tests/LaunchOptionsTest.java \
    android/tests/GameDataSyncTest.java
java -cp out/android-import-tests org.restunts.android.GameDataImportTest
java -cp out/android-import-tests org.restunts.android.GameDataVersionTest
java -cp out/android-import-tests org.restunts.android.LaunchOptionsTest
java -cp out/android-import-tests org.restunts.android.GameDataSyncTest
```

Host CTest covers touch geometry, visibility, simultaneous holds, cancellation,
single-tap shifting, replay gestures, and pointer delivery. Test installation,
audio, background/resume, keyboard, and real multitouch on the target phone.
