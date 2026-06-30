#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-4: IR_PATTERN_CAPTURE is now a pure passthrough box. SNOBOL4 invariant captures freeze to IR_REF_INVARIANT (no per-shape bb_build_break_capture_blob); the remaining producer is Raku (lower_raku.c TT_CAPTURE/NAMED/FH), whose capture is realised on its own NFA path, so this box only sequences α→γ. */
std::string bb_pattern_capture() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_PATTERN_CAPTURE passthrough")
         + x86("label",   _.lbl_α)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_capture_call(void) { bb_emit_x86(bb_pattern_capture()); }
