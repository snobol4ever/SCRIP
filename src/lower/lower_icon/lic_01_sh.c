#include "lower/lower_icon/lic_head.h"
#include <string.h>
#include "ir/cset_block.h"
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "runtime/builtins/gen_runtime.h"
#include "lower/lower_common.h"
#include "lower/lc_vec.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_02_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
#include "lower/lower_icon/lic_05_sh.h"
#include "lower/lower_icon/lic_06_sh.h"
extern int icn_builtin_is_known(const char *);
extern int icn_builtin_arity(const char *);
extern int icn_builtin_is_generator(const char *);
static IR_t * lower_alt_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lval);
static IR_t * lower_alt_lv(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res);
static int icn_trace_var_form(const icx_t * cx, const tree_t * t);
static int icn_return_var_form(icx_t * cx, const tree_t * t);
icn_loop_fr * icn_loop_at(const icx_t * cx, int idx) {
    icn_loop_fr * f = cx->loop_top;
    if (idx < 0 || idx >= cx->loop_sp) return NULL;
    for (int d = cx->loop_sp - 1; d > idx; d--) f = f->up;
    return f;
}
IR_t * icn_scan_at(const icx_t * cx, int k) { icn_scan_fr * f = cx->scan_top; for (int d = cx->scan_sp - 1; d > k; d--) f = f->up; return f->enter; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_is_local(const icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->nln; i++) if (cx->ln[i] && !strcmp(cx->ln[i], nm)) return 1; return 0; }
static int icn_kw_assignable(const char * kw) {
    const char * id = (kw && kw[0] == '&') ? kw + 1 : kw;
    return id && (!strcmp(id, "subject") || !strcmp(id, "pos") || !strcmp(id, "random") || !strcmp(id, "trace") || !strcmp(id, "error") || !strcmp(id, "dump"));
}
const char * icn_variable_lit(const icx_t * cx, const tree_t * t) {
    if (!t || t->t != TT_FNC || t->n != 2 || !t->c[0] || !t->c[1]) return NULL;
    const tree_t * f = t->c[0];
    if (f->t != TT_VAR || !f->v.sval || strcmp(f->v.sval, "variable") || icn_is_local(cx, "variable")) return NULL;
    { extern void rt_icn_global_note(const char *); rt_icn_global_note(f->v.sval); }
    const tree_t * g = t->c[1];
    return (g->t == TT_QLIT && g->v.sval && g->v.sval[0]) ? g->v.sval : NULL;
}
int icn_is_own_global(const icx_t * cx, const char * nm) { for (int i = 0; i < cx->ngn; i++) if (cx->gn[i] && !strcmp(cx->gn[i], nm)) return 1; return 0; }
int icn_operand_derefs_late(const icx_t * cx, const tree_t * t) { if (!t || t->t != TT_VAR || !t->v.sval || t->v.sval[0] == '&') return 0; return !icn_is_local(cx, t->v.sval); }
int icn_tree_is_kw_var(const tree_t * t) { return t && (t->t == TT_KEYWORD || t->t == TT_VAR) && t->v.sval && t->v.sval[0] == '&' && icn_kw_assignable(t->v.sval); }
int icn_tree_is_kw_scan(const tree_t * t) { return t && (t->t == TT_KEYWORD || t->t == TT_VAR) && t->v.sval && (!strcmp(t->v.sval, "&pos") || !strcmp(t->v.sval, "&subject")); }
int icn_scan_window_name(const char * nm) { return nm && (!strcmp(nm, "upto") || !strcmp(nm, "any") || !strcmp(nm, "many") || !strcmp(nm, "find") || !strcmp(nm, "match")); }
int icn_is_proc(const char * nm) { for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) return 1; return 0; }
int icn_call_yields_a_variable(icx_t * cx, const char * nm) { (void) cx; return nm && icn_is_proc(nm); }
static int icn_is_static_mangled_name(const char * nm) {
    if (!nm || !strstr(nm, "__STATIC__")) return 0;
    for (int i = 0; i < g_lower.icn.synth_excl.n; i++) { const char * m = LC_AT(&g_lower.icn.synth_excl, const char *, i); if (m && !strcmp(m, nm)) return 1; }
    return 0;
}
static int icn_ret_global_name(const icx_t * cx, const char * nm) { return nm && nm[0] != '&' && !icn_is_local(cx, nm) && (icn_is_own_global(cx, nm) || icn_is_static_mangled_name(nm)); }
static int icn_is_implicit_local(const icx_t * cx, const char * nm) {
    lc_vec iv;
    lc_vec_init(&iv, (int) sizeof(const char *));
    iv.n = 0;
    icn_collect_implicit_locals(cx->body, cx->ln, cx->nln, &iv, cx->prog && cx->prog->v.ival);
    for (int i = 0; i < iv.n; i++) if (!strcmp(LC_AT(&iv, const char *, i), nm)) return 1;
    return 0;
}
tree_t * icn_variable_lit_target(const icx_t * cx, const char * vn) {
    if (vn[0] == '&') return icn_kw_assignable(vn) ? icn_variable_lit_tree(vn) : NULL;
    if (icn_is_local(cx, vn)) return icn_variable_lit_tree(vn);
    { const char * m = icn_static_mangled(cx, vn); if (m) return icn_variable_lit_tree(m); }
    if (icn_is_own_global(cx, vn) || icn_is_proc(vn) || icn_builtin_reassigned(vn)) return icn_variable_lit_tree(vn);
    if (icn_is_implicit_local(cx, vn)) return icn_variable_lit_tree(vn);
    return NULL;
}
tree_t * icn_variable_case(icx_t * cx, const tree_t * t) {
    if (!t || t->t != TT_FNC || t->n != 2 || !t->c[0] || t->c[0]->t != TT_VAR || !t->c[0]->v.sval || strcmp(t->c[0]->v.sval, "variable")) return NULL;
    if (icn_is_local(cx, "variable") || !t->c[1] || t->c[1]->t == TT_QLIT || t->c[1] == cx->varcase_arg) return NULL;
    lc_vec iv;
    lc_vec_init(&iv, (int) sizeof(const char *));
    icn_collect_implicit_locals(cx->body, cx->ln, cx->nln, &iv, cx->prog && cx->prog->v.ival);
    if (cx->nln + iv.n <= 0) return NULL;
    tree_t * e = ast_node_new(TT_CASE);
    ast_push(e, (tree_t *) t->c[1]);
    for (int i = 0; i < cx->nln; i++) {
        if (!cx->ln[i] || cx->ln[i][0] == '$') continue;
        tree_t * k = ast_node_new(TT_QLIT);
        k->v.sval = (char *) cx->ln[i];
        k->slen = (int) strlen(cx->ln[i]);
        ast_push(e, k);
        tree_t * v = ast_node_new(TT_VAR);
        v->v.sval = (char *) cx->ln[i];
        ast_push(e, v);
    }
    for (int i = 0; i < iv.n; i++) {
        const char * nm = LC_AT(&iv, const char *, i);
        if (!nm || nm[0] == '$') continue;
        tree_t * k = ast_node_new(TT_QLIT);
        k->v.sval = (char *) nm;
        k->slen = (int) strlen(nm);
        ast_push(e, k);
        tree_t * v = ast_node_new(TT_VAR);
        v->v.sval = (char *) nm;
        ast_push(e, v);
    }
    ast_push(e, ast_node_new(TT_NUL));
    ast_push(e, (tree_t *) t);
    cx->varcase_arg = t->c[1];
    return e;
}
int icn_name_assigned(const char * nm) { if (!nm) return 0; for (int k = 0; k < g_lower.icn.reassigned.n; k++) if (!strcmp(LC_AT(&g_lower.icn.reassigned, const char *, k), nm)) return 1; return 0; }
int icn_proc_reassigned(const char * nm) { return nm && icn_is_proc(nm) && icn_name_assigned(nm); }
static int icn_record_reassigned(const char * nm) { extern void * dat_find_type(const char *); return nm && !icn_is_proc(nm) && dat_find_type(nm) && icn_name_assigned(nm); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_proc_is_generator(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_call_allow_gen(const char * name) { return name && (icn_proc_is_generator(name) || !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "key") || !strcmp(name, "seq")); }
static int icn_is_unicon_thread_name(const char * nm) {
    return nm && (!strncmp(nm, "ICN$", 4) || !strcmp(nm, "wait") || !strcmp(nm, "lock") || !strcmp(nm, "unlock") || !strcmp(nm, "trylock") || !strcmp(nm, "mutex") || !strcmp(nm, "spawn"));
}
char * icn_unicon_row(const char * nm) {
    extern int icn_builtin_is_leaf_row(const char *);
    size_t n = strlen(nm);
    char * u = (char *) ct_alloc(n + 6);
    memcpy(u, "ICN$U", 5);
    for (size_t i = 0; i <= n; i++) u[5 + i] = (char) toupper((unsigned char) nm[i]);
    if (icn_builtin_is_leaf_row(u)) return u;
    ct_drop(u);
    return NULL;
}
static int icn_has_unicon_row(const char * nm) { char * u = nm ? icn_unicon_row(nm) : NULL; if (u) ct_drop(u); return u != NULL; }
static int icn_callee_is_a_value(const char * nm) {
    extern int icn_builtin_is_known(const char *);
    extern void * dat_find_type(const char *);
    return nm && nm[0] != '&' &&
        (icn_proc_reassigned(nm) || icn_record_reassigned(nm) ||
        (!icn_is_proc(nm) && !icn_is_icon_function(nm) && !icn_builtin_is_known(nm) && !icn_is_unicon_thread_name(nm) && !icn_has_unicon_row(nm) && !dat_find_type(nm)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int augop_code(int aop) {
    switch (aop) {
        case AUGOP_ADD:
        return BINOP_ADD;
        case AUGOP_SUB:
        return BINOP_SUB;
        case AUGOP_MUL:
        return BINOP_MUL;
        case AUGOP_DIV:
        return BINOP_DIV;
        case AUGOP_MOD:
        return BINOP_MOD;
        case AUGOP_POW:
        return BINOP_POW;
        case AUGOP_CONCAT:
        return BINOP_CONCAT;
        case AUGOP_LCONCAT:
        return BINOP_LCONCAT;
        case AUGOP_EQ:
        return BINOP_EQ;
        case AUGOP_SEQ:
        return BINOP_SEQ;
        case AUGOP_LT:
        return BINOP_LT;
        case AUGOP_LE:
        return BINOP_LE;
        case AUGOP_GT:
        return BINOP_GT;
        case AUGOP_GE:
        return BINOP_GE;
        case AUGOP_NE:
        return BINOP_NE;
        case AUGOP_SLT:
        return BINOP_SLT;
        case AUGOP_SLE:
        return BINOP_SLE;
        case AUGOP_SGT:
        return BINOP_SGT;
        case AUGOP_SGE:
        return BINOP_SGE;
        case AUGOP_SNE:
        return BINOP_SNE;
        case AUGOP_CSET_UNION:
        return BINOP_CUNION;
        case AUGOP_CSET_DIFF:
        return BINOP_CDIFF;
        case AUGOP_CSET_INTER:
        return BINOP_CINTER;
        case AUGOP_IDENTICAL:
        return BINOP_EQV;
        case AUGOP_NIDENTICAL:
        return BINOP_NEQV;
        default:
        return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * icn_augop_build(icx_t * cx, int aop, IR_t * asn, IR_t * ω, IR_t ** op, IR_t ** ca, IR_t ** cb) {
    int bc = augop_code(aop);
    int ar = (bc >= BINOP_ADD && bc <= BINOP_MOD) || bc == BINOP_POW;
    int rel = (bc >= BINOP_LT && bc <= BINOP_NE) || (bc >= BINOP_SLT && bc <= BINOP_SNE) || bc == BINOP_EQV || bc == BINOP_NEQV;
    *op = lic_build(cx, (aop == AUGOP_ACTIVATE) ? IR_ACTIVATE : rel ? IR_BINOP_TEST : IR_BINOP, asn, ω);
    if (aop == AUGOP_ACTIVATE) IR_LIT(*op).sval = cx->pname;
    else IR_LIT(*op).ival = (bc == BINOP_ADD) ? BINOP_ADD_BIG : (bc == BINOP_SUB) ? BINOP_SUB_BIG : (bc == BINOP_MUL) ? BINOP_MUL_BIG : (bc == BINOP_CONCAT) ? BINOP_CONCAT_FRACDIGIT : bc;
    *cb = ar ? lic_build(cx, IR_COERCE_NUMERIC, *op, ω) : NULL;
    if (*cb) IR_LIT(*cb).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE | ((bc == BINOP_POW) ? COERCE_KEEP_INT : 0) | COERCE_OP_TAG(bc) | COERCE_OP_SELF_RIGHT;
    *ca = ar ? lic_build(cx, IR_COERCE_NUMERIC, *cb, ω) : NULL;
    if (*ca) IR_LIT(*ca).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE | COERCE_OP_TAG(bc);
    return *ca ? *ca : *op;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_augop_operands(int aop, IR_t * op, IR_t * ca, IR_t * cb, IR_t * lval, IR_t * rval) {
    if (aop == AUGOP_ACTIVATE) {
        ir_operand_push(op, rval);
        ir_operand_push(op, lval);
    } else {
        if (ca) { ir_operand_push(ca, lval); ir_operand_push(ca, rval); ir_operand_push(cb, rval); ir_operand_push(cb, lval); }
        ir_operand_push(op, ca ? ca : lval);
        ir_operand_push(op, cb ? cb : rval);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_unop_tt(tree_e tt) { switch (tt) { case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_CSET_COMPL: case TT_INTERROGATE: return 1; default: return 0; } }
int icn_tree_calls_a_generator(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && icn_proc_is_generator(t->c[0]->v.sval)) return 1;
    for (int i = 0; i < t->n; i++) if (icn_tree_calls_a_generator(t->c[i])) return 1;
    return 0;
}
static int icn_is_patfn_name(const char * nm) {
    const char * const pn[] = { "Len", "Pos", "Rpos", "Tab", "Rtab", "Any", "NotAny", "Span", "Break", "Breakx", "Nspan", "Arb", "Rem", "Bal", "Fail", "Succeed", "Abort", "Fence", "Arbno" };
    for (size_t i = 0; i < sizeof pn / sizeof pn[0]; i++) if (!strcmp(pn[i], nm)) return 1;
    return 0;
}
int is_resumable(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_STMT) t = lic_stmt_subj(t);
    if (!t) return 0;
    if (t->t == TT_BANG_BINARY) return 1;
    if (t->t == TT_FIELD || t->t == TT_NULL || t->t == TT_NONNULL) return (t->n > 0) ? is_resumable(t->c[0]) : 0;
    if (t->t == TT_REPALT) return 1;
    if (is_unop_tt(t->t)) return (t->n > 0) ? is_resumable(t->c[0]) : 0;
    if (t->t == TT_FNC) {
        const char * nm = (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) ? t->c[0]->v.sval : NULL;
        if (nm && !strcmp(nm, "ICN$CRIT") && t->n == 3) return is_resumable(t->c[1]);
        if (nm && icn_is_patfn_name(nm) && !icn_is_proc(nm)) { for (int i = 1; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
        if (!nm || icn_call_allow_gen(nm) || icn_callee_is_a_value(nm)) return 1;
        for (int i = 1; i < t->n; i++) if (is_resumable(t->c[i])) return 1;
        return 0;
    }
    if (t->t == TT_LIMIT) return (t->n > 0) ? is_resumable(t->c[0]) : 0;
    if (lc_is_binop(t->t)) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_IDX) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_ASSIGN) { if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) return 1; return (t->n > 1) ? is_resumable(t->c[1]) : 0; }
    if (t->t == TT_SWAP) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_CONJ) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i]) || icn_tree_is_cursor_mover(t->c[i])) return 1; return 0; }
    if (t->t == TT_MAKELIST || t->t == TT_VLIST) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i]) || icn_tree_is_cursor_mover(t->c[i])) return 1; return 0; }
    if (t->t == TT_CASE) { for (int i = 2; i < t->n; i += 2) if (is_resumable(t->c[i])) return 1; return (t->n > 1 && (t->n - 1) % 2 == 1) ? is_resumable(t->c[t->n - 1]) : 0; }
    switch (t->t) {
        case TT_IF:
        case TT_SCAN:
        case TT_EVERY:
        case TT_TO:
        case TT_TO_BY:
        case TT_ALTERNATE:
        case TT_REPEAT:
        case TT_WHILE:
        case TT_UNTIL:
        case TT_REVASSIGN:
        case TT_REVSWAP:
        case TT_ITERATE:
        return 1;
        default:
        return 0;
    }
}
static int icn_snap_scan_base(const icx_t * cx, const tree_t * b0, int snap) { return snap && icn_tree_is_kw_scan(b0) && cx->scan_sp == 0; }
static IR_t * lower_lvalue_var_k(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res, int snap) { cx->kw_snap = snap; return lower_lvalue_var(cx, t, ω, var_res); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    icx_t * cx = (icx_t *) vcx;
    IR_t * sps = cx->psucc;
    IR_t * spf = cx->pfail;
    cx->psucc = NULL;
    cx->pfail = F;
    IR_t * r = NULL;
    IR_t * e = lic_lower(cx, a, NULL, F, &r);
    cx->psucc = sps;
    cx->pfail = spf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((icx_t *) vcx)->g, icn_arg_lower, vcx, a); }
static int icn_is_proc_or_record_name(const char * nm) {
    if (icn_is_proc(nm)) return 1;
    for (int ri = 0; ri < g_lower.icn.icn_record_name_count; ri++) if (!strcmp(g_lower.icn.icn_record_names[ri], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_undeclared_assigned_var(const icx_t * cx, const char * nm) {
    if (!nm || !nm[0] || nm[0] == '&') return 0;
    if (icn_is_local(cx, nm) || icn_is_own_global(cx, nm) || icn_is_proc_or_record_name(nm)) return 0;
    if (icn_builtin_is_known(nm) || icn_builtin_is_generator(nm) || icn_builtin_arity(nm) != -99) return 0;
    return icn_name_assigned(nm);
}
int icn_callee_is_name(const icx_t * cx, const tree_t * fn) {
    return fn && fn->t == TT_VAR && fn->v.sval && fn->v.sval[0] != '&' && !icn_is_local(cx, fn->v.sval) && !(icn_is_own_global(cx, fn->v.sval) && !icn_is_proc(fn->v.sval)) &&
        !icn_proc_reassigned(fn->v.sval) && !icn_record_reassigned(fn->v.sval) && !icn_builtin_reassigned(fn->v.sval) && !icn_undeclared_assigned_var(cx, fn->v.sval) &&
        !icn_is_static_mangled_name(fn->v.sval);
}
int icn_arg_stages(const icx_t * cx, const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_IDX || a->t == TT_FIELD) return 1;
    if (a->t == TT_SECTION || a->t == TT_SECTION_PLUS || a->t == TT_SECTION_MINUS) return 1;
    if ((a->t == TT_ASSIGN || a->t == TT_AUGOP || a->t == TT_SWAP) && a->n > 1 && a->c[0] && a->c[0]->t == TT_VAR) return icn_tree_is_kw_scan(a->c[0]) ? 0 : icn_arg_stages(cx, a->c[0]);
    if (icn_tree_is_kw_var(a)) return 1;
    if (a->t == TT_FNC && a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval && icn_callee_is_name(cx, a->c[0]) && icn_call_yields_a_variable((icx_t *) cx, a->c[0]->v.sval)) return 1;
    if (a->t != TT_VAR || !a->v.sval || a->v.sval[0] == '&') return 0;
    const char * nm = a->v.sval;
    if (icn_is_proc_or_record_name(nm)) return 0;
    if (icn_is_local(cx, nm)) return 1;
    for (int i = 0; i < cx->npn; i++) if (cx->pn[i] && !strcmp(cx->pn[i], nm)) return 1;
    for (int i = 0; i < cx->ngn; i++) if (cx->gn[i] && !strcmp(cx->gn[i], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int after_line = cx->after_line;
    cx->after_line = 0;
    if (name && nargs == 1 && cx->prog && cx->prog->v.ival && !strcmp(name, "system") && icn_callable_proc_index("system") < 0) name = "ICN$USYSTEM";
    int nm_form = name && nargs == 1 && !strcmp(name, "name");
    if (name && !nm_form && cx->prog && cx->prog->v.ival && strncmp(name, "ICN$", 4) && !icn_is_proc(name)) { char * un = icn_unicon_row(name); if (un) name = un; }
    if (name && nargs <= 1 && cx->prog && cx->prog->v.ival && !strcmp(name, "mutex") && icn_callable_proc_index("mutex") < 0) name = "ICN$UMUTEX";
    if (name && !strcmp(name, "seq") && icn_callable_proc_index("seq") < 0) { IR_t * sq = lic_lower_seq(cx, t, argbase, nargs, γ, ω, res); if (sq) return sq; }
    if (name && !strcmp(name, "function") && nargs == 0 && icn_callable_proc_index("function") < 0) { IR_t * fg = lower_function_gen(cx, γ, ω, res); if (fg) return fg; }
    if (name && !strcmp(name, "globalnames") && (nargs == 0 || (nargs == 1 && t->c[argbase] && t->c[argbase]->t == TT_VAR && t->c[argbase]->v.sval && t->c[argbase]->v.sval[0] == '&')) &&
        icn_callable_proc_index("globalnames") < 0) {
        IR_t * gg = lower_globalnames_gen(cx, γ, ω, res);
        if (gg) return gg;
    }
    if (name && !strcmp(name, "key") && nargs == 1 && icn_callable_proc_index("key") < 0) { IR_t * kg = lower_key(cx, t, argbase, nargs, γ, ω, res); if (kg) return kg; }
    if (name && !strcmp(name, "name") && nargs == 1) {
        const tree_t * na = t->c[argbase];
        if (na && na->t == TT_ALTERNATE && na->n >= 1) {
            tree_t * alt = ast_node_new(TT_ALTERNATE);
            for (int k = 0; k < na->n; k++) {
                tree_t * call = ast_node_new(TT_FNC);
                tree_t * fnv = ast_node_new(TT_VAR);
                fnv->v.sval = (char *) "name";
                ast_push(call, fnv);
                ast_push(call, (tree_t *) na->c[k]);
                ast_push(alt, call);
            }
            return lic_lower(cx, alt, γ, ω, res);
        }
        if (na && (na->t == TT_VAR || na->t == TT_KEYWORD) && na->v.sval && na->v.sval[0] == '&') {
            IR_t * nd = lic_build(cx, IR_LIT_STRING, γ, ω);
            IR_LIT(nd).sval = na->v.sval;
            if (res) *res = nd;
            return nd;
        }
        if (na && na->t == TT_VAR && na->v.sval && na->v.sval[0] != '&') {
            const char * vn = na->v.sval;
            const char * st = strstr(vn, "__STATIC__");
            if (st && cx->pname && !strncmp(vn, cx->pname, strlen(cx->pname)) && st == vn + strlen(cx->pname)) vn = st + 10;
            IR_t * nd = lic_build(cx, IR_LIT_STRING, γ, ω);
            IR_LIT(nd).sval = (char *) vn;
            if (res) *res = nd;
            return nd;
        }
        IR_t * vr = NULL;
        IR_t * ve = lower_lvalue_var(cx, na, ω, &vr);
        if (ve && vr) {
            IR_t * ncall = lic_build(cx, IR_CALL, γ, ω);
            IR_LIT(ncall).sval = (char *) (cx->prog && cx->prog->v.ival ? "ICN$UNAME" : name);
            if (res) *res = ncall;
            IR_t * argβ = cx->beta;
            lc_γ_to(vr, ncall);
            ir_operand_push(ncall, vr);
            if (argβ && argβ != ω) { lic_ω_to(ncall, argβ); cx->beta = argβ; } else cx->beta = ω;
            return ve;
        }
    }
    int gb = name && icn_callable_proc_index(name) < 0 &&
        ((nargs >= 2 && nargs <= 4 && (!strcmp(name, "find") || !strcmp(name, "upto"))) || (nargs == 1 && (!strcmp(name, "find") || !strcmp(name, "upto"))) || (nargs <= 6 && !strcmp(name, "bal")));
    int is_cursor_mover = name && (!strcmp(name, "tab") || !strcmp(name, "move"));
    int fill_bal_cset = gb && nargs == 0 && !strcmp(name, "bal");
    IR_t * call = lic_build(cx, icn_proc_is_generator(name) ? IR_PROC_GEN : (gb ? IR_CALL_BUILTIN_GEN : IR_CALL), γ, ω);
    IR_LIT(call).sval = (char *) name;
    if (res) *res = call;
    int cline = (t && t->line > 0) ? t->line : ((t && argbase > 0 && t->c[argbase - 1]) ? t->c[argbase - 1]->line : 0);
    int own_line = cline;
    if (after_line > 0) cline = after_line;
    IR_t * callin = (name && cline > 0) ? icn_line_mark(cx, cline, call) : call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    if (!chains) { for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { if (is_cursor_mover && icn_arg_is_scan_fn(t->c[argbase + k])) continue; chains = 1; break; } }
    IR_t * prev = NULL;
    IR_t * entry = callin;
    IR_t * aω = ω;
    IR_t * last_ar = NULL;
    int nstage = 0;
    for (int k = 0; k < nargs; k++) if (icn_arg_stages(cx, t->c[argbase + k])) nstage++;
    IR_t * args_r[nargs > 0 ? nargs : 1];
    int staged[nargs > 0 ? nargs : 1];
    int entry_set = 0;
    for (int k = 0; k < nargs; k++) {
        const tree_t * a = t->c[argbase + k];
        IR_t * ar = NULL;
        IR_t * ae;
        staged[k] = 0;
        if (nstage && icn_tree_is_kw_var(a)) { staged[k] = 2; args_r[k] = NULL; continue; }
        if (nstage && icn_arg_stages(cx, a)) { cx->beta = aω; ae = lower_lvalue_var(cx, a, aω, &ar); if (ae && ar) staged[k] = 1; }
        if (!staged[k]) {
            const tree_t * na = k < nargs - 1 ? t->c[argbase + k + 1] : NULL;
            if (k == nargs - 1) cx->after_line = own_line;
            else if (na && na->line > 0 && (na->t == TT_KEYWORD || (na->t == TT_VAR && na->v.sval && na->v.sval[0] == '&'))) cx->after_line = na->line;
            ae = lic_lower(cx, a, (k == nargs - 1 && !nstage) ? callin : NULL, aω, &ar);
        }
        aω = cx->beta;
        if (!entry_set) { entry = ae; entry_set = 1; }
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        args_r[k] = ar;
    }
    for (int k = 0; k < nargs; k++) if (staged[k] == 2 || (staged[k] && args_r[k])) {
        IR_t * drf;
        if (icn_tree_is_kw_var(t->c[argbase + k])) {
            drf = lic_build(cx, IR_KW_ICON, NULL, aω);
            IR_LIT(drf).sval = (char *) t->c[argbase + k]->v.sval;
        } else {
            drf = lic_build(cx, IR_DEREF, NULL, aω);
            ir_operand_push(drf, args_r[k]);
        }
        if (prev) lc_γ_to(prev, drf);
        else entry = drf;
        prev = drf;
        args_r[k] = drf;
    }
    for (int k = 0; k < nargs; k++) if (args_r[k]) { ir_operand_push(call, args_r[k]); last_ar = args_r[k]; }
    if (nstage && prev) { last_ar = prev; if (!(icn_proc_is_generator(name) || gb || is_cursor_mover)) lc_γ_to(prev, callin); }
    if (fill_bal_cset) {
        IR_t * kc = lic_build(cx, IR_KW_ICON, NULL, aω);
        IR_LIT(kc).sval = (char *) "&cset";
        if (prev) lc_γ_to(prev, kc);
        else entry = kc;
        ir_operand_push(call, kc);
        prev = kc;
        last_ar = kc;
    }
    if ((icn_proc_is_generator(name) || gb || is_cursor_mover) && last_ar) lc_γ_to(last_ar, callin);
    const tree_t * la = (nargs > 0) ? t->c[argbase + nargs - 1] : NULL;
    int la_res = la && is_resumable(la) && !(is_cursor_mover && icn_arg_is_scan_fn(la));
    int chain_live = (aω != ω);
    if (la_res || chain_live) lic_ω_to(call, aω);
    cx->beta = (icn_proc_is_generator(name) || gb || is_cursor_mover) ? call : aω;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_assign_var_name(const tree_t * t) {
    while (t && (t->t == TT_ASSIGN || t->t == TT_AUGOP || t->t == TT_SWAP || t->t == TT_REVSWAP) && t->n >= 2) t = t->c[0];
    return (t && t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') ? t->v.sval : NULL;
}
static int icn_assign_names_var(const tree_t * t) { return t && (t->t == TT_ASSIGN || t->t == TT_AUGOP || t->t == TT_SWAP || t->t == TT_REVSWAP) && icn_assign_var_name(t) != NULL; }
IR_t * lower_idx_base(icx_t * cx, const tree_t * b0, IR_t * ω, IR_t ** br) {
    int snap = cx->kw_snap;
    cx->kw_snap = 0;
    if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&' && !(cx->ret_lv && !icn_ret_global_name(cx, b0->v.sval))) {
        IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω);
        IR_LIT(vr).sval = b0->v.sval;
        *br = vr;
        return vr;
    }
    if (b0->t == TT_IDX) return lower_idx_var(cx, b0, ω, br);
    if (b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_FIELD || icn_assign_names_var(b0)) {
        IR_t * e2 = lower_lvalue_var_k(cx, b0, ω, br, snap);
        return e2 ? e2 : lic_lower(cx, b0, NULL, ω, br);
    }
    if (icn_snap_scan_base(cx, b0, snap)) return lower_lvalue_var_k(cx, b0, ω, br, snap);
    return lic_lower(cx, b0, NULL, ω, br);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_idx_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    int snap = cx->kw_snap;
    cx->kw_snap = 0;
    if (t->n < 2 || !t->c[0]) { IR_t * su = lic_build(cx, IR_SUCCEED, NULL, ω); *var_res = su; return su; }
    IR_t * br = NULL;
    const tree_t * b0 = t->c[0];
    IR_t * b4 = cx->beta;
    cx->kw_snap = snap;
    IR_t * entry = lower_idx_base(cx, b0, ω, &br);
    IR_t * cur = br;
    IR_t * hook = br;
    IR_t * prevβ = (b0->t == TT_VAR || icn_tree_is_kw_var(b0)) ? NULL : (b0->t == TT_IDX && cx->beta == b4) ? NULL : cx->beta;
    for (int k = 1; k < t->n; k++) {
        IR_t * ir = NULL;
        IR_t * ie = lic_lower(cx, t->c[k], NULL, prevβ ? prevβ : ω, &ir);
        prevβ = cx->beta;
        lc_γ_to(hook, ie);
        IR_t * sub = lic_build(cx, IR_SUBSCRIPT, NULL, prevβ ? prevβ : ω);
        lic_γ_to(ir, sub);
        ir_operand_push(sub, cur);
        ir_operand_push(sub, ir);
        cur = sub;
        hook = sub;
    }
    *var_res = cur;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    int snap = cx->kw_snap;
    cx->kw_snap = 0;
    if (!t) return NULL;
    if (t->t == TT_ALTERNATE && t->n >= 1) return lower_alt_lv(cx, t, ω, var_res);
    if (t->t == TT_IF) return lower_if_impl(cx, t, NULL, ω, var_res, 1);
    if (t->t == TT_CASE) return lower_case_impl(cx, t, NULL, ω, var_res, 1);
    if (t->t == TT_SEQ_EXPR) return lower_seq_impl(cx, t, NULL, ω, var_res, 1);
    if (t->t == TT_LIMIT) return lower_limit_impl(cx, t, NULL, ω, var_res, 1);
    if (icn_tree_is_kw_var(t)) {
        IR_t * kv = lic_build(cx, IR_KW_ICON, NULL, ω);
        IR_LIT(kv).sval = (char *) t->v.sval;
        kv->pat_static = (snap && icn_tree_is_kw_scan(t)) ? 2 : 1;
        *var_res = kv;
        return kv;
    }
    if (t->t == TT_FNC && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "variable") && !icn_is_local(cx, "variable")) {
        IR_t * mk = lic_build(cx, IR_CALL, NULL, ω);
        IR_LIT(mk).sval = (char *) "ICN$VARNAME";
        IR_t * nr = NULL;
        IR_t * ne = lic_lower(cx, t->c[1], mk, ω, &nr);
        if (nr) ir_operand_push(mk, nr);
        *var_res = mk;
        return ne;
    }
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') { IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = t->v.sval; *var_res = vr; return vr; }
    if (icn_assign_names_var(t)) {
        IR_t * ar = NULL;
        IR_t * ae = lic_lower(cx, t, NULL, ω, &ar);
        IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω);
        IR_LIT(vr).sval = (char *) icn_assign_var_name(t);
        lc_γ_to(ar, vr);
        *var_res = vr;
        return ae;
    }
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && icn_callee_is_name(cx, t->c[0]) && t->c[0]->t == TT_VAR && icn_call_yields_a_variable(cx, t->c[0]->v.sval) && !icn_variable_lit(cx, t)) {
        IR_t * cr = NULL;
        IR_t * ce = lic_lower_call(cx, t->c[0]->v.sval, t, 1, t->n - 1, NULL, ω, &cr);
        if (!ce || !cr) return NULL;
        *var_res = cr;
        return ce;
    }
    if (t->t == TT_IDX) { cx->kw_snap = snap; return lower_idx_var(cx, t, ω, var_res); }
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0]) {
        IR_t * it = lic_build(cx, IR_ITERATE, NULL, ω);
        IR_LIT(it).ival = 0;
        IR_LIT(it).sval = "lv";
        const tree_t * b0 = t->c[0];
        IR_t * ar = NULL;
        IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
            IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω);
            IR_LIT(vr).sval = b0->v.sval;
            ar = vr;
            ae = vr;
        } else if (b0->t == TT_IDX || b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_FIELD) {
            IR_t * e2 = lower_lvalue_var(cx, b0, ω, &ar);
            ae = e2 ? e2 : lic_lower(cx, b0, NULL, ω, &ar);
        } else {
            cx->beta = ω;
            ae = lic_lower(cx, b0, NULL, ω, &ar);
            IR_t * bβ = cx->beta;
            if (bβ && bβ != ω && is_resumable(b0)) lc_ω_to_β(it, bβ);
        }
        lc_γ_to(ar, it);
        ir_operand_push(it, ar);
        cx->beta = it;
        *var_res = it;
        return ae;
    }
    if (t->t == TT_RANDOM && t->n > 0 && t->c[0]) {
        IR_t * rn = lic_build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = t->c[0];
        IR_t * ar = NULL;
        IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
            IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω);
            IR_LIT(vr).sval = b0->v.sval;
            ar = vr;
            ae = vr;
        } else if (b0->t == TT_IDX || b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_FIELD) {
            IR_t * e2 = lower_lvalue_var(cx, b0, ω, &ar);
            ae = e2 ? e2 : lic_lower(cx, b0, NULL, ω, &ar);
        } else ae = lic_lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn);
        ir_operand_push(rn, ar);
        *var_res = rn;
        return ae;
    }
    if ((t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS) && t->n >= 3 && t->c[0] && t->c[1] && t->c[2]) {
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = lic_build(cx, IR_SUBSCRIPT, NULL, ω);
        IR_LIT(sec).ival = 0;
        IR_LIT(sec).sval = "lv";
        IR_t * ar = NULL;
        IR_t * ae;
        const tree_t * b0 = t->c[0];
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
            IR_t * vr = lic_build(cx, IR_VAR_REF, NULL, ω);
            IR_LIT(vr).sval = b0->v.sval;
            ar = vr;
            ae = vr;
        } else if (b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_IDX || b0->t == TT_FIELD || icn_assign_names_var(b0) || icn_snap_scan_base(cx, b0, snap))
            ae = lower_lvalue_var_k(cx, b0, ω, &ar, snap);
        else ae = lic_lower(cx, b0, NULL, ω, &ar);
        IR_t * aβ = (b0->t == TT_VAR || icn_tree_is_kw_var(b0)) ? NULL : cx->beta;
        IR_t * ωa = (aβ && aβ != ω && aβ != sec) ? aβ : ω;
        IR_t * br = NULL;
        IR_t * be = lic_lower(cx, t->c[1], NULL, ωa, &br);
        lc_γ_to(ar, be);
        IR_t * bβ = cx->beta;
        IR_t * ωb = (bβ && bβ != ωa && bβ != sec) ? bβ : ωa;
        IR_t * cr = NULL;
        IR_t * ce = lic_lower(cx, t->c[2], sec_variant ? NULL : sec, ωb, &cr);
        lc_γ_to(br, ce);
        IR_t * cβ = cx->beta;
        IR_t * ωc = (cβ && cβ != ωb && cβ != sec) ? cβ : ωb;
        if (sec_variant) {
            IR_t * op = lic_build(cx, IR_BINOP, sec, ωc);
            IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB;
            ir_operand_push(op, br);
            ir_operand_push(op, cr);
            lic_γ_to(cr, op);
            cr = op;
        }
        ir_operand_push(sec, ar);
        ir_operand_push(sec, br);
        ir_operand_push(sec, cr);
        lc_γ_to(cr, sec);
        if (ωc != ω) lic_ω_to(sec, ωc);
        cx->beta = ωc;
        *var_res = sec;
        return ae;
    }
    if (t->t == TT_FIELD && t->n > 0 && t->c[0]) {
        IR_t * fg = lic_build(cx, IR_FIELD_VAR, NULL, ω);
        IR_LIT(fg).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        fg->seal = icn_field_hint(cx->prog, IR_LIT(fg).sval);
        IR_t * br = NULL;
        IR_t * be = lic_lower(cx, t->c[0], NULL, ω, &br);
        lc_γ_to(br, fg);
        ir_operand_push(fg, br);
        *var_res = fg;
        return be;
    }
    if ((t->t == TT_NULL || t->t == TT_NONNULL) && t->n > 0 && t->c[0]) {
        IR_t * b4 = cx->beta;
        IR_t * clv = NULL;
        IR_t * ce = lower_lvalue_var(cx, t->c[0], ω, &clv);
        if (!ce || !clv) return NULL;
        IR_t * ut = lic_build(cx, IR_NULLTEST_VAR, NULL, ω);
        IR_LIT(ut).sval = (t->t == TT_NONNULL) ? "nonnull" : "null";
        ir_operand_push(ut, clv);
        lc_γ_to(clv, ut);
        if (clv && ir_is_generator_kind(clv->op)) lc_ω_to_β(ut, clv);
        else if (cx->beta && cx->beta != b4 && cx->beta != ω && cx->beta != ut) lic_ω_to(ut, cx->beta);
        *var_res = ut;
        return ce;
    }
    if (t->t == TT_CONJ && t->n > 1 && t->c[0] && t->c[t->n - 1]) {
        const tree_t * head = t->c[0];
        if (t->n > 2) { tree_t * hc = ast_node_new(TT_CONJ); for (int k = 0; k < t->n - 1; k++) ast_push(hc, (tree_t *) t->c[k]); head = hc; }
        IR_t * ir = NULL;
        IR_t * ie = lic_lower(cx, head, NULL, ω, &ir);
        if (!ie || !ir) return NULL;
        IR_t * lv = NULL;
        IR_t * lve = lower_lvalue_var(cx, t->c[t->n - 1], ω, &lv);
        if (!lve || !lv) return NULL;
        lc_γ_to(ir, lve);
        *var_res = lv;
        return ie;
    }
    if ((t->t == TT_ASSIGN || t->t == TT_SWAP || t->t == TT_AUGOP || t->t == TT_REVSWAP || t->t == TT_REVASSIGN) && t->n > 1 && t->c[0]) {
        int sv = cx->ret_lv;
        int svr = cx->var_road;
        cx->ret_lv = 0;
        cx->var_road = 1;
        IR_t * ir = NULL;
        IR_t * ie = lic_lower(cx, t, NULL, ω, &ir);
        cx->ret_lv = sv;
        cx->var_road = svr;
        if (!ie || !ir) return NULL;
        if ((ir->op == IR_ASSIGN_VAR || ir->op == IR_SWAP_VAR || ir->op == IR_REV_ASSIGN_VAR) && ir->n_operands == 2 && ir->operands[0]) {
            IR_t * fw = lic_build(cx, IR_CONJUNCTION, NULL, ω);
            ir_operand_push(fw, ir->operands[0]);
            lc_γ_to(ir, fw);
            *var_res = fw;
            return ie;
        }
        IR_t * lv = NULL;
        IR_t * lve = lower_lvalue_var(cx, t->c[0], ω, &lv);
        if (!lve || !lv) return NULL;
        lc_γ_to(ir, lve);
        *var_res = lv;
        return ie;
    }
    return NULL;
}
tree_t * pm_imm_commit(void) { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) "*$imm"; q->slen = 5; return pm_call("SNO$PDEF", q, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * pl = cx->op_last;
    int pn = cx->op_line;
    if (t && t == pl && pn > 0 && cx->after_line == 0) cx->after_line = pn;
    if (t && t->line > 0 && t->n > 0 && icn_op_hands_line(t->t)) { cx->op_last = t->c[t->n - 1]; cx->op_line = t->line; }
    IR_t * r = lower_node(cx, t, γ, ω, res);
    cx->op_last = pl;
    cx->op_line = pn;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_trace_var_form(const icx_t * cx, const tree_t * t) {
    if (!t) return 0;
    if (icn_tree_is_kw_var(t)) return !icn_tree_is_kw_scan(t) || cx->scan_sp == 0;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') return !icn_is_local(cx, t->v.sval);
    if ((t->t == TT_IDX || t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS) && t->n >= 2 && t->c[0]) return icn_trace_var_form(cx, t->c[0]);
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0]) return 1;
    return 0;
}
int icn_trace_operand_has_var(const icx_t * cx, const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ALTERNATE) { for (int i = 0; i < t->n; i++) if (icn_trace_var_form(cx, t->c[i])) return 1; return 0; }
    return icn_trace_var_form(cx, t);
}
static void icn_trace_iterate_relax(const icx_t * cx, const tree_t * t, IR_t * ar) {
    if (!ar || ar->op != IR_ITERATE || !t || t->t != TT_ITERATE || t->n < 1) return;
    if (!IR_LIT(ar).sval || strcmp(IR_LIT(ar).sval, "lv")) return;
    if (!icn_trace_var_form(cx, t->c[0])) IR_LIT(ar).sval = (char *) "lvv";
}
static int icn_conduit_reaches(const IR_graph_t * g, const IR_t * x, const IR_t * dj) {
    for (int i = 0; x && x->op == IR_GOTO && x->γ.node == x->ω.node && i < g->n; i++) { x = x->ω.node; if (x == dj) return 1; }
    return 0;
}
static IR_t * lower_alt_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lval) {
    int n = t->n;
    if (n < 1) { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    IR_graph_t * g = cx->g;
    IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
    lic_γ_to(dj, γ);
    lic_ω_to(dj, ω);
    IR_t * resv[n];
    for (int j = 0; j < n; j++) {
        int before = g->n;
        IR_t * ar = NULL;
        cx->beta = dj;
        int use_lv = (lval == 2) ? icn_trace_var_form(cx, t->c[j]) : lval;
        if (lval == 3) use_lv = 0;
        IR_t * ej = use_lv ? lower_lvalue_var_k(cx, t->c[j], dj, &ar, lval == 2) : (lval == 3) ? lower_result_operand(cx, t->c[j], dj, dj, &ar) : lic_lower(cx, t->c[j], dj, dj, &ar);
        if (use_lv && !(ej && ar)) { ar = NULL; ej = lic_lower(cx, t->c[j], dj, dj, &ar); use_lv = 0; }
        if (use_lv && lval == 2) icn_trace_iterate_relax(cx, t->c[j], ar);
        if (use_lv && ar) lc_γ_to(ar, dj);
        IR_t * ab = cx->beta;
        int ab_in_arm = 0;
        if (ab && ab != dj) for (int k = before; k < g->n; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
        if (ab_in_arm && icn_conduit_reaches(g, ab, dj)) ab_in_arm = 0;
        IR_t * rj = ab_in_arm ? ab : dj;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        ir_operand_push(dj, ej);
        ir_operand_push(dj, rj);
        resv[j] = ar;
    }
    for (int j = 0; j < n; j++) ir_operand_push(dj, icn_arm_result(resv[j]));
    IR_LIT(dj).ival = (long) n;
    cx->beta = dj;
    *res = dj;
    return icn_dj_α_entry(g, dj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) { return lower_alt_impl(cx, t, γ, ω, res, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt_lv(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) { return lower_alt_impl(cx, t, NULL, ω, var_res, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_trace_operand(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (t && t->t == TT_ALTERNATE && t->n >= 1) return lower_alt_impl(cx, t, γ, ω, res, 2);
    int st = t && (t->t == TT_IF || t->t == TT_CASE || t->t == TT_SEQ_EXPR || t->t == TT_CONJ || t->t == TT_LIMIT);
    int as = t && (t->t == TT_ASSIGN || t->t == TT_AUGOP || t->t == TT_SWAP || t->t == TT_REVASSIGN || t->t == TT_REVSWAP);
    if ((st || as) && icn_return_var_form(cx, t)) return lower_result_operand(cx, t, γ, ω, res);
    if (icn_trace_var_form(cx, t)) { IR_t * ar = NULL; IR_t * e = lower_lvalue_var_k(cx, t, ω, &ar, 1); if (e && ar) { icn_trace_iterate_relax(cx, t, ar); lc_γ_to(ar, γ); *res = ar; return e; } }
    return lic_lower(cx, t, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_ret_base_global(const icx_t * cx, const tree_t * b) { return b && ((b->t == TT_VAR && icn_ret_global_name(cx, b->v.sval)) || icn_tree_is_kw_var(b)); }
static int icn_return_var_form(icx_t * cx, const tree_t * t) {
    if (!t) return 0;
    if (icn_tree_is_kw_var(t)) return !icn_tree_is_kw_scan(t) || cx->scan_sp == 0;
    if (t->t == TT_VAR) return icn_ret_global_name(cx, t->v.sval);
    if (t->t == TT_FIELD) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_IDX) return t->n >= 2 && t->c[0] != NULL;
    if (t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS || t->t == TT_ITERATE || t->t == TT_RANDOM) return t->n > 0 && icn_ret_base_global(cx, t->c[0]);
    if ((t->t == TT_NULL || t->t == TT_NONNULL) && t->n > 0) return icn_return_var_form(cx, t->c[0]);
    if ((t->t == TT_ASSIGN || t->t == TT_AUGOP || t->t == TT_SWAP || t->t == TT_REVASSIGN || t->t == TT_REVSWAP) && t->n > 1) return icn_return_var_form(cx, t->c[0]);
    if (t->t == TT_LIMIT) return t->n > 0 && icn_return_var_form(cx, t->c[0]);
    if (t->t == TT_CONJ || t->t == TT_SEQ_EXPR) {
        for (int k = t->n - 1; k >= 0; k--) { const tree_t * e = (t->c[k] && t->c[k]->t == TT_STMT) ? lic_stmt_subj(t->c[k]) : t->c[k]; if (e) return icn_return_var_form(cx, e); }
        return 0;
    }
    if (t->t == TT_ALTERNATE) { for (int k = 0; k < t->n; k++) if (icn_return_var_form(cx, t->c[k])) return 1; return 0; }
    if (t->t == TT_IF) return (t->n > 1 && icn_return_var_form(cx, t->c[1])) || (t->n > 2 && icn_return_var_form(cx, t->c[2]));
    if (t->t == TT_CASE) { for (int k = 2; k < t->n; k += 2) if (icn_return_var_form(cx, t->c[k])) return 1; return (t->n > 1 && (t->n - 1) % 2 == 1) ? icn_return_var_form(cx, t->c[t->n - 1]) : 0; }
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && icn_callee_is_name(cx, t->c[0]) && t->c[0]->t == TT_VAR && icn_call_yields_a_variable(cx, t->c[0]->v.sval) && !icn_variable_lit(cx, t)) return 1;
    return 0;
}
IR_t * lower_result_operand(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (icn_return_var_form(cx, t)) {
        switch (t->t) {
            case TT_ALTERNATE:
            return lower_alt_impl(cx, t, γ, ω, res, 3);
            case TT_IF:
            return lower_if_impl(cx, t, γ, ω, res, 3);
            case TT_CASE:
            return lower_case_impl(cx, t, γ, ω, res, 3);
            case TT_CONJ:
            case TT_SEQ_EXPR:
            return lower_seq_impl(cx, t, γ, ω, res, 3);
            case TT_LIMIT:
            return lower_limit_impl(cx, t, γ, ω, res, 3);
            default:
            break;
        }
        int sv = cx->ret_lv;
        cx->ret_lv = 1;
        IR_t * ar = NULL;
        IR_t * e = lower_lvalue_var_k(cx, t, ω, &ar, 1);
        cx->ret_lv = sv;
        if (e && ar) { lc_γ_to(ar, γ); *res = ar; return e; }
    }
    return lic_lower(cx, t, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) { return lower_if_impl(cx, t, γ, ω, res, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_case(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) { return lower_case_impl(cx, t, γ, ω, res, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_collect_implicit_locals(const tree_t * n, const char ** excl, int nexcl, lc_vec * out, int unicon) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_LOCAL || n->t == TT_STATIC_DECL || n->t == TT_GLOBAL) return;
    if (n->t == TT_FIELD) { if (n->n > 0) icn_collect_implicit_locals(n->c[0], excl, nexcl, out, unicon); return; }
    if (n->t == TT_VAR && n->v.sval) {
        const char * nm = n->v.sval;
        int found = 0;
        for (int k = 0; k < nexcl && !found; k++) if (excl[k] && !strcmp(excl[k], nm)) found = 1;
        for (int k = 0; k < out->n && !found; k++) if (!strcmp(LC_AT(out, const char *, k), nm)) found = 1;
        if (!found) {
            extern int icn_builtin_is_known(const char *);
            extern void * dat_find_type(const char *);
            if (!icn_builtin_is_known(nm) && icn_builtin_arity(nm) == -99 && !dat_find_type(nm) && !icn_proc_reassigned(nm) && !(unicon && icn_has_unicon_row(nm))) lc_vec_push(out, &nm);
        }
    }
    for (int i = 0; i < n->n; i++) icn_collect_implicit_locals(n->c[i], excl, nexcl, out, unicon);
}
