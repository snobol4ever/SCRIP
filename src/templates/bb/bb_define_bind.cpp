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
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rodata.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_entry_cell_data(const char * lbl, const char * init) {
    return IF(emit_entry_cell_first(lbl),
               x86("directive", std::string(".section .data"))
             + x86("directive", std::string(".align 8"))
             + x86("directive", std::string(lbl) + std::string(":"))
             + x86("directive", std::string(".quad ") + std::string(init))
             + x86("directive", std::string(".section .text"))
             + x86("directive", std::string(".intel_syntax noprefix")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_bind() {
    x86_begin();
    return x86_alpha()
         + x86("comment", "DEFINE-SITE s57: constant-folded registration AT the statement (shared chain)")
         + x86("mov", "rdi", ROQ(0))
         + x86("mov", "rsi", ROQ(1))
         + x86("mov32", "edx", (long)_.op_dq_np)
         + x86("mov32", "ecx", (long)_.op_dq_nf)
         + x86("mov32", "r8d", (long)_.op_dq_fb)
         + IF(_.lbl_t0, x86("lea", "r9", std::string("[rip + __]"), (uint64_t)(uintptr_t)_.op_dq_fn, _.op_dq_blbl))
         + IF(!_.lbl_t0, x86_load_got("r9", _.op_dq_blbl, (uint64_t)(uintptr_t)_.op_dq_fn))
         + x86("call", "rt_define_site", (uint64_t)(uintptr_t)(void *)rt_define_site)
         + x86_rt_gc_poll()
         + IF(_.op_dq_seal_alpha, x86("comment", "M4-ALPHA-SEAL: alpha$<FN> <- &<FN>_α, the m4 twin of the driver seal")
                                + x86("mov", "rdi", ROQ(0))
                                + x86("lea", "rsi", std::string("[rip + __]"), (uint64_t)0, _.op_dq_alpha_sym)
                                + x86("call", "bb_ab_seal_alpha", (uint64_t)(uintptr_t)(void *)bb_ab_seal_alpha))
         + IF(_.op_dq_seal_entry, x86("comment", "M4-ENTRY-SEAL (term 3): entry_cell$<FN> <- &LBL__<this DEFINE's entry>, so a call reads the binding in force when it runs rather than a baked winner")
                                + bb_define_entry_cell_data(_.op_dq_entry_cell_sym, _.lbl_t0)
                                + x86("lea", "rax", std::string("[rip + __]"), (uint64_t)0, _.lbl_t0)
                                + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)0, _.op_dq_entry_cell_sym)
                                + x86("mov", RDQ("rcx", 0), "rax"))
         + IF(_.op_dq_bind, x86("mov", "rdi", ROQ(0))
                          + x86("mov", "rsi", ROQ(2))
                          + x86("call", "rt_define_bind_entry", (uint64_t)(uintptr_t)(void *)rt_define_bind_entry)
                          + x86_rt_gc_poll())
         + IF(_.op_dq_site_entry, x86("mov", "rdi", ROQ(0))
                                + x86("mov", "rsi", ROQ(3))
                                + x86("call", "rt_define_site_entry", (uint64_t)(uintptr_t)(void *)rt_define_site_entry)
                                + x86_rt_gc_poll())
         + x86_pair_loop()
         + x86(".quad_ro", L(0), LS(0), _.op_dq_fname)
         + x86("label", LS(0))
         + x86(".string", _.op_dq_fname)
         + x86(".quad_ro", L(1), LS(1), (_.op_dq_csv ? _.op_dq_csv : ""))
         + x86("label", LS(1))
         + x86(".string", (_.op_dq_csv ? _.op_dq_csv : ""))
         + IF(_.op_dq_bind, x86(".quad_ro", L(2), LS(2), _.op_dq_ent)
                          + x86("label", LS(2))
                          + x86(".string", _.op_dq_ent))
         + IF(_.op_dq_site_entry, x86(".quad_ro", L(3), LS(3), _.op_entry)
                                + x86("label", LS(3))
                                + x86(".string", _.op_entry));
}
