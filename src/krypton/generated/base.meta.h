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

/// ---- Branch Predictor Hints --- //

/// ---- Metagen --- //

/// ---- Strings --- //
typedef struct  String String;

/// ---- Flags --- //

/// ---- Conversions --- //

/// ---- Math --- //

/// ---- Memory --- //

/// ---- Arena Types --- //
typedef enum  ArenaFlags ArenaFlags;
typedef struct  ArenaParams ArenaParams;
typedef struct  Arena Arena;
typedef struct  Temp Temp;

/// ---- Arena Functions --- //

#endif

#ifdef BASE_FUNCTIONS

/// --- Strings --- //
fn i32    CStrlen        (char* cstr);
fn String Sprintf        (Arena* arena, const char* fmt, ...);
fn char*  SprintfCallback(const char* buf, void* puser, int len);
fn i32    Printf         (const char* fmt, ...);
fn char*  PrintfCallback (const char* buf, void* user, int len);
fn void   Print          (String str);
fn String StrFrom        (String s, i64 from);
fn String StrFromTo      (String s, i64 from, i64 to);
fn i32    StrCmp         (String s1, String s2);
fn b32    StrEq          (String s1, String s2);
fn b32    StrEqFIC       (String s1, String s2);

/// --- Files --- //

/// --- Memory --- //
fn b32 MemIsPow2      (u64 x);
fn u64 MemAlignForward(uptr ptr, u64 alignment);

/// --- Arena --- //
fn Arena* ArenaAlloc_ (ArenaParams* params);
fn void   ArenaRelease(Arena* arena);
fn void*  ArenaPush   (Arena* arena, u64 size, u64 align, char* allocationSiteFile, int allocationSiteLine);
fn u64    ArenaPos    (Arena* arena);
fn void   ArenaPopTo  (Arena* arena, u64 pos);
fn void   AreanClear  (Arena* arena);
fn void   ArenaPop    (Arena* arena, u64 amount);
fn Temp   TempBegin   (Arena* arena);
fn void   TempEnd     (Temp temp);

#endif
