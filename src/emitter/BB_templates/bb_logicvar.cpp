#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_logicvar(IR_t * pBB) {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("ins1", "# BOX RESOLVE_VAR(slot=" + std::to_string(_.op_ival) + ") [stackless pass-through]"))
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return std::string();
}