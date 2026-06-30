#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_var() {
    x86_begin();
    if (PLATFORM_X86)
        return (g_gvar_flat_chain && _.op_off >= 0 && _.op_gva_k >= 0) ?
               x86("comment", "IR_VAR gva")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rax", RDQ("rbx", _.op_gva_k * 16))
             + x86("mov",     "rdx", RDQ("rbx", _.op_gva_k * 16 + 8))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω") :
               (g_gvar_flat_chain && _.op_off >= 0 && _.op_sval && _.op_sval[0] != '&') ?
               x86("comment", "IR_VAR")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω")
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval) :
               g_gvar_flat_chain ?
               x86("comment", "IR_VAR")
             + x86("label",   _.lbl_α)
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω") :
               (g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0) ?
               x86("comment", "IR_VAR")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω") :
               x86_bomb("bb_var: unhandled arm (no flat-chain mode or missing slot)");
    return std::string();
}
