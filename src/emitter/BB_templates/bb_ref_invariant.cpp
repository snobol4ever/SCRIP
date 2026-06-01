/* bb_ref_invariant.cpp — STACKLESS BB template for the SNOBOL4 pattern REF_INVARIANT box (IR_REF_INVARIANT).
   PB-RB-1 SCAFFOLDING (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01, Opus 4.8). This file is
   the LAYOUT/PLUMBING reserved for PB-RB-1; the real load-sealed-head logic + lowering land in PB-RB-1 proper.
   It is DORMANT: nothing lowers TT_QLIT (or anything else) to IR_REF_INVARIANT yet, so this stub is never
   reached at emit time and the build stays green / gates stay invariant.

   INTENDED ROLE (PB-RB-1): a pattern element that is INVARIANT (a literal, fixed LEN/POS, ALT/CAT of such) is
   emitted + port-wired at COMPILE time as a sealed `bb_box_fn` (the EXISTING IR_PAT_* matcher box — e.g.
   bb_lit.cpp for a literal). REF_INVARIANT references that sealed element: it loads the sealed `bb_box_fn`
   HEAD (RO `[rip+disp]` in the TEXT arm / movabs immediate in the BINARY arm — RO data, never on a stack)
   into a `ζ`-frame RW slot `[ζ=r12+off]` (PER-BOX LOCAL STORAGE FACT RULE). For a FULLY-invariant pattern,
   this one box's output IS the pattern's `DT_P` value (a `bb_box_fn` graph head) handed to BB_MATCH (PB-RB-3);
   no runtime construction runs (Fork A/E). It is the runtime READ of a sealed piece — distinct from
   STITCH_SEQ/STITCH_ALT (PB-RB-4), which WIRE instance records, and from BB_PAT_BUILD (PB-RB-6), which builds
   dynamic boxes for structural variance. NO PATND_t, NO tree_t, NO value stack, NO ring.

   Bounded single-shot: referencing a sealed head once is enough (β = jmp ω). BINARY + TEXT arms land in
   PB-RB-1 proper (model on bb_sno_subject.cpp / bb_sno_pat_build_lit.cpp: load operand -> store to [ζ+off]).
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ref_invariant_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_REF_INVARIANT");
    /* PB-RB-1 SCAFFOLDING: dormant placeholder. The sealed-head load + BINARY/TEXT arms land in PB-RB-1
       proper; until a lowerer emits IR_REF_INVARIANT this is unreachable. Fail loud if ever reached early. */
    if (MEDIUM_BINARY) {
        fprintf(stderr, "[SBB] FATAL bb_ref_invariant: PB-RB-1 not yet implemented (REF_INVARIANT sealed-head load)\n");
        abort();
    }
    return bomb_text("SNO IR_REF_INVARIANT: PB-RB-1 scaffolding — sealed-head load not yet implemented");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ref_invariant(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_ref_invariant_str(pBB, bin), bin); }
