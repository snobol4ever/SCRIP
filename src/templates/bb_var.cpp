#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var() {
    x86_begin();
    if (PLATFORM_X86)
        return _.op_off >= 0 && _.op_sa >= 0 ?
               x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
               x86_bomb("bb_var: unhandled arm (no flat-chain mode or missing slot)");
    return std::string();
}
