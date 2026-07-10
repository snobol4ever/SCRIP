#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subscript() {
    if (PLATFORM_X86)
        return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0)
             ? x86_alpha() + x86_bomb("bb_subscript: needs own slot + base/index operand slots")
             : x86("comment", "IR_SUBSCRIPT x[i] variable")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("call",    "rt_subscript_var", (uint64_t)(uintptr_t)(void *)rt_subscript_var)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return std::string();
}
