#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
extern int64_t rt_gvar_get_int(const char *name);
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_slot() {
    if (PLATFORM_X86 && _.op_arith_descr && _.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0)
        return x86_alpha()
             + x86("comment", "IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)")
             + IF(_.bb_lk == (int)IR_VAR && _.op_name1 && _.op_parts_lbl[0], x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0]))
             + IF(_.bb_lk == (int)IR_VAR && _.op_name1 && _.op_parts_lbl[0], x86("call_rt", "NV_GET_fn", (long)_.op_sa, (uint64_t)(uintptr_t)(void *) NV_GET_fn))
             + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("movabs", "rax", (uint64_t)DT_I))
             + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("mov", FRQ(_.op_sa), "rax"))
             + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("movabs", "rax", (uint64_t)_.bb_li))
             + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("mov", FRQ(_.op_sa + 8), "rax"))
             + IF(_.bb_rk == (int)IR_VAR && _.op_name2 && _.op_parts_lbl[1], x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1]))
             + IF(_.bb_rk == (int)IR_VAR && _.op_name2 && _.op_parts_lbl[1], x86("call_rt", "NV_GET_fn", (long)_.op_sb, (uint64_t)(uintptr_t)(void *) NV_GET_fn))
             + IF(_.bb_rk == (int)IR_LIT_INTEGER, x86("movabs", "rax", (uint64_t)DT_I))
             + IF(_.bb_rk == (int)IR_LIT_INTEGER, x86("mov", FRQ(_.op_sb), "rax"))
             + IF(_.bb_rk == (int)IR_LIT_INTEGER, x86("movabs", "rax", (uint64_t)_.bb_ri))
             + IF(_.bb_rk == (int)IR_LIT_INTEGER, x86("mov", FRQ(_.op_sb + 8), "rax"))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8d", (long)_.op_ival)
             + x86("rtcc_wb")
             + x86("call_bare", "rt_num_arith", (uint64_t)(uintptr_t)(void *) rt_num_arith)
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("rtcc_rl")
             + x86_gamma() + x86_beta() + x86_omega();
    return PLATFORM_X86 ? (IF(_.op_off >= 0
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && (_.bb_lk == (int)IR_LIT_INTEGER || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_INTEGER || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0),
                            x86_alpha()
                          + x86("comment", "IR_BINOP_GVAR_ARITH_SLOT")
                          + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("mov", "rax", (long)_.bb_li))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0]))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(!(_.bb_lk == (int)IR_LIT_INTEGER) && !(_.bb_lk == (int)IR_VAR && _.op_name1 != 0),
                            x86("mov", "rax", FRQ(_.op_sa + ((_.bb_lk == (int)IR_CALL || _.bb_lk == (int)IR_OP_COUNT || _.bb_lk == (int)IR_OP_COUNT) ? 8 : 0))))
                          + IF(_.bb_rk == (int)IR_LIT_INTEGER, x86("mov", "rcx", (long)_.bb_ri))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", FRQ(_.op_off), "rax"))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1]))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", "rcx", "rax"))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", "rax", FRQ(_.op_off)))
                          + IF(!(_.bb_rk == (int)IR_LIT_INTEGER) && !(_.bb_rk == (int)IR_VAR && _.op_name2 != 0),
                            x86("mov", "rcx", FRQ(_.op_sb + ((_.bb_rk == (int)IR_CALL || _.bb_rk == (int)IR_OP_COUNT || _.bb_rk == (int)IR_OP_COUNT) ? 8 : 0))))
                          + IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
                          + IF(_.op_ival == BINOP_DIV, x86("cqo"))
                          + IF(_.op_ival == BINOP_DIV, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("cqo"))
                          + IF(_.op_ival == BINOP_MOD, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("mov", "rax", "rdx"))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86_gamma()
                          + x86_beta_trampoline())
                          + IF(!(_.op_off >= 0
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && (_.bb_lk == (int)IR_LIT_INTEGER || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_INTEGER || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0)),
                            x86_bomb("bb_binop_gvar_arith_slot: shape mismatch (dispatch chose this arm but predicate failed)"))) : std::string();
}
