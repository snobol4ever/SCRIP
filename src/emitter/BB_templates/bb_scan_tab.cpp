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
static std::string bb_scan_tab_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off; long n = (long) _.op_sb; int sa = _.op_sa;
    if (!(g_descr_flat_chain && off >= 0 && (n >= 1 || sa >= 0))) return std::string();
    std::string tgt = (n >= 1) ? x86("mov", "rax", n) : x86("mov", "rax", FRQ(sa + 8));
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                         + s_comment("# BOX ICN IR_SCAN_TAB tab(i) [ICN-SCAN-7 fscan.r: target in [1,Δ+1] else ω; save δ->[off+16]; δ<-i-1; rt_icn_substr(Σ,oldδ,newδ)->slot->γ; β REVERSES: δ<-saved->ω]"))
         + tgt
         + x86("cmp64", "rax", (long)1)
         + x86("jl",   PORT_OMEGA)
         + x86("mov",  "rcx", "r15")
         + x86("add",  "rcx", (long)1)
         + x86("cmp",  "rax", "rcx")
         + x86("jg",   PORT_OMEGA)
         + x86("mov", FRQ(off + 16), "r14")
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
         + x86("mov", FRQ(off),     "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("mov", "r14", FRQ(off + 16))
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_tab(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_tab_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_tab: unhandled (needs literal positive n or sibling scan-producer slot + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
