#ifndef RUNTIME_LIB_BN_16_RK_H
#define RUNTIME_LIB_BN_16_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) int rk_exc_is(DESCR_t d);
__attribute__((visibility("hidden"))) void rk_exc_init(void);
__attribute__((visibility("hidden"))) void rk_exc_throw(DESCR_t ex);
__attribute__((visibility("hidden"))) int rk_failure_is(DESCR_t d);
__attribute__((visibility("hidden"))) void rk_failure_handle(DESCR_t d);
__attribute__((visibility("hidden"))) const char *rk_join_gists(const char **tx, int n, const char *sep, const char *open, const char *close);
__attribute__((visibility("hidden"))) const char *rk_raku_scalar(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_raku_str(DESCR_t v, int in_hash);
__attribute__((visibility("hidden"))) const char *rk_dat_tname(const char *nm);
__attribute__((visibility("hidden"))) const char *rk_value_type(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_hash_from_list(const DESCR_t *el, int n);
__attribute__((visibility("hidden"))) DESCR_t rk_hash_from_list(const DESCR_t *el, int n);
__attribute__((visibility("hidden"))) void rk_combos_of(rk_av_t a, int k, DESCR_t outl);
__attribute__((visibility("hidden"))) DESCR_t rk_exc_adhoc(DESCR_t payload);
__attribute__((visibility("hidden"))) void rk_exc_init(void);
__attribute__((visibility("hidden"))) DESCR_t rk_seq_plan(DESCR_t seedv, int gen_code, DESCR_t ev);
__attribute__((visibility("hidden"))) DESCR_t rk_seq_next(int kind, DESCR_t step, DESCR_t last);
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen);
__attribute__((visibility("hidden"))) int rk_io_fail(const char *cls, const char *msg);
__attribute__((visibility("hidden"))) void rk_qh_refuse(const char *fmt, const char *a, const char *b);
__attribute__((visibility("hidden"))) const char *rk_exc_message(DESCR_t ex);
__attribute__((visibility("hidden"))) int rk_failure_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) DESCR_t rk_failure_str(DESCR_t f);
__attribute__((visibility("hidden"))) void rk_method_missing(DESCR_t recv, const char *mname, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_exc_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
__attribute__((visibility("hidden"))) DESCR_t rk_bindattr_impl(DESCR_t *args);
__attribute__((visibility("hidden"))) DESCR_t rk_box_impl(DESCR_t *args);
__attribute__((visibility("hidden"))) int icn_dtax_plain(int bid);
__attribute__((visibility("hidden"))) int dtax_off(void);
#endif
