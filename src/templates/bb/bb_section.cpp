#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
extern DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end);
extern DESCR_t rt_section_var(DESCR_t base, DESCR_t i1, DESCR_t i2);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_section() {
        return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sb < 0)
             ? x86_alpha() + x86_bomb("bb_section: needs own slot + base/i1/i2 operand slots")
             : (_.op_sval && _.op_sval[0] == 'l' && _.op_sval[1] == 'v' && !_.op_sval[2])
             ? x86("comment", "IR_SUBSCRIPT section VARIABLE (ASSIGN-LV: rt_section_var tvsubs trap; FAIL routes ω)")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("mov",     "r8",  FRQ(_.op_sb))
             + x86("mov",     "r9",  FRQ(_.op_sb + 8))
             + x86("call",    "rt_section_var", (uint64_t)(uintptr_t)(void *)rt_section_var)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             : (_.op_sval && (_.op_sval[0] == '+' || _.op_sval[0] == '-'))
             ? x86("comment", "IR_SUBSCRIPT section extended (x[i+:n]/x[i-:n]; end pre-computed by IR_BINOP; wraparound→ω)")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("mov",     "r8",  FRQ(_.op_sb))
             + x86("mov",     "r9",  FRQ(_.op_sb + 8))
             + x86("call",    "subscript_get2_ext", (uint64_t)(uintptr_t)(void *)subscript_get2_ext)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             : x86("comment", "IR_SUBSCRIPT section")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("mov",     "r8",  FRQ(_.op_sb))
             + x86("mov",     "r9",  FRQ(_.op_sb + 8))
             + x86("call",    "subscript_get2", (uint64_t)(uintptr_t)(void *)subscript_get2)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
}
