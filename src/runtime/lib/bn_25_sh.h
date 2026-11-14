#ifndef RUNTIME_LIB_BN_25_SH_H
#define RUNTIME_LIB_BN_25_SH_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_dtax_ent.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int icn_true_len(DESCR_t d, const char *materialized);
void pas_tf_write(FILE *fp, DESCR_t v);
int pas_tf_read(FILE *fp, DESCR_t *o);
__attribute__((visibility("hidden"))) int icn_bi_direct(int bidlen, DESCR_t *args, int nargs, DESCR_t *out);
DESCR_t rt_make_list(DESCR_t *args, int nargs);
int pas_main_argc(void);
const char *pas_main_arg(int k);
void rt_main_args_bind(void);
__attribute__((visibility("hidden"))) int icn_bi_string_of_int(DESCR_t av, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_trim(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_put(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_get(DESCR_t ld, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_read_fh(DESCR_t a0, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_right(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_left(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_icn_center(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) void dtx4(dtax_ent_t *e, unsigned char l, const char *nm, void *h);
__attribute__((visibility("hidden"))) int bn_uni_list_insert(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_uni_list_delete(DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_uni_list_reverse(DESCR_t *args, int nargs, DESCR_t *out);
DESCR_t rt_sno_mkexpr_of(DESCR_t a);
#endif
