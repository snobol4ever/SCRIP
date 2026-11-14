#ifndef RUNTIME_LIB_BN_09_RK_H
#define RUNTIME_LIB_BN_09_RK_H
#include "runtime/lib/bn_head_rk_av.h"
#include "runtime/lib/bn_head_pl_vtab.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int rk_order_both_numeric(DESCR_t a, DESCR_t b);
__attribute__((visibility("hidden"))) long long rk_order_cmp_num(DESCR_t a, DESCR_t b);
__attribute__((visibility("hidden"))) rk_av_t rk_av_args(DESCR_t *args, int from, int to);
extern __attribute__((visibility("hidden"))) const char rk_proto_slip[5];
extern __attribute__((visibility("hidden"))) const char rk_proto_tlgot[6];
extern __attribute__((visibility("hidden"))) const char rk_proto_list[5];
__attribute__((visibility("hidden"))) int rk_text_int(const char *t, long long *v);
__attribute__((visibility("hidden"))) DESCR_t rk_av_minmax(rk_av_t a, int want_max);
__attribute__((visibility("hidden"))) long long rk_int_arg(DESCR_t v, long long d);
__attribute__((visibility("hidden"))) int rk_int_or_rat(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_list_more_methods(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) void pl_vt_init(pl_vtab_t *vt, int occ);
__attribute__((visibility("hidden"))) DESCR_t rk_hash_copy(DESCR_t h);
#endif
