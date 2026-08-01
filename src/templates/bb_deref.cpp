#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_deref(DESCR_t d);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_deref() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_DEREF variable -> value zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
             + x86("call",    "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-2j: ONE operand (the :1563 mapping operands[0] -> op_a_slot -> rdi/rsi), by value at its staged difference; result to the box's own cell.  This is SNOBOL4 indirect reference, the $ operator (manual Ch.7 "Indirect Reference", p.81): the operand's STRING VALUE names a variable and the box yields that variable's value, so the DT_FAIL omega is the name-not-resolvable edge and is preserved exactly as the legacy arm has it. */
    return IF(_.op_off < 0 || _.op_a_slot < 0, x86_alpha() + x86_bomb("bb_deref: needs own slot + variable operand slot"))
         + IF(!(_.op_off < 0 || _.op_a_slot < 0),
             x86("comment", "IR_DEREF variable -> value")
           + x86_alpha()
           + x86("mov",     "rdi", FRQ(_.op_a_slot))
           + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
           + x86("call",    "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
           + x86("cmp",     "eax", (long)DT_FAIL)
           + x86_omega("je")
           + x86("mov",     FRQ(_.op_off),     "rax")
           + x86("mov",     FRQ(_.op_off + 8), "rdx")
           + x86_gamma()
           + x86_beta_trampoline());
}
