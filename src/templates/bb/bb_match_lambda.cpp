#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_lambda() {
    return x86("comment", "IR_MATCH_LAMBDA immediate — epsilon; the expression ran on the wired edge into alpha")
         + x86_alpha()
         + x86_gamma()
         + x86_beta_trampoline();
}
