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
 * ⭐ FZ-1/FZ-2 SELECTIVE RELEASE (killswitch SCRIP_FENCE0_WHACK, DEFAULT OFF — and s166 found the SECOND reason it must stay off).  The planner measures how much of the frontier is provably dead:
 * fence0_release_bytes() (emit.cpp) walks the executed prefix over SPAN/BREAK/BREAKX/TAB/RTAB/REM/BAL — leaves whose 16B cell is their OWN cnt/cur retry state, dead the instant the cut forbids re-entry,
 * result delivered in r13/r14/r15 rather than the cell — and STOPS at the first carving node that is anything else, because LIFO means one pinned cell pins everything under it.  ASSIGN_SAVE excluded by
 * name though it carves 16 (its cell is read back by the paired COND/IMM across the fence).  The template spends the count, it does not compute it.
 * ✅ FZ-2 (s166) — THE s154 COUNT IS RECONCILED, AND THE CAUSE WAS NEITHER HYPOTHESIS THE CURSOR OFFERED.  FZ-1 walked g_emit_cfg->all[] by descending index and called it carve order; all[] is ALLOCATION
 * order and SNOBOL4 lowering is RIGHT-FIRST, so on `S ? SPAN('a') FENCE SPAN('b') SPAN('c')` the two SPANs left of the fence in all[] were 'b' and 'c' — both carved AFTER the cut runs — while 'a', the one
 * dead cell, sat to the RIGHT at index 28 and was never counted.  It billed 32 where the emitted carve holds one releasable 16 standing on MATCH_BEGIN's LIVE 24, i.e. `add rsp,32` popped 16 bytes of the
 * bracket's own cell — the SAME over-release-into-live-storage class that cored the floor whack, reached by a different road.  FIXED by walking the EXECUTED prefix: chase γ from the enclosing MATCH_BEGIN
 * through zd_chase(), the same edge-following authority zd_plan's run walker uses, then step back down that prefix.  Witness now bills 16 and the release exactly matches the carve.
 * ⛔⛔ FZ-3 IS THE REAL WALL, AND IT IS WHY ARMING BY DEFAULT IS STILL REFUSED (s166, MEASURED — DO NOT ARM ON THE STRENGTH OF THE FIXED COUNT).  A CORRECT count is necessary and NOT sufficient: the release
 * is INVISIBLE TO THE ζ DEPTH PLANNER.  zd_plan/zvo_resolve stage every [rsp+off] in the statement at the depth model they computed WITHOUT this `add rsp,K`, so the instant K>0 every static offset to the
 * RIGHT of the cut is stale by exactly K.  PROOF, one diff: armed vs disarmed on corpus/probe/fz/fz3 case D differs in ONLY the two `add rsp,16` lines — every staged offset is byte-identical while RSP moved.
 * WITNESS: `S ? (SPAN('a') FENCE SPAN('b')) . W` preceded by ANY other fenced statement prints W=`abbb` armed against the oracle's `aaabbb` (the group's COND reads its SAVE cursor back at a stale offset);
 * both modes, m3 and m4 alike.  fz1/fz2 pass only because nothing to the right of their cuts reads a cell at a staged offset.  THE RUNG THIS NAMES: thread the release into the depth model (zd_plan's staging),
 * so the offsets right of the cut are computed at the post-release depth — the template comment below already predicted this shape ("it is a PLANNER fact ... not a one-liner here").  Until that lands the
 * count is right, the arm is wrong, and DEFAULT OFF is the only honest position.  HISTORICAL, KEPT AS THE REASON THIS SHAPE EXISTS — the first 
 * ⭐ WHY IT IS WRONG, AND WHY THE ___ FLOOR WAS WRONG THE SAME WAY: FENCE0's cut kills BACKTRACKING into the left context; it does NOT kill the left context's DATA.  Capture cells, ARBNO instance
 * records and cursor state carved left of the fence are read on the FORWARD path and at MATCH_END, so ANY bulk floor restore — ___ or rbp — frees storage that is still live and the next reader lands
 * in reused stack.  Changing which floor you restore to only moves the crash.  A correct FENCE0 release must free BACKTRACK-ONLY cells (choice points, resume records) and nothing else, which the
 * template cannot know: it is a PLANNER fact (zd_plan/zeta_storage must classify a cell as backtrack-only vs value-bearing).  That is the real rung, and it is not a one-liner here.
 * FENCE1 is safe for the opposite reason and is not a precedent for this: its watermark frees only what P carved AFTER the bank, and P's cells are provably dead once P commits. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence0() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    int rel = _.op_fence0_release;   /* ⭐ FZ-3 (s168): THE TEMPLATE SPENDS THE COUNT AND NO LONGER RE-ASKS WHETHER IT IS ARMED.  fence0_release_bytes() (emit.cpp) now owns the arm gate as part of the count itself, because the ζ DEPTH PLANNER has to spend the same number: zd_plan subtracts it from the depth accumulator so every [rsp+off] right of the cut is staged at the POST-release depth (that was FZ-3, the s166 wall -- a correct count with an invisible pop left every downstream offset stale by exactly the count).  Two spellings of the arm would let planner and template disagree, which is the s66 coherent-worlds law and the reason this test moved rather than got copied.  Disarmed still yields rel==0 and a box byte-identical to the no-whack box -- the property that let this rung land dark. */   /* ⭐ FZ-1: the PLANNER decides how much is releasable (fence0_release_bytes); the template only spends it.  rel==0 ⇒ byte-identical to the no-whack box, which is every fence whose left neighbour is not a pure retry-cell carrier. */
    return x86("comment", rel > 0 ? "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits, FZ-1 releases the contiguous backtrack-only spine at the frontier, then gamma; beta abandons to omega)"
                                  : "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits — match null — then gamma; beta abandons to omega; nothing releasable here)")
         + x86_alpha()
         + IF(rel > 0, x86("add", "rsp", rel))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
