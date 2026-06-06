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
static int bafr_voff() { return 16 + (int) _.op_ival * 16; }
static std::string bafr_hop() { return x86_frame_lea("rcx", 0) + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rcx", "rcx", 0); }) + x86_reg_disp32_load64("rcx", "rcx", bafr_voff() + 8); }
static std::string bafr_hdr() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", std::string("BOX IR_ASSIGN_FRAME_REF \"") + (_.op_sval ? _.op_sval : "") + "\" slot=" + std::to_string((int) _.op_ival)
                         + " hops=" + std::to_string((int) _.op_dval) + " deref rhs_kind=" + std::to_string(_.op_a_node_kind)));
}
static std::string bafr_rip(const char * reg, const char * rs) { const char * rl = emit_intern_str(rs); char rb[80]; if (!rl) { strtab_label(rb, sizeof rb, rs); rl = rb; } return x86("lea", reg, "[rip + __]", (uint64_t)(uintptr_t) rs, rl); }
static int bafr_known() { int k = _.op_a_node_kind;
    return k == (int) IR_LIT_I || k == (int) IR_LIT_NUL || k == (int) IR_LIT_S || k == (int) IR_VAR
        || k == (int) IR_VAR_FRAME || k == (int) IR_VAR_FRAME_REF || k == (int) IR_BINOP || k == (int) IR_CALL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bafr_lit_i()   { return bafr_hop() + x86_reg_disp32_store_imm64("rcx", 0, 6) + x86_movabs_r64("rax", (uint64_t) _.op_a_ival_sg) + x86_reg_disp32_store64("rcx", 8, "rax"); }
static std::string bafr_lit_nul() { return bafr_hop() + x86_reg_disp32_store_imm64("rcx", 0, 0) + x86_reg_disp32_store_imm64("rcx", 8, 0); }
static std::string bafr_lit_s()   { return bafr_hop() + x86_reg_disp32_store_imm64("rcx", 0, 1) + bafr_rip("rax", _.op_a_sval ? _.op_a_sval : "") + x86_reg_disp32_store64("rcx", 8, "rax"); }
static std::string bafr_var() { uint64_t fptr; { DESCR_t (*fp)(const char *) = rt_gvar_get_descr; fptr = (uint64_t)(uintptr_t)(void *) fp; }
    return bafr_rip("rdi", _.op_a_sval ? _.op_a_sval : "") + x86("call", "rt_gvar_get_descr", fptr)
         + bafr_hop() + x86_reg_disp32_store64("rcx", 0, "rax") + x86_reg_disp32_store64("rcx", 8, "rdx");
}
static std::string bafr_vframe() {
    return x86_frame_lea("rax", 0)
         + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); })
         + x86_reg_disp32_load64("rsi", "rax", 16 + (int) _.op_a_ival_sg * 16)
         + x86_reg_disp32_load64("rdi", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8)
         + bafr_hop() + x86_reg_disp32_store64("rcx", 0, "rsi") + x86_reg_disp32_store64("rcx", 8, "rdi");
}
static std::string bafr_vfref() {
    return x86_frame_lea("rax", 0)
         + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86_reg_disp32_load64("rax", "rax", 0); })
         + x86_reg_disp32_load64("rax", "rax", 16 + (int) _.op_a_ival_sg * 16 + 8)
         + x86_reg_disp32_load64("rsi", "rax", 0) + x86_reg_disp32_load64("rdi", "rax", 8)
         + bafr_hop() + x86_reg_disp32_store64("rcx", 0, "rsi") + x86_reg_disp32_store64("rcx", 8, "rdi");
}
static std::string bafr_binop() { return x86_frame_load64("rax", _.op_a_slot) + bafr_hop() + x86_reg_disp32_store_imm64("rcx", 0, 6) + x86_reg_disp32_store64("rcx", 8, "rax"); }
static std::string bafr_call() { return x86_frame_load64("rax", _.op_a_slot) + x86_frame_load64("rdx", _.op_a_slot + 8) + bafr_hop() + x86_reg_disp32_store64("rcx", 0, "rax") + x86_reg_disp32_store64("rcx", 8, "rdx"); }
static std::string bafr_arm() {
    if (_.op_a_node_kind == (int) IR_LIT_I)         return bafr_lit_i();
    if (_.op_a_node_kind == (int) IR_LIT_NUL)      return bafr_lit_nul();
    if (_.op_a_node_kind == (int) IR_LIT_S)        return bafr_lit_s();
    if (_.op_a_node_kind == (int) IR_VAR)          return bafr_var();
    if (_.op_a_node_kind == (int) IR_VAR_FRAME)    return bafr_vframe();
    if (_.op_a_node_kind == (int) IR_VAR_FRAME_REF) return bafr_vfref();
    if (_.op_a_node_kind == (int) IR_BINOP)        return bafr_binop();
    return bafr_call();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_frame_ref_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!g_gvar_flat_chain) return x86_bomb("bb_assign_frame_ref: gvar flat-chain only");
    if (_.op_a_node_kind == (int) IR_BINOP && _.op_a_slot < 0) return x86_bomb("bb_assign_frame_ref int-binop: op_a_slot==-1 (binop slot not promoted)");
    if (_.op_a_node_kind == (int) IR_CALL && _.op_a_slot < 0) return x86_bomb("bb_assign_frame_ref call-result: op_a_slot==-1 (call result slot not promoted)");
    if (!bafr_known()) return x86_bomb("bb_assign_frame_ref: unhandled rhs shape");
    return bafr_hdr() + bafr_arm() + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_frame_ref(IR_t * pBB) { bb_emit_x86(bb_assign_frame_ref_str(pBB)); }
