#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_keyword_read_snobol4(const char *sval);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SNOBOL4 keywords are a distinct set from Icon's and are plain single-value reads:
 * no &subject/&pos scan-register arms, no resumable-generator counter. The opcode
 * (IR_KEYWORD_SNOBOL4) selects this template; the emitter never tests a language. */
std::string bb_keyword_snobol4() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read zd")
             + x86_alpha()
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval);   /* ZD-2l: a ZERO-OPERAND LEAF, the same shape as the LIT_* producers already on the cells -- zd_nops returns 0 for it and needs no edit, and zd_plan's K is 16 because it yields a value.  Only the STORE destination moves: the returned DESCR halves land in this box's own cell instead of the flat slot.  The sealed RO tail after the trampoline is carried verbatim -- it is rip-relative constant data that ROQ(0) reaches, wholly independent of the frame base, so it neither moves nor needs compensation.  Port topology is gamma-only and that is correct: a keyword READ always yields a value (manual Ch.16 -- protected keywords cannot be ASSIGNED, but reading one never fails), so there is no omega edge, matching the legacy arm below. */
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_snobol4: no slot");
    return x86("comment", "IR_KEYWORD_SNOBOL4_read")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
