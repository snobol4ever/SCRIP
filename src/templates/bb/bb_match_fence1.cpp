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
    return sn4_u2_fence() && _.op_ival != SNO_FENCE_LIT_ARG_IN_ARBNO;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_release(int off, int kk = 0) {
    return _.op_fence_frame_off != -1 ? x86("mov", "rsp", FFCQ(0))
         : fence_u2_frame()           ? x86("mov", "rsp", FRQ(off+32+kk))
                                       : x86("mov", "rsp", FRQ(off+kk));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * fence_cap_top(int off) {
    return _.op_fence_frame_off != -1 ? FFCQ(8) : FRQ(off+8);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * fence_cursor_tag(int off) {
    return _.op_fence_frame_off != -1 ? FFCQ(16) : FRQ(off+16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * fence_cursor(int off) {
    return _.op_fence_frame_off != -1 ? FFC(20) : FR(off+20);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    return IF(_.op_ival == SNO_FENCE_LIT_BARE,
               x86("comment", "IR_MATCH_FENCE1")
             + x86_alpha()
             + x86_gamma()
             + x86_beta()
             + x86_omega())
         + IF(_.op_ival != SNO_FENCE_LIT_BARE && _.op_off < 0,
               x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)"))
         + IF(_.op_ival != SNO_FENCE_LIT_BARE && _.op_off >= 0,
               x86("comment", "IR_MATCH_FENCE1")
         + x86_alpha()
         + (_.op_fence_frame_off != -1 ? x86("mov", FFCQ(0), "rsp")
          : fence_u2_frame()           ? x86("mov", FRQ(_.op_off), "rsp")
                                      + x86("mov", FRQ(_.op_off+32), "rsp")
                                        : x86("mov", FRQ(_.op_off), "rsp"))
         + x86("mov", fence_cap_top(_.op_off), "r12")
         + x86("mov", fence_cursor_tag(_.op_off), 0L)
         + x86("mov", fence_cursor(_.op_off), "r14d")
         + IF(fence_u2_frame(), bb_glue_framed_enter())
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + fence_release(_.op_off, _.op_fence_body_kk)
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(4))
         + IF(fence_u2_frame(), bb_glue_framed_leave())
         + x86_beta()
         + x86("mov", "r12", fence_cap_top(_.op_off))
         + x86("mov", "r14d", fence_cursor(_.op_off))
         + fence_release(_.op_off)
         + x86_omega());
}
