#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int64_t core_icn_limit_count_check(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit() {
    x86_begin();
    return IF(!(_.op_off >= 0 && _.op_sa >= 0 && _.op_sc >= 0 && _.lbl_t0), x86_alpha() + x86_bomb("bb_limit: unhandled (needs descr flat-chain, static slots, count slot, gen-β)"))
         + IF(_.op_off >= 0 && _.op_sa >= 0 && _.op_sc >= 0 && _.lbl_t0,
             x86("comment", "IR_LIMIT")
           + x86_alpha()
           + x86("comment", "FRESH-ENTRY RESET: op_off+24 is the resumed flag. alpha is reached BOTH on a fresh evaluation (via the count operand, which is on the fresh path only) and on every re-yield of the inner generator (its gamma targets this same label), so the counter cannot be zeroed unconditionally here. beta sets the flag before it resumes the generator; alpha consumes and clears it. Without this the counter was zeroed once per ACTIVATION (bb_limit_init in the prologue), so a limit expression re-evaluated inside a loop -- suspend expr \\ 1 in a while, the IPL words() shape -- stayed exhausted after its first evaluation and yielded nothing thereafter.")
           + x86("mov",   "rax", FRQ(_.op_off + 24))
           + x86("mov",   FRQ(_.op_off + 24), (long)0)
           + x86("test",  "rax", "rax")
           + x86("jnz",   L(0))
           + x86("mov",   FRQ(_.op_off + 16), (long)0)
           + x86("def",   L(0))
           + x86("mov",   "rdi", FRQ(_.op_sc))
           + x86("mov",   "rsi", FRQ(_.op_sc + 8))
           + x86("call",  "core_icn_limit_count_check", (uint64_t)(uintptr_t)(void*)core_icn_limit_count_check)
           + x86("mov",   "rcx", "rax")
           + x86("mov",   "rax", FRQ(_.op_off + 16))
           + x86("cmp",   "rax", "rcx")
           + x86_omega("jge")
           + x86("inc",   FRQ(_.op_off + 16))
           + x86("mov",   "rax", FRQ(_.op_sa))
           + x86("mov",   FRQ(_.op_off),     "rax")
           + x86("mov",   "rax", FRQ(_.op_sa + 8))
           + x86("mov",   FRQ(_.op_off + 8), "rax")
           + x86_gamma()
           + x86_beta()
           + x86("mov",   "rdi", FRQ(_.op_sc))
           + x86("mov",   "rsi", FRQ(_.op_sc + 8))
           + x86("call",  "core_icn_limit_count_check", (uint64_t)(uintptr_t)(void*)core_icn_limit_count_check)
           + x86("mov",   "rcx", "rax")
           + x86("mov",   "rax", FRQ(_.op_off + 16))
           + x86("cmp",   "rax", "rcx")
           + x86_omega("jge")
           + x86("mov",   FRQ(_.op_off + 24), (long)1)
           + x86_jmp_tgt(X86T_TGT0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit_init() {
    x86_begin();
    return x86("mov", FRQ(_.op_off + 16), (long)0);
}
