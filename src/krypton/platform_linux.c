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

// NOTE(kyren): other linux headers we might need later
#include <dirent.h>
#include <dlfcn.h>
#include <features.h>
#include <linux/limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/random.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <time.h>

/// --- OS Entry --- ///

global OsSystemInfo system_info = {0};

int main(int argc, char **argv) {
  // NOTE(kyren): set up OS layer

  // NOTE(kyren): get statically-allocated system/process info
  OsSystemInfo *info = &system_info;
  info->logicalProcessorCount = (u32)get_nprocs();
  info->pageSize = (u64)getpagesize();
  info->largePageSize = MB(2);
  info->allocationGranularity  = info->pageSize;

  // NOTE(kyren): call into the "real" entry point
  EntryPoint(argc, argv);
}

/// --- OS Info --- ///

OsSystemInfo *OsSysInfo(void) {
  return &system_info;
}

/// --- Allocation API --- ///

void *OsReserve(u64 size) {
  void *result = mmap(0, size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (result == MAP_FAILED) {
    return null;
  }
  return result;
}

b32 OsCommit(void *ptr, u64 size) {
  b32 result = (mprotect(ptr, size, PROT_READ|PROT_WRITE) != -1);
  return result;
}

void OsDecommit(void *ptr, u64 size) {
  madvise(ptr, size, MADV_DONTNEED);
  mprotect(ptr, size, PROT_NONE);
}

void OsRelease(void *ptr, u64 size) {
  munmap(ptr, size);
}

void *OsReserveLarge(u64 size) {
  void *result = mmap(0, size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB, -1, 0);
  if (result == MAP_FAILED) {
    return null;
  }
  return result;
}

b32 OsCommitLarge(void *ptr, u64 size) {
  b32 result = (mprotect(ptr, size, PROT_READ|PROT_WRITE) != -1);
  return result;
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

/// --- IO API --- ///

i32 OsPrint(String str) {
  i32 result = write(STDOUT_FILENO, str.value, str.length);

  if (result == -1) {
    return false;
  }

  return true;
}

#define READLINE_BUF_SIZE 512

String OsReadLine(Arena *arena) {
  String s = {0};

  char buf[READLINE_BUF_SIZE];

  for (;;) {
    i32 bytesRead = read(STDIN_FILENO, buf, READLINE_BUF_SIZE);

    if (bytesRead == -1) {
      return (String){0};
    }

    for (i32 i = 0; i < bytesRead; i++) {
      if (buf[i] == '\n') {
        s.length += i;
        char *dest = PushArrayNoZero(arena, char, i);
        MemCopy(dest, buf, i);
        if (!s.value) {
          s.value = dest;
        }
        return s;
      }
    }

    s.length += bytesRead;
    char *dest = PushArrayNoZero(arena, char, bytesRead);
    MemCopy(dest, buf, bytesRead);
    if (!s.value) {
      s.value = dest;
    }

  }

  return s;
}

/// --- File API --- ///

b32 OsIsValidFile(File file) {
  return file.fd >= 0;
}

File OsOpenFile(String path) {
  int fd = open(path.value, O_RDONLY);
  return (File){ .fd = fd };
}

b32 OsCloseFile(File file) {
  i32 result = close(file.fd);

  if (result == -1) {
    return false;
  }

  return true;
}

i64 OsFileSize(File file) {
  struct stat st;

  if (fstat(file.fd, &st) != 0) {
    return OS_FILE_SIZE_ERROR;
  }

  return st.st_size;
}

String OsReadFile(File file, void *location, i32 size) {
  ssize_t bytesRead = read(file.fd, location, size);

  if (bytesRead == -1) {
    return (String){0};
  }

  if (bytesRead == 0) {
    return (String){ .value = "", .length = 0 };
  }

  return (String){ .value = location, .length = (u64)bytesRead };
}

/// --- Abort --- ///

void OsAbort(i32 exitCode) {
  exit(exitCode);
}
