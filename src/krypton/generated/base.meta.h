// This header is generated, DO NOT EDIT!

#ifdef BASE_DEFS

/// ---- Clang OS/Arch --- //

/// ---- MSVC OS/Arch--- //

/// ---- GCC OS/Arch --- //

/// ---- Arch--- //

/// ---- Language--- //

/// ---- Build Options --- //

/// ---- Zero All Undefined Options --- //

/// ---- Unsupported Errors --- //

/// ---- Asserts --- //

/// ---- Types --- //

/// ---- Asserts --- //

/// ---- Metagen --- //

/// ---- Strings --- //
typedef struct  String String;

/// ---- Flags --- //

/// ---- Memory --- //

/// ---- Arena Types --- //
typedef enum  ArenaFlags ArenaFlags;
typedef struct  ArenaParams ArenaParams;
typedef struct  Arena Arena;
typedef struct  Tem Tem;

/// ---- Arena Functions --- //

#endif
#ifdef BASE_FUNCTIONS

/// --- Strings --- //
fn i32    Printf         (const char *fmt, ...);
fn char   *PrintfCallback(const char *buf, void *user, int len);
fn String StrFrom        (String s, i64 from);
fn String StrFromTo      (String s, i64 from, i64 to);

/// --- Files --- //

#endif