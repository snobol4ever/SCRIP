#include <string>
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
static std::string bb_lit_scalar_str() {
    if (PLATFORM_X86) {
        if ((g_descr_flat_chain || g_gvar_callarg_live) && _.op_node_kind == (int)IR_LIT_I && _.op_off >= 0) {
            int off = _.op_off;
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_I [GZ-3 x86() stackless: {DT_I,val}->[r12+off]; val sealed RO [rip+disp] (REG-RO)]"))
                 + x86_frame_mov_imm64(off, (long)DT_I)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(off + 8, "rax")
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA)
                 + x86_ro_seal_q(0, (uint64_t) _.op_ival);
        }
        if (g_descr_flat_chain && _.op_node_kind == (int)IR_LIT_S && _.op_off >= 0) {
            int off = _.op_off;
            const char * lit = _.op_sval ? _.op_sval : "";
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX BB_LIT_scalar IR_LIT_S [GZ-11+ x86() stackless: {DT_S,0,&str}->[r12+off]; str sealed RO [rip+disp] (string REG-RO)]"))
                 + x86_frame_mov_imm64(off, (long)DT_S)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(off + 8, "rax")
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA)
                 + x86_ro_seal_str(0, lit);
        }
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX BB_LIT_scalar (pass-through; value is RO, consumer reads it [rip+disp])"))
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_lit_scalar(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_lit_scalar_str()); }
