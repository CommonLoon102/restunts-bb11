#!/usr/bin/env bash
# GitHub's Ubuntu runner supplies Java 17 and Android command-line tools.
set -euo pipefail
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)
source "$repo_dir/android/toolchain.properties"
sdk_root=${ANDROID_HOME:-${ANDROID_SDK_ROOT:?Set ANDROID_HOME or ANDROID_SDK_ROOT}}
sdkmanager="$sdk_root/cmdline-tools/latest/bin/sdkmanager"

# Avoid pipefail treating yes's expected SIGPIPE as a failed license acceptance.
set +o pipefail
yes | "$sdkmanager" --sdk_root="$sdk_root" --licenses > /dev/null
license_status=${PIPESTATUS[1]}
set -o pipefail
if [[ "$license_status" != 0 ]]; then
    exit "$license_status"
fi
"$sdkmanager" --sdk_root="$sdk_root" "platforms;android-$ANDROID_COMPILE_SDK" \
    "build-tools;$ANDROID_BUILD_TOOLS" "ndk;$ANDROID_NDK" "cmake;$ANDROID_CMAKE"
