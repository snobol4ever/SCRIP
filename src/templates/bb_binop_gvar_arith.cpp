#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../runtime/builtins/gen.h"
extern int64_t rt_gvar_arith(const char *a, const char *b, int op);
extern int64_t rt_gvar_get_int(const char *name);
void rt_gvar_assign_descr(const char *name, int64_t lo, int64_t hi);
extern DESCR_t POWER_fn(DESCR_t, DESCR_t);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_arith() {
    x86_begin();
    union { int64_t q; uint64_t u; } _ufl; _ufl.q = _.bb_li; uint64_t _powl = _ufl.u;
    union { int64_t q; uint64_t u; } _ufr; _ufr.q = _.bb_ri; uint64_t _powr = _ufr.u;
    /* GVA-3a coercion fix: a direct cell read at [rbx+k*16+8] yields the raw value field, which for a DT_S or DT_R operand is a pointer or double-bits, not an integer; guard on the type tag and fall to rt_gvar_get_int (which coerces string to int via strtoll) when the cell is not DT_I, so the integer hot path stays call-free while string operands like WORD plus 0 coerce correctly */
    auto gva_ld = [&](const char *reg, int k, const char *nm, const char *pl, int lb) -> std::string {
        return x86("mov", "rdx", RDQ("rbx", k * 16))
             + x86("cmp", "edx", (long)DT_I)
             + x86("jne", L(lb))
             + x86("mov", reg, RDQ("rbx", k * 16 + 8))
             + x86("jmp", L(lb + 1))
             + x86("def", L(lb))
             + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) nm, pl)
             + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int)
             + IF(strcmp(reg, "rax") != 0, x86("mov", reg, "rax"))
             + x86("def", L(lb + 1));
    };
    if (PLATFORM_X86) return IF(_.op_off >= 0 && _.op_kind && !strcmp(_.op_kind, "POW") && !_.op_name1 && !_.op_name2 && _.op_sval,
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_ARITH")
                          + IF(_.bb_lk == (int)IR_LIT_F, x86("mov", "rdi", (long)DT_R) + x86("movabs", "rsi", _powl))
                          + IF(_.bb_lk != (int)IR_LIT_F, x86("mov", "rdi", (long)DT_I) + x86("mov", "rsi", (long)_.op_sa))
                          + IF(_.bb_rk == (int)IR_LIT_F, x86("mov", "rdx", (long)DT_R) + x86("movabs", "rcx", _powr))
                          + IF(_.bb_rk != (int)IR_LIT_F, x86("mov", "rdx", (long)DT_I) + x86("mov", "rcx", (long)_.op_sb))
                          + x86("call", "POWER_fn", (uint64_t)(uintptr_t)(void *)POWER_fn)
                          + x86("push", "rdx")
                          + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_sval, _.op_parts_lbl[2])
                          + x86("mov", "rsi", "rax")
                          + x86("pop", "rdx")
                          + x86("call", "rt_gvar_assign_descr", (uint64_t)(uintptr_t)(void *) rt_gvar_assign_descr)
                          + x86("jmp", "γ")
                          + x86("def", "β")
                          + x86("jmp", "ω"))
                          + IF(_.op_off >= 0 && !(_.op_kind && !strcmp(_.op_kind, "POW"))
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && _.op_name1 && _.op_name2,
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_ARITH")
                          + IF(_.op_gva_k1 >= 0 && _.op_gva_k2 >= 0,
                              gva_ld("rcx", _.op_gva_k2, _.op_name2, _.op_parts_lbl[1], 0)
                            + x86("mov", FRQ(_.op_off), "rcx")
                            + gva_ld("rax", _.op_gva_k1, _.op_name1, _.op_parts_lbl[0], 2)
                            + x86("mov", "rcx", FRQ(_.op_off))
                            + IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
                            + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
                            + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
                            + IF(_.op_ival == BINOP_DIV, x86("cqo") + x86("idiv", "rcx"))
                            + IF(_.op_ival == BINOP_MOD, x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx")))
                          + IF(!(_.op_gva_k1 >= 0 && _.op_gva_k2 >= 0),
                              x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
                            + x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
                            + x86("mov", "rdx", (long)_.op_ival)
                            + x86("call", "rt_gvar_arith", (uint64_t)(uintptr_t)(void *) rt_gvar_arith))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86("jmp", "γ")
                          + x86("def", "β")
                          + x86("jmp", "ω"))
                          + IF(_.op_off >= 0 && !(_.op_kind && !strcmp(_.op_kind, "POW"))
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && (_.op_name1 || _.op_name2) && !(_.op_name1 && _.op_name2),
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_ARITH")
                          + IF((_.op_name1 ? _.op_gva_k1 : _.op_gva_k2) >= 0,
                              gva_ld("rax", (_.op_name1 ? _.op_gva_k1 : _.op_gva_k2), (_.op_name1 ? _.op_name1 : _.op_name2), (_.op_name1 ? _.op_parts_lbl[0] : _.op_parts_lbl[1]), 0))
                          + IF((_.op_name1 ? _.op_gva_k1 : _.op_gva_k2) < 0,
                              x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_name1 ? _.op_name1 : _.op_name2), (_.op_name1 ? _.op_parts_lbl[0] : _.op_parts_lbl[1]))
                            + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *) rt_gvar_get_int))
                          + IF( _.op_name1, x86("mov", "rcx", (long)_.op_sb))
                          + IF(!_.op_name1, x86("mov", "rcx", "rax"))
                          + IF(!_.op_name1, x86("mov", "rax", (long)_.op_sa))
                          + IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
                          + IF(_.op_ival == BINOP_DIV, x86("cqo"))
                          + IF(_.op_ival == BINOP_DIV, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("cqo"))
                          + IF(_.op_ival == BINOP_MOD, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("mov", "rax", "rdx"))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86("jmp", "γ")
                          + x86("def", "β")
                          + x86("jmp", "ω"))
                          + IF(_.op_off >= 0 && !(_.op_kind && !strcmp(_.op_kind, "POW"))
                              && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD)
                              && !_.op_name1 && !_.op_name2,
                            x86("label", _.lbl_α)
                          + x86("comment", "IR_BINOP_GVAR_ARITH")
                          + x86("mov", "rax", (long)_.op_sa)
                          + x86("mov", "rcx", (long)_.op_sb)
                          + IF(_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
                          + IF(_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
                          + IF(_.op_ival == BINOP_DIV, x86("cqo"))
                          + IF(_.op_ival == BINOP_DIV, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("cqo"))
                          + IF(_.op_ival == BINOP_MOD, x86("idiv", "rcx"))
                          + IF(_.op_ival == BINOP_MOD, x86("mov", "rax", "rdx"))
                          + x86("mov", FRQ(_.op_off), "rax")
                          + x86("jmp", "γ")
                          + x86("def", "β")
                          + x86("jmp", "ω"))
                          + IF(!(_.op_off >= 0 && !(_.op_kind && !strcmp(_.op_kind, "POW"))
                                && (_.op_ival == BINOP_ADD || _.op_ival == BINOP_SUB || _.op_ival == BINOP_MUL || _.op_ival == BINOP_DIV || _.op_ival == BINOP_MOD))
                              && !(_.op_off >= 0 && _.op_kind && !strcmp(_.op_kind, "POW") && !_.op_name1 && !_.op_name2 && _.op_sval),
                            x86_bomb("bb_binop_gvar_arith: shape mismatch (dispatch chose this arm but predicate failed)"));
    return std::string();
}
