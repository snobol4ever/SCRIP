#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/trace.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_ids.h"
#include "runtime/builtin_leaf.h"
#include "runtime/rt/rt_list_view.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_19_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_21_sh.h"
#include "runtime/lib/bn_22_rk.h"
#include "runtime/lib/bn_23_rk.h"
#include "runtime/lib/bn_36_sh.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern DESCR_t dat_construct_rq(DatType *, DESCR_t *, int, long *);
extern void *rt_ws_alloc_descr(size_t);
extern DESCR_t rt_deref(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_flat_agg(DESCR_t *args, int nargs) {
    if (nargs <= 0) return rk_mk_arr(NULL, 0);
    rk_cbh_t hold[nargs];
    for (int i = 0; i < nargs; i++) { hold[i] = (rk_cbh_t) { g_rk_cbh_cur, { args[i], NULVCL } }; g_rk_cbh_cur = &hold[i]; }
    int cap = 0;
    for (int i = 0; i < nargs; i++) cap += rk_flat_count(hold[i].d[0]);
    DESCR_t *r = cap > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) cap) : NULL;
    int k = 0;
    for (int i = 0; i < nargs; i++) k = rk_flat_into(r, k, hold[i].d[0]);
    g_rk_cbh_cur = hold[0].prev;
    return rk_mk_arr(r, k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_cool_str(const char *m) {
    static const char *const cm[] = { "chars", "codes", "lc", "uc", "tc", "fc", "tclc", "flip", "chop", "chomp", "trim", "trim-leading", "trim-trailing", "wordcase", "ord", "ords", "contains",
        "starts-with", "ends-with", "index", "rindex", "indices", "substr", "samecase", "samemark", "samespace", "subst", "trans", "parse-base", "encode", "indent", "wrap", NULL };
    for (int i = 0; cm[i]; i++) if (!strcmp(m, cm[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_ref_val(DESCR_t *args, int nargs) { extern DESCR_t rt_deref(DESCR_t); return rt_deref(args[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_byref_assign_impl(DESCR_t *args) { if (args[0].v != DT_N) return args[1]; DESCR_t rv = rtx_try_rt_assign_var(args[0], args[1]); return IS_FAIL_fn(rv) ? args[1] : rv; }
DESCR_t rt_rk_byref_assign(DESCR_t *args, int nargs) { (void) nargs; return rk_byref_assign_impl(args); }
DESCR_t bt___rk_arr_values(DESCR_t *args, int nargs) { if (nargs >= 1) { rk_av_t a = rk_av(args[0]); return rk_mk_arr(a.el, a.n); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_flat(DESCR_t *args, int nargs) { if (nargs >= 0) { return rk_flat_agg(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_join(DESCR_t *args, int nargs) {
    if (nargs == 0) return STRVAL("");
    if (nargs >= 1) {
        rk_cbh_t H = { g_rk_cbh_cur, { args[0], NULVCL } };
        g_rk_cbh_cur = &H;
        rk_av_t a = rk_av(rk_flat_agg(args + 1, nargs - 1));
        g_rk_cbh_cur = H.prev;
        char sb[64];
        const char *s0 = to_cstring(H.d[0], sb, sizeof sb);
        size_t sl = strlen(s0);
        char sep[sl + 1];
        memcpy(sep, s0, sl + 1);
        return STRVAL(rk_av_joined(a, sep, "", ""));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_at(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 2) { rk_arr_at_impl(args, out); return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_byref_assign(DESCR_t *args, int nargs) { if (nargs == 2) { return rk_byref_assign_impl(args); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
int script_try_call_builtin_by_name_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (fn[0] == '_' && fn[1] == '_' && fn[2] == 'r' && fn[3] == 'k' && fn[4] == '_' && rk_fast_rat_builtin(fn, args, nargs, out)) return 1;
#if RT_DIAG
    if (!strcmp(fn, "__trace_stmt") && nargs == 1) { extern void rt_trace_stmt(long line); rt_trace_stmt(IS_INT_fn(args[0]) ? (long)args[0].i : 0L); *out = NULVCL; return 1; }
    if (!strcmp(fn, "__trace_call") && nargs >= 1) {
        extern void rt_trace_call(const char *name, DESCR_t *args, int nargs);
        const char *nm = VARVAL_fn(args[0]);
        rt_trace_call(nm, args + 1, nargs - 1);
        *out = NULVCL;
        return 1;
    }
    if (!strcmp(fn, "__trace_return") && (nargs == 1 || nargs == 2)) {
        extern void rt_trace_return(const char *name, DESCR_t retval);
        rt_trace_return(VARVAL_fn(args[0]), (nargs == 2 ? args[1] : NULVCL));
        *out = NULVCL;
        return 1;
    }
    if (!strcmp(fn, "__trace_value") && nargs == 2) {
        extern void rt_trace_value_sigil(const char *name, DESCR_t val, int hook);
        rt_trace_value_sigil(VARVAL_fn(args[0]), args[1], 1);
        *out = NULVCL;
        return 1;
    }
    if (!strcmp(fn, "__trace_tap_off") && nargs == 0) { extern void rt_trace_tap_off(void); rt_trace_tap_off(); *out = NULVCL; return 1; }
#else
    if (fn[0] == '_' && fn[1] == '_' && !strncmp(fn, "__trace_", 8)) {
        const char *tk = fn + 8;
        if ((!strcmp(tk, "stmt") && nargs == 1) || (!strcmp(tk, "call") && nargs >= 1) || (!strcmp(tk, "return") && (nargs == 1 || nargs == 2))) { *out = NULVCL; return 1; }
        if ((!strcmp(tk, "value") && nargs == 2) || (!strcmp(tk, "tap_off") && nargs == 0)) { *out = NULVCL; return 1; }
    }
#endif
    {
        int _br = bt_row_of(BT_VOC_RK, fn, (unsigned)strlen(fn));
        if (_br >= 0 && g_bt_leaf[_br]) {
            const DESCR_t _bv = g_bt_leaf[_br](args, nargs);
            if (_bv.v != RTX_NOT_HANDLED) { *out = _bv; return 1; }
        } else if (_br >= 0 && g_bt_open[_br] && g_bt_open[_br](args, nargs, out, rq)) return 1;
    }
    if (!fn) return 0;
    extern int fh_capture_begin(char **, size_t *, int *);
    extern void fh_capture_end(int, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    {
        int _br = bt_row_of(BT_VOC_RK, fn, (unsigned)strlen(fn));
        if (_br >= 0 && g_bt_leaf[_br]) {
            const DESCR_t _bv = g_bt_leaf[_br](args, nargs);
            if (_bv.v != RTX_NOT_HANDLED) { *out = _bv; return 1; }
        } else if (_br >= 0 && g_bt_open[_br] && g_bt_open[_br](args, nargs, out, rq)) return 1;
    }
    if (fn[0] == '_' && fn[1] == '_' && fn[2] == 'p' && fn[3] == 'a' && fn[4] == 's' && fn[5] == '_' && pas_bi_chain(fn, args, nargs, out, rq)) return 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    if ((!strncmp(fn, "__rk_set_", 9) || !strncmp(fn, "__rk_bag_", 9)) && nargs == 2 && rk_qh_fn(fn, args, nargs, out)) return 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    if (!strcmp(fn, "__pas_elem_ref") && nargs == 3) { *out = pas_elem_ref(args, nargs); return 1; }
    if (!strcmp(fn, "__pas_ref_val") && nargs == 1) { *out = pas_ref_val(args, nargs); return 1; }
    if (!strcmp(fn, "str_substr") || (!strcmp(fn, "substr") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        long slen = (long)strlen(s);
        long start = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (start < 0) start = slen + start;
        if (start < 0) start = 0;
        if (start > slen) start = slen;
        long len = slen - start;
        if (nargs >= 3) { long ll = IS_INT_fn(args[2]) ? args[2].i : len; if (ll < 0) ll = 0; if (start + ll > slen) ll = slen - start; len = ll; }
        char *o = rt_wsb_alloc((size_t)len + 1);
        memcpy(o, s + start, (size_t)len);
        o[len] = '\0';
        *out = STRVAL(o);
        return 1;
    }
    if (!strcmp(fn, "str_index") || (!strcmp(fn, "index") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        const char *n = VARVAL_fn(args[1]);
        if (!n) n = "";
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : 0;
        if (from < 0) from = 0;
        if (*n == '\0') { *out = INTVAL(from); return 1; }
        const char *p = strstr(s + from, n);
        *out = p ? INTVAL((long)(p - s)) : NULVCL;
        return 1;
    }
    if (!strcmp(fn, "str_rindex") || (!strcmp(fn, "rindex") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        const char *n = VARVAL_fn(args[1]);
        if (!n) n = "";
        long slen = (long)strlen(s);
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : slen;
        size_t nlen = strlen(n);
        if (nlen == 0) { *out = INTVAL(from < slen ? from : slen); return 1; }
        long best = -1;
        for (long i = 0; i <= slen - (long)nlen && i <= from; i++) if (memcmp(s + i, n, nlen) == 0) best = i;
        *out = (best >= 0) ? INTVAL(best) : NULVCL;
        return 1;
    }
    if ((!strcmp(fn, "sprintf") || !strcmp(fn, "__rk_sprintf") || !strcmp(fn, "__rk_sprintf_d") || !strcmp(fn, "printf") || !strcmp(fn, "__rk_printf") || !strcmp(fn, "__rk_printf_d")) && nargs >= 2) {
        int ns = 0;
        int one_list = nargs == 2 && args[1].v == DT_A && args[1].arr && !rk_is_pair(args[1]) && !rk_is_jct(args[1]);
        for (int i = 1; i < nargs; i++) ns += (rk_is_slip(args[i]) || one_list) ? rk_av(args[i]).n : 1;
        if (ns != nargs - 1 || one_list) {
            DESCR_t *fa = (DESCR_t *) rt_ws_alloc_descr((size_t) ns + 1);
            int k = 0;
            fa[k++] = args[0];
            for (int i = 1; i < nargs; i++) { if (rk_is_slip(args[i]) || one_list) { rk_av_t sa = rk_av(args[i]); for (int j = 0; j < sa.n; j++) fa[k++] = sa.el[j]; } else fa[k++] = args[i]; }
            return script_try_call_builtin_by_name_rq(fn, fa, k, out, rq);
        }
    }
    if ((!strcmp(fn, "sprintf") || !strcmp(fn, "__rk_sprintf") || !strcmp(fn, "__rk_sprintf_d")) && nargs >= 1) {
        char fb[512];
        const char *fmt = to_cstring(args[0], fb, sizeof fb);
        if (!fmt) fmt = "";
        char *r = NULL;
        size_t rl = 0;
        if (fn[0] == '_' && fn[strlen(fn) - 1] == 'd') rk_rsprintf_legacy(fmt, args, nargs, 1, &r, &rl);
        else if (fn[0] == '_') rk_rsprintf(fmt, args, nargs, 1, &r, &rl);
        else rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        *out = BSTRVAL(r, rl);
        return 1;
    }
    if ((!strcmp(fn, "printf") || !strcmp(fn, "__rk_printf") || !strcmp(fn, "__rk_printf_d")) && nargs >= 1) {
        char fb[512];
        const char *fmt = to_cstring(args[0], fb, sizeof fb);
        if (!fmt) fmt = "";
        char *r = NULL;
        size_t rl = 0;
        if (fn[0] == '_' && fn[strlen(fn) - 1] == 'd') rk_rsprintf_legacy(fmt, args, nargs, 1, &r, &rl);
        else if (fn[0] == '_') rk_rsprintf(fmt, args, nargs, 1, &r, &rl);
        else rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        fwrite(r, 1, rl, stdout);
        *out = INTVAL(1);
        return 1;
    }
    if (!strcmp(fn, "callsame") || !strcmp(fn, "nextsame") || !strcmp(fn, "callwith")) {
        rk_redisp_t *top = g_redisp_cur;
        if (!top) { *out = FAILDESCR; return 1; }
        const char *fmname = VARVAL_fn(top->mname);
        int mro_len = rd_mro_len(top->cname);
        const char *mro[mro_len];
        { extern int dat_mro(const char *name, const char **out, int max); int got = dat_mro(top->cname, mro, mro_len); if (got <= 0) { mro[0] = top->cname; mro_len = 1; } else mro_len = got; }
        int next = -1;
        for (int i = top->found_idx + 1; i < mro_len; i++) {
            char cand[fmt_len("%s__%s", mro[i], fmname)];
            snprintf(cand, sizeof cand, "%s__%s", mro[i], fmname);
            if (meth_is_user_proc(cand)) { next = i; break; }
        }
        if (next < 0) { *out = NULVCL; return 1; }
        char nproc[fmt_len("%s__%s", mro[next], fmname)];
        snprintf(nproc, sizeof nproc, "%s__%s", mro[next], fmname);
        extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
        *out = rt_c2bb_bomb("callsame/callwith (a next candidate rk_callsame_open / rk_callwith_open could not open: the box enters it, C never does)", nproc);
        return 1;
    }
    if (!strncmp(fn, "nqp::bindattr", 13) && nargs == 4 && (!fn[13] || !strcmp(fn + 13, "_n") || !strcmp(fn + 13, "_i") || !strcmp(fn + 13, "_s"))) { *out = rk_bindattr_impl(args); return 1; }
    if (!strncmp(fn, "__rk_new_", 9) && nargs >= 1) {
        const char *cname = VARVAL_fn(args[0]);
        DatType *dt = (cname && *cname) ? dat_find_type(cname) : NULL;
        if (!dt) { *out = FAILDESCR; return 1; }
        if (!strcmp(fn, "__rk_new_check") && nargs == 2) { extern void rt_construct_check(DatType *t, DESCR_t self); rt_construct_check(dt, args[1]); *out = args[1]; return 1; }
        if (!strcmp(fn, "__rk_new_defaults") && nargs == 2) { extern DESCR_t rk_attr_defaults(DatType *dt, DESCR_t inst); *out = rk_attr_defaults(dt, args[1]); return 1; }
        if (!strcmp(fn, "__rk_new_alloc")) {
            extern int dat_has_build_mro(const char *cls);
            extern DESCR_t rt_construct_alloc(DatType *t, DESCR_t *args, int nargs);
            if (dat_has_build_mro(cname)) { *out = rt_construct_alloc(dt, (DESCR_t *)0, 0); return 1; }
            DESCR_t fvals[dt->nfields > 0 ? dt->nfields : 1];
            for (int fi = 0; fi < dt->nfields; fi++) fvals[fi] = NULVCL;
            for (int ci = 1; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]);
                if (!kname) continue;
                for (int fi = 0; fi < dt->nfields; fi++) { if (strcmp(dt->fields[fi], kname) == 0) { if (!dt->priv[fi]) fvals[fi] = args[ci + 1]; break; } }
            }
            *out = rt_construct_alloc(dt, fvals, dt->nfields);
            return 1;
        }
    }
    if (!strcmp(fn, "meth_call") && nargs >= 2) {
        const char *mname0 = VARVAL_fn(args[1]);
        if (mname0 && rk_is_seq(args[0])) {
            if (!strcmp(mname0, "is-lazy")) { *out = (DESCR_t){ .v = DT_BOOL, .i = rk_seq_is_lazy(args[0]) }; return 1; }
            if (!strcmp(mname0, "list") || !strcmp(mname0, "Seq") || !strcmp(mname0, "lazy") || !strcmp(mname0, "iterator")) { *out = args[0]; return 1; }
            if (nargs == 2 && !strcmp(mname0, "pull-one")) {
                rk_seq_reify(&args[0], 1L);
                ARBLK_t *pl = (ARBLK_t *) ((ARBLK_t *) args[0].arr)->data[3].arr;
                if (pl->hi < pl->lo) { *out = rk_typeobj("IterationEnd"); return 1; }
                *out = pl->data[0];
                for (int pi = 0; pi < pl->hi - pl->lo; pi++) pl->data[pi] = pl->data[pi + 1];
                pl->hi--;
                return 1;
            }
            {
                long need = -1L;
                if (!strcmp(mname0, "head")) need = (nargs >= 3 && IS_INT_fn(args[2]) && args[2].i >= 0) ? (long) args[2].i : 1L;
                rk_seq_reify(&args[0], need);
                args[0] = ((ARBLK_t *) args[0].arr)->data[3];
            }
        }
        if (mname0 && rk_is_range(args[0]) && rk_range_method(mname0, args[0], nargs - 2, out)) return 1;
        if (mname0 && (rk_is_methval(args[0]) || rk_is_attrval(args[0]))) {
            rk_av_t ma = rk_av(args[0]);
            int ism = rk_is_methval(args[0]);
            if (!strcmp(mname0, "name")) { *out = ism ? ma.el[0] : ma.el[1]; return 1; }
            if (!strcmp(mname0, "gist") || !strcmp(mname0, "Str")) { *out = STRVAL(rt_heap_strdup_c(ism ? rk_cstr(ma.el[0]) : rk_gist_str(args[0]))); return 1; }
            if (!strcmp(mname0, "type") && !ism) { *out = rk_typeobj(rk_cstr(ma.el[0])); return 1; }
            if (!strcmp(mname0, "WHAT")) { *out = rk_typeobj(ism ? "Method" : "Attribute"); return 1; }
        }
        if (mname0 && rk_is_code(args[0]) && !strcmp(mname0, "signature") && rk_code_name(args[0])) { *out = rk_mk_metaval(rk_proto_sigval, rk_code_name(args[0]), NULL); return 1; }
        if (mname0 && rk_is_sigval(args[0])) {
            extern const char *rt_proc_pname(const char *name, int k);
            const char *pn = rk_cstr(rk_av(args[0]).el[0]);
            int np = 0;
            while (pn && rt_proc_pname(pn, np)) np++;
            if (!strcmp(mname0, "params")) {
                DESCR_t el[np > 0 ? np : 1];
                for (int k = 0; k < np; k++) el[k] = rk_mk_metaval(rk_proto_paramval, rt_proc_pname(pn, k), rk_param_sigil(rt_proc_pname(pn, k)));
                *out = rk_mk_arr(el, np);
                return 1;
            }
            if (!strcmp(mname0, "WHAT")) { *out = rk_typeobj("Signature"); return 1; }
        }
        if (mname0 && rk_is_paramval(args[0])) { if (!strcmp(mname0, "sigil")) { *out = rk_av(args[0]).el[1]; return 1; } if (!strcmp(mname0, "WHAT")) { *out = rk_typeobj("Parameter"); return 1; } }
        int maybe = 0;
        if (mname0 && mname0[0] == '?' && mname0[1]) {
            DESCR_t *fa = rt_ws_alloc_descr((size_t) nargs);
            for (int k = 0; k < nargs; k++) fa[k] = args[k];
            mname0++;
            fa[1] = STRVAL(rt_heap_strdup_c(mname0));
            args = fa;
            maybe = 1;
        }
        if (IS_STR_fn(args[0]) && args[0].s && ((args[0].s[0] == 'X' && args[0].s[1] == ':') || args[0].s[0] == 'E') && (args[0].s[0] == 'X' || !strcmp(args[0].s, "Exception"))) rk_exc_init();
        if (mname0 && mname0[0] == '^' && !(!strcmp(mname0 + 1, "can") && nargs == 3)) {
            const char *mm = mname0 + 1;
            const char *cn = NULL;
            if (IS_DATA_INST_fn(args[0]) && args[0].u) {
                DATINST_t *di = (DATINST_t *)args[0].u;
                cn = (di && di->type) ? rk_dat_tname(di->type->name) : NULL;
            } else {
                cn = VARVAL_fn(args[0]);
                if (cn && !dat_find_type(cn)) cn = NULL;
            }
            if (cn && !strcmp(mm, "name")) { *out = STRVAL(rt_heap_strdup_c(cn)); return 1; }
            if (!cn && !strcmp(mm, "name")) { *out = STRVAL(rt_heap_strdup_c(rk_value_type(args[0]))); return 1; }
            if (cn && (!strcmp(mm, "parents") || !strcmp(mm, "methods") || !strcmp(mm, "attributes"))) {
                int local = 0;
                for (int k = 2; k < nargs; k++) {
                    if (rk_is_pair(args[k]) && !strcmp(rk_av_text(rk_av(args[k]), 0), "local") && rk_is_truthy(rk_av(args[k]).el[1])) local = 1;
                    if (IS_STR_fn(args[k]) && k + 1 < nargs && !strcmp(rk_cstr(args[k]), "local") && rk_is_truthy(args[k + 1])) local = 1;
                }
                *out = rk_meta_list(cn, mm[0], local);
                return 1;
            }
            *out = FAILDESCR;
            return 1;
        }
        if (mname0 && !strcmp(mname0, "WHAT")) {
            const char *cn = NULL;
            if (IS_DATA_INST_fn(args[0]) && args[0].u) {
                DATINST_t *di = (DATINST_t *)args[0].u;
                cn = (di && di->type) ? rk_dat_tname(di->type->name) : NULL;
            } else {
                cn = VARVAL_fn(args[0]);
                if (cn && !dat_find_type(cn)) cn = NULL;
            }
            if (cn) { *out = rk_typeobj(cn); return 1; }
            *out = rk_typeobj(rk_value_type(args[0]));
            return 1;
        }
        if (mname0 && (!strcmp(mname0, "isa") || !strcmp(mname0, "does")) && nargs >= 3) {
            const char *cn = NULL;
            if (IS_DATA_INST_fn(args[0]) && args[0].u) {
                DATINST_t *di = (DATINST_t *)args[0].u;
                cn = (di && di->type && di->type->name[0] != 1) ? di->type->name : NULL;
            } else {
                cn = VARVAL_fn(args[0]);
                if (cn && !dat_find_type(cn)) cn = NULL;
            }
            const char *target = rk_typeobj_name(args[2]) ? rk_typeobj_name(args[2]) : VARVAL_fn(args[2]);
            if (!cn && target) { *out = INTVAL(rk_type_isa(rk_value_type(args[0]), target) ? 1 : 0); return 1; }
            if (!cn || !target) { *out = INTVAL(0); return 1; }
            extern int dat_mro(const char *name, const char **out, int max);
            extern int dat_roles(const char *name, const char **out, int max);
            int hit = 0;
            const char *mro[bnd_dat_need(cn, 0)];
            int mn = dat_mro(cn, mro, (int)(sizeof mro / sizeof mro[0]));
            if (mn == 0) { mro[0] = cn; mn = 1; }
            for (int i = 0; i < mn && !hit; i++) if (mro[i] && !strcmp(mro[i], target)) hit = 1;
            if (!hit && !strcmp(mname0, "does")) {
                for (int i = 0; i < mn && !hit; i++) {
                    const char *rls[bnd_dat_need(mro[i], 1)];
                    int rn = dat_roles(mro[i], rls, (int)(sizeof rls / sizeof rls[0]));
                    for (int j = 0; j < rn && !hit; j++) if (rls[j] && !strcmp(rls[j], target)) hit = 1;
                }
            }
            *out = INTVAL(hit ? 1 : 0);
            return 1;
        }
        if (mname0 && (!strcmp(mname0, "can") || !strcmp(mname0, "^can")) && nargs == 3) {
            const char *cn = NULL;
            if (IS_DATA_INST_fn(args[0]) && args[0].u) {
                DATINST_t *di = (DATINST_t *)args[0].u;
                cn = (di && di->type && di->type->name[0] != 1) ? di->type->name : NULL;
            } else {
                cn = VARVAL_fn(args[0]);
                if (cn && !dat_find_type(cn)) cn = NULL;
            }
            const char *want = VARVAL_fn(args[2]);
            if (cn && want) {
                extern int dat_mro(const char *name, const char **out, int max);
                const char *mro[bnd_dat_need(cn, 0)];
                int mn = dat_mro(cn, mro, (int)(sizeof mro / sizeof mro[0]));
                if (mn == 0) { mro[0] = cn; mn = 1; }
                DESCR_t hits[mn];
                int nh = 0;
                for (int i = 0; i < mn; i++) {
                    if (!mro[i]) continue;
                    char *pn = rt_heap_fmt("%s__%s", mro[i], want);
                    if (meth_is_user_proc(pn)) { hits[nh] = (DESCR_t){ 0 }; hits[nh].v = DT_BLK; hits[nh].s = pn; nh++; continue; }
                    DatType *ft = dat_find_type(mro[i]);
                    if (ft) for (int f = 0; f < ft->nfields; f++) if (!ft->priv[f] && !strcmp(ft->fields[f], want)) { hits[nh++] = STRVAL(rt_heap_strdup_c(want)); break; }
                }
                *out = rk_mark_list(rk_mk_arr(hits, nh));
                return 1;
            }
            if (!cn && want) { DESCR_t hit = STRVAL(rt_heap_strdup_c(want)); *out = rk_mark_list(rk_mk_arr(&hit, rk_is_rakudo_method(want) ? 1 : 0)); return 1; }
        }
        if (mname0 && !strcmp(mname0, "bless")) {
            const char *cname = VARVAL_fn(args[0]);
            if (!cname || !*cname) { *out = FAILDESCR; return 1; }
            DatType *dt = dat_find_type(cname);
            if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[dt->nfields > 0 ? dt->nfields : 1];
            for (int fi = 0; fi < dt->nfields; fi++) fvals[fi] = NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]);
                if (!kname) continue;
                for (int fi = 0; fi < dt->nfields; fi++) if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct_rq(dt, fvals, dt->nfields, rq);
            return 1;
        }
        if (mname0 && !strcmp(mname0, "clone") && IS_DATA_INST_fn(args[0]) && args[0].u) {
            DATINST_t *src = (DATINST_t *)args[0].u;
            const char *cname = (src && src->type) ? src->type->name : NULL;
            DatType *dt = cname ? dat_find_type(cname) : NULL;
            if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[dt->nfields > 0 ? dt->nfields : 1];
            for (int fi = 0; fi < dt->nfields; fi++) fvals[fi] = (src->fields && fi < src->type->nfields) ? src->fields[fi] : NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]);
                if (!kname) continue;
                for (int fi = 0; fi < dt->nfields; fi++) if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct_rq(dt, fvals, dt->nfields, rq);
            return 1;
        }
        if (mname0 && (!strcmp(mname0, "parse") || !strcmp(mname0, "subparse")) && nargs >= 3) {
            const char *gname = VARVAL_fn(args[0]);
            if (gname && rt_grammar_has_top(gname)) { const char *subj = VARVAL_fn(args[2]); return grammar_parse_core(gname, subj, out, mname0[0] == 's', nargs > 3 ? args[3] : (DESCR_t) { 0 }); }
        }
        if (mname0 && rk_match_is(args[0]) && rk_match_method(mname0, args, nargs, out)) return 1;
        if (mname0 && rk_failure_is(args[0]) && rk_failure_method(mname0, args, nargs, out)) return 1;
        if (mname0 && (args[0].v == DT_DATA || args[0].v == DT_BIG || args[0].v == DT_I) && nargs >= 2 && rk_rat_dispatch(mname0, &args[0], nargs > 2 ? &args[2] : NULL, nargs - 2, out)) return 1;
        if (mname0 && rk_qh_hook(mname0, args, nargs, out)) return 1;
        if (mname0 && rk_io_is(args[0]) && rk_io_path_method(mname0, args, nargs, out)) return 1;
        if (mname0 && rk_io_is(args[0]) && rk_io_cool_str(mname0) && rt_str_method(mname0, STRVAL(rt_heap_strdup_c(rk_io_pathstr(args[0]))), nargs > 2 ? &args[2] : NULL, nargs - 2, out)) return 1;
        if (mname0 && IS_FH_fn(args[0]) && rk_io_fh_method(mname0, args, nargs, out)) return 1;
        if (mname0 && nargs >= 2 && (IS_INT_fn(args[0]) || IS_REAL_fn(args[0])) &&
            (!strcmp(mname0, "push") || !strcmp(mname0, "append") || !strcmp(mname0, "unshift") || !strcmp(mname0, "prepend") || !strcmp(mname0, "splice"))) {
            extern void rt_script_die_surface(const char *msg);
            char m[fmt_len("Cannot resolve caller %s(...); none of these signatures matches", mname0)];
            snprintf(m, sizeof m, "Cannot resolve caller %s(...); none of these signatures matches", mname0);
            rt_script_die_surface(m);
            *out = FAILDESCR;
            return 1;
        }
        if (mname0 && nargs >= 2 && (IS_INT_fn(args[0]) || IS_REAL_fn(args[0])) && (!strcmp(mname0, "pop") || !strcmp(mname0, "shift"))) { rk_method_missing(args[0], mname0, out); return 1; }
        DESCR_t _recv0 = args[0];
        int _was_list_recv = 0;
        { DESCR_t *_lva = 0; int _lvn = 0; if (_recv0.v == DT_DATA && rt_lv_is_list(_recv0, &_lva, &_lvn)) { _recv0 = rk_mark_list(rt_make_item_agg(_lva, _lvn)); _was_list_recv = 1; } }
        if (_recv0.v == DT_A && _recv0.arr && mname0 && nargs >= 2 && !rk_is_pair(_recv0) && !rk_is_jct(_recv0) &&
            (!strcmp(mname0, "push") || !strcmp(mname0, "append") || !strcmp(mname0, "unshift") || !strcmp(mname0, "prepend") || !strcmp(mname0, "pop") || !strcmp(mname0, "shift") ||
            !strcmp(mname0, "splice"))) {
            if (!strcmp(mname0, "splice")) {
                ARBLK_t *sb = (ARBLK_t *) _recv0.arr;
                int sn = sb->hi - sb->lo + 1;
                if (sn < 0) sn = 0;
                int fa = 0;
                for (int k = 2; k < nargs; k++) fa += rk_spill_count(args[k]);
                DESCR_t *fl = fa ? (DESCR_t *) rt_ws_alloc_descr((size_t) fa) : NULL;
                int fn = 0;
                for (int k = 2; k < nargs; k++) fn = rk_spill(fl, fn, args[k]);
                long st = fn > 0 && IS_INT_fn(fl[0]) ? (long) fl[0].i : 0, cnt = fn > 1 && IS_INT_fn(fl[1]) ? (long) fl[1].i : (long) sn - st;
                if ((fn > 0 && !IS_INT_fn(fl[0])) || (fn > 1 && !IS_INT_fn(fl[1])) || st < 0 || st > sn || cnt < 0) return 0;
                if (st + cnt > sn) cnt = sn - st;
                DESCR_t *nw = fn > 2 ? fl + 2 : NULL, *old = sn ? (DESCR_t *) rt_ws_alloc_descr((size_t) sn) : NULL;
                int c = fn > 2 ? fn - 2 : 0;
                for (int k = 0; k < sn; k++) old[k] = sb->data[k];
                DESCR_t *rem = cnt ? (DESCR_t *) rt_ws_alloc_descr((size_t) cnt) : NULL;
                for (long k = 0; k < cnt; k++) rem[k] = old[st + k];
                sb->hi = sb->lo - 1;
                for (long k = 0; k < st; k++) rk_arr_append(sb, old[k]);
                for (int k = 0; k < c; k++) rk_arr_append(sb, nw[k]);
                for (long k = st + cnt; k < sn; k++) rk_arr_append(sb, old[k]);
                *out = rk_unmark(rk_mk_arr(rem, (int) cnt));
                return 1;
            }
            if ((!strcmp(mname0, "pop") || !strcmp(mname0, "shift")) && nargs > 2) {
                char xm[96];
                extern void rt_script_die_surface(const char *msg);
                snprintf(xm, sizeof xm, "Too many positionals passed; expected 1 argument but got %d", nargs - 1);
                rt_script_die_surface(xm);
                *out = FAILDESCR;
                return 1;
            }
            if ((!strcmp(mname0, "pop") || !strcmp(mname0, "shift")) && nargs > 2) {
                char xm[96];
                extern void rt_script_die_surface(const char *msg);
                snprintf(xm, sizeof xm, "Too many positionals passed; expected 1 argument but got %d", nargs - 1);
                rt_script_die_surface(xm);
                *out = FAILDESCR;
                return 1;
            }
            ARBLK_t *mb = (ARBLK_t *) _recv0.arr;
            int n0 = mb->hi - mb->lo + 1;
            if (n0 < 0) n0 = 0;
            if (mname0[0] == 'p' && mname0[1] == 'o') { if (!n0) { *out = rk_empty_failure("pop"); return 1; } *out = mb->data[n0 - 1]; mb->data[n0 - 1] = NULVCL; mb->hi--; return 1; }
            if (mname0[0] == 's') {
                if (!n0) { *out = rk_empty_failure("shift"); return 1; }
                *out = mb->data[0];
                memmove(mb->data, mb->data + 1, (size_t) (n0 - 1) * sizeof(DESCR_t));
                mb->data[n0 - 1] = NULVCL;
                mb->hi--;
                return 1;
            }
            int pre = mname0[0] == 'u' || (mname0[0] == 'p' && mname0[1] == 'r'), flat = mname0[0] == 'a' || (mname0[0] == 'p' && mname0[1] == 'r');
            int add = 0;
            for (int k = 2; k < nargs; k++) add += flat ? rk_spill_count(args[k]) : rk_item_count(args[k]);
            DESCR_t *nw = add ? (DESCR_t *) rt_ws_alloc_descr((size_t) add) : NULL;
            int c = 0;
            for (int k = 2; k < nargs; k++) c = flat ? rk_spill(nw, c, args[k]) : rk_item(nw, c, args[k]);
            if (!pre) {
                for (int k = 0; k < c; k++) rk_arr_append(mb, nw[k]);
            } else {
                for (int k = 0; k < c; k++) rk_arr_append(mb, NULVCL);
                memmove(mb->data + c, mb->data, (size_t) n0 * sizeof(DESCR_t));
                for (int k = 0; k < c; k++) mb->data[k] = nw[k];
            }
            *out = _recv0;
            return 1;
        }
        if (_recv0.v == DT_T && _recv0.tbl && mname0) {
            if (!strcmp(mname0, "elems") || (nargs == 2 && (!strcmp(mname0, "Numeric") || !strcmp(mname0, "Int") || !strcmp(mname0, "Real")))) { *out = INTVAL(_recv0.tbl->size); return 1; }
            if (nargs == 2 && !strcmp(mname0, "Num")) { *out = (DESCR_t){ .v = DT_R, .r = (double) _recv0.tbl->size }; return 1; }
            if (!strcmp(mname0, "keys")) { *out = rk_hash_list(_recv0, 'k'); return 1; }
            if (!strcmp(mname0, "values")) { *out = rk_hash_list(_recv0, 'v'); return 1; }
            if (!strcmp(mname0, "pairs")) { *out = rk_hash_list(_recv0, 'p'); return 1; }
            if (!strcmp(mname0, "kv")) { *out = rk_hash_list(_recv0, 'w'); return 1; }
            if (!strcmp(mname0, "antipairs")) {
                rk_av_t ps = rk_av(rk_hash_list(_recv0, 'p'));
                DESCR_t res = rk_mk_arr(NULL, 0);
                for (int i = 0; i < ps.n; i++) { rk_av_t pa = rk_av(ps.el[i]); rk_arr_append((ARBLK_t *) res.arr, rk_mk_pair(pa.el[1], pa.el[0])); }
                *out = rk_seq_done(res);
                return 1;
            }
            if (!strcmp(mname0, "Bool") || !strcmp(mname0, "so") || !strcmp(mname0, "defined")) { *out = (DESCR_t){ .v = DT_BOOL, .i = _recv0.tbl->size > 0 || mname0[0] == 'd' }; return 1; }
            if (!strcmp(mname0, "not")) { *out = (DESCR_t){ .v = DT_BOOL, .i = _recv0.tbl->size == 0 }; return 1; }
            if (!strcmp(mname0, "gist")) { *out = STRVAL((char *) rk_gist_str(_recv0)); return 1; }
            if (!strcmp(mname0, "Str")) { *out = STRVAL((char *) rk_tbl_text(_recv0)); return 1; }
            if (!strcmp(mname0, "raku") || !strcmp(mname0, "perl")) { *out = STRVAL((char *) rk_raku_str(_recv0, 0)); return 1; }
            if (!strcmp(mname0, "WHAT")) { *out = rk_typeobj("Hash"); return 1; }
            if (nargs == 2 && (!strcmp(mname0, "item") || !strcmp(mname0, "self"))) { *out = _recv0; return 1; }
            _recv0 = rk_hash_list(_recv0, 'p');
        }
        if (nargs >= 2 && _recv0.v != DT_DATA && !rk_class_user_meth(_recv0, mname0) && rt_str_method(mname0, _recv0, &args[2], nargs - 2, out)) return 1;
        if (_recv0.v != DT_DATA && mname0) {
            const char *rtn = VARVAL_fn(_recv0);
            int is_dat_recv = (rtn && dat_find_type(rtn));
            if (!is_dat_recv) {
                int is_arrm = !strcmp(mname0, "kv") || !strcmp(mname0, "reverse") || !strcmp(mname0, "unique") || !strcmp(mname0, "sort") ||
                    (!strcmp(mname0, "map") && nargs == 3 && rk_is_code(args[2])) || (!strcmp(mname0, "grep") && nargs == 3) || !strcmp(mname0, "elems") || !strcmp(mname0, "end") ||
                    !strcmp(mname0, "join") || !strcmp(mname0, "sum") || !strcmp(mname0, "head") || !strcmp(mname0, "tail") || !strcmp(mname0, "pick") || !strcmp(mname0, "min") ||
                    !strcmp(mname0, "max") || !strcmp(mname0, "first") || (!strcmp(mname0, "reduce") && nargs == 3 && rk_is_code(args[2])) || !strcmp(mname0, "keys") || !strcmp(mname0, "values") ||
                    rk_list_coercion_meth(mname0, nargs);
                if (is_arrm) {
                    const char *afn = !strcmp(mname0, "map") ? "__rk_arr_map" : !strcmp(mname0, "grep") ? "__rk_arr_grep" : !strcmp(mname0, "kv") ? "__rk_arr_kv" : !strcmp(mname0, "sort") ?
                        "__rk_arr_sort" : !strcmp(mname0, "min") ? "__rk_arr_min" : !strcmp(mname0, "max") ? "__rk_arr_max" : !strcmp(mname0, "first") ? "__rk_arr_first" : !strcmp(mname0, "reduce") ?
                        "__rk_arr_reduce" : !strcmp(mname0, "keys") ? "__rk_arr_keys" : !strcmp(mname0, "Array") ? "__rk_to_array" : !strcmp(mname0, "flat") ? "__rk_arr_flat" :
                        !strcmp(mname0, "Slip") ? "__rk_arr_slip" : (!strcmp(mname0, "values") || rk_list_coercion_meth(mname0, nargs)) ? "__rk_arr_values" : !strcmp(mname0, "end") ? "elems" : mname0;
                    int total = 1 + (nargs - 2);
                    DESCR_t *fa = rt_ws_alloc_descr((size_t)total);
                    if (!strcmp(mname0, "join")) {
                        for (int k = 0; k < nargs - 2; k++) fa[k] = args[2 + k];
                        fa[nargs - 2] = _recv0;
                    } else {
                        fa[0] = _recv0;
                        for (int k = 0; k < nargs - 2; k++) fa[1 + k] = args[2 + k];
                    }
                    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                    if (script_try_call_builtin_by_name(afn, fa, total, out)) { if (!strcmp(mname0, "end") && IS_INT_fn(*out)) *out = INTVAL(out->i - 1); return 1; }
                }
            }
        }
        if (args[0].v != DT_DATA) {
            const char *tname = rk_typeobj_name(args[0]) ? rk_typeobj_name(args[0]) : VARVAL_fn(args[0]);
            extern int rt_proc_has_native_fn(const char *name);
            if (tname && mname0 && !strcmp(mname0, "new") && dat_find_type(tname)) {
                {
                    DESCR_t *ma = rt_ws_alloc_descr((size_t) nargs);
                    for (int k = 0; k < nargs; k++) ma[k] = args[k];
                    ma[0] = STRVAL(rt_heap_strdup_c(tname));
                    if (rt_multi_meth_dispatch(tname, "new", ma, nargs, out, rq)) return 1;
                }
                DESCR_t *na = rt_ws_alloc_descr((size_t)(nargs - 1));
                na[0] = STRVAL(rt_heap_strdup_c(tname));
                for (int k = 2; k < nargs; k++) na[k - 1] = args[k];
                return script_try_call_builtin_by_name_rq("obj_new", na, nargs - 1, out, rq);
            }
            if (tname && dat_find_type(tname)) {
                const char *rmc = resolve_method_chain(tname, mname0, NULL);
                char tproc[fmt_len("%s__%s", rmc, mname0)];
                snprintf(tproc, sizeof tproc, "%s__%s", rmc, mname0);
                if (meth_is_user_proc(tproc)) {
                    int nextra = nargs - 2, total = 1 + nextra;
                    DESCR_t *ca = rt_ws_alloc_descr((size_t)total);
                    ca[0] = args[0];
                    for (int k = 0; k < nextra; k++) ca[1 + k] = args[2 + k];
                    int pi;
                    for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, tproc)) break;
                    if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(tproc)) {
                        rt_call_args_need(total);
                        for (int k = 0; k < total; k++) CALL_ARGS[k] = ca[k];
                        *out = rk_proc_descr_rq(tproc, total, rq);
                        return 1;
                    }
                    extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
                    *out = ir_call_proc(pi, ca, total);
                    return 1;
                }
            }
        }
        if (_was_list_recv || !IS_DATA_INST_fn(args[0]) || !args[0].u) {
            if (!_was_list_recv && mname0 && *mname0) {
                const char *tn = rk_value_type(args[0]);
                const char *cands[6];
                int nc = 0;
                cands[nc++] = tn;
                if (!strcmp(tn, "Bool")) cands[nc++] = "Int";
                if (!strcmp(tn, "Array")) cands[nc++] = "List";
                cands[nc++] = "Cool";
                cands[nc++] = "Any";
                cands[nc++] = "Mu";
                for (int ci = 0; ci < nc; ci++) {
                    char pn[fmt_len("%s__%s", cands[ci], mname0)];
                    snprintf(pn, sizeof pn, "%s__%s", cands[ci], mname0);
                    if (!meth_is_user_proc(pn)) continue;
                    int total = nargs - 1;
                    DESCR_t ca[total > 0 ? total : 1];
                    ca[0] = args[0];
                    for (int k = 1; k < total; k++) ca[k] = args[1 + k];
                    *out = invoke_method_proc(pn, ca, total);
                    return 1;
                }
            }
            int is_class = IS_STR_fn(args[0]) && args[0].s && dat_find_type(args[0].s) != NULL;
            if (maybe) *out = NULVCL;
            else rk_method_missing(args[0], mname0 ? mname0 : "", out);
            return 1;
        }
        DATINST_t *inst = (DATINST_t *)args[0].u;
        const char *cname = (inst && inst->type) ? inst->type->name : NULL;
        if (!cname) { *out = FAILDESCR; return 1; }
        const char *mname = VARVAL_fn(args[1]);
        if (!mname || !*mname) { *out = FAILDESCR; return 1; }
        int found_idx = -1;
        const char *rmc = resolve_method_chain(cname, mname, &found_idx);
        char procname[fmt_len("%s__%s", rmc, mname)];
        snprintf(procname, sizeof procname, "%s__%s", rmc, mname);
        if (!meth_is_user_proc(procname) && rt_multi_meth_dispatch(cname, mname, args, nargs, out, rq)) return 1;
        if (!meth_is_user_proc(procname) && rk_exc_is(args[0]) && rk_exc_method(mname, args, nargs, out, rq)) return 1;
        if (nargs == 2 && !meth_is_user_proc(procname)) {
            extern int dat_field_is_private(const char *cls, const char *field);
            if (dat_field_is_private(cname, mname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[fmt_len("Attribute '$!%s' not accessible outside of class %s (it is private)", mname, cname)];
                snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", mname, cname);
                rt_script_die_surface(_m);
                *out = FAILDESCR;
                return 1;
            }
            extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
            DESCR_t *acc = data_field_ptr(mname, args[0]);
            if (acc) { *out = *acc; return 1; }
        }
        if (!meth_is_user_proc(procname)) {
            extern int dat_handles_field(const char *cls, const char *meth, char *out, int outsz);
            char delegfield[bnd_handles_need(cname, mname)];
            if (dat_handles_field(cname, mname, delegfield, (int)sizeof delegfield)) {
                extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
                DESCR_t deleg = rtx_try_dat_field_get(delegfield, args[0]);
                DESCR_t *fwd = rt_ws_alloc_descr((size_t)nargs);
                fwd[0] = deleg;
                fwd[1] = args[1];
                for (int k = 2; k < nargs; k++) fwd[k] = args[k];
                return script_try_call_builtin_by_name_rq("meth_call", fwd, nargs, out, rq);
            }
        }
        if (!meth_is_user_proc(procname) && rk_is_pair(args[0])) {
            DESCR_t *lfw = rt_ws_alloc_descr((size_t)nargs);
            for (int k = 0; k < nargs; k++) lfw[k] = args[k];
            lfw[0] = rk_pair_legacy(args[0]);
            return script_try_call_builtin_by_name_rq("meth_call", lfw, nargs, out, rq);
        }
        if (!meth_is_user_proc(procname) && rk_mu_method(mname, args[0], nargs - 2, out)) return 1;
        int nextra = nargs - 2;
        int total = 1 + nextra;
        DESCR_t callargs[total > 0 ? total : 1];
        callargs[0] = args[0];
        for (int k = 0; k < nextra; k++) callargs[1 + k] = args[2 + k];
        if (!meth_is_user_proc(procname)) {
            const char *fbc = resolve_method_chain(cname, "FALLBACK", NULL);
            char fbp[fmt_len("%s__%s", fbc, "FALLBACK")];
            snprintf(fbp, sizeof fbp, "%s__%s", fbc, "FALLBACK");
            if (!maybe && strcmp(mname, "FALLBACK") && meth_is_user_proc(fbp)) {
                DESCR_t fargs[total + 1];
                fargs[0] = args[0];
                fargs[1] = STRVAL(rt_heap_strdup_c(mname));
                for (int k = 1; k < total; k++) fargs[1 + k] = callargs[k];
                *out = invoke_method_proc(fbp, fargs, total + 1);
                return 1;
            }
            if (maybe) *out = NULVCL;
            else rk_method_missing(args[0], mname, out);
            return 1;
        }
        rk_redisp_t rec;
        rec.self = args[0];
        rec.mname = args[1];
        rec.cname = cname;
        rec.found_idx = found_idx;
        rec.nargs = total;
        rec.args = callargs;
        rec.skip_mname = 0;
        rec.local = 1;
        rec.prev = g_redisp_cur;
        g_redisp_cur = &rec;
        DESCR_t _mr = invoke_method_proc(procname, callargs, total);
        g_redisp_cur = rec.prev;
        *out = _mr;
        return 1;
    }
    if (!strcmp(fn, "grammar_parse") && nargs == 2) { const char *gname = VARVAL_fn(args[0]); const char *subj = VARVAL_fn(args[1]); return grammar_parse_core(gname, subj, out, 0, (DESCR_t) { 0 }); }
    return 0;
}
