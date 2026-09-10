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
extern void *rt_pl_goal_spine_prep(DESCR_t goal, DESCR_t *argv, int n);
extern DESCR_t rt_pl_goal_gen_h(DESCR_t goal, DESCR_t *argv, int n, void **hslot);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool cv_is_apply() { return _.op_sval && strcmp(_.op_sval, "apply") == 0 && _.op_arg_slot_n == 1; }
static bool cv_is_goal() { return _.op_sval && strcmp(_.op_sval, "goal") == 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool cv_pl_proto() { return cv_is_goal() && x86_fb_pinned(); }
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
    uint64_t gprep_fp; { void *(*fp)(DESCR_t, DESCR_t *, int) = rt_pl_goal_spine_prep; gprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
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
    s += x86_scan_sync_out()
       + x86_anchor_enter()
       + x86("mov",   FRQ(H), 0L)
       + IF(cv_pl_proto(), x86("comment", "PL META-CALL: zero the resume-address word too, because it is this site's ONLY discriminator between its two runtime arms. A goal resolved at run time reaches either the PL Byrd box (prep resolved a jmp-entry generator: FRQ(H)=callee frame base, FRQ(H+8)=its graph beta, both non-zero) or the one-shot C window (prep returned 0: FRQ(H)=an opaque handle, FRQ(H+8) never written). The old spine flag 1 cannot separate them -- a retained PL frame base is an arbitrary pointer, so `cmp rax,1` would read it as a C-window handle and resume the wrong protocol. A zeroed H+8 is the fact that distinguishes them and it must be established at alpha, not inferred at beta.")
                          + x86("mov", FRQ(H + 8), 0L))
       + x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + (cv_is_apply()
            ? x86("mov",   "rdx", FRQ(_.op_off + 16))
            + x86("mov",   "rcx", FRQ(_.op_off + 24))
            + x86("call",  "rt_call_apply_spine_prep", aprep_fp)
            : x86("lea",   "rdx", FRQ(_.op_off + 16))
            + x86("mov32", "ecx", (long)n)
            + (cv_is_goal() ? x86("call", "rt_pl_goal_spine_prep", gprep_fp) : x86("call", "rt_call_value_spine_prep", vprep_fp)))
       + x86("test",  "rax", "rax")
       + x86("je",    L(7))
       + IF(cv_pl_proto(), x86("comment", "PL-CALL-ALIGN + jump-back word, byte-identical to bb_call_proc_staged's own PL call site: the callee is a PL Byrd box and it reads a FOUR-word entry stack ([rsp+0]=gamma [rsp+8]=omega [rsp+16]=jump-back [rsp+24]=pad), not the two-word pair an Icon flat generator takes. Pushing the pair alone left the box reading its jump-back out of the caller's locals AND left rsp 8-mod-16 into the callee, which is a real ABI violation before it is a protocol one. Emitting the same four words is what lets the landings below pop the same 32.")
                          + x86("sub", "rsp", 8L)
                          + x86_lea_id("rcx", 11)
                          + x86("push", "rcx"))
       + bb_glue_pass_wires_blob_regs(3, 4)
       + x86("def", L(3))
       + (cv_pl_proto()
            ? x86("comment", "PL gamma-LANDING (ARCH sec B.3 rung 2), the same contract bb_call_proc_staged banks: the callee hands rax = its own frame base when it RETAINED a live choice and 0 when it released, and rdx = its graph beta. Bank both. Pop the four entry words ONLY on the released arm -- a retained callee frame is carved BELOW them, so moving rsp back over it would let this caller carve straight over a live choice point on its very next call. This is the half of the box the bomb stood in for: the ENTRY was always shareable with the Icon spine and the RE-DRIVE never was.")
              + x86("mov",  FRQ(H), "rax")
              + x86("mov",  FRQ(H + 8), "rdx")
              + x86("test", "rax", "rax")
              + x86("jne",  L(21))
              + x86("add",  "rsp", 32L)
              + x86("def",  L(21))
              + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
              + x86("jmp",  L(2))
            : bb_glue_wire_land()
              + x86("mov",  FRQ(H + 8), "rsp")
              + x86("mov",  "rax", FRQ(H))
              + x86("test", "rax", "rax")
              + x86("jne",  L(5))
              + x86("mov",  FRQ(H), 1L)
              + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
              + x86("jmp",  L(2))
              + x86("def", L(5))
              + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
              + x86("jmp",  L(2)))
       + x86("def", L(4))
       + (cv_pl_proto()
            ? x86("comment", "PL omega-LANDING (ARCH sec B.3 rung 2): the callee always releases on concede, so rsp is back at the four entry words -- pop them, clear BOTH the re-entry token and the resume address so a later beta cannot re-enter a dead frame nor mistake it for a live C-window handle, and fail forward.")
              + x86("add",  "rsp", 32L)
              + x86("mov",  FRQ(H), 0L)
              + x86("mov",  FRQ(H + 8), 0L)
              + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
              + x86("jmp",  L(2))
            : bb_glue_wire_land()
              + x86("mov",  FRQ(H + 8), "rsp")
              + x86("mov",  "rax", FRQ(H))
              + x86("test", "rax", "rax")
              + x86("jne",  L(6))
              + x86("mov",  FRQ(H), 1L)
              + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
              + x86("jmp",  L(2))
              + x86("def", L(6))
              + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
              + x86("jmp",  L(2)))
       + x86("def", L(7))
       + x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + (cv_is_apply()
            ? x86("mov",   "rdx", FRQ(_.op_off + 16))
            + x86("mov",   "rcx", FRQ(_.op_off + 24))
            + x86("lea",   "r8",  FRQ(H))
            + x86("call",  "rt_call_apply_gen_h", (uint64_t)(uintptr_t)(void *)rt_call_apply_gen_h)
            : x86("lea",   "rdx", FRQ(_.op_off + 16))
            + x86("mov32", "ecx", (long)n)
            + x86("lea",   "r8",  FRQ(H))
            + (cv_is_goal() ? x86("call", "rt_pl_goal_gen_h", (uint64_t)(uintptr_t)(void *)rt_pl_goal_gen_h) : x86("call", "rt_call_value_gen_h", (uint64_t)(uintptr_t)(void *)rt_call_value_gen_h)))
       + x86("def", L(2))
       + x86_anchor_leave()
       + x86_scan_sync_in_rr()
       + x86("mov",   FRQ(_.op_off),     "rax")
       + x86("mov",   FRQ(_.op_off + 8), "rdx")
       + x86("cmp",   "al", (long)DT_FAIL)
       + x86_omega("je")
       + x86_gamma()
       + x86_beta()
       + (cv_pl_proto()
            ? x86("comment", "PL beta -- THE HALF THIS BOX USED TO BOMB ON. A ball in flight takes omega first: SCRIP wires box N+1 omega to box N beta, so without this test a throw would land here and be run as a redo. Then re-enter only a genuinely retained PL callee: FRQ(H) is its frame base and FRQ(H+8) its graph beta. A ZERO H+8 with a non-zero H is the OTHER arm -- prep refused this goal and the one-shot C window owns the handle -- so it falls to the resume_h path below rather than jumping a resume address that was never banked. rsp is deliberately NOT moved: it still sits below the retained frame, exactly where the retaining gamma left it, which is the whole reason the Icon spine's `mov rsp,banked; jmp [rsp]` wrecked this callee.")
              + x86("test", "r15", "r15")
              + x86("jne",  L(12))
              + x86("mov",  "rax", FRQ(H))
              + x86("test", "rax", "rax")
              + x86("je",   L(12))
              + x86("mov",  "rcx", FRQ(H + 8))
              + x86("test", "rcx", "rcx")
              + x86("je",   L(8))
              + x86_scan_sync_out()
              + x86("mov",  x86_fb(), "rax")
              + x86("call", "rt_gen_spine_resume_enter", rsen_fp)
              + x86_jmp_reg("rcx")
              + x86("def",  L(12))
              + x86_omega()
            : x86("mov",  "rax", FRQ(H))
              + x86("cmp",  "rax", 1L)
              + x86("jne",  L(8))
              + x86_scan_sync_out()
              + x86("call", "rt_gen_spine_resume_enter", rsen_fp)
              + x86("mov",  "rsp", FRQ(H + 8))
              + x86_jmp_mem("rsp", 0))
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
       + IF(cv_pl_proto(), x86("comment", "PL det jump-back (L11): the callee reached no choice point and returned rather than taking a wire, popping the pair and this word itself; only its 16-byte alignment pad is left to drop.")
                          + x86("def", L(11))
                          + x86("add", "rsp", 8L)
                          + x86_anchor_leave()
                          + x86_scan_sync_in_rr()
                          + x86("mov", FRQ(_.op_off),     "rax")
                          + x86("mov", FRQ(_.op_off + 8), "rdx")
                          + x86("cmp", "al", (long)DT_FAIL)
                          + x86_omega("je")
                          + x86_gamma())
       + x86_omega();
    return s;
}
