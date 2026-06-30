#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "descr.h"
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_descr() {
    if (PLATFORM_X86)
        return (!(_.op_sa >= 0 && _.op_off >= 0)) ? x86_bomb("bb_gvar_assign_descr: needs rhs slot + own slot") :
               (_.op_gva_k >= 0) ?
               x86("label",  _.lbl_α)
             + x86("comment", "IR_ASSIGN_DESCR gva")
             + x86("mov",    "rax", FRQ(_.op_sa))
             + x86("mov",    "rcx", FRQ(_.op_sa + 8))
             + x86("mov",    RDQ("rbx", _.op_gva_k * 16),     "rax")
             + x86("mov",    RDQ("rbx", _.op_gva_k * 16 + 8), "rcx")
             + x86("mov",    FRQ(_.op_off),     "rax")
             + x86("mov",    FRQ(_.op_off + 8), "rcx")
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("jmp",    "ω") :
               x86("label",  _.lbl_α)
             + x86("comment", "IR_ASSIGN_DESCR")
             + x86("mov",    "rsi", FRQ(_.op_sa))
             + x86("mov",    "rdx", FRQ(_.op_sa + 8))
             + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
             + x86("call",   "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
             + x86("mov",    FRQ(_.op_off), "rax")
             + x86("mov",    FRQ(_.op_off + 8), "rdx")
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("jmp",    "ω");
    return std::string();
}
