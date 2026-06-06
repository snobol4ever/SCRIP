#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int al_ok() { return g_descr_flat_chain && _.op_sb >= 0 && _.op_off >= 0 && _.op_a_slot >= 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_local_str() {
    return IF(PLATFORM_X86,
           IF(al_ok(),
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_ASSIGN local(\"") + (_.op_sval ? _.op_sval : "") + "\") [descr flat-chain: rhs slot "
                            + std::to_string(_.op_a_slot) + " -> varslot " + std::to_string(_.op_sb) + " + own slot " + std::to_string(_.op_off) + "]"))
            + x86_frame_load64("rax", _.op_a_slot)
            + x86_frame_load64("rdx", _.op_a_slot + 8)
            + x86_frame_store64(_.op_sb, "rax")
            + x86_frame_store64(_.op_sb + 8, "rdx")
            + x86_frame_store64(_.op_off, "rax")
            + x86_frame_store64(_.op_off + 8, "rdx")
            + x86("jmp", "γ")
            + x86("def", "β")
            + x86("jmp", "ω"))
         + IF(!al_ok(), x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot")));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_local(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_assign_local_str()); }
