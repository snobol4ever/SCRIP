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
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + x86_omega();
}
