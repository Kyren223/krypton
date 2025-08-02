// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(kyren): Includes

// NOTE(kyren): [h]
#include "base.h"

// NOTE(kyren): [c]
#include "base.c"
#include "platform.c"

////////////////////////////////
// NOTE(kyren): Entry point

i32 KryptonMain() {
  PlatformWriteStdout(S("Hey\n"));
  return 0;
}

