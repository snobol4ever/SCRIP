#include <stdio.h>
#ifndef PL_CELL_H
#define PL_CELL_H
#include "rt/rt_arena.h"
#include "descr.h"
#include "pl_area.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef DESCR_t pl_cell_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int      pl_tag(const pl_cell_t *c)     { return (int)c->v; }
static inline uint32_t pl_disc(const pl_cell_t *c)    { return c->slen; }
static inline pl_cell_t pl_make_int(int64_t v)   { pl_cell_t c; c.v = DT_I;  c.slen = 0;        c.i = v;       return c; }
static inline pl_cell_t pl_make_atom(int id)     { pl_cell_t c; c.v = DT_A;  c.slen = (uint32_t)id; c.i = id;  return c; }
static inline pl_cell_t pl_make_float(double d)  { pl_cell_t c; c.v = DT_R;  c.slen = 0;        c.r = d;       return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline pl_cell_t pl_make_compound(int functor, int arity, void *heap) {
    pl_cell_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)functor) << 16) | ((uint32_t)arity & 0xFFFFu); c.p = heap; return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_init_var(pl_cell_t *c, int slot) { c->v = (DTYPE_t)DT_PLVAR; c->slen = (uint32_t)slot; c->p = (void *)c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline pl_cell_t pl_make_ref(pl_cell_t *target, int slot) {
    pl_cell_t c; c.v = (DTYPE_t)DT_PLVAR; c.slen = (uint32_t)slot; c.p = (void *)target; return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline pl_cell_t *pl_deref(pl_cell_t *c) {
    for (;;) {
        if ((int)c->v == DT_PLVAR && c->p != (void *)c) { c = (pl_cell_t *)c->p; continue; }
        if (c->v == DT_N && c->slen == 1 && c->p) { c = (pl_cell_t *)c->p; continue; }
        if (c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) { c = ((VCELL_t *)c->p)->cellp; continue; }
        return c;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_cell_unbound(const pl_cell_t *d) { return (int)d->v == DT_PLVAR || d->v == DT_SNUL || d->v == DT_FAIL; }
static inline int pl_is_var(pl_cell_t *c)      { pl_cell_t *d = pl_deref(c); return pl_cell_unbound(d); }
static inline int pl_is_int(pl_cell_t *c)      { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_I; }
static inline int pl_is_atom(pl_cell_t *c)     { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_A; }
static inline int plc_is_float(pl_cell_t *c)    { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_R; }
static inline int pl_is_compound(pl_cell_t *c) { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_PLREF; }
static inline int64_t pl_int_val(pl_cell_t *c)  { return pl_deref(c)->i; }
static inline int     pl_atom_id(pl_cell_t *c)  { return (int)pl_deref(c)->i; }
static inline double  pl_float_val(pl_cell_t *c) { return pl_deref(c)->r; }
static inline int plc_functor(pl_cell_t *c) { return (int)(pl_deref(c)->slen >> 16); }
static inline int pl_arity(pl_cell_t *c)   { return (int)(pl_deref(c)->slen & 0xFFFFu); }
static inline void *pl_compound_heap(pl_cell_t *c) { return pl_deref(c)->p; }
typedef struct { pl_cell_t *addr; pl_cell_t old; } pl_trail_ent_t;
typedef struct { pl_area_t area; int top; } pl_trail_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_trail_init(pl_trail_t *t) { t->area.base = t->area.top = t->area.limit = (char *)0; t->area.cap = 0; t->top = 0; }
static inline int  pl_trail_mark(const pl_trail_t *t) { return t->top; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_trail_push(pl_trail_t *t, pl_cell_t *addr) {
    if (!t->area.base) pl_area_init(&t->area, PL_AREA_DEFAULT_BYTES);
    if ((size_t)(t->top + 1) * sizeof(pl_trail_ent_t) > t->area.cap) { if (!pl_area_grow(&t->area, sizeof(pl_trail_ent_t))) return; }
    pl_trail_ent_t *ents = (pl_trail_ent_t *)t->area.base;
    ents[t->top].addr = addr;
    ents[t->top].old  = *addr;
    t->top++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int plc_dead_cstack(const void *p) {
    extern char *g_plw_unwind_floor;
    if (!g_plw_unwind_floor) return 0;
    static char *stk_lo, *stk_hi; static int stk_have;
    if (!stk_have || ((char *)p < stk_lo && (char *)p >= stk_lo - (64L << 20))) {
        FILE *mf = fopen("/proc/self/maps", "r"); char ln[256]; unsigned long a = 0, b = 0;
        if (mf) { while (fgets(ln, sizeof ln, mf)) if (strstr(ln, "[stack]")) { if (sscanf(ln, "%lx-%lx", &a, &b) == 2) { stk_lo = (char *)a; stk_hi = (char *)b; stk_have = 1; } break; } fclose(mf); }
        if (!stk_have) return 0;
    }
    if ((char *)p < stk_lo || (char *)p >= stk_hi) return 0;
    return (char *)p < g_plw_unwind_floor + 16;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_trail_unwind(pl_trail_t *t, int mark) {
    if (mark < 0) { fprintf(stderr, "SCRIP FATAL: pl_trail_unwind refuses corrupt trail mark %d (top=%d, caller=%p): its PRODUCER handed over garbage.\n", mark, t->top, __builtin_return_address(0));
                    fprintf(stderr, "SCRIP FATAL: unwinding it would index ents[-1] and write 16 bytes past the trail array. TRIPWIRE, not a cure.\n");
                    fflush(stderr); abort(); }
    pl_trail_ent_t *ents = (pl_trail_ent_t *)t->area.base;
    while (t->top > mark) {
        t->top--;
        if (!plc_dead_cstack(ents[t->top].addr)) *ents[t->top].addr = ents[t->top].old;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_bind(pl_cell_t *cell, pl_cell_t word, pl_trail_t *trail) {
    pl_cell_t *v = pl_deref(cell);
    char probe; char *floor_ = &probe;
    if ((char *)v <= floor_) { extern void *rt_ws_alloc(size_t); pl_cell_t *j = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *j = word; word.v = (DTYPE_t)DT_PLVAR; word.slen = 0; word.p = (void *)j; }
    pl_trail_push(trail, v);
    *v = word;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_unify(pl_cell_t *a, pl_cell_t *b, pl_trail_t *trail) {
    pl_cell_t *A = pl_deref(a), *B = pl_deref(b);
    if (A == B) return 1;
    int av = pl_cell_unbound(A), bv = pl_cell_unbound(B);
    if (av && bv) { extern void *rt_ws_alloc(size_t); pl_cell_t *j = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; pl_cell_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; pl_bind(A, r, trail); pl_bind(B, r, trail); return 1; }
    if (av) { pl_bind(A, *B, trail); return 1; }
    if (bv) { pl_bind(B, *A, trail); return 1; }
    if (((int)A->v == DT_S || (int)A->v == DT_A) && ((int)B->v == DT_S || (int)B->v == DT_A)) {
        extern const char *prolog_atom_name(int);
        const char *as = ((int)A->v == DT_S) ? (A->s ? A->s : "") : prolog_atom_name((int)A->i);
        const char *bs = ((int)B->v == DT_S) ? (B->s ? B->s : "") : prolog_atom_name((int)B->i);
        return as && bs && !strcmp(as, bs);
    }
    if (A->v != B->v) return 0;
    if ((int)A->v == DT_I) return A->i == B->i;
    if ((int)A->v == DT_R) return A->r == B->r;
    if ((int)A->v == DT_PLREF) {
        if (A->slen != B->slen) return 0;
        int ar = (int)(A->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)A->p, *bb = (pl_cell_t *)B->p;
        for (int i = 0; i < ar; i++) if (!pl_unify(&aa[i], &bb[i], trail)) return 0;
        return 1;
    }
    return 0;
}
#endif
