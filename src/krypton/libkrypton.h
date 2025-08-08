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
  KrTokenType_lparen,
  KrTokenType_rparen,
  KrTokenType_lbrace,
  KrTokenType_rbrace,
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
  KrTokenType_unknown,
};

struct KrToken 
{
  u32 index : 24;
  KrTokenType type : 8;
};

StaticAssert(sizeof(KrToken) == 4, expected_4_byte_packed_struct);

struct KrKeywordEntry 
{
  String name;
  KrTokenType type;
};

// NOTE(kyren): this is subtracted from a keyword to get
// it's index in the kr_keyword_entries table
// This means that all keywords must come in the same order
// as their enum counterpart and be continuous
global u32 kr_first_keyword = KrTokenType_const;

global KrKeywordEntry kr_keyword_entries[] = {
  {S("const"), KrTokenType_const},
  {S("fn"), KrTokenType_fn},
  {S("return"), KrTokenType_return},
  {S("i32"), KrTokenType_i32},
};

KrToken KrTokenizerNext(KrTokenizer* tokenizer);
String KrTokenSprint(Arena* arena, KrTokenizer* tokenizer, KrToken token);
void KrTokenizerPrettyPrint(Arena* arena, KrTokenizer* tokenizer, char sep, char end);
void KrTokenizerPrint(KrTokenizer* tokenizer, char sep, char end);

#endif
