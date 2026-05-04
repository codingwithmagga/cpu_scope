#!/usr/bin/env bash
set -e

if [ ! -f build/compile_commands.json ]; then
  echo "compile_commands.json not found. Run CMake with:"
  echo "  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
  exit 1
fi

find . \
  \( -path ./build -o -path ./build-* -o -path ./.git -o -path ./scripts \) -prune \
  -o \( -name "*.cpp" \) -print0 \
  | xargs -0 -n1 clang-tidy -p build
