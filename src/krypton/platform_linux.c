// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

/// --- Includes --- ///

#include "platform_linux.h"
#include "base.h"

#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

/// --- Memory API --- ///

// void *PageAlloc() {
//   mmap()
// }

void *MemCopy(void *dest, void *src, u64 size) {
  return memcpy(dest, src, size);
}

void *MemSet(void *ptr, u8 value, u64 size) {
  return memset(ptr, value, size);
}

i32 MemCmp(void *ptr1, void *ptr2, u64 count) {
  return memcmp(ptr1, ptr2, count);
}

/// --- Print API --- ///

i32 Print(String str) {
  return write(STDOUT_FILENO, str.value, str.length);
}

/// --- File API --- ///

b8 PlatformIsValidFile(File file) {
  return file.fd >= 0;
}

File PlatformOpenFile(String path) {
  int fd = open(path.value, O_RDONLY);
  return (File){ .fd = fd };
}

i32 PlatformCloseFile(File file) {
  i32 result = close(file.fd);
  return result;
}

String PlatformReadFile(File file, void *location, i32 size) {
  ssize_t bytesRead = read(file.fd, location, size);

  if (bytesRead == -1) {
    return (String){0};
  }

  if (bytesRead == 0) {
    return (String){ .value = "", .length = 0 };
  }

  return (String){ .value = location, .length = (u64)bytesRead };
}

