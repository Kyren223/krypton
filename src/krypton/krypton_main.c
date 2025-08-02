// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Includes --- ///

/// --- Headers --- ///
#include "base.h"

/// --- C/C++ Files --- ///
#include "base.c"
#include "platform.c"

/// --- Entry Point --- ///

i32 KryptonMain() {
  PlatformWriteStdout(S("Hey\n"));
  return 0;
}

