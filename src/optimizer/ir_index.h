#ifndef IR_INDEX_H
#define IR_INDEX_H
#include "IR.h"
#include <stdlib.h>
#include <stdint.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* O(1) POINTER->INDEX MAP OVER g->all[].  Replaces the one-line linear scan that dg_run and bc_run each ran per operand inside a loop over every node -- O(N^2), and 48.7% of every
   compile instruction on a 400-statement program (FINDING s251; dg_index_of alone was 42.9%).  Semantics are byte-exact with the scan it replaces: the FIRST matching index wins, a
   NULL query matches the first NULL slot, and an absent pointer returns -1.  The map is a function-local value built once per run and freed at the end, so no file-scope state is
   introduced and the no-new-globals rule is not engaged.  If either allocation fails the map degrades to the original scan rather than changing an answer.  ⛔ Valid only while
   g->all[] is unmutated: dg_run NULLs dead slots only after every lookup has happened, which is why one build per run is sound. */
typedef struct { IR_t ** key; int * val; size_t cap; int nullix; IR_graph_t * g; } ir_index_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t ir_index_slot(const ir_index_t * ix, const IR_t * p) {
    uint64_t h = (uint64_t)(uintptr_t)p; h ^= h >> 33; h *= 0xff51afd7ed558ccdULL; h ^= h >> 29; return (size_t)h & (ix->cap - 1u);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ir_index_build(ir_index_t * ix, IR_graph_t * g) {
    ix->g = g; ix->key = (IR_t **)0; ix->val = (int *)0; ix->nullix = -1; ix->cap = 16u;
    while (ix->cap < (size_t)(g->n > 0 ? g->n : 1) * 2u) ix->cap <<= 1;
    ix->key = (IR_t **)calloc(ix->cap, sizeof(IR_t *)); ix->val = (int *)calloc(ix->cap, sizeof(int));
    if (!ix->key || !ix->val) { free(ix->key); free(ix->val); ix->key = (IR_t **)0; ix->val = (int *)0; return; }
    for (int i = 0; i < g->n; i++) {
        IR_t * p = g->all[i];
        if (!p) { if (ix->nullix < 0) ix->nullix = i; continue; }
        size_t s = ir_index_slot(ix, p);
        while (ix->key[s] && ix->key[s] != p) s = (s + 1u) & (ix->cap - 1u);
        if (!ix->key[s]) { ix->key[s] = p; ix->val[s] = i; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_index_of(const ir_index_t * ix, IR_t * p) {
    if (!p) return ix->nullix;
    if (!ix->key) { IR_graph_t * g = ix->g; for (int i = 0; i < g->n; i++) if (g->all[i] == p) return i; return -1; }
    size_t s = ir_index_slot(ix, p);
    while (ix->key[s]) { if (ix->key[s] == p) return ix->val[s]; s = (s + 1u) & (ix->cap - 1u); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ir_index_free(ir_index_t * ix) { free(ix->key); free(ix->val); ix->key = (IR_t **)0; ix->val = (int *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* O(1) INCREMENTAL POINTER SET.  The map above needs the whole membership up front; a traversal that discovers nodes as it walks does not have that, so this is the grow-as-you-go twin.  It replaces
   the same linear-scan-for-membership shape found in the emitter's RPO walk, where a scan over every already-seen node ran per pushed edge -- the O(N^2) that survived the optimizer fix (FINDING
   s251 §7).  NULL is tracked in its own flag rather than as a key, so has(NULL) answers exactly what a linear scan of the old array would have answered instead of colliding with the empty marker.
   Heap-backed and function-local: it REPLACES a file-scope static array, so it lowers the global count rather than raising it.  An allocation failure leaves the set unchanged, which can only make
   has() answer 0 -- a re-visit, never a wrong emit. */
typedef struct { const IR_t ** slot; size_t cap; size_t cnt; int has_null; } ir_pset_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t ir_pset_probe(const IR_t ** slot, size_t cap, const IR_t * p) {
    uint64_t h = (uint64_t)(uintptr_t)p; h ^= h >> 33; h *= 0xff51afd7ed558ccdULL; h ^= h >> 29;
    size_t i = (size_t)h & (cap - 1u); while (slot[i] && slot[i] != p) i = (i + 1u) & (cap - 1u); return i;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ir_pset_init(ir_pset_t * s) { s->slot = (const IR_t **)0; s->cap = 0; s->cnt = 0; s->has_null = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_pset_has(const ir_pset_t * s, const IR_t * p) {
    if (!p) return s->has_null;
    return s->cap != 0 && s->slot[ir_pset_probe(s->slot, s->cap, p)] == p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ir_pset_add(ir_pset_t * s, const IR_t * p) {
    if (!p) { s->has_null = 1; return; }
    if (s->cnt * 2u >= s->cap) {
        size_t nc = s->cap ? s->cap * 2u : 1024u;
        const IR_t ** ns = (const IR_t **)calloc(nc, sizeof(const IR_t *));
        if (!ns) return;
        for (size_t i = 0; i < s->cap; i++) if (s->slot[i]) ns[ir_pset_probe(ns, nc, s->slot[i])] = s->slot[i];
        free(s->slot); s->slot = ns; s->cap = nc;
    }
    size_t i = ir_pset_probe(s->slot, s->cap, p);
    if (!s->slot[i]) { s->slot[i] = p; s->cnt++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ir_pset_free(ir_pset_t * s) { free(s->slot); s->slot = (const IR_t **)0; s->cap = 0; s->cnt = 0; s->has_null = 0; }
#endif
