// This header is generated, DO NOT EDIT!

#ifdef LIBKRYPTON_DEFS
typedef struct  KrTokenizer KrTokenizer;
typedef enum  KrTokenType KrTokenType;
typedef struct  KrToken KrToken;

#endif

#ifdef LIBKRYPTON_FUNCTIONS
fn KrToken KrTokenizeIdentifier(KrTokenizer* tokenizer, char c);
fn KrToken KrProduceToken_     (KrTokenizer* tokenizer, KrTokenType type, u32 advance, u32 location);
fn KrToken KrProduceToken2_    (KrTokenizer* tokenizer, char c, KrTokenType two, KrTokenType one);
fn char    KrTokenizerAdvance  (KrTokenizer* tokenizer);
fn b32     KrTokenizerMatch_   (KrTokenizer* tokenizer, char match);
fn String  KrTokenString       (KrTokenizer* tokenizer, KrToken token);
fn b32     KrIsIdentifierStart (char c);
fn b32     KrIsIdentifier      (char c);
fn b32     KrIsAtEnd           (KrTokenizer* tokenizer);

#endif
