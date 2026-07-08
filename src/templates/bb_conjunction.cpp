#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_conjunction() {
    if (!PLATFORM_X86) return std::string();
    /* IR_CONJUNCTION was relying entirely on the driver's removed direct bb_label_define(lbls[i]) call for
     * its own α. Grouped with IR_GOTO in emit_chain_arity (both "arity 0, transparent" for that unrelated
     * purpose) but NOT proven transparent for jump-targeting the way IR_GOTO's fold-loop is -- own α def
     * restores the ONE-WAY-OUTPUT invariant regardless: every live template defines its own α, never the
     * driver. Costs nothing if some path never actually jumps here. */
    if (_.op_sa >= 0 && _.op_off >= 0 && _.op_sa != _.op_off) {
        return x86_alpha()
             + x86("comment", "IR_CONJUNCTION value-forward copy")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_pair_loop();
    }
    return x86_alpha() + x86_pair_loop();
}
