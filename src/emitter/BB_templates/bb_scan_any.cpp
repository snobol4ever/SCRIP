#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_any() {
    x86_begin();
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1)) return x86_bomb("bb_scan_any: unhandled (needs literal cset arg + descr flat-chain slot)");
    return x86("comment", "IR_SCAN_ANY")
         + x86("label",   _.lbl_α)
         + x86("mov",     "eax", "r14d")
         + x86("cmp",     "eax", "r15d")
         + x86("jge",     "ω")
         + x86("movsxd",  "rcx", "r14d")
         + x86("movzx",   "esi", "[r13+rcx]")
         + x86("mov",     "rdi", ROQ(0))
         + x86("push",    "r10")
         + x86("call",    "strchr", (uint64_t)(uintptr_t)(void*)(const char *(*)(const char *, int))strchr)
         + x86("pop",     "r10")
         + x86("test",    "rax", "rax")
         + x86("je",      "ω")
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("mov",     "rax", "r14")
         + x86("add",     "rax", (long)2)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω")
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_name1)
         + x86("label",   LS(0))
         + x86(".string", _.op_name1);
}
