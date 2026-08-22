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
static int tab_admit() { return _.op_off >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_tab() {
    x86_begin();
    return (!PLATFORM_X86 || !tab_admit()) ? x86_alpha() + x86_bomb("bb_scan_tab: no result slot (op_off)") :
           x86("comment", "IR_SCAN_TAB")
         + x86_alpha()
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp64",   "rax", (long)1)
         + x86("jge",     "L0")
         + x86("add",     "rax", "r15")
         + x86("add",     "rax", (long)1)
         + x86("def",     "L0")
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
         + x86("sub",     "rsp", (long)16)
         + x86("call",    "rt_substr", (uint64_t)(uintptr_t)(void*)rt_substr)
         + x86("add",     "rsp", (long)16)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86("mov",     "r14", FRQ(_.op_off + 16))
         + x86_omega();
}
