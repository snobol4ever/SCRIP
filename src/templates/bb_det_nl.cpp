#include <string>
#include <stdio.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_nl() {
    return x86_alpha()
         + x86("comment", "IR_DET_NL")
         + x86("mov32", "edi", 10L)
         + x86("call", "putchar", (uint64_t)(uintptr_t)(void *)putchar)
         + x86_gamma()
         + IF(!_.op_bounded, x86_beta() + x86_omega());
}
