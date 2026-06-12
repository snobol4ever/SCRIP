#include <string>
#include <stdio.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_nl() {
    if (!PLATFORM_X86) return std::string();
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_NL")
         + x86("mov32", "edi", 10L)
         + x86("call", "putchar", (uint64_t)(uintptr_t)(void *)putchar)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}