#ifndef BN_HEAD_RT_CTOR_IC_FIND_H
#define BN_HEAD_RT_CTOR_IC_FIND_H
#include <glob.h>
#include "runtime/core/g_runtime_bn.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_rt_ctor_ic_h.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline int rt_ctor_ic_find(const char *fn, long gen) {
    unsigned h = rt_ctor_ic_h(fn);
    if (g_rt_bn.ctor_ic[h].fn == fn && g_rt_bn.ctor_ic[h].gen == gen) return (int)h;
    h ^= 1u;
    if (g_rt_bn.ctor_ic[h].fn == fn && g_rt_bn.ctor_ic[h].gen == gen) return (int)h;
    return -1;
}
#ifdef __cplusplus
}
#endif
#endif
