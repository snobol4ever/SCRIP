#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------
 * bb_move_label -- IR_MOVE_LABEL template (unbounded alternation, GOAL-IR-IMMUTABLE-EMIT.md label-variable rung).
 * JCON ir_a_Alt's /bounded arm inserts, on EVERY arm's success path (irgen.icn:187-190),
 *     ir_MoveLabel(t, eList[i].resume); Goto p.ir.success
 * i.e. "record where THIS arm resumes, then deliver the value to the consumer". This template is that pair plus
 * the value-convergence copy JCON gets from its shared per-arm `target` parameter:
 *   1. copy the arm's own 16-byte value DESCR ([r12+op_sa]) into the alternation's SHARED value cell
 *      ([r12+op_off], owned by the sibling IR_INDIRECT_GOTO node -- one slot serves every consumer, per the
 *      lhs<->tmp doctrine; skipped when the arm produced no slot, op_sa<0, e.g. a pure control arm);
 *   2. lea rax, [rip + <resume label>] -- the sibling-node chain label threaded through the t0 port by
 *      emit_drive's IR_MOVE_LABEL arm (the g_create_body_entry / x86_lea_tgt precedent from co-expression RUNG 3b;
 *      LOWER stamps ival=1 when the captured arm-beta is ir_is_generator_kind, resolving to the target's BETA
 *      label -- resume produces the arm's next value -- and ival=0 otherwise, resolving to the ALPHA of the arm's
 *      failure continuation, JCON's "a non-generator's resume IS its failure": next arm fresh, or the alt's omega);
 *   3. mov [r12 + op_off + 16], rax -- write the label variable t;
 *   4. jmp gamma -- deliver to the consumer.
 * beta defined defensively (jmp omega): a MoveLabel is a pass-through on the success path, never itself resumed.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_move_label() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("bb_move_label: no shared-slot owner (op_off<0)");
    if (!_.lbl_t0) return x86_bomb("bb_move_label: resume-target label unresolved (lbl_t0 NULL)");
    std::string s = x86("comment", "IR_MOVE_LABEL: shared value := arm value; t := &arm-resume")
                  + x86("label", _.lbl_α);
    if (_.op_sa >= 0 && _.op_sa != _.op_off)
        s += x86("mov", "rax", FRQ(_.op_sa))     + x86("mov", FRQ(_.op_off), "rax")
           + x86("mov", "rax", FRQ(_.op_sa + 8)) + x86("mov", FRQ(_.op_off + 8), "rax");
    s += x86_lea_tgt("rax", X86T_TGT0)
       + x86("mov", FRQ(_.op_off + 16), "rax")
       + x86("jmp", "γ")
       + x86("def", "β")
       + x86("jmp", "ω");
    return s;
}
