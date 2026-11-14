#include "runtime/lib/bn_head_call_args.h"
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_icn_opproc.h"
#include "runtime/lib/bn_head_icn_opgen.h"
#include "runtime/lib/bn_head_icn_opgen_bang.h"
#include "runtime/lib/bn_head_bn_win.h"
#include "runtime/core/core_error.h"
#include "runtime/core/icn_builtins.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom_prolog_max_arity.h"
#include "runtime/builtins/gen_value.h"
#include "runtime/core/runtime_shim.h"
#include "runtime/builtin_ids.h"
#include "runtime/rt/rt_list_view.h"
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_26_sh.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/core/g_runtime_bn.h"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_rt_asm_helpers_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_runtime_init_api.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/gc_heap_rt_gc_callback.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/builtins/gen.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern int rt_icn_cset_member_n(const char *, long, int);
int core_icn_error(int code, DESCR_t val);
extern gv_t g_call_args;
void rt_call_args_need(int n);
void rt_call_args_clear_from(int n);
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
extern DESCR_t rt_deref(DESCR_t d);
extern DESCR_t rt_proc_resume_frame_h(void **);
extern void rt_proc_drop_frame_h(void **);
static int bn_icn_upto_resume(DESCR_t *args, int nargs, int64_t *resume, DESCR_t *out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_procval_resolves_builtin(DESCR_t v) {
    extern int rt_proc_is_registered(const char *);
    if (IS_PROCVAL_BUILTIN_fn(v)) return 1;
    if (!v.s) return 1;
    return !(rt_proc_is_registered(v.s) || !strcmp(v.s, "main"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_procval_same(DESCR_t a, DESCR_t b) {
    if (!IS_PROCVAL_fn(a) || !IS_PROCVAL_fn(b)) return 0;
    if (!a.s || !b.s || strcmp(a.s, b.s) != 0) return 0;
    return rt_procval_resolves_builtin(a) == rt_procval_resolves_builtin(b);
}
static const icn_opproc_t *icn_opproc_table(int *n) {
    static const icn_opproc_t t[] = { {"+",1},{"-",1},{"*",1},{"/",1},{"\\",1},{"=",1},{"?",1},{"~",1},{"!",1},{"^",1},{".",1}, {"+",2},{"-",2},{"*",2},{"/",2},{"%",2},{"^",2},{"=",2},{"~=",2},{"<",
        2},{"<=",2},{">",2},{">=",2},{"==",2},{"~==",2},{"<<",2},{"<<=",2},{">>",2},{">>=",2}, {"===",2},{"~===",2},{"||",2},{"|||",2},{"++",2},{"--",2},{"**",2},{"[]",2},{":=",2},{"<-",2},{":=:",2},
        {"<->",2}, {"...",3},{"[:]",3}};
    *n = (int)(sizeof t / sizeof t[0]);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *icn_opproc_find(const char *nm, int arity) {
    int n;
    const icn_opproc_t *t = icn_opproc_table(&n);
    for (int i = 0; i < n; i++) if (t[i].ar == arity && !strcmp(t[i].opsym, nm)) return t[i].opsym;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_opproc_arity(const char *s) {
    int n;
    const icn_opproc_t *t = icn_opproc_table(&n);
    if (!s || (uintptr_t)s < (uintptr_t)t[0].opsym || (uintptr_t)s > (uintptr_t)t[n - 1].opsym) return -1;
    for (int i = 0; i < n; i++) if (s == t[i].opsym) return t[i].ar;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_opproc_typed(const char *nm, DESCR_t *argv, int n, DESCR_t *out) {
    extern void c_rt_coerce_num2_d(const DESCR_t *, const DESCR_t *, DESCR_t *, long);
    extern DESCR_t c_str_concat_fracdigit_d(DESCR_t, DESCR_t);
    extern DESCR_t rt_num_arith_strict(DESCR_t, DESCR_t, int);
    extern DESCR_t c_rt_div_strict(DESCR_t, DESCR_t);
    extern DESCR_t c_rt_mod_strict(DESCR_t, DESCR_t);
    extern DESCR_t rt_pow_strict(DESCR_t, DESCR_t);
    if (icn_opproc_arity(nm) != n) return 0;
    if (n == 2 && !strcmp(nm, "||")) { *out = RT_GC_CALLBACK(c_str_concat_fracdigit_d(argv[0], argv[1])); return 1; }
    int bc = !strcmp(nm, "+") ? BINOP_ADD : !strcmp(nm, "-") ? BINOP_SUB : !strcmp(nm, "*") ? BINOP_MUL : !strcmp(nm, "/") ? BINOP_DIV : !strcmp(nm, "%") ? BINOP_MOD : !strcmp(nm, "^") ? BINOP_POW :
        -1;
    DESCR_t l, r;
    if (bc < 0 || (n == 1 && bc != BINOP_ADD && bc != BINOP_SUB)) return 0;
    if (n == 1) {
        c_rt_coerce_num2_d(&argv[0], &argv[0], &r, 102 | COERCE_ERR_FAILURE_CONVERTIBLE | (bc == BINOP_SUB ? COERCE_OP_UNARY_NEG : COERCE_OP_UNARY_POS));
        *out = IS_FAIL_fn(r) ? FAILDESCR : (bc == BINOP_SUB) ? RT_GC_CALLBACK(rt_num_arith_strict(INTVAL(0), r, BINOP_SUB)) : r;
        return 1;
    }
    c_rt_coerce_num2_d(&argv[0], &argv[1], &l, 102 | COERCE_ERR_FAILURE_CONVERTIBLE | COERCE_OP_TAG(bc));
    if (IS_FAIL_fn(l)) { *out = FAILDESCR; return 1; }
    c_rt_coerce_num2_d(&argv[1], &argv[0], &r, 102 | COERCE_ERR_FAILURE_CONVERTIBLE | (bc == BINOP_POW ? COERCE_KEEP_INT : 0) | COERCE_OP_TAG(bc) | COERCE_OP_SELF_RIGHT);
    if (IS_FAIL_fn(r)) { *out = FAILDESCR; return 1; }
    *out = RT_GC_CALLBACK(bc == BINOP_DIV ? c_rt_div_strict(l, r) : bc == BINOP_MOD ? c_rt_mod_strict(l, r) : bc == BINOP_POW ? rt_pow_strict(l, r) : rt_num_arith_strict(l, r, bc));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_opproc_fit(const char *nm, DESCR_t **argv, int *n) {
    extern void *rt_ws_alloc_descr(size_t);
    int oa = icn_opproc_arity(nm);
    if (oa <= 0 || *n == oa) return;
    if (*n > oa) { *n = oa; return; }
    DESCR_t *pv = (DESCR_t *)rt_ws_alloc_descr((size_t)oa);
    for (int k = 0; k < oa; k++) pv[k] = (k < *n) ? (*argv)[k] : NULVCL;
    *argv = pv;
    *n = oa;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * procval_name(DESCR_t v) {
    if (v.v != DT_E) return 0;
    if (IS_PROCVAL_fn(v)) return v.s;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].entry_pc == (int)v.i) return g_stage2.proc_table[i].name;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *icn_opgen_scan_name(const char *nm) {
    if (!nm) return 0;
    if (!strcmp(nm, "find")) return "find";
    if (!strcmp(nm, "upto")) return "upto";
    if (!strcmp(nm, "bal")) return "bal";
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_opgen_pump(ICN_OPGEN_t *g, DESCR_t *argv, int n) {
    extern int list_bang_at(DESCR_t, int64_t *, DESCR_t *);
    extern DESCR_t rt_list_bang_key_at(DESCR_t, int64_t *);
    extern DESCR_t rt_call_arr_gen_strict(const char *, DESCR_t *, int, int64_t *);
    DESCR_t out;
    if (!g) return FAILDESCR;
    if (g->kind == ICN_OPGEN_TOBY) { if (g->step > 0 ? g->cur > g->lim : g->cur < g->lim) return FAILDESCR; out = INTVAL(g->cur); g->cur += g->step; return out; }
    if (g->kind == ICN_OPGEN_CURSOR) { extern uint64_t *rt_co_env(void); rt_co_env()[1] = (uint64_t)(int64_t)(g->cur - 1); return FAILDESCR; }
    if (g->kind == ICN_OPGEN_FNAMES) { const char *f = icn_builtin_nth((int)g->idx); if (!f) return FAILDESCR; g->idx++; return STRVAL(f); }
    if (g->kind == ICN_OPGEN_SCAN) { int hi = (g->nm[0] == 'b') ? 6 : 4; return RT_GC_CALLBACK(rt_call_arr_gen_strict(g->nm, argv, n > hi ? hi : n, &g->idx)); }
    if (n < 1) return FAILDESCR;
    if (g->kind == ICN_OPGEN_KEY) { out = rt_list_bang_key_at(argv[0], &g->idx); if (IS_FAIL_fn(out)) return FAILDESCR; g->idx++; return out; }
    if (!list_bang_at(argv[0], &g->idx, &out)) return FAILDESCR;
    g->idx++;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ICN_OPGEN_t *icn_opgen_new(int kind, const char *nm) { ICN_OPGEN_t *g = (ICN_OPGEN_t *)ct_zalloc(1, sizeof *g); if (g) { g->magic = ICN_OPGEN_MAGIC; g->kind = kind; g->nm = nm; } return g; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_opgen_start(ICN_OPGEN_t *g, DESCR_t *argv, int n, void **hslot) {
    if (!g) return FAILDESCR;
    DESCR_t first = icn_opgen_pump(g, argv, n);
    if (IS_FAIL_fn(first) || !hslot) { ct_drop(g); return first; }
    *hslot = (void *)g;
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ICN_OPGEN_t *icn_opgen_seq(DESCR_t *argv, int n) {
    extern int64_t core_icn_to_int_d(DESCR_t);
    extern int core_icn_by_zero_check(int64_t);
    int64_t lo = (n > 0 && argv[0].v != DT_SNUL && argv[0].v != 0) ? core_icn_to_int_d(argv[0]) : 1;
    int64_t st = (n > 1 && argv[1].v != DT_SNUL && argv[1].v != 0) ? core_icn_to_int_d(argv[1]) : 1;
    core_icn_by_zero_check(st);
    ICN_OPGEN_t *g = icn_opgen_new(ICN_OPGEN_TOBY, 0);
    if (g) { g->cur = lo; g->step = st; g->lim = st > 0 ? INT64_MAX : INT64_MIN; }
    return g;
}
static int icn_opproc_assigns(const char *nm) { if (icn_opproc_arity(nm) != 2) return 0; return (!strcmp(nm, ":=") || !strcmp(nm, "<-")) ? 1 : (!strcmp(nm, ":=:") || !strcmp(nm, "<->")) ? 2 : 0; }
static void icn_callee_resolve_op(DESCR_t *callee, const char **nm, int n) {
    if (IS_PROCVAL_fn(*callee) || !*nm) return;
    const char *op = icn_opproc_find(*nm, n);
    if (!op || !strcmp(op, "...")) return;
    *callee = (DESCR_t){0};
    callee->v = DT_E;
    callee->slen = 0xFFFFFFFEu;
    callee->s = (char *)op;
    *nm = op;
}
static void icn_call_value_deref_args(const char *nm, DESCR_t *argv, int n) {
    if (nm && !strcmp(nm, "name")) return;
    for (int k = icn_opproc_assigns(nm); k < n; k++) if (argv[k].v == DT_N) argv[k] = rt_deref(argv[k]);
}
static int icn_call_value_name_invocable(DESCR_t callee, const char *nm, int n) {
    if (IS_PROCVAL_fn(callee) || !IS_STR_fn(callee) || !nm) return 1;
    DESCR_t pa[2], pv = FAILDESCR;
    pa[0] = STRVAL((char *)nm);
    pa[1] = INTVAL(n);
    if (!try_call_builtin_by_name_bl_s("proc", pa, 2, &pv, -1, 1)) return 1;
    return !IS_FAIL_fn(pv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_call_value_not_invocable(DESCR_t callee, DESCR_t *argv, int n) {
    icn_bi_rec_t bi;
    core_icn_bi_push(&bi, (const char *)0, argv, n);
    bi.callee = callee;
    core_icn_error(106, callee);
    core_icn_bi_pop(&bi);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * icn_callee_name(DESCR_t callee) {
    if (callee.v != DT_S || !callee.slen || callee.slen == 0xFFFFFFFFu || !callee.s[callee.slen]) return callee.s;
    char * b = rt_str_alloc((int) callee.slen);
    memcpy(b, callee.s, (size_t) callee.slen);
    b[callee.slen] = 0;
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n) {
    if (callee.v == DT_R) callee = INTVAL((int64_t)callee.r);
    if (IS_INT_fn(callee)) { icn_call_value_deref_args(NULL, argv, n); long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = icn_callee_name(callee);
    if (!nm) return icn_call_value_not_invocable(callee, argv, n);
    icn_callee_resolve_op(&callee, &nm, n);
    if (IS_PROCVAL_fn(callee)) icn_opproc_fit(nm, &argv, &n);
    icn_call_value_deref_args(nm, argv, n);
    if (IS_PROCVAL_EXTERNAL_fn(callee)) { extern DESCR_t rt_extfn_invoke(DESCR_t, DESCR_t *, int); return rt_extfn_invoke(callee, argv, n); }
    if (IS_PROCVAL_fn(callee)) { DESCR_t ov; if (icn_opproc_typed(nm, argv, n, &ov)) return ov; }
    if (!IS_PROCVAL_BUILTIN_fn(callee) && (rt_proc_is_registered(nm) || !strcmp(nm, "main"))) {
        extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        rt_call_args_need(n);
        for (int k = 0; k < n; k++) CALL_ARGS[k] = argv[k];
        rt_call_args_clear_from(n);
        return RT_GC_CALLBACK(rt_call_proc_descr(nm, n));
    }
    if (n == 3 && !strcmp(nm, "...")) {
        extern int core_icn_by_zero_check(int64_t);
        extern int64_t core_icn_to_int_d(DESCR_t);
        int64_t lo = core_icn_to_int_d(argv[0]), hi = core_icn_to_int_d(argv[1]), st = core_icn_to_int_d(argv[2]);
        core_icn_by_zero_check(st);
        if (st > 0 ? lo > hi : lo < hi) return FAILDESCR;
        return INTVAL(lo);
    }
    if (n == 1 && !strcmp(nm, "!")) { extern int list_bang_at(DESCR_t, int64_t *, DESCR_t *); DESCR_t out; int64_t c0 = 0; return list_bang_at(argv[0], &c0, &out) ? out : FAILDESCR; }
    if (n == 1 && !strcmp(nm, "/")) return (argv[0].v == DT_SNUL || argv[0].v == 0) ? argv[0] : FAILDESCR;
    if (n == 2 && IS_PROCVAL_fn(callee) && icn_opproc_assigns(nm)) {
        extern DESCR_t c_rt_assign_var_strict(DESCR_t, DESCR_t);
        extern DESCR_t rt_swap_var(DESCR_t, DESCR_t);
        return RT_GC_CALLBACK(icn_opproc_assigns(nm) == 1 ? c_rt_assign_var_strict(argv[0], argv[1]) : rt_swap_var(argv[0], argv[1]));
    }
    if (n == 1 && IS_PROCVAL_fn(callee) && icn_opproc_arity(nm) == 1) {
        if (!strcmp(nm, ".")) return argv[0];
        if (!strcmp(nm, "*")) { uint64_t w[2]; memcpy(w, &argv[0], sizeof w); return RT_GC_CALLBACK(rtx_try_rt_size_d(w[0], w[1])); }
        if (!strcmp(nm, "~")) { extern DESCR_t rt_cset_compl(DESCR_t); return RT_GC_CALLBACK(rt_cset_compl(argv[0])); }
        if (!strcmp(nm, "^")) return RT_GC_CALLBACK(rt_call_arr_strict("ICN$REFRESH", argv, 1));
        if (!strcmp(nm, "=")) { DESCR_t m = RT_GC_CALLBACK(rt_call_arr_strict("match", argv, 1)); if (IS_FAIL_fn(m)) return FAILDESCR; return RT_GC_CALLBACK(rt_call_arr_strict("tab", &m, 1)); }
    }
    if (!strcmp(nm, "seq")) return icn_opgen_start(icn_opgen_seq(argv, n), argv, n, 0);
    if (n == 0 && !strcmp(nm, "function")) return icn_opgen_start(icn_opgen_new(ICN_OPGEN_FNAMES, 0), argv, n, 0);
    if (!icn_call_value_name_invocable(callee, nm, n)) return icn_call_value_not_invocable(callee, argv, n);
    return RT_GC_CALLBACK(rt_call_arr_strict(nm, argv, n));
}
DESCR_t rt_call_value_gen_h(DESCR_t callee, DESCR_t *argv, int n, void **hslot) {
    { extern void rt_proc_drop_frame_h(void **hslot); rt_proc_drop_frame_h(hslot); }
    if (callee.v == DT_R) callee = INTVAL((int64_t)callee.r);
    if (IS_INT_fn(callee)) { icn_call_value_deref_args(NULL, argv, n); long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = icn_callee_name(callee);
    if (!nm) return icn_call_value_not_invocable(callee, argv, n);
    icn_callee_resolve_op(&callee, &nm, n);
    if (IS_PROCVAL_fn(callee)) icn_opproc_fit(nm, &argv, &n);
    icn_call_value_deref_args(nm, argv, n);
    if (IS_PROCVAL_EXTERNAL_fn(callee)) { extern DESCR_t rt_extfn_invoke(DESCR_t, DESCR_t *, int); return rt_extfn_invoke(callee, argv, n); }
    if (!IS_PROCVAL_BUILTIN_fn(callee) && rt_proc_is_registered(nm)) {
        extern DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout, const uint64_t *regs);
        rt_call_args_need(n);
        for (int k = 0; k < n; k++) CALL_ARGS[k] = argv[k];
        rt_call_args_clear_from(n);
#if RT_DIAG
        rt_c2bb_hit("gen_h.call_value", nm);
#endif
        return rt_proc_call_gen_h(nm, n, hslot, (const uint64_t *)0);
    }
    if (n == 3 && !strcmp(nm, "...")) {
        extern int core_icn_by_zero_check(int64_t);
        extern int64_t core_icn_to_int_d(DESCR_t);
        int64_t lo = core_icn_to_int_d(argv[0]), hi = core_icn_to_int_d(argv[1]), st = core_icn_to_int_d(argv[2]);
        core_icn_by_zero_check(st);
        ICN_OPGEN_t *g = (ICN_OPGEN_t *)ct_zalloc(1, sizeof *g);
        if (!g) return FAILDESCR;
        g->magic = ICN_OPGEN_MAGIC;
        g->kind = ICN_OPGEN_TOBY;
        g->cur = lo;
        g->lim = hi;
        g->step = st;
        return icn_opgen_start(g, argv, n, hslot);
    }
    if (n == 1 && !strcmp(nm, "!")) return icn_opgen_start(icn_opgen_new(ICN_OPGEN_BANG, 0), argv, n, hslot);
    if (!strcmp(nm, "seq")) return icn_opgen_start(icn_opgen_seq(argv, n), argv, n, hslot);
    if (n == 0 && !strcmp(nm, "function")) return icn_opgen_start(icn_opgen_new(ICN_OPGEN_FNAMES, 0), argv, n, hslot);
    if (n == 1 && !strcmp(nm, "key")) return icn_opgen_start(icn_opgen_new(ICN_OPGEN_KEY, 0), argv, n, hslot);
    if ((n >= 1 || !strcmp(nm, "bal")) && icn_opgen_scan_name(nm)) return icn_opgen_start(icn_opgen_new(ICN_OPGEN_SCAN, icn_opgen_scan_name(nm)), argv, n, hslot);
    if (hslot && (!strcmp(nm, "tab") || !strcmp(nm, "move") || (n == 1 && IS_PROCVAL_fn(callee) && !strcmp(nm, "=")))) {
        extern uint64_t *rt_co_env(void);
        int64_t at = (int64_t)rt_co_env()[1] + 1;
        DESCR_t v = RT_GC_CALLBACK(rt_call_value(callee, argv, n));
        if (IS_FAIL_fn(v)) return v;
        ICN_OPGEN_t *g = icn_opgen_new(ICN_OPGEN_CURSOR, 0);
        if (g) { g->cur = at; *hslot = (void *)g; }
        return v;
    }
    if (!icn_call_value_name_invocable(callee, nm, n)) return icn_call_value_not_invocable(callee, argv, n);
    return RT_GC_CALLBACK(rt_call_value(callee, argv, n));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_apply_unpack(DESCR_t lv, DESCR_t *buf, int cap) {
    if (lv.v != DT_DATA) return -1;
    {
        DESCR_t *arr = (DESCR_t *)0;
        int ln = 0;
        if (rt_lv_is_list(lv, &arr, &ln)) { if (!arr) return -1; if (ln < 0) ln = 0; if (!buf) return ln; if (ln > cap) ln = cap; for (int k = 0; k < ln; k++) buf[k] = arr[k]; return ln; }
    }
    if (rt_data_is_record(lv)) {
        DATINST_t *di = (DATINST_t *)lv.u;
        int n = di->type->nfields;
        if (n < 0) n = 0;
        if (!buf) return n;
        if (n > cap) n = cap;
        for (int k = 0; k < n; k++) buf[k] = di->fields[k];
        return n;
    }
    return -1;
}
DESCR_t rt_call_apply_gen_h(DESCR_t callee, DESCR_t lv, void **hslot) {
    int an = rt_apply_unpack(lv, (DESCR_t *)0, 0);
    DESCR_t buf[an > 0 ? an : 1];
    int n = an < 0 ? an : rt_apply_unpack(lv, buf, an);
    if (n < 0) { core_icn_error(126, lv); return FAILDESCR; }
    return RT_GC_CALLBACK(rt_call_value_gen_h(callee, buf, n, hslot));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static FILE *cvprep_log(void) {
    static FILE *f = (FILE *)0;
    static int tried = 0;
    if (!tried) { const char *e = getenv("SCRIP_CV_PREP_LOG"); tried = 1; if (e && *e) { f = fopen(e, "a"); if (f) setvbuf(f, (char *)0, _IOLBF, 0); } }
    return f;
}
static int cvprep_trace_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_CV_PREP_TRACE"); p = ((e && *e && *e != '0') || cvprep_log()) ? 1 : 0; } return p; }
static void cvprep_say(const char *what, const char *why, const char *nm) {
    if (!cvprep_trace_on()) return;
    FILE *f = cvprep_log();
    if (!f) f = stderr;
    fprintf(f, "[CV-PREP] %s %s %s\n", what, why, nm ? nm : "(noname)");
    if (f == stderr) fflush(f);
}
static CVSPINE_t cvprep_decline(const char *why, const char *nm) { cvprep_say("decline", why, nm); return (CVSPINE_t){ 0, 0 }; }
#else
#define cvprep_say(what, why, nm) ((void)0)
#define cvprep_decline(why, nm) ((CVSPINE_t){ 0, 0 })
#endif
static CVSPINE_t rt_call_value_spine_prep_x(DESCR_t callee, DESCR_t *argv, int n, int blk_ok) {
    extern int rt_proc_jmp_entry(const char *name);
    extern void *rt_proc_fn(const char *name);
    extern long rt_proc_call_open(const char *name, int nargs);
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = icn_callee_name(callee);
    if (!nm) return cvprep_decline("noname", nm);
    icn_callee_resolve_op(&callee, &nm, n);
    icn_call_value_deref_args(nm, argv, n);
    if (IS_PROCVAL_BUILTIN_fn(callee)) return cvprep_decline("builtin", nm);
    if (IS_PROCVAL_EXTERNAL_fn(callee)) return cvprep_decline("external", nm);
    if (!rt_proc_is_registered(nm)) return cvprep_decline("unregistered", nm);
    if (!rt_proc_jmp_entry(nm)) return cvprep_decline("nojmpentry", nm);
    { extern int rt_proc_pinned(const char *); if (!blk_ok && rt_proc_pinned(nm)) return cvprep_decline("block", nm); }
    {
        extern int rt_proc_gen_region_ft(const char *);
        extern int rt_proc_pinned(const char *);
        if (rt_proc_gen_region_ft(nm) > 0 && (!blk_ok || !rt_proc_pinned(nm))) return cvprep_decline("genregionft", nm);
    }
    { rt_call_args_need(n); for (int k = 0; k < n; k++) CALL_ARGS[k] = argv[k]; rt_call_args_clear_from(n); }
    if (!rt_proc_call_open(nm, n)) return cvprep_decline("openfailed", nm);
    {
        extern int rt_proc_pinned(const char *);
        int gen = rt_proc_is_generator(nm), blk = rt_proc_pinned(nm) ? rt_proc_nparams(nm) : 0;
        long ftc = ((long)rt_proc_gen_region_ft(nm) + 15L) & ~15L;
        cvprep_say("open", ftc > 0 ? "spinen2" : gen ? "spine" : "spinedet", nm);
        if (ftc > 0) return (CVSPINE_t){ (long)(intptr_t)rt_proc_fn(nm), 3L | ((long)blk << 8) | (ftc << 32) };
        return (CVSPINE_t){ (long)(intptr_t)rt_proc_fn(nm), (gen ? 0 : 2) | ((long)(blk > 0 ? blk : 0) << 8) | ((long)((gen || rt_proc_dyn_scope(nm)) ? 1 : 0) << 16) };
    }
}
CVSPINE_t rt_call_value_spine_prep(DESCR_t callee, DESCR_t *argv, int n) { return RT_GC_CALLBACK(rt_call_value_spine_prep_x(callee, argv, n, 0)); }
CVSPINE_t rt_call_value_spine_prep_blk(DESCR_t callee, DESCR_t *argv, int n, void **hslot) {
    { extern void rt_proc_drop_frame_h(void **hslot); rt_proc_drop_frame_h(hslot); }
    return RT_GC_CALLBACK(rt_call_value_spine_prep_x(callee, argv, n, 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static CVSPINE_t rt_call_apply_spine_prep_x(DESCR_t callee, DESCR_t lv, int blk_ok) {
    int an = rt_apply_unpack(lv, (DESCR_t *)0, 0);
    DESCR_t buf[an > 0 ? an : 1];
    int n = an < 0 ? an : rt_apply_unpack(lv, buf, an);
    if (n < 0) return (CVSPINE_t){ 0, 0 };
    return RT_GC_CALLBACK(rt_call_value_spine_prep_x(callee, buf, n, blk_ok));
}
CVSPINE_t rt_call_apply_spine_prep(DESCR_t callee, DESCR_t lv) { return rt_call_apply_spine_prep_x(callee, lv, 0); }
CVSPINE_t rt_call_apply_spine_prep_blk(DESCR_t callee, DESCR_t lv, void **hslot) {
    { extern void rt_proc_drop_frame_h(void **hslot); rt_proc_drop_frame_h(hslot); }
    return rt_call_apply_spine_prep_x(callee, lv, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_key_live(char *key, size_t cap, void *root) {
    extern void *pl_runtime_define_goal_wrapper(const char *wkey, const char *nm, int ar, void *rt_root);
    size_t kn = strlen(key);
    char wkey[kn + 5];
    char nm[kn + 1];
    const char *sl;
    size_t kl;
    int ar;
    if (rt_proc_is_registered(key)) return 1;
    { extern int rt_pl_db_materialize_key(void *, const char *); if (rt_pl_db_materialize_key(root, key) && rt_proc_is_registered(key)) return 1; }
    snprintf(wkey, sizeof wkey, "$mc:%s", key);
    if (!rt_proc_is_registered(wkey)) {
        sl = strrchr(key, '/');
        if (!sl) return 0;
        kl = (size_t)(sl - key);
        memcpy(nm, key, kl);
        nm[kl] = 0;
        ar = atoi(sl + 1);
        if (!pl_runtime_define_goal_wrapper(wkey, nm, ar, root)) return 0;
    }
    snprintf(key, cap, "%s", wkey);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_key_is_control(const char *key) { return !strcmp(key, ",/2") || !strcmp(key, ";/2") || !strcmp(key, "->/2") || !strcmp(key, "*->/2") || !strcmp(key, "|/2"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *pl_goal_body_conv_ball(DESCR_t goal, const char *key, int n) {
    extern void *rt_pl_dop_goal_guard_c(DESCR_t *args, int nargs);
    DESCR_t gg = goal;
    if (n != 0 || !pl_goal_key_is_control(key)) return (void *)0;
    return rt_pl_dop_goal_guard_c(&gg, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_goal_stage(DESCR_t *kids, int ar, DESCR_t *argv, int n) {
    int k = 0;
    rt_call_args_need((ar > 0 ? ar : 0) + (n > 0 ? n : 0));
    for (int i = 0; i < ar; i++) CALL_ARGS[k++] = kids[i];
    for (int i = 0; i < n; i++) CALL_ARGS[k++] = argv[i];
    rt_call_args_clear_from(k);
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *pl_goal_arity_ball(void) { extern void *rt_pl_ball_kind1(const char *, const char *); return rt_pl_ball_kind1("representation_error", "max_procedure_arity"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CVSPINE_t rt_pl_goal_resolve_c(DESCR_t goal, int n, void **ball, void *root) {
    extern int rt_proc_jmp_entry(const char *name);
    extern void *rt_proc_fn(const char *name);
    extern int rt_proc_pinned(const char *name);
    extern int rt_proc_gen_region_ft(const char *);
    extern void *rt_pl_ball_existence_key(const char *key);
    extern void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar);
    extern void *rt_pl_dop_goal_guard_c(DESCR_t *args, int nargs);
    DESCR_t *kids = (DESCR_t *)0;
    int ar = 0;
    CVSPINE_t no = { 0, 0 };
    CVSPINE_t r;
    const char *gnm = rt_pl_goal_key(goal, n, &kids, &ar);
    char key[gnm ? fmt_len("$mc:%s/%d", gnm, ar + n) : 1];
    if (gnm) snprintf(key, sizeof key, "%s/%d", gnm, ar + n);
    if (!gnm) { DESCR_t gg = goal; void *gb = rt_pl_dop_goal_guard_c(&gg, 1); if (ball) *ball = gb ? gb : rt_pl_ball_type_pi("type_error", "callable", "?", 0); return no; }
    if (ar + n > PROLOG_MAX_PROCEDURE_ARITY) { if (ball) *ball = pl_goal_arity_ball(); return no; }
    { void *cb = pl_goal_body_conv_ball(goal, key, n); if (cb) { if (ball) *ball = cb; return no; } }
    if (!pl_goal_key_live(key, sizeof key, root) || !rt_proc_jmp_entry(key) || !rt_proc_is_generator(key) || !rt_proc_pinned(key) || rt_proc_gen_region_ft(key) > 0) {
        if (ball) *ball = rt_pl_ball_existence_key(key);
        return no;
    }
    r.fn = (long)(uintptr_t)rt_proc_fn(key);
    r.act0 = (long)ar;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx) {
    extern DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot, const uint64_t *regs);
    uint64_t regs[5];
    if (!cx)
        core_runtime_error(288,
        "the Prolog generator road was entered without an emitted context: a writer reached portray/1 from a road that carries no veneer frame (ARCH-PROLOG-C-OUT-OF-THE-BOX 8)");
    regs[0] = cx->rbx;
    regs[1] = (uint64_t)(uintptr_t)cx->tr;
    regs[2] = (uint64_t)(uintptr_t)cx->b;
    regs[3] = cx->r14;
    regs[4] = cx->r15;
    extern void *rt_pl_ball_existence_key(const char *key);
    extern void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar);
    DESCR_t *kids = (DESCR_t *)0;
    int ar = 0;
    { extern void rt_proc_drop_frame_h(void **hslot); rt_proc_drop_frame_h(hslot); }
    const char *gnm = rt_pl_goal_key(goal, n, &kids, &ar);
    size_t kcap = gnm ? (size_t)fmt_len("$mc:%s/%d", gnm, ar + n) : 1;
    char kfix[288];
    char *key = kcap <= sizeof kfix ? kfix : (char *)rt_wsb_alloc(kcap);
    if (kcap < sizeof kfix) kcap = sizeof kfix;
    if (gnm) snprintf(key, kcap, "%s/%d", gnm, ar + n);
    if (!gnm) {
        extern void *rt_pl_dop_goal_guard_c(DESCR_t *args, int nargs);
        DESCR_t gg = goal;
        void *gb = rt_pl_dop_goal_guard_c(&gg, 1);
        if (ball) { *ball = gb ? gb : rt_pl_ball_type_pi("type_error", "callable", "?", 0); return FAILDESCR; }
        {
            extern void rt_bomb(const char *msg);
            rt_bomb(
                "rt_pl_goal_gen_h_c: the no-ball path is UNREACHABLE BY CONSTRUCTION -- its one caller (unification.c, the C meta-call road) always passes a ball slot; since the meta-call row this i"
                "s a C-only road, never emitted.");
        }
        return FAILDESCR;
    }
    if (ar + n > PROLOG_MAX_PROCEDURE_ARITY && ball) { *ball = pl_goal_arity_ball(); return FAILDESCR; }
    { void *cb = pl_goal_body_conv_ball(goal, key, n); if (cb && ball) { *ball = cb; return FAILDESCR; } }
    if (!pl_goal_key_live(key, kcap, (void *)(uintptr_t)cx->r14)) { if (ball) { *ball = rt_pl_ball_existence_key(key); return FAILDESCR; } rt_pl_iso_throw_existence_key(key); return FAILDESCR; }
    rt_pl_goal_stage(kids, ar, argv, n);
#if RT_DIAG
    rt_c2bb_hit("gen_h.pl_goal", key);
#endif
    return rt_proc_call_gen_h(key, ar + n, hslot, regs);
}
DESCR_t rt_call_value_resume_h(void **hslot, int n) {
    extern DESCR_t rt_proc_resume_frame_h(void **hslot);
    if (!hslot || !*hslot) return FAILDESCR;
    DESCR_t *argv = (DESCR_t *)hslot - (n < 0 ? 1 : n);
    {
        ICN_OPGEN_t *g = (ICN_OPGEN_t *)*hslot;
        if (g->magic == ICN_OPGEN_MAGIC) {
            if (n < 0) {
                extern void *rt_ws_alloc_descr(size_t);
                int ln = argv ? rt_apply_unpack(argv[0], (DESCR_t *)0, 0) : -1;
                DESCR_t *buf = (ln > 0) ? (DESCR_t *)rt_ws_alloc_descr((size_t)ln) : (DESCR_t *)0;
                n = buf ? rt_apply_unpack(argv[0], buf, ln) : 0;
                if (n < 0) n = 0;
                argv = buf;
                icn_call_value_deref_args(g->nm, argv, n);
            }
            DESCR_t v = icn_opgen_pump(g, argv, n);
            if (IS_FAIL_fn(v)) { ct_drop(g); *hslot = (void *)0; }
            return v;
        }
    }
#if RT_DIAG
    rt_c2bb_hit("gen_h.resume_frame", "?");
#endif
    return rt_proc_resume_frame_h(hslot);
}
static DESCR_t rt_call_arr_gen_s(const char *fn, DESCR_t *args, int nargs, int64_t *resume, int strict) {
    DESCR_t out = FAILDESCR;
    if (fn && resume && fn[0] == 'u' && nargs >= 2 && nargs <= 3 && !strcmp(fn, "upto") && icn_dtax_plain(BID_upto) && bn_icn_upto_resume(args, nargs, resume, &out)) return out;
    if (fn && resume && nargs <= 6 && !strcmp(fn, "bal")) {
        if (*resume == (0x7FFFFFFFll << 32)) return FAILDESCR;
        if (*resume == 0) { icn_bi_rec_t bi; core_icn_bi_push(&bi, fn, args, nargs); int bad = icn_argtype_gate(BID_bal, args, nargs, &out, strict); core_icn_bi_pop(&bi); if (bad) return FAILDESCR; }
        if (bn_bal_gen(args, nargs, &out, resume) && !IS_FAIL_fn(out)) return out;
        return FAILDESCR;
    }
    if (fn && resume && nargs >= 1 && nargs <= 4 && (!strcmp(fn, "find") || !strcmp(fn, "upto"))) {
        if (*resume == 0) {
            icn_bi_rec_t bi;
            core_icn_bi_push(&bi, fn, args, nargs);
            int bad = icn_argtype_gate(fn[0] == 'f' ? BID_find : BID_upto, args, nargs, &out, strict);
            core_icn_bi_pop(&bi);
            if (bad) return FAILDESCR;
        }
        DESCR_t a4[4];
        a4[0] = args[0];
        a4[1] = (nargs > 1) ? args[1] : NULVCL;
        {
            const char *hs;
            int ni, nj;
            if (!bn_str_anal(args, nargs, 1, &hs, &ni, &nj)) return FAILDESCR;
            long i1 = ni;
            if (*resume > 0 && (long)*resume > i1) i1 = (long)*resume;
            a4[2] = INTVAL(i1);
            a4[3] = INTVAL(nj);
        }
        if (try_call_builtin_by_name_bl_s(fn, a4, 4, &out, -1, strict) && !IS_FAIL_fn(out)) { *resume = (int)out.i + 1; return out; }
        return FAILDESCR;
    }
    return RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, -1, strict, 0, (long *)0));
}
DESCR_t rt_call_arr_gen(const char *fn, DESCR_t *args, int nargs, int64_t *resume) { return RT_GC_CALLBACK(rt_call_arr_gen_s(fn, args, nargs, resume, 0)); }
DESCR_t rt_call_arr_gen_strict(const char *fn, DESCR_t *args, int nargs, int64_t *resume) { return RT_GC_CALLBACK(rt_call_arr_gen_s(fn, args, nargs, resume, 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_cvpos(long long pos, int len, int *out_p) { if (pos < -(long long)len || pos > (long long)len + 1) return 0; *out_p = (int)(pos > 0 ? pos : (long long)len + pos + 1); return 1; }
bn_win_t bn_win(void) {
    extern uint64_t *rt_co_env(void);
    bn_win_t w;
    w.env = rt_co_env();
    w.s = w.env[0] ? (const char *)(uintptr_t)w.env[0] : "";
    w.n = w.env[0] ? (long)(uint32_t)w.env[2] : 0;
    w.pos = (int)(int64_t)w.env[1] + 1;
    return w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_bal_gen(DESCR_t *args, int nargs, DESCR_t *out, int64_t *state) {
    const char *c1 = 0;
    int c1len = 0;
    int c1any = 0;
    if (nargs < 1 || IS_FAIL_fn(args[0]) || args[0].v == DT_SNUL) c1any = 1;
    else if (!cset_resolve(args[0], &c1, &c1len)) { *out = FAILDESCR; return 1; }
    const char *c2 = "(";
    int c2len = 1;
    const char *c3 = ")";
    int c3len = 1;
    if (nargs >= 2) { const char *v; int vlen; if (cset_resolve(args[1], &v, &vlen) && vlen > 0) { c2 = v; c2len = vlen; } }
    if (nargs >= 3) { const char *v; int vlen; if (cset_resolve(args[2], &v, &vlen) && vlen > 0) { c3 = v; c3len = vlen; } }
    const char *s;
    int slen;
    int p;
    if (nargs >= 4 && !IS_FAIL_fn(args[3]) && args[3].v != DT_SNUL) {
        s = VARVAL_fn(args[3]);
        if (!s) s = "";
        slen = (int)strlen(s);
        int i1 = (nargs >= 5 && (IS_INT_fn(args[4]) || IS_REAL_fn(args[4]))) ? (int)to_int(args[4]) : 1;
        int i2 = (nargs >= 6 && (IS_INT_fn(args[5]) || IS_REAL_fn(args[5]))) ? (int)to_int(args[5]) : slen + 1;
        if (i1 <= 0) i1 = slen + 1 + i1;
        if (i2 <= 0) i2 = slen + 1 + i2;
        if (i1 < 1 || i1 > slen + 1) { *out = FAILDESCR; return 1; }
        if (i2 - 1 < slen) slen = i2 - 1;
        p = i1 - 1;
    } else {
        bn_win_t w = bn_win();
        s = w.s;
        slen = (int)w.n;
        p = w.pos - 1;
    }
    long long cnt = 0;
    if (state && *state > 0) { p = (int)(*state & 0xFFFFFFFFll) - 1; cnt = (long long)(*state >> 32); }
    while (p < slen) {
        unsigned char ch = (unsigned char)s[p];
        if (cnt == 0 && (c1any || cset_has(c1, c1len, ch))) {
            long long nc = cnt + (cset_has(c2, c2len, ch) ? 1 : 0) - (cset_has(c3, c3len, ch) ? 1 : 0);
            if (state) *state = (nc < 0 ? 0x7FFFFFFFll << 32 : (nc << 32)) | (long long)(p + 2);
            *out = INTVAL(p + 1);
            return 1;
        }
        if (cset_has(c2, c2len, ch)) cnt++;
        else if (cset_has(c3, c3len, ch)) { cnt--; if (cnt < 0) { *out = FAILDESCR; return 1; } }
        p++;
    }
    *out = FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_str_anal(DESCR_t *args, int nargs, int si, const char **out_s, int *out_i, int *out_j) {
    int have_s = (nargs >= si + 1) && !IS_FAIL_fn(args[si]) && args[si].v != DT_SNUL;
    int have_i = (nargs >= si + 2) && !IS_FAIL_fn(args[si + 1]) && args[si + 1].v != DT_SNUL;
    int have_j = (nargs >= si + 3) && !IS_FAIL_fn(args[si + 2]) && args[si + 2].v != DT_SNUL;
    const char *s;
    int i, j, slen;
    long s_auth = -1;
    if (!have_s) {
        bn_win_t w = bn_win();
        s = w.s;
        i = w.pos;
        s_auth = w.n;
    } else {
        if (args[si].v == DT_S && args[si].s && args[si].slen != 0xFFFFFFFFu) { s = args[si].s; s_auth = (long)args[si].slen; } else { s = VARVAL_fn(args[si]); if (!s) s = ""; }
        i = 1;
    }
    slen = (s_auth >= 0) ? (int)s_auth : (int)strlen(s);
    if (have_i && !bn_cvpos((long long)to_int(args[si + 1]), slen, &i)) return 0;
    if (!have_j) j = slen + 1;
    else { if (!bn_cvpos((long long)to_int(args[si + 2]), slen, &j)) return 0; if (i > j) { int t = i; i = j; j = t; } }
    if (i < 1 || i > slen + 1 || j < 1 || j > slen + 1) return 0;
    *out_s = s;
    *out_i = i;
    *out_j = j;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_scan_span(DESCR_t *args, int nargs, const char **s, int *i1, int *i2) {
    if (nargs < 2 || nargs > 3 || args[1].v != DT_S || !args[1].s || args[1].slen == 0xFFFFFFFFu || IS_CSET_fn(args[1])) return 0;
    long long slen = args[1].slen, pos = 1;
    if (nargs == 3 && args[2].v != DT_SNUL) { if (args[2].v != DT_I) return 0; pos = args[2].i; }
    if (pos <= 0) pos += slen + 1;
    if (pos < 1 || pos > slen + 1) return 0;
    *s = args[1].s;
    *i1 = (int)pos;
    *i2 = (int)slen + 1;
    return 1;
}
static int bn_icn_upto_resume(DESCR_t *args, int nargs, int64_t *resume, DESCR_t *out) {
    const char *s;
    int i1, i2;
    if (args[0].v != DT_S || !bn_scan_span(args, nargs, &s, &i1, &i2)) return 0;
    const char *cv = VARVAL_fn(args[0]);
    if (!cv) { *out = FAILDESCR; return 1; }
    long cvn = IS_CSET_fn(args[0]) ? (long)descr_slen(args[0]) : (long)args[0].slen;
    const unsigned char *bits = IS_CSET_fn(args[0]) ? cset_blk_bits(cv) : (const unsigned char *)0;
    int p = (*resume > i1) ? (int)*resume : i1;
    if (bits) while (p < i2 && !((bits[(unsigned char)s[p - 1] >> 3] >> ((unsigned char)s[p - 1] & 7)) & 1)) p++;
    else while (p < i2 && !rt_icn_cset_member_n(cv, cvn, s[p - 1])) p++;
    if (p >= i2) { *out = FAILDESCR; return 1; }
    *out = INTVAL(p);
    *resume = (int64_t)p + 1;
    return 1;
}
int bn_icn_any(DESCR_t *args, int nargs, DESCR_t *out) {
    const char *cv = VARVAL_fn(args[0]);
    if (!cv) { *out = FAILDESCR; return 1; }
    long cvn = IS_CSET_fn(args[0]) ? (long)descr_slen(args[0]) : (args[0].v == DT_S) ? (long)args[0].slen : -1;
    const char *s;
    int i1, i2;
    if (!bn_scan_span(args, nargs, &s, &i1, &i2) && !bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
    if (i1 >= i2) { *out = FAILDESCR; return 1; }
    if (!rt_icn_cset_member_n(cv, cvn, s[i1 - 1])) { *out = FAILDESCR; return 1; }
    *out = INTVAL(i1 + 1);
    return 1;
}
int bn_icn_many(DESCR_t *args, int nargs, DESCR_t *out) {
    const char *cv = VARVAL_fn(args[0]);
    if (!cv) { *out = FAILDESCR; return 1; }
    long cvn = IS_CSET_fn(args[0]) ? (long)descr_slen(args[0]) : (args[0].v == DT_S) ? (long)args[0].slen : -1;
    const char *s;
    int i1, i2;
    if (!bn_scan_span(args, nargs, &s, &i1, &i2) && !bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
    const unsigned char *bits = IS_CSET_fn(args[0]) ? cset_blk_bits(cv) : (const unsigned char *)0;
    int p = i1;
    if (bits) while (p < i2 && ((bits[(unsigned char)s[p - 1] >> 3] >> ((unsigned char)s[p - 1] & 7)) & 1)) p++;
    else while (p < i2 && rt_icn_cset_member_n(cv, cvn, s[p - 1])) p++;
    if (p == i1) { *out = FAILDESCR; return 1; }
    *out = INTVAL(p);
    return 1;
}
int bn_icn_tab(DESCR_t *args, DESCR_t *out) {
    bn_win_t w = bn_win();
    int slen = (int)w.n;
    int target = (int)to_int(args[0]);
    if (target <= 0) target = slen + 1 + target;
    if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
    int old = w.pos;
    w.env[1] = (uint64_t)(int64_t)(target - 1);
    int lo = old < target ? old : target, hi = old < target ? target : old;
    int len = hi - lo;
    char *buf = rt_wsb_alloc(len + 1);
    memcpy(buf, w.s + lo - 1, len);
    buf[len] = '\0';
    *out = BSTRVAL(buf, len);
    return 1;
}
int bn_icn_move(DESCR_t *args, DESCR_t *out) {
    bn_win_t w = bn_win();
    int slen = (int)w.n;
    int n = (int)to_int(args[0]);
    int target = w.pos + n;
    if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
    int old = w.pos;
    w.env[1] = (uint64_t)(int64_t)(target - 1);
    int lo = old < target ? old : target, hi = old < target ? target : old;
    int len = hi - lo;
    char *buf = rt_wsb_alloc(len + 1);
    memcpy(buf, w.s + lo - 1, len);
    buf[len] = '\0';
    *out = BSTRVAL(buf, len);
    return 1;
}
int icn_bi_direct_scanpos(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    if (g_rt_bn.dtax_bid[bid].kind != 0 && g_rt_bn.dtax_bid[bid].kind != 3) return 0;
    if (nargs != 1 || args[0].v != DT_I) return 0;
    core_icn_op_ctx_clear();
    return bid == BID_tab ? bn_icn_tab(args, out) : bn_icn_move(args, out);
}
int icn_bi_direct_scan(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    if (g_rt_bn.dtax_bid[bid].kind != 0 && g_rt_bn.dtax_bid[bid].kind != 3) return 0;
    if (nargs < 1 || nargs > 4 || args[0].v != DT_S) return 0;
    if (nargs != 1 && (args[1].v != DT_S || IS_CSET_fn(args[1]))) return 0;
    for (int i = 2; i < nargs; i++) if (args[i].v != DT_I && args[i].v != DT_SNUL) return 0;
    core_icn_op_ctx_clear();
    return bid == BID_any ? bn_icn_any(args, nargs, out) : bn_icn_many(args, nargs, out);
}
