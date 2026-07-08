#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rtab() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_RTAB")
         + x86("def",     "α")
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("mov",  "ecx", "r15d")
         + x86("sub",  "ecx", "eax")
         + x86("cmp",  "r14d", "ecx")
         + x86("jg",   "ω")
         + x86("mov",  "r14d", "ecx")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
