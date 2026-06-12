#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_char_type_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_char = (int)_.op_parts_ival[0];
    int s_type = (int)_.op_parts_ival[1];
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_CHAR_TYPE")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(s_char)))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(s_type)))
         + x86("xor", "edx", "edx")
         + x86("call", "rt_pl_char_type_cell", (uint64_t)(uintptr_t)(void *)rt_pl_char_type_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_char_type(void) { bb_emit_x86(bb_det_char_type_str()); }
