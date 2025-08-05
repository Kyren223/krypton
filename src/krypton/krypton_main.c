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

  File file = PlatformOpenFile(S("file.txt"));
  if (!PlatformIsValidFile(file)) {
    Print(S("Invalid file\n"));
    return 1;
  }

  char buf[1024];
  String contents = PlatformReadFile(file, buf, 1024);
  Printf("'%s'\n", contents.value);


  // TODO(kyren): check if closed properly
  PlatformCloseFile(file);

  return 0;
}

