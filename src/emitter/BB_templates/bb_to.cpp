#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
#include "../../runtime/builtins/gen.h"
extern int g_descr_flat_chain;
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int64_t bb_to_by() { return (_.op_node_kind == (int)IR_OP_COUNT && _.op_ival) ? _.op_ival : 1; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0 && bb_to_by() != 0))
        return x86_bomb("bb_to: unhandled (needs static operands, nonzero by, descr flat-chain)");
    if (_.op_num_real) {
        int cmp_op = (bb_to_by() > 0) ? (int)BINOP_LE : (int)BINOP_GE;
        return x86("comment", "IR_TO real")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(10))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)cmp_op)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86("jz",      "ω")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 24))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", ROQ(0))
             + x86("mov",     "rcx", ROQ(1))
             + x86("mov",     "r8d", (long)BINOP_ADD)
             + x86("call",    "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("jmp",     L(10))
             + x86_ro_seal_q(0, (uint64_t)(int64_t)DT_R)
             + x86_ro_seal_q(1, (uint64_t)(int64_t)bb_to_by());
    }
    return x86("comment", "IR_TO")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rax", FRQ(_.op_sa + 8))
         + x86("mov",     FRQ(_.op_off + 16), "rax")
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("mov",     "rcx", FRQ(_.op_sb + 8))
         + x86("cmp",     "rax", "rcx")
         + IF(bb_to_by() > 0, x86("jg", "ω"))
         + IF(bb_to_by() < 0, x86("jl", "ω"))
         + x86("mov",     FRQ(_.op_off),     (long)DT_I)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + IF(bb_to_by() == 1, x86("inc", FRQ(_.op_off + 16)))
         + IF(bb_to_by() != 1, x86("mov", "rax", FRQ(_.op_off + 16))
                             + x86("add", "rax", (long)bb_to_by())
                             + x86("mov", FRQ(_.op_off + 16), "rax"))
         + x86("jmp",     L(0));
}
