#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi);
void rt_scan_leave(uint64_t outer_sigma, uint64_t outer_delta, uint64_t outer_len);
void rt_scan_leave_ns(uint64_t outer_sigma, uint64_t outer_delta, uint64_t outer_len);
void rt_scan_sync_out(uint64_t delta);
ScanSubjRegs rt_scan_reenter(void);
uint64_t rt_scan_sync_in(void);
uint64_t rt_scan_live_subj(void);
ScanSubjRegs rt_scan_reenter_live(uint64_t subj, uint64_t len);
}
#include "x86_asm.h"
#include <cstdlib>
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static_assert(sizeof(uint64_t) == 8, "THE SUSPEND-LEAVE BANK LIVES IN THE SCAN NODE'S OWN FIELDS AT ITS RESULT SLOT +16 (delta, RAW) AND +24 (the live subject, PTR_GC), THE OFFSETS frame_layout.c GRANTS -- IR_SCAN is not a shifted-locals kind, so zls_off is the RESULT slot and the fields start 16 past it; banking at +0/+8 put the delta in a DESCR tag word and hid the subject pointer from the walker, and a collection during the suspension handed rt_scan_reenter_live vacated ground (unitgenr, cto 2026-09-23, CTO-154)");
static int scan_bank_off() { static int said = 0; const char * e = getenv("SCRIP_GC_PLANT_SCAN_BANK"); if (!(e && *e == '1')) return 16; if (!said) { said = 1; fprintf(stderr, "[GC-SCANBANK] plant: the suspend-leave bank is emitted at the scan node's RESULT slot again, delta in the tag word and the live subject invisible to the walker (SCRIP_GC_PLANT_SCAN_BANK=1). THIS LINE IS THE ONLY PROOF THE PLANT APPLIED, so it prints ONCE PER PROCESS at the first suspend-leave box emitted.\n"); } return 0; }
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
             + x86_rt_gc_poll_rec_subject_new()
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86("mov", "r13", "rax")
             + x86("mov", "r15", "rdx")
             + x86("mov", "r14", (long)0)
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_sb != 1 && _.op_off >= 0,
               IF(_.op_sa >= 0 && _.op_ival >= 0 && _.op_sb != 2,
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_ival), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_ival + 8), "rax"))
             + IF(_.op_sb == 2,
                   x86("mov", FR(_.op_ival + scan_bank_off()), "r14d")
                 + x86("mov", FR(_.op_ival + scan_bank_off() + 4), "r15d")
                 + x86("call", "rt_scan_live_subj", (uint64_t)(uintptr_t)(void *)rt_scan_live_subj)
                 + x86("mov", FRQ(_.op_ival + scan_bank_off() + 8), "rax"))
             + x86("mov", "rdi", FRQ(_.op_off))
             + x86("mov", "rsi", FRQ(_.op_off + 8))
             + x86("comment", "the outer Delta travels with the outer Sigma: leave restores the length CACHE too, or an embedded/trailing NUL in the outer subject is lost to strlen on the way out")
             + x86("mov", "rdx", FRQ(_.op_off + 16))
             + (_.op_sb >= 2
                ? x86("call", "rt_scan_leave_ns", (uint64_t)(uintptr_t)(void *)rt_scan_leave_ns)
                : x86("call", "rt_scan_leave", (uint64_t)(uintptr_t)(void *)rt_scan_leave))
             + x86("mov", "r13", FRQ(_.op_off))
             + x86("mov", "r14", FRQ(_.op_off + 8))
             + x86("mov", "r15", FRQ(_.op_off + 16))
             + x86_gamma()
             + x86_beta()
             + IF(_.lbl_t0_p != 0 && _.op_sb == 2,
                   x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + x86("mov", "rdi", FRQ(_.op_ival + scan_bank_off() + 8))
                 + x86("mov", "esi", FR(_.op_ival + scan_bank_off() + 4))
                 + x86("call", "rt_scan_reenter_live", (uint64_t)(uintptr_t)(void *)rt_scan_reenter_live)
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("mov", "r14d", FR(_.op_ival + scan_bank_off()))
                 + x86("mov", "rdi", "r14")
                 + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out)
                 + x86_jmp_tgt(X86T_TGT0))
             + IF(_.lbl_t0_p != 0 && _.op_sb == 4,
                   x86("mov", "rdi", FRQ(_.op_sc))
                 + x86("mov", "rsi", FRQ(_.op_sc + 8))
                 + x86("call", "rt_scan_enter", (uint64_t)(uintptr_t)(void *)rt_scan_enter)
                 + x86_rt_gc_poll_rec_subject_new()
                 + x86("test", "rax", "rax")
                 + x86_omega("je")
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("mov", "r14", (long)0)
                 + x86_jmp_tgt(X86T_TGT0))
             + IF(_.lbl_t0_p != 0 && _.op_sb != 2 && _.op_sb != 4,
                   x86("call", "rt_scan_reenter", (uint64_t)(uintptr_t)(void *)rt_scan_reenter)
                 + x86("mov", "r13", "rax")
                 + x86("mov", "r15", "rdx")
                 + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
                 + x86("mov", "r14", "rax")
                 + x86_jmp_tgt(X86T_TGT0))
             + x86_omega())
         + IF(_.op_sb != 1 && _.op_off < 0, x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)"));
}
