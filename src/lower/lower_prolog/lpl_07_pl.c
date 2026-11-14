#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include <stdlib.h>
#include "ir/stage2_sm_prog_api.h"
#include "ir/stage2_lpl_01_pl_api.h"
#include "ir/stage2_pl.h"
#include "lower/lower_common.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_06_pl.h"
#include "lower/lower_prolog/lpl_07_pl.h"
#include "lower/g_lower_pl.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#include "ir/ct_arena.h"
static int pl_det_specs(const tree_t * a, const char ** keys, int n, int cap);
static int pl_auto_clause_dep(pl_auto_t * a, int self, const tree_t * g, int need, int * memo);
static void pl_hook_dyn_mark(const char * name, int arity) {
    if (!name) return;
    for (int i = 0; i < g_lower_pl.hook_dyn_n; i++) if (g_lower_pl.hook_dyn_name[i] && !strcmp(g_lower_pl.hook_dyn_name[i], name) && g_lower_pl.hook_dyn_arity[i] == arity) return;
    if (g_lower_pl.hook_dyn_n >= g_lower_pl.hook_dyn_cap) {
        int nc = g_lower_pl.hook_dyn_cap > 0 ? g_lower_pl.hook_dyn_cap * 2 : 16;
        g_lower_pl.hook_dyn_name = (const char **) ct_grow((void *) g_lower_pl.hook_dyn_name, (size_t) nc * sizeof(const char *));
        g_lower_pl.hook_dyn_arity = (int *) ct_grow((void *) g_lower_pl.hook_dyn_arity, (size_t) nc * sizeof(int));
        g_lower_pl.hook_dyn_cap = nc;
    }
    g_lower_pl.hook_dyn_name[g_lower_pl.hook_dyn_n] = ct_strdup(name);
    g_lower_pl.hook_dyn_arity[g_lower_pl.hook_dyn_n] = arity;
    g_lower_pl.hook_dyn_n++;
}
void pl_lower_hook_dyn_mark(const char * name, int arity) { pl_hook_dyn_mark(name, arity); }
int pl_hook_dyn_is(const char * name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pl.hook_dyn_n; i++) if (g_lower_pl.hook_dyn_name[i] && !strcmp(g_lower_pl.hook_dyn_name[i], name) && g_lower_pl.hook_dyn_arity[i] == arity) return 1;
    return 0;
}
void pl_hook_dyn_record(const tree_t * spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_hook_dyn_record(spec->c[0]); pl_hook_dyn_record(spec->c[1]); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_hook_dyn_record(spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ":") && spec->n == 2) { pl_hook_dyn_record(spec->c[1]); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) pl_hook_dyn_mark(spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
}
static int pl_det_plain_call(const tree_t * g) {
    if (!g || (g->t != TT_FNC && g->t != TT_QLIT && g->t != TT_NAME) || !g->v.sval) return 0;
    const char * f = g->v.sval;
    static const char * const meta[] = { ",", ";", "->", "*->", "\\+", "!", "true", "fail", "false", ":", "$", "call", "findall", "forall", "catch", "once", "ignore", "not", "bagof", "setof",
        "aggregate_all", (const char *) 0 };
    for (int i = 0; meta[i]; i++) if (!strcmp(f, meta[i])) return 0;
    return 1;
}
static const char * pl_det_chain_end(const char * nm, int ar, int depth) {
    extern int pl_prelude_defines(const char *, int);
    if (pl_prelude_defines(nm, ar)) return (const char *) 0;
    char key[strlen(nm) + 16];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    if (!ch) return (const char *) 0;
    if (ch->t == TT_CHOICE && ch->n == 1 && depth < 8) {
        const tree_t * cl = ch->c[0];
        const tree_t * last = (cl && cl->t == TT_CLAUSE && cl->n > ar) ? cl->c[cl->n - 1] : (const tree_t *) 0;
        if (pl_det_plain_call(last)) { const char * r = pl_det_chain_end(last->v.sval, last->t == TT_FNC ? last->n : 0, depth + 1); if (r) return r; }
    }
    return ct_strdup(key);
}
tree_t * pl_det_guard_callee(const char * own_key, const tree_t * const * gl, int ng, int k) {
    const tree_t * g = (k + 1 < ng) ? gl[ng - 1] : (const tree_t *) 0;
    if (!pl_det_plain_call(g)) return (tree_t *) 0;
    const char * end = pl_det_chain_end(g->v.sval, g->t == TT_FNC ? g->n : 0, 0);
    return pl_cc_key_pi(end ? end : own_key);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_clauseq_goal_arg(const char * f, int n, int i) {
    if (!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->") || !strcmp(f, "*->")) return n == 2;
    if (!strcmp(f, ":") || !strcmp(f, "^")) return n == 2 && i == 1;
    if (!strcmp(f, "\\+") || !strcmp(f, "once") || !strcmp(f, "ignore") || !strcmp(f, "not") || !strcmp(f, "call")) return n == 1 && i == 0;
    if (!strcmp(f, "findall") || !strcmp(f, "bagof") || !strcmp(f, "setof")) return (n == 3 || n == 4) && i == 1;
    if (!strcmp(f, "forall")) return n == 2;
    if (!strcmp(f, "aggregate_all")) return (n == 3 && i == 1) || (n == 4 && i == 2);
    if (!strcmp(f, "catch")) return n == 3 && (i == 0 || i == 2);
    if (!strcmp(f, "setup_call_cleanup")) return n == 3;
    if (!strcmp(f, "call_cleanup")) return n == 2;
    return 0;
}
static const tree_t * pl_clauseq_walk(const tree_t * t, int g, int har) {
    if (!t) return t;
    {
        const tree_t * k[t->n + 1];
        int ch = 0;
        int ar = (t->t == TT_CHOICE && t->v.sval && strrchr(t->v.sval, '/')) ? atoi(strrchr(t->v.sval, '/') + 1) : -1;
        for (int i = 0; i < t->n; i++) {
            int cg = 0;
            if (t->t == TT_CLAUSE) cg = har >= 0 && i >= har;
            else if (g && t->t == TT_FNC && t->v.sval) cg = pl_clauseq_goal_arg(t->v.sval, t->n, i);
            else if (g && (t->t == TT_IF || t->t == TT_PROGRAM)) cg = 1;
            k[i] = pl_clauseq_walk(t->c[i], cg, ar);
            ch |= k[i] != t->c[i];
        }
        if (g && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "clause") && (t->n == 2 || t->n == 3) && k[0] && k[0]->t == TT_FNC && k[0]->v.sval && !strcmp(k[0]->v.sval, ":") && k[0]->n == 2 &&
            k[0]->c[0] && k[0]->c[1]) {
            tree_t * f = ast_node_new(TT_FNC);
            f->v.sval = (char *) "$pl_clause_q";
            f->line = t->line;
            ast_push(f, (tree_t *) k[0]->c[0]);
            ast_push(f, (tree_t *) k[0]->c[1]);
            for (int i = 1; i < t->n; i++) ast_push(f, (tree_t *) k[i]);
            return f;
        }
        if (!ch) return t;
        { tree_t * c = ast_node_new(t->t); c->v = t->v; c->line = t->line; c->slen = t->slen; for (int i = 0; i < t->n; i++) ast_push(c, (tree_t *) k[i]); return c; }
    }
}
const tree_t * pl_clauseq_rewrite(const tree_t * t) { return pl_clauseq_walk(t, 0, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_rs_dyn(const tree_t * spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_rs_dyn(spec->c[0]); pl_rs_dyn(spec->c[1]); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_rs_dyn(spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "as") && spec->n == 2 && (pl_table_opt_kind(spec->c[1]) & 3)) pl_hook_dyn_record(spec->c[0]);
}
static int pl_rs_ctl(const tree_t * g, const char * f) { return g && g->t == TT_FNC && g->v.sval && g->n == 2 && !strcmp(g->v.sval, f); }
static int pl_rs_source(const tree_t * g, const char ** tk, int ntk) {
    char b[g && g->v.sval ? strlen(g->v.sval) + 24 : 24];
    int ar;
    if (!g || !g->v.sval || !(g->t == TT_FNC || g->t == TT_QLIT || g->t == TT_NAME)) return 0;
    ar = g->t == TT_FNC ? g->n : 0;
    if (pl_hook_dyn_is(g->v.sval, ar)) return 1;
    snprintf(b, sizeof b, "%s/%d", g->v.sval, ar);
    for (int i = 0; i < ntk; i++) if (!strcmp(tk[i], b)) return 2;
    return 0;
}
static const tree_t * pl_rs_conj(const tree_t * a, const tree_t * b) { return !a ? b : !b ? a : pl_cc_fnc2(",", (tree_t *) a, (tree_t *) b); }
static void pl_rs_walk(const tree_t * g, const tree_t * after, const char ** tk, int ntk, const tree_t ** node, const tree_t ** aft, int * ns) {
    if (!g) return;
    if (pl_rs_ctl(g, ",")) { pl_rs_walk(g->c[0], pl_rs_conj(g->c[1], after), tk, ntk, node, aft, ns); pl_rs_walk(g->c[1], after, tk, ntk, node, aft, ns); return; }
    if (pl_rs_ctl(g, ";") || pl_rs_ctl(g, "|")) { pl_rs_walk(g->c[0], after, tk, ntk, node, aft, ns); pl_rs_walk(g->c[1], after, tk, ntk, node, aft, ns); return; }
    if (pl_rs_ctl(g, "->") || pl_rs_ctl(g, "*->")) { pl_rs_walk(g->c[0], pl_rs_conj(g->c[1], after), tk, ntk, node, aft, ns); pl_rs_walk(g->c[1], after, tk, ntk, node, aft, ns); return; }
    if (pl_rs_source(g, tk, ntk)) { if (node) { node[*ns] = g; aft[*ns] = after; } (*ns)++; }
}
static void pl_rs_vars(const tree_t * t, long long * slots, int * n, int cap) {
    if (!t) return;
    if (t->t == TT_VAR) { for (int i = 0; i < *n; i++) if (slots[i] == t->v.ival) return; if (*n < cap) slots[(*n)++] = t->v.ival; return; }
    for (int i = 0; i < t->n; i++) pl_rs_vars(t->c[i], slots, n, cap);
}
static tree_t * pl_rs_inst(const tree_t * g, const tree_t ** node, tree_t ** rec, int ns) {
    if (!g) return NULL;
    for (int j = 0; j < ns; j++) if (node[j] == g) return pl_cc_fnc2(",", rec[j], (tree_t *) g);
    if (pl_rs_ctl(g, ",") || pl_rs_ctl(g, ";") || pl_rs_ctl(g, "|") || pl_rs_ctl(g, "->") || pl_rs_ctl(g, "*->")) {
        tree_t * c = ast_node_new(TT_FNC);
        c->v = g->v;
        c->line = g->line;
        ast_push(c, pl_rs_inst(g->c[0], node, rec, ns));
        ast_push(c, pl_rs_inst(g->c[1], node, rec, ns));
        return c;
    }
    return (tree_t *) g;
}
static void pl_rs_push(tree_t * cl, const tree_t * g) { if (!g) return; if (pl_rs_ctl(g, ",")) { pl_rs_push(cl, g->c[0]); pl_rs_push(cl, g->c[1]); return; } ast_push(cl, (tree_t *) g); }
int pl_rs_clause(tree_t * ch, int ci, const char * key, const char ** tk, int ntk, tree_t ** out, int nout) {
    tree_t * cl = pl_tree_copy(ch->c[ci]);
    int ar = (int) cl->v.dval, ns = 0, nr = 0;
    const tree_t * bt = NULL;
    const char * sl = strrchr(key, '/');
    char base[sl ? (size_t)(sl - key) + 1 : strlen(key) + 1];
    long long slots[256];
    if (cl->t != TT_CLAUSE || ar < 0 || ar > cl->n) return nout;
    snprintf(base, sizeof base, "%.*s", (int)(sl ? sl - key : (long)strlen(key)), key);
    for (int i = cl->n - 1; i >= ar; i--) bt = pl_rs_conj(cl->c[i], bt);
    if (!bt) return nout;
    pl_rs_walk(bt, NULL, tk, ntk, NULL, NULL, &ns);
    if (ns == 0) return nout;
    const tree_t * node[ns], * aft[ns];
    tree_t * rec[ns], * vs[ns];
    ns = 0;
    pl_rs_walk(bt, NULL, tk, ntk, node, aft, &ns);
    for (int j = 0; j < ns; j++) {
        int nv = 0;
        char nm[strlen(base) + 40];
        tree_t * v, * src;
        for (int i = 0; i < ar; i++) pl_rs_vars(cl->c[i], slots, &nv, 256);
        pl_rs_vars(node[j], slots, &nv, 256);
        pl_rs_vars(aft[j], slots, &nv, 256);
        v = nv > 0 ? ast_node_new(TT_FNC) : (tree_t *) pl_atom_goal("v");
        if (nv > 0) { v->v.sval = (char *) "v"; for (int i = 0; i < nv; i++) { tree_t * pv = ast_node_new(TT_VAR); pv->v.ival = slots[i]; ast_push(v, pv); } }
        vs[j] = v;
        snprintf(nm, sizeof nm, "$rs %s#%d.%d", base, ci, j);
        src = ast_node_new(TT_FNC);
        src->v.sval = (char *) (pl_rs_source(node[j], tk, ntk) == 1 ? "dyn" : "tab");
        {
            char sb[strlen(node[j]->v.sval) + 2];
            tree_t * pi = ast_node_new(TT_FNC), * nmn = ast_node_new(TT_QLIT);
            snprintf(sb, sizeof sb, "%s", node[j]->v.sval);
            nmn->v.sval = lp_strdup(sb);
            pi->v.sval = (char *) "/";
            ast_push(pi, nmn);
            ast_push(pi, pl_cc_ilit(node[j]->t == TT_FNC ? node[j]->n : 0));
            ast_push(src, pi);
        }
        { tree_t * r = ast_node_new(TT_FNC); r->v.sval = (char *) "$tbl_rec"; ast_push(r, src); ast_push(r, (tree_t *) pl_atom_goal(lp_strdup(nm))); ast_push(r, pl_tree_copy(v)); rec[j] = r; }
    }
    for (int j = 0; j < ns && nout + nr < 512; j++) {
        char nm[strlen(base) + 40], rk[strlen(base) + 48];
        tree_t * rcl = ast_node_new(TT_CLAUSE), * rch = ast_node_new(TT_CHOICE);
        rcl->v.dval = ar + 1;
        for (int i = 0; i < ar; i++) ast_push(rcl, pl_tree_copy(cl->c[i]));
        ast_push(rcl, pl_tree_copy(vs[j]));
        ast_push(rcl, pl_tree_copy(node[j]));
        pl_rs_push(rcl, pl_rs_inst(aft[j], node, rec, ns));
        snprintf(nm, sizeof nm, "$rs %s#%d.%d", base, ci, j);
        snprintf(rk, sizeof rk, "%s/%d", nm, ar + 1);
        rch->v.sval = (char *) lp_strdup(rk);
        ast_push(rch, pl_tree_copy(rcl));
        out[nout + nr++] = rch;
    }
    {
        tree_t * ncl = ast_node_new(TT_CLAUSE);
        ncl->v = cl->v;
        ncl->line = cl->line;
        for (int i = 0; i < ar; i++) ast_push(ncl, cl->c[i]);
        for (int i = ar; i < cl->n; i++) ast_push(ncl, pl_rs_inst(cl->c[i], node, rec, ns));
        ch->c[ci] = ncl;
    }
    return nout + nr;
}
static int pl_auto_user_key(const char * key) { for (uint32_t i = 0; i < g_stage2_pl.pl_user_keys_files.len; i++) if (!strcmp(CV_AT(g_stage2_pl.pl_user_keys_files, char *, i), key)) return 1; return 0; }
static int pl_auto_find(const char ** keys, int n, const char * key) { for (int i = 0; i < n; i++) if (!strcmp(keys[i], key)) return i; return -1; }
static int pl_auto_count(const tree_t * t) { int c = 1; if (!t) return 0; for (int i = 0; i < t->n; i++) c += pl_auto_count(t->c[i]); return c; }
static const char * pl_auto_name(const tree_t * g) { return (g && (g->t == TT_FNC || g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval) ? g->v.sval : NULL; }
static int pl_auto_arity(const tree_t * g) { return g && g->t == TT_FNC ? g->n : 0; }
static int pl_auto_idx(pl_auto_t * a, const tree_t * g, int extra) {
    const char * f = pl_auto_name(g);
    if (!f) return -1;
    char b[strlen(f) + 24];
    snprintf(b, sizeof b, "%s/%d", f, pl_auto_arity(g) + extra);
    return pl_auto_find(a->keys, a->nk, b);
}
static void pl_auto_goal(pl_auto_t * a, int self, const tree_t * g, int inmeta) {
    const char * f = pl_auto_name(g);
    int n = pl_auto_arity(g), j;
    if (g && g->t == TT_IF) { for (int i = 0; i < g->n; i++) pl_auto_goal(a, self, g->c[i], inmeta); return; }
    if (!f) return;
    if (g->t == TT_FNC && n == 2 && (!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->") || !strcmp(f, "*->"))) {
        pl_auto_goal(a, self, g->c[0], inmeta);
        pl_auto_goal(a, self, g->c[1], inmeta);
        return;
    }
    if (g->t == TT_FNC && n == 1 && (!strcmp(f, "\\+") || !strcmp(f, "not") || !strcmp(f, "call"))) { pl_auto_goal(a, self, g->c[0], 1); return; }
    if (g->t == TT_FNC && n == 3 && (!strcmp(f, "findall") || !strcmp(f, "setof") || !strcmp(f, "bagof"))) { pl_auto_goal(a, self, g->c[1], 1); return; }
    if (g->t == TT_FNC && n == 2 && !strcmp(f, "^") && inmeta) { pl_auto_goal(a, self, g->c[1], 1); return; }
    if (g->t == TT_FNC && ((n == 1 && !strcmp(f, "tnot")) || (n == 3 && (!strcmp(f, "tfindall") || !strcmp(f, "tbagof") || !strcmp(f, "tsetof"))))) {
        const tree_t * arg = g->c[n == 1 ? 0 : 1];
        j = pl_auto_idx(a, arg, 0);
        if (j >= 0) a->tn[j] = 1;
        return;
    }
    j = pl_auto_idx(a, g, 0);
    if (j >= 0) { a->from[a->ne] = self; a->to[a->ne++] = j; }
}
static int pl_auto_calls(pl_auto_t * a, int v, int w) { for (int e = 0; e < a->ne; e++) if (a->from[e] == v && a->to[e] == w) return 1; return 0; }
static int pl_auto_var_in(const tree_t * t, long long slot) {
    if (!t) return 0;
    if (t->t == TT_VAR) return t->v.ival == slot;
    for (int i = 0; i < t->n; i++) if (pl_auto_var_in(t->c[i], slot)) return 1;
    return 0;
}
static int pl_auto_decreasing(const tree_t * goal, const tree_t * cl, int ar, const char * mode) {
    for (int i = 0; i < ar && i < goal->n && mode[i]; i++) {
        const tree_t * ga = goal->c[i], * ha = cl->c[i];
        if (mode[i] == '+' && ga && ga->t == TT_VAR && ha && (ha->t == TT_FNC || ha->t == TT_MAKELIST) && ha->n > 0 && pl_auto_var_in(ha, ga->v.ival)) return 1;
    }
    return 0;
}
static int pl_auto_term_body(const tree_t * g, const tree_t * cl, int ar, const char * mode, const char * self) {
    const char * f = pl_auto_name(g);
    int n = pl_auto_arity(g);
    if (g && g->t == TT_IF) { for (int i = 0; i < g->n; i++) if (!pl_auto_term_body(g->c[i], cl, ar, mode, self)) return 0; return 1; }
    if (!f) return 1;
    if (g->t == TT_FNC && n == 2 && (!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->") || !strcmp(f, "*->"))) return pl_auto_term_body(g->c[0], cl, ar, mode, self) &&
        pl_auto_term_body(g->c[1], cl, ar, mode, self);
    if (g->t == TT_FNC && n == 1 && (!strcmp(f, "\\+") || !strcmp(f, "not"))) return pl_auto_term_body(g->c[0], cl, ar, mode, self);
    { char b[strlen(f) + 24]; snprintf(b, sizeof b, "%s/%d", f, n); if (strcmp(b, self)) return 1; }
    return pl_auto_decreasing(g, cl, ar, mode);
}
static int pl_auto_terminates(pl_auto_t * a, int v) {
    const char * sl = strrchr(a->keys[v], '/');
    int ar = sl ? atoi(sl + 1) : 0;
    if (!a->mode[v]) return 0;
    for (int c = 0; c < a->defs[v]->n; c++) {
        const tree_t * cl = a->defs[v]->c[c];
        if (!cl || cl->t != TT_CLAUSE) continue;
        for (int b = ar; b < cl->n; b++) if (!pl_auto_term_body(cl->c[b], cl, ar, a->mode[v], a->keys[v])) return 0;
    }
    return 1;
}
static int pl_auto_path(pl_auto_t * a, const unsigned char * in, unsigned char * seen, int cur, int target) {
    for (int e = 0; e < a->ne; e++) {
        int w;
        if (a->from[e] != cur) continue;
        w = a->to[e];
        if (w == target) { if (cur != target) return 1; continue; }
        if (!in[w] || a->tab[w] || seen[w]) continue;
        seen[w] = 1;
        if (pl_auto_path(a, in, seen, w, target)) return 1;
    }
    return 0;
}
static int pl_auto_reach(pl_auto_t * a, const unsigned char * in, int from, int target) {
    unsigned char * seen = (unsigned char *) ct_zalloc((size_t) a->nk + 1, 1);
    if (a->tab[from] || !in[from]) return 0;
    seen[from] = 1;
    return pl_auto_path(a, in, seen, from, target);
}
static void pl_auto_break_loops(pl_auto_t * a, unsigned char * in) {
    for (;;) {
        int nl = 0, best = -1, bestn = -1;
        unsigned char * loop = (unsigned char *) ct_zalloc((size_t) a->nk + 1, 1);
        for (int v = 0; v < a->nk; v++) if (in[v] && !a->tab[v] && pl_auto_reach(a, in, v, v)) { loop[v] = 1; nl++; }
        if (!nl) return;
        for (int v = 0; v < a->nk; v++) {
            int cnt = 0;
            if (!loop[v]) continue;
            for (int w = 0; w < a->nk; w++) {
                if (!loop[w] || w == v || !pl_auto_calls(a, v, w)) continue;
                for (int x = 0; x < a->nk; x++) if (loop[x] && pl_auto_calls(a, x, v) && (x == w || pl_auto_reach(a, loop, w, x))) cnt++;
            }
            if (best < 0) { best = v; bestn = cnt; continue; }
            if (cnt > bestn) {
                best = v;
                bestn = cnt;
            } else if (cnt == bestn) {
                int ax = atoi(strrchr(a->keys[v], '/') + 1), ay = atoi(strrchr(a->keys[best], '/') + 1);
                if (a->ex[v] && !a->ex[best]) best = v;
                else if (a->ex[v] == a->ex[best] && ax < ay) best = v;
            }
        }
        a->tab[best] = 1;
        for (int v = 0; v < a->nk; v++) in[v] = loop[v] && v != best;
    }
}
static int pl_auto_edb_dep(pl_auto_t * a, int v, int need, int * memo) {
    const char * sl = strrchr(a->keys[v], '/');
    int ar = sl ? atoi(sl + 1) : 0, maxd = 0;
    if (memo[v] >= 0) return memo[v];
    if (a->tab[v] || a->edb[v]) return memo[v] = 1;
    memo[v] = 0;
    for (int c = 0; c < a->defs[v]->n; c++) {
        const tree_t * cl = a->defs[v]->c[c];
        int d = 0;
        if (!cl || cl->t != TT_CLAUSE) continue;
        for (int b = ar; b < cl->n; b++) d += pl_auto_clause_dep(a, v, cl->c[b], need, memo);
        if (d > maxd) maxd = d;
    }
    if (maxd >= need) { a->tab[v] = 1; return memo[v] = 1; }
    return memo[v] = maxd;
}
static int pl_auto_clause_dep(pl_auto_t * a, int self, const tree_t * g, int need, int * memo) {
    const char * f = pl_auto_name(g);
    int n = pl_auto_arity(g), j, d0, d1;
    if (g && g->t == TT_IF && g->n >= 2) {
        d0 = pl_auto_clause_dep(a, self, g->c[0], need, memo) + pl_auto_clause_dep(a, self, g->c[1], need, memo);
        d1 = g->n > 2 ? pl_auto_clause_dep(a, self, g->c[2], need, memo) : 0;
        return d0 > d1 ? d0 : d1;
    }
    if (!f) return 0;
    if (g->t == TT_FNC && n == 2 && (!strcmp(f, ",") || !strcmp(f, "->") || !strcmp(f, "*->"))) return pl_auto_clause_dep(a, self, g->c[0], need, memo) +
        pl_auto_clause_dep(a, self, g->c[1], need, memo);
    if (g->t == TT_FNC && n == 2 && !strcmp(f, ";")) { d0 = pl_auto_clause_dep(a, self, g->c[0], need, memo); d1 = pl_auto_clause_dep(a, self, g->c[1], need, memo); return d0 > d1 ? d0 : d1; }
    if (g->t == TT_FNC && n == 1 && (!strcmp(f, "\\+") || !strcmp(f, "not"))) return pl_auto_clause_dep(a, self, g->c[0], need, memo);
    char b[strlen(f) + 24];
    snprintf(b, sizeof b, "%s/%d", f, n);
    j = pl_auto_find(a->keys, a->nk, b);
    if (j == self) return 0;
    if (j >= 0) return pl_auto_edb_dep(a, j, need, memo);
    return (pl_auto_find(a->edbk, a->nedb, b) >= 0 || pl_auto_find(a->tabk, a->ntab, b) >= 0) ? 1 : 0;
}
static void pl_auto_modes(const tree_t * m, pl_auto_t * a) {
    const char * f = pl_auto_name(m);
    int n = pl_auto_arity(m), j;
    if (!f) return;
    if (m->t == TT_FNC && n == 2 && !strcmp(f, ",")) { pl_auto_modes(m->c[0], a); pl_auto_modes(m->c[1], a); return; }
    j = pl_auto_idx(a, m, 0);
    if (j < 0 || n < 1) return;
    { char * md = (char *) ct_zalloc((size_t) n + 1, 1); for (int i = 0; i < n; i++) { const char * s = pl_auto_name(m->c[i]); md[i] = (s && s[0] && !s[1]) ? s[0] : '?'; } a->mode[j] = md; }
}
const tree_t * pl_auto_table_rewrite(const tree_t * prog) {
    int auto_on = 0, suppl_n = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * f = pl_auto_name(subj);
        if (!f) continue;
        if (!strcmp(f, "auto_table") && pl_auto_arity(subj) == 0) auto_on = 1;
        else if (!strcmp(f, "suppl_table") && pl_auto_arity(subj) == 0) suppl_n = 2;
        else if (!strcmp(f, "suppl_table") && pl_auto_arity(subj) == 1 && subj->c[0] && subj->c[0]->t == TT_ILIT && subj->c[0]->v.ival >= 1) suppl_n = (int) subj->c[0]->v.ival;
    }
    if (!auto_on && !suppl_n) return prog;
    int nk = 0, nedb = 0, ntab = 0, nexp = 0, nbody = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t == TT_CHOICE && subj->v.sval && pl_auto_user_key(subj->v.sval)) { nk++; nbody += pl_auto_count(subj); }
        if (subj && subj->t == TT_FNC && subj->v.sval && subj->n == 1 && !strcmp(subj->v.sval, "edb")) nedb = pl_det_specs(subj->c[0], NULL, nedb, 0);
        if (subj && subj->t == TT_FNC && subj->v.sval && subj->n == 1 && !strcmp(subj->v.sval, "export")) nexp = pl_det_specs(subj->c[0], NULL, nexp, 0);
        ntab = pl_table_specs(pl_table_dir(s), NULL, NULL, ntab, 0);
    }
    pl_auto_t a;
    const char ** expk = (const char **) ct_zalloc((size_t) (nexp ? nexp : 1), sizeof(char *));
    const tree_t ** tabs = (const tree_t **) ct_zalloc((size_t) (ntab ? ntab : 1), sizeof(tree_t *));
    int k = 0, ne0 = 0, nt0 = 0, nx0 = 0;
    a.nk = nk;
    a.keys = (const char **) ct_zalloc((size_t) (nk ? nk : 1), sizeof(char *));
    a.defs = (const tree_t **) ct_zalloc((size_t) (nk ? nk : 1), sizeof(tree_t *));
    a.edbk = (const char **) ct_zalloc((size_t) (nedb ? nedb : 1), sizeof(char *));
    a.tabk = (const char **) ct_zalloc((size_t) (ntab ? ntab : 1), sizeof(char *));
    a.tab = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    a.decl = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    a.edb = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    a.ex = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    a.tn = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    a.mode = (const char **) ct_zalloc((size_t) (nk ? nk : 1), sizeof(char *));
    a.from = (int *) ct_zalloc((size_t) nbody + 1, sizeof(int));
    a.to = (int *) ct_zalloc((size_t) nbody + 1, sizeof(int));
    a.ne = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t == TT_CHOICE && subj->v.sval && pl_auto_user_key(subj->v.sval)) { a.keys[k] = subj->v.sval; a.defs[k++] = subj; }
        if (subj && subj->t == TT_FNC && subj->v.sval && subj->n == 1 && !strcmp(subj->v.sval, "edb")) ne0 = pl_det_specs(subj->c[0], a.edbk, ne0, nedb);
        if (subj && subj->t == TT_FNC && subj->v.sval && subj->n == 1 && !strcmp(subj->v.sval, "export")) nx0 = pl_det_specs(subj->c[0], expk, nx0, nexp);
        nt0 = pl_table_specs(pl_table_dir(s), a.tabk, tabs, nt0, ntab);
    }
    a.nedb = ne0;
    a.ntab = nt0;
    for (int v = 0; v < nk; v++) {
        a.decl[v] = a.tab[v] = pl_auto_find(a.tabk, nt0, a.keys[v]) >= 0;
        a.edb[v] = pl_auto_find(a.edbk, ne0, a.keys[v]) >= 0;
        a.ex[v] = nx0 ? pl_auto_find(expk, nx0, a.keys[v]) >= 0 : 1;
    }
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t == TT_FNC && subj->v.sval && subj->n == 1 && !strcmp(subj->v.sval, "mode")) pl_auto_modes(subj->c[0], &a);
    }
    for (int v = 0; v < nk; v++) {
        const char * sl = strrchr(a.keys[v], '/');
        int ar = sl ? atoi(sl + 1) : 0;
        for (int c = 0; c < a.defs[v]->n; c++) { const tree_t * cl = a.defs[v]->c[c]; if (cl && cl->t == TT_CLAUSE) for (int b = ar; b < cl->n; b++) pl_auto_goal(&a, v, cl->c[b], 0); }
    }
    unsigned char * in = (unsigned char *) ct_zalloc((size_t) nk + 1, 1);
    if (auto_on) {
        for (int v = 0; v < nk; v++) if (a.tn[v]) a.tab[v] = 1;
        for (int v = 0; v < nk; v++) if (!a.tab[v] && pl_auto_calls(&a, v, v) && !pl_auto_terminates(&a, v)) a.tab[v] = 1;
        for (int v = 0; v < nk; v++) in[v] = !a.tab[v] || a.decl[v];
        pl_auto_break_loops(&a, in);
    }
    if (suppl_n) {
        int ok = auto_on;
        int * memo = (int *) ct_zalloc((size_t) nk + 1, sizeof(int));
        for (int v = 0; v < nk; v++) in[v] = 1;
        if (!ok) { ok = 1; for (int v = 0; v < nk && ok; v++) if (!a.tab[v] && pl_auto_reach(&a, in, v, v)) ok = 0; }
        for (int v = 0; v < nk; v++) memo[v] = -1;
        if (ok) for (int v = 0; v < nk; v++) pl_auto_edb_dep(&a, v, suppl_n, memo);
    }
    int np_ = 0;
    for (int v = 0; v < nk; v++) if (a.tab[v] && !a.decl[v]) np_++;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    const tree_t * tmpl = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * f = pl_auto_name(subj);
        if (f && ((!strcmp(f, "auto_table") && pl_auto_arity(subj) == 0) || (!strcmp(f, "suppl_table") && pl_auto_arity(subj) <= 1))) { if (!tmpl) tmpl = s; continue; }
        ast_push(np, (tree_t *) s);
    }
    if (np_ && tmpl) {
        tree_t * spec = NULL;
        for (int v = 0; v < nk; v++) {
            if (!a.tab[v] || a.decl[v]) continue;
            const char * sl = strrchr(a.keys[v], '/');
            char nm[sl ? (size_t) (sl - a.keys[v]) + 1 : 1];
            memcpy(nm, a.keys[v], sl ? (size_t) (sl - a.keys[v]) : 0);
            nm[sl ? (size_t) (sl - a.keys[v]) : 0] = 0;
            tree_t * pi = pl_cc_fnc2("/", (tree_t *) pl_atom_goal(lp_strdup(nm)), pl_cc_ilit(sl ? atoi(sl + 1) : 0));
            spec = spec ? pl_cc_fnc2(",", spec, pi) : pi;
        }
        tree_t * td = ast_node_new(TT_FNC);
        td->v.sval = (char *) "table";
        ast_push(td, spec);
        ast_push(np, pl_table_stmt(tmpl, td));
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_det_specs(const tree_t * a, const char ** keys, int n, int cap) {
    if (!a) return n;
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, ",") && a->n == 2) return pl_det_specs(a->c[1], keys, pl_det_specs(a->c[0], keys, n, cap), cap);
    if (a->t == TT_MAKELIST) { for (int i = 0; i < a->n; i++) n = pl_det_specs(a->c[i], keys, n, cap); return n; }
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, ":") && a->n == 2) return pl_det_specs(a->c[1], keys, n, cap);
    if (a->t != TT_FNC || !a->v.sval || strcmp(a->v.sval, "/") || a->n != 2 || !a->c[0] || !a->c[0]->v.sval || !a->c[1] || a->c[1]->t != TT_ILIT) return n;
    if (keys && n < cap) { char b[strlen(a->c[0]->v.sval) + 24]; snprintf(b, sizeof b, "%s/%d", a->c[0]->v.sval, (int) a->c[1]->v.ival); keys[n] = lp_strdup(b); }
    return n + 1;
}
static const tree_t * pl_det_dir(const tree_t * s) {
    const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
    return (subj && subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "det") && subj->n == 1) ? subj->c[0] : NULL;
}
static const char * pl_det_prog_chain(const tree_t * prog, const char * key, int depth) {
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!subj || subj->t != TT_CHOICE || !subj->v.sval || strcmp(subj->v.sval, key)) continue;
        const char * sl = strrchr(key, '/');
        int ar = atoi(sl + 1);
        const tree_t * cl = subj->n == 1 ? subj->c[0] : (const tree_t *) 0;
        const tree_t * last = (cl && cl->t == TT_CLAUSE && cl->n > ar) ? cl->c[cl->n - 1] : (const tree_t *) 0;
        if (depth < 8 && pl_det_plain_call(last)) {
            char nk[strlen(last->v.sval) + 16];
            snprintf(nk, sizeof nk, "%s/%d", last->v.sval, last->t == TT_FNC ? last->n : 0);
            const char * r = pl_det_prog_chain(prog, nk, depth + 1);
            if (r) return r;
        }
        return lp_strdup(key);
    }
    return (const char *) 0;
}
const tree_t * pl_det_rewrite(const tree_t * prog) {
    int nk = 0;
    for (int i = 0; i < prog->n; i++) nk = pl_det_specs(pl_det_dir(prog->c[i]), NULL, nk, 0);
    if (!nk) return prog;
    const char * keys[nk];
    unsigned char done[nk];
    int nf = 0;
    memset(done, 0, sizeof done);
    for (int i = 0; i < prog->n; i++) nf = pl_det_specs(pl_det_dir(prog->c[i]), keys, nf, nk);
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        if (pl_det_dir(s)) { ast_push(np, (tree_t *) s); continue; }
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        int k = -1;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && subj->v.sval) for (int j = 0; j < nf; j++) if (!strcmp(keys[j], subj->v.sval)) k = j;
        if (k < 0) { ast_push(np, (tree_t *) s); continue; }
        const char * sl = strrchr(keys[k], '/');
        int ar = atoi(sl + 1);
        char nm[sl - keys[k] + 1], tn[sl - keys[k] + 6];
        memcpy(nm, keys[k], (size_t)(sl - keys[k]));
        nm[sl - keys[k]] = 0;
        snprintf(tn, sizeof tn, "$det %s", nm);
        tree_t * rn = pl_tree_copy(subj);
        char b[strlen(keys[k]) + 6];
        snprintf(b, sizeof b, "$det %s", keys[k]);
        rn->v.sval = (char *) lp_strdup(b);
        ast_push(np, pl_table_stmt(s, rn));
        if (done[k]) continue;
        done[k] = 1;
        const char * end = pl_det_prog_chain(prog, keys[k], 0);
        tree_t * cl = ast_node_new(TT_CLAUSE);
        cl->v.dval = ar;
        for (int j = 0; j < ar; j++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = j; ast_push(cl, v); }
        ast_push(cl, pl_cc_fnc3("$det_prop", pl_cc_key_pi(end ? end : keys[k]), pl_cc_key_pi(keys[k]), pl_table_goal(lp_strdup(tn), ar)));
        tree_t * ch = ast_node_new(TT_CHOICE);
        ch->v.sval = (char *) lp_strdup(keys[k]);
        ast_push(ch, cl);
        ast_push(np, pl_table_stmt(s, ch));
    }
    return np;
}
