/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "coerce.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <gc/gc.h>
const char *real_str(double r, char *buf, int bufsz);
int64_t     to_int(DESCR_t v);
double      to_real(DESCR_t v);
DESCR_t descr_to_str(DESCR_t d)
{
    if (IS_INT_fn(d)) {
        char *nbuf = GC_malloc(32);
        snprintf(nbuf, 32, "%lld", (long long)d.i);
        return STRVAL(nbuf);
    }
    if (IS_REAL_fn(d)) {
        char tmp[64];
        real_str(d.r, tmp, sizeof tmp);
        size_t len = strlen(tmp);
        char *nbuf = GC_malloc(len + 1);
        memcpy(nbuf, tmp, len + 1);
        return STRVAL(nbuf);
    }
    if (IS_STR_fn(d) || d.v == DT_SNUL) return d;
    return FAILDESCR;
}
