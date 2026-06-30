#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_local() {
    if (PLATFORM_X86) return IF(!(_.op_sb >= 0 && _.op_off >= 0 && (_.op_a_slot >= 0 || _.op_a_node_kind == (int)IR_OP_COUNT)), x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot"))
                           + IF(_.op_sb >= 0 && _.op_off >= 0 && _.op_a_node_kind == (int)IR_OP_COUNT,
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_ASSIGN local=NUL")
                           + x86("mov", FRQ(_.op_sb), "0")
                           + x86("mov", FRQ(_.op_sb + 8), "0")
                           + x86("mov", FRQ(_.op_off), "0")
                           + x86("mov", FRQ(_.op_off + 8), "0")
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω"))
                           + IF(_.op_sb >= 0 && _.op_off >= 0 && _.op_a_slot >= 0,
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
