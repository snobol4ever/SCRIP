#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
extern "C" void rt_pl_tr_unwind(void *);
extern "C" void rt_pl_disj_open(void *, void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_bound() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)");
    if (x86_fb_pinned()) {
        uint64_t fp; { void (*f)(void *) = rt_pl_tr_unwind; fp = (uint64_t)(uintptr_t)(void *)f; }
        uint64_t fp2; { void (*f)(void *, void *) = rt_pl_disj_open; fp2 = (uint64_t)(uintptr_t)(void *)f; }
        return _.op_sb == 1
             ? x86("comment", "IR_BOUND pinned arm (rung 5, ARCH sec B.7/B.8): bank the TRAIL top as this construct's mark. An if-then-else is entered here, and everything its condition or its taken arm goes on to bind must be undoable at the one place sec B.8 names -- a SUCCESSFUL goal whose enclosing negation then fails, where no enclosing choice ever backtracks past the binding.")
               + x86_alpha() + x86("mov", FRQ(_.op_off), "r12")
               + x86("comment", "and OPEN THE CHOICE, exactly as bb_disjunction does at rung 3 and the generator does at rung 7 under the ceo's sec B.13 ruling: a mark is useless while nothing is LOGGED, and with no live choice pl_tr_needs_log returns 0 for every cell. rt_pl_disj_open lowers F.HI at [H+32] to this frame's base and raises B to H only when the live choice is older than this frame or absent.")
               + x86("lea", "rdi", RDQ(x86_fb(), g_emit.flat_frame_bytes - 64)) + x86("mov", "rsi", x86_fb())
               + x86("call_bare", "rt_pl_disj_open", fp2)
               + x86_gamma() + x86_beta_trampoline()
             : x86("comment", "IR_UNMARK pinned arm (rung 5): undo to the paired IR_BOUND's mark before leaving. Same named rtx helper the rung-2 clause step, the rung-3 disjunction step and the rung-7 generator step use -- the only writer of r12 on this path.")
               + x86_alpha() + x86("mov", "rdi", FRQ(_.op_off)) + x86("call_bare", "rt_pl_tr_unwind", fp)
               + IF(_.op_ival == 1,
                     x86("comment", "AND THE C9 BALL GUARD (rung 9, ARCH sec A.1 review C9 + sec B.7): this unmark is a CONDITION-FAILURE landing, and a ball in flight is not a failure -- without the test, `( throw(oops) -> yes ; no )` undoes the trail and then runs the ELSE arm, which is an if-then-else SWALLOWING an exception (measured: it printed no/after). The lowerer sets the flag only on the landings that can be reached with a ball, so this costs an Icon or SNOBOL4 unmark nothing: they never carry it.")
                   + x86("test", "r15", "r15") + x86_omega("jne"))
               + x86_gamma() + x86_beta_trampoline();
    }
    if (_.op_zres) {
        if (_.op_sb == 1)
            return x86("comment", "IR_BOUND cells arm -- save rsp to frame slot (Op_Mark: bounded-expression entry frontier)")
                 + x86_alpha()
                 + x86("mov", FRQ(_.op_off), "rsp")
                 + x86_gamma()
                 + x86_beta_trampoline();
        _.op_zgpop = 0;
        return x86("comment", "IR_UNMARK cells arm -- restore rsp from BOUND frame slot (depth-immune FRQ)")
             + x86_alpha()
             + x86("mov", "rsp", FRQ(_.op_off))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", _.op_sb == 1 ? "IR_BOUND" : "IR_UNMARK")
         + x86_alpha()
         + IF(_.op_sb == 1, x86("mov", FRQ(_.op_off), "rsp"))
         + IF(_.op_sb != 1, x86("mov", "rsp", FRQ(_.op_off)))
         + x86_gamma()
         + x86_beta_trampoline();
}
