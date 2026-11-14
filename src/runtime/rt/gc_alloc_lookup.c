#define _GNU_SOURCE
#include <string.h>
#include <dlfcn.h>
#include "runtime/rt/gc_uw_caller.h"
#include "runtime/rt/gc_heap_rt_emitted_regs.h"
#include "runtime/rt/gc_heap_gc_alloc_lookup_api.h"
#define GC_RET_VOID 0
#define GC_RET_DESCR 1
#define GC_RET_OTHER 2
#define GC_RET_UNKNOWN 3
#include "templates/x86/gc_allocating_table.inc"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_emitted_caller_of_descr_call(rt_emitted_regs_t *r) {
    extern int rtx_entry_is(const char *sym);
    rt_uw_caller_t u;
    Dl_info di;
    memset(&u, 0, sizeof u);
    _Unwind_Backtrace(rt_uw_caller_step, &u);
    if (!u.hit || !u.callee || !dladdr((void *)(uintptr_t)u.callee, &di) || !di.dli_sname || rtx_entry_is(di.dli_sname)) return 0;
    for (int i = 0; i < GC_ALLOC_TAB_N; i++) if (!strcmp(GC_ALLOC_TAB[i].n, di.dli_sname)) { if (GC_ALLOC_TAB[i].rc != GC_RET_DESCR) return 0; *r = u.r; return 1; }
    return 0;
}
