#ifndef BN_HEAD_PAS_VIEW_RAW_H
#define BN_HEAD_PAS_VIEW_RAW_H
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
#define PTF_BUF(k) (((DESCR_t *)g_rt_pas.pas_tf.p)[2 * (k)])
#define PTF_ST(k) (((DESCR_t *)g_rt_pas.pas_tf.p)[2 * (k) + 1])
#define PTF_HAS(k) ((int)(PTF_ST(k).i & 1))
#define PTF_MODE(k) ((int)(PTF_ST(k).i >> 1))
#define PTF_N ((int)(g_rt_pas.pas_tf.len / 2))
#define PAS_VIEW_STR 64
#define PAS_VIEW_RAW 128
#ifdef __cplusplus
}
#endif
#endif
