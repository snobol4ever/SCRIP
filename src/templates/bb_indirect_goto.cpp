#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_indirect_goto() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86("def", "α") + x86_bomb("bb_indirect_goto: no label-variable slot (op_off<0)");
    return x86("comment", "IR_INDIRECT_GOTO alt-resume: jmp *t")
         + x86("def",     "α")
         + x86("jmp", FRQ(_.op_off + 16))
         + x86("def", "β")
         + x86("jmp", "ω");
}
