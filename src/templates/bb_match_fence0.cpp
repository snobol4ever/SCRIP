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
std::string bb_match_fence0() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    int rel = _.op_fence0_release;
    return x86("comment", rel > 0 ? "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits, FZ-1 releases the contiguous backtrack-only spine at the frontier, then gamma; beta abandons to omega)"
                                  : "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits — match null — then gamma; beta abandons to omega; nothing releasable here)")
         + x86_alpha()
         + IF(rel > 0, x86("add", "rsp", rel))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
