#include <string>
#include <cstdint>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_MATCH_FENCE0 — the BARE `FENCE` protected pattern VARIABLE.  ⛔ THIS IS NOT A FENCE1 WITH ival=0, AND THE TWO MUST NEVER SHARE A KIND AGAIN (Lon ruling, this session): they are different
 * constructs with OPPOSITE β semantics.  FENCE0 (manual Ch.4/18) matches the null string moving forward, and a scanner BACKING UP through it FAILS THE WHOLE ATTEMPT — the cut; FENCE0 first in a
 * pattern is what "effectively anchors" it.  FENCE1 (`FENCE(P)`, manual Ch.19) matches as P and merely makes the alternatives WITHIN P invisible on backup — backup ALWAYS PASSES THROUGH it and does
 * NOT abort.  One kind carrying both forced every consumer to re-derive the difference from arity or ival, and each re-derivation was a chance to get it wrong: emit.cpp's NOWHACK ledger had to be
 * split by n_operands after the fact (s67, which measured the blanket exclusion masking a genuine frame_need_of==1 on H26/H27), and the ζ grant comment had to explain that FENCE0 "takes this quad
 * grant uniformly but its template arm never reads it".  Separate kinds retire the whole class of question.
 *
 * THE BOX: pure four-port, operand-free.  α IS the commit (match the null string, fall to γ); β ≡ abandon, routed by the lowerer's ω edge exactly as the s133 erasure routed it.  FIRST-POSITION FENCE0
 * (the anchor idiom) never reaches here — it stays node-free in the lowerer, having zero left context and nothing to seal.
 *
 * ⛔ NO RSP WHACK — deliberate, and NOT an oversight to be "fixed" by reinstating the old line.  WHACK CONTRACT clause 5: statement-accumulated ζ release is STATEMENT_END's authority.  The op_zw arm
 * that once whacked here targeted the ___ activation floor ABSOLUTELY, and under UCLAIM the statement claim (`sub rsp,K` at the statement head) lives BELOW that floor — so the whack released memory
 * the statement's staged `add rsp,K` fail exits still owned: DOUBLE-RELEASE, measured as SEGV-after-correct-output and as the H26/G23 hang class (ZWS-FENCE0-UCLAIM fix, MECH s6).  A safe whack here
 * is possible — but ⛔ NOT by copying FENCE1's own-extent shape: FENCE0's box is α→γ with NOTHING BETWEEN, so banking rsp at α and restoring it at γ frees exactly zero bytes.  FENCE0 has no extent
 * of its own; what its cut kills is the LEFT CONTEXT, which is why the retired whack reached for a floor rather than a watermark, and why it collided with UCLAIM.
 *
 * ⭐ FENCE0-WHACK (this rung, killswitch SCRIP_FENCE0_WHACK, DEFAULT OFF): the safe floor is RBP, not ___.  bb_match_begin establishes the ζ-STANDING frame with `push rbp; mov rbp,rsp` (:46-47) and
 * names `mov rsp,rbp` its own whole-frame whack, so rbp is the match's entry frontier — recorded AFTER the statement head's `sub rsp,K`, hence DEEPER than the UCLAIM claim and incapable of releasing
 * it.  That is the precise difference from the retired line: ___ is the graph floor and sits ABOVE the claim, rbp is the match floor and sits below it.  Restoring rsp:=rbp frees every cell the match
 * carved to the left of the cut — which is dead by definition once FENCE0 commits (backup through a bare FENCE fails the whole attempt, so nothing left of it can ever be re-entered).  If an ARBNO or
 * DEFER activation frame is live, rbp names THAT frame instead and the whack frees less; still safe, still correct, never over-free.  Gated on x86_port_cstack() (the arena ports have no rsp frames)
 * and emit_match_rbp() (no standing frame ⇒ no floor to restore to).  ⛔⛔ FALSIFIED, MEASURED — DO NOT ARM THIS WITHOUT READING WHAT FOLLOWS.  Armed, treebank-match-fence
 * SIGSEGVs (rc=139) at EVERY stack size; disarmed it runs in 256 KB and answers check: 100155.  The crosscheck gate did NOT catch it (306/10, DIVERGE=0, FAIL-set identical armed and disarmed) — the
 * witness is a demo workload, which is exactly why the workload family exists.
 * ⭐ WHY IT IS WRONG, AND WHY THE ___ FLOOR WAS WRONG THE SAME WAY: FENCE0's cut kills BACKTRACKING into the left context; it does NOT kill the left context's DATA.  Capture cells, ARBNO instance
 * records and cursor state carved left of the fence are read on the FORWARD path and at MATCH_END, so ANY bulk floor restore — ___ or rbp — frees storage that is still live and the next reader lands
 * in reused stack.  Changing which floor you restore to only moves the crash.  A correct FENCE0 release must free BACKTRACK-ONLY cells (choice points, resume records) and nothing else, which the
 * template cannot know: it is a PLANNER fact (zd_plan/zeta_storage must classify a cell as backtrack-only vs value-bearing).  That is the real rung, and it is not a one-liner here.
 * FENCE1 is safe for the opposite reason and is not a precedent for this: its watermark frees only what P carved AFTER the bank, and P's cells are provably dead once P commits. */
static int fence0_whack_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FENCE0_WHACK"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }   /* DEFAULT OFF: armed OFF is byte-identical to the no-whack box, so landing this rung cannot move a single program until the switch is thrown */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence0() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    int whack = fence0_whack_on() && x86_port_cstack() && emit_match_rbp();
    return x86("comment", whack ? "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits, WHACK rsp:=rbp — the match standing floor, below the UCLAIM statement claim — then gamma; beta abandons to omega)"
                                : "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits — match null — then gamma; beta abandons to omega; whack OFF — SCRIP_FENCE0_WHACK=1 to arm)")
         + x86_alpha()
         + IF(whack, x86("mov", "rsp", "rbp"))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
