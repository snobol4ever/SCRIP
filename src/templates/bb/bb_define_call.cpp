#include <algorithm>
#include <string>
#include <cstdint>
#include <cstdio>
#include <vector>
extern "C" {
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
std::string bb_define_call() {
    x86_begin();
    return x86_alpha()
         + x86_bomb("IR_DEFINE role 0 (the CALL2BB slice-2 producer) retired 2026-09-26 on a plant proof: "
                    "no lowerer builds an SR-citizen DEFINE outside roles 1..5 (0 of 4693 corpus sources reached this arm or armed a handoff)");
}
