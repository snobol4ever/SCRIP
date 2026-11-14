#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/g_lower_rk.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#include "ir/ct_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_find_type_decl(const tree_t * prog, const char * name) {
    if (!prog || !name) return NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * tn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (tn && !strcmp(tn, name)) return d;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_gath_var(const char * pfx, int id, int decl) {
    char nm[strlen(pfx) + 24];
    snprintf(nm, sizeof nm, "%s%d", pfx, id);
    tree_t * v = leaf_sval2(TT_VAR, intern(nm));
    if (decl) v->slen |= 4;
    return v;
}
tree_t * rk_gath_asg(tree_t * lhs, tree_t * rhs) { tree_t * a = ast_node_new(TT_ASSIGN); ast_push(a, lhs); ast_push(a, rhs); return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_gather_pump(tree_t * gcall, tree_t * limit) {
    int id = ++g_lower_rk.rk_gather_ctr;
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = gcall->line;
    ast_push(seq, rk_gath_asg(rk_gath_var("@__pump", id, 1), rk_fnc2("__rk_undef", NULL, NULL)));
    if (limit) { ast_push(seq, rk_gath_asg(rk_gath_var("$__plim", id, 1), limit)); ast_push(seq, rk_gath_asg(rk_gath_var("$__pcnt", id, 1), rk_ti(0))); }
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    { tree_t * mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_gath_var("@__pump", id, 0)); ast_push(mc, leaf_sval2(TT_QLIT, "push")); ast_push(mc, rk_gath_var("$__pv", id, 0)); ast_push(body, mc); }
    if (limit) {
        ast_push(body, rk_gath_asg(rk_gath_var("$__pcnt", id, 0), rk_t2(TT_ADD, rk_gath_var("$__pcnt", id, 0), rk_ti(1))));
        tree_t * brk = ast_node_new(TT_SEQ_EXPR);
        ast_push(brk, ast_node_new(TT_LOOP_BREAK));
        tree_t * cond = ast_node_new(TT_IF);
        ast_push(cond, rk_t2(TT_GE, rk_gath_var("$__pcnt", id, 0), rk_gath_var("$__plim", id, 0)));
        ast_push(cond, brk);
        ast_push(body, cond);
    }
    tree_t * it = ast_node_new(TT_ITERATE);
    { char vn[48]; snprintf(vn, sizeof vn, "$__pv%d", id); it->v.sval = (char *) intern(vn); }
    ast_push(it, gcall);
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = gcall->line;
    ast_push(ev, it);
    ast_push(ev, body);
    ast_push(seq, ev);
    ast_push(seq, rk_fnc2("__rk_arr_values", rk_gath_var("@__pump", id, 0), NULL));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gath_eager_meth(const char * m) {
    static const char ms[][12] = { "elems", "sum", "join", "list", "Array", "List", "Seq", "eager", "cache", "flat", "reverse", "sort", "min", "max", "minmax", "unique", "Str", "gist", "perl", "raku",
        "end", "keys", "values", "kv", "pairs", "antipairs", "pick", "roll", "rotate", "squish", "tail", "Bool", "so", "not", "Numeric", "Int", "contains", "starts-with", "ends-with", "chars", "" };
    for (int i = 0; ms[i][0]; i++) if (!strcmp(ms[i], m)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_sq_seq1(tree_t * a) { tree_t * q = ast_node_new(TT_SEQ_EXPR); ast_push(q, a); return q; }
static tree_t * rk_sq_if(tree_t * cond, tree_t * a, tree_t * b) { tree_t * f = ast_node_new(TT_IF); ast_push(f, cond); ast_push(f, rk_sq_seq1(a)); if (b) ast_push(f, rk_sq_seq1(b)); return f; }
static tree_t * rk_sq_take(tree_t * v) { tree_t * q = ast_node_new(TT_SUSPEND); ast_push(q, v); return q; }
static tree_t * rk_sq_invoke(tree_t * fnv, tree_t * a, tree_t * b, tree_t * c) {
    tree_t * iv = ast_node_new(TT_INVOKE);
    ast_push(iv, fnv);
    if (a) ast_push(iv, a);
    if (b) ast_push(iv, b);
    if (c) ast_push(iv, c);
    return iv;
}
static tree_t * rk_sq_fnc(const char * nm, tree_t * a, tree_t * b, tree_t * c, tree_t * d, tree_t * e) {
    tree_t * f = rk_fnc2(nm, a, b);
    if (c) ast_push(f, c);
    if (d) ast_push(f, d);
    if (e) ast_push(f, e);
    return f;
}
static tree_t * rk_sq_brk(void) { return ast_node_new(TT_LOOP_BREAK); }
static tree_t * rk_sq_at(tree_t * arr, long long i) { return rk_t2(TT_ARR_GET, arr, rk_ti(i)); }
static tree_t * rk_seq_to_gather(tree_t * t) {
    int excl = t->c[1] && t->c[1]->t == TT_ILIT && t->c[1]->v.ival;
    tree_t * seeds = t->c[2], * end = t->c[3];
    int id = ++g_lower_rk.rk_gather_ctr;
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    body->line = t->line;
    tree_t * sq = rk_fnc2("__rk_arr", NULL, NULL);
    tree_t * gen = NULL;
    if (seeds && seeds->t == TT_FNC && seeds->v.sval && !strcmp(seeds->v.sval, "__rk_arr")) {
        for (int i = 1; i < seeds->n; i++) { if (i == seeds->n - 1 && seeds->c[i] && seeds->c[i]->t == TT_ANON_BLOCK) gen = seeds->c[i]; else ast_push(sq, seeds->c[i]); }
    } else if (seeds && seeds->t == TT_ANON_BLOCK) gen = seeds;
    else if (seeds) ast_push(sq, seeds);
    ast_push(body, rk_gath_asg(rk_gath_var("@__sq", id, 1), sq));
    if (gen) {
        ast_push(body, rk_gath_asg(rk_gath_var("$__sgb", id, 1), gen));
        ast_push(body, rk_gath_asg(rk_gath_var("$__sga", id, 1), rk_fnc2("__rk_seq_arity", rk_gath_var("$__sgb", id, 0), NULL)));
    }
    int infinite = !end || (end->t == TT_VAR && end->v.sval && !strcmp(end->v.sval, "*"));
    tree_t * ev = end, * tail = NULL;
    if (end && end->t == TT_FNC && end->v.sval && !strcmp(end->v.sval, "__rk_arr") && end->n >= 2) { ev = end->c[1]; tail = end; }
    ast_push(body, rk_gath_asg(rk_gath_var("$__sev", id, 1), infinite ? ast_node_new(TT_NUL) : ev));
    ast_push(body, rk_gath_asg(rk_gath_var("@__sp", id, 1), rk_sq_fnc("__rk_seq_plan", rk_gath_var("@__sq", id, 0), rk_ti(gen ? 1 : 0), rk_gath_var("$__sev", id, 0), NULL, NULL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sk", id, 1), rk_ti(0)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp1", id, 1), ast_node_new(TT_NUL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp2", id, 1), ast_node_new(TT_NUL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp3", id, 1), ast_node_new(TT_NUL)));
    tree_t * lb = ast_node_new(TT_SEQ_EXPR);
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sv", id, 1), ast_node_new(TT_NUL)));
    {
        tree_t * elems = ast_node_new(TT_METHCALL);
        ast_push(elems, rk_gath_var("@__sq", id, 0));
        ast_push(elems, leaf_sval2(TT_QLIT, "elems"));
        tree_t * from_seed = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_t2(TT_ARR_GET, rk_gath_var("@__sq", id, 0), rk_gath_var("$__sk", id, 0)));
        tree_t * next;
        if (gen) {
            tree_t * a0 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), NULL, NULL, NULL));
            tree_t * a1 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp1", id, 0), NULL, NULL));
            tree_t * a2 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0), NULL));
            tree_t * a3 =
                rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp3", id, 0), rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0)));
            next =
                rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(0)), a0,
                rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(1)), a1, rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(2)), a2, a3)));
        } else next =
            rk_gath_asg(rk_gath_var("$__sv", id, 0),
            rk_sq_fnc("__rk_seq_next", rk_sq_at(rk_gath_var("@__sp", id, 0), 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 1), rk_gath_var("$__sp1", id, 0), NULL, NULL));
        ast_push(lb, rk_sq_if(rk_t2(TT_LT, rk_gath_var("$__sk", id, 0), elems), from_seed, next));
    }
    if (!infinite) {
        ast_push(lb,
            rk_gath_asg(rk_gath_var("$__sst", id, 1),
            rk_sq_fnc("__rk_seq_stop", rk_gath_var("$__sv", id, 0), rk_gath_var("$__sev", id, 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 2),
            rk_ti(gen ? 1 : 0))));
        ast_push(lb,
            rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(4)),
            rk_sq_if(rk_sq_invoke(rk_gath_var("$__sev", id, 0), rk_gath_var("$__sv", id, 0), NULL, NULL), rk_gath_asg(rk_gath_var("$__sst", id, 0), rk_ti(1)),
            rk_gath_asg(rk_gath_var("$__sst", id, 0), rk_ti(0))), NULL));
        ast_push(lb, rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(2)), rk_sq_brk(), NULL));
        {
            tree_t * q = ast_node_new(TT_SEQ_EXPR);
            if (!excl) ast_push(q, rk_sq_take(rk_gath_var("$__sev", id, 0)));
            ast_push(q, rk_sq_brk());
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(3)));
            ast_push(f, q);
            ast_push(lb, f);
        }
        {
            tree_t * q = ast_node_new(TT_SEQ_EXPR);
            if (!excl) ast_push(q, rk_sq_take(rk_gath_var("$__sv", id, 0)));
            ast_push(q, rk_sq_brk());
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(1)));
            ast_push(f, q);
            ast_push(lb, f);
        }
    }
    ast_push(lb, rk_sq_take(rk_gath_var("$__sv", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp3", id, 0), rk_gath_var("$__sp2", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp1", id, 0), rk_gath_var("$__sv", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sk", id, 0), rk_t2(TT_ADD, rk_gath_var("$__sk", id, 0), rk_ti(1))));
    { tree_t * loop = ast_node_new(TT_WHILE); ast_push(loop, rk_ti(1)); ast_push(loop, lb); ast_push(body, loop); }
    if (tail) for (int i = 2; i < tail->n; i++) ast_push(body, rk_sq_take(tail->c[i]));
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = t->line;
    ast_push(g, body);
    if (infinite) g->slen |= 256;
    return g;
}
void rk_return_outside_routine(tree_t * t, int in_routine) {
    if (!t) return;
    if (t->t == TT_SUB_DECL || (t->t == TT_ANON_BLOCK && t->slen == 1)) in_routine = 1;
    if (t->t == TT_RETURN && !in_routine) { t->t = TT_DIE; t->n = 0; ast_push(t, leaf_sval2(TT_QLIT, "Attempt to return outside of any Routine")); return; }
    for (int i = 0; i < t->n; i++) rk_return_outside_routine(t->c[i], in_routine);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_seq_desugar(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_seq_desugar(t->c[i]);
    for (int i = 0; i < t->n; i++) { tree_t * c = t->c[i]; if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_seq") && c->n == 4) t->c[i] = rk_seq_to_gather(c); }
}
void rk_lazy_stages(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_lazy_stages(t->c[i]);
    if (t->t != TT_METHCALL || t->n != 3 || !t->c[0] || t->c[0]->t != TT_GATHER || !t->c[1] || !t->c[1]->v.sval || !t->c[2] || t->c[2]->t != TT_ANON_BLOCK) return;
    const char * m = t->c[1]->v.sval;
    int k = !strcmp(m, "map") ? 1 : !strcmp(m, "grep") ? 2 : !strcmp(m, "first") ? 3 : 0;
    if (!k) return;
    int id = ++g_lower_rk.rk_gather_ctr;
    char vn[48];
    snprintf(vn, sizeof vn, "__lx%d", id);
    const char * lx = intern(vn);
    tree_t * it = ast_node_new(TT_ITERATE);
    it->v.sval = (char *) lx;
    ast_push(it, t->c[0]);
    tree_t * inv = rk_sq_invoke(t->c[2], leaf_sval2(TT_VAR, lx), NULL, NULL);
    tree_t * lb = ast_node_new(TT_SEQ_EXPR);
    if (k == 1) ast_push(lb, rk_sq_take(inv));
    else {
        tree_t * q = ast_node_new(TT_SEQ_EXPR);
        ast_push(q, rk_sq_take(leaf_sval2(TT_VAR, lx)));
        if (k == 3) ast_push(q, rk_sq_brk());
        tree_t * f = ast_node_new(TT_IF);
        ast_push(f, inv);
        ast_push(f, q);
        ast_push(lb, f);
    }
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = t->line;
    ast_push(ev, it);
    ast_push(ev, lb);
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = t->line;
    ast_push(g, rk_sq_seq1(ev));
    if (k != 3 && (t->c[0]->slen & 256)) g->slen |= 256;
    long long lz = (t->c[0]->t == TT_GATHER && t->c[0]->v.ival == 1) ? 1 : 0;
    if (k == 3) {
        t->t = TT_ARR_GET;
        t->n = 0;
        t->v.sval = NULL;
        ast_push(t, g);
        ast_push(t, rk_ti(0));
    } else {
        t->t = TT_GATHER;
        t->n = 0;
        t->v.sval = NULL;
        t->v.ival = lz;
        t->slen = g->slen;
        ast_push(t, g->c[0]);
    }
}
static tree_t * rk_gather_callnode(const tree_t * g) {
    const tree_t * c = g->c[0];
    const tree_t * cl = rk_closure_block(c) ? c : NULL;
    const tree_t * blk = cl ? cl->c[1] : c;
    if (!blk || blk->t != TT_ANON_BLOCK || !blk->v.sval) return NULL;
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = blk->v.sval;
    f->line = g->line;
    f->slen |= 32;
    ast_push(f, leaf_sval2(TT_VAR, blk->v.sval));
    for (int i = 2; cl && i < cl->n; i++) ast_push(f, cl->c[i]);
    return f;
}
static tree_t * rk_gather_seqnode(const tree_t * g) {
    const tree_t * c = g->c[0];
    tree_t * cl = rk_closure_block(c) ? (tree_t *) c : NULL;
    if (!cl) { if (!c || c->t != TT_ANON_BLOCK || !c->v.sval) return NULL; cl = rk_fnc2("__blk_close", c, NULL); }
    tree_t * q = rk_fnc2("__rk_seq_new", cl, (g->v.ival == 1 || (g->slen & 256)) ? rk_ti(1) : NULL);
    q->line = g->line;
    return q;
}
void rk_gather_consumers(tree_t * t, tree_t * parent, int idx) {
    if (!t) return;
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && parent) {
        tree_t * rep = NULL;
        int dyn = (t->slen & 128) != 0, inf = (t->slen & 256) != 0 || t->v.ival == 1;
        tree_t * call = dyn ? NULL : rk_gather_callnode(t);
        if (call && (parent->t == TT_ITERATE || ((parent->t == TT_MAP || parent->t == TT_GREP) && idx == 1))) rep = call;
        else if (call && !inf && parent->t == TT_ASSIGN && idx == 1 && parent->c[0] && parent->c[0]->t == TT_VAR && parent->c[0]->v.sval && parent->c[0]->v.sval[0] == '@') rep =
            rk_gather_pump(call, NULL);
        else if (call && !inf && (parent->t == TT_SAY || parent->t == TT_PRINT)) rep = rk_gather_pump(call, NULL);
        else if (call && parent->t == TT_ARR_GET && idx == 0 && parent->n > 1 && parent->c[1] && parent->c[1]->t == TT_ILIT && parent->c[1]->v.ival >= 0) rep =
            rk_gather_pump(call, rk_ti(parent->c[1]->v.ival + 1));
        else if (call && parent->t == TT_METHCALL && idx == 0 && parent->n > 1 && parent->c[1] && parent->c[1]->v.sval) {
            const char * m = parent->c[1]->v.sval;
            if (!strcmp(m, "head") && parent->n > 2 && parent->c[2]) rep = rk_gather_pump(call, parent->c[2]);
            else if (!strcmp(m, "head")) rep = rk_gather_pump(call, rk_ti(1));
            else if (!inf && rk_gath_eager_meth(m)) rep = rk_gather_pump(call, NULL);
        }
        if (!rep) rep = rk_gather_seqnode(t);
        if (!rep) rk_refuse(t, "a gather whose body is not a hoisted block");
        parent->c[idx] = rep;
        t = rep;
    }
    for (int i = 0; i < t->n; i++) rk_gather_consumers(t->c[i], t, i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_tv(const char * nm) { return leaf_sval2(TT_VAR, nm); }
tree_t * rk_ti(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
tree_t * rk_t2(tree_e k, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(k); ast_push(n, a); ast_push(n, b); return n; }
tree_t * rk_tset(const char * v, tree_t * e) { return rk_t2(TT_ASSIGN, rk_tv(v), e); }
static tree_t * rk_tinc(const char * v, tree_t * by) { return rk_tset(v, rk_t2(TT_ADD, rk_tv(v), by)); }
static tree_t * rk_tsq(tree_t * a, tree_t * b, tree_t * c) { tree_t * s = ast_node_new(TT_SEQ_EXPR); if (a) ast_push(s, a); if (b) ast_push(s, b); if (c) ast_push(s, c); return s; }
static tree_t * rk_tadd(tree_t * s, tree_t * x) { ast_push(s, x); return s; }
static tree_t * rk_tarr(const char * a, const char * i) { return rk_t2(TT_ARR_GET, rk_tv(a), rk_tv(i)); }
static tree_t * rk_tput(const char * a, const char * i, tree_t * v) { tree_t * n = ast_node_new(TT_ARR_SET); ast_push(n, rk_tv(a)); ast_push(n, rk_tv(i)); ast_push(n, v); return n; }
static tree_t * rk_tclamp(const char * v, const char * lim) {
    tree_t * f = ast_node_new(TT_IF);
    ast_push(f, rk_t2(TT_GT, rk_tv(v), rk_tv(lim)));
    ast_push(f, rk_tsq(rk_tset(v, rk_tv(lim)), NULL, NULL));
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_sort_helper(const char * hname, const char * cmp, int nref) {
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->v.ival = 1 + nref;
    ast_push(sd, rk_tv(hname));
    tree_t * ps = rk_tv("@src");
    ast_push(ps, leaf_sval2(TT_QLIT, "@"));
    ast_push(sd, ps);
    tree_t * cc = rk_fnc2(cmp, NULL, NULL);
    for (int i = 0; i < nref; i++) {
        char rn[24];
        snprintf(rn, sizeof rn, "$?cr%d", i);
        tree_t * rp = rk_tv(lp_strdup(rn));
        ast_push(rp, leaf_sval2(TT_QLIT, "@"));
        ast_push(sd, rp);
        ast_push(cc, rk_tv(lp_strdup(rn)));
    }
    ast_push(cc, rk_tarr("@?sa", "$?sr"));
    ast_push(cc, rk_tarr("@?sa", "$?sl"));
    ast_push(sd, rk_tset("@?sa", rk_tv("@src")));
    { tree_t * m = ast_node_new(TT_METHCALL); ast_push(m, rk_tv("@?sa")); ast_push(m, leaf_sval2(TT_QLIT, "elems")); ast_push(sd, rk_tset("$?sn", m)); }
    ast_push(sd, rk_tset("@?sb", rk_fnc2("__rk_undef", NULL, NULL)));
    ast_push(sd, rk_tset("$?sw", rk_ti(1)));
    tree_t * lt0 = rk_t2(TT_LT, cc, rk_ti(0));
    tree_t * ifc = ast_node_new(TT_IF);
    ast_push(ifc, lt0);
    ast_push(ifc, rk_tsq(rk_tput("@?sb", "$?sk", rk_tarr("@?sa", "$?sr")), rk_tinc("$?sr", rk_ti(1)), NULL));
    ast_push(ifc, rk_tsq(rk_tput("@?sb", "$?sk", rk_tarr("@?sa", "$?sl")), rk_tinc("$?sl", rk_ti(1)), NULL));
    tree_t * both = ast_node_new(TT_SEQ);
    both->v.ival = 1;
    ast_push(both, rk_t2(TT_LT, rk_tv("$?sl"), rk_tv("$?smid")));
    ast_push(both, rk_t2(TT_LT, rk_tv("$?sr"), rk_tv("$?shi")));
    tree_t * w1 = rk_t2(TT_WHILE, both, rk_tsq(ifc, rk_tinc("$?sk", rk_ti(1)), NULL));
    tree_t * w2 = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("$?sl"), rk_tv("$?smid")), rk_tsq(rk_tput("@?sb", "$?sk", rk_tarr("@?sa", "$?sl")), rk_tinc("$?sl", rk_ti(1)), rk_tinc("$?sk", rk_ti(1))));
    tree_t * w3 = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("$?sr"), rk_tv("$?shi")), rk_tsq(rk_tput("@?sb", "$?sk", rk_tarr("@?sa", "$?sr")), rk_tinc("$?sr", rk_ti(1)), rk_tinc("$?sk", rk_ti(1))));
    tree_t * ib =
        rk_tsq(rk_tset("$?smid", rk_t2(TT_ADD, rk_tv("$?si"), rk_tv("$?sw"))), rk_tclamp("$?smid", "$?sn"), rk_tset("$?shi", rk_t2(TT_ADD, rk_tv("$?si"), rk_t2(TT_MUL, rk_ti(2), rk_tv("$?sw")))));
    rk_tadd(ib, rk_tclamp("$?shi", "$?sn"));
    rk_tadd(ib, rk_tset("$?sl", rk_tv("$?si")));
    rk_tadd(ib, rk_tset("$?sr", rk_tv("$?smid")));
    rk_tadd(ib, rk_tset("$?sk", rk_tv("$?si")));
    rk_tadd(ib, w1);
    rk_tadd(ib, w2);
    rk_tadd(ib, w3);
    rk_tadd(ib, rk_tinc("$?si", rk_t2(TT_MUL, rk_ti(2), rk_tv("$?sw"))));
    tree_t * inner = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("$?si"), rk_tv("$?sn")), ib);
    tree_t * ob = rk_tsq(rk_tset("$?si", rk_ti(0)), inner, rk_tset("@?sa", rk_tv("@?sb")));
    rk_tadd(ob, rk_tset("$?sw", rk_t2(TT_MUL, rk_tv("$?sw"), rk_ti(2))));
    ast_push(sd, rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("$?sw"), rk_tv("$?sn")), ob));
    { tree_t * r = ast_node_new(TT_RETURN); tree_t * m = ast_node_new(TT_METHCALL); ast_push(m, rk_tv("@?sa")); ast_push(m, leaf_sval2(TT_QLIT, "list")); ast_push(r, m); ast_push(sd, r); }
    return sd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_new_sub(const tree_t * cdecl, const char * mn) {
    for (int j = 1; cdecl && j < cdecl->n; j++) {
        const tree_t * ch = cdecl->c[j];
        if (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0] && ch->c[0]->v.sval && !strcmp(ch->c[0]->v.sval, mn) && !rk_method_is_stub(ch)) return ch;
    }
    return NULL;
}
static const char * rk_new_parent(const tree_t * cdecl, int * bad) {
    const tree_t * tn = cdecl->c[0];
    const char * parent = NULL;
    for (int ti = 0; tn && ti < tn->n; ti++) { const char * s = tn->c[ti] ? tn->c[ti]->v.sval : NULL; if (!s || !s[0]) continue; if (s[0] != 'i' || parent) { *bad = 1; return NULL; } parent = s + 1; }
    return parent;
}
static int rk_new_chain_ok(const tree_t * prog, const char * cls, int depth, int * hooks) {
    const tree_t * d = rk_find_type_decl(prog, cls);
    if (!d || d->t != TT_CLASS_DECL || depth > prog->n) return 0;
    for (int j = 1; j < d->n; j++) {
        const tree_t * ch = d->c[j];
        const char * mn = (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0]) ? ch->c[0]->v.sval : NULL;
        if (mn && (!strcmp(mn, "new") || !strncmp(mn, "BUILD$", 6) || !strncmp(mn, "TWEAK$", 6) || !strncmp(mn, "new$", 4))) return 0;
    }
    if (rk_new_sub(d, "BUILD") || rk_new_sub(d, "TWEAK")) *hooks = 1;
    int bad = 0;
    const char * parent = rk_new_parent(d, &bad);
    if (bad) return 0;
    return parent ? rk_new_chain_ok(prog, parent, depth + 1, hooks) : 1;
}
static tree_t * rk_new_call(const char * proc, tree_t * self) { tree_t * f = ast_node_new(TT_FNC); f->v.sval = (char *) proc; ast_push(f, rk_tv(proc)); ast_push(f, self); return f; }
static void rk_new_hooks(const tree_t * prog, const char * cls, const char * hook, tree_t * sd, const tree_t * site, char ** pnames) {
    const tree_t * d = rk_find_type_decl(prog, cls);
    int bad = 0;
    const char * parent = rk_new_parent(d, &bad);
    if (parent) rk_new_hooks(prog, parent, hook, sd, site, pnames);
    const tree_t * h = rk_new_sub(d, hook);
    if (!h) return;
    char pn[fmt_len("%s__%s", cls, hook)];
    snprintf(pn, sizeof pn, "%s__%s", cls, hook);
    tree_t * f = rk_new_call(lp_strdup(pn), rk_tv("s"));
    if (!strcmp(hook, "BUILD")) {
        int bs = (int) h->v.ival;
        for (int p = 1; p < bs && p < h->n; p++) {
            const char * pk = (h->c[p] && h->c[p]->v.sval) ? h->c[p]->v.sval : NULL;
            if (!pk || !*pk) continue;
            tree_t * arg = NULL;
            for (int k = 0; 1 + 2 * k + 1 < site->n; k++) if (!strcmp(site->c[1 + 2 * k]->v.sval, pk)) arg = rk_tv(pnames[k]);
            ast_push(f, arg ? arg : rk_fnc2("__rk_undef", NULL, NULL));
        }
    }
    ast_push(sd, f);
}
static int rk_new_site_ok(const tree_t * t) {
    if (!t || t->t != TT_NEW || t->n < 1 || !t->c[0] || t->c[0]->t != TT_QLIT || !t->c[0]->v.sval || (t->n - 1) % 2) return 0;
    for (int k = 1; k < t->n; k += 2) if (!t->c[k] || t->c[k]->t != TT_QLIT || !t->c[k]->v.sval) return 0;
    return 1;
}
void rk_new_build_walk(tree_t * prog, tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_new_build_walk(prog, t->c[i], seq);
    if (!rk_new_site_ok(t)) return;
    const char * cls = t->c[0]->v.sval;
    int hooks = 0;
    if (!rk_new_chain_ok(prog, cls, 0, &hooks) || !hooks) return;
    int nkv = (t->n - 1) / 2;
    char hn[fmt_len("__rk_newp_%s_%d", cls, *seq)];
    snprintf(hn, sizeof hn, "__rk_newp_%s_%d", cls, (*seq)++);
    const char * hname = lp_strdup(hn);
    char * pnames[nkv > 0 ? nkv : 1];
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->v.ival = nkv;
    ast_push(sd, rk_tv(hname));
    for (int k = 0; k < nkv; k++) { char pn[fmt_len("__rk_nv%d", k)]; snprintf(pn, sizeof pn, "__rk_nv%d", k); pnames[k] = lp_strdup(pn); ast_push(sd, rk_tv(pnames[k])); }
    tree_t * al = ast_node_new(TT_FNC);
    al->v.sval = (char *) "__rk_new_alloc";
    ast_push(al, rk_tv("__rk_new_alloc"));
    ast_push(al, leaf_sval2(TT_QLIT, cls));
    for (int k = 0; k < nkv; k++) { ast_push(al, leaf_sval2(TT_QLIT, t->c[1 + 2 * k]->v.sval)); ast_push(al, rk_tv(pnames[k])); }
    ast_push(sd, rk_tset("s", al));
    rk_new_hooks(prog, cls, "BUILD", sd, t, pnames);
    ast_push(sd, rk_fnc2("__rk_new_check", leaf_sval2(TT_QLIT, cls), rk_tv("s")));
    rk_new_hooks(prog, cls, "TWEAK", sd, t, pnames);
    { tree_t * r = ast_node_new(TT_RETURN); ast_push(r, rk_fnc2("__rk_new_defaults", leaf_sval2(TT_QLIT, cls), rk_tv("s"))); ast_push(sd, r); }
    ast_push(prog, sd);
    tree_t * vals[nkv > 0 ? nkv : 1];
    for (int k = 0; k < nkv; k++) vals[k] = t->c[2 + 2 * k];
    t->t = TT_FNC;
    t->v.sval = (char *) hname;
    t->n = 0;
    ast_push(t, rk_tv(hname));
    for (int k = 0; k < nkv; k++) ast_push(t, vals[k]);
}
static int rk_mentions_topic(const tree_t * t) {
    if (!t || t->t == TT_ANON_BLOCK) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "_")) return 1;
    for (int i = 0; i < t->n; i++) if (rk_mentions_topic(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_smartmatch_topic_walk(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_smartmatch_topic_walk(t->c[i]);
    if (t->t != TT_FNC || !t->v.sval || (strcmp(t->v.sval, "__rk_smartmatch") && strcmp(t->v.sval, "__rk_not_smartmatch")) || t->n != 3) return;
    const char * fn = t->v.sval;
    tree_t * r = t->c[2];
    if (!r || r->t == TT_VAR || !rk_mentions_topic(r)) return;
    if (t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "_")) return;
    char b[64];
    snprintf(b, sizeof b, "__smt%p", (const void *) t);
    const char * sv = ct_strdup(b);
    snprintf(b, sizeof b, "__smr%p", (const void *) t);
    const char * rs = ct_strdup(b);
    tree_t * lhs = t->c[1];
    tree_t * inner = rk_fnc2(fn, rk_tv("_"), r);
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, rk_gath_asg(rk_tv(sv), rk_tv("_")));
    ast_push(t, rk_gath_asg(rk_tv("_"), lhs));
    ast_push(t, rk_gath_asg(rk_tv(rs), inner));
    ast_push(t, rk_gath_asg(rk_tv("_"), rk_tv(sv)));
    ast_push(t, rk_tv(rs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_parse_actions_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_parse_actions_walk(t->c[i], seq);
    if (t->t != TT_METHCALL || t->n < 3 || !t->c[1] || !t->c[1]->v.sval || (strcmp(t->c[1]->v.sval, "parse") && strcmp(t->c[1]->v.sval, "subparse"))) return;
    int ai = -1;
    for (int i = 2; i < t->n; i++) {
        const tree_t * a = t->c[i];
        if (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_pair") && a->n == 3 && a->c[1] && a->c[1]->t == TT_QLIT && a->c[1]->v.sval && !strcmp(a->c[1]->v.sval, "actions")) ai = i;
    }
    int positional = ai < 0 && t->n == 4 && t->c[3] && !(t->c[3]->t == TT_FNC && t->c[3]->v.sval && !strcmp(t->c[3]->v.sval, "__rk_pair"));
    if (positional) ai = 3;
    if (ai < 0) return;
    char b1[32], b2[32], b3[32], b4[32];
    snprintf(b1, sizeof b1, "__rk_ga%d", *seq);
    snprintf(b2, sizeof b2, "__rk_gm%d", *seq);
    snprintf(b3, sizeof b3, "__rk_gp%d", *seq);
    snprintf(b4, sizeof b4, "__rk_gi%d", (*seq)++);
    const char * ga = lp_strdup(b1), * gm = lp_strdup(b2), * gp = lp_strdup(b3), * gi = lp_strdup(b4);
    tree_t * act = positional ? t->c[ai] : t->c[ai]->c[2];
    tree_t * pc = ast_node_new(TT_METHCALL);
    pc->line = t->line;
    for (int i = 0; i < t->n; i++) if (i != ai) ast_push(pc, t->c[i]);
    tree_t * call = rk_fnc2("__rk_named_call", rk_fnc2("__rk_arr_at", rk_tv(gp), rk_tv(gi)), rk_ti(2));
    ast_push(call, rk_tv(ga));
    ast_push(call, rk_fnc2("__rk_arr_at", rk_tv(gp), rk_t2(TT_ADD, rk_tv(gi), rk_ti(1))));
    tree_t * wh = ast_node_new(TT_WHILE);
    ast_push(wh, rk_t2(TT_LT, rk_tv(gi), rk_fnc2("elems", rk_tv(gp), NULL)));
    ast_push(wh, rk_tsq(call, rk_tinc(gi, rk_ti(2)), NULL));
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, rk_tset(ga, act));
    ast_push(t, rk_tset(gm, pc));
    ast_push(t, rk_tset(gp, rk_fnc2("__rk_actions_plan", rk_tv(gm), rk_tv(ga))));
    ast_push(t, rk_tset(gi, rk_ti(0)));
    ast_push(t, wh);
    ast_push(t, rk_tv(gm));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_hyper_meth_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_hyper_meth_walk(t->c[i], seq);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_hyper_meth") || t->n < 3 || !t->c[2] || t->c[2]->t != TT_QLIT || !t->c[2]->v.sval) return;
    const tree_t * recv = t->c[1];
    if (!recv || (recv->t == TT_VAR && recv->v.sval && recv->v.sval[0] == '%')) return;
    char hv[32];
    snprintf(hv, sizeof hv, "__rk_hy%d", (*seq)++);
    const char * h = lp_strdup(hv);
    tree_t * mc = ast_node_new(TT_METHCALL);
    mc->line = t->line;
    ast_push(mc, leaf_sval2(TT_VAR, intern("_")));
    for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    ast_push(body, mc);
    tree_t * blk = ast_node_new(TT_ANON_BLOCK);
    blk->line = t->line;
    ast_push(blk, body);
    tree_t * map = ast_node_new(TT_METHCALL);
    map->line = t->line;
    ast_push(map, rk_tv(h));
    ast_push(map, leaf_sval2(TT_QLIT, "map"));
    ast_push(map, blk);
    tree_t * done = rk_fnc2("__rk_hyper_done", map, rk_tv(h));
    tree_t * as = rk_tset(h, (tree_t *) recv);
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, as);
    ast_push(t, done);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_user_terms_find(const tree_t * t, int * declared) {
    if (!t) return;
    if (t->t == TT_ASSIGN && t->n >= 1 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 1) && t->c[0]->v.sval) {
        for (int k = 0; rk_shadowable_terms[k]; k++) if (!strcmp(t->c[0]->v.sval, rk_shadowable_terms[k])) declared[k] = 1;
    }
    for (int i = 0; i < t->n; i++) rk_user_terms_find(t->c[i], declared);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_user_terms_unmark(tree_t * t, const int * declared) {
    if (!t) return;
    if (t->t == TT_VAR && (t->slen & 1) && t->v.sval) for (int k = 0; rk_shadowable_terms[k]; k++) if (declared[k] && !strcmp(t->v.sval, rk_shadowable_terms[k])) t->slen &= ~1;
    for (int i = 0; i < t->n; i++) rk_user_terms_unmark(t->c[i], declared);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_user_terms(tree_t * prog) { int declared[sizeof rk_shadowable_terms / sizeof *rk_shadowable_terms] = { 0 }; rk_user_terms_find(prog, declared); rk_user_terms_unmark(prog, declared); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_end_is_infinite(const tree_t * e) { return e && e->t == TT_VAR && e->v.sval && (!strcmp(e->v.sval, "Inf") || !strcmp(e->v.sval, "*") || !strcmp(e->v.sval, "∞")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_infinite_range_gather(tree_t * to, int * seq) {
    char * k = (char *) ct_alloc(32);
    snprintf(k, 32, "__rk_inf%d", (*seq)++);
    tree_t * lo = to->c[0];
    if (to->v.ival & 1) lo = rk_t2(TT_ADD, lo, rk_ti(1));
    tree_t * step = ast_node_new(TT_SEQ_EXPR);
    tree_t * tk = ast_node_new(TT_SUSPEND);
    ast_push(tk, rk_tv(k));
    ast_push(step, tk);
    ast_push(step, rk_tset(k, rk_t2(TT_ADD, rk_tv(k), rk_ti(1))));
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    ast_push(body, rk_tset(k, lo));
    ast_push(body, rk_t2(TT_WHILE, rk_ti(1), step));
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = to->line;
    g->v.ival = 1;
    ast_push(g, body);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_iterates(const tree_t * t, int i) {
    static const char * const meths[] = { "head", "map", "grep", "first", "list", "Seq", "Array", "flat", "skip", "kv", "pairs", "values", "rotor", "batch", "toggle", "produce", "lazy", "cache",
        "iterator", "is-lazy", NULL };
    if (t->t == TT_ITERATE || (t->t == TT_ARR_GET && i == 0)) return 1;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_slice") && i == 1) return 1;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_range_arr")) return 0;
    if (t->t != TT_METHCALL || i != 0 || t->n < 2 || !t->c[1] || t->c[1]->t != TT_QLIT || !t->c[1]->v.sval) return 0;
    for (int k = 0; meths[k]; k++) if (!strcmp(t->c[1]->v.sval, meths[k])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_infinite_ranges(tree_t * t, int * seq, int insub) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        int sub = ((t->t == TT_ARR_GET || t->t == TT_ARR_SET || t->t == TT_HASH_GET || t->t == TT_HASH_SET) && i > 0) || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_slice") && i > 1);
        rk_infinite_ranges(c, seq, sub);
        if (!c) continue;
        int star = c->t == TT_VAR && c->v.sval && !strcmp(c->v.sval, "*");
        if (star && ((t->t == TT_TO && t->n == 2 && i == 1 && !insub) || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_in_range") && i == 3))) {
            t->c[i] = rk_tv("Inf");
            t->c[i]->slen = 1;
            continue;
        }
        if (c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_range_arr") && c->n >= 3 && rk_range_end_is_infinite(c->c[2]) && !rk_range_end_is_infinite(c->c[1])) {
            tree_t * to = rk_t2(TT_TO, c->c[1], c->c[2]);
            to->line = c->line;
            to->v.ival = (c->n >= 4 && c->c[3] && c->c[3]->t == TT_ILIT) ? c->c[3]->v.ival : 0;
            t->c[i] = rk_infinite_range_gather(to, seq);
            continue;
        }
        if (c->t != TT_TO || c->n != 2 || sub || !rk_range_end_is_infinite(c->c[1]) || rk_range_end_is_infinite(c->c[0]) || !rk_range_iterates(t, i)) continue;
        t->c[i] = rk_infinite_range_gather(c, seq);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_xx_thunks(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_xx_thunks(c);
        if (!c || c->t != TT_FNC || c->n != 3 || !c->v.sval || strcmp(c->v.sval, "__rk_arr_xx")) continue;
        tree_t * cnt = c->c[2];
        if (rk_range_end_is_infinite(cnt)) {
            tree_t * tk = ast_node_new(TT_SUSPEND);
            ast_push(tk, c->c[1]);
            tree_t * g = ast_node_new(TT_GATHER);
            g->line = c->line;
            ast_push(g, rk_t2(TT_WHILE, rk_ti(1), tk));
            t->c[i] = g;
            continue;
        }
        if (!cnt || rk_xx_value_is_pure(c->c[1])) continue;
        tree_t * to = rk_t2(TT_TO, rk_ti(0), cnt);
        to->v.ival = 2;
        to->line = c->line;
        tree_t * body = ast_node_new(TT_SEQ_EXPR);
        ast_push(body, c->c[1]);
        tree_t * blk = ast_node_new(TT_ANON_BLOCK);
        ast_push(blk, body);
        tree_t * mc = ast_node_new(TT_METHCALL);
        mc->line = c->line;
        ast_push(mc, to);
        ast_push(mc, leaf_sval2(TT_QLIT, "map"));
        ast_push(mc, blk);
        t->c[i] = mc;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_array_single_arg(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_array_single_arg(t->c[i]);
    if (t->t != TT_FNC || t->n != 2 || !t->v.sval || strcmp(t->v.sval, "__rk_arr_lit") || !t->c[0] || t->c[0]->t != TT_VAR) return;
    const tree_t * x = t->c[1];
    if (!x || x->t == TT_QLIT || x->t == TT_ILIT || x->t == TT_FLIT || x->t == TT_NUL || x->t == TT_TO) return;
    if (x->t == TT_VAR && (!x->v.sval || x->v.sval[0] != '@')) return;
    t->v.sval = (char *) "__rk_arr_spread";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_arr_spread");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_wrap_bodies(tree_t * t) {
    if (!t) return;
    if ((t->t == TT_IF || t->t == TT_UNLESS || t->t == TT_WHILE || t->t == TT_UNTIL || t->t == TT_EVERY) && t->n > 1 && t->c[1] && t->c[1]->t != TT_SEQ_EXPR) t->c[1] = rk_sq_seq1(t->c[1]);
    for (int i = 0; i < t->n; i++) rk_wrap_bodies(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pl_enum(tree_t * t) {
    int ni = t->n - 2;
    long long idx = 0;
    tree_t * ty = t->c[1], * it[ni + 1];
    for (int i = 0; i < ni; i++) it[i] = t->c[2 + i];
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    if (ty && ty->t == TT_VAR) { tree_t * q = ty->n ? ty->c[0] : NULL; ty->n = 0; ast_push(t, rk_t2(TT_ASSIGN, ty, rk_fnc2("__rk_typeobj", q, NULL))); }
    for (int i = 0; i < ni; i++) {
        tree_t * v = it[i]->n ? it[i]->c[0] : NULL;
        it[i]->n = 0;
        if (v && v->t == TT_ILIT) idx = v->v.ival;
        ast_push(t, rk_t2(TT_ASSIGN, it[i], v ? v : rk_ti(idx)));
        idx++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pl_modifier(tree_t * t, int kind) {
    tree_t * stmt = t->c[1], * cond = t->c[2];
    char sv[48];
    snprintf(sv, sizeof sv, "__modsv_%p", (void *) t);
    const char * svn = lp_strdup(sv);
    tree_t * save = rk_t2(TT_ASSIGN, leaf_sval2(TT_VAR, svn), leaf_sval2(TT_VAR, "_")), * topic = rk_t2(TT_ASSIGN, leaf_sval2(TT_VAR, "_"), cond),
        * back = rk_t2(TT_ASSIGN, leaf_sval2(TT_VAR, "_"), leaf_sval2(TT_VAR, svn));
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, save);
    ast_push(t, topic);
    if (kind == 2) ast_push(t, stmt);
    else ast_push(t, rk_t2(kind ? TT_UNLESS : TT_IF, rk_fnc2("__rk_defined", leaf_sval2(TT_VAR, "_"), NULL), rk_sq_seq1(stmt)));
    ast_push(t, back);
}
