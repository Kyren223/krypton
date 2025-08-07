// This header is generated, DO NOT EDIT!

#ifdef LIBKRYPTON_DEFS
typedef struct  KrTokenizer KrTokenizer;
typedef enum  KrTokenType KrTokenType;
typedef struct  KrToken KrToken;

#endif

#ifdef LIBKRYPTON_FUNCTIONS
fn KrToken KrProduceToken_   (KrTokenizer* tokenizer, KrTokenType type, u32 advance, i32 offset);
fn KrToken KrProduceToken2_  (KrTokenizer* tokenizer, KrTokenType one, char c, KrTokenType two);
fn char    KrTokenizerAdvance(KrTokenizer* tokenizer);
fn b32     KrTokenizerMatch_ (KrTokenizer* tokenizer, char match);
fn String  KrTokenString     (KrTokenizer* tokenizer, KrToken token);

#endif
