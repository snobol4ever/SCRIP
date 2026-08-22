#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_deref(DESCR_t d);
extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rev_assign_var() {
    return !PLATFORM_X86 ? std::string()
         : (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sc < 0) ? x86_alpha() + x86_bomb("bb_rev_assign_var: needs own slot + variable/value operand slots + save slot")
         : x86("comment", "IR_REV_ASSIGN_VAR")
         + x86_alpha()
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("call",    "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
         + x86("mov",     FRQ(_.op_sc),     "rax")
         + x86("mov",     FRQ(_.op_sc + 8), "rdx")
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rdx", FRQ(_.op_sa))
         + x86("mov",     "rcx", FRQ(_.op_sa + 8))
         + x86("call",    "rt_assign_var", (uint64_t)(uintptr_t)(void *)rt_assign_var)
         + x86("cmp",     "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rdx", FRQ(_.op_sc))
         + x86("mov",     "rcx", FRQ(_.op_sc + 8))
         + x86("call",    "rt_assign_var", (uint64_t)(uintptr_t)(void *)rt_assign_var)
         + x86_omega();
}
