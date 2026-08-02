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
    return x86("comment", "IR_STATEMENT statement-terminal TRAILER (ZW-5, OMEGA O-1+O-2; lower mints under SCRIP_ZW5=1 default-ON, killswitch SCRIP_ZW5=0).  SHAPE: success wire -> alpha -> jmp gamma (release rides X86H_JMP gamma hook via op_zgpop), beta trampoline -> omega, then per-depth omega stubs emitted BY THE DRIVE LOOP (codegen_flat_chain_body) immediately after this template returns -- NOT by the template itself.  Each stub: emit_label_define_bb(&zw5_stub_lbls[d]) + x86(add rsp, K) + x86_jmp_ext(&lbl_omega) -- one stub per distinct zwpop value over the statement run members.  Lower routes member fail edges through the statement box omega (stb->omega = fT) so they jmp to the appropriate per-depth stub label.  ATOMICITY (s22h law): stubs + planner (zd_plan omega depth collection) land in the same commit.  Retires the 5,923-firing fused-pop placement debt (s23k census, STALE-CITED re-run at gate).")
         + x86_alpha()
         + x86_gamma()
         + x86_beta_trampoline();
}
