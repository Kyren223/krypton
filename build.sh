#!/usr/bin/env bash
set -eu
cd "$(dirname "$0")"

START_TIME=$(date +%s.%3N)

# --- Unpack Arguments --------------------------------------------------------
for arg in "$@"; do declare $arg='1'; done
if [ ! -v gcc ];     then clang=1; fi
if [ ! -v release ]; then debug=1; fi
if [ ! -v fast ];    then safe=1; fi
if [ -v debug ];     then echo "[debug mode]"; fi
if [ -v release ];   then echo "[release mode]"; fi
if [ -v safe ];     then echo "[safe mode]"; fi
if [ -v fast ];   then echo "[fast mode]"; fi
if [ -v clang ];     then compiler="${CC:-clang}"; echo "[clang compile]"; fi
if [ -v gcc ];       then compiler="${CC:-gcc}"; echo "[gcc compile]"; fi

if [ -v gcc ]; then echo "[gcc not supported]"; exit 1; fi

# --- Unpack Command Line Build Arguments -------------------------------------
auto_compile_flags=''

# --- Get Current Git Commit Id -----------------------------------------------
git_hash=$(git describe --always --dirty)
git_hash_full=$(git rev-parse HEAD)

# --- Compile/Link Line Definitions -------------------------------------------
clang_common="-g -DBUILD_GIT_HASH=\"$git_hash\" -DBUILD_GIT_HASH_FULL=\"$git_hash_full\" -fdiagnostics-absolute-paths -Wall -Wno-unused-function"
clang_safe="-DBUILD_SAFE=1 -fsanitize=undefined"
clang_debug_safe="$compiler -g -O0 -DBUILD_DEBUG=1 ${clang_safe} ${clang_common} ${auto_compile_flags}"
clang_debug_fast="$compiler -g -O2 -DBUILD_DEBUG=1 -DBUILD_SAFE=0 ${clang_common} ${auto_compile_flags}"
clang_release_safe="$compiler -g -O2 -DBUILD_DEBUG=0 ${clang_safe} ${clang_common} ${auto_compile_flags}"
clang_release_fast="$compiler -g -O2 -DBUILD_DEBUG=0 -DBUILD_SAFE=0 ${clang_common} ${auto_compile_flags}"
clang_out="-o"

# --- Choose Compile/Link Lines -----------------------------------------------
if [ -v clang ];   then compile_debug_safe="$clang_debug_safe"; fi
if [ -v clang ];   then compile_debug_fast="$clang_debug_fast"; fi
if [ -v clang ];   then compile_release_safe="$clang_release_safe"; fi
if [ -v clang ];   then compile_release_fast="$clang_release_fast"; fi
if [ -v clang ];   then out="$clang_out"; fi

if [ -v debug ] && [ -v safe ];   then compile="$compile_debug_safe"; fi
if [ -v debug ] && [ -v fast ];   then compile="$compile_debug_fast"; fi
if [ -v release ] && [ -v safe ]; then compile="$compile_release_safe"; fi
if [ -v release ] && [ -v fast ]; then compile="$compile_release_fast"; fi

# --- Prep Directories --------------------------------------------------------
mkdir -p build

# --- Build & Run Metaprogram -------------------------------------------------
if [ -v no_meta ]; then echo "[skipping metagen]"; fi
if [ ! -v no_meta ]
then
  cd build
  $compile_debug_safe ../src/metagen/metagen_main.c $out metagen
  find ../src -type f -name '*.meta.*' -delete # Clean
  ./metagen || { echo "[ERROR] metagen failed with exit code $?"; exit 1; }
  cd ..
fi

# --- Build Everything (@build_targets) ---------------------------------------
cd build
if [ -v krypton ]; then didbuild=1 && $compile ../src/krypton/krypton_main.c     $out krypton; fi
cd ..

# --- Warn On No Builds -------------------------------------------------------
if [ ! -v didbuild ]
then
  echo "[WARNING] no valid build target specified; must use build target names as arguments to this script, like \`./build.sh krypton\` or \`./build.sh metagen\`."
  exit 1
fi

END_TIME=$(date +%s.%4N)
echo "[ctime $(awk "BEGIN {printf \"%.3f\", ${END_TIME} - ${START_TIME}}") seconds]"
