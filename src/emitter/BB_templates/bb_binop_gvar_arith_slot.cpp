#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/builtins/gen.h"
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_slot() {
    if (PLATFORM_X86) return IF(_.op_off >= 0
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && (_.bb_lk == (int)IR_LIT_I || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_I || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0),
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_ARITH_SLOT")
                          + IF(_.bb_lk == (int)IR_LIT_I, x86("mov", "rax", (long)_.bb_li))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0]))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(!(_.bb_lk == (int)IR_LIT_I) && !(_.bb_lk == (int)IR_VAR && _.op_name1 != 0),
                            x86("mov", "rax", FRQ(_.op_sa + ((_.bb_lk == (int)IR_CALL || _.bb_lk == (int)IR_VAR_FRAME || _.bb_lk == (int)IR_VAR_FRAME_REF) ? 8 : 0))))
                          + IF(_.bb_rk == (int)IR_LIT_I, x86("mov", "rcx", (long)_.bb_ri))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", FRQ(_.op_off), "rax"))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1]))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", "rcx", "rax"))
                          + IF(_.bb_rk == (int)IR_VAR && _.op_name2 != 0, x86("mov", "rax", FRQ(_.op_off)))
                          + IF(!(_.bb_rk == (int)IR_LIT_I) && !(_.bb_rk == (int)IR_VAR && _.op_name2 != 0),
                            x86("mov", "rcx", FRQ(_.op_sb + ((_.bb_rk == (int)IR_CALL || _.bb_rk == (int)IR_VAR_FRAME || _.bb_rk == (int)IR_VAR_FRAME_REF) ? 8 : 0))))
                          + IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
                          + IF(_.op_ival == BINOP_DIV, x86("cqo"))
                          + IF(_.op_ival == BINOP_DIV, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("cqo"))
                          + IF(_.op_ival == BINOP_MOD, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("mov", "rax", "rdx"))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86("jmp", "γ")
                          + x86("def", "β")
                          + x86("jmp", "ω"))
                          + IF(!(_.op_off >= 0
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && (_.bb_lk == (int)IR_LIT_I || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)
                              && (_.bb_rk == (int)IR_LIT_I || (_.bb_rk == (int)IR_VAR && _.op_name2 != 0) || _.op_sb >= 0)),
                            x86_bomb("bb_binop_gvar_arith_slot: shape mismatch (dispatch chose this arm but predicate failed)"));
    return std::string();
}
