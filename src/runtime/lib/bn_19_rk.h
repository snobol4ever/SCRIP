#ifndef RUNTIME_LIB_BN_19_RK_H
#define RUNTIME_LIB_BN_19_RK_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int rk_qh_is(DESCR_t d);
__attribute__((visibility("hidden"))) int rk_qh_n(DESCR_t q);
__attribute__((visibility("hidden"))) const char *rk_qh_render(DESCR_t q, int gist);
__attribute__((visibility("hidden"))) int rk_qh_hook(const char *m, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_qh_fn(const char *fn, DESCR_t *a, int n, DESCR_t *out);
__attribute__((visibility("hidden"))) DESCR_t rk_qh_get(DESCR_t q, DESCR_t k);
__attribute__((visibility("hidden"))) DESCR_t rk_qh_put(DESCR_t q, DESCR_t k, DESCR_t v);
__attribute__((visibility("hidden"))) int rk_qh_del(DESCR_t q, DESCR_t k, DESCR_t *old);
__attribute__((visibility("hidden"))) int rk_num_dbl(DESCR_t v, double *d);
__attribute__((visibility("hidden"))) DESCR_t rk_range_numeric(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_mk_jct(const char *flavor, DESCR_t *args, int nargs);
#endif
