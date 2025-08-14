// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(kyren): TODO notes
// 
// bug fixes
//
// improvements
// [ ] support floating numbers in parsing
// [ ] should parse functions return their node or just write into it?
//
// performance
// [ ] replace 16-bit count field with a 1-bit flag for "last child" to save 15-bits
// [ ] parallelize per top-level decl in each file (single-pass to get top-level then workers)
// [ ] string interning for identifiers, use 16-bit value in AST node to store ID?

/// --- Includes --- ///

/// --- Headers --- ///
#include "platform.h"
#include "base.h"
#include "krypton_main.h"
#include "libkrypton.h"

/// --- C/C++ Files --- ///
#include "base.c"
#include "platform.c"
#include "libkrypton.c"

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
    Printf("%S ", argv[i]);
  }
  Printf("%S\n\n", argv[argc-1]);

  Arena* arena = ArenaAlloc(.commitSize = 4096);
  Repl(arena);
  ArenaRelease(arena);

  return 0;
}

fn void Repl(Arena* arena) {
  while (true) {
    Temp temp = TempBegin(arena);

    Printf("> ");
    String line = OsReadLine(temp.arena);

    if (StrEqFIC(line, S("exit")) || StrEqFIC(line, S("quit"))) {
      break;
    }

    KrTokenizer tokenizer1 = {0};
    tokenizer1.filename = S("REPL");
    tokenizer1.src = line;
    KrTokenizerPrettyPrint(temp.arena, &tokenizer1, ' ', '\n');

    KrTokenizer tokenizer2 = {0};
    tokenizer2.filename = S("REPL");
    tokenizer2.src = line;
    KrTokenizerPrint(&tokenizer2, ' ', '\n');

    KrParser parser = {0};
    parser.arena = temp.arena;
    parser.tokenizer.filename = S("REPL");
    parser.tokenizer.src = line;
    KrNode* ast = KrParse(&parser);
    KrParserPrettyPrint(&parser, ast, 0);

    TempEnd(temp);
  }
}
