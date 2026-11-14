#include <algorithm>
#include <string>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "emitter/emit_str.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "bb_templates.h"
#include "runtime/rt/rt.h"
#include "runtime/core/trace.h"
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
void rt_kw_set_rtntype_role(int);
void rt_define_bind_entry(const char *fname, const char *entry);
extern int * const rt_k_level_p;
void sno_trace_call(const char *fname);
void sno_trace_return(const char *fname, DESCR_t retval);
void rt_trace_call_hook_p(const char *fname, trace_pend_t *pend);
void rt_trace_return_hook_p(const char *fname, DESCR_t retval, trace_pend_t *pend, long long rstno);
void rt_trace_fail_hook_p(const char *fname, trace_pend_t *pend, long long rstno);
void rt_define_site(const char *, const char *, int, int, int, void *);
void rt_define_site_entry(const char *, const char *);
void bb_ab_seal_alpha(const char * pname, void * alpha);
void rt_shim_nv_in(const char *, DESCR_t *, const long *, const char *); DESCR_t rt_shim_nv_gamma(const char *, DESCR_t *); void rt_shim_nv_omega(const char *, DESCR_t *);
}
#include "templates/x86/x86_ops.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_return() {
    x86_begin();
    return x86("comment", "IR_DEFINE RETURN floater (s64 RSP-ONLY: pop {gamma,omega} pair at TOS — depth IS the anchor)")
         + x86_alpha()
         + x86("mov", "edi", (long)1)
         + x86("call", "rt_kw_set_rtntype_role", (uint64_t)(uintptr_t)(void *)rt_kw_set_rtntype_role)
         + IF(_.op_frag_release, x86("add", "rsp", (long)_.flat_frame_bytes))
         + x86("pop", "rcx")
         + x86("add", "rsp", (long)8)
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_freturn() {
    x86_begin();
    return x86("comment", "IR_DEFINE FRETURN floater (s64 RSP-ONLY: skip gamma, pop omega — depth IS the anchor)")
         + x86_alpha()
         + x86("mov", "edi", (long)2)
         + x86("call", "rt_kw_set_rtntype_role", (uint64_t)(uintptr_t)(void *)rt_kw_set_rtntype_role)
         + IF(_.op_frag_release, x86("add", "rsp", (long)_.flat_frame_bytes))
         + x86("add", "rsp", (long)8)
         + x86("pop", "rcx")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_nreturn_mark() {
    return x86_alpha()
         + x86("comment", "NRETURN floater: by-name mark (manual p.133); rt_nret_fix in the call epilogue reads+clears it; depth-agnostic — zero [rsp+K], zero calls; glue continues at RETURN")
         + x86("push", "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)1)
         + x86("pop", "rax")
         + x86_gamma();
}
