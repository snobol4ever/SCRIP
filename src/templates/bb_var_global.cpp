#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_global() {
    if (!(PLATFORM_X86 && _.op_off >= 0)) return x86_bomb("bb_var_global: unhandled (needs descr flat-chain + own slot)");
    if (g_gva_active && _.op_gva_k >= 0)
        return x86("comment", "IR_VAR gva")
             + x86("label",  _.lbl_α)
             + x86("mov",    "rax", RDQ("rbx", _.op_gva_k * 16))
             + x86("mov",    "rdx", RDQ("rbx", _.op_gva_k * 16 + 8))
             + x86("mov",    FRQ(_.op_off),     "rax")
             + x86("mov",    FRQ(_.op_off + 8), "rdx")
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("jmp",    "ω");
    return x86("comment", "IR_VAR")
         + x86("label",  _.lbl_α)
         + x86("mov",    "rdi", ROQ(0))
         + x86("call",   "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
         + x86("mov",    FRQ(_.op_off),     "rax")
         + x86("mov",    FRQ(_.op_off + 8), "rdx")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("jmp",    "ω")
         + x86("def",    L(0))
         + x86(".quad",  LS(0), _.op_sval)
         + x86("label",  LS(0))
         + x86(".string", _.op_sval);
}
