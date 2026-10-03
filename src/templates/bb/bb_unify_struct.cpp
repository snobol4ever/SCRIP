#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_tr_refuse(const char *);
DESCR_t rt_pl_unify_struct_fresh(long);
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_struct() {
    x86_begin();
    return IF(_.op_zres,
               x86_alpha()
             + x86_bomb("bb_unify_struct: a Prolog head is a flat-frame graph; no ZD arm exists")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 1,
               x86_alpha()
             + x86_bomb("bb_unify_struct: operands are a cell source, a child index and an IR_LIT_INTEGER functor id")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 2,
               x86_alpha()
             + x86_bomb("bb_unify_struct: no result slot granted for the compound DESCR")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 3,
               x86_alpha()
             + x86_bomb("bb_unify_struct: the functor id names no compound functor")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 0,
               x86("comment", "IR_UNIFY_STRUCT: read mode keeps the compound DESCR in this box's own slot for its children; write mode takes a fresh block and binds the cell to it")
             + x86_alpha()
             + PL_SRC_RDI()
             + PL_DEREF(10, 11, 12, 13)
             + x86("cmp", "al", (long)DT_PLREF)
             + x86("jne", L(20))
             + x86("mov", "ecx", RDD("rdi", 4))
             + x86("cmp", "ecx", _.op_u_fid)
             + x86_omega("jne")
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("mov", "rdx", RDQ("rdi", 8))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86("def", L(20))
             + PL_UNBOUND(30, 25)
             + x86_omega()
             + x86("def", L(30))
             + x86("note", "write mode: the fresh block of self-referencing cells comes from a value service (it allocates, so the DESCR is stored into the mapped slot and the poll follows)")
             + x86("note", "the cell is re-derived after the poll and bound to the block")
             + x86("mov32", "edi", _.op_u_fid)
             + x86("call", "rt_pl_unify_struct_fresh", (uint64_t)(uintptr_t)(void *)rt_pl_unify_struct_fresh)
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + PL_SRC_RDI()
             + PL_DEREF(14, 15, 16, 17)
             + PL_TRAIL(31, 39)
             + x86("mov", "rax", FRQ(_.op_off))
             + x86("mov", "rdx", FRQ(_.op_off + 8))
             + x86("mov", RDQ("rdi", 0), "rax")
             + x86("mov", RDQ("rdi", 8), "rdx")
             + x86_gamma()
             + x86("def", L(39))
             + x86("mov", "rdi", "r12")
             + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
             + x86_beta_trampoline());
}
