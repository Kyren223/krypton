// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Includes --- ///

/// --- Headers --- ///
#include "base.h"
#include "platform.h"
#include "krypton_main.h"

/// --- C/C++ Files --- ///
#include "base.c"
#include "platform.c"

/// --- Entry Point --- ///

int main(int argc, char **argv) {
  // assert(argc);
  return KryptonMain(argc, null);
}

fn i32 KryptonMain(i32 argc, String *argv) {
  Printf("Welcome to %s - made by %s%d\n", "Krypton", "Kyren", 223);

  char buf[1024];
  String contents = ReadFile(buf, S("file.txt"), sizeof(buf));
  if (contents.value == null) {
    Print(S("Unable to read file\n"));
    return 1;
  }

  Printf("'%s'\n", contents.value);

  return 0;
}

