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
