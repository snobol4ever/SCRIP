#include "rt/rt_arena.h"
#include "core.h"
#include "sil_macros.h"
#include "coerce.h"
#include "string_ops.h"
#include "rt/gc_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int so_is_list(DESCR_t v) {
    if (v.v != DT_DATA || !v.u) return 0;
    DESCR_t t = FIELD_GET_fn(v, "gen_type");
    return t.v == DT_S && t.s && !strcmp(t.s, "list");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_str_concat_d(DESCR_t a, DESCR_t b) {
    { extern void rt_gc_point_arr(DESCR_t *, int, const char **); DESCR_t sh[2]; sh[0] = a; sh[1] = b; rt_gc_point_arr(sh, 2, (const char **)0); a = sh[0]; b = sh[1]; }
    if (a.v == DT_P || b.v == DT_P || a.v == DT_X || b.v == DT_X) { extern DESCR_t pat_cat(DESCR_t, DESCR_t); return pat_cat(a, b); }
    extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return FAILDESCR;
    if (IS_NULL_fn(a)) return b;
    if (IS_NULL_fn(b)) return a;
    if (so_is_list(a) && so_is_list(b)) {
        static int so_list_reg = 0;
        if (!so_list_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); so_list_reg = 1; }
        int64_t an = FIELD_GET_fn(a, "frame_size").i, bn = FIELD_GET_fn(b, "frame_size").i;
        DESCR_t *ae = (DESCR_t *)FIELD_GET_fn(a, "frame_elems").ptr;
        DESCR_t *be = (DESCR_t *)FIELD_GET_fn(b, "frame_elems").ptr;
        int64_t n = an + bn;
        DESCR_t *ne = (DESCR_t *)rt_ws_alloc((size_t)((n > 0 ? n : 1) * (int64_t)sizeof(DESCR_t)));
        for (int64_t i = 0; i < an; i++) ne[i] = ae ? ae[i] : NULVCL;
        for (int64_t i = 0; i < bn; i++) ne[an + i] = be ? be[i] : NULVCL;
        DESCR_t ep; ep.v = DT_DATA; ep.slen = 0; ep.ptr = (void *)ne;
        return DATCON_fn("list", ep, INTVAL(n), STRVAL("list"), INTVAL(n));
    }
    const char *asp, *bsp;
    long alc = -1; long al_auth = -1, bl_auth = -1;
    if (a.v == DT_DATA) asp = rk_obj_stringify(a, 0); else { DESCR_t as = descr_to_str(a); if (as.v == DT_S || as.v == DT_SNUL) { asp = VARVAL_fn(as); al_auth = (long)descr_slen(as); } else asp = NULL; }
    if (a.v == DT_S && asp) alc = rt_sxt_match(asp);
    if (b.v == DT_DATA) bsp = rk_obj_stringify(b, 0); else { DESCR_t bs = descr_to_str(b); if (bs.v == DT_S || bs.v == DT_SNUL) { bsp = VARVAL_fn(bs); bl_auth = (long)descr_slen(bs); } else bsp = NULL; }
    if (!asp) asp = "";
    if (!bsp) bsp = "";
    size_t bl = (bl_auth >= 0) ? (size_t)bl_auth : strlen(bsp);
    if (alc >= 0) {
        char *x = rt_sxt_extend((char *)asp, alc, (long)bl);
        if (x) { memcpy(x + alc, bsp, bl); x[(size_t)alc + bl] = '\0'; rt_sxt_note(x, alc + (long)bl); return BSTRVAL(x, alc + (long)bl); }
    }
    size_t al = (alc >= 0) ? (size_t)alc : ((al_auth >= 0) ? (size_t)al_auth : strlen(asp));
    char *buf = rt_str_alloc((long)(al + bl));
    memcpy(buf, asp, al);
    memcpy(buf + al, bsp, bl);
    buf[al + bl] = '\0';
    rt_sxt_note(buf, (long)(al + bl));
    return BSTRVAL(buf, (long)(al + bl));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Concatenation for consumers whose real->string convention carries a mandatory fraction digit ("10.0"), as against str_concat_d's SPITBOL trailing-point convention ("10.").  Canonical cater()
 * (refs/icon-master/src/runtime/ocat.r) settles both halves of the shape: cnv:string is applied to BOTH operands UNCONDITIONALLY -- so a real is converted even when the other operand is the null string,
 * and abstract{return string} means the result is a string, never the type-preserving passthrough str_concat_d owes SPITBOL (manual Ch.3 p.22: (20-17) '' is the INTEGER 3).  Only the real arms differ,
 * so this pre-coerces exactly those and delegates the rest to str_concat_d UNCHANGED -- which keeps one authority for pattern routing, list concatenation, the null-string identity on non-real operands,
 * the string extend-in-place path, and the rtx_str.S fast path, and leaves every SNOBOL4/Snocone caller of str_concat_d byte-identical BY CONSTRUCTION rather than by measurement.  Coercing before the
 * delegate is GC-safe: allocation only raises g_gc_pending (gc_heap.c), collection runs at the explicit gc point inside str_concat_d, and both converted DESCRs are live arguments to it by then. */
DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b) {
    if (IS_REAL_fn(a)) a = descr_to_str_fracdigit(a);
    if (IS_REAL_fn(b)) b = descr_to_str_fracdigit(b);
    return str_concat_d(a, b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t str_repeat_d(DESCR_t s, DESCR_t n) {
    { extern void rt_gc_point_arr(DESCR_t *, int, const char **); DESCR_t sh[2]; sh[0] = s; sh[1] = n; rt_gc_point_arr(sh, 2, (const char **)0); s = sh[0]; n = sh[1]; }
    if (IS_FAIL_fn(s) || IS_FAIL_fn(n)) return FAILDESCR;
    extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
    const char *sp;
    if (s.v == DT_DATA) sp = rk_obj_stringify(s, 0); else { DESCR_t sd = descr_to_str(s); sp = (sd.v == DT_S || sd.v == DT_SNUL) ? VARVAL_fn(sd) : NULL; }
    if (!sp) sp = "";
    long cnt = IS_INT_fn(n) ? (long)n.i : (IS_REAL_fn(n) ? (long)n.r : 0);
    size_t sl = strlen(sp);
    if (cnt < 1 || sl == 0) { char *e = rt_str_alloc(0); e[0] = '\0'; rt_sxt_note(e, 0); return STRVAL(e); }
    size_t total = sl * (size_t)cnt;
    char *buf = rt_str_alloc((long)total);
    for (long k = 0; k < cnt; k++) memcpy(buf + (size_t)k * sl, sp, sl);
    buf[total] = '\0';
    rt_sxt_note(buf, (long)total);
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Format a real in SPITBOL's "standard representation" (manual, Real->string):
 *   - the string ALWAYS carries a decimal point; trailing zeros are dropped.
 *   - decimal form when the leading significant digit sits at 10^E, -1 <= E <= 14
 *     (verified against the x64 oracle: 1.0 -> "1.", 100.0 -> "100.", 0.5 -> "0.5",
 *      1e14 -> "100000000000000.").
 *   - otherwise exponential, mantissa normalised to 0.<digits>, lowercase 'e',
 *     signed exponent E+1 (0.01 -> "0.1e-1", 1e15 -> "0.1e+16", 1e100 -> "0.1e+101").
 *   The oracle emits a lowercase 'e' and keeps up to the round-trippable digit count;
 *   the .ref corpus was generated by that oracle, so it is the authority here (the v3.7
 *   manual's uppercase-'E' example predates this build). */
const char *real_str(double r, char *buf, int bufsz) {
    if (isnan(r)) { snprintf(buf, bufsz, "%s", "nan"); return buf; }
    if (isinf(r)) { snprintf(buf, bufsz, "%s", r < 0 ? "-inf" : "inf"); return buf; }

    int neg = (r < 0.0);
    double ar = fabs(r);
    if (ar == 0.0) { snprintf(buf, bufsz, "%s", "0."); return buf; }

    /* SPITBOL renders a FIXED significant-digit count (cfp$s, sbl.min:1230 "number of significant digits to produce"), NOT the shortest round-trippable form; measured 15 against the live oracle. */
    char sci[64];
    snprintf(sci, sizeof sci, "%.14e", ar);

    /* pull out significant digits (no point) and decimal exponent E (value = d.ddd x 10^E) */
    char digits[40]; int nd = 0; int E = 0;
    const char *p = sci;
    if (*p >= '0' && *p <= '9') digits[nd++] = *p++;
    if (*p == '.') { p++; while (*p >= '0' && *p <= '9' && nd < (int)sizeof digits - 1) digits[nd++] = *p++; }
    if (*p == 'e' || *p == 'E') { p++; E = (int)strtol(p, (char **)0, 10); }
    while (nd > 1 && digits[nd - 1] == '0') nd--;   /* strip trailing zeros, keep one digit */
    digits[nd] = '\0';

    char out[80]; int o = 0;
    if (neg) out[o++] = '-';

    if (E >= -1 && E <= 14) {
        if (E >= 0) {
            int intdigits = E + 1;
            if (nd <= intdigits) {
                for (int i = 0; i < nd; i++)          out[o++] = digits[i];
                for (int i = nd; i < intdigits; i++)  out[o++] = '0';
                out[o++] = '.';
            } else {
                for (int i = 0; i < intdigits; i++)   out[o++] = digits[i];
                out[o++] = '.';
                for (int i = intdigits; i < nd; i++)  out[o++] = digits[i];
            }
        } else {                                       /* E == -1: 0.<digits> */
            out[o++] = '0'; out[o++] = '.';
            for (int i = 0; i < nd; i++) out[o++] = digits[i];
        }
    } else {                                           /* exponential: 0.<digits>e<sign><E+1> */
        out[o++] = '0'; out[o++] = '.';
        for (int i = 0; i < nd; i++) out[o++] = digits[i];
        out[o++] = 'e';
        o += snprintf(out + o, sizeof out - (size_t)o, "%+d", E + 1);
    }
    out[o] = '\0';
    snprintf(buf, bufsz, "%s", out);
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Real -> string under the SHORTEST-ROUND-TRIP, MANDATORY-FRACTION-DIGIT convention: the shortest
 * decimal that reads back as the same double, always carrying a decimal point and at least one
 * digit after it, decimal form while 1e-3 <= |x| < 1e7 and d.ddde<E> outside that band.  This is a
 * DIFFERENT representation from real_str() above, which implements SPITBOL's trailing-point
 * convention ("10.", exponential as a normalised 0.<digits>e<E+1> mantissa over a [-1,14] decimal
 * band) -- both are correct for their own consumers, so they are two functions named by the
 * convention they produce, never one function branching on a caller.
 *
 * DERIVED FROM THE ORACLE CORPUS, NOT ASSUMED.  The first attempt transcribed Arizona Icon's
 * rtos() (refs/icon-master/src/runtime/cnv.r, sprintf "%.*g" at Precision 10 from cpuconf.h:121)
 * and REGRESSED rung19_pow_toby_pow_real and rung30_builtins_misc_sqrt, because %.10g truncates
 * sqrt(2) to 1.414213562 where the corpus wants 1.4142135623730951.  These rung36 programs are
 * JCON (&version reads "Jcon Version 2.2"), so their .expected files carry Java Double.toString
 * semantics, not Arizona's.  The band and form are read straight off the corpus:
 *   2.0e13 · -1.2157665459056929e19 · 1.0275128510570371e-19   (exponential: one leading digit,
 *       mandatory fraction digit, lowercase 'e', plain signed exponent -- no '+', no zero pad)
 *   0.05 · 10.0 · 2.718281828459045 · 1.4142135623730951        (decimal, full round-trip digits)
 * ⚠ Arizona icont/iconx (RULES.md's sanctioned oracle) would print the %.10g form here; where the
 * two disagree this corpus is JCON's.  Do not "fix" this back to Precision 10 -- that is the
 * regression above, already measured. */
const char *icon_real_str(double r, char *buf, int bufsz) {
    if (isnan(r)) { snprintf(buf, (size_t)bufsz, "%s", "nan"); return buf; }
    if (isinf(r)) { snprintf(buf, (size_t)bufsz, "%s", r < 0 ? "-inf" : "inf"); return buf; }
    int neg = (r < 0.0); double ar = fabs(r);
    if (ar == 0.0) { snprintf(buf, (size_t)bufsz, "%s", neg ? "-0.0" : "0.0"); return buf; }
    char sci[64];
    for (int prec = 0; prec <= 17; prec++) { snprintf(sci, sizeof sci, "%.*e", prec, ar); if (strtod(sci, (char **)0) == ar) break; }
    char digits[40]; int nd = 0; int E = 0; const char *p = sci;
    if (*p >= '0' && *p <= '9') digits[nd++] = *p++;
    if (*p == '.') { p++; while (*p >= '0' && *p <= '9' && nd < (int)sizeof digits - 1) digits[nd++] = *p++; }
    if (*p == 'e' || *p == 'E') { p++; E = (int)strtol(p, (char **)0, 10); }
    while (nd > 1 && digits[nd - 1] == '0') nd--;
    digits[nd] = '\0';
    char out[96]; int o = 0;
    if (neg) out[o++] = '-';
    if (E >= -3 && E <= 6) {
        if (E >= 0) {
            int intdigits = E + 1;
            if (nd <= intdigits) { for (int i = 0; i < nd; i++) out[o++] = digits[i]; for (int i = nd; i < intdigits; i++) out[o++] = '0'; out[o++] = '.'; out[o++] = '0'; }
            else { for (int i = 0; i < intdigits; i++) out[o++] = digits[i]; out[o++] = '.'; for (int i = intdigits; i < nd; i++) out[o++] = digits[i]; }
        } else { out[o++] = '0'; out[o++] = '.'; for (int i = 0; i < -E - 1; i++) out[o++] = '0'; for (int i = 0; i < nd; i++) out[o++] = digits[i]; }
    } else {
        out[o++] = digits[0]; out[o++] = '.';
        if (nd > 1) { for (int i = 1; i < nd; i++) out[o++] = digits[i]; } else out[o++] = '0';
        out[o++] = 'e';
        o += snprintf(out + o, sizeof out - (size_t)o, "%d", E);
    }
    out[o] = '\0';
    snprintf(buf, (size_t)bufsz, "%s", out);
    return buf;
}
