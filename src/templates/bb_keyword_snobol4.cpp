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
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_snobol4: no slot");
    return x86("comment", "IR_KEYWORD_SNOBOL4_read")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86_omega()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
