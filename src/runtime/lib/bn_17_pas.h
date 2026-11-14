#ifndef RUNTIME_LIB_BN_17_PAS_H
#define RUNTIME_LIB_BN_17_PAS_H
#include "runtime/lib/bn_head.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) long long rk_ival(DESCR_t v);
__attribute__((visibility("hidden"))) unsigned pas_mt_u32rand(void);
__attribute__((visibility("hidden"))) DESCR_t *pas_heap_ref(DESCR_t p, const char *fn);
__attribute__((visibility("hidden"))) unsigned char pas_ch_of(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t pas_agg_copy(DESCR_t v);
__attribute__((visibility("hidden"))) void pas_fill(DESCR_t v, long long c);
__attribute__((visibility("hidden"))) void pas_fill_from(DESCR_t v, long long idx, long long bytes, long long esz, long long c);
__attribute__((visibility("hidden"))) DESCR_t pas_dyn_block(long long n);
__attribute__((visibility("hidden"))) DESCR_t pas_dynset(DESCR_t old, DESCR_t proto, int depth, DESCR_t *dims, int nd);
__attribute__((visibility("hidden"))) DESCR_t pas_chars_of(DESCR_t v, long lo);
__attribute__((visibility("hidden"))) DESCR_t pas_chars_full(DESCR_t v, long lo);
__attribute__((visibility("hidden"))) size_t pas_slen(DESCR_t v, const char *s);
__attribute__((visibility("hidden"))) DESCR_t pas_str_setch(DESCR_t v, long eidx, unsigned char ch);
__attribute__((visibility("hidden"))) int pas_bi_chain(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
void pas_file_err(const char *clause, const char *what, const char *proc);
DESCR_t pas_arr_get(DESCR_t *args, int nargs);
DESCR_t pas_arr_set(DESCR_t *args, int nargs);
DESCR_t pas_elem_ref(DESCR_t *args, int nargs);
DESCR_t pas_dispose(DESCR_t *args, int nargs);
DESCR_t pas_deref(DESCR_t *args, int nargs);
DESCR_t pas_deref_set(DESCR_t *args, int nargs);
DESCR_t pas_addr(DESCR_t *args, int nargs);
DESCR_t pas_addr_var(DESCR_t *args, int nargs);
DESCR_t pas_ptr_inc(DESCR_t *args, int nargs);
DESCR_t pas_ptr_eq(DESCR_t *args, int nargs);
DESCR_t pas_pdx(DESCR_t *args, int nargs);
DESCR_t pas_row_get(DESCR_t *args, int nargs);
DESCR_t pas_row_set(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) int pas_io_fail(int e);
__attribute__((visibility("hidden"))) const char *pas_io_message(long code);
DESCR_t pas_ptr_diff(DESCR_t *args, int nargs);
DESCR_t pas_pchar_of(DESCR_t *args, int nargs);
DESCR_t pas_pstrlen(DESCR_t *args, int nargs);
DESCR_t pas_pstr(DESCR_t *args, int nargs);
DESCR_t pas_field_set(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) int pas_stdout_fh(void);
__attribute__((visibility("hidden"))) int pas_val_scan(const char *s, int real, long long *iv, double *dv);
__attribute__((visibility("hidden"))) int pas_bi_chr(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int pas_bi_chrlit(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int pas_bi_trunc(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int pas_bi_abs(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int pas_bi_round(DESCR_t *args, int nargs, DESCR_t *out);
#endif
