#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_conjunction() {
    if (_.op_sa >= 0 && _.op_off >= 0 && _.op_sa != _.op_off) {
        return x86_alpha()
             + x86("comment", "IR_CONJUNCTION value-forward copy")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_pair_loop();
    }
    return x86_alpha() + x86_pair_loop();
}
