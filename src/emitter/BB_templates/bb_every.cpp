#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_every() {
    if (PLATFORM_X86) return x86("label", _.lbl_β)
                           + x86("comment", "IR_EVERY")
                           + x86("def", "β")
                           + x86("jmp", "ω");
    return std::string();
}