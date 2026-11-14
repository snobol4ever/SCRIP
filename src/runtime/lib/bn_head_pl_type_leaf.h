#ifndef BN_HEAD_PL_TYPE_LEAF_H
#define BN_HEAD_PL_TYPE_LEAF_H
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
#define PL_TYPE_LEAF(nm) DESCR_t dop_pl_##nm(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 1 && rt_pl_type_test_cell(&args[0], #nm)) ? pl_ok() : FAILDESCR; }
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#endif
