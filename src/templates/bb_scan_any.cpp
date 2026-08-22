#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
const char * rt_nv_cstr(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_any() {
    x86_begin();
    return (PLATFORM_X86 && _.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
           x86("comment", "IR_SCAN_ANY (var cset)")
             + x86_alpha()
             + x86("mov",     "eax", "r14d")
             + x86("cmp",     "eax", "r15d")
             + x86_omega("jge")
             + x86("movsxd",  "rcx", "r14d")
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("push",    "r11")
             + x86("call",    "strchr", (uint64_t)(uintptr_t)(void*)(const char *(*)(const char *, int))strchr)
             + x86("pop",     "r11")
             + x86("test",    "rax", "rax")
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("mov",     "rax", "r14")
             + x86("add",     "rax", (long)2)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
           (!PLATFORM_X86 || !(_.op_off >= 0 && _.op_name1)) ? x86_alpha() + x86_bomb("bb_scan_any: unhandled (needs literal cset arg + descr flat-chain slot)") :
           x86("comment", "IR_SCAN_ANY")
         + x86_alpha()
         + x86("mov",     "eax", "r14d")
         + x86("cmp",     "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd",  "rcx", "r14d")
         + x86("movzx",   "esi", "[r13+rcx]")
         + x86("mov",     "rdi", ROQ(0))
         + x86("push",    "r11")
         + x86("call",    "strchr", (uint64_t)(uintptr_t)(void*)(const char *(*)(const char *, int))strchr)
         + x86("pop",     "r11")
         + x86("test",    "rax", "rax")
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("mov",     "rax", "r14")
         + x86("add",     "rax", (long)2)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1);
}
