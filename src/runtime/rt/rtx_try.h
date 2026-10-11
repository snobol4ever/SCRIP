#ifndef RTX_TRY_H
#define RTX_TRY_H
#include "descr.h"
#include "gc_heap.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t rt_assign_var(DESCR_t, DESCR_t);
DESCR_t c_rt_assign_var(DESCR_t, DESCR_t);
DESCR_t str_concat_d(DESCR_t, DESCR_t);
DESCR_t c_str_concat_d(DESCR_t, DESCR_t);
DESCR_t rt_subscript_var(DESCR_t, DESCR_t);
DESCR_t c_rt_subscript_var(DESCR_t, DESCR_t);
DESCR_t dat_field_get(const char *, DESCR_t);
DESCR_t c_dat_field_get(const char *, DESCR_t);
DESCR_t rt_size_d(uint64_t, uint64_t);
DESCR_t c_rt_size_d(uint64_t, uint64_t);
DESCR_t rt_str_coerce(DESCR_t);
DESCR_t c_rt_str_coerce(DESCR_t);
int rt_jct_relop(DESCR_t, DESCR_t, int);
int c_rt_jct_relop(DESCR_t, DESCR_t, int);
int descr_identical(DESCR_t, DESCR_t);
int c_descr_identical(DESCR_t, DESCR_t);
static inline __attribute__((always_inline)) DESCR_t rtx_try_rt_assign_var(DESCR_t a, DESCR_t b) { DESCR_t r = rt_assign_var(a, b); return r.v == RTX_NOT_HANDLED ? c_rt_assign_var(a, b) : r; }
static inline __attribute__((always_inline)) DESCR_t rtx_try_str_concat_d(DESCR_t a, DESCR_t b) {
    if (a.v == DT_S && (g_sxt_fr.off < 0 || (g_sxt_fr.off == 0 && a.s == g_sxt_fr.owner))) return c_str_concat_d(a, b);
    DESCR_t r = str_concat_d(a, b);
    return r.v == RTX_NOT_HANDLED ? c_str_concat_d(a, b) : r;
}
static inline __attribute__((always_inline)) DESCR_t rtx_try_rt_subscript_var(DESCR_t a, DESCR_t b) {
    DESCR_t r = rt_subscript_var(a, b);
    return r.v == RTX_NOT_HANDLED ? c_rt_subscript_var(a, b) : r;
}
static inline __attribute__((always_inline)) DESCR_t rtx_try_dat_field_get(const char *f, DESCR_t o) { DESCR_t r = dat_field_get(f, o); return r.v == RTX_NOT_HANDLED ? c_dat_field_get(f, o) : r; }
static inline __attribute__((always_inline)) DESCR_t rtx_try_rt_size_d(uint64_t lo, uint64_t hi) { DESCR_t r = rt_size_d(lo, hi); return r.v == RTX_NOT_HANDLED ? c_rt_size_d(lo, hi) : r; }
static inline __attribute__((always_inline)) DESCR_t rtx_try_rt_str_coerce(DESCR_t d) { DESCR_t r = rt_str_coerce(d); return r.v == RTX_NOT_HANDLED ? c_rt_str_coerce(d) : r; }
static inline __attribute__((always_inline)) int rtx_try_rt_jct_relop(DESCR_t a, DESCR_t b, int op) {
    int r = rt_jct_relop(a, b, op);
    return (unsigned char)r == RTX_NOT_HANDLED ? c_rt_jct_relop(a, b, op) : r;
}
static inline __attribute__((always_inline)) int rtx_try_descr_identical(DESCR_t a, DESCR_t b) {
    int r = descr_identical(a, b);
    return (unsigned char)r == RTX_NOT_HANDLED ? c_descr_identical(a, b) : r;
}
#ifdef __cplusplus
}
#endif
#endif
