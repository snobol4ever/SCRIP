#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi);
void rt_scan_leave(uint64_t outer_sigma, uint64_t outer_delta);
void rt_scan_leave_ns(uint64_t outer_sigma, uint64_t outer_delta);
void rt_scan_sync_out(uint64_t delta);
ScanSubjRegs rt_scan_reenter(void);
uint64_t rt_scan_sync_in(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gen_scan() {
    x86_begin();
    return x86("comment", "IR_GEN_SCAN [N-3: outer Sigma/delta/Delta save-restore ONE HOME -- ENTER's own zls grant (FRQ off/+8/+16), the enclosing activation's own frame -- no C-global scan_stack]")
         + x86_alpha()
         + IF(_.op_sb == 1,
               x86("mov", FRQ(_.op_off),      "r13")
             + x86("mov", FRQ(_.op_off + 8),  "r14")
             + x86("mov", FRQ(_.op_off + 16), "r15")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_scan_enter", (uint64_t)(uintptr_t)(void *)rt_scan_enter)
             + x86("mov", "r13", "rax")
             + x86("mov", "r15", "rdx")
             + x86("mov", "r14", (long)0)
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_sb != 1 && _.op_off >= 0,
               IF(_.op_sa >= 0 && _.op_ival >= 0 && _.op_sb != 2,   /* sb=2 owns op_ival as its inner-δ bank and reads the subject descr straight from the enter's own FRQ(op_sa) slots — this ordinary-value stash copy would collide (measured: the δ bank overwrote the just-copied subject word, suspend_scan spun) */
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_ival), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_ival + 8), "rax"))
             + IF(_.op_sb == 2, x86("mov", FRQ(_.op_ival), "r14"))
             + x86("mov", "rdi", FRQ(_.op_off))
             + x86("mov", "rsi", FRQ(_.op_off + 8))
             + (_.op_sb >= 2   /* sb=2: suspend-crossing leave (banks inner δ above). sb=3: a leave of a scan a suspend crossed — no-push too, or each exhaustion leaks one scan_saved entry (its pop-based β edge is accepted dead: within-activation backtracking into such a scan would need the push, and the battery guards that shape) */
                ? x86("call", "rt_scan_leave_ns", (uint64_t)(uintptr_t)(void *)rt_scan_leave_ns)
                : x86("call", "rt_scan_leave", (uint64_t)(uintptr_t)(void *)rt_scan_leave))
             + x86("mov", "r13", FRQ(_.op_off))
             + x86("mov", "r14", FRQ(_.op_off + 8))
             + x86("mov", "r15", FRQ(_.op_off + 16))
             + x86_gamma()
             + x86_beta()
             + IF(_.lbl_t0_p != 0 && _.op_sb == 2,   /* slice 3 β: per-activation re-enter — refresh the banked outer-δ from the mirror (the outer moved during suspension), re-enter the inner env FRESH from the enter's own subject-descr slots (never the shared scan_saved stack — +1 leak per exhaustion measured), restore the banked inner δ, sync the mirror, jmp inward */
                   x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + x86("mov", "rdi", FRQ(_.op_sa))
                 + x86("mov", "rsi", FRQ(_.op_sa + 8))
                 + x86("call", "rt_scan_enter", (uint64_t)(uintptr_t)(void *)rt_scan_enter)
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("mov", "r14", FRQ(_.op_ival))
                 + x86("mov", "rdi", "r14")
                 + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out)
                 + x86_jmp_tgt(X86T_TGT0))
             + IF(_.lbl_t0_p != 0 && _.op_sb != 2,
                   x86("call", "rt_scan_reenter", (uint64_t)(uintptr_t)(void *)rt_scan_reenter)
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
                 + x86("mov", "r14", "rax")
                 + x86_jmp_tgt(X86T_TGT0))
             + x86_omega())
         + IF(_.op_sb != 1 && _.op_off < 0, x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)"));
}
