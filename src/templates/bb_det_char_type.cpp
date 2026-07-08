#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_char_type() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_char = (int)_.op_parts_ival[0];
    int s_type = (int)_.op_parts_ival[1];
    return x86("def",     "α")
         + x86("comment", "IR_DET_CHAR_TYPE")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(s_char)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(s_type)))
         + x86("xor", "edx", "edx")
         + x86("call", "rt_pl_char_type_cell", (uint64_t)(uintptr_t)(void *)rt_pl_char_type_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
}
