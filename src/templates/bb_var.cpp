#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var() {
    x86_begin();
    if (PLATFORM_X86) {
        if (_.op_zres)   /* ZK-2 (s212): cells-arm -- local lives at frame slot; read it and write to ZRES(0/8) (this box's own RSP cell). Both media: x86() encoders handle both. ONE AUTHORITY: op_zres gate (same pattern as bb_lit_scalar / bb_var_global). SNOBOL4 watermark: SN4 has no lexical locals so graph_has_local=0 and this arm is invisible by construction. */
            return x86("comment", "IR_VAR local -> ZRES (ZK-2 cells arm)")
                 + x86_alpha()
                 + x86("mov",  "rax", FRQ(_.op_sa))
                 + x86("mov",  "rdx", FRQ(_.op_sa + 8))
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
                 + ((_.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx") : std::string())   /* PL-ZK-5B DUAL-WRITE (Bug 4 Option C): copy ZRES to FRQ(op_off) so bcps_spine_gen_arm's stage_arg_inline->FRQ(slot) finds the value. Guard op_off>=0: absent sentinel -1 means no ZLS slot was granted (shouldn't happen for an admitted IR_VAR, but guard defensively). ONE AUTHORITY. WREG W-0 (2026-08-12): rax/rdx already hold exactly ZRES(0)/ZRES(8)'s values from the two lines above (nothing between here and x86_gamma()/x86_beta_trampoline() touches either register, and both of those are bare jmp/label emitters with no register-content dependency), so the prior r10/r11 round-trip through ZRES was a redundant reload -- reusing rax/rdx directly is behaviorally identical and removes this site from the r10/r11 wire-claim sweep surface entirely. */
                 + x86_gamma()
                 + x86_beta_trampoline();
        return _.op_off != -1 && _.op_sa != -1 ?   /* ICN-FR-3/PL-FR-2: both op_off and op_sa may be negative for ___-relative zframe locals; -1 is the absent sentinel */
               x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
               x86_bomb("bb_var: unhandled arm (no flat-chain mode or missing slot)");
    }
    return std::string();
}
