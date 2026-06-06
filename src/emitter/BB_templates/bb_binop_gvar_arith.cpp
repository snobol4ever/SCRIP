#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
extern int64_t rt_gvar_arith(const char *a, const char *b, int op);
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int bga_ok() { return g_gvar_flat_chain && _.op_off >= 0 && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD); }
static std::string bga_name(const char *reg, const char *n) { char b[80]; strtab_label(b, sizeof b, n); return x86_load_ro(reg, b, (uint64_t)(uintptr_t)n); }
static std::string bga_arith(int64_t op) {
    return IF(op == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF(op == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF(op == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(op == BINOP_DIV, x86("cqo") + x86("idiv", "rcx"))
         + IF(op == BINOP_MOD, x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_str() {
    return IF(PLATFORM_X86,
           IF(bga_ok() && _.op_name1 && _.op_name2,
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", "BOX IR_BINOP gvar-arith VAR+VAR [RO name ptrs, FRQ slot, @PLT]"))
            + bga_name("rdi", _.op_name1)
            + bga_name("rsi", _.op_name2)
            + x86("mov", "rdx", (long)_.op_ival)
            + x86_call_ro("rt_gvar_arith", (uint64_t)(uintptr_t)(void *)rt_gvar_arith)
            + x86("mov", FRQ(_.op_off), "rax")
            + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"))
         + IF(bga_ok() && (_.op_name1 || _.op_name2) && !(_.op_name1 && _.op_name2),
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_BINOP gvar-arith ") + (_.op_name1 ? "VAR+LIT" : "LIT+VAR")
                            + " op=" + std::to_string(_.op_ival) + " -> [r12+" + std::to_string(_.op_off) + "]"))
            + bga_name("rdi", _.op_name1 ? _.op_name1 : _.op_name2)
            + x86_call_ro("rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int)
            + IF( _.op_name1, x86("mov", "rcx", (long)_.op_sb))
            + IF(!_.op_name1, x86("mov", "rcx", "rax") + x86("mov", "rax", (long)_.op_sa))
            + bga_arith(_.op_ival)
            + x86("mov", FRQ(_.op_off), "rax")
            + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"))
         + IF(bga_ok() && !_.op_name1 && !_.op_name2,
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_BINOP gvar-arith lhs=") + std::to_string(_.op_sa)
                            + " op=" + std::to_string(_.op_ival) + " rhs=" + std::to_string(_.op_sb) + " -> [r12+" + std::to_string(_.op_off) + "]"))
            + x86("mov", "rax", (long)_.op_sa)
            + x86("mov", "rcx", (long)_.op_sb)
            + bga_arith(_.op_ival)
            + x86("mov", FRQ(_.op_off), "rax")
            + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"))
         + IF(!bga_ok(), x86_bomb("bb_binop_gvar_arith: shape mismatch (dispatch chose this arm but predicate failed)")));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gvar_arith(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_binop_gvar_arith_str()); }
