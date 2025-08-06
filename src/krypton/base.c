// Copyright (c) Kyren223
// Licensed under the MIT license (https://opensource.org/license/mit/)

#include "base.h"
#include "platform.h"

/// --- Strings --- ///

fn i32 CStrlen(char *cstr) {
  Assert(cstr != null);

  i32 len = 0;
  while (cstr[len] != '\0') {
    len++;
  }

  return len;
}

#define STB_SPRINTF_NOUNALIGNED
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

fn i32 Printf(const char *fmt, ...) {
  char buf[STB_SPRINTF_MIN];

  int result;
  va_list va;
  va_start(va, fmt);
  result = stbsp_vsprintfcb(PrintfCallback, 0, buf, fmt, va);
  va_end(va);

  return result;
}

fn char *PrintfCallback(const char *buf, void *user, int len) {
  b32 result = OsPrint((String){ .value = (char*)buf, .length = len });

  if (!result) {
    return 0; // stop
  }

  return (char*)buf;
}

fn void Print(String str) {
  OsPrint(str);
}

fn String StrFrom(String s, i64 from) {
  if (from < 0) {
    from = s.length + from;
  }

  Assert(0 <= from && from <= s.length); // Bounds check

  // NOTE(kyren): when from == s.length, value will be out of bounds
  // So we return an empty string instead
  if (from == s.length) {
    return S("");
  }

  return (String){
    .value = s.value + from,
    .length = s.length - from,
  };
}

fn String StrFromTo(String s, i64 from, i64 to) {
  if (from < 0) {
    from = s.length + from;
  }
  if (to < 0) {
    to = s.length + to;
  }

  Assert(0 <= from && from <= s.length); // Bounds check
  Assert(from <= to);
  Assert(0 <= to && to <= s.length);     // Bounds check

  // NOTE(kyren): when from == to, value will be out of bounds
  // So we return an empty string instead
  if (from == to) {
    return S("");
  }

  return (String){
    .value = s.value + from,
    .length = to - from,
  };
}

/// --- Files --- ///

String ReadFile(Arena *arena, String path) {
  // Buf is only used when length > 0 for the returned string

  File file = OsOpenFile(path);

  if (!OsIsValidFile(file)) {
    return (String){0};
  }

  i64 size = OsFileSize(file);
  if (size == OS_FILE_SIZE_ERROR) {
    return (String){0};
  }

  char *buf = PushArrayNoZero(arena, char, size);
  String s = OsReadFile(file, buf, size);

  b32 result = OsCloseFile(file);

  if (s.value == null) {
    return (String){0};
  }

  if (!result) {
    return (String){0};
  }

  return s;
}

/// --- Memory --- ///

fn b32 MemIsPow2(u64 x) {
	return (x & (x-1)) == 0;
}

fn u64 MemAlignForward(uptr ptr, u64 alignment) {
	uptr p, a, modulo;

  Assert(MemIsPow2(alignment));

	p = ptr;
	a = (uptr)alignment;
	// NOTE(kyren): same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// NOTE(kyren): if 'p' address is not aligned,
		// push the address to the next value which is aligned
		p += a - modulo;
	}
	return p;
}

/// --- Arena --- ///

// NOTE(kyren): original copyright (later modified by Kyren223)
// Copyright (c) Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

fn Arena *ArenaAlloc_(ArenaParams *params) {
  // NOTE(kyren): round up reserve/commit sizes
  u64 reserveSize = params->reserveSize;
  u64 commitSize = params->commitSize;

  if (params->flags & ArenaFlags_LargePages) {
    reserveSize = MemAlignPow2(reserveSize, OsSysInfo()->largePageSize);
    commitSize  = MemAlignPow2(commitSize,  OsSysInfo()->largePageSize);
  } else {
    reserveSize = MemAlignPow2(reserveSize, OsSysInfo()->pageSize);
    commitSize  = MemAlignPow2(commitSize,  OsSysInfo()->pageSize);
  }

  // NOTE(kyren): reserve/commit initial block
  void *base = params->optionalBackingBuffer;
  if (base == 0) {
    if (params->flags & ArenaFlags_LargePages) {
      base = OsReserveLarge(reserveSize);
      OsCommitLarge(base, commitSize);
    } else {
      base = OsReserve(reserveSize);
      OsCommitLarge(base, commitSize);
    }
  }

  // NOTE(kyren): panic on arena creation failure
  // TODO: test this
  if (Unlikely(base == 0)) {
    Printf("%s:%d: runtime error: fatal allocation failure\n", params->allocationSiteFile, params->allocationSiteLine);
    OsAbort(1);
  }

  // NOTE(kyren): extract arena header and fill it
  Arena *arena = (Arena*)base;
  arena->current = arena;
  arena->flags = params->flags;
  arena->commitSize = params->commitSize;
  arena->reserveSize = params->reserveSize;
  arena->basePos = 0;
  arena->pos = ARENA_HEADER_SIZE;
  arena->commit = commitSize;
  arena->reserve = reserveSize;
  arena->allocationSiteFile = params->allocationSiteFile;
  arena->allocationSiteLine = params->allocationSiteLine;
#if ARENA_FREE_LIST
  arena->freeSize = 0;
  arena->freeLast = 0;
#endif
  AsanPoisonMemoryRegion(base, commitSize);
  AsanUnpoisonMemoryRegion(base, ARENA_HEADER_SIZE);
  return arena;
}

fn void ArenaRelease(Arena *arena) {
  for (Arena *n = arena->current, *prev = 0; n != 0; n = prev) {
    prev = n->prev;
    OsRelease(n, n->reserve);
  }
}

fn void *ArenaPush(Arena *arena, u64 size, u64 align, char *allocationSiteFile, int allocationSiteLine) {
  Arena *current = arena->current;
  u64 posPre = MemAlignPow2(current->pos, align);
  u64 posPost = posPre + size;

  // NOTE(kyren): chain, if needed
  if (current->reserve < posPost && !(arena->flags & ArenaFlags_NoChain)) {
    Arena *newBlock = 0;

#if ARENA_FREE_LIST
      Arena *prevBlock;
      for (newBlock = arena->freeLast, prevBlock = 0; newBlock != 0; prevBlock = newBlock, newBlock = newBlock->prev) {
        if (newBlock->reserve >= MemAlignPow2(size, align)) {
          if (prevBlock) {
            prevBlock->prev = newBlock->prev;
          } else {
            arena->freeLast = newBlock->prev;
          }
          arena->freeSize -= newBlock->reserveSize;
          AsanUnpoisonMemoryRegion((u8*)newBlock + ARENA_HEADER_SIZE, newBlock->reserveSize - ARENA_HEADER_SIZE);
          break;
        }
      }
#endif

    if (newBlock == 0) {
      u64 reserveSize = current->reserveSize;
      u64 commitSize = current->commitSize;

      if (size + ARENA_HEADER_SIZE > reserveSize) {
        reserveSize = MemAlignPow2(size + ARENA_HEADER_SIZE, align);
        commitSize = MemAlignPow2(size + ARENA_HEADER_SIZE, align);
      }

      newBlock = ArenaAlloc(
        .reserveSize = reserveSize,
        .commitSize = commitSize,
        .flags = current->flags,
        .allocationSiteFile = allocationSiteFile,
        .allocationSiteLine = allocationSiteLine,
      );
    }

    newBlock->basePos = current->basePos + current->reserve;

    // NOTE(kyren): push
    newBlock->prev = arena->current;
    arena->current = newBlock;

    current = newBlock;
    posPre = MemAlignPow2(current->pos, align);
    posPost = posPre + size;
  }

  // NOTE(kyren): commit new pages, if needed
  if (current->commit < posPost) {
    u64 commitPostAligned = posPost + current->commitSize-1;
    commitPostAligned -= commitPostAligned % current->commitSize;
    u64 commitPostClamped = ClampTop(commitPostAligned, current->reserve); // TODO: maybe here?
    u64 commitSize = commitPostClamped - current->commit;
    u8 *commitPtr = (u8 *)current + current->commit;

    if (current->flags & ArenaFlags_LargePages) {
      OsCommitLarge(commitPtr, commitSize);
    } else {
      OsCommit(commitPtr, commitSize);
    }

    current->commit = commitPostClamped;
  }

  // NOTE(kyren): push onto current block
  void *result = 0;
  if (current->commit >= posPost) {
    result = (u8*)current + posPre;
    current->pos = posPost;
    AsanUnpoisonMemoryRegion(result, size);
  }

  // NOTE(kyren): panic on failure
  if (Unlikely(result == 0)) {
    Printf("%s:%d: runtime error: fatal allocation failure\n", current->allocationSiteFile, current->allocationSiteLine);
    Printf("First created at %s:%d\n", arena->allocationSiteFile, arena->allocationSiteLine);
    OsAbort(1);
  }

  return result;
}

fn u64 ArenaPos(Arena *arena) {
  Arena *current = arena->current;
  u64 pos = current->basePos + current->pos;
  return pos;
}

fn void ArenaPopTo(Arena *arena, u64 pos) {
  u64 bigPos = ClampBottom(ARENA_HEADER_SIZE, pos);
  Arena *current = arena->current;

#if ARENA_FREE_LIST
  for (Arena *prev = 0; current->basePos >= bigPos; current = prev) {
    prev = current->prev;
    current->pos = ARENA_HEADER_SIZE;
    arena->freeSize += current->reserveSize;

    // NOTE(kyren): push
    current->prev = arena->freeLast;
    arena->freeLast = current;

    AsanPoisonMemoryRegion((u8*)current + ARENA_HEADER_SIZE, current->reserveSize - ARENA_HEADER_SIZE);
  }
#else
  for(Arena *prev = 0; current->basePos >= bigPos; current = prev) {
    prev = current->prev;
    OsRelease(current, current->reserve);
  }
#endif

  arena->current = current;
  u64 newPos = bigPos - current->basePos;
  Assert(newPos <= current->pos);
  AsanPoisonMemoryRegion((u8*)current + newPos, (current->pos - newPos));
  current->pos = newPos;
}

fn void AreanClear(Arena *arena) {
  ArenaPopTo(arena, 0);
}

fn void ArenaPop(Arena *arena, u64 amount) {
  u64 posOld = ArenaPos(arena);
  u64 posnew = posOld;

  if (amount < posOld) {
    posnew = posOld - amount;
  }

  ArenaPopTo(arena, posnew);
}

fn Temp TempBegin(Arena *arena) {
  u64 pos = ArenaPos(arena);
  Temp temp = { arena, pos };
  return temp;
}

fn void TempEnd(Temp temp) {
  ArenaPopTo(temp.arena, temp.pos);
}

