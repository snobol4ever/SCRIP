#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
int rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int rt_relop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out);
int rt_binop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out);
DESCR_t rt_str_coerce(DESCR_t d);
void rt_relop_val_coerce(DESCR_t a, DESCR_t b, DESCR_t *out);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop() {
    return (!_.op_num_real && _.op_off >= 0 && _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE && _.op_sa >= 0 && _.op_sb >= 0)
             ? x86_alpha()
             + x86("comment", "IR_BINOP_TEST")
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "al", (long)DT_DATA)
             + x86("je", L(0))
             + x86("mov", "eax", FR(_.op_sb))
             + x86("cmp", "al", (long)DT_DATA)
             + x86("je", L(0))
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(2))
             + x86("mov", "eax", FR(_.op_sb))
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(2))
             + x86("def", L(1))
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("cmp", "rax", "rcx")
             + x86_omega(_.op_ival == BINOP_LT ? "jge" : _.op_ival == BINOP_LE ? "jg" : _.op_ival == BINOP_GT ? "jle" : _.op_ival == BINOP_GE ? "jl" : _.op_ival == BINOP_EQ ? "jne" : "je")
             + x86("mov", "rcx", FRQ(_.op_sb))
             + x86("mov", FRQ(_.op_off), "rcx")
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", FRQ(_.op_off + 8), "rcx")
             + x86_gamma()
             + x86("def", L(0))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8d", (long)_.op_ival)
             + x86("lea", "r9", FRQ(_.op_off))
             + x86("call", "rt_relop_overload", (uint64_t)(uintptr_t)(void*)rt_relop_overload)
             + x86("test", "eax", "eax")
             + x86("je", L(1))
             + x86("cmp", "eax", (long)1)
             + x86_omega("je")
             + x86_gamma()
             + x86("def", L(2))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8d", (long)_.op_ival)
             + x86("call", "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test", "eax", "eax")
             + x86_omega("jz")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("lea", "r8", FRQ(_.op_off))
             + x86("call", "rt_relop_val_coerce", (uint64_t)(uintptr_t)(void*)rt_relop_val_coerce)
             + x86_gamma()
             + x86_beta_trampoline()
         : (_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0
            && ((_.op_num_real && _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE) || (_.op_ival >= BINOP_SLT && _.op_ival <= BINOP_SNE) || _.op_ival == BINOP_EQV || _.op_ival == BINOP_NEQV))
             ? x86_alpha()
             + x86("comment", "IR_BINOP_TEST")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8d", (long)_.op_ival)
             + x86("call", "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test", "eax", "eax")
             + x86_omega("jz")
             + (_.op_ival >= BINOP_SLT && _.op_ival <= BINOP_SNE
                 ? x86("mov", "rdi", FRQ(_.op_sb))
                 + x86("mov", "rsi", FRQ(_.op_sb + 8))
                 + x86("call_rt", "rt_str_coerce", (long)_.op_off, (uint64_t)(uintptr_t)(void*)rt_str_coerce)
                 : x86("mov", "rax", FRQ(_.op_sb))
                 + x86("mov", FRQ(_.op_off), "rax")
                 + x86("mov", "rax", FRQ(_.op_sb + 8))
                 + x86("mov", FRQ(_.op_off + 8), "rax"))
             + x86_gamma()
             + x86_beta_trampoline()
         : x86_bomb("bb_binop_relop: shape mismatch");
}
