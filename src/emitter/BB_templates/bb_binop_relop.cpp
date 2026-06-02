#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
extern int g_icn_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static int rel_is_numrel(int64_t op) { return op >= BINOP_LT && op <= BINOP_NE; }
static const char * rel_fail_mnem(int64_t op) {
    switch (op) {
    case BINOP_LT: return "jge";
    case BINOP_LE: return "jg";
    case BINOP_GT: return "jle";
    case BINOP_GE: return "jl";
    case BINOP_EQ: return "jne";
    case BINOP_NE: return "je";
    default:       return "jmp";
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop_str() {
    if (!PLATFORM_X86) return std::string();
    int64_t op = _.op_ival;
    if (!(g_icn_flat_chain && _.op_off >= 0 && rel_is_numrel(op))) return std::string();
    int sa = _.op_sa, sb = _.op_sb;
    if (sa < 0 || sb < 0) return std::string();
    const char * mnem = rel_fail_mnem(op);
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_BINOP relop [GZ-8 x86() stackless cmp + %s->omega; jmp gamma]", mnem)))
         + x86("mov", "rax", FRQ(sa + 8))
         + x86("mov", "rcx", FRQ(sb + 8))
         + x86("cmp", "rax", "rcx")
         + x86(mnem,  PORT_OMEGA)
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
