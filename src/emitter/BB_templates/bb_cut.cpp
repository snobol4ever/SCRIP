/* bb_cut.cpp — BB template for IR_CUT: Prolog cut (!). x86() self-encoding (template-revamp PL-RV-1, 2026-06-02, Opus 4.8).
   WAM-CP-9 (2026-05-28, Opus 4.7): in mode-4 IR_CUT sets g_resolve_cut_flag only (via rt_pl_cut_set) and does NOT call
   resolve_cp_truncate. The actual CP-chain truncate is deferred to the enclosing IR_CHOICE template (bb_choice.cpp), which
   checks the flag at β / exit_γ entries and calls rt_pl_choice_cut_unwind(cp) — restoring the outer cut state from
   cp->saved_cut_{flag,barrier} AND truncating to cp->parent in one step. The defer is required because if CUT truncated
   eagerly, the CHOICE's own CP record would be freed before the body returns to dispatch and the saved-state slots inside it
   would be unreadable. Mode-2's eager-truncate behavior is unchanged in bb_exec.c — it keeps saved_cut/saved_barrier on the C
   stack instead. Ports: α calls rt_pl_cut_set then falls to γ (success); β also goes to γ (cut already committed, a redo
   just re-succeeds the barrier). NO operands → no g_emit promotions read; reads only _ (labels/ports). x86 arm: ONE return,
   pure x86() concat, NO bb_bin_t, medium invisible (the deleted MEDIUM_BINARY twin was dead for Prolog — mode-3 routes the
   oracle, mode-4 uses TEXT; verified zero BINARY-arm fires across the rung suite). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void rt_pl_cut_set(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cut_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":") + s_comment("# BOX RESOLVE_CUT  [x86() self-encoding]"))
             + x86("call", "rt_pl_cut_set", (uint64_t)(uintptr_t)(void*)rt_pl_cut_set)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_GAMMA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cut(void) { bb_emit_x86(bb_cut_str()); }
