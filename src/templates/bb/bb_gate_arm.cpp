#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_zrow_gcell(const IR_t * nd);
static std::string gate_arm_cell() {
    if (_.op_ival < 0) return x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)");
    if (_.op_xrec & 4) return x86_alpha() + x86_bomb("bb_gate_arm: a gamma record restores rsp where the arm pushes its index cell");
    std::string a = x86_alpha();
    const long w = (long)_.op_zgpop;
    _.op_zgpop = 0;
    return a
         + x86("note", "the arm pushes its live-arm index as its own spine cell {DT_RAW, arm}, below the join wall, so the GATE's beta finds it at [rsp + 8] whichever arm ran "
                       "(ARCH-PROLOG-ACTIVATION-LIFETIME 8.2)")
         + IF(w < 0, x86_add("rsp", w))
         + x86_rsp_store64_imm(0, (long)DT_RAW)
         + x86_rsp_store64_imm(8, (long)_.op_ival)
         + IF(w > 0, x86("note", "a popping wall: the continuation restores below this construct (a commit), so no beta reaches the gate again and the cell goes with the carve")
                   + x86_add("rsp", w))
         + x86_gamma()
         + x86_beta_trampoline();
}
std::string bb_gate_arm() {
    x86_begin();
    if (emit_zrow_gcell(_.node)) return gate_arm_cell();
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_gate_arm: no gate slot (op_off<0)"))
         + IF(_.op_off >= 0 && _.op_ival < 0, x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)"))
         + IF(_.op_off >= 0 && _.op_ival >= 0,
               x86("comment", "IR_GATE_ARM")
             + x86_alpha()
             + x86("mov", FR(_.op_off + 16), (int) _.op_ival)
             + x86_gamma()
             + x86_beta_trampoline());
}
