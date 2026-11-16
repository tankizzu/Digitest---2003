#!/bin/bash
# Builds the APK: the game for Android arm64 (build_unix.py --android), then the Gradle app around it.
#   ANDROID_HOME   the Android SDK (platform 35, build-tools 35)
#   ANDROID_NDK    the NDK (27.x)
#   SDL3_PREFIX    SDL 3.4.16 built for arm64-v8a (default: ../sdl3-android next to this repository)
set -e
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
python3 "$ROOT/build_unix.py" --android
LIBS="$ROOT/android/app/src/main/jniLibs/arm64-v8a"
mkdir -p "$LIBS"
cp "$ROOT/build-android/libdw3.so" "$ROOT/build-android/libmain.so" "${SDL3_PREFIX:-$ROOT/../sdl3-android}/lib/libSDL3.so" "$LIBS/"
cd "$ROOT/android" && ./gradlew --no-daemon -q assembleRelease
cp app/build/outputs/apk/release/app-release.apk "$ROOT/build-android/dw2003.apk"
echo "OK $ROOT/build-android/dw2003.apk"
