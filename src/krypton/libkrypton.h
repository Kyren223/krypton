// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef LIBKRYPTON_H
#define LIBKRYPTON_H

#include "base.h"

#define LIBKRYPTON_DEFS
#define LIBKRYPTON_FUNCTIONS
#include "generated/libkrypton.meta.h"

struct KrTokenizer 
{
  String filename;
  String src;
  u32 current;
};

enum KrTokenType 
{
  // NOTE(kyren): Single char
  KrTokenType_semicolon,
  KrTokenType_plus,
  KrTokenType_minus,
  KrTokenType_star,
  KrTokenType_slash,
  // KrTokenType_,

  // NOTE(kyren): One or two chars
  KrTokenType_equal,
  KrTokenType_equalEqual,

  // NOTE(kyren): Literals
  KrTokenType_identifier,
  KrTokenType_number,
  KrTokenType_char,
  KrTokenType_string,

  // NOTE(kyren): Keywords
  KrTokenType_const,
  KrTokenType_fn,
  KrTokenType_return,
  KrTokenType_i32,
  // KrTokenType_,

  KrTokenType_eof,
};

struct KrToken 
{
  u32 index : 24;
  KrTokenType type : 8;
};

StaticAssert(sizeof(KrToken) == 4, expected_4_byte_packed_struct);

KrToken KrTokenizerNext(KrTokenizer* tokenizer);
String KrTokenSprint(Arena* arena, KrTokenizer* tokenizer, KrToken token);

#endif
