#ifndef BN_HEAD_PL_CX_LEAF_HEAD_H
#define BN_HEAD_PL_CX_LEAF_HEAD_H
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
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
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
#define PL_SP_NPROP 15
#define PL_CX_LEAF_HEAD(nm, ar) DESCR_t rt_pl_dop_##nm##_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {int ok; if (nargs != ar) return FAILDESCR;pl_atoms_ready();rt_pl_tr_gc_sync(cx->tr);
#define PL_CX_LEAF_TAIL rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
#define pl_eng_unify_int(a, v, cx) plw_unify_vals((a), INTVAL((int64_t)(v)), (cx))
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#endif
