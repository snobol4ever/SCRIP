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
std::string bb_match_arb() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_ARB")
         + x86("label",   _.lbl_α)
         + x86("mov",    FR(_.x86_scratch_off), (long)0)
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jg",     "ω")
         + x86("mov",    "r14d", "eax")
         + x86("jmp",    "γ");
}
