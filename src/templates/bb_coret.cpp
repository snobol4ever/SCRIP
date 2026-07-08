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
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0) return x86_bomb("bb_coret: no produced-value slot (coret.operand[0] has no DESCR slot -- body value node missing from chain)");
    return x86("comment", "IR_CORET yield")
         + x86("def",     "α")
         + x86("mov",  "rdi", FRQ(_.op_sa))
         + x86("mov",  "rsi", FRQ(_.op_sa + 8))
         + x86("xor",  "edx", "edx")
         + x86("call", "scrip_coret", (uint64_t)(uintptr_t)(void *)scrip_coret)
         + x86("jmp",  "γ");
}
