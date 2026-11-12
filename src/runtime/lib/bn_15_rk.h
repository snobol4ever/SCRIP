#ifndef RUNTIME_LIB_BN_15_RK_H
#define RUNTIME_LIB_BN_15_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) const char *rk_param_sigil(const char *pn);
__attribute__((visibility("hidden"))) DESCR_t rk_mk_metaval(const char *proto, const char *a, const char *b);
__attribute__((visibility("hidden"))) int bnd_dat_need(const char *c, int what);
__attribute__((visibility("hidden"))) DESCR_t rk_meta_list(const char *cn, char what, int local);
__attribute__((visibility("hidden"))) int pas_move_side(DESCR_t a, DESCR_t i, DESCR_t **base, long long *k, long long *len);
__attribute__((visibility("hidden"))) int rk_exc_isa(DESCR_t d, const char *target);
#endif
