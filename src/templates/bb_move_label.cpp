#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_retry_push(void *addr);
void rt_pl_cp_push(void *addr);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_move_label() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_move_label: no shared-slot owner (op_off<0)");
    if (!_.lbl_t0) return x86_alpha() + x86_bomb("bb_move_label: resume-target label unresolved (lbl_t0 NULL)");
    /* PL-ZK-4: ZD arm -- push the callee α label (lbl_t1) to g_pl_retry retry stack.  lbl_t1 was staged by the emit.cpp pre-pass when wantb=1 on a pl_cells_graph.  rt_pl_retry_push(addr) is the WAM ALTB-write equivalent (frame-independent LIFO).  The value copy (op_sa -> op_off DESCR pair) is unchanged; only the continuation pointer storage moves from FRQ(op_off+16) to the retry stack so it survives ζ-frame epilogue teardown.  SN4/Icon watermarks: _.op_zres=0 for non-pl_cells graphs by ZK-3 choke suppression -- byte-identical. */
    if (_.op_zres && _.lbl_t1_p) {   /* PL-ZK-5A: lbl_t1_p staged only when wantb=1 (choice-point); cut-barrier nodes (wantb=0) leave lbl_t1_p=NULL -- guard prevents null deref on x86_lea_tgt(X86T_TGT1). Non-ZD arm below handles wantb=0 correctly. ONE AUTHORITY: emit.cpp pre-pass line ~2561 sets lbl_t1 only for wantb=1. */
        uint64_t push_fp; { void (*fp)(void *) = rt_pl_retry_push; push_fp = (uint64_t)(uintptr_t)(void *)fp; }
        return x86("comment", "IR_MOVE_LABEL (ZD/cells: push alpha to retry stack)")
             + x86_alpha()
             + IF(_.op_sa >= 0 && _.op_sa != _.op_off,
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_off), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_off + 8), "rax"))
             + x86_lea_tgt("rdi", X86T_TGT1)
             + x86("call", "rt_pl_retry_push", push_fp)
             + x86_gamma()
             + x86_beta_trampoline();
    }
    /* PL-FR-4 ZFRAME DESIGN NOTE (s10, 2026-08-08): the frame-independent retry stack (g_pl_cp_stack,
     * rt_pl_cp_push/pop, rt.c) is the correct mechanism.  The address to push is NOT the callee α
     * (re-entering α resets the generator frame and always retries clause 1).  It is NOT the β-resume
     * (the frame that β lives in is torn down by the ζ epilogue).
     * The correct source is the GENERATOR SUSPEND β label INSIDE the disjunction proc — the same address
     * the working non-zframe (ZD/cells, op_zres=1) path pushes (observed: "n51_suspend_β" in bt6.pl output).
     * BUT that address is usable only if the disjunction proc's ζ-frame is still live at jump time.
     * Under ζ-frames, the disjunction proc's frame is torn down after each yield.
     * FULL RESOLUTION: store the suspend-β address in the PLJ heap choice-point record (allocated outside
     * the frame) at suspend time; push the heap CP record pointer to g_pl_cp_stack at move_label time;
     * at bb_indirect_goto, pop the CP pointer and read the stored suspend-β to jump there AFTER re-allocating
     * a fresh disjunction proc frame (via rt_proc_call_open_det).  This is the rt_pl_cp_set_retry/get_retry
     * design the FR-4 cursor names.  Multi-rung work: (A) PLJ heap CP record for clause cursor; (B) save
     * suspend-β to heap CP at n14_suspend_α; (C) restore and jump at indirect_goto.  DEFERRED to next session. */
    return x86("comment", "IR_MOVE_LABEL")
         + x86_alpha()
         + IF(_.op_sa >= 0 && _.op_sa != _.op_off,
               x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86_lea_tgt("rax", X86T_TGT0)
         + x86("mov", FRQ(_.op_off + 16), "rax")
         + x86_gamma()
         + x86_beta_trampoline();
}
