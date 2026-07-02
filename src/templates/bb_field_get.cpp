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
    if (_.op_off < 0 || _.op_a_slot < 0) return x86_bomb("bb_field_get: needs own slot + object operand slot");
    int lv = (_.op_node_kind == IR_FIELD_VAR);
    return x86("comment", lv ? "IR_FIELD_GET lv" : "IR_FIELD_GET")
         + x86("label",   _.lbl_α)
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + (lv ? x86("call", "rt_field_var",  (uint64_t)(uintptr_t)(void *)rt_field_var)
               : x86("call", "dat_field_get", (uint64_t)(uintptr_t)(void *)dat_field_get))
         + x86("cmp",     "eax", "99")
         + x86("je",      "ω")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
