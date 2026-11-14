#ifndef BN_HEAD_PL_OUT_LEAF_H
#define BN_HEAD_PL_OUT_LEAF_H
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
#define PL_OUT_LEAF(nm, body1) DESCR_t dop_pl_##nm##_s(DESCR_t *args, int nargs) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx(args[0], 1); if (nargs \
    < 1 || idx < 0) return FAILDESCR; { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = body1(args + 1, nargs - 1); fh_set_output(sv); return r; } }
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#endif
