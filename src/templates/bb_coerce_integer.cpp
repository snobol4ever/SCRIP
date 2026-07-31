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
             + x86("lea",  "rdi", ZOPQ(0, 0))
             + x86("lea",  "rsi", ZRES(0))
             + x86("mov",  "rdx", (long)_.op_ival)
             + x86("call", "rt_coerce_int_d", (uint64_t)(uintptr_t)(void *)rt_coerce_int_d)
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-2k: sibling of the COERCE_STRING arm above it in the same rung -- one operand by address, own cell out, gamma-only.  The integer case is where the manual's Ch.3 coercion rules bite (a string operand is parsed with leading and trailing blanks ignored, the null string becomes integer 0, and an interior blank or a non-numeric body is the ERROR path), but every one of those decisions lives inside rt_coerce_int_d and none of them reaches the port topology: the runtime either writes the out cell and returns, or raises.  So the ZD arm changes WHERE the two descriptors live, never what the conversion means. */
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
