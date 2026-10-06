#!/usr/bin/env bash
# Build one installable ABI-specific APK and its source/license runtime tree.
set -euo pipefail
script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_dir=$(cd -- "$script_dir/../.." && pwd)
target=${1:?Usage: build-android-package.sh TARGET RUNTIME_DIRECTORY}
runtime_dir=${2:?Specify the empty runtime directory}
source "$repo_dir/android/toolchain.properties"
apk_alignment_bytes=4
apk_page_alignment_kb=16
case "$target" in
    android-armv7) flavor=armv7; abi=armeabi-v7a; minimum_sdk=$ANDROID_ARMV7_MIN_SDK ;;
    android-arm64) flavor=arm64; abi=arm64-v8a; minimum_sdk=$ANDROID_ARM64_MIN_SDK ;;
    *) echo "Unknown Android package target: $target" >&2; exit 1 ;;
esac
build_root=${RESTUNTS_ANDROID_BUILD_ROOT:-${RUNNER_TEMP:-/tmp}/restunts-$target}
sdk_root=${ANDROID_HOME:-${ANDROID_SDK_ROOT:?Set ANDROID_HOME or ANDROID_SDK_ROOT}}
build_tools="$sdk_root/build-tools/$ANDROID_BUILD_TOOLS"
mkdir -p "$runtime_dir"
if [[ -n "$(ls -A "$runtime_dir")" ]]; then
    echo "Android runtime directory must be empty: $runtime_dir" >&2
    exit 1
fi
bash "$script_dir/build-android.sh" "$target" --no-daemon \
    "-PrestuntsAndroidBuildRoot=$build_root"
apk="$build_root/app/build/outputs/apk/$flavor/debug/app-$flavor-debug.apk"
"${JAVA_HOME:?Set JAVA_HOME to Java 17}/bin/java" -jar "$build_tools/lib/apksigner.jar" \
    verify --verbose "$apk"
"$build_tools/zipalign" -c -P "$apk_page_alignment_kb" "$apk_alignment_bytes" "$apk"
badging=$("$build_tools/aapt" dump badging "$apk")
if ! [[ "$badging" == *"sdkVersion:'$minimum_sdk'"* && \
        "$badging" == *"targetSdkVersion:'$ANDROID_COMPILE_SDK'"* && \
        "$badging" == *"native-code: '$abi'"* ]]; then
    echo "Android APK has incorrect SDK or native ABI requirements." >&2
    exit 1
fi

# A reused build root can retain the previous minimum SDK's configuration hash.
# Only install the native tree matching the selected ABI and minimum API.
native_builds=()
for cache in "$build_root/app/.cxx/Debug/"*"/$abi/CMakeCache.txt"; do
    if [[ -f "$cache" ]] && \
            grep --quiet --extended-regexp "^ANDROID_PLATFORM:[^=]+=android-$minimum_sdk$" "$cache" && \
            grep --quiet --extended-regexp "^ANDROID_ABI:[^=]+=$abi$" "$cache"; then
        native_builds+=("$cache")
    fi
done
if [[ "${#native_builds[@]}" != 1 ]]; then
    echo "Expected one Android native build for $abi at API $minimum_sdk." >&2
    exit 1
fi
"$sdk_root/cmake/$ANDROID_CMAKE/bin/cmake" --install "${native_builds[0]%/CMakeCache.txt}" \
    --prefix "$runtime_dir" --component Runtime
mkdir -p "$runtime_dir/bin"
cp -- "$apk" "$runtime_dir/bin/restunts.apk"
