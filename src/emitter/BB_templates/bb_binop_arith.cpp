#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/builtins/gen.h"
extern int g_descr_flat_chain;
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int rt_binop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long long bo() { return (long long)_.op_ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    if (g_descr_flat_chain && _.op_num_real && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0)
        return x86("label", _.lbl_α)
             + x86("comment", "IR_BINOP_ARITH_REAL")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8d", (long)_.op_ival)
             + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86("je", "ω")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    return IF(g_descr_flat_chain && _.op_off >= 0 && !_.op_num_real && (bo() == BINOP_ADD || bo() == BINOP_SUB || bo() == BINOP_MUL || bo() == BINOP_DIV || bo() == BINOP_MOD),
           x86("label", _.lbl_α)
         + x86("comment", "IR_BINOP_ARITH")
         + x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0))
         + x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0))
         + x86("def", L(1))
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + IF(bo() == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF(bo() == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF(bo() == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(bo() == BINOP_DIV, x86("cqo")
                               + x86("idiv", "rcx"))
         + IF(bo() == BINOP_MOD, x86("cqo")
                               + x86("idiv", "rcx")
                               + x86("mov", "rax", "rdx"))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "γ")
         + x86("def", L(0))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("lea", "r9", FRQ(_.op_off))
         + x86("call", "rt_binop_overload", (uint64_t)(uintptr_t)(void*)rt_binop_overload)
         + x86("test", "eax", "eax")
         + x86("je", L(1))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"));
}
