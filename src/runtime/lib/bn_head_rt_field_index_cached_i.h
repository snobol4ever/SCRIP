#ifndef BN_HEAD_RT_FIELD_INDEX_CACHED_I_H
#define BN_HEAD_RT_FIELD_INDEX_CACHED_I_H
#include <glob.h>
#include "runtime/core/g_runtime_bn.h"
#include "runtime/core/datatype.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline int rt_field_index_cached_i(const char *fn, const DATBLK_t *idb) {
    unsigned h = (unsigned)(((((uintptr_t)fn ^ ((uintptr_t)idb << 1)) * 0x9E3779B97F4A7C15ull) >> 56) & (RT_FIELD_IC_N - 1));
    if (g_rt_bn.field_ic[h].fn == fn && g_rt_bn.field_ic[h].type == idb) { int fi = g_rt_bn.field_ic[h].idx; if (fi < idb->nfields && idb->fields[fi] && !strcmp(idb->fields[fi], fn)) return fi; }
    {
        const char _f0 = fn[0];
        for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) {
            g_rt_bn.field_ic[h].fn = fn;
            g_rt_bn.field_ic[h].type = idb;
            g_rt_bn.field_ic[h].idx = fi;
            return fi;
        }
    }
    return -1;
}
#ifdef __cplusplus
}
#endif
#endif
