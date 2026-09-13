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
    std::string r;
    for (int i = 0; i < n; i++) r += x86("cmp", "eax", i) + x86("je", PAIR(i));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gate() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_gate: no arm-index slot (op_off<0)");
    return x86("comment", "IR_GATE box: alpha CONCEDES, beta RESUMES the arm this activation banked, by index over compile-time wired ports")
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + x86("mov", "eax", FR(_.op_off + 16))
         + gate_dispatch_chain((int) _.op_ival)
         + x86_omega();
}
