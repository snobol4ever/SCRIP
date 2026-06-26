#include <string>
#include <string.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
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
    if (live && _.op_node_kind == (int)IR_LIT_I && _.op_off >= 0)
        return x86("comment", "IR_LIT_I")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_I)
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_q(0, (uint64_t)_.op_ival);
    if (live && _.op_node_kind == (int)IR_LIT_S && _.op_off >= 0)
        return x86("comment", "IR_LIT_S")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_S)
             + IF(_.op_ival != 0, x86("mov", FR(_.op_off + 4), (long)-1))
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (live && _.op_node_kind == (int)IR_LIT_F && _.op_off >= 0)
        return x86("comment", "IR_LIT_F")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_R)
             + x86_ro_load_q("rax", 0)
             + x86("mov",    FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86_ro_seal_q(0, blsc_bits(_.op_dval));
    if (live && _.op_node_kind == (int)IR_LIT_NUL && _.op_off >= 0)
        return x86("comment", "IR_LIT_NUL")
             + x86("label",   _.lbl_α)
             + x86("mov",    FRQ(_.op_off), (long)DT_SNUL)
             + x86("mov",    FRQ(_.op_off + 8), (long)0)
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    return x86("comment", "IR_LIT_scalar")
         + x86("label",   _.lbl_α)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
