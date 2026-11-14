#ifndef BN_HEAD_RT_CTOR_IC_H_H
#define BN_HEAD_RT_CTOR_IC_H_H
#include <glob.h>
#include "runtime/core/g_runtime_bn.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline unsigned rt_ctor_ic_h(const char *fn) { return (unsigned)((((uintptr_t)fn) * 0x9E3779B97F4A7C15ull) >> 56) & (RT_CTOR_IC_N - 1); }
#ifdef __cplusplus
}
#endif
#endif
