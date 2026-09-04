#include <string>
#include <string.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ast.h"
#include "../runtime/builtins/gen.h"
extern DESCR_t rt_call_arr_gen(const char *, DESCR_t *, int, int64_t *);
int  bb_slot_get(IR_t * nd);
int  bb_varslot_peek(const char * name);
int  is_global(const char * name);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
extern "C" {
#include "builtin_ids.h"
}
static int bid_bake_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_BID_BAKE"); v = (e && *e == '0') ? 0 : 1; } return v; }
static long bid_bake_of(const char * fn) { if (!bid_bake_on() || !fn) return -1L; size_t n = strlen(fn); if (n > 0xFFFFu) return -1L; return (long)(((unsigned long)n << 16) | (unsigned long)(unsigned)bid_of(fn, (unsigned)n)); }
DESCR_t rt_pl_throw_raise(DESCR_t *, int); DESCR_t rt_pl_exist_raise(DESCR_t *, int); DESCR_t rt_pl_catch_handle(DESCR_t *, int); DESCR_t rt_pl_dop_unify(DESCR_t *, int); DESCR_t rt_pl_dop_mkc(DESCR_t *, int); DESCR_t dop_write(DESCR_t *, int); DESCR_t dop_nl(DESCR_t *, int); DESCR_t rt_pl_dop_is_v(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_add(DESCR_t *, int); DESCR_t rt_pl_dop_ax_sub(DESCR_t *, int); DESCR_t rt_pl_dop_ax_mul(DESCR_t *, int); DESCR_t rt_pl_dop_ax_div(DESCR_t *, int); DESCR_t rt_pl_dop_ax_idiv(DESCR_t *, int); DESCR_t rt_pl_dop_ax_mod(DESCR_t *, int);
DESCR_t rt_pl_dop_cmp_lt(DESCR_t *, int); DESCR_t rt_pl_dop_cmp_gt(DESCR_t *, int); DESCR_t rt_pl_dop_cmp_le(DESCR_t *, int); DESCR_t rt_pl_dop_cmp_ge(DESCR_t *, int); DESCR_t rt_pl_dop_cmp_eq(DESCR_t *, int); DESCR_t rt_pl_dop_cmp_ne(DESCR_t *, int);
DESCR_t rt_pl_dop_compare(DESCR_t *, int); DESCR_t rt_pl_dop_functor(DESCR_t *, int); DESCR_t rt_pl_dop_arg(DESCR_t *, int); DESCR_t rt_pl_dop_univ(DESCR_t *, int);
DESCR_t rt_pl_dop_copy_term(DESCR_t *, int); DESCR_t rt_pl_dop_numbervars3(DESCR_t *, int); DESCR_t rt_pl_dop_numbervars1(DESCR_t *, int); DESCR_t rt_pl_dop_succ(DESCR_t *, int);
DESCR_t rt_pl_dop_plus(DESCR_t *, int); DESCR_t rt_pl_dop_sort(DESCR_t *, int); DESCR_t rt_pl_dop_msort(DESCR_t *, int); DESCR_t rt_pl_dop_char_type(DESCR_t *, int);
DESCR_t rt_pl_dop_findall_new(DESCR_t *, int); DESCR_t rt_pl_dop_findall_add(DESCR_t *, int); DESCR_t rt_pl_dop_findall_result(DESCR_t *, int); DESCR_t rt_pl_dop_bagof_result(DESCR_t *, int); DESCR_t rt_pl_dop_setof_result(DESCR_t *, int);
DESCR_t rt_pl_dop_sub_atom_n(DESCR_t *, int); DESCR_t rt_pl_dop_sub_atom_at(DESCR_t *, int);
DESCR_t rt_pl_dop_db_assertz(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta(DESCR_t *, int); DESCR_t rt_pl_dop_db_n(DESCR_t *, int); DESCR_t rt_pl_dop_db_at(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase(DESCR_t *, int);
DESCR_t rt_pl_dop_db_abolish(DESCR_t *, int); DESCR_t rt_pl_dop_db_head(DESCR_t *, int); DESCR_t rt_pl_dop_db_body(DESCR_t *, int); DESCR_t rt_pl_dop_db_head_fact(DESCR_t *, int);
DESCR_t rt_pl_dop_db_retractall(DESCR_t *, int);
DESCR_t rt_pl_dop_db_alive(DESCR_t *, int);
DESCR_t rt_pl_dop_term_string(DESCR_t *, int); DESCR_t rt_pl_dop_atom_length(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat(DESCR_t *, int); DESCR_t rt_pl_dop_atom_chars(DESCR_t *, int);
DESCR_t rt_pl_dop_atom_codes(DESCR_t *, int); DESCR_t rt_pl_dop_atom_number(DESCR_t *, int); DESCR_t rt_pl_dop_atom_string(DESCR_t *, int); DESCR_t rt_pl_dop_upcase_atom(DESCR_t *, int);
DESCR_t rt_pl_dop_downcase_atom(DESCR_t *, int); DESCR_t rt_pl_dop_string_concat(DESCR_t *, int); DESCR_t rt_pl_dop_string_length(DESCR_t *, int); DESCR_t rt_pl_dop_string_lower(DESCR_t *, int);
DESCR_t rt_pl_dop_string_upper(DESCR_t *, int); DESCR_t rt_pl_dop_string_to_atom(DESCR_t *, int); DESCR_t rt_pl_dop_number_string(DESCR_t *, int);
DESCR_t rt_pl_dop_atomic_list_concat(DESCR_t *, int); DESCR_t rt_pl_dop_concat_atom(DESCR_t *, int); DESCR_t rt_pl_dop_char_code(DESCR_t *, int); DESCR_t rt_pl_dop_number_codes(DESCR_t *, int);
DESCR_t rt_pl_dop_number_chars(DESCR_t *, int); DESCR_t rt_pl_dop_name(DESCR_t *, int); DESCR_t rt_pl_dop_get_char(DESCR_t *, int); DESCR_t rt_pl_dop_peek_char(DESCR_t *, int);
DESCR_t rt_pl_dop_read(DESCR_t *, int); DESCR_t rt_pl_dop_atom_to_term(DESCR_t *, int); DESCR_t dop_pl_var(DESCR_t *, int); DESCR_t dop_pl_nonvar(DESCR_t *, int); DESCR_t dop_pl_atom(DESCR_t *, int); DESCR_t dop_pl_number(DESCR_t *, int);
DESCR_t dop_pl_integer(DESCR_t *, int); DESCR_t dop_pl_float(DESCR_t *, int); DESCR_t dop_pl_atomic(DESCR_t *, int); DESCR_t dop_pl_compound(DESCR_t *, int);
DESCR_t dop_pl_callable(DESCR_t *, int); DESCR_t dop_pl_ground(DESCR_t *, int); DESCR_t dop_pl_is_list(DESCR_t *, int); DESCR_t dop_pl_acyclic_term(DESCR_t *, int);
DESCR_t dop_pl_atop_lt(DESCR_t *, int); DESCR_t dop_pl_atop_le(DESCR_t *, int); DESCR_t dop_pl_atop_gt(DESCR_t *, int); DESCR_t dop_pl_atop_ge(DESCR_t *, int);
DESCR_t dop_pl_atop_eq(DESCR_t *, int); DESCR_t dop_pl_atop_ne(DESCR_t *, int); DESCR_t dop_pl_writeq(DESCR_t *, int); DESCR_t dop_pl_write_canonical(DESCR_t *, int);
DESCR_t dop_pl_writeln(DESCR_t *, int); DESCR_t dop_pl_tab(DESCR_t *, int); DESCR_t dop_pl_put_char(DESCR_t *, int); DESCR_t dop_pl_halt(DESCR_t *, int);
DESCR_t dop_pl_flush_output(DESCR_t *, int); DESCR_t dop_pl_format(DESCR_t *, int);
DESCR_t dop_pl_write_s(DESCR_t *, int); DESCR_t dop_pl_writeq_s(DESCR_t *, int); DESCR_t dop_pl_write_canonical_s(DESCR_t *, int); DESCR_t dop_pl_writeln_s(DESCR_t *, int); DESCR_t dop_pl_nl_s(DESCR_t *, int);
DESCR_t dop_pl_tab_s(DESCR_t *, int); DESCR_t dop_pl_put_char_s(DESCR_t *, int); DESCR_t dop_pl_flush_output_s(DESCR_t *, int); DESCR_t dop_pl_set_output(DESCR_t *, int); DESCR_t dop_pl_set_input(DESCR_t *, int);
DESCR_t dop_pl_close(DESCR_t *, int); DESCR_t rt_pl_dop_read_s(DESCR_t *, int); DESCR_t rt_pl_dop_get_char_s(DESCR_t *, int); DESCR_t rt_pl_dop_peek_char_s(DESCR_t *, int); DESCR_t rt_pl_dop_current_output(DESCR_t *, int);
DESCR_t rt_pl_dop_current_input(DESCR_t *, int); DESCR_t rt_pl_dop_open(DESCR_t *, int); DESCR_t rt_pl_dop_open4(DESCR_t *, int); DESCR_t rt_pl_dop_keysort(DESCR_t *, int); DESCR_t rt_pl_dop_format3(DESCR_t *, int); DESCR_t dop_pl_op(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_rem(DESCR_t *, int); DESCR_t rt_pl_dop_ax_fpow(DESCR_t *, int); DESCR_t rt_pl_dop_ax_pow(DESCR_t *, int); DESCR_t rt_pl_dop_ax_min(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_max(DESCR_t *, int); DESCR_t rt_pl_dop_ax_gcd(DESCR_t *, int); DESCR_t rt_pl_dop_ax_xor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_shr(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_shl(DESCR_t *, int); DESCR_t rt_pl_dop_ax_band(DESCR_t *, int); DESCR_t rt_pl_dop_ax_bor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_neg(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_pos(DESCR_t *, int); DESCR_t rt_pl_dop_ax_abs(DESCR_t *, int); DESCR_t rt_pl_dop_ax_sign(DESCR_t *, int); DESCR_t rt_pl_dop_ax_trunc(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_intg(DESCR_t *, int); DESCR_t rt_pl_dop_ax_flt(DESCR_t *, int); DESCR_t rt_pl_dop_ax_floor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_ceil(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_round(DESCR_t *, int); DESCR_t rt_pl_dop_ax_sqrt(DESCR_t *, int); DESCR_t rt_pl_dop_ax_msb(DESCR_t *, int); DESCR_t rt_pl_dop_ax_bnot(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_sin(DESCR_t *, int); DESCR_t rt_pl_dop_ax_cos(DESCR_t *, int); DESCR_t rt_pl_dop_ax_atan(DESCR_t *, int); DESCR_t rt_pl_dop_ax_log(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_exp(DESCR_t *, int); DESCR_t rt_pl_dop_ax_fip(DESCR_t *, int); DESCR_t rt_pl_dop_ax_ffp(DESCR_t *, int); DESCR_t rt_pl_dop_ax_pi(DESCR_t *, int);
int64_t rt_gvar_get_int(const char * name);
extern int g_gva_active;
int gva_index_of(const char * name);
DESCR_t NV_GET_fn(const char * name);
int  rt_is_truthy(DESCR_t v);
int  rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int zoff(const IR_t * nd) { return nd ? zls_off(nd) : -1; }
extern std::string bb_call_proc_staged_str(IR_t *);
extern std::string bb_call_write_slot_str(IR_t *);
extern std::string bb_call_write_binop_str(IR_t *);
extern std::string bb_call_write_legacy_str(IR_t *, int);
extern std::string bb_call_fn_str(IR_t *);
extern std::string bb_call_bool_str(IR_t *);
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void arith_operands(IR_graph_t * sg, IR_t * nd, IR_t ** a, IR_t ** b) {
    *a = ir_pair_arg(nd, 0); *b = ir_pair_arg(nd, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_is_arith_binop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && (IR_LIT(nd).ival == BINOP_ADD || IR_LIT(nd).ival == BINOP_SUB || IR_LIT(nd).ival == BINOP_MUL || IR_LIT(nd).ival == BINOP_DIV || IR_LIT(nd).ival == BINOP_MOD);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lits_int_val(IR_t * nd, long long * out) {
    if (!nd || nd->op != IR_LIT_STRING || !IR_LIT(nd).sval) return 0;
    const char * p = IR_LIT(nd).sval; while (*p == ' ' || *p == '\t') p++;
    const char * e = p + strlen(p); while (e > p && (e[-1] == ' ' || e[-1] == '\t')) e--;
    if (p == e) { *out = 0; return 1; }
    const char * q = p; if (q < e && (*q == '+' || *q == '-')) q++;
    if (q == e) return 0;
    for (const char * r = q; r < e; r++) if (*r < '0' || *r > '9') return 0;
    *out = strtoll(p, NULL, 10); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_kind_ok(IR_t * nd) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_STRING) { long long _v; return lits_int_val(nd, &_v); }
    if (nd->op == IR_LIT_INTEGER) return 1;
    if (nd->op == IR_VAR && IR_LIT(nd).sval) return 1;
    if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) return 1;
    if (arith_is_arith_binop(nd)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_is_relop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * relop_fail_mnem(IR_t * nd) {
    return IR_LIT(nd).ival == BINOP_LT ? "jge" : IR_LIT(nd).ival == BINOP_LE ? "jg" : IR_LIT(nd).ival == BINOP_GT ? "jle"
         : IR_LIT(nd).ival == BINOP_GE ? "jl"  : IR_LIT(nd).ival == BINOP_EQ ? "jne" : "je";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_a(IR_graph_t * sg, IR_t * a, int gk_lb = -1) {
    std::string s;
    if (a->op == IR_VAR && IR_LIT(a).sval) {
        char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(a).sval);
        std::string slow = x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(a).sval, b1)
                         + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
        int k = (gk_lb >= 0 && g_gva_active) ? gva_index_of(IR_LIT(a).sval) : -1;
        if (k >= 0)
            s += x86("note", gva_name(k)) + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(k, 0) : ABSQ(RT_GVA_VA + k * 16))
               + x86("cmp", "dl", (long)DT_I)
               + x86("jne", L(gk_lb))
               + x86("note", gva_name(k)) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(k, 8) : ABSQ(RT_GVA_VA + k * 16 + 8))
               + x86("jmp", L(gk_lb + 1))
               + x86("def", L(gk_lb))
               + slow
               + x86("def", L(gk_lb + 1));
        else s += slow;
    } else if (a->op == IR_LIT_INTEGER) {
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(a).ival);
    } else if (a->op == IR_LIT_STRING) {
        long long av = 0; if (!lits_int_val(a, &av)) return x86_bomb("marshal inline-arith: non-numeric string left operand");
        s += x86_movabs_r64("rax", (uint64_t)av);
    } else if (a->op == IR_CALL || ir_is_call_kind(a->op)) {
        int sc = zoff(a);
        if (sc < 0) return x86_bomb("marshal inline-arith: nested call has no LOWER slot grant (TMP-ERADICATE)");
        return x86_bomb("bb_call: call-in-arith/marshal-arg via the PARKED marshal_single_call trampoline (dead at NCB-1b, 0/592 sweep). "
                        "If you are seeing this, LOWER routed a shape here that it did not before: reuse the NCB-1b bcps_det_arm window, "
                        "do NOT resurrect the trampoline. See bb_call.marshal-single-call-parked-e49b25db.cpp");
    } else if (arith_is_arith_binop(a)) {
        s += marshal_arith_rax(sg, a);
    } else return x86_bomb("marshal inline-arith: unhandled left operand shape");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_b(IR_graph_t * sg, IR_t * b, int gk_lb = -1) {
    std::string s;
    if (b->op == IR_VAR && IR_LIT(b).sval) {
        char b2[80]; strtab_label(b2, sizeof b2, IR_LIT(b).sval);
        std::string slow = x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(b).sval, b2)
                         + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
        slow += x86("mov", "rcx", "rax");
        int k = (gk_lb >= 0 && g_gva_active) ? gva_index_of(IR_LIT(b).sval) : -1;
        if (k >= 0)
            s += x86("note", gva_name(k)) + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(k, 0) : ABSQ(RT_GVA_VA + k * 16))
               + x86("cmp", "dl", (long)DT_I)
               + x86("jne", L(gk_lb))
               + x86("note", gva_name(k)) + x86("mov", "rcx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(k, 8) : ABSQ(RT_GVA_VA + k * 16 + 8))
               + x86("jmp", L(gk_lb + 1))
               + x86("def", L(gk_lb))
               + slow
               + x86("def", L(gk_lb + 1));
        else s += slow;
    } else if (b->op == IR_LIT_INTEGER) {
        s += x86("mov", "rcx", (long)IR_LIT(b).ival);
    } else if (b->op == IR_LIT_STRING) {
        long long bv = 0; if (!lits_int_val(b, &bv)) return x86_bomb("marshal inline-arith: non-numeric string right operand");
        s += x86_movabs_r64("rcx", (uint64_t)bv);
    } else if (b->op == IR_CALL || ir_is_call_kind(b->op)) {
        int sc = zoff(b);
        if (sc < 0) return x86_bomb("marshal inline-arith: nested call has no LOWER slot grant (TMP-ERADICATE)");
        return x86_bomb("bb_call: call-in-arith/marshal-arg via the PARKED marshal_single_call trampoline (dead at NCB-1b, 0/592 sweep). "
                        "If you are seeing this, LOWER routed a shape here that it did not before: reuse the NCB-1b bcps_det_arm window, "
                        "do NOT resurrect the trampoline. See bb_call.marshal-single-call-parked-e49b25db.cpp");
    } else if (arith_is_arith_binop(b)) {
        s += marshal_arith_rax(sg, b);
        s += x86("mov", "rcx", "rax");
    } else return x86_bomb("marshal inline-arith: unhandled right operand shape");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd) {
    IR_t * a = NULL, * b = NULL;
    arith_operands(sg, nd, &a, &b);
    if (!a || !b) return x86_bomb("marshal inline-arith: binop operands unresolved");
    int scratch = zoff(nd);
    if (scratch < 0) return x86_bomb("marshal inline-arith: binop has no LOWER slot grant (TMP-ERADICATE)");
    std::string s = arith_opnd_a(sg, a);
    s += x86("mov", FRQ(scratch), "rax");
    s += arith_opnd_b(sg, b);
    s += x86("mov", "rax", FRQ(scratch));
    switch ((int)IR_LIT(nd).ival) {
    case BINOP_ADD: s += x86("add",  "rax", "rcx"); break;
    case BINOP_SUB: s += x86("sub",  "rax", "rcx"); break;
    case BINOP_MUL: s += x86("imul", "rax", "rcx"); break;
    case BINOP_DIV: s += x86("cqo")
                       + x86("idiv", "rcx"); break;
    case BINOP_MOD: s += x86("cqo")
                       + x86("idiv", "rcx")
                       + x86("mov", "rax", "rdx"); break;
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx) {
    if (owner && owner == _.node && idx >= 0 && idx < _.op_arg_slot_n && _.op_arg_slot[idx] >= 0) {
        int ps = _.op_arg_slot[idx];
        std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                          + " = producer-box slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", "rax", FRQ(ps));
        s += x86("mov", FRQ(aoff), "rax");
        s += x86("mov", "rax", FRQ(ps + 8));
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (!lf) return std::string();
    if (lf->op == IR_LIT_INTEGER) {
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = LIT_I -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_I);
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(lf).ival);
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (lf->op == IR_LIT_REAL) {
        uint64_t bits; double d = IR_LIT(lf).dval; memcpy(&bits, &d, 8);
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = LIT_F -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_R);
        s += x86_movabs_r64("rax", bits);
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (lf->op == IR_LIT_STRING) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx)
           + " = LIT_S (string REG-RO sealed in-band) -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_S);
        s += x86("mov", "rax", ROQ(nseal));
        s += x86("mov", FRQ(aoff + 8), "rax");
        s += x86_jmp_id(nskip);
        s += x86_ro_seal_str(nseal, IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        s += x86_deflabel_id(nskip);
        return s;
    }
    if ((lf->op == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0)) || ir_is_call_kind(lf->op)) {
        int staged = (lf->op == IR_CALL_PROC_STAGED || lf->op == IR_PROC_GEN);
        if (owner && owner == _.node && staged && bb_slot_get(lf) >= 0) {
            int ps = bb_slot_get(lf); std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                                      + " = spine call-result slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
            s += x86("mov", "rax", FRQ(ps));
            s += x86("mov", FRQ(aoff), "rax");
            s += x86("mov", "rax", FRQ(ps + 8));
            s += x86("mov", FRQ(aoff + 8), "rax");
            return s;
        } return x86_bomb("bb_call: call-in-arith/marshal-arg via the PARKED marshal_single_call trampoline (dead at NCB-1b, 0/592 sweep). "
                        "If you are seeing this, LOWER routed a shape here that it did not before: reuse the NCB-1b bcps_det_arm window, "
                        "do NOT resurrect the trampoline. See bb_call.marshal-single-call-parked-e49b25db.cpp");
    }
    if (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && is_global(IR_LIT(lf).sval)) {
        char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(lf).sval);
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = global VAR NV_GET -> [zr+" + std::to_string(aoff) + "]");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(lf).sval, b1);
        s += x86("call_rt", "NV_GET_fn", (long)aoff, (uint64_t)(uintptr_t)(void *)NV_GET_fn);
        return s;
    }
    {
        int is_local_var = (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && !is_global(IR_LIT(lf).sval));
        int ps = is_local_var ? -1 : bb_slot_get(lf);
        if (ps < 0 && !is_local_var) ps = zoff(lf);
        if (ps >= 0) {
            std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                              + " = nested producer-box slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
            s += x86("mov", "rax", FRQ(ps));
            s += x86("mov", FRQ(aoff), "rax");
            s += x86("mov", "rax", FRQ(ps + 8));
            s += x86("mov", FRQ(aoff + 8), "rax");
            return s;
        }
    }
    {
        int voff = bb_varslot_peek(IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        if (voff < 0) return x86_bomb("bb_call marshal: IR_VAR arg names a local with no LOWER-granted varslot (TE-4: grant in ir_drive_slot_assign)");
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = varslot [zr+" + std::to_string(voff) + "] -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", "rax", FRQ(voff));
        s += x86("mov", FRQ(aoff), "rax");
        s += x86("mov", "rax", FRQ(voff + 8));
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym) {
    static const struct { const char * nm; int ar; const char * sy; DESCR_t (*fp)(DESCR_t *, int); } t[] = {
        { "$unify", 2, "rt_pl_dop_unify", rt_pl_dop_unify }, { "$mkc", -1, "rt_pl_dop_mkc", rt_pl_dop_mkc },
        { "$throw", 1, "rt_pl_throw_raise", rt_pl_throw_raise }, { "$existence_error", 1, "rt_pl_exist_raise", rt_pl_exist_raise },
        { "$catch_handle", 1, "rt_pl_catch_handle", rt_pl_catch_handle },
        { "$write", 1, "dop_write", dop_write }, { "$nl", 0, "dop_nl", dop_nl }, { "$is_v", 2, "rt_pl_dop_is_v", rt_pl_dop_is_v },
        { "$ax_add", 2, "rt_pl_dop_ax_add", rt_pl_dop_ax_add }, { "$ax_sub", 2, "rt_pl_dop_ax_sub", rt_pl_dop_ax_sub }, { "$ax_mul", 2, "rt_pl_dop_ax_mul", rt_pl_dop_ax_mul },
        { "$ax_div", 2, "rt_pl_dop_ax_div", rt_pl_dop_ax_div }, { "$ax_idiv", 2, "rt_pl_dop_ax_idiv", rt_pl_dop_ax_idiv }, { "$ax_mod", 2, "rt_pl_dop_ax_mod", rt_pl_dop_ax_mod },
        { "$cmp_lt", 2, "rt_pl_dop_cmp_lt", rt_pl_dop_cmp_lt }, { "$cmp_gt", 2, "rt_pl_dop_cmp_gt", rt_pl_dop_cmp_gt }, { "$cmp_le", 2, "rt_pl_dop_cmp_le", rt_pl_dop_cmp_le },
        { "$cmp_ge", 2, "rt_pl_dop_cmp_ge", rt_pl_dop_cmp_ge }, { "$cmp_eq", 2, "rt_pl_dop_cmp_eq", rt_pl_dop_cmp_eq }, { "$cmp_ne", 2, "rt_pl_dop_cmp_ne", rt_pl_dop_cmp_ne },
        { "$compare", 3, "rt_pl_dop_compare", rt_pl_dop_compare }, { "$functor", 3, "rt_pl_dop_functor", rt_pl_dop_functor }, { "$arg", 3, "rt_pl_dop_arg", rt_pl_dop_arg },
        { "$univ", 2, "rt_pl_dop_univ", rt_pl_dop_univ }, { "$copy_term", 2, "rt_pl_dop_copy_term", rt_pl_dop_copy_term }, { "$numbervars3", 3, "rt_pl_dop_numbervars3", rt_pl_dop_numbervars3 },
        { "$numbervars1", 1, "rt_pl_dop_numbervars1", rt_pl_dop_numbervars1 }, { "$succ", 2, "rt_pl_dop_succ", rt_pl_dop_succ }, { "$plus", 3, "rt_pl_dop_plus", rt_pl_dop_plus },
        { "$sort", 2, "rt_pl_dop_sort", rt_pl_dop_sort }, { "$msort", 2, "rt_pl_dop_msort", rt_pl_dop_msort }, { "$char_type", 2, "rt_pl_dop_char_type", rt_pl_dop_char_type },
        { "$findall_new", 0, "rt_pl_dop_findall_new", rt_pl_dop_findall_new }, { "$findall_add", 2, "rt_pl_dop_findall_add", rt_pl_dop_findall_add },
        { "$findall_result", 2, "rt_pl_dop_findall_result", rt_pl_dop_findall_result }, { "$bagof_result", 2, "rt_pl_dop_bagof_result", rt_pl_dop_bagof_result },
        { "$setof_result", 2, "rt_pl_dop_setof_result", rt_pl_dop_setof_result },
        { "$sub_atom_n", 1, "rt_pl_dop_sub_atom_n", rt_pl_dop_sub_atom_n }, { "$sub_atom_at", 6, "rt_pl_dop_sub_atom_at", rt_pl_dop_sub_atom_at },
        { "$db_assertz", 2, "rt_pl_dop_db_assertz", rt_pl_dop_db_assertz }, { "$db_asserta", 2, "rt_pl_dop_db_asserta", rt_pl_dop_db_asserta }, { "$db_n", 1, "rt_pl_dop_db_n", rt_pl_dop_db_n },
        { "$db_at", 2, "rt_pl_dop_db_at", rt_pl_dop_db_at }, { "$db_erase", 2, "rt_pl_dop_db_erase", rt_pl_dop_db_erase }, { "$db_abolish", 1, "rt_pl_dop_db_abolish", rt_pl_dop_db_abolish },
        { "$db_head", 1, "rt_pl_dop_db_head", rt_pl_dop_db_head }, { "$db_body", 1, "rt_pl_dop_db_body", rt_pl_dop_db_body }, { "$db_head_fact", 1, "rt_pl_dop_db_head_fact", rt_pl_dop_db_head_fact },
        { "$db_retractall", 2, "rt_pl_dop_db_retractall", rt_pl_dop_db_retractall },
        { "$db_alive", 2, "rt_pl_dop_db_alive", rt_pl_dop_db_alive },
        { "$term_string", 2, "rt_pl_dop_term_string", rt_pl_dop_term_string }, { "$atom_length", 2, "rt_pl_dop_atom_length", rt_pl_dop_atom_length },
        { "$atom_concat", 3, "rt_pl_dop_atom_concat", rt_pl_dop_atom_concat }, { "$atom_chars", 2, "rt_pl_dop_atom_chars", rt_pl_dop_atom_chars },
        { "$atom_codes", 2, "rt_pl_dop_atom_codes", rt_pl_dop_atom_codes }, { "$atom_number", 2, "rt_pl_dop_atom_number", rt_pl_dop_atom_number },
        { "$atom_string", 2, "rt_pl_dop_atom_string", rt_pl_dop_atom_string }, { "$upcase_atom", 2, "rt_pl_dop_upcase_atom", rt_pl_dop_upcase_atom },
        { "$downcase_atom", 2, "rt_pl_dop_downcase_atom", rt_pl_dop_downcase_atom }, { "$string_concat", 3, "rt_pl_dop_string_concat", rt_pl_dop_string_concat },
        { "$string_length", 2, "rt_pl_dop_string_length", rt_pl_dop_string_length }, { "$string_lower", 2, "rt_pl_dop_string_lower", rt_pl_dop_string_lower },
        { "$string_upper", 2, "rt_pl_dop_string_upper", rt_pl_dop_string_upper }, { "$string_to_atom", 2, "rt_pl_dop_string_to_atom", rt_pl_dop_string_to_atom },
        { "$number_string", 2, "rt_pl_dop_number_string", rt_pl_dop_number_string }, { "$atomic_list_concat", 2, "rt_pl_dop_atomic_list_concat", rt_pl_dop_atomic_list_concat },
        { "$atomic_list_concat", 3, "rt_pl_dop_atomic_list_concat", rt_pl_dop_atomic_list_concat }, { "$concat_atom", 2, "rt_pl_dop_concat_atom", rt_pl_dop_concat_atom },
        { "$concat_atom", 3, "rt_pl_dop_concat_atom", rt_pl_dop_concat_atom }, { "$char_code", 2, "rt_pl_dop_char_code", rt_pl_dop_char_code },
        { "$number_codes", 2, "rt_pl_dop_number_codes", rt_pl_dop_number_codes }, { "$number_chars", 2, "rt_pl_dop_number_chars", rt_pl_dop_number_chars },
        { "$name", 2, "rt_pl_dop_name", rt_pl_dop_name }, { "$get_char", 1, "rt_pl_dop_get_char", rt_pl_dop_get_char }, { "$peek_char", 1, "rt_pl_dop_peek_char", rt_pl_dop_peek_char },
        { "$read", 1, "rt_pl_dop_read", rt_pl_dop_read }, { "$atom_to_term", 3, "rt_pl_dop_atom_to_term", rt_pl_dop_atom_to_term }, { "$var", 1, "dop_pl_var", dop_pl_var }, { "$nonvar", 1, "dop_pl_nonvar", dop_pl_nonvar }, { "$atom", 1, "dop_pl_atom", dop_pl_atom },
        { "$number", 1, "dop_pl_number", dop_pl_number }, { "$integer", 1, "dop_pl_integer", dop_pl_integer }, { "$float", 1, "dop_pl_float", dop_pl_float },
        { "$atomic", 1, "dop_pl_atomic", dop_pl_atomic }, { "$compound", 1, "dop_pl_compound", dop_pl_compound }, { "$callable", 1, "dop_pl_callable", dop_pl_callable },
        { "$ground", 1, "dop_pl_ground", dop_pl_ground }, { "$is_list", 1, "dop_pl_is_list", dop_pl_is_list }, { "$acyclic_term", 1, "dop_pl_acyclic_term", dop_pl_acyclic_term },
        { "$atop_lt", 2, "dop_pl_atop_lt", dop_pl_atop_lt }, { "$atop_le", 2, "dop_pl_atop_le", dop_pl_atop_le }, { "$atop_gt", 2, "dop_pl_atop_gt", dop_pl_atop_gt },
        { "$atop_ge", 2, "dop_pl_atop_ge", dop_pl_atop_ge }, { "$atop_eq", 2, "dop_pl_atop_eq", dop_pl_atop_eq }, { "$atop_ne", 2, "dop_pl_atop_ne", dop_pl_atop_ne },
        { "$writeq", 1, "dop_pl_writeq", dop_pl_writeq }, { "$write_canonical", 1, "dop_pl_write_canonical", dop_pl_write_canonical }, { "$writeln", 1, "dop_pl_writeln", dop_pl_writeln },
        { "$tab", 1, "dop_pl_tab", dop_pl_tab }, { "$put_char", 1, "dop_pl_put_char", dop_pl_put_char }, { "$halt", 0, "dop_pl_halt", dop_pl_halt }, { "$halt", 1, "dop_pl_halt", dop_pl_halt },
        { "$flush_output", 0, "dop_pl_flush_output", dop_pl_flush_output }, { "$format", 1, "dop_pl_format", dop_pl_format }, { "$format", 2, "dop_pl_format", dop_pl_format },
        { "$write_s", 2, "dop_pl_write_s", dop_pl_write_s }, { "$writeq_s", 2, "dop_pl_writeq_s", dop_pl_writeq_s }, { "$write_canonical_s", 2, "dop_pl_write_canonical_s", dop_pl_write_canonical_s },
        { "$writeln_s", 2, "dop_pl_writeln_s", dop_pl_writeln_s }, { "$nl_s", 1, "dop_pl_nl_s", dop_pl_nl_s }, { "$tab_s", 2, "dop_pl_tab_s", dop_pl_tab_s }, { "$put_char_s", 2, "dop_pl_put_char_s", dop_pl_put_char_s },
        { "$flush_output_s", 1, "dop_pl_flush_output_s", dop_pl_flush_output_s }, { "$set_output", 1, "dop_pl_set_output", dop_pl_set_output }, { "$set_input", 1, "dop_pl_set_input", dop_pl_set_input },
        { "$close", 1, "dop_pl_close", dop_pl_close }, { "$close", 2, "dop_pl_close", dop_pl_close }, { "$read_s", 2, "rt_pl_dop_read_s", rt_pl_dop_read_s }, { "$get_char_s", 2, "rt_pl_dop_get_char_s", rt_pl_dop_get_char_s },
        { "$peek_char_s", 2, "rt_pl_dop_peek_char_s", rt_pl_dop_peek_char_s }, { "$current_output", 1, "rt_pl_dop_current_output", rt_pl_dop_current_output }, { "$current_input", 1, "rt_pl_dop_current_input", rt_pl_dop_current_input },
        { "$open", 3, "rt_pl_dop_open", rt_pl_dop_open }, { "$open4", 4, "rt_pl_dop_open4", rt_pl_dop_open4 }, { "$keysort", 2, "rt_pl_dop_keysort", rt_pl_dop_keysort }, { "$format3", 3, "rt_pl_dop_format3", rt_pl_dop_format3 }, { "$op", 3, "dop_pl_op", dop_pl_op },
        { "$ax_rem", 2, "rt_pl_dop_ax_rem", rt_pl_dop_ax_rem }, { "$ax_fpow", 2, "rt_pl_dop_ax_fpow", rt_pl_dop_ax_fpow }, { "$ax_pow", 2, "rt_pl_dop_ax_pow", rt_pl_dop_ax_pow },
        { "$ax_min", 2, "rt_pl_dop_ax_min", rt_pl_dop_ax_min }, { "$ax_max", 2, "rt_pl_dop_ax_max", rt_pl_dop_ax_max }, { "$ax_gcd", 2, "rt_pl_dop_ax_gcd", rt_pl_dop_ax_gcd },
        { "$ax_xor", 2, "rt_pl_dop_ax_xor", rt_pl_dop_ax_xor }, { "$ax_shr", 2, "rt_pl_dop_ax_shr", rt_pl_dop_ax_shr }, { "$ax_shl", 2, "rt_pl_dop_ax_shl", rt_pl_dop_ax_shl },
        { "$ax_band", 2, "rt_pl_dop_ax_band", rt_pl_dop_ax_band }, { "$ax_bor", 2, "rt_pl_dop_ax_bor", rt_pl_dop_ax_bor }, { "$ax_neg", 1, "rt_pl_dop_ax_neg", rt_pl_dop_ax_neg },
        { "$ax_pos", 1, "rt_pl_dop_ax_pos", rt_pl_dop_ax_pos }, { "$ax_abs", 1, "rt_pl_dop_ax_abs", rt_pl_dop_ax_abs }, { "$ax_sign", 1, "rt_pl_dop_ax_sign", rt_pl_dop_ax_sign },
        { "$ax_trunc", 1, "rt_pl_dop_ax_trunc", rt_pl_dop_ax_trunc }, { "$ax_intg", 1, "rt_pl_dop_ax_intg", rt_pl_dop_ax_intg }, { "$ax_flt", 1, "rt_pl_dop_ax_flt", rt_pl_dop_ax_flt },
        { "$ax_floor", 1, "rt_pl_dop_ax_floor", rt_pl_dop_ax_floor }, { "$ax_ceil", 1, "rt_pl_dop_ax_ceil", rt_pl_dop_ax_ceil }, { "$ax_round", 1, "rt_pl_dop_ax_round", rt_pl_dop_ax_round },
        { "$ax_sqrt", 1, "rt_pl_dop_ax_sqrt", rt_pl_dop_ax_sqrt }, { "$ax_msb", 1, "rt_pl_dop_ax_msb", rt_pl_dop_ax_msb }, { "$ax_bnot", 1, "rt_pl_dop_ax_bnot", rt_pl_dop_ax_bnot },
        { "$ax_sin", 1, "rt_pl_dop_ax_sin", rt_pl_dop_ax_sin }, { "$ax_cos", 1, "rt_pl_dop_ax_cos", rt_pl_dop_ax_cos }, { "$ax_atan", 1, "rt_pl_dop_ax_atan", rt_pl_dop_ax_atan },
        { "$ax_log", 1, "rt_pl_dop_ax_log", rt_pl_dop_ax_log }, { "$ax_exp", 1, "rt_pl_dop_ax_exp", rt_pl_dop_ax_exp }, { "$ax_fip", 1, "rt_pl_dop_ax_fip", rt_pl_dop_ax_fip },
        { "$ax_ffp", 1, "rt_pl_dop_ax_ffp", rt_pl_dop_ax_ffp }, { "$ax_pi", 0, "rt_pl_dop_ax_pi", rt_pl_dop_ax_pi },
        { 0, 0, 0, 0 } };
    for (int i = 0; t[i].nm; i++) if (!strcmp(fn, t[i].nm) && (t[i].ar < 0 ? narg >= 1 : narg == t[i].ar)) {
        const char * nd = getenv("SCRIP_NO_DOP"); if (nd && nd[0] == '1') return (void *)0;
        const char * sk = getenv("SCRIP_DOP_SKIP"); if (sk && sk[0] && strstr(sk, t[i].nm + 1)) return (void *)0;
        *sym = t[i].sy; return (void *)(uintptr_t)t[i].fp;
    }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_str(IR_t * pBB) {
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    if (_.op_zres) {
        uint64_t fptr_bl; { DESCR_t (*fp)(const char *, DESCR_t *, int, int) = rt_call_arr_bl; fptr_bl = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX CALL ZD-7 byname ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (narg > 0) {
            s += x86("sub", "rsp", (long)(narg * 16));
            for (int i = 0; i < (int)narg; i++) {
                s += x86("mov", "r8", ZOPQ(i, (int)narg * 16 + 0));
                s += x86("mov", x86_zref(i * 16 + 0, 1), "r8");
                s += x86("mov", "r8", ZOPQ(i, (int)narg * 16 + 8));
                s += x86("mov", x86_zref(i * 16 + 8, 1), "r8");
            }
        }
        {
            std::string fl = std::string(".L") + x86_boxkind() + "_bynamefnzd" + std::to_string((long long)_.nid);
            bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
            if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix"); }
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        if (narg > 0) s += x86_reg_disp32_lea64("rsi", "rsp", 0);
        else          s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)narg);
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call", "rt_call_arr_bl", fptr_bl);
        if (narg > 0) s += x86("add", "rsp", (long)(narg * 16));
        s += x86("cmp", "al", (long)DT_FAIL);
        s += x86_omega("je");
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    int resoff  = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    std::string fl = std::string(".L") + x86_boxkind() + "_bynamefn" + std::to_string((long long)_.nid);
    uint64_t fptr_bl; { DESCR_t (*fp)(const char *, DESCR_t *, int, int) = rt_call_arr_bl; fptr_bl = (uint64_t)(uintptr_t)(void*)fp; }
    std::string s = x86_alpha()
        + x86("comment", std::string("BOX CALL ") + fn + "(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]");
    for (int i = (int)narg - 1; i >= 0; i--)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    bool scansync = x86_is_scan_builtin_name(fn);
    bool curmov = fn && (!strcmp(fn, "tab") || !strcmp(fn, "move"));
    int  dsave  = argbase + 16 * (int)narg;
    if (curmov) s += x86("mov", FRQ(dsave), "r14");
    if (scansync) s += x86_scan_sync_out_force();
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, narg, &dsym);
    if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)narg);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        { bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
          if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text")
           + x86("directive", ".intel_syntax noprefix"); } }
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)narg);
        s += x86("rtcc_wb");
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call_bare", "rt_call_arr_bl", fptr_bl);
        s += x86("rtcc_rl");
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    if (scansync) s += x86_scan_sync_in_rr_force();
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    if (curmov) s += x86("mov", "r14", FRQ(dsave));
    s += x86_omega();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_gen_str(IR_t * pBB) {
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname_gen: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname_gen: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    int genoff  = resoff + 16 * (1 + (int)narg);
    std::string fl = std::string(".L") + x86_boxkind() + "_bynamegenfn" + std::to_string((long long)_.nid);
    uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int, int64_t *) = rt_call_arr_gen; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    std::string s = x86_alpha()
        + x86("comment", std::string("BOX CALL_GEN ") + fn + "(...) -> rt_call_arr_gen by-name [four-port generator; alpha zeroes resume cell, beta re-pumps invoke with persisted cell]");
    for (int i = (int)narg - 1; i >= 0; i--)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    s += x86("mov", FRQ(genoff), (long)0);
    bool scansync = x86_is_scan_builtin_name(fn);
    if (scansync) s += x86_scan_sync_out_force();
    s += x86("def", L(60));
    { bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
      if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
    s += x86("directive", ".section .rodata")
       + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
       + x86("directive", ".section .text")
       + x86("directive", ".intel_syntax noprefix"); } }
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
    s += x86("lea", "rsi", FRQ(argbase));
    s += x86("mov32", "edx", (long)narg);
    s += x86("lea", "rcx", FRQ(genoff));
    s += x86("rtcc_wb");
    s += x86("call_bare", "rt_call_arr_gen", fptr);
    s += x86("rtcc_rl");
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86("jmp", L(60));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rkbool_cond_relop(IR_graph_t * cond) {
    if (!cond) return NULL;
    IR_t * p = cond->entry; int g = 0;
    while (p && g++ < 256) { if (arith_is_relop(p)) return p; if (!p->γ.node) break; p = p->γ.node; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_jct_call(IR_t * r) {
    return r && (r->op == IR_CALL || ir_is_call_kind(r->op)) && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_truthy_cond_str(IR_t * pBB) {
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * e = cond ? cond->entry : NULL;
    if (!e) return x86_alpha() + x86_bomb("bb_call_bool_truthy: empty cond sub-graph");
    std::string s = x86_alpha()
                  + x86("comment", "BOX __rk_bool [dval=2 truthy condition -> rt_is_truthy -> branch true=γ / false=ω]");
    if (e->op == IR_LIT_INTEGER) {
        s += x86("mov32", "edi", (long)DT_I) + x86_movabs_r64("rsi", (uint64_t)IR_LIT(e).ival);
    } else if (e->op == IR_LIT_STRING) {
        s += x86("mov32", "edi", (long)DT_S)
           + x86("mov", "rsi", ROQ(0))
           + x86_jmp_id(1)
           + x86_ro_seal_str(0, IR_LIT(e).sval ? IR_LIT(e).sval : "")
           + x86_deflabel_id(1);
    } else if (e->op == IR_VAR && IR_LIT(e).sval) {
        int voff = bb_varslot_peek(IR_LIT(e).sval);
        if (voff < 0) return x86_alpha() + x86_bomb("bb_call_bool_truthy: IR_VAR cond names a local with no LOWER-granted varslot (TE-4: grant in ir_drive_slot_assign)");
        s += x86("mov", "rdi", FRQ(voff));
        s += x86("mov", "rsi", FRQ(voff + 8));
    } else {
        return x86_alpha() + x86_bomb("bb_call_bool_truthy: unhandled cond entry kind");
    }
    return s + x86("call", "rt_is_truthy", (uint64_t)(uintptr_t)(void *)rt_is_truthy)
             + x86("test", "eax", "eax") + x86_omega("je") + x86_gamma() + x86_beta() + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_jct_cond_str(IR_t * pBB) {
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * relnd = rkbool_cond_relop(cond);
    if (!relnd) return x86_alpha() + x86_bomb("bb_call_bool_jct: no relop in cond sub-graph");
    IR_t * ra = NULL, * rb = NULL; arith_operands(cond, relnd, &ra, &rb);
    if (!ra || !rb) return x86_alpha() + x86_bomb("bb_call_bool_jct: relop operands unresolved");
    int lhs_slot = (zoff(_.node) >= 0) ? zoff(_.node) + 16 : -1;
    if (lhs_slot < 0) return x86_alpha() + x86_bomb("bb_call_bool_jct: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node->n_operands < 2) return x86_alpha() + x86_bomb("bb_call_bool_jct: grant narrower than 2 slots (TMP-ERADICATE)");
    int rhs_slot = lhs_slot + 16;
    std::string s = x86_alpha()
                  + x86("comment", "BOX __rk_bool [dval=2 junction relop -> rt_jct_relop -> branch true=γ / false=ω]");
    s += marshal_call_arg(ra, cond, lhs_slot, NULL, 0);
    s += marshal_call_arg(rb, cond, rhs_slot, NULL, 1);
    s += x86("mov", "rdi", FRQ(lhs_slot));
    s += x86("mov", "rsi", FRQ(lhs_slot + 8));
    s += x86("mov", "rdx", FRQ(rhs_slot));
    s += x86("mov", "rcx", FRQ(rhs_slot + 8));
    s += x86("mov32", "r8d", (long)IR_LIT(relnd).ival);
    return s + x86("call", "rt_jct_relop", (uint64_t)(uintptr_t)(void *)rt_jct_relop)
             + x86("test", "eax", "eax") + x86_omega("je") + x86_gamma() + x86_beta() + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_cond_str(IR_t * pBB) {
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * relnd = rkbool_cond_relop(cond);
    if (!relnd) return bb_call_bool_truthy_cond_str(pBB);
    IR_t * ra = NULL, * rb = NULL; arith_operands(cond, relnd, &ra, &rb);
    if (!ra || !rb) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop operands unresolved");
    if (is_jct_call(ra) || is_jct_call(rb)) return bb_call_bool_jct_cond_str(pBB);
    if (!arith_kind_ok(ra) || !arith_kind_ok(rb)) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop operands unhandled");
    int scratch = zoff(relnd);
    if (scratch < 0) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop has no LOWER slot grant (TMP-ERADICATE)");
    return x86_alpha()
         + x86("comment", "BOX __rk_bool [dval=2 relop condition -> branch true=γ / false=ω]")
         + arith_opnd_a(cond, ra) + x86("mov", FRQ(scratch), "rax")
         + arith_opnd_b(cond, rb) + x86("mov", "rax", FRQ(scratch))
         + x86("cmp", "rax", "rcx")
         + x86_omega(relop_fail_mnem(relnd))
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call(IR_t * pBB) {
    switch (_.op_call_route) {
        case CALL_ROUTE_BYNAME:        return bb_call_byname_str(pBB);
        case CALL_ROUTE_BYNAME_GEN:    return bb_call_byname_gen_str(pBB);
        case CALL_ROUTE_RK_BOOL_COND:  return bb_call_bool_cond_str(pBB);
        case CALL_ROUTE_DVAL2_BOMB:    return x86_alpha() + x86_bomb("CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule");
        case CALL_ROUTE_PROC_STAGED:   return bb_call_proc_staged_str(pBB);
        case CALL_ROUTE_RK_BOOL_SLOT:  return bb_call_bool_str(pBB);
        case CALL_ROUTE_WRITE_SLOT:    return bb_call_write_slot_str(pBB);
        case CALL_ROUTE_WRITE_BINOP:   return bb_call_write_binop_str(pBB);
        case CALL_ROUTE_WRITE_LEGACY:  return bb_call_write_legacy_str(pBB, 1);
        case CALL_ROUTE_WRITE_EMPTY:   return std::string();
        case CALL_ROUTE_FN:            return bb_call_fn_str(pBB);
        default: break;
    }
    fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s'\n", _.op_sval ? _.op_sval : "");
    abort();
    return std::string();
}
