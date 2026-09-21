#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
extern "C" void rt_pl_tr_unwind(void *);
extern "C" void rt_pl_disj_open(void *, void *);
extern "C" void rt_pl_fence_commit(void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_bound() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)");
    if (x86_fb_pinned()) {
        uint64_t fp; { void (*f)(void *) = rt_pl_tr_unwind; fp = (uint64_t)(uintptr_t)(void *)f; }
        uint64_t fp2; { void (*f)(void *, void *) = rt_pl_disj_open; fp2 = (uint64_t)(uintptr_t)(void *)f; }
        uint64_t fp3; { void (*f)(void *) = rt_pl_fence_commit; fp3 = (uint64_t)(uintptr_t)(void *)f; }
        return _.op_sb == 1
             ?  x86_alpha() + x86("mov", FRQ(_.op_off), "r12")
         + IF(emit_pl_fence_on(), x86("mov", FRQ(_.op_off + 8), "rsp"))
         + IF(emit_pl_fence_on(), x86("mov", "rax", "r13") + x86("mov", FRQ(_.op_off + 16), "rax"))
         + x86("lea", "rdi", RDQ(x86_fb(), g_emit.flat_frame_bytes - 64)) + x86("mov", "rsi", x86_fb())
               + x86("call_bare", "rt_pl_disj_open", fp2)
               + x86_gamma() + x86_beta_trampoline()
             : (_.op_ival & 2)
             ?  x86_alpha()
         + x86("mov", "rdi", FRQ(_.op_off + 16)) + x86("call_bare", "rt_pl_fence_commit", fp3)
               + x86("comment", "then the frames themselves, off a FRAME-relative slot that stays readable across the move.")
               + x86("mov", "rsp", FRQ(_.op_off + 8))
               + x86_gamma() + x86_beta_trampoline()
             :  x86_alpha() + x86("mov", "rdi", FRQ(_.op_off)) + x86("call_bare", "rt_pl_tr_unwind", fp)
               + IF((_.op_ival & 1) != 0,
                      x86("test", "r15", "r15") + x86_omega("jne"))
               + IF((_.op_ival & 4) != 0,  x86("mov", "rdi", FRQ(_.op_off + 16)) + x86("call_bare", "rt_pl_fence_commit", fp3) + x86("mov", "rsp", FRQ(_.op_off + 8)))
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
