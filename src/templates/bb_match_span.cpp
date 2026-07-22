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
static long sp_chainp() { return _.op_sa < 0 && CSK() >= 1 && CSK() <= 8; }
static long sp_tablep() { return _.op_sa < 0 && !sp_chainp(); }
static std::string sp_memb(long u, long i) { return i >= CSK() ? x86("jmp", L(1)) + x86("def", L(10 + u)) : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(10 + u)) + sp_memb(u, i + 1); }
static std::string sp_char(long u) { return x86("cmp", "ecx", "r15d") + x86("jge", L(1)) + x86("movzx", "esi", "[r13+rcx]") + (sp_chainp() ? sp_memb(u, 0) : x86("cmpb0", "[rdi+rsi]", "0") + x86("je", L(1))) + x86("add", "ecx", (long)1); }
static std::string sp_unroll(long u) { return u >= 4 ? x86("jmp", L(0)) : sp_char(u) + sp_unroll(u + 1); }
std::string bb_match_span() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = sp_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_SPAN")
         + x86_alpha()
         + IF(_.op_sa >= 0,
              x86("mov",    FR(_.x86_scratch_off), (long)0)
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("cmp",    "eax", "r15d")
            + x86("jge",    L(1))
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + x86("mov",    "rdi", FRQ(_.op_sa + 8))
            + x86("call",   "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
            + x86("test",   "rax", "rax")
            + x86("je",     L(1))
            + x86("add",    FR(_.x86_scratch_off), (long)1)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "eax", FR(_.x86_scratch_off))
            + x86("test",   "eax", "eax")
            + x86_omega("jle")
            + x86("mov",    "edx", "r14d")
            + x86("mov",    FR(_.x86_scratch_off + 4), "edx")
            + x86("add",    "edx", "eax")
            + x86("mov",    "r14d", "edx"))
         + IF(_.op_sa < 0,
              IF(sp_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + sp_unroll(0)
            + x86("def",    L(1))
            + x86("cmp",    "ecx", "r14d")
            + x86_omega("jle")
            + x86("mov",    FR(_.x86_scratch_off + 4), "r14d")
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + x86_beta()
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
