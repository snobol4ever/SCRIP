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
static std::string fence_release(int off, int kk = 0) {
    return _.op_fence_frame_off != -1 ? x86("mov", "rsp", FFCQ(0))
         : fence_u2_frame()           ? x86("mov", "rsp", FRQ(off+32+kk))
                                       : x86("mov", "rsp", FRQ(off+kk));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    if (_.op_ival == 0)
        return x86("comment", "IR_MATCH_FENCE1")
             + x86_alpha()
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)")
         : x86("comment", "IR_MATCH_FENCE1")
         + x86_alpha()
         + (_.op_fence_frame_off != -1 ? x86("mov", FFCQ(0), "rsp")
          : fence_u2_frame()           ? x86("mov", FRQ(_.op_off), "rsp")
                                      + x86("mov", FRQ(_.op_off+32), "rsp")
                                        : x86("mov", FRQ(_.op_off), "rsp"))
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
         + fence_release(_.op_off, _.op_fence_body_kk)
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(4))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + x86_beta()
         + fence_release(_.op_off)
         + x86_omega();
}
