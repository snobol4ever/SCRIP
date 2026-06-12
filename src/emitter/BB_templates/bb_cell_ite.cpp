#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcit_gate_off() { return GZ_CELL_OFF((int)_.op_parts_ival[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_ite() {
    x86_begin();
    if (PLATFORM_X86) return IF(_.op_parts_ival[0] < 0, x86_bomb("bb_cell_ite: unadmitted ifstmt shape reached the emitter"))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 0,
           x86("label", _.lbl_α)
         + x86("comment", "IR_CELL_ITE"))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 1,
           x86("comment", "IR_CELL_ITE")
         + x86("mov", FR(bcit_gate_off()), (long)1)
         + x86("jmp", PORT_DELTA))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 2,
           x86("comment", "IR_CELL_ITE")
         + x86("mov", FR(bcit_gate_off()), (long)2)
         + x86("jmp", PORT_DELTA))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 3,
           x86("comment", "IR_CELL_ITE")
         + x86("def", "β")
         + x86("mov", "eax", FR(bcit_gate_off()))
         + x86("cmp", "eax", (long)1)
         + x86("je", PORT_DELTA)
         + x86("jmp", PORT_EPSILON))
         + IF(_.op_parts_ival[0] >= 0 && (_.op_sa < 0 || _.op_sa > 3), x86_bomb("bb_cell_ite: unknown aspect"));
    return std::string();
}