/* bb_atom.cpp — BB template for IR_ATOM: Prolog atom literal.
   PL-T-2 (GOAL-BB-TEMPLATE-LADDER.md). x86 only per Invariant #14 (2026-05-22).
   STACKLESS (2026-05-31, Lon directive): the former body pushed the atom string onto the global value
   stack via rt_pl_atom_push -> rt_push_str -> vstack_push(g_vstack) — exactly the value-stack traffic the
   PER-BOX LOCAL STORAGE FACT RULE bans. rt_pl_atom_push is DELETED. An executed IR_ATOM leaf has no
   per-box runtime work: when an atom's value matters it is a READ-ONLY operand CONSTANT read directly by
   its consumer (bb_unify / bb_builtin-write via rt_pl_node_to_term / rt_pl_write_atom — the test_sno_1.c
   "consumer reads the producer's sealed constant" model), never pushed. Empirically RESOLVE_ATOM fires
   zero times across all live mode-3/mode-4 paths. So the box is now the minimal four-port pass-through
   (mirrors bb_lit_scalar's scalar-literal pass-through): α -> γ, β -> ω. No bytes touch g_vstack. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_atom_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *atom = pBB->sval ? pBB->sval : "";
        bin = bb_bin_t{ {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — RESOLVE_ATOM"))
             + IF(MEDIUM_BINARY,
                   bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                   + s_comment(emit_fmt("# BOX RESOLVE_ATOM('%s') [stackless pass-through]", atom))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_atom(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_atom_str(pBB, bin), bin);
}

