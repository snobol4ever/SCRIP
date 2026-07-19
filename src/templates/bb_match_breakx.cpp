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
std::string bb_match_breakx() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = (_.op_sa < 0) ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_BREAKX")
         + x86_alpha()
         + IF(_.op_sa <  0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
         + x86("mov",    FR(_.x86_scratch_off), (long)0)
         + x86("def",    L(0))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(_.op_sa >= 0, x86("mov", "rdi", FRQ(_.op_sa + 8))
                          + x86("call", "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
                          + x86("test", "rax", "rax"))
         + IF(_.op_sa <  0, x86("cmpb0", "[rdi+rsi]", "0"))
         + x86("jnz",    L(1))
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("jmp",    L(0))
         + x86("def",    L(1))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sa <  0, x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
         + x86("mov",    "eax", "r14d")
         + x86("sub",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("def",    L(2))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(4))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(_.op_sa >= 0, x86("mov", "rdi", FRQ(_.op_sa + 8))
                          + x86("call", "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
                          + x86("test", "rax", "rax"))
         + IF(_.op_sa <  0, x86("cmpb0", "[rdi+rsi]", "0"))
         + x86("jnz",    L(3))
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("jmp",    L(2))
         + x86("def",    L(3))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86("def",    L(4))
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
