// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#pragma once

#define pub
#define external
#define fn static

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef int8_t b8;
typedef int32_t b32;
typedef int64_t b64;
typedef int32_t rune; // Unicode codepoint
typedef uintptr_t usize;
typedef intptr_t isize;

#define enum8(type) u8
#define enum16(type) u16
#define enum32(type) u32
#define enum64(type) u64

#define true 1
#define false 0

////////////////////////////////

#define S(cstr) ((String){.value = ("" cstr), .length = sizeof("" cstr) - 1})

typedef struct String {
  char *value;
  u64 length;
} String;
