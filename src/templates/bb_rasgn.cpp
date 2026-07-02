#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_REV_ASSIGN — Icon reversible assignment through a NAMED variable `x <- v` (canonical oasgn.r rasgn: GeneralAsgn(x,v); suspend x; on resume GeneralAsgn(x,saved); fail).  ONE shape: the value
   lives in a frame slot; save old (op_sb->op_sc), write new (op_a_slot->op_sb), yield, restore on β.  The SUBSCRIPT lvalue form `x[i] <- v` is IR_REV_ASSIGN_VAR (bb_rev_assign_var.cpp) — the
   IDX-UNIFY through-variable sibling, exactly as IR_ASSIGN_VAR sibling-of-IR_ASSIGN; the DORMANT pre-variable-doctrine subscript arm formerly here (LOWER never produced its selector) is deleted. */
std::string bb_rasgn() {
    if (!PLATFORM_X86) return std::string();
    return IF(!(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0), x86_bomb("bb_rasgn: x<-v needs varslot + rhs slot + save slot + own slot"))
                           + IF(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0,
                             x86("label", _.lbl_α)
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
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("mov", "rax", FRQ(_.op_sc))
                           + x86("mov", "rdx", FRQ(_.op_sc + 8))
                           + x86("mov", FRQ(_.op_sb), "rax")
                           + x86("mov", FRQ(_.op_sb + 8), "rdx")
                           + x86("jmp", "ω"));
}
