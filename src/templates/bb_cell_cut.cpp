#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_cut() {
    x86_begin();
    if (PLATFORM_X86) return x86("label", _.lbl_α)
                           + x86("comment", "IR_CELL_CUT")
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω");
    return std::string();
}