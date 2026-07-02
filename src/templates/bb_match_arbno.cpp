#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (!g_emit.bb_child_lbl || !g_emit.bb_child_lbl[0]) ? std::string()
         : [&](std::string base) { return x86("comment", "IR_MATCH_ARBNO")
         + x86("label",   _.lbl_α)
         + x86("mov", FR(_.x86_scratch_off), "r14d")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("mov", FR(_.x86_scratch_off + 4), "r14d")
         + x86("jmp", base + "_α")
         + x86("label", base + "_wγ")
         + x86("mov", "eax", FR(_.x86_scratch_off + 4))
         + x86("cmp", "r14d", "eax")
         + x86("je", "L0")
         + x86("jmp", "γ")
         + x86("label", base + "_wω")
         + x86("def", "L0")
         + x86("mov", "r14d", FR(_.x86_scratch_off))
         + x86("jmp", "ω"); }
           (std::string(g_emit.bb_child_lbl).size() > 3 ? std::string(g_emit.bb_child_lbl).substr(0, std::string(g_emit.bb_child_lbl).size() - 3) : std::string(g_emit.bb_child_lbl));
}
