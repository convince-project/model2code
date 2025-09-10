#!/usr/bin/env bash
# Helper script to build a portable model2code binary.
# It sets PORTABLE_BUILD=ON and uses a conservative -march to avoid SIGILL
# when running on different machines.
set -euo pipefail
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ROOT_DIR=$(cd "$SCRIPT_DIR/.." && pwd)
BUILD_DIR="$ROOT_DIR/build-portable"
mkdir -p "$BUILD_DIR"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DPORTABLE_BUILD=ON -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --target model2code -j $(nproc)

echo "Portable build produced at: $BUILD_DIR/model2code"
