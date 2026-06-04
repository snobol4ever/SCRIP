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
static std::string bb_pat_pos_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", is_rpos() ? "# BOX RPOS()  [REG-3 δ=r14 Δ=r15, x86() self-encoding]"
                                       : "# BOX POS()  [REG-3 δ=r14, x86() self-encoding]"))
             + (is_rpos()
                  ? ( x86("mov", "ecx", "r15d")
                    + x86("sub", "ecx", posN())
                    + x86("cmp", "r14d", "ecx") )
                  : ( x86("cmp", "r14d", posN()) ))
             + x86("jne", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_pos(void) { bb_emit_x86(bb_pat_pos_str()); }
