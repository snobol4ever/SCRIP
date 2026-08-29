#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_cut() {
    x86_begin();
    return x86_alpha()
                           + x86("comment", "IR_CELL_CUT")
                           + x86_gamma()
                           + x86_beta_trampoline();
}
