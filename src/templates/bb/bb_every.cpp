#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_every() {
    return x86("comment", "IR_EVERY")
                           + x86_omega()
                           + x86_beta_trampoline();
}
