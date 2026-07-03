#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_capture() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_PATTERN_CAPTURE passthrough")
         + x86("label",   _.lbl_α)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_capture_call(void) { bb_emit_x86(bb_pattern_capture()); }
