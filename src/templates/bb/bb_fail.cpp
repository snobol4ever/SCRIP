#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_fail() {
    return x86_alpha()
                           + x86("comment", "IR_FAIL")
                           + x86_omega()
                           + x86_beta_trampoline();
    return std::string();
}
