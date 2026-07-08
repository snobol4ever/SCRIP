#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_pos() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", (_.op_sval && _.op_sval[0] == 'r') ? "IR_MATCH_RPOS" : "IR_MATCH_POS")
         + x86("def",     "α")
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + ((_.op_sval && _.op_sval[0] == 'r')
              ? ( x86("mov", "ecx", "r15d")
                + x86("sub", "ecx", "eax")
                + x86("cmp", "r14d", "ecx") )
              : ( x86("cmp", "r14d", "eax") ))
         + x86("jne", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
