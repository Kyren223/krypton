// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "libkrypton.h"
#include "base.h"

#define KrProduceToken(type, advance) (KrProduceToken_(tokenizer, type, advance, 0))
#define KrProduceTokenOffset(type, advance, offset) (KrProduceToken_(tokenizer, type, advance, offset))

#define KrTokenizerMatch(match) (KrTokenizerMatch_(tokenizer, match))

KrToken KrTokenizerNext(KrTokenizer* tokenizer) {
  Assert(tokenizer->src.length <= MaxUint(24))

  String src = tokenizer->src;

  if (tokenizer->current >= src.length) {
    return KrProduceToken(KrTokenType_eof, 0);
  }

  char c = tokenizer->src.value[tokenizer->current];
  switch (c) {

    // NOTE(kyren): single char
    case '+': {
      return KrProduceToken(KrTokenType_plus, 1);
    }break;
    case '-': {
      return KrProduceToken(KrTokenType_minus, 1);
    }break;
    case '*': {
      return KrProduceToken(KrTokenType_star, 1);
    }break;
    case '/': {
      return KrProduceToken(KrTokenType_slash, 1);
    }break;
    case ';': {
      return KrProduceToken(KrTokenType_semicolon, 1);
    }break;

    // NOTE(kyren): one or two chars
    case '=': {
      KrTokenizerAdvance(tokenizer);
      if (KrTokenizerMatch('=')) {
        return KrProduceTokenOffset(KrTokenType_equalEqual, 0, -2);
      }
      return KrProduceTokenOffset(KrTokenType_equal, 0, -1);
    }break;

    default: {
      // TODO(kyren): produce an unknown/invalid token
      // Potentially even stop execution and just return EOF?
      return KrProduceToken(KrTokenType_char, 1);
    }break;
  }

}

String KrTokenSprint(Arena* arena, KrTokenizer* tokenizer, KrToken token) {
  switch (token.type) {

    case KrTokenType_plus: {
      return S("<plus>");
    }break;

    case KrTokenType_minus: {
      return S("<minus>");
    }break;

    case KrTokenType_star: {
      return S("<star>");
    }break;

    case KrTokenType_slash: {
      return S("<slash>");
    }break;

    case KrTokenType_semicolon: {
      return S("<semicolon>");
    }break;

    case KrTokenType_equal: {
      return S("<equal>");
    }break;

    case KrTokenType_equalEqual: {
      // TODO: do we want to also print the location so we can verify it?
      // String literal = (String){ .value = src.value + token.index, .length = 2 };
      // return Sprintf(arena, "%S", literal);
      return S("<equal_equal>");
    }break;

    case KrTokenType_char: {
      return Sprintf(arena, "'%S'", KrTokenString(tokenizer, token));
    }break;

    case KrTokenType_eof: {
      return S("<eof>");
    }break;

    default: {
      return S("<unknown>");
    }break;
  }

}

fn KrToken KrProduceToken_(KrTokenizer* tokenizer, KrTokenType type, u32 advance, i32 offset) {
  KrToken token = (KrToken){ .index = tokenizer->current+offset, .type = type };
  tokenizer->current += advance;
  return token;
}

fn char KrTokenizerAdvance(KrTokenizer* tokenizer) {
  char c = tokenizer->src.value[tokenizer->current];
  tokenizer->current += 1;
  return c;
}

fn b32 KrTokenizerMatch_(KrTokenizer* tokenizer, char match) {
  char c = tokenizer->src.value[tokenizer->current];

  if (c != match) {
    return false;
  }

  tokenizer->current += 1;

  return true;
}

fn String KrTokenString(KrTokenizer* tokenizer, KrToken token) {
  i32 length = -1;

  switch (token.type) {
    // NOTE(kyren): single char
    case KrTokenType_semicolon:
    case KrTokenType_plus:
    case KrTokenType_minus:
    case KrTokenType_star:
    case KrTokenType_slash:
    case KrTokenType_equal: {
      length = 1;
    }break;

    // NOTE(kyren): two chars
    case KrTokenType_equalEqual: {
      length = 2;
    }break;

    // NOTE(kyren): literals
    // TODO(kyren): add lengths
    case KrTokenType_identifier: {
    }break;
    case KrTokenType_number: {
    }break;
    case KrTokenType_char: {
      length = 1;
    }break;
    case KrTokenType_string: {
    }break;

    // NOTE(kyren): keywords
    // TODO(kyren): add lengths
    // Consider not hardocing and instead use a lookup table for keywords?
    case KrTokenType_const: {
    }break;
    case KrTokenType_fn: {
    }break;
    case KrTokenType_return: {
    }break;
    case KrTokenType_i32: {
    }break;

    case KrTokenType_eof: {
      return (String){0};
    }break;

    default: {
      Assert(!"Missing token type");
    }break;

  }

  Assert(length > 0 && "Missing token type");

  return (String){ .value = tokenizer->src.value + token.index, .length = length };
}

