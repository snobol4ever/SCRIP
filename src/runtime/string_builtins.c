#include "core.h"
#include "sil_macros.h"
#include "utf8.h"
#include "rt/gc_heap.h"
#include <string.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t DUPL_fn(DESCR_t s, DESCR_t n) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int64_t times   = to_int(n);
    if (times < 0) return FAILDESCR;
    if (times == 0 || !STRVAL_fn || !*STRVAL_fn) return STRVAL(rt_str_dup(""));
    size_t slen = strlen(STRVAL_fn);
    char *r = rt_str_alloc((long)(slen * (size_t)times));
    r[0] = '\0';
    for (int64_t i = 0; i < times; i++) memcpy(r + i * slen, STRVAL_fn, slen);
    r[slen * times] = '\0';
    return STRVAL(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t REPLACE_fn(DESCR_t s, DESCR_t from, DESCR_t to) {
    const char *sp   = IS_STR(s)    ? s.s    : VARVAL_fn(s);
    const char *fp   = IS_STR(from) ? from.s : VARVAL_fn(from);
    const char *tp   = IS_STR(to)   ? to.s   : VARVAL_fn(to);
    size_t slen_val  = descr_slen(s);
    unsigned char xlat[256];
    for (int i = 0; i < 256; i++) xlat[i] = (unsigned char)i;
    size_t flen = descr_slen(from), tlen = descr_slen(to);
    for (size_t i = 0; i < flen; i++) {
        unsigned char fc = (unsigned char)fp[i];
        unsigned char tc = (i < tlen) ? (unsigned char)tp[i] : 0;
        xlat[fc] = tc;
    }
    int binary_mode = (IS_STR(from) && from.slen) || (IS_STR(to) && to.slen)
                   || (IS_STR(s) && s.slen);
    char *r = rt_str_alloc((long)slen_val);
    size_t rlen = 0;
    for (size_t i = 0; i < slen_val; i++) {
        unsigned char c = xlat[(unsigned char)sp[i]];
        if (binary_mode || c) r[rlen++] = (char)c;
    }
    r[rlen] = '\0';
    return binary_mode ? BSTRVAL(r, rlen) : STRVAL(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t SUBSTR_fn(DESCR_t s, DESCR_t i, DESCR_t n) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int64_t start   = to_int(i);
    int64_t len_    = to_int(n);
    size_t blen     = (s.v == DT_S && s.slen != 0xFFFFFFFFu) ? (s.slen ? (size_t)s.slen : (STRVAL_fn?strlen(STRVAL_fn):0)) : (STRVAL_fn?strlen(STRVAL_fn):0);
    size_t ncpts    = utf8_strlen_n(STRVAL_fn, blen);
    if (start < 1 || (size_t)start > ncpts + 1) return FAILDESCR;
    int64_t avail = (int64_t)ncpts - start + 1;
    if (len_ < 0 || len_ > avail) return FAILDESCR;
    if (len_ == 0) len_ = avail;
    size_t boff  = utf8_char_offset(STRVAL_fn, blen, (size_t)start);
    size_t bspan = utf8_char_bytes(STRVAL_fn, blen, boff, (size_t)len_);
    char *r = rt_str_alloc((long)bspan);
    memcpy(r, STRVAL_fn + boff, bspan);
    r[bspan] = '\0';
    return BSTRVAL(r, bspan);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t TRIM_fn(DESCR_t s) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int len = (int)strlen(STRVAL_fn);
    while (len > 0 && STRVAL_fn[len-1] == ' ') len--;
    char *r = rt_str_alloc(len);
    memcpy(r, STRVAL_fn, (size_t)len);
    r[len] = '\0';
    return STRVAL(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t lpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int64_t width   = to_int(n);
    const char *p   = VARVAL_fn(pad);
    char padch      = (p && *p) ? p[0] : ' ';
    size_t slen_v   = descr_slen(s);
    if (slen_v == 0 && STRVAL_fn && STRVAL_fn[0]) slen_v = strlen(STRVAL_fn);
    int64_t slen    = (int64_t)slen_v;
    if (width <= slen) { char *d = rt_str_alloc((long)slen); if (slen) memcpy(d, STRVAL_fn, (size_t)slen); d[slen] = '\0'; return BSTRVAL(d, (uint32_t)slen); }
    int64_t npad = width - slen;
    char *r = rt_str_alloc((long)width);
    memset(r, padch, (size_t)npad);
    memcpy(r + npad, STRVAL_fn, (size_t)slen);
    r[width] = '\0';
    return BSTRVAL(r, (uint32_t)width);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int64_t width   = to_int(n);
    const char *p   = VARVAL_fn(pad);
    char padch      = (p && *p) ? p[0] : ' ';
    size_t slen_v   = descr_slen(s);
    if (slen_v == 0 && STRVAL_fn && STRVAL_fn[0]) slen_v = strlen(STRVAL_fn);
    int64_t slen    = (int64_t)slen_v;
    if (width <= slen) { char *d = rt_str_alloc((long)slen); if (slen) memcpy(d, STRVAL_fn, (size_t)slen); d[slen] = '\0'; return BSTRVAL(d, (uint32_t)slen); }
    char *r = rt_str_alloc((long)width);
    memcpy(r, STRVAL_fn, (size_t)slen);
    memset(r + slen, padch, (size_t)(width - slen));
    r[width] = '\0';
    return BSTRVAL(r, (uint32_t)width);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t REVERS_fn(DESCR_t s) {
    const char *STRVAL_fn = VARVAL_fn(s);
    int len = (s.v == DT_S && s.slen != 0xFFFFFFFFu) ? (int)(s.slen ? s.slen : (STRVAL_fn?strlen(STRVAL_fn):0)) : (int)(STRVAL_fn?strlen(STRVAL_fn):0);
    char *r = rt_str_alloc(len);
    for (int i = 0; i < len; i++) r[i] = STRVAL_fn[len - 1 - i];
    r[len] = '\0';
    return BSTRVAL(r, len);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t BCHAR_fn(DESCR_t n) {
    int64_t code = to_int(n);
    if (code < 0 || code >= 256) return FAILDESCR;
    char *buf = rt_str_alloc(1);
    buf[0] = (char)(code & 0xFF);
    buf[1] = '\0';
    return BSTRVAL(buf, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ident(DESCR_t a, DESCR_t b) {
    if (a.v != b.v) {
        int a_null = (IS_NULL(a) || (IS_STR(a) && descr_slen(a) == 0));
        int b_null = (IS_NULL(b) || (IS_STR(b) && descr_slen(b) == 0));
        if (a_null && b_null) return 1;
        return 0;
    }
    switch (a.v) {
        case DT_SNUL: return 1;
        case DT_S: {
            size_t la = descr_slen(a), lb = descr_slen(b);
            const char *sa = a.s ? a.s : "", *sb = b.s ? b.s : "";
            return la == lb && memcmp(sa, sb, la) == 0;
        }
        case DT_I:  return a.i == b.i;
        case DT_R: return a.r == b.r;
        case DT_DATA: return a.u == b.u;
        default:       return a.ptr == b.ptr;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int differ(DESCR_t a, DESCR_t b) { return !ident(a, b); }
