#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string scanalt_dispatch_chain(long N, int base, int lo) { std::string r; for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i) + x86("je", PAIR((int)(base + i))); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_SCAN_ALTERNATE: value/state slot not granted (zls)")
             : x86("comment", "IR_SCAN_ALT_NARY")
             + x86_alpha()
             /* rbp-dcap (s46 mirror of bb_match_alternate): the rt_dcap_height (α) / rt_dcap_restore_to (switch) C-call windows are DELETED — alternation touches the pend stack nowhere (generator scoping IS the LIFO discipline; see the theorem in bb_match_alternate.cpp).  +20 is now dead pad; quad offsets unchanged. */
             + x86("mov", FR(_.op_off + 16), "r14d")
             + x86("mov", FR(_.op_off + 24), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86("mov", "rdi", "r13")
             + x86("movsxd", "rsi", FR(_.op_off + 16))
             + x86("mov", "rdx", "r14")
             + x86("sub", "rdx", "rsi")
             + x86("push", "r10")
             + x86("push", "r10")
             + x86("call", "rt_substr", (uint64_t)(uintptr_t)(void*)rt_substr)
             + x86("pop", "r10")
             + x86("pop", "r10")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86("mov", "eax", FR(_.op_off + 24))
             + scanalt_dispatch_chain(_.op_ival - 1, (int)_.op_ival, 0)
             + x86("jmp", PAIR((int)(_.op_ival + _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("add", FR(_.op_off + 24), 1)
             + x86("mov", "r14d", FR(_.op_off + 16))
             + x86("mov", "eax", FR(_.op_off + 24))
             + scanalt_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
