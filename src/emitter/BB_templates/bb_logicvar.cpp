/* bb_logicvar.cpp — BB template for IR_LOGICVAR: Prolog variable slot.
   PL-T-2 (GOAL-BB-TEMPLATE-LADDER.md). x86 only per Invariant #14 (2026-05-22).
   STACKLESS (2026-05-31, Lon directive): the former body pushed the slot's value onto the global value
   stack via rt_pl_var_push -> rt_push_int/rt_push_str -> vstack_push(g_vstack) — the value-stack traffic
   the PER-BOX LOCAL STORAGE FACT RULE bans (GOAL-PROLOG-BB completion test forbids g_vstack and the
   rt_push / rt_pop value ops). rt_pl_var_push is DELETED, never to be resurrected. A logic variable's
   binding lives in its per-activation slot g_resolve_env[slot]; when its value matters it is read DIRECTLY
   by its consumer (bb_unify / bb_builtin-write / bb_arith via rt_pl_node_to_term / rt_pl_write_var /
   rt_pl_arith — the test_sno_1.c consumer-reads-the-producer-slot model), never pushed. Empirically
   RESOLVE_VAR fires zero times across all live mode-3/mode-4 paths. So the box is the minimal four-port
   pass-through: alpha -> gamma, beta -> omega. No bytes touch g_vstack. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_logicvar_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — RESOLVE_VAR"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_1asm(emit_fmt("# BOX RESOLVE_VAR(slot=%d) [stackless pass-through]", (int)pBB->ival))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_logicvar(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_logicvar_str(pBB, bin), bin);
}
