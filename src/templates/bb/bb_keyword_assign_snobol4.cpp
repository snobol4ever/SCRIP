#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_kw_index(const char *kw);
DESCR_t rt_kw_write_idx(int64_t idx, DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword_assign_snobol4() {
    if (_.op_a_slot < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: rhs operand slot unresolved");
    const int kwi = _.op_sval ? rt_kw_index(_.op_sval) : -1;
    if (kwi < 0) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: keyword not named by the block");
    if (_.op_zres)
        return x86("comment", "IR_KW_ASSIGN_SNOBOL4 zd [KW-3b static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("mov",     "rsi", ZOPQ(0, 0))
             + x86("mov",     "rdx", ZOPQ(0, 8))
             + x86("call",    "rt_kw_write_idx", (uint64_t)(uintptr_t)(void *)rt_kw_write_idx)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: no result slot");
    return x86("comment", "IR_KW_ASSIGN_SNOBOL4 [KW-3b static idx]")
         + x86_alpha()
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + x86("call",    "rt_kw_write_idx", (uint64_t)(uintptr_t)(void *)rt_kw_write_idx)
         + x86("cmp",     "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
}
