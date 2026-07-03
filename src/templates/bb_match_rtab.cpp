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
         + x86("label",   _.lbl_α)
         + x86("mov",  "ecx", "r15d")
         + x86("sub",  "ecx", (long)(int)_.op_ival)
         + x86("cmp",  "r14d", "ecx")
         + x86("jg",   "ω")
         + x86("mov",  "r14d", "ecx")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
