#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_statement() {
    x86_begin();
    if (!PLATFORM_X86) return x86_alpha() + x86_bomb("bb_statement: X86 only");
    return x86("comment", "IR_STATEMENT bracket (ZW-5 slice 1, DORMANT -- lower does not mint; design of record FINDING-2026-08-02d + GOAL-SNOBOL4-BB s23k addendum).  The statement-scope twin of MATCH_END and op_zgpop's emission HOME BY STAGING: the dispatch case stages op_zgpop=K_total (nd->ival, the future lower stamp) so the whack rides the ONE existing X86H_JMP gamma/omega hook arm -- no second whack spelling exists (s22k one-authority law).  Retires the 5,923-firing fused-pop placement debt (s23k census, STALE-CITED; re-run at the lighting slice).  omega depth-stub ladder EXPLICITLY ABSENT: it lands atomically with the planner that computes the depth set (s22h law); until then the lower gate must decline fail edges arriving at depth > 0, so the hook's single-depth whack is exact for the admitted class.")
         + x86_alpha()
         + x86_bomb("bb_statement: alpha->body wire is slice 2 (driver child handoff; lower mints first per the s23k addendum)")
         + x86_gamma()
         + x86_beta_trampoline();
}
