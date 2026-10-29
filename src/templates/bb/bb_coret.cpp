#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coret() {
    x86_begin();
    return IF(_.op_sa < 0, x86_alpha() + x86_bomb("bb_coret: no produced-value slot (coret.operand[0] has no DESCR slot -- body value node missing from chain)"))
         + IF(!(_.op_sa < 0),
             x86("comment", "IR_CORET yield")
           + x86_alpha()
           + x86("call", "rt_co_env", (uint64_t)(uintptr_t)(void *)rt_co_env)
           + x86("mov", "[rax + 0]", "r13")
           + x86("mov", "[rax + 8]", "r14")
           + x86("mov", "[rax + 16]", "r15")
           + x86("mov", "rdi", FRQ(_.op_sa))
           + x86("mov", "rsi", FRQ(_.op_sa + 8))
           + x86("xor", "edx", "edx")
           + x86("call", "scrip_coret", (uint64_t)(uintptr_t)(void *)scrip_coret)
           + x86("mov", "r13", "[rax + 0]")
           + x86("mov", "r14", "[rax + 8]")
           + x86("mov", "r15", "[rax + 16]")
           + x86("mov", RDQ("rax", 0), 0L)
           + x86("mov", RDQ("rax", 8), 0L)
           + x86("mov", RDQ("rax", 16), 0L)
           + x86_rt_gc_poll()
           + x86_gamma());
}
