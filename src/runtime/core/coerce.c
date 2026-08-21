#include "coerce.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../rt/gc_heap.h"
const char *real_str(double r, char *buf, int bufsz);
const char *icon_real_str(double r, char *buf, int bufsz);
int64_t     to_int(DESCR_t v);
double      to_real(DESCR_t v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t real_descr_via(double r, const char *(*fmt)(double, char *, int))
{
    char tmp[64];
    fmt(r, tmp, sizeof tmp);
    size_t len = strlen(tmp);
    char *nbuf = rt_str_alloc((long)len);
    memcpy(nbuf, tmp, len + 1);
    return STRVAL(nbuf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t descr_to_str(DESCR_t d)
{
    if (IS_INT_fn(d)) {
        char tmp[24]; char *p = tmp + 23; long long iv = (long long)d.i; unsigned long long uv = (iv < 0) ? (unsigned long long)(-(iv + 1)) + 1ULL : (unsigned long long)iv; *p = '\0';
        do { *--p = (char)('0' + (uv % 10ULL)); uv /= 10ULL; } while (uv);
        if (iv < 0) *--p = '-';
        size_t len = (size_t)(tmp + 23 - p); char *nbuf = rt_str_alloc((long)len); memcpy(nbuf, p, len + 1);
        return BSTRVAL(nbuf, len);
    }
    if (IS_REAL_fn(d)) return real_descr_via(d.r, real_str);
    if (IS_STR_fn(d) || d.v == DT_SNUL) return d;
    if (d.v == DT_N && d.slen == 0 && d.s) return STRVAL(d.s);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t descr_to_str_fracdigit(DESCR_t d)
{
    if (IS_REAL_fn(d)) return real_descr_via(d.r, icon_real_str);
    return descr_to_str(d);
}
