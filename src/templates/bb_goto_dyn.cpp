#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_goto_dyn(const char *name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goto_dyn() {
    if (!PLATFORM_X86) return std::string();
    const char *nm = _.op_sval ? _.op_sval : "";
    return x86("comment", "IR_GOTO_DYN")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_goto_dyn", (uint64_t)(uintptr_t)(void *)rt_goto_dyn)
         + x86_gamma()
         + x86_beta()
         + x86_omega()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), nm)
         + x86("label",   LS(0))
         + x86(".string", nm);
}
