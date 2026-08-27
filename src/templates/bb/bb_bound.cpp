#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_bound() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)");
    if (_.op_zres) {
        if (_.op_sb == 1)
            return x86("comment", "IR_BOUND cells arm -- save rsp to frame slot (Op_Mark: bounded-expression entry frontier)")
                 + x86_alpha()
                 + x86("mov", FRQ(_.op_off), "rsp")
                 + x86_gamma()
                 + x86_beta_trampoline();
        _.op_zgpop = 0;
        return x86("comment", "IR_UNMARK cells arm -- restore rsp from BOUND frame slot (depth-immune FRQ)")
             + x86_alpha()
             + x86("mov", "rsp", FRQ(_.op_off))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", _.op_sb == 1 ? "IR_BOUND" : "IR_UNMARK")
         + x86_alpha()
         + IF(_.op_sb == 1, x86("mov", FRQ(_.op_off), "rsp"))
         + IF(_.op_sb != 1, x86("mov", "rsp", FRQ(_.op_off)))
         + x86_gamma()
         + x86_beta_trampoline();
}
