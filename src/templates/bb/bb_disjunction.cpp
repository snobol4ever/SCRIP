#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_pl_disj_open(void *, void *);
extern "C" void rt_pl_tr_unwind(void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_dispatch_chain(long N, int base, int lo)
{ std::string r;
  for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i)
                                    + x86("je", PAIR((int)(base + i)));
  return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_sigma_copy() {
    std::string r = x86("mov", "eax", FR(_.op_off + 16));
    for (int i = 0; i < _.op_parts_n; i++) {
        r += x86("cmp", "eax", i)
           + x86("jne", L(i))
           + IF(_.op_parts_ival[i] >= 0,
                 x86("mov", "rax", FRQ((int)_.op_parts_ival[i]))
               + x86("mov", FRQ(_.op_off), "rax")
               + x86("mov", "rax", FRQ((int)_.op_parts_ival[i] + 8))
               + x86("mov", FRQ(_.op_off + 8), "rax"))
           + x86_gamma()
           + x86("def", L(i));
    }
    return r + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_choice_open() {
    if (!x86_fb_pinned()) return std::string();
    int kt = g_emit.flat_frame_bytes;
    uint64_t fp; { void (*f)(void *, void *) = rt_pl_disj_open; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL DISJUNCTION OPEN (rung 3, ARCH sec B.5): the disjunction is FRAMELESS BUT A CHOICE, so it banks its own trail mark in its box pad and lowers this frame's log threshold F.HI at [H+32] to the frame base -- while a branch is untried, every cell of THIS activation is loggable, because the branch step below undoes bindings the clause step would otherwise have re-seeded away. B is raised to H only when the live choice is OLDER than this frame or absent; a retained callee to the left is already younger and stays the choice. Named rtx helper, never an emitted write of r13.")
         + x86("mov", FRQ(_.op_off + 24), "r12")
         + x86("lea", "rdi", RDQ(x86_fb(), kt - 64))
         + x86("mov", "rsi", x86_fb())
         + x86("call_bare", "rt_pl_disj_open", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_step_unwind() {
    if (!x86_fb_pinned()) return std::string();
    uint64_t fp; { void (*f)(void *) = rt_pl_tr_unwind; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL DISJUNCTION STEP (rung 3, ARCH sec B.5): a branch conceded, so undo everything it bound back to the mark taken at this box's alpha before the next branch is entered -- the same rtx helper the rung-2 clause step uses, the only writer of r12 on this path.")
         + x86("mov", "rdi", FRQ(_.op_off + 24))
         + x86("call_bare", "rt_pl_tr_unwind", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_disjunction() {
    x86_begin();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_DISJUNCTION nary: value/state slot not granted (zls)")
             : x86("comment", "IR_DISJUNCTION_NARY")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), 0L)
             + x86("mov", FRQ(_.op_off + 8), 0L)
             + x86("mov", FR(_.op_off + 16), 0)
             + disj_choice_open()
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + disj_sigma_copy()
             + x86_beta()
             + x86("mov", "eax", FR(_.op_off + 16))
             + disj_dispatch_chain(_.op_ival - 1, (int)_.op_ival, 0)
             + x86("jmp", PAIR((int)(_.op_ival + _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 2)))
             + disj_step_unwind()
             + x86("add", FR(_.op_off + 16), 1)
             + x86("mov", "eax", FR(_.op_off + 16))
             + disj_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
