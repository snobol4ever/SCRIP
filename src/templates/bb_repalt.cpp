#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_clear() {
    x86_begin();
    return x86_alpha()
         + x86("comment", "IR_REPALT clear")
         + x86("mov", FRQ(_.op_off + 16), (long)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_yield() {
    x86_begin();
    return [&]() { return (_.op_sa < 0) ? x86_bomb("bb_repalt_yield: sub-expression value slot not materialised")
         : x86("comment", "IR_REPALT yield")
         + x86("mov", "rax", FRQ(_.op_sa))
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("mov", FRQ(_.op_off + 16), (long)1); }();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_test() {
    x86_begin();
    return x86("comment", "IR_REPALT test")
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("cmp", "rax", (long)1);
}
