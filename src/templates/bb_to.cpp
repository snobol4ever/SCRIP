#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0))
        return x86("def", "α") + x86_bomb("bb_to: unhandled (needs static operands, descr flat-chain)");
    if (_.op_num_real) {
        return x86("comment", "IR_TO real")
             + x86("def",     "α")
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(10))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)BINOP_LE)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86("jz",      "ω")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 24))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", ROQ(0))
             + x86("mov",     "rcx", ROQ(1))
             + x86("mov",     "r8d", (long)BINOP_ADD)
             + x86("call",    "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("jmp",     L(10))
             + x86_ro_seal_q(0, (uint64_t)(int64_t)DT_R)
             + x86_ro_seal_q(1, (uint64_t)(int64_t)1);
    }
    return x86("comment", "IR_TO")
         + x86("def",     "α")
         + x86("mov",     "rax", FRQ(_.op_sa + 8))
         + x86("mov",     FRQ(_.op_off + 16), "rax")
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("mov",     "rcx", FRQ(_.op_sb + 8))
         + x86("cmp",     "rax", "rcx")
         + x86("jg",      "ω")
         + x86("mov",     FRQ(_.op_off),     (long)DT_I)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0));
}
