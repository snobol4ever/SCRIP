#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int sdoff() { return _.x86_scratch_off; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX FENCE()  [REG-3 δ=r14, ζ-frame saved_δ, x86() self-encoding]"))
             + x86("mov", FR(sdoff()), "r14d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("mov", "r14d", FR(sdoff()))
             + x86("jmp", "ω");
    }
    return std::string();
}