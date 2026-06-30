#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival);
extern "C" int rt_term_cmp_terms(const char *op, void *t0, void *t1);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_cmp() {
    x86_begin();
    if (PLATFORM_X86) return IF(_.op_parts_ival[0] == -1, x86_bomb("bb_det_cmp: unknown cmp op"))
         + IF(_.op_parts_ival[0] == -2, x86_bomb("bb_det_cmp: null operand"))
         + IF(_.op_parts_ival[0] == 0 || _.op_parts_ival[0] == 1,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_CMP")
             + x86("jmp", _.op_parts_ival[0] == 1 ? "γ" : "ω")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω")))
         + IF(_.op_parts_ival[0] == 2,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_CMP")
             + x86("mov", "rdi", ROQ(0))
             + IF(_.op_parts_ival[1] != 0, x86("lea", "rsi", FR(GZ_CELL_OFF((int)_.op_parts_ival[2]))))
             + IF(_.op_parts_ival[1] == 0, x86("xor", "esi", "esi"))
             + x86("mov", "rdx", _.op_parts_ival[3])
             + IF(_.op_parts_ival[4] != 0, x86("lea", "rcx", FR(GZ_CELL_OFF((int)_.op_parts_ival[5]))))
             + IF(_.op_parts_ival[4] == 0, x86("xor", "ecx", "ecx"))
             + x86("mov", "r8", _.op_parts_ival[6])
             + x86("call", "rt_pl_arith_cmp_cell_val", (uint64_t)(uintptr_t)(void *)rt_pl_arith_cmp_cell_val)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"))
             + x86("def", L(0))
             + x86(".quad", LS(0), _.op_sval)
             + x86("label", LS(0))
             + x86(".string", _.op_sval))
         + IF(_.op_parts_ival[0] == 3,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_CMP term-order")
             + x86("mov", "rdi", ROQ(0))
             + IF(_.op_parts_ival[1] >= 0, x86("lea", "rsi", FR(GZ_CELL_OFF((int)_.op_parts_ival[1]))))
             + IF(_.op_parts_ival[1] < 0,  x86("xor", "esi", "esi"))
             + IF(_.op_parts_ival[2] >= 0, x86("lea", "rdx", FR(GZ_CELL_OFF((int)_.op_parts_ival[2]))))
             + IF(_.op_parts_ival[2] < 0,  x86("xor", "edx", "edx"))
             + x86("call", "rt_term_cmp_terms", (uint64_t)(uintptr_t)(void *)rt_term_cmp_terms)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"))
             + x86("def", L(0))
             + x86(".quad", LS(0), _.op_sval)
             + x86("label", LS(0))
             + x86(".string", _.op_sval));
    return std::string();
}