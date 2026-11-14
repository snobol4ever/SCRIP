#ifndef RUNTIME_LIB_BN_37_PAS_H
#define RUNTIME_LIB_BN_37_PAS_H
#include "runtime/lib/bn_head_pas_wsink.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) void pas_real_str_exp(double r, char *buf, int bufsz, int prec, int expdig, int upper);
DESCR_t c_rt_pas_sqr(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) void pas_fmt_fixed_double(double v, int min_width, int frac, char *o);
__attribute__((visibility("hidden"))) size_t pas_format(const char *fmt, const DESCR_t *a, int na, char *out, size_t cap);
__attribute__((visibility("hidden"))) void pas_write_items(pas_wsink_t *k, DESCR_t *args, int nargs, int start);
__attribute__((visibility("hidden"))) DESCR_t pas_writestr(DESCR_t *args, int nargs);
#endif
