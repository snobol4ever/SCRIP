#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence_u2_frame(void) {
    static int v = -1;
    if (v < 0)
        { const char * e = getenv("SCRIP_U2_FENCE"); v = (e && e[0] == '0') ? 0 : 1; }
    return v && _.op_ival != 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_mark_save(int off) {
    if (_.op_fence_frame_off != -1) return x86("mov", FFCQ(0), "rsp");
    if (fence_u2_frame())
        return x86("mov", FRQ(off), "rsp")
             + x86("mov", FRQ(off+32), "rsp");
    return x86("mov", FRQ(off), "rsp");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_release(int off, int kk = 0) {
    if (_.op_fence_frame_off != -1) return x86("mov", "rsp", FFCQ(0));
    if (fence_u2_frame()) return x86("mov", "rsp", FRQ(off+32+kk));
    return x86("mov", "rsp", FRQ(off+kk));
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
         + IF(emit_arbno_rbp_unwind(),
              x86_arbno_rbp_unwind_at(
                  _.op_fence_frame_off != -1 ? FFCQ(0)
                : fence_u2_frame()           ? FRQ(_.op_off+32+_.op_fence_body_kk)
                                              : FRQ(_.op_off+_.op_fence_body_kk),
                  11, 12))
         + fence_whack_commit(_.op_off)
         + x86_gamma()
         + x86("def", PAIR(3))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + x86_beta()
         + fence_release(_.op_off)
         + x86_omega();
}
