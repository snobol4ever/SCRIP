#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_MATCH_BAL — SPITBOL manual Ch.9 p.124 + Ch.18 p.203: "Matches any non-null string which is balanced
 * with respect to parentheses.  A string without parentheses is considered balanced.  BAL matches the
 * shortest string possible."  It is therefore a GENERATOR over extents (shortest-first, longer on retry) and
 * so it OWNS RUNTIME STATE — it earns a node under the s31 rule, unlike ABORT/FAIL/FENCE which are edges.
 *
 * Structurally BAL is ARB (bb_match_arb.cpp) plus a parenthesis-depth counter, with two differences the
 * manual forces:  (1) NON-NULL — the scan consumes a character BEFORE it may yield, so extent 0 is never a
 * result (ARB does yield 0);  (2) it only yields where depth == 0, and it EXHAUSTS the moment depth goes
 * NEGATIVE — a ')' that closes nothing can never be balanced by anything further right, so ")A+B" is not
 * merely a non-match at this extent, it is the END of the generator (manual's own counter-examples).
 *
 * ζ (16B, the SPAN shape — a pure in-frame generator, no ζ push, so NO ZLS2 participation unlike ARB):
 *      +0  n      extent: characters consumed from the entry cursor
 *      +4  δ0     entry cursor (the give-back value on exhaust)
 *      +8  depth  parenthesis depth at δ0+n
 *      +12 pad
 * Registers per the house convention: r13 = subject base, r14d = cursor, r15d = subject length.
 * α initialises and falls through into the scan; β re-enters the SAME scan with n/depth intact, which is
 * exactly what "resume and extend to the next balanced extent" means — one loop serves both ports. */
std::string bb_match_bal() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_BAL")
         + x86_alpha()
         + x86("mov",    FR(_.x86_scratch_off),     (long)0)      /* n     = 0                     */
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")        /* δ0    = cursor                */
         + x86("mov",    FR(_.x86_scratch_off + 8), (long)0)      /* depth = 0                     */
         + x86_beta()                                             /* resume: n/δ0/depth intact     */
         + x86("def",    L(0))                                    /* SCAN: consume one character   */
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))            /* eax = δ0 + n = next index     */
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(3))                                    /* subject exhausted             */
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")                      /* esi = subj[δ0+n]              */
         + x86("add",    FR(_.x86_scratch_off), (long)1)          /* n++  (the non-null guarantee) */
         + x86("mov",    "edx", FR(_.x86_scratch_off + 8))        /* edx = depth                   */
         + x86("cmp",    "esi", (long)40)                         /* '('                           */
         + x86("jne",    L(1))
         + x86("add",    "edx", (long)1)
         + x86("jmp",    L(2))
         + x86("def",    L(1))
         + x86("cmp",    "esi", (long)41)                         /* ')'                           */
         + x86("jne",    L(2))
         + x86("sub",    "edx", (long)1)
         + x86("cmp",    "edx", (long)0)
         + x86("jl",     L(3))                                    /* depth < 0 ⇒ generator is DONE */
         + x86("def",    L(2))
         + x86("mov",    FR(_.x86_scratch_off + 8), "edx")        /* depth back to ζ               */
         + x86("test",   "edx", "edx")
         + x86("jne",    L(0))                                    /* unbalanced ⇒ keep scanning    */
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")                           /* cursor = δ0 + n : YIELD       */
         + x86_gamma()
         + x86("def",    L(3))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("mov",    "r14d", "eax")                           /* give the cursor back          */
         + x86_omega();
}
