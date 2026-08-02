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
    return x86("comment", "IR_STATEMENT statement-terminal TRAILER (ZW-5 slice 2, OMEGA O-1; lower mints under SCRIP_ZW5=1 only -- see the killswitch comment in lower_snobol4.c for the zd_wl_kind cross-front dependency that keeps it off).  ⭐ SHAPE CORRECTION, MEASURED s23p: slice 1's text called this a BRACKET with an alpha->body wire, and the bomb that stood in for that wire is DELETED here.  x86_alpha() DEFINES a label (x86_asm.h:544) but x86_gamma() IS A JMP (x86_asm.h:547), so this body has exactly ONE entry and control cannot return into it -- a bracket is not expressible in this template at all.  The box is the statement's SUCCESS TRAILER: the body's success wire enters alpha, falls through to jmp gamma, and the release rides that jmp's X86H_JMP hook arm.  Everything the bracket reading promised is delivered by the trailer reading with strictly less machinery.  The statement-scope twin of MATCH_END and op_zgpop's emission HOME BY STAGING: the dispatch case stages op_zgpop=K_total (nd->ival, the future lower stamp) so the whack rides the ONE existing X86H_JMP gamma/omega hook arm -- no second whack spelling exists (s22k one-authority law).  Retires the 5,923-firing fused-pop placement debt (s23k census, STALE-CITED; re-run at the lighting slice).  omega depth-stub ladder EXPLICITLY ABSENT: it lands atomically with the planner that computes the depth set (s22h law); until then the lower gate must decline fail edges arriving at depth > 0, so the hook's single-depth whack is exact for the admitted class.")
         + x86_alpha()
         + x86_gamma()
         + x86_beta_trampoline();
}
