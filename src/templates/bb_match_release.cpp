#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
extern "C" long rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern "C" long rt_dcap_step(DESCR_t fret);
extern "C" uint64_t g_patstk_sp;
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
#include "x86_asm.h"
#define rfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + (x86_zc_frame() == ZC_FRAME_RSP ? x86("mov", "rdi", RSP((int)(_.op_off + 32 + 32)))
                                     : x86("mov",  "rdi", FRQ(_.op_off + 32)))
         + x86("mov",  "rsi", ABSQ(RT_CAS_TOP))   /* R12-FREE-1: pass the CELL top */
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + IF(x86_zc_frame() != ZC_FRAME_RSP, x86("push", x86_zr())
                                      + x86("sub",  "rsp", 8L))
         + x86_lea_id("rcx", 3)
         + x86_lea_id("rdx", 4)
         + IF(x86_zc_frame() != ZC_FRAME_RSP, x86("mov",  x86_zr(), "rsp"))
         + x86_jmp_reg("rax")
         + x86("def",  L(3))
         + IF(x86_zc_frame() != ZC_FRAME_RSP, x86("mov",  "rax", "rsp")
             + x86("mov",  "rax", RDQ("rax", 8))
             + x86("mov",  "rdi", RDQ("rax", 0))
             + x86("mov",  "rsi", RDQ("rax", 8))
             + x86("mov",  "rsp", x86_zr())
             + x86("add",  "rsp", 8L)
             + x86("pop",  x86_zr()))
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(4))
         + IF(x86_zc_frame() != ZC_FRAME_RSP, x86("mov",  "rsp", x86_zr())
             + x86("add",  "rsp", 8L)
             + x86("pop",  x86_zr()))
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("mov", "rax", FRQ(_.op_off + 32)) + x86("mov", ABSQ(RT_CAS_TOP), "rax")   /* R12-FREE-1: one-mov unwind now restores the CELL */
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("mov", "rbp", FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): paired with head's gated +40 save */
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_release() {
    x86_begin();
    return !PLATFORM_X86 ? std::string()
         : _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_RELEASE: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail && rfc()
         ? x86("comment", "IR_MATCH_RELEASE (R12-EXIT-1 tail: bracket read off the TOP ELEMENT, then the one-mov unwind collapses every element, every suspended cell, and HEAD's cell together; Z4-6 residual: the one-mov unwind is FORTH-cell physics, so a non-FORTH port takes the general FRQ bracket arm below -- the CSTACK reader was reading fc cells nothing carved, rsp=1 at the arbno probe)")
         + x86_alpha()
         + x86("mov", "rax", RSP((int)_.op_fc_disp + 0))
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rsp", RSP((int)_.op_fc_disp + 8))
         + release_pump()
         : x86("comment", "IR_MATCH_RELEASE")
         + x86_alpha()
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (rfc() ? x86("mov", "rax", RSP((int)_.op_fc_disp + 8))
                                               : x86("mov", "rax", FRQ(_.op_off + 8)))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax"))
         + (_.op_dval != 0.0
                ? IF(rfc(), x86("mov", "eax", RDD("rsp", (int)_.op_fc_disp))
                          + (x86_zc_frame() == ZC_FRAME_RSP ? x86("mov", RDD("rsp", (int)(_.op_off + _.op_fc_disp + 32)), "eax")
                                                      : x86("mov", FR(_.op_off), "eax")))
                + (x86_zc_frame() == ZC_FRAME_RSP && rfc() ? x86("mov", RSP((int)(_.op_off + 24 + _.op_fc_disp + 32)), "r14")
                                                     : x86("mov", FRQ(_.op_off + 24), "r14"))
                : std::string())
         + IF(x86_zc_frame() != ZC_FRAME_RSP, IF(rfc(),  x86("mov",  "rdi", RSP((int)_.op_fc_disp + 8)))
             + x86_align_enter()
             + IF(!rfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
             + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to))
         + (rfc() ? x86_zls2_release_to_rspd((int)_.op_fc_disp + 16) : x86_zls2_release_to_call(_.op_off + 16))
         + x86_align_leave()
         + release_pump();
}
