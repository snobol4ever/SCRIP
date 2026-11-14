#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_abort() {
    return x86("comment", "IR_MATCH_ABORT")
         + x86_alpha()
         + IF(_.op_ival == 1, x86("mov", "r14d", -2L))
         + x86_omega()
         + ((_.op_ival == 1) ? x86_beta() + x86("mov", "r14d", -2L) + x86_omega() : x86_beta_trampoline());
}
