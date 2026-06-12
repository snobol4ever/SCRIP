#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_sort() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int do_msort = (int)_.op_parts_ival[0];
    int s_list   = (int)_.op_parts_ival[1];
    int s_result = (int)_.op_parts_ival[2];
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_SORT")
         + x86("mov", "edi", (uint32_t)do_msort)
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(s_list)))
         + x86("mov", "rdx", FRQ(GZ_CELL_OFF(s_result)))
         + x86("call", "rt_pl_sort_cell", (uint64_t)(uintptr_t)(void *)rt_pl_sort_cell)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}