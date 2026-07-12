#include "rt/rt_arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gc.h>
#include "core.h"
#include "sil_macros.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t VARVAL_d_fn(DESCR_t d)
{
    if (d.v == DT_FAIL) return FAILDESCR;
    if (d.v == DT_N) {
        if (d.slen == 0 && d.s && *d.s) d = NV_GET_fn(d.s);
        else if (d.slen == 1 && d.ptr)  d = *(DESCR_t *)d.ptr;
        else if (d.slen == 2 && d.ptr)  { extern DESCR_t rt_deref(DESCR_t); d = rt_deref(d); }
        else return NULVCL;
    }
    if (d.v == DT_K && d.s) d = NV_GET_fn(d.s);
    if (d.v == DT_FAIL) return FAILDESCR;
    if (d.v == DT_S || d.v == DT_SNUL) return d;
    if (d.v == DT_I) {
        char buf[64];
        snprintf(buf, sizeof buf, "%lld", (long long)d.i);
        return STRVAL(rt_ws_strdup(buf));
    }
    if (d.v == DT_R) {
        char buf[64];
        snprintf(buf, sizeof buf, "%g", d.r);
        return STRVAL(rt_ws_strdup(buf));
    }
    return FAILDESCR;
}
