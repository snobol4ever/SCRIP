#include "gen_runtime.h"
#include "g_runtime.h"
#include "ct_arena.h"
#include "gen_value.h"
#include "ast.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include "gen.h"
#include "../rt/gc_heap.h"
#include "rt_diag.h"
#include "coerce.h"
#include "../by_name_dispatch.h"
#include "../../lower/lower.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
extern int core_icn_argtype_check(uint64_t lo, uint64_t hi, uint64_t code);
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_icn_subject_coerce(uint64_t lo, uint64_t hi, uint64_t mode) {
    uint64_t w[2];
    w[0] = lo;
    w[1] = hi;
    DESCR_t sv;
    memcpy(&sv, w, sizeof sv);
    ScanSubjRegs r;
    r.ptr = 0;
    r.len = 0;
    if (mode < 2) {
        extern int rt_big_is(DESCR_t);
        extern int core_icn_error(int, DESCR_t);
        int strish = (sv.v == DT_S) || (sv.v == DT_SNUL && sv.s);
        if (!strish && !IS_INT_fn(sv) && !IS_REAL_fn(sv) && !rt_big_is(sv)) { if (mode == 1) core_icn_error(103, sv); return r; }
    }
    if (sv.v == DT_BIG) { extern char *rt_big_str(DESCR_t); sv = STRVAL(rt_big_str(sv)); } else if (mode == 2 && core_icn_argtype_check(lo, hi, 103)) return r;
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str_fracdigit(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    if (IS_CSET_fn(sv)) {
        extern int kw_cset_len(const char *);
        int kn = sv.s ? kw_cset_len(sv.s) : -1;
        r.len = (kn >= 0) ? (uint64_t)kn : (uint64_t)strlen(s);
    } else r.len = (sv.v == DT_S && sv.slen) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    r.ptr = (uint64_t)(uintptr_t)s;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_icn_needle_coerce(uint64_t lo, uint64_t hi) {
    uint64_t w[2];
    w[0] = lo;
    w[1] = hi;
    DESCR_t sv;
    memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str_fracdigit(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    uint64_t L;
    if (IS_CSET_fn(sv)) {
        extern int kw_cset_len(const char *);
        int kn = sv.s ? kw_cset_len(sv.s) : -1;
        L = (kn >= 0) ? (uint64_t)kn : (uint64_t)strlen(s);
    } else L = (sv.v == DT_S && sv.slen != 0xFFFFFFFFu) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    ScanSubjRegs r;
    r.ptr = (uint64_t)(uintptr_t)s;
    r.len = L;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs c_rt_match_enter(uint64_t lo, uint64_t hi) {
    extern const char *Σ;
    extern int Σlen;
    extern void rt_cap_match_begin(void);
    extern void rt_dcap_lazy_init(void);
    rt_cap_match_begin();
    rt_dcap_lazy_init();
    uint64_t w[2];
    w[0] = lo;
    w[1] = hi;
    DESCR_t sv;
    memcpy(&sv, w, sizeof sv);
    if (sv.v == DT_A || sv.v == DT_T || sv.v == DT_P || sv.v == DT_C || sv.v == DT_E || sv.v == DT_X || IS_DATA_INST_fn(sv) || rt_name_textless(sv)) {
        extern void core_runtime_error(int, const char *);
        core_runtime_error(241, "pattern match left operand is not a string");
        ScanSubjRegs z;
        z.ptr = 0;
        z.len = 0;
        return z;
    }
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    uint64_t L = (sv.v == DT_S && sv.slen) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    Σ = s;
    Σlen = (int)L;
    ScanSubjRegs r;
    r.ptr = (uint64_t)(uintptr_t)s;
    r.len = L;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void c_rt_match_replace(const char *name, uint64_t sub_lo, uint64_t sub_hi, int64_t start, int64_t end, DESCR_t *replp) {
    extern char * rt_str_alloc(long n);
    uint64_t w[2];
    w[0] = sub_lo;
    w[1] = sub_hi;
    DESCR_t sv;
    memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    int64_t slen = (sv.v == DT_S && sv.slen != 0xFFFFFFFFu) ? (int64_t)sv.slen : (int64_t)strlen(s);
    DESCR_t rv = replp ? *replp : sv;
    if (IS_INT_fn(rv) || IS_REAL_fn(rv)) rv = descr_to_str(rv);
    const char *rs = (!replp || IS_NULL_fn(rv)) ? "" : VARVAL_fn(rv);
    if (!rs) rs = "";
    int64_t rlen = (rv.v == DT_S && rv.slen != 0xFFFFFFFFu) ? (int64_t)rv.slen : (int64_t)strlen(rs);
#if RT_DIAG
    int64_t raw_start = start, raw_end = end;
#endif
    if (start < 0) start = 0;
    if (start > slen) start = slen;
    if (end < start) end = start;
    if (end > slen) end = slen;
#if RT_DIAG
    {
        int _rpt = g_runtime.abi.repl_trace;
        if (_rpt < 0) { const char *_e = getenv("SCRIP_REPL_TRACE"); _rpt = g_runtime.abi.repl_trace = (_e && _e[0]) ? 1 : 0; }
        if (_rpt)
            fprintf(stderr, "[REPL] name=%s slen=%lld raw_start=%lld raw_end=%lld start=%lld end=%lld rs=\"%s\" rlen=%lld\n", name?name:"(null)", (long long)slen, (long long)raw_start,
            (long long)raw_end, (long long)start, (long long)end, rs, (long long)rlen);
    }
#endif
    int64_t nlen = start + rlen + (slen - end);
    char *buf = rt_str_alloc((long)nlen);
    if (buf) { memcpy(buf, s, (size_t)start); memcpy(buf + start, rs, (size_t)rlen); memcpy(buf + start + rlen, s + end, (size_t)(slen - end)); buf[nlen] = '\0'; }
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)nlen, .s = buf ? buf : "" };
    if (name && name[0]) NV_SET_fn(name, d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_match_capture(uint64_t sigma, int64_t start, int64_t end, const char *var) {
    DESCR_t sub = rt_substr((const char *)(uintptr_t)sigma, start, end);
    if (var && var[0]) NV_SET_fn(var, sub);
    return sub;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long cvpos_of(DESCR_t v, long len, int *ok) {
    long i;
    *ok = 1;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) { *ok = 0; return 0; } i = t; } else { *ok = 0; return 0; }
    if (i < -len || i > len + 1) { *ok = 0; return 0; }
    return (i > 0) ? i : (len + i + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_cvpos_pos(DESCR_t v, int64_t len) { int ok; long p = cvpos_of(v, (long)len, &ok); return ok ? (int64_t)p : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rsw_get(long kind, DESCR_t *vp, int64_t *spill) {
    if (kind == 0) return vp ? *vp : NULVCL;
    if (kind == 1) return INTVAL(spill[0] + 1);
    if (kind == 3) { return INTVAL((int64_t)g_runtime.abi.random); }
    if (kind == 4) { return INTVAL((int64_t)g_runtime.abi.error); }
    if (kind == 6) { return INTVAL((int64_t)g_runtime.abi.dump); }
    fprintf(stderr, "[REVSWAP] FATAL: <-> read of unimplemented keyword kind %ld (only plain vars and &pos are wired; add the kind to rsw_get/rsw_set)\n", kind);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rsw_set(long kind, DESCR_t *vp, int64_t *spill, DESCR_t v) {
    if (kind == 0) { if (vp) *vp = v; return 1; }
    if (kind == 1) { int ok; long p = cvpos_of(v, (long)spill[1], &ok); if (!ok) return 0; spill[0] = (int64_t)(p - 1); return 1; }
    extern DESCR_t rt_keyword_random_set(DESCR_t), rt_keyword_error_set(DESCR_t), rt_keyword_dump_set(DESCR_t);
    if (kind == 3) return !IS_FAIL_fn(rt_keyword_random_set(v));
    if (kind == 4) return !IS_FAIL_fn(rt_keyword_error_set(v));
    if (kind == 6) return !IS_FAIL_fn(rt_keyword_dump_set(v));
    fprintf(stderr, "[REVSWAP] FATAL: <-> write of unimplemented keyword kind %ld (only plain vars and &pos are wired; add the kind to rsw_get/rsw_set)\n", kind);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_rev_swap_fwd(long lkind, DESCR_t *lp, long rkind, DESCR_t *rp, DESCR_t *save, int64_t *spill) {
    DESCR_t old_l = rsw_get(lkind, lp, spill);
    DESCR_t old_r = rsw_get(rkind, rp, spill);
    save[0] = old_l;
    save[1] = old_r;
    if (!rsw_set(lkind, lp, spill, old_r)) return FAILDESCR;
    if (!rsw_set(rkind, rp, spill, old_l)) return FAILDESCR;
    return old_r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_rev_swap_undo(long lkind, DESCR_t *lp, long rkind, DESCR_t *rp, DESCR_t *save, int64_t *spill) {
    if (!rsw_set(lkind, lp, spill, save[0])) return FAILDESCR;
    rsw_set(rkind, rp, spill, save[1]);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_random_set(DESCR_t v) {
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; } else return FAILDESCR;
    g_runtime.abi.random = i;
    return INTVAL((int64_t)i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_error_set(DESCR_t v) {
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; } else return FAILDESCR;
    g_runtime.abi.error = i;
    return INTVAL((int64_t)i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
DESCR_t rt_keyword_trace_set(DESCR_t v) {
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; } else return FAILDESCR;
    g_runtime.abi.trace = i;
    { extern void rt_trace_all_set(int on); rt_trace_all_set(i != 0); }
    return INTVAL((int64_t)i);
}
#else
DESCR_t rt_keyword_trace_set(DESCR_t v) {
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; } else return FAILDESCR;
    g_runtime.abi.trace = i;
    return INTVAL((int64_t)i);
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_dump_set(DESCR_t v) {
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; } else return FAILDESCR;
    g_runtime.abi.dump = i;
    return INTVAL((int64_t)i);
}
#include "../../driver/driver_private.h"
#include <time.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
