#!/usr/bin/env bash
set -euo pipefail
restunts_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)
task=assembleDebug
case "${1:-}" in
    android-armv7) task=assembleArmv7Debug; shift ;;
    android-arm64) task=assembleArm64Debug; shift ;;
    all) shift ;;
    -h|--help)
        echo 'Usage: build-android.sh [android-armv7|android-arm64|all] [GRADLE_OPTIONS...]'
        exit 0
        ;;
    ""|-*) ;;
    *) echo "Unknown Android build target: $1" >&2; exit 1 ;;
esac
python3 "$restunts_root/tools/scripts/prepare-android.py"
exec "$restunts_root/out/android-gradle/gradlew" -p "$restunts_root/android" "$task" "$@"
