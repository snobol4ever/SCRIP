#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_FENCE")
         + x86_alpha()
         + x86("mov", FR(_.x86_scratch_off), "r14d")
         + x86_gamma()
         + x86_beta()
         + x86("mov", "r14d", FR(_.x86_scratch_off))
         + x86_omega();
}
