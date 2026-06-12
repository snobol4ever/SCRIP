#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_abort() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_ABORT")
         + x86("label",   _.lbl_α)
         + x86("jmp",  "ω")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
