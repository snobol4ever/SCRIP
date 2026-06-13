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
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!g_emit.bb_child_lbl || !g_emit.bb_child_lbl[0]) return std::string();
    std::string base(g_emit.bb_child_lbl);
    if (base.size() > 3) base = base.substr(0, base.size() - 3);
    return x86("comment", "BOX ARBNO() [BROK-2 wired generator, shy per SPITBOL pp.121/212: null first, each retry adds one instance; child entered jmp child_α, child returns jmp _wγ/_wω; δ=r14d, saved/prev in ζ-frame]")
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
         + x86("jmp", "ω");
}
