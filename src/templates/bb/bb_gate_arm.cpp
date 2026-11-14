#include <string>
#include <utility>
#include <stdint.h>
#include "ir/ir_node.h"
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "ir/descr_tags_dt_platom.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_frame.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gate_arm_gpop_take() { return std::exchange(_.op_zgpop, 0); }
static std::string gate_arm_carve(long w) {
    return x86("note", "the arm pushes its live-arm index as its own spine cell {DT_RAW, arm}, below the join wall, so the GATE's beta finds it at [rsp + 8] whichever arm ran "
                       "(ARCH-PROLOG-ACTIVATION-LIFETIME 8.2)")
         + IF(w < 0, x86_add("rsp", w))
         + x86_rsp_store64_imm(0, (long)DT_RAW)
         + x86_rsp_store64_imm(8, (long)_.op_ival)
         + IF(w > 0, x86("note", "a popping wall: the continuation restores below this construct (a commit), so no beta reaches the gate again and the cell goes with the carve")
                   + x86_add("rsp", w));
}
static std::string gate_arm_cell() {
    return IF(_.op_ival < 0, x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)"))
         + IF(_.op_ival >= 0 && (_.op_xrec & 4), x86_alpha() + x86_bomb("bb_gate_arm: a gamma record restores rsp where the arm pushes its index cell"))
         + IF(_.op_ival >= 0 && !(_.op_xrec & 4), emit_seq({x86_alpha(), gate_arm_carve((long)gate_arm_gpop_take()), x86_gamma(), x86_beta_trampoline()}));
}
std::string bb_gate_arm() {
    x86_begin();
    return IF(_.op_gcell, gate_arm_cell())
         + IF(!_.op_gcell && _.op_off < 0, x86_alpha() + x86_bomb("bb_gate_arm: no gate slot (op_off<0)"))
         + IF(!_.op_gcell && _.op_off >= 0 && _.op_ival < 0, x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)"))
         + IF(!_.op_gcell && _.op_off >= 0 && _.op_ival >= 0,
               x86("comment", "IR_GATE_ARM")
             + x86_alpha()
             + x86("mov", FR(_.op_off + 16), (int) _.op_ival)
             + x86_gamma()
             + x86_beta_trampoline());
}
