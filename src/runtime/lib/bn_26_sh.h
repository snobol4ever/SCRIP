#ifndef RUNTIME_LIB_BN_26_SH_H
#define RUNTIME_LIB_BN_26_SH_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_bn_win.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
int rt_procval_same(DESCR_t a, DESCR_t b);
__attribute__((visibility("hidden"))) const char *icn_opproc_find(const char *nm, int arity);
__attribute__((visibility("hidden"))) int icn_opproc_arity(const char *s);
__attribute__((visibility("hidden"))) const char * procval_name(DESCR_t v);
__attribute__((visibility("hidden"))) const char * icn_callee_name(DESCR_t callee);
DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n);
int rt_apply_unpack(DESCR_t lv, DESCR_t *buf, int cap);
DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int bn_bal_gen(DESCR_t *args, int nargs, DESCR_t *out, int64_t *state);
__attribute__((visibility("hidden"))) int bn_str_anal(DESCR_t *args, int nargs, int si, const char **out_s, int *out_i, int *out_j);
__attribute__((visibility("hidden"))) bn_win_t bn_win(void);
__attribute__((visibility("hidden"))) int bn_icn_any(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_many(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_tab(DESCR_t *args, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_move(DESCR_t *args, DESCR_t *out);
__attribute__((visibility("hidden"))) int icn_bi_direct_scanpos(int bid, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int icn_bi_direct_scan(int bid, DESCR_t *args, int nargs, DESCR_t *out);
#endif
