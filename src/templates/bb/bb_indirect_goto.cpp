#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_indirect_goto() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_indirect_goto: no label-variable slot (op_off<0)");
    return x86("comment", "IR_INDIRECT_GOTO gate box: alpha CONCEDES, beta RESUMES through the banked gate")
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + x86("jmp", FRQ(_.op_off + 16));
}
