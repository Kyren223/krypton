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

#if !defined(BUILD_SUPPLEMENTARY_UNIT)
# define BUILD_SUPPLEMENTARY_UNIT 0
#endif

#if !defined(BUILD_ENTRY_DEFINING_UNIT)
# define BUILD_ENTRY_DEFINING_UNIT 1
#endif

#if !defined(BUILD_CONSOLE_INTERFACE)
# define BUILD_CONSOLE_INTERFACE 0
#endif

#if !defined(BUILD_VERSION_MAJOR)
# define BUILD_VERSION_MAJOR 0
#endif

#if !defined(BUILD_VERSION_MINOR)
# define BUILD_VERSION_MINOR 9
#endif

#if !defined(BUILD_VERSION_PATCH)
# define BUILD_VERSION_PATCH 21
#endif

#define BUILD_VERSION_STRING_LITERAL Stringify(BUILD_VERSION_MAJOR) "." Stringify(BUILD_VERSION_MINOR) "." Stringify(BUILD_VERSION_PATCH)
#if BUILD_DEBUG
# define BUILD_MODE_STRING_LITERAL_APPEND " [Debug]"
#else
# define BUILD_MODE_STRING_LITERAL_APPEND ""
#endif
#if defined(BUILD_GIT_HASH)
# define BUILD_GIT_HASH_STRING_LITERAL_APPEND " [" BUILD_GIT_HASH "]"
#else
# define BUILD_GIT_HASH_STRING_LITERAL_APPEND ""
#endif

#if !defined(BUILD_TITLE)
# define BUILD_TITLE "Untitled"
#endif

#if !defined(BUILD_RELEASE_PHASE_STRING_LITERAL)
# define BUILD_RELEASE_PHASE_STRING_LITERAL "ALPHA"
#endif

#if !defined(BUILD_ISSUES_LINK_STRING_LITERAL)
# define BUILD_ISSUES_LINK_STRING_LITERAL "https://github.com/EpicGamesExt/raddebugger/issues"
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
# error You tried to build with an unsupported architecture. Currently, only building in x64 mode is supported.
#endif

/// --- Types --- ///

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
typedef int8_t   b8;
typedef int32_t  b32;
typedef int64_t  b64;
typedef int32_t  rune; // Unicode codepoint

#define enum8(type)  u8
#define enum16(type) u16
#define enum32(type) u32
#define enum64(type) u64

#define true  1
#define false 0
#define null  0

/// --- Metagen --- ///

#define fn static

#define BASE_DEFS
#include "generated/base.meta.h"
#define BASE_FUNCTIONS
#include "generated/base.meta.h"

/// --- Strings --- ///

#define S(cstr) ((String){ .value = ("" cstr), .length = sizeof("" cstr) - 1 })
#define ToS(cstr) ((String){ .value = (cstr), .length = strlen((cstr)) })

struct String 
{
  char *value;
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

/// --- Memory --- ///

#define MemZero(ptr, size) MemSet(ptr, 0, size)

/// --- Asserts --- ///

#define assert(expr) if (!(expr)) { UNREACHABLE(); }


#endif
