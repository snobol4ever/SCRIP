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
    if (_.op_zres)
        return x86("comment", "IR_DEREF variable -> value zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
             + x86("call",    "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_off < 0 || _.op_a_slot < 0, x86_alpha() + x86_bomb("bb_deref: needs own slot + variable operand slot"))
         + IF(!(_.op_off < 0 || _.op_a_slot < 0),
             x86("comment", "IR_DEREF variable -> value")
           + x86_alpha()
           + x86("mov",     "rdi", FRQ(_.op_a_slot))
           + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
           + x86("call",    "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
           + x86("cmp",     "al", (long)DT_FAIL)
           + x86_omega("je")
           + x86("mov",     FRQ(_.op_off),     "rax")
           + x86("mov",     FRQ(_.op_off + 8), "rdx")
           + x86_gamma()
           + x86_beta_trampoline());
}
