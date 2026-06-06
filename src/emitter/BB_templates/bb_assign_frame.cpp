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
static int baf_voff() { return 16 + (int) _.op_ival * 16; }
static std::string baf_hop() { return x86_frame_lea("rcx", 0) + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rcx", "rcx", 0); }); }
static std::string baf_hdr() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", std::string("BOX IR_ASSIGN_FRAME \"") + (_.op_sval ? _.op_sval : "") + "\" slot=" + std::to_string((int) _.op_ival)
                         + " hops=" + std::to_string((int) _.op_dval) + " rhs_kind=" + std::to_string(_.op_a_node_kind)));
}
static std::string baf_rip(const char * reg, const char * rs) { const char * rl = emit_intern_str(rs); char rb[80]; if (!rl) { strtab_label(rb, sizeof rb, rs); rl = rb; } return x86("lea", reg, "[rip + __]", (uint64_t)(uintptr_t) rs, rl); }
static int baf_known() { int k = _.op_a_node_kind;
    return k == (int) IR_LIT_I || k == (int) IR_LIT_NUL || k == (int) IR_LIT_S || k == (int) IR_VAR
        || k == (int) IR_VAR_FRAME || k == (int) IR_VAR_FRAME_REF || k == (int) IR_BINOP || k == (int) IR_CALL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string baf_lit_i()   { return baf_hop() + x86_reg_disp32_store_imm64("rcx", baf_voff(), 6) + x86_movabs_r64("rax", (uint64_t) _.op_a_ival_sg) + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rax"); }
static std::string baf_lit_nul() { return baf_hop() + x86_reg_disp32_store_imm64("rcx", baf_voff(), 0) + x86_reg_disp32_store_imm64("rcx", baf_voff() + 8, 0); }
static std::string baf_lit_s()   { return baf_hop() + x86_reg_disp32_store_imm64("rcx", baf_voff(), 1) + baf_rip("rax", _.op_a_sval ? _.op_a_sval : "") + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rax"); }
static std::string baf_var() { uint64_t fptr; { DESCR_t (*fp)(const char *) = rt_gvar_get_descr; fptr = (uint64_t)(uintptr_t)(void *) fp; }
    return baf_rip("rdi", _.op_a_sval ? _.op_a_sval : "") + x86("call", "rt_gvar_get_descr", fptr)
         + baf_hop() + x86_reg_disp32_store64("rcx", baf_voff(), "rax") + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rdx");
}
static std::string baf_vframe() {
    return x86_frame_lea("rax", 0)
         + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); })
         + x86_reg_disp32_load64("rsi", "rax", 16 + (int) _.op_a_ival_sg * 16)
         + x86_reg_disp32_load64("rdi", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8)
         + baf_hop() + x86_reg_disp32_store64("rcx", baf_voff(), "rsi") + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rdi");
}
static std::string baf_vfref() {
    return x86_frame_lea("rax", 0)
         + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); })
         + x86_reg_disp32_load64("rax", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8)
         + x86_reg_disp32_load64("rsi", "rax", 0) + x86_reg_disp32_load64("rdi", "rax", 8)
         + baf_hop() + x86_reg_disp32_store64("rcx", baf_voff(), "rsi") + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rdi");
}
static std::string baf_binop() { return x86_frame_load64("rax", _.op_a_slot) + baf_hop() + x86_reg_disp32_store_imm64("rcx", baf_voff(), 6) + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rax"); }
static std::string baf_call() { return x86_frame_load64("rax", _.op_a_slot) + x86_frame_load64("rdx", _.op_a_slot + 8) + baf_hop() + x86_reg_disp32_store64("rcx", baf_voff(), "rax") + x86_reg_disp32_store64("rcx", baf_voff() + 8, "rdx"); }
static std::string baf_arm() {
    if (_.op_a_node_kind == (int) IR_LIT_I)         return baf_lit_i();
    if (_.op_a_node_kind == (int) IR_LIT_NUL)      return baf_lit_nul();
    if (_.op_a_node_kind == (int) IR_LIT_S)        return baf_lit_s();
    if (_.op_a_node_kind == (int) IR_VAR)          return baf_var();
    if (_.op_a_node_kind == (int) IR_VAR_FRAME)    return baf_vframe();
    if (_.op_a_node_kind == (int) IR_VAR_FRAME_REF) return baf_vfref();
    if (_.op_a_node_kind == (int) IR_BINOP)        return baf_binop();
    return baf_call();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_frame_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!g_gvar_flat_chain) return x86_bomb("bb_assign_frame: gvar flat-chain only");
    if (_.op_a_node_kind == (int) IR_BINOP && _.op_a_slot < 0) return x86_bomb("bb_assign_frame int-binop: op_a_slot==-1 (binop slot not promoted)");
    if (_.op_a_node_kind == (int) IR_CALL && _.op_a_slot < 0) return x86_bomb("bb_assign_frame call-result: op_a_slot==-1 (call result slot not promoted)");
    if (!baf_known()) return x86_bomb("bb_assign_frame: unhandled rhs shape");
    return baf_hdr() + baf_arm() + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_frame(IR_t * pBB) { bb_emit_x86(bb_assign_frame_str(pBB)); }
