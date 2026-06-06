#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
extern int g_gvar_flat_chain;
#include "emit.h"
#include "descr.h"
DESCR_t rt_gvar_get_descr(const char * name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_frame_ref_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!g_gvar_flat_chain) return x86_bomb("bb_assign_frame_ref: gvar flat-chain only");
    int hops = (int) _.op_dval;
    int voff = 16 + (int) _.op_ival * 16;
    int k = _.op_a_node_kind;
    std::string hop = x86_frame_lea("rcx", 0);
    for (int h = 0; h < hops; h++) hop += x86_reg_disp32_load64("rcx", "rcx", 0);
    hop += x86_reg_disp32_load64("rcx", "rcx", voff + 8);
    std::string s = IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                  + x86("comment", std::string("BOX IR_ASSIGN_FRAME_REF \"") + (_.op_sval ? _.op_sval : "") + "\" slot=" + std::to_string((int) _.op_ival)
                                  + " hops=" + std::to_string(hops) + " deref rhs_kind=" + std::to_string(k)));
    if (k == (int) IR_LIT_I) {
        s += hop + x86_reg_disp32_store_imm64("rcx", 0, 6) + x86_movabs_r64("rax", (uint64_t) _.op_a_ival_sg) + x86_reg_disp32_store64("rcx", 8, "rax");
    } else if (k == (int) IR_LIT_NUL) {
        s += hop + x86_reg_disp32_store_imm64("rcx", 0, 0) + x86_reg_disp32_store_imm64("rcx", 8, 0);
    } else if (k == (int) IR_LIT_S) {
        const char * rs = _.op_a_sval ? _.op_a_sval : "";
        const char * rl = emit_intern_str(rs); char rb[80]; if (!rl) { strtab_label(rb, sizeof rb, rs); rl = rb; }
        s += hop + x86_reg_disp32_store_imm64("rcx", 0, 1) + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t) rs, rl) + x86_reg_disp32_store64("rcx", 8, "rax");
    } else if (k == (int) IR_VAR) {
        const char * rs = _.op_a_sval ? _.op_a_sval : "";
        const char * rl = emit_intern_str(rs); char rb[80]; if (!rl) { strtab_label(rb, sizeof rb, rs); rl = rb; }
        uint64_t fptr; { DESCR_t (*fp)(const char *) = rt_gvar_get_descr; fptr = (uint64_t)(uintptr_t)(void *) fp; }
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) rs, rl) + x86("call", "rt_gvar_get_descr", fptr);
        s += hop + x86_reg_disp32_store64("rcx", 0, "rax") + x86_reg_disp32_store64("rcx", 8, "rdx");
    } else if (k == (int) IR_VAR_FRAME) {
        s += x86_frame_lea("rax", 0);
        s += FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); });
        s += x86_reg_disp32_load64("rsi", "rax", 16 + (int) _.op_a_ival_sg * 16)
           + x86_reg_disp32_load64("rdi", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8);
        s += hop + x86_reg_disp32_store64("rcx", 0, "rsi") + x86_reg_disp32_store64("rcx", 8, "rdi");
    } else if (k == (int) IR_VAR_FRAME_REF) {
        s += x86_frame_lea("rax", 0);
        s += FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); });
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8);
        s += x86_reg_disp32_load64("rsi", "rax", 0) + x86_reg_disp32_load64("rdi", "rax", 8);
        s += hop + x86_reg_disp32_store64("rcx", 0, "rsi") + x86_reg_disp32_store64("rcx", 8, "rdi");
    } else if (k == (int) IR_BINOP) {
        if (_.op_a_slot < 0) return x86_bomb("bb_assign_frame_ref int-binop: op_a_slot==-1 (binop slot not promoted)");
        s += x86_frame_load64("rax", _.op_a_slot) + hop + x86_reg_disp32_store_imm64("rcx", 0, 6) + x86_reg_disp32_store64("rcx", 8, "rax");
    } else if (k == (int) IR_CALL) {
        if (_.op_a_slot < 0) return x86_bomb("bb_assign_frame_ref call-result: op_a_slot==-1 (call result slot not promoted)");
        s += x86_frame_load64("rax", _.op_a_slot) + x86_frame_load64("rdx", _.op_a_slot + 8) + hop + x86_reg_disp32_store64("rcx", 0, "rax") + x86_reg_disp32_store64("rcx", 8, "rdx");
    } else {
        return x86_bomb("bb_assign_frame_ref: unhandled rhs shape");
    }
    s += x86("jmp", "γ")
       + x86("def", "β")
       + x86("jmp", "ω");
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_frame_ref(IR_t * pBB) { bb_emit_x86(bb_assign_frame_ref_str(pBB)); }
