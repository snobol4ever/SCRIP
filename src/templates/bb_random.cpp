#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_random_var(DESCR_t base);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_random() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0 || _.op_a_slot < 0) return x86("def", "α") + x86_bomb("bb_random: needs own slot + base operand slot");
    return x86("comment", "IR_RANDOM ?x -> element variable / value (canonical LCG, oref.r random)")
         + x86("def",     "α")
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("call",    "rt_random_var", (uint64_t)(uintptr_t)(void *)rt_random_var)
         + x86("cmp",     "eax", (long)DT_FAIL)
         + x86("je",  "ω")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
