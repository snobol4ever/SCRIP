#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_pos() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_POS")
         + x86_alpha()
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp", "r14d", "eax")
         + x86_omega("jne")
         + x86_gamma()
         + x86_beta_trampoline();
}
