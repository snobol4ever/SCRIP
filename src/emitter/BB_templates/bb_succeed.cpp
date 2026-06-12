#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_succeed() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX SUCCEED() [x86() self-encoding — jmp γ; β: jmp ω]"))
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return std::string();
}