#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
typedef struct { IR_graph_t * g; IR_t * tω; IR_t * beta; IR_t * cut_ω; } lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_var_name(int slot) {
    static char * cache[1024]; static char buf[24];
    if (slot >= 0 && slot < 1024) { if (!cache[slot]) { snprintf(buf, sizeof buf, "G%d", slot); cache[slot] = strdup(buf); } return cache[slot]; }
    snprintf(buf, sizeof buf, "G%d", slot); return strdup(buf);
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
static const char * g_pl_nl_builtins[] = { "<", "<=", "=..", "=:=", "=<", "==", "=\\=", ">", ">=", "@<", "@=<", "@>", "@>=", "\\==", "abolish", "arg", "assert", "asserta", "assertz", "atom",
    "atom_chars", "atom_codes", "atom_concat", "atom_length", "atom_number", "atom_string", "atomic", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term",
    "downcase_atom", "float", "functor", "ground", "integer", "is", "is_list", "msort", "nb_getval", "nb_setval", "nonvar", "number", "number_string", "numbervars", "plus", "print", "retract",
    "retractall", "sort", "string_chars", "string_codes", "string_concat", "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "term_string", "term_to_atom", "throw",
    "upcase_atom", "var", "write_canonical", "writeln", "writeq", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_builtin_exec(const char * s) { if (!s) return 0; if (is_builtin_visible(s)) return 1; for (int i = 0; g_pl_nl_builtins[i]; i++) if (!strcmp(s, g_pl_nl_builtins[i])) return 1; return 0; }
static int is_builtin_argw(const char * s) { return s && (!strcmp(s, "is") || !strcmp(s, "<") || !strcmp(s, ">") || !strcmp(s, "=<") || !strcmp(s, ">=") || !strcmp(s, "=:=") || !strcmp(s, "=\\=")); }
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, cx->tω); IR_LIT(nd).sval = "$mkc";
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
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
static IR_t * term(lcx_t * cx, const tree_t * t) { return term_e(cx, t, NULL); }
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (t && t->t == TT_VAR) { IR_t * nd = build(cx, IR_VAR_REF, NULL, cx->tω); IR_LIT(nd).sval = pl_var_name((int) t->v.ival); return nd; }
    return term_e(cx, t, entry_out);
}
static IR_t * term_lval(lcx_t * cx, const tree_t * t) { return term_lval_e(cx, t, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_same_functor(const tree_t * a, const tree_t * b) {
    return a && b && a->t == TT_FNC && b->t == TT_FNC && a->n == b->n && a->v.sval && b->v.sval && !strcmp(a->v.sval, b->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    if (pl_same_functor(lt, rt)) {
        IR_t * next = γ; IR_t * first_entry = γ; IR_t * head = NULL;
        for (int i = lt->n - 1; i >= 0; i--) { IR_t * e = NULL; IR_t * u = unify_pair(cx, lt->c[i], rt->c[i], next, ω, &e); next = e ? e : u; head = u; if (i == 0) first_entry = next; }
        if (entry_out) *entry_out = first_entry;
        return head;
    }
    IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γ, ω); IR_LIT(nd).sval = "$unify";
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
    IR_t * next = γtail;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        cx->beta = NULL;
        IR_t * ωeff = (cut_idx >= 0 && i > cut_idx) ? cx->cut_ω : ωbase;
        IR_t * nd = goal(cx, gl[i], next, ωeff, &e);
        gn[i] = nd; en[i] = e ? e : nd; rz[i] = cx->beta;
        next = en[i];
    }
    IR_t * last_res = ωbase; int last_res_beta = 0;
    for (int i = 0; i < ng; i++) {
        if (cut_idx >= 0 && i == cut_idx + 1) { last_res = cx->cut_ω; last_res_beta = 0; }
        if (gn[i] && gn[i]->op == IR_GOTO) { if (last_res_beta) lc_γ_to_β(gn[i], last_res); else γ_to(gn[i], last_res); }
        else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else ω_to(gn[i], last_res);
        if (rz[i]) { last_res = rz[i]; last_res_beta = (rz[i]->op == IR_CALL); }
    }
    free(rz);
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
    if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "**")) return "pow"; if (!strcmp(s, "^")) return "pow";
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
        if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "rem")) return "rem"; if (!strcmp(s, "**")) return "pow"; if (!strcmp(s, "^")) return "pow";
        if (!strcmp(s, "min")) return "min"; if (!strcmp(s, "max")) return "max"; if (!strcmp(s, "gcd")) return "gcd"; if (!strcmp(s, "xor")) return "xor";
        if (!strcmp(s, ">>")) return "shr"; if (!strcmp(s, "<<")) return "shl"; if (!strcmp(s, "/\\")) return "band"; if (!strcmp(s, "\\/")) return "bor";
        return NULL;
    }
    if (ar == 1) {
        if (!strcmp(s, "-")) return "neg"; if (!strcmp(s, "+")) return "pos"; if (!strcmp(s, "abs")) return "abs"; if (!strcmp(s, "sign")) return "sign";
        if (!strcmp(s, "truncate")) return "trunc"; if (!strcmp(s, "integer")) return "intg"; if (!strcmp(s, "float")) return "flt";
        if (!strcmp(s, "floor")) return "floor"; if (!strcmp(s, "ceiling")) return "ceil"; if (!strcmp(s, "round")) return "round";
        if (!strcmp(s, "sqrt")) return "sqrt"; if (!strcmp(s, "msb")) return "msb"; if (!strcmp(s, "sin")) return "sin"; if (!strcmp(s, "cos")) return "cos";
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
        IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
        if (entry_out) *entry_out = nd;
        return nd;
    }
    if (t && t->t == TT_FNC && t->v.sval && (t->n == 1 || t->n == 2) && pl_ax_suffix(t->v.sval, t->n)) {
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, t->n));
        IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
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
    IR_t * tn = thread1(cx, T, γnext, ωfail, &te);
    IR_t * en = E ? thread1(cx, E, γnext, ωfail, &ee) : NULL;
    IR_t * cω = E ? (ee ? ee : en) : ωfail;
    IR_t * cn = thread1(cx, C, (te ? te : tn), cω, &ce);
    if (entry_out) *entry_out = ce ? ce : cn;
    cx->beta = NULL;
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
            bb_ite_state_t * zi = (bb_ite_state_t *) calloc(1, sizeof *zi);
            IR_t * es = build(cx, IR_SUCCEED, γnext, ωfail);
            IR_t * tf = build(cx, IR_FAIL, γnext, ωfail);
            IR_t * commit = build(cx, IR_OP_COUNT, tf, ωfail); IR_LIT(commit).ival = (long long)(intptr_t) zi;
            IR_t * gate   = build(cx, IR_OP_COUNT, es, ωfail);   IR_LIT(gate).ival   = (long long)(intptr_t) zi;
            IR_t * u = build(cx, IR_OP_COUNT, commit, gate);
            ir_operand_push(u, term(cx, t->c[0]));
            ir_operand_push(u, term(cx, t->c[1]));
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).ival = (long long)(intptr_t) zi;
            ir_operand_push(nd, u);
            zi->cond = u; zi->then_ = tf; zi->else_ = es;
            zi->cond_root = u; zi->then_root = tf; zi->else_root = es;
            return nd;
        }
        if (!strcmp(nm, "catch") && t->n == 3) {
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail);
            bb_catch_state_t * zc = (bb_catch_state_t *) calloc(1, sizeof *zc);
            IR_graph_t * sub = IR_alloc(256);
            lcx_t scx; scx.g = sub; scx.tω = NULL; scx.beta = NULL;
            IR_t * ssucc = build(&scx, IR_SUCCEED, NULL, NULL);
            IR_t * sfail = build(&scx, IR_FAIL, NULL, NULL);
            const tree_t * gone[1] = { t->c[0] };
            tree_t blkw; memset(&blkw, 0, sizeof blkw); blkw.n = 1; blkw.c = (tree_t **) gone;
            IR_t * sentry = NULL;
            thread_goals(&scx, &blkw, 0, 1, ssucc, sfail, &sentry, NULL);
            sub->entry = sentry ? sentry : ssucc;
            zc->goal_g = sub;
            IR_t * cnode = term(cx, t->c[1]);
            zc->catcher = cnode;
            ir_operand_push(nd, cnode);
            IR_graph_t * rsub = IR_alloc(256);
            lcx_t rcx; rcx.g = rsub; rcx.tω = NULL; rcx.beta = NULL;
            IR_t * rsucc = build(&rcx, IR_SUCCEED, NULL, NULL);
            IR_t * rfail = build(&rcx, IR_FAIL, NULL, NULL);
            const tree_t * rone[1] = { t->c[2] };
            tree_t rblkw; memset(&rblkw, 0, sizeof rblkw); rblkw.n = 1; rblkw.c = (tree_t **) rone;
            IR_t * rentry = NULL;
            thread_goals(&rcx, &rblkw, 0, 1, rsucc, rfail, &rentry, NULL);
            rsub->entry = rentry ? rentry : rsucc;
            zc->rec_g = rsub;
            IR_LIT(nd).ival = (long long)(intptr_t) zc;
            return nd;
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
                IR_t * hnew = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(hnew).sval = "$findall_new";
                IR_t * res = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(res).sval = op;
                IR_t * res_e = NULL; IR_t * res_lv = term_lval_e(cx, res_ast, &res_e);
                lc_γ_to(res_lv, res); lc_ω_to(res_lv, ωfail);
                ir_operand_push(res, hnew); ir_operand_push(res, res_lv);
                IR_t * addnd = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(addnd).sval = "$findall_add";
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
                    if (goal_beta->op == IR_CALL) { lc_γ_to_β(forced_fail, goal_beta); lc_ω_to_β(forced_fail, goal_beta); }
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
            IR_t * call = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(call).sval = "$write";
            IR_t * ae = NULL; IR_t * a = term_e(cx, t->c[0], &ae);
            lc_γ_to(a, call); lc_ω_to(a, ωfail);
            ir_operand_push(call, a);
            if (entry_out) *entry_out = ae ? ae : a;
            return call;
        }
        if (!strcmp(nm, "is") && t->n == 2) {
            IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$is_v";
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
            IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(nd).sval = strdup(nb);
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
            IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(nd).sval = strdup(nb);
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
            IR_t * hnew = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(hnew).sval = "$findall_new";
            IR_t * res = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(res).sval = "$findall_result";
            ir_operand_push(res, hnew);
            IR_t * bindnd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(bindnd).sval = "$unify";
            IR_t * lst_e = NULL; IR_t * lst_lv = term_lval_e(cx, lst_ast, &lst_e);
            lc_γ_to(res, lst_e ? lst_e : lst_lv); lc_ω_to(lst_lv, ωfail);
            lc_γ_to(lst_lv, bindnd); lc_ω_to(lst_lv, ωfail);
            ir_operand_push(bindnd, lst_lv); ir_operand_push(bindnd, res);
            IR_t * addnd = build(cx, IR_CALL_BUILTIN_PROLOG, NULL, ωfail); IR_LIT(addnd).sval = "$findall_add";
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
                if (goal_beta->op == IR_CALL) { lc_γ_to_β(forced_fail, goal_beta); lc_ω_to_β(forced_fail, goal_beta); }
                else { lc_γ_to(forced_fail, goal_beta); lc_ω_to(forced_fail, goal_beta); }
            } else forced_fail = build(cx, IR_GOTO, res, res);
            lc_γ_to(addnd, forced_fail);
            lc_γ_to(hnew, goal_entry ? goal_entry : goal_nd);
            if (entry_out) *entry_out = hnew;
            cx->beta = NULL;
            return hnew;
        }
        if (!strcmp(nm, "abolish") && t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "/") && t->c[0]->n == 2) {
            IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(nd).sval = "$abolish";
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
        { static const struct { const char * nm; int ar; const char * tgt; } g_pl_det_tab[] = {
              { "sort", 2, "$sort" }, { "msort", 2, "$msort" }, { "numbervars", 3, "$numbervars" }, { "copy_term", 2, "$copy_term" },
              { "char_type", 2, "$char_type" }, { "writeq", 1, "$writeq" }, { "print", 1, "$print" }, { "write_canonical", 1, "$write_canonical" },
              { "functor", 3, "$functor" }, { "arg", 3, "$arg" }, { "=..", 2, "$univ" },
              { "compound", 1, "$tt_compound" }, { "callable", 1, "$tt_callable" }, { "ground", 1, "$tt_ground" }, { "is_list", 1, "$tt_is_list" },
              { "var", 1, "$tt_var" }, { "nonvar", 1, "$tt_nonvar" }, { "atom", 1, "$tt_atom" }, { "number", 1, "$tt_number" },
              { "integer", 1, "$tt_integer" }, { "float", 1, "$tt_float" }, { "atomic", 1, "$tt_atomic" },
              { "==", 2, "$atop_eq" }, { "\\==", 2, "$atop_ne" },
              { "format", 1, "$format1" }, { "format", 2, "$format2" },
              { "atom_string", 2, "$aop_atom_string" }, { "number_string", 2, "$aop_number_string" }, { "atom_number", 2, "$aop_atom_number" },
              { "string_upper", 2, "$aop_string_upper" }, { "string_lower", 2, "$aop_string_lower" },
              { "string_concat", 3, "$aop_string_concat" }, { "string_length", 2, "$aop_string_length" }, { "string_to_atom", 2, "$aop_string_to_atom" },
              { "atomic_list_concat", 2, "$aop_atomic_list_concat" }, { "atomic_list_concat", 3, "$aop_atomic_list_concat" },
              { "concat_atom", 2, "$aop_concat_atom" }, { "concat_atom", 3, "$aop_concat_atom" },
              { "term_string", 2, "$term_string" }, { "term_to_atom", 2, "$term_string" },
              { "nb_setval", 2, "$nb_setval" }, { "nb_getval", 2, "$nb_getval" },
              { "assertz", 1, "$dyn_assertz" }, { "assert", 1, "$dyn_assertz" }, { "asserta", 1, "$dyn_asserta" }, { "retract", 1, "$retract" },
              { "@<", 2, "$atop_lt" }, { "@=<", 2, "$atop_le" }, { "@>", 2, "$atop_gt" }, { "@>=", 2, "$atop_ge" },
              { 0, 0, 0 } };
          for (int di = 0; g_pl_det_tab[di].nm; di++) if (!strcmp(nm, g_pl_det_tab[di].nm) && t->n == g_pl_det_tab[di].ar) {
              IR_t * nd = build(cx, IR_CALL_BUILTIN_PROLOG, γnext, ωfail); IR_LIT(nd).sval = g_pl_det_tab[di].tgt;
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
        if (is_builtin_exec(nm)) {
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = t->n;
            IR_t * sav = cx->tω; if (is_builtin_argw(nm)) cx->tω = ωfail;
            for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
            cx->tω = sav;
            return nd;
        }
        if (!strcmp(nm, "phrase") && (t->n == 2 || t->n == 3)) {
            const tree_t * gt = t->c[0];
            const char * callee = (gt && gt->v.sval) ? gt->v.sval : "?";
            int base_n = (gt && gt->t == TT_FNC) ? gt->n : 0;
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = strdup(callee);
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
        IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = strdup(nm);
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
        if (!strcmp(nm, "nl")) {
            IR_t * call = build(cx, IR_CALL, γnext, ωfail); IR_LIT(call).sval = "write";
            IR_t * a = build(cx, IR_LIT_STRING, call, ωfail); IR_LIT(a).sval = "";
            ir_operand_push(call, a);
            if (entry_out) *entry_out = a;
            return call;
        }
        if (is_builtin_exec(nm)) { IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm; return nd; }
        IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = strdup(nm);
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
    case TT_PROGRAM: {
        IR_t * bg = build(cx, IR_OP_COUNT, γnext, ωfail);
        IR_t * e = NULL;
        thread_goals(cx, t, 0, t->n, bg, ωfail, &e, bg);
        if (entry_out) *entry_out = e ? e : bg;
        return bg;
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
        IR_t * u = build(cx, IR_CALL_BUILTIN_PROLOG, next, ωfail); IR_LIT(u).sval = "$unify";
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
IR_graph_t * lower_prolog_clause(const tree_t * clause) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    IR_graph_t * g = IR_alloc(256);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL;
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
    if (arity == 0 && !bentry) { g->entry = succeed; g->body_root = NULL; return g; }
    g->entry = next;
    g->body_root = NULL;
    return g;
}
#include <stdio.h>
#include "stage2.h"
#include "../parser/snobol4/scrip_cc.h"
#include "bb_program.h"
#include "../runtime/builtins/resolution.h"
#include "../parser/prolog/term.h"
#include "../parser/prolog/prolog_atom.h"
#include <gc/gc.h>
extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
extern int ir_is_generator_kind(IR_e t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_pred_graph_new(const tree_t * ch, int arity) {
    const tree_t * one[1]; const tree_t ** clauses; int nc;
    if (ch->t == TT_CLAUSE) { one[0] = ch; clauses = one; nc = 1; }
    else if (ch->t == TT_CHOICE && ch->n >= 1) { clauses = (const tree_t **) ch->c; nc = ch->n; }
    else return -1;
    if (arity < 0) arity = 0;
    IR_graph_t * g = IR_alloc(1024);
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL;
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) calloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * dj = build(&cx, IR_DISJUNCTION, succeed, fail);
    IR_t * mk = build(&cx, IR_CALL_BUILTIN_PROLOG, NULL, fail); IR_LIT(mk).sval = "$trail_mark";
    IR_t ** centry = (IR_t **) calloc((size_t) nc, sizeof(IR_t *));
    IR_t ** uw = (IR_t **) calloc((size_t)(nc + 1), sizeof(IR_t *));
    IR_t * uwf = build(&cx, IR_CALL_BUILTIN_PROLOG, fail, fail); IR_LIT(uwf).sval = "$trail_unwind"; ir_operand_push(uwf, mk);
    uw[nc] = uwf;
    cx.cut_ω = uwf;
    int maxlocal = -1;
    for (int k = nc - 1; k >= 0; k--) {
        IR_t * next_fail = uw[k + 1];
        IR_t * ml = build(&cx, IR_MOVE_LABEL, succeed, fail);
        IR_t * ce = NULL; IR_t * redo = NULL;
        lower_pl_clause_into(&cx, clauses[k], arity, ml, next_fail, &ce, &redo);
        IR_t * ab = redo ? redo : next_fail;
        IR_LIT(ml).ival = (ab && (ir_is_generator_kind(ab->op) || ab->op == IR_CALL)) ? 1 : 0;
        ir_operand_push(ml, ab); ir_operand_push(ml, dj); ir_operand_push(ml, NULL);
        centry[k] = ce;
        if (k > 0) { IR_t * u = build(&cx, IR_CALL_BUILTIN_PROLOG, ce, fail); IR_LIT(u).sval = "$trail_unwind"; ir_operand_push(u, mk); uw[k] = u; }
        maxlocal = max_var_slot(clauses[k], maxlocal);
    }
    lc_γ_to(mk, centry[0]);
    g->entry = mk;
    g->body_root = dj;
    g->nslots = arity + (maxlocal + 1) + nc + 8;
    g->resume_slot = 0;
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
    IR_graph_t **bodies = (IR_graph_t **)GC_MALLOC((size_t)n * sizeof(IR_graph_t *));
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
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd = IR_node_alloc(g, IR_OP_COUNT);
    if (!nd) return -1;
    bb_choice_state_t *zc = (bb_choice_state_t *)GC_MALLOC(sizeof *zc);
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
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.beta = NULL; cx.cut_ω = NULL;
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) calloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gen = build(&cx, IR_CALL_BUILTIN_GEN, succeed, fail); IR_LIT(gen).sval = "$dyn_iter";
    IR_t * nmop = build(&cx, IR_LIT_STRING, NULL, NULL); IR_LIT(nmop).sval = strdup(name);
    ir_operand_push(gen, nmop);
    IR_t * prev = nmop;
    for (int i = 0; i < arity; i++) {
        IR_t * vr = build(&cx, IR_VAR_REF, NULL, NULL); IR_LIT(vr).sval = pl_param_name(i);
        lc_γ_to(prev, vr); prev = vr; ir_operand_push(gen, vr);
    }
    lc_γ_to(prev, gen);
    g->entry = nmop; g->body_root = NULL;
    return bb_program_add(&g_stage2.bbp, g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_pl_register_all_preds(void) {
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
            int bb_idx = -1;
            if (dyn) {
                const char *slash2 = strrchr(key, '/');
                static char nmbuf[200]; int kl = slash2 ? (int)(slash2 - key) : (int)strlen(key); if (kl > 199) kl = 199; memcpy(nmbuf, key, kl); nmbuf[kl] = 0;
                bb_idx = lower_pl_dyniter_graph(nmbuf, ar);
            } else if (ch->t == TT_CLAUSE) {
                bb_idx = lower_pl_pred_graph_new(ch, ar);
            } else if (ch->t == TT_CHOICE) {
                bb_idx = lower_pl_pred_graph_new(ch, ar);
            }
            if (bb_idx >= 0) {
                resolve_bb_register(key, ar, bb_idx);
                {
                    static char nmp[200]; int kl2 = slash ? (int)(slash - key) : (int)strlen(key); if (kl2 > 199) kl2 = 199; memcpy(nmp, key, kl2); nmp[kl2] = 0;
                    int pi = stage2_proc_grow(&g_stage2);
                    g_stage2.proc_table[pi].name         = strdup(nmp);
                    g_stage2.proc_table[pi].proc         = NULL;
                    g_stage2.proc_table[pi].entry_pc     = -1;
                    g_stage2.proc_table[pi].bb_idx       = bb_idx;
                    g_stage2.proc_table[pi].nparams      = ar;
                    g_stage2.proc_table[pi].is_generator = 1;
                }
            }
        }
    }
}
extern tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out);
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
    int bb_idx = lower_pl_pred_graph_new(cl, nhead);
    if (bb_idx < 0) return;
    resolve_bb_register(key, nhead, bb_idx);
    { int pi = stage2_proc_grow(&g_stage2);
      g_stage2.proc_table[pi].name         = strdup(nm);
      g_stage2.proc_table[pi].proc         = NULL;
      g_stage2.proc_table[pi].entry_pc     = -1;
      g_stage2.proc_table[pi].bb_idx       = bb_idx;
      g_stage2.proc_table[pi].nparams      = nhead;
      g_stage2.proc_table[pi].is_generator = 1; }
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
stage2_t *lower_pl_stage2(const tree_t *prog) {
    pl_register_program(&g_stage2, prog);
    pl_ll_prepass();
    pl_dyn_mark_prepass();
    const char *goal_key = NULL;
    char keybuf[128];
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME) && gt->v.sval) {
                snprintf(keybuf, sizeof keybuf, "%s/0", gt->v.sval);
                goal_key = keybuf;
            } else if (gt && gt->t == TT_FNC && gt->v.sval) {
                snprintf(keybuf, sizeof keybuf, "%s/%d", gt->v.sval, gt->n);
                goal_key = keybuf;
            }
        }
    }
    if (!goal_key) goal_key = "main/0";
    const tree_t *choice = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, goal_key);
    const tree_t *clause = NULL;
    if (choice) {
        if (choice->t == TT_CLAUSE) clause = choice;
        else if (choice->t == TT_CHOICE && choice->n >= 1) clause = choice->c[0];
    }
    if (clause) {
        int bb_idx = lower_pl_clause_graph(clause);
        if (bb_idx >= 0) {
            int pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pi].name     = "main";
            g_stage2.proc_table[pi].proc     = NULL;
            g_stage2.proc_table[pi].entry_pc = -1;
            g_stage2.proc_table[pi].bb_idx   = bb_idx;
            g_stage2.proc_table[pi].nparams  = 0;
        }
    }
    lower_pl_register_all_preds();
    return &g_stage2;
}
