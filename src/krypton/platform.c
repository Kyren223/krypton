// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "base.h"

#if OS_LINUX
# include "platform_linux.c"
#else
# error This operating system is not supported.
#endif
