#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arb() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_ARB")
         + x86_alpha()
         + x86("mov",    FR(_.x86_scratch_off), (long)0)
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86_gamma()
         + x86_beta()
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jg",     L(0))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86("def",    L(0))
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
