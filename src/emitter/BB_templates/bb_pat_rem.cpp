#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_rem_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX REM()  [REG-2 δ=r14 Δ=r15, x86() self-encoding]"))
             + x86("mov", "r14d", "r15d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_rem(void) { bb_emit_x86(bb_pat_rem_str()); }
