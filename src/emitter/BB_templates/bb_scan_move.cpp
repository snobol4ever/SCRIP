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
static std::string bb_scan_move_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off; long n = (long) _.op_sb; int sa = _.op_sa;
    if (!(g_descr_flat_chain && off >= 0 && sa == 1)) return std::string();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_MOVE move(i) [ICN-SCAN-8 fscan.r: target=δ+1+i in [1,Δ+1] else ω; save δ->[off+16]; δ+=i; rt_icn_substr(Σ,oldδ,newδ)->slot->γ; β"
                           " REVERSES: δ<-saved->ω]"))
         + x86("mov",  "rax", "r14")
         + x86("add",  "rax", (long)(1 + n))
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
extern "C" void bb_scan_move(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_move_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_move: unhandled (needs literal integer arg + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
