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
std::string bb_scan_match() {
    x86_begin();
    if (PLATFORM_X86 && _.op_off >= 0 && !_.op_name1 && _.op_sa >= 0)
        return x86("comment", "IR_SCAN_MATCH (var needle) [fstranl.r match: len=strlen(slot); fail unless Delta-delta>=len && memcmp==0; result {DT_I, delta+1+len}]")
             + x86("def",     "α")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("push",    "r10")
             + x86("call",    "strlen", (uint64_t)(uintptr_t)(void*)(size_t (*)(const char *))strlen)
             + x86("pop",     "r10")
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", "r15")
             + x86("sub",     "rax", "r14")
             + x86("mov",     "rcx", FRQ(_.op_off + 16))
             + x86("cmp",     "rax", "rcx")
             + x86("jl",      "ω")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("mov",     "rsi", "r13")
             + x86("add",     "rsi", "r14")
             + x86("mov",     "rdx", FRQ(_.op_off + 16))
             + x86("push",    "r10")
             + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
             + x86("pop",     "r10")
             + x86("test",    "eax", "eax")
             + x86("jne",     "ω")
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("mov",     "rax", "r14")
             + x86("mov",     "rcx", FRQ(_.op_off + 16))
             + x86("add",     "rax", "rcx")
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω");
    if (!PLATFORM_X86 || !(_.op_off >= 0 && _.op_name1)) return x86_bomb("bb_scan_match: unhandled (needs literal string arg + descr flat-chain slot)");
    return x86("comment", "IR_SCAN_MATCH")
         + x86("def",     "α")
         + x86("mov",     "rax", "r15")
         + x86("sub",     "rax", "r14")
         + x86("cmp64",   "rax", (long)strlen(_.op_name1))
         + x86("jl",      "ω")
         + x86("mov",     "rdi", ROQ(0))
         + x86("mov",     "rsi", "r13")
         + x86("add",     "rsi", "r14")
         + x86("mov",     "rdx", (long)strlen(_.op_name1))
         + x86("push",    "r10")
         + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
         + x86("pop",     "r10")
         + x86("test",    "eax", "eax")
         + x86("jne",     "ω")
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("mov",     "rax", "r14")
         + x86("add",     "rax", (long)(1 + (long)strlen(_.op_name1)))
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω")
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1);
}
