#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_tr_refuse(const char *);
int rt_pl_unify_value(DESCR_t *, DESCR_t *);
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_value() {
    x86_begin();
    return IF(_.op_zres,
               x86_alpha()
             + x86_bomb("bb_unify_value: a Prolog head is a flat-frame graph; no ZD arm exists")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 1,
               x86_alpha()
             + x86_bomb("bb_unify_value: operand 0 is the cell source, the node's ival the child index and its sval the variable's name")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 4,
               x86_alpha()
             + x86_bomb("bb_unify_value: the variable has no frame slot")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 0,
               x86("comment", "IR_UNIFY_VALUE: an unbound side binds to the other inline, two atomics compare inline, anything else goes to the general-unify leaf")
             + x86_alpha()
             + PL_SRC_RDI()
             + PL_DEREF(10, 11, 12, 13)
             + x86("mov", "r8", "rdi")
             + x86("lea", "rdi", FRQ(_.op_u_vo))
             + PL_DEREF(14, 15, 16, 17)
             + x86("mov", "rsi", "rdi")
             + x86("mov", "rdi", "r8")
             + x86("note", "rdi = the cell, rsi = the variable's cell, both dereferenced; eax = the cell's tag, ecx = the variable's")
             + x86("mov", "eax", RDD("rdi", 0))
             + x86("and", "eax", 255L)
             + x86("mov", "ecx", RDD("rsi", 0))
             + x86("and", "ecx", 255L)
             + x86("cmp", "eax", 0L)
             + x86("je", L(40))
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86("je", L(40))
             + x86("cmp", "eax", (long)DT_PLVAR)
             + x86("je", L(40))
             + x86("cmp", "ecx", 0L)
             + x86("je", L(50))
             + x86("cmp", "ecx", (long)DT_FAIL)
             + x86("je", L(50))
             + x86("cmp", "ecx", (long)DT_PLVAR)
             + x86("je", L(50))
             + x86("note", "both bound: two atoms or two small integers compare inline, everything else is the leaf's")
             + x86("cmp", "eax", "ecx")
             + x86("jne", L(60))
             + x86("cmp", "eax", (long)DT_PLATOM)
             + x86("je", L(45))
             + x86("cmp", "eax", (long)DT_I)
             + x86("jne", L(60))
             + x86("mov", "edx", RDD("rdi", 4))
             + x86("mov", "ecx", RDD("rsi", 4))
             + x86("or", "edx", "ecx")
             + x86("jne", L(60))
             + x86("def", L(45))
             + x86("mov", "rax", RDQ("rdi", 8))
             + x86("mov", "rdx", RDQ("rsi", 8))
             + x86("cmp", "rax", "rdx")
             + x86_omega("jne")
             + x86_gamma()
             + x86("def", L(40))
             + x86("note", "the cell is unbound: a bound variable binds it inline; two unbound cells are the leaf's (it orders the binding)")
             + x86("cmp", "ecx", 0L)
             + x86("je", L(60))
             + x86("cmp", "ecx", (long)DT_FAIL)
             + x86("je", L(60))
             + x86("cmp", "ecx", (long)DT_PLVAR)
             + x86("je", L(60))
             + x86("cmp", "ecx", (long)DT_N)
             + x86("je", L(60))
             + x86("mov", "r8", "rsi")
             + PL_TRAIL(31, 39)
             + x86("mov", "rax", RDQ("r8", 0))
             + x86("mov", "rdx", RDQ("r8", 8))
             + x86("mov", RDQ("rdi", 0), "rax")
             + x86("mov", RDQ("rdi", 8), "rdx")
             + x86_gamma()
             + x86("def", L(50))
             + x86("note", "the variable is unbound and the cell is bound: bind the variable's cell to the value")
             + x86("mov", "r8", "rdi")
             + x86("mov", "rdi", "rsi")
             + PL_TRAIL(33, 39)
             + x86("mov", "rax", RDQ("r8", 0))
             + x86("mov", "rdx", RDQ("r8", 8))
             + x86("mov", RDQ("rdi", 0), "rax")
             + x86("mov", RDQ("rdi", 8), "rdx")
             + x86_gamma()
             + x86("def", L(60))
             + x86("note", "the general-unify leaf: rtx_pl_unify in R4.3; today the ctx veneer over plw_unify_cells, which can allocate through plw_bind's boxing, so it is polled")
             + x86("call", "rt_pl_unify_value", (uint64_t)(uintptr_t)(void *)rt_pl_unify_value)
             + x86_rt_gc_poll()
             + x86("test", "eax", "eax")
             + x86_omega("je")
             + x86_gamma()
             + x86("def", L(39))
             + x86("mov", "rdi", "r12")
             + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
             + x86_beta_trampoline());
}
