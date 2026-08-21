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
    if (PLATFORM_X86) {
        if (_.op_zres)
            return x86("comment", "IR_SUBSCRIPT x[i] variable zd")
                 + x86_alpha()
                 + x86("note", ZOPN(0)) + x86("mov",     "rdi", ZOPQ(0, 0))
                 + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 8))
                 + x86("note", ZOPN(1)) + x86("mov",     "rdx", ZOPQ(1, 0))
                 + x86("note", ZOPN(1)) + x86("mov",     "rcx", ZOPQ(1, 8))
                 + x86("call",    sub_nm, sub_fn)
                 + x86("cmp",     "eax", (long)DT_FAIL)
                 + x86_omega("je")
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
                 + x86_gamma()
                 + x86_beta_trampoline();   /* ZD-2f: operand 0 is the base aggregate and operand 1 the index (the :1537 mapping vb->op_a_slot, vi->op_sa -- same register order as the legacy arm), both by VALUE at staged differences; result to the box's own cell.  The DT_FAIL omega is GENUINE and load-bearing, not defensive: an out-of-bounds subscript FAILS rather than erroring, and SPITBOL documents looping until an array reference fails as the idiomatic traversal (manual Ch.7, arrays) -- so this edge is a language contract and must never be optimized into an error path.  ⛔ ARMED ONLY FOR n_operands==2: the dispatch at emit.cpp:992 routes 2-operand IR_SUBSCRIPT here and everything else to bb_section, which has NO ZD arm. */
        return (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0)
             ? x86_alpha() + x86_bomb("bb_subscript: needs own slot + base/index operand slots")
             : x86("comment", "IR_SUBSCRIPT x[i] variable")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("call",    sub_nm, sub_fn)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* ⭐⭐⭐ THE CONTAINER-ONLY SELECTOR (queue row `subscript-silent-accept`) -- op_sval == "container-only" is the SNOBOL4 lowerer's mark (lower_snobol4.c sx_sub_agg) and it swaps the CALLEE, nothing else: same four registers, same DT_FAIL omega test, same result store, both arms and BOTH MEDIA (x86("call", name, ptr) renders the symbol in TEXT and the baked pointer in BINARY, so m3 and m4 select the same face from the same mark).  The two names are FACES over one body -- rt_subscript_var_container_only tests the deref'd base for DT_A/DT_T, raises 235 through kwb_error and otherwise hands the ORIGINAL base straight to rt_subscript_var -- so the RTX veneer's array and table fast paths are still reached and no arm of the subscript body is duplicated here.  ⛔ THE MARK IS READ, NEVER INFERRED: an unmarked node is the permissive Icon/Snocone-list arm and must stay that way, which is also what makes the SCRIP_SUB_AGG=0 killswitch a true revert -- it stops the lowerer stamping and this selector falls back by construction.  ⛔ bb_section IS NOT AND MUST NOT BE TAUGHT THIS: it only ever sees n_operands != 2 (emit.cpp:1261) and its "lv"/"+"/"-" svals come from the Icon section lowerer, which has no aggregate rule to enforce. */
    }
    return std::string();
}
