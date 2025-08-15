// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "libkrypton.h"
#include "base.h"
#include "platform.h"

/// --- Tokenizer --- ///

#define KrProduceToken(type, advance) (KrProduceToken_(tokenizer, type, (advance), tokenizer->current))
#define KrProduceTokenLoc(type, advance, location) (KrProduceToken_(tokenizer, type, advance, location))

#define KrProduceToken1(type) KrProduceToken1_(tokenizer, type)
#define KrProduceToken2(c, two, one) KrProduceToken2_(tokenizer, c, two, one)

#define KrTokenizerMatch(match) (KrTokenizerMatch_(tokenizer, match))

KrToken KrTokenizerNext(KrTokenizer* tokenizer) {
  Assert(tokenizer->src.length <= MaxUint(24))

  if (KrIsAtEnd(tokenizer)) {
    return KrProduceToken(KrTokenType_eof, 0);
  }

  char c = tokenizer->src.value[tokenizer->current];
  while (IsWhitespace(c)) {
    FlagClear(tokenizer->flags, KrTokenizerFlags_noLiteral);
    tokenizer->current++;
    if (KrIsAtEnd(tokenizer)) {
      return KrProduceToken(KrTokenType_eof, 0);
    }
    // NOTE(kyren): only access current AFTER the IsAtEnd check
    c = tokenizer->src.value[tokenizer->current];
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
    case '.': {
      return KrProduceToken1(KrTokenType_dot);
    }break;
    case '(': {
      return KrProduceToken1(KrTokenType_lparen);
    }break;
    case ')': {
      return KrProduceToken1(KrTokenType_rparen);
    }break;
    case '{': {
      return KrProduceToken1(KrTokenType_lbrace);
    }break;
    case '}': {
      return KrProduceToken1(KrTokenType_rbrace);
    }break;

    // NOTE(kyren): one or two chars
    case '=': {
      return KrProduceToken2('=', KrTokenType_equalEqual, KrTokenType_equal);
    }break;

  }

  // NOTE(kyren): Literals + Keywords
  if (KrIsIdentifierStart(c)) {
    if (FlagExists(tokenizer->flags, KrTokenizerFlags_noLiteral)) {
      KrToken token = KrTokenizeIdentifier(tokenizer, c);
      b32 isKeyword = KrIsKeyword(token.type);
      KrTokenType type = isKeyword ? KrTokenType_illegalKeyword : KrTokenType_illegalIdentifier;
      return KrProduceTokenLoc(type, 0, token.index);
    }
    return KrTokenizeIdentifier(tokenizer, c);
  }

  if (KrIsNumberStart(c)) {
    if (FlagExists(tokenizer->flags, KrTokenizerFlags_noLiteral)) {
      // NOTE(kyren): currently this flag is only set by a number
      // so it's impossible for this to happen
      // this might change in the future
      UNREACHABLE();
    }
    return KrTokenizeNumber(tokenizer, c);
  }

  return KrProduceToken(KrTokenType_unknown, 1);
}

fn KrToken KrTokenizeIdentifier(KrTokenizer* tokenizer, char c) {
  u32 start = tokenizer->current;

  // NOTE(kyren): skip first char
  tokenizer->current++;
  while (true) {
    if (KrIsAtEnd(tokenizer)) {
      break;
    }

    c = tokenizer->src.value[tokenizer->current];
    if (!KrIsIdentifier(c)) {
      break;
    }
    tokenizer->current++;
  }

  // NOTE(kyren): keywords
  u32 length = tokenizer->current - start;
  String identifier = (String){ .value = tokenizer->src.value + start, .length = length };
  for (u32 i = 0; i < SizeofArray(kr_keyword_entries); i++) {
    KrKeywordEntry keyword = kr_keyword_entries[i];
    if (StrEq(identifier, keyword.name)) {
      return KrProduceTokenLoc(keyword.type, 0, start);
    }
  }

  return KrProduceTokenLoc(KrTokenType_identifier, 0, start);
}

KrToken KrTokenizerPeek(KrTokenizer tokenizer) {
  // NOTE(kyren): pass by value so it doesn't modify the actual tokenizer
  // this allows for just peeking without advancing
  return KrTokenizerNext(&tokenizer);
}

fn KrToken KrTokenizeNumber(KrTokenizer* tokenizer, char c) {
  // TODO(kyren): add support for floating point numbers

  // TODO: parse number
  u32 start = tokenizer->current;

  // NOTE(kyren): get base
  i8 base = -1;
  if (c != '0') {
    base = 10;
  } else {

    tokenizer->current += 1;
    if (KrIsAtEnd(tokenizer)) {
      // NOTE(kyren): produce just "0" as a number, and now it's EOF
      return KrProduceTokenLoc(KrTokenType_number, 0, start);
    }
    c = tokenizer->src.value[tokenizer->current];

    switch (c) {
      case 'b': {
        base = 2;
      }break;

      case 'o': {
        base = 8;
      }break;

      case 'x': {
        base = 16;
      }break;
    }

    // NOTE(kyren): if no specifier, it's base 10
    if (base == -1) {
      base = 10;
    } else {
      tokenizer->current += 1;
      if (KrIsAtEnd(tokenizer)) {
        return KrProduceTokenLoc(KrTokenType_illegalNumber, 0, start);
      }
      c = tokenizer->src.value[tokenizer->current];
    }


    // TODO(kyren): handle `0.` case

    if (!KrIsNumber(c, base)) {
      FlagSet(tokenizer->flags, KrTokenizerFlags_noLiteral);
      return KrProduceTokenLoc(KrTokenType_number, 0, start);
    }
  }

  while (true) {
    if (KrIsAtEnd(tokenizer)) {
      break;
    }

    c = tokenizer->src.value[tokenizer->current];
    if (!KrIsNumber(c, base)) {
      break;
    }
    tokenizer->current++;
  }

  return KrProduceTokenLoc(KrTokenType_number, 0, start);
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

    case KrTokenType_dot: {
      return S("<dot>");
    }break;

    case KrTokenType_lparen: {
      return S("<lparen>");
    }break;

    case KrTokenType_rparen: {
      return S("<rparen>");
    }break;

    case KrTokenType_lbrace: {
      return S("<lbrace>");
    }break;

    case KrTokenType_rbrace: {
      return S("<rbrace>");
    }break;

    case KrTokenType_equal: {
      return S("<equal>");
    }break;

    case KrTokenType_equalEqual: {
      return S("<equal_equal>");
    }break;

    case KrTokenType_char: {
      return S("<char>");
    }break;

    case KrTokenType_unknown: {
      return S("<unknown>");
    }break;

    case KrTokenType_eof: {
      return S("<eof>");
    }break;

    case KrTokenType_illegalIdentifier: {
      return S("<illegal-ident>");
    }break;
    case KrTokenType_identifier: {
      return S("<ident>");
    }break;
    case KrTokenType_illegalNumber: {
      return S("<illegal-number>");
    }break;
    case KrTokenType_number: {
      return S("<number>");
    }break;
    case KrTokenType_string: {
      return S("<string>");
    }break;

    case KrTokenType_illegalKeyword: {
      return S("<illegal-keyword>");
    }break;
    case KrTokenType_const: {
      return S("<const>");
    }break;
    case KrTokenType_var: {
      return S("<var>");
    }break;
    case KrTokenType_pub: {
      return S("<pub>");
    }break;
    case KrTokenType_fn: {
      return S("<fn>");
    }break;
    case KrTokenType_return: {
      return S("<return>");
    }break;
    case KrTokenType_i32: {
      return S("<i32>");
    }break;
  }

  Assert(!"Missing token type");
}

fn KrToken KrProduceToken_(KrTokenizer* tokenizer, KrTokenType type, u32 advance, u32 location) {
  KrToken token = (KrToken){ .index = location, .type = type };
  tokenizer->current += advance;
  return token;
}

fn KrToken KrProduceToken1_(KrTokenizer* tokenizer, KrTokenType type) {
  // NOTE(kyren): single char are always not a literal
  FlagClear(tokenizer->flags, KrTokenizerFlags_noLiteral);

  return KrProduceToken(type, 1);
}

fn KrToken KrProduceToken2_(KrTokenizer* tokenizer, char c, KrTokenType two, KrTokenType one) {
  // NOTE(kyren): two chars are always not a literal
  FlagClear(tokenizer->flags, KrTokenizerFlags_noLiteral);

  u32 start = tokenizer->current;
  tokenizer->current += 1;
  return KrProduceTokenLoc(KrTokenizerMatch(c) ? two : one, 0, start);
}

fn b32 KrTokenizerMatch_(KrTokenizer* tokenizer, char match) {
  if (KrIsAtEnd(tokenizer)) {
    return false;
  }

  // NOTE(kyren): only access current AFTER the IsAtEnd check
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
    case KrTokenType_dot:
    case KrTokenType_plus:
    case KrTokenType_minus:
    case KrTokenType_star:
    case KrTokenType_slash:
    case KrTokenType_lparen:
    case KrTokenType_rparen:
    case KrTokenType_lbrace:
    case KrTokenType_rbrace:
    case KrTokenType_equal: {
      length = 1;
    }break;

    // NOTE(kyren): two chars
    case KrTokenType_equalEqual: {
      length = 2;
    }break;

    // NOTE(kyren): we treat this like an identifier, bcz we can't
    // index directly into the keyword table
    case KrTokenType_illegalKeyword:
    case KrTokenType_illegalIdentifier:
    // NOTE(kyren): literals
    case KrTokenType_identifier: {
      length = 0;
      u32 index = token.index;
      while (index < tokenizer->src.length) {
        char c = tokenizer->src.value[index++];
        if (!KrIsIdentifier(c)) {
          break;
        }
        length++;
      }
    }break;
    case KrTokenType_illegalNumber: {
      // TODO/NOTE(kyren): currently illegal numbers can
      // occur only due to EOF, so this is fine
      length = tokenizer->src.length - token.index;
    }break;
    case KrTokenType_number: {
      u32 index = token.index;

      i8 base = 10;
      b8 isZero = tokenizer->src.value[index] == '0';

      while (index < tokenizer->src.length) {
        char c = tokenizer->src.value[index];

        if (isZero && index-token.index == 1) {
          switch (c) {
            case 'b': {
              base = 2;
            }break;
            case 'o': {
              base = 8;
            }break;
            case 'x': {
              base = 16;
            }break;
            default: {
              if (!KrIsNumber(c, 10)) {
                base = -1;
              }
            }break;
          }
          if (base == -1) break;
          index++;
          continue;
        }

        if (!KrIsNumber(c, base)) {
          break;
        }
        index++;
      }
      length = index - token.index;

    }break;
    case KrTokenType_char: {
      // TODO(kyren): add lengths (this is at least 3, for 'a' but might be more for '\xFF')
      length = 1;
    }break;
    case KrTokenType_string: {
      // TODO(kyren): add lengths
    }break;

    // NOTE(kyren): keywords
    case KrTokenType_const:
    case KrTokenType_var:
    case KrTokenType_pub:
    case KrTokenType_fn:
    case KrTokenType_return:
    case KrTokenType_i32: {
      u32 index = token.type - kr_first_keyword;
      KrKeywordEntry keyword = kr_keyword_entries[index];
      length = keyword.name.length;
    }break;

    case KrTokenType_unknown: {
      length = 1;
    }break;

    case KrTokenType_eof: {
      return S("EOF");
      // TODO: return null string?
      // return (String){0};
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
    Printf("'%S'%c", KrTokenString(tokenizer, token), sep);
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

fn b32 KrIsNumberStart(char c) {
  return IsNumeric(c);
}

fn b32 KrIsNumber(char c, u8 base) {
  return c == '_' || IsNumericBase(c, base);
}

fn b32 KrIsAtEnd(KrTokenizer* tokenizer) {
  return tokenizer->current >= tokenizer->src.length;
}

b32 KrIsKeyword(KrTokenType type) {
  return kr_first_keyword <= type && type < kr_end_keyword;
}

fn b32 KrIsLiteral(KrTokenType type) {
  // TODO(kyren): complete this with all literals
  return type == KrTokenType_number || type == KrTokenType_identifier;
}

fn b32 KrIsOperator(KrTokenType type) {
  // TODO(kyren): complete this with all operators
  return type == KrTokenType_plus ||
         type == KrTokenType_minus ||
         type == KrTokenType_star ||
         type == KrTokenType_slash;
}

/// --- Parser --- ///

KrNode* KrParse(KrParser* parser) {
  // NOTE(kyren): returns zeroed memory, so 0 children by default
  KrNode* node = PushSingle(parser->arena, KrNode);
  parser->base = (u64)node;

  return KrParseTopLevel(parser, node);
}

fn KrNode* KrParseTopLevel(KrParser* parser, KrNode* node) {
  KrToken startToken = KrTokenizerNext(&parser->tokenizer);

  node->type = KrNodeType_topLevelDecl;
  node->token = startToken;
  KrNode* identifier = PushArray(parser->arena, KrNode, 2);
  KrNode* expr = identifier + 1;
  FlagSet(expr->data, KrNodeFlags_lastChild);
  node->children = KrParserChildIndex(parser, identifier);

  if (startToken.type == KrTokenType_pub) {
    FlagSet(node->data, KrDataDecl_pub);
    startToken = KrTokenizerNext(&parser->tokenizer);
  }

  if (startToken.type == KrTokenType_const) {
    FlagSet(node->data, KrDataDecl_const);
  } else if (startToken.type != KrTokenType_var) {
    // TODO(kyren): handle parsing errors
    UNREACHABLE();
  }

  // TODO(kyren): set const flags

  KrToken ident = KrTokenizerNext(&parser->tokenizer);
  if (ident.type != KrTokenType_identifier) {
    // TODO(kyren): handle parsing errors
    UNREACHABLE();
  }
  identifier->type = KrNodeType_identifier;
  identifier->token = ident;

  // TODO(kyren); parse type here (for now just skip to =)

  KrToken eq = KrTokenizerNext(&parser->tokenizer);
  if (eq.type != KrTokenType_equal) {
    // TODO(kyren): handle parsing errors
    UNREACHABLE();
  }

  KrParseExpr(parser, expr, 0);

  KrToken semicolon = KrTokenizerNext(&parser->tokenizer);
  if (semicolon.type != KrTokenType_semicolon) {
    // TODO(kyren): handle parsing errors
    UNREACHABLE();
  }

  return node;
}

fn KrNode* KrParseExpr(KrParser* parser, KrNode* node, u8 minPrecedence) {
  KrToken lhsToken = KrTokenizerNext(&parser->tokenizer);
  node->type = KrNodeType_literal;
  node->token = lhsToken;

  if (!KrIsLiteral(lhsToken.type)) {
    // TODO(kyren): handle parsing errors
    UNREACHABLE();
  }

  while (true) {
    KrToken opToken = KrTokenizerPeek(parser->tokenizer);

    // TODO(kyren): should I assume that any non-operator means the expression ended
    // as if it were an EOF?
    // or should I just special case the valid "end" tokens such as a semicolon
    // in which case, this won't be self-contained
    if (opToken.type == KrTokenType_eof || opToken.type == KrTokenType_semicolon) {
      break;
    }

    if (!KrIsOperator(opToken.type)) {
      // TODO(kyren): handle parsing errors
      UNREACHABLE();
    }

    KrPrecedence precedence = KrInfixPrecendence(opToken.type);
    if (precedence.left < minPrecedence) {
      break;
    }

    KrTokenizerNext(&parser->tokenizer);

    // NOTE(kyren): preallocate lhs and rhs nodes
    KrNode* lhs = PushArray(parser->arena, KrNode, 2);
    MemCopy(lhs, node, sizeof(KrNode));

    KrNode* rhs = lhs + 1;
    KrParseExpr(parser, rhs, precedence.right);
    FlagSet(rhs->data, KrNodeFlags_lastChild);

    node->type = KrNodeType_binaryOp;
    node->token = opToken;
    node->children = KrParserChildIndex(parser, lhs);
  }

  return node;
}

void KrParserPrettyPrint(KrParser* parser, KrNode* node, u32 indent) {
  const u32 spaces = 2;

  switch (node->type) {
    case KrNodeType_literal: {
      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      Printf("'%S'\n", KrTokenString(&parser->tokenizer, node->token));
    }break;

    case KrNodeType_binaryOp: {
      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      Printf("(binaryOp '%S'\n", KrTokenString(&parser->tokenizer, node->token));

      KrNode* lhs = KrParserGetChild(parser, node, 0);
      KrNode* rhs = KrParserGetChild(parser, node, 1);
      KrParserPrettyPrint(parser, lhs, indent+spaces);
      KrParserPrettyPrint(parser, rhs, indent+spaces);

      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      Printf(")\n");
    }break;

    case KrNodeType_identifier: {
      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      Printf("(identifier '%S')\n", KrTokenString(&parser->tokenizer, node->token));
    }break;

    case KrNodeType_topLevelDecl: {
      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      String isPub = FlagExists(node->data, KrDataDecl_pub) ? S("pub") : S("");
      String isConst = FlagExists(node->data, KrDataDecl_const) ? S("const") : S("");
      Printf("(topLevelDecl %S %S\n", isPub, isConst);

      KrNode* expr = KrParserGetChild(parser, node, 1);
      KrNode* identifier = KrParserGetChild(parser, node, 0);
      KrParserPrettyPrint(parser, identifier, indent+spaces);
      KrParserPrettyPrint(parser, expr, indent+spaces);

      for (u32 i = 0; i < indent; i++) {
        Print(S(" "));
      }
      Printf(")\n");
   }break;

  }

}

fn KrPrecedence KrInfixPrecendence(KrTokenType type) {
  Assert(KrIsOperator(type));

  switch (type) {
    case KrTokenType_minus:
    case KrTokenType_plus: {
      return (KrPrecedence){ .left = 3, .right = 4 };
    }break;

    case KrTokenType_star:
    case KrTokenType_slash: {
      return (KrPrecedence){ .left = 7, .right = 8 };
    }break;
  }

  Assert(!"Missing operator precedence");
}

fn KrNode* KrParserGetChild(KrParser* parser, KrNode* parent, u16 index) {
  return (KrNode*)(parser->base + parent->children + (index * sizeof(KrNode)));
}

fn u32 KrParserChildIndex(KrParser* parser, KrNode* child) {
  return (u64)child - parser->base;
}
