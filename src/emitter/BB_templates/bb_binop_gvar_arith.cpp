/* bb_binop_gvar_arith.cpp — BB box: IR_BINOP integer arithmetic for the global variable (gvar) flat-chain.
   SNOBOL4 `OUTPUT = 2 + 3` path. x86() self-encoding (template-revamp). pBB-FREE: reads ONLY g_emit
   (FACT RULE). The driver deposits op_sa=lhs_ival, op_sb=rhs_ival (literal int32 values, NOT frame
   slot offsets), op_ival=op-code, op_off=8-byte ζ-slot for the raw int64 result. The consuming
   bb_gvar_assign int-binop arm reads [r12+op_off] (8 bytes) and calls rt_gvar_assign_int(name, val).
   SPITBOL semantics (manual ch.3): integer add/sub/mul/div/mod; div truncates toward zero.
   Guard: g_gvar_flat_chain && op_off>=0. op_sa/op_sb are immediates, not slot offsets. */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/interp/gen.h"
extern int g_gvar_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return std::string();
    int64_t op  = _.op_ival;
    if (!(op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD)) return std::string();
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
