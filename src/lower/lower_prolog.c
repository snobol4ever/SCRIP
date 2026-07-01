#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; IR_t * tω; } lcx_t;
/*====================================================================================================================================================================================================*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*====================================================================================================================================================================================================*/
static const char * g_pl_nl_arith[] = { "*", "**", "+", "-", "/", "//", "/\\", "<<", ">>", "\\", "\\/", "^", "abs", "acos", "asin", "atan", "ceiling", "cos", "div", "exp", "float",
    "float_fractional_part", "float_integer_part", "floor", "gcd", "integer", "log", "max", "min", "mod", "msb", "rem", "round", "sign", "sin", "sqrt", "tan", "truncate", "xor", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_arith_fn(const char * s, int n) { if (!s || n < 1 || n > 2) return 0; for (int i = 0; g_pl_nl_arith[i]; i++) if (!strcmp(s, g_pl_nl_arith[i])) return 1; return 0; }
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
static int is_builtin_argw(const char * s) { return s && (!strcmp(s, "is") || !strcmp(s, "<") || !strcmp(s, ">") || !strcmp(s, "=<") || !strcmp(s, ">=") || !strcmp(s, "=:=") || !strcmp(s, "=\\=")); }
/*====================================================================================================================================================================================================*/
static IR_t * term(lcx_t * cx, const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_nest(lcx_t * cx, const tree_t * t, int from) {
    if (from == t->n - 1) return term(cx, t->c[from]);
    IR_t * nd = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = 2;
    ir_operand_push(nd, term(cx, t->c[from]));
    ir_operand_push(nd, term_nest(cx, t, from + 1));
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t) {
    if (!t) return NULL;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_MAKELIST: {
        int bar = (t->v.ival == 1 && t->n > 0);
        IR_t * prev;
        if (bar) prev = term(cx, t->c[t->n - 1]);
        else { prev = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(prev).sval = "[]"; }
        for (int i = (bar ? t->n - 2 : t->n - 1); i >= 0; i--) {
            IR_t * e = term(cx, t->c[i]);
            IR_t * c = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(c).sval = "."; IR_LIT(c).ival = 2;
            ir_operand_push(c, e); ir_operand_push(c, prev); prev = c;
        }
        return prev;
    }
    case TT_FNC: {
        if (t->v.sval && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";")) && t->n > 2) return term_nest(cx, t, 0);
        IR_t * nd = build(cx, is_arith_fn(t->v.sval, t->n) ? IR_OP_COUNT : IR_OP_COUNT, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = t->n;
        for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
        return nd;
    }
    default: { IR_t * nd = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
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
        ω_to(gn[i], last_res);
        if (gn[i]->op == IR_OP_COUNT) last_res = gn[i];
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
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail);
            IR_t ** arms = (IR_t **) calloc((size_t) t->n, sizeof(IR_t *));
            IR_t * nbf = ωfail;
            IR_t * bentry = NULL;
            for (int b = t->n - 1; b >= 0; b--) {
                const tree_t * br = t->c[b];
                if (br && br->t == TT_FNC && br->v.sval && !strcmp(br->v.sval, ",")) {
                    IR_t * bg = build(cx, IR_OP_COUNT, γnext, nbf);
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
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail);
            ir_operand_push(nd, term(cx, t->c[0]));
            ir_operand_push(nd, term(cx, t->c[1]));
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
        if (!strcmp(nm, "findall") && t->n == 3) {
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm;
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
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail);
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
        if (!strcmp(nm, "aggregate_all") && t->n == 3) {
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm;
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
            int total = base_n + 2;
            IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = callee;
            bb_goal_state_t * z = (bb_goal_state_t *) calloc(1, sizeof *z);
            z->callee = strdup(callee); z->arity = total; z->nargs = total;
            z->args = (IR_t **) calloc((size_t) total, sizeof(IR_t *));
            for (int i = 0; i < base_n; i++) z->args[i] = term(cx, gt->c[i]);
            z->args[base_n] = term(cx, t->c[1]);
            if (t->n == 3) z->args[base_n + 1] = term(cx, t->c[2]);
            else { IR_t * nil = build(cx, IR_OP_COUNT, NULL, cx->tω); IR_LIT(nil).sval = "[]"; z->args[base_n + 1] = nil; }
            IR_LIT(nd).ival = (long long)(intptr_t) z;
            return nd;
        }
        IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm;
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
        if (is_builtin_exec(nm)) { IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm; return nd; }
        IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).sval = nm;
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
        IR_t * commit = build(cx, IR_OP_COUNT, tentry, ωfail); IR_LIT(commit).ival = (long long)(intptr_t) zi;
        IR_t * gate   = build(cx, IR_OP_COUNT, eentry, ωfail);   IR_LIT(gate).ival   = (long long)(intptr_t) zi;
        IR_t * centry = NULL; IR_t * cfirst = thread1(cx, cnd, commit, gate, &centry);
        IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail); IR_LIT(nd).ival = (long long)(intptr_t) zi;
        ir_operand_push(nd, cfirst);
        zi->cond = centry; zi->then_ = tentry; zi->else_ = eentry;
        zi->cond_root = cfirst; zi->then_root = tfirst; zi->else_root = efirst;
        return nd;
    }
    case TT_UNIFY: {
        IR_t * nd = build(cx, IR_OP_COUNT, γnext, ωfail);
        ir_operand_push(nd, term(cx, t->c[0]));
        ir_operand_push(nd, term(cx, t->c[1]));
        return nd;
    }
    case TT_CUT: return build(cx, IR_OP_COUNT, γnext, ωfail);
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
/*====================================================================================================================================================================================================*/
static int max_var_slot(const tree_t * t, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot(t->c[i], mx);
    return mx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_prolog_clause(const tree_t * clause) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    IR_graph_t * g = IR_alloc(256, IR_LANG_PL);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gconj   = build(&cx, IR_OP_COUNT, succeed, fail);
    int arity = (int) clause->v.dval;
    if (arity < 0) arity = 0;
    if (arity > clause->n) arity = clause->n;
    IR_t * bentry = NULL;
    thread_goals(&cx, clause, arity, clause->n, gconj, fail, &bentry, gconj);
    IR_t * next = bentry ? bentry : gconj;
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
    if (hu) {
        bb_conj_state_t * z = (bb_conj_state_t *)(intptr_t) IR_LIT(gconj).ival;
        if (z) {
            int total = arity + z->ngoals;
            IR_t ** g2 = (IR_t **) calloc((size_t)(total > 0 ? total : 1), sizeof(IR_t *));
            for (int i = 0; i < arity; i++) g2[i] = hu[i];
            for (int i = 0; i < z->ngoals; i++) g2[arity + i] = z->goals[i];
            free(z->goals); z->goals = g2; z->ngoals = total;
        }
        free(hu);
    }
    g->nslots = max_var_slot(clause, arity - 1) + 1;
    if (arity == 0 && !bentry) { g->entry = succeed; g->body_root = succeed; return g; }
    g->entry = next;
    g->body_root = gconj;
    return g;
}
/*====================================================================================================================================================================================================*/
#include <stdio.h>
#include "stage2.h"
#include "../parser/snobol4/scrip_cc.h"
#include "bb_program.h"
#include "../runtime/builtins/resolution.h"
#include "../parser/prolog/term.h"
#include "../parser/prolog/prolog_atom.h"
#include <gc/gc.h>
extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
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
    IR_graph_t *g = IR_alloc(8, IR_LANG_PL);
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
    IR_graph_t *g = IR_alloc(8, IR_LANG_PL);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd = IR_node_alloc(g, IR_OP_COUNT);
    if (!nd) return -1;
    pl_gz_dyniter_state_t *st = (pl_gz_dyniter_state_t *)GC_MALLOC(sizeof *st);
    if (!st) return -1;
    st->functor_atom = prolog_atom_intern(name); st->functor_name = prolog_atom_name(st->functor_atom); st->arity = arity; st->cursor_slot = arity; st->mark_slot = arity + 1;
    IR_LIT(nd).ival = (int64_t)(intptr_t)st;
    nd->γ.node = PSUCC; memcpy(nd->γ.sz, "α", 3); nd->ω.node = PFAIL; memcpy(nd->ω.sz, "α", 3);
    g->entry = nd; g->body_root = nd; g->nslots = arity + 2;
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
                bb_idx = lower_pl_clause_graph(ch);
            } else if (ch->t == TT_CHOICE) {
                if (ch->n == 1) bb_idx = lower_pl_clause_graph(ch->c[0]);
                else            bb_idx = lower_pl_choice_graph(ch);
            }
            if (bb_idx >= 0) resolve_bb_register(key, ar, bb_idx);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* findall/aggregate_all lambda-lift (m3/m4 compound inner goal): when the GOAL of findall(T,G,L)
 * (or aggregate_all) is not a simple single call with all-logicvar args, hoist it into a fresh
 * predicate  '$faN'(SharedVars) :- G.  and rewrite the goal to a plain call '$faN'(SharedVars).
 * SharedVars = distinct vars of the TEMPLATE (their ORIGINAL slots become the call args so the
 * collected bindings flow back; the helper head renumbers them to 0..k-1). The single-callee
 * findall drive box then handles conjunction / list-arg / disjunction goals unchanged. */
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
    if (nhead > 3) return;                       /* findall box passes <=3 callee args */
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
    int bb_idx = lower_pl_clause_graph(cl);
    if (bb_idx < 0) return;                      /* lift failed: leave original goal in place */
    resolve_bb_register(key, nhead, bb_idx);
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
void lower_pl_stage2(const tree_t *prog) {
    pl_ll_prepass();
    pl_dyn_mark_prepass();
    const char *goal_key = NULL;
    char keybuf[128];
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        if (lp_s_int(s, ":lang") != LANG_PL) continue;
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
}
