#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_str() {
    if (!PLATFORM_X86) return std::string();
    if (g_gvar_flat_chain)
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX IR_VAR \"%s\" [SNO flat-chain: by-name pass-through]", _.op_sval ? _.op_sval : "")))
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    if (g_icn_flat_chain && _.op_off >= 0 && _.op_sa >= 0)
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX IR_VAR read(\"%s\") [GZ-7 ICN flat-chain: var slot %d -> own slot %d]", _.op_sval ? _.op_sval : "", _.op_sa, _.op_off)))
             + x86_frame_load64("rax", _.op_sa)
             + x86_frame_store64(_.op_off, "rax")
             + x86_frame_load64("rax", _.op_sa + 8)
             + x86_frame_store64(_.op_off + 8, "rax")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    return x86_bomb("bb_var: unhandled arm (not sno/icn flat-chain or missing slot)");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_var_str()); }
