#ifndef RUNTIME_LIB_BN_01_SH_H
#define RUNTIME_LIB_BN_01_SH_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_dtax_ent.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) void icn_loadfunc_cstr_args(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) int icn_nxttab(int col, const int *stops, int nstops, int gap);
__attribute__((visibility("hidden"))) int icn_numeric_tail_is_blank(const char *p);
__attribute__((visibility("hidden"))) int icn_numeric_is_c_hex(const char *p);
__attribute__((visibility("hidden"))) int icn_cvt_int_ok(DESCR_t d);
__attribute__((visibility("hidden"))) int icn_argtype_raise(int code, DESCR_t val, DESCR_t *out);
__attribute__((visibility("hidden"))) int builtin_is_generator(const char *name);
int icon_radix_int(const char *s, long long *out);
DESCR_t icon_radix_big(const char *s);
__attribute__((visibility("hidden"))) int try_call_builtin_by_name_eval_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
int rt_data_is_record(DESCR_t v);
__attribute__((visibility("hidden"))) int icn_argtype_gate(int bid, DESCR_t *args, int nargs, DESCR_t *out, int strict);
__attribute__((visibility("hidden"))) int relop_radix_coerce(DESCR_t v, DESCR_t *out);
__attribute__((visibility("hidden"))) int bn_type_datatype(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) void dtx5(dtax_ent_t *e, unsigned char l, const char *nm, void *h, short x);
__attribute__((visibility("hidden"))) int bn_sno_name(DESCR_t *args, int nargs, DESCR_t *out);
void bidprof_init(void);
__attribute__((visibility("hidden"))) int sort_family(int bid, DESCR_t *args, int nargs, DESCR_t *out, int num);
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int icn_arg_is_list(DESCR_t v);
__attribute__((visibility("hidden"))) double icn_dtor(double x);
__attribute__((visibility("hidden"))) double icn_rtod(double x);
__attribute__((visibility("hidden"))) int icn_math1(double (*f)(double), int dom, DESCR_t a, DESCR_t *out);
__attribute__((visibility("hidden"))) int icn_math_log(DESCR_t *args, int nargs, DESCR_t *out);
#endif
