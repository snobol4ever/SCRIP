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
#define PL_UNBOUND(lbind, lbound) (x86("note", "unbound after the deref: the zero DESCR, DT_FAIL, a self PLVAR (the deref stopped on it) or a self name") \
                    + x86("cmp", "al", 0L) \
                    + x86("je", L(lbind)) \
                    + x86("cmp", "al", (long)DT_FAIL) \
                    + x86("je", L(lbind)) \
                    + x86("cmp", "al", (long)DT_PLVAR) \
                    + x86("je", L(lbind)) \
                    + x86("cmp", "al", (long)DT_N) \
                    + x86("jne", L(lbound)) \
                    + x86("mov", "rsi", RDQ("rdi", 8)) \
                    + x86("cmp", "rsi", "rdi") \
                    + x86("je", L(lbind)) \
                    + x86("def", L(lbound)))
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
