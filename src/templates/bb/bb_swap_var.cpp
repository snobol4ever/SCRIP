#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_swap_var(DESCR_t va, DESCR_t vb);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_swap_var() {
        return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0) ? x86_alpha() + x86_bomb("bb_swap_var: needs own slot + two variable operand slots") :
               x86("comment", "IR_SWAP_VAR x:=:y through variables (canonical swap, oasgn.r:265)")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("call",    "rt_swap_var", (uint64_t)(uintptr_t)(void *)rt_swap_var)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
}
