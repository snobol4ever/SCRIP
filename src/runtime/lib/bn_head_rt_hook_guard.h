#ifndef BN_HEAD_RT_HOOK_GUARD_H
#define BN_HEAD_RT_HOOK_GUARD_H
#include <glob.h>
#include "runtime/core/g_runtime.h"
#include "runtime/rt/rtx_try.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define RT_HOOK_GUARD(nm) do { if (g_runtime.abi.error != 0 || g_runtime.abi.kw_errlimit != 0) return c_rt_call_bid_sn4((nm), args, nargs, -1); } while (0)
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#endif
