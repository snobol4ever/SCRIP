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
static inline int gvr_numrel() { return _.op_ival >= BINOP_LT && _.op_ival <= BINOP_NE; }
static inline int gvr_llit() { return _.bb_lk == (int)IR_LIT_I; }
static inline int gvr_lvar() { return _.bb_lk == (int)IR_VAR && _.op_name1 != 0; }
static inline int gvr_rlit() { return _.bb_rk == (int)IR_LIT_I; }
static inline int gvr_rvar() { return _.bb_rk == (int)IR_VAR && _.op_name2 != 0; }
static inline int gvr_disp(int kind) { return (kind == (int)IR_CALL || kind == (int)IR_VAR_FRAME || kind == (int)IR_VAR_FRAME_REF) ? 8 : 0; }
static inline int gvr_ok() { return g_gvar_flat_chain && _.op_off >= 0 && gvr_numrel() && (gvr_llit() || gvr_lvar() || _.op_sa >= 0) && (gvr_rlit() || gvr_rvar() || _.op_sb >= 0); }
static inline const char * gvr_mnem() {
    return _.op_ival == BINOP_LT ? "jge" : _.op_ival == BINOP_LE ? "jg" : _.op_ival == BINOP_GT ? "jle"
         : _.op_ival == BINOP_GE ? "jl"  : _.op_ival == BINOP_EQ ? "jne" : "je";
}
static std::string gvr_name(const char *reg, const char *n) { char b[80]; strtab_label(b, sizeof b, n); return x86_load_ro(reg, b, (uint64_t)(uintptr_t)n); }
static std::string gvr_jcc() {
    return IF(_.op_ival == BINOP_LT, x86("jge", "ω"))
         + IF(_.op_ival == BINOP_LE, x86("jg",  "ω"))
         + IF(_.op_ival == BINOP_GT, x86("jle", "ω"))
         + IF(_.op_ival == BINOP_GE, x86("jl",  "ω"))
         + IF(_.op_ival == BINOP_EQ, x86("jne", "ω"))
         + IF(_.op_ival == BINOP_NE, x86("je",  "ω"));
}
static std::string gvr_lhs() {
    return IF(gvr_llit(), x86("mov", "rax", (long)_.bb_li))
         + IF(gvr_lvar(), gvr_name("rdi", _.op_name1) + x86_call_ro("rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int))
         + IF(!gvr_llit() && !gvr_lvar(), x86("mov", "rax", FRQ(_.op_sa + gvr_disp(_.bb_lk))));
}
static std::string gvr_rhs() {
    return IF(gvr_rlit(), x86("mov", "rcx", (long)_.bb_ri))
         + IF(gvr_rvar(), x86("mov", FRQ(_.op_off), "rax")
                        + gvr_name("rdi", _.op_name2)
                        + x86_call_ro("rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int)
                        + x86("mov", "rcx", "rax")
                        + x86("mov", "rax", FRQ(_.op_off)))
         + IF(!gvr_rlit() && !gvr_rvar(), x86("mov", "rcx", FRQ(_.op_sb + gvr_disp(_.bb_rk))));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_relop_str() {
    return IF(PLATFORM_X86,
           IF(gvr_ok(),
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_BINOP gvar-relop op=") + std::to_string(_.op_ival)
                            + " lk=" + std::to_string(_.bb_lk) + " rk=" + std::to_string(_.bb_rk) + " [stackless cmp + " + gvr_mnem() + "->omega; jmp gamma]"))
            + gvr_lhs()
            + gvr_rhs()
            + x86("cmp", "rax", "rcx")
            + gvr_jcc()
            + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"))
         + IF(!gvr_ok(), x86_bomb("bb_binop_gvar_relop: shape mismatch (dispatch chose this arm but predicate failed)")));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gvar_relop(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_binop_gvar_relop_str()); }
