#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_copy_term_cell(void *term_cell, void *copy_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_copy_term() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int s_term = (int)_.op_parts_ival[0];
    int s_copy = (int)_.op_parts_ival[1];
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_COPY_TERM")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(s_term)))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(s_copy)))
         + x86("call", "rt_pl_copy_term_cell", (uint64_t)(uintptr_t)(void *)rt_pl_copy_term_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
