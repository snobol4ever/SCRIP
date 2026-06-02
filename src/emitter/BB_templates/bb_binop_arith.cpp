#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/builtins/gen.h"
extern int g_icn_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_icn_flat_chain && _.op_off >= 0)) return std::string();
    int64_t op  = _.op_ival;
    if (!(op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD)) return std::string();
    int     sa  = _.op_sa, sb = _.op_sb, off = _.op_off;
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
                          + s_comment(emit_fmt("# BOX IR_BINOP arith op=%lld [GZ-9 x86() self-encoding, stackless slot->slot DESCR]", (long long)op)))
         + x86("mov", "rax", FRQ(sa + 8))
         + x86("mov", "rcx", FRQ(sb + 8))
         + opb
         + x86("mov", FRQ(off),     (long)DT_I)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_arith(IR_t * pBB) {
    (void)pBB;
    std::string s = bb_binop_arith_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_binop_arith: shape mismatch (dispatch chose this arm but predicate failed)")); return; }
    bb_emit_x86(s);
}
