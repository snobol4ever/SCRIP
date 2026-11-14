#ifndef GC_HEAP_ETC_H
#define GC_HEAP_ETC_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/core/g_runtime.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_gc_roots_args.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
struct DESCR_t;
#ifdef __cplusplus
extern "C" {
#endif
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
char *rt_str_alloc(long n);
void rt_gc_run_roots(gc_roots_args_t *a);
void rt_gc_run_proc_roots(gc_roots_args_t *a);
void rt_gc_run_flags_roots(gc_roots_args_t *a);
void rt_gc_run_ptr_fields(const void *base, long n, long stride, long noff, const long *off);
void rt_gc_run_loc_list(const char *const *const *list, long n);
void rt_gc_visit_descr(struct DESCR_t *d);
#ifdef __cplusplus
}
#endif
#endif
