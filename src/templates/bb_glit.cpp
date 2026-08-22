#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glit() {
    x86_begin();
    return (PLATFORM_X86 && _.op_name1) ?
           x86("comment", "IR_GLIT (grammar literal): match RO string at [Sigma+delta]; delta<Delta bounds; gamma advances delta by len; omega on mismatch/EOS")
         + x86_alpha()
         + x86("mov",     "rax", "r15")
         + x86("sub",     "rax", "r14")
         + x86("cmp64",   "rax", (long)strlen(_.op_name1))
         + x86_omega("jl")
         + x86("mov",     "rdi", ROQ(0))
         + x86("mov",     "rsi", "r13")
         + x86("add",     "rsi", "r14")
         + x86("mov",     "rdx", (long)strlen(_.op_name1))
         + x86("sub",     "rsp", (long)8)
         + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
         + x86("add",     "rsp", (long)8)
         + x86("test",    "eax", "eax")
         + x86_omega("jne")
         + x86("add",     "r14", (long)strlen(_.op_name1))
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1) :
           x86_alpha() + x86_bomb("bb_glit: IR_GLIT with no literal string (op_name1 unset)");
}
