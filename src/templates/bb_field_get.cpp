#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t dat_field_get(const char *fname, DESCR_t obj);
extern DESCR_t rt_field_var(const char *fname, DESCR_t obj);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_field_get() {
    if (!PLATFORM_X86) return std::string();
    int lv = (_.op_node_kind == IR_FIELD_VAR);
    if (_.op_zres)
        return x86("comment", lv ? "IR_FIELD_GET lv zd" : "IR_FIELD_GET zd")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("note", ZOPN(0)) + x86("mov",     "rsi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov",     "rdx", ZOPQ(0, 8))
             + (lv ? x86("call", "rt_field_var",  (uint64_t)(uintptr_t)(void *)rt_field_var)
                   : x86("call", "dat_field_get", (uint64_t)(uintptr_t)(void *)dat_field_get))
             + x86("cmp",     "eax", std::to_string((long)DT_FAIL))
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");   /* ZD-2m: the LAST value-spine member, a by-value clone of bb_deref's ZD arm with the RO field-name pointer added as the leading argument.  ONE operand -- the object -- read at its staged difference and passed in rsi/rdx as the two DESCR halves, exactly as the legacy arm passes them; the box's own cell takes the returned field.  The eax==99 omega is NOT a value-spine failure edge and must not be confused with one: it is the datatype/field mismatch guard (manual Ch.8 p.112 -- a field reference function's argument must be an object created by that prototype's creation function), and it is preserved bit-for-bit from the legacy arm.  The RO seal STAYS OUTSIDE the port bracket in both arms: it is sealed adjacent to the blob and reached [rip+disp], never through the frame, so the depth regime cannot touch it. */
    if (_.op_off < 0 || _.op_a_slot < 0) return x86_alpha() + x86_bomb("bb_field_get: needs own slot + object operand slot");
    return x86("comment", lv ? "IR_FIELD_GET lv" : "IR_FIELD_GET")
         + x86_alpha()
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + (lv ? x86("call", "rt_field_var",  (uint64_t)(uintptr_t)(void *)rt_field_var)
               : x86("call", "dat_field_get", (uint64_t)(uintptr_t)(void *)dat_field_get))
         + x86("cmp",     "eax", std::to_string((long)DT_FAIL))
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
