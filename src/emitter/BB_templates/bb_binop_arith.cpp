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
static inline long long bo() { return (long long)_.op_ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    return IF(PLATFORM_X86 && g_descr_flat_chain && _.op_off >= 0 && (bo() == BINOP_ADD || bo() == BINOP_SUB || bo() == BINOP_MUL || bo() == BINOP_DIV || bo() == BINOP_MOD),
           x86("label", _.lbl_α)
         + x86("comment", "IR_BINOP_ARITH")
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + IF(bo() == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF(bo() == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF(bo() == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(bo() == BINOP_DIV, x86("cqo")
                               + x86("idiv", "rcx"))
         + IF(bo() == BINOP_MOD, x86("cqo")
                               + x86("idiv", "rcx")
                               + x86("mov", "rax", "rdx"))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"));
}
