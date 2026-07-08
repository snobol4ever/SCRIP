#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_dyn_retract_cell(void *head_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_retract() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_head = (int)_.op_parts_ival[0];
    return x86("def",     "α")
         + x86("comment", "IR_DET_RETRACT")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_head)))
         + x86("call", "rt_pl_dyn_retract_cell", (uint64_t)(uintptr_t)(void *)rt_pl_dyn_retract_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
}
