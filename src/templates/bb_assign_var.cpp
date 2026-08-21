#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_var() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_ASSIGN_VAR zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov",     "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov",     "rcx", ZOPQ(1, 8))
             + x86("call",    "rt_assign_var", (uint64_t)(uintptr_t)(void *)rt_assign_var)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0, x86_alpha() + x86_bomb("bb_assign_var: needs own slot + variable/value operand slots"))
         + IF(_.op_off >= 0 && _.op_a_slot >= 0 && _.op_sa >= 0,
               x86("comment", "IR_ASSIGN_VAR")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("call",    "rt_assign_var", (uint64_t)(uintptr_t)(void *)rt_assign_var)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline());
}
