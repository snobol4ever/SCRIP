#ifndef RUNTIME_LIB_BN_14_RK_H
#define RUNTIME_LIB_BN_14_RK_H
#include "runtime/lib/bn_head.h"
int junction_is(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_unmark(DESCR_t d);
__attribute__((visibility("hidden"))) DESCR_t rk_hash_store(DESCR_t h, DESCR_t k, DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_arr_text(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_seq(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_seq_is_lazy(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_pair(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_range(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_list(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_mark_list(DESCR_t d);
extern __attribute__((visibility("hidden"))) const char rk_proto_methval[7];
extern __attribute__((visibility("hidden"))) const char rk_proto_attrval[10];
__attribute__((visibility("hidden"))) int rk_is_methval(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_attrval(DESCR_t v);
extern __attribute__((visibility("hidden"))) const char rk_proto_sigval[10];
extern __attribute__((visibility("hidden"))) const char rk_proto_paramval[10];
__attribute__((visibility("hidden"))) int rk_is_sigval(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_paramval(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_typeobj(const char *nm);
extern __attribute__((visibility("hidden"))) const char rk_proto_pair[5];
extern __attribute__((visibility("hidden"))) const char rk_proto_jct_any[4];
extern __attribute__((visibility("hidden"))) const char rk_proto_jct_all[4];
extern __attribute__((visibility("hidden"))) const char rk_proto_jct_one[4];
extern __attribute__((visibility("hidden"))) const char rk_proto_jct_none[5];
__attribute__((visibility("hidden"))) int rk_hy_seq(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_is_jct(DESCR_t v);
__attribute__((visibility("hidden"))) void rk_reify_deep(DESCR_t v, int depth);
__attribute__((visibility("hidden"))) const char *rk_range_text(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_paren(const char *s);
__attribute__((visibility("hidden"))) const char *rk_gist_str(DESCR_t v);
__attribute__((visibility("hidden"))) void rk_range_bounds(DESCR_t v, DESCR_t *lo, DESCR_t *hi, int *xl, int *xh);
__attribute__((visibility("hidden"))) int rk_range_num(DESCR_t v, double *d, int *isint);
__attribute__((visibility("hidden"))) DESCR_t rk_range_arr_make_fl(DESCR_t a, DESCR_t b, int fl);
__attribute__((visibility("hidden"))) DESCR_t rk_range_value(DESCR_t lo, DESCR_t hi, int fl);
__attribute__((visibility("hidden"))) int rk_range_method(const char *m, DESCR_t v, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_in_range_value(DESCR_t x, DESCR_t r);
__attribute__((visibility("hidden"))) double rk_cstr_num(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_key_tmp(DESCR_t k);
__attribute__((visibility("hidden"))) DESCR_t rk_key_own(DESCR_t k);
DESCR_t rt_make_item_agg(DESCR_t *args, int nargs);
DESCR_t rt_make_plus_agg(DESCR_t *args, int nargs, int as_list);
__attribute__((visibility("hidden"))) void rk_arr_set_impl(DESCR_t *args, DESCR_t *out);
__attribute__((visibility("hidden"))) void rk_elem_put_impl(DESCR_t *args, DESCR_t *out);
__attribute__((visibility("hidden"))) void rk_arr_at_impl(DESCR_t *args, DESCR_t *out);
extern int junction_is(DESCR_t v);
#endif
