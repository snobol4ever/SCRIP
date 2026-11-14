#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_rk_av.h"
#include "runtime/by_name_dispatch.h"
#include <sys/stat.h>
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include "runtime/rt/rt_list_view.h"
#include "runtime/lib/bn_02_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_hb_aggb.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/gc_heap_rt_gc_callback.h"
#include "runtime/builtins/gen.h"
#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern void *rt_ws_alloc_descr(size_t);
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs);
extern FILE *fh_get(int);
int icn_open_spec_is_icon(const char *spec) { for (const char *p = spec; p && *p; p++) if (!strchr("aAbBcCrRwWuUtTpPhH", *p)) return 0; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_open_untranslated(const char *spec) { int un = 0; for (const char *p = spec; p && *p; p++) { if (*p == 'u' || *p == 'U') un = 1; else if (*p == 't' || *p == 'T') un = 0; } return un; }
void icn_open_cmode(const char *spec, char *out, int *is_pipe) {
    int rd = 0, wr = 0, ap = 0, cr = 0, un = 0, pi = 0, n = 0;
    for (const char *p = spec; p && *p; p++) switch (*p) {
        case 'a':
        case 'A':
        wr = 1;
        ap = 1;
        break;
        case 'b':
        case 'B':
        rd = 1;
        wr = 1;
        break;
        case 'c':
        case 'C':
        cr = 1;
        wr = 1;
        break;
        case 'r':
        case 'R':
        rd = 1;
        break;
        case 'w':
        case 'W':
        wr = 1;
        break;
        case 'u':
        case 'U':
        un = 1;
        break;
        case 'p':
        case 'P':
        pi = 1;
        break;
        case 't':
        case 'T':
        un = 0;
        break;
        default:
        break;
    }
    if (!rd && !wr) rd = 1;
    out[n++] = cr ? 'w' : ap ? 'a' : rd ? 'r' : 'w';
    if (rd && wr) out[n++] = '+';
    if (un && !pi) out[n++] = 'b';
    out[n] = '\0';
    if (is_pipe) *is_pipe = pi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_gist_text(DESCR_t v, char open, char close) {
    rk_av_t a = rk_av(v);
    const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
    for (int i = 0; i < a.n; i++) tx[i] = rk_gist_str(a.el[i]);
    char op[2] = { open, 0 }, cl[2] = { close, 0 };
    return rk_join_gists(tx, a.n, " ", op, cl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_capture_list(int g, int wrap) {
    int n = 0;
    for (int k = 0; k < g_match.ncaplog; k++) if (match_caplog_group(&g_match, k) == g) n++;
    DESCR_t *elems = rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
    int j = 0;
    for (int k = 0; k < g_match.ncaplog; k++) {
        if (match_caplog_group(&g_match, k) != g) continue;
        int cs = match_caplog_start(&g_match, k), ce = match_caplog_end(&g_match, k), len = ce > cs ? ce - cs : 0;
        char *o = rt_wsb_alloc((size_t)len + (wrap ? 7 : 1));
        if (wrap) {
            memcpy(o, "\xef\xbd\xa2", 3);
            if (len) memcpy(o + 3, g_subject + cs, (size_t)len);
            memcpy(o + 3 + len, "\xef\xbd\xa3", 3);
            o[len + 6] = '\0';
        } else {
            if (len) memcpy(o, g_subject + cs, (size_t)len);
            o[len] = '\0';
        }
        elems[j++] = STRVAL(o);
    }
    extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
    return rt_make_list(elems, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_failure_new(DESCR_t ex) {
    static int rk_failure_reg = 0;
    if (!rk_failure_reg) { DEFDAT_fn("Failure(exception,handled)"); rk_failure_reg = 1; }
    return DATCON_fn("Failure", ex, INTVAL(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_failure_of(DESCR_t *args, int nargs) {
    if (nargs >= 1 && rk_exc_is(args[0])) return rk_failure_new(args[0]);
    return rk_failure_new(rk_exc_adhoc(nargs >= 1 ? args[0] : STRVAL(rt_heap_strdup_c("Failed"))));
}
static DESCR_t bta_rk_write(DESCR_t *args, int nargs, int _bop) {
    for (int _fi = 0; _fi < nargs; _fi++) if (rk_failure_is(args[_fi]) && !FIELD_GET_fn(args[_fi], "handled").i) { DESCR_t _fo; rk_failure_method("Str", &args[_fi], 1, &_fo); return FAILDESCR; }
    {
        DESCR_t tmp[nargs > 0 ? nargs : 1];
        for (int _ri = 0; _ri < nargs; _ri++) {
            if (args[_ri].v == DT_BOOL) tmp[_ri] = STRVAL(rt_heap_strdup_c(args[_ri].i ? "True" : "False"));
            else if (rk_failure_is(args[_ri])) tmp[_ri] = rk_failure_str(args[_ri]);
            else if (args[_ri].v == DT_ORDER) tmp[_ri] = STRVAL(rt_heap_strdup_c(args[_ri].i < 0 ? "Less" : (args[_ri].i > 0 ? "More" : "Same")));
            else if (args[_ri].v == DT_SNUL) tmp[_ri] = STRVAL(rt_heap_strdup_c("Nil"));
            else if ((_bop == 0) && rk_seq_is_lazy(args[_ri])) tmp[_ri] = STRVAL(rt_heap_strdup_c("(...)"));
            else if (IS_REAL_fn(args[_ri])) {
                char *_rb = rt_wsb_alloc(64);
                rk_real_str(args[_ri].r, _rb, 64);
                tmp[_ri] = STRVAL(_rb);
            } else if (rk_typeobj_name(args[_ri])) {
                const char *tn = rk_typeobj_name(args[_ri]);
                { const char *sc = strrchr(tn, ':'); if (sc && strcmp(tn, "Nil")) tn = sc + 1; }
                size_t l = strlen(tn);
                char *r = rt_wsb_alloc(l + 3);
                if ((_bop == 0) && !strcmp(tn, "Nil")) memcpy(r, "Nil", 4);
                else if ((_bop == 0)) { r[0] = '('; memcpy(r + 1, tn, l); r[l + 1] = ')'; r[l + 2] = '\0'; } else r[0] = '\0';
                tmp[_ri] = STRVAL(r);
            } else tmp[_ri] = args[_ri];
        }
        return RT_GC_CALLBACK(rt_call_arr((_bop == 0) ? "write" : "writes", tmp, nargs));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_write(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_rk_write(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rk_writes(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_rk_write(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rk_write_list(DESCR_t *args, int nargs) {
    if (nargs == 1) { DESCR_t tmp1 = STRVAL((char *) rk_gist_text(args[0], '(', ')')); return RT_GC_CALLBACK(rt_call_arr("write", &tmp1, 1)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_write_arr(DESCR_t *args, int nargs) {
    if (nargs == 1) { DESCR_t tmp1 = STRVAL((char *) rk_gist_text(args[0], '[', ']')); return RT_GC_CALLBACK(rt_call_arr("write", &tmp1, 1)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t rk_dz_failure(DESCR_t a, int is_div) {
    rk_exc_init();
    char ab[64];
    const char *as = IS_INT_fn(a) ? NULL : to_cstring(a, ab, sizeof ab);
    char msg[160];
    if (IS_INT_fn(a)) snprintf(msg, sizeof msg, "Attempt to divide %lld by zero using %s", (long long) a.i, is_div ? "div" : "%");
    else snprintf(msg, sizeof msg, "Attempt to divide %s by zero using %s", as ? as : "", is_div ? "div" : "%");
    return rk_failure_new(DATCON_fn("X::Numeric::DivideByZero", STRVAL(rt_heap_strdup_c(msg)), NULVCL, NULVCL, NULVCL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bta___rk_intdiv(DESCR_t *args, int nargs, int _bop) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 2) {
        extern void rt_script_die_surface(const char *msg);
        int is_div = ((_bop == 0));
        DESCR_t a = args[0], b = args[1];
        if (a.v == DT_BOOL) a = INTVAL(a.i != 0);
        if (b.v == DT_BOOL) b = INTVAL(b.i != 0);
        { extern int rk_rat_binop(DESCR_t, DESCR_t, int, DESCR_t *); if (!is_div && (a.v == DT_DATA || b.v == DT_DATA) && rk_rat_binop(a, b, BINOP_MOD, out)) return _bo; }
        if ((a.v == DT_BIG || b.v == DT_BIG) && (a.v == DT_BIG || a.v == DT_I) && (b.v == DT_BIG || b.v == DT_I)) {
            extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
            extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
            extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
            extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
            extern int rt_big_sign(DESCR_t);
            int bs = rt_big_sign(b);
            if (bs == 0) { rt_script_die_surface(is_div ? "Attempt to divide by zero" : "Attempt to divide by zero using infix:<%>"); return FAILDESCR; }
            DESCR_t q = rt_big_div(a, b), r = rt_big_mod(a, b);
            int rs = rt_big_sign(r);
            if (rs != 0 && (rs < 0) != (bs < 0)) { q = rt_big_sub(q, INTVAL(1)); r = rt_big_add(r, b); }
            return is_div ? q : r;
        }
        int ai = IS_INT_fn(a), arl = IS_REAL_fn(a), bi = IS_INT_fn(b), brl = IS_REAL_fn(b);
        double ad = arl ? a.r : (ai ? (double)a.i : 0.0), bd = brl ? b.r : (bi ? (double)b.i : 0.0);
        if (!ai && !arl) { char sa[64]; const char *cs = to_cstring(a, sa, sizeof sa); ad = cs ? strtod(cs, (char **)0) : 0.0; }
        if (!bi && !brl) { char sb[64]; const char *cs = to_cstring(b, sb, sizeof sb); bd = cs ? strtod(cs, (char **)0) : 0.0; }
        if (ai && bi) {
            if (b.i == 0) return rk_dz_failure(a, is_div);
            if (b.i == -1) { extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int); return is_div ? rt_num_arith(a, b, BINOP_DIV) : INTVAL(0); }
            long long q = a.i / b.i, r = a.i % b.i;
            if (r != 0 && ((r < 0) != (b.i < 0))) { q -= 1; r += b.i; }
            return INTVAL(is_div ? q : r);
        }
        if (bd == 0.0) return rk_dz_failure(a, is_div);
        double qf = floor(ad / bd);
        if (is_div) { return REALVAL(qf); }
        double rf = ad - bd * qf;
        return REALVAL(rf);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_mod(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_intdiv(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_intdiv(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_intdiv(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_uc(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        size_t n = strlen(s);
        char *o = rt_wsb_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i]);
        o[n] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_lc(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        size_t n = strlen(s);
        char *o = rt_wsb_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'A' && s[i] <= 'Z') ? s[i] + 32 : s[i]);
        o[n] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_chars(DESCR_t *args, int nargs) { if (nargs == 1) { const char *s = VARVAL_fn(args[0]); if (!s) s = ""; return INTVAL((long)strlen(s)); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_array_sort(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        rk_av_t a = rk_av(args[0]);
        if (!a.n) { return rk_mk_arr(NULL, 0); }
        const char **tx = (const char **) rt_pvec_alloc((size_t) a.n);
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) a.n);
        long long *kv = (long long *) rt_wsb_alloc(sizeof(long long) * (size_t) a.n);
        int all_int = 1;
        for (int i = 0; i < a.n; i++) { char *ep; tx[i] = rk_av_text(a, i); r[i] = a.el[i]; kv[i] = strtoll(tx[i], &ep, 10); if (*ep) all_int = 0; }
        for (int i = 1; i < a.n; i++) {
            const char *kt = tx[i];
            DESCR_t kd = r[i];
            long long kn = kv[i];
            int j = i - 1;
            while (j >= 0 && (all_int ? kv[j] > kn : strcmp(tx[j], kt) > 0)) { tx[j + 1] = tx[j]; r[j + 1] = r[j]; kv[j + 1] = kv[j]; j--; }
            tx[j + 1] = kt;
            r[j + 1] = kd;
            kv[j + 1] = kn;
        }
        return rk_mk_arr(r, a.n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_array_reverse(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr;
            int n = b->hi - b->lo + 1;
            if (n < 0) n = 0;
            ARBLK_t *r = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
            r->id = rt_agg_serial_list();
            r->dumpno = rt_sno_dumpno_next();
            r->lo = b->lo;
            r->hi = b->hi;
            r->ndim = 1;
            r->lo2 = 0;
            r->hi2 = 0;
            r->proto_bare = 0;
            r->data = (DESCR_t *)rt_ws_alloc_descr((size_t)(n ? n : 1));
            for (int i = 0; i < n; i++) r->data[i] = b->data[n - 1 - i];
            DESCR_t d = {0};
            d.v = DT_A;
            d.slen = 0;
            d.arr = r;
            return d;
        }
        rk_av_t a = rk_av(args[0]);
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) r[i] = a.el[a.n - 1 - i];
        return rk_mk_arr(r, a.n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_failure(DESCR_t *args, int nargs) { if (nargs <= 1) { return rk_failure_of(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_re_capture(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        int n = rk_positional_group((int)(IS_INT_fn(args[0]) ? args[0].i : 0));
        if (g_match.matched && n >= 0 && n < g_match.ngroups && match_group_repeatable(&g_match, n)) { return rk_capture_list(n, 0); }
        if (!g_match.matched || n < 0 || n >= g_match.ngroups || match_group_start(&g_match, n) < 0) { return STRVAL(rt_heap_strdup_c("")); }
        int gs = match_group_start(&g_match, n), ge = match_group_end(&g_match, n);
        if (ge < gs) { return STRVAL(rt_heap_strdup_c("")); }
        int len = ge - gs;
        char *o = rt_wsb_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len);
        o[len] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_re_named_capture(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        const char *name = VARVAL_fn(args[0]);
        if (!name) name = "";
        if (!g_match.matched) { return STRVAL(rt_heap_strdup_c("")); }
        int g = -1;
        for (int i = 0; i < g_match.ngroups; i++) if (strcmp(match_group_name(&g_match, i), name) == 0) { g = i; break; }
        if (g >= 0 && match_group_repeatable(&g_match, g)) { return rk_capture_list(g, 0); }
        if (g < 0 || match_group_start(&g_match, g) < 0) { return STRVAL(rt_heap_strdup_c("")); }
        int gs = match_group_start(&g_match, g), ge = match_group_end(&g_match, g);
        if (ge < gs) { return STRVAL(rt_heap_strdup_c("")); }
        int len = ge - gs;
        char *o = rt_wsb_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len);
        o[len] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_push_pure(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        int is_app = (_bop == 2), is_pre = (_bop == 0 || _bop == 3) && (_bop == 3), is_push = (_bop == 0 || _bop == 3) && (_bop == 0);
        int back = is_push || is_app, flat = (is_app || is_pre) && nargs == 2;
        if (IS_INT_fn(args[0]) || IS_REAL_fn(args[0])) {
            extern void rt_script_die_surface(const char *msg);
            const char *base = is_push ? "push" : is_app ? "append" : is_pre ? "prepend" : "unshift";
            char m[fmt_len("Cannot resolve caller %s(...); none of these signatures matches", base)];
            snprintf(m, sizeof m, "Cannot resolve caller %s(...); none of these signatures matches", base);
            rt_script_die_surface(m);
            return FAILDESCR;
        }
        if (back && args[0].v == DT_A && args[0].arr && ((ARBLK_t *) args[0].arr)->proto == NULL) {
            int add = 0;
            for (int i = 1; i < nargs; i++) add += flat ? rk_spill_count(args[i]) : rk_item_count(args[i]);
            DESCR_t *t = (DESCR_t *) rt_ws_alloc_descr((size_t) (add ? add : 1));
            int k = 0;
            for (int i = 1; i < nargs; i++) k = flat ? rk_spill(t, k, args[i]) : rk_item(t, k, args[i]);
            for (int i = 0; i < k; i++) rk_arr_append((ARBLK_t *) args[0].arr, t[i]);
            return args[0];
        }
        rk_av_t a = rk_av(args[0]);
        int add = 0;
        for (int i = 1; i < nargs; i++) add += flat ? rk_spill_count(args[i]) : rk_item_count(args[i]);
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) (a.n + add));
        int k = 0;
        if (back) for (int i = 0; i < a.n; i++) r[k++] = rk_av_elem(a, i);
        for (int i = 1; i < nargs; i++) k = flat ? rk_spill(r, k, args[i]) : rk_item(r, k, args[i]);
        if (!back) for (int i = 0; i < a.n; i++) r[k++] = rk_av_elem(a, i);
        return rk_unmark(rk_mk_arr(r, k));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_unshift_pure(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_push_pure(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_append_pure(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_push_pure(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_push_pure(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_push_pure(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_prepend_pure(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_push_pure(args, nargs, 3); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rk_x24system(DESCR_t *args, int nargs) {
    if (nargs == 1) { const char *cmd = VARVAL_fn(args[0]); if (!cmd) { return FAILDESCR; } fflush(NULL); return INTVAL(system(cmd)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_open(DESCR_t *args, int nargs) {
    if ((nargs == 1 || nargs == 2)) {
        const char *path = VARVAL_fn(args[0]);
        if (!path || !*path) { return FAILDESCR; }
        const char *mode = "r";
        char mbuf[8];
        int is_pipe = 0;
        if (nargs == 2) {
            const char *ms = VARVAL_fn(args[1]);
            if (!ms) ms = "";
            if (icn_open_spec_is_icon(ms)) { icn_open_cmode(ms, mbuf, &is_pipe); mode = mbuf; } else if (strstr(ms, ":w") || strstr(ms, "w")) mode = "w";
            else if (strstr(ms, ":a") || strstr(ms, "a")) mode = "a";
        }
        extern void fh_ensure_init(void);
        extern int fh_alloc(FILE *);
        fh_ensure_init();
        if (is_pipe && (strcmp(mode, "r") && strcmp(mode, "w"))) { return FAILDESCR; }
        if (is_pipe) { fflush(NULL); setvbuf(stdout, NULL, _IOLBF, 0); }
        FILE *fp = NULL;
        extern FILE *rt_dir_snapshot(const char *);
        int _isdir = 0;
        struct stat _dst;
        if (!is_pipe && !strcmp(mode, "r") && !stat(path, &_dst) && S_ISDIR(_dst.st_mode)) { fp = rt_dir_snapshot(path); if (!fp) { return FAILDESCR; } _isdir = 1; }
        if (!fp) fp = is_pipe ? popen(path, mode) : fopen(path, mode);
        if (!fp) { return FAILDESCR; }
        int idx = fh_alloc(fp);
        if (idx < 0) { if (is_pipe) pclose(fp); else fclose(fp); return FAILDESCR; }
        if (is_pipe) g_fh[idx].type = 'p';
        if (_isdir && idx >= 0 && idx < FH_N) g_fh[idx].type = 'd';
        {
            extern void fh_set_untranslated(int, int);
            const char *_us = (nargs == 2 && (args[1].v == DT_S || args[1].v == DT_SNUL)) ? VARVAL_fn(args[1]) : NULL;
            fh_set_untranslated(idx, (_us && icn_open_spec_is_icon(_us)) ? icn_open_untranslated(_us) : 0);
        }
        return INTVAL(idx);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_close(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        extern FILE *fh_get(int);
        extern void fh_free(int);
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        FILE *fp = fh_get(idx);
        int pst = -1;
        if (fp) { if (g_fh[idx].type == 'p') { fflush(stdout); pst = pclose(fp); } else fclose(fp); g_fh[idx].type = 0; fh_free(idx); }
        if (pst >= 0) { return INTVAL((pst >> 8) & 0xFF); }
        return INTVAL(0);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_say_capture(DESCR_t *args, int nargs, int _bop) {
    static const char *const _bn[] = { "__rk_say_capture", "__rk_say_named_capture", "__rk_gist_capture", "__rk_gist_named_capture" };
    const char *const fn = _bn[_bop];
    if (nargs == 1) {
        int g = -1, say = fn[5] == 's', named = fn[say ? 9 : 10] == 'n';
        if (g_match.matched && !named) g = rk_positional_group(IS_INT_fn(args[0]) ? (int) args[0].i : -1);
        else if (g_match.matched) { const char *name = VARVAL_fn(args[0]); for (int i = 0; name && i < g_match.ngroups; i++) if (!strcmp(match_group_name(&g_match, i), name)) { g = i; break; } }
        if (g >= 0 && g < g_match.ngroups && match_group_repeatable(&g_match, g)) {
            DESCR_t l = rk_capture_list(g, 1);
            DESCR_t *ea = NULL;
            int ln = 0;
            rt_lv_is_list(l, &ea, &ln);
            rk_av_t av = { ln, ea, 0 };
            DESCR_t s2 = STRVAL(rk_av_joined(av, " ", "[", "]"));
            return say ? RT_GC_CALLBACK(rt_call_arr("write", &s2, 1)) : s2;
        }
        DESCR_t s;
        if (g < 0 || g >= g_match.ngroups || match_group_start(&g_match, g) < 0 || match_group_end(&g_match, g) < match_group_start(&g_match, g)) s = STRVAL(rt_heap_strdup_c("Nil"));
        else {
            int gs = match_group_start(&g_match, g), len = match_group_end(&g_match, g) - gs;
            char *o = rt_wsb_alloc((size_t) len + 7);
            memcpy(o, "\xef\xbd\xa2", 3);
            memcpy(o + 3, g_subject + gs, (size_t) len);
            memcpy(o + 3 + len, "\xef\xbd\xa3", 3);
            o[len + 6] = '\0';
            s = STRVAL(o);
        }
        return say ? RT_GC_CALLBACK(rt_call_arr("write", &s, 1)) : s;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_say_named_capture(DESCR_t *args, int nargs) {
    { const DESCR_t _r = bta___rk_say_capture(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_gist_capture(DESCR_t *args, int nargs) {
    { const DESCR_t _r = bta___rk_say_capture(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_gist_named_capture(DESCR_t *args, int nargs) {
    { const DESCR_t _r = bta___rk_say_capture(args, nargs, 3); if (_r.v != RTX_NOT_HANDLED) return _r; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_say_capture(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_say_capture(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_undef(DESCR_t *args, int nargs) { { (void) args; (void) nargs; return NULVCL; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs) { return RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, -1, 0, 0, (long *)0)); }
