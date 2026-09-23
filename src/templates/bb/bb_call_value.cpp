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
extern void rt_proc_drop_frame_h(void **hslot);
extern CVSPINE_t rt_call_value_spine_prep(DESCR_t callee, DESCR_t *argv, int n);
extern CVSPINE_t rt_call_apply_spine_prep(DESCR_t callee, DESCR_t lv);
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
    uint64_t vprep_fp; { CVSPINE_t (*fp)(DESCR_t, DESCR_t *, int) = rt_call_value_spine_prep; vprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t aprep_fp; { CVSPINE_t (*fp)(DESCR_t, DESCR_t) = rt_call_apply_spine_prep; aprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t gprep_fp; { void *(*fp)(DESCR_t, DESCR_t *, int) = rt_pl_goal_spine_prep; gprep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t pasg_fp;  { DESCR_t (*fp)(DESCR_t) = rt_gen_spine_pass_γ; pasg_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t pasw_fp;  { DESCR_t (*fp)(void) = rt_gen_spine_pass_ω; pasw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t rsen_fp;  { void (*fp)(void) = rt_gen_spine_resume_enter; rsen_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s =  x86_alpha();
    for (int i = 0; i < n; i++)
        s += x86("mov", "rax", FRQ(_.op_arg_slot[i]))
           + x86("mov", FRQ(_.op_off + 16 + i * 16), "rax")
           + x86("mov", "rax", FRQ(_.op_arg_slot[i] + 8))
           + x86("mov", FRQ(_.op_off + 16 + i * 16 + 8), "rax");
    s += x86_scan_sync_out()
       + x86_anchor_enter()
         + x86("mov",   "rax", FRQ(H))
       + x86("test",  "rax", "rax")
       + x86("je",    L(22))
       + x86("lea",   "rdi", FRQ(H))
       + x86("call", "rt_proc_drop_frame_h", (uint64_t)(uintptr_t)(void *)rt_proc_drop_frame_h)
       + x86("def",   L(22))
       + IF(cv_pl_proto(),  x86("mov", FRQ(H + 8), 0L))
       + x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + (cv_is_apply()
            ? x86("mov",   "rdx", FRQ(_.op_off + 16))
            + x86("mov",   "rcx", FRQ(_.op_off + 24))
            + x86("call",  "rt_call_apply_spine_prep", aprep_fp)
            : x86("lea",   "rdx", FRQ(_.op_off + 16))
            + x86("mov32", "ecx", (long)n)
            + (cv_is_goal() ? x86("call", "rt_pl_goal_spine_prep", gprep_fp) : x86("call", "rt_call_value_spine_prep", vprep_fp)))
       + x86_rt_gc_poll()
       + x86("test",  "rax", "rax")
       + x86("je",    L(7))
       + IF(!cv_is_goal(),  x86("mov", FRQ(H), "rdx"))
       + IF(cv_pl_proto(),  x86("sub", "rsp", 8L)
                          + x86_lea_id("rcx", 11)
                          + x86("push", "rcx"))
       + bb_glue_pass_wires_blob_regs(3, 4)
       + x86("def", L(3))
       + (cv_pl_proto()
            ?  x86("mov",  FRQ(H), "rax")
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
         + x86("cmp",  "rax", 2L)
              + x86("je",   L(23))
              + x86("test", "rax", "rax")
              + x86("jne",  L(5))
              + x86("mov",  FRQ(H), 1L)
              + x86("def",  L(23))
              + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
              + x86("jmp",  L(2))
              + x86("def", L(5))
              + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
              + x86("jmp",  L(2)))
       + x86("def", L(4))
       + (cv_pl_proto()
            ?  x86("add",  "rsp", 32L)
              + x86("mov",  FRQ(H), 0L)
              + x86("mov",  FRQ(H + 8), 0L)
              + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
              + x86("jmp",  L(2))
            : bb_glue_wire_land()
              + x86("mov",  FRQ(H + 8), "rsp")
              + x86("mov",  "rax", FRQ(H))
              + x86("cmp",  "rax", 2L)
              + x86("je",   L(24))
              + x86("test", "rax", "rax")
              + x86("jne",  L(6))
              + x86("mov",  FRQ(H), 1L)
              + x86("def",  L(24))
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
       + x86_rt_gc_poll()
       + x86_gamma()
       + x86_beta()
       + (cv_pl_proto()
            ?  x86("test", "r15", "r15")
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
         + x86("cmp",  "rax", 2L)
              + x86_omega("je")
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
       + x86_rt_gc_poll()
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
