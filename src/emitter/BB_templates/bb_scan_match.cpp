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
std::string bb_scan_match() {
    x86_begin();
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1)) return x86_bomb("bb_scan_match: unhandled (needs literal string arg + descr flat-chain slot)");
    uint64_t memcmp_fp; { int (*fp)(const void *, const void *, size_t) = memcmp; memcmp_fp = (uint64_t)(uintptr_t)(void *)fp; }
    return x86("comment", "IR_SCAN_MATCH")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rax", "r15")
         + x86("sub",     "rax", "r14")
         + x86("cmp64",   "rax", (long)strlen(_.op_name1))
         + x86("jl",      "ω")
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", "r13")
         + x86("add",     "rsi", "r14")
         + x86("mov",     "rdx", (long)strlen(_.op_name1))
         + x86("push",    "r10")
         + x86("call",    "memcmp", memcmp_fp)
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
         + x86_ro_seal_str(0, _.op_name1);
}
