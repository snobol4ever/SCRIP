#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_ite() {
    x86_begin();
    return IF(_.op_parts_ival[0] < 0, x86_bomb("bb_cell_ite: unadmitted ifstmt shape reached the emitter"))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 0,
           x86_alpha()
         + x86("comment", "IR_CELL_ITE"))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 1,
           x86("comment", "IR_CELL_ITE")
         + x86("mov", FR(GZ_CELL_OFF((int)_.op_parts_ival[0])), (long)1)
         + x86_jmp_tgt(X86T_TGT0))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 2,
           x86("comment", "IR_CELL_ITE")
         + x86("mov", FR(GZ_CELL_OFF((int)_.op_parts_ival[0])), (long)2)
         + x86_jmp_tgt(X86T_TGT0))
         + IF(_.op_parts_ival[0] >= 0 && _.op_sa == 3,
           x86("comment", "IR_CELL_ITE")
         + x86_beta()
         + x86("mov", "eax", FR(GZ_CELL_OFF((int)_.op_parts_ival[0])))
         + x86("cmp", "eax", (long)1)
         + x86_jcc_tgt("je", X86T_TGT0)
         + x86_jmp_tgt(X86T_TGT1))
         + IF(_.op_parts_ival[0] >= 0 && (_.op_sa < 0 || _.op_sa > 3), x86_bomb("bb_cell_ite: unknown aspect"));
    return std::string();
}
