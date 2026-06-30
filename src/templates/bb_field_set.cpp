#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern DESCR_t dat_field_set(const char *fname, DESCR_t obj, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_field_set() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_a_slot < 0 || _.op_sb < 0) return x86_bomb("bb_field_set: needs object operand slot + rhs slot");
    return x86("comment", "IR_FIELD_SET")
         + x86("label",   _.lbl_α)
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rcx", FRQ(_.op_sb))
         + x86("mov",     "r8",  FRQ(_.op_sb + 8))
         + x86("call",    "dat_field_set", (uint64_t)(uintptr_t)(void *)dat_field_set)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
