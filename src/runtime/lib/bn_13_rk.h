#ifndef RUNTIME_LIB_BN_13_RK_H
#define RUNTIME_LIB_BN_13_RK_H
#include "runtime/lib/bn_head.h"
#include "driver/driver_globals.h"
#include "runtime/rx.h"
__attribute__((visibility("hidden"))) RxProg *rk_rx_compile(RxEnv *env, const char *gname, const char *pat, int flags);
__attribute__((visibility("hidden"))) void rk_rx_to_match(Match *gm, const RxProg *p, const RxMatch *rm);
__attribute__((visibility("hidden"))) double rk_rat_num(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_rat_dispatch(const char *meth, DESCR_t *recv, const DESCR_t *margs, int nmargs, DESCR_t *out);
#endif
