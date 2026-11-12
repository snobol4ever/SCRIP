#ifndef RUNTIME_LIB_BN_33_PL_H
#define RUNTIME_LIB_BN_33_PL_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) DESCR_t pl_mk_cmp1(const char *f, DESCR_t a0);
__attribute__((visibility("hidden"))) int pl_sp_stream_live(int i);
__attribute__((visibility("hidden"))) int pl_sp_named(DESCR_t d);
__attribute__((visibility("hidden"))) int pl_open_leaf(DESCR_t *args, int nargs, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_gs_ts(const char *op, DESCR_t a, DESCR_t res, pl_tr_ctx_t *cx);
#endif
