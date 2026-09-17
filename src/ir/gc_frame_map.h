#ifndef GC_FRAME_MAP_H
#define GC_FRAME_MAP_H
#include <stdint.h>
#include <stddef.h>
#define GC_FRAME_MAP_MAGIC 0x50414D5Au
#define GC_FRAME_MAP_ROOT 1u
#define GC_FRAME_MAP_GEN_ANCHOR 2u
typedef struct gc_frame_map_t {
    uint32_t     magic;
    uint32_t     frame_bytes;
    uint32_t     header_bytes;
    uint32_t     flags;
    const char * graph_name;
    uint64_t     reserved;
} gc_frame_map_t;
#ifdef __cplusplus
static_assert(sizeof(gc_frame_map_t) == 32, "gc_frame_map_t is four sealed quads beside the code in both media (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.3): magic|frame_bytes, header_bytes|flags, graph_name, reserved -- the emitter writes them as four quads and the collector reads them through this struct, so the size is the contract");
static_assert(offsetof(gc_frame_map_t, graph_name) == 16 && offsetof(gc_frame_map_t, header_bytes) == 8, "the quad order is magic|frame_bytes, header_bytes|flags, graph_name, reserved");
#else
_Static_assert(sizeof(gc_frame_map_t) == 32, "gc_frame_map_t is four sealed quads beside the code in both media (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.3): magic|frame_bytes, header_bytes|flags, graph_name, reserved -- the emitter writes them as four quads and the collector reads them through this struct, so the size is the contract");
_Static_assert(offsetof(gc_frame_map_t, graph_name) == 16 && offsetof(gc_frame_map_t, header_bytes) == 8, "the quad order is magic|frame_bytes, header_bytes|flags, graph_name, reserved");
#endif
#define ZLS_FRAME_ALLOWANCE 64
#define ZLS_FRAME_ALLOWANCE_PINNED 96
#ifdef __cplusplus
static_assert(ZLS_FRAME_ALLOWANCE == 48 + 16 && ZLS_FRAME_ALLOWANCE_PINNED == 80 + 16, "the allowance above the value region is the wire header (48, or 80 with the Prolog quad) plus ONE 16-byte map cell (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2, CTO-65); every frame-size formula reads these two names, never a literal, because the generator reserve in frame_layout.c and the two prologue formulas in emit.cpp must agree to the byte or a generator delivers its result from the wrong cell");
#else
_Static_assert(ZLS_FRAME_ALLOWANCE == 48 + 16 && ZLS_FRAME_ALLOWANCE_PINNED == 80 + 16, "the allowance above the value region is the wire header (48, or 80 with the Prolog quad) plus ONE 16-byte map cell (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2, CTO-65); every frame-size formula reads these two names, never a literal, because the generator reserve in frame_layout.c and the two prologue formulas in emit.cpp must agree to the byte or a generator delivers its result from the wrong cell");
#endif
#endif
