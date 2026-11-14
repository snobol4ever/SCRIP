#include <string>
#include <stdint.h>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_enter_init() {
    x86_begin();
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_enter_init: op_off < 0 (no slot assigned)"))
         + IF(!(_.op_off < 0),
             x86_alpha()
           + x86("comment", "IR_INITIAL")
           + x86("mov", "rax", FRQ(_.op_off + 8))
           + x86("cmp", "rax", (long)0)
           + x86_omega("jne")
           + x86("mov", FRQ(_.op_off + 8), (long)1)
           + x86_gamma()
           + x86_beta_trampoline());
}
