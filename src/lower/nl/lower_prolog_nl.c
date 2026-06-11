#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"
#include "IR.h"
#include "IR_interp_state.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; IR_t * tω; } lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * g_pl_nl_arith[] = { "*", "**", "+", "-", "/", "//", "/\\", "<<", ">>", "\\", "\\/", "^", "abs", "acos", "asin", "atan", "ceiling", "cos", "div", "exp", "float",
    "float_fractional_part", "float_integer_part", "floor", "gcd", "integer", "log", "max", "min", "mod", "msb", "rem", "round", "sign", "sin", "sqrt", "tan", "truncate", "xor", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_arith_fn(const char * s, int n) { if (!s || n < 1 || n > 2) return 0; for (int i = 0; g_pl_nl_arith[i]; i++) if (!strcmp(s, g_pl_nl_arith[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γα_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ωβ_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(cx->g, op); γα_to(nd, γ); ωβ_to(nd, ω); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_builtin_visible(const char * s) { return s && (!strcmp(s, "write") || !strcmp(s, "nl") || !strcmp(s, "format") || !strcmp(s, "aggregate_all")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * g_pl_nl_builtins[] = { "<", "<=", "=..", "=:=", "=<", "==", "=\\=", ">", ">=", "@<", "@=<", "@>", "@>=", "\\==", "abolish", "arg", "assert", "asserta", "assertz", "atom",
    "atom_chars", "atom_codes", "atom_concat", "atom_length", "atom_number", "atom_string", "atomic", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term",
    "downcase_atom", "float", "functor", "ground", "integer", "is", "is_list", "msort", "nb_getval", "nb_setval", "nonvar", "number", "number_string", "numbervars", "plus", "print", "retract",
    "retractall", "sort", "string_chars", "string_codes", "string_concat", "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "term_string", "term_to_atom", "throw",
    "upcase_atom", "var", "write_canonical", "writeln", "writeq", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_builtin_exec(const char * s) { if (!s) return 0; if (is_builtin_visible(s)) return 1; for (int i = 0; g_pl_nl_builtins[i]; i++) if (!strcmp(s, g_pl_nl_builtins[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_nest(lcx_t * cx, const tree_t * t, int from) {
    if (from == t->n - 1) return term(cx, t->c[from]);
    IR_t * nd = build(cx, IR_STRUCT, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = 2;
    ir_operand_push(nd, term(cx, t->c[from]));
    ir_operand_push(nd, term_nest(cx, t, from + 1));
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t) {
    if (!t) return NULL;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = build(cx, IR_ATOM, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_LOGICVAR, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_MAKELIST: {
        IR_t * prev = build(cx, IR_ATOM, NULL, cx->tω); IR_LIT(prev).sval = "[]";
        for (int i = t->n - 1; i >= 0; i--) {
            IR_t * e = term(cx, t->c[i]);
            IR_t * c = build(cx, IR_STRUCT, NULL, cx->tω); IR_LIT(c).sval = "."; IR_LIT(c).ival = 2;
            ir_operand_push(c, e); ir_operand_push(c, prev); prev = c;
        }
        return prev;
    }
    case TT_FNC: {
        if (t->v.sval && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";")) && t->n > 2) return term_nest(cx, t, 0);
        IR_t * nd = build(cx, is_arith_fn(t->v.sval, t->n) ? IR_ARITH : IR_STRUCT, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = t->n;
        for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
        return nd;
    }
    default: { IR_t * nd = build(cx, IR_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int collect_conj(const tree_t * t, const tree_t ** out, int n, int cap) {
    if (!t) return n;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",")) { for (int i = 0; i < t->n; i++) n = collect_conj(t->c[i], out, n, cap); return n; }
    if (n < cap) out[n++] = t;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * thread_goals(lcx_t * cx, const tree_t * blk, int from, int to, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t * conj_owner) {
    const tree_t * gl[1024]; int ng = 0;
    for (int i = from; i < to; i++) ng = collect_conj(blk->c[i], gl, ng, 1024);
    IR_t ** gn = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** en = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t * next = γtail;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        IR_t * nd = goal(cx, gl[i], next, ωbase, &e);
        gn[i] = nd; en[i] = e ? e : nd;
        next = en[i];
    }
    IR_t * last_res = ωbase;
    for (int i = 0; i < ng; i++) {
        ωβ_to(gn[i], last_res);
        if (gn[i]->op == IR_GOAL) last_res = gn[i];
    }
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
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return build(cx, IR_SUCCEED, γnext, ωfail);
    switch (t->t) {
    case TT_FNC: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, ";") && t->n >= 2) {
            IR_t * nd = build(cx, IR_DISJ, γnext, ωfail);
            IR_t ** arms = (IR_t **) calloc((size_t) t->n, sizeof(IR_t *));
            IR_t * nbf = ωfail;
            IR_t * bentry = NULL;
            for (int b = t->n - 1; b >= 0; b--) {
                const tree_t * br = t->c[b];
                if (br && br->t == TT_FNC && br->v.sval && !strcmp(br->v.sval, ",")) {
                    IR_t * bg = build(cx, IR_GCONJ, γnext, nbf);
                    thread_goals(cx, br, 0, br->n, bg, nbf, &bentry, bg);
                    arms[b] = bg;
                    nbf = bg;
                } else {
                    IR_t * e = NULL;
                    IR_t * bn = goal(cx, br, γnext, nbf, &e);
                    bentry = e ? e : bn;
                    arms[b] = bn;
                    nbf = bn;
                }
            }
            bb_operand_aux_set(cx->g, nd, arms, t->n);
            free(arms);
            if (entry_out) *entry_out = bentry;
            return nd;
        }
        if (!strcmp(nm, "=") && t->n == 2) {
            IR_t * nd = build(cx, IR_UNIFY, γnext, ωfail);
            ir_operand_push(nd, term(cx, t->c[0]));
            ir_operand_push(nd, term(cx, t->c[1]));
            return nd;
        }
        if (!strcmp(nm, "findall") && t->n == 3) {
            IR_t * nd = build(cx, IR_BUILTIN, γnext, ωfail); IR_LIT(nd).sval = nm;
            bb_findall_state_t * fs = (bb_findall_state_t *) calloc(1, sizeof *fs);
            fs->tmpl = term(cx, t->c[0]);
            IR_graph_t * sub = IR_alloc(256, IR_LANG_PL);
            lcx_t scx; scx.g = sub; scx.tω = NULL;
            IR_t * ssucc = build(&scx, IR_SUCCEED, NULL, NULL);
            IR_t * sfail = build(&scx, IR_FAIL, NULL, NULL);
            const tree_t * gone[1] = { t->c[1] };
            tree_t blkw; memset(&blkw, 0, sizeof blkw); blkw.n = 1; blkw.c = (tree_t **) gone;
            IR_t * sentry = NULL;
            thread_goals(&scx, &blkw, 0, 1, ssucc, sfail, &sentry, NULL);
            sub->entry = sentry ? sentry : ssucc;
            fs->gcfg = sub;
            fs->result = term(cx, t->c[2]);
            fs->goal_node = nd;
            IR_LIT(nd).ival = (long long)(intptr_t) fs;
            return nd;
        }
        if (!strcmp(nm, "catch") && t->n == 3) {
            IR_t * nd = build(cx, IR_CATCH, γnext, ωfail);
            bb_catch_state_t * zc = (bb_catch_state_t *) calloc(1, sizeof *zc);
            IR_graph_t * sub = IR_alloc(256, IR_LANG_PL);
            lcx_t scx; scx.g = sub; scx.tω = NULL;
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
            IR_graph_t * rsub = IR_alloc(256, IR_LANG_PL);
            lcx_t rcx; rcx.g = rsub; rcx.tω = NULL;
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
        if (is_builtin_exec(nm)) {
            IR_t * nd = build(cx, IR_BUILTIN, γnext, ωfail); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = t->n;
            IR_t * sav = cx->tω; if (!is_builtin_visible(nm)) cx->tω = ωfail;
            for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
            cx->tω = sav;
            return nd;
        }
        IR_t * nd = build(cx, IR_GOAL, γnext, ωfail); IR_LIT(nd).sval = nm;
        bb_goal_state_t * z = (bb_goal_state_t *) calloc(1, sizeof *z);
        z->callee = strdup(nm); z->arity = t->n; z->nargs = t->n;
        z->args = (IR_t **) calloc((t->n ? (size_t) t->n : 1), sizeof(IR_t *));
        for (int i = 0; i < t->n; i++) z->args[i] = term(cx, t->c[i]);
        IR_LIT(nd).ival = (long long)(intptr_t) z;
        return nd;
    }
    case TT_QLIT: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, "true"))  return build(cx, IR_SUCCEED, γnext, ωfail);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_FAIL, γnext, ωfail);
        if (is_builtin_exec(nm)) { IR_t * nd = build(cx, IR_BUILTIN, γnext, ωfail); IR_LIT(nd).sval = nm; return nd; }
        IR_t * nd = build(cx, IR_GOAL, γnext, ωfail); IR_LIT(nd).sval = nm;
        bb_goal_state_t * z = (bb_goal_state_t *) calloc(1, sizeof *z);
        z->callee = strdup(nm); z->arity = 0; z->nargs = 0;
        z->args = (IR_t **) calloc(1, sizeof(IR_t *));
        IR_LIT(nd).ival = (long long)(intptr_t) z;
        return nd;
    }
    case TT_IF: {
        const tree_t * cnd = (t->n > 0) ? t->c[0] : NULL; const tree_t * thn = (t->n > 1) ? t->c[1] : NULL; const tree_t * els = (t->n > 2) ? t->c[2] : NULL;
        bb_ite_state_t * zi = (bb_ite_state_t *) calloc(1, sizeof *zi);
        IR_t * eentry = NULL; IR_t * efirst = els ? thread1(cx, els, γnext, ωfail, &eentry) : NULL;
        if (!els) eentry = ωfail;
        IR_t * tentry = NULL; IR_t * tfirst = thread1(cx, thn, γnext, ωfail, &tentry);
        IR_t * commit = build(cx, IR_ITE_COMMIT, tentry, ωfail); IR_LIT(commit).ival = (long long)(intptr_t) zi;
        IR_t * gate   = build(cx, IR_ITE_GATE, eentry, ωfail);   IR_LIT(gate).ival   = (long long)(intptr_t) zi;
        IR_t * centry = NULL; IR_t * cfirst = thread1(cx, cnd, commit, gate, &centry);
        IR_t * nd = build(cx, IR_ITE, γnext, ωfail); IR_LIT(nd).ival = (long long)(intptr_t) zi;
        ir_operand_push(nd, cfirst);
        zi->cond = centry; zi->then_ = tentry; zi->else_ = eentry;
        zi->cond_root = cfirst; zi->then_root = tfirst; zi->else_root = efirst;
        return nd;
    }
    case TT_UNIFY: {
        IR_t * nd = build(cx, IR_UNIFY, γnext, ωfail);
        ir_operand_push(nd, term(cx, t->c[0]));
        ir_operand_push(nd, term(cx, t->c[1]));
        return nd;
    }
    case TT_CUT: return build(cx, IR_CUT, γnext, ωfail);
    default: return build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_prolog(const tree_t * prog) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PL);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    const tree_t * clause = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d->t == TT_CHOICE && d->v.sval && !strcmp(d->v.sval, "main/0") && d->n > 0) { clause = d->c[0]; break; }
    }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gconj   = build(&cx, IR_GCONJ, succeed, fail);
    if (!clause) { IR_LIT(gconj).ival = (long long)(intptr_t) calloc(1, sizeof(bb_conj_state_t)); g->entry = gconj; return g; }
    int arity = (int) clause->v.dval;
    IR_t * entry = NULL;
    thread_goals(&cx, clause, arity, clause->n, gconj, fail, &entry, gconj);
    g->entry = entry ? entry : gconj;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_prolog_nl_clause(const tree_t * clause) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    IR_graph_t * g = IR_alloc(256, IR_LANG_PL);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gconj   = build(&cx, IR_GCONJ, succeed, fail);
    int arity = (int) clause->v.dval;
    if (arity < 0) arity = 0;
    if (arity > clause->n) arity = clause->n;
    IR_t * bentry = NULL;
    thread_goals(&cx, clause, arity, clause->n, gconj, fail, &bentry, gconj);
    IR_t * next = bentry ? bentry : gconj;
    for (int i = arity - 1; i >= 0; i--) {
        const tree_t * h = clause->c[i];
        if (h && h->t == TT_VAR && (int) h->v.ival == i) continue;
        IR_t * u = build(&cx, IR_UNIFY, next, fail);
        IR_t * lv = build(&cx, IR_LOGICVAR, NULL, NULL); IR_LIT(lv).ival = i;
        ir_operand_push(u, lv);
        ir_operand_push(u, term(&cx, h));
        next = u;
    }
    g->entry = next;
    return g;
}
