#ifndef BN_HEAD_BN_PTRLEN_H
#define BN_HEAD_BN_PTRLEN_H
#include <glob.h>
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_pas.h"
#include "runtime/core/g_runtime_bn.h"
#include "runtime/rt/rtx_try.h"
#include "runtime/core/array.h"
#include "runtime/core/datatype.h"
#include "runtime/core/table.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags_dt_platom.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define BN_PTRLEN(A, P, L, F) do { DESCR_t _a = (A); if ((F) && _a.v == DT_S && _a.s && _a.slen != 0xFFFFFFFFu && _a.slen != 0u) { (P) = _a.s; (L) = (size_t)_a.slen; } else { (P) = VARVAL_fn(_a); if \
    (!(P)) (P) = ""; (L) = sv_len(_a, (P)); } } while (0)
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head_sv_len.h"
#endif
