#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_scan_regs_live;
struct DESCR_t rt_rev_swap_fwd(long lkind, struct DESCR_t *lp, long rkind, struct DESCR_t *rp, struct DESCR_t *save, int64_t *spill);
struct DESCR_t rt_rev_swap_undo(long lkind, struct DESCR_t *lp, long rkind, struct DESCR_t *rp, struct DESCR_t *save, int64_t *spill);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rsw_kind(const char *n) { if (!n || n[0] != '&') return 0; if (!strcmp(n, "&pos")) return 1; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rev_swap() {
    return [&](long lk, long rk) {
        return (lk < 0 || rk < 0) ? x86_alpha() + x86_bomb("bb_rev_swap: <-> keyword operand other than &pos is its own rung (wire it in rsw_kind + rsw_get/rsw_set)")
             : ((lk == 0 && _.op_sb < 0) || (rk == 0 && _.op_sa < 0)) ? x86_alpha() + x86_bomb("bb_rev_swap: plain <-> operand has no LOWER-granted varslot")
             : (_.op_off < 0) ? x86_alpha() + x86_bomb("bb_rev_swap: no result slot")
             : x86("comment", "IR_REV_SWAP")
             + x86_alpha()
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", FRQ(_.op_off + 48), "r14")
                                       + x86("note", "scan_Δ")
                                       + x86("mov", FRQ(_.op_off + 56), "r15"))
             + x86("mov", "rdi", (long)lk)
             + (lk == 0 ? x86("lea", "rsi", FRQ(_.op_sb))
                        : x86("mov", "rsi", (long)0))
             + x86("mov", "rdx", (long)rk)
             + (rk == 0 ? x86("lea", "rcx", FRQ(_.op_sa))
                        : x86("mov", "rcx", (long)0))
             + x86("lea", "r8", FRQ(_.op_off + 16))
             + (g_scan_regs_live != 0 ? x86("note", "scan_δ")
                                        + x86("lea", "r9", FRQ(_.op_off + 48))
                                      : x86("mov", "r9", (long)0))
             + x86("call", "rt_rev_swap_fwd", (uint64_t)(uintptr_t)(void *)rt_rev_swap_fwd)
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", "r14", FRQ(_.op_off + 48)))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", FRQ(_.op_off + 48), "r14")
                                       + x86("note", "scan_Δ")
                                       + x86("mov", FRQ(_.op_off + 56), "r15"))
             + x86("mov", "rdi", (long)lk)
             + (lk == 0 ? x86("lea", "rsi", FRQ(_.op_sb))
                        : x86("mov", "rsi", (long)0))
             + x86("mov", "rdx", (long)rk)
             + (rk == 0 ? x86("lea", "rcx", FRQ(_.op_sa))
                        : x86("mov", "rcx", (long)0))
             + x86("lea", "r8", FRQ(_.op_off + 16))
             + (g_scan_regs_live != 0 ? x86("note", "scan_δ")
                                        + x86("lea", "r9", FRQ(_.op_off + 48))
                                      : x86("mov", "r9", (long)0))
             + x86("call", "rt_rev_swap_undo", (uint64_t)(uintptr_t)(void *)rt_rev_swap_undo)
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", "r14", FRQ(_.op_off + 48)))
             + x86_omega();
    }(rsw_kind(_.op_sval), rsw_kind(_.op_name2));
}
