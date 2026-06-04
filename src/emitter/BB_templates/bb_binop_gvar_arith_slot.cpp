#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static int gva_is_arith(int64_t op) { return op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD; }
static int gva_slot_disp(int kind) { return (kind == (int)IR_CALL || kind == (int)IR_VAR_FRAME || kind == (int)IR_VAR_FRAME_REF) ? 8 : 0; }
static std::string gva_op_body(int64_t op) {
    switch (op) {
    case BINOP_ADD: return x86("add",  "rax", "rcx");
    case BINOP_SUB: return x86("sub",  "rax", "rcx");
    case BINOP_MUL: return x86("imul", "rax", "rcx");
    case BINOP_DIV: return x86("cqo") + x86("idiv", "rcx");
    case BINOP_MOD: return x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx");
    default:        return x86("add",  "rax", "rcx");
    }
}
static std::string gva_op_body_text(int64_t op) {
    switch (op) {
    case BINOP_ADD: return x86("ins2", "add",  "rax, rcx");
    case BINOP_SUB: return x86("ins2", "sub",  "rax, rcx");
    case BINOP_MUL: return x86("ins2", "imul", "rax, rcx");
    case BINOP_DIV: return x86("ins1", "cqo") + x86("ins2", "idiv", "rcx");
    case BINOP_MOD: return x86("ins1", "cqo") + x86("ins2", "idiv", "rcx") + x86("ins2", "mov", "rax, rdx");
    default:        return x86("ins2", "add",  "rax, rcx");
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_slot_str() {
    if (!PLATFORM_X86) return std::string();
    int64_t op = _.op_ival;
    if (!(g_gvar_flat_chain && _.op_off >= 0 && gva_is_arith(op))) return std::string();
    int lk = _.bb_lk, rk = _.bb_rk;
    int l_lit = (lk == (int)IR_LIT_I), l_var = (lk == (int)IR_VAR && _.op_name1);
    int r_lit = (rk == (int)IR_LIT_I), r_var = (rk == (int)IR_VAR && _.op_name2);
    if (!l_lit && !l_var && _.op_sa < 0) return std::string();
    if (!r_lit && !r_var && _.op_sb < 0) return std::string();
    if (MEDIUM_TEXT) {
        std::string s = x86("label", _.lbl_α)
            + x86("comment", emit_fmt("BOX IR_BINOP gvar-arith-slot op=%lld lk=%d rk=%d -> [r12+%d]", (long long)op, lk, rk, _.op_off));
        if (l_lit)      s += x86("ins2", "mov", emit_fmt("rax, %lld", (long long)_.bb_li));
        else if (l_var) { char b1[80]; strtab_label(b1, sizeof b1, _.op_name1);
                          s += x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_get_int@PLT"); }
        else            s += x86("ins2", "mov", emit_fmt("rax, [r12 + %d]", _.op_sa + gva_slot_disp(lk)));
        if (r_lit)      s += x86("ins2", "mov", emit_fmt("rcx, %lld", (long long)_.bb_ri));
        else if (r_var) { char b2[80]; strtab_label(b2, sizeof b2, _.op_name2);
                          s += x86("ins2", "mov", emit_fmt("qword ptr [r12 + %d], rax", _.op_off))
                             + x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b2)) + x86("ins2", "call", "rt_gvar_get_int@PLT")
                             + x86("ins2", "mov", "rcx, rax")
                             + x86("ins2", "mov", emit_fmt("rax, qword ptr [r12 + %d]", _.op_off)); }
        else            s += x86("ins2", "mov", emit_fmt("rcx, [r12 + %d]", _.op_sb + gva_slot_disp(rk)));
        s += gva_op_body_text(op)
           + x86("ins2", "mov", emit_fmt("qword ptr [r12 + %d], rax", _.op_off))
           + x86("ins2", "jmp", _.lbl_γ)
           + x86("Lins1", emit_fmt("%s:", _.lbl_β), "")
           + x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    std::string s;
    if (l_lit)      s += x86("mov", "rax", (long)_.bb_li);
    else if (l_var) s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)_.op_name1)
                       + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
    else            s += x86("mov", "rax", FRQ(_.op_sa + gva_slot_disp(lk)));
    if (r_lit)      s += x86("mov", "rcx", (long)_.bb_ri);
    else if (r_var) s += x86("mov", FRQ(_.op_off), "rax")
                       + x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)_.op_name2)
                       + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int)
                       + x86("mov", "rcx", "rax")
                       + x86("mov", "rax", FRQ(_.op_off));
    else            s += x86("mov", "rcx", FRQ(_.op_sb + gva_slot_disp(rk)));
    s += gva_op_body(op)
       + x86("mov", FRQ(_.op_off), "rax")
       + x86("jmp", PORT_GAMMA)
       + x86("def", PORT_BETA)
       + x86("jmp", PORT_OMEGA);
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gvar_arith_slot(IR_t * pBB) {
    (void)pBB;
    std::string s = bb_binop_gvar_arith_slot_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_binop_gvar_arith_slot: shape mismatch (dispatch chose this arm but predicate failed)")); return; }
    bb_emit_x86(s);
}
