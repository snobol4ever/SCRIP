#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_dyn_abolish_cell(void *fn_cell, void *ar_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_abolish() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_fn = (int)_.op_parts_ival[0];
    int s_ar = (int)_.op_parts_ival[1];
    return x86_alpha()
         + x86("comment", "IR_DET_ABOLISH")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_fn)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s_ar)))
         + x86("call", "rt_pl_dyn_abolish_cell", (uint64_t)(uintptr_t)(void *)rt_pl_dyn_abolish_cell)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86_omega()
         + IF(!_.op_bounded, x86_beta() + x86_omega());
}
