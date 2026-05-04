#!/usr/bin/env bash
set -e

FAILED=0

while IFS= read -r -d '' file; do
  if ! diff -u "$file" <(clang-format "$file"); then
    FAILED=1
  fi
done < <(
  find . \
    \( -path ./build -o -path ./build-\* -o -path ./.git -o -path ./scripts \) -prune \
    -o \( -name "*.cpp" -o -name "*.hpp" \) -print0
)

if [ "$FAILED" -ne 0 ]; then
  echo "Formatting issues detected."
  exit 1
fi