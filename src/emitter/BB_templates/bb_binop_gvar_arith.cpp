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
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return std::string();
    int64_t op  = _.op_ival;
    if (!(op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD)) return std::string();
    if (_.op_name1 && _.op_name2) {
        if (MEDIUM_TEXT) return x86_bomb("bb_binop_gvar_arith VAR+VAR: TEXT(mode-4) needs RO-interned var-name ptrs (SNOBOL m4 pending LOWER four-port wiring)");
        return x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)_.op_name1)
             + x86_load_ro("rsi", "??", (uint64_t)(uintptr_t)_.op_name2)
             + x86("mov",  "rdx", (long)op)
             + x86("call", "rt_gvar_arith", (uint64_t)(uintptr_t)(void *)rt_gvar_arith)
             + x86("mov",  FRQ(_.op_off), "rax")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    long    lhs = (long)_.op_sa;
    long    rhs = (long)_.op_sb;
    int     off = _.op_off;
    std::string opb;
    switch (op) {
    case BINOP_ADD: opb = x86("add",  "rax", "rcx"); break;
    case BINOP_SUB: opb = x86("sub",  "rax", "rcx"); break;
    case BINOP_MUL: opb = x86("imul", "rax", "rcx"); break;
    case BINOP_DIV: opb = x86("cqo") + x86("idiv", "rcx"); break;
    case BINOP_MOD: opb = x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
    default:        opb = x86("add",  "rax", "rcx"); break;
    }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_BINOP gvar-arith lhs=%ld op=%lld rhs=%ld -> [r12+%d]", lhs, (long long)op, rhs, off)))
         + x86("mov", "rax", lhs)
         + x86("mov", "rcx", rhs)
         + opb
         + x86("mov", FRQ(off), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gvar_arith(IR_t * pBB) {
    (void)pBB;
    std::string s = bb_binop_gvar_arith_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_binop_gvar_arith: shape mismatch (dispatch chose this arm but predicate failed)")); return; }
    bb_emit_x86(s);
}
