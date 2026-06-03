#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_icn_scan_enter(uint64_t lo, uint64_t hi, uint64_t sigma, uint64_t delta, uint64_t Delta);
void rt_icn_scan_leave(uint64_t *out3);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gen_scan_str() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_sb == 1) {
        int sa = _.op_sa;
        ScanSubjRegs (*fp)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = rt_icn_scan_enter; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_comment("# BOX ICN IR_GEN_SCAN enter [ICN-SCAN-0: ledger pushes prior triple; subject DESCR slot->rt->SIGMA=r13 delta=r14(0 for pos=1) DELTA=r15->body]"))
             + x86("mov", "rdi", FRQ(sa))
             + x86("mov", "rsi", FRQ(sa + 8))
             + x86("mov", "rdx", "r13")
             + x86("mov", "rcx", "r14")
             + x86("mov", "r8",  "r15")
             + x86("call", "rt_icn_scan_enter", fptr)
             + x86("mov", "r13", "rax")
             + x86("mov", "r15", "rdx")
             + x86("mov", "r14", (long)0)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (_.op_off < 0) return std::string();
    int off = _.op_off;
    void (*fp)(uint64_t *) = rt_icn_scan_leave; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
    return IF(MEDIUM_TEXT, s_comment("# BOX ICN IR_GEN_SCAN leave [ICN-SCAN-0: rt restores &subject/&pos + pops prior triple through frame out-area -> r13/r14/r15]"))
         + x86_frame_lea("rdi", off)
         + x86("call", "rt_icn_scan_leave", fptr)
         + x86("mov", "r13", FRQ(off))
         + x86("mov", "r14", FRQ(off + 8))
         + x86("mov", "r15", FRQ(off + 16))
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gen_scan(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_gen_scan_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)")); return; }
    bb_emit_x86(s);
}
