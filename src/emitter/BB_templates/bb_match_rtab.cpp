#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rtab() {
    if (!PLATFORM_X86) return std::string();
    long n = (long)(int)_.op_ival;
    return x86("comment", "IR_MATCH_RTAB")
         + x86("label",   _.lbl_α)
         + x86("mov",  "ecx", "r15d")
         + x86("sub",  "ecx", n)
         + x86("cmp",  "r14d", "ecx")
         + x86("jg",   "ω")
         + x86("mov",  "r14d", "ecx")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
