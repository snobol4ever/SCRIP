#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_conjunction() {
    return IF(_.op_sa >= 0 && _.op_off >= 0 && _.op_sa != _.op_off,
               x86_alpha()
             + x86("comment", "IR_CONJUNCTION value-forward copy")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_pair_loop())
         + IF(!(_.op_sa >= 0 && _.op_off >= 0 && _.op_sa != _.op_off),
               x86_alpha() + x86_pair_loop());
}
