#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rev_assign() {
    return IF(!(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0), x86_bomb("bb_rev_assign: x<-v needs varslot + rhs slot + save slot + own slot"))
                           + IF(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0,
                             x86_alpha()
                           + x86("comment", "IR_REV_ASSIGN x<-v")
                           + x86("mov", "rax", FRQ(_.op_sb))
                           + x86("mov", "rdx", FRQ(_.op_sb + 8))
                           + x86("mov", FRQ(_.op_sc), "rax")
                           + x86("mov", FRQ(_.op_sc + 8), "rdx")
                           + x86("mov", "rcx", FRQ(_.op_a_slot))
                           + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
                           + x86("mov", FRQ(_.op_sb), "rcx")
                           + x86("mov", FRQ(_.op_sb + 8), "rsi")
                           + x86("mov", FRQ(_.op_off), "rcx")
                           + x86("mov", FRQ(_.op_off + 8), "rsi")
                           + x86_gamma()
                           + x86_beta()
                           + x86("mov", "rax", FRQ(_.op_sc))
                           + x86("mov", "rdx", FRQ(_.op_sc + 8))
                           + x86("mov", FRQ(_.op_sb), "rax")
                           + x86("mov", FRQ(_.op_sb + 8), "rdx")
                           + x86_omega());
}
