#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_scan_regs_live;
int is_global(const char * name);
struct DESCR_t * NV_PTR_fn(const char * name);
struct DESCR_t rt_rev_swap_fwd(long lkind, struct DESCR_t *lp, long rkind, struct DESCR_t *rp, struct DESCR_t *save, int64_t *spill);
struct DESCR_t rt_rev_swap_undo(long lkind, struct DESCR_t *lp, long rkind, struct DESCR_t *rp, struct DESCR_t *save, int64_t *spill);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rsw_kind(const char *n) { if (!n || n[0] != '&') return 0; if (!strcmp(n, "&pos")) return 1; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rsw_eff(long k, int slot, const char *n) { return (k == 0 && slot < 0 && n && is_global(n)) ? 2 : k; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rsw_cell(int lbl, int arm) {
    return x86("mov", "rdi", ROQ(lbl))
         + x86("call", "NV_PTR_fn", (uint64_t)(uintptr_t)(void *)NV_PTR_fn)
         + x86("test", "rax", "rax")
         + x86("jne", L(2 + arm * 2 + lbl))
         + x86_bomb("bb_rev_swap: NV_PTR_fn refuses this global by name, so <-> has no cell to exchange — the name is one the name table declines to hand out a pointer for (the SNOBOL4 keyword set), and an exchange that silently read &null and wrote nowhere is the failure this refusal replaces")
         + x86("def", L(2 + arm * 2 + lbl));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rsw_operands(long lk, long rk, int arm) {
    return IF(lk == 2, rsw_cell(0, arm) + x86("mov", FRQ(_.op_off + 64), "rax"))
         + IF(rk == 2, rsw_cell(1, arm) + x86("mov", "rcx", "rax"))
         + IF(rk == 0, x86("lea", "rcx", FRQ(_.op_sa)))
         + IF(rk == 1, x86("mov", "rcx", (long)0))
         + IF(lk == 2, x86("mov", "rsi", FRQ(_.op_off + 64)))
         + IF(lk == 0, x86("lea", "rsi", FRQ(_.op_sb)))
         + IF(lk == 1, x86("mov", "rsi", (long)0))
         + x86("mov", "rdi", (long)(lk == 2 ? 0 : lk))
         + x86("mov", "rdx", (long)(rk == 2 ? 0 : rk))
         + x86("lea", "r8", FRQ(_.op_off + 16))
         + (g_scan_regs_live != 0 ? x86("note", "scan_δ")
                                    + x86("lea", "r9", FRQ(_.op_off + 48))
                                  : x86("mov", "r9", (long)0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rsw_names(long lk, long rk) {
    return IF(lk == 2, x86("def", L(0)) + x86(".quad", LS(0), _.op_sval) + x86("label", LS(0)) + x86(".string", _.op_sval))
         + IF(rk == 2, x86("def", L(1)) + x86(".quad", LS(1), _.op_name2) + x86("label", LS(1)) + x86(".string", _.op_name2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rev_swap() {
    return [&](long lk, long rk) {
        return (lk < 0 || rk < 0) ? x86_alpha() + x86_bomb("bb_rev_swap: <-> keyword operand other than &pos is its own rung (wire it in rsw_kind + rsw_get/rsw_set)")
             : (_.op_off < 0) ? x86_alpha() + x86_bomb("bb_rev_swap: no result slot")
             : x86("comment", "IR_REV_SWAP")
             + x86_alpha()
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", FRQ(_.op_off + 48), "r14")
                                       + x86("note", "scan_Δ")
                                       + x86("mov", FRQ(_.op_off + 56), "r15"))
             + rsw_operands(lk, rk, 0)
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
             + rsw_operands(lk, rk, 1)
             + x86("call", "rt_rev_swap_undo", (uint64_t)(uintptr_t)(void *)rt_rev_swap_undo)
             + IF(g_scan_regs_live != 0, x86("note", "scan_δ")
                                       + x86("mov", "r14", FRQ(_.op_off + 48)))
             + x86_omega()
             + rsw_names(lk, rk);
    }(rsw_eff(rsw_kind(_.op_sval), _.op_sb, _.op_sval), rsw_eff(rsw_kind(_.op_name2), _.op_sa, _.op_name2));
}
