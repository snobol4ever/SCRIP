#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_dyn_assertz_cell(void *clause_cell, int prepend);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_assertz() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_cl = (int)_.op_parts_ival[0];
    long prepend = _.op_parts_ival[1];
    return x86("def",     "α")
         + x86("comment", "IR_DET_ASSERTZ")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_cl)))
         + x86("mov32", "esi", prepend)
         + x86("call", "rt_pl_dyn_assertz_cell", (uint64_t)(uintptr_t)(void *)rt_pl_dyn_assertz_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
}
