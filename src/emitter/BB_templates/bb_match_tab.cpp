#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long tabN() { return (long)(int)_.op_ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_tab() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "# BOX TAB()  [REG-3 δ=r14, x86() self-encoding]"))
             + x86("cmp",   "r14d", tabN())
             + x86("jg",    "ω")
             + x86("mov32", "r14d", tabN())
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    }
    return std::string();
}