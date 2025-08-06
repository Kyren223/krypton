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

/// ---- Metagen --- //

/// ---- Strings --- //
typedef struct  String String;

/// ---- Flags --- //

/// ---- Memory --- //

#endif
#ifdef BASE_FUNCTIONS

/// --- Strings --- //
fn i32    Printf         (const char *fmt, ...);
fn char   *PrintfCallback(const char *buf, void *user, int len);
fn String StrFrom        (String s, u64 from);
fn String StrFromTo      (String s, u64 from, u64 to);

/// --- Asserts --- //
fn void assert(b8 ok);

/// --- Files --- //

#endif