#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../runtime/builtins/gen.h"
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop_gvar_slot() {
    return IF(_.op_off >= 0
                              && (_.op_ival == TT_MNS || _.op_ival == TT_PLS)
                              && (_.bb_lk == (int)IR_LIT_INTEGER || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0),
                            x86_alpha()
                          + x86("comment", "IR_UNOP_GVAR_SLOT")
                          + IF(_.bb_lk == (int)IR_LIT_INTEGER, x86("mov", "rax", (long)_.bb_li))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0]))
                          + IF(_.bb_lk == (int)IR_VAR && _.op_name1 != 0, x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF(!(_.bb_lk == (int)IR_LIT_INTEGER) && !(_.bb_lk == (int)IR_VAR && _.op_name1 != 0),
                            x86("mov", "rax", FRQ(_.op_sa + ((_.bb_lk == (int)IR_CALL || _.bb_lk == (int)IR_EXCISED || _.bb_lk == (int)IR_EXCISED) ? 8 : 0))))
                          + IF(_.op_ival == TT_MNS, x86("neg", "rax"))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86_gamma()
                          + x86_beta_trampoline())
                          + IF(!(_.op_off >= 0
                              && (_.op_ival == TT_MNS || _.op_ival == TT_PLS)
                              && (_.bb_lk == (int)IR_LIT_INTEGER || (_.bb_lk == (int)IR_VAR && _.op_name1 != 0) || _.op_sa >= 0)),
                            x86_bomb("bb_unop_gvar_slot: shape mismatch (dispatch chose this arm but predicate failed)"));
}
