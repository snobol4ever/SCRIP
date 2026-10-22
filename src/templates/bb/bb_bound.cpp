#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BND_BOMB() (_.op_off < 0)
#define BND_PL() x86_resumable_frame()
static std::string bound_unmark_cells() {
    _.op_zgpop = _.op_upost;
    return x86("comment", "IR_UNMARK cells arm -- restore rsp from BOUND frame slot (depth-immune FRQ)")
         + x86_alpha()
         + x86("mov", "rsp", FRQ(_.op_off))
         + x86_release_acts(true, 5)
         + x86_gamma()
         + x86_beta_trampoline();
}
static std::string bound_unmark_zd() {
    return (_.op_unmark_chain && x86_release_acts(true, 5).empty())
         ? x86("comment", "IR_UNMARK chain arm -- the bounded body's carves unwind box by box")
         + x86_alpha()
         + x86_gamma()
         + x86_beta_trampoline()
         : bound_unmark_cells();
}
std::string bb_bound() {
    x86_begin();
    return IF(BND_BOMB(), x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)"))
         + IF(!BND_BOMB() && BND_PL(),
               _.op_sb == 1
             ? x86_alpha() + x86_pl_choice_mark(FRQ(_.op_off), 245)
         + IF(emit_pl_fence_on(), x86("mov", FRQ(_.op_off + 8), "rsp"))
         + x86_pl_header_open(_.op_off, _.op_off + 16)
               + x86_gamma() + x86_beta_trampoline()
             : (_.op_ival & 8)
             ? x86_alpha()
         + x86("lea", "rax", FRQ(_.op_off))
         + x86("cmp", "r13", "rax")
         + x86("jne", L(1))
         + x86("note", "pl_choice_restore inline (D1): the bounded goal exits leaving no choice of its own, so the bound's header is no choice: B returns to the choice banked at the bound")
         + x86("mov", "r13", FRQ(_.op_off + 16))
         + x86("def", L(1))
               + x86_gamma() + x86_beta_trampoline()
             : (_.op_ival & 2)
             ? x86_alpha()
         + x86("note", "pl_fence_commit inline (ARCH-PROLOG-C-OUT-OF-THE-BOX 2.2): B := the choice banked at the bound")
         + x86("mov", "r13", FRQ(_.op_off + 16))
         + x86_pl_tr_tidy(400)
               + x86("comment", "then the frames themselves, off a FRAME-relative slot that stays readable across the move.")
               + x86("mov", "rsp", FRQ(_.op_off + 8))
               + x86_release_acts(true)
               + x86_gamma() + x86_beta_trampoline()
             : x86_alpha() + x86_pl_tr_unwind_at(FRQ(_.op_off), 200, 201)
               + IF((_.op_ival & 1) != 0,
                      x86("test", "r15", "r15") + x86_omega("jne"))
               + x86("note", "pl_choice_restore inline (D1): the condition failed, so the bound's header is no choice: B returns to the choice banked at the bound")
               + x86("mov", "r13", FRQ(_.op_off + 16))
               + IF((_.op_ival & 4) != 0, x86("mov", "rsp", FRQ(_.op_off + 8))
               + x86_release_acts(true))
               + x86_gamma() + x86_beta_trampoline())
         + IF(!BND_BOMB() && !BND_PL() && _.op_zres && _.op_sb == 1,
               x86("comment", "IR_BOUND cells arm -- save rsp to frame slot (Op_Mark: bounded-expression entry frontier)")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), "rsp")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BND_BOMB() && !BND_PL() && _.op_zres && _.op_sb != 1,
               bound_unmark_zd())
         + IF(!BND_BOMB() && !BND_PL() && !_.op_zres,
               x86("comment", _.op_sb == 1 ? "IR_BOUND" : "IR_UNMARK")
             + x86_alpha()
             + IF(_.op_sb == 1, x86("mov", FRQ(_.op_off), "rsp"))
             + IF(_.op_sb != 1, x86("mov", "rsp", FRQ(_.op_off)) + x86_release_acts(true, 5))
             + x86_gamma()
             + x86_beta_trampoline());
}
