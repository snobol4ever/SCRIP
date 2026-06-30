#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_pos() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", (_.op_sval && _.op_sval[0] == 'r') ? "IR_MATCH_RPOS" : "IR_MATCH_POS")
         + x86("label",   _.lbl_α)
         + ((_.op_sval && _.op_sval[0] == 'r')
              ? ( x86("mov", "ecx", "r15d")
                + x86("sub", "ecx", (long)(int)_.op_ival)
                + x86("cmp", "r14d", "ecx") )
              : ( x86("cmp", "r14d", (long)(int)_.op_ival) ))
         + x86("jne", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
