#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_nb_setval_cell(void *key_cell, void *val_cell);
extern "C" void *rt_pl_nb_copy_persist(void *val_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_nb_setval() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_key = (int)_.op_parts_ival[0];
    int s_val = (int)_.op_parts_ival[1];
    if (_.op_gva_k >= 0)
        return x86("def",     "α")
             + x86("comment", "IR_DET_NB_SETVAL gva")
             + x86("lea", "rdi", FR(GZ_CELL_OFF(s_val)))
             + x86("call", "rt_pl_nb_copy_persist", (uint64_t)(uintptr_t)(void *)rt_pl_nb_copy_persist)
             + x86("mov", RDQ("rbx", _.op_gva_k * 16), "rax")
             + x86("jmp", "γ")
             + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
    return x86("def",     "α")
         + x86("comment", "IR_DET_NB_SETVAL")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_key)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s_val)))
         + x86("call", "rt_pl_nb_setval_cell", (uint64_t)(uintptr_t)(void *)rt_pl_nb_setval_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
}
