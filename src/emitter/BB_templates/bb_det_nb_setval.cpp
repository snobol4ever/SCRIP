#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_nb_setval_cell(void *key_cell, void *val_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_nb_setval() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_key = (int)_.op_parts_ival[0];
    int s_val = (int)_.op_parts_ival[1];
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_NB_SETVAL")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(s_key)))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(s_val)))
         + x86("call", "rt_pl_nb_setval_cell", (uint64_t)(uintptr_t)(void *)rt_pl_nb_setval_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
