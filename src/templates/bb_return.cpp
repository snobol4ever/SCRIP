#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_return() {
    if (!PLATFORM_X86) return x86("def", "α") + x86_bomb("bb_return: unhandled (needs descr flat-chain)");
    return x86("comment", "IR_RETURN")
         + x86("def",     "α")
         + IF(_.op_sa >= 0,
               x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", "rdx", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(0), "rax")
             + x86("mov", FRQ(8), "rdx"))
         + IF(_.op_sa < 0,
               x86("mov", FRQ(0), (long)DT_SNUL)
             + x86("mov", FRQ(8), 0L))
         + IF(_.op_dval == 2.0, x86("jmp", "ω"))
         + IF(_.op_dval != 2.0, x86("jmp", "γ"));
}
