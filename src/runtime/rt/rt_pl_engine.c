#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "g_runtime.h"
#include "core.h"
#include "rt/rt.h"
#include "ct_arena.h"
#include "rt_pl_trail.h"
extern void rt_proc_drop_frame_h(void **);
extern int rt_genp_valid(void *);
extern void rt_pl_tr_free_arena(const char *, void *);
extern int rt_gc_weak_keep(const char **loc);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_ENG_TYPE "\001PlEngine"
static g_pl_eng_t *E(void) { return &g_runtime.pl_eng; }
static int pl_eng_is_inst(DESCR_t d) { return d.v == DT_DATA && d.u && IS_DATA_INST_fn(d) && d.u->type && d.u->type->name && !strcmp(d.u->type->name, PL_ENG_TYPE); }
static g_pl_eng_ent_t *pl_eng_find(DESCR_t d) {
    g_pl_eng_t *e = E();
    if (!pl_eng_is_inst(d)) return (g_pl_eng_ent_t *)0;
    for (int i = 0; i < e->n; i++) if (e->ent[i].inst == (void *)d.u) return &e->ent[i];
    return (g_pl_eng_ent_t *)0;
}
static DESCR_t pl_eng_descr(const g_pl_eng_ent_t *x) { DESCR_t d = {0}; d.v = DT_DATA; d.slen = DATA_INST_SLEN; d.u = (DATINST_t *)x->inst; return d; }
static void pl_eng_free_ctx(int64_t ctx, int64_t arena, int64_t slab) {
    if (ctx && rt_genp_valid((void *)(intptr_t)ctx)) { void *slot = (void *)(intptr_t)ctx; rt_proc_drop_frame_h(&slot); }
    if (arena && slab) rt_pl_tr_free_arena((const char *)(intptr_t)arena, (void *)(intptr_t)slab);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_eng_make(int64_t ctx, int64_t arena, int64_t slab) {
    g_pl_eng_t *e = E();
    DESCR_t d;
#if RT_DIAG
    {
        extern int rt_gc_addr_in_heap(const void *);
        if (rt_gc_addr_in_heap((const void *)(intptr_t)ctx) || rt_gc_addr_in_heap((const void *)(intptr_t)arena) || rt_gc_addr_in_heap((const void *)(intptr_t)slab)) {
            fprintf(stderr, "rt_pl_eng_make: an address held as an integer in an engine handle lies inside the collected heap (ctx=%lx arena=%lx slab=%lx)\n", (long)ctx, (long)arena, (long)slab);
            abort();
        }
    }
#endif
    DEFDAT_fn(PL_ENG_TYPE "(ctx,arena,slab)");
    d = DATCON_fn(PL_ENG_TYPE, INTVAL(ctx), INTVAL(arena), INTVAL(slab));
    if (d.v != DT_DATA || !d.u) return FAILDESCR;
    if (e->n >= e->cap) { e->cap = e->cap ? 2 * e->cap : 16; e->ent = (g_pl_eng_ent_t *)ct_grow(e->ent, (size_t)e->cap * sizeof *e->ent); }
    e->ent[e->n].inst = (void *)d.u;
    e->ent[e->n].id = d.u->id;
    e->ent[e->n].ctx = ctx;
    e->ent[e->n].arena = arena;
    e->ent[e->n].slab = slab;
    e->ent[e->n].pinned = 0;
    e->n++;
    e->nmade++;
    return d;
}
int64_t rt_pl_eng_stat(int which) { g_pl_eng_t *e = E(); return which == 0 ? e->nmade : which == 1 ? e->nreaped_total : which == 2 ? e->nfreed : (int64_t)e->n; }
void rt_pl_eng_pin(DESCR_t d) { g_pl_eng_ent_t *x = pl_eng_find(d); if (x) x->pinned = 1; }
int rt_pl_eng_by_id(int64_t id, DESCR_t *out) { g_pl_eng_t *e = E(); for (int i = 0; i < e->n; i++) if (e->ent[i].id == id) { *out = pl_eng_descr(&e->ent[i]); return 1; } return 0; }
void rt_pl_eng_gc_roots(void) { g_pl_eng_t *e = E(); extern void rt_gc_visit_raw(const char **); for (int i = 0; i < e->n; i++) if (e->ent[i].pinned) rt_gc_visit_raw((const char **)&e->ent[i].inst); }
int rt_pl_eng_is(DESCR_t d) { return pl_eng_find(d) != (g_pl_eng_ent_t *)0; }
int rt_pl_eng_inst_is(DESCR_t d) { return pl_eng_is_inst(d); }
int64_t rt_pl_eng_inst_id(DESCR_t d) { return pl_eng_is_inst(d) ? (int64_t)d.u->id : -1; }
int64_t rt_pl_eng_id(DESCR_t d) { g_pl_eng_ent_t *x = pl_eng_find(d); return x ? x->id : -1; }
int64_t rt_pl_eng_ctx(DESCR_t d) { g_pl_eng_ent_t *x = pl_eng_find(d); return x ? x->ctx : 0; }
void rt_pl_eng_ctx_done(DESCR_t d) { g_pl_eng_ent_t *x = pl_eng_find(d); if (x) x->ctx = 0; }
void rt_pl_eng_destroy(DESCR_t d) {
    g_pl_eng_ent_t *x = pl_eng_find(d);
    int64_t c, a, s;
    if (!x) return;
    c = x->ctx;
    a = x->arena;
    s = x->slab;
    x->ctx = x->arena = x->slab = 0;
    pl_eng_free_ctx(c, a, s);
}
int rt_pl_eng_self(void *genp, DESCR_t *out) {
    g_pl_eng_t *e = E();
    if (!genp) return 0;
    for (int i = 0; i < e->n; i++) if (e->ent[i].ctx == (int64_t)(intptr_t)genp) { *out = pl_eng_descr(&e->ent[i]); return 1; }
    return 0;
}
int rt_pl_eng_count(void) { return E()->n; }
DESCR_t rt_pl_eng_at(int i) { g_pl_eng_t *e = E(); DESCR_t z = {0}; return i >= 0 && i < e->n ? pl_eng_descr(&e->ent[i]) : z; }
int rt_pl_eng_reaped_count(void) { return E()->nreaped; }
int64_t rt_pl_eng_reaped_take(int i) { g_pl_eng_t *e = E(); return i >= 0 && i < e->nreaped ? e->reaped[i] : -1; }
void rt_pl_eng_reaped_clear(void) { E()->nreaped = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_eng_gc_weak(void) {
    g_pl_eng_t *e = E();
    for (int i = 0; i < e->n; i++) {
        g_pl_eng_ent_t *x = &e->ent[i];
        if (!x->inst || x->pinned || rt_gc_weak_keep((const char **)&x->inst)) continue;
        if (x->ctx || x->arena) {
            if (e->npend >= e->cpend) { e->cpend = e->cpend ? 2 * e->cpend : 8; e->pend = (g_pl_eng_pend_t *)ct_grow(e->pend, (size_t)e->cpend * sizeof *e->pend); }
            e->pend[e->npend].ctx = x->ctx;
            e->pend[e->npend].arena = x->arena;
            e->pend[e->npend].slab = x->slab;
            e->npend++;
        }
        if (e->nreaped >= e->creaped) { e->creaped = e->creaped ? 2 * e->creaped : 16; e->reaped = (int64_t *)ct_grow(e->reaped, (size_t)e->creaped * sizeof *e->reaped); }
        e->reaped[e->nreaped++] = x->id;
        e->nreaped_total++;
        x->inst = (void *)0;
    }
}
void rt_pl_eng_reap_run(void) {
    g_pl_eng_t *e = E();
    int k = 0;
    for (int i = 0; i < e->n; i++) if (e->ent[i].inst) { if (k != i) e->ent[k] = e->ent[i]; k++; }
    e->n = k;
    while (e->npend > 0) { g_pl_eng_pend_t p = e->pend[--e->npend]; pl_eng_free_ctx(p.ctx, p.arena, p.slab); e->nfreed++; }
}
