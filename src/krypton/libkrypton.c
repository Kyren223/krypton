// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "libkrypton.h"

KrToken KrTokenizerNext(KrTokenizer* tokenizer) {
  if (tokenizer->current >= tokenizer->src.length) {
    return KrTokenOf(tokenizer, KrTokenType_EOF);
  }

  KrToken token = KrTokenOf(tokenizer, KrTokenType_CharLiteral);
  tokenizer->current++;

  return token;
}

String KrTokenSprint(Arena* arena, KrTokenizer tokenizer, KrToken token) {
  String src = tokenizer.src;

  String literal = {0};
  String stype = {0};

  switch (token.type) {
    case KrTokenType_EOF: {
      return S("EOF");
    } break;

    case KrTokenType_CharLiteral: {
      // TODO(kyren): wrap in quotes?
      literal = (String){ .value = src.value + token.index, .length = 1 };
      stype = S("CharLiteral");
    } break;

    default: {
      return (String){0};
    } break;
  }

  return Sprintf(arena, "%S(%S)", stype, literal);
}

fn KrToken KrTokenOf(KrTokenizer* tokenizer, KrTokenType type) {
  return (KrToken){ .index = tokenizer->current, .type = type };
}
