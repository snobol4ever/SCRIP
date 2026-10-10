#ifndef BB_PL_CELL_H
#define BB_PL_CELL_H
#include "stage2.h"
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
#define PL_TRAIL(lstore, lrefuse, larm) (IF(g_stage2.pl_attv_armed, \
                      x86("note", "attvar: a cell whose slen is PL_ATTV_SLEN is always trailed and lowers the wake word at the trail header +48 to this entry's offset") \
                    + x86("mov", "edx", RDD("rdi", 4)) \
                    + x86("cmp", "edx", (long)PL_ATTV_SLEN) \
                    + x86("jne", L(larm)) \
                    + x86("mov", "rax", "r12") \
                    + x86("and", "rax", X86_PL_TR_ARENA_MASK) \
                    + x86("mov", "rdx", "r12") \
                    + x86("sub", "rdx", "rax") \
                    + x86("mov", "rax", RDQ("rax", PL_TR_WAKE_OFF)) \
                    + x86("test", "rax", "rax") \
                    + x86("je", L(larm + 1)) \
                    + x86("cmp", "rax", "rdx") \
                    + x86("jbe", L(larm + 2)) \
                    + x86("def", L(larm + 1)) \
                    + x86("mov", "rax", "r12") \
                    + x86("and", "rax", X86_PL_TR_ARENA_MASK) \
                    + x86("mov", RDQ("rax", PL_TR_WAKE_OFF), "rdx") \
                    + x86("def", L(larm + 2)) \
                    + x86("jmp", L(lstore + 1)) \
                    + x86("def", L(larm))) \
                    + x86("note", \
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
#endif
