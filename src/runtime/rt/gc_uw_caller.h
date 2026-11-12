#ifndef GC_UW_CALLER_H
#define GC_UW_CALLER_H
#include <unwind.h>
#include "runtime/rt/gc_heap.h"
typedef struct { rt_emitted_regs_t r; uint64_t callee; int hit; } rt_uw_caller_t;
__attribute__((visibility("hidden"))) _Unwind_Reason_Code rt_uw_caller_step(struct _Unwind_Context *c, void *a);
#endif
