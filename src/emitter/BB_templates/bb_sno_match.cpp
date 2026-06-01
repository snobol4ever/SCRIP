/* bb_sno_match.cpp — STACKLESS BB template for the SNOBOL4 pattern-match MATCH phase (IR_PAT_MATCH).
   PB-RB-3 (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01, Opus 4.8). Phase 3 of the five-phase
   `SUBJ ? PAT [= REPL]` native model: BB_MATCH DRIVES the pattern element graph over Σ/δ/Δ with the SPITBOL
   Manual ch.18 unanchored OUTER start-loop. INLINE-JUMP model (Lon directive 2026-06-01, verbatim: "BB_MATCH
   would jump in and be jumped back into from the PATTERNS BB. Jump to box's alpha, return from box's omega") —
   NO C call, the proven combinator mechanism (walk_bb_flat port-threading, as XCAT/XALT already use). This box
   is entered by a JUMP and threads control by JUMP, never a (ζ,int entry) C call (NO C BYRD-BOX FUNCTIONS FACT
   RULE).

   CONTROL-FLOW CONTRACT (the four ports, SPITBOL ch.18 algorithm steps 1-6):
     α (entry): REG-0 — establish the ratified registers from SUBJECT's ζ-frame slots: R13 <- Σ (base),
       R15 <- Δ (length); R14 <- δ = 0 (ch.18 step 1, cursor zeroed when the match begins). Seed the OUTER
       start cursor (a ζ-slot or R14) to 0, then JUMP into the element graph's α (the element entry is held in
       this node's operand_aux, PEERS RULE — resolved at emit time exactly as REF_INVARIANT resolves its sealed
       child).
     element γ (match success) = the OUTER-loop EXIT: record the match span [start, δ] / set the cursor result,
       then JUMP to BB_MATCH's γ (the statement success continuation).
     element ω (no match at this start) = JUMP BACK INTO BB_MATCH's retry: advance the start cursor by one;
       if start <= Δ and &ANCHOR is zero, re-seed R14 <- start and JUMP the element α again (ch.18 step 6,
       unanchored advance); else (anchored, or start exhausted) JUMP to BB_MATCH's ω (match failed).
     β (resume): statement-level single-shot — a `SUBJ ? PAT` match does not re-offer once the statement
       consumed it (bounded; β = ω). Within-pattern backtracking is the element boxes' OWN β/ω, not this box's.

   PER-BOX LOCAL STORAGE / NO-VALUE-STACK FACT RULES: Σ/δ/Δ live in the ratified registers (R13/R14/R15,
   callee-saved so they persist across the element chain and survive any element's `call …@PLT`); the OUTER
   start cursor + match-span result are RW ζ-slots [r12+off]. NO PATND_t, NO tree_t, NO value stack, NO ring.
   The REG ladder (GOAL-SNOBOL4-BB) migrates the element matcher bodies (bb_lit + family) to read R13/R14/R15;
   until then BB_MATCH's α must ALSO populate the legacy [r10]/&Σ/&Σlen the un-migrated elements still read
   (the shim), dropped once REG-1+ land.

   STATUS: TOPOLOGY PROVEN (prove_lower2.sh: IR_PAT_MATCH inline-drives IR_PAT_LIT, element γ+ω -> MATCH; 64->65).
   The BINARY (mode-3 `--run`) and TEXT (mode-4 `--compile`) outer-loop arms are the IMMEDIATE NEXT increment;
   this template fail-louds until they are written (RULES: an honest stub is acceptable while the arm is in
   progress — a permanent silent `jmp ω` for a real opcode is NOT). The fail-loud lives in the MEDIUM_BINARY
   arm (purity-audit-exempt) / bomb_text (TEXT).
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_match_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_PAT_MATCH");
    if (MEDIUM_BINARY) {
        fprintf(stderr, "[SBB] FATAL bb_sno_match: IR_PAT_MATCH BINARY outer-loop arm not yet written — PB-RB-3 topology proven, drive arm is the next increment\n");
        abort();
    }
    return bomb_text("SNO IR_PAT_MATCH: ch.18 outer-loop drive arm not yet written (PB-RB-3 next increment)");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_sno_match(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_match_str(pBB, bin), bin); }
