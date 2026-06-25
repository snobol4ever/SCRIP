#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_univ_cell(void *t0_cell, void *list_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_univ() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int sl0 = (int)_.op_parts_ival[0];
    int sl1 = (int)_.op_parts_ival[1];
    if (sl0 < 0 || sl1 < 0) return x86_bomb("bb_det_univ: invalid slot");
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_UNIV")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(sl0)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(sl1)))
         + x86("call", "rt_pl_univ_cell", (uint64_t)(uintptr_t)(void *)rt_pl_univ_cell)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + IF(!_.op_bounded, x86("def", "β") + x86("jmp", "ω"));
}