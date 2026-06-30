#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_is_cell_int(void *lhs_cell, long val);
extern "C" int rt_pl_is_cell_float(void *lhs_cell, double val);
extern "C" int rt_pl_is_cell_arith(void *lhs_cell, void *rhs_cell, const char *op, long rhs_ival);
extern "C" int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double bdi_fv() { double d; memcpy(&d, &_.op_parts_ival[2], 8); return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_is() {
    x86_begin();
    if (PLATFORM_X86) return IF(_.op_parts_ival[0] == -1, x86_bomb("bb_det_is: lhs not LOGICVAR"))
         + IF(_.op_parts_ival[0] == -2, x86_bomb("bb_det_is: unsupported rhs shape in GZ"))
         + IF(_.op_parts_ival[0] == 0,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[1])))
             + x86("mov", "rsi", _.op_parts_ival[2])
             + x86("call", "rt_pl_is_cell_int", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_int)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω")))
         + IF(_.op_parts_ival[0] == 1,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[1])))
             + x86("lea", "rsi", FR(GZ_CELL_OFF((int)_.op_parts_ival[2])))
             + x86("mov", "rdx", ROQ(0))
             + x86("mov", "rcx", _.op_parts_ival[3])
             + x86("call", "rt_pl_is_cell_arith", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_arith)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"))
             + x86("def", L(0))
             + x86(".quad", LS(0), _.op_parts_str[0])
             + x86("label", LS(0))
             + x86(".string", _.op_parts_str[0]))
         + IF(_.op_parts_ival[0] == 2,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[1])))
             + x86("lea", "rsi", FR(GZ_CELL_OFF((int)_.op_parts_ival[2])))
             + x86("lea", "rdx", FR(GZ_CELL_OFF((int)_.op_parts_ival[3])))
             + x86("mov", "rcx", ROQ(0))
             + x86("call", "rt_pl_is_cell_bivar", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_bivar)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"))
             + x86("def", L(0))
             + x86(".quad", LS(0), _.op_parts_str[0])
             + x86("label", LS(0))
             + x86(".string", _.op_parts_str[0]))
         + IF(_.op_parts_ival[0] == 3,
               x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS float-const")
             + x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[1])))
             + x86("movsd", "xmm0", F64(bdi_fv()))
             + x86("call", "rt_pl_is_cell_float", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_float)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω")));
    return std::string();
}
