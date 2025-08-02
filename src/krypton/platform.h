// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef PLATFORM_H
#define PLATFORM_H

#include "base.h"

i32 PlatformWriteStdout(String str);

/// --- File API --- ///

typedef struct File File;

File PlatformOpenFile(String path);
void PlatformCloseFile(File file);
String PlatformReadFile(File file, void *location, i32 size);
b8 PlatformIsValidFile(File file);

#endif
