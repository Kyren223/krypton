// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include "base.h"

#define PLATFORM_LINUX_DEFS
#include "generated/platform_linux.meta.h"
#define PLATFORM_LINUX_FUNCTIONS
#include "generated/platform_linux.meta.h"

struct File 
{
  int fd;
};

#endif
