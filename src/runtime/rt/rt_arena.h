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
 * D3 (proposed default): allocations ZERO-FILLED (raw malloc does not zero —
 *    silent-breakage class). Flip: RT_ARENA_ZERO=0.
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
/* THE WORKSPACE (TR-3 → GC-U-6 s84): rt_ws_* live in gc_heap.c — HB_WS-titled blocks in the ONE collected span; pinned-immortal v1, collector = gc_collect_ex. */
void        *rt_ws_alloc(size_t n);
void        *rt_ws_realloc(void *p, size_t n);   /* grow-only; old size read from the title word */
char        *rt_ws_strdup(const char *s);
void        *rt_ws_alloc_c(size_t n);            /* COLLECTABLE workspace class (HB_WSC): zero-pointer char payloads referenced only via DESCRs/scanned locations */
char        *rt_ws_strdup_c(const char *s);
/* PL-WS RECLAIMABLE COMPOUND ARENA (GC-W-2): A_TRANS default home for Prolog term_new_*; reclaims on backtrack. */
void        *rt_pl_cterm_alloc(size_t n);
arena_mark_t rt_pl_cterm_mark(void);
void         rt_pl_cterm_release(arena_mark_t m);
void         rt_pl_cterm_roots(void **base, size_t *bytes);
#endif
