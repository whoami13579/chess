#!/bin/sh
set -eu

if ! command -v cmake >/dev/null 2>&1; then
    echo "Missing CMake. Install CMake 3.16 or newer." >&2
    exit 1
fi

compiler_found=false
for compiler in "${CXX:-}" c++ g++ clang++; do
    if [ -n "$compiler" ] && command -v "$compiler" >/dev/null 2>&1; then
        compiler_found=true
        break
    fi
done
if [ "$compiler_found" = false ]; then
    echo "Missing C++ compiler. Install a compiler with C++17 support." >&2
    exit 1
fi

script_dir=$(CDPATH= cd "$(dirname "$0")" && pwd)
exec cmake "$@" -P "$script_dir/../cmake/CheckEnvironment.cmake"
