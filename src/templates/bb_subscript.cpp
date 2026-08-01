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
    if (PLATFORM_X86) {
        if (_.op_zres)
            return x86("comment", "IR_SUBSCRIPT x[i] variable zd")
                 + x86_alpha()
                 + x86("mov",     "rdi", ZOPQ(0, 0))
                 + x86("mov",     "rsi", ZOPQ(0, 8))
                 + x86("mov",     "rdx", ZOPQ(1, 0))
                 + x86("mov",     "rcx", ZOPQ(1, 8))
                 + x86("call",    "rt_subscript_var", (uint64_t)(uintptr_t)(void *)rt_subscript_var)
                 + x86("cmp",     "eax", (long)DT_FAIL)
                 + x86_omega("je")
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
                 + x86_gamma()
                 + x86_beta_trampoline();   /* ZD-2f: operand 0 is the base aggregate and operand 1 the index (the :1537 mapping vb->op_a_slot, vi->op_sa -- same register order as the legacy arm), both by VALUE at staged differences; result to the box's own cell.  The DT_FAIL omega is GENUINE and load-bearing, not defensive: an out-of-bounds subscript FAILS rather than erroring, and SPITBOL documents looping until an array reference fails as the idiomatic traversal (manual Ch.7, arrays) -- so this edge is a language contract and must never be optimized into an error path.  ⛔ ARMED ONLY FOR n_operands==2: the dispatch at emit.cpp:992 routes 2-operand IR_SUBSCRIPT here and everything else to bb_section, which has NO ZD arm. */
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
             + x86_beta_trampoline();
    }
    return std::string();
}
