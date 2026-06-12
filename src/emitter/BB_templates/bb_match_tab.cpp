#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_tab() {
    if (!PLATFORM_X86) return std::string();
    long n = (long)(int)_.op_ival;
    return x86("comment", "IR_MATCH_TAB")
         + x86("label",   _.lbl_α)
         + x86("cmp",   "r14d", n)
         + x86("jg",    "ω")
         + x86("mov32", "r14d", n)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
