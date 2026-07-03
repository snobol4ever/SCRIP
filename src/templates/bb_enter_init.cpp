#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_enter_init() {
    x86_begin();
    if (!PLATFORM_X86) return x86_bomb("bb_enter_init: no x86 platform");
    if (_.op_off < 0) return x86_bomb("bb_enter_init: op_off < 0 (no slot assigned)");
    return x86("comment", "IR_INITIAL")
         + x86("mov",  "rax", FRQ(_.op_off + 8))
         + x86("cmp",  "rax", (long)0)
         + x86("jne",  "ω")
         + x86("mov",  FRQ(_.op_off + 8), (long)1)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
