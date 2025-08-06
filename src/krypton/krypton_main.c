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

int EntryPoint(int argc, char** argv) {
  Assert(argc <= 255 && "Max arguments allowed");

  String args[255];
  for (i32 i = 0; i < argc; i++) {
    args[i] = (String){ .value = argv[i], .length = CStrlen(argv[i]) };
  }

  return KryptonMain(argc, args);
}

fn i32 KryptonMain(i32 argc, String* argv) {
  Printf("%s\n", BUILD_TITLE_STRING_LITERAL);

  Print(S("\nArgs:\n"));
  for (i32 i = 0; i < argc-1; i++) {
    Printf("%s ", argv[i]);
  }
  Printf("%s\n\n", argv[argc-1]);


  Arena* arena = ArenaAlloc(.commitSize = 4096);
  Repl(arena);
  ArenaRelease(arena);

  return 0;
}

fn void Repl(Arena* arena) {
  for (;;) {
    Printf("> ");
    String line = OsReadLine(arena);

    Printf("echo '%S'\n", line);

    if (StrEqFIC(line, S("exit")) || StrEqFIC(line, S("quit"))) {
      break;
    }
  }
}
