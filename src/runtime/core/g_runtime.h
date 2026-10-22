#ifndef G_RUNTIME_H
#define G_RUNTIME_H
#include "g_runtime_atoms.h"
#include "g_runtime_slab.h"
#include "g_runtime_pc.h"
#include "g_runtime_agg.h"
#include "g_runtime_zsm.h"
#include "g_runtime_eval.h"
#include "g_runtime_bbprof.h"
#include "g_runtime_core.h"
#include "g_runtime_rt.h"
#include "g_runtime_kw.h"
#include "g_runtime_coexpr.h"
#include "g_runtime_pas.h"
#include "g_runtime_tap.h"
#include "g_runtime_bn.h"
#include "g_runtime_gc.h"
typedef struct {
    g_atoms_t atoms;
    g_slab_t slab;
    g_pc_t pc;
    g_agg_t agg;
    g_zsm_t zsm;
    g_eval_t eval;
    g_bbprof_t bbprof;
    g_core_t core;
    g_rt_t rt;
    g_kw_t kw;
    g_coexpr_t coexpr;
    g_pas_t pas;
    g_tap_t tap;
    g_bn_t bn;
    g_gc_t gc;
} g_runtime_t;
#ifdef __cplusplus
extern "C" {
#endif
extern g_runtime_t g_runtime;
#ifdef __cplusplus
}
#endif
#endif
