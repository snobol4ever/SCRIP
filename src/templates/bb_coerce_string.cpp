#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_coerce_str_d(const DESCR_t *in, DESCR_t *out, long codes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coerce_string() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_COERCE_STRING zd")
             + x86_alpha()
             + x86("note", ZOPAN()) + x86("lea",  "rdi", ZOPQ(0, 0))
             + x86("note", ZRESN()) + x86("lea",  "rsi", ZRES(0))
             + x86("mov",  "rdx", (long)_.op_ival)
             + x86("call", "rt_coerce_str_d", (uint64_t)(uintptr_t)(void *)rt_coerce_str_d)
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-2k: the IMPLICIT string coercion the lowerer inserts, one operand in and this box's own cell out -- a by-address clone of bb_coerce_numeric's slow path with ONE lea instead of three.  Port topology is gamma-only and that is the MANUAL's contract, not an omission: Ch.17 splits conversion in two, where explicit CONVERT() fails when the conversion is impossible but IMPLICIT conversion "will be attempted, and an error message given if conversion is not possible" -- an error through core_runtime_error, never a statement failure, so there is no omega edge to wire and the legacy arm below has none either.  Matches that arm exactly. */
    return IF(_.op_sa < 0 || _.op_off < 0, x86_bomb("bb_coerce_string: needs operand slot (op_sa) + own value slot (op_off)"))
         + IF(!(_.op_sa < 0 || _.op_off < 0),
             x86("comment", "IR_COERCE_STRING")
           + x86_alpha()
           + x86("lea",  "rdi", FRQ(_.op_sa))
           + x86("lea",  "rsi", FRQ(_.op_off))
           + x86("mov",  "rdx", (long)_.op_ival)
           + x86("call", "rt_coerce_str_d", (uint64_t)(uintptr_t)(void *)rt_coerce_str_d)
           + x86_gamma()
           + x86_beta_trampoline());
}
