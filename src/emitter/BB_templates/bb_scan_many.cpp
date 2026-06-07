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
static inline uint64_t strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_many_str() {
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1)) return x86_bomb("bb_scan_many: unhandled (needs literal cset arg + descr flat-chain slot)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_MANY many(c) [ICN-SCAN-6 fstranl.r: walk p=δ while p<Δ∧s[p]∈c; p==δ->ω; {DT_I,p+1}->slot->γ; δ untouched; β->ω]"))
         + x86("mov",    "eax", "r14d")
         + x86("def",    L(0))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(1))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86_ro_load_q("rdi", 2)
         + x86("push",   "rax")
         + x86("push",   "r10")
         + x86("call",   "strchr", strchr_ptr())
         + x86("pop",    "r10")
         + x86("test",   "rax", "rax")
         + x86("pop",    "rax")
         + x86("je",     L(1))
         + x86("add",    "eax", (long)1)
         + x86("jmp",    L(0))
         + x86("def",    L(1))
         + x86("cmp",    "eax", "r14d")
         + x86("je",     "ω")
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("movsxd", "rcx", "eax")
         + x86("add",    "rcx", (long)1)
         + x86("mov", FRQ(_.op_off + 8), "rcx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86_ro_seal_str(2, _.op_name1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_many(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_scan_many_str()); }
