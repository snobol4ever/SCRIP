#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_activate() {
    x86_begin();
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_activate: op_off < 0 (no slot assigned -- IR_ACTIVATE missing from ir_node_produces_value?)"))
         + IF(_.op_off >= 0 && _.op_sa < 0, x86_alpha() + x86_bomb("bb_activate: no coexpression operand slot (operand[0] unregistered -- LOWER/BFS wiring bug)"))
         + IF(_.op_off >= 0 && _.op_sa >= 0,
               x86("comment", "IR_ACTIVATE")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_sa + 8))
             + IF(_.op_sb >= 0,
                   x86("mov", "rsi", FRQ(_.op_sb))
                 + x86("mov", "rdx", FRQ(_.op_sb + 8)))
             + IF(_.op_sb < 0,
                   x86("xor", "esi", "esi")
                 + x86("xor", "edx", "edx"))
             + x86("lea",  "rcx", FRQ(_.op_off))
             + x86("call", "scrip_coexpr_activate", (uint64_t)(uintptr_t)(void *)scrip_coexpr_activate)
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86_gamma()
             + x86_beta_trampoline());
}
