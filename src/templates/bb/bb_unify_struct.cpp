#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_tr_refuse(const char *);
int prolog_functor_arity(int);
#include "rt/gc_heap.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_fresh_cells(int ar) {
    std::string s;
    if (ar <= 8) {
        for (int i = 0; i < ar; i++)
            s += x86("mov", RDQ("r10", 16 * i), (long)DT_PLVAR) + x86("lea", "rcx", RDQ("r10", 16 * i)) + x86("mov", RDQ("r10", 16 * i + 8), "rcx");
        return s;
    }
    return x86("mov", "r11", (long)ar)
         + x86("def", L(40))
         + x86("mov", RDQ("r10", 0), (long)DT_PLVAR)
         + x86("mov", RDQ("r10", 8), "r10")
         + x86("add", "r10", 16L)
         + x86("sub", "r11", 1L)
         + x86("jne", L(40));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_struct() {
    x86_begin();
    const int ar = (!_.op_zres && _.op_u_why == 0) ? prolog_functor_arity((int)_.op_u_fid) : 1;
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
             + x86("note", "write mode (ARCH-PROLOG-C-OUT-OF-THE-BOX 10): the box builds the fresh block -- one allocating call for the argument block (HB_DVEC, zero-filled by the allocator, "
                           "so every cell is typed before the poll), the result cell {DT_PLREF, functor id, block} stored into the mapped slot before the poll, then each argument cell made a "
                           "self-reference inline; the cell is re-derived after the poll and bound to the block. No C value service builds it.")
             + x86("mov32", "edi", (long)HB_DVEC)
             + x86("mov32", "esi", (long)(16 * ar))
             + x86("call", "rt_gcheap_alloc", (uint64_t)(uintptr_t)(void *)rt_gcheap_alloc)
             + x86_movabs_r64("rcx", (uint64_t)DT_PLREF | ((uint64_t)(uint32_t)_.op_u_fid << 32))
             + x86("mov", FRQ(_.op_off), "rcx")
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_rt_gc_poll()
             + x86("mov", "r10", FRQ(_.op_off + 8))
             + pl_fresh_cells(ar)
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
