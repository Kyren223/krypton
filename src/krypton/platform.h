// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef PLATFORM_H
#define PLATFORM_H

#include "base.h"

#if defined(OS_LINUX)
# include "platform_linux.h"
#else
# error This operating system is not supported.
#endif

/// --- OS Info --- ///

typedef struct OsSystemInfo OsSystemInfo;
struct OsSystemInfo
{
  u32 logicalProcessorCount;
  u64 pageSize;
  u64 largePageSize;
  u64 allocationGranularity;
};

OsSystemInfo* OsSysInfo(void);

/// --- Allocation API --- ///

void* OsReserve(u64 size);
b32 OsCommit(void* ptr, u64 size);
void OsDecommit(void* ptr, u64 size);
void OsRelease(void* ptr, u64 size);
void* OsReserveLarge(u64 size);
b32 OsCommitLarge(void* ptr, u64 size);

/// --- Memory API --- ///

void* MemCopy(void* dest, void* src, u64 size);
void* MemSet(void* ptr, u8 value, u64 size);
i32 MemCmp(void* ptr1, void* ptr2, u64 count);

/// --- IO API --- ///

b32 OsPrint(String str);
String OsReadLine(Arena* arena);

/// --- File API --- ///

#define OS_FILE_SIZE_ERROR -1

typedef struct File File;

File OsOpenFile(String path);
b32 OsIsValidFile(File file);
b32 OsCloseFile(File file);
String OsReadFile(File file, void* buf, i32 size);
i64 OsFileSize(File file);

/// --- Abort --- ///

void OsAbort(i32 exitCode);

#endif
