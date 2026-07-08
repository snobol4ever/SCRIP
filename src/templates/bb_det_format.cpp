#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void rt_pl_format_cell(const char *fmt, void *list_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_format() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const char *fmt = _.op_parts_str[0];
    int arity = (int)_.op_parts_ival[0];
    int list_slot = (int)_.op_parts_ival[1];
    if (!fmt) return x86_bomb("bb_det_format: missing fmt");
    return x86_alpha()
         + x86("comment", "IR_DET_FORMAT")
         + x86("mov", "rdi", ROQ(0))
         + IF(arity == 2 && list_slot >= 0,
               x86("lea", "rsi", FR(GZ_CELL_OFF(list_slot))))
         + IF(!(arity == 2 && list_slot >= 0),
               x86("xor", "esi", "esi"))
         + x86("call", "rt_pl_format_cell", (uint64_t)(uintptr_t)(void *)rt_pl_format_cell)
         + x86_gamma()
         + IF(!_.op_bounded, x86_beta() + x86_omega())
         + x86("def", L(0))
         + x86(".quad", LS(0), fmt)
         + x86("label", LS(0))
         + x86(".string", fmt);
}
