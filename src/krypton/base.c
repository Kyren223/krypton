// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "base.h"
#include "platform.h"

/// --- Strings --- ///

#define STB_SPRINTF_NOUNALIGNED
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

fn i32 Printf(const char *fmt, ...) {
  char buf[STB_SPRINTF_MIN];

  int result;
  va_list va;
  va_start(va, fmt);
  result = stbsp_vsprintfcb(PrintfCallback, 0, buf, fmt, va);
  va_end(va);

  return result;
}

fn char *PrintfCallback(const char *buf, void *user, int len) {
  i32 result = Print((String){ .value = (char*)buf, .length = len });
  if (result == PRINT_ERROR) {
    return 0; // stop
  }
  return (char*)buf;
}

fn String StrFrom(String s, u64 from) {
  if (from < 0) {
    from = s.length - from;
  }
  if (BUILD_DEBUG) {
    // TODO(kyren): check that s.value is 0?
    assert(0 <= from && from < s.length); // Bounds check
  }
  return (String){
    .value = s.value + from,
    .length = s.length,
  };
}

fn String StrFromTo(String s, u64 from, u64 to) {
  if (from < 0) {
    from = s.length - from;
  }
  if (to < 0) {
    to = s.length - to;
  }
  if (BUILD_DEBUG) {
    // TODO(kyren): check that s.value is 0?
    assert(0 <= from && from < s.length); // Bounds check
    assert(0 <= to && to < s.length);     // Bounds check
  }
  return (String){
    .value = s.value + from,
    .length = to - from,
  };
}

/// --- Asserts --- ///

fn void assert(b8 ok) {
  if (!ok) {
    UNREACHABLE();
  }
}
