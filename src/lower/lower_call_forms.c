#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "IR.h"
#include "stage2.h"
#include "call_forms.h"
#include "builtin_ids.h"
#include "snobol4_system_fns.h"
#include "bb_pl_leaf_kinds.h"
extern int rt_builtin_is_generator(const char *);
extern int rt_builtin_is_known(const char *);
extern int icn_builtin_is_generator(const char *);
extern int icn_builtin_is_known(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
extern int rt_proc_entry_pending(const char *);
extern int dop_direct_leaf_known(const char *, int);
extern int rt_builtin_tail_may_open(const char *);
extern int rt_pas_bi_index(const char *);
extern int rt_dat_field_of_any(const char *);
extern void * dat_find_type(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_knob_unless_zero(const char * name) { const char * e = getenv(name); return (e && *e == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long bid_bake_of(const char * fn) {
    if (!lower_knob_unless_zero("SCRIP_BID_BAKE") || !fn) return -1L;
    size_t n = strlen(fn);
    if (n > 0xFFFFu) return -1L;
    return (long)
        (((unsigned long)n << 16) | (unsigned long)(unsigned)bid_of(fn, (unsigned)n) | (sn4_is_system_fn(fn) ? (unsigned long)BID_BAKE_SYSFN : 0UL) |
        ((sn4_direct_on() && sn4_is_leaf_fn(fn)) ? (unsigned long)BID_BAKE_LEAF : 0UL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_byname_kind(const char * fn, int strict) {
    long bw = bid_bake_of(fn);
    if (strict != 1 && bw >= 0 && (bw & BID_BAKE_LEAF)) return 1;
    if (strict == 2 && bw >= 0 && !(bw & BID_BAKE_SYSFN) && (bw & BID_BAKE_MASK) == 0 && sn4_direct_on()) return 2;
    if (strict == 2) return 3;
    if (strict) return 4;
    return 5;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_callee_baked_kind(const char * fn, int strict) { if (!fn || !fn[0] || sn4_byname_kind(fn, strict) != 2) return 0; return (rt_dat_field_of_any(fn) && !dat_find_type(fn)) ? 1 : 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_is_scan_builtin(const char * fn) {
    if (!fn) return 0;
    return !strcmp(fn, "tab") || !strcmp(fn, "move") || !strcmp(fn, "upto") || !strcmp(fn, "many") || !strcmp(fn, "any") || !strcmp(fn, "find") || !strcmp(fn, "match") || !strcmp(fn, "pos") ||
        !strcmp(fn, "bal");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_is_curmov(const char * fn) { return fn && (!strcmp(fn, "tab") || !strcmp(fn, "move")); }
int lower_call_node_is_curmov(const IR_t * nd) {
    if (!nd || ir_kind0(nd) != IR_CALL || !lower_call_is_curmov(nd->sval)) return 0;
    { int r = lower_call_route(nd); return (r == CALL_ROUTE_BYNAME && lower_call_road_kind(nd, r) == IR_CALL_BYNAME) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_pas_bi(const char * fn) { return (fn && fn[0] == '_' && fn[1] == '_' && fn[2] == 'p' && fn[3] == 'a' && fn[4] == 's' && fn[5] == '_') ? rt_pas_bi_index(fn) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_tail_try(const char * fn, int strict) { int k = sn4_byname_kind(fn, strict); return k == 3 || (k == 5 && rt_builtin_tail_may_open(fn)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_open(const char * fn, int narg, int strict) {
    if (!fn) return CALL_OPEN_NONE;
    if (narg >= 2 && !strcmp(fn, "meth_call")) return CALL_OPEN_METHOD;
    if (!strcmp(fn, "callsame") || !strcmp(fn, "nextsame") || !strcmp(fn, "callwith") || !strcmp(fn, "nextwith")) return CALL_OPEN_REDISP;
    if (narg >= 1 && !strcmp(fn, "APPLY")) return CALL_OPEN_APPLY;
    if (narg == 1 && strict == 2 && !strcmp(fn, "EVAL")) return CALL_OPEN_EVAL;
    return CALL_OPEN_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_arity(const char * op) {
    static const char * const b2[] = { "add", "sub", "mul", "div", "idiv", "divf", "mod", "rem", "fpow", "pow", "min", "max", "gcd", "xor", "shr", "shl", "band", "bor", 0 };
    static const char * const b1[] = { "neg", "pos", "abs", "sign", "trunc", "intg", "flt", "floor", "ceil", "round", "sqrt", "msb", "bnot", "sin", "cos", "atan", "log", "exp", "fip", "ffp", "asin",
        "acos", "tan", "log10", 0 };
    static const char * const b0[] = { "pi", "e", 0 };
    for (int i = 0; b2[i]; i++) if (!strcmp(op, b2[i])) return 2;
    for (int i = 0; b1[i]; i++) if (!strcmp(op, b1[i])) return 1;
    for (int i = 0; b0[i]; i++) if (!strcmp(op, b0[i])) return 0;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_leaf_kind(const char * fn, int narg, const char ** op) {
    static const char * const cmps[] = { "lt", "gt", "le", "ge", "eq", "ne", 0 };
    static const char * const types[] = { "var", "nonvar", "atom", "number", "integer", "float", "atomic", "compound", "callable", "string", "rational", 0 };
    *op = 0;
    if (!fn || fn[0] != '$') return PLK_NONE;
    if (!strcmp(fn, "$mkc")) return narg >= 1 ? PLK_MKC : PLK_NONE;
    if (!strcmp(fn, "$db_decls")) return narg >= 1 ? PLK_DBDECLS : PLK_NONE;
    if (!strcmp(fn, "$unify")) return narg == 2 ? PLK_UNIFY : PLK_NONE;
    if (!strcmp(fn, "$ax_zguard")) return narg == 2 ? PLK_ZGUARD : PLK_NONE;
    if (!strcmp(fn, "$ax_eguard")) return PLK_NONE;
    if (!strncmp(fn, "$ax_", 4)) { *op = fn + 4; return pl_ax_arity(*op) == narg ? PLK_AX : PLK_NONE; }
    if (!strncmp(fn, "$cmp_", 5) && narg == 2) { for (int i = 0; cmps[i]; i++) if (!strcmp(fn + 5, cmps[i])) { *op = fn + 5; return PLK_CMP; } return PLK_NONE; }
    if (!strcmp(fn, "$is_v")) return narg == 2 ? PLK_IS : PLK_NONE;
    if (!strncmp(fn, "$atop_", 6) && narg == 2) { for (int i = 0; cmps[i]; i++) if (!strcmp(fn + 6, cmps[i])) { *op = fn + 6; return PLK_ATOP; } return PLK_NONE; }
    if (!strcmp(fn, "$pl_anum_guard2")) return narg == 3 ? PLK_ANUM : PLK_NONE;
    if (!strcmp(fn, "$pl_anum_guard3")) return narg == 4 ? PLK_ANUM : PLK_NONE;
    if (!strcmp(fn, "$pl_anum_guard5")) return narg == 6 ? PLK_ANUM : PLK_NONE;
    if (narg == 1) for (int i = 0; types[i]; i++) if (!strcmp(fn + 1, types[i])) { *op = fn + 1; return PLK_TYPE; }
    return PLK_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_leaf_inline_known(const char * fn, int narg) { const char * op = 0; return pl_leaf_kind(fn, narg, &op) != PLK_NONE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const int * pl_anum_rules(const char * name, int nargs) {
    static const int r_atom_length[] = { PLR_TEXT, PLR_UNB_OR_INT0 }, r_text_list[] = { PLR_TEXT, PLR_UNB }, r_num_list[] = { PLR_NUM, PLR_UNB }, r_number_string[] = { PLR_NUM, PLR_ANY };
    static const int r_char_code[] = { PLR_UNB, PLR_INTCODE }, r_atom_concat[] = { PLR_TEXT, PLR_TEXT, PLR_UNB_OR_TEXT }, r_atomic_concat[] = { PLR_TEXT_OR_NUM, PLR_TEXT_OR_NUM, PLR_UNB_OR_TEXT };
    static const int r_sub_atom[] = { PLR_TEXT, PLR_UNB_OR_INT0, PLR_UNB_OR_INT0, PLR_UNB_OR_INT0, PLR_UNB_OR_TEXT }, r_arg[] = { PLR_INT0, PLR_COMP, PLR_ANY }, r_functor[] = { PLR_NONVAR, PLR_ANY,
        PLR_ANY };
    if (!name) return 0;
    if (nargs == 2 && !strcmp(name, "atom_length")) return r_atom_length;
    if (nargs == 2 && (!strcmp(name, "atom_chars") || !strcmp(name, "atom_codes"))) return r_text_list;
    if (nargs == 2 && (!strcmp(name, "number_chars") || !strcmp(name, "number_codes"))) return r_num_list;
    if (nargs == 2 && !strcmp(name, "number_string")) return r_number_string;
    if (nargs == 2 && !strcmp(name, "char_code")) return r_char_code;
    if (nargs == 3 && !strcmp(name, "atom_concat")) return r_atom_concat;
    if (nargs == 3 && !strcmp(name, "atomic_concat")) return r_atomic_concat;
    if (nargs == 5 && !strcmp(name, "sub_atom")) return r_sub_atom;
    if (nargs == 3 && !strcmp(name, "arg")) return r_arg;
    if (nargs == 3 && !strcmp(name, "functor")) return r_functor;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_ax_sub(const char * op) {
    if (!op) return PLAX_OTHER;
    return !strcmp(op, "add") ? PLAX_ADD : !strcmp(op, "sub") ? PLAX_SUB : !strcmp(op, "mul") ? PLAX_MUL : !strcmp(op, "idiv") ? PLAX_IDIV : !strcmp(op, "mod") ? PLAX_MOD : !strcmp(op, "rem") ?
        PLAX_REM : PLAX_OTHER;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_type_sub(const char * kind) { return !kind ? PLTYPE_TAGS : !strcmp(kind, "var") ? PLTYPE_VAR : !strcmp(kind, "nonvar") ? PLTYPE_NONVAR : PLTYPE_TAGS; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_type_tags(const char * kind) {
    if (!kind) return "";
    return !strcmp(kind, "atom") ? "A" : !strcmp(kind, "string") ? "S" : !strcmp(kind, "number") ? "IRB" : !strcmp(kind, "integer") ? "IB" : !strcmp(kind, "float") ? "R" : !strcmp(kind, "atomic") ?
        "SAIRB" : !strcmp(kind, "compound") ? "P" : !strcmp(kind, "callable") ? "AP" : !strcmp(kind, "rational") ? "IB" : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_type_rat(const char * kind) { return kind && (!strcmp(kind, "number") || !strcmp(kind, "atomic") || !strcmp(kind, "rational")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_atop_sub(const char * op) { return !op ? PLATOP_OTHER : !strcmp(op, "eq") ? PLATOP_EQ : !strcmp(op, "ne") ? PLATOP_NE : PLATOP_OTHER; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_cmp_fail_jcc(const char * op) {
    if (!op) return "je";
    return !strcmp(op, "lt") ? "jge" : !strcmp(op, "gt") ? "jle" : !strcmp(op, "le") ? "jg" : !strcmp(op, "ge") ? "jl" : !strcmp(op, "eq") ? "jne" : "je";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_call_route(const IR_t * nd) {
    const char * fn = nd->sval ? nd->sval : "";
    int64_t narg = nd->n_operands;
    double dv = nd->dval;
    IR_e k = ir_kind0(nd);
    if (k == IR_CALL_BUILTIN_GEN) return CALL_ROUTE_BYNAME_GEN;
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_PROC_STAGED) return CALL_ROUTE_PROC_STAGED;
    if (k == IR_CALL && fn[0] && dop_direct_leaf_known(fn, (int)narg)) return CALL_ROUTE_FN;
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (k == IR_CALL_ICON && fn[0] && icn_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_ICON && fn[0] && icn_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (!strcmp(fn, "__rk_bool") && narg >= 1) return CALL_ROUTE_RK_BOOL;
    if (dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn)) return CALL_ROUTE_PROC_STAGED;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0) return CALL_ROUTE_DVAL2_BOMB;
    if (fn[0] && rt_proc_is_registered(fn)) return rt_proc_entry_pending(fn) ? CALL_ROUTE_BYNAME : CALL_ROUTE_PROC_STAGED;
    if (fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (fn[0] && !rt_proc_is_registered(fn)) return CALL_ROUTE_BYNAME;
    return CALL_ROUTE_FATAL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_e lower_call_road_kind(const IR_t * nd, int route) {
    const char * fn = nd->sval ? nd->sval : "";
    int narg = nd->n_operands;
    int strict = nd->strict;
    int bk;
    if (route == CALL_ROUTE_BYNAME_GEN) return IR_CALL_GEN;
    if (route != CALL_ROUTE_FN && route != CALL_ROUTE_BYNAME) return (IR_e)0;
    if (pl_leaf_inline_known(fn, narg)) return IR_CALL_PL_LEAF;
    if (route == CALL_ROUTE_FN && dop_direct_leaf_known(fn, narg)) return IR_CALL_DIRECT;
    if ((route == CALL_ROUTE_BYNAME || route == CALL_ROUTE_FN) && lower_call_pas_bi(fn) >= 0) return IR_CALL_PAS_BI;
    if (route == CALL_ROUTE_FN) {
        int open = lower_call_open(fn, narg, strict);
        if (open == CALL_OPEN_METHOD) return IR_CALL_METHOD;
        if (open == CALL_OPEN_REDISP) return IR_CALL_REDISP;
        if (open == CALL_OPEN_APPLY) return IR_CALL_APPLY;
        if (open == CALL_OPEN_EVAL) return IR_CALL_EVAL;
    }
    bk = (route == CALL_ROUTE_BYNAME && (lower_call_is_scan_builtin(fn) || lower_call_is_curmov(fn))) ? 0 : (fn[0] ? bb_callee_baked_kind(fn, strict) : 0);
    if (bk == 2) return IR_CALL_CALLEE_REC;
    if (bk == 1) return IR_CALL_FIELD_REC;
    return lower_call_tail_try(fn, strict) ? IR_CALL_TRY : IR_CALL_BYNAME;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_call_forms_graph(IR_graph_t * g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || nd->op0 || nd->op == IR_CALL_VALUE || ir_norm_call_kind(nd->op) != IR_CALL) continue;
        { IR_e k = lower_call_road_kind(nd, lower_call_route(nd)); if (k) { nd->op0 = nd->op; nd->op = k; } }
    }
}
void lower_call_forms_all(struct stage2_t * s2) { for (int gi = 0; gi < s2->bbp.count; gi++) lower_call_forms_graph(s2->bbp.table[gi]); }
