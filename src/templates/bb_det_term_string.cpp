#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_term_string_cell(void *term_cell, void *str_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_term_string() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_term   = (int)_.op_parts_ival[0];
    int s_result = (int)_.op_parts_ival[1];
    return x86_alpha()
         + x86("comment", "IR_DET_TERM_STRING")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_term)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s_result)))
         + x86("call", "rt_pl_term_string_cell", (uint64_t)(uintptr_t)(void *)rt_pl_term_string_cell)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86_omega()
         + IF(!_.op_bounded, x86_beta() + x86_omega());
}
