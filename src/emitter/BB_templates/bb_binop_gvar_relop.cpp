#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_relop() {
    if (PLATFORM_X86) return IF(!(g_gvar_flat_chain && _.op_off >= 0
                              && _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE
                              && (_.bb_lk == (int)IR_LIT_I || _.bb_lk == (int)IR_LIT_NUL || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_I || _.bb_rk == (int)IR_LIT_NUL || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0)),
                            x86_bomb("bb_binop_gvar_relop: shape mismatch (dispatch chose this arm but predicate failed)"))
                          + IF(g_gvar_flat_chain && _.op_off >= 0
                              && _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE
                              && (_.bb_lk == (int)IR_LIT_I || _.bb_lk == (int)IR_LIT_NUL || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_I || _.bb_rk == (int)IR_LIT_NUL || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0),
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_RELOP")
                          + IF(_.bb_lk == (int)IR_LIT_I || _.bb_lk == (int)IR_LIT_NUL, x86("mov", "rax", (long) _.bb_li))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0,
                            x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
                          + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(!(_.bb_lk == (int)IR_LIT_I || _.bb_lk == (int)IR_LIT_NUL) && !(_.bb_lk == (int)IR_VAR && _.op_name1 != 0),
                            x86("mov", "rax", FRQ(_.op_sa + ((_.bb_lk == (int)IR_CALL || _.bb_lk == (int)IR_VAR_FRAME || _.bb_lk == (int)IR_VAR_FRAME_REF) ? 8 : 0))))
                          + IF(_.bb_rk == (int)IR_LIT_I || _.bb_rk == (int)IR_LIT_NUL, x86("mov", "rcx", (long) _.bb_ri))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0,
                            x86("mov", FRQ(_.op_off), "rax")
                          + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
                          + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int)
                          + x86("mov", "rcx", "rax")
                          + x86("mov", "rax", FRQ(_.op_off)))
                          + IF(!(_.bb_rk == (int)IR_LIT_I || _.bb_rk == (int)IR_LIT_NUL) && !(_.bb_rk == (int)IR_VAR && _.op_name2 != 0),
                            x86("mov", "rcx", FRQ(_.op_sb + ((_.bb_rk == (int)IR_CALL || _.bb_rk == (int)IR_VAR_FRAME || _.bb_rk == (int)IR_VAR_FRAME_REF) ? 8 : 0))))
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
    return std::string();
}
