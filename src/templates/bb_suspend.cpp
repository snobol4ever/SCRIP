#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (PLATFORM_X86)
        return (_.op_sa < 0) ? x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)") :
               x86("comment", "IR_SUSPEND yield+resume")
             + x86_alpha()
             + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(0), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(8), "rax")
             + x86_scan_sync_out()   /* ICN-SCAN-SUSPEND-SYNC: register-world exit — publish live δ (r14) to scan_pos so the caller-side sync_in / by-name callees see the callee's advance (x86_asm.h world-boundary doctrine; no-op unless g_scan_regs_live) */
             + x86_gamma()
             + x86_beta()
             + x86_scan_sync_in_rr() /* ICN-SCAN-SUSPEND-SYNC: resume re-entry — re-read scan_pos into r14 (a caller/by-name advance between resumes IS our advance); rax/rdx-preserving variant */
             + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
    return std::string();
}
