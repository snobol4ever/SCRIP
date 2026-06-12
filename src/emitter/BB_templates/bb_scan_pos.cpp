#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_pos() {
    x86_begin();
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && (long)_.op_sb >= 1)) return x86_bomb("bb_scan_pos: unhandled (needs literal positive n + descr flat-chain slot)");
    return x86("label", _.lbl_α)
         + x86("comment", "BOX ICN IR_SCAN_POS pos(n) [ICN-SCAN-3 fscan.r: succeed iff &pos==n i.e. r14==n-1; {DT_I,n}->slot->γ.node; single-shot β->ω.node]")
         + x86("cmp64", "r14", (long)((long)_.op_sb - 1))
         + x86("jne", "ω")
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), (long)_.op_sb)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}