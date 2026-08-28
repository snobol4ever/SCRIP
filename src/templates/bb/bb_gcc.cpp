#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_icn_cset_member(const char *needle, int ch);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gcc() {
    x86_begin();
    return (_.op_name1) ?
           x86("comment", "IR_GCC (grammar char-class): match one char at [Sigma+delta] against RO-sealed cset; delta<Delta bounds; gamma advances delta by 1; omega on non-member/EOS")
         + x86_alpha()
         + x86("mov",     "eax", "r14d")
         + x86("cmp",     "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd",  "rcx", "r14d")
         + x86("movzx",   "esi", "[r13+rcx]")
         + x86("mov",     "rdi", ROQ(0))
         + x86("sub",     "rsp", (long)8)
         + x86("call",    "rt_icn_cset_member", (uint64_t)(uintptr_t)(void*)(int (*)(const char *, int))rt_icn_cset_member)
         + x86("add",     "rsp", (long)8)
         + x86("test",    "rax", "rax")
         + x86_omega("je")
         + x86("add",     "r14", (long)1)
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1) :
           x86_alpha() + x86_bomb("bb_gcc: IR_GCC with no cset string (op_name1 unset)");
}
