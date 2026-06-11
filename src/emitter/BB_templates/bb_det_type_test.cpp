#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_type_test_cell(void *cell_term, const char *fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_type_test_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const char *fn = _.op_parts_str[0];
    int slot = (int)_.op_parts_ival[0];
    if (!fn || slot < 0) return x86_bomb("bb_det_type_test: missing fn or slot");
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_TYPE_TEST")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
         + x86("mov", "rsi", ROQ(0))
         + x86("call", "rt_pl_type_test_cell", (uint64_t)(uintptr_t)(void *)rt_pl_type_test_cell)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86("def", L(0))
         + x86(".quad", LS(0), fn)
         + x86("label", LS(0))
         + x86(".string", fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_type_test(void) { bb_emit_x86(bb_det_type_test_str()); }
