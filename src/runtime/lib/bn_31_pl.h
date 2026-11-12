#ifndef RUNTIME_LIB_BN_31_PL_H
#define RUNTIME_LIB_BN_31_PL_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) DESCR_t pl_mk_atom_dup(const char *s, size_t n);
__attribute__((visibility("hidden"))) DESCR_t pl_text_list(const char *s, int codes);
__attribute__((visibility("hidden"))) DESCR_t pl_text_dlist(const char *s, size_t len, int codes, DESCR_t tail);
__attribute__((visibility("hidden"))) DESCR_t pl_gnu_fn(const char *f, int ar, const DESCR_t *a);
__attribute__((visibility("hidden"))) DESCR_t pl_gnu_fn1(const char *f, DESCR_t a);
__attribute__((visibility("hidden"))) DESCR_t pl_gnu_dt(time_t t);
__attribute__((visibility("hidden"))) DESCR_t pl_fx_err(void);
__attribute__((visibility("hidden"))) int pl_ct_n(DESCR_t t);
__attribute__((visibility("hidden"))) DESCR_t pl_os_err_of(int e);
__attribute__((visibility("hidden"))) int pl_sk_fd(DESCR_t s);
#endif
