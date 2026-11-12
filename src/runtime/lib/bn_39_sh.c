#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_39_sh.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_numrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    DESCR_t _l = args[0], _r = args[1];
    _SNOCOERCE(_l);
    _SNOCOERCE(_r);
    double a = IS_REAL_fn(_l) ? _l.r : (double)_l.i, b = IS_REAL_fn(_r) ? _r.r : (double)_r.i;
    int ok = op==0?(a==b):op==1?(a!=b):op==2?(a<b):op==3?(a<=b):op==4?(a>b):(a>=b);
    *out = ok ? NULVCL : FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_lexrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    int c = lex_cmp_pair(args[0], args[1]);
    int ok = op==0?(c>0):op==1?(c<0):op==2?(c>=0):op==3?(c<=0):op==4?(c==0):(c!=0);
    *out = ok ? NULVCL : FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_identdiffer(DESCR_t *args, int nargs, DESCR_t *out, int ident) {
    extern int c_descr_identical(DESCR_t, DESCR_t);
    DESCR_t a = nargs > 0 ? args[0] : NULVCL, b = nargs > 1 ? args[1] : NULVCL;
    int same = rtx_try_descr_identical(a, b);
    *out = (ident ? same : !same) ? NULVCL : FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_size(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t v = args[0];
    if (IS_FAIL_fn(v)) { *out = FAILDESCR; return 1; }
    if (rt_name_textless(v)) { core_runtime_error(189, "SIZE argument is not a string"); *out = FAILDESCR; return 1; }
    if (v.v == DT_T) { *out = INTVAL(v.tbl ? v.tbl->size : 0); return 1; }
    if (v.v == DT_CO) { extern long scrip_coexpr_activations_of(void *); *out = INTVAL(scrip_coexpr_activations_of(v.p)); return 1; }
    if (v.v == DT_A) { *out = INTVAL(v.arr ? (v.arr->hi - v.arr->lo + 1) : 0); return 1; }
    if (v.v == DT_DATA) { int ln = 0; if (rt_lv_is_list(v, (DESCR_t **)0, &ln)) { *out = INTVAL(ln); return 1; } }
    if (v.v == DT_I) { const char *is = VARVAL_fn(v); *out = INTVAL(is ? (long)strlen(is) : 0); return 1; }
    if (v.v == DT_R) { char rb[40]; real_str(v.r, rb, (int)sizeof rb); *out = INTVAL((long)strlen(rb)); return 1; }
    if (v.v == DT_S && v.slen == 0xFFFFFFFFu) { *out = INTVAL((long)descr_slen(v)); return 1; }
    const char *s = VARVAL_fn(v);
    if (!s) { *out = INTVAL(0); return 1; }
    long len = v.slen > 0 ? v.slen : (long)strlen(s);
    *out = INTVAL(len);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_time(DESCR_t *args, int nargs, DESCR_t *out) { (void)args; if (nargs > 1) return -1; *out = INTVAL(rt_time_ns()); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_date(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)args;
    if (nargs > 1) return -1;
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char *buf = (char *)rt_heap_alloc_c(64);
    strftime(buf, 64, "%m/%d/%y %H:%M:%S", tm);
    *out = STRVAL(buf);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_trim(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    if (rt_str_arg_bad(args[0])) { core_runtime_error(200, "trim argument is not a string"); *out = FAILDESCR; return 1; }
    const char *sv = VARVAL_fn(args[0]);
    if (!sv) sv = "";
    size_t n = sv_len(args[0], sv);
    while (n > 0 && (sv[n-1] == ' ' || sv[n-1] == '\t')) n--;
    char *buf = (char *)rt_heap_alloc_c(n + 1);
    memcpy(buf, sv, n);
    buf[n] = 0;
    *out = BSTRVAL(buf, n);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_dupl(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    if (!is_numeric_like(args[1])) { core_runtime_error(90, "dupl second argument is not integer"); *out = FAILDESCR; return 1; }
    if (args[0].v == DT_P || args[0].v == DT_X || rt_str_arg_bad(args[0])) { extern DESCR_t DUPL_fn(DESCR_t, DESCR_t); *out = DUPL_fn(args[0], args[1]); return 1; }
    const char *sv = VARVAL_fn(args[0]);
    if (!sv) sv = "";
    DESCR_t nn = args[1];
    _SNOCOERCE(nn);
    long long k = IS_REAL_fn(nn) ? (long long)nn.r : nn.i;
    if (k < 0) { *out = FAILDESCR; return 1; }
    size_t sl = sv_len(args[0], sv);
    if (sl > 0 && k > g_runtime.abi.maxlngth / (int64_t)sl) { *out = FAILDESCR; return 1; }
    char *buf = (char *)rt_heap_alloc_c(sl * (size_t)k + 1);
    for (long long i = 0; i < k; i++) memcpy(buf + (size_t)i * sl, sv, sl);
    buf[sl * (size_t)k] = 0;
    *out = BSTRVAL(buf, sl * (size_t)k);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int repl_pl_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_REPL_PL"); v = (e && *e == '0') ? 1 : 0; } return v; }
void c_rt_translate_bytes(char *dst, const char *src, size_t n, const char *map) { for (size_t i = 0; i < n; i++) dst[i] = map[(unsigned char)src[i]]; }
__attribute__((noinline)) int bn_replace(DESCR_t *args, int nargs, DESCR_t *out) {
    static struct { unsigned char v, n; char rm_from[63], rm_to[63], rm_map[256]; } g_rm[4];
    static int g_rm_off = -1;
    if (g_rm_off == 0 && nargs == 3 && args[0].v == DT_S && args[1].v == DT_S && args[2].v == DT_S) {
        const char *fsv = args[0].s, *ffv = args[1].s, *ftv = args[2].s;
        uint32_t fsl = args[0].slen, ffl = args[1].slen;
        if (fsv && ffv && ftv && fsl && fsl != 0xFFFFFFFFu && ffl && ffl == args[2].slen && ffl < 63u) {
            unsigned fs = ((unsigned char)ffv[0] * 31u + ffl) & 3u;
            if (g_rm[fs].v && g_rm[fs].n == ffl && !memcmp(g_rm[fs].rm_from, ffv, ffl) && !memcmp(g_rm[fs].rm_to, ftv, ffl)) {
                char *fbuf = (char *)rt_gcheap_alloc(HB_WSC, (uint64_t)fsl + 1);
                rt_translate_bytes(fbuf, fsv, fsl, g_rm[fs].rm_map);
                fbuf[fsl] = 0;
                *out = BSTRVAL(fbuf, fsl);
                return 1;
            }
        }
    }
    if (nargs != 3) return -1;
    if (rt_str_arg_bad(args[2])) { core_runtime_error(168, "replace third argument is not a string"); *out = FAILDESCR; return 1; }
    if (rt_str_arg_bad(args[1])) { core_runtime_error(169, "replace second argument is not a string"); *out = FAILDESCR; return 1; }
    const char *sv, *fv, *tv;
    size_t sl, fl, tl;
    const int _plf = !repl_pl_off();
    BN_PTRLEN(args[0], sv, sl, _plf);
    BN_PTRLEN(args[1], fv, fl, _plf);
    BN_PTRLEN(args[2], tv, tl, _plf);
    if (fl != tl || !fl) { core_runtime_error(171, "null or unequally long 2nd, 3rd args to replace"); *out = FAILDESCR; return 1; }
    if (rt_str_arg_bad(args[0])) { core_runtime_error(170, "replace first argument is not a string"); *out = FAILDESCR; return 1; }
    if (g_rm_off < 0) { const char *e = getenv("SCRIP_REPLMAP_OFF"); g_rm_off = (e && *e) ? 1 : repl_pl_off() ? 2 : 0; }
    char mloc[256];
    char *map = mloc;
    if (g_rm_off != 1 && fl < 63) {
        unsigned s = ((unsigned char)fv[0] * 31u + (unsigned)fl) & 3u;
        if (g_rm[s].v && g_rm[s].n == (unsigned char)fl && !memcmp(g_rm[s].rm_from, fv, fl) && !memcmp(g_rm[s].rm_to, tv, fl)) map = g_rm[s].rm_map;
        else {
            map = g_rm[s].rm_map;
            for (int i = 0; i < 256; i++) map[i] = (char)i;
            for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k];
            memcpy(g_rm[s].rm_from, fv, fl);
            memcpy(g_rm[s].rm_to, tv, fl);
            g_rm[s].n = (unsigned char)fl;
            g_rm[s].v = 1;
        }
    } else {
        for (int i = 0; i < 256; i++) map[i] = (char)i;
        for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k];
    }
    size_t n = sl;
    char *buf = (char *)rt_heap_alloc_c(n + 1);
    rt_translate_bytes(buf, sv, n, map);
    buf[n] = 0;
    *out = BSTRVAL(buf, n);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_substr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = SUBSTR_bytes_fn(args[0], args[1], (nargs == 3) ? args[2] : INTVAL(0));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_reverse(DESCR_t *args, int nargs, DESCR_t *out) { if (nargs != 1) return -1; *out = REVERS_fn(args[0]); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_lpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = lpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" "));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_rpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = rpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" "));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_integer(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t av = args[0];
    if (IS_INT_fn(av)) { *out = NULVCL; return 1; }
    if (IS_REAL_fn(av) || rt_name_textless(av)) { *out = FAILDESCR; return 1; }
    const char *sv = IS_STR(av) ? rt_cstr_d(av) : VARVAL_fn(av);
    if (!sv) sv = "";
    const char *p = sv;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0') { *out = NULVCL; return 1; }
    if (*p == '+' || *p == '-') p++;
    const char *d = p;
    while (*p >= '0' && *p <= '9') p++;
    const char *e = p;
    while (*e == ' ' || *e == '\t') e++;
    *out = (p != d && *e == '\0') ? NULVCL : FAILDESCR;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline)) int bn_remdr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    DESCR_t a = args[0], b = args[1];
    if (!is_numeric_like(a)) { core_runtime_error(166, "remdr first argument is not numeric"); *out = FAILDESCR; return 1; }
    if (!is_numeric_like(b)) { core_runtime_error(165, "remdr second argument is not numeric"); *out = FAILDESCR; return 1; }
    _SNOCOERCE(a);
    _SNOCOERCE(b);
    if (IS_REAL_fn(a) || IS_REAL_fn(b)) {
        double rb = IS_REAL_fn(b) ? b.r : (double)b.i;
        if (rb == 0.0) { core_runtime_error(312, "remdr caused real overflow"); *out = FAILDESCR; return 1; }
        double rr = fmod(IS_REAL_fn(a) ? a.r : (double)a.i, rb);
        if (isinf(rr)) { core_runtime_error(312, "remdr caused real overflow"); *out = FAILDESCR; return 1; }
        *out = REALVAL(rr);
        return 1;
    }
    long long ai = a.i, bi = b.i;
    if (bi == 0) { core_runtime_error(167, "remdr caused integer overflow"); *out = FAILDESCR; return 1; }
    if (bi == -1) { *out = INTVAL(0); return 1; }
    *out = INTVAL(ai % bi);
    return 1;
}
