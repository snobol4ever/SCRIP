#ifndef GC_HEAP_GC_ALLOC_LOOKUP_API_H
#define GC_HEAP_GC_ALLOC_LOOKUP_API_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/rt/gc_heap_rt_emitted_regs.h"
#ifdef __cplusplus
extern "C" {
#endif
int rt_emitted_caller_of_descr_call(rt_emitted_regs_t *r);
#ifdef __cplusplus
}
#endif
#endif
