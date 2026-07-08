#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_len() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_LEN")
         + x86_alpha()
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", (long)(int)_.op_ival)
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + x86("add", "r14d", (long)(int)_.op_ival)
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
