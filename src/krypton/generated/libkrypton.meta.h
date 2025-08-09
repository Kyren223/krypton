// This header is generated, DO NOT EDIT!

#ifdef LIBKRYPTON_DEFS

/// ---- Tokenizer --- //
typedef enum  KrTokenizerFlags KrTokenizerFlags;
typedef struct  KrTokenizer KrTokenizer;
typedef enum  KrTokenType KrTokenType;
typedef struct  KrToken KrToken;
typedef struct  KrKeywordEntry KrKeywordEntry;

/// ---- Parser --- //
typedef enum  KrNodeType KrNodeType;
typedef struct  KrNode KrNode;
typedef struct  KrParser KrParser;

#endif

#ifdef LIBKRYPTON_FUNCTIONS

/// --- Tokenizer --- //
fn KrToken KrTokenizeIdentifier(KrTokenizer* tokenizer, char c);
fn KrToken KrTokenizeNumber    (KrTokenizer* tokenizer, char c);
fn KrToken KrProduceToken_     (KrTokenizer* tokenizer, KrTokenType type, u32 advance, u32 location);
fn KrToken KrProduceToken1_    (KrTokenizer* tokenizer, KrTokenType type);
fn KrToken KrProduceToken2_    (KrTokenizer* tokenizer, char c, KrTokenType two, KrTokenType one);
fn b32     KrTokenizerMatch_   (KrTokenizer* tokenizer, char match);
fn String  KrTokenString       (KrTokenizer* tokenizer, KrToken token);
fn b32     KrIsIdentifierStart (char c);
fn b32     KrIsIdentifier      (char c);
fn b32     KrIsNumberStart     (char c);
fn b32     KrIsNumber          (char c, u8 base);
fn b32     KrIsAtEnd           (KrTokenizer* tokenizer);

/// --- Parser --- //

#endif
