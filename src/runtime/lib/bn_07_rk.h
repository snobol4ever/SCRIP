#ifndef RUNTIME_LIB_BN_07_RK_H
#define RUNTIME_LIB_BN_07_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) const char *rk_rx_pat(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_code(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_code_name(DESCR_t v);
extern __attribute__((visibility("hidden"))) rk_cbh_t *g_rk_cbh_cur;
__attribute__((visibility("hidden"))) int rk_blk_nref(DESCR_t blk);
__attribute__((visibility("hidden"))) rk_cb_t rk_blk_snap(DESCR_t blk, DESCR_t *refs);
__attribute__((visibility("hidden"))) DESCR_t rk_proc_descr_rq(const char *name, int nargs, long *rq);
__attribute__((visibility("hidden"))) DESCR_t rk_call_snap(rk_cb_t c, const DESCR_t *refs, int na);
__attribute__((visibility("hidden"))) DESCR_t rk_call_block(DESCR_t blk, int na);
__attribute__((visibility("hidden"))) DESCR_t rk_av_elem(rk_av_t a, int i);
__attribute__((visibility("hidden"))) void rk_arr_append(ARBLK_t *b, DESCR_t v);
__attribute__((visibility("hidden"))) int rk_seq_pull(DESCR_t *sp);
__attribute__((visibility("hidden"))) DESCR_t rk_seq_done(DESCR_t list);
__attribute__((visibility("hidden"))) int rk_rx_collect(const RxProg *prog, const char *subj, const RkRxOpt *o, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_trans_has_rx(const DESCR_t *margs, int nmargs);
__attribute__((visibility("hidden"))) int rk_trans_rx(DESCR_t subjD, const DESCR_t *margs, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) DESCR_t rk_empty_failure(const char *action);
#endif
