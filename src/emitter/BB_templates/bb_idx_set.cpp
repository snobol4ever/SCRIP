#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_idx_set() {
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_sa >= 0 && _.op_sb >= 0 && _.op_sc >= 0 && _.op_off >= 0))
        return x86_bomb("bb_idx_set: unhandled (needs base/idx/rhs frame slots + descr flat-chain)");
    return x86("label",   _.lbl_α)
         + x86("comment", "IR_IDX_SET")
         + x86("mov",     "rdi", FRQ(_.op_sa))
         + x86("mov",     "rsi", FRQ(_.op_sa + 8))
         + x86("mov",     "rdx", FRQ(_.op_sb))
         + x86("mov",     "rcx", FRQ(_.op_sb + 8))
         + x86("mov",     "r8",  FRQ(_.op_sc))
         + x86("mov",     "r9",  FRQ(_.op_sc + 8))
         + x86("call",    "subscript_set", (uint64_t)(uintptr_t)(void*)subscript_set)
         + x86("cmp",     "eax", (long)0)
         + x86("je",      "ω")
         + x86("mov",     "rax", FRQ(_.op_sc))
         + x86("mov",     "rdx", FRQ(_.op_sc + 8))
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
