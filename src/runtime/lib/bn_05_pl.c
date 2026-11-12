#include "runtime/lib/bn_head.h"
#include <sys/stat.h>
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "runtime/rt_pl_reader.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_06_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_29_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
#include "runtime/lib/bn_33_pl.h"
#include "runtime/lib/bn_34_pl.h"
extern void *rt_ws_alloc_descr(size_t);
int rt_pl_keysort_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_compare_cell(void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_atop_cell(int, void *, void *);
void rt_pl_writeq_cell(void *);
void rt_pl_write_canonical_cell(void *);
int rt_pl_char_type_cell(void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_copy_term_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_copy_term_share_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_copy_keep_cell(void *, void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_factorize_term_cell(void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_copy_term_nat_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_numbervars1_cell(void *, pl_tr_ctx_t *);
int rt_pl_sort_cell(int, void *, void *, pl_tr_ctx_t *);
extern int rt_plco_mark(char *tr);
extern int rt_plco_b0(char *b);
extern int rt_plco_inside(void);
extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
extern void *rt_genp_self_handle(void);
extern int rt_pl_eng_is(DESCR_t);
extern int64_t rt_pl_eng_id(DESCR_t);
extern int rt_pl_eng_self(void *, DESCR_t *);
extern int rt_pl_eng_count(void);
extern DESCR_t rt_pl_eng_at(int);
extern int rt_pl_eng_reaped_count(void);
extern int64_t rt_pl_eng_reaped_take(int);
extern void rt_pl_eng_reaped_clear(void);
extern int64_t rt_pl_eng_stat(int);
extern void rt_gc_request(void);
void rt_pl_residue_recopy(DESCR_t *el, int n, pl_tr_ctx_t *cx);
    static int pl_cur_alias(DESCR_t a, int cur);
extern FILE *fh_get(int);
extern void * rt_pl_db_get(void *, int64_t);
extern int rt_pl_db_assert(void *, void *, void *, int);
extern int rt_pl_db_erase(void *, void *, int);
extern int rt_pl_db_abolish(void *, void *);
extern char * pl_tr_base_of(const char *);
extern int rt_pl_tbl_dl(void *, int, void *, pl_tr_ctx_t *);
extern int rt_pl_tx_db(void *, int, void *);
extern int rt_pl_mod_ext(void *, void *, void *, void *, void *, pl_tr_ctx_t *);
extern int rt_pl_db_live_count(void *);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
extern int rt_pl_db_match_erase(void *, void *, void *);
extern int rt_pl_db_killed(void *);
extern void *rt_pl_db_get_by_key(void *, const char *, int);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
extern void rt_pl_db_unqual_term(void *);
int plw_bind_vv(DESCR_t *A, DESCR_t *B, pl_tr_ctx_t *cx, const char *floor_) {
    int as = (const char *)A > floor_, bs = (const char *)B > floor_;
    DESCR_t *from = as && bs ? (A < B ? A : B) : as ? A : bs ? B : (A > B ? A : B);
    DESCR_t *to = from == A ? B : A;
    DESCR_t r = {0};
    r.v = (DTYPE_t)DT_PLVAR;
    r.slen = 0;
    r.p = (void *)to;
    if (to->v != (DTYPE_t)DT_PLVAR || to->p != (void *)to) { if (pl_tr_needs_log_bind(cx, to, floor_)) pl_tr_push(cx, to); pl_var_make(to, cx, 0); }
    if (pl_tr_needs_log_bind(cx, from, floor_)) pl_tr_push(cx, from);
    *from = r;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) int plw_unify_vals(DESCR_t va, DESCR_t vb, pl_tr_ctx_t *cx) { DESCR_t ta = va, tb = vb; return plw_unify_cells(plw_entry(&ta), plw_entry(&tb), cx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_det_cell(DESCR_t *tmp) {
    { extern void prolog_atom_init(void); if (g_runtime.atoms.atom_dot < 0) prolog_atom_init(); }
    DESCR_t *c = plw_cell_deref(plw_entry(tmp));
    if (c->v == DT_SNUL || c->v == DT_FAIL) pl_var_make(c, (const pl_tr_ctx_t *)0, 0);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_deref_val(DESCR_t v) { DESCR_t t = v; return *plw_cell_deref(plw_entry(&t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_goal_key(DESCR_t goal, int extra, DESCR_t **args_out, int *ar_out) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(goal);
    const char *nm = (const char *)0;
    int ar = 0;
    DESCR_t *kids = (DESCR_t *)0;
    if ((int)v.v == DT_PLREF) { nm = prolog_atom_name(plc_fid_name(v.slen)); ar = plc_fid_arity(v.slen); kids = (DESCR_t *)v.p; } else if ((int)v.v == (int)DT_PLATOM) nm = prolog_atom_name((int)v.i);
    else if ((int)v.v == (int)DT_S) nm = v.s;
    if (!nm || extra < 0) return (const char *)0;
    if ((int)v.v == DT_PLREF && extra == 0 && ar == 2 && plc_fid_name(v.slen) == g_runtime.atoms.atom_dot) { kids = (DESCR_t *)rt_ws_alloc_descr(1); kids[0] = v; nm = "consult"; ar = 1; }
    *args_out = kids;
    *ar_out = ar;
    return nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *pl_atom_str(DESCR_t v) {
    if (v.v == DT_S) return v.s ? v.s : "";
    if (v.v == (DTYPE_t)DT_PLATOM) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm ? nm : ""; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pl_mk_atom(const char *s) { extern int prolog_atom_intern(const char *); DESCR_t d = {0}; d.v = DT_PLATOM; d.slen = 0; d.i = prolog_atom_intern(s); return d; }
DESCR_t pl_nil(void) { DESCR_t d = {0}; d.v = DT_PLATOM; d.slen = 0; d.i = g_runtime.atoms.atom_nil; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pl_cons(DESCR_t head, DESCR_t tail) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(2);
    kids[0] = head;
    kids[1] = tail;
    DESCR_t c = {0};
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)g_runtime.atoms.functor_dot2;
    c.p = (void *)kids;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pl_list_from_arr(DESCR_t *elems, int n) { DESCR_t acc = pl_nil(); for (int i = n - 1; i >= 0; i--) acc = pl_cons(elems[i], acc); return acc; }
DESCR_t pl_ok(void) { DESCR_t r = {0}; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
void pl_atoms_ready(void) { extern void prolog_atom_init(void); if (g_runtime.atoms.atom_dot < 0) prolog_atom_init(); }
int pl_is_cons(DESCR_t d) { return d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == g_runtime.atoms.atom_dot && plc_fid_arity(d.slen) == 2; }
DESCR_t pl_spine_end(DESCR_t cur, long *n_out, int *cyclic) {
    void *ck = (void *)0;
    long n = 0, st = 0, pw = 1;
    *cyclic = 0;
    while (pl_is_cons(cur) && cur.p) { if (cur.p == ck) { *cyclic = 1; break; } if (++st == pw) { ck = cur.p; pw <<= 1; st = 0; } n++; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
    if (n_out) *n_out = n;
    return cur;
}
int pl_val_unbound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_cell_text(DESCR_t v, char *buf, size_t n, const char **out) { size_t len; return pl_cell_text_n(v, buf, n, out, &len); }
PL_TYPE_LEAF(ground) PL_TYPE_LEAF(is_list) DESCR_t rt_pl_type_cold_c(DESCR_t *args, int nargs, const char *kind) {
    pl_atoms_ready();
    return (nargs == 1 && kind && rt_pl_type_test_cell(&args[0], kind)) ? pl_ok() : FAILDESCR;
}
DESCR_t dop_pl_writeq(DESCR_t *args, int nargs) { if (nargs != 1) return FAILDESCR; rt_pl_writeq_cell(&args[0]); return pl_ok(); }
DESCR_t dop_pl_write_canonical(DESCR_t *args, int nargs) { if (nargs != 1) return FAILDESCR; rt_pl_write_canonical_cell(&args[0]); return pl_ok(); }
DESCR_t dop_pl_flush_output(DESCR_t *args, int nargs) { extern FILE *fh_cur_out_fp(void); (void)args; (void)nargs; fflush(fh_cur_out_fp()); return pl_ok(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_stream_idx_ball(DESCR_t s, int out, int textop, pl_tr_ctx_t *cx) { void *b = (void *)0; int idx = pl_stream_resolve(s, out, textop, &b); if (b) cx->ball = b; return idx; }
DESCR_t rt_pl_dop_engine_self_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t d = {0};
    if (nargs != 1 || !rt_pl_eng_self(rt_genp_self_handle(), &d)) return FAILDESCR;
    return plw_unify_vals(args[0], d, cx) ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_engine_id_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t hd;
    if (nargs != 2) return FAILDESCR;
    hd = rt_pl_deref_val(args[0]);
    if (!rt_pl_eng_is(hd)) return FAILDESCR;
    return plw_unify_vals(args[1], INTVAL(rt_pl_eng_id(hd)), cx) ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_engine_list_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t l = pl_nil();
    if (nargs != 1) return FAILDESCR;
    for (int i = rt_pl_eng_count() - 1; i >= 0; i--) l = pl_cons(rt_pl_eng_at(i), l);
    return plw_unify_vals(args[0], l, cx) ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_engine_stats_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    if (nargs != 4) return FAILDESCR;
    for (int i = 0; i < 4; i++) if (!plw_unify_vals(args[i], INTVAL(rt_pl_eng_stat(i)), cx)) return FAILDESCR;
    return pl_ok();
}
DESCR_t rt_pl_dop_gc_now_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { (void)args; (void)cx; if (nargs != 0) return FAILDESCR; rt_gc_request(); return pl_ok(); }
DESCR_t rt_pl_dop_engine_reaped_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t l = pl_nil();
    if (nargs != 1) return FAILDESCR;
    for (int i = rt_pl_eng_reaped_count() - 1; i >= 0; i--) l = pl_cons(INTVAL(rt_pl_eng_reaped_take(i)), l);
    rt_pl_eng_reaped_clear();
    return plw_unify_vals(args[0], l, cx) ? pl_ok() : FAILDESCR;
}
PL_CX_LEAF_HEAD(co_mark, 0) ok = rt_plco_mark(cx->tr); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(co_b0, 0) ok = rt_plco_b0(cx->b); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(co_inside, 0) ok = rt_plco_inside(); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(compare, 3) ok = rt_pl_compare_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(functor, 3) ok = rt_pl_functor_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(arg, 3) ok = rt_pl_arg_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(univ, 2) ok = rt_pl_univ_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(copy_term, 2) ok = rt_pl_copy_term_share_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(copy_keep, 4) ok = rt_pl_copy_keep_cell(&args[0], &args[1], &args[2], &args[3], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(factorize_term, 3) ok = rt_pl_factorize_term_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(copy_term_nat, 2) ok = rt_pl_copy_term_nat_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(numbervars1, 1) ok = rt_pl_numbervars1_cell(&args[0], cx); PL_CX_LEAF_TAIL
static DESCR_t pl_attv_word(DESCR_t *a, pl_tr_ctx_t *cx) {
    char probe;
    char *floor_ = &probe;
    DESCR_t *c = plw_cell_deref(plw_entry(a));
    DESCR_t r = {0};
    if (!plw_unbound_tag(c)) return *c;
    r.v = (DTYPE_t)DT_PLVAR;
    if ((char *)c > floor_) { DESCR_t *j = (DESCR_t *)rt_ws_alloc_descr(1); pl_var_move(j, c, cx); r.p = (void *)j; plw_bind(c, r, cx); return r; }
    r.p = (void *)c;
    return r;
}
static int rt_pl_attv_set_cell(DESCR_t *av, DESCR_t *aa, pl_tr_ctx_t *cx) {
    char probe;
    char *floor_ = &probe;
    DESCR_t w = pl_attv_word(aa, cx);
    DESCR_t *c = plw_cell_deref(plw_entry(av));
    int rs = pl_tr_residue_on(cx);
    if (pl_attv_is(c)) { if (rs) pl_tr_push_residue(cx, c + 1); else if (pl_tr_needs_log(cx, c + 1, floor_)) pl_tr_push(cx, c + 1); c[1] = w; return 1; }
    if (!plw_unbound_tag(c)) return 0;
    {
        DESCR_t *blk = (DESCR_t *)rt_ws_alloc_descr(2);
        DESCR_t r = {0};
        pl_var_make(blk, cx, 1);
        if (rs) { memset(&blk[1], 0, sizeof blk[1]); pl_tr_push_residue(cx, blk + 1); }
        blk[1] = w;
        r.v = (DTYPE_t)DT_PLVAR;
        r.p = (void *)blk;
        plw_bind(c, r, cx);
    }
    return 1;
}
static int rt_pl_attv_take_cell(DESCR_t *al, pl_tr_ctx_t *cx) {
    extern int prolog_atom_intern(const char *);
    extern int prolog_functor_intern(int, int);
    char *base = pl_tr_base_of(cx->tr);
    uint64_t *wk = pl_tr_wake_slot(base);
    uint64_t lo = *wk;
    int fid = prolog_functor_intern(prolog_atom_intern("wakeup"), 3);
    DESCR_t acc = pl_nil();
    *wk = 0;
    if (lo) for (char *p = cx->tr - PL_TR_ENTRY_BYTES; p >= base + lo; p -= PL_TR_ENTRY_BYTES) {
        pl_tr_entry_t *e = (pl_tr_entry_t *)p;
        if (e->old.v == (DTYPE_t)DT_PLVAR && (e->old.src_node0 & 1) && e->old.p == (void *)e->cell) {
            DESCR_t *k = (DESCR_t *)rt_ws_alloc_descr(3);
            DESCR_t t = {0};
            k[0] = e->cell[1];
            k[1].v = (DTYPE_t)DT_PLVAR;
            k[1].slen = 0;
            k[1].p = (void *)e->cell;
            k[2] = acc;
            t.v = (DTYPE_t)DT_PLREF;
            t.slen = (uint32_t)fid;
            t.p = (void *)k;
            acc = t;
        }
    }
    if (lo && !cx->b && base + lo < cx->tr) { cx->tr = base + lo; *(char **)base = cx->tr; }
    return plw_unify_cell_val(plw_entry(al), acc, cx);
}
static int rt_pl_crv_adj_cell(DESCR_t *ad, pl_tr_ctx_t *cx) {
    DESCR_t *d = plw_cell_deref(plw_entry(ad));
    uint64_t *rs = pl_tr_residue_slot(pl_tr_base_of(cx->tr));
    if (d->v != DT_I) return 0;
    if (d->i < 0 && *rs < (uint64_t)-d->i) {
        extern void rt_bomb(const char *msg);
        rt_bomb("$crv_adj: the call_residue_vars depth would fall below zero -- a goal left it more times than it entered");
    }
    *rs += (uint64_t)d->i;
    return 1;
}
static int rt_pl_crv_collect_cell(DESCR_t *am, DESCR_t *av, pl_tr_ctx_t *cx) {
    char *base = pl_tr_base_of(cx->tr);
    DESCR_t *m = plw_cell_deref(plw_entry(am));
    DESCR_t nil = pl_nil(), head = nil, *tail = &head;
    if (m->v != DT_I || m->i < PL_TR_HEADER_BYTES || base + m->i > cx->tr) return 0;
    for (char *p = base + m->i; p + PL_TR_ENTRY_BYTES <= cx->tr; p += PL_TR_ENTRY_BYTES) {
        pl_tr_entry_t *e = (pl_tr_entry_t *)p;
        DESCR_t *blk = e->cell - 1, r = {0};
        if (e->pad != PL_TR_PAD_RESIDUE || !pl_attv_is(blk) || (blk[1].v == nil.v && blk[1].i == nil.i)) continue;
        blk[0].src_node0 &= (uint8_t)~1;
        r.v = (DTYPE_t)DT_PLVAR;
        r.p = (void *)blk;
        *tail = pl_cons(r, nil);
        tail = &((DESCR_t *)tail->p)[1];
    }
    for (DESCR_t l = head; l.v == (DTYPE_t)DT_PLREF; l = ((DESCR_t *)l.p)[1]) ((DESCR_t *)((DESCR_t *)l.p)[0].p)->src_node0 |= 1;
    return plw_unify_cell_val(plw_entry(av), head, cx);
}
DESCR_t pl_unif_global(DESCR_t *c, const char *floor_, pl_tr_ctx_t *cx) {
    DESCR_t r = {0};
    r.v = (DTYPE_t)DT_PLVAR;
    c = plw_cell_deref(c);
    if (plw_unbound_tag(c) && (const char *)c > floor_) { DESCR_t *j = (DESCR_t *)rt_ws_alloc_descr(1); pl_var_move(j, c, cx); r.p = (void *)j; plw_bind(c, r, cx); return r; }
    if (plw_unbound_tag(c)) { r.p = (void *)c; return r; }
    return *c;
}
PL_CX_LEAF_HEAD(attv_set, 2) ok = rt_pl_attv_set_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(unifiable, 3) ok = rt_pl_unifiable_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(attv_atts, 2) ok = rt_pl_attv_atts_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(attv_take, 1) ok = rt_pl_attv_take_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(crv_enter, 1) ok = rt_pl_crv_enter_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(crv_adj, 1) ok = rt_pl_crv_adj_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(crv_collect, 2) ok = rt_pl_crv_collect_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
static int rt_pl_setarg_cell(DESCR_t *na, DESCR_t *ta, DESCR_t *va, pl_tr_ctx_t *cx, int trail) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    char probe;
    char *floor_ = &probe;
    DESCR_t *n = plw_cell_deref(plw_entry(na)), *t = plw_cell_deref(plw_entry(ta));
    if (plw_unbound_tag(n) || plw_unbound_tag(t)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
    if (n->v != (DTYPE_t)DT_I) { cx->ball = rt_pl_ball_kind2("type_error", "integer", *n); return 0; }
    if (t->v != (DTYPE_t)DT_PLREF || !t->p) { cx->ball = rt_pl_ball_kind2("type_error", "compound", *t); return 0; }
    if (n->i < 0) { cx->ball = rt_pl_ball_kind2("domain_error", "not_less_than_zero", *n); return 0; }
    if (n->i < 1 || n->i > (int64_t)plc_fid_arity(t->slen)) return 0;
    {
        DESCR_t *cell = &((DESCR_t *)t->p)[n->i - 1];
        DESCR_t w;
        if (trail != 0) w = pl_attv_word(va, cx);
        else {
            DESCR_t *j = (DESCR_t *)rt_ws_alloc_descr(1), *d;
            pl_var_make(j, cx, 0);
            if (!rt_pl_copy_term_cell(va, j, cx)) return 0;
            d = plw_cell_deref(j);
            if (plw_unbound_tag(d)) { w.v = (DTYPE_t)DT_PLVAR; w.slen = 0; w.p = (void *)d; } else w = *d;
        }
        if (trail == 1 && pl_tr_needs_log_bind(cx, cell, floor_)) pl_tr_push(cx, cell);
        *cell = w;
        if (trail != 1 && (w.v == (DTYPE_t)DT_PLREF || w.v == (DTYPE_t)DT_PLVAR)) rt_pl_tr_freeze();
    }
    return 1;
}
PL_CX_LEAF_HEAD(setarg, 3) ok = rt_pl_setarg_cell(&args[0], &args[1], &args[2], cx, 1); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(nb_setarg, 3) ok = rt_pl_setarg_cell(&args[0], &args[1], &args[2], cx, 0); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(nb_linkarg, 3) ok = rt_pl_setarg_cell(&args[0], &args[1], &args[2], cx, 2); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(wall_us, 1) ok = rt_pl_wall_clock_cell(0, &args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(wall_ms, 1) ok = rt_pl_wall_clock_cell(1, &args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(cpu_ms, 1) ok = rt_pl_wall_clock_cell(2, &args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(succ, 2) ok = rt_pl_succ_plus_cell(2, &args[0], &args[1], (void *)0, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(plus, 3) ok = rt_pl_succ_plus_cell(3, &args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
static int pl_is_pair(DESCR_t d) { extern int prolog_atom_intern(const char *); return d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == prolog_atom_intern("-") && plc_fid_arity(d.slen) == 2; }
static void *pl_sort_list_ball(DESCR_t orig, int pairs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    DESCR_t cur = orig, e;
    int cyc;
    cur = pl_spine_end(orig, (long *)0, &cyc);
    if (cyc) return rt_pl_ball_kind2("type_error", "list", orig);
    if (pl_val_unbound(cur)) return rt_pl_ball_instantiation();
    if (!pl_is_nil(cur)) return rt_pl_ball_kind2("type_error", "list", orig);
    for (cur = orig; pairs && pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (pl_val_unbound(e)) return rt_pl_ball_instantiation();
        if (!pl_is_pair(e)) return rt_pl_ball_kind2("type_error", "pair", e);
    }
    return (void *)0;
}
static void *pl_sort_args_ball(DESCR_t *args, int pairs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_dop_list_guard_c(DESCR_t *, int);
    DESCR_t cur, e;
    void *b;
    if ((b = pl_sort_list_ball(rt_pl_deref_val(args[0]), pairs))) return b;
    if ((b = rt_pl_dop_list_guard_c(&args[1], 1))) return b;
    for (cur = rt_pl_deref_val(args[1]); pairs && pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (!pl_val_unbound(e) && !pl_is_pair(e)) return rt_pl_ball_kind2("type_error", "pair", e);
    }
    return (void *)0;
}
static DESCR_t pl_sort_cycle_cells(DESCR_t orig) {
    extern int rt_pl_dialect_swi(void);
    DESCR_t cur = rt_pl_deref_val(orig), acc = pl_nil();
    long n = 0;
    int cyc;
    if (!rt_pl_dialect_swi()) return orig;
    (void)pl_spine_end(cur, &n, &cyc);
    if (!cyc) return orig;
    for (long i = 0; i < n; i++) { acc = pl_cons(((DESCR_t *)cur.p)[0], acc); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
    return acc;
}
PL_CX_LEAF_HEAD(sort, 2)
    { DESCR_t a2[2] = { pl_sort_cycle_cells(args[0]), args[1] }; void *b = pl_sort_args_ball(a2, 0); if (b) { cx->ball = b; ok = 0; } else ok = rt_pl_sort_cell(0, &a2[0], &args[1], cx); }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(msort, 2) { void *b = pl_sort_args_ball(args, 0); if (b) { cx->ball = b; ok = 0; } else ok = rt_pl_sort_cell(1, &args[0], &args[1], cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(char_type, 2)
    {
        extern void *rt_pl_char_type_ball(void *, void *);
        void *b = rt_pl_char_type_ball(&args[0], &args[1]);
        if (b) { cx->ball = b; ok = 0; } else ok = rt_pl_char_type_cell(&args[0], &args[1], (void *)0, cx);
    }
PL_CX_LEAF_TAIL
static DESCR_t pl_fa_handle(void *a) { DESCR_t d; memset(&d, 0, sizeof d); d.v = (DTYPE_t)DT_DATA; d.slen = DATA_ELEMS_SLEN; d.ptr = a; return d; }
static void *pl_fa_of(DESCR_t h) { return (h.v == DT_DATA && h.slen == DATA_ELEMS_SLEN) ? h.ptr : (void *)0; }
DESCR_t rt_pl_dop_findall_new(DESCR_t *args, int nargs) {
    (void)args;
    if (nargs != 0) return FAILDESCR;
    pl_atoms_ready();
    { extern void *rt_pl_findall_begin(void); void *a = rt_pl_findall_begin(); if (!a) return FAILDESCR; return pl_fa_handle(a); }
}
DESCR_t rt_pl_dop_findall_add(DESCR_t *args, int nargs) {
    if (nargs != 2) return FAILDESCR;
    {
        extern void rt_pl_findall_collect(void *, void *);
        DESCR_t h = rt_pl_deref_val(args[0]);
        if (!pl_fa_of(h)) return FAILDESCR;
        rt_pl_findall_collect(pl_fa_of(h), (void *)&args[1]);
        return pl_ok();
    }
}
static int rt_pl_all_solutions_cell(DESCR_t *args, pl_tr_ctx_t *cx, int mode) {
    extern int rt_pl_findall_count(void *);
    extern void rt_pl_findall_item(void *, int, void *);
    extern int rt_pl_sort_cell(int, void *, void *, pl_tr_ctx_t *);
    DESCR_t h = rt_pl_deref_val(args[0]);
    if (!pl_fa_of(h)) return 0;
    {
        void *acc = pl_fa_of(h);
        int n = rt_pl_findall_count(acc);
        int i;
        DESCR_t lst;
        if (mode != 0 && n == 0) return 0;
        DESCR_t el[n > 0 ? n : 1];
        for (i = 0; i < n; i++) rt_pl_findall_item(acc, i, (void *)&el[i]);
        rt_pl_residue_recopy(el, n, cx);
        lst = pl_list_from_arr(el, n);
        if (mode == 2) return rt_pl_sort_cell(0, (void *)&lst, (void *)&args[1], cx);
        return plw_unify_vals(args[1], lst, cx);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_all_solutions_tail_cell(DESCR_t *args, pl_tr_ctx_t *cx) {
    extern int rt_pl_findall_count(void *);
    extern void rt_pl_findall_item(void *, int, void *);
    DESCR_t h = rt_pl_deref_val(args[0]);
    if (!pl_fa_of(h)) return 0;
    {
        void *acc = pl_fa_of(h);
        int n = rt_pl_findall_count(acc);
        int i;
        DESCR_t *tv = (DESCR_t *)rt_ws_alloc_descr(1);
        DESCR_t tref = {0}, lst;
        DESCR_t el[n > 0 ? n : 1];
        pl_var_make(tv, cx, 0);
        tref = pl_make_ref(tv, 0);
        lst = tref;
        for (i = 0; i < n; i++) rt_pl_findall_item(acc, i, (void *)&el[i]);
        rt_pl_residue_recopy(el, n, cx);
        for (i = n - 1; i >= 0; i--) lst = pl_cons(el[i], lst);
        return plw_unify_vals(args[1], lst, cx) && plw_unify_vals(tref, args[2], cx);
    }
}
PL_CX_LEAF_HEAD(findall_result4, 3) ok = rt_pl_all_solutions_tail_cell(args, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(findall_result, 2) ok = rt_pl_all_solutions_cell(args, cx, 0); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(bagof_result, 2) ok = rt_pl_all_solutions_cell(args, cx, 1); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(setof_result, 2) ok = rt_pl_all_solutions_cell(args, cx, 2); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pair_parts(DESCR_t it, DESCR_t *w, DESCR_t *t) {
    DESCR_t d = rt_pl_deref_val(it);
    if (d.v != (DTYPE_t)DT_PLREF || plc_fid_arity(d.slen) != 2) return 0;
    { DESCR_t *aa = (DESCR_t *)d.p; *w = aa[0]; *t = aa[1]; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_bagof_groups(void *acc, int n, DESCR_t *it, int *ord, int *gs, int *used, int *gi) {
    extern void rt_pl_findall_item(void *, int, void *);
    extern int rt_pl_variant_cells(void *, void *);
    extern int rt_pl_atop_cell(int, void *, void *);
    int i, j, ng = 0, k = 0;
    if (n <= 0) return 0;
    for (i = 0; i < n; i++) { rt_pl_findall_item(acc, i, (void *)&it[i]); used[i] = 0; }
    for (i = 0; i < n; i++) {
        DESCR_t wi, ti;
        if (used[i] || !pl_pair_parts(it[i], &wi, &ti)) continue;
        gs[ng++] = k;
        ord[k++] = i;
        used[i] = 1;
        for (j = i + 1; j < n; j++) { DESCR_t wj, tj; if (used[j] || !pl_pair_parts(it[j], &wj, &tj)) continue; if (rt_pl_variant_cells((void *)&wi, (void *)&wj)) { ord[k++] = j; used[j] = 1; } }
    }
    gs[ng] = k;
    for (i = 0; i < ng; i++) gi[i] = i;
    for (i = 0; i < ng; i++) {
        int best = i;
        for (j = i + 1; j < ng; j++) {
            DESCR_t wb, tb, wj, tj;
            if (!pl_pair_parts(it[ord[gs[gi[best]]]], &wb, &tb) || !pl_pair_parts(it[ord[gs[gi[j]]]], &wj, &tj)) continue;
            if (rt_pl_atop_cell(0, (void *)&wj, (void *)&wb)) best = j;
        }
        if (best != i) { int tmp = gi[i]; gi[i] = gi[best]; gi[best] = tmp; }
    }
    return ng;
}
DESCR_t rt_pl_dop_bagof_group_n(DESCR_t *args, int nargs) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    {
        extern int rt_pl_findall_count(void *);
        DESCR_t h = rt_pl_deref_val(args[0]);
        int n, ng;
        if (!pl_fa_of(h)) return FAILDESCR;
        n = rt_pl_findall_count(pl_fa_of(h));
        if (n <= 0) return FAILDESCR;
        { DESCR_t it[n]; int ord[n], gs[n + 1], used[n], gi[n]; ng = pl_bagof_groups(pl_fa_of(h), n, it, ord, gs, used, gi); }
        if (ng <= 0) return FAILDESCR;
        return INTVAL(ng - 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_bagof_group_at_cell(DESCR_t *args, pl_tr_ctx_t *cx, int sorted) {
    extern int rt_pl_sort_cell(int, void *, void *, pl_tr_ctx_t *);
    extern int rt_pl_findall_count(void *);
    DESCR_t h = rt_pl_deref_val(args[0]), iv = rt_pl_deref_val(args[1]);
    int ng, idx, i, cnt, g, n;
    DESCR_t wrep, trep, lst;
    char *tr0 = cx->tr;
    int ok;
    if (!pl_fa_of(h) || iv.v != DT_I) return 0;
    n = rt_pl_findall_count(pl_fa_of(h));
    if (n <= 0) return 0;
    DESCR_t it[n];
    int ord[n], gs[n + 1], used[n], gi[n];
    ng = pl_bagof_groups(pl_fa_of(h), n, it, ord, gs, used, gi);
    idx = (int)iv.i;
    if (ng <= 0 || idx < 0 || idx >= ng) return 0;
    g = gi[idx];
    cnt = gs[g + 1] - gs[g];
    if (cnt <= 0) return 0;
    DESCR_t el[cnt];
    if (!pl_pair_parts(it[ord[gs[g]]], &wrep, &trep)) return 0;
    ok = 1;
    for (i = 0; i < cnt && ok; i++) { DESCR_t w2, t2; if (!pl_pair_parts(it[ord[gs[g] + i]], &w2, &t2)) { ok = 0; break; } el[i] = t2; ok = plw_unify_vals(args[2], w2, cx); }
    lst = pl_list_from_arr(el, cnt);
    if (ok) ok = sorted ? rt_pl_sort_cell(0, (void *)&lst, (void *)&args[3], cx) : plw_unify_vals(args[3], lst, cx);
    (void)wrep;
    if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
    return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(bagof_group_at, 4) ok = rt_pl_bagof_group_at_cell(args, cx, 0); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(setof_group_at, 4) ok = rt_pl_bagof_group_at_cell(args, cx, 1); PL_CX_LEAF_TAIL
PL_ATOM_OP_LEAF(atom_length, 2) PL_ATOM_OP_LEAF(atom_concat, 3) PL_ATOM_OP_LEAF(atomic_concat, 3) PL_ATOM_OP_LEAF(atom_chars, 2) PL_ATOM_OP_LEAF(atom_codes, 2) PL_ATOM_OP_LEAF(atom_number, 2)
    PL_ATOM_OP_LEAF(atom_string, 2) PL_ATOM_OP_LEAF(upcase_atom, 2) PL_ATOM_OP_LEAF(downcase_atom, 2) PL_ATOM_OP_LEAF(string_concat, 3) PL_ATOM_OP_LEAF(string_length, 2)
    PL_ATOM_OP_LEAF(string_lower, 2) PL_ATOM_OP_LEAF(string_upper, 2) PL_ATOM_OP_LEAF(string_to_atom, 2) PL_ATOM_OP_LEAF(number_string, 2) PL_ATOM_OP_LEAF(string_codes, 2)
DESCR_t rt_pl_dop_concat_atom_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    int ok;
    if (nargs != 2 && nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    ok = rt_pl_atom_op_cell("concat_atom", &args[0], &args[1], nargs == 3 ? (void *)&args[2] : (void *)0, cx);
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(char_code, 2)
    {
        char b[64];
        const char *s;
        int adv;
        DESCR_t a = rt_pl_deref_val(args[0]), c = rt_pl_deref_val(args[1]);
        if (pl_atom_str(a) && pl_cell_text(a, b, sizeof b, &s) && s[0]) ok = plw_unify_vals(args[1], INTVAL((long long)rt_pl_u8_get(s, &adv)), cx);
        else if (c.v == DT_I) { char c2[8]; int bl = rt_pl_u8_put(c2, (int)c.i); c2[bl] = 0; ok = plw_unify_vals(args[0], pl_mk_atom_dup(c2, (size_t)bl), cx); } else ok = 0;
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_number_text_leaf(DESCR_t *args, int codes, pl_tr_ctx_t *cx) {
    char b[4096];
    const char *s;
    DESCR_t a = rt_pl_deref_val(args[0]);
    {
        DESCR_t num, l = rt_pl_deref_val(args[1]);
        const char *t = l.v == DT_S ? pl_skip_layout(l.s ? l.s : "") : pl_list_text_heap(args[1]);
        if (l.v == DT_S && pl_swi_string_zero_quote(t)) return plw_unify_vals(args[0], INTVAL(0), cx);
        if (t && pl_parse_number(t, &num)) return plw_unify_vals(args[0], num, cx);
    }
    if (a.v == DT_I || a.v == DT_R || a.v == DT_BIG) { if (!pl_cell_text(a, b, sizeof b, &s)) return 0; return plw_unify_vals(args[1], pl_text_list(s, codes), cx); }
    return 0;
}
PL_CX_LEAF_HEAD(number_codes, 2) ok = pl_number_text_leaf(args, 1, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(number_chars, 2) ok = pl_number_text_leaf(args, 0, cx); PL_CX_LEAF_TAIL
int pl_read_term_text(char **bp, size_t *np) { int r = pl_read_term_text_raw(bp, np); if (r > 0 && pl_swi_dialect() && pl_in_is_text() && !pl_text_fffd(bp, np)) return -1; return r; }
PL_READ_TERM_LEAF(read_term_from_atom, 1) PL_READ_TERM_LEAF(read_term_from_chars, 0) PL_READ_TERM_LEAF(read_term_from_codes, 0)
static void *pl_ball_sora(void *ball, DESCR_t sora) {
    extern const char *prolog_atom_name(int);
    DESCR_t *e = (DESCR_t *)ball, f, w;
    if (!ball || !pl_gnu_dialect() || e->v != (DTYPE_t)DT_PLREF || plc_fid_arity(e->slen) != 2) return ball;
    f = rt_pl_deref_val(((DESCR_t *)e->p)[0]);
    if (f.v != (DTYPE_t)DT_PLREF || plc_fid_arity(f.slen) != 3 || strcmp(prolog_atom_name(plc_fid_name(f.slen)), "permission_error")) return ball;
    w = rt_pl_deref_val(((DESCR_t *)f.p)[2]);
    if (w.v == (DTYPE_t)DT_PLATOM && !strcmp(prolog_atom_name((int)w.i), "current_input_stream")) ((DESCR_t *)f.p)[2] = rt_pl_deref_val(sora);
    return ball;
}
PL_IN_LEAF(read, 2) PL_IN_LEAF(read_term_opts, 3)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_out_ioerr(int idx, DESCR_t s, pl_tr_ctx_t *cx) {
    extern FILE *fh_get(int);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    FILE *fp = idx >= 3 ? fh_get(idx) : (FILE *)0;
    if (!fp || !ferror(fp)) return 1;
    clearerr(fp);
    if (!cx->ball) cx->ball = rt_pl_ball_kind2("io_error", "write", rt_pl_deref_val(s));
    return 0;
}
PL_OUT_BALL_LEAF(write, dop_write, 2) PL_OUT_BALL_LEAF(writeq, dop_pl_writeq, 2) PL_OUT_BALL_LEAF(write_canonical, dop_pl_write_canonical, 2) PL_OUT_BALL_LEAF(writeln, dop_pl_writeln, 2)
    PL_OUT_BALL_LEAF(nl, dop_nl, 1) PL_OUT_BALL_LEAF(tab, dop_pl_tab, 2) PL_OUT_BALL_LEAF(put_char, dop_pl_put_char, 2) PL_OUT_BALL_LEAF(put_code, dop_pl_put_code, 2)
    PL_OUT_BALL_LEAF(flush_output, dop_pl_flush_output, 1)
PL_OUT_CX_LEAF(put_byte, 2)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(put_char_c, 1)
    {
        extern FILE *fh_cur_out_fp(void);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t v = rt_pl_deref_val(args[0]);
        char b[64];
        const char *t;
        ok = 0;
        if (pl_val_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (!pl_cell_text(args[0], b, sizeof b, &t) || !t || !t[0]) cx->ball = rt_pl_ball_kind2("type_error", "character", v);
        else { fputs(t, fh_cur_out_fp()); ok = 1; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_OUT_CX_LEAF(put_char_c, 2)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_OUT_CX_LEAF(write_term, 3)
PL_CX_LEAF_HEAD(display, 1) { extern void *rt_pl_display_cell_ball(void *); void *b = rt_pl_display_cell_ball(&args[0]); if (b) { cx->ball = b; ok = 0; } else ok = 1; } PL_CX_LEAF_TAIL
PL_OUT_CX_LEAF(display, 2)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(aggregate_reduce, 3)
    {
        extern int gt(DESCR_t, DESCR_t);
        extern int lt(DESCR_t, DESCR_t);
        DESCR_t kd = rt_pl_deref_val(args[0]);
        const char *kind = pl_atom_str(kd);
        DESCR_t lst = rt_pl_deref_val(args[1]);
        ok = 0;
        if (!kind) {
            ok = 0;
        } else if (!strcmp(kind, "bag")) {
            ok = plw_unify_vals(args[2], lst, cx);
        } else if (!strcmp(kind, "count")) {
            long long n = 0;
            DESCR_t c = lst;
            while (pl_is_cons(c)) { n++; c = rt_pl_deref_val(((DESCR_t *)c.p)[1]); }
            ok = plw_unify_vals(args[2], INTVAL(n), cx);
        } else if (!strcmp(kind, "sum")) {
            long long isum = 0;
            double dsum = 0;
            int isreal = 0;
            DESCR_t c = lst;
            while (pl_is_cons(c)) {
                DESCR_t e = rt_pl_deref_val(((DESCR_t *)c.p)[0]);
                if (e.v == DT_R) { if (!isreal) { dsum = (double)isum; isreal = 1; } dsum += e.r; } else if (e.v == DT_I) { if (isreal) dsum += (double)e.i; else isum += e.i; }
                c = rt_pl_deref_val(((DESCR_t *)c.p)[1]);
            }
            ok = plw_unify_vals(args[2], isreal ? REALVAL(dsum) : INTVAL(isum), cx);
        } else if (!strcmp(kind, "max") || !strcmp(kind, "min")) {
            int ismax = !strcmp(kind, "max");
            DESCR_t c = lst;
            DESCR_t best = INTVAL(0);
            int have = 0;
            while (pl_is_cons(c)) {
                DESCR_t e = rt_pl_deref_val(((DESCR_t *)c.p)[0]);
                if (!have || (ismax ? gt(e, best) : lt(e, best))) { best = e; have = 1; }
                c = rt_pl_deref_val(((DESCR_t *)c.p)[1]);
            }
            if (have) ok = plw_unify_vals(args[2], best, cx);
        }
    }
PL_CX_LEAF_TAIL
int rt_pl_flag_at_read(const char *nm, int swi, const char **aval, long *ver) {
    pl_flag_t *fl = nm ? pl_flag_find(nm) : (pl_flag_t *)0;
    *aval = (const char *)0;
    if (!fl) return 0;
    if (!strcmp(fl->nm, "version_data")) { *aval = swi ? "swi" : "scrip"; ver[0] = swi ? 9 : 0; ver[1] = 0; ver[2] = swi ? 4 : 0; return 2; }
    if (!strcmp(fl->nm, "dialect") && swi) { *aval = "swi"; return 1; }
    if (!strcmp(fl->nm, "debug") && swi) { *aval = strcmp(fl->val, "on") ? "false" : "true"; return 1; }
    if (fl->is_term || !strcmp(fl->nm, "argv") || !strcmp(fl->nm, "os_argv") || !fl->val || !fl->val[0]) return 3;
    *aval = fl->val;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_flag_opt_true(const char *on) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kk = (DESCR_t *)rt_ws_alloc_descr(1), c = {0};
    kk[0] = pl_mk_atom("true");
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(on), 1);
    c.p = (void *)kk;
    return c;
}
DESCR_t pl_flag_term_value(const pl_flag_t *fl) {
    if (fl && fl->is_term && fl->val && fl->val[0]) return fl->term;
    return pl_cons(pl_flag_opt_true("portray"), pl_cons(pl_flag_opt_true("numbervars"), pl_nil()));
}
PL_IN_LEAF(get_char, 2) PL_IN_LEAF(peek_char, 2) PL_IN_LEAF(get_code, 2) PL_IN_LEAF(peek_code, 2) PL_IN_LEAF(get_byte, 2) PL_IN_LEAF(peek_byte, 2) PL_IN_LEAF(unget_char, 2) PL_IN_LEAF(unget_code, 2)
PL_IN_LEAF(unget_byte, 2) PL_CX_LEAF_HEAD(current_output, 1)
    { extern int fh_current_output(void); int al = pl_cur_alias(args[0], fh_current_output()); ok = al >= 0 ? al : plw_unify_vals(args[0], pl_mk_stream(fh_current_output()), cx); }
PL_CX_LEAF_TAIL
static int pl_cur_alias(DESCR_t a, int cur) {
    const char *nm;
    int ix;
    a = rt_pl_deref_val(a);
    if (a.v != (DTYPE_t)DT_PLATOM || !(nm = pl_atom_str(a))) return -1;
    ix = !strcmp(nm, "user_input") ? 0 : !strcmp(nm, "user_output") ? 1 : !strcmp(nm, "user_error") ? 2 : pl_alias_idx(nm);
    return ix < 0 ? -1 : ix == cur;
}
PL_CX_LEAF_HEAD(current_input, 1)
    { extern int fh_current_input(void); int al = pl_cur_alias(args[0], fh_current_input()); ok = al >= 0 ? al : plw_unify_vals(args[0], pl_mk_stream(fh_current_input()), cx); }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(open, 3) ok = pl_open_leaf(args, 3, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(open4, 4) ok = pl_open_leaf(args, 4, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(keysort, 2) { void *b = pl_sort_args_ball(args, 1); if (b) { cx->ball = b; ok = 0; } else ok = rt_pl_keysort_cell(&args[0], &args[1], cx); } PL_CX_LEAF_TAIL
PL_GNU_SORT1_LEAF(gnu_sort1, 0) PL_GNU_SORT1_LEAF(gnu_msort1, 1) PL_GNU_SORT1_LEAF(gnu_keysort1, 2)
PL_CX_LEAF_HEAD(gnu_line_count, 2) { long c, l, p; ok = pl_gnu_position(args[0], &c, &l, &p, cx, "line_count/2") && plw_unify_vals(args[1], INTVAL(l), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_line_position, 2) { long c, l, p; ok = pl_gnu_position(args[0], &c, &l, &p, cx, "line_position/2") && plw_unify_vals(args[1], INTVAL(p), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_character_count, 2) { long c, l, p; ok = pl_gnu_position(args[0], &c, &l, &p, cx, "character_count/2") && plw_unify_vals(args[1], INTVAL(c), cx); } PL_CX_LEAF_TAIL
const char *pl_gnu_path_arg(DESCR_t a, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    DESCR_t f = rt_pl_deref_val(a);
    const char *nm = pl_atom_str(f), *ab;
    if (pl_val_unbound(f)) { cx->ball = rt_pl_ball_instantiation(); return (const char *)0; }
    if (!nm) { cx->ball = rt_pl_ball_kind2("type_error", "atom", f); return (const char *)0; }
    if (!(ab = pl_gnu_abs_path(nm))) { cx->ball = rt_pl_ball_kind2("domain_error", "os_path", f); return (const char *)0; }
    return ab;
}
int pl_gnu_os_fail(pl_tr_ctx_t *cx) { extern void *rt_pl_ball_kind1(const char *, const char *); cx->ball = rt_pl_ball_kind1("system_error", strerror(errno)); return 0; }
void pl_gnu_ctx(pl_tr_ctx_t *cx, const char *nm, int ar) { extern void *rt_pl_ball_set_pi(void *, const char *, int); if (cx->ball) cx->ball = rt_pl_ball_set_pi(cx->ball, nm, ar); }
PL_CX_LEAF_HEAD(gnu_working_directory, 1)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t d = rt_pl_deref_val(args[0]);
        char *w;
        ok = 0;
        if (!pl_val_unbound(d) && !pl_atom_str(d)) cx->ball = rt_pl_ball_kind2("type_error", "atom", d);
        else if (!(w = pl_gnu_cwd())) ok = pl_gnu_os_fail(cx);
        else ok = plw_unify_vals(args[0], pl_mk_atom_dup(w, strlen(w)), cx);
        pl_gnu_ctx(cx, "working_directory", 1);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_change_directory, 1)
    { const char *p = pl_gnu_path_arg(args[0], cx); ok = 0; if (p) ok = chdir(p) == 0 ? 1 : pl_gnu_os_fail(cx); pl_gnu_ctx(cx, "change_directory", 1); }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_make_directory, 1)
    { const char *p = pl_gnu_path_arg(args[0], cx); ok = 0; if (p) ok = mkdir(p, 0777) == 0 ? 1 : pl_gnu_os_fail(cx); pl_gnu_ctx(cx, "make_directory", 1); }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_delete_file, 1) { const char *p = pl_gnu_path_arg(args[0], cx); ok = 0; if (p) ok = unlink(p) == 0 ? 1 : pl_gnu_os_fail(cx); pl_gnu_ctx(cx, "delete_file", 2); } PL_CX_LEAF_TAIL
DESCR_t pl_ct_a(DESCR_t t, int i) { return rt_pl_deref_val(((DESCR_t *)t.p)[i]); }
int pl_os_errv(DESCR_t r, pl_tr_ctx_t *cx) { return plw_unify_vals(r, pl_os_err_of(errno), cx); }
int pl_os_okv(DESCR_t r, pl_tr_ctx_t *cx) { return plw_unify_vals(r, pl_mk_atom("ok"), cx); }
PL_CX_LEAF_HEAD(sk_listen, 3)
    { int fd = pl_sk_fd(args[0]); DESCR_t n = rt_pl_deref_val(args[1]); ok = (fd >= 0 && listen(fd, n.v == (DTYPE_t)DT_I ? (int)n.i : 5) == 0) ? pl_os_okv(args[2], cx) : pl_os_errv(args[2], cx); }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_pc_strv(DESCR_t t, const char **v, int n) { for (int i = 0; i < n; i++) v[i] = pl_atom_str(pl_ct_a(t, i)); v[n] = (const char *)0; }
static int pl_pc_spec(DESCR_t c, int k, int *pp, int *dst, char *ty, int *own) {
    extern int pipe2(int *, int);
    extern FILE *fh_get(int);
    const char *a = pl_atom_str(c);
    int f[2];
    *ty = 't';
    *own = 0;
    pp[0] = pp[1] = -1;
    *dst = -1;
    if (a && !strcmp(a, "std")) return 0;
    if (a && !strcmp(a, "null")) { *own = 1; *dst = open("/dev/null", (k ? O_WRONLY : O_RDONLY) | O_CLOEXEC); return *dst < 0 ? -1 : 0; }
    if (pl_ct_n(c) == 1 && plc_fid_name(c.slen) == prolog_atom_intern("pipe")) {
        const char *t = pl_atom_str(pl_ct_a(c, 0));
        if (t && !strcmp(t, "binary")) *ty = 'b';
        if (pipe2(f, O_CLOEXEC) != 0) return -1;
        pp[0] = f[0];
        pp[1] = f[1];
        *dst = k ? f[1] : f[0];
        return 0;
    }
    if (pl_ct_n(c) == 1 && plc_fid_name(c.slen) == prolog_atom_intern("fd")) {
        void *b = (void *)0;
        int si = pl_stream_resolve(pl_ct_a(c, 0), k ? 1 : 0, 0, &b);
        FILE *fp = si >= 0 ? fh_get(si) : (FILE *)0;
        if (!fp) { errno = EBADF; return -1; }
        fflush(fp);
        *dst = fileno(fp);
        return 0;
    }
    return 0;
}
PL_CX_LEAF_HEAD(proc_create, 7)
    {
        extern int fh_alloc(FILE *);
        DESCR_t io = rt_pl_deref_val(args[2]), av = rt_pl_deref_val(args[1]), ev = rt_pl_deref_val(args[4]), cwd = rt_pl_deref_val(args[3]), dt = rt_pl_deref_val(args[5]);
        const char *path = pl_atom_str(rt_pl_deref_val(args[0])), *cw = pl_atom_str(cwd);
        int na = pl_ct_n(av), pp[3][2], dst[3], own[3], e = 0, rep = pl_ct_n(ev) == 1 && plc_fid_name(ev.slen) == prolog_atom_intern("replace"),
            det = pl_atom_str(dt) && !strcmp(pl_atom_str(dt), "true");
        DESCR_t eterm = pl_ct_n(ev) == 1 ? pl_ct_a(ev, 0) : FAILDESCR;
        int ne = IS_FAIL_fn(eterm) ? 0 : pl_ct_n(eterm);
        const char *argv[na + 2], *envv[ne + 1];
        char ty[3];
        pid_t pid = -1;
        DESCR_t r[4];
        argv[0] = path;
        pl_pc_strv(av, argv + 1, na);
        if (ne) pl_pc_strv(eterm, envv, ne);
        else envv[0] = (const char *)0;
        errno = 0;
        for (int k = 0; k < 3; k++) { pp[k][0] = pp[k][1] = dst[k] = -1; own[k] = 0; ty[k] = 't'; }
        for (int k = 0; k < 3; k++) if (!e && pl_pc_spec(pl_ct_n(io) == 3 ? pl_ct_a(io, k) : pl_mk_atom("std"), k, pp[k], &dst[k], &ty[k], &own[k]) != 0) e = errno ? errno : EINVAL;
        if (!e && path) {
            fflush((FILE *)0);
            pid = fork();
            if (pid == 0) {
                if (det) setsid();
                for (int k = 0; k < 3; k++) if (dst[k] >= 0 && dst[k] != k) dup2(dst[k], k);
                if (cw && *cw && chdir(cw) != 0) _exit(127);
                if (rep) execve(path, (char *const *)argv, (char *const *)envv);
                for (int i = 0; i < ne; i++) putenv((char *)envv[i]);
                g_runtime.scrip_env = 0;
                execv(path, (char *const *)argv);
                _exit(127);
            }
            if (pid < 0) e = errno;
        }
        for (int k = 0; k < 3; k++) {
            int mine = k ? pp[k][0] : pp[k][1], theirs = k ? pp[k][1] : pp[k][0];
            if (theirs >= 0) close(theirs);
            else if (own[k] && dst[k] >= 0) close(dst[k]);
            r[k + 1] = pl_mk_atom("[]");
            if (mine >= 0 && e) close(mine);
            else if (mine >= 0) { int si = fh_alloc(fdopen(mine, k ? "r" : "w")); g_fh[si].mode = k ? 'r' : 'w'; g_fh[si].type = ty[k]; r[k + 1] = pl_mk_stream(si); }
        }
        if (e || !path) { errno = e ? e : ENOENT; ok = pl_os_errv(args[6], cx); } else { r[0] = INTVAL((long long)pid); ok = plw_unify_vals(args[6], pl_gnu_fn("pr", 4, r), cx); }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(proc_kill, 3)
    {
        DESCR_t p = rt_pl_deref_val(args[0]), s = rt_pl_deref_val(args[1]);
        ok = (p.v == (DTYPE_t)DT_I && s.v == (DTYPE_t)DT_I && kill((pid_t)p.i, (int)s.i) == 0) ? pl_os_okv(args[2], cx) : pl_os_errv(args[2], cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(fx_symlink, 3)
    {
        const char *x = pl_atom_str(rt_pl_deref_val(args[0])), *y = pl_atom_str(rt_pl_deref_val(args[1]));
        ok = x && y && (symlink(x, y) == 0 ? pl_os_okv(args[2], cx) : plw_unify_vals(args[2], pl_fx_err(), cx));
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(fx_link, 3)
    {
        const char *x = pl_atom_str(rt_pl_deref_val(args[0])), *y = pl_atom_str(rt_pl_deref_val(args[1]));
        ok = x && y && (link(x, y) == 0 ? pl_os_okv(args[2], cx) : plw_unify_vals(args[2], pl_fx_err(), cx));
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(fx_chmod, 3)
    {
        const char *x = pl_atom_str(rt_pl_deref_val(args[0]));
        DESCR_t m = rt_pl_deref_val(args[1]);
        ok = x && m.v == (DTYPE_t)DT_I && (chmod(x, (mode_t)(m.i & 07777)) == 0 ? pl_os_okv(args[2], cx) : plw_unify_vals(args[2], pl_fx_err(), cx));
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_file_exists, 1) { const char *p = pl_gnu_path_arg(args[0], cx); ok = p && access(p, F_OK) == 0; pl_gnu_ctx(cx, "file_exists", 1); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(set_stream_position, 2)
    {
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
        extern FILE *fh_get(int);
        extern int prolog_atom_intern(const char *);
        extern int fh_repos(int);
        DESCR_t sd = rt_pl_deref_val(args[0]);
        DESCR_t pd = rt_pl_deref_val(args[1]);
        const char *nm;
        int si = -1;
        ok = 0;
        if (pl_val_unbound(sd)) { cx->ball = rt_pl_ball_instantiation(); return FAILDESCR; }
        nm = pl_atom_str(sd);
        if (nm) {
            si = pl_sp_named(sd);
            if (si < 0 || !fh_get(si)) { cx->ball = rt_pl_ball_kind2("existence_error", "stream", sd); return FAILDESCR; }
        } else if (sd.v == (DTYPE_t)DT_PLREF && plc_fid_name(sd.slen) == prolog_atom_intern("$stream") && plc_fid_arity(sd.slen) == 1) {
            DESCR_t a = rt_pl_deref_val(((DESCR_t *)sd.p)[0]);
            if (a.v != DT_I || (int)a.i < 0 || (int)a.i >= FH_N || !fh_get((int)a.i)) { cx->ball = rt_pl_ball_kind2("existence_error", "stream", sd); return FAILDESCR; }
            si = (int)a.i;
        } else {
            cx->ball = rt_pl_ball_kind2("domain_error", "stream_or_alias", sd);
            return FAILDESCR;
        }
        if (pl_val_unbound(pd)) { cx->ball = rt_pl_ball_instantiation(); return FAILDESCR; }
        if (!(pd.v == (DTYPE_t)DT_PLREF && plc_fid_name(pd.slen) == prolog_atom_intern("$stream_position") && (plc_fid_arity(pd.slen) == 1 || (plc_fid_arity(pd.slen) == 4 && pl_gnu_dialect())))) {
            cx->ball = rt_pl_ball_kind2("domain_error", "stream_position", pd);
            return FAILDESCR;
        }
        {
            DESCR_t off = rt_pl_deref_val(((DESCR_t *)pd.p)[0]);
            if (off.v != DT_I) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_position", pd); return FAILDESCR; }
            if (!fh_repos(si)) { cx->ball = rt_pl_ball_permission3("reposition", "stream", sd); return FAILDESCR; }
            { FILE *fp = fh_get(si); if (!fp || fseek(fp, (long)off.i, SEEK_SET) != 0) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_position", pd); return FAILDESCR; } }
            ok = 1;
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_op_count, 1) { extern int prolog_op_table_count(void); ok = plw_unify_vals(args[0], INTVAL((long long)prolog_op_table_count()), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_op_nth, 4)
    {
        extern int prolog_op_table_get(int, const char **, int *, const char **);
        extern int prolog_op_col_get(int, int, int *, int *);
        const char *onm = (const char *)0;
        const char *oty = (const char *)0;
        int opr = 0;
        DESCR_t iv = rt_pl_deref_val(args[0]);
        ok = 0;
        if (iv.v == DT_I && prolog_op_table_get((int)iv.i - 1, &onm, &opr, &oty) && onm && oty &&
            (!pl_gnu_dialect() || prolog_op_col_get(prolog_atom_intern(onm), oty[0] == 'f' ? 0 : oty[2] ? 1 : 2, 0, 0))) ok = plw_unify_vals(args[1], INTVAL((long long)opr), cx) &&
            plw_unify_vals(args[2], pl_mk_atom_dup(oty, strlen(oty)), cx) && plw_unify_vals(args[3], pl_mk_atom_dup(onm, strlen(onm)), cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_cs_count, 1) { ok = plw_unify_vals(args[0], INTVAL((long long)FH_N), cx); } PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * pl_db_cell_of(DESCR_t *args, void *root) { DESCR_t k = rt_pl_deref_val(args[0]); if (k.v != DT_I) return (void *)0; return rt_pl_db_get(root, k.i); }
static DESCR_t pl_tbl_dl_leaf(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root, int set) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { int ok = rt_pl_tbl_dl(root, set, (void *)&args[0], cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_mod_ext_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    if (nargs != 4) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { int ok = rt_pl_mod_ext(root, (void *)&args[0], (void *)&args[1], (void *)&args[2], (void *)&args[3], cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_tx_db_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    DESCR_t op;
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    op = rt_pl_deref_val(args[0]);
    { int ok = op.v == DT_I && rt_pl_tx_db(root, (int)op.i, (void *)&args[1]); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_tbl_dlget_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_tbl_dl_leaf(args, nargs, cx, root, 0); }
DESCR_t rt_pl_dop_tbl_dlset_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_tbl_dl_leaf(args, nargs, cx, root, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_erase_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    {
        extern int rt_pl_db_slot_erased(void *, int);
        void *db = pl_db_cell_of(args, root);
        DESCR_t iv = rt_pl_deref_val(args[1]);
        if (!db || iv.v != DT_I) return FAILDESCR;
        return (rt_pl_db_erase(root, db, (int)iv.i) || rt_pl_db_slot_erased(db, (int)iv.i)) ? pl_ok() : FAILDESCR;
    }
}
DESCR_t rt_pl_dop_ball_pending_c(void *ball) { return ball ? FAILDESCR : pl_ok(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_nonempty_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 1) return FAILDESCR;
    { void *db = pl_db_cell_of(args, root); return (db && rt_pl_db_live_count(db) > 0) ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_abolish_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 1) return FAILDESCR;
    { void *db = pl_db_cell_of(args, root); if (!db) return pl_ok(); return rt_pl_db_abolish(root, db) ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_pair_parts(DESCR_t *p, DESCR_t *h, DESCR_t *b) {
    DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)p);
    if ((int)d->v != DT_PLREF || pl_arity((pl_cell_t *)d) != 2) return 0;
    { DESCR_t *aa = (DESCR_t *)d->p; if (h) *h = aa[0]; if (b) *b = aa[1]; return 1; }
}
DESCR_t rt_pl_dop_db_retractall_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { extern void rt_pl_db_define(void *); void *db = pl_db_cell_of(args, root); if (!db) return pl_ok(); rt_pl_db_define(db); rt_pl_db_match_erase(root, db, (void *)&args[1]); return pl_ok(); }
}
PL_CX_LEAF_HEAD(pl_cp_guard, 1)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern int prolog_atom_intern(const char *);
        DESCR_t s = rt_pl_deref_val(args[0]);
        ok = 1;
        if (!pl_iso_unbound(s)) {
            int good = 0;
            if (s.v == (DTYPE_t)DT_PLREF && plc_fid_arity(s.slen) == 2 && plc_fid_name(s.slen) == prolog_atom_intern("/")) {
                DESCR_t n = rt_pl_deref_val(((DESCR_t *)s.p)[0]);
                DESCR_t a = rt_pl_deref_val(((DESCR_t *)s.p)[1]);
                good = (pl_iso_unbound(n) || pl_anum_is_text(n)) && (pl_iso_unbound(a) || (a.v == DT_I && (long long)a.i >= 0));
            }
            if (!good && pl_swi_dialect() && s.v == (DTYPE_t)DT_PLREF && plc_fid_arity(s.slen) == 2 && plc_fid_name(s.slen) == prolog_atom_intern("/") &&
                pl_atom_str(rt_pl_deref_val(((DESCR_t *)s.p)[0])) && rt_pl_deref_val(((DESCR_t *)s.p)[1]).v == DT_I && (long long)rt_pl_deref_val(((DESCR_t *)s.p)[1]).i < 0) {
                cx->ball = rt_pl_ball_kind2("domain_error", "not_less_than_zero", rt_pl_deref_val(((DESCR_t *)s.p)[1]));
                ok = 0;
            } else if (!good) {
                cx->ball = rt_pl_ball_kind2("type_error", "predicate_indicator", s);
                ok = 0;
            }
        }
    }
PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_halt_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    pl_atoms_ready();
    if (nargs == 0) { fflush((FILE *)0); exit(0); }
    {
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (pl_iso_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (v.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", v);
        else { fflush((FILE *)0); exit((int)v.i); }
        rt_pl_tr_gc_sync(cx->tr);
        return FAILDESCR;
    }
}
int pl_cell_is_clref(DESCR_t t) {
    extern int prolog_atom_intern(const char *);
    return t.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t.slen) == 2 && plc_fid_name(t.slen) == prolog_atom_intern("$clref");
}
static void *pl_db_of_clref(DESCR_t r, void *root, int *ref_out) {
    extern void *rt_pl_db_get_by_key(void *, const char *, int);
    DESCR_t k;
    DESCR_t n;
    const char *ks;
    if (!pl_cell_is_clref(r)) return (void *)0;
    k = rt_pl_deref_val(((DESCR_t *)r.p)[0]);
    n = rt_pl_deref_val(((DESCR_t *)r.p)[1]);
    ks = pl_atom_str(k);
    if (!ks || n.v != DT_I) return (void *)0;
    if (ref_out) *ref_out = (int)n.i;
    return rt_pl_db_get_by_key(root, ks, 0);
}
PL_CX_LEAF_HEAD(db_clause_src, 3)
    {
        extern int rt_pl_db_src_of_ref(void *, int, int *, int *);
        int ref = 0, file = -1, line = 0;
        void *db = pl_db_of_clref(rt_pl_deref_val(args[0]), (void *)(uintptr_t)cx->r14, &ref);
        ok = db && rt_pl_db_src_of_ref(db, ref, &file, &line) && file >= 0 && plw_unify_vals(args[1], pl_make_atom(file), cx) && plw_unify_vals(args[2], INTVAL((long long)line), cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(db_unload_file, 1)
    { extern int rt_pl_db_unload_src(void *, int); DESCR_t fv = rt_pl_deref_val(args[0]); ok = (int)fv.v == DT_PLATOM; if (ok) rt_pl_db_unload_src((void *)(uintptr_t)cx->r14, (int)fv.i); }
PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_db_erase_ref_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_slot_of_ref(void *, int);
    extern int rt_pl_db_erase(void *, void *, int);
    int ref = 0;
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    {
        void *db = pl_db_of_clref(rt_pl_deref_val(args[0]), root, &ref);
        if (!db) return FAILDESCR;
        { int i = rt_pl_db_slot_of_ref(db, ref); if (i < 0) return FAILDESCR; return rt_pl_db_erase(root, db, i) ? pl_ok() : FAILDESCR; }
    }
}
static void *pl_db_of_term(DESCR_t *t, void *root, int create) {
    char key[bnd_db_key_need((void *)t)];
    int ar = 0;
    if (!rt_pl_db_term_key((void *)t, key, sizeof key, &ar)) return (void *)0;
    return rt_pl_db_get_by_key(root, key, create);
}
static DESCR_t pl_db_add_t(DESCR_t *args, int nargs, void *root, int prepend) {
    if (nargs != (prepend ? 1 : 3)) return FAILDESCR;
    pl_atoms_ready();
    DESCR_t ua[3] = { args[0], prepend ? args[0] : args[1], prepend ? args[0] : args[2] };
    args = ua;
    rt_pl_db_unqual_term((void *)&args[0]);
    {
        void *db = pl_db_of_term(&args[0], root, 1);
        if (!db) return FAILDESCR;
        return (prepend ? rt_pl_db_assert(root, db, (void *)&args[0], 1) : pl_seed_src(db, &args[1], rt_pl_db_assert(root, db, (void *)&args[0], 0))) ? pl_ok() : FAILDESCR;
    }
}
DESCR_t rt_pl_dop_db_assertz_t_c(DESCR_t *args, int nargs, void *root) { return pl_db_add_t(args, nargs, root, 0); }
DESCR_t rt_pl_dop_db_asserta_t_c(DESCR_t *args, int nargs, void *root) { return pl_db_add_t(args, nargs, root, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_consult_done_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_materialize_all(void *, int);
    DESCR_t w = nargs == 1 ? rt_pl_deref_val(args[0]) : FAILDESCR;
    if (w.v != DT_I) return FAILDESCR;
    rt_pl_db_materialize_all(root, (int)w.i);
    return pl_ok();
}
DESCR_t rt_pl_dop_db_erase_t_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { void *db = pl_db_of_term(&args[0], root, 0); DESCR_t iv = rt_pl_deref_val(args[1]); if (!db || iv.v != DT_I) return FAILDESCR; return rt_pl_db_erase(root, db, (int)iv.i) ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_db_retractall_t_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_db_unqual_term((void *)&args[0]);
    {
        extern void rt_pl_db_define(void *);
        void *db = pl_db_of_term(&args[0], root, 1);
        if (!db) return pl_ok();
        rt_pl_db_define(db);
        rt_pl_db_match_erase(root, db, (void *)&args[0]);
        return pl_ok();
    }
}
void * rt_pl_dop_db_alive_c(DESCR_t *args, int nargs, void *root) {
    extern void *rt_pl_ball_existence(DESCR_t *, int);
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    extern int rt_pl_db_born_undef(void *, void *, const char *);
    {
        void *db = pl_db_cell_of(args, root);
        if (!db || (rt_pl_db_killed(db) != 1 && !rt_pl_db_born_undef(root, db, pl_atom_str(rt_pl_deref_val(args[1]))))) return (void *)0;
        return rt_pl_ball_existence(&args[1], 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_anum_is_text(DESCR_t d) { return pl_atom_str(d) != (const char *)0; }
