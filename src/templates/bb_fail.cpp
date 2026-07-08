#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_fail() {
    if (PLATFORM_X86) return x86_alpha()
                           + x86("comment", "IR_FAIL")
                           + x86_omega()
                           + x86_beta()
                           + x86_omega();
    return std::string();
}
