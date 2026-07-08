#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_atom_op() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const char *fn = _.op_parts_str[0];
    int arity = (int)_.op_parts_ival[0];
    int s0 = (int)_.op_parts_ival[1], s1 = (int)_.op_parts_ival[2], s2 = (int)_.op_parts_ival[3];
    if (!fn) return x86_bomb("bb_det_atom_op: missing fn");
    return x86_alpha()
         + x86("comment", "IR_DET_ATOM_OP")
         + x86("mov", "rdi", ROQ(0))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s0)))
         + x86("lea", "rdx", FR(GZ_CELL_OFF(s1)))
         + IF(arity == 3 && s2 >= 0, x86("lea", "rcx", FR(GZ_CELL_OFF(s2))))
         + IF(!(arity == 3 && s2 >= 0), x86("xor", "ecx", "ecx"))
         + x86("call", "rt_pl_atom_op_cell", (uint64_t)(uintptr_t)(void *)rt_pl_atom_op_cell)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86_omega()
         + IF(!_.op_bounded, x86_beta() + x86_omega())
         + x86("def", L(0))
         + x86(".quad", LS(0), fn)
         + x86("label", LS(0))
         + x86(".string", fn);
}
