#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_icn_substr(const char *sigma, int64_t a, int64_t b);
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline uint64_t substr_ptr() { DESCR_t (*fp)(const char *, int64_t, int64_t) = rt_icn_substr; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_tab() {
    x86_begin();
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && ((long)_.op_sb >= 1 || _.op_sa >= 0))) return x86_bomb("bb_scan_tab: unhandled (needs literal positive n or sibling scan-producer slot + descr flat-chain slot)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_TAB tab(i) [ICN-SCAN-7 fscan.r: target in [1,Δ+1] else ω; save δ->[off+16]; δ<-i-1; rt_icn_substr(Σ,oldδ,newδ)->slot->γ.node; β"
                           " REVERSES: δ<-saved->ω.node]"))
         + IF((long)_.op_sb >= 1, x86("mov", "rax", (long)_.op_sb))
         + IF((long)_.op_sb < 1,  x86("mov", "rax", FRQ(_.op_sa + 8)))
         + x86("cmp64", "rax", (long)1)
         + x86("jl",   "ω")
         + x86("mov",  "rcx", "r15")
         + x86("add",  "rcx", (long)1)
         + x86("cmp",  "rax", "rcx")
         + x86("jg",   "ω")
         + x86("mov", FRQ(_.op_off + 16), "r14")
         + x86("mov",  "rdi", "r13")
         + x86("mov",  "rsi", "r14")
         + x86("mov",  "rdx", "rax")
         + x86("sub",  "rdx", (long)1)
         + x86("mov",  "r14", "rdx")
         + x86("push", "r10")
         + x86("push", "r10")
         + x86("call", "rt_icn_substr", substr_ptr())
         + x86("pop",  "r10")
         + x86("pop",  "r10")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("mov", "r14", FRQ(_.op_off + 16))
         + x86("jmp", "ω");
}