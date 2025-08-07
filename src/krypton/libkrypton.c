// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "libkrypton.h"
#include "base.h"

#define KrProduceToken(type, advance) (KrProduceToken_(tokenizer, type, advance, 0))
#define KrProduceTokenOffset(type, advance, offset) (KrProduceToken_(tokenizer, type, advance, offset))

#define KrProduceToken1(type) KrProduceToken(type, 1)
#define KrProduceToken2(one, c, two) KrProduceToken2_(tokenizer, one, c, two)

#define KrTokenizerMatch(match) (KrTokenizerMatch_(tokenizer, match))

KrToken KrTokenizerNext(KrTokenizer* tokenizer) {
  Assert(tokenizer->src.length <= MaxUint(24))

  if (KrIsAtEnd(tokenizer)) {
    return KrProduceToken(KrTokenType_eof, 0);
  }

  char c = tokenizer->src.value[tokenizer->current];
  while (IsWhitespace(c)) {
    tokenizer->current++;
    c = tokenizer->src.value[tokenizer->current];

    if (KrIsAtEnd(tokenizer)) {
      return KrProduceToken(KrTokenType_eof, 0);
    }
  }

  switch (c) {

    // NOTE(kyren): single char
    case '+': {
      return KrProduceToken1(KrTokenType_plus);
    }break;
    case '-': {
      return KrProduceToken1(KrTokenType_minus);
    }break;
    case '*': {
      return KrProduceToken1(KrTokenType_star);
    }break;
    case '/': {
      return KrProduceToken1(KrTokenType_slash);
    }break;
    case ';': {
      return KrProduceToken1(KrTokenType_semicolon);
    }break;

    // NOTE(kyren): one or two chars
    case '=': {
      return KrProduceToken2(KrTokenType_equal, '=', KrTokenType_equalEqual);
    }break;

  }

  // NOTE(kyren): Literals
  // TODO(kyren): add keywords

  return KrProduceToken(KrTokenType_unknown, 1);
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
      return S("<equal_equal>");
    }break;

    case KrTokenType_char: {
      return Sprintf(arena, "'%S'", KrTokenString(tokenizer, token));
    }break;

    case KrTokenType_unknown: {
      return S("<unknown>");
    }break;

    case KrTokenType_eof: {
      return S("<eof>");
    }break;

    case KrTokenType_identifier: {
      // TODO(kyren): Re-calculate identifier length and display it instead
      return S("<ident>");
    }break;
    case KrTokenType_number: {
      // TODO(kyren): Re-calculate number length and display it instead
      return S("<number>");
    }break;
    case KrTokenType_string: {
      // TODO(kyren): Re-calculate string length and display it instead
      return S("<string>");
    }break;
    case KrTokenType_const: {
    }break;
    case KrTokenType_fn: {
    }break;
    case KrTokenType_return: {
    }break;
    case KrTokenType_i32:break; {
    }break;
  }

  Assert(!"Missing token type");
}

fn KrToken KrProduceToken_(KrTokenizer* tokenizer, KrTokenType type, u32 advance, i32 offset) {
  KrToken token = (KrToken){ .index = tokenizer->current+offset, .type = type };
  tokenizer->current += advance;
  return token;
}

fn KrToken KrProduceToken2_(KrTokenizer* tokenizer, KrTokenType one, char c, KrTokenType two) {
  KrTokenizerAdvance(tokenizer);
  if (KrTokenizerMatch(c)) {
    return KrProduceTokenOffset(two, 0, -2);
  }
  return KrProduceTokenOffset(one, 0, -1);
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

    case KrTokenType_unknown: {
      length = 1;
    }break;

    case KrTokenType_eof: {
      return (String){0};
    }break;

  }

  Assert(length > 0 && "Unhandled token type");

  return (String){ .value = tokenizer->src.value + token.index, .length = length };
}

void KrTokenizerPrettyPrint(Arena* arena, KrTokenizer* tokenizer, char sep, char end) {
  while (true) {
    KrToken token = KrTokenizerNext(tokenizer);
    if (token.type == KrTokenType_eof) {
      break;
    }
    Printf("%S%c", KrTokenSprint(arena, tokenizer, token), sep);
  }
  KrToken token = KrTokenizerNext(tokenizer);
  Printf("%S%c", KrTokenSprint(arena, tokenizer, token), end);
}

void KrTokenizerPrint(KrTokenizer* tokenizer, char sep, char end) {
  while (true) {
    KrToken token = KrTokenizerNext(tokenizer);
    if (token.type == KrTokenType_eof) {
      break;
    }
    Printf("%S%c", KrTokenString(tokenizer, token), sep);
  }
  KrToken token = KrTokenizerNext(tokenizer);
  Printf("%S%c", KrTokenString(tokenizer, token), end);
}

fn b32 KrIsIdentifierStart(char c) {
  return IsAlpha(c) || c == '_';
}

fn b32 KrIsIdentifier(char c) {
  return IsAlphaNumeric(c) || c == '_';
}

fn b32 KrIsAtEnd(KrTokenizer* tokenizer) {
  return tokenizer->current >= tokenizer->src.length;
}


