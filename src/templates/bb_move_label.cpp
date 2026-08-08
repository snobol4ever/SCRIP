#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_retry_push(void *addr);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_move_label() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_move_label: no shared-slot owner (op_off<0)");
    if (!_.lbl_t0) return x86_alpha() + x86_bomb("bb_move_label: resume-target label unresolved (lbl_t0 NULL)");
    /* PL-ZK-4: ZD arm -- push the callee α label (lbl_t1) to g_pl_retry retry stack.  lbl_t1 was staged by the emit.cpp pre-pass when wantb=1 on a pl_cells_graph.  rt_pl_retry_push(addr) is the WAM ALTB-write equivalent (frame-independent LIFO).  The value copy (op_sa -> op_off DESCR pair) is unchanged; only the continuation pointer storage moves from FRQ(op_off+16) to the retry stack so it survives ζ-frame epilogue teardown.  SN4/Icon watermarks: _.op_zres=0 for non-pl_cells graphs by ZK-3 choke suppression -- byte-identical. */
    if (_.op_zres) {
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
