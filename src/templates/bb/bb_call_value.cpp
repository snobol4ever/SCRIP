#include <string>
#include <stdint.h>
#include "emit.h"
#include "bb_templates.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include <string.h>
extern DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n);
extern DESCR_t rt_call_value_gen_h(DESCR_t callee, DESCR_t *argv, int n, void **hslot);
extern DESCR_t rt_call_apply_gen_h(DESCR_t callee, DESCR_t lv, void **hslot);
extern DESCR_t rt_call_value_resume_h(void **hslot);
extern void *rt_call_value_spine_prep(DESCR_t callee, DESCR_t *argv, int n);
extern void *rt_call_apply_spine_prep(DESCR_t callee, DESCR_t lv);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool cv_is_apply() { return _.op_sval && strcmp(_.op_sval, "apply") == 0 && _.op_arg_slot_n == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_value() {
    x86_begin();
    if (_.op_off < 0 || _.op_sa < 0) return x86_alpha() + x86_bomb("bb_call_value: needs own slot + callee operand slot");
    int n = _.op_arg_slot_n;
    for (int i = 0; i < n; i++)
        if (_.op_arg_slot[i] < 0)
            return x86_alpha() + x86_bomb("bb_call_value: argument slot unfilled");
    int H = _.op_off + 16 + n * 16;
    uint64_t vprep_fp; { void *(*fp)(DESCR_t, DESCR_t *, int) = rt_call_value_spine_prep; vprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t aprep_fp; { void *(*fp)(DESCR_t, DESCR_t) = rt_call_apply_spine_prep; aprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t pasg_fp;  { DESCR_t (*fp)(DESCR_t) = rt_gen_spine_pass_γ; pasg_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t pasw_fp;  { DESCR_t (*fp)(void) = rt_gen_spine_pass_ω; pasw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t rsen_fp;  { void (*fp)(void) = rt_gen_spine_resume_enter; rsen_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s = x86("comment", "IR_CALL_VALUE invoke through a runtime callee value (proc/string/int); a jmp-entry GENERATOR callee takes the SPINE transfer (bcps_spine_gen_arm contract: prep resolves+stages+opens, wires in rcx/rdx, once-flag in the granted handle cell [zeta+off+16+n*16], beta resumes jmp [rsp]); det/builtin/unresolved callees fall back to the one-shot C window whose handle parks in the same cell (values 0/ptr, disjoint from the spine flag 1)")
                  + x86_alpha();
    for (int i = 0; i < n; i++)
        s += x86("mov", "rax", FRQ(_.op_arg_slot[i]))
           + x86("mov", FRQ(_.op_off + 16 + i * 16), "rax")
           + x86("mov", "rax", FRQ(_.op_arg_slot[i] + 8))
           + x86("mov", FRQ(_.op_off + 16 + i * 16 + 8), "rax");
    bool n2_align = icn_gen_regime() && g_emit.flat_gen;   /* row icon-n2-apply-nested-coexpr (seat06 2026-08-29): an N2 region-resident generator body rests at the OLD carve's 8-mod-16 parity (emit.cpp:2903), never call-ready 0-mod-16 -- ζ is RBP-relative in this regime (x86_zop), so RSP is free to move and a bare call from here misaligns the callee 8 bytes into everything below it. Latent until the callee's own call chain needs real 16B alignment: gdb-measured, rt_call_apply_gen_h entered at rsp mod16=0 from inside a generator body vs the correct 8 from a plain procedure, SIGSEGV in glibc's malloc-arena bootstrap movaps three frames deeper (FINDING-2026-08-29-seat13-icon-n2-apply-nested-coexpr-crash-is-stack-misalignment-in-malloc-not-pthread-create.md). One throwaway word restores ordinary call-ready parity for each bare call below, mirroring bb_call_proc_staged.cpp's PL-CALL-ALIGN precedent for the identical defect class. Scoped to exactly these two call sites, NOT the whole anchor_enter/leave bracket: the spine-success path in between (L3/L4) snapshots the live RSP into FRQ(H+8) and later loads it straight back into the register (line ~112) -- padding across that store/reload would bake an 8-byte error into a value that is used as-is, not just as a call boundary. ⛔ MUST be gated on g_emit.flat_gen too, not bare icn_gen_regime(): that predicate is a whole-PROGRAM flag ("does this compilation use N2 anywhere"), true even for a plain procedure's own apply-call to a generator (e.g. suspend_apply's main->gen), which already sits at the correct 0-mod-16 baseline and regressed CRASH 5/5 under a first attempt that padded unconditionally on icn_gen_regime() alone. g_emit.flat_gen (emit.cpp:3598) is the per-procedure flag: true only when the ENCLOSING procedure being compiled is itself a suspend-capable generator, which is what actually determines the resting parity at this box's entry. */
    s += x86_scan_sync_out()
       + x86_anchor_enter()
       + x86("mov",   FRQ(H), 0L)
       + x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + IF(n2_align, x86("sub", "rsp", 8L))
       + (cv_is_apply()
            ? x86("mov",   "rdx", FRQ(_.op_off + 16))
            + x86("mov",   "rcx", FRQ(_.op_off + 24))
            + x86("call",  "rt_call_apply_spine_prep", aprep_fp)
            : x86("lea",   "rdx", FRQ(_.op_off + 16))
            + x86("mov32", "ecx", (long)n)
            + x86("call",  "rt_call_value_spine_prep", vprep_fp))
       + IF(n2_align, x86("add", "rsp", 8L))
       + x86("test",  "rax", "rax")
       + x86("je",    L(7))
       + bb_glue_pass_wires_blob(3, 4)
       + x86("def", L(3))
       + bb_glue_wire_land()
       + x86("mov",  FRQ(H + 8), "rsp")
       + x86("mov",  "rax", FRQ(H))
       + x86("test", "rax", "rax")
       + x86("jne",  L(5))
       + x86("mov",  FRQ(H), 1L)
       + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
       + x86("jmp",  L(2))
       + x86("def", L(5))
       + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
       + x86("jmp",  L(2))
       + x86("def", L(4))
       + bb_glue_wire_land()
       + x86("mov",  FRQ(H + 8), "rsp")
       + x86("mov",  "rax", FRQ(H))
       + x86("test", "rax", "rax")
       + x86("jne",  L(6))
       + x86("mov",  FRQ(H), 1L)
       + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
       + x86("jmp",  L(2))
       + x86("def", L(6))
       + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
       + x86("jmp",  L(2))
       + x86("def", L(7))
       + x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + IF(n2_align, x86("sub", "rsp", 8L))
       + (cv_is_apply()
            ? x86("mov",   "rdx", FRQ(_.op_off + 16))
            + x86("mov",   "rcx", FRQ(_.op_off + 24))
            + x86("lea",   "r8",  FRQ(H))
            + x86("call",  "rt_call_apply_gen_h", (uint64_t)(uintptr_t)(void *)rt_call_apply_gen_h)
            : x86("lea",   "rdx", FRQ(_.op_off + 16))
            + x86("mov32", "ecx", (long)n)
            + x86("lea",   "r8",  FRQ(H))
            + x86("call",  "rt_call_value_gen_h", (uint64_t)(uintptr_t)(void *)rt_call_value_gen_h))
       + IF(n2_align, x86("add", "rsp", 8L))
       + x86("def", L(2))
       + x86_anchor_leave()
       + x86_scan_sync_in_rr()
       + x86("mov",   FRQ(_.op_off),     "rax")
       + x86("mov",   FRQ(_.op_off + 8), "rdx")
       + x86("cmp",   "al", (long)DT_FAIL)
       + x86_omega("je")
       + x86_gamma()
       + x86_beta()
       + x86("mov",  "rax", FRQ(H))
       + x86("cmp",  "rax", 1L)
       + x86("jne",  L(8))
       + x86_scan_sync_out()
       + x86("call", "rt_gen_spine_resume_enter", rsen_fp)
       + x86("mov",  "rsp", FRQ(H + 8))
       + x86_jmp_mem("rsp", 0)
       + x86("def", L(8))
       + x86_scan_sync_out()
       + x86("lea",   "rdi", FRQ(H))
       + x86("call",  "rt_call_value_resume_h", (uint64_t)(uintptr_t)(void *)rt_call_value_resume_h)
       + x86_scan_sync_in_rr()
       + x86("cmp",   "al", (long)DT_FAIL)
       + x86_omega("je")
       + x86("mov",   FRQ(_.op_off),     "rax")
       + x86("mov",   FRQ(_.op_off + 8), "rdx")
       + x86_gamma()
       + x86_omega();
    return s;
}
