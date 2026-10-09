#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define disj_dispatch_chain(N, base, lo) emit_for((int)(lo), (int)(N), [&](int i) { return x86("cmp", "eax", i) \
                                                                       + x86("je", PAIR((int)(base) + i)); })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define disj_sigma_copy() emit_seq({ \
      x86("mov", "eax", FR(_.op_off + 16)), \
      emit_for(0, _.op_parts_n, [&](int i) { return x86("cmp", "eax", i) \
                                             + x86("jne", L(i)) \
                                             + IF(_.op_parts_ival[i] >= 0, \
                                                   x86("mov", "rax", FRQ((int)_.op_parts_ival[i])) \
                                                 + x86("mov", FRQ(_.op_off), "rax") \
                                                 + x86("mov", "rax", FRQ((int)_.op_parts_ival[i] + 8)) \
                                                 + x86("mov", FRQ(_.op_off + 8), "rax")) \
                                             + x86_gamma() \
                                             + x86("def", L(i)); }), \
      x86_gamma() })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define disj_choice_open() ( \
      IF(x86_fb_pinned(), \
      x86("mov", FRQ(_.op_off + 24), "r12") \
    + x86_pl_disj_open(x86_fb(), g_emit.flat_frame_bytes, 226, 227)) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define disj_step_unwind() ( \
      IF(!x86_fb_pinned(), \
          std::string()) \
    + IF(!(!x86_fb_pinned()), \
          x86_pl_tr_unwind_at(FRQ(_.op_off + 24), 200, 201)) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define disj_step_ball() ( \
      IF(!x86_fb_pinned(), \
          std::string()) \
    + IF(!(!x86_fb_pinned()), \
          x86("test", "r15", "r15") + x86_omega("jne")) \
)
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
             + disj_step_ball()
             + x86("add", FR(_.op_off + 16), 1)
             + x86("mov", "eax", FR(_.op_off + 16))
             + disj_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
