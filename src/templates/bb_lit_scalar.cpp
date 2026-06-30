#include <string>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
extern int g_gvar_callarg_live;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static uint64_t blsc_bits(double d) { uint64_t b; memcpy(&b, &d, 8); return b; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_scalar() {
    if (!PLATFORM_X86) return std::string();
    int live = g_descr_flat_chain || g_gvar_callarg_live;
    if (live && _.op_node_kind == (int)IR_LIT_INTEGER && _.op_off >= 0)
        return x86("comment", "IR_LIT_INTEGER")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_I)
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_q(0, (uint64_t)_.op_ival);
    if (live && _.op_node_kind == (int)IR_LIT_STRING && _.op_off >= 0)
        return x86("comment", "IR_LIT_STRING")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_S)
             + IF(_.op_ival != 0, x86("mov", FR(_.op_off + 4), (long)-1))
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (live && _.op_node_kind == (int)IR_LIT_REAL && _.op_off >= 0)
        return x86("comment", "IR_LIT_REAL")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_R)
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_q(0, blsc_bits(_.op_dval));
    return x86("comment", "IR_LIT_scalar")
         + x86("label",   _.lbl_α)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
