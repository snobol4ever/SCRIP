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
    x86_begin();
    if (PLATFORM_X86) {
        if ((g_descr_flat_chain || g_gvar_callarg_live) && _.op_node_kind == (int)IR_LIT_I && _.op_off >= 0)
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_I [GZ-3 x86() stackless: {DT_I,val}->[r12+off]; val sealed RO [rip+disp] (REG-RO)]"))
                 + x86_frame_mov_imm64(_.op_off, (long)DT_I)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(_.op_off + 8, "rax")
                 + x86("jmp", "γ")
                 + x86("def", "β")
                 + x86("jmp", "ω")
                 + x86_ro_seal_q(0, (uint64_t) _.op_ival);
        if ((g_descr_flat_chain || g_gvar_callarg_live) && _.op_node_kind == (int)IR_LIT_S && _.op_off >= 0)
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_S [GZ-11+ x86() stackless: {DT_S,0,&str}->[r12+off]; str sealed RO [rip+disp] (string REG-RO)]"))
                 + x86_frame_mov_imm64(_.op_off, (long)DT_S)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(_.op_off + 8, "rax")
                 + x86("jmp", "γ")
                 + x86("def", "β")
                 + x86("jmp", "ω")
                 + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
        if ((g_descr_flat_chain || g_gvar_callarg_live) && _.op_node_kind == (int)IR_LIT_F && _.op_off >= 0)
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_F [HY-7f x86() stackless: {DT_R,bits}->[r12+off]; bits sealed RO [rip+disp] (REG-RO)]"))
                 + x86_frame_mov_imm64(_.op_off, (long)DT_R)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(_.op_off + 8, "rax")
                 + x86("jmp", "γ")
                 + x86("def", "β")
                 + x86("jmp", "ω")
                 + x86_ro_seal_q(0, blsc_bits(_.op_dval));
        if ((g_descr_flat_chain || g_gvar_callarg_live) && _.op_node_kind == (int)IR_LIT_NUL && _.op_off >= 0)
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_NUL [HY-7f x86() stackless: {DT_SNUL,0}->[r12+off] (null = two zero immediates, no RO seal)]"))
                 + x86_frame_mov_imm64(_.op_off, (long)DT_SNUL)
                 + x86_frame_mov_imm64(_.op_off + 8, (long)0)
                 + x86("jmp", "γ")
                 + x86("def", "β")
                 + x86("jmp", "ω");
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX BB_LIT_scalar (pass-through; value is RO, consumer reads it [rip+disp])"))
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return std::string();
}