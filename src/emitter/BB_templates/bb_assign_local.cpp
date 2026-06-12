#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int al_ok() { return g_descr_flat_chain && _.op_sb >= 0 && _.op_off >= 0 && _.op_a_slot >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_local() {
    if (PLATFORM_X86) return IF(!al_ok(), x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot"))
                           + IF(al_ok(),
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_ASSIGN local")
                           + x86("mov", "rax", FRQ(_.op_a_slot))
                           + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                           + x86("mov", FRQ(_.op_sb), "rax")
                           + x86("mov", FRQ(_.op_sb + 8), "rdx")
                           + x86("mov", FRQ(_.op_off), "rax")
                           + x86("mov", FRQ(_.op_off + 8), "rdx")
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω"));
    return std::string();
}