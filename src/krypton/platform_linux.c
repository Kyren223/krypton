// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Linux entry point --- ///

#include "base.h"
#include "krypton_main.h"
#include <string.h>

int main() {
  return KryptonMain();
}

/// --- Includes --- ///

#include "platform_linux.h"

#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

/// --- Platform API --- ///

i32 PlatformWriteStdout(String str) {
  return write(1, str.value, str.length);
}

/// --- Memory API --- ///

void *MemCopy(void *dest, void *src, u64 size) {
  return memcpy(dest, src, size);
}

void *MemSet(void *ptr, u8 value, u64 size) {
  return memset(ptr, value, size);
}

i32 MemCmp(void *ptr1, void *ptr2, u64 count) {
  return memcmp(ptr1, ptr2, count);
}

/// --- File API --- ///

b8 PlatformIsValidFile(File file) {
  return file.fd >= 0;
}

File PlatformOpenFile(String path) {
  int fd = openat(AT_FDCWD, path.value, O_RDONLY);
  return (File){ .fd = fd };
}

void PlatformCloseFile(File file) {
  // TODO(kyren): Add a return value to check for failure
  close(file.fd);
}

String PlatformReadFile(File file, void *location, i32 size) {
  u64 n = read(file.fd, location, size);

  if (n == -1) {
    // TODO(kyren): handle errors
    UNREACHABLE();
  }

  if (n == 0) {
    return (String){0};
  }

  return (String){ .value = location, .length = n };
}

