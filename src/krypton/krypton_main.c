// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Includes --- ///

/// --- Headers --- ///
#include "platform.h"
#include "base.h"
#include "krypton_main.h"

/// --- C/C++ Files --- ///
#include "base.c"
#include "platform.c"

/// --- Entry Point --- ///

int EntryPoint(int argc, char **argv) {
  Assert(argc <= 255 && "Max arguments allowed");

  String args[255];
  for (i32 i = 0; i < argc; i++) {
    args[i] = (String){ .value = argv[i], .length = CStrlen(argv[i]) };
  }

  return KryptonMain(argc, args);
}

fn i32 KryptonMain(i32 argc, String *argv) {
  Printf("%s\n", BUILD_TITLE_STRING_LITERAL);

  Print(S("\nArgs:\n"));
  for (i32 i = 0; i < argc; i++) {
    Printf("%s\n", argv[i]);
  }

  Print(S("\nFile contents:\n"));

  Arena *arena = ArenaAlloc(.commitSize = 4096);

  String contents = ReadFile(arena, S("file.txt"));
  if (contents.value == null) {
    Print(S("Unable to read file\n"));
    return 1;
  }

  // NOTE(kyren): -1 to remove \n at the end of the file
  Printf("'%S'\n", StrFromTo(contents, 0, -1));

  Printf("Arena Commited Size: %d\n", arena->commit);

  String line = OsReadLine(arena);
  Printf("echo '%S'\n", line);

  ArenaRelease(arena);

  return 0;
}

