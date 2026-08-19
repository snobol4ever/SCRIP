#include <string>
#include <cstdint>
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
 * is possible and is a SEPARATE rung: bank rsp at α into this node's granted quad and restore THAT at γ, the own-extent shape bb_match_fence1 already proves — relative, so it cannot dive under the
 * claim.  The grant is reserved for exactly that (zeta_storage.c IR_MATCH_FENCE0); this template does not yet read it, and the rung that lands the whack owns the killswitch, gates and blast radius. */
std::string bb_match_fence0() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_FENCE0 (bare FENCE sync box: alpha commits — match null — then gamma; beta abandons to omega; NO RSP whack — WHACK CONTRACT clause 5, see ZWS-FENCE0-UCLAIM)")
         + x86_alpha()
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
