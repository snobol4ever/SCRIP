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
    return x86("comment", _.op_fence0_floor > 0 ? "IR_MATCH_FENCE0 (bare FENCE cut box, BLOB DYNAMIC RELEASE: alpha commits and restores rsp to the blob activation floor rbp-K, freeing every left-context backtrack record the static chase cannot size — the unbounded-leak class of FINDING-2026-08-23-seat04-json-fence0; then gamma; beta abandons to omega)"
                        : _.op_fence0_release > 0
                              ? "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits, FZ-1 releases the contiguous backtrack-only spine at the frontier, then gamma; beta abandons to omega)"
                                  : "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits — match null — then gamma; beta abandons to omega; nothing releasable here)")
         + x86_alpha()
         + IF(_.op_fence0_floor > 0, x86("mov", "rsp", "rbp")
                                    + x86("sub", "rsp", (long)_.op_fence0_floor))
         + IF(_.op_fence0_floor <= 0 && _.op_fence0_release > 0, x86("add", "rsp", _.op_fence0_release))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
