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
static std::string bb_var_frame_ref_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return x86_bomb("bb_var_frame_ref: needs gvar flat-chain + own slot");
    int hops = (int) _.op_dval;
    int voff = 16 + (int) _.op_ival * 16;
    std::string s = IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                  + s_comment(emit_fmt("# BOX IR_VAR_FRAME_REF \"%s\" slot=%d hops=%d deref -> [r12+%d]", _.op_sval ? _.op_sval : "", (int) _.op_ival, hops, _.op_off)));
    s += x86_frame_lea("rax", 0);
    for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
    s += x86_reg_disp32_load64("rax", "rax", voff + 8);
    s += x86_reg_disp32_load64("rcx", "rax", 0) + x86_frame_store64(_.op_off, "rcx");
    s += x86_reg_disp32_load64("rcx", "rax", 8) + x86_frame_store64(_.op_off + 8, "rcx");
    s += x86("jmp", PORT_GAMMA)
       + x86("def", PORT_BETA)
       + x86("jmp", PORT_OMEGA);
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var_frame_ref(IR_t * pBB) { bb_emit_x86(bb_var_frame_ref_str(pBB)); }
