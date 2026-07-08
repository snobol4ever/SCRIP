#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_atom() {
    if (PLATFORM_X86) return x86("def",     "α")
                           + x86("comment", "IR_ATOM")
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω");
    return std::string();
}
