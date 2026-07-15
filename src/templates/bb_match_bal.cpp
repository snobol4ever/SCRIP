#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_bal() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_BAL")
         + x86_alpha()
         + x86("mov",    FR(_.x86_scratch_off),     (long)0)
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86("mov",    FR(_.x86_scratch_off + 8), (long)0)
         + x86_beta()
         + x86("def",    L(0))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(3))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("mov",    "edx", FR(_.x86_scratch_off + 8))
         + x86("cmp",    "esi", (long)40)
         + x86("jne",    L(1))
         + x86("add",    "edx", (long)1)
         + x86("jmp",    L(2))
         + x86("def",    L(1))
         + x86("cmp",    "esi", (long)41)
         + x86("jne",    L(2))
         + x86("sub",    "edx", (long)1)
         + x86("cmp",    "edx", (long)0)
         + x86("jl",     L(3))
         + x86("def",    L(2))
         + x86("mov",    FR(_.x86_scratch_off + 8), "edx")
         + x86("test",   "edx", "edx")
         + x86("jne",    L(0))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86("def",    L(3))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("mov",    "r14d", "eax")
         + x86_omega();
}
