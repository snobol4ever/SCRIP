#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_tab() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_TAB")
         + x86("def",     "α")
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp",   "r14d", "eax")
         + x86("jg",    "ω")
         + x86("cmp",   "r15d", "eax")
         + x86("jl",    "ω")
         + x86("mov",   "r14d", "eax")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
