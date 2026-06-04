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
static inline uint64_t memcmp_ptr() { int (*fp)(const void *, const void *, size_t) = memcmp; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_match_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off; const char * s1 = _.op_name1;
    if (!(g_descr_flat_chain && off >= 0 && s1)) return std::string();
    long len = (long) strlen(s1);
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                         + s_comment("# BOX ICN IR_SCAN_MATCH match(s1) [ICN-SCAN-5 fstranl.r: Δ-δ<len->ω; memcmp(s1,Σ+δ,len) prefix; {DT_I,δ+1+len}->slot->γ; δ untouched; single-shot β->ω]"))
         + x86("mov",  "rax", "r15")
         + x86("sub",  "rax", "r14")
         + x86("cmp64", "rax", len)
         + x86("jl",   PORT_OMEGA)
         + x86_ro_load_q("rdi", 0)
         + x86("mov",  "rsi", "r13")
         + x86("add",  "rsi", "r14")
         + x86("mov",  "rdx", len)
         + x86("push", "r10")
         + x86("call", "memcmp", memcmp_ptr())
         + x86("pop",  "r10")
         + x86("test", "eax", "eax")
         + x86("jne",  PORT_OMEGA)
         + x86("mov", FRQ(off), (long)DT_I)
         + x86("mov", "rax", "r14")
         + x86("add", "rax", (long)(1 + len))
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA)
         + x86_ro_seal_str(0, s1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_match(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_match_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_match: unhandled (needs literal string arg + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
