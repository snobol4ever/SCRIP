#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string gate_dispatch_chain(int n) {
    return FOR(0, n, [&](int i) { return x86("cmp", "eax", i)
                                       + x86("je", PAIR(i)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gate() {
    x86_begin();
    return IF(_.op_off < 0,
               x86_alpha() + x86_bomb("bb_gate: no arm-index slot (op_off<0)"))
         + IF(!(_.op_off < 0),
               x86("comment", "IR_GATE box: alpha CONCEDES, beta RESUMES the arm this activation banked, by index over compile-time wired ports")
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + IF(x86_fb_pinned(), x86_pl_ball_rule_omega(30))
         + x86("mov", "eax", FR(_.op_off + 16))
         + gate_dispatch_chain((int) _.op_ival)
         + x86_omega());
}
