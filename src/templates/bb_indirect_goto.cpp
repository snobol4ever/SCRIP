#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------
 * bb_indirect_goto -- IR_INDIRECT_GOTO template (unbounded alternation, GOAL-IR-IMMUTABLE-EMIT.md label-variable
 * rung). JCON ir_a_Alt's /bounded arm places `ir_IndirectGoto(t)` at the alternation's RESUME label
 * (irgen.icn:183): a consumer that backtracks into the alternation must re-enter WHICHEVER ARM LAST PRODUCED --
 * a data-dependent target no static edge can name (the exact gap the punch list proved against the chain-BFS).
 * This node IS that resume position: lower_alt sets cx->beta to it, so every consumer backtrack edge lands on its
 * alpha, whose entire body is one indirect jump through the label variable t at [r12 + op_off + 16] (written by
 * whichever bb_move_label ran on the last-successful arm's gamma path; flow guarantees a MoveLabel always precedes
 * the first indirect jump, matching JCON's own uninitialized-until-first-success discipline). [op_off + 0..15] is
 * the alternation's SHARED value DESCR (see bb_move_label.cpp / scrip_ir.c's k+=2 comment); this template never
 * touches it. beta is defined defensively (jmp omega) -- nothing should ever resume the resume node itself. The
 * binary arm of the frame-indirect jmp reuses the byte pattern (49 FF /4 [r12+disp]) already proven live by the
 * generator-proc suspend machinery in emit.cpp's chain body; here it goes through the x86() encoder front-end per
 * TEMPLATE-ONLY EMISSION.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_indirect_goto() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("bb_indirect_goto: no label-variable slot (op_off<0)");
    return x86("comment", "IR_INDIRECT_GOTO alt-resume: jmp *t")
         + x86("label", _.lbl_α)
         + x86("jmp", FRQ(_.op_off + 16))
         + x86("def", "β")
         + x86("jmp", "ω");
}
