/* DEAD CODE — moved from src/runtime/core/invoke.c (GC oracle --gc-sections 2026-06-14, sweep pass 3).
 * Dead: ARGVAL_fn, INVOKE_fn. No surviving caller (ld: zero unresolved refs to these on removal).
 * Validated: all-6-language emit byte-identical real-vs-GC + zero emitted-call references. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gc.h>
#include "core.h"
#include "sil_macros.h"
DESCR_t INVOKE_fn(const char *name, DESCR_t *args, int nargs)
{
    if (!name) return FAILDESCR;
    return APPLY_fn(name, args, nargs);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t ARGVAL_fn(DESCR_t d)
{
    if (d.v == DT_FAIL) return FAILDESCR;
    if (d.v == DT_N) {
        if (d.slen == 0 && d.s && *d.s)
            return NV_GET_fn(d.s);
        if (d.slen == 1 && d.ptr)
            return *(DESCR_t *)d.ptr;
        return NULVCL;
    }
    if (d.v == DT_K && d.s)
        return NV_GET_fn(d.s);
    return d;
}
