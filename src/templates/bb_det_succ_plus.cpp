#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_succ_plus() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    long arity = (long)_.op_parts_ival[0];
    int s0 = (int)_.op_parts_ival[1], s1 = (int)_.op_parts_ival[2], s2 = (int)_.op_parts_ival[3];
    return IF(!((arity == 2 && s0 >= 0 && s1 >= 0) || (arity == 3 && s0 >= 0 && s1 >= 0 && s2 >= 0)),
              x86_bomb("bb_det_succ_plus: unadmitted shape reached the emitter"))
         + IF((arity == 2 && s0 >= 0 && s1 >= 0) || (arity == 3 && s0 >= 0 && s1 >= 0 && s2 >= 0),
              x86("label", _.lbl_α)
            + x86("comment", "IR_DET_SUCC_PLUS")
            + x86("mov", "edi", arity)
            + x86("lea", "rsi", FR(GZ_CELL_OFF(s0)))
            + x86("lea", "rdx", FR(GZ_CELL_OFF(s1)))
            + IF(arity == 3, x86("lea", "rcx", FR(GZ_CELL_OFF(s2))))
            + IF(arity != 3, x86("xor", "ecx", "ecx"))
            + x86("call", "rt_pl_succ_plus_cell", (uint64_t)(uintptr_t)(void *)rt_pl_succ_plus_cell)
            + x86("test", "eax", "eax")
            + x86("jne", "γ")
            + x86("jmp", "ω")
            + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω")));
}