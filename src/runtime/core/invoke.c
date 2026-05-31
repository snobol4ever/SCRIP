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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t ARGVAL_fn(DESCR_t d)
{
    if (d.v == DT_FAIL) return FAILDESCR;
    if (d.v == DT_N) {
        if (IS_NAMEVAL(d) && *d.s)
            return NV_GET_fn(d.s);
        if (IS_NAMEPTR(d))
            return *(DESCR_t *)GET_PTR(d);
        return NULVCL;
    }
    if (d.v == DT_K && d.s)
        return NV_GET_fn(d.s);
    return d;
}
