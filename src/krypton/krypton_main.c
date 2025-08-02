// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Includes --- ///

/// --- Headers --- ///
#include "base.h"
#include "platform.h"

/// --- C/C++ Files --- ///
#include "base.c"
#include "platform.c"

/// --- Entry Point --- ///

i32 KryptonMain() {
  PlatformWriteStdout(S("Hey\n"));

  File file = PlatformOpenFile(S("file.txt"));
  if (!PlatformIsValidFile(file)) {
    PlatformWriteStdout(S("Invalid file\n"));
    return 1;
  }

  // TODO(kyren): read file
  char buf[1024];
  String contents = PlatformReadFile(file, buf, 1024);
  PlatformWriteStdout(S("'"));
  PlatformWriteStdout(contents);
  PlatformWriteStdout(S("'\n"));

  // TODO(kyren): check if closed properly
  PlatformCloseFile(file);

  return 0;
}

