#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_pos_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off; long n = (long) _.op_sb;
    if (!(g_descr_flat_chain && off >= 0 && n >= 1)) return std::string();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_POS pos(n) [ICN-SCAN-3 fscan.r: succeed iff &pos==n i.e. r14==n-1; {DT_I,n}->slot->γ; single-shot β->ω]"))
         + x86("cmp64", "r14", (long)(n - 1))
         + x86("jne", PORT_OMEGA)
         + x86("mov", FRQ(off),     (long)DT_I)
         + x86("mov", FRQ(off + 8), n)
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_pos(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_pos_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_pos: unhandled (needs literal positive n + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
