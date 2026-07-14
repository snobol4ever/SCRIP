#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string scanseq_dispatch_chain(long N, int base, int lo) { std::string r; for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i) + x86("je", PAIR((int)(base + i))); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* scanseq_concat_chain — the sequence value is arm[0]||arm[1]||...||arm[N-1], each arm's OWN consumed slice
 * (its value DESCR at FRQ(zls_off(operand[2j+1]))), NOT the subject span [saved_δ,δ).  The span equals the
 * concatenation only for contiguous forward arms; it gives the wrong value for non-contiguous/backward tab/move
 * (e.g. "hello" ? (tab(4)||tab(2)) is "helel", the span is "h").  operand[2j+1] is the resume node = the
 * SCAN_TAB that produced arm j's value (=s desugars to tab(match(s)), so the producer is a SCAN_TAB either way),
 * confirmed by the ZLS grant + drive_value_slot writing each arm's DESCR there.  r10 (the cursor mirror, live in
 * scan context) is preserved across the str_concat_d C-calls exactly as the former rt_substr call preserved it;
 * the doubled push keeps rsp 16-aligned for every call in the chain. */
static std::string scanseq_concat_chain() {
    long N = (long)_.op_ival; const IR_t * nd = _.node; int av0 = zls_off(nd->operands[1]);
    std::string r = x86("mov", "rax", FRQ(av0)) + x86("mov", "rdx", FRQ(av0 + 8));
    if (N > 1) {
        r += x86("push", "r10") + x86("push", "r10");
        for (long j = 1; j < N; j++) {
            int avj = zls_off(nd->operands[2 * j + 1]);
            r += x86("mov", "rdi", "rax") + x86("mov", "rsi", "rdx") + x86("mov", "rdx", FRQ(avj)) + x86("mov", "rcx", FRQ(avj + 8)) + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d);
        }
        r += x86("pop", "r10") + x86("pop", "r10");
    }
    return r + x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_sequence() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_SCAN_SEQUENCE: value/state slot not granted (zls)")
             : x86("comment", "IR_SCAN_SEQ_NARY")
             + x86_alpha()
             + x86("mov", FR(_.op_off + 16), "r14d")
             + x86("mov", FR(_.op_off + 20), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86("mov", "eax", FR(_.op_off + 20))
             + x86("add", "eax", 1)
             + x86("mov", FR(_.op_off + 20), "eax")
             + scanseq_dispatch_chain(_.op_ival, 0, 1)
             + scanseq_concat_chain()
             + x86_gamma()
             + x86_beta()
             + x86("mov", FR(_.op_off + 20), (int)_.op_ival)
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("mov", "eax", FR(_.op_off + 20))
             + x86("sub", "eax", 1)
             + x86("mov", FR(_.op_off + 20), "eax")
             + scanseq_dispatch_chain(_.op_ival, (int)_.op_ival, 0)
             + x86_omega();
}
