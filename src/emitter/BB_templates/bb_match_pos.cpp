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
    long n = (long)(int)_.op_ival;
    int is_rpos = _.op_sval && _.op_sval[0] == 'r';
    return x86("comment", is_rpos ? "IR_MATCH_RPOS" : "IR_MATCH_POS")
         + x86("label",   _.lbl_α)
         + (is_rpos
              ? ( x86("mov", "ecx", "r15d")
                + x86("sub", "ecx", n)
                + x86("cmp", "r14d", "ecx") )
              : ( x86("cmp", "r14d", n) ))
         + x86("jne", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
