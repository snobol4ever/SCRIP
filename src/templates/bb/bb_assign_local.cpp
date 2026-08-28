#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_local() {
    if (_.op_zres)
        return x86("comment", "IR_ASSIGN local <- ZOPQ (ZK-2 cells arm)")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rax", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rdx", ZOPQ(0, 8))
             + x86("mov", FRQ(_.op_sb),     "rax")
             + x86("mov", FRQ(_.op_sb + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(!(_.op_sb != -1 && _.op_off >= 0 && (_.op_a_slot >= 0 || _.op_a_node_kind == (int)IR_OP_COUNT || _.op_a_node_kind < 0)),
                         x86_alpha()
                       + x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot")
                       + x86_beta_trampoline())
                       + IF(_.op_sb != -1 && _.op_off >= 0 && (_.op_a_node_kind == (int)IR_OP_COUNT || _.op_a_node_kind < 0),
                         x86_alpha()
                       + x86("comment", "IR_ASSIGN local=NUL")
                       + x86("mov", FRQ(_.op_sb), "0")
                       + x86("mov", FRQ(_.op_sb + 8), "0")
                       + IF(_.op_res_live, x86("mov", FRQ(_.op_off), "0")
                                         + x86("mov", FRQ(_.op_off + 8), "0"))
                       + x86_gamma()
                       + x86_beta_trampoline())
                       + IF(_.op_sb != -1 && _.op_off >= 0 && _.op_a_slot >= 0 && _.op_a_node_kind != (int)IR_OP_COUNT && _.op_a_node_kind >= 0,
                         x86_alpha()
                       + x86("comment", "IR_ASSIGN local")
                       + x86("mov", "rax", FRQ(_.op_a_slot))
                       + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                       + x86("mov", FRQ(_.op_sb), "rax")
                       + x86("mov", FRQ(_.op_sb + 8), "rdx")
                       + IF(_.op_res_live, x86("mov", FRQ(_.op_off), "rax")
                                         + x86("mov", FRQ(_.op_off + 8), "rdx"))
                       + x86_gamma()
                       + x86_beta_trampoline());
}
