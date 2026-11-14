#ifndef BN_HEAD_PLW_CELL_DEREF_H
#define BN_HEAD_PLW_CELL_DEREF_H
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
#include "runtime/lib/bn_head_bn_04_pl_api.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline __attribute__((always_inline)) DESCR_t *plw_cell_deref(DESCR_t *c) {
    if (c->v != DT_N && (c->v != (DTYPE_t)DT_PLVAR || !c->p || c->p == (void *)c)) return c;
    return plw_cell_deref_slow(c);
}
#ifdef __cplusplus
}
#endif
#endif
