#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "rt/rt_pl_trail.h"
extern "C" void rt_pl_disj_open(void *, void *);
extern "C" void rt_pl_fence_commit(void *);
#include "x86_asm.h"
static_assert(X86_PL_TR_ENTRY_BYTES == PL_TR_ENTRY_BYTES, "x86_pl_tr_pop_entry spells the trail entry size as a literal");
static_assert(X86_PL_TR_ARENA_MASK == -(long)PL_TR_ARENA_BYTES, "x86_pl_tr_top_sync spells the trail arena mask as a literal");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BND_BOMB() (_.op_off < 0)
#define BND_FP2() ((uint64_t)(uintptr_t)(void *)(void (*)(void *, void *))rt_pl_disj_open)
#define BND_FP3() ((uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_pl_fence_commit)
static std::string bound_unmark_zd() {
    _.op_zgpop = 0;
    return x86("comment", "IR_UNMARK cells arm -- restore rsp from BOUND frame slot (depth-immune FRQ)")
         + x86_alpha()
         + x86("mov", "rsp", FRQ(_.op_off))
         + x86_gamma()
         + x86_beta_trampoline();
}
std::string bb_bound() {
    x86_begin();
    return IF(BND_BOMB(), x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)"))
         + IF(!BND_BOMB() && x86_fb_pinned(),
               _.op_sb == 1
             ?  x86_alpha() + x86("mov", FRQ(_.op_off), "r12")
         + IF(emit_pl_fence_on(), x86("mov", FRQ(_.op_off + 8), "rsp"))
         + IF(emit_pl_fence_on(), x86("mov", "rax", "r13") + x86("mov", FRQ(_.op_off + 16), "rax"))
         + x86("lea", "rdi", RDQ(x86_fb(), g_emit.flat_frame_bytes - 64)) + x86("mov", "rsi", x86_fb())
               + x86("call_bare", "rt_pl_disj_open", BND_FP2())
               + x86_gamma() + x86_beta_trampoline()
             : (_.op_ival & 2)
             ?  x86_alpha()
         + x86("mov", "rdi", FRQ(_.op_off + 16)) + x86("call_bare", "rt_pl_fence_commit", BND_FP3())
               + x86("comment", "then the frames themselves, off a FRAME-relative slot that stays readable across the move.")
               + x86("mov", "rsp", FRQ(_.op_off + 8))
               + x86_gamma() + x86_beta_trampoline()
             :  x86_alpha() + x86_pl_tr_unwind_at(FRQ(_.op_off), 200, 201)
               + IF((_.op_ival & 1) != 0,
                      x86("test", "r15", "r15") + x86_omega("jne"))
               + IF((_.op_ival & 4) != 0,  x86("mov", "rdi", FRQ(_.op_off + 16)) + x86("call_bare", "rt_pl_fence_commit", BND_FP3()) + x86("mov", "rsp", FRQ(_.op_off + 8)))
               + x86_gamma() + x86_beta_trampoline())
         + IF(!BND_BOMB() && !x86_fb_pinned() && _.op_zres && _.op_sb == 1,
               x86("comment", "IR_BOUND cells arm -- save rsp to frame slot (Op_Mark: bounded-expression entry frontier)")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), "rsp")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BND_BOMB() && !x86_fb_pinned() && _.op_zres && _.op_sb != 1,
               bound_unmark_zd())
         + IF(!BND_BOMB() && !x86_fb_pinned() && !_.op_zres,
               x86("comment", _.op_sb == 1 ? "IR_BOUND" : "IR_UNMARK")
             + x86_alpha()
             + IF(_.op_sb == 1, x86("mov", FRQ(_.op_off), "rsp"))
             + IF(_.op_sb != 1, x86("mov", "rsp", FRQ(_.op_off)))
             + x86_gamma()
             + x86_beta_trampoline());
}
