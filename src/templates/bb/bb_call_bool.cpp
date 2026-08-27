#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "../runtime/rt/rt.h"
#include "../runtime/builtins/gen.h"
int bb_slot_get(IR_t *nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rkbool_arg_is_relop(IR_t * a0) {
    return a0 && a0->op == IR_BINOP && IR_LIT(a0).ival >= BINOP_LT && IR_LIT(a0).ival <= BINOP_NE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_bool_str(IR_t * pBB) {
    IR_t * a0 = ir_call_arg(_.node, 0);
    if (rkbool_arg_is_relop(a0))
        return x86_alpha()
             + x86("comment", "BOX __rk_bool [relop pass-through: BINOP already branched γ/ω]")
             + x86_gamma()
             + x86_beta_trampoline();
    int off = _.op_a_slot;
    if (off < 0) return x86_alpha() + x86_bomb("bb_call_bool: arg slot not allocated");
    return x86_alpha()
         + x86("comment", "BOX __rk_bool [descr flat-chain: slot truthiness test]")
         + x86("mov", "rdi", FRQ(off))
         + x86("mov", "rsi", FRQ(off + 8))
         + x86("call", "rt_is_truthy", (uint64_t)(uintptr_t)(void *)rt_is_truthy)
         + x86("test", "eax", "eax")
         + x86_omega("je")
         + x86_gamma()
         + x86_beta_trampoline();
}
