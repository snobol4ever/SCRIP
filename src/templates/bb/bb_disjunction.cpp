#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
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
std::string bb_disjunction() {
    x86_begin();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_DISJUNCTION nary: value/state slot not granted (zls)")
             : x86("comment", "IR_DISJUNCTION_NARY")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), 0L)
             + x86("mov", FRQ(_.op_off + 8), 0L)
             + x86("mov", FR(_.op_off + 16), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + disj_sigma_copy()
             + x86_beta()
             + x86("mov", "eax", FR(_.op_off + 16))
             + disj_dispatch_chain(_.op_ival - 1, (int)_.op_ival, 0)
             + x86("jmp", PAIR((int)(_.op_ival + _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("add", FR(_.op_off + 16), 1)
             + x86("mov", "eax", FR(_.op_off + 16))
             + disj_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
