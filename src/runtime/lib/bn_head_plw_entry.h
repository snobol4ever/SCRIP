#ifndef BN_HEAD_PLW_ENTRY_H
#define BN_HEAD_PLW_ENTRY_H
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
static inline __attribute__((always_inline)) DESCR_t *plw_entry(DESCR_t *tmp) {
    if (tmp->v == DT_N && tmp->slen == 1 && tmp->p) return (DESCR_t *)tmp->p;
    if (tmp->v == DT_N && tmp->slen == 2 && tmp->p && ((VCELL_t *)tmp->p)->cellp) return ((VCELL_t *)tmp->p)->cellp;
    return tmp;
}
#ifdef __cplusplus
}
#endif
#endif
