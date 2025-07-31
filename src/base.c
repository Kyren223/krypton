#pragma once
#define S(cstr) ((String){.value = ("" cstr), .length = sizeof("" cstr) - 1})

typedef int u64;

typedef struct String {
  char *value;
  u64 length;
} String;
