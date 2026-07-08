#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_type_test_cell(void *cell_term, const char *fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_type_test() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const char *fn = _.op_parts_str[0];
    int slot = (int)_.op_parts_ival[0];
    if (!fn || slot < 0) return x86_bomb("bb_det_type_test: missing fn or slot");
    return x86_alpha()
         + x86("comment", "IR_DET_TYPE_TEST")
         + x86("lea", "rdi", FR(GZ_CELL_OFF(slot)))
         + x86("mov", "rsi", ROQ(0))
         + x86("call", "rt_pl_type_test_cell", (uint64_t)(uintptr_t)(void *)rt_pl_type_test_cell)
         + x86("test", "eax", "eax")
         + x86_omega("je")
         + x86_gamma()
         + IF(!_.op_bounded, x86_beta() + x86_omega())
         + x86("def", L(0))
         + x86(".quad", LS(0), fn)
         + x86("label", LS(0))
         + x86(".string", fn);
}
