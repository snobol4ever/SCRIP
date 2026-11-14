#ifndef GC_HEAP_RT_ASM_HELPERS_API_H
#define GC_HEAP_RT_ASM_HELPERS_API_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/core/g_runtime.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
struct DESCR_t;
#ifdef __cplusplus
extern "C" {
#endif
long rt_gc_collect(void);
void rt_gc_point_arr(struct DESCR_t *arr, int n, const char **r0);
#ifdef __cplusplus
}
#endif
#endif
