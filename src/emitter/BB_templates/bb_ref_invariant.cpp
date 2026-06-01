/* bb_ref_invariant.cpp — STACKLESS BB template for the SNOBOL4 pattern REF_INVARIANT box (IR_REF_INVARIANT).
   PB-RB-1 (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01, Opus 4.8). A SNOBOL4 pattern is a
   graph of EMITTED BYRD-BOXES, NOT a PATND_t data structure. An INVARIANT pattern element (here a literal)
   is the EXISTING IR_PAT_LIT matcher box (bb_lit.cpp), SEALED at compile time. REF_INVARIANT REFERENCES that
   sealed element: it loads the sealed element's bb_box_fn HEAD (an EMIT-TIME CONSTANT — the child-cache fn
   ptr) into a ζ-frame RW slot [ζ=r12+off], then jmps γ. NO runtime construction (Fork A/E): nothing is built;
   the head address is a constant baked into the box (movabs imm64 in the BINARY arm / lea [rip+label] in the
   TEXT arm — RO data, never on a stack). For a FULLY-invariant pattern this one box's output IS the pattern's
   DT_P value (a bb_box_fn graph head) handed to the PB-RB-3 BB_MATCH box to drive via the four-port broker.

   SPITBOL Manual ch.18 ("Patterns and Pattern Matching" / "Pattern Matching"): a literal such as 'BLUE' is a
   primitive component matched left-to-right; the sealed matcher (bb_lit) carries the bytes + length and, when
   BB_MATCH drives it, advances the cursor δ past a successful match. REF_INVARIANT is phase-2 construction
   (it makes the matcher head available); the actual scan is phase 3 (BB_MATCH), so this box does NOT run the
   matcher — it only references it.

   PER-BOX LOCAL STORAGE FACT RULE: the sealed-head address is RO (movabs/lea, never a stack); the built head
   is RW into an 8-byte ζ-slot [r12+off]. NO PATND_t, NO tree_t, NO value stack, NO ring. Bounded single-shot:
   referencing the sealed head once is enough -> β = jmp ω.

   The sealed child was emitted once by pre_build_children / pre_build_children_text (keyed in the child
   cache, resolved from operand_aux per the PEERS RULE); flat_drive_sno_ref_invariant hands its head to this
   box via g_emit.bb_child_fn (BINARY fn ptr) / g_emit.bb_child_lbl (TEXT α-label).

   Bytes (BINARY arm, 25 total):
       0  : 48 B8 + u64le child_fn       movabs rax, sealed_head           (RO emit-time constant)
       10 : 49 89 84 24 + u32le off      mov [r12+off], rax                (RW ζ-slot = built head)
       18 : E9 + u32le γ_rel32           jmp γ                             (rel32 patch at 19 -> lbl_γ)
       23 : E9 + u32le ω_rel32           β: jmp ω                          (β-def at 23, patch at 24 -> lbl_ω)
       25 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
int bb_slot_alloc(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ref_invariant_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_REF_INVARIANT");
    const char * child_lbl = _.bb_child_lbl;
    void       * child_fn  = _.bb_child_fn;
    int have_child = MEDIUM_BINARY ? (child_fn != NULL) : (child_lbl && child_lbl[0]);
    if (!pBB || !have_child) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_ref_invariant: no sealed element head (child_fn=%p) — PB-RB-1 needs a pre-built IR_PAT_LIT\n", child_fn);
            abort();
        }
        return bomb_text("SNO IR_REF_INVARIANT: no sealed element head (child not pre-built)");
    }
    int off = bb_slot_alloc(pBB);
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_REF_INVARIANT load sealed head '%s' -> [r12+%d] [stackless ζ-frame, RO [rip+disp]]", child_lbl, off))
             + s_2asm("lea", emit_fmt("rax, [rip + %s]", child_lbl))
             + s_2asm("mov", emit_fmt("[r12 + %d], rax", off))
             + s_2asm("jmp", _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp", _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t cfn = (uint64_t)(uintptr_t)child_fn;
        bin = { {19, 23, 24}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xB8")         + u64le(cfn)
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(1, "\xE9")             + u32le(0)
             + bytes(1, "\xE9")             + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ref_invariant(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_ref_invariant_str(pBB, bin), bin); }
