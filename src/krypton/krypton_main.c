// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(kyren): Build Options

#define KRYPTON_VERSION_MAJOR 0
#define KRYPTON_VERSION_MINOR 1
#define KRYPTON_VERSION_PATCH 0

////////////////////////////////
// NOTE(kyren): Includes

// NOTE(kyren): [h]
#include "platform.h"
#include "base.h"

// NOTE(kyren): [c]
#include "base.c"



////////////////////////////////
// NOTE(kyren): Top-Level Execution Types

fn int KryptonMain() {
  // char msg[3] = { 'h', 'e', 'y' };
  if (true) {
    PlatformWriteStdout(S("Hey"));
  }
  return 0;
}
