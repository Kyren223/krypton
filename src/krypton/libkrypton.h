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
  KrTokenType_CharLiteral,
  KrTokenType_EOF,
};

struct KrToken 
{
  KrTokenType type;
  u32 index;
};


KrToken KrTokenizerNext(KrTokenizer* tokenizer);
String KrTokenSprint(Arena* arena, KrTokenizer tokenizer, KrToken token);

#endif
