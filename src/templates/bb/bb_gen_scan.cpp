#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi);
void rt_scan_leave(uint64_t outer_sigma, uint64_t outer_delta);
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
               IF(_.op_sa >= 0 && _.op_ival >= 0,
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_ival), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_ival + 8), "rax"))
             + x86("mov", "rdi", FRQ(_.op_off))
             + x86("mov", "rsi", FRQ(_.op_off + 8))
             + x86("call", "rt_scan_leave", (uint64_t)(uintptr_t)(void *)rt_scan_leave)
             + x86("mov", "r13", FRQ(_.op_off))
             + x86("mov", "r14", FRQ(_.op_off + 8))
             + x86("mov", "r15", FRQ(_.op_off + 16))
             + x86_gamma()
             + x86_beta()
             + IF(_.lbl_t0_p != 0,
                   x86("call", "rt_scan_reenter", (uint64_t)(uintptr_t)(void *)rt_scan_reenter)
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
                 + x86("mov", "r14", "rax")
                 + x86_jmp_tgt(X86T_TGT0))
             + x86_omega())
         + IF(_.op_sb != 1 && _.op_off < 0, x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)"));
}
