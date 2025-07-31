#!/usr/bin/env bash
set -eu
cd "$(dirname "$0")"

# --- Unpack Arguments --------------------------------------------------------
for arg in "$@"; do declare $arg='1'; done
if [ ! -v gcc ];     then clang=1; fi
if [ ! -v release ]; then debug=1; fi
if [ -v debug ];     then echo "[debug mode]"; fi
if [ -v release ];   then echo "[release mode]"; fi
if [ -v clang ];     then compiler="${CC:-clang}"; echo "[clang compile]"; fi
if [ -v gcc ];       then compiler="${CC:-gcc}"; echo "[gcc compile]"; fi

if [ -v gcc ]; then echo "[gcc not supported]"; exit 1; fi

# --- Unpack Command Line Build Arguments -------------------------------------
auto_compile_flags=''

# --- Get Current Git Commit Id -----------------------------------------------
git_hash=$(git describe --always --dirty)
git_hash_full=$(git rev-parse HEAD)

# --- Compile/Link Line Definitions -------------------------------------------
clang_common="-g -DBUILD_GIT_HASH=$git_hash -DBUILD_GIT_HASH_FULL=$git_hash_full -fdiagnostics-absolute-paths -Wall  -Xclang -flto-visibility-public-std"
clang_debug="$compiler -g -O0 -DBUILD_DEBUG=1 ${clang_common} ${auto_compile_flags}"
clang_release="$compiler -g -O2 -DBUILD_DEBUG=0 ${clang_common} ${auto_compile_flags}"
clang_out="-o"

# --- Choose Compile/Link Lines -----------------------------------------------
if [ -v clang ];   then compile_debug="$clang_debug"; fi
if [ -v clang ];   then compile_release="$clang_release"; fi
if [ -v clang ];   then out="$clang_out"; fi
if [ -v debug ];   then compile="$compile_debug"; fi
if [ -v release ]; then compile="$compile_release"; fi

# --- Prep Directories --------------------------------------------------------
mkdir -p build

# --- Build  ------------------------------------------------------------------
cd build
$compile ../src/main.c $out krypton
cd ..
