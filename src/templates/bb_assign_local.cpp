#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_local() {
    if (PLATFORM_X86) {
        if (_.op_zres)   /* ZK-2 (s212): cells-arm -- read rhs from ZOPQ(0,0/8) (predecessor's RSP cell, staged depth-diff), write to varslot FRQ(op_sb/op_sb+8) AND own cell ZRES(0/8). IR_ASSIGN is K=0 in zd_k (it is a SINK -- it writes the local's slot but yields no result cell of its own; ZRES writes the result FOR CALLERS in the ZD model, but here the variable slot IS the backing store). Actually: K=0 means we write the varslot only; op_zres is set because a downstream reader needs a cell. Per the ZD-2h comment, assign local writes the varslot. Both media: x86() handles both. ONE AUTHORITY: op_zres gate. SNOBOL4 watermark: invisible (no SN4 locals). */
            return x86("comment", "IR_ASSIGN local <- ZOPQ (ZK-2 cells arm)")
                 + x86_alpha()
                 + x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 0))
                 + x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8))
                 + x86("mov", FRQ(_.op_sb),     "rax")
                 + x86("mov", FRQ(_.op_sb + 8), "rdx")
                 + x86_gamma()
                 + x86_beta_trampoline();
        return IF(!(_.op_sb >= 0 && _.op_off >= 0 && (_.op_a_slot >= 0 || _.op_a_node_kind == (int)IR_OP_COUNT || _.op_a_node_kind < 0)),
                             x86_alpha()
                           + x86_bomb("bb_assign_local: needs descr flat-chain + rhs slot + varslot + own slot")
                           + x86_beta_trampoline())
                           + IF(_.op_sb >= 0 && _.op_off >= 0 && (_.op_a_node_kind == (int)IR_OP_COUNT || _.op_a_node_kind < 0),
                             x86_alpha()
                           + x86("comment", "IR_ASSIGN local=NUL")
                           + x86("mov", FRQ(_.op_sb), "0")
                           + x86("mov", FRQ(_.op_sb + 8), "0")
                           + IF(_.op_res_live, x86("mov", FRQ(_.op_off), "0")
                                             + x86("mov", FRQ(_.op_off + 8), "0"))
                           + x86_gamma()
                           + x86_beta_trampoline())
                           + IF(_.op_sb >= 0 && _.op_off >= 0 && _.op_a_slot >= 0 && _.op_a_node_kind != (int)IR_OP_COUNT && _.op_a_node_kind >= 0,
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
    return std::string();
}
