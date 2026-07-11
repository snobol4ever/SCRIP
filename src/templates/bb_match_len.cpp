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
         + IF(_.op_sa >= 0, x86("mov", "rcx", FRQ(_.op_sa + 8)))
         + x86("mov", "eax", "r14d")
         + IF(_.op_sa >= 0, x86("add", "eax", "ecx"))
         + IF(_.op_sa <  0, x86("add", "eax", (long)(int)_.op_ival))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + IF(_.op_sa >= 0, x86("add", "r14d", "ecx"))
         + IF(_.op_sa <  0, x86("add", "r14d", (long)(int)_.op_ival))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sa >= 0,
             x86("mov", "rcx", FRQ(_.op_sa + 8))
           + x86("sub", "r14d", "ecx"))
         + IF(_.op_sa <  0, x86("sub", "r14d", (long)(int)_.op_ival))
         + x86_omega();
}
