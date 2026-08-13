#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_bound() {
    x86_begin();
    if (!PLATFORM_X86 || _.op_off < 0) return x86_alpha() + x86_bomb("bb_bound: no mark slot (op_off)");
    /* ZK-3 CELLS ARM (s213): op_zres=1 means the cells arm is active (set by the staging choke when
     * zd_wl_kind admits IR_BOUND/IR_UNMARK on icn_cells_graph).  BOUND (op_sb==1): save RSP to the frame
     * slot via FRQ(op_off) so the UNMARK can compute depth-delta; no FORTH cell carved (K=0, pure bracket).
     * UNMARK (op_sb!=1): the accumulated depth-since-BOUND is op_zgpop (computed by zd_plan: UNMARK's gamma
     * exits the bound, so zgpop[unmark] = K_total_since_mark); x86_gamma() emits the add rsp,op_zgpop
     * synth automatically when op_zgpop>0 (x86_asm.h X86H_JMP/GAMMA hook).  BOTH-MEDIUM: x86() encoders
     * handle TEXT/BINARY.  SNOBOL4/ZFRAME watermark: op_zres=0 for non-cells graphs -> legacy slot arm. */
    if (_.op_zres) {
        if (_.op_sb == 1)
            return x86("comment", "IR_BOUND cells arm -- save rsp to frame slot (Op_Mark: bounded-expression entry frontier)")
                 + x86_alpha()
                 + x86("mov", FRQ(_.op_off), "rsp")
                 + x86_gamma()
                 + x86_beta_trampoline();
        /* ZK-3 UNMARK cells arm: restore RSP from the BOUND's frame slot (Op_Unmark: rsp=efp semantics).
         * op_off points to the paired BOUND's zls slot (set by the dispatch: op_off = zls_off(operands[0])).
         * FRQ(op_off) = [___+N] is depth-immune so this works at any FORTH spine depth.
         * We do NOT use op_zgpop here: op_zgpop is the absolute depth-from-statement-start (= K_total),
         * but the BOUND/UNMARK bracket spans only a SUBSET of the statement's cells -- subtracting K_total
         * overshoots and releases the TO counter+lo+hi cells that must persist across the every-do loop body.
         * The correct release is exactly the cells between BOUND and UNMARK entry, which is what
         * the saved RSP encodes: restored RSP == RSP at BOUND alpha == RSP after TO carved its 32B.
         * Measured: op_zgpop=96 caused add rsp,96 which released the TO cells and corrupted the counter.
         * MUST zero op_zgpop before x86_gamma(): the planner sets op_zgpop=K_total (absolute), and
         * x86_gamma()'s X86H_JMP hook emits `add rsp,op_zgpop` when op_zgpop>0 -- without zeroing,
         * both the mov-rsp-restore AND the add would fire, double-releasing and corrupting RSP. */
        _.op_zgpop = 0;
        return x86("comment", "IR_UNMARK cells arm -- restore rsp from BOUND frame slot (depth-immune FRQ)")
             + x86_alpha()
             + x86("mov", "rsp", FRQ(_.op_off))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", _.op_sb == 1 ? "IR_BOUND" : "IR_UNMARK")
         + x86_alpha()
         + IF(_.op_sb == 1, x86("mov", FRQ(_.op_off), "rsp"))
         + IF(_.op_sb != 1, x86("mov", "rsp", FRQ(_.op_off)))
         + x86_gamma()
         + x86_beta_trampoline();
}
