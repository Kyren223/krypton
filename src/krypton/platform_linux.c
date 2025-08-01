// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "platform.h"
#include "krypton_main.meta.h"

#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  KryptonMain();
  return 0;
}

int PlatformWriteStdout(String str) {
  return write(1, str.value, str.length);
}
