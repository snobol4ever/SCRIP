#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "driver/driver_globals.h"
#include "runtime/core/utf8.h"
#include "runtime/pl_rat.h"
#include "runtime/rt/pl_rational.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_26_sh.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
#include "runtime/lib/bn_33_pl.h"
static int plw_oc_refuse(DESCR_t *v, DESCR_t *t, pl_tr_ctx_t *cx);
extern int rt_pl_eng_inst_is(DESCR_t);
extern void *rt_pl_tr_new_arena(void **);
extern void rt_pl_tr_free_arena(const char *, void *);
extern int rt_genp_valid(void *);
extern void *rt_genp_self_handle(void);
extern void rt_genp_yield(uint64_t, uint64_t);
extern DESCR_t rt_proc_resume_frame_h(void **);
extern void rt_proc_drop_frame_h(void **);
extern DESCR_t rt_pl_eng_make(int64_t, int64_t, int64_t);
extern int rt_pl_eng_inst_is(DESCR_t);
extern int64_t rt_pl_eng_inst_id(DESCR_t);
extern void rt_pl_eng_pin(DESCR_t);
extern int64_t rt_pl_eng_ctx(DESCR_t);
extern void rt_pl_eng_ctx_done(DESCR_t);
extern void rt_pl_eng_destroy(DESCR_t);
extern FILE *fh_get(int);
extern char * pl_tr_base_of(const char *);
static const icn_bi_ent_t *icn_bi_tbl(void) {
    static const icn_bi_ent_t tbl[] = { {"abs",1},{"acos",1},{"any",4},{"args",1},{"asin",1},{"atan",2},{"bal",6},{"center",3},{"char",1},{"chdir",1},{"close",1},{"collect",2},{"copy",1},{"cos",1},
        {"cset",1}, {"delay",1},{"delete",2},{"detab",-2},{"display",2},{"dtor",1},{"entab",-2},{"errorclear",0},{"exit",1},{"exp",1},{"find",4},{"flush",1},{"function",0},{"get",1},{"getch",0},
        {"getche",0},{"getenv",1},{"iand",2},{"icom",1},{"image",1},{"insert",3},{"integer",1},{"ior",2},{"ishift",2},{"ixor",2},{"kbhit",0},{"key",1},{"left",3},{"list",2},{"loadfunc",2}, {"log",2},
        {"many",4},{"map",3},{"match",4},{"member",2},{"move",1},{"name",1},{"numeric",1},{"open",2},{"ord",1},{"pop",1},{"pos",1},{"proc",2},{"pull",1},{"push",-2},{"put",-2}, {"read",1},{"reads",2},
        {"real",1},{"remove",1},{"rename",2},{"repl",2},{"reverse",1},{"right",3},{"rtod",1},{"runerr",-2},{"seek",2},{"seq",2},{"serial",1},{"set",1},{"sin",1}, {"sort",2},{"sortf",2},{"sqrt",1},
        {"stop",-1},{"string",1},{"system",1},{"tab",1},{"table",1},{"tan",1},{"trim",2},{"type",1},{"upto",4},{"variable",1},{"where",1},{"write",-1},{"writes",-1}, {NULL,0}};
    return tbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *icn_builtin_nth(int k) { const icn_bi_ent_t *tbl = icn_bi_tbl(); for (int i = 0; tbl[i].nm; i++) if (i == k) return tbl[i].nm; return (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_arity(const char *name) {
    if (!name || !name[0] || name[0] < 'a' || name[0] > 'z') return ICN_ARITY_UNKNOWN;
    const icn_bi_ent_t *tbl = icn_bi_tbl();
    for (int i = 0; tbl[i].nm; i++) if (tbl[i].nm[0] == name[0] && !strcmp(tbl[i].nm, name)) return tbl[i].np;
    return ICN_ARITY_UNKNOWN;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *plw_cell_deref_slow(DESCR_t *c) {
    if (c->v == DT_N && c->slen == 1 && c->p && ((DESCR_t *)c->p)->v == DT_N && ((DESCR_t *)c->p)->slen == 1 && ((DESCR_t *)c->p)->p) {
        DESCR_t *last = (DESCR_t *)c->p;
        DESCR_t *q;
        while (((DESCR_t *)last->p)->v == DT_N && ((DESCR_t *)last->p)->slen == 1 && ((DESCR_t *)last->p)->p) last = (DESCR_t *)last->p;
        for (q = c; q != last; ) { DESCR_t *nx = (DESCR_t *)q->p; q->p = (void *)last; q = nx; }
        c = last;
    }
    for (unsigned long hops = 0;; hops++) {
        if (c->v == DT_N && c->slen == 1 && c->p) {
            c = (DESCR_t *)c->p;
        } else if (c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) {
            c = ((VCELL_t *)c->p)->cellp;
        } else if (c->v == (DTYPE_t)DT_PLVAR && c->p && c->p != (void *)c) {
            c = (DESCR_t *)c->p;
        } else return c;
        if (hops == (1UL << 28)) {
            extern void rt_bomb(const char *msg);
            rt_bomb("plw_cell_deref_slow: a reference chain of 2^28 hops is a cycle, which no sound binding can make -- a bind wrote a cell into its own chain");
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plw_bind(DESCR_t *cell, DESCR_t word, pl_tr_ctx_t *cx) {
    char probe;
    char *floor_ = &probe;
    if (pl_attv_is(cell)) {
        if (!cx || !cx->tr) { extern void rt_bomb(const char *msg); rt_bomb("plw_bind: an attributed variable bound with no trail context, so its wakeup cannot be recorded"); }
        pl_tr_push_attv(cx, cell);
    } else if (pl_tr_needs_log_bind(cx, cell, floor_)) pl_tr_push(cx, cell);
    *cell = word;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_unify_leaf(DESCR_t *A, DESCR_t *B, void *ctx) {
    pl_tr_ctx_t *cx = (pl_tr_ctx_t *)ctx;
    int av = plw_unbound_tag(A), bv = plw_unbound_tag(B);
    if (av && bv) {
        char probe;
        char *floor_ = &probe;
        int aa = pl_attv_is(A), ba = pl_attv_is(B);
        if (aa || ba) {
            DESCR_t r = {0};
            r.v = (DTYPE_t)DT_PLVAR;
            if (aa && ba) { DESCR_t *lo = A < B ? A : B, *hi = A < B ? B : A; r.p = (void *)lo; plw_bind(hi, r, cx); return 1; }
            if (aa) { r.p = (void *)A; plw_bind(B, r, cx); return 1; }
            r.p = (void *)B;
            plw_bind(A, r, cx);
            return 1;
        }
        return plw_bind_vv(A, B, cx, floor_);
    }
    if (av) { if (B->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_oc_refuse(A, B, cx)) return 0; plw_bind(A, *B, cx); return 1; }
    if (bv) { if (A->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_oc_refuse(B, A, cx)) return 0; plw_bind(B, *A, cx); return 1; }
    if (A->v == (DTYPE_t)DT_PLREF && B->v == (DTYPE_t)DT_PLREF) return A->slen == B->slen ? PLR_DESCEND : 0;
    if (A->v == (DTYPE_t)DT_PLREF || B->v == (DTYPE_t)DT_PLREF) return 0;
    if (A->v == DT_I && B->v == DT_I && !A->slen && !B->slen) return A->i == B->i;
    if (A->v == (DTYPE_t)DT_PLATOM && B->v == (DTYPE_t)DT_PLATOM) return A->i == B->i;
    { int aa = (int)A->v == DT_S || (int)A->v == DT_PLATOM, ba = (int)B->v == DT_S || (int)B->v == DT_PLATOM; if (aa || ba) { return aa && ba && rt_pl_text_eq(A, B); } }
    if (A->v == DT_R && B->v == DT_R) return (isnan(A->r) && isnan(B->r)) || (A->r == B->r && signbit(A->r) == signbit(B->r));
    if (A->v == (DTYPE_t)DT_BIG || B->v == (DTYPE_t)DT_BIG || pl_rat_is(*A) || pl_rat_is(*B)) { extern int rt_pl_unify_atomic_cold(DESCR_t *, DESCR_t *); return rt_pl_unify_atomic_cold(A, B); }
    { extern int rt_descr_equal(DESCR_t, DESCR_t); return rt_descr_equal(*A, *B); }
}
PLR_WALK2(plw_unify_walk, plw_unify_leaf, plw_cell_deref) int plw_unify_cells(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) { return plw_unify_walk(a, b, (void *)cx, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_occurs_visit(DESCR_t *d, void *v) { return d == (DESCR_t *)v ? PLR_V_STOP : PLR_V_GO; }
PLR_WALK1(plw_occurs_walk, plw_occurs_visit, plw_cell_deref) int rt_pl_occurs_a(DESCR_t *v, DESCR_t *t, const void *functors, long nfunctors);
static int plw_occurs_in(DESCR_t *v, DESCR_t *t) {
    extern const void *rt_pl_functor_entries(void);
    extern int prolog_functor_count(void);
    int r = rt_pl_occurs_a(v, t, rt_pl_functor_entries(), (long)prolog_functor_count());
    return r != 2 ? r : plw_occurs_walk(t, (void *)v);
}
static int plw_oc_refuse(DESCR_t *v, DESCR_t *t, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_occurs(DESCR_t *, DESCR_t *);
    if (!plw_occurs_in(v, t)) return 0;
    if (pl_tr_ocheck_mode(cx) == 2 && !cx->ball) cx->ball = rt_pl_ball_occurs(v, t);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_unify_oc_leaf(DESCR_t *A, DESCR_t *B, void *ctx) {
    pl_tr_ctx_t *cx = (pl_tr_ctx_t *)ctx;
    int av = plw_unbound_tag(A), bv = plw_unbound_tag(B);
    if (av && bv) {
        int aa = pl_attv_is(A), ba = pl_attv_is(B);
        if (aa || ba) {
            DESCR_t r = {0};
            r.v = (DTYPE_t)DT_PLVAR;
            if (aa && ba) { DESCR_t *lo = A < B ? A : B, *hi = A < B ? B : A; r.p = (void *)lo; plw_bind(hi, r, cx); return 1; }
            if (aa) { r.p = (void *)A; plw_bind(B, r, cx); return 1; }
            r.p = (void *)B;
            plw_bind(A, r, cx);
            return 1;
        }
        { char probe; return plw_bind_vv(A, B, cx, &probe); }
    }
    if (av) { if (plw_occurs_in(A, B)) return 0; plw_bind(A, *B, cx); return 1; }
    if (bv) { if (plw_occurs_in(B, A)) return 0; plw_bind(B, *A, cx); return 1; }
    if (A->v == (DTYPE_t)DT_PLREF && B->v == (DTYPE_t)DT_PLREF) return A->slen == B->slen ? PLR_DESCEND : 0;
    if (A->v == (DTYPE_t)DT_PLREF || B->v == (DTYPE_t)DT_PLREF) return 0;
    if (A->v == DT_I && B->v == DT_I && !A->slen && !B->slen) return A->i == B->i;
    if (A->v == (DTYPE_t)DT_PLATOM && B->v == (DTYPE_t)DT_PLATOM) return A->i == B->i;
    { int aa = (int)A->v == DT_S || (int)A->v == DT_PLATOM, ba = (int)B->v == DT_S || (int)B->v == DT_PLATOM; if (aa || ba) { return aa && ba && rt_pl_text_eq(A, B); } }
    if (A->v == DT_R && B->v == DT_R) return (isnan(A->r) && isnan(B->r)) || (A->r == B->r && signbit(A->r) == signbit(B->r));
    if (A->v == (DTYPE_t)DT_BIG || B->v == (DTYPE_t)DT_BIG || pl_rat_is(*A) || pl_rat_is(*B)) { extern int rt_pl_unify_atomic_cold(DESCR_t *, DESCR_t *); return rt_pl_unify_atomic_cold(A, B); }
    { extern int rt_descr_equal(DESCR_t, DESCR_t); return rt_descr_equal(*A, *B); }
}
PLR_WALK2(plw_unify_oc_walk, plw_unify_oc_leaf, plw_cell_deref) static int plw_unify_cells_oc(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) { return plw_unify_oc_walk(a, b, (void *)cx, 1); }
__attribute__((visibility("hidden"))) int plw_unify_cells_x(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) { return plw_unify_cells(a, b, cx); }
__attribute__((visibility("hidden"))) int plw_unify_cell_val(DESCR_t *dst, DESCR_t val, pl_tr_ctx_t *cx) { DESCR_t tmp = val; return plw_unify_cells(dst, plw_entry(&tmp), cx); }
__attribute__((visibility("hidden"))) void plw_bind_x(DESCR_t *cell, DESCR_t word, pl_tr_ctx_t *cx) { plw_bind(cell, word, cx); }
__attribute__((visibility("hidden"))) int plw_unify_vals_oc(DESCR_t va, DESCR_t vb, pl_tr_ctx_t *cx) { DESCR_t ta = va, tb = vb; return plw_unify_cells_oc(plw_entry(&ta), plw_entry(&tb), cx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_catch_trim(const pl_tr_ctx_t *cx, pl_tr_ctx_t *c2, char *mark) {
    char probe;
    char *floor_ = &probe, *s = mark, *d = mark;
    for (; s < c2->tr; s += PL_TR_ENTRY_BYTES) {
        const pl_tr_entry_t *e = (const pl_tr_entry_t *)s;
        if (e->cell && !e->pad && !pl_tr_needs_log_bind(cx, e->cell, floor_)) continue;
        if (d != s) memcpy(d, s, PL_TR_ENTRY_BYTES);
        d += PL_TR_ENTRY_BYTES;
    }
    c2->tr = d;
    *(char **)pl_tr_base_of(d) = d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_catch_handle_c(DESCR_t *a, int n, void *ball, pl_tr_ctx_t *cx) {
    char fakeb[PL_TR_FRAME_HEADER_BYTES];
    char *base, *mark;
    uint64_t wake0;
    pl_tr_ctx_t c2;
    int ok;
    if (!a || n < 1 || !ball) return 0;
    base = pl_tr_base_of(cx->tr);
    mark = cx->tr;
    wake0 = *pl_tr_wake_slot(base);
    c2 = *cx;
    memset(fakeb, 0, sizeof fakeb);
    pl_tr_frame_hi_set(fakeb, (const void *)0);
    c2.b = fakeb;
    ok = plw_unify_cells(plw_entry(&a[0]), (DESCR_t *)ball, &c2);
    if (ok) { if (*pl_tr_wake_slot(base) == wake0) plc_catch_trim(cx, &c2, mark); cx->tr = c2.tr; cx->ball = c2.ball; return 1; }
    cx->tr = rt_pl_tr_unwind_to(c2.tr, mark);
    *pl_tr_wake_slot(base) = wake0;
    cx->ball = c2.ball;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_descr_equal(DESCR_t a, DESCR_t b) {
    if (a.v == DT_I && b.v == DT_I) return a.i == b.i;
    if (a.v == DT_R && b.v == DT_R) return a.r == b.r;
    if (a.v == DT_DATA && b.v == DT_DATA && a.u && a.u == b.u) return 1;
    if ((a.v == DT_S || a.v == DT_SNUL) && (b.v == DT_S || b.v == DT_SNUL)) { const char *x = a.s ? a.s : ""; const char *y = b.s ? b.s : ""; return strcmp(x, y) == 0; }
    { const char *x = VARVAL_fn(a); const char *y = VARVAL_fn(b); if (x && y) return strcmp(x, y) == 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_oc_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern int plw_unify_vals_oc(DESCR_t, DESCR_t, pl_tr_ctx_t *);
    if (nargs != 2) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    {
        char *tr0 = cx->tr;
        DESCR_t out;
        if (plw_unify_vals_oc(args[0], args[1], cx)) out = rt_pl_deref_val(args[0]);
        else { cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); out = FAILDESCR; }
        rt_pl_tr_gc_sync(cx->tr);
        return out;
    }
}
int rt_pl_unify_const_cold(const DESCR_t *c, int64_t k, int atom) { if (atom) return c->v == DT_PLATOM && c->i == k; return c->v == DT_I && c->i == k; }
int rt_pl_unify_atomic_cold(DESCR_t *A, DESCR_t *B) {
    int av = (int)A->v, bv = (int)B->v;
    if (av == DT_I && bv == DT_I) return A->i == B->i;
    if (av == DT_R && bv == DT_R) return (A->r == B->r && signbit(A->r) == signbit(B->r)) || (isnan(A->r) && isnan(B->r));
    if (av == DT_BIG || bv == DT_BIG) { extern int rt_big_eq(DESCR_t, DESCR_t); return rt_big_eq(*A, *B); }
    { int as = av == DT_S || av == DT_PLATOM, bs = bv == DT_S || bv == DT_PLATOM; if (as && bs) { return rt_pl_text_eq(A, B); } }
    if (pl_rat_is(*A) && pl_rat_is(*B)) return pl_rat_eq(*A, *B);
    if (av == DT_DATA && bv == DT_DATA && A->u == B->u && rt_pl_eng_inst_is(*A)) return 1;
    return 0;
}
int plu_leaf(DESCR_t *A, DESCR_t *B, void *ctx) {
    pl_tr_ctx_t *cx = ((plu_ctx_t *)ctx)->cx;
    const char *floor_ = ((plu_ctx_t *)ctx)->floor_;
    {
        int au = plw_unbound_tag(A), bu = plw_unbound_tag(B);
        int aa = au && pl_attv_is(A), ba = bu && pl_attv_is(B);
        if (aa || ba) {
            DESCR_t r = {0};
            r.v = (DTYPE_t)DT_PLVAR;
            if (au && bu && aa != ba) { DESCR_t *plain = aa ? B : A; r.p = (void *)(aa ? A : B); if (pl_tr_needs_log_bind(cx, plain, floor_)) pl_tr_push(cx, plain); *plain = r; return 1; }
            if (au && bu) { DESCR_t *lo = A < B ? A : B, *hi = A < B ? B : A; r.p = (void *)lo; pl_tr_push_attv(cx, hi); *hi = r; return 1; }
            if (aa) { if (B->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_occurs_in(A, B)) return 0; pl_tr_push_attv(cx, A); *A = *B; return 1; }
            if (A->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_occurs_in(B, A)) return 0;
            pl_tr_push_attv(cx, B);
            *B = *A;
            return 1;
        }
        if (au && bu) {
            int ah = (const char *)A <= floor_, bh = (const char *)B <= floor_;
            DESCR_t *from, *to;
            if (!ah && !bh) { from = A < B ? A : B; to = A < B ? B : A; } else if (ah != bh) { from = ah ? B : A; to = ah ? A : B; } else { from = A > B ? A : B; to = A > B ? B : A; }
            if (to->v != (DTYPE_t)DT_PLVAR || to->p != (void *)to) { if (pl_tr_needs_log_bind(cx, to, floor_)) pl_tr_push(cx, to); pl_var_make(to, cx, 0); }
            { DESCR_t r = {0}; r.v = (DTYPE_t)DT_PLVAR; r.p = (void *)to; if (pl_tr_needs_log_bind(cx, from, floor_)) pl_tr_push(cx, from); *from = r; return 1; }
        }
        if (au) { if (B->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_occurs_in(A, B)) return 0; if (pl_tr_needs_log_bind(cx, A, floor_)) pl_tr_push(cx, A); *A = *B; return 1; }
        if (bu) { if (A->v == (DTYPE_t)DT_PLREF && pl_tr_ocheck_mode(cx) && plw_occurs_in(B, A)) return 0; if (pl_tr_needs_log_bind(cx, B, floor_)) pl_tr_push(cx, B); *B = *A; return 1; }
    }
    if (A->v == (DTYPE_t)DT_PLREF) return (B->v != (DTYPE_t)DT_PLREF || A->slen != B->slen) ? 0 : PLR_DESCEND;
    if (B->v == (DTYPE_t)DT_PLREF) return 0;
    return rt_pl_unify_atomic_cold(A, B);
}
DESCR_t rt_pl_dop_engine_new_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t ag[2], r, nm, hd;
    void *h = (void *)0, *ball = (void *)0, *slab = (void *)0;
    pl_tr_ctx_t c2;
    char *etr;
    int ok;
    if (nargs != 4) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    nm = rt_pl_deref_val(args[0]);
    if (!pl_atom_str(nm)) return FAILDESCR;
    etr = (char *)rt_pl_tr_new_arena(&slab);
    if (!etr) return FAILDESCR;
    c2 = *cx;
    c2.tr = etr;
    ag[0] = rt_pl_deref_val(args[1]);
    ag[1] = rt_pl_deref_val(args[2]);
    r = rt_pl_goal_gen_h_c(nm, ag, 2, &h, &ball, &c2);
    if (ball || !h) { rt_pl_tr_free_arena(etr, slab); cx->ball = ball; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    (void)r;
    hd = rt_pl_eng_make((int64_t)(intptr_t)h, (int64_t)(intptr_t)etr, (int64_t)(intptr_t)slab);
    if (hd.v != DT_DATA) { void *sl = h; rt_proc_drop_frame_h(&sl); rt_pl_tr_free_arena(etr, slab); rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    ok = plw_unify_vals(args[3], hd, cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_engine_next_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t hd, r;
    int64_t ctx;
    void *slot;
    int ok;
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    hd = rt_pl_deref_val(args[0]);
    ctx = rt_pl_eng_ctx(hd);
    if (!ctx || !rt_genp_valid((void *)(intptr_t)ctx)) return plw_unify_vals(args[1], pl_mk_atom("done"), cx) ? pl_ok() : FAILDESCR;
    slot = (void *)(intptr_t)ctx;
    rt_pl_tr_gc_sync(cx->tr);
    r = rt_proc_resume_frame_h(&slot);
    if (!slot) { rt_pl_eng_ctx_done(hd); r = pl_mk_atom("done"); }
    ok = plw_unify_vals(args[1], r, cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_engine_yield_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t r;
    uint64_t d0, d1;
    if (nargs != 1) return FAILDESCR;
    if (!rt_genp_self_handle()) return FAILDESCR;
    r = rt_pl_deref_val(args[0]);
    rt_pl_tr_gc_sync(cx->tr);
    memcpy(&d0, &r, 8);
    memcpy(&d1, (char *)&r + 8, 8);
    rt_genp_yield(d0, d1);
    rt_pl_tr_gc_sync(cx->tr);
    return pl_ok();
}
DESCR_t rt_pl_dop_engine_destroy_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { (void)cx; if (nargs != 1) return FAILDESCR; rt_pl_eng_destroy(rt_pl_deref_val(args[0])); return pl_ok(); }
PL_CX_LEAF_HEAD(skip_list, 3)
    {
        DESCR_t *c = plw_cell_deref(&args[1]);
        void *ck = (void *)0;
        long n = 0, st = 0, pw = 1;
        while (pl_is_cons(*c) && c->p && c->p != ck) { if (++st == pw) { ck = c->p; pw <<= 1; st = 0; } n++; c = plw_cell_deref(&((DESCR_t *)c->p)[1]); }
        ok = plw_unify_cell_val(&args[0], INTVAL((int64_t)n), cx) && plw_unify_cells(&args[2], c, cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(seek_list, 4)
    {
        DESCR_t cnt = rt_pl_deref_val(args[0]);
        DESCR_t *c = plw_cell_deref(&args[1]);
        int64_t size = cnt.i;
        ok = 0;
        if (cnt.v == DT_I) {
            while (size > 0 && pl_is_cons(*c) && c->p) { c = plw_cell_deref(&((DESCR_t *)c->p)[1]); size--; }
            ok = plw_unify_cells(&args[3], c, cx) && plw_unify_cell_val(&args[2], INTVAL(size), cx);
        }
    }
PL_CX_LEAF_TAIL
static int rt_pl_scc_bnew_cell(DESCR_t *a, pl_tr_ctx_t *cx) { return plw_unify_cell_val(plw_entry(a), INTVAL((int64_t)*pl_tr_probe_b_slot(pl_tr_base_of(cx->tr))), cx); }
static int rt_pl_scc_take_cell(DESCR_t *ab, DESCR_t *ac, pl_tr_ctx_t *cx) {
    char **cell = pl_tr_cleanup_slot(pl_tr_base_of(cx->tr));
    char *rec = *cell;
    if (!rec || *(uint64_t *)(rec + CLEANUP_REC_STATE) != CLEANUP_PENDING) return 0;
    DESCR_t *bc = plw_cell_deref(plw_entry(ab));
    if (bc->v != DT_I) return 0;
    if (bc->i && *(uint64_t *)(rec + CLEANUP_REC_BS) > (uint64_t)bc->i) return 0;
    *cell = *(char **)(rec + CLEANUP_REC_LINK);
    *(uint64_t *)(rec + CLEANUP_REC_STATE) = CLEANUP_DONE;
    return plw_unify_cell_val(plw_entry(ac), *(DESCR_t *)(rec + CLEANUP_REC_C), cx);
}
PL_CX_LEAF_HEAD(scc_bnew, 1) ok = rt_pl_scc_bnew_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(scc_take, 2) ok = rt_pl_scc_take_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
int rt_pl_attv_atts_cell(DESCR_t *av, DESCR_t *aa, pl_tr_ctx_t *cx) {
    DESCR_t *c = plw_cell_deref(plw_entry(av));
    if (!pl_attv_is(c)) return 0;
    return plw_unify_cell_val(plw_entry(aa), c[1], cx);
}
int rt_pl_crv_enter_cell(DESCR_t *am, pl_tr_ctx_t *cx) {
    char *base = pl_tr_base_of(cx->tr);
    (*pl_tr_residue_slot(base))++;
    return plw_unify_cell_val(plw_entry(am), INTVAL((int64_t)(cx->tr - base)), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_undo_push_cell(DESCR_t *ag, pl_tr_ctx_t *cx) {
    extern void rt_pl_undo_goal_copy(DESCR_t *, DESCR_t *, pl_tr_ctx_t *);
    char *base = pl_tr_base_of(cx->tr);
    DESCR_t *list = (DESCR_t *)(base + PL_TR_UNDO_LIST_OFF), g, c;
    rt_pl_undo_goal_copy(plw_cell_deref(plw_entry(ag)), &g, cx);
    c = pl_cons(g, *list);
    pl_tr_push(cx, list);
    *list = c;
    return 1;
}
static int rt_pl_undo_take_cell(DESCR_t *av, pl_tr_ctx_t *cx) {
    char *base = pl_tr_base_of(cx->tr);
    DESCR_t *pend = (DESCR_t *)(base + PL_TR_UNDO_PENDING_OFF), *list = (DESCR_t *)(base + PL_TR_UNDO_LIST_OFF), nil = pl_nil(), head = nil, *tail = &head, l = *pend;
    memset(pend, 0, sizeof *pend);
    for (; l.v == (DTYPE_t)DT_PLREF && !(l.v == list->v && l.p == list->p); l = ((DESCR_t *)l.p)[1]) { *tail = pl_cons(((DESCR_t *)l.p)[0], nil); tail = &((DESCR_t *)tail->p)[1]; }
    return plw_unify_cell_val(plw_entry(av), head, cx);
}
PL_CX_LEAF_HEAD(undo_push, 1) ok = rt_pl_undo_push_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(undo_take, 1) ok = rt_pl_undo_take_cell(&args[0], cx); PL_CX_LEAF_TAIL
static DESCR_t pl_mk_string_dup(const char *s, size_t n) { char *b = (char *)rt_wsb_alloc(n + 1); memcpy(b, s, n); b[n] = 0; return rt_pl_make_string_cell(b); }
static int pl_sa_isnum(DESCR_t d) { return d.v == DT_I || d.v == DT_BIG || d.v == DT_R; }
static size_t pl_sa_cap(DESCR_t v) { DESCR_t d = rt_pl_deref_val(v); return d.v == DT_I || d.v == DT_R ? 64 : 1; }
static const char *pl_sa_text(DESCR_t v, size_t *blen, char *nb, size_t nbn) {
    DESCR_t d = rt_pl_deref_val(v);
    const char *s = pl_atom_str_n(d, blen);
    if (s || !pl_sa_isnum(d) || !pl_swi_dialect()) return s;
    return pl_cell_text_n(d, nb, nbn, &s, blen) ? s : (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sa_int(DESCR_t v, long *out) { DESCR_t d = rt_pl_deref_val(v); if (d.v != DT_I) return 0; *out = (long)d.i; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long pl_sa_next(const pl_sa_t *p, long b, size_t *off) {
    while (b <= p->n && *off + p->sublen <= p->blen) { if (!p->sublen || !memcmp(p->s + *off, p->sub, p->sublen)) return b; *off += utf8_char_bytes(p->s, p->blen, *off, 1); b++; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_sa_plan(DESCR_t *bla, const char *s, size_t blen, pl_sa_t *p, char *sb, size_t sbn) {
    long B = 0, L = 0, A = 0;
    int hb = pl_sa_int(bla[0], &B), hl = pl_sa_int(bla[1], &L), ha = pl_sa_int(bla[2], &A);
    p->s = s;
    p->blen = blen;
    p->n = (long)utf8_strlen_n(s, blen);
    p->count = 0;
    p->k0 = p->k1 = 0;
    p->sub = pl_sa_text(bla[3], &p->sublen, sb, sbn);
    p->subn = p->sub ? (long)utf8_strlen_n(p->sub, p->sublen) : 0;
    if (hb + hl + ha >= 2) { long b = hb ? B : p->n - L - A, l = hl ? L : p->n - B - A; p->mode = 1; p->k0 = b; p->k1 = l; p->count = (b >= 0 && l >= 0 && b + l <= p->n) ? 1 : 0; return; }
    if (p->sub && hb) { p->mode = 1; p->k0 = B; p->k1 = p->subn; p->count = (B >= 0 && B + p->subn <= p->n) ? 1 : 0; return; }
    if (p->sub) {
        size_t off = 0;
        long b = 0, c = 0, hit;
        p->mode = 2;
        while ((hit = pl_sa_next(p, b, &off)) >= 0) { c++; if (off >= p->blen) break; off += utf8_char_bytes(p->s, p->blen, off, 1); b = hit + 1; }
        p->count = c;
        return;
    }
    if (hb) { p->mode = 3; p->k0 = B; p->count = (B >= 0 && B <= p->n) ? p->n - B + 1 : 0; return; }
    if (hl) { p->mode = 4; p->k0 = L; p->count = (L >= 0 && L <= p->n) ? p->n - L + 1 : 0; return; }
    if (ha) { p->mode = 5; p->k0 = A; p->count = (A >= 0 && A <= p->n) ? p->n - A + 1 : 0; return; }
    p->mode = 0;
    p->count = (p->n + 1) * (p->n + 2) / 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sa_pick(const pl_sa_t *p, long idx, long *bo, long *lo) {
    long n = p->n, b = 0, l = 0;
    if (idx < 0 || idx >= p->count) return 0;
    if (p->mode == 1) {
        b = p->k0;
        l = p->k1;
    } else if (p->mode == 2) {
        size_t off = 0;
        long at = 0, hit = -1;
        for (long k = 0; k <= idx; k++) {
            hit = pl_sa_next(p, at, &off);
            if (hit < 0) return 0;
            if (k < idx) { if (off >= p->blen) return 0; off += utf8_char_bytes(p->s, p->blen, off, 1); at = hit + 1; }
        }
        b = hit;
        l = p->subn;
    } else if (p->mode == 3) {
        b = p->k0;
        l = idx;
    } else if (p->mode == 4) {
        b = idx;
        l = p->k0;
    } else if (p->mode == 5) {
        b = idx;
        l = n - p->k0 - idx;
    } else {
        double m = (double)(2 * n + 3);
        b = (long)((m - sqrt(m * m - 8.0 * (double)idx)) / 2.0);
        if (b < 0) b = 0;
        while (b > 0 && b * (2 * n + 3 - b) / 2 > idx) b--;
        while ((b + 1) * (2 * n + 3 - (b + 1)) / 2 <= idx) b++;
        l = idx - b * (2 * n + 3 - b) / 2;
    }
    if (b < 0 || l < 0 || b + l > n) return 0;
    *bo = b;
    *lo = l;
    return 1;
}
DESCR_t rt_pl_dop_sub_atom_n(DESCR_t *args, int nargs) {
    const char *s;
    size_t blen = 0;
    pl_sa_t p;
    if (nargs != 5) return FAILDESCR;
    pl_atoms_ready();
    {
        char nb[pl_sa_cap(args[0])], sb[pl_sa_cap(args[4])];
        s = pl_sa_text(args[0], &blen, nb, sizeof nb);
        if (!s) return FAILDESCR;
        pl_sa_plan(args + 1, s, blen, &p, sb, sizeof sb);
        return INTVAL(p.count - 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_sub_atom_at_cell(DESCR_t *args, pl_tr_ctx_t *cx) {
    size_t blen = 0;
    DESCR_t iv = rt_pl_deref_val(args[1]), sv = rt_pl_deref_val(args[5]);
    char nb[pl_sa_cap(args[0])], sb[pl_sa_cap(args[5])];
    const char *s = pl_sa_text(args[0], &blen, nb, sizeof nb);
    pl_sa_t p;
    long b, l, a;
    size_t boff, bspan;
    if (!s || iv.v != DT_I) return 0;
    pl_sa_plan(args + 2, s, blen, &p, sb, sizeof sb);
    if (!pl_sa_pick(&p, (long)iv.i, &b, &l)) return 0;
    a = p.n - b - l;
    if ((size_t)p.n == p.blen) { boff = (size_t)b; bspan = (size_t)l; } else { boff = utf8_char_offset(s, p.blen, (size_t)b + 1); bspan = utf8_char_bytes(s, p.blen, boff, (size_t)l); }
    {
        char *tr0 = cx->tr;
        int ok = plw_unify_vals(args[2], INTVAL(b), cx) && plw_unify_vals(args[3], INTVAL(l), cx) && plw_unify_vals(args[4], INTVAL(a), cx) &&
            (pl_sa_isnum(sv) && p.sub ? bspan == p.sublen &&
            !memcmp(s + boff, p.sub, bspan) : plw_unify_vals(args[5], sv.v == DT_S ? pl_mk_string_dup(s + boff, bspan) : pl_mk_atom_dup(s + boff, bspan), cx));
        if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
        return ok;
    }
}
PL_CX_LEAF_HEAD(sub_atom_at, 6) ok = rt_pl_sub_atom_at_cell(args, cx); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long pl_atom_concat_count(DESCR_t a, DESCR_t b, DESCR_t c) {
    char ba[8192], bb[8192], bc[8192];
    const char *sa = 0, *sb = 0, *sc = 0;
    size_t la, lb, lc;
    int ha = pl_cell_text_n(a, ba, sizeof ba, &sa, &la), hb = pl_cell_text_n(b, bb, sizeof bb, &sb, &lb);
    if (ha && hb) return 0;
    if (!pl_cell_text_n(c, bc, sizeof bc, &sc, &lc)) return -1;
    if (ha) { long na = (long)utf8_strlen_n(sa, la), nc = (long)utf8_strlen_n(sc, lc); return na <= nc ? na : -1; }
    return (long)utf8_strlen_n(sc, lc);
}
DESCR_t rt_pl_dop_atom_concat_n(DESCR_t *args, int nargs) {
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    { long n = pl_atom_concat_count(args[0], args[1], args[2]); if (n < 0) return FAILDESCR; return INTVAL(n); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_atom_concat_at_cell(DESCR_t *args, pl_tr_ctx_t *cx) {
    char ba[8192], bb[8192], bc[8192];
    const char *sa = 0, *sb = 0, *sc = 0;
    size_t la = 0, lb = 0, blen = 0;
    DESCR_t iv = rt_pl_deref_val(args[0]);
    int ha, hb;
    if (iv.v != DT_I) return 0;
    ha = pl_cell_text_n(args[1], ba, sizeof ba, &sa, &la);
    hb = pl_cell_text_n(args[2], bb, sizeof bb, &sb, &lb);
    if (ha && hb) {
        char *cb;
        if ((long)iv.i != 0) return 0;
        cb = pl_atom_carve_cx(la + lb, cx);
        if (!cb) return 0;
        if (la) memcpy(cb, sa, la);
        if (lb) memcpy(cb + la, sb, lb);
        return plw_unify_vals(args[3], pl_mk_atom_carved(cb, la + lb, la + lb), cx);
    }
    if (!pl_cell_text_n(args[3], bc, sizeof bc, &sc, &blen)) return 0;
    {
        long n = (long)utf8_strlen_n(sc, blen), i = (long)iv.i;
        char *tr0 = cx->tr;
        int ok;
        size_t boff;
        if (i < 0 || i > n) return 0;
        boff = utf8_char_offset(sc, blen, (size_t)i + 1);
        ok = plw_unify_vals(args[1], pl_mk_atom_dup(sc, boff), cx) && plw_unify_vals(args[2], pl_mk_atom_dup(sc + boff, blen - boff), cx);
        if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
        return ok;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(atom_concat_at, 4) ok = rt_pl_atom_concat_at_cell(args, cx); PL_CX_LEAF_TAIL
int pl_db_body_ill_typed(DESCR_t b) {
    extern const char *prolog_atom_name(int);
    for (;;) {
        DESCR_t d = rt_pl_deref_val(b);
        if (d.v == DT_I || d.v == DT_R) return 1;
        if (d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 2) {
            const char *f = prolog_atom_name(plc_fid_name(d.slen));
            if (f && (!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->") || !strcmp(f, "*->") || !strcmp(f, "|"))) {
                DESCR_t *kids = (DESCR_t *)d.p;
                if (pl_db_body_ill_typed(kids[0])) return 1;
                b = kids[1];
                continue;
            }
        }
        if (d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 1) { const char *f = prolog_atom_name(plc_fid_name(d.slen)); if (f && !strcmp(f, "\\+")) { b = ((DESCR_t *)d.p)[0]; continue; } }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_curstream_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern FILE *fh_get(int);
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t d = rt_pl_deref_val(args[0]);
        int swi = pl_swi_dialect();
        if (pl_iso_unbound(d)) return (void *)0;
        if (d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == prolog_atom_intern("$stream") && plc_fid_arity(d.slen) == 1) {
            DESCR_t k = rt_pl_deref_val(((DESCR_t *)d.p)[0]);
            if (k.v == DT_I && k.i >= 0 && k.i < FH_N && pl_sp_stream_live((int)k.i)) return (void *)0;
            if (swi) return rt_pl_ball_kind2("existence_error", "stream", d);
        }
        if (swi && d.v == (DTYPE_t)DT_PLATOM) {
            const char *nm = pl_atom_str(d);
            int ix = !strcmp(nm, "user_input") ? 0 : !strcmp(nm, "user_output") ? 1 : !strcmp(nm, "user_error") ? 2 : pl_alias_idx(nm);
            return (ix >= 0 && fh_get(ix)) ? (void *)0 : rt_pl_ball_kind2("existence_error", "stream", d);
        }
        return rt_pl_ball_kind2("domain_error", swi ? "stream_or_alias" : "stream", d);
    }
}
static int pl_ctv_find(pl_ctv_t *vt, void *d) {
    unsigned h = (unsigned)(((uintptr_t)d >> 4) * 2654435761u) & (unsigned)vt->hmask;
    while (vt->hs[h] >= 0) { if (vt->addr[vt->hs[h]] == d) return vt->hs[h]; h = (h + 1) & (unsigned)vt->hmask; }
    return -(int)h - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_tree_name_n(const char *s, size_t n) { char *q = (char *)ct_alloc(n + 1); if (!q) return (char *)""; if (n) memcpy(q, s, n); q[n] = 0; return q; }
char * pl_tree_name(const char *s) { if (!s) s = ""; return pl_tree_name_n(s, strlen(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cell_tree(DESCR_t *c, pl_ctv_t *vt) {
    extern const char *prolog_atom_name(int);
    DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)c);
    if (pl_cell_unbound(d)) {
        int fi = pl_ctv_find(vt, (void *)d);
        if (fi >= 0) { tree_t *v = ast_node_new(TT_VAR); v->v.sval = vt->nm[fi]; return v; }
        {
            char b[24];
            tree_t *v = ast_node_new(TT_VAR);
            snprintf(b, sizeof b, "_A%d", vt->n);
            if (vt->n < vt->cap) { vt->hs[-fi - 1] = vt->n; vt->addr[vt->n] = (void *)d; vt->nm[vt->n] = pl_tree_name(b); v->v.sval = vt->nm[vt->n]; vt->n++; } else v->v.sval = pl_tree_name(b);
            return v;
        }
    }
    if ((int)d->v == DT_I) { tree_t *t = ast_node_new(TT_ILIT); t->v.ival = d->i; return t; }
    if ((int)d->v == DT_R) { tree_t *t = ast_node_new(TT_FLIT); t->v.dval = d->r; return t; }
    if (pl_rat_is(*d)) {
        size_t n = pl_rat_text(*d, (char *)0, 0) + 1;
        char rb[n];
        tree_t *t = ast_node_new(TT_FNC), *q = ast_node_new(TT_QLIT);
        (void)pl_rat_text(*d, rb, n);
        t->v.sval = pl_tree_name("$pl_big");
        q->v.sval = pl_tree_name(rb);
        ast_push(t, q);
        return t;
    }
    if (rt_pl_eng_inst_is(*d)) {
        tree_t *t = ast_node_new(TT_FNC), *q = ast_node_new(TT_ILIT);
        t->v.sval = pl_tree_name("$pl_engine");
        q->v.ival = rt_pl_eng_inst_id(*d);
        ast_push(t, q);
        rt_pl_eng_pin(*d);
        return t;
    }
    if ((int)d->v == DT_BIG) {
        extern char *rt_big_str(DESCR_t);
        char *bs = rt_big_str(*d);
        tree_t *t = ast_node_new(TT_FNC), *q = ast_node_new(TT_QLIT);
        t->v.sval = pl_tree_name("$pl_big");
        q->v.sval = pl_tree_name(bs ? bs : "0");
        ast_push(t, q);
        return t;
    }
    if ((int)d->v == DT_PLATOM) {
        tree_t *t = ast_node_new(TT_QLIT);
        const char *nm = prolog_atom_name((int)d->i);
        size_t n = nm ? prolog_atom_len((int)d->i) : 0;
        t->v.sval = pl_tree_name_n(nm ? nm : "", n);
        if (memchr(t->v.sval, 0, n)) t->slen = (int)n;
        return t;
    }
    if ((int)d->v == DT_S) { tree_t *t = ast_node_new(TT_DQLIT), *q = ast_node_new(TT_QLIT); q->v.sval = pl_tree_name(d->s ? d->s : ""); ast_push(t, q); return t; }
    if ((int)d->v == DT_SNUL) { tree_t *t = ast_node_new(TT_QLIT); t->v.sval = pl_tree_name(""); return t; }
    if ((int)d->v == DT_PLREF) {
        int fn = plc_functor((pl_cell_t *)d), ar = pl_arity((pl_cell_t *)d);
        const char *nm = prolog_atom_name(fn);
        DESCR_t *aa = (DESCR_t *)d->p;
        if (nm && !strcmp(nm, ".") && ar == 2) {
            tree_t *lst = ast_node_new(TT_MAKELIST);
            DESCR_t *cur = d;
            unsigned long guard = 0;
            for (;;) {
                DESCR_t *cd = (DESCR_t *)pl_deref((pl_cell_t *)cur);
                if (++guard == (1UL << 28)) { extern void rt_bomb(const char *msg); rt_bomb("pl_cell_tree: a list of 2^28 cells is a cycle, which this reader cannot represent"); }
                if ((int)cd->v == DT_PLREF && pl_arity((pl_cell_t *)cd) == 2 && plc_functor((pl_cell_t *)cd) == fn) {
                    DESCR_t *kk = (DESCR_t *)cd->p;
                    ast_push(lst, pl_cell_tree(&kk[0], vt));
                    cur = &kk[1];
                    continue;
                }
                {
                    const char *tn = ((int)cd->v == DT_PLATOM) ? prolog_atom_name((int)cd->i) : (const char *)0;
                    if (tn && !strcmp(tn, "[]")) { lst->v.ival = 0; return lst; }
                    ast_push(lst, pl_cell_tree(cur, vt));
                    lst->v.ival = 1;
                    return lst;
                }
            }
            return lst;
        }
        {
            tree_t *root = ast_node_new(TT_FNC);
            tree_t *t = root;
            t->v.sval = pl_tree_name(nm ? nm : "?");
            for (;;) {
                DESCR_t *ld;
                const char *ln;
                for (int i = 0; i + 1 < ar; i++) ast_push(t, pl_cell_tree(&aa[i], vt));
                if (ar < 1) return root;
                ld = (DESCR_t *)pl_deref((pl_cell_t *)&aa[ar - 1]);
                ln = (int)ld->v == DT_PLREF && !pl_rat_is(*ld) ? prolog_atom_name(plc_functor((pl_cell_t *)ld)) : (const char *)0;
                if (!ln || (!strcmp(ln, ".") && pl_arity((pl_cell_t *)ld) == 2)) { ast_push(t, pl_cell_tree(&aa[ar - 1], vt)); return root; }
                ar = pl_arity((pl_cell_t *)ld);
                aa = (DESCR_t *)ld->p;
                { tree_t *n = ast_node_new(TT_FNC); n->v.sval = pl_tree_name(ln); ast_push(t, n); t = n; }
            }
        }
    }
    { tree_t *t = ast_node_new(TT_QLIT); t->v.sval = pl_tree_name("?"); return t; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rt_pl_clause_tree(void *clause_cell) {
    extern int rt_pl_var_occ(void *);
    int occ = rt_pl_var_occ(clause_cell) + 1;
    void *ab[occ];
    char *nb[occ];
    pl_ctv_t vt;
    int hn = 16;
    while (hn < 2 * occ) hn <<= 1;
    int hs[hn];
    for (int i = 0; i < hn; i++) hs[i] = -1;
    vt.addr = ab;
    vt.nm = nb;
    vt.n = 0;
    vt.cap = occ;
    vt.hs = hs;
    vt.hmask = hn - 1;
    return pl_cell_tree((DESCR_t *)clause_cell, &vt);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_choice_new(const char *key) { tree_t *c = ast_node_new(TT_CHOICE); c->v.sval = pl_tree_name(key ? key : "?"); return (void *)c; }
