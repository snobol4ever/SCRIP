#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_abort() {
    return x86("comment", "IR_MATCH_ABORT")
         + x86_alpha()
         + IF(_.node && IR_LIT(_.node).ival == 1, x86("mov", "r14d", -2L))
         + x86_omega()
         + ((_.node && IR_LIT(_.node).ival == 1) ? x86_beta() + x86("mov", "r14d", -2L) + x86_omega() : x86_beta_trampoline());
}
