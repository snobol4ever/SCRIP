#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goto() {
    if (!PLATFORM_X86) return std::string();
    /* IR_GOTO was relying entirely on the driver's removed direct bb_label_define(lbls[i]) call for its own
     * α -- unlike most nodes it CAN be a genuine jump target (e.g. a compiled procedure's own entry, per
     * --dump-ir: entry=n2, n2 an IR_GOTO) even though a predecessor whose OWN target IS a GOTO gets folded
     * past it (codegen_flat_chain_body's γ/ω fold-loop). Own α def restores the ONE-WAY-OUTPUT invariant: the
     * driver no longer defines any node's α directly: every template, including this one, defines its own. */
    return x86_alpha() + x86_pair_loop();
}
