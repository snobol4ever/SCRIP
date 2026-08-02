#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_tab() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_MATCH_TAB zd")
             + x86_alpha()
             + x86("mov",   FR(_.x86_scratch_off), "r14d")
             + x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 8))
             + x86("cmp",   "r14d", "eax")
             + x86_omega("jg")
             + x86("cmp",   "r15d", "eax")
             + x86_omega("jl")
             + x86("mov",   "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",   "r14d", FR(_.x86_scratch_off))
             + x86_omega();
    return x86("comment", "IR_MATCH_TAB")
         + x86_alpha()
         + x86("mov",   FR(_.x86_scratch_off), "r14d")
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp",   "r14d", "eax")
         + x86_omega("jg")
         + x86("cmp",   "r15d", "eax")
         + x86_omega("jl")
         + x86("mov",   "r14d", "eax")
         + x86_gamma()
         + x86_beta()
         + x86("mov",   "r14d", FR(_.x86_scratch_off))
         + x86_omega();
}
