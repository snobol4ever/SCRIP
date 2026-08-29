#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cut() {
    return x86_alpha()
                           + x86("comment", "IR_CUT")
                           + x86_gamma()
                           + x86_beta_trampoline();
}
