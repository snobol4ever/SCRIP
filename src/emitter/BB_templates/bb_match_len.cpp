#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_len() {
    if (!PLATFORM_X86) return std::string();
    long n = (long)(int)_.op_ival;
    return x86("comment", "IR_MATCH_LEN")
         + x86("label",   _.lbl_α)
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", n)
         + x86("cmp", "eax", "r15d")
         + x86("jg",  "ω")
         + x86("add", "r14d", n)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
