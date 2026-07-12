/* rt_arena.h — RUNG TR-1: named bump arenas over the slab pool (REGION 2).
 * TAXONOMY (Lon, ratified s35, refined s36): the C stack carries rsp-relative ζ
 * cells + C runtime frames (REGION 1); these arenas carry EVERYTHING ELSE,
 * including the REUSABLE ζ blocks for co-expressions and generator procedures.
 * Flavors are thin POLICY over one mechanism:
 *   A_PROG   — program-lifetime; never released (strings, DT_P recipes, NV nodes).
 *   A_TRANS  — transient; mark/release at the nesting language brackets
 *              (statement ⊃ match ⊃ EVAL ⊃ call — LIFO, zero holes).
 *   A_SUSP   — escaping activations (suspended generators); ZH death-mark protocol
 *              rebased here at TR-2.
 *   A_COEXPR — reusable ζ blocks for co-expressions / generator procedures
 *              (Lon s36 "re-usable": fixed blocks recycled via a free list on death).
 * D3 (proposed default): allocations ZERO-FILLED (GC_malloc zero-filled; raw malloc
 *    does not — silent-breakage class). Flip: RT_ARENA_ZERO=0.
 * D6 (proposed default): A_PROG objects carry an 8-byte title word (size|flavor)
 *    as compaction insurance. Flip: RT_ARENA_TITLE=0. */
#ifndef RT_ARENA_H
#define RT_ARENA_H
#include <stddef.h>
#include <stdint.h>
#include "rt_slab.h"

#ifndef RT_ARENA_ZERO
#define RT_ARENA_ZERO 1
#endif
#ifndef RT_ARENA_TITLE
#define RT_ARENA_TITLE 1
#endif

typedef enum { A_PROG = 0, A_TRANS = 1, A_SUSP = 2, A_COEXPR = 3 } arena_flavor_t;

typedef struct rt_arena {
    rt_slab_t     *head;     /* newest slab (alloc target) — chain via slab->next to older */
    uint8_t       *cur, *end;
    arena_flavor_t flavor;
    void          *reuse;    /* A_COEXPR: free list of recycled fixed ζ blocks */
    size_t         block_sz; /* A_COEXPR: the fixed block size (0 until first use) */
} rt_arena_t;

typedef struct { rt_slab_t *slab; uint8_t *cur; } arena_mark_t;

void         rt_arena_init(rt_arena_t *a, arena_flavor_t f);
void        *rt_arena_alloc(rt_arena_t *a, size_t n);          /* 16-aligned; zero per D3 */
arena_mark_t rt_arena_mark(rt_arena_t *a);
void         rt_arena_release(rt_arena_t *a, arena_mark_t m);  /* LIFO; A_PROG aborts */
void         rt_arena_destroy(rt_arena_t *a);                  /* all slabs -> pool */
/* A_COEXPR reusable ζ blocks (Lon s36): fixed-size, recycled on death. */
void        *rt_arena_zblock_get(rt_arena_t *a, size_t block_sz);
void         rt_arena_zblock_put(rt_arena_t *a, void *blk);
/* THE WORKSPACE (TR-3): the one Region-2 instance; grow-only until GC-W-2. */
void        *rt_ws_alloc(size_t n);
#endif
