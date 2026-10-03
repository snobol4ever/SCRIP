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
#define PL_SRC_RDI() (IF(!_.op_u_kid, x86("lea", "rdi", FRQ(_.op_u_slot))) \
                    + IF(_.op_u_kid, \
                          x86("note", "the cell is child " + std::to_string((long long)_.op_u_idx) + " of the parent box's compound: [parent.p + 16j]") \
                        + x86("mov", "rax", FRQ(_.op_u_slot + 8)) \
                        + x86("lea", "rdi", RDQ("rax", 16 * _.op_u_idx))))
#define PL_DEREF(l0, lvar, ln2, ldone) (x86("note", "deref: follow a name (slen 1: the cell; slen 2: the VCELL's cell) or a bound PLVAR to the cell that holds the value; eax = its tag word") \
                    + x86("def", L(l0)) \
                    + x86("mov", "eax", RDD("rdi", 0)) \
                    + x86("cmp", "al", (long)DT_PLVAR) \
                    + x86("je", L(lvar)) \
                    + x86("cmp", "al", (long)DT_N) \
                    + x86("jne", L(ldone)) \
                    + x86("mov", "rsi", RDQ("rdi", 8)) \
                    + x86("test", "rsi", "rsi") \
                    + x86("jz", L(ldone)) \
                    + x86("mov", "ecx", RDD("rdi", 4)) \
                    + x86("cmp", "ecx", 1L) \
                    + x86("jne", L(ln2)) \
                    + x86("cmp", "rsi", "rdi") \
                    + x86("je", L(ldone)) \
                    + x86("mov", "rdi", "rsi") \
                    + x86("jmp", L(l0)) \
                    + x86("def", L(ln2)) \
                    + x86("cmp", "ecx", 2L) \
                    + x86("jne", L(ldone)) \
                    + x86("mov", "rsi", RDQ("rsi", 0)) \
                    + x86("test", "rsi", "rsi") \
                    + x86("jz", L(ldone)) \
                    + x86("mov", "rdi", "rsi") \
                    + x86("jmp", L(l0)) \
                    + x86("def", L(lvar)) \
                    + x86("mov", "rsi", RDQ("rdi", 8)) \
                    + x86("test", "rsi", "rsi") \
                    + x86("jz", L(ldone)) \
                    + x86("cmp", "rsi", "rdi") \
                    + x86("je", L(ldone)) \
                    + x86("mov", "rdi", "rsi") \
                    + x86("jmp", L(l0)) \
                    + x86("def", L(ldone)))
#define PL_TRAIL(lstore, lrefuse) (x86("note", \
                      "trail test: no choice (B = r13 = 0) records nothing; a cell below rsp (the heap) or at or above the youngest choice's frame top [B + 32] is older than the choice") \
                    + x86("test", "r13", "r13") \
                    + x86("jz", L(lstore)) \
                    + x86("cmp", "rdi", "rsp") \
                    + x86("jbe", L(lstore + 1)) \
                    + x86("mov", "rax", RDQ("r13", PL_TR_FRAME_HI_OFF)) \
                    + x86_raw_unpack("rax") \
                    + x86("cmp", "rdi", "rax") \
                    + x86("jb", L(lstore)) \
                    + x86("def", L(lstore + 1)) \
                    + x86("note", "the push: {cell, 0, old} on the r12 trail, the arena's top word synced for the collector") \
                    + x86("mov", "rax", "r12") \
                    + x86("and", "rax", (long)(PL_TR_ARENA_BYTES - 1)) \
                    + x86("cmp", "rax", (long)(PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES)) \
                    + x86("jae", L(lrefuse)) \
                    + x86("mov", "rax", RDQ("rdi", 0)) \
                    + x86("mov", "rdx", RDQ("rdi", 8)) \
                    + x86("mov", RDQ("r12", 0), "rdi") \
                    + x86("mov", RDQ("r12", 8), 0L) \
                    + x86("mov", RDQ("r12", 16), "rax") \
                    + x86("mov", RDQ("r12", 24), "rdx") \
                    + x86("add", "r12", X86_PL_TR_ENTRY_BYTES) \
                    + x86_pl_tr_top_sync() \
                    + x86("def", L(lstore)))
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
