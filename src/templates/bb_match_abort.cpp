#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_abort() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_ABORT")
         + x86("def",     "α")
         + x86("jmp",  "ω")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
