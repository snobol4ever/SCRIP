#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_arg_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int sl0 = (int)_.op_parts_ival[0];
    int sl1 = (int)_.op_parts_ival[1];
    int sl2 = (int)_.op_parts_ival[2];
    if (sl0 < 0 || sl1 < 0 || sl2 < 0) return x86_bomb("bb_det_arg: invalid slot");
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_ARG")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(sl0)))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(sl1)))
         + x86("mov", "rdx", FRQ(GZ_CELL_OFF(sl2)))
         + x86("call", "rt_pl_arg_cell", (uint64_t)(uintptr_t)(void *)rt_pl_arg_cell)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_arg(void) { bb_emit_x86(bb_det_arg_str()); }
