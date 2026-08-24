#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void *rt_zls2_mark(void);
extern "C" void  rt_zls2_release_to(void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence_whack_on() { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_FENCE_WHACK"); v = (e && e[0] == '0') ? 0 : 1; } return v; }
static int fence_u2_frame(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_U2_FENCE"); v = (e && e[0] == '0') ? 0 : 1; } return v && x86_port_cstack() && _.op_ival != 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_mark_save(int off) {
    if (x86_port_cstack() && _.op_fence_frame_off != -1) return x86("mov", FFCQ(0), "rsp");
    if (x86_port_cstack() && fence_u2_frame()) return x86("mov", FRQ(off), "rsp") + x86("mov", FRQ(off+32), "rsp");
    if (x86_port_cstack()) return x86("mov", FRQ(off), "rsp");
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", RDQ("rdi", 0)) + x86("mov", FRQ(off), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("call", "rt_zls2_mark", (uint64_t)(uintptr_t)(void *)rt_zls2_mark) + x86_align_leave() + x86("mov", FRQ(off), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_release(int off, int kk = 0) {
    if (x86_port_cstack() && _.op_fence_frame_off != -1) return x86("mov", "rsp", FFCQ(0));
    if (x86_port_cstack() && fence_u2_frame()) return x86("mov", "rsp", FRQ(off+32+kk));
    if (x86_port_cstack()) return x86("mov", "rsp", FRQ(off+kk));
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", FRQ(off)) + x86("mov", RDQ("rdi", 0), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("mov", "rdi", FRQ(off)) + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)rt_zls2_release_to) + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_whack_commit(int off) {
    return fence_release(off, _.op_fence_body_kk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    if (_.op_ival == 0)
        return x86("comment", "IR_MATCH_FENCE1 ival=0 (FENCE0 interior sync box: alpha commits — match null — then gamma; beta abandons to omega; NO RSP whack — WHACK CONTRACT clause 5: statement ζ release is STATEMENT_END's authority; the op_zw arm that lived here double-released the UCLAIM statement claim below the ___ floor — ZWS-FENCE0-UCLAIM fix, MECH s6)")
             + x86_alpha()
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)")
         : x86("comment", "IR_MATCH_FENCE1 (SYNC-POINT zeta RELEASE: watermark at alpha, OVER-SEAL whack to the ___ activation floor at the seal-success glue — s137 ruling; H29-FIX s33: U-2 dual-slot save avoids ZLS alias between outer FRQ(off) and inner FRQ(off+32))")
         + x86_alpha()
         + fence_mark_save(_.op_off)
         + IF(fence_u2_frame(), bb_glue_framed_enter())
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + IF(emit_arbno_rbp_unwind() && x86_port_cstack(), x86_arbno_rbp_unwind_at(_.op_fence_frame_off != -1 ? FFCQ(0) : fence_u2_frame() ? FRQ(_.op_off+32+_.op_fence_body_kk) : FRQ(_.op_off+_.op_fence_body_kk), 11, 12))
         + fence_whack_commit(_.op_off)
         + x86_gamma()
         + x86("def", PAIR(3))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + x86_beta()
         + fence_release(_.op_off)
         + x86_omega();
}
