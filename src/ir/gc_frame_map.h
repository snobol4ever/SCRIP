#ifndef GC_FRAME_MAP_H
#define GC_FRAME_MAP_H
#include <stdint.h>
#include <stddef.h>
#define GC_FRAME_MAP_MAGIC 0x50414D5Au
#define GC_FRAME_MAP_ROOT 1u
#define GC_FRAME_MAP_GEN_ANCHOR 2u
#define GC_FRAME_MAP_BLOB 4u
#define GC_FRAME_MAP_LAYOUT 8u
#define GC_LAY_DESCR 0u
#define GC_LAY_RAW 1u
#define GC_LAY_PTR_GC 2u
#define GC_LAY_PTR_CODE 3u
#define GC_LAY_Q(off, kind, size) ((uint64_t)(uint32_t)(int32_t)(off) | ((uint64_t)(kind) << 32) | ((uint64_t)(size) << 40))
#define GC_LAY_OFF(q) ((int)(int32_t)(uint32_t)(q))
#define GC_LAY_KIND(q) ((unsigned)(((q) >> 32) & 0xFFu))
#define GC_LAY_SIZE(q) ((int)(((q) >> 40) & 0xFFFFu))
typedef struct gc_frame_map_t {
    uint32_t     magic;
    uint32_t     frame_bytes;
    uint32_t     header_bytes;
    uint32_t     flags;
    const char * graph_name;
    uint64_t     map_off;
} gc_frame_map_t;
#ifdef __cplusplus
static_assert(sizeof(gc_frame_map_t) == 32, "gc_frame_map_t is four sealed quads beside the code in both media (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.3): magic|frame_bytes, header_bytes|flags, graph_name, map_off -- the emitter writes them as four quads and the collector reads them through this struct, so the size is the contract; map_off is the cell's offset from the frame's region base (the value-region size), so the walker finds the region as cell - map_off in every regime (ARCH-GC section 6.2h)");
static_assert(offsetof(gc_frame_map_t, graph_name) == 16 && offsetof(gc_frame_map_t, header_bytes) == 8, "the quad order is magic|frame_bytes, header_bytes|flags, graph_name, map_off");
#else
_Static_assert(sizeof(gc_frame_map_t) == 32, "gc_frame_map_t is four sealed quads beside the code in both media (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.3): magic|frame_bytes, header_bytes|flags, graph_name, map_off -- the emitter writes them as four quads and the collector reads them through this struct, so the size is the contract; map_off is the cell's offset from the frame's region base (the value-region size), so the walker finds the region as cell - map_off in every regime (ARCH-GC section 6.2h)");
_Static_assert(offsetof(gc_frame_map_t, graph_name) == 16 && offsetof(gc_frame_map_t, header_bytes) == 8, "the quad order is magic|frame_bytes, header_bytes|flags, graph_name, map_off");
#endif
#ifdef __cplusplus
static_assert(GC_LAY_OFF(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == -72 && GC_LAY_KIND(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == GC_LAY_PTR_GC && GC_LAY_SIZE(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == 8, "a blob layout entry is ONE quad: rbp-relative offset in the low 32 bits (sign-extended on read), kind in bits 32..39, byte size in bits 40..55 -- the emitter seals n then n entries right after the map's four quads when GC_FRAME_MAP_BLOB is set, and the collector reads them through (const uint64_t *)(map + 1); no relocation in either medium, so the map's sizeof stays 32 (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2e)");
#else
_Static_assert(GC_LAY_OFF(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == -72 && GC_LAY_KIND(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == GC_LAY_PTR_GC && GC_LAY_SIZE(GC_LAY_Q(-72, GC_LAY_PTR_GC, 8)) == 8, "a blob layout entry is ONE quad: rbp-relative offset in the low 32 bits (sign-extended on read), kind in bits 32..39, byte size in bits 40..55 -- the emitter seals n then n entries right after the map's four quads when GC_FRAME_MAP_BLOB is set, and the collector reads them through (const uint64_t *)(map + 1); no relocation in either medium, so the map's sizeof stays 32 (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2e)");
#endif
#define ZLS_FRAME_ALLOWANCE 64
#define ZLS_FRAME_ALLOWANCE_PINNED 96
#ifdef __cplusplus
static_assert(ZLS_FRAME_ALLOWANCE == 48 + 16 && ZLS_FRAME_ALLOWANCE_PINNED == 80 + 16, "the allowance above the value region is the wire header (48, or 80 with the Prolog quad) plus ONE 16-byte map cell (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2, CTO-65); every frame-size formula reads these two names, never a literal, because the generator reserve in frame_layout.c and the two prologue formulas in emit.cpp must agree to the byte or a generator delivers its result from the wrong cell");
#else
_Static_assert(ZLS_FRAME_ALLOWANCE == 48 + 16 && ZLS_FRAME_ALLOWANCE_PINNED == 80 + 16, "the allowance above the value region is the wire header (48, or 80 with the Prolog quad) plus ONE 16-byte map cell (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2, CTO-65); every frame-size formula reads these two names, never a literal, because the generator reserve in frame_layout.c and the two prologue formulas in emit.cpp must agree to the byte or a generator delivers its result from the wrong cell");
#endif
#endif
