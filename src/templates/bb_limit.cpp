#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit() {
    x86_begin();
    if (!(_.op_off >= 0 && _.op_sa >= 0 && _.op_sc >= 0 && _.lbl_t0))
        return x86_alpha() + x86_bomb("bb_limit: unhandled (needs descr flat-chain, static slots, count slot, gen-β)");
    return x86("comment", "IR_LIMIT")
         + x86_alpha()
         + x86("mov",   "rax", FRQ(_.op_off + 16))
         + x86("mov",   "rcx", FRQ(_.op_sc + 8))
         + x86("cmp",   "rax", "rcx")
         + x86_omega("jge")
         + x86("inc",   FRQ(_.op_off + 16))
         + x86("mov",   "rax", FRQ(_.op_sa))
         + x86("mov",   FRQ(_.op_off),     "rax")
         + x86("mov",   "rax", FRQ(_.op_sa + 8))
         + x86("mov",   FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta()
         + x86_jmp_tgt(X86T_TGT0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit_init() {
    x86_begin();
    return x86("mov", FRQ(_.op_off + 16), (long)0);
}
