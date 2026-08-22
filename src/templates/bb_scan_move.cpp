#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_move() {
    x86_begin();
    if (!PLATFORM_X86 || !(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_scan_move: no result slot (op_off)");
    return x86("comment", "IR_SCAN_MOVE [fscan.r move: j=&pos+i; fail unless 1<=j<=Delta+1; result substr; data-backtrack restores r14 on beta]")
         + x86_alpha()
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("add",     "rax", "r14")
         + x86("add",     "rax", (long)1)
         + x86("cmp64",   "rax", (long)1)
         + x86_omega("jl")
         + x86("mov",     "rcx", "r15")
         + x86("add",     "rcx", (long)1)
         + x86("cmp",     "rax", "rcx")
         + x86_omega("jg")
         + x86("mov",     FRQ(_.op_off + 16), "r14")
         + x86("mov",     "rdi", "r13")
         + x86("mov",     "rsi", "r14")
         + x86("mov",     "rdx", "rax")
         + x86("sub",     "rdx", (long)1)
         + x86("mov",     "r14", "rdx")
         + x86("push",    "r11")
         + x86("push",    "r11")
         + x86("call",    "rt_substr", (uint64_t)(uintptr_t)(void*)rt_substr)
         + x86("pop",     "r11")
         + x86("pop",     "r11")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86("mov",     "r14", FRQ(_.op_off + 16))
         + x86_omega();
}
