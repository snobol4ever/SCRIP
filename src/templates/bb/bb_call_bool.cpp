#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "../runtime/rt/rt.h"
#include "../runtime/builtins/gen.h"
int bb_slot_get(IR_t *nd);
int rk_is_truthy(DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_bool_relop() {
    return x86_alpha()
         + x86("comment", "BOX __rk_bool [relop pass-through: BINOP already branched γ/ω]")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_bool_slot() {
    return IF(_.op_a_slot < 0, x86_alpha() + x86_bomb("bb_call_bool: arg slot not allocated"))
         + IF(_.op_a_slot >= 0,
               x86_alpha()
             + x86("comment", "BOX __rk_bool [descr flat-chain: slot truthiness test]")
             + x86("mov", "rdi", _.op_zres ? ZOPQ(0, 0) : FRQ(_.op_a_slot))
             + x86("mov", "rsi", _.op_zres ? ZOPQ(0, 8) : FRQ(_.op_a_slot + 8))
             + x86("call", "rk_is_truthy", (uint64_t)(uintptr_t)(void *)rk_is_truthy)
             + x86("test", "eax", "eax")
             + x86_omega("je")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline());
}
