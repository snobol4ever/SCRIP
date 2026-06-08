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
static std::string bb_var_frame_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return x86_bomb("bb_var_frame: needs gvar flat-chain + own slot");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", std::string("BOX IR_VAR_FRAME \"") + (_.op_sval ? _.op_sval : "") + "\" slot=" + std::to_string((int) _.op_ival) + " hops=" + std::to_string((int) _.op_dval) + " -> [r12+" + std::to_string(_.op_off) + "]"))
         + x86_frame_lea("rax", 0)
         + FOR(0, (int) _.op_dval, [](int) { return x86_reg_disp32_load64("rax", "rax", 0); })
         + x86_reg_disp32_load64("rcx", "rax", 16 + (int) _.op_ival * 16)     + x86_frame_store64(_.op_off, "rcx")
         + x86_reg_disp32_load64("rcx", "rax", 16 + (int) _.op_ival * 16 + 8) + x86_frame_store64(_.op_off + 8, "rcx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var_frame(IR_t * pBB) { bb_emit_x86(bb_var_frame_str(pBB)); }
