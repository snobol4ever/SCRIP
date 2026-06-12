#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
extern int g_gvar_flat_chain;
#include "emit.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_frame(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return x86_bomb("bb_var_frame: needs gvar flat-chain + own slot");
    return x86("comment", "IR_VAR_FRAME")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rax", FRQ(0))
         + FOR(0, (int)_.op_dval, [](int) { return x86("mov", "rax", "[rax+0]"); })
         + x86("mov",     "rcx", "[rax+" + std::to_string(16 + (int)_.op_ival * 16) + "]")
         + x86("mov",     FRQ(_.op_off),     "rcx")
         + x86("mov",     "rcx", "[rax+" + std::to_string(16 + (int)_.op_ival * 16 + 8) + "]")
         + x86("mov",     FRQ(_.op_off + 8), "rcx")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
