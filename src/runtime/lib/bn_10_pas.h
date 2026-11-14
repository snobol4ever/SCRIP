#ifndef RUNTIME_LIB_BN_10_PAS_H
#define RUNTIME_LIB_BN_10_PAS_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
void rt_pl_iso_throw_existence_key(const char *key);
__attribute__((visibility("hidden"))) DESCR_t elem_to_descr(const char *s, size_t slen);
__attribute__((visibility("hidden"))) int jct_one_cmp_num(double a, double b, int op);
__attribute__((visibility("hidden"))) int jct_one_cmp_str(const char *a, const char *b, int op);
__attribute__((visibility("hidden"))) void pas_tf_set(int idx, DESCR_t buf, int has, int mode);
__attribute__((visibility("hidden"))) DESCR_t *pas_heap_cell(long n, int grow);
__attribute__((visibility("hidden"))) DESCR_t pas_read_number(FILE *f, int real, int fpc, const char *fn);
__attribute__((visibility("hidden"))) int pas_tag_ne(long long a, long long b);
__attribute__((visibility("hidden"))) long pas_heap_take(void);
__attribute__((visibility("hidden"))) void pas_heap_give(long n);
DESCR_t pas_rdecode(DESCR_t reg, long off, long kind, long rsz);
DESCR_t pas_rpatch(DESCR_t reg, long off, long kind, long rsz, DESCR_t val);
DESCR_t pas_view_make(DESCR_t *args, int nargs);
DESCR_t pas_alloc_rec(DESCR_t *args, int nargs);
#endif
