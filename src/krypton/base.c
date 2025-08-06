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

fn String StrFrom(String s, i64 from) {
  if (from < 0) {
    from = s.length + from;
  }

  if (BUILD_SAFE) {
    Assert(0 <= from && from <= s.length); // Bounds check
  }

  // NOTE(kyren): when from == s.length, value will be out of bounds
  // So we return an empty string instead
  if (from == s.length) {
    return S("");
  }

  return (String){
    .value = s.value + from,
    .length = s.length - from,
  };
}

fn String StrFromTo(String s, i64 from, i64 to) {
  if (from < 0) {
    from = s.length + from;
  }
  if (to < 0) {
    to = s.length + to;
  }

  if (BUILD_SAFE) {
    Assert(0 <= from && from <= s.length); // Bounds check
    Assert(from <= to);
    Assert(0 <= to && to <= s.length);     // Bounds check
  }

  // NOTE(kyren): when from == to, value will be out of bounds
  // So we return an empty string instead
  if (from == to) {
    return S("");
  }

  return (String){
    .value = s.value + from,
    .length = to - from,
  };
}

/// --- Files --- ///

String ReadFile(void *buf, String path, i32 size) {
  // Buf is only used when length > 0 for the returned string

  File file = PlatformOpenFile(path);

  if (!PlatformIsValidFile(file)) {
    return (String){0};
  }

  String s = PlatformReadFile(file, buf, size);

  i32 result = PlatformCloseFile(file);

  if (s.value == null) {
    return (String){0};
  }

  if (result == FILE_ERROR) {
    return (String){0};
  }

  return s;
}

