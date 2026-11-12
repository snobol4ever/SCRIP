#ifndef RUNTIME_LIB_BN_39_SH_H
#define RUNTIME_LIB_BN_39_SH_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_numrel(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_lexrel(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_identdiffer(DESCR_t *args, int nargs, DESCR_t *out, int ident);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_size(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_time(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_date(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_trim(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_dupl(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_replace(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_substr(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_reverse(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_lpad(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_rpad(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_integer(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) __attribute__((noinline)) int bn_remdr(DESCR_t *args, int nargs, DESCR_t *out);
#endif
