#ifndef RUNTIME_LIB_BN_24_SH_H
#define RUNTIME_LIB_BN_24_SH_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
__attribute__((visibility("hidden"))) void rt_apply_stno_land(const long *sv);
void rt_fire_buildplan_tweak_rq(const char *cname, DESCR_t self, long *rq);
DESCR_t rt_call_arr_strict(const char *fn, DESCR_t *args, int nargs);
DESCR_t dop_nl(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) DESCR_t rt_call_arr_bl_s(const char *fn, DESCR_t *args, int nargs, int bidlen, int strict, int sn4, long *rq);
__attribute__((visibility("hidden"))) void rt_ctor_ic_note(const char *fn, void *dt);
__attribute__((visibility("hidden"))) DESCR_t rt_call_arr_bl_sn4_rq(const char *fn, DESCR_t *args, int nargs, int bidlen, long *rq);
__attribute__((visibility("hidden"))) DESCR_t rt_call_name_sn4_rq(const char *fn, DESCR_t *args, int nargs, int bidlen, long *rq);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
__attribute__((visibility("hidden"))) int bn_identical(DESCR_t a, DESCR_t b);
int rt_dat_field_of_any(const char *name);
#endif
