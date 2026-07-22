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
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static long bx_chainp() { return _.op_sa < 0 && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long bx_tablep() { return _.op_sa < 0 && !bx_chainp(); }
static std::string bx_memb(long f, long i) { return i >= CSK() ? std::string() : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(f)) + bx_memb(f, i + 1); }
static std::string bx_char(long f, long e) { return x86("cmp", "ecx", "r15d") + (e ? x86("jge", L(4)) : x86_omega("jge")) + x86("movzx", "esi", "[r13+rcx]") + (bx_chainp() ? bx_memb(f, 0) : x86("cmpb0", "[rdi+rsi]", "0") + x86("jnz", L(f))) + x86("add", "ecx", (long)1); }
static std::string bx_unroll(long t, long f, long e, long u) { return u >= (ZC_SPAN_LIT_UNROLL ? ZC_UNROLL_FACTOR : 1) ? x86("jmp", L(t)) : bx_char(f, e) + bx_unroll(t, f, e, u + 1); }
static std::string bx_slot_scan(long t, long f, long e) {
    return x86("def",    L(t))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + (e ? x86("jge", L(4)) : x86_omega("jge"))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("mov",    "rdi", FRQ(_.op_sa + 8))
         + x86("call",   "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
         + x86("test",   "rax", "rax")
         + x86("jnz",    L(f))
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("jmp",    L(t));
}
std::string bb_match_breakx() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = bx_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_BREAKX")
         + x86_alpha()
         + IF(_.op_sa >= 0,
              x86("mov",    FR(_.x86_scratch_off + 4), "r14d")
            + x86("mov",    FR(_.x86_scratch_off), (long)0)
            + bx_slot_scan(0, 1, 0)
            + x86("def",    L(1))
            + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa < 0,
              IF(bx_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("mov",    FR(_.x86_scratch_off + 4), "r14d")
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + bx_unroll(0, 1, 0, 0)
            + x86("def",    L(1))
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sa >= 0,
              x86("add",    FR(_.x86_scratch_off), (long)1)
            + bx_slot_scan(2, 3, 1)
            + x86("def",    L(3))
            + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa < 0,
              IF(bx_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("add",    "ecx", (long)1)
            + x86("def",    L(2))
            + bx_unroll(2, 3, 1, 0)
            + x86("def",    L(3))
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86("def",    L(4))
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
