/* bb_cut.cpp — BB template for IR_CUT: Prolog cut (!).
   AGW-9 (GOAL-PROLOG-BB.md). x86 only per Invariant #14 / RULES.md "X86 ONLY FOR NOW".
   WAM-CP-9 (2026-05-28, Opus 4.7): in mode-4 IR_CUT sets `g_resolve_cut_flag` only and does NOT call
   resolve_cp_truncate.  The actual CP-chain truncate is deferred to the enclosing IR_CHOICE template
   (bb_choice.cpp), which checks the flag at β / exit_γ entries and calls
   rt_pl_choice_cut_unwind(cp) — restoring the outer cut state from cp->saved_cut_{flag,barrier}
   AND truncating to cp->parent in one step.  The defer is required because if CUT truncated
   eagerly, the CHOICE's own CP record would be freed before the body returns to dispatch and the
   saved-state slots inside it would be unreadable.  Mode-2's eager-truncate behavior is unchanged
   in bb_exec.c — it keeps saved_cut/saved_barrier on the C stack instead. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" void rt_pl_cut_set(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cut_str(IR_t * pBB, bb_bin_t & bin) {
    (void)pBB; bin = {};
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_CUT");
        if (MEDIUM_BINARY) {
            std::string b = bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_cut_set) + bytes(2, "\xFF\xD0");
            int j = (int)b.size();
            bin = { {j + 1, j + 5, j + 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_γ_p}, {false, true, false} };
            return b + bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX RESOLVE_CUT")
                 + s_2asm("call", "rt_pl_cut_set@PLT")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cut(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_cut_str(pBB, bin), bin);
}
