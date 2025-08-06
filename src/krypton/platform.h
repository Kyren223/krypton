// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef PLATFORM_H
#define PLATFORM_H

#include "base.h"

#if OS_LINUX
# include "platform_linux.h"
#else
# error This operating system is not supported.
#endif


/// --- Memory API --- ///

void *MemCopy(void *dest, void *src, u64 size);
void *MemSet(void *ptr, u8 value, u64 size);
i32 MemCmp(void *ptr1, void *ptr2, u64 count);

/// --- Print API --- ///

#define PRINT_ERROR -1

// Returns the number of bytes written or PRINT_ERROR if failed
i32 Print(String str);

/// --- File API --- ///

#define FILE_ERROR -1

typedef struct File File;

File PlatformOpenFile(String path);
i32 PlatformCloseFile(File file);
String PlatformReadFile(File file, void *location, i32 size);
b8 PlatformIsValidFile(File file);

#endif
