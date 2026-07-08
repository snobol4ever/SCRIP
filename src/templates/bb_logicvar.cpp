#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_logicvar(IR_t * pBB) {
    if (PLATFORM_X86) {
        return x86_alpha() + x86("comment", "# BOX RESOLVE_VAR(slot=" + std::to_string(_.op_ival) + ") [stackless pass-through]")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    return std::string();
}
