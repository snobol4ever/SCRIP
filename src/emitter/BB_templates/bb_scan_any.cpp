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
static std::string bb_scan_any_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off; const char * cs = _.op_name1;
    if (!(g_descr_flat_chain && off >= 0 && cs)) return std::string();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_ANY any(c) [ICN-SCAN-4 fstranl.r: δ==Δ->ω; s[δ]∈c via strchr; {DT_I,δ+2}->slot->γ; δ untouched; single-shot β->ω]"))
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    PORT_OMEGA)
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86_ro_load_q("rdi", 0)
         + x86("push",   "r10")
         + x86("call",   "strchr", strchr_ptr())
         + x86("pop",    "r10")
         + x86("test",   "rax", "rax")
         + x86("je",     PORT_OMEGA)
         + x86("mov", FRQ(off), (long)DT_I)
         + x86("mov", "rax", "r14")
         + x86("add", "rax", (long)2)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA)
         + x86_ro_seal_str(0, cs);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_any(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_any_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_any: unhandled (needs literal cset arg + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
