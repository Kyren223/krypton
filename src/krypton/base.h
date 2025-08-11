// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef BASE_H
#define BASE_H

/// --- Clang OS/Arch --- ///

#if defined(__clang__)

# define COMPILER_CLANG 1

# if defined(_WIN32)
#  define OS_WINDOWS 1
# elif defined(__gnu_linux__) || defined(__linux__)
#  define OS_LINUX 1
# define _GNU_SOURCE
# elif defined(__APPLE__) && defined(__MACH__)
#  define OS_MAC 1
# else
#  error This compiler/OS combo is not supported.
# endif

# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# else
#  error Architecture not supported.
# endif

/// --- MSVC OS/Arch--- ///

#elif defined(_MSC_VER)

# define COMPILER_MSVC 1

# if _MSC_VER >= 1920
#  define COMPILER_MSVC_YEAR 2019
# elif _MSC_VER >= 1910
#  define COMPILER_MSVC_YEAR 2017
# elif _MSC_VER >= 1900
#  define COMPILER_MSVC_YEAR 2015
# elif _MSC_VER >= 1800
#  define COMPILER_MSVC_YEAR 2013
# elif _MSC_VER >= 1700
#  define COMPILER_MSVC_YEAR 2012
# elif _MSC_VER >= 1600
#  define COMPILER_MSVC_YEAR 2010
# elif _MSC_VER >= 1500
#  define COMPILER_MSVC_YEAR 2008
# elif _MSC_VER >= 1400
#  define COMPILER_MSVC_YEAR 2005
# else
#  define COMPILER_MSVC_YEAR 0
# endif

# if defined(_WIN32)
#  define OS_WINDOWS 1
# else
#  error This compiler/OS combo is not supported.
# endif

# if defined(_M_AMD64)
#  define ARCH_X64 1
# elif defined(_M_IX86)
#  define ARCH_X86 1
# elif defined(_M_ARM64)
#  define ARCH_ARM64 1
# elif defined(_M_ARM)
#  define ARCH_ARM32 1
# else
#  error Architecture not supported.
# endif

/// --- GCC OS/Arch --- ///

#elif defined(__GNUC__) || defined(__GNUG__)

# define COMPILER_GCC 1

# if defined(__gnu_linux__) || defined(__linux__)
#  define OS_LINUX 1
# define _GNU_SOURCE
# else
#  error This compiler/OS combo is not supported.
# endif

# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# else
#  error Architecture not supported.
# endif

#else
# error Compiler not supported.
#endif

/// --- Arch--- ///

#if defined(ARCH_X64)
# define ARCH_64BIT 1
#elif defined(ARCH_X86)
# define ARCH_32BIT 1
#endif

#if ARCH_ARM32 || ARCH_ARM64 || ARCH_X64 || ARCH_X86
# define ARCH_LITTLE_ENDIAN 1
#else
# error Endianness of this architecture is not supported.
#endif

/// --- Language--- ///

#if defined(__cplusplus)
# define LANG_CPP 1
#else
# define LANG_C 1
#endif

/// --- Build Options --- ///

#if !defined(BUILD_DEBUG)
# define BUILD_DEBUG 1
#endif

#if !defined(BUILD_SAFE)
# define BUILD_SAFE 1
#endif

#if !defined(BUILD_VERSION_MAJOR)
# define BUILD_VERSION_MAJOR 0
#endif

#if !defined(BUILD_VERSION_MINOR)
# define BUILD_VERSION_MINOR 0
#endif

#if !defined(BUILD_VERSION_PATCH)
# define BUILD_VERSION_PATCH 1
#endif

#define StringifyHelper(x) #x
#define Stringify(x) StringifyHelper(x)

#define BUILD_VERSION_STRING_LITERAL Stringify(BUILD_VERSION_MAJOR) "." Stringify(BUILD_VERSION_MINOR) "." Stringify(BUILD_VERSION_PATCH)
#if BUILD_DEBUG && BUILD_SAFE
# define BUILD_MODE_STRING_LITERAL_APPEND " [Debug Safe]"
#elif BUILD_DEBUG
# define BUILD_MODE_STRING_LITERAL_APPEND " [Debug]"
#elif BUILD_SAFE
# define BUILD_MODE_STRING_LITERAL_APPEND " [Safe]"
#else
# define BUILD_MODE_STRING_LITERAL_APPEND ""
#endif
#if defined(BUILD_GIT_HASH)
# define BUILD_GIT_HASH_STRING_LITERAL_APPEND " [" BUILD_GIT_HASH "]"
#else
# define BUILD_GIT_HASH_STRING_LITERAL_APPEND ""
#endif

#if !defined(BUILD_TITLE)
# define BUILD_TITLE "Krypton"
#endif

#if !defined(BUILD_RELEASE_PHASE_STRING_LITERAL)
# define BUILD_RELEASE_PHASE_STRING_LITERAL "ALPHA"
#endif

#if !defined(BUILD_ISSUES_LINK_STRING_LITERAL)
# define BUILD_ISSUES_LINK_STRING_LITERAL "https://github.com/kyren223/krypton/issues"
#endif

#define BUILD_TITLE_STRING_LITERAL BUILD_TITLE " (" BUILD_VERSION_STRING_LITERAL " " BUILD_RELEASE_PHASE_STRING_LITERAL ") - " __DATE__ "" BUILD_GIT_HASH_STRING_LITERAL_APPEND BUILD_MODE_STRING_LITERAL_APPEND

/// --- Zero All Undefined Options --- ///

#if !defined(ARCH_32BIT)
# define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
# define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
# define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
# define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
# define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
# define ARCH_ARM32 0
#endif
#if !defined(COMPILER_MSVC)
# define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif
#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif
#if !defined(OS_MAC)
# define OS_MAC 0
#endif
#if !defined(LANG_CPP)
# define LANG_CPP 0
#endif
#if !defined(LANG_C)
# define LANG_C 0
#endif

/// --- Unsupported Errors --- ///

#if ARCH_X86
# error You tried to build in x86 (32 bit) mode, but currently, only building in x64 (64 bit) mode is supported.
#endif
#if !ARCH_X64
# error You tried to build with an unsupported architecture. Currently, only building in x64 mode is supported.
#endif

/// --- Asserts --- ///

#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
# define UNREACHABLE() __builtin_unreachable()
# elif defined(COMPILER_MSVC)
# define UNREACHABLE() __assume(0)
#else
# error You tried to build with an unsupported compiler. Currently, only clang, gcc and msvc are supported.
#endif

#define StaticAssert(expr, msg) typedef char static_assert_##msg[(expr) ? 1 : -1]

#ifdef BUILD_SAFE
# define Assert(expr) if (!(expr)) { (*(volatile int*)0); UNREACHABLE(); }
#else
# define Assert(expr) if (!(expr)) { UNREACHABLE(); }
#endif

#if COMPILER_MSVC
# if defined(__SANITIZE_ADDRESS__)
#  define ASAN_ENABLED 1
#  define NO_ASAN __declspec(no_sanitize_address)
# else
#  define NO_ASAN
# endif
#elif COMPILER_CLANG
# if defined(__has_feature)
#  if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#   define ASAN_ENABLED 1
#  endif
# endif
# define NO_ASAN __attribute__((no_sanitize("address")))
#else
# define NO_ASAN
#endif

#if BUILD_SAFE && ASAN_ENABLED
C_LINKAGE void __asan_poison_memory_region(void const volatile* addr, size_t size);
C_LINKAGE void __asan_unpoison_memory_region(void const volatile* addr, size_t size);
# define AsanPoisonMemoryRegion(addr, size)   __asan_poison_memory_region((addr), (size))
# define AsanUnpoisonMemoryRegion(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
# define AsanPoisonMemoryRegion(addr, size)   ((void)(addr), (void)(size))
# define AsanUnpoisonMemoryRegion(addr, size) ((void)(addr), (void)(size))
#endif

/// --- Types --- ///

#include <stdint.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef int8_t    b8;
typedef int32_t   b32;
typedef int64_t   b64;
typedef int32_t   rune; // Unicode codepoint
typedef uintptr_t uptr;
typedef intptr_t  iptr;

#define enum8(type)  u8
#define enum16(type) u16
#define enum32(type) u32
#define enum64(type) u64

#define true  1
#define false 0
#define null  0

/// --- Branch Predictor Hints --- ///

#if defined(__clang__)
# define Expect(expr, val) __builtin_expect((expr), (val))
#else
# define Expect(expr, val) (expr)
#endif

#define Likely(expr)            Expect(expr,1)
#define Unlikely(expr)          Expect(expr,0)

/// --- Metagen --- ///

#define fn static
#define global static

#define BASE_DEFS
#define BASE_FUNCTIONS
#include "generated/base.meta.h"

/// --- Strings --- ///

#define S(cstr) ((String){ .value = ("" cstr), .length = sizeof("" cstr) - 1 })
#define ToS(cstr) ((String){ .value = (cstr), .length = strlen((cstr)) })

struct String 
{
  char* value;
  u64 length;
};

/// --- Flags --- ///

#define Bit(x) (1 << (x))
#define BBit(x) ((u64)1 << (x))
#define HasBit(n, pos) ((n) & (1 << (pos)))

#define FlagSet(n, f) ((n) |= (f))
#define FlagClear(n, f) ((n) &= ~(f))
#define FlagToggle(n, f) ((n) ^= (f))
#define FlagExists(n, f) (((n) & (f)) == (f)) // Checks if all bits in 'f' are set in 'n'
#define FlagEquals(n, f) (((n) == (f))) // Checks if 'n' is exactly equal to 'f'
#define FlagIntersects(n, f) (((n) & (f)) > 0) // Checks if any bits in 'f' are set in 'n'

/// --- Conversions --- ///

#define KB(n) ((n) * 1024)
#define MB(n) (KB(n) * 1024)
#define GB(n) (MB(n) * 1024)
#define TB(n) (GB(n) * 1024)

/// --- Math --- ///

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define ClampTop(a, x) Min(a, x)
#define ClampBottom(x, b) Max(x, b)
#define Clamp(a, x, b) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

#define MaxUint(bits) (((u64)1 << (bits))-1)
#define MinUint(bits) (0)

#define MaxInt(bits) (((i64)1 << ((bits)-1))-1)
#define MinInt(bits) (-(i64)1 << ((bits)-1))

#define SizeofArray(arr) (sizeof(arr)/(sizeof((arr)[0])))

/// --- Memory --- ///

#if COMPILER_MSVC
# define AlignOf(T) __alignof(T)
#elif COMPILER_CLANG
# define AlignOf(T) __alignof(T)
#elif COMPILER_GCC
# define AlignOf(T) __alignof__(T)
#else
# error AlignOf not defined for this compiler.
#endif

#define MemZero(ptr, size) MemSet(ptr, 0, size)

#define MemAlignPow2(x,b) (((x) + (b) - 1) & (~((b) - 1)))

/// --- Arena Types --- ///

#define ARENA_HEADER_SIZE 128

enum ArenaFlags 
{
  ArenaFlags_noChain    = (1 << 0),
  ArenaFlags_largePages = (1 << 1),
};

struct ArenaParams 
{
  ArenaFlags flags;
  u64 reserveSize;
  u64 commitSize;
  void* optionalBackingBuffer;
  char* allocationSiteFile;
  int allocationSiteLine;
};

struct Arena 
{
  Arena* prev;    // previous arena in chain
  Arena* current; // current arena in chain
  ArenaFlags flags;
  u64 commitSize;
  u64 reserveSize;
  u64 basePos;
  u64 pos;
  u64 commit;
  u64 reserve;
  char* allocationSiteFile;
  int allocationSiteLine;
#if ARENA_FREE_LIST
  u64 freeSize;
  Arena* freeLast;
#endif
};

StaticAssert(sizeof(Arena) <= ARENA_HEADER_SIZE, arena_header_size_check);

struct Temp 
{
  Arena* arena;
  u64 pos;
};

global const u64 arena_default_reserve_size = MB(64);
global const u64 arena_default_commit_size  = KB(64);
global const ArenaFlags arena_default_flags = 0;

/// --- Arena Functions --- ///

#define ArenaAlloc(...) ArenaAlloc_(&(ArenaParams){ \
  .reserveSize = arena_default_reserve_size, \
  .commitSize = arena_default_commit_size, \
  .flags = arena_default_flags, \
  .allocationSiteFile = __FILE__, \
  .allocationSiteLine = __LINE__, \
  __VA_ARGS__})

#define PushArrayNoZeroAligned(a, T, c, align) (T*)ArenaPush((a), sizeof(T) * (c), (align), __FILE__, __LINE__)
#define PushArrayAligned(a, T, c, align) (T*)MemZero(PushArrayNoZeroAligned(a, T, c, align), sizeof(T) * (c))
#define PushArrayNoZero(a, T, c) PushArrayNoZeroAligned(a, T, c, Max(8, AlignOf(T)))
#define PushArray(a, T, c) PushArrayAligned(a, T, c, Max(8, AlignOf(T)))
#define PushSingleNoZero(a, T) PushArrayNoZero(a, T, 1)
#define PushSingle(a, T) PushArray(a, T, 1)

#endif
