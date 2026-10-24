#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_act_arm() {
    return x86("comment", "IR_MATCH_FENCE1 (ACT: the record in the match frame, the planner's word for an indefinite body)")
         + x86_alpha()
         + x86("mov", FFCQ(0), "rsp")
         + x86("mov", FFCQ(8), "r12")
         + x86("mov", FFCQ(16), 0L)
         + x86("mov", FFC(20), "r14d")
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "rsp", FFCQ(0))
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(4))
         + x86_beta()
         + x86("mov", "r12", FFCQ(8))
         + x86("mov", "r14d", FFC(20))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_spine_arm() {
    return x86("comment",
        "IR_MATCH_FENCE1 (SPINE: the fence's own 16-byte record -- tag 0, cursor, the capture-stack top -- and the body's cells discarded here by the plan's constant, the body's exit edge priced at "
        "zero)")
         + x86_alpha()
         + x86("sub", "rsp", 16L)
         + x86("mov", RDD("rsp", 0), 0L)
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86("mov", RDQ("rsp", 8), "r12")
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + IF(_.op_fence_body_kk > 0, x86("add", "rsp", (long)_.op_fence_body_kk))
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(4))
         + x86_beta()
         + x86("mov", "r12", RDQ("rsp", 8))
         + x86("mov", "r14d", RDD("rsp", 4))
         + x86("add", "rsp", 16L)
         + x86_omega();
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
         + IF(_.op_ival != SNO_FENCE_LIT_BARE && _.op_fence_frame_off != -1, fence_act_arm())
         + IF(_.op_ival != SNO_FENCE_LIT_BARE && _.op_fence_frame_off == -1 && _.op_fence_body_kk >= 0, fence_spine_arm())
         + IF(_.op_ival != SNO_FENCE_LIT_BARE && _.op_fence_frame_off == -1 && _.op_fence_body_kk < 0,
               x86_alpha()
             + x86_bomb("IR_MATCH_FENCE1: the body is indefinite and the planner granted no frame word (no enclosing match frame), CEO-1938")
             + x86("def", PAIR(2))
             + x86("def", PAIR(3))
             + x86("def", PAIR(4))
             + x86_beta()
             + x86_omega());
}
