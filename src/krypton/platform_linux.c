// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(kyren): Linux entry point

#include "krypton_main.h"

int main() {
  return KryptonMain();
}

////////////////////////////////
// NOTE(kyren): Includes

#include "platform_linux.h"

#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

////////////////////////////////
// NOTE(kyren): Platform APi

i32 PlatformWriteStdout(String str) {
  return write(1, str.value, str.length);
}
