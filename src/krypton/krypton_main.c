// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(kyren): Includes

// NOTE(kyren): [h]
#include "platform.h"
#include "base.h"

// NOTE(kyren): [c]
#include "base.c"
#include "platform.c"

////////////////////////////////
// NOTE(kyren): Top-Level Execution Types

pub fn int KryptonMain() {
  // char msg[3] = { 'h', 'e', 'y' };
  if (true) {
    PlatformWriteStdout(S("Hey"));
  }
  return 0;
}

