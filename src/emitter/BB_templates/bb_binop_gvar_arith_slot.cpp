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
static inline int gvs_arith() { return _.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD; }
static inline int gvs_llit() { return _.bb_lk == (int)IR_LIT_I; }
static inline int gvs_lvar() { return _.bb_lk == (int)IR_VAR && _.op_name1 != 0; }
static inline int gvs_rlit() { return _.bb_rk == (int)IR_LIT_I; }
static inline int gvs_rvar() { return _.bb_rk == (int)IR_VAR && _.op_name2 != 0; }
static inline int gvs_disp(int kind) { return (kind == (int)IR_CALL || kind == (int)IR_VAR_FRAME || kind == (int)IR_VAR_FRAME_REF) ? 8 : 0; }
static inline int gvs_ok() { return g_gvar_flat_chain && _.op_off >= 0 && gvs_arith() && (gvs_llit() || gvs_lvar() || _.op_sa >= 0) && (gvs_rlit() || gvs_rvar() || _.op_sb >= 0); }
static std::string gvs_name(const char *reg, const char *n) { char b[80]; strtab_label(b, sizeof b, n); return x86_load_ro(reg, b, (uint64_t)(uintptr_t)n); }
static std::string gvs_op() {
    return IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(_.op_ival == BINOP_DIV, x86("cqo") + x86("idiv", "rcx"))
         + IF(_.op_ival == BINOP_MOD, x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"));
}
static std::string gvs_lhs() {
    return IF(gvs_llit(), x86("mov", "rax", (long)_.bb_li))
         + IF(gvs_lvar(), gvs_name("rdi", _.op_name1) + x86_call_ro("rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int))
         + IF(!gvs_llit() && !gvs_lvar(), x86("mov", "rax", FRQ(_.op_sa + gvs_disp(_.bb_lk))));
}
static std::string gvs_rhs() {
    return IF(gvs_rlit(), x86("mov", "rcx", (long)_.bb_ri))
         + IF(gvs_rvar(), x86("mov", FRQ(_.op_off), "rax")
                        + gvs_name("rdi", _.op_name2)
                        + x86_call_ro("rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int)
                        + x86("mov", "rcx", "rax")
                        + x86("mov", "rax", FRQ(_.op_off)))
         + IF(!gvs_rlit() && !gvs_rvar(), x86("mov", "rcx", FRQ(_.op_sb + gvs_disp(_.bb_rk))));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_slot() {
    return IF(PLATFORM_X86,
           IF(gvs_ok(),
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_BINOP gvar-arith-slot op=") + std::to_string(_.op_ival)
                            + " lk=" + std::to_string(_.bb_lk) + " rk=" + std::to_string(_.bb_rk) + " -> [r12+" + std::to_string(_.op_off) + "]"))
            + gvs_lhs()
            + gvs_rhs()
            + gvs_op()
            + x86("mov", FRQ(_.op_off), "rax")
            + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"))
         + IF(!gvs_ok(), x86_bomb("bb_binop_gvar_arith_slot: shape mismatch (dispatch chose this arm but predicate failed)")));
}