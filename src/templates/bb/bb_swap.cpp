#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_swap() {
    return IF(!(_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0), x86_bomb("bb_swap: x:=:y needs both var slots + own slot"))
                           + IF(_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0,
                             x86_alpha()
                           + x86("comment", "IR_SWAP x:=:y")
                           + x86("mov", "rax", FRQ(_.op_sa))
                           + x86("mov", "rdx", FRQ(_.op_sa + 8))
                           + x86("mov", "rcx", FRQ(_.op_sb))
                           + x86("mov", "rsi", FRQ(_.op_sb + 8))
                           + x86("mov", FRQ(_.op_sa), "rcx")
                           + x86("mov", FRQ(_.op_sa + 8), "rsi")
                           + x86("mov", FRQ(_.op_sb), "rax")
                           + x86("mov", FRQ(_.op_sb + 8), "rdx")
                           + x86("mov", FRQ(_.op_off), "rcx")
                           + x86("mov", FRQ(_.op_off + 8), "rsi")
                           + x86_gamma()
                           + x86_beta_trampoline());
}
