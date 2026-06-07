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
static int g_saved_off, g_prev_off;
static inline int an_ok() { return g_emit.bb_child_lbl && g_emit.bb_child_lbl[0]; }
static inline std::string an_base() { std::string s(g_emit.bb_child_lbl ? g_emit.bb_child_lbl : ""); return s.size() > 3 ? s.substr(0, s.size() - 3) : s; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_str() {
    return IF(PLATFORM_X86 && MEDIUM_TEXT && an_ok(),
           x86("label", _.lbl_α)
         + x86("comment", "BOX ARBNO() [BROK-2 wired generator, shy per SPITBOL pp.121/212: null first, each retry adds one instance; child entered jmp child_α, child returns jmp _wγ/_wω; δ=r14d, saved/prev in ζ-frame]")
         + x86("mov", FR(g_saved_off), "r14d")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("mov", FR(g_prev_off), "r14d")
         + x86("jmp", an_base() + "_α")
         + x86("label", an_base() + "_wγ")
         + x86("mov", "eax", FR(g_prev_off))
         + x86("cmp", "r14d", "eax")
         + x86("je", "L0")
         + x86("jmp", "γ")
         + x86("label", an_base() + "_wω")
         + x86("def", "L0")
         + x86("mov", "r14d", FR(g_saved_off))
         + x86("jmp", "ω"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_arbno(IR_t *pBB) {
    (void)pBB; x86_begin();
    g_saved_off = bb_slot_claim(4);
    g_prev_off  = bb_slot_claim(4);
    bb_emit_x86(IF(!an_ok(), x86_bomb("bb_match_arbno: no child label"))
              + IF(an_ok() && MEDIUM_BINARY, x86_bomb("bb_match_arbno: BINARY arm not yet implemented"))
              + bb_match_arbno_str());
}
