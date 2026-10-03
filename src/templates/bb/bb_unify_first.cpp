#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
static_assert(offsetof(VCELL_t, cellp) == 0, "the deref follows a DT_N slen 2 name through the VCELL's first word");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_first() {
    x86_begin();
    return IF(_.op_zres,
               x86_alpha()
             + x86_bomb("bb_unify_first: a Prolog head is a flat-frame graph; no ZD arm exists")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 1,
               x86_alpha()
             + x86_bomb("bb_unify_first: operand 0 is the cell source, the node's ival the child index and its sval the variable's name")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 4,
               x86_alpha()
             + x86_bomb("bb_unify_first: the variable has no frame slot")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 0,
               x86("comment", "IR_UNIFY_FIRST: a bound value is copied into the variable's slot, an unbound cell is referenced from it")
             + x86_alpha()
             + PL_SRC_RDI()
             + PL_DEREF(10, 11, 12, 13)
             + PL_UNBOUND(30, 25)
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("mov", "rdx", RDQ("rdi", 8))
             + x86("mov", FRQ(_.op_u_vo), "rax")
             + x86("mov", FRQ(_.op_u_vo + 8), "rdx")
             + x86_gamma()
             + x86("def", L(30))
             + x86("mov", "rax", (long)DT_PLVAR)
             + x86("mov", FRQ(_.op_u_vo), "rax")
             + x86("mov", FRQ(_.op_u_vo + 8), "rdi")
             + x86_gamma()
             + x86_beta_trampoline());
}
