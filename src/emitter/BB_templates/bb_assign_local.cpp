#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_local_str() {
    if (!PLATFORM_X86) return std::string();
    if (g_descr_flat_chain && _.op_sb >= 0 && _.op_off >= 0 && _.op_a_slot >= 0)
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX IR_ASSIGN local(\"%s\") [descr flat-chain: rhs slot %d -> varslot %d + own slot %d]", _.op_sval ? _.op_sval : "", _.op_a_slot, _.op_sb, _.op_off)))
             + x86_frame_load64("rax", _.op_a_slot)
             + x86_frame_load64("rdx", _.op_a_slot + 8)
             + x86_frame_store64(_.op_sb, "rax")
             + x86_frame_store64(_.op_sb + 8, "rdx")
             + x86_frame_store64(_.op_off, "rax")
             + x86_frame_store64(_.op_off + 8, "rdx")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    return x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_local(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_assign_local_str()); }
