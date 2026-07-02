#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
extern DESCR_t rt_section_var(DESCR_t base, DESCR_t i1, DESCR_t i2);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_section() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sb < 0) return x86_bomb("bb_section: needs own slot + base/i1/i2 operand slots");
    if (_.op_sval && _.op_sval[0] == 'l' && _.op_sval[1] == 'v' && !_.op_sval[2])
        return x86("comment", "IR_SUBSCRIPT section VARIABLE (ASSIGN-LV: rt_section_var tvsubs trap; FAIL routes ω)")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rdi", FRQ(_.op_a_slot))
             + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov",     "rdx", FRQ(_.op_sa))
             + x86("mov",     "rcx", FRQ(_.op_sa + 8))
             + x86("mov",     "r8",  FRQ(_.op_sb))
             + x86("mov",     "r9",  FRQ(_.op_sb + 8))
             + x86("call",    "rt_section_var", (uint64_t)(uintptr_t)(void *)rt_section_var)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86("je",  "ω")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    if (_.op_ival != 0) return x86_bomb("bb_section: only plain s[i:j] has a native arm (s[i+:n]/s[i-:n] pending)");
    return x86("comment", "IR_SUBSCRIPT section")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rdi", FRQ(_.op_a_slot))
         + x86("mov",     "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",     "rdx", FRQ(_.op_sa))
         + x86("mov",     "rcx", FRQ(_.op_sa + 8))
         + x86("mov",     "r8",  FRQ(_.op_sb))
         + x86("mov",     "r9",  FRQ(_.op_sb + 8))
         + x86("call",    "subscript_get2", (uint64_t)(uintptr_t)(void *)subscript_get2)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
