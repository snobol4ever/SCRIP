#ifndef BN_HEAD_PL_VTAB_H
#define BN_HEAD_PL_VTAB_H
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
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/lib/bn_head.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    const char **nm;
    DESCR_t *v;
    int *cnt;
    int *hs;
    int n;
    int cap;
    unsigned hmask;
    const char *err;
    const tree_t *tr;
    const char *src;
    int shift;
    int base;
    const int *cm;
    int ncm;
} pl_vtab_t;
#ifdef __cplusplus
}
#endif
#endif
