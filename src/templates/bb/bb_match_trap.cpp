#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_trap() {
    return x86("comment", "IR_MATCH_TRAP")
         + x86_alpha()
         + IF(_.lbl_t1_p,
               x86("cmp", "r14d", -2L)
             + x86_jcc_tgt("je", X86T_TGT1))
         + x86_omega()
         + x86_beta_trampoline();
}
