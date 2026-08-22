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
#endif
