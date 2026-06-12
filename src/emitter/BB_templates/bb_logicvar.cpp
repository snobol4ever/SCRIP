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
    (void)pBB;
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_LOGICVAR")
         + x86("label",   _.lbl_α)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
