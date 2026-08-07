#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_GALT (RK-GRAM-3d): two-arm grammar alternation with delta-restore-on-beta.
 * Grammar procs have zframe_graph=1 but do NOT pin rbp (emit_rec_pin()=0 for grammar nodes --
 * they carry no IR_SUSPEND/SCAN/etc.).  The δ-save slot therefore lives at [rsp+op_off] using
 * LITERAL-RSP addressing ("dword ptr [rsp + N]") NOT FR(off) -- FR uses the frame-base selector
 * which resolves to rsp+op_zdepth for unpinned graphs, adding depth compensation that is zero
 * here (K=0 for all grammar nodes) but the x86 parser dispatches via XK_RSP32 when it sees
 * "dword ptr [rsp + N]", bypassing x86_fb_data() entirely.  op_off is always 0 for grammar procs
 * (first slot in the zframe data region, below the wire header at [rsp+kt-24/16/8]).
 *
 * lbl_t0 = arm-1 alpha label (operands[1] root; emitter jmps here from alpha).
 * lbl_t1 = arm-2 alpha label (operands[0] root; emitter jmps here from beta).
 * IR_GALT.gamma=NULL (proc success), IR_GALT.omega=NULL (proc failure) -- DRIVE_FILL resolves
 * these to flat_succ_p/flat_fail_p = proc_gram__..._gamma/omega exit labels. */
std::string bb_rk_galt() {
    x86_begin();
    if (!PLATFORM_X86 || _.op_off < 0 || !_.lbl_t0 || !_.lbl_t1)
        return x86_alpha() + x86_bomb("bb_rk_galt: IR_GALT missing dslot or arm labels");
    std::string dptr = "dword ptr [rsp + " + std::to_string(_.op_off) + "]";
    return x86("comment", "IR_GALT (grammar alternation): save delta to [rsp+dslot] at alpha; restore+jmp-arm2 at beta")
         + x86_alpha()
         + x86("mov", dptr, "r14d")   /* save delta (dword cursor) to rsp-relative zframe slot */
         + x86("jmp", _.lbl_t0)       /* explicit jmp to arm-1 alpha */
         + x86_beta()
         + x86("mov", "r14d", dptr)   /* restore delta at beta (arm-1 exhausted) */
         + x86("jmp", _.lbl_t1)       /* enter arm-2 alpha */
         + x86_gamma()                /* proc success (DRIVE_FILL -> flat_succ_p = proc_gamma) */
         + x86_omega();               /* proc failure (DRIVE_FILL -> flat_fail_p = proc_omega) */
}
