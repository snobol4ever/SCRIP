#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/builtins/gen.h"
extern int g_descr_flat_chain;
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
static inline int brr_ok() { return g_descr_flat_chain && _.op_off >= 0 && rel_is_numrel(_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop() {
    return IF(PLATFORM_X86,
           IF(brr_ok(),
              IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                            + x86("comment", std::string("BOX IR_BINOP relop [GZ-8 x86() stackless cmp + ") + rel_fail_mnem(_.op_ival) + "->ω.node; stores y->result; jmp γ]"))
            + x86("mov", "rax", FRQ(_.op_sa + 8))
            + x86("mov", "rcx", FRQ(_.op_sb + 8))
            + x86("cmp", "rax", "rcx")
            + x86(rel_fail_mnem(_.op_ival), "ω")
            + x86("mov", "rcx", FRQ(_.op_sb))
            + x86("mov", FRQ(_.op_off), "rcx")
            + x86("mov", "rcx", FRQ(_.op_sb + 8))
            + x86("mov", FRQ(_.op_off + 8), "rcx")
            + x86("jmp", "γ")
            + x86("def", "β")
            + x86("jmp", "ω"))
         + IF(!brr_ok(), x86_bomb("bb_binop_relop: shape mismatch (dispatch chose this arm but predicate failed)")));
}