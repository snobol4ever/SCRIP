#include <string>
#include <stdio.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_nl_str() {
    if (!PLATFORM_X86) return std::string();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX DET_NL  [PL-GZ det VALUE call]"))
         + x86("mov32", "edi", 10L)
         + x86("call", "putchar", (uint64_t)(uintptr_t)(void *)putchar)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_nl(void) { bb_emit_x86(bb_det_nl_str()); }
