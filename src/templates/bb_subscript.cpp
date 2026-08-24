#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subscript() {
    const int conly = _.op_sval && !strcmp(_.op_sval, "container-only");
    const char *      sub_nm = conly ? "rt_subscript_var_container_only" : "rt_subscript_var";
    const uint64_t    sub_fn = conly ? (uint64_t)(uintptr_t)(void *)rt_subscript_var_container_only : (uint64_t)(uintptr_t)(void *)rt_subscript_var;
    if (_.op_zres)
        return x86("comment", "IR_SUBSCRIPT x[i] variable zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov",     "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov",     "rcx", ZOPQ(1, 8))
             + x86("call",    sub_nm, sub_fn)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0)
         ? x86_alpha() + x86_bomb("bb_subscript: needs own slot + base/index operand slots")
         : x86("comment", "IR_SUBSCRIPT x[i] variable")
         + x86_alpha()
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rdx", FRQ(_.op_sa))
         + x86("mov",     "rcx", FRQ(_.op_sa + 8))
         + x86("call",    sub_nm, sub_fn)
         + x86("cmp",     "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
    return std::string();
}
