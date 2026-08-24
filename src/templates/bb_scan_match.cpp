#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs_needle_t;
ScanSubjRegs_needle_t rt_scan_needle(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_match() {
    x86_begin();
    return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
           x86("comment", "IR_SCAN_MATCH (var needle) [fstranl.r match: rt_scan_needle coerces (int/real->string) -> {ptr,len}; fail unless Delta-delta>=len && memcmp==0; result {DT_I, delta+1+len}]")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("push",    "r12")
             + x86("push",    "r12")
             + x86("call",    "rt_scan_needle", (uint64_t)(uintptr_t)(void*)rt_scan_needle)
             + x86("pop",     "r12")
             + x86("pop",     "r12")
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("mov",     "rax", "r15")
             + x86("sub",     "rax", "r14")
             + x86("cmp",     "rax", "rdx")
             + x86_omega("jl")
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", "r13")
             + x86("add",     "rsi", "r14")
             + x86("mov",     "rdx", FRQ(_.op_off + 24))
             + x86("push",    "r12")
             + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
             + x86("pop",     "r12")
             + x86("test",    "eax", "eax")
             + x86_omega("jne")
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("mov",     "rax", "r14")
             + x86("mov",     "rcx", FRQ(_.op_off + 24))
             + x86("add",     "rax", "rcx")
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
           (!(_.op_off >= 0 && _.op_name1)) ? x86_alpha() + x86_bomb("bb_scan_match: unhandled (needs literal string arg + descr flat-chain slot)") :
           x86("comment", "IR_SCAN_MATCH")
         + x86_alpha()
         + x86("mov",     "rax", "r15")
         + x86("sub",     "rax", "r14")
         + x86("cmp64",   "rax", (long)strlen(_.op_name1))
         + x86_omega("jl")
         + x86("mov",     "rdi", ROQ(0))
         + x86("mov",     "rsi", "r13")
         + x86("add",     "rsi", "r14")
         + x86("mov",     "rdx", (long)strlen(_.op_name1))
         + x86("push",    "r12")
         + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
         + x86("pop",     "r12")
         + x86("test",    "eax", "eax")
         + x86_omega("jne")
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("mov",     "rax", "r14")
         + x86("add",     "rax", (long)(1 + (long)strlen(_.op_name1)))
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1);
}
