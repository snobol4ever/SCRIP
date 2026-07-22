#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" long rt_sg_scan_member(void);
extern "C" long rt_sg_scan_nonmember(void);
extern "C" long rt_sg_member(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static long bk_chainp() { return _.op_sa < 0 && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long bk_tablep() { return _.op_sa < 0 && !bk_chainp(); }
static std::string bk_memb(long i) { return i >= CSK() ? std::string() : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(1)) + bk_memb(i + 1); }
static std::string bk_char() { return x86("cmp", "ecx", "r15d") + x86_omega("jge") + x86("movzx", "esi", "[r13+rcx]") + (bk_chainp() ? bk_memb(0) : x86("cmpb0", "[rdi+rsi]", "0") + x86("jnz", L(1))) + x86("add", "ecx", (long)1); }
static std::string bk_unroll(long u) { return u >= (ZC_SPAN_LIT_UNROLL ? ZC_UNROLL_FACTOR : 1) ? x86("jmp", L(0)) : bk_char() + bk_unroll(u + 1); }
std::string bb_match_break() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = bk_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_BREAK")
         + x86_alpha()
         + IF(_.op_sa >= 0 && ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE,
              x86("mov",    FR(_.x86_scratch_off), (long)0)
            + x86("mov",    "r8",  FRQ(_.op_sa + 8))
            + x86("mov",    "r9d", FR(_.op_sa + 4))
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("cmp",    "eax", "r15d")
            + x86_omega("jge")
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + x86("mov",    "edx", (long)0)
            + x86("def",    L(2))
            + x86("cmp",    "edx", "r9d")
            + x86("jge",    L(3))
            + x86("movzx",  "edi", "[r8+rdx]")
            + x86("cmp",    "esi", "edi")
            + x86("je",     L(1))
            + x86("add",    "edx", (long)1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("add",    FR(_.x86_scratch_off), (long)1)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("mov",    FR(_.x86_scratch_off), "r14d")
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa >= 0 && ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL,
              x86("mov",    "edi", "r14d")
            + x86("mov",    "rsi", FRQ(_.op_sa + 8))
            + x86("mov",    "edx", FR(_.op_sa + 4))
            + x86("call",   "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
            + x86("cmp",    "eax", "r15d")
            + x86_omega("jge")
            + x86("mov",    FR(_.x86_scratch_off), "r14d")
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa < 0,
              IF(bk_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + bk_unroll(0)
            + x86("def",    L(1))
            + x86("mov",    FR(_.x86_scratch_off), "r14d")
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + x86("mov",    "r14d", FR(_.x86_scratch_off))
         + x86_omega();
}
