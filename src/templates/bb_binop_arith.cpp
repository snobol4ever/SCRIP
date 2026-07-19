#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int rt_binop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    return IF(_.op_num_real && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0,
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH_REAL")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off), "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(_.op_off >= 0 && !_.op_num_real
              && ((long long)_.op_ival == BINOP_ADD || (long long)_.op_ival == BINOP_SUB || (long long)_.op_ival == BINOP_MUL
                  || (long long)_.op_ival == BINOP_DIV || (long long)_.op_ival == BINOP_MOD),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH")
         + IF(!_.op_imm_a_ok,
           x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0)))
         + IF(!_.op_imm_b_ok,
           x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0)))
         + IF(!_.op_imm_a_ok,
           x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2)))
         + IF(!_.op_imm_b_ok,
           x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2)))
         + x86("def", L(1))
         + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
         + IF(!_.op_imm_b_ok, x86("mov", "rcx", FRQ(_.op_sb + 8)))
         + IF( _.op_imm_b_ok, x86("mov", "rcx", (long)_.op_imm_b))
         + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_DIV, x86("cqo")
                               + x86("idiv", "rcx"))
         + IF((long long)_.op_ival == BINOP_MOD, x86("cqo")
                               + x86("idiv", "rcx")
                               + x86("mov", "rax", "rdx"))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(0))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("lea", "r9", FRQ(_.op_off))
         + x86("call", "rt_binop_overload", (uint64_t)(uintptr_t)(void*)rt_binop_overload)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86("def", L(2))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86_omega())
         + IF(_.op_off >= 0 && !_.op_num_real
              && ((long long)_.op_ival == BINOP_POW || (long long)_.op_ival == BINOP_CUNION || (long long)_.op_ival == BINOP_CDIFF || (long long)_.op_ival == BINOP_CINTER),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH_DYN")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86_omega());
}
