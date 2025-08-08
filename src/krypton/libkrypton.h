// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef LIBKRYPTON_H
#define LIBKRYPTON_H

#include "base.h"

#define LIBKRYPTON_DEFS
#define LIBKRYPTON_FUNCTIONS
#include "generated/libkrypton.meta.h"

enum KrTokenizerFlags 
{
  // NOTE(kyren): indicates the current token shouldn't be
  // a literal or a keyword (char, string, number, keyword)
  KrTokenizerFlags_noLiteral = (1 << 0),

  // KrTokenizerFlags_ = (1 << 1),
};

struct KrTokenizer 
{
  String filename;
  String src;
  u32 current;
  KrTokenizerFlags flags;
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
  KrTokenType_illegalIdentifier,
  KrTokenType_illegalKeyword,
  KrTokenType_illegalNumber,
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

// NOTE(kyren): keyword range for table indexing or other things
// first is inclusive, points to the first keywords
// end is exclusive, points to the token immediately after the last keyword
global u32 kr_first_keyword = KrTokenType_const;
global u32 kr_end_keyword = KrTokenType_eof;

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
b32 KrIsKeyword(KrTokenType type);

#endif
