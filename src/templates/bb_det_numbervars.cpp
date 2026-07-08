#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_numbervars_cell(void *term_cell, void *start_cell, void *end_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_numbervars() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_term  = (int)_.op_parts_ival[0];
    int s_start = (int)_.op_parts_ival[1];
    int s_end   = (int)_.op_parts_ival[2];
    return x86_alpha()
         + x86("comment", "IR_DET_NUMBERVARS")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_term)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s_start)))
         + x86("lea", "rdx", FR(GZ_CELL_OFF(s_end)))
         + x86("call", "rt_pl_numbervars_cell", (uint64_t)(uintptr_t)(void *)rt_pl_numbervars_cell)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86_omega()
         + IF(!_.op_bounded, x86_beta() + x86_omega());
}
