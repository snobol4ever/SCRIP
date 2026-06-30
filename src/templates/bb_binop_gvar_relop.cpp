#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
extern int64_t rt_gvar_get_int(const char *name);
extern DESCR_t rt_gvar_get_descr(const char *name);
extern int64_t rt_relop_descr2(int64_t l_lo, int64_t l_hi, int64_t r_lo, int64_t r_hi, int op);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_relop() {
    if (PLATFORM_X86) {
        bool P = (g_gvar_flat_chain && _.op_off >= 0
                  && _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE
                  && (_.bb_lk == (int)IR_LIT_INTEGER || _.bb_lk == (int)IR_OP_COUNT || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                  && (_.bb_rk == (int)IR_LIT_INTEGER || _.bb_rk == (int)IR_OP_COUNT || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0));
        bool D = (_.op_relop_descr
                  && (_.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE)
                  && (_.bb_lk == (int)IR_CALL || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || (_.bb_lk == (int)IR_LIT_STRING && _.op_parts_lbl[0] != 0))
                  && (_.bb_rk == (int)IR_CALL || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || (_.bb_rk == (int)IR_LIT_STRING && _.op_parts_lbl[1] != 0)));
        return IF(_.op_relop_descr && !D, x86_bomb("bb_binop_gvar_relop: descr arm shape mismatch (op_relop_descr set but operands unexpected)"))
             + IF(D,
                 x86("label", _.lbl_α)
               + x86("comment", "IR_BINOP_GVAR_RELOP_DESCR")
               + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0,
                   x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
                 + x86("call", "rt_gvar_get_descr", (uint64_t)(uintptr_t)(void *) rt_gvar_get_descr)
                 + x86("mov", FRQ(_.op_off), "rax")
                 + x86("mov", FRQ(_.op_off + 8), "rdx"))
               + IF(_.bb_lk == (int)IR_CALL,
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_off), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_off + 8), "rax"))
               + IF(_.bb_lk == (int)IR_LIT_STRING && _.op_parts_lbl[0] != 0,
                   x86("mov", FRQ(_.op_off), (long) DT_S)
                 + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)(_.op_parts_str[0] ? _.op_parts_str[0] : ""), _.op_parts_lbl[0])
                 + x86("mov", FRQ(_.op_off + 8), "rax"))
               + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0,
                   x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
                 + x86("call", "rt_gvar_get_descr", (uint64_t)(uintptr_t)(void *) rt_gvar_get_descr)
                 + x86("mov", "rcx", "rdx")
                 + x86("mov", "rdx", "rax"))
               + IF(_.bb_rk == (int)IR_CALL,
                   x86("mov", "rdx", FRQ(_.op_sb))
                 + x86("mov", "rcx", FRQ(_.op_sb + 8)))
               + IF(_.bb_rk == (int)IR_LIT_STRING && _.op_parts_lbl[1] != 0,
                   x86("mov", "rdx", (long) DT_S)
                 + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(_.op_parts_str[1] ? _.op_parts_str[1] : ""), _.op_parts_lbl[1]))
               + x86("mov", "rdi", FRQ(_.op_off))
               + x86("mov", "rsi", FRQ(_.op_off + 8))
               + x86("mov", "r8", (long) _.op_ival)
               + x86("call", "rt_relop_descr2", (uint64_t)(uintptr_t)(void *) rt_relop_descr2)
               + x86("test", "rax", "rax")
               + x86("je", "ω")
               + x86("jmp", "γ")
               + x86("def", "β")
               + x86("jmp", "ω"))
             + IF(!_.op_relop_descr && !P, x86_bomb("bb_binop_gvar_relop: shape mismatch (dispatch chose this arm but predicate failed)"))
             + IF(!_.op_relop_descr && P,
                 x86("label", _.lbl_α)
               + x86("comment", "IR_BINOP_GVAR_RELOP")
               + IF(_.bb_lk == (int)IR_LIT_INTEGER || _.bb_lk == (int)IR_OP_COUNT, x86("mov", "rax", (long) _.bb_li))
               + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0,
                 x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
               + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
               + IF(!(_.bb_lk == (int)IR_LIT_INTEGER || _.bb_lk == (int)IR_OP_COUNT) && !(_.bb_lk == (int)IR_VAR && _.op_name1 != 0),
                 x86("mov", "rax", FRQ(_.op_sa + ((_.bb_lk == (int)IR_CALL || _.bb_lk == (int)IR_OP_COUNT || _.bb_lk == (int)IR_OP_COUNT) ? 8 : 0))))
               + IF(_.bb_rk == (int)IR_LIT_INTEGER || _.bb_rk == (int)IR_OP_COUNT, x86("mov", "rcx", (long) _.bb_ri))
               + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0,
                 x86("mov", FRQ(_.op_off), "rax")
               + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
               + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int)
               + x86("mov", "rcx", "rax")
               + x86("mov", "rax", FRQ(_.op_off)))
               + IF(!(_.bb_rk == (int)IR_LIT_INTEGER || _.bb_rk == (int)IR_OP_COUNT) && !(_.bb_rk == (int)IR_VAR && _.op_name2 != 0),
                 x86("mov", "rcx", FRQ(_.op_sb + ((_.bb_rk == (int)IR_CALL || _.bb_rk == (int)IR_OP_COUNT || _.bb_rk == (int)IR_OP_COUNT) ? 8 : 0))))
               + x86("cmp", "rax", "rcx")
               + IF(_.op_ival == BINOP_LT, x86("jge", "ω"))
               + IF(_.op_ival == BINOP_LE, x86("jg",  "ω"))
               + IF(_.op_ival == BINOP_GT, x86("jle", "ω"))
               + IF(_.op_ival == BINOP_GE, x86("jl",  "ω"))
               + IF(_.op_ival == BINOP_EQ, x86("jne", "ω"))
               + IF(_.op_ival == BINOP_NE, x86("je",  "ω"))
               + x86("jmp", "γ")
               + x86("def", "β")
               + x86("jmp", "ω"));
    }
    return std::string();
}
