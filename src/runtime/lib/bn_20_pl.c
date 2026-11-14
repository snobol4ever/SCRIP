#include "runtime/lib/bn_head_call_args.h"
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_rk_cbh.h"
#include "runtime/lib/bn_head_gram_ent.h"
#include "runtime/lib/bn_head_gram_n.h"
#include "runtime/lib/bn_head_rk_redisp.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#include "runtime/lib/bn_head_pl_in_leaf.h"
#include "runtime/lib/bn_head_plf_float.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/lib/bn_head_pl_dia_flag.h"
#include "runtime/lib/bn_head_rt_hook_guard.h"
#include "runtime/core/core_error.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include <fenv.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_29_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_36_sh.h"
#include "runtime/rk_rakudo_method_names.inc"
#include "runtime/core/g_runtime_bn.h"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_rt_asm_helpers_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_gc_roots_args.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/builtins/gen_runtime_invocation_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
extern FILE *fh_get(int);
extern char * pl_tr_base_of(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_rakudo_method(const char *m) {
    int lo = 0, hi = rk_rakudo_methods_n;
    while (lo < hi) { int mid = (lo + hi) / 2; int c = strcmp(m, rk_rakudo_methods[mid]); if (!c) return 1; if (c < 0) hi = mid; else lo = mid + 1; }
    return 0;
}
int bnd_handles_need(const char *cls, const char *meth) {
    DatType *t = cls ? dat_find_type(cls) : (DatType *)0;
    size_t n = 1;
    if (t && meth) for (int mi = 0; mi < (t->mro_len > 0 ? t->mro_len : 1); mi++) {
        DatType *k = dat_find_type(mi ? t->mro[mi] : t->name);
        if (!k) continue;
        for (int i = 0; i < k->nhandles; i++) if (!strcmp(k->handles_meth[i], meth)) { size_t l = strlen(k->handles_fld[i]) + 1; if (l > n) n = l; }
    }
    return (int)n;
}
const char *resolve_method_chain(const char *cls, const char *mname, int *pfound) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_roles(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (pfound) *pfound = -1;
    const char *chain[bnd_dat_need(cls, 0)];
    int nchain = dat_mro(cls, chain, (int)(sizeof chain / sizeof chain[0]));
    if (nchain == 0) { chain[0] = cls; nchain = 1; }
    for (int ci = 0; ci < nchain; ci++) {
        char buf[fmt_len("%s__%s", chain[ci], mname)];
        snprintf(buf, sizeof buf, "%s__%s", chain[ci], mname);
        int found = rt_proc_has_native_fn(buf);
        if (!found) for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { found = 1; break; }
        if (found) { if (pfound) *pfound = ci; return chain[ci]; }
        const char *roles[bnd_dat_need(chain[ci], 1)];
        int nr = dat_roles(chain[ci], roles, (int)(sizeof roles / sizeof roles[0]));
        for (int ri = 0; ri < nr; ri++) {
            char rb[fmt_len("%s__%s", roles[ri], mname)];
            snprintf(rb, sizeof rb, "%s__%s", roles[ri], mname);
            int rf = rt_proc_has_native_fn(rb);
            if (!rf) for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, rb)) { rf = 1; break; }
            if (rf) { if (pfound) *pfound = ci; return roles[ri]; }
        }
    }
    return cls;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int meth_is_user_proc(const char *procname) {
    extern int rt_proc_has_native_fn(const char *name);
    if (procname && rt_proc_has_native_fn(procname)) return 1;
    if (procname) for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, procname)) return 1;
    return 0;
}
rk_redisp_t *g_redisp_cur = (rk_redisp_t *)0;
int rk_class_user_meth(DESCR_t d, const char *m) {
    if (!m || !IS_STR_fn(d) || !d.s || !dat_find_type(d.s)) return 0;
    const char *rmc = resolve_method_chain(d.s, m, NULL);
    char proc[fmt_len("%s__%s", rmc, m)];
    snprintf(proc, sizeof proc, "%s__%s", rmc, m);
    return meth_is_user_proc(proc);
}
static DESCR_t rd_arg(const rk_redisp_t *r, int k) { return r->skip_mname == 2 ? (k == 0 ? r->self : r->args[k - 1]) : (r->skip_mname && k > 0) ? r->args[1 + k] : r->args[k]; }
int rd_mro_len(const char *cname) {
    extern int dat_mro(const char *name, const char **out, int max);
    int cap = 16;
    for (;;) { const char *buf[cap]; int got = dat_mro(cname, buf, cap); if (got < cap) return got > 0 ? got : 1; cap *= 2; }
}
void bnd_gc_roots(void) {
    extern void rt_gc_visit_descr(DESCR_t *);
    extern void rt_gc_visit_raw(const char **);
    {
        static const long off[2] = { (long)__builtin_offsetof(gram_ent_t, qname), (long)__builtin_offsetof(gram_ent_t, body) };
        static const char *const *const words[2] = { (const char *const *)&g_match.blk, (const char *const *)&g_subject };
        if (gram_n > 0) rt_gc_run_ptr_fields(g_rt_bn.gram.p, (long)gram_n, (long)sizeof(gram_ent_t), 2, off);
        rt_gc_run_loc_list(words, 2);
    }
    for (rk_redisp_t *r = g_redisp_cur; r; r = r->prev) if (r->local) {
        rt_gc_visit_descr(&r->self);
        rt_gc_visit_descr(&r->mname);
        for (int k = 0; k < r->nargs; k++) rt_gc_visit_descr((DESCR_t *)&r->args[k]);
    }
    for (rk_cbh_t *h = g_rk_cbh_cur; h; h = h->prev) { rt_gc_visit_descr(&h->d[0]); rt_gc_visit_descr(&h->d[1]); }
    { extern void rt_main_args_gc_root(void); rt_main_args_gc_root(); }
    { extern void rt_pl_flags_gc_roots(void); rt_pl_flags_gc_roots(); }
    { extern void bnd_text_gc_roots(void); bnd_text_gc_roots(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t invoke_method_proc_rq(const char *procname, DESCR_t *callargs, int total, long *rq) {
    int pi;
    for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
    if (pi >= g_stage2.proc_count) { rt_call_args_need(total); for (int k = 0; k < total; k++) CALL_ARGS[k] = callargs[k]; return rk_proc_descr_rq(procname, total, rq); }
    {
        extern int rt_proc_has_native_fn(const char *name);
        if (rt_proc_has_native_fn(procname)) { rt_call_args_need(total); for (int k = 0; k < total; k++) CALL_ARGS[k] = callargs[k]; return rk_proc_descr_rq(procname, total, rq); }
    }
    return proc_table_call(pi, callargs, total);
}
DESCR_t invoke_method_proc(const char *procname, DESCR_t *callargs, int total) { return invoke_method_proc_rq(procname, callargs, total, (long *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_call_next_t rk_open_pinned_block(rt_call_next_t n, const char *procname, int nargs) {
    extern int rt_proc_pinned(const char *name);
    extern int rt_proc_nparams(const char *name);
    if (!n.fn || !rt_proc_pinned(procname)) return n;
    int np = rt_proc_nparams(procname);
    if (np < 0 || nargs > np) {
        fprintf(stderr, "rk_open_pinned_block: the method %s is a pinned block-protocol graph called with %d arguments over %d parameters (a variadic pinned method has no block road yet)\n", procname,
            nargs, np);
        { volatile char *z = (volatile char *)0; (void)*z; }
    }
    rt_call_args_need(np);
    for (int k = nargs; k < np; k++) CALL_ARGS[k] = NULVCL;
    n.how = (n.how & ~(63L | (0xffffffffL << 8))) | 3L | ((long)(unsigned)np << 8);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_call_next_t rk_method_open_at(DESCR_t *args, int nargs, rk_redisp_t *rec, const char *procname, const char *cname, int found_idx, int fallback, const char *mstrip) {
    extern int rt_define_returns_by_frame(const char *name);
    extern int rt_proc_has_native_fn(const char *name);
    rt_call_next_t none = { 0, 0 };
    if (!rec || !meth_is_user_proc(procname)) return none;
    {
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, procname)) break;
        if (pi < g_stage2.proc_count && !rt_proc_has_native_fn(procname)) return none;
    }
    if (!rt_define_returns_by_frame(procname)) return none;
    DESCR_t *argv = (DESCR_t *)((char *)rec - 16 * nargs);
    if (argv != args) for (int k = 0; k < nargs; k++) argv[k] = args[k];
    if (mstrip) argv[1] = STRVAL(rt_heap_strdup_c(mstrip));
    rec->self = argv[0];
    rec->mname = argv[1];
    rec->cname = cname;
    rec->found_idx = found_idx;
    rec->nargs = fallback ? nargs : nargs - 1;
    rec->args = argv;
    rec->skip_mname = fallback ? 0 : 1;
    rec->local = 0;
    rec->prev = g_redisp_cur;
    g_redisp_cur = rec;
    rt_call_args_need(rec->nargs);
    for (int k = 0; k < rec->nargs; k++) CALL_ARGS[k] = rd_arg(rec, k);
    { rt_call_next_t n = rt_call_open_by_name(procname, rec->nargs); if (!n.fn) { g_redisp_cur = rec->prev; return none; } return rk_open_pinned_block(n, procname, rec->nargs); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_call_next_t rk_method_open_d(DESCR_t *args, int nargs, rk_redisp_t *rec, int depth) {
    extern int dat_handles_field(const char *cls, const char *meth, char *out, int outsz);
    extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
    rt_call_next_t none = { 0, 0 };
    if (nargs < 2 || !args || !rec || depth > 16) return none;
    const char *mname = VARVAL_fn(args[1]);
    if (!mname || !*mname) return none;
    int maybe = mname[0] == '?' && mname[1];
    if (maybe) mname++;
    if (mname[0] == '^' || !strcmp(mname, "WHAT") || !strcmp(mname, "isa") || !strcmp(mname, "does") || !strcmp(mname, "bless") || !strcmp(mname, "clone") || !strcmp(mname, "parse")) return none;
    if (args[0].v != DT_DATA || !IS_DATA_INST_fn(args[0]) || !args[0].u || !args[0].u->type || !args[0].u->type->name) {
        if (maybe || rk_is_rakudo_method(mname)) return none;
        const char *tn = rk_value_type(args[0]), *cands[6];
        int nc = 0;
        cands[nc++] = tn;
        if (!strcmp(tn, "Bool")) cands[nc++] = "Int";
        if (!strcmp(tn, "Array")) cands[nc++] = "List";
        cands[nc++] = "Cool";
        cands[nc++] = "Any";
        cands[nc++] = "Mu";
        for (int ci = 0; ci < nc; ci++) {
            char pn[fmt_len("%s__%s", cands[ci], mname)];
            snprintf(pn, sizeof pn, "%s__%s", cands[ci], mname);
            if (meth_is_user_proc(pn)) return rk_method_open_at(args, nargs, rec, pn, cands[ci], -1, 0, (const char *)0);
        }
        return none;
    }
    const char *cname = args[0].u->type->name;
    int found_idx = -1;
    const char *rmc = resolve_method_chain(cname, mname, &found_idx);
    char procname[fmt_len("%s__%s", rmc, mname)];
    snprintf(procname, sizeof procname, "%s__%s", rmc, mname);
    if (meth_is_user_proc(procname)) return rk_method_open_at(args, nargs, rec, procname, cname, found_idx, 0, maybe ? mname : (const char *)0);
    {
        char df[bnd_handles_need(cname, mname)];
        if (dat_handles_field(cname, mname, df, (int)sizeof df)) {
            DESCR_t fwd[nargs];
            for (int k = 0; k < nargs; k++) fwd[k] = args[k];
            fwd[0] = rtx_try_dat_field_get(df, args[0]);
            return rk_method_open_d(fwd, nargs, rec, depth + 1);
        }
    }
    if (maybe || !strcmp(mname, "FALLBACK")) return none;
    { DESCR_t mu; if (rk_mu_method(mname, args[0], nargs - 2, &mu)) return none; }
    const char *fbc = resolve_method_chain(cname, "FALLBACK", NULL);
    char fbp[fmt_len("%s__%s", fbc, "FALLBACK")];
    snprintf(fbp, sizeof fbp, "%s__%s", fbc, "FALLBACK");
    return rk_method_open_at(args, nargs, rec, fbp, cname, -1, 1, (const char *)0);
}
rt_call_next_t rk_method_open(DESCR_t *args, int nargs, void *recp) { return rk_method_open_d(args, nargs, (rk_redisp_t *)recp, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_call_next_t rk_redispatch_open(DESCR_t *args, int nargs, void *recp, int with) {
    rk_redisp_t *rec = (rk_redisp_t *)recp, *top = g_redisp_cur;
    extern int rt_define_returns_by_frame(const char *name);
    extern int rt_proc_has_native_fn(const char *name);
    extern int dat_mro(const char *name, const char **out, int max);
    rt_call_next_t none = { 0, 0 };
    if (!rec || !top) return none;
    const char *fmname = VARVAL_fn(top->mname);
    if (!fmname || !*fmname) return none;
    int mro_len = rd_mro_len(top->cname);
    const char *mro[mro_len];
    { int got = dat_mro(top->cname, mro, mro_len); if (got <= 0) { mro[0] = top->cname; mro_len = 1; } else mro_len = got; }
    int next = -1;
    for (int i = top->found_idx + 1; i < mro_len && next < 0; i++) {
        char cand[fmt_len("%s__%s", mro[i], fmname)];
        snprintf(cand, sizeof cand, "%s__%s", mro[i], fmname);
        if (meth_is_user_proc(cand)) next = i;
    }
    if (next < 0) return none;
    char nproc[fmt_len("%s__%s", mro[next], fmname)];
    snprintf(nproc, sizeof nproc, "%s__%s", mro[next], fmname);
    {
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, nproc)) break;
        if (pi < g_stage2.proc_count && !rt_proc_has_native_fn(nproc)) return none;
    }
    if (!rt_define_returns_by_frame(nproc)) return none;
    rec->self = top->self;
    rec->mname = top->mname;
    rec->cname = top->cname;
    rec->found_idx = next;
    rec->local = 0;
    if (with) {
        DESCR_t *argv = (DESCR_t *)((char *)rec - 16 * nargs);
        if (argv != args) for (int k = 0; k < nargs; k++) argv[k] = args[k];
        rec->args = argv;
        rec->nargs = 1 + nargs;
        rec->skip_mname = 2;
    } else {
        rec->args = top->args;
        rec->nargs = top->nargs;
        rec->skip_mname = top->skip_mname;
    }
    rec->prev = g_redisp_cur;
    g_redisp_cur = rec;
    rt_call_args_need(rec->nargs);
    for (int k = 0; k < rec->nargs; k++) CALL_ARGS[k] = rd_arg(rec, k);
    { rt_call_next_t n = rt_call_open_by_name(nproc, rec->nargs); if (!n.fn) { g_redisp_cur = rec->prev; return none; } return rk_open_pinned_block(n, nproc, rec->nargs); }
}
rt_call_next_t rk_callsame_open(DESCR_t *args, int nargs, void *recp) { return rk_redispatch_open(args, nargs, recp, 0); }
rt_call_next_t rk_callwith_open(DESCR_t *args, int nargs, void *recp) { return rk_redispatch_open(args, nargs, recp, 1); }
DESCR_t rt_apply_land_ω(long word, const long *sv) { extern DESCR_t rt_call_land_ω(long); rt_apply_stno_land(sv); return rt_call_land_ω(word); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_method_land_γ(DESCR_t frame0, long word) {
    extern DESCR_t rt_call_land_γ(DESCR_t, long);
    DESCR_t r = rt_call_land_γ(frame0, word);
    if (g_redisp_cur) g_redisp_cur = g_redisp_cur->prev;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_method_land_ω(long word) { extern DESCR_t rt_call_land_ω(long); DESCR_t r = rt_call_land_ω(word); if (g_redisp_cur) g_redisp_cur = g_redisp_cur->prev; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bnd_tweak_cache_clear(void) { g_rt_bn.tweak_gen_bump++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_edin_switch(DESCR_t *args, pl_tr_ctx_t *cx, int is_out, const char *fmode) {
    extern int fh_alloc(FILE *);
    extern void fh_set_output(int);
    extern void fh_set_input(int);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    char fb[4096];
    const char *fn;
    FILE *fp;
    int idx;
    DESCR_t f = rt_pl_deref_val(args[0]);
    if (pl_val_unbound(f)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
    if ((int)f.v == DT_PLREF && plc_fid_name(f.slen) == prolog_atom_intern("$stream") && plc_fid_arity(f.slen) == 1) {
        extern FILE *fh_get(int);
        DESCR_t sa = rt_pl_deref_val(((DESCR_t *)f.p)[0]);
        if (sa.v != DT_I || (int)sa.i < 0 || (int)sa.i >= FH_N || !fh_get((int)sa.i)) { cx->ball = rt_pl_ball_kind2("existence_error", "stream", f); return 0; }
        if (is_out) { fh_set_output((int)sa.i); g_rt_bn.pl_edin_out_name = ""; } else { fh_set_input((int)sa.i); g_rt_bn.pl_edin_in_name = ""; }
        return 1;
    }
    if (!pl_cell_text(args[0], fb, sizeof fb, &fn)) { cx->ball = rt_pl_ball_kind2("domain_error", "source_sink", f); return 0; }
    if (!strcmp(fn, "user")) { if (is_out) { fh_set_output(1); g_rt_bn.pl_edin_out_name = ""; } else { fh_set_input(0); g_rt_bn.pl_edin_in_name = ""; } return 1; }
    fp = fopen(fn, fmode);
    if (!fp) { cx->ball = rt_pl_ball_kind2("existence_error", "source_sink", f); return 0; }
    idx = fh_alloc(fp);
    if (idx < 0) { fclose(fp); return 0; }
    if (is_out) { fh_set_output(idx); g_rt_bn.pl_edin_out_name = rt_heap_strdup_c(fn); } else { fh_set_input(idx); g_rt_bn.pl_edin_in_name = rt_heap_strdup_c(fn); }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_edin_revert(int is_out) {
    extern FILE *fh_get(int);
    extern void fh_free(int);
    extern void fh_set_output(int);
    extern void fh_set_input(int);
    extern int fh_current_output(void);
    extern int fh_current_input(void);
    int cur = is_out ? fh_current_output() : fh_current_input();
    if (cur >= 3) { FILE *fp = fh_get(cur); if (fp) fclose(fp); fh_free(cur); }
    if (is_out) { fh_set_output(1); g_rt_bn.pl_edin_out_name = ""; } else { fh_set_input(0); g_rt_bn.pl_edin_in_name = ""; }
    return 1;
}
static const pl_flag_t pl_flags_init[] = { { "bounded", "false", 0, { 0 } }, { "integer_rounding_function", "toward_zero", 0, { 0 } }, { "max_arity", "2147483647", 0, { 0 } }, { "max_procedure_arity",
    "1024", 0, { 0 } }, { "max_char_code", "1114111", 0, { 0 } }, { "float_overflow", "error", 1, { "error", "infinity", 0 } }, { "float_zero_div", "error", 1, { "error", "infinity", 0 } },
    { "float_undefined", "error", 1, { "error", "nan", 0 } }, { "char_conversion", "off", 1, { "on", "off", 0 } }, { "debug", "off", 1, { "on", "off", 0 } }, { "unknown", "error", 1, { "error",
    "fail", "warning", 0 } }, { "double_quotes", "codes", 1, { "atom", "chars", "codes", "string", 0 } }, { "protect_static_code", "false", 1, { "true", "false", 0 } }, { "iso", "false", 1, { "true",
    "false", 0 } }, { "occurs_check", "false", 1, { "true", "false", "error", 0 } }, { "print_write_options", "", 1, { 0 }, 1, { 0 } }, { "encoding", "UTF-8", 1, { "UTF-8", "UTF-16BE", "UTF-16LE",
    "UTF-32BE", "UTF-32LE", 0 } }, { "argv", "[]", 0, { 0 } }, { "os_argv", "[]", 0, { 0 } }, { "dialect", "scrip", 0, { 0 } }, { "arch", "x86_64-linux", 0, { 0 } }, { "prolog_name", "SCRIP", 0,
    { 0 } }, { "prolog_version", "0.0.0", 0, { 0 } }, { "version_data", "", 0, { 0 } }, { "singleton_warning", "on", 1, { "on", "off", 0 } }, { "suspicious_warning", "on", 1, { "on", "off", 0 } },
    { "multifile_warning", "on", 1, { "on", "off", 0 } }, { "character_escapes_unicode", "true", 1, { "true", "false", 0 } }, { "$dialect", "iso", 1, { "iso", "swi", "gnu", "xsb", 0 } },
    { "prefer_rationals", "false", 1, { "true", "false", 0 } }, { "threads", "false", 0, { 0 } }, { "float_underflow", "ignore", 1, { "error", "ignore", 0 } }, { "float_rounding", "to_nearest", 1,
    { "to_nearest", "to_positive", "to_negative", "to_zero", 0 } }, { "max_tagged_integer", "72057594037927935", 0, { 0 } }, { "max_answers_for_subgoal", "infinite", 1, { 0 }, 0, { 0 }, 1, PLF_GONE },
    { "min_tagged_integer", "-72057594037927936", 0, { 0 } }, { "float_max", "", 0, { 0 } }, { "float_min", "", 0, { 0 } }, { "float_max_integer", "", 0, { 0 } }, { "stack_limit", "1073741824", 1,
    { 0 } }, { "rational_syntax", "compatibility", 1, { "natural", "compatibility", 0 } }, { "verbose", "silent", 1, { "silent", "normal", 0 } }, { "$apply_macros", "false", 1, { "true", "false",
    0 } }, { "color_term", "false", 1, { "true", "false", 0 } }, { "pipe", "true", 0, { 0 } }, { "pid", "", 0, { 0 } }, { 0, "", 0, { 0 } } };
static const pl_dia_flag_t pl_dia_flags[] = { { "xsb", "max_integer", "9223372036854775807" }, { "xsb", "min_integer", "-9223372036854775807" }, { "xsb", "bounded", "true" }, { "gnu", "max_integer",
    "1152921504606846975" }, { "gnu", "min_integer", "-1152921504606846976" }, { "gnu", "bounded", "true" }, { 0, 0, 0 } };
const char *pl_dia_flag(const char *nm) {
    for (int i = 0; pl_dia_flags[i].nm; i++) if (!strcmp(pl_dia_flags[i].nm, nm) && pl_float_flag("$dialect", pl_dia_flags[i].dia)) return pl_dia_flags[i].val;
    return (const char *)0;
}
void pl_flags_ready(void) {
    if (g_rt_bn.g_pl_flags.len) return;
    for (int i = 0; ; i++) { pl_flag_t *e = &CV_PUSH(g_rt_bn.g_pl_flags, pl_flag_t); *e = pl_flags_init[i]; if (!pl_flags_init[i].nm) break; }
}
static const char *rt_sno_patck_text(int code) {
    return code == 240 ? "pattern match right operand is not pattern" : code == 5 ? "alternation right operand is not pattern" : code == 6 ? "alternation left operand is not pattern" : code == 61 ?
        "arbno argument is not pattern" : code == 259 ? "fence argument is not pattern" : code == 30 ? "pattern assignment left operand is not pattern" : code == 25 ?
        "immediate assignment left operand is not pattern" : code == 8 ? "concatenation left operand is not a string or pattern" : "concatenation right operand is not a string or pattern";
}
DESCR_t rt_sno_patck_d(DESCR_t *args, int nargs) {
    extern void core_runtime_error(int, const char *);
    DESCR_t r = nargs > 0 ? args[0] : NULVCL;
    if (nargs < 2 || IS_FAIL_fn(r) || !(r.v == DT_A || r.v == DT_T || IS_DATA_INST_fn(r))) return r;
    int code = (int)to_int(args[1]);
    core_runtime_error(code, rt_sno_patck_text(code));
    return FAILDESCR;
}
DESCR_t rt_sno_chknm_d(DESCR_t *args, int nargs) {
    extern int rt_g_want_name;
    extern int rt_g_ret_by_name;
    extern int rt_proc_is_registered(const char *);
    extern void core_runtime_error(int, const char *);
    DESCR_t r = nargs > 0 ? args[0] : NULVCL;
    if (!rt_g_want_name && !(nargs > 1 && args[1].v == DT_S && args[1].s && !strcmp(args[1].s, "APPLY"))) { rt_g_ret_by_name = 0; return r; }
    rt_g_want_name = 0;
    const int byn = rt_g_ret_by_name;
    rt_g_ret_by_name = 0;
    const char * fnm = (nargs > 1 && args[1].v == DT_S && args[1].s) ? args[1].s : "";
    if (!byn && fnm[0] && rt_proc_is_registered(fnm)) {
        core_runtime_error(21, "function called by name returned a value");
        return FAILDESCR;
    } else if (nargs > 1 && r.v != DT_N && !IS_FAIL_fn(r) && strcmp(fnm, "EVAL") && strcmp(fnm, "ITEM")) {
        core_runtime_error(21, "function called by name returned a value");
        return FAILDESCR;
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_flag_name(int i) { pl_flags_ready(); return (i >= 0 && i + 1 < (int)g_rt_bn.g_pl_flags.len) ? pl_flags[i].nm : (const char *)0; }
void rt_pl_flags_gc_roots(void) {
    gc_roots_args_t a = { (void **)pl_flags, (long)g_rt_bn.g_pl_flags.len, (void **)0, 0, (const char *)0, 0, (const char *)0, 0, (const char *)0, (const char *)0 };
    rt_gc_run_flags_roots(&a);
}
void bnd_text_gc_roots(void) {
    static const char *const *const words[2] = { (const char *const *)&g_rt_bn.pl_edin_out_name, (const char *const *)&g_rt_bn.pl_edin_in_name };
    rt_gc_run_loc_list(words, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_flag_t * pl_flag_find_or_create(const char *nm) {
    int i = 0;
    pl_flags_ready();
    for (; pl_flags[i].nm; i++) if (!strcmp(nm, pl_flags[i].nm)) return &pl_flags[i];
    {
        extern char *rt_heap_strdup_c(const char *);
        char *persist = rt_heap_strdup_c(nm);
        if (!persist) return (pl_flag_t *)0;
        (void)CV_PUSH(g_rt_bn.g_pl_flags, pl_flag_t);
        pl_flags[i].nm = persist;
        pl_flags[i].val = "";
        pl_flags[i].mod = 1;
        pl_flags[i].allow[0] = 0;
    }
    return &pl_flags[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *const pl_flag_type_name[] = { "atom", "boolean", "atom", "integer", "float", "term", "integer" };
static int pl_flag_bool_text(const char *s, const char **norm) {
    if (!strcmp(s, "true") || !strcmp(s, "on")) { *norm = "true"; return 1; }
    if (!strcmp(s, "false") || !strcmp(s, "off")) { *norm = "false"; return 1; }
    return 0;
}
static int pl_flag_value_type(DESCR_t v) {
    const char *t, *n;
    if (v.v == (DTYPE_t)DT_PLATOM) return (t = pl_atom_str(v)) && pl_flag_bool_text(t, &n) ? PLF_BOOL : PLF_ATOM;
    if (v.v == DT_I) return PLF_INT;
    if (v.v == DT_R) return PLF_FLOAT;
    return PLF_TERM;
}
static int pl_flag_type_of(const pl_flag_t *fl) {
    int bools = 0, others = 0;
    const char *d, *n;
    if (fl->type) return fl->type;
    if (fl->is_term) return PLF_TERM;
    for (int i = 0; fl->allow[i]; i++) { if (pl_flag_bool_text(fl->allow[i], &n)) bools++; else others++; }
    if (bools && !others) return PLF_BOOL;
    d = fl->val ? fl->val : "";
    if (!fl->allow[0] && pl_flag_bool_text(d, &n)) return PLF_BOOL;
    if (*d == '-') d++;
    return *d && strspn(d, "0123456789") == strlen(d) ? PLF_INT : PLF_ATOM;
}
static const char *pl_flag_coerce(int type, DESCR_t v, const char **text) {
    extern void *rt_wsb_alloc(size_t);
    const char *s;
    char *buf;
    *text = (const char *)0;
    if (type == PLF_TERM) return (const char *)0;
    if (type == PLF_BOOL) return v.v == (DTYPE_t)DT_PLATOM && (s = pl_atom_str(v)) && pl_flag_bool_text(s, text) ? (const char *)0 : "bool";
    if (type == PLF_ATOM) { *text = v.v == (DTYPE_t)DT_PLATOM ? pl_atom_str(v) : (const char *)0; return *text ? (const char *)0 : "atom"; }
    if (type == PLF_INT) { if (v.v != DT_I) return "integer"; buf = (char *)rt_wsb_alloc(32); snprintf(buf, 32, "%lld", (long long)v.i); *text = buf; return (const char *)0; }
    if (v.v != DT_R && v.v != DT_I) return "float";
    buf = (char *)rt_wsb_alloc(40);
    snprintf(buf, 40, "%.17g", v.v == DT_R ? v.r : (double)v.i);
    *text = buf;
    return (const char *)0;
}
static int pl_flag_store(pl_flag_t *fl, void *cell, DESCR_t v, int type, int normalise, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern pl_cell_t rt_pl_cell_snapshot(void *);
    extern char *rt_heap_strdup_c(const char *);
    const char *text, *bad = pl_flag_coerce(type, v, &text);
    if (bad) { cx->ball = rt_pl_ball_kind2("type_error", bad, v); return 0; }
    if (type == PLF_TERM) {
        fl->term = rt_pl_cell_snapshot(cell);
        fl->val = "set";
        fl->is_term = 1;
    } else {
        if (!normalise && type == PLF_BOOL) text = pl_atom_str(v);
        fl->val = rt_heap_strdup_c(text);
        fl->is_term = 0;
    }
    fl->type = type;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_restraint_kind(const char *nm) {
    if (!strcmp(nm, "max_table_subgoal_size") || !strcmp(nm, "max_table_answer_size") || !strcmp(nm, "max_answers_for_subgoal")) return 1;
    if (!strcmp(nm, "max_table_subgoal_size_action")) return 2;
    return !strcmp(nm, "max_table_answer_size_action") || !strcmp(nm, "max_answers_for_subgoal_action") ? 3 : 0;
}
static int pl_restraint_action_ok(int kind, const char *t) { return !strcmp(t, "error") || !strcmp(t, "suspend") || !strcmp(t, "warning") || (kind == 3 && !strcmp(t, "bounded_rationality")); }
static int pl_restraint_set(const char *nm, int kind, void *cell, DESCR_t v, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    const char *t = v.v == (DTYPE_t)DT_PLATOM ? pl_atom_str(v) : (const char *)0;
    pl_flag_t *fl;
    if (kind == 1 && t && !strcmp(t, "infinite")) { if ((fl = pl_flag_find(nm))) fl->type = PLF_GONE; return 1; }
    if (kind == 1 && v.v != DT_I) { cx->ball = rt_pl_ball_kind2("type_error", "integer", v); return 0; }
    if (kind == 1 && v.i < 0) { cx->ball = rt_pl_ball_kind2("domain_error", "not_less_than_zero", v); return 0; }
    if (kind != 1 && !t) { cx->ball = rt_pl_ball_kind2("type_error", "atom", v); return 0; }
    if (kind != 1 && !pl_restraint_action_ok(kind, t)) { cx->ball = rt_pl_ball_kind2("domain_error", "restraint_action", v); return 0; }
    if (!(fl = pl_flag_find_or_create(nm))) { cx->ball = rt_pl_ball_kind2("domain_error", "prolog_flag", pl_mk_atom_dup(nm, strlen(nm))); return 0; }
    fl->user = 1;
    fl->mod = 1;
    return pl_flag_store(fl, cell, v, kind == 1 ? PLF_INT : PLF_ATOM, 1, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_round_mode(const char *v) {
    if (!v) return -1;
    return !strcmp(v, "to_nearest") ? FE_TONEAREST : !strcmp(v, "to_positive") ? FE_UPWARD : !strcmp(v, "to_negative") ? FE_DOWNWARD : !strcmp(v, "to_zero") ? FE_TOWARDZERO : -1;
}
static void pl_flag_effects(const pl_flag_t *fl, pl_tr_ctx_t *cx) {
    if (!strcmp(fl->nm, "float_rounding") && pl_round_mode(fl->val) >= 0) { fesetround(pl_round_mode(fl->val)); return; }
    if (strcmp(fl->nm, "occurs_check") || !cx || !cx->tr) return;
    *pl_tr_ocheck_slot(pl_tr_base_of(cx->tr)) = !strcmp(fl->val, "true") ? 1 : (!strcmp(fl->val, "error") ? 2 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(set_prolog_flag, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
        char fb[128], vb[128];
        const char *fn, *vn;
        DESCR_t f = rt_pl_deref_val(args[0]);
        DESCR_t v = rt_pl_deref_val(args[1]);
        pl_flag_t *fl;
        int kind;
        ok = 0;
        if (pl_val_unbound(f) || pl_val_unbound(v)) {
            cx->ball = rt_pl_ball_instantiation();
        } else if ((f.v != (DTYPE_t)DT_S && f.v != (DTYPE_t)DT_PLATOM) || !pl_cell_text(args[0], fb, sizeof fb, &fn)) {
            cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        } else if ((kind = pl_restraint_kind(fn)) && (pl_swi_dialect() || pl_flag_find(fn))) {
            ok = pl_restraint_set(fn, kind, &args[1], v, cx);
        } else if (!(fl = pl_flag_find(fn)) && !(pl_swi_dialect() && (fl = pl_flag_find_or_create(fn)) && (fl->user = 1))) {
            cx->ball = rt_pl_ball_kind2("domain_error", "prolog_flag", f);
        } else if (fl->type && !fl->mod) {
            cx->ball = rt_pl_ball_permission3("modify", "flag", f);
        } else if (fl->type) {
            ok = pl_flag_store(fl, &args[1], v, fl->type, 1, cx);
        } else if (fl->user) {
            ok = pl_flag_store(fl, &args[1], v, pl_flag_value_type(v), 0, cx);
            fl->mod = 1;
        } else if (fl->is_term) {
            extern pl_cell_t rt_pl_cell_snapshot(void *);
            extern void *rt_pl_ball_instantiation(void);
            DESCR_t bad = {0};
            int k = pl_write_opts_check(args[1], &bad);
            if (!k) { fl->term = rt_pl_cell_snapshot(&args[1]); fl->val = "set"; ok = 1; } else if (k == 1) cx->ball = rt_pl_ball_instantiation();
            else {
                DESCR_t *kk = (DESCR_t *)rt_ws_alloc_descr(2);
                extern int prolog_atom_intern(const char *);
                DESCR_t c = {0};
                kk[0] = pl_mk_atom_dup(fl->nm, strlen(fl->nm));
                kk[1] = rt_pl_deref_val(args[1]);
                c.v = (DTYPE_t)DT_PLREF;
                c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("+"), 2);
                c.p = (void *)kk;
                cx->ball = rt_pl_ball_kind2("domain_error", "flag_value", c);
            }
        } else if (!pl_cell_text(args[1], vb, sizeof vb, &vn)) {
            cx->ball = rt_pl_ball_kind2("domain_error", "flag_value", v);
        } else if (!fl->mod) {
            cx->ball = rt_pl_ball_permission3("modify", "flag", f);
        } else {
            int good = !fl->allow[0];
            if (!strcmp(fl->nm, "debug") && (!strcmp(vn, "true") || !strcmp(vn, "false"))) vn = vn[0] == 't' ? "on" : "off";
            for (int i = 0; fl->allow[i]; i++) if (!strcmp(vn, fl->allow[i])) good = 1;
            if (!good) {
                DESCR_t *kk = (DESCR_t *)rt_ws_alloc_descr(2);
                extern int prolog_atom_intern(const char *);
                kk[0] = pl_mk_atom_dup(fl->nm, strlen(fl->nm));
                kk[1] = rt_pl_deref_val(args[1]);
                {
                    DESCR_t c = {0};
                    c.v = (DTYPE_t)DT_PLREF;
                    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("+"), 2);
                    c.p = (void *)kk;
                    cx->ball = rt_pl_ball_kind2("domain_error", "flag_value", c);
                }
            } else {
                fl->val = rt_heap_strdup_c(vn);
                pl_flag_effects(fl, cx);
                ok = 1;
            }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(create_prolog_flag, 4)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        const char *fn, *tn, *an, *text;
        DESCR_t f = rt_pl_deref_val(args[0]), v = rt_pl_deref_val(args[1]);
        pl_flag_t *fl;
        int type;
        ok = 0;
        if (pl_val_unbound(f) || pl_val_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (f.v != (DTYPE_t)DT_PLATOM || !pl_cell_text(args[0], (char *)0, 0, &fn)) cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        else if (!pl_cell_text(args[2], (char *)0, 0, &tn) || !pl_cell_text(args[3], (char *)0, 0, &an)) cx->ball = rt_pl_ball_kind2("domain_error", "prolog_flag_type", rt_pl_deref_val(args[2]));
        else {
            int declared = strcmp(tn, "none") != 0;
            const char *bad;
            type = !strcmp(tn, "boolean") ? PLF_BOOL : !strcmp(tn, "atom") ? PLF_ATOM : !strcmp(tn, "integer") ? PLF_INT : !strcmp(tn, "float") ? PLF_FLOAT : !strcmp(tn, "term") ? PLF_TERM :
                pl_flag_value_type(v);
            if ((bad = pl_flag_coerce(type, v, &text))) cx->ball = rt_pl_ball_kind2("type_error", bad, v);
            else if (!(fl = pl_flag_find_or_create(fn))) cx->ball = rt_pl_ball_kind2("domain_error", "prolog_flag", f);
            else if (pl_flag_store(fl, &args[1], v, type, declared, cx)) { fl->user = 1; fl->mod = strcmp(an, "read") != 0; ok = 1; }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(flag_meta, 3)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        const char *fn, *dv;
        DESCR_t f = rt_pl_deref_val(args[0]);
        pl_flag_t *fl;
        ok = 0;
        if (pl_val_unbound(f)) cx->ball = rt_pl_ball_instantiation();
        else if ((f.v != (DTYPE_t)DT_S && f.v != (DTYPE_t)DT_PLATOM) || !pl_cell_text(args[0], (char *)0, 0, &fn)) cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        else if ((dv = pl_dia_flag(fn))) ok = plw_unify_vals(args[1], pl_mk_atom(dv[0] == 't' || dv[0] == 'f' ? "boolean" : "integer"), cx) && plw_unify_vals(args[2], pl_mk_atom("read"), cx);
        else if ((fl = pl_flag_find(fn))) ok = plw_unify_vals(args[1], pl_mk_atom(pl_flag_type_name[pl_flag_type_of(fl)]), cx) && plw_unify_vals(args[2], pl_mk_atom(fl->mod ? "write" : "read"), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(set_prolog_flag_declare, 2)
    {
        char fb[128], vb[128];
        const char *fn, *vn;
        pl_flag_t *fl;
        int kind;
        ok = 1;
        if (pl_cell_text(args[0], fb, sizeof fb, &fn) && (kind = pl_restraint_kind(fn)) && (pl_swi_dialect() || pl_flag_find(fn))) {
            ok = pl_restraint_set(fn, kind, &args[1], rt_pl_deref_val(args[1]), cx);
        } else if (pl_cell_text(args[0], fb, sizeof fb, &fn) && pl_cell_text(args[1], vb, sizeof vb, &vn)) {
            if (!strcmp(fn, "$tbl_hook")) {
                extern void pl_lower_hook_dyn_mark(const char *, int);
                const char *sl = strrchr(vn, '/');
                if (sl && sl > vn) { char nb[sl - vn + 1]; memcpy(nb, vn, (size_t)(sl - vn)); nb[sl - vn] = 0; pl_lower_hook_dyn_mark(nb, atoi(sl + 1)); }
            }
            fl = strcmp(fn, "$tbl_hook") ? pl_flag_find_or_create(fn) : (pl_flag_t *)0;
            if (fl && fl->user && (!fl->type || fl->type == PLF_TERM)) {
                extern pl_cell_t rt_pl_cell_snapshot(void *);
                fl->term = rt_pl_cell_snapshot(&args[1]);
                fl->val = "set";
                fl->is_term = 1;
                fl->mod = 1;
            } else if (fl && fl->mod) {
                fl->val = rt_heap_strdup_c(vn);
                pl_flag_effects(fl, cx);
            }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(print, 1)
    {
        extern void *rt_pl_write_term_cell(void *, void *, void *);
        DESCR_t o = pl_flag_term_value(pl_flag_find("print_write_options"));
        cx->ball = rt_pl_write_term_cell(&args[0], &o, cx);
        ok = !cx->ball;
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_OUT_CX_LEAF(print, 2)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(tell, 1) ok = pl_edin_switch(args, cx, 1, "w"); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(append1, 1) ok = pl_edin_switch(args, cx, 1, "a"); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(see, 1) ok = pl_edin_switch(args, cx, 0, "r"); PL_CX_LEAF_TAIL
DESCR_t dop_pl_told(DESCR_t *args, int nargs) { (void)args; if (nargs != 0) return FAILDESCR; pl_edin_revert(1); return pl_ok(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bt___rk_meth_nc(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs < 4) return 0;
    extern const char *rt_proc_pname(const char *name, int k);
    extern int rt_proc_named_rest(const char *name);
    extern int rt_proc_is_variadic(const char *name);
    DESCR_t inv = args[0];
    char mb[256];
    const char *mname = to_cstring(args[1], mb, sizeof mb);
    int npos = IS_INT_fn(args[2]) ? (int) args[2].i : 0;
    int nold = IS_INT_fn(args[3]) ? (int) args[3].i : 0;
    if (npos < 0 || 4 + npos > nargs) npos = 0;
    int nnm = (nargs - 4 - npos) / 2;
    if (nold < 0 || nold > nnm) nold = nnm;
    const char *cname = (IS_DATA_INST_fn(inv) && inv.u && inv.u->type) ? inv.u->type->name : (IS_STR_fn(inv) && inv.s && dat_find_type(inv.s)) ? inv.s : NULL;
    if (cname && mname) {
        const char *rmc = resolve_method_chain(cname, mname, NULL);
        char proc[fmt_len("%s__%s", rmc, mname)];
        snprintf(proc, sizeof proc, "%s__%s", rmc, mname);
        if (meth_is_user_proc(proc) && !rt_proc_is_variadic(proc)) {
            int np = 0;
            while (rt_proc_pname(proc, np)) np++;
            int nrest = rt_proc_named_rest(proc), nrx = nrest > 0 ? nrest - 1 : -1;
            int nsl = 1 + npos > np ? 1 + npos : np;
            if (nrx + 1 > nsl) nsl = nrx + 1;
            DESCR_t slots[nsl];
            for (int i = 0; i < nsl; i++) slots[i] = NULVCL;
            slots[0] = inv;
            int maxslot = 1;
            DESCR_t hh = NULVCL;
            for (int i = 0; i < npos; i++) { slots[1 + i] = args[4 + i]; if (2 + i > maxslot) maxslot = 2 + i; }
            for (int i = 0; i < nnm; i++) {
                DESCR_t kd = args[4 + npos + 2 * i], vd = args[4 + npos + 2 * i + 1];
                char kb[128];
                const char *k = to_cstring(kd, kb, sizeof kb);
                int found = -1;
                for (int sl = 1; sl < np; sl++) { if (sl == nrx) continue; const char *pn = rt_proc_pname(proc, sl); if (pn && k && !strcmp(pn, k)) { found = sl; break; } }
                if (found < 0) { if (nrx >= 0) hh = rk_hash_store(hh, kd, vd); continue; }
                slots[found] = vd;
                if (found + 1 > maxslot) maxslot = found + 1;
            }
            if (nrx >= 0) { slots[nrx] = hh.v == DT_T ? hh : TABLE_VAL(table_new()); if (nrx + 1 > maxslot) maxslot = nrx + 1; }
            *out = invoke_method_proc_rq(proc, slots, np > maxslot ? np : maxslot, rq);
            return 1;
        }
    }
    extern int script_try_call_builtin_by_name_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
    DESCR_t fwd[nargs];
    int fn = 0;
    fwd[fn++] = inv;
    fwd[fn++] = args[1];
    for (int i = 0; i < npos; i++) fwd[fn++] = args[4 + i];
    for (int i = 0; i < nnm; i++) { if (i < nold) fwd[fn++] = args[4 + npos + 2 * i]; fwd[fn++] = args[4 + npos + 2 * i + 1]; }
    return script_try_call_builtin_by_name_rq("meth_call", fwd, fn, out, rq);
}
DESCR_t bt___rk_exc_umeth(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *mn = rk_cstr(args[1]);
        if (!rk_exc_is(args[0]) || !mn || !*mn) { return FAILDESCR; }
        const char *rmc = resolve_method_chain(args[0].u->type->name, mn, NULL);
        char proc[fmt_len("%s__%s", rmc, mn)];
        snprintf(proc, sizeof proc, "%s__%s", rmc, mn);
        return meth_is_user_proc(proc) ? INTVAL(1) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_tl_got(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 2) { *out = rk_mk_arr(args, 2); ((ARBLK_t *) out->arr)->proto = rk_proto_tlgot; return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t rt_main_args_fetch(void) { rt_main_args_bind(); return g_rt_bn.main_args_descr; }
DESCR_t rt_sno_pbalt_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_alt(DESCR_t, DESCR_t);
    RT_HOOK_GUARD("SNO$PBALT");
    if (nargs != 2) return FAILDESCR;
    DESCR_t lc[2] = { args[0], INTVAL(6) };
    DESCR_t rc[2] = { args[1], INTVAL(5) };
    if (IS_FAIL_fn(rt_sno_patck_d(lc, 2)) || IS_FAIL_fn(rt_sno_patck_d(rc, 2))) return FAILDESCR;
    return pat_alt(args[0], args[1]);
}
