#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_cmp_str() {
    x86_begin();
    if (PLATFORM_X86) return IF(_.op_parts_ival[0] == -1, x86_bomb("bb_det_cmp: term-order cmp not yet supported in GZ"))
         + IF(_.op_parts_ival[0] == -2, x86_bomb("bb_det_cmp: null operand"))
         + IF(_.op_parts_ival[0] == 0 || _.op_parts_ival[0] == 1,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_CMP")
             + x86("jmp", _.op_parts_ival[0] == 1 ? "γ" : "ω")
             + x86("def", "β")
             + x86("jmp", "ω"))
         + IF(_.op_parts_ival[0] == 2,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_CMP")
             + x86("mov", "rdi", ROQ(0))
             + IF(_.op_parts_ival[1] != 0, x86("mov", "rsi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[2]))))
             + IF(_.op_parts_ival[1] == 0, x86("xor", "esi", "esi"))
             + x86("mov", "rdx", _.op_parts_ival[3])
             + IF(_.op_parts_ival[4] != 0, x86("mov", "rcx", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[5]))))
             + IF(_.op_parts_ival[4] == 0, x86("xor", "ecx", "ecx"))
             + x86("mov", "r8", _.op_parts_ival[6])
             + x86("call", "rt_pl_arith_cmp_cell_val", (uint64_t)(uintptr_t)(void *)rt_pl_arith_cmp_cell_val)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86("def", L(0))
             + x86(".quad", LS(0), _.op_sval)
             + x86("label", LS(0))
             + x86(".string", _.op_sval));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_cmp(void) { bb_emit_x86(bb_det_cmp_str()); }
