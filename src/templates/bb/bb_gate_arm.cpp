#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gate_arm() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_gate_arm: no gate slot (op_off<0)");
    if (_.op_ival < 0) return x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)");
    return x86("comment", "IR_GATE_ARM")
         + x86_alpha()
         + x86("mov", FR(_.op_off + 16), (int) _.op_ival)
         + x86_gamma()
         + x86_beta_trampoline();
}
