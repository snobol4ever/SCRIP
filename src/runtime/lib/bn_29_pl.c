#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_dop_body_fn.h"
#include "runtime/lib/bn_head_plu_ctx.h"
#include "runtime/lib/bn_head_pl_out_leaf.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#include "runtime/lib/bn_head_plw_cell_deref.h"
#include "runtime/lib/bn_head_plw_entry.h"
#include "ir/ct_vec.h"
#include "ir/ir_fwd.h"
#include "parsers/snobol4/scrip_cc.h"
#include "ir/SM.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/core/g_runtime_misc.h"
#include "runtime/rt/prolog_atom.h"
#include "driver/driver_globals.h"
#include "runtime/pl_rat.h"
#include <ctype.h>
#include "runtime/rt/pl_rational.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_29_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/core/core_etc.h"
#include "runtime/rt/rt_pl_trail_pl_tr_entry.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern void *rt_ws_alloc_descr(size_t);
int rt_pl_atop_cell(int, void *, void *);
int rt_pl_acyclic_cell(void *);
extern FILE *fh_get(int);
extern char * pl_tr_base_of(const char *);
extern int rt_pl_db_cp_count(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_num_cmp(const char *s, DESCR_t a, DESCR_t b) {
    extern int rt_big_cmp(DESCR_t, DESCR_t);
    extern char *rt_big_str(DESCR_t);
    if ((pl_rat_is(a) || pl_rat_is(b)) && !IS_REAL_fn(a) && !IS_REAL_fn(b)) {
        int c = pl_rat_cmp(a, b);
        return !strcmp(s, "lt") ? c < 0 : !strcmp(s, "gt") ? c > 0 : !strcmp(s, "le") ? c <= 0 : !strcmp(s, "ge") ? c >= 0 : !strcmp(s, "eq") ? c == 0 : !strcmp(s, "ne") ? c != 0 : 0;
    }
    if (pl_rat_is(a)) a = REALVAL(pl_rat_real(a));
    if (pl_rat_is(b)) b = REALVAL(pl_rat_real(b));
    if ((a.v == DT_BIG || b.v == DT_BIG) && !IS_REAL_fn(a) && !IS_REAL_fn(b)) {
        int c = rt_big_cmp(a, b);
        if (!strcmp(s, "lt")) return c < 0;
        if (!strcmp(s, "gt")) return c > 0;
        if (!strcmp(s, "le")) return c <= 0;
        if (!strcmp(s, "ge")) return c >= 0;
        if (!strcmp(s, "eq")) return c == 0;
        if (!strcmp(s, "ne")) return c != 0;
        return 0;
    }
    double av = IS_REAL_fn(a) ? a.r : (a.v == DT_BIG ? strtod(rt_big_str(a), (char **)0) : (double)a.i);
    double bv = IS_REAL_fn(b) ? b.r : (b.v == DT_BIG ? strtod(rt_big_str(b), (char **)0) : (double)b.i);
    if (!strcmp(s, "lt")) return av < bv;
    if (!strcmp(s, "gt")) return av > bv;
    if (!strcmp(s, "le")) return av <= bv;
    if (!strcmp(s, "ge")) return av >= bv;
    if (!strcmp(s, "eq")) return av == bv;
    if (!strcmp(s, "ne")) return av != bv;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_proc_defined_static(const char *name, long arity) {
    if (!name) return 0;
    char key[fmt_len("%s/%ld", name, arity)];
    snprintf(key, sizeof key, "%s/%ld", name, arity);
    int idx = rt_proc_index_of(key);
    return (idx >= 0 && !rt_proc_is_generator(key)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_cmp(const char *op, DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    DESCR_t a = rt_pl_deref_val(args[0]);
    DESCR_t b = rt_pl_deref_val(args[1]);
    if (pl_num_cmp(op, a, b)) { *out = a; return 1; }
    *out = FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_call(dop_body_fn body, DESCR_t *args, int nargs) { DESCR_t out = FAILDESCR; body(args, nargs, &out); return out; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_cmp_lt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("lt", a, n, o); }
static int dop_cmp_gt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("gt", a, n, o); }
static int dop_cmp_le(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("le", a, n, o); }
static int dop_cmp_ge(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ge", a, n, o); }
static int dop_cmp_eq(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("eq", a, n, o); }
static int dop_cmp_ne(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ne", a, n, o); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PLR_WALK2(plu_walk, plu_leaf, plw_cell_deref) int rt_pl_unify_deep_c(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) {
    char probe;
    plu_ctx_t k;
    k.cx = cx;
    k.floor_ = &probe;
    return plu_walk(a, b, (void *)&k, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_deep_top_c(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) {
    extern void *rt_pl_cap_error(void);
    int arm = !g_rt_misc.stack_refuse_armed, ok;
    if (arm) g_rt_misc.stack_refuse_armed = 1;
    ok = rt_pl_unify_deep_c(a, b, cx);
    cx->ball = (void *)0;
    if (!arm) return ok;
    g_rt_misc.stack_refuse_armed = 0;
    if (!g_rt_misc.stack_refused) return ok;
    g_rt_misc.stack_refused = 0;
    cx->ball = rt_pl_cap_error();
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_is_cold_c(DESCR_t *args, int nargs, void **ball) {
    if (nargs != 2) return FAILDESCR;
    {
        DESCR_t v = rt_pl_deref_val(args[1]);
        if (v.v != DT_I && v.v != DT_R && v.v != DT_BIG) { void *bl = (void *)0; DESCR_t ev; if (pl_ax_eval(v, &ev, &bl)) v = ev; else { if (ball && bl) *ball = bl; return FAILDESCR; } }
        if (v.v != DT_I && v.v != DT_R && v.v != DT_BIG && !pl_rat_is(v)) { extern void *rt_pl_dop_ax_eguard_c(DESCR_t *, int); if (ball) *ball = rt_pl_dop_ax_eguard_c(&v, 1); return FAILDESCR; }
        return v;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_ax_cold_c(DESCR_t *args, int nargs, const char *op, void **ball) {
    DESCR_t out = FAILDESCR;
    if (!op || !args || nargs < 0 || nargs > 2) return FAILDESCR;
    dop_ax(op, args, nargs, &out, ball);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_cmp_fast(DESCR_t *args, int rel, dop_body_fn slow, void **ball) {
    DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]);
    if ((a.v == DT_I || a.v == DT_R) && (b.v == DT_I || b.v == DT_R)) {
        double av = IS_REAL_fn(a) ? a.r : (double)a.i, bv = IS_REAL_fn(b) ? b.r : (double)b.i;
        int t = rel == 0 ? av < bv : rel == 1 ? av > bv : rel == 2 ? av <= bv : rel == 3 ? av >= bv : rel == 4 ? av == bv : av != bv;
        return t ? a : FAILDESCR;
    }
    { void *bl = (void *)0; DESCR_t ev[2]; if (!pl_ax_eval(a, &ev[0], &bl) || !pl_ax_eval(b, &ev[1], &bl)) { if (ball) *ball = bl; return FAILDESCR; } return dop_call(slow, ev, 2); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_cmp_cold_c(DESCR_t *args, int nargs, const char *op, void **ball) {
    if (nargs != 2 || !op) return FAILDESCR;
    if (!strcmp(op, "lt")) return dop_cmp_fast(args, 0, dop_cmp_lt, ball);
    if (!strcmp(op, "gt")) return dop_cmp_fast(args, 1, dop_cmp_gt, ball);
    if (!strcmp(op, "le")) return dop_cmp_fast(args, 2, dop_cmp_le, ball);
    if (!strcmp(op, "ge")) return dop_cmp_fast(args, 3, dop_cmp_ge, ball);
    if (!strcmp(op, "eq")) return dop_cmp_fast(args, 4, dop_cmp_eq, ball);
    if (!strcmp(op, "ne")) return dop_cmp_fast(args, 5, dop_cmp_ne, ball);
    return FAILDESCR;
}
DESCR_t dop_write(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    if (nargs != 1) return FAILDESCR;
    {
        FILE *wd = fh_cur_out_fp();
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (pl_val_unbound(v)) {
            extern void rt_pl_write_cell_fp(void *, FILE *);
            rt_pl_write_cell_fp((void *)&args[0], wd);
        } else if (v.v == DT_R) {
            char fb[64];
            fputs(pl_real_iso_str(v.r, fb, sizeof fb), wd);
        } else if (pl_rat_is(v)) pl_rat_fput(v, wd);
        else out_write_descr(wd, v, 0);
    }
    { DESCR_t r = {0}; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
}
DESCR_t dop_pl_between_special(DESCR_t *args, int nargs) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    DESCR_t lo = rt_pl_deref_val(args[0]), hi = rt_pl_deref_val(args[1]);
    const char *h = hi.v == (DTYPE_t)DT_PLATOM ? pl_atom_str(hi) : (const char *)0;
    return (lo.v == (DTYPE_t)DT_BIG || hi.v == (DTYPE_t)DT_BIG || (h && (!strcmp(h, "inf") || !strcmp(h, "infinite")))) ? pl_ok() : FAILDESCR;
}
DESCR_t dop_pl_acyclic_term(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 1 && rt_pl_acyclic_cell(&args[0])) ? pl_ok() : FAILDESCR; }
static int pl_atop_op_of(const char *op) {
    return !op ? -1 : !strcmp(op, "lt") ? 0 : !strcmp(op, "le") ? 1 : !strcmp(op, "gt") ? 2 : !strcmp(op, "ge") ? 3 : !strcmp(op, "eq") ? 4 : !strcmp(op, "ne") ? 5 : -1;
}
DESCR_t rt_pl_atop_cold_c(DESCR_t *args, int nargs, const char *op) {
    int o = pl_atop_op_of(op);
    pl_atoms_ready();
    return (nargs == 2 && o >= 0 && rt_pl_atop_cell(o, &args[0], &args[1])) ? pl_ok() : FAILDESCR;
}
DESCR_t dop_pl_writeln(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    extern int rt_pl_dialect_xsb(void);
    extern void rt_pl_write_cell_fp_p(void *, FILE *, int);
    if (nargs != 1) return FAILDESCR;
    {
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (rt_pl_dialect_xsb() && (v.v == (DTYPE_t)DT_PLREF || v.v == (DTYPE_t)DT_PLVAR)) {
            FILE *wd = fh_cur_out_fp();
            DESCR_t pt = v;
            fflush(wd);
            rt_pl_write_cell_fp_p(plw_entry(&pt), wd, 999);
            fputc('\n', wd);
            return pl_ok();
        }
    }
    dop_write(args, 1);
    fputc('\n', fh_cur_out_fp());
    return pl_ok();
}
DESCR_t dop_pl_tab(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    if (nargs != 1) return FAILDESCR;
    { DESCR_t v = rt_pl_deref_val(args[0]); if (v.v != DT_I) return FAILDESCR; FILE *o = fh_cur_out_fp(); for (long long i = 0;i < v.i;i++) fputc(' ', o); }
    return pl_ok();
}
DESCR_t dop_pl_put_char(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    char b[64];
    const char *s;
    if (nargs != 1 || !pl_cell_text(args[0], b, sizeof b, &s)) return FAILDESCR;
    fputs(s, fh_cur_out_fp());
    return pl_ok();
}
DESCR_t dop_pl_put_code(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    extern int rt_pl_u8_put(char *, int);
    if (nargs != 1) return FAILDESCR;
    { DESCR_t v = rt_pl_deref_val(args[0]); char cb[8]; int bl; if (v.v != DT_I) return FAILDESCR; bl = rt_pl_u8_put(cb, (int)v.i); fwrite(cb, 1, (size_t)bl, fh_cur_out_fp()); return pl_ok(); }
}
static void *pl_format_body(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void *rt_pl_format_run(const char *, void *, pl_tr_ctx_t *);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    char fb[8192];
    const char *fmt;
    DESCR_t lst, d0;
    if (nargs < 1 || nargs > 2) return (void *)0;
    pl_atoms_ready();
    d0 = rt_pl_deref_val(args[0]);
    if (pl_val_unbound(d0)) return rt_pl_ball_instantiation();
    if (d0.v == DT_I || d0.v == DT_R) return rt_pl_ball_kind2("type_error", "atom", d0);
    if (pl_xsb_dialect() && pl_is_cons(d0)) {
        int kind = 0;
        for (DESCR_t c = d0; pl_is_cons(c); c = rt_pl_deref_val(((DESCR_t *)c.p)[1])) {
            DESCR_t e = rt_pl_deref_val(((DESCR_t *)c.p)[0]);
            int k = e.v == DT_I ? 1 : 2;
            if (kind && kind != k) return rt_pl_ball_kind2("type_error", "text", d0);
            kind = k;
        }
    }
    if (pl_is_nil(d0)) fmt = "";
    else if (!pl_cell_text(args[0], fb, sizeof fb, &fmt)) return rt_pl_ball_kind2("type_error", "atom", d0);
    lst = pl_nil();
    if (nargs == 2) {
        DESCR_t cur = rt_pl_deref_val(args[1]);
        if ((pl_swi_dialect() || pl_xsb_dialect()) && !pl_is_cons(cur) && !pl_is_nil(cur)) return rt_pl_format_run(fmt, (lst = pl_cons(args[1], pl_nil()), &lst), cx);
        while (pl_is_cons(cur)) cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]);
        if (pl_val_unbound(cur)) return rt_pl_ball_instantiation();
        if (!pl_is_nil(cur)) return rt_pl_ball_kind2("type_error", "list", rt_pl_deref_val(args[1]));
        lst = args[1];
    }
    return rt_pl_format_run(fmt, &lst, cx);
}
DESCR_t rt_pl_dop_format_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void *rt_pl_format_failed_token(void);
    void *b = pl_format_body(args, nargs, cx);
    if (b == rt_pl_format_failed_token()) return FAILDESCR;
    if (b) { if (cx) cx->ball = b; return FAILDESCR; }
    return (nargs < 1 || nargs > 2) ? FAILDESCR : pl_ok();
}
DESCR_t dop_pl_format(DESCR_t *args, int nargs) { return rt_pl_dop_format_c(args, nargs, (pl_tr_ctx_t *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_stream_idx(DESCR_t s, int out) {
    extern FILE *fh_get(int);
    extern int prolog_atom_intern(const char *);
    DESCR_t d = rt_pl_deref_val(s);
    const char *nm = pl_atom_str(d);
    if (nm) { if (!strcmp(nm, "user_input")) return out ? -1 : 0; if (!strcmp(nm, "user_output")) return out ? 1 : -1; if (!strcmp(nm, "user_error")) return out ? 2 : -1; return pl_alias_idx(nm); }
    if (d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == prolog_atom_intern("$stream") && plc_fid_arity(d.slen) == 1) {
        DESCR_t a = rt_pl_deref_val(((DESCR_t *)d.p)[0]);
        if (a.v == DT_I && fh_get((int)a.i)) return (int)a.i;
    }
    return -1;
}
PL_OUT_LEAF(write, dop_write) PL_OUT_LEAF(writeq, dop_pl_writeq) PL_OUT_LEAF(write_canonical, dop_pl_write_canonical) PL_OUT_LEAF(writeln, dop_pl_writeln) PL_OUT_LEAF(nl, dop_nl)
    PL_OUT_LEAF(tab, dop_pl_tab) PL_OUT_LEAF(put_char, dop_pl_put_char) PL_OUT_LEAF(put_code, dop_pl_put_code) PL_OUT_LEAF(flush_output, dop_pl_flush_output) RT_ENTRY_DECL(dop_pl_set_output, leaf, 0);
DESCR_t dop_pl_set_output(DESCR_t *args, int nargs) {
    extern void fh_set_output(int);
    int idx;
    if (nargs != 1 || (idx = pl_stream_idx(args[0], 1)) < 0) return FAILDESCR;
    fh_set_output(idx);
    return pl_ok();
}
int rt_pl_unifiable_cell(DESCR_t *ax, DESCR_t *ay, DESCR_t *au, pl_tr_ctx_t *cx) {
    extern int prolog_atom_intern(const char *);
    extern int prolog_functor_intern(int, int);
    char probe;
    char *floor_ = &probe;
    char fakeb[PL_TR_FRAME_HEADER_BYTES];
    char *base = pl_tr_base_of(cx->tr);
    uint64_t wake0 = *pl_tr_wake_slot(base);
    char *mark = cx->tr;
    pl_tr_ctx_t c2 = *cx;
    memset(fakeb, 0, sizeof fakeb);
    pl_tr_frame_hi_set(fakeb, (const void *)0);
    c2.b = fakeb;
    int ok = rt_pl_unify_deep_c(plw_entry(ax), plw_entry(ay), &c2);
    int n = (int)((c2.tr - mark) / PL_TR_ENTRY_BYTES);
    DESCR_t *cells[n > 0 ? n : 1];
    DESCR_t vals[n > 0 ? n : 1];
    int k = 0;
    if (ok) for (char *p = mark; p < c2.tr; p += PL_TR_ENTRY_BYTES) {
        pl_tr_entry_t *e = (pl_tr_entry_t *)p;
        if (e->cell->v == (DTYPE_t)DT_PLVAR && e->cell->p == (void *)e->cell) continue;
        cells[k] = e->cell;
        vals[k] = *e->cell;
        k++;
    }
    cx->tr = rt_pl_tr_unwind_to(c2.tr, mark);
    *(char **)base = cx->tr;
    *pl_tr_wake_slot(base) = wake0;
    cx->ball = c2.ball;
    if (!ok) return 0;
    {
        int eq = prolog_functor_intern(prolog_atom_intern("="), 2);
        DESCR_t acc = pl_nil();
        for (int i = 0; i < k; i++) {
            DESCR_t *kk = (DESCR_t *)rt_ws_alloc_descr(2);
            DESCR_t t = {0};
            kk[0] = pl_unif_global(cells[i], floor_, cx);
            kk[1] = vals[i].v == (DTYPE_t)DT_PLVAR && vals[i].p ? pl_unif_global((DESCR_t *)vals[i].p, floor_, cx) : vals[i];
            t.v = (DTYPE_t)DT_PLREF;
            t.slen = (uint32_t)eq;
            t.p = (void *)kk;
            acc = pl_cons(t, acc);
        }
        return plw_unify_cell_val(plw_entry(au), acc, cx);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_var_name_ok(const char *n) {
    extern int prolog_u_letter(const char *, int *);
    int adv = 1;
    if (!n || !n[0]) return 0;
    if ((unsigned char)n[0] >= 0x80 ? prolog_u_letter(n, &adv) != 1 : !(isupper((unsigned char)n[0]) || n[0] == '_')) return 0;
    for (const char *q = n + adv; *q; q += adv) { adv = 1; if ((unsigned char)*q >= 0x80 ? !prolog_u_letter(q, &adv) : !(isalnum((unsigned char)*q) || *q == '_')) return 0; }
    return 1;
}
static const char *const pl_write_bools[] = { "quoted", "ignore_ops", "numbervars", "portray", "portrayed", "character_escapes", "quote_non_ascii", "partial", "character_escapes_unicode",
    "space_args", "$lists", 0 };
static int pl_write_opt_bare_ok(const char *on) { for (int i = 0; on && pl_write_bools[i]; i++) if (!strcmp(on, pl_write_bools[i])) return 1; return 0; }
static int pl_write_opt_bad(const char *on, DESCR_t a, DESCR_t *culprit) {
    const char *as;
    if (!on) return 2;
    if (!strcmp(on, "truncated")) return 0;
    if (pl_val_unbound(a)) {
        for (int i = 0; pl_write_bools[i]; i++) if (!strcmp(on, pl_write_bools[i])) return 1;
        if (!strcmp(on, "variable_names") || !strcmp(on, "max_depth") || !strcmp(on, "max_text") || !strcmp(on, "portray_goal") || !strcmp(on, "blobs") || !strcmp(on, "spacing")) return 1;
        if (!strcmp(on, "module") && !pl_gnu_dialect()) return 1;
        if (pl_xsb_dialect() &&
            (!strcmp(on, "portable") || !strcmp(on, "integer_prefix") || !strcmp(on, "integer_base") || !strcmp(on, "radix") || !strcmp(on, "float_precision") || !strcmp(on, "float_specifier")))
            return 1;
        return 2;
    }
    as = pl_atom_str(a);
    if (pl_xsb_dialect()) {
        if (!strcmp(on, "portable") || !strcmp(on, "integer_prefix")) return (as && (!strcmp(as, "true") || !strcmp(as, "false"))) ? 0 : 2;
        if (!strcmp(on, "float_precision")) return (a.v == DT_I && a.i >= 0) ? 0 : 2;
        if (!strcmp(on, "float_specifier")) return (as && (!strcmp(as, "f") || !strcmp(as, "e") || !strcmp(as, "g"))) ? 0 : 2;
        if (!strcmp(on, "integer_base")) return ((a.v == DT_I && a.i >= 2 && a.i <= 36) || (as && (!strcmp(as, "hex") || !strcmp(as, "octal") || !strcmp(as, "decimal")))) ? 0 : 2;
        if (!strcmp(on, "radix")) return (as && (!strcmp(as, "hex") || !strcmp(as, "octal") || !strcmp(as, "decimal"))) ? 0 : 2;
    }
    if (!strcmp(on, "portray_goal")) return 0;
    if (!strcmp(on, "blobs")) return as ? 0 : 2;
    if (!strcmp(on, "spacing")) { *culprit = a; return !as ? 4 : (!strcmp(as, "standard") || !strcmp(as, "next_argument")) ? 0 : 6; }
    if (!strcmp(on, "module") && !pl_gnu_dialect()) return as ? 0 : 2;
    for (int i = 0; pl_write_bools[i]; i++) if (!strcmp(on, pl_write_bools[i])) return (as && (!strcmp(as, "true") || !strcmp(as, "false"))) ? 0 : 2;
    if (!strcmp(on, "max_depth") || !strcmp(on, "max_text")) return (a.v == DT_I && a.i >= 0) ? 0 : 2;
    if (!strcmp(on, "priority")) return (a.v == DT_I && a.i >= 0 && a.i <= 1200) ? 0 : 2;
    if (!strcmp(on, "variable_names")) {
        DESCR_t l = a;
        for (;;) {
            if (pl_val_unbound(l)) return 1;
            if (pl_is_nil(l)) return 0;
            if (!pl_is_cons(l)) return 2;
            {
                DESCR_t *k = (DESCR_t *)l.p;
                DESCR_t e = rt_pl_deref_val(k[0]);
                if (pl_val_unbound(e)) return 1;
                if (e.v != (DTYPE_t)DT_PLREF || plc_fid_arity(e.slen) != 2 || plc_fid_name(e.slen) != prolog_atom_intern("=")) return 2;
                {
                    DESCR_t nmv = rt_pl_deref_val(((DESCR_t *)e.p)[0]);
                    if (pl_val_unbound(nmv)) return 1;
                    if ((int)nmv.v != DT_PLATOM || !pl_atom_str(nmv)) { *culprit = nmv; return 4; }
                    if (!pl_var_name_ok(pl_atom_str(nmv))) { *culprit = nmv; return 5; }
                }
                l = rt_pl_deref_val(k[1]);
            }
        }
    }
    return 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_write_opt_known(const char *on) {
    static const char *const known[] = { "quoted", "ignore_ops", "numbervars", "portray", "portrayed", "character_escapes", "quote_non_ascii", "partial", "character_escapes_unicode", "space_args",
        "truncated", "variable_names", "max_depth", "max_text", "portray_goal", "blobs", "module", "priority", "spacing", 0 };
    for (int i = 0; on && known[i]; i++) if (!strcmp(on, known[i])) return 1;
    return 0;
}
int pl_write_opts_check(DESCR_t optlist, DESCR_t *culprit) {
    DESCR_t o = rt_pl_deref_val(optlist);
    for (;;) {
        if (pl_val_unbound(o)) return 1;
        if (pl_is_nil(o)) return 0;
        if (!pl_is_cons(o)) { *culprit = rt_pl_deref_val(optlist); return 2; }
        {
            DESCR_t *kids = (DESCR_t *)o.p;
            DESCR_t opt = rt_pl_deref_val(kids[0]);
            int bad;
            if (pl_val_unbound(opt)) return 1;
            *culprit = opt;
            if (opt.v == (DTYPE_t)DT_PLREF && plc_fid_arity(opt.slen) > 0 && pl_swi_dialect() && !pl_write_opt_known(prolog_atom_name(plc_fid_name(opt.slen)))) {
                o = rt_pl_deref_val(kids[1]);
                continue;
            }
            if ((int)opt.v == DT_PLATOM && (pl_swi_dialect() || pl_xsb_dialect()) && pl_write_opt_bare_ok(pl_atom_str(opt))) { o = rt_pl_deref_val(kids[1]); continue; }
            if (opt.v != (DTYPE_t)DT_PLREF || plc_fid_arity(opt.slen) != 1) return 3;
            bad = pl_write_opt_bad(prolog_atom_name(plc_fid_name(opt.slen)), rt_pl_deref_val(((DESCR_t *)opt.p)[0]), culprit);
            if (bad) return bad == 1 ? 1 : bad >= 4 ? bad : 3;
            o = rt_pl_deref_val(kids[1]);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(write_term, 2)
    {
        extern void *rt_pl_write_term_cell(void *, void *, void *);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t bad = {0};
        int k = pl_write_opts_check(args[1], &bad);
        ok = 0;
        if (k == 1) cx->ball = rt_pl_ball_instantiation();
        else if (k == 2) cx->ball = rt_pl_ball_kind2("type_error", "list", bad);
        else if (k == 3) cx->ball = rt_pl_ball_kind2("domain_error", "write_option", bad);
        else if (k == 4) cx->ball = rt_pl_ball_kind2("type_error", "atom", bad);
        else if (k == 5) cx->ball = rt_pl_ball_kind2("domain_error", "variable_name", bad);
        else if (k == 6) cx->ball = rt_pl_ball_kind2("domain_error", "spacing", bad);
        else { cx->ball = rt_pl_write_term_cell(&args[0], &args[1], cx); ok = !cx->ball; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(wot_open, 3)
    {
        extern int fh_alloc(FILE *);
        extern int fh_current_output(void);
        extern FILE *fh_memsink_open_slot(int *);
        extern char *fh_memsink_take(int, size_t *);
        ok = 0;
        {
            int si = -1;
            FILE *fp = fh_memsink_open_slot(&si);
            if (fp) {
                int idx = fh_alloc(fp);
                if (idx < 0) {
                    fclose(fp);
                    fh_memsink_take(si, (size_t *)0);
                } else {
                    extern void fh_set_sink(int, int);
                    int oldout = fh_current_output();
                    fh_set_sink(idx, si);
                    ok = plw_unify_vals(args[0], pl_mk_stream(idx), cx) && plw_unify_vals(args[1], INTVAL((long long)oldout), cx) && plw_unify_vals(args[2], INTVAL((long long)si), cx);
                }
            }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(wot_capture, 4)
    {
        extern void fh_set_output(int);
        extern void fh_free(int);
        extern FILE *fh_get(int);
        extern int prolog_atom_intern(const char *);
        extern char *fh_memsink_take(int, size_t *);
        ok = 0;
        {
            int idx = pl_stream_idx(args[0], 1);
            DESCR_t oo = rt_pl_deref_val(args[1]);
            int oldout = (oo.v == DT_I) ? (int)oo.i : 1;
            DESCR_t hv = rt_pl_deref_val(args[2]);
            fh_set_output(oldout);
            if (idx >= 0 && hv.v == DT_I) {
                FILE *fp = fh_get(idx);
                if (fp) fclose(fp);
                fh_free(idx);
                {
                    size_t len = 0;
                    char *buf = fh_memsink_take((int)hv.i, &len);
                    DESCR_t sink = rt_pl_deref_val(args[3]);
                    if (sink.v == (DTYPE_t)DT_PLREF && plc_fid_arity(sink.slen) == 2) {
                        int fid = plc_fid_name(sink.slen);
                        DESCR_t *sa = (DESCR_t *)sink.p;
                        if (fid == prolog_atom_intern("codes") || fid == prolog_atom_intern("chars")) ok =
                            plw_unify_vals(sa[0], pl_text_dlist(buf ? buf : "", len, fid == prolog_atom_intern("codes"), sa[1]), cx);
                    }
                    if (sink.v == (DTYPE_t)DT_PLREF && plc_fid_arity(sink.slen) == 1) {
                        int fid = plc_fid_name(sink.slen);
                        DESCR_t *sa = (DESCR_t *)sink.p;
                        if (fid == prolog_atom_intern("atom")) ok = plw_unify_vals(sa[0], pl_mk_atom_dup(buf ? buf : "", len), cx);
                        else if (fid == prolog_atom_intern("string")) {
                            ok = plw_unify_vals(sa[0], rt_pl_make_string_cell(buf ? buf : ""), cx);
                        } else if (fid == prolog_atom_intern("codes")) {
                            DESCR_t acc = pl_nil();
                            for (size_t i = len; i > 0; i--) acc = pl_cons(INTVAL((unsigned char)buf[i - 1]), acc);
                            ok = plw_unify_vals(sa[0], acc, cx);
                        } else if (fid == prolog_atom_intern("chars")) {
                            DESCR_t *el = (DESCR_t *)rt_ws_alloc_descr(len ? len : 1);
                            size_t n = 0;
                            for (size_t i = 0; i < len; i++) { char c1[2]; c1[0] = buf[i]; c1[1] = 0; el[n++] = pl_mk_atom_dup(c1, 1); }
                            ok = plw_unify_vals(sa[0], pl_list_from_arr(el, (int)n), cx);
                        }
                    }
                }
            }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(wot_discard, 3)
    {
        extern void fh_set_output(int);
        extern void fh_free(int);
        extern FILE *fh_get(int);
        extern char *fh_memsink_take(int, size_t *);
        {
            int idx = pl_stream_idx(args[0], 1);
            DESCR_t oo = rt_pl_deref_val(args[1]);
            int oldout = (oo.v == DT_I) ? (int)oo.i : 1;
            DESCR_t hv = rt_pl_deref_val(args[2]);
            fh_set_output(oldout);
            if (idx >= 0) { FILE *fp = fh_get(idx); if (fp) fclose(fp); fh_free(idx); }
            if (hv.v == DT_I) fh_memsink_take((int)hv.i, (size_t *)0);
            ok = 1;
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_gnu_position(DESCR_t s, long *ch, long *ln, long *lp, pl_tr_ctx_t *cx, const char *who) {
    extern int fh_query_position(int, long *, long *, long *);
    void *b = (void *)0;
    int idx = pl_stream_resolve(s, -1, 1, &b), r;
    if (b) { cx->ball = b; return 0; }
    r = fh_query_position(idx, ch, ln, lp);
    if (r == 1) return 1;
    fprintf(stderr, "scrip: prolog: %s on %s is not tracked yet -- positions are read with pread over a seekable input stream or a regular-file output stream\n", who,
        r == -1 ? "an output stream that is not a regular file" : "a stream that cannot seek");
    exit(2);
}
PL_CX_LEAF_HEAD(gnu_stream_line_column, 3)
    {
        long c, l, p;
        extern void pl_gnu_ctx(pl_tr_ctx_t *, const char *, int);
        ok = pl_gnu_position(args[0], &c, &l, &p, cx, "stream_line_column/3") && plw_unify_vals(args[1], INTVAL(l + 1), cx) && plw_unify_vals(args[2], INTVAL(p + 1), cx);
        pl_gnu_ctx(cx, "stream_line_column", 3);
    }
PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_mod_get_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    extern int rt_pl_db_mod_get(void *, const char *, int, int64_t *);
    int64_t v = 0;
    int ok;
    int limit;
    const char *m;
    const char *w;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    m = pl_atom_str(rt_pl_deref_val(args[0]));
    w = pl_atom_str(rt_pl_deref_val(args[1]));
    if (!m || !w) return FAILDESCR;
    limit = w[0] == 'l';
    if (!rt_pl_db_mod_get(root, m, limit, &v) || (limit && v == 0)) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    ok = plw_unify_vals(args[2], INTVAL((long long)v), cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_mod_set_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    extern int rt_pl_db_mod_set_limit(void *, const char *, int64_t);
    DESCR_t n;
    const char *m;
    int r;
    int ok;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    m = pl_atom_str(rt_pl_deref_val(args[0]));
    n = rt_pl_deref_val(args[1]);
    if (!m || n.v != DT_I) return FAILDESCR;
    r = rt_pl_db_mod_set_limit(root, m, (int64_t)n.i);
    rt_pl_tr_gc_sync(cx->tr);
    ok = plw_unify_vals(args[2], INTVAL((long long)r), cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_pl_cp_count_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int ok;
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    ok = plw_unify_vals(args[0], INTVAL((long long)rt_pl_db_cp_count(root)), cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_zguard_cold_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    char ob[64];
    const char *op = "is";
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    if (pl_cell_text(args[1], ob, sizeof ob, &op) && op && op[0]) { } else { op = "is"; }
    {
        DESCR_t d = rt_pl_deref_val(args[0]);
        if (d.v == DT_I && d.i == 0) return rt_pl_ball_eval_error("zero_divisor", op, 2);
        if (d.v == DT_R && d.r == 0.0) return rt_pl_ball_eval_error("zero_divisor", op, 2);
        return (void *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_ax_eguard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_evaluable(const char *, int);
    extern const char *prolog_atom_name(int);
    if (nargs != 1) return (void *)0;
    {
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (v.v == DT_I || v.v == DT_R) return (void *)0;
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) return rt_pl_ball_instantiation();
        if ((int)v.v == DT_PLATOM) return rt_pl_ball_evaluable(prolog_atom_name((int)v.i), 0);
        if (v.v == DT_S) return rt_pl_ball_evaluable(v.s, 0);
        if ((int)v.v == DT_PLREF) return rt_pl_ball_evaluable(prolog_atom_name(plc_fid_name(v.slen)), plc_fid_arity(v.slen));
        return rt_pl_ball_evaluable("?", 0);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_guard_n(DESCR_t *args, int nargs, int want) {
    char nb[64];
    const char *nm;
    if (nargs != want + 1) return (void *)0;
    pl_atoms_ready();
    if (!pl_cell_text(args[0], nb, sizeof nb, &nm) || !nm || !nm[0]) return (void *)0;
    return pl_anum_check(nm, &args[1], want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_anum_cold_c(DESCR_t *args, int nargs) { return nargs >= 2 ? pl_anum_guard_n(args, nargs, nargs - 1) : (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_stream_guard_c(DESCR_t *args, int nargs) {
    char db[64];
    const char *dop = "output";
    void *ball = (void *)0;
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    if (!pl_cell_text(args[1], db, sizeof db, &dop) || !dop || !dop[0]) dop = "output";
    pl_stream_resolve(args[0], !strcmp(dop, "output"), 0, &ball);
    return ball;
}
