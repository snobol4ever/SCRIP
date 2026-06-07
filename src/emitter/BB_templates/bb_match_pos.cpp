#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long posN()    { return (long)(int)_.op_ival; }
static inline int  is_rpos() { return _.op_sval && _.op_sval[0] == 'r'; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_pos_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", is_rpos() ? "BOX RPOS()  [Σ=r13 δ=r14 Δ=r15, x86() self-encoding]"
                                                        : "BOX POS()  [Σ=r13 δ=r14 Δ=r15, x86() self-encoding]"))
             + (is_rpos()
                  ? ( x86("mov", "ecx", "r15d")
                    + x86("sub", "ecx", posN())
                    + x86("cmp", "r14d", "ecx") )
                  : ( x86("cmp", "r14d", posN()) ))
             + x86("jne", "\xCF\x89")
             + x86("jmp", "\xCE\xB3")
             + x86("def", "\xCE\xB2")
             + x86("jmp", "\xCF\x89");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_pos(void) { bb_emit_x86(bb_match_pos_str()); }
