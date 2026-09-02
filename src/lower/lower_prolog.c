#include "rt/rt_arena.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
typedef struct { IR_graph_t * g; IR_t * tω; IR_t * beta; IR_t * cut_ω; IR_t * ite_funnel; } lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
static void pl_cells_stamp(IR_graph_t * g) { static int _on = -1; if (_on < 0) { const char * e = getenv("SCRIP_PL_CELLS"); _on = (e && *e == '1') ? 1 : 0; } if (_on && g) g->pl_cells_graph = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_var_name(int slot) {
    static char * cache[1024]; static char buf[24];
    if (slot >= 0 && slot < 1024) { if (!cache[slot]) { snprintf(buf, sizeof buf, "G%d", slot); cache[slot] = strdup(buf); } return cache[slot]; }
    snprintf(buf, sizeof buf, "G%d", slot); return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_pi_name(const char * nm, int ar) {
    static char buf[264];
    if (!nm) return strdup("?");
    if (ar == 0 && strcmp(nm, "main") == 0) return strdup(nm);
    snprintf(buf, sizeof buf, "%s/%d", nm, ar); return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_existence_err(lcx_t * cx, const char * nm, int ar, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$existence_error";
    IR_t * a = build(cx, IR_LIT_STRING, call, ωfail); IR_LIT(a).sval = pl_pi_name(nm, ar);
    ir_operand_push(call, a);
    if (entry_out) *entry_out = a;
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_fact_choice_lookup(const char * nm, int arity) {
    extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
    extern stage2_t g_stage2;
    char key[128]; snprintf(key, sizeof key, "%s/%d", nm, arity);
    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    if (!ch || ch->t != TT_CHOICE || ch->n < 1) return NULL;
    for (int i = 0; i < ch->n; i++) {
        const tree_t * cl = ch->c[i];
        if (!cl || cl->t != TT_CLAUSE || cl->n != arity || (int) cl->v.dval != arity) return NULL;
        for (int j = 0; j < arity; j++) { const tree_t * h = cl->c[j]; if (!h || (h->t != TT_QLIT && h->t != TT_ILIT)) return NULL; }
    }
    return ch;
}
static const char * g_pl_nl_arith[] = { "*", "**", "+", "-", "/", "//", "/\\", "<<", ">>", "\\", "\\/", "^", "abs", "acos", "asin", "atan", "ceiling", "cos", "div", "exp", "float",
    "float_fractional_part", "float_integer_part", "floor", "gcd", "integer", "log", "max", "min", "mod", "msb", "rem", "round", "sign", "sin", "sqrt", "tan", "truncate", "xor", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_arith_fn(const char * s, int n) { if (!s || n < 1 || n > 2) return 0; for (int i = 0; g_pl_nl_arith[i]; i++) if (!strcmp(s, g_pl_nl_arith[i])) return 1; return 0; }
static int is_builtin_visible(const char * s) { return s && (!strcmp(s, "write") || !strcmp(s, "nl") || !strcmp(s, "format") || !strcmp(s, "aggregate_all")); }
static const char * g_pl_nl_builtins[] = { "<", "<=", "=..", "=:=", "=<", "==", "=\\=", ">", ">=", "@<", "@=<", "@>", "@>=", "\\==", "abolish", "acyclic_term", "arg", "assert", "asserta", "assertz", "atom",
    "atom_chars", "atom_codes", "atom_concat", "atom_length", "atom_number", "atom_string", "atomic", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term",
    "downcase_atom", "float", "functor", "ground", "integer", "is", "is_list", "msort", "name", "nb_getval", "nb_setval", "nonvar", "number", "number_chars", "number_codes", "number_string", "numbervars", "plus", "print", "retract",
    "retractall", "sort", "string_chars", "string_codes", "string_concat", "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "term_string", "term_to_atom", "throw",
    "upcase_atom", "var", "write_canonical", "writeln", "writeq", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_builtin_exec(const char * s) { if (!s) return 0; if (is_builtin_visible(s)) return 1; for (int i = 0; g_pl_nl_builtins[i]; i++) if (!strcmp(s, g_pl_nl_builtins[i])) return 1; return 0; }
static void pl_ensure_call_bridge(int nparams);
static void pl_ensure_gen_builtin_pred(const char *gen_sval, const char *pred_nm, int nparams);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_synth_qlit(const char * s) { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) s; return q; }
static tree_t * pl_synth_fnc1(const char * f, const tree_t * a) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, (tree_t *) a); return n; }
static tree_t * pl_synth_fnc2(const char * f, const tree_t * a, const tree_t * b) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, (tree_t *) a); ast_push(n, (tree_t *) b); return n; }
static int is_builtin_argw(const char * s) { return s && (!strcmp(s, "is") || !strcmp(s, "<") || !strcmp(s, ">") || !strcmp(s, "=<") || !strcmp(s, ">=") || !strcmp(s, "=:=") || !strcmp(s, "=\\=")); }
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL_PROLOG, NULL, cx->tω); IR_LIT(nd).sval = "$mkc";
    IR_t * fn = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(fn).sval = strdup(fname);
    ir_operand_push(nd, fn);
    IR_t * prev = fn; IR_t * first = fn;
    for (int i = 0; i < nkids; i++) {
        IR_t * ke = kid_entries[i] ? kid_entries[i] : kids[i];
        lc_γ_to(prev, ke); if (cx->tω) lc_ω_to(kids[i], cx->tω);
        prev = kids[i];
        ir_operand_push(nd, kids[i]);
    }
    lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first;
    return nd;
}
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return NULL;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_VAR, NULL, cx->tω); IR_LIT(nd).sval = pl_var_name((int) t->v.ival); return nd; }
    case TT_MAKELIST: {
        int bar = (t->v.ival == 1 && t->n > 0);
        IR_t * prev; IR_t * prev_e = NULL;
        if (bar) prev = term_lval_e(cx, t->c[t->n - 1], &prev_e);
        else { prev = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(prev).sval = "[]"; }
        for (int i = (bar ? t->n - 2 : t->n - 1); i >= 0; i--) {
            IR_t * ee = NULL; IR_t * e = term_lval_e(cx, t->c[i], &ee);
            IR_t * kids[2] = { e, prev }; IR_t * kes[2] = { ee, prev_e };
            prev = mkc_node(cx, ".", 2, kids, kes, &prev_e);
        }
        if (entry_out) *entry_out = prev_e;
        return prev;
    }
    case TT_FNC: {
        int nk = t->n;
        if (nk == 0) { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : "?"; return nd; }
        IR_t ** kids = (IR_t **) calloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
        IR_t ** kes  = (IR_t **) calloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
        for (int i = 0; i < nk; i++) kids[i] = term_lval_e(cx, t->c[i], &kes[i]);
        IR_t * nd = mkc_node(cx, t->v.sval ? t->v.sval : "?", nk, kids, kes, entry_out);
        free(kids); free(kes);
        return nd;
    }
    default: { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t) { return term_e(cx, t, NULL); }
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (t && t->t == TT_VAR) { IR_t * nd = build(cx, IR_VAR_REF, NULL, cx->tω); IR_LIT(nd).sval = pl_var_name((int) t->v.ival); return nd; }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval(lcx_t * cx, const tree_t * t) { return term_lval_e(cx, t, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_same_functor(const tree_t * a, const tree_t * b) {
    return a && b && a->t == TT_FNC && b->t == TT_FNC && a->n == b->n && a->v.sval && b->v.sval && !strcmp(a->v.sval, b->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_no_ul(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_NO_UL"); p = (e && e[0] == '1') ? 1 : 0; } return p; }
static int pl_is_lstpat1(const tree_t * t) { return t && t->t == TT_MAKELIST && t->v.ival == 1 && t->n == 2; }
static int pl_no_ix(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_NO_IX"); p = (e && e[0] == '1') ? 1 : 0; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ix_key(const tree_t * h, long long * ki, const char ** ks) {
    if (!h) return 0;
    if (h->t == TT_ILIT) { *ki = (long long) h->v.ival; return 1; }
    if (h->t == TT_MAKELIST) { if (h->n == 0) { *ks = "[]"; return 2; } return 3; }
    if (h->t == TT_FNC && h->v.sval) { if (h->n == 0) { *ks = h->v.sval; return 2; } *ks = h->v.sval; *ki = (long long) h->n; return 4; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * unify_lst_build(lcx_t * cx, IR_t * subj, IR_t * subj_entry, const tree_t * eh, const tree_t * et, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL_PROLOG, γ, ω); IR_LIT(nd).sval = "$unify_lst";
    IR_t * e1 = NULL; IR_t * a1 = term_lval_e(cx, eh, &e1);
    IR_t * e2 = NULL; IR_t * a2 = term_lval_e(cx, et, &e2);
    lc_γ_to(subj, e1 ? e1 : a1); lc_ω_to(subj, ω);
    lc_γ_to(a1, e2 ? e2 : a2); lc_ω_to(a1, ω);
    lc_γ_to(a2, nd); lc_ω_to(a2, ω);
    ir_operand_push(nd, subj); ir_operand_push(nd, a1); ir_operand_push(nd, a2);
    if (entry_out) *entry_out = subj_entry ? subj_entry : subj;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    if (pl_same_functor(lt, rt)) {
        IR_t * next = γ; IR_t * first_entry = γ; IR_t * head = NULL;
        for (int i = lt->n - 1; i >= 0; i--) { IR_t * e = NULL; IR_t * u = unify_pair(cx, lt->c[i], rt->c[i], next, ω, &e); next = e ? e : u; head = u; if (i == 0) first_entry = next; }
        if (entry_out) *entry_out = first_entry;
        return head;
    }
    if (!pl_no_ul() && pl_is_lstpat1(rt) && !pl_is_lstpat1(lt)) { IR_t * e0 = NULL; IR_t * a0 = term_lval_e(cx, lt, &e0); return unify_lst_build(cx, a0, e0, rt->c[0], rt->c[1], γ, ω, entry_out); }
    if (!pl_no_ul() && pl_is_lstpat1(lt) && !pl_is_lstpat1(rt)) { IR_t * e0 = NULL; IR_t * a0 = term_lval_e(cx, rt, &e0); return unify_lst_build(cx, a0, e0, lt->c[0], lt->c[1], γ, ω, entry_out); }
    IR_t * nd = build(cx, IR_CALL_PROLOG, γ, ω); IR_LIT(nd).sval = "$unify";
    IR_t * e0 = NULL; IR_t * e1 = NULL;
    IR_t * a0 = term_lval_e(cx, lt, &e0); IR_t * a1 = term_lval_e(cx, rt, &e1);
    lc_γ_to(a0, e1 ? e1 : a1); lc_ω_to(a0, ω);
    lc_γ_to(a1, nd); lc_ω_to(a1, ω);
    ir_operand_push(nd, a0); ir_operand_push(nd, a1);
    if (entry_out) *entry_out = e0 ? e0 : a0;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_conj(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",")) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    if (t->t == TT_PROGRAM) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    lc_vec_push(out, &t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * thread_goals(lcx_t * cx, const tree_t * blk, int from, int to, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t * conj_owner) {
    lc_vec glv; lc_vec_init(&glv, (int) sizeof(const tree_t *));
    for (int i = from; i < to; i++) collect_conj(blk->c[i], &glv);
    const tree_t ** gl = (const tree_t **) glv.data; int ng = glv.n;
    int cut_idx = -1;
    if (cx->cut_ω) for (int i = 0; i < ng; i++) if (gl[i] && gl[i]->t == TT_CUT) { cut_idx = i; break; }
    IR_t ** gn = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** en = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** rz = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** fz = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t * next = γtail;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        cx->beta = NULL;
        cx->ite_funnel = NULL;
        IR_t * ωeff = (cut_idx >= 0 && i > cut_idx) ? cx->cut_ω : ωbase;
        IR_t * nd = goal(cx, gl[i], next, ωeff, &e);
        gn[i] = nd; en[i] = e ? e : nd; rz[i] = cx->beta; fz[i] = cx->ite_funnel;
        next = en[i];
    }
    IR_t * last_res = ωbase; int last_res_beta = 0;
    for (int i = 0; i < ng; i++) {
        if (cut_idx >= 0 && i == cut_idx + 1) { last_res = cx->cut_ω; last_res_beta = 0; }
        if (gn[i] && gn[i]->op == IR_GOTO) { if (last_res_beta) lc_γ_to_β(gn[i], last_res); else γ_to(gn[i], last_res); }
        else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else ω_to(gn[i], last_res);
        if (fz[i]) { if (last_res_beta) lc_γ_to_β(fz[i], last_res); else γ_to(fz[i], last_res); }
        if (rz[i]) { last_res = rz[i]; last_res_beta = (rz[i]->op == IR_CALL || rz[i]->op == IR_CALL_PROC_STAGED); }
    }
    cx->beta = last_res_beta ? last_res : NULL;
    free(rz); free(fz);
    if (entry_out) *entry_out = (ng > 0) ? en[0] : γtail;
    IR_t * first = (ng > 0) ? gn[0] : NULL;
    if (conj_owner) {
        bb_conj_state_t * z = (bb_conj_state_t *) calloc(1, sizeof *z);
        z->goals = gn; z->ngoals = ng;
        IR_LIT(conj_owner).ival = (long long)(intptr_t) z;
    } else free(gn);
    free(en);
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * thread1(lcx_t * cx, const tree_t * gt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    const tree_t * one[1] = { gt };
    tree_t w; memset(&w, 0, sizeof w); w.n = 1; w.c = (tree_t **)(intptr_t) one;
    return thread_goals(cx, &w, 0, 1, γ, ω, entry_out, NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_arith_op_suffix(const char * s) {
    if (!s) return NULL;
    if (!strcmp(s, "+")) return "add"; if (!strcmp(s, "-")) return "sub"; if (!strcmp(s, "*")) return "mul";
    if (!strcmp(s, "/")) return "div"; if (!strcmp(s, "//")) return "idiv"; if (!strcmp(s, "div")) return "idiv";
    if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "**")) return "fpow"; if (!strcmp(s, "^")) return "pow";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_cmp_op_suffix(const char * s) {
    if (!s) return NULL;
    if (!strcmp(s, "<")) return "lt"; if (!strcmp(s, ">")) return "gt"; if (!strcmp(s, "=<")) return "le";
    if (!strcmp(s, ">=")) return "ge"; if (!strcmp(s, "=:=")) return "eq"; if (!strcmp(s, "=\\=")) return "ne";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_ax_suffix(const char * s, int ar) {
    if (!s) return NULL;
    if (ar == 2) {
        if (!strcmp(s, "+")) return "add"; if (!strcmp(s, "-")) return "sub"; if (!strcmp(s, "*")) return "mul";
        if (!strcmp(s, "/")) return "div"; if (!strcmp(s, "//")) return "idiv"; if (!strcmp(s, "div")) return "idiv";
        if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "rem")) return "rem"; if (!strcmp(s, "**")) return "fpow"; if (!strcmp(s, "^")) return "pow";
        if (!strcmp(s, "min")) return "min"; if (!strcmp(s, "max")) return "max"; if (!strcmp(s, "gcd")) return "gcd"; if (!strcmp(s, "xor")) return "xor";
        if (!strcmp(s, ">>")) return "shr"; if (!strcmp(s, "<<")) return "shl"; if (!strcmp(s, "/\\")) return "band"; if (!strcmp(s, "\\/")) return "bor";
        return NULL;
    }
    if (ar == 1) {
        if (!strcmp(s, "-")) return "neg"; if (!strcmp(s, "+")) return "pos"; if (!strcmp(s, "abs")) return "abs"; if (!strcmp(s, "sign")) return "sign";
        if (!strcmp(s, "truncate")) return "trunc"; if (!strcmp(s, "integer")) return "intg"; if (!strcmp(s, "float")) return "flt";
        if (!strcmp(s, "floor")) return "floor"; if (!strcmp(s, "ceiling")) return "ceil"; if (!strcmp(s, "round")) return "round";
        if (!strcmp(s, "sqrt")) return "sqrt"; if (!strcmp(s, "msb")) return "msb"; if (!strcmp(s, "\\")) return "bnot"; if (!strcmp(s, "sin")) return "sin"; if (!strcmp(s, "cos")) return "cos";
        if (!strcmp(s, "atan")) return "atan"; if (!strcmp(s, "log")) return "log"; if (!strcmp(s, "exp")) return "exp";
        if (!strcmp(s, "float_integer_part")) return "fip"; if (!strcmp(s, "float_fractional_part")) return "ffp";
        return NULL;
    }
    if (ar == 0) { if (!strcmp(s, "pi")) return "pi"; return NULL; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_arith_val(lcx_t * cx, const tree_t * t, IR_t * ωfail, IR_t ** entry_out) {
    if (t && t->t == TT_QLIT && t->v.sval && pl_ax_suffix(t->v.sval, 0)) {
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, 0));
        IR_t * nd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
        if (entry_out) *entry_out = nd;
        return nd;
    }
    if (t && t->t == TT_FNC && t->v.sval && (t->n == 1 || t->n == 2) && pl_ax_suffix(t->v.sval, t->n)) {
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, t->n));
        IR_t * nd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
        IR_t * prev = NULL; IR_t * first = NULL;
        for (int i = 0; i < t->n; i++) {
            IR_t * ke = NULL; IR_t * k = lower_arith_val(cx, t->c[i], ωfail, &ke); IR_t * en = ke ? ke : k;
            if (prev) lc_γ_to(prev, en); else first = en;
            prev = k; ir_operand_push(nd, k);
        }
        if (prev) lc_γ_to(prev, nd);
        if (entry_out) *entry_out = first ? first : nd;
        return nd;
    }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_ite(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * te = NULL; IR_t * ee = NULL; IR_t * ce = NULL;
    IR_t * F = build(cx, IR_GOTO, ωfail, ωfail);
    IR_t * tn = thread1(cx, T, γnext, F, &te);
    IR_t * en = E ? thread1(cx, E, γnext, F, &ee) : NULL;
    IR_t * cω = E ? (ee ? ee : en) : F;
    // A thrown exception reaches the condition's ω edge exactly like an ordinary failure (row
    // prolog-exceptions-uncaught-propagation) -- this guard is the only thing between "condition failed,
    // try the else branch" and "condition threw, unwind past the else branch too", so it sits ON the edge,
    // never folded into cω itself: pending -> ωfail (skip else); not pending -> cω (else, unchanged).
    IR_t * G = build(cx, IR_CALL_PROLOG, cω, ωfail); IR_LIT(G).sval = "$no_throw_or_fail";
    IR_t * cn = thread1(cx, C, (te ? te : tn), G, &ce);
    if (entry_out) *entry_out = ce ? ce : cn;
    cx->beta = NULL;
    cx->ite_funnel = F;
    return E ? (en ? en : cn) : cn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return build(cx, IR_SUCCEED, γnext, ωfail);
    switch (t->t) {
    case TT_FNC: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, ",") && t->n == 2) {
            const tree_t * flat[64]; int nf = 0;
            const tree_t * stk[64]; int sp = 0; stk[sp++] = t;
            while (sp > 0) {
                const tree_t * cur = stk[--sp];
                if (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ",") && cur->n == 2) { stk[sp++] = cur->c[1]; stk[sp++] = cur->c[0]; }
                else if (nf < 64) flat[nf++] = cur;
            }
            tree_t blkw; memset(&blkw, 0, sizeof blkw); blkw.n = nf; blkw.c = (tree_t **) flat;
            return thread_goals(cx, &blkw, 0, nf, γnext, ωfail, entry_out, NULL);
        }
        if (!strcmp(nm, ";") && t->n == 2 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "->") && t->c[0]->n == 2)
            return lower_ite(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
        if (!strcmp(nm, "->") && t->n == 2)
            return lower_ite(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out);
        if (!strcmp(nm, ";") && t->n >= 2) {
            IR_t * nbf = ωfail;
            IR_t * bentry = NULL;
            IR_t * bn = NULL;
            for (int b = t->n - 1; b >= 0; b--) {
                const tree_t * br = t->c[b];
                IR_t * e = NULL;
                if (br && br->t == TT_FNC && br->v.sval && !strcmp(br->v.sval, ",")) {
                    IR_t * first = thread_goals(cx, br, 0, br->n, γnext, nbf, &e, NULL);
                    if (b == t->n - 1) bn = first;
                } else {
                    IR_t * g0 = goal(cx, br, γnext, nbf, &e);
                    if (b == t->n - 1) bn = g0;
                    if (!e) e = g0;
                }
                bentry = e;
                nbf = bentry;
            }
            if (entry_out) *entry_out = bentry;
            cx->beta = NULL;
            return bn;
        }
        if (!strcmp(nm, "=") && t->n == 2) {
            IR_t * e = NULL;
            IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e);
            if (entry_out) *entry_out = e ? e : nd;
            return nd;
        }
        if (!strcmp(nm, "\\=") && t->n == 2) {
            return lower_ite(cx, pl_synth_fnc2("=", t->c[0], t->c[1]), pl_synth_qlit("fail"), pl_synth_qlit("true"), γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "catch") && t->n == 3) {
            IR_t * ce = NULL; IR_t * cnode = term_lval_e(cx, t->c[1], &ce);
            IR_t * marknd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(marknd).sval = "$trail_mark";
            IR_t * checknd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(checknd).sval = "$catch_check";
            ir_operand_push(checknd, marknd);
            ir_operand_push(checknd, cnode);
            IR_t * rentry = NULL;
            IR_t * rnode = thread1(cx, t->c[2], γnext, checknd, &rentry);
            cx->beta = NULL;
            lc_γ_to(checknd, rentry ? rentry : rnode);
            IR_t * gentry = NULL;
            IR_t * gnode = thread1(cx, t->c[0], γnext, checknd, &gentry);
            cx->beta = NULL;
            lc_γ_to(cnode, marknd);
            lc_γ_to(marknd, gentry ? gentry : gnode);
            lc_ω_to(cnode, ωfail);
            if (entry_out) *entry_out = ce ? ce : cnode;
            return checknd;
        }
        if (!strcmp(nm, "call") && t->n >= 1 && t->n <= 8) {
            pl_ensure_call_bridge(t->n);
            IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name("$call", t->n);
            IR_t * prev = NULL; IR_t * first = NULL;
            for (int i = 0; i < t->n; i++) {
                IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
                if (prev) lc_γ_to(prev, en); else first = en;
                lc_ω_to(a, ωfail);
                prev = a; ir_operand_push(nd, a);
            }
            if (prev) lc_γ_to(prev, nd);
            if (entry_out) *entry_out = first ? first : nd;
            cx->beta = nd;
            return nd;
        }
        if (!strcmp(nm, "sub_atom") && t->n == 5) pl_ensure_gen_builtin_pred("$sub_atom", "sub_atom", 5);
        if (!strcmp(nm, "between") && t->n == 3) pl_ensure_gen_builtin_pred("$between", "between", 3);
        if (!strcmp(nm, "for") && t->n == 3) pl_ensure_gen_builtin_pred("$for", "for", 3);
        if (!strcmp(nm, "$bag_group") && t->n == 3) pl_ensure_gen_builtin_pred("$bag_group", "$bag_group", 3);
        if (!strcmp(nm, "clause") && t->n == 2) pl_ensure_gen_builtin_pred("$clause", "clause", 2);
        if (!strcmp(nm, "current_predicate") && t->n == 1) pl_ensure_gen_builtin_pred("$current_predicate", "current_predicate", 1);
        if (!strcmp(nm, "predicate_property") && t->n == 2) pl_ensure_gen_builtin_pred("$predicate_property", "predicate_property", 2);
        if (!strcmp(nm, "current_op") && t->n == 3) pl_ensure_gen_builtin_pred("$current_op", "current_op", 3);
        if (!strcmp(nm, "current_prolog_flag") && t->n == 2) pl_ensure_gen_builtin_pred("$current_prolog_flag", "current_prolog_flag", 2);
        if (!strcmp(nm, "current_stream") && t->n == 1) pl_ensure_gen_builtin_pred("$current_stream", "current_stream", 1);
        if (!strcmp(nm, "stream_property") && t->n == 2) pl_ensure_gen_builtin_pred("$stream_property", "stream_property", 2);
        if ((!strcmp(nm, "\\+") || !strcmp(nm, "not")) && t->n == 1) return lower_ite(cx, t->c[0], pl_synth_qlit("fail"), pl_synth_qlit("true"), γnext, ωfail, entry_out);
        if (!strcmp(nm, "once") && t->n == 1) return lower_ite(cx, t->c[0], pl_synth_qlit("true"), NULL, γnext, ωfail, entry_out);
        if (!strcmp(nm, "ignore") && t->n == 1) return lower_ite(cx, t->c[0], pl_synth_qlit("true"), pl_synth_qlit("true"), γnext, ωfail, entry_out);
        if (!strcmp(nm, "with_output_to") && t->n == 2) {
            tree_t * arm = pl_synth_fnc2(";", pl_synth_fnc2("->", pl_synth_fnc1("once", t->c[1]), pl_synth_fnc1("$wot_end", t->c[0])), pl_synth_fnc2(",", pl_synth_qlit("$wot_abort"), pl_synth_qlit("fail")));
            tree_t * whole = pl_synth_fnc2(",", pl_synth_qlit("$wot_begin"), arm);
            return goal(cx, whole, γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "forall") && t->n == 2) {
            tree_t * inner = pl_synth_fnc2(",", t->c[0], pl_synth_fnc1("\\+", t->c[1]));
            return goal(cx, pl_synth_fnc1("\\+", inner), γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "setup_call_cleanup") && t->n == 3) {
            tree_t * ite = pl_synth_fnc2(";", pl_synth_fnc2("->", t->c[1], pl_synth_fnc1("once", t->c[2])), pl_synth_fnc2(",", pl_synth_fnc1("once", t->c[2]), pl_synth_qlit("fail")));
            tree_t * whole = pl_synth_fnc2(",", pl_synth_fnc1("once", t->c[0]), ite);
            return goal(cx, whole, γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "aggregate_all") && t->n == 3) {
            const tree_t * spec = t->c[0]; const tree_t * goal_ast = t->c[1]; const tree_t * res_ast = t->c[2];
            const char * op = NULL; const tree_t * tmpl_ast = NULL;
            if (spec && spec->t == TT_QLIT && spec->v.sval && !strcmp(spec->v.sval, "count")) { op = "$agg_count"; }
            else if (spec && spec->t == TT_FNC && spec->n == 1 && spec->v.sval) {
                if (!strcmp(spec->v.sval, "sum")) op = "$agg_sum";
                else if (!strcmp(spec->v.sval, "max")) op = "$agg_max";
                else if (!strcmp(spec->v.sval, "min")) op = "$agg_min";
                tmpl_ast = spec->c[0];
            }
            if (op) {
                IR_t * hnew = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(hnew).sval = "$findall_new";
                IR_t * res = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(res).sval = op;
                IR_t * res_e = NULL; IR_t * res_lv = term_lval_e(cx, res_ast, &res_e);
                lc_γ_to(res_lv, res); lc_ω_to(res_lv, ωfail);
                ir_operand_push(res, hnew); ir_operand_push(res, res_lv);
                IR_t * addnd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(addnd).sval = "$findall_add";
                IR_t * tmpl_e = NULL; IR_t * tmpl_val;
                if (tmpl_ast) tmpl_val = term_e(cx, tmpl_ast, &tmpl_e);
                else { tmpl_val = build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(tmpl_val).ival = 1; }
                IR_t * tmpl_entry = tmpl_e ? tmpl_e : tmpl_val;
                lc_γ_to(tmpl_val, addnd); lc_ω_to(tmpl_val, ωfail);
                ir_operand_push(addnd, hnew); ir_operand_push(addnd, tmpl_val);
                cx->beta = NULL;
                IR_t * goal_entry = NULL;
                IR_t * goal_nd = thread1(cx, goal_ast, tmpl_entry, res_e ? res_e : res_lv, &goal_entry);
                IR_t * goal_beta = cx->beta;
                IR_t * forced_fail;
                if (goal_beta) {
                    forced_fail = build(cx, IR_GOTO, NULL, NULL);
                    if (goal_beta->op == IR_CALL || goal_beta->op == IR_CALL_PROC_STAGED) { lc_γ_to_β(forced_fail, goal_beta); lc_ω_to_β(forced_fail, goal_beta); }
                    else { lc_γ_to(forced_fail, goal_beta); lc_ω_to(forced_fail, goal_beta); }
                } else forced_fail = build(cx, IR_GOTO, res_e ? res_e : res_lv, res_e ? res_e : res_lv);
                lc_γ_to(addnd, forced_fail);
                lc_γ_to(hnew, goal_entry ? goal_entry : goal_nd);
                if (entry_out) *entry_out = hnew;
                cx->beta = NULL;
                return res;
            }
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm;
            return nd;
        }
        if (!strcmp(nm, "write") && t->n == 1) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$write";
            IR_t * ae = NULL; IR_t * a = term_e(cx, t->c[0], &ae);
            lc_γ_to(a, call); lc_ω_to(a, ωfail);
            ir_operand_push(call, a);
            if (entry_out) *entry_out = ae ? ae : a;
            return call;
        }
        if (!strcmp(nm, "writeln") && t->n == 1) {
            return goal(cx, pl_synth_fnc2(",", pl_synth_fnc1("write", t->c[0]), pl_synth_qlit("nl")), γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "writeln") && t->n == 2) {
            return goal(cx, pl_synth_fnc2(",", pl_synth_fnc2("write", t->c[0], t->c[1]), pl_synth_fnc1("nl", t->c[0])), γnext, ωfail, entry_out);
        }
        if (!strcmp(nm, "write") && t->n == 2) {
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$write2";
            IR_t * se = NULL; IR_t * s = term_e(cx, t->c[0], &se);
            IR_t * ae = NULL; IR_t * a = term_e(cx, t->c[1], &ae);
            lc_γ_to(s, ae ? ae : a); lc_ω_to(s, ωfail); lc_γ_to(a, nd); lc_ω_to(a, ωfail);
            ir_operand_push(nd, s); ir_operand_push(nd, a);
            if (entry_out) *entry_out = se ? se : s;
            return nd;
        }
        if (!strcmp(nm, "nl") && t->n == 1) {
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$nl1";
            IR_t * se = NULL; IR_t * s = term_e(cx, t->c[0], &se);
            lc_γ_to(s, nd); lc_ω_to(s, ωfail);
            ir_operand_push(nd, s);
            if (entry_out) *entry_out = se ? se : s;
            return nd;
        }
        if (!strcmp(nm, "put_char") && t->n == 1) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$put_char";
            IR_t * ae = NULL; IR_t * a = term_e(cx, t->c[0], &ae);
            lc_γ_to(a, call); lc_ω_to(a, ωfail);
            ir_operand_push(call, a);
            if (entry_out) *entry_out = ae ? ae : a;
            return call;
        }
        if (!strcmp(nm, "tab") && t->n == 1) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$tab";
            IR_t * ve = NULL; IR_t * v = lower_arith_val(cx, t->c[0], ωfail, &ve);
            lc_γ_to(v, call); lc_ω_to(v, ωfail);
            ir_operand_push(call, v);
            if (entry_out) *entry_out = ve ? ve : v;
            return call;
        }
        if (!strcmp(nm, "is") && t->n == 2) {
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$is_v";
            IR_t * xl = term_lval(cx, t->c[0]);
            IR_t * ve = NULL; IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
            lc_γ_to(xl, ve ? ve : v); lc_ω_to(xl, ωfail); lc_γ_to(v, nd); lc_ω_to(v, ωfail);
            ir_operand_push(nd, xl); ir_operand_push(nd, v);
            if (entry_out) *entry_out = xl;
            return nd;
        }
        { const char * csuf = (t->n == 2) ? pl_cmp_op_suffix(nm) : NULL;
          if (csuf) {
            char nb[16]; snprintf(nb, sizeof nb, "$cmp_%s", csuf);
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = strdup(nb);
            IR_t * ea = NULL; IR_t * eb = NULL;
            IR_t * a = lower_arith_val(cx, t->c[0], ωfail, &ea); IR_t * b = lower_arith_val(cx, t->c[1], ωfail, &eb);
            lc_γ_to(a, eb ? eb : b); lc_ω_to(a, ωfail); lc_γ_to(b, nd); lc_ω_to(b, ωfail);
            ir_operand_push(nd, a); ir_operand_push(nd, b);
            if (entry_out) *entry_out = ea ? ea : a;
            return nd;
          } }
        if ((!strcmp(nm, "succ") && t->n == 2) || (!strcmp(nm, "plus") && t->n == 3)
            || (!strcmp(nm, "atom_length") && t->n == 2) || (!strcmp(nm, "upcase_atom") && t->n == 2) || (!strcmp(nm, "downcase_atom") && t->n == 2)
            || (!strcmp(nm, "atom_concat") && t->n == 3) || (!strcmp(nm, "atom_chars") && t->n == 2) || (!strcmp(nm, "atom_codes") && t->n == 2)) {
            char nb[24]; snprintf(nb, sizeof nb, "$%s", nm);
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = strdup(nb);
            IR_t * prev = NULL; IR_t * first = NULL;
            for (int i = 0; i < t->n; i++) {
                IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
                if (prev) lc_γ_to(prev, en); else first = en;
                lc_ω_to(a, ωfail);
                prev = a; ir_operand_push(nd, a);
            }
            if (prev) lc_γ_to(prev, nd);
            if (entry_out) *entry_out = first ? first : nd;
            return nd;
        }
        if (!strcmp(nm, "findall") && t->n == 3) {
            const tree_t * tmpl_ast = t->c[0]; const tree_t * goal_ast = t->c[1]; const tree_t * lst_ast = t->c[2];
            IR_t * hnew = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(hnew).sval = "$findall_new";
            IR_t * res = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(res).sval = "$findall_result";
            ir_operand_push(res, hnew);
            IR_t * bindnd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(bindnd).sval = "$unify";
            IR_t * lst_e = NULL; IR_t * lst_lv = term_lval_e(cx, lst_ast, &lst_e);
            lc_γ_to(res, lst_e ? lst_e : lst_lv); lc_ω_to(lst_lv, ωfail);
            lc_γ_to(lst_lv, bindnd); lc_ω_to(lst_lv, ωfail);
            ir_operand_push(bindnd, lst_lv); ir_operand_push(bindnd, res);
            IR_t * addnd = build(cx, IR_CALL_PROLOG, NULL, ωfail); IR_LIT(addnd).sval = "$findall_add";
            IR_t * tmpl_e = NULL; IR_t * tmpl_val = term_e(cx, tmpl_ast, &tmpl_e);
            IR_t * tmpl_entry = tmpl_e ? tmpl_e : tmpl_val;
            lc_γ_to(tmpl_val, addnd); lc_ω_to(tmpl_val, ωfail);
            ir_operand_push(addnd, hnew); ir_operand_push(addnd, tmpl_val);
            cx->beta = NULL;
            IR_t * goal_entry = NULL;
            IR_t * goal_nd = thread1(cx, goal_ast, tmpl_entry, res, &goal_entry);
            IR_t * goal_beta = cx->beta;
            IR_t * forced_fail;
            if (goal_beta) {
                forced_fail = build(cx, IR_GOTO, NULL, NULL);
                if (goal_beta->op == IR_CALL || goal_beta->op == IR_CALL_PROC_STAGED) { lc_γ_to_β(forced_fail, goal_beta); lc_ω_to_β(forced_fail, goal_beta); }
                else { lc_γ_to(forced_fail, goal_beta); lc_ω_to(forced_fail, goal_beta); }
            } else forced_fail = build(cx, IR_GOTO, res, res);
            lc_γ_to(addnd, forced_fail);
            lc_γ_to(hnew, goal_entry ? goal_entry : goal_nd);
            if (entry_out) *entry_out = hnew;
            cx->beta = NULL;
            return hnew;
        }
        if (!strcmp(nm, "abolish") && t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "/") && t->c[0]->n == 2) {
            IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$abolish";
            IR_t * prev = NULL; IR_t * first = NULL;
            for (int i = 0; i < 2; i++) {
                IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[0]->c[i], &ae); IR_t * en = ae ? ae : a;
                if (prev) lc_γ_to(prev, en); else first = en;
                lc_ω_to(a, ωfail);
                prev = a; ir_operand_push(nd, a);
            }
            if (prev) lc_γ_to(prev, nd);
            if (entry_out) *entry_out = first ? first : nd;
            return nd;
        }
        { extern const char * rt_pl_det_builtin_target(const char * nm2, int ar2);
          const char * det_tgt = rt_pl_det_builtin_target(nm, t->n);
          if (det_tgt) {
              IR_t * nd = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(nd).sval = det_tgt;
              IR_t * prev = NULL; IR_t * first = NULL;
              for (int i = 0; i < t->n; i++) {
                  IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
                  if (prev) lc_γ_to(prev, en); else first = en;
                  lc_ω_to(a, ωfail);
                  prev = a; ir_operand_push(nd, a);
              }
              if (prev) lc_γ_to(prev, nd);
              if (entry_out) *entry_out = first ? first : nd;
              return nd;
          } }
        if (is_builtin_exec(nm)) return pl_existence_err(cx, nm, t->n, γnext, ωfail, entry_out);
        if (!strcmp(nm, "phrase") && (t->n == 2 || t->n == 3)) {
            const tree_t * gt = t->c[0];
            const char * callee = (gt && gt->v.sval) ? gt->v.sval : "?";
            int base_n = (gt && gt->t == TT_FNC) ? gt->n : 0;
            IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name(callee, base_n + 2);
            IR_t * prev = NULL; IR_t * first = NULL;
            for (int i = 0; i < base_n + 2; i++) {
                const tree_t * at = (i < base_n) ? gt->c[i] : (i == base_n) ? t->c[1] : (t->n == 3 ? t->c[2] : (const tree_t *)0);
                IR_t * ae = NULL; IR_t * a;
                if (at) a = term_lval_e(cx, at, &ae);
                else { a = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(a).sval = "[]"; }
                IR_t * en = ae ? ae : a;
                if (prev) lc_γ_to(prev, en); else first = en;
                lc_ω_to(a, ωfail);
                prev = a; ir_operand_push(nd, a);
            }
            if (prev) lc_γ_to(prev, nd);
            if (entry_out) *entry_out = first ? first : nd;
            cx->beta = nd;
            return nd;
        }
        IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name(nm, t->n);
        IR_t * prev = NULL; IR_t * first = NULL;
        for (int i = 0; i < t->n; i++) {
            IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
            if (prev) lc_γ_to(prev, en); else first = en;
            lc_ω_to(a, ωfail);
            prev = a; ir_operand_push(nd, a);
        }
        if (prev) lc_γ_to(prev, nd);
        if (entry_out) *entry_out = first ? first : nd;
        cx->beta = nd;
        return nd;
    }
    case TT_QLIT: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, "true"))  return build(cx, IR_SUCCEED, γnext, ωfail);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_GOTO, ωfail, ωfail);
        if (!strcmp(nm, "nl") && t->n == 0) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$nl0";
            IR_t * a = build(cx, IR_LIT_STRING, call, ωfail); IR_LIT(a).sval = "";
            ir_operand_push(call, a);
            if (entry_out) *entry_out = a;
            return call;
        }
        if (!strcmp(nm, "flush_output") && t->n == 0) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$flush_output";
            IR_t * a = build(cx, IR_LIT_STRING, call, ωfail); IR_LIT(a).sval = "";
            ir_operand_push(call, a);
            if (entry_out) *entry_out = a;
            return call;
        }
        { extern const char * rt_pl_det_builtin_target(const char * nm2, int ar2);
          const char * det_tgt0 = rt_pl_det_builtin_target(nm, 0);
          if (det_tgt0) {
              IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = det_tgt0;
              if (entry_out) *entry_out = call;
              return call;
          } }
        if (is_builtin_exec(nm)) return pl_existence_err(cx, nm, 0, γnext, ωfail, entry_out);
        if (!strcmp(nm, "$wot_begin") || !strcmp(nm, "$wot_abort")) {
            IR_t * call = build(cx, IR_CALL_PROLOG, γnext, ωfail); IR_LIT(call).sval = nm;
            IR_t * a = build(cx, IR_LIT_STRING, call, ωfail); IR_LIT(a).sval = "";
            ir_operand_push(call, a);
            if (entry_out) *entry_out = a;
            return call;
        }
        IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name(nm, 0);
        if (entry_out) *entry_out = nd;
        cx->beta = nd;
        return nd;
    }
    case TT_IF:
        return lower_ite(cx, (t->n > 0) ? t->c[0] : NULL, (t->n > 1) ? t->c[1] : NULL, (t->n > 2) ? t->c[2] : NULL, γnext, ωfail, entry_out);
    case TT_UNIFY: {
        IR_t * e = NULL;
        IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e);
        if (entry_out) *entry_out = e ? e : nd;
        return nd;
    }
    case TT_CUT: return build(cx, IR_CUT, γnext, ωfail);
    case TT_PROGRAM:
        return thread_goals(cx, t, 0, t->n, γnext, ωfail, entry_out, NULL);
    case TT_VAR: {
        pl_ensure_call_bridge(1);
        IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name("$call", 1);
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t, &ae);
        lc_γ_to(a, nd); lc_ω_to(a, ωfail);
        ir_operand_push(nd, a);
        if (entry_out) *entry_out = ae ? ae : a;
        cx->beta = nd;
        return nd;
    }
    default: return build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int max_var_slot(const tree_t * t, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot(t->c[i], mx);
    return mx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_param_name(int i) {
    static char * cache[64]; static char buf[16];
    if (i >= 0 && i < 64) { if (!cache[i]) { snprintf(buf, sizeof buf, "A%d", i); cache[i] = strdup(buf); } return cache[i]; }
    snprintf(buf, sizeof buf, "A%d", i); return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_pl_clause_into(lcx_t * cx, const tree_t * clause, int arity, IR_t * γsucc, IR_t * ωfail, IR_t ** centry_out, IR_t ** redo_out) {
    IR_t * bentry = NULL;
    cx->beta = NULL;
    thread_goals(cx, clause, arity, clause->n, γsucc, ωfail, &bentry, NULL);
    if (redo_out) *redo_out = cx->beta;
    IR_t * next = bentry ? bentry : γsucc;
    for (int i = arity - 1; i >= 0; i--) {
        const tree_t * h = clause->c[i];
        if (!pl_no_ul() && pl_is_lstpat1(h)) {
            IR_t * lhs = build(cx, IR_VAR_REF, NULL, NULL); IR_LIT(lhs).sval = pl_param_name(i);
            unify_lst_build(cx, lhs, NULL, h->c[0], h->c[1], next, ωfail, NULL);
            next = lhs; continue;
        }
        IR_t * u = build(cx, IR_CALL_PROLOG, next, ωfail); IR_LIT(u).sval = "$unify";
        IR_t * lhs = build(cx, IR_VAR_REF, NULL, NULL); IR_LIT(lhs).sval = pl_param_name(i);
        IR_t * he = NULL; IR_t * rhs = term_lval_e(cx, h, &he);
        lc_γ_to(lhs, he ? he : rhs); lc_ω_to(lhs, ωfail);
        lc_γ_to(rhs, u); lc_ω_to(rhs, ωfail);
        ir_operand_push(u, lhs); ir_operand_push(u, rhs);
        next = lhs;
    }
    if (centry_out) *centry_out = next;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_is_bounded(const IR_t * nd) {
    if (!nd) return 0;
    switch (nd->op) {
    case IR_CALL_PROLOG: { const char * fn = nd->sval; if (!fn || !fn[0]) return 0; if (!strcmp(fn, "$retract")) return 0; return 1; }
    case IR_CUT: case IR_SUCCEED: case IR_FAIL: case IR_GOTO: case IR_MOVE_LABEL: return 1;
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL: case IR_VAR: case IR_VAR_REF: return 1;
    case IR_CALL_BUILTIN_GEN: case IR_DISJUNCTION: case IR_CALL_PROC_STAGED: case IR_CALL: return 0;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_bounded_dump(const IR_graph_t * g) {
    static int on = -1; if (on < 0) on = getenv("SCRIP_PL_BOUNDED_DUMP") ? 1 : 0; if (!on || !g) return;
    for (int i = 0; i < g->n; i++) { const IR_t * nd = g->all[i]; if (!nd) continue; const char * fn = (nd->op == IR_CALL_PROLOG) ? nd->sval : (const char *) 0; fprintf(stderr, "PLBND op=%d bounded=%d sval=%s\n", (int) nd->op, pl_goal_is_bounded(nd), fn ? fn : "-"); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_prolog_clause(const tree_t * clause) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    IR_graph_t * g = IR_alloc(256);
    pl_cells_stamp(g);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL; cx.ite_funnel = NULL;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    int arity = (int) clause->v.dval;
    if (arity < 0) arity = 0;
    if (arity > clause->n) arity = clause->n;
    IR_t * bentry = NULL;
    thread_goals(&cx, clause, arity, clause->n, succeed, fail, &bentry, NULL);
    IR_t * next = bentry ? bentry : succeed;
    IR_t ** hu = (arity > 0) ? (IR_t **) calloc((size_t) arity, sizeof(IR_t *)) : NULL;
    for (int i = arity - 1; i >= 0; i--) {
        const tree_t * h = clause->c[i];
        IR_t * u = build(&cx, IR_OP_COUNT, next, fail);
        IR_t * lv = build(&cx, IR_OP_COUNT, NULL, NULL); IR_LIT(lv).ival = i;
        ir_operand_push(u, lv);
        ir_operand_push(u, term(&cx, h));
        next = u;
        if (hu) hu[i] = u;
    }
    if (hu) free(hu);
    g->nslots = max_var_slot(clause, arity - 1) + 1;
    pl_bounded_dump(g);
    if (arity == 0 && !bentry) { g->entry = succeed; g->body_root = NULL; return g; }
    g->entry = next;
    g->body_root = NULL;
    return g;
}
#include <stdio.h>
#include "stage2.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "bb_program.h"
#include "../runtime/builtins/resolution.h"
#include "../parsers/prolog/term.h"
#include "../parsers/prolog/prolog_atom.h"
extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
extern int ir_is_generator_kind(IR_e t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_pred_graph_new(const tree_t * ch, int arity, int suspend_deliver) {
    const tree_t * one[1]; const tree_t ** clauses; int nc;
    if (ch->t == TT_CLAUSE) { one[0] = ch; clauses = one; nc = 1; }
    else if (ch->t == TT_CHOICE && ch->n >= 1) { clauses = (const tree_t **) ch->c; nc = ch->n; }
    else return -1;
    if (arity < 0) arity = 0;
    IR_graph_t * g = IR_alloc(1024);
    pl_cells_stamp(g);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL; cx.ite_funnel = NULL;
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) calloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * dj = build(&cx, IR_DISJUNCTION, succeed, fail);
    IR_t * mk = build(&cx, IR_CALL_PROLOG, NULL, fail); IR_LIT(mk).sval = "$trail_mark";
    IR_t ** centry = (IR_t **) calloc((size_t) nc, sizeof(IR_t *));
    IR_t ** uw = (IR_t **) calloc((size_t)(nc + 1), sizeof(IR_t *));
    IR_t * uwf = build(&cx, IR_CALL_PROLOG, fail, fail); IR_LIT(uwf).sval = "$trail_unwind"; ir_operand_push(uwf, mk);
    uw[nc] = uwf;
    cx.cut_ω = uwf;
    int maxlocal = -1;
    for (int k = nc - 1; k >= 0; k--) {
        IR_t * next_fail = uw[k + 1];
        IR_t * ml = build(&cx, suspend_deliver ? IR_SUSPEND : IR_MOVE_LABEL, succeed, fail);
        IR_t * ce = NULL; IR_t * redo = NULL;
        lower_pl_clause_into(&cx, clauses[k], arity, ml, next_fail, &ce, &redo);
        IR_t * ab = redo ? redo : next_fail;
        if (suspend_deliver) { ir_operand_push(ml, mk); ir_operand_push(ml, ab); }
        else {
            IR_LIT(ml).ival = (ab && ab->op != IR_DISJUNCTION && (ir_is_generator_kind(ab->op) || ab->op == IR_CALL || ab->op == IR_CALL_PROC_STAGED)) ? 1 : 0;
            ir_operand_push(ml, ab); ir_operand_push(ml, dj); ir_operand_push(ml, NULL);
        }
        if (arity > 0 && nc > 1 && !pl_no_ix() && (int) clauses[k]->v.dval >= 1) { long long ki = 0; const char * ks = 0; int kk = pl_ix_key(clauses[k]->c[0], &ki, &ks);
            if (kk) { IR_t * skp = (k < nc - 1) ? centry[k + 1] : uwf;
                IR_t * gnode = build(&cx, IR_CALL_PROLOG, ce, skp); IR_LIT(gnode).sval = "$ix_g";
                IR_t * glhs = build(&cx, IR_VAR_REF, NULL, NULL); IR_LIT(glhs).sval = pl_param_name(0);
                IR_t * gk1 = build(&cx, IR_LIT_INTEGER, NULL, NULL); IR_LIT(gk1).ival = (int64_t)((long long) kk | (((kk == 4) ? ki : 0) << 8));
                IR_t * gk2; if (kk == 2 || kk == 4) { gk2 = build(&cx, IR_LIT_STRING, NULL, NULL); IR_LIT(gk2).sval = ks; } else { gk2 = build(&cx, IR_LIT_INTEGER, NULL, NULL); IR_LIT(gk2).ival = (kk == 1) ? (int64_t) ki : 0; }
                lc_γ_to(glhs, gk1); lc_ω_to(glhs, skp); lc_γ_to(gk1, gk2); lc_ω_to(gk1, skp); lc_γ_to(gk2, gnode); lc_ω_to(gk2, skp);
                ir_operand_push(gnode, glhs); ir_operand_push(gnode, gk1); ir_operand_push(gnode, gk2);
                ce = glhs; } }
        centry[k] = ce;
        if (k > 0) { IR_t * u = build(&cx, IR_CALL_PROLOG, ce, fail); IR_LIT(u).sval = "$unwind_nothrow"; ir_operand_push(u, mk); uw[k] = u; }
        maxlocal = max_var_slot(clauses[k], maxlocal);
    }
    lc_γ_to(mk, centry[0]);
    g->entry = mk;
    g->body_root = dj;
    g->nslots = arity + (maxlocal + 1) + nc + 8;
    g->resume_slot = 0;
    if (maxlocal >= 0) { g->nlocals = maxlocal + 1; g->lnames = (const char **) calloc((size_t)(maxlocal + 1), sizeof(const char *)); for (int k = 0; k <= maxlocal; k++) g->lnames[k] = pl_var_name(k); }
    free(centry); free(uw);
    return bb_program_add(&g_stage2.bbp, g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_clause_graph(const tree_t *clause) {
    if (!clause || clause->t != TT_CLAUSE) return -1;
    IR_graph_t *gnl = lower_prolog_clause(clause);
    return gnl ? bb_program_add(&g_stage2.bbp, gnl) : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_choice_graph(const tree_t *choice) {
    if (!choice || choice->t != TT_CHOICE || choice->n < 1) return -1;
    int n = choice->n;
    IR_graph_t **bodies = (IR_graph_t **)rt_ws_alloc((size_t)n * sizeof(IR_graph_t *));
    if (!bodies) return -1;
    int any = 0;
    for (int ci = 0; ci < n; ci++) {
        const tree_t *cl = choice->c[ci];
        int bidx = lower_pl_clause_graph(cl);
        bodies[ci] = (bidx >= 0) ? g_stage2.bbp.table[bidx] : NULL;
        if (bodies[ci]) any = 1;
    }
    if (!any) return -1;
    IR_graph_t *g = IR_alloc(8);
    if (!g) return -1;
    pl_cells_stamp(g);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd = IR_node_alloc(g, IR_OP_COUNT);
    if (!nd) return -1;
    bb_choice_state_t *zc = (bb_choice_state_t *)rt_ws_alloc(sizeof *zc);
    if (!zc) return -1;
    memset(zc, 0, sizeof *zc);
    zc->bodies = bodies; zc->nbodies = n; zc->last_body = NULL; zc->cp = NULL; zc->cut_barrier = NULL;
    zc->idx_ok = 0; zc->idx_key = NULL;
    IR_LIT(nd).ival = (int64_t)(intptr_t)zc;
    nd->γ.node = PSUCC; memcpy(nd->γ.sz, "α", 3); nd->ω.node = PFAIL; memcpy(nd->ω.sz, "α", 3);
    g->entry = nd;
    return bb_program_add(&g_stage2.bbp, g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_dyn_mark(const char *name, int arity) {
    if (!name) return;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return;
    if (g_stage2.pl_dyn_n >= 64) return;
    g_stage2.pl_dyn_name[g_stage2.pl_dyn_n] = name; g_stage2.pl_dyn_arity[g_stage2.pl_dyn_n] = arity; g_stage2.pl_dyn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_is_marked(const char *name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_dyn_mark_from_spec(const tree_t *spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && spec->c[1]) {
        const tree_t *nm = spec->c[0], *ar = spec->c[1];
        if ((nm->t == TT_QLIT || nm->t == TT_NAME) && nm->v.sval && ar->t == TT_ILIT) pl_dyn_mark(strdup(nm->v.sval), (int)ar->v.ival);
        return;
    }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_dyn_mark_from_spec(spec->c[0]); pl_dyn_mark_from_spec(spec->c[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_dyn_mark_from_clause_arg(const tree_t *arg) {
    if (!arg) return;
    const tree_t *h = arg;
    if (arg->t == TT_FNC && arg->v.sval && !strcmp(arg->v.sval, ":-") && arg->n == 2) h = arg->c[0];
    if (!h) return;
    if (h->t == TT_FNC && h->v.sval) pl_dyn_mark(strdup(h->v.sval), h->n);
    else if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) pl_dyn_mark(strdup(h->v.sval), 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_dyn_scan(const tree_t *t) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && t->n >= 1) {
        const char *fn = t->v.sval;
        if ((!strcmp(fn,"assertz")||!strcmp(fn,"asserta")||!strcmp(fn,"assert")||!strcmp(fn,"retract")||!strcmp(fn,"retractall")) && t->n == 1) pl_dyn_mark_from_clause_arg(t->c[0]);
        else if (!strcmp(fn,"abolish") && t->n == 1) pl_dyn_mark_from_spec(t->c[0]);
        else if (!strcmp(fn,"dynamic") && t->n == 1) pl_dyn_mark_from_spec(t->c[0]);
    }
    for (int i = 0; i < t->n; i++) pl_dyn_scan(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_dyn_mark_prepass(void) {
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++)
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            const tree_t *ch = pe->choice;
            if (!ch) continue;
            if (ch->t == TT_CHOICE) { for (int i = 0; i < ch->n; i++) pl_dyn_scan(ch->c[i]); }
            else pl_dyn_scan(ch);
        }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_dyniter_graph(const char *name, int arity) {
    IR_graph_t *g = IR_alloc(64);
    if (!g) return -1;
    pl_cells_stamp(g);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL; cx.ite_funnel = NULL;
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) calloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gen = build(&cx, IR_CALL_BUILTIN_GEN, NULL, fail); IR_LIT(gen).sval = "$dyn_iter";
    IR_t * nmop = build(&cx, IR_LIT_STRING, NULL, NULL); IR_LIT(nmop).sval = strdup(name);
    ir_operand_push(gen, nmop);
    IR_t * prev = nmop;
    for (int i = 0; i < arity; i++) {
        IR_t * vr = build(&cx, IR_VAR_REF, NULL, NULL); IR_LIT(vr).sval = pl_param_name(i);
        lc_γ_to(prev, vr); prev = vr; ir_operand_push(gen, vr);
    }
    IR_t * sp = build(&cx, IR_SUSPEND, succeed, fail);
    ir_operand_push(sp, gen); ir_operand_push(sp, gen);
    lc_γ_to(prev, gen); lc_γ_to(gen, sp);
    g->entry = nmop; g->body_root = gen;
    return bb_program_add(&g_stage2.bbp, g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ensure_gen_builtin_pred(const char *gen_sval, const char *pred_nm, int nparams) {
    { char key[64]; snprintf(key, sizeof key, "%s/%d", pred_nm, nparams); if (resolve_bb_lookup(key, nparams)) return; }
    IR_graph_t * g = IR_alloc(64);
    if (!g) return;
    pl_cells_stamp(g);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL; cx.ite_funnel = NULL;
    g->nparams = nparams;
    g->pnames = (const char **) calloc((size_t) nparams, sizeof(const char *)); for (int i = 0; i < nparams; i++) g->pnames[i] = pl_param_name(i);
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gen = build(&cx, IR_CALL_BUILTIN_GEN, NULL, fail); IR_LIT(gen).sval = gen_sval;
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nparams; i++) {
        IR_t * vr = build(&cx, IR_VAR_REF, NULL, NULL); IR_LIT(vr).sval = pl_param_name(i);
        if (prev) lc_γ_to(prev, vr); else first = vr;
        prev = vr; ir_operand_push(gen, vr);
    }
    IR_t * sp = build(&cx, IR_SUSPEND, succeed, fail);
    ir_operand_push(sp, gen); ir_operand_push(sp, gen);
    lc_γ_to(prev, gen); lc_γ_to(gen, sp);
    g->entry = first; g->body_root = gen;
    int bb_idx = bb_program_add(&g_stage2.bbp, g);
    if (bb_idx < 0) return;
    { char key[64]; snprintf(key, sizeof key, "%s/%d", pred_nm, nparams);
      resolve_bb_register(strdup(key), nparams, bb_idx);
      int pi = stage2_proc_grow(&g_stage2);
      g_stage2.proc_table[pi].name         = strdup(key);
      g_stage2.proc_table[pi].proc         = NULL;
      g_stage2.proc_table[pi].entry_pc     = -1;
      g_stage2.proc_table[pi].bb_idx       = bb_idx;
      g_stage2.proc_table[pi].nparams      = nparams;
      g_stage2.proc_table[pi].is_generator = 1;
      { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) g_stage2.module_registry.mods[mi].nprocs++; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ensure_call_bridge(int nparams) { if (nparams >= 1 && nparams <= 8) pl_ensure_gen_builtin_pred("$call", "$call", nparams); }
static void pl_det_compute(void);
static int pl_det_key_is_det(const char * key);
static int g_pl_disj_ctr = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dj_collect_vars(const tree_t * t, int * idx, int * n, int max) {
    if (!t) return;
    if (t->t == TT_VAR) { int v = (int) t->v.ival; for (int i = 0; i < *n; i++) if (idx[i] == v) return; if (*n < max) idx[(*n)++] = v; return; }
    for (int i = 0; i < t->n; i++) dj_collect_vars(t->c[i], idx, n, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dj_has_cut(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_CUT) return 1;
    for (int i = 0; i < t->n; i++) if (dj_has_cut(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dj_is_plain(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ";") && t->n == 2 && !(t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "->"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * dj_mkvar(int idx) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = idx; return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dj_expand_node(tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_IF || t->t == TT_PROGRAM) { int any = 0; for (int i = 0; i < t->n; i++) if (dj_expand_node(t->c[i])) any = 1; return any; }
    if (t->t != TT_FNC || !t->v.sval) return 0;
    const char * f = t->v.sval;
    if (dj_is_plain(t) && !dj_has_cut(t)) {
        const tree_t * branches[64]; int nb = 0; { const tree_t * d = t; while (dj_is_plain(d) && nb < 63) { branches[nb++] = d->c[0]; d = d->c[1]; } branches[nb++] = d; }
        int idx[128]; int ni = 0; dj_collect_vars(t, idx, &ni, 128);
        char nm[32]; snprintf(nm, sizeof nm, "$disj%d", g_pl_disj_ctr++);
        tree_t * choice = ast_node_new(TT_CHOICE);
        for (int b = 0; b < nb; b++) { tree_t * cl = ast_node_new(TT_CLAUSE); cl->v.dval = (double) ni; for (int i = 0; i < ni; i++) ast_push(cl, dj_mkvar(idx[i])); ast_push(cl, (tree_t *) branches[b]); ast_push(choice, cl); }
        char key[48]; snprintf(key, sizeof key, "%s/%d", nm, ni); resolve_pred_table_insert(&g_stage2.resolve_pred_table, strdup(key), choice);
        tree_t * call = ast_node_new(TT_FNC); for (int i = 0; i < ni; i++) ast_push(call, dj_mkvar(idx[i]));
        t->v.sval = strdup(nm); t->c = call->c; t->n = call->n;
        return 1;
    }
    if ((!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->")) && t->n == 2) { int a = dj_expand_node(t->c[0]); int b = dj_expand_node(t->c[1]); return a || b; }
    if ((!strcmp(f, "\\+") || !strcmp(f, "not") || !strcmp(f, "once") || !strcmp(f, "ignore") || !strcmp(f, "call")) && t->n >= 1) return dj_expand_node(t->c[0]);
    if ((!strcmp(f, "findall") || !strcmp(f, "aggregate_all")) && t->n == 3) return dj_expand_node(t->c[1]);
    if (!strcmp(f, "forall") && t->n == 2) { int a = dj_expand_node(t->c[0]); int b = dj_expand_node(t->c[1]); return a || b; }
    if (!strcmp(f, "catch") && t->n == 3) { int a = dj_expand_node(t->c[0]); int b = dj_expand_node(t->c[2]); return a || b; }
    if (!strcmp(f, "setup_call_cleanup") && t->n == 3) { int a = dj_expand_node(t->c[0]); int b = dj_expand_node(t->c[1]); int c = dj_expand_node(t->c[2]); return a || b || c; }
    if (!strcmp(f, "^") && t->n == 2) return dj_expand_node(t->c[1]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_expand_disjunctions(void) {
    int changed = 1; int guard = 0;
    while (changed && guard++ < 4096) {
        changed = 0;
        for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++)
            for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
                tree_t * ch = (tree_t *) pe->choice; if (!ch) continue;
                if (ch->t == TT_CLAUSE) { for (int i = (int) ch->v.dval; i < ch->n; i++) if (dj_expand_node(ch->c[i])) changed = 1; }
                else if (ch->t == TT_CHOICE) { for (int k = 0; k < ch->n; k++) { tree_t * cl = ch->c[k]; if (cl && cl->t == TT_CLAUSE) for (int i = (int) cl->v.dval; i < cl->n; i++) if (dj_expand_node(cl->c[i])) changed = 1; } }
            }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_pl_register_all_preds(void) {
    pl_det_compute();
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *key = pe->key;
            const tree_t *ch = pe->choice;
            const char *slash = key ? strrchr(key, '/') : NULL;
            int ar = slash ? atoi(slash + 1) : 0;
            if (resolve_bb_lookup(key, ar)) continue;
            int dyn = 0;
            { char nm[200]; int kl = slash ? (int)(slash - key) : (int)strlen(key); if (kl > 199) kl = 199; memcpy(nm, key, kl); nm[kl] = 0; dyn = pl_dyn_is_marked(nm, ar); }
            int bb_idx = -1; int det = 0;
            if (dyn) {
                const char *slash2 = strrchr(key, '/');
                static char nmbuf[200]; int kl = slash2 ? (int)(slash2 - key) : (int)strlen(key); if (kl > 199) kl = 199; memcpy(nmbuf, key, kl); nmbuf[kl] = 0;
                bb_idx = lower_pl_dyniter_graph(nmbuf, ar);
            } else if (ch->t == TT_CLAUSE || ch->t == TT_CHOICE) {
                det = (strcmp(key, "main/0") != 0) && !(getenv("SCRIP_PL_DET") && !strcmp(getenv("SCRIP_PL_DET"), "0")) && pl_det_key_is_det(key);
                bb_idx = lower_pl_pred_graph_new(ch, ar, strcmp(key, "main/0") != 0 && !det);
            }
            if (bb_idx >= 0) {
                resolve_bb_register(key, ar, bb_idx);
                {
                    int pi = stage2_proc_grow(&g_stage2);
                    g_stage2.proc_table[pi].name         = (strcmp(key, "main/0") == 0) ? strdup("main") : strdup(key);
                    g_stage2.proc_table[pi].proc         = NULL;
                    g_stage2.proc_table[pi].entry_pc     = -1;
                    g_stage2.proc_table[pi].bb_idx       = bb_idx;
                    g_stage2.proc_table[pi].nparams      = ar;
                    g_stage2.proc_table[pi].is_generator = det ? 0 : 1;
                    { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++;
                        if (strcmp(key, "main/0") == 0 && g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; } }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_pl_register_dyn_only_preds(void) {
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) {
        const char * nm = g_stage2.pl_dyn_name[i]; int ar = g_stage2.pl_dyn_arity[i];
        if (!nm) continue;
        char key[200]; snprintf(key, sizeof key, "%s/%d", nm, ar);
        if (resolve_bb_lookup(key, ar)) continue;
        int bb_idx = lower_pl_dyniter_graph(nm, ar);
        if (bb_idx < 0) continue;
        resolve_bb_register(strdup(key), ar, bb_idx);
        int pi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[pi].name         = strdup(key);
        g_stage2.proc_table[pi].proc         = NULL;
        g_stage2.proc_table[pi].entry_pc     = -1;
        g_stage2.proc_table[pi].bb_idx       = bb_idx;
        g_stage2.proc_table[pi].nparams      = ar;
        g_stage2.proc_table[pi].is_generator = 1;
        { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) g_stage2.module_registry.mods[mi].nprocs++; }
    }
}
typedef struct { const char * key; const tree_t * ch; int state; } pl_det_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_name_in(const char * nm, const char * const * lst) { for (int i = 0; lst[i]; i++) if (!strcmp(nm, lst[i])) return 1; return 0; }
static int pl_det_lookup(const char * nm, int ar, const pl_det_ent_t * v, int n) { char kb[256]; snprintf(kb, sizeof kb, "%s/%d", nm, ar); for (int i = 0; i < n; i++) if (v[i].key && !strcmp(v[i].key, kb)) return i; return -1; }
static int pl_det_top_cut(const tree_t * t) { if (!t) return 0; if (t->t == TT_CUT) return 1; if (t->t == TT_PROGRAM || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ","))) { for (int i = 0; i < t->n; i++) if (pl_det_top_cut(t->c[i])) return 1; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_goal_ok(const tree_t * t, const pl_det_ent_t * v, int n) {
    if (!t) return 1;
    if (t->t == TT_CUT || t->t == TT_UNIFY) return 1;
    if (t->t == TT_PROGRAM) { for (int i = 0; i < t->n; i++) if (!pl_det_goal_ok(t->c[i], v, n)) return 0; return 1; }
    if (t->t != TT_FNC || !t->v.sval) return 0;
    { const char * nm = t->v.sval;
      if (!strcmp(nm, ",")) { for (int i = 0; i < t->n; i++) if (!pl_det_goal_ok(t->c[i], v, n)) return 0; return 1; }
      if (!strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "|")) return 0;
      { const char * const black[] = { "retract", "throw", "catch", "$dyn_iter", "findall", "setof", "bagof", "forall", "call", "not", "\\+", "read", "read_term", 0 }; if (pl_det_name_in(nm, black)) return 0; }
      { const char * const extra[] = { "true", "fail", "false", "is", "<", ">", "=<", ">=", "=:=", "=\\=", "\\=", "=", "==", "\\==", "@<", "@>", "@=<", "@>=", "halt", 0 }; if (pl_det_name_in(nm, extra)) return 1; }
      { extern const char * rt_pl_det_builtin_target(const char * nm2, int ar2); if (rt_pl_det_builtin_target(nm, t->n)) return 1; }
      if (is_builtin_exec(nm)) return 1;
      { int j = pl_det_lookup(nm, t->n, v, n); return (j >= 0 && v[j].state == 1) ? 1 : 0; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_clause_ok(const tree_t * cl, int need_cut, const pl_det_ent_t * v, int n) {
    if (!cl || cl->t != TT_CLAUSE) return 0;
    { int ar = (int) cl->v.dval; if (ar < 0) ar = 0; if (ar > cl->n) ar = cl->n;
      for (int i = ar; i < cl->n; i++) if (!pl_det_goal_ok(cl->c[i], v, n)) return 0;
      if (!need_cut) return 1;
      for (int i = ar; i < cl->n; i++) if (pl_det_top_cut(cl->c[i])) return 1;
      return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_tree_eq(const tree_t * a, const tree_t * b) {
    if (!a || !b) return a == b;
    if (a->t != b->t || a->n != b->n) return 0;
    if (a->t == TT_VAR || a->t == TT_ILIT) { if (a->v.ival != b->v.ival) return 0; }
    else if (a->t == TT_FNC || a->t == TT_NAME || a->t == TT_QLIT) { if ((a->v.sval == NULL) != (b->v.sval == NULL)) return 0; if (a->v.sval && strcmp(a->v.sval, b->v.sval)) return 0; }
    for (int i = 0; i < a->n; i++) if (!pl_det_tree_eq(a->c[i], b->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_det_first_goal(const tree_t * cl) {
    int ar = (int) cl->v.dval; if (ar < 0) ar = 0; if (ar > cl->n) ar = cl->n;
    if (ar >= cl->n) return NULL;
    { const tree_t * g = cl->c[ar];
      while (g && ((g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, ",")) || g->t == TT_PROGRAM) && g->n > 0) g = g->c[0];
      return g; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_heads_allvar_same(const tree_t * c1, const tree_t * c2) {
    if (!c1 || !c2 || c1->t != TT_CLAUSE || c2->t != TT_CLAUSE) return 0;
    { int a1 = (int) c1->v.dval, a2 = (int) c2->v.dval; if (a1 != a2 || a1 < 0 || a1 > c1->n || a2 > c2->n) return 0;
      for (int i = 0; i < a1; i++) {
          if (!c1->c[i] || !c2->c[i] || c1->c[i]->t != TT_VAR || c2->c[i]->t != TT_VAR) return 0;
          if (c1->c[i]->v.ival != c2->c[i]->v.ival) return 0;
          for (int j = 0; j < i; j++) if (c1->c[j] && c1->c[j]->v.ival == c1->c[i]->v.ival) return 0; }
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_guard_comp(const tree_t * c1, const tree_t * c2) {
    const tree_t * g1 = pl_det_first_goal(c1); const tree_t * g2 = pl_det_first_goal(c2);
    if (!g1 || !g2 || g1->t != TT_FNC || g2->t != TT_FNC || !g1->v.sval || !g2->v.sval || g1->n != 2 || g2->n != 2) return 0;
    { static const char * const cp[][2] = { { ">", "=<" }, { "=<", ">" }, { "<", ">=" }, { ">=", "<" }, { "=:=", "=\\=" }, { "=\\=", "=:=" }, { 0, 0 } };
      int ok = 0; for (int i = 0; cp[i][0]; i++) if (!strcmp(g1->v.sval, cp[i][0]) && !strcmp(g2->v.sval, cp[i][1])) { ok = 1; break; }
      if (!ok) return 0; }
    return pl_det_tree_eq(g1->c[0], g2->c[0]) && pl_det_tree_eq(g1->c[1], g2->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_pred_ok(const tree_t * ch, const pl_det_ent_t * v, int n) {
    if (!ch) return 0;
    if (ch->t == TT_CLAUSE) return pl_det_clause_ok(ch, 0, v, n);
    if (ch->t == TT_CHOICE && ch->n == 2 && pl_det_heads_allvar_same(ch->c[0], ch->c[1]) && pl_det_guard_comp(ch->c[0], ch->c[1])
        && pl_det_clause_ok(ch->c[0], 0, v, n) && pl_det_clause_ok(ch->c[1], 0, v, n)) return 1;
    if (ch->t == TT_CHOICE && ch->n >= 1) { for (int i = 0; i < ch->n; i++) if (!pl_det_clause_ok(ch->c[i], (i < ch->n - 1), v, n)) return 0; return 1; }
    return 0;
}
enum { PL_DET_MAX = 4096 };
static pl_det_ent_t g_pl_det_v[PL_DET_MAX]; static int g_pl_det_n = 0; static int g_pl_det_done = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_det_compute(void) {
    if (g_pl_det_done) return;
    g_pl_det_done = 1;
    int n = 0;
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++)
        for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice || n >= PL_DET_MAX) continue;
            { int dyn = 0;
              { const char * sl = strrchr(pe->key, '/'); int ar = sl ? atoi(sl + 1) : 0; char nm[200]; int kl = sl ? (int)(sl - pe->key) : (int) strlen(pe->key); if (kl > 199) kl = 199; memcpy(nm, pe->key, kl); nm[kl] = 0; dyn = pl_dyn_is_marked(nm, ar); }
              g_pl_det_v[n].key = pe->key; g_pl_det_v[n].ch = pe->choice; g_pl_det_v[n].state = dyn ? 0 : 1; n++; } }
    { int changed = 1; for (int pass = 0; changed && pass < n + 2; pass++) { changed = 0; for (int i = 0; i < n; i++) { if (g_pl_det_v[i].state == 1 && !pl_det_pred_ok(g_pl_det_v[i].ch, g_pl_det_v, n)) { g_pl_det_v[i].state = 0; changed = 1; } } } }
    g_pl_det_n = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_key_is_det(const char * key) { if (!key) return 0; for (int i = 0; i < g_pl_det_n; i++) if (g_pl_det_v[i].key && !strcmp(g_pl_det_v[i].key, key)) return g_pl_det_v[i].state == 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_det_classify_all(void) {
    pl_det_compute();
    { int ndet = 0;
      for (int i = 0; i < g_pl_det_n; i++) {
          if (g_pl_det_v[i].state != 1) continue;
          ndet++;
          for (int pi = 0; pi < g_stage2.proc_count; pi++) {
              const char * pn = g_stage2.proc_table[pi].name;
              int match = pn && (!strcmp(pn, g_pl_det_v[i].key) || (!strcmp(g_pl_det_v[i].key, "main/0") && !strcmp(pn, "main")));
              if (!match) continue;
              { int bx = g_stage2.proc_table[pi].bb_idx; if (bx >= 0 && bx < g_stage2.bbp.count && g_stage2.bbp.table[bx]) g_stage2.bbp.table[bx]->deterministic = 1; } } }
      if (getenv("SCRIP_DET_REPORT")) { fprintf(stderr, "DET-CLASS preds=%d det=%d\n", g_pl_det_n, ndet); for (int i = 0; i < g_pl_det_n; i++) fprintf(stderr, "DET-CLASS %-6s %s\n", g_pl_det_v[i].state == 1 ? "DET" : "NONDET", g_pl_det_v[i].key); } }
}
static int pl_ll_ctr = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ll_collect_vars(const tree_t *t, int *order, int *norder, int cap) {
    if (!t) return;
    if (t->t == TT_VAR) {
        int s = (int)t->v.ival;
        for (int i = 0; i < *norder; i++) if (order[i] == s) return;
        if (*norder < cap) order[(*norder)++] = s;
        return;
    }
    for (int i = 0; i < t->n; i++) pl_ll_collect_vars(t->c[i], order, norder, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_ll_copy_remap(const tree_t *t, const int *remap, int rn) {
    if (!t) return NULL;
    tree_t *c = ast_node_new(t->t);
    c->v = t->v;
    if (t->t == TT_VAR) { int s = (int)t->v.ival; if (s >= 0 && s < rn && remap[s] >= 0) c->v.ival = remap[s]; }
    for (int i = 0; i < t->n; i++) ast_push(c, pl_ll_copy_remap(t->c[i], remap, rn));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ll_maybe_lift(tree_t *fa) {
    if (!fa || fa->t != TT_FNC || !fa->v.sval || fa->n != 3) return;
    if (strcmp(fa->v.sval, "findall") && strcmp(fa->v.sval, "aggregate_all")) return;
    tree_t *tmpl = fa->c[0], *g = fa->c[1];
    if (!g) return;
    if (g->t == TT_VAR) return;
    int need;
    if (g->t == TT_QLIT) need = 0;
    else if (g->t == TT_FNC && g->v.sval && strcmp(g->v.sval, ",") && strcmp(g->v.sval, ";")) {
        need = 0;
        for (int i = 0; i < g->n; i++) if (!g->c[i] || g->c[i]->t != TT_VAR) { need = 1; break; }
    } else need = 1;
    if (!need) return;
    int head_slots[8]; int nhead = 0;
    pl_ll_collect_vars(tmpl, head_slots, &nhead, 8);
    if (nhead > 3) return;
    int maxs = -1, tmp[256], nt = 0;
    pl_ll_collect_vars(tmpl, tmp, &nt, 256);
    for (int i = 0; i < nt; i++) if (tmp[i] > maxs) maxs = tmp[i];
    int gt[256], ngv = 0;
    pl_ll_collect_vars(g, gt, &ngv, 256);
    for (int i = 0; i < ngv; i++) if (gt[i] > maxs) maxs = gt[i];
    int rn = maxs + 1; if (rn < 1) rn = 1;
    int *remap = (int *)malloc((size_t)rn * sizeof(int));
    for (int i = 0; i < rn; i++) remap[i] = -1;
    for (int j = 0; j < nhead; j++) if (head_slots[j] >= 0 && head_slots[j] < rn) remap[head_slots[j]] = j;
    int next_new = nhead;
    for (int i = 0; i < ngv; i++) { int s = gt[i]; if (s >= 0 && s < rn && remap[s] < 0) remap[s] = next_new++; }
    char *nm = (char *)malloc(16); snprintf(nm, 16, "$fa%d", pl_ll_ctr++);
    char *key = (char *)malloc(24); snprintf(key, 24, "%s/%d", nm, nhead);
    tree_t *cl = ast_node_new(TT_CLAUSE);
    cl->v.sval = key; cl->v.dval = (double)nhead;
    for (int j = 0; j < nhead; j++) { tree_t *hv = ast_node_new(TT_VAR); hv->v.ival = j; ast_push(cl, hv); }
    ast_push(cl, pl_ll_copy_remap(g, remap, rn));
    free(remap);
    int bb_idx = lower_pl_pred_graph_new(cl, nhead, 1);
    if (bb_idx < 0) return;
    resolve_bb_register(key, nhead, bb_idx);
    { int pi = stage2_proc_grow(&g_stage2);
      g_stage2.proc_table[pi].name         = strdup(key);
      g_stage2.proc_table[pi].proc         = NULL;
      g_stage2.proc_table[pi].entry_pc     = -1;
      g_stage2.proc_table[pi].bb_idx       = bb_idx;
      g_stage2.proc_table[pi].nparams      = nhead;
      g_stage2.proc_table[pi].is_generator = 1;
      { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) g_stage2.module_registry.mods[mi].nprocs++; } }
    tree_t *call = ast_node_new(TT_FNC); call->v.sval = nm;
    for (int j = 0; j < nhead; j++) { tree_t *av = ast_node_new(TT_VAR); av->v.ival = head_slots[j]; ast_push(call, av); }
    fa->c[1] = call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ll_scan(tree_t *t) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "findall") || !strcmp(t->v.sval, "aggregate_all")) && t->n == 3)
        pl_ll_maybe_lift(t);
    for (int i = 0; i < t->n; i++) pl_ll_scan(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ll_prepass(void) {
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++)
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            tree_t *ch = pe->choice;
            if (!ch) continue;
            if (ch->t == TT_CHOICE) { for (int i = 0; i < ch->n; i++) pl_ll_scan(ch->c[i]); }
            else pl_ll_scan(ch);
        }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * sub = lp_s_expr(s, ":subj");
        if (!sub) continue;
        if ((sub->t == TT_CHOICE || sub->t == TT_CLAUSE) && sub->v.sval) {
            resolve_pred_table_insert(&s2->resolve_pred_table, sub->v.sval, sub);
            g_resolve_active = 1;
            if (strcmp(sub->v.sval, "main/0") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_clause_body_only(const tree_t *rc) {
    int ar = (int) rc->v.dval; if (ar < 0) ar = 0; if (ar > rc->n) ar = rc->n;
    if (ar >= rc->n) return pl_synth_qlit("true");
    const tree_t *body = rc->c[rc->n - 1];
    for (int j = rc->n - 2; j >= ar; j--) body = pl_synth_fnc2(",", rc->c[j], body);
    return body;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* row prolog-toplevel-main-multiclause-no-fallthrough-on-backtrack: a multi-clause TT_CHOICE used to
   resolve to ONLY choice->c[0]'s body, so a toplevel `main :- ..., fail.` / `main :- ...` idiom never
   reached its second clause -- an ordinary BY-NAME call already backtracks across clauses correctly
   (verified: only this inline-the-first-clause path did not), so for the arity-0 case (the toplevel
   convention; the only case exercised here) every alternative's body is folded into one right-nested
   disjunction, giving inline-the-body the same backtrack-across-clauses semantics a by-name call has.
   Head-argument unification across a synthesized disjunction is a bigger change and out of scope here,
   so a non-zero-arity choice still falls back to its first alternative only, unchanged from before. */
static const tree_t * pl_choice_all_bodies(const tree_t *choice) {
    if (!choice) return NULL;
    if (choice->t == TT_CLAUSE) return pl_clause_body_only(choice);
    if (choice->t != TT_CHOICE || choice->n < 1) return NULL;
    int ar0 = (int) choice->c[0]->v.dval; if (ar0 < 0) ar0 = 0;
    if (ar0 != 0) return pl_clause_body_only(choice->c[0]);
    const tree_t *disj = pl_clause_body_only(choice->c[choice->n - 1]);
    for (int i = choice->n - 2; i >= 0; i--) disj = pl_synth_fnc2(";", pl_clause_body_only(choice->c[i]), disj);
    return disj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_init_resolve_body(const tree_t *gt) {
    char kb[128]; const char *k = NULL;
    if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME) && gt->v.sval) { snprintf(kb, sizeof kb, "%s/0", gt->v.sval); k = kb; }
    else if (gt && gt->t == TT_FNC && gt->v.sval) { snprintf(kb, sizeof kb, "%s/%d", gt->v.sval, gt->n); k = kb; }
    if (!k) return NULL;
    const tree_t *choice = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, k);
    return choice ? pl_choice_all_bodies(choice) : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_init_goal_name(const tree_t *gt) {
    char kb[128];
    if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME) && gt->v.sval) snprintf(kb, sizeof kb, "%s/0", gt->v.sval);
    else if (gt && gt->t == TT_FNC && gt->v.sval) snprintf(kb, sizeof kb, "%s/%d", gt->v.sval, gt->n);
    else snprintf(kb, sizeof kb, "?");
    return strdup(kb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pl_stage2(const tree_t *prog) {
    int _pl_bb0 = g_stage2.bbp.count;
    pl_register_program(&g_stage2, prog);
    pl_expand_disjunctions();
    pl_ll_prepass();
    pl_dyn_mark_prepass();
    enum { PL_INIT_GOALS_MAX = 256 };
    static const tree_t *pl_init_goals_acc[PL_INIT_GOALS_MAX];
    static int pl_init_ngoals_acc = 0;
    static int pl_init_main_pi = -1;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            /* prolog_lower.c wraps a bare `:- Goal.` load directive into a synthetic `pj_dir_<N> :- Goal.`
               helper plus an `initialization(pj_dir_<N>)` statement. These USED to be excluded here, with the
               rationale that chaining them as by-name calls surfaced a dispatch gap -- but the accumulator
               below resolves every goal and INLINES its clause body (never a by-name call), so that hazard is
               gone, and the exclusion had become the row prolog-load-directives-dropped-when-main-exists:
               with any real initialization goal present, every load directive (begin_tests, assertz, dynamic
               setup...) silently never ran -- plunit registered zero tests off exactly this. They now ride the
               chain at their source position, which reproduces load order: directives fire in file order,
               ahead of a trailing initialization(main). */
            if (gt && ((gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval)) {
                if (pl_init_ngoals_acc < PL_INIT_GOALS_MAX) pl_init_goals_acc[pl_init_ngoals_acc++] = gt;
            }
        } else if ((subj->t == TT_FNC || subj->t == TT_NAME || subj->t == TT_QLIT) && subj->v.sval) {
            /* ⭐ row prolog-directive-only-file-fatals-no-main-bb-graph: a BARE `:- Goal.` load directive whose goal is not in prolog_lower.c's callable_with_args allowlist
               (begin_tests/dynamic/use_module/...) is never wrapped into a pj_dir_<N> helper, so it never became an initialization(...) statement and never reached this accumulator --
               the file then had no init goals AND no main/0, `clause` stayed NULL, nothing was registered as main, and the driver aborted with "[IBB] FATAL: main BB graph not found"
               (rc=134, core dump) on a legal program both oracles run fine. A clause is TT_CHOICE here; a directive is TT_FNC/TT_NAME/TT_QLIT, so the two are separable without a name
               test. Accumulating at the directive's own source position preserves load order alongside the wrapped ones, exactly as the initialization branch above documents. */
            if (pl_init_ngoals_acc < PL_INIT_GOALS_MAX) pl_init_goals_acc[pl_init_ngoals_acc++] = subj;
        }
    }
    const tree_t *clause = NULL;
    if (pl_init_ngoals_acc == 0) {
        const tree_t *choice = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, "main/0");
        const tree_t *auto_body = choice ? pl_choice_all_bodies(choice) : NULL;
        if (auto_body) {
            tree_t *cl = ast_node_new(TT_CLAUSE);
            cl->v.sval = (char *) "$auto_main/0"; cl->v.dval = 0.0;
            ast_push(cl, (tree_t *) auto_body);
            clause = cl;
        }
        if (!clause) {
            /* ⭐ row prolog-directive-only-file-fatals-no-main-bb-graph, the DOMINANT arm (ceo measured ~139 of 371 master-suite entries, 37%, are this one class): a CLAUSE-ONLY file
               -- no directives, no main/0 -- left `clause` NULL, so nothing was ever registered as main and the driver ABORTED (rc=134 + core dump) rather than doing what consulting
               the file does. Reference behaviour, measured on swipl for `foo(X,Y) :- X @>= Y.`: rc=0 with NO output. main/0 is SANCTIONED as scrip's entry-point convention
               (ARCH-LANGUAGES.md § ENTRY-POINT CONVENTION) but it is a DEFAULT, not a requirement; synthesising `main :- true.` here makes the default hold for files that never asked
               for one, and keeps the driver's "main BB graph not found" a genuine internal-invariant violation (scrip.c:1817's abort()-vs-return ruling) instead of a reachable state a
               user's legal program can drive it into. */
            tree_t *cl = ast_node_new(TT_CLAUSE);
            cl->v.sval = (char *) "$empty_main/0"; cl->v.dval = 0.0;
            ast_push(cl, pl_synth_qlit("true"));
            clause = cl;
        }
    } else {
        /* Each accumulated goal is resolved and INLINED as its target clause's own body -- never called
           by name -- so a user predicate that happens to share a name with something this function's own
           bookkeeping uses (e.g. a plain `main :- ...` picked up via `:- initialization(main).`) can never
           collide with a live by-name dispatch cell; this mirrors the single-goal path's own pre-existing
           inline-not-call semantics, just threaded across every accumulated goal instead of only the last. */
        /* ⭐ rows prolog-failed-initialization-goal-exits-1-silently + prolog-failed-initialization-goal-
           exits-1-where-swipl-exits-0 + the plunit-registers-zero-tests witness: reference load semantics run
           each directive/initialization goal INDEPENDENTLY -- a failing `:- use_module(...)` warns and loading
           CONTINUES, and a failed initialization goal warns (naming that goal) with exit 0. The old comma
           chain aborted every goal after the first failure (one unsupported use_module emptied the whole
           plunit registry) and exited 1. Each goal now wraps as `(Body ; warn-and-continue)` individually:
           the warning names ITS goal, the goals after it always run, the synthetic clause always succeeds
           (rc 0, matching swipl). Cross-goal backtracking does not exist in the reference semantics -- each
           goal is its own top-level query -- so per-goal isolation is the correct behavior, and no `->` cut
           is needed: a failing Body simply falls to the warn arm. */
        const tree_t *body = NULL;
        for (int i = pl_init_ngoals_acc - 1; i >= 0; i--) {
            const tree_t *b = pl_init_resolve_body(pl_init_goals_acc[i]);
            /* ⛔ WAS `if (!b) continue;` -- A SILENT DROP, and the second half of the directive-only fatal. pl_init_resolve_body() resolves a goal NAME to a user clause's body, which
               is right for `initialization(main)` but returns NULL for a bare directive like `write(hello)` (a builtin owns no clause row) and for `initialization(undefined_pred)`.
               Dropping it lost the only goal in a directive-only file. A goal that resolves to no clause body IS its own body, so it is used verbatim; an unresolvable one then fails
               at run time into the warn-and-continue arm below (warning names the goal, exit 0) instead of vanishing at compile time, which is what both oracles do with an unknown
               procedure. */
            if (!b) b = pl_init_goals_acc[i];
            char msg[256]; char *nm = pl_init_goal_name(pl_init_goals_acc[i]);
            snprintf(msg, sizeof msg, "Warning: initialization goal failed: %s\n", nm ? nm : "?");
            free(nm);
            tree_t *warn = pl_synth_fnc2("write", pl_synth_qlit("user_error"), pl_synth_qlit(strdup(msg)));
            tree_t *wrapped = pl_synth_fnc2(";", (tree_t *) b, pl_synth_fnc2(",", warn, pl_synth_qlit("true")));
            body = body ? pl_synth_fnc2(",", wrapped, (tree_t *) body) : (const tree_t *) wrapped;
        }
        if (body) {
            tree_t *cl = ast_node_new(TT_CLAUSE);
            cl->v.sval = (char *) "$init_chain/0"; cl->v.dval = 0.0;
            ast_push(cl, (tree_t *) body);
            clause = cl;
        }
    }
    if (clause) {
        int bb_idx = lower_pl_clause_graph(clause);
        if (bb_idx >= 0) {
            /* Claim "main/0" in the resolve_bb cache so the later lower_pl_register_all_preds() sweep
               (which walks resolve_pred_table and independently lowers+registers anything not already
               there) skips re-lowering the plain, un-chained "main/0" clause under the same proc_table
               name -- otherwise that second, unaware-of-the-chain row shadows this one under mode-3's
               last-registration-wins main lookup, silently dropping every goal before the last. Only
               needed when a real chain was built here (pl_init_ngoals_acc>0); the ngoals==0 fallback path
               resolves the SAME clause lower_pl_register_all_preds() would build anyway, so the harmless
               pre-existing duplicate there is left alone. */
            if (pl_init_ngoals_acc > 0 && !resolve_bb_lookup("main/0", 0)) resolve_bb_register("main/0", 0, bb_idx);
            if (pl_init_main_pi < 0) {
                pl_init_main_pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pl_init_main_pi].name     = "main";
                g_stage2.proc_table[pl_init_main_pi].proc     = NULL;
                g_stage2.proc_table[pl_init_main_pi].entry_pc = -1;
                g_stage2.proc_table[pl_init_main_pi].nparams  = 0;
                { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++;
                    if (g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; } }
            }
            g_stage2.proc_table[pl_init_main_pi].bb_idx = bb_idx;
        }
    }
    lower_pl_register_all_preds();
    lower_pl_register_dyn_only_preds();
    pl_det_classify_all();
    { static int _zf = -1; if (_zf < 0) { const char *_e = getenv("SCRIP_PL_ZFRAME"); _zf = (_e && *_e == '0') ? 0 : 1; }
      if (_zf) for (int _gi = _pl_bb0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) g_stage2.bbp.table[_gi]->zframe_graph = 1; }
    return &g_stage2;
}
