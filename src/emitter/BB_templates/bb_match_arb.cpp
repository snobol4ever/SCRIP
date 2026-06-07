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
static inline int zoff()  { return _.x86_scratch_off; }
static inline int zooff() { return _.x86_scratch_off + 4; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arb_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX ARB()  [REG-4 Σ=r13 δ=r14 Δ=r15, ζ-frame z/zo, x86() self-encoding]"))
             + x86("mov",    FR(zoff()), (long)0)
             + x86("mov",    "eax", "r14d")
             + x86("mov",    FR(zooff()), "eax")
             + x86("jmp",    "\xCE\xB3")
             + x86("def",    "\xCE\xB2")
             + x86("add",    FR(zoff()), (long)1)
             + x86("mov",    "eax", FR(zooff()))
             + x86("add",    "eax", FR(zoff()))
             + x86("cmp",    "eax", "r15d")
             + x86("jg",     "\xCF\x89")
             + x86("mov",    "r14d", "eax")
             + x86("jmp",    "\xCE\xB3");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_arb(void) {
    _.x86_scratch_off = bb_slot_claim(8);
    bb_emit_x86(bb_match_arb_str());
}
