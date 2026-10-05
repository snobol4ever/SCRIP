#ifndef PL_RATIONAL_H
#define PL_RATIONAL_H
#include <stdint.h>
#include <string.h>
#define PLR_LINK    ((DTYPE_t)(DT_PLREF + 2))
#define PLR_SEEN    ((DTYPE_t)(DT_PLREF + 3))
#define PLR_COPY    ((DTYPE_t)(DT_PLREF + 4))
#define PLR_VCOPY   ((DTYPE_t)(DT_PLREF + 5))
#define PLR_FZ_OPEN ((DTYPE_t)(DT_PLREF + 6))
#define PLR_FZ_DONE ((DTYPE_t)(DT_PLREF + 7))
#define PLR_DESCEND 0x7f
#define PLR_V_GO    0
#define PLR_V_STOP  1
#define PLR_V_SKIP  2
#define PLR_SEG0    64
#define PLR_SEGMAX  16384
_Static_assert(DT_PLREF + 7 < DT_X && DT_PLREF + 7 < 0x100,
               "RATIONAL TREES (CEO-1521, row prolog-rational-trees-...): PLR_LINK, PLR_SEEN, PLR_COPY, PLR_VCOPY and the two PLR_FZ marks are private type bytes a Prolog term walk writes into a "
               "cell it is "
               "walking and restores before the runtime call returns; they sit between DT_PLREF and the next tag, the collector never sees one (no collection inside a runtime call), "
               "and every walk resolves PLR_LINK to its partner. The walk is SWI-Prolog's (pl-prims.c do_unify / do_compare): a left-to-right agenda and a link per compound pair, "
               "undone at the end; the agenda and the link log live on the C stack in segments a VLA grows (THE LIFETIME RULE, CEO-1354), never on the heap");
typedef struct { void *x; void *y; uint64_t n; } plr_ent_t;
typedef struct plr_seg_s { plr_ent_t *e; long cap; long n; struct plr_seg_s *prev; struct plr_seg_s *next; } plr_seg_t;
typedef struct { plr_seg_t *top; long want; } plr_stk_t;
typedef int (*plr_run_t)(void *);
typedef struct { plr_stk_t ag, lg; void *ctx; DESCR_t *a, *b; int cont; } plr_w2_t;
typedef struct { plr_stk_t ag, lg; void *ctx; DESCR_t *c; } plr_w1_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void plr_stk_init(plr_stk_t *s) { s->top = (plr_seg_t *)0; s->want = PLR_SEG0; }
static inline int plr_room(const plr_stk_t *s) { return s->top && (s->top->n < s->top->cap || s->top->next); }
static inline int plr_empty(const plr_stk_t *s) { return !s->top || (!s->top->n && !s->top->prev); }
static inline plr_ent_t *plr_peek(plr_stk_t *s) { return &s->top->e[s->top->n - 1]; }
static inline void plr_drop(plr_stk_t *s) { if (!--s->top->n && s->top->prev) s->top = s->top->prev; }
static inline DESCR_t *plr_resolve(DESCR_t *c) { while (c->v == PLR_LINK) c = (DESCR_t *)c->p; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void plr_push(plr_stk_t *s, void *x, void *y, uint64_t n)
{
    plr_ent_t *e;
    if (s->top->n == s->top->cap) s->top = s->top->next;
    e = &s->top->e[s->top->n++]; e->x = x; e->y = y; e->n = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void plr_link(plr_stk_t *s, plr_seg_t *g, plr_ent_t *e, long cap)
{
    g->e = e; g->cap = cap; g->n = 0; g->prev = s->top; g->next = (plr_seg_t *)0;
    if (s->top) s->top->next = g; else s->top = g;
    if (s->want < PLR_SEGMAX) s->want *= 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((unused)) int plr_more(plr_stk_t *a, plr_stk_t *b, plr_run_t run, void *w)
{
    long na = !plr_room(a) ? a->want : 0, nb = !plr_room(b) ? b->want : 0;
    plr_ent_t ea[na > 0 ? na : 1], eb[nb > 0 ? nb : 1]; plr_seg_t sa, sb;
    if (na) plr_link(a, &sa, ea, na);
    if (nb) plr_link(b, &sb, eb, nb);
    return run(w);
}
#define PLR_WALK2(NAME, LEAF, DEREF) \
static int NAME##_run(void *wv) \
{ \
    plr_w2_t *w = (plr_w2_t *)wv; int r; \
    for (;;) { \
        DESCR_t *A = plr_resolve(DEREF(w->a)), *B = plr_resolve(DEREF(w->b)); \
        if (A != B) { \
            r = LEAF(A, B, w->ctx); \
            if (r == PLR_DESCEND) { \
                int ar = plc_fid_arity(A->slen); \
                if (!plr_room(&w->lg) || (ar > 0 && !plr_room(&w->ag))) return plr_more(&w->ag, &w->lg, NAME##_run, w); \
                plr_push(&w->lg, A, A->p, 0); \
                if (ar > 0) plr_push(&w->ag, A->p, B->p, (uint64_t)ar); \
                A->v = PLR_LINK; A->p = (void *)B; \
            } else if (r != w->cont) break; \
        } \
        r = w->cont; \
        if (plr_empty(&w->ag)) break; \
        { plr_ent_t *t = plr_peek(&w->ag); w->a = (DESCR_t *)t->x; w->b = (DESCR_t *)t->y; t->x = (void *)(w->a + 1); t->y = (void *)(w->b + 1); if (!--t->n) plr_drop(&w->ag); } \
    } \
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { DESCR_t *c = (DESCR_t *)g->e[i].x; c->v = (DTYPE_t)DT_PLREF; c->p = g->e[i].y; } \
    return r; \
} \
static int NAME(DESCR_t *a, DESCR_t *b, void *ctx, int cont) \
{ \
    plr_w2_t w; plr_stk_init(&w.ag); plr_stk_init(&w.lg); w.ctx = ctx; w.a = a; w.b = b; w.cont = cont; \
    return NAME##_run(&w); \
}
#define PLR_WALK1(NAME, VISIT, DEREF) \
static int NAME##_run(void *wv) \
{ \
    plr_w1_t *w = (plr_w1_t *)wv; int r = 0; \
    for (;;) { \
        DESCR_t *d = plr_resolve(DEREF(w->c)); \
        if (d->v != PLR_SEEN) { \
            int ar = d->v == (DTYPE_t)DT_PLREF ? plc_fid_arity(d->slen) : 0; \
            if (ar > 0 && (!plr_room(&w->lg) || !plr_room(&w->ag))) return plr_more(&w->ag, &w->lg, NAME##_run, w); \
            r = VISIT(d, w->ctx); \
            if (r == PLR_V_STOP) break; \
            if (r == PLR_V_GO && ar > 0) { plr_push(&w->lg, d, (void *)0, 0); plr_push(&w->ag, d->p, (void *)0, (uint64_t)ar); d->v = PLR_SEEN; } \
            r = 0; \
        } \
        if (plr_empty(&w->ag)) break; \
        { plr_ent_t *t = plr_peek(&w->ag); w->c = (DESCR_t *)t->x; t->x = (void *)(w->c + 1); if (!--t->n) plr_drop(&w->ag); } \
    } \
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) ((DESCR_t *)g->e[i].x)->v = (DTYPE_t)DT_PLREF; \
    return r == PLR_V_STOP; \
} \
static int NAME(DESCR_t *c, void *ctx) \
{ \
    plr_w1_t w; plr_stk_init(&w.ag); plr_stk_init(&w.lg); w.ctx = ctx; w.c = c; \
    return NAME##_run(&w); \
}
#endif
