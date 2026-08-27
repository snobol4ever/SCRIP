#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t c_rt_subscript_var2(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
extern DESCR_t c_rt_subscript_var2_lv(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subscript2() {
    const int         lv = _.op_sval && !strcmp(_.op_sval, "nd2-lv");
    const char *      sub_nm = lv ? "c_rt_subscript_var2_lv" : "c_rt_subscript_var2";
    const uint64_t    sub_fn = lv ? (uint64_t)(uintptr_t)(void *)c_rt_subscript_var2_lv : (uint64_t)(uintptr_t)(void *)c_rt_subscript_var2;
    if (_.op_zres)
        return x86("comment", "IR_SUBSCRIPT a[i,j] combined 2-D dispatch zd (row table-int-keys-and-nd-subscript)")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov",     "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov",     "rcx", ZOPQ(1, 8))
             + x86("note", ZOPN(2)) + x86("mov",     "r8",  ZOPQ(2, 0))
             + x86("note", ZOPN(2)) + x86("mov",     "r9",  ZOPQ(2, 8))
             + x86("call",    sub_nm, sub_fn)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sb < 0)
         ? x86_alpha() + x86_bomb("bb_subscript2: needs own slot + base/idx1/idx2 operand slots")
         : x86("comment", "IR_SUBSCRIPT a[i,j] combined 2-D dispatch (row table-int-keys-and-nd-subscript)")
         + x86_alpha()
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rdx", FRQ(_.op_sa))
         + x86("mov",     "rcx", FRQ(_.op_sa + 8))
         + x86("mov",     "r8",  FRQ(_.op_sb))
         + x86("mov",     "r9",  FRQ(_.op_sb + 8))
         + x86("call",    sub_nm, sub_fn)
         + x86("cmp",     "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
