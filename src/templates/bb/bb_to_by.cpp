#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
int     core_icn_by_zero_check(int64_t by);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_to_by() {
    x86_begin();
        return !(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0 && _.op_sc >= 0) ? x86_alpha() + x86_bomb("bb_to_by: unhandled (needs from/to/by operand slots, descr flat-chain)") :
               _.op_num_real ?
               x86("comment", "IR_TO_BY real")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_sc + 8))
             + x86("test",    "rax", "rax")
             + x86("js",      L(1))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)BINOP_LE)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86_omega("jz")
             + x86("jmp",     L(2))
             + x86("def",     L(1))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)BINOP_GE)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86_omega("jz")
             + x86("def",     L(2))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 24))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sc))
             + x86("mov",     "rcx", FRQ(_.op_sc + 8))
             + x86("mov",     "r8d", (long)BINOP_ADD)
             + x86("call",    "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("jmp",     L(0)) :
               x86("comment", "IR_TO_BY")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sa),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sa + 8), "rax")
             + x86("mov",     "rdi", FRQ(_.op_sb))
             + x86("mov",     "rsi", FRQ(_.op_sb + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sb),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sb + 8), "rax")
             + x86("mov",     "rdi", FRQ(_.op_sc))
             + x86("mov",     "rsi", FRQ(_.op_sc + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sc),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sc + 8), "rax")
             + x86("mov",     "rdi", "rax")
             + x86("call",    "core_icn_by_zero_check", (uint64_t)(uintptr_t)(void*)core_icn_by_zero_check)
             + x86("cmp",     "eax", (long)0)
             + x86_omega("jne")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "rdx", FRQ(_.op_sc + 8))
             + x86("cmp",     "rdx", (long)0)
             + x86("jl",      L(1))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("jmp",     L(2))
             + x86("def",     L(1))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jl")
             + x86("def",     L(2))
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",     "rdx", FRQ(_.op_sc + 8))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("add",     "rax", "rdx")
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("jmp",     L(0));
}
