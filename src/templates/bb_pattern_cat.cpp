#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-4: IR_PATTERN_CAT is now a pure passthrough box. Every invariant stored concatenation freezes to IR_REF_INVARIANT (FZ-2/FZ-3) so the per-shape bb_build_break_cap_lit_blob runtime builder is retired; a non-frozen stored CAT shape (structural variance) is rejected at the emit_core dispatch with a loud stub until the stitch path lands, so this box only sequences α→γ as a gvar-chain entry. */
std::string bb_pattern_cat() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_PATTERN_CAT passthrough (chain entry)")
         + x86("label",   _.lbl_α)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_cat_call(void) { bb_emit_x86(bb_pattern_cat()); }
