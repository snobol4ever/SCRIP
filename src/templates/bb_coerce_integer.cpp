#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_coerce_int_d(const DESCR_t *in, DESCR_t *out, long codes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coerce_integer() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_COERCE_INTEGER zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("lea",  "rdi", ZOPQ(0, 0))
             + x86("note", ZRESN()) + x86("lea",  "rsi", ZRES(0))
             + x86("mov",  "rdx", (long)_.op_ival)
             + x86("call", "rt_coerce_int_d", (uint64_t)(uintptr_t)(void *)rt_coerce_int_d)
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_sa < 0 || _.op_off < 0, x86_bomb("bb_coerce_integer: needs operand slot (op_sa) + own value slot (op_off)"))
         + IF(!(_.op_sa < 0 || _.op_off < 0),
             x86("comment", "IR_COERCE_INTEGER")
           + x86_alpha()
           + x86("lea",  "rdi", FRQ(_.op_sa))
           + x86("lea",  "rsi", FRQ(_.op_off))
           + x86("mov",  "rdx", (long)_.op_ival)
           + x86("call", "rt_coerce_int_d", (uint64_t)(uintptr_t)(void *)rt_coerce_int_d)
           + x86_gamma()
           + x86_beta_trampoline());
}
