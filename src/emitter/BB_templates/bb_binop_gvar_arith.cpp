#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
extern int64_t rt_gvar_arith(const char *a, const char *b, int op);
extern int64_t rt_gvar_get_int(const char *name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return std::string();
    int64_t op  = _.op_ival;
    if (!(op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD)) return std::string();
    if (_.op_name1 && _.op_name2) {
        if (MEDIUM_TEXT) {
            char b1[80], b2[80];
            strtab_label(b1, sizeof b1, _.op_name1);
            strtab_label(b2, sizeof b2, _.op_name2);
            return s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX SNO IR_BINOP gvar-arith VAR+VAR [RO name ptrs, FRQ slot, @PLT]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", b1))
                 + s_2asm("lea", emit_fmt("rsi, [rip + %s]", b2))
                 + s_2asm("mov", emit_fmt("rdx, %lld", (long long)op))
                 + s_2asm("call", "rt_gvar_arith@PLT")
                 + s_2asm("mov", emit_fmt("qword ptr [r12 + %d], rax", _.op_off))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp", _.lbl_ω);
        }
        return x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)_.op_name1)
             + x86_load_ro("rsi", "??", (uint64_t)(uintptr_t)_.op_name2)
             + x86("mov",  "rdx", (long)op)
             + x86("call", "rt_gvar_arith", (uint64_t)(uintptr_t)(void *)rt_gvar_arith)
             + x86("mov",  FRQ(_.op_off), "rax")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    if (_.op_name1 || _.op_name2) {
        const char * vn  = _.op_name1 ? _.op_name1 : _.op_name2;
        long         imm = _.op_name1 ? (long)_.op_sb : (long)_.op_sa;
        int          voff = _.op_off;
        std::string opb;
        switch (op) {
        case BINOP_ADD: opb = x86("add",  "rax", "rcx"); break;
        case BINOP_SUB: opb = x86("sub",  "rax", "rcx"); break;
        case BINOP_MUL: opb = x86("imul", "rax", "rcx"); break;
        case BINOP_DIV: opb = x86("cqo") + x86("idiv", "rcx"); break;
        case BINOP_MOD: opb = x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
        default:        opb = x86("add",  "rax", "rcx"); break;
        }
        std::string seq;
        if (MEDIUM_TEXT) {
            char b1[80];
            strtab_label(b1, sizeof b1, vn);
            seq  = s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX IR_BINOP gvar-arith %s op=%lld -> [r12+%d]", _.op_name1 ? "VAR+LIT" : "LIT+VAR", (long long)op, voff))
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", b1))
                 + s_2asm("call", "rt_gvar_get_int@PLT");
            if (_.op_name1) seq += s_2asm("mov", emit_fmt("rcx, %ld", imm));
            else            seq += s_2asm("mov", "rcx, rax") + s_2asm("mov", emit_fmt("rax, %ld", imm));
            seq += opb
                 + s_2asm("mov", emit_fmt("qword ptr [r12 + %d], rax", voff))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp", _.lbl_ω);
            return seq;
        }
        seq  = x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)vn)
             + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
        if (_.op_name1) seq += x86("mov", "rcx", imm);
        else            seq += x86("mov", "rcx", "rax") + x86("mov", "rax", imm);
        seq += opb
             + x86("mov", FRQ(voff), "rax")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
        return seq;
    }
    long    lhs = (long)_.op_sa;
    long    rhs = (long)_.op_sb;
    int     off = _.op_off;
    std::string opb;
    switch (op) {
    case BINOP_ADD: opb = x86("add",  "rax", "rcx"); break;
    case BINOP_SUB: opb = x86("sub",  "rax", "rcx"); break;
    case BINOP_MUL: opb = x86("imul", "rax", "rcx"); break;
    case BINOP_DIV: opb = x86("cqo") + x86("idiv", "rcx"); break;
    case BINOP_MOD: opb = x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
    default:        opb = x86("add",  "rax", "rcx"); break;
    }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_BINOP gvar-arith lhs=%ld op=%lld rhs=%ld -> [r12+%d]", lhs, (long long)op, rhs, off)))
         + x86("mov", "rax", lhs)
         + x86("mov", "rcx", rhs)
         + opb
         + x86("mov", FRQ(off), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gvar_arith(IR_t * pBB) {
    (void)pBB;
    std::string s = bb_binop_gvar_arith_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_binop_gvar_arith: shape mismatch (dispatch chose this arm but predicate failed)")); return; }
    bb_emit_x86(s);
}
