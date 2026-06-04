#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long tabN()    { return (long)(int)_.op_ival; }
static inline int  is_rtab() { return _.op_sval && _.op_sval[0] == 'r'; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_tab_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", is_rtab() ? "# BOX RTAB()  [REG-3 δ=r14 Δ=r15, x86() self-encoding]"
                                       : "# BOX TAB()  [REG-3 δ=r14, x86() self-encoding]"))
             + (is_rtab()
                  ? ( x86("mov",   "ecx", "r15d")
                    + x86("sub",   "ecx", tabN())
                    + x86("cmp",   "r14d", "ecx")
                    + x86("jg",    PORT_OMEGA)
                    + x86("mov",   "r14d", "ecx") )
                  : ( x86("cmp",   "r14d", tabN())
                    + x86("jg",    PORT_OMEGA)
                    + x86("mov32", "r14d", tabN()) ))
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_tab(void) { bb_emit_x86(bb_pat_tab_str()); }
