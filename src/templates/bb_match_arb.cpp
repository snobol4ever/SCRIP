#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arb() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    int _diag = getenv("SCRIP_ARB_DIAG") ? 1 : 0;
    std::string _line1 = x86("comment", "IR_MATCH_ARB")
         + x86_alpha()
         + x86("mov",    FR(_.x86_scratch_off), (long)0);
    if (_diag) fprintf(stderr, "[ARB] AFTER-LINE1 x86_scratch_off=%d op_fc_bytes=%ld op_fc_base=%ld op_zdepth=%d op_fb_data=%d zstorage=%d fc_on=%d fc_hit0=%d regime0=%d emitted_tail=%s\n", _.x86_scratch_off, (long)_.op_fc_bytes, (long)_.op_fc_base, _.op_zdepth, x86_fb_data(), (int)x86_zstorage(), x86_fc_on(), x86_fc_hit(_.x86_scratch_off), x86_zop_regime(_.x86_scratch_off), _line1.substr(_line1.size() > 80 ? _line1.size() - 80 : 0).c_str());   /* L-3b ARB RESIDUAL DIAGNOSTIC (this session): prints the LAST 80 CHARS OF THE ACTUAL EMITTED STRING right after the first FR() write is built into it, PLUS every predicate x86_zop_regime consults, evaluated in the SAME expression so nothing about their evaluation context can differ from the real call. Env-gated, additive-only, fprintf only, no behavior change. */
    return _line1
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86_gamma()
         + x86_beta()
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jg",     L(0))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86("def",    L(0))
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
