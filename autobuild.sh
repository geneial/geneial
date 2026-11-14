#!/usr/bin/env sh
set -eu
cd "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
preset="${1:-release}"
cmake --preset "$preset"
cmake --build --preset "$preset" --parallel
if [ "$preset" != library ]; then
    ctest --preset "$preset" --output-on-failure
fi
