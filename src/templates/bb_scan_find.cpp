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
std::string bb_scan_find() {
    x86_begin();
    return (PLATFORM_X86 && _.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
           x86("comment", "IR_SCAN_FIND (var needle) [fstranl.r find: generate positions where needle@slot matches; cursor off+16, len off+24; beta resumes]")
             + x86_alpha()
             + x86("mov",     FRQ(_.op_off + 16), "r14")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("push",    "r11")
             + x86("call",    "strlen", (uint64_t)(uintptr_t)(void*)(size_t (*)(const char *))strlen)
             + x86("pop",     "r11")
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", "r15")
             + x86("mov",     "rdx", FRQ(_.op_off + 24))
             + x86("sub",     "rcx", "rdx")
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("mov",     "rsi", "r13")
             + x86("add",     "rsi", "rax")
             + x86("push",    "r11")
             + x86("call",    "memcmp", (uint64_t)(uintptr_t)(void*)(int (*)(const void *, const void *, size_t))memcmp)
             + x86("pop",     "r11")
             + x86("test",    "eax", "eax")
             + x86("jne",     L(1))
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86("def",     L(1))
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0))
             + x86_beta()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0)) :
           (!PLATFORM_X86 || !(_.op_off >= 0 && _.op_name1 && _.op_name1[0] && strlen(_.op_name1) <= 32)) ?
           x86_alpha() + x86_bomb("bb_scan_find: unhandled (needs nonempty literal needle <=32 + descr flat-chain slot)") :
           x86("comment", "IR_SCAN_FIND")
         + x86_alpha()
         + x86("mov",     FRQ(_.op_off + 16), "r14")
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("mov",     "rcx", "r15")
         + x86("sub",     "rcx", (long)strlen(_.op_name1))
         + x86("cmp",     "rax", "rcx")
         + x86_omega("jg")
         + FOR(0, (int)strlen(_.op_name1), [&](int i) -> std::string {
               return x86("mov",   "rcx", "rax")
                    + IF(i != 0, x86("add", "rcx", (long)i))
                    + x86("movzx", "esi", "[r13+rcx]")
                    + x86("cmp64", "rsi", (long)(unsigned char)_.op_name1[i])
                    + x86("jne",   L(1)); })
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("add",     "rax", (long)1)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def",     L(1))
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0))
         + x86_beta()
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0));
}
