#include <string>
#include <stdint.h>
#include "ir/ir_node.h"
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_jump.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string gate_dispatch_chain(int n) {
    return FOR(0, n, [&](int i) { return (i < g_emit.xa_bb_emit_pair_n && XA_PAIR(i).rsp != 0)
                                       ? x86("cmp", "eax", i)
                                       + x86("jne", L(10 + i))
                                       + x86("add", "rsp", (long) XA_PAIR(i).rsp)
                                       + x86("jmp", PAIR(i))
                                       + x86("def", L(10 + i))
                                       : x86("cmp", "eax", i)
                                       + x86("je", PAIR(i)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gate_ball_self_arm(int i) { return i < g_emit.xa_bb_emit_pair_n && XA_PAIR(i).rsp != 0 && XA_PAIR(i).jmp == g_emit.lbl_α_p; }
static int gate_ball_self_arms(int n) { return n > 0 && (gate_ball_self_arm(n - 1) || gate_ball_self_arms(n - 1)); }
static std::string gate_ball_self_road(int n) {
    return x86("test", "r15", "r15")
         + x86("je", L(69))
         + FOR(0, n, [&](int i) { return gate_ball_self_arm(i)
                                       ? x86("cmp", "eax", i)
                                       + x86("jne", L(70 + i))
                                       + x86("add", "rsp", (long) XA_PAIR(i).rsp)
                                       + x86("jmp", PAIR(i))
                                       + x86("def", L(70 + i))
                                       : std::string(); })
         + x86("def", L(69));
}
static std::string gate_catch_relink() {
    return x86("test", "eax", "eax")
         + x86("jne", L(90))
         + x86_pl_catch_link(_.op_catch_off + 48)
         + x86("def", L(90));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gate() {
    x86_begin();
    return IF(_.op_gcell,
               x86("comment", "IR_GATE box: alpha CONCEDES, beta RESUMES the arm that ran -- its index is the spine cell the arm pushed, at [rsp + 8] (ARCH-PROLOG-ACTIVATION-LIFETIME 8.2)")
             + x86_alpha()
             + x86_omega()
             + x86_beta()
             + IF(x86_resumable_frame(), x86_pl_ball_rule_omega(30))
             + x86("mov", "eax", RDD("rsp", 8))
             + IF(_.op_catch, gate_catch_relink())
             + gate_dispatch_chain((int) _.op_ival)
             + x86_omega())
         + IF(!_.op_gcell && _.op_off < 0,
               x86_alpha() + x86_bomb("bb_gate: no arm-index slot (op_off<0)"))
         + IF(!_.op_gcell && !(_.op_off < 0),
               x86("comment", "IR_GATE box: alpha CONCEDES, beta RESUMES the arm this activation banked, by index over compile-time wired ports")
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + IF(x86_resumable_frame() && gate_ball_self_arms((int) _.op_ival), x86("mov", "eax", FR(_.op_off + 16)))
         + IF(x86_resumable_frame() && gate_ball_self_arms((int) _.op_ival), gate_ball_self_road((int) _.op_ival))
         + IF(x86_resumable_frame(), x86_pl_ball_rule_omega(30))
         + x86("mov", "eax", FR(_.op_off + 16))
         + IF(_.op_catch, gate_catch_relink())
         + gate_dispatch_chain((int) _.op_ival)
         + x86_omega());
}
