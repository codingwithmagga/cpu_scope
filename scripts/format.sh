#!/usr/bin/env bash
set -e

find . \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-format-18 -i {} +
