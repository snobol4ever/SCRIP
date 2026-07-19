#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SPD-1 slice A (s108): the elementary-test C round-trip is the measured claws5 wall/Ir dominator (blob 58.7% Ir; per-attempt lea+lea+mov+call memcmp+test ≈ 15+ insts + call).  len==1 literals — the
 * dominant class in tag grammars — inline to movzx+cmp (the bb_match_any subject-byte idiom); len==0 emits no test at all (null match: no bounds, no cursor move, β falls to ω).  len>=2 keeps memcmp
 * (an 8B masked-load fast path is slice B, gated on verifying the carve tail-pad over-read invariant — see RUNG SPD).  Interleaved lib-swap A/B x7 is the acceptance evidence. */
static inline long litn()  { return (long) strlen(_.op_sval ? _.op_sval : ""); }
static inline int  litck(int k) { return (int)(unsigned char)_.op_sval[k]; }
static std::string lit_unroll(long n) {
    std::string r;
    char m[24];
    for (long k = 0; k < n; k++) {
        snprintf(m, sizeof m, "[r13+rcx+%ld]", k);
        r += x86("movzx", "eax", k ? m : "[r13+rcx]")
           + x86("cmp",   "eax", litck((int)k))
           + x86_omega("jne");
    }
    return r;
}
std::string bb_match_lit() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return x86("comment", "IR_MATCH_LIT")
         + x86_alpha()
         + IF(litn() > 0,
              x86("mov",    "eax", "r14d")
            + x86("add",    "eax", litn())
            + x86("cmp",    "eax", "r15d")
            + x86_omega("jg")
            + x86("movsxd", "rcx", "r14d"))
         + IF(litn() >= 1 && litn() <= 10, lit_unroll(litn()))
         + IF(litn() > 10,
              x86("lea",    "rdi", "[r13+rcx]")
            + x86("lea",    "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
            + x86("mov",    "edx", litn())
            + x86("call",   "memcmp", (uint64_t)(uintptr_t)(void *)(int (*)(const void *, const void *, size_t)) memcmp)
            + x86("test",   "eax", "eax")
            + x86_omega("jne"))
         + IF(litn() > 0, x86("add", "r14d", litn()))
         + x86_gamma()
         + x86_beta()
         + IF(litn() > 0, x86("sub", "r14d", litn()))
         + x86_omega();
}
