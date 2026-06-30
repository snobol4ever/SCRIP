#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_global() {
    if (!(PLATFORM_X86 && _.op_a_slot >= 0 && _.op_off >= 0)) return x86_bomb("bb_assign_global: unhandled (needs descr flat-chain + rhs slot + own slot)");
    if (g_gva_active && _.op_gva_k >= 0)
        return x86("comment", "IR_ASSIGN gva")
             + x86("label",  _.lbl_α)
             + x86("mov",    "rax", FRQ(_.op_a_slot))
             + x86("mov",    "rdx", FRQ(_.op_a_slot + 8))
             + x86("mov",    RDQ("rbx", _.op_gva_k * 16),     "rax")
             + x86("mov",    RDQ("rbx", _.op_gva_k * 16 + 8), "rdx")
             + x86("mov",    FRQ(_.op_off),     "rax")
             + x86("mov",    FRQ(_.op_off + 8), "rdx")
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("jmp",    "ω");
    return x86("comment", "IR_ASSIGN global")
         + x86("label",  _.lbl_α)
         + x86("mov",    "rsi", FRQ(_.op_a_slot))
         + x86("mov",    "rdx", FRQ(_.op_a_slot + 8))
         + x86("mov",    "rdi", ROQ(0))
         + x86("call",   "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
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
