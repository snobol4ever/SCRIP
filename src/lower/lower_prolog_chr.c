#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "lower.h"
typedef struct { const char * name; int arity; } ch_con_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_atom(const char * s) { tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) s; return n; }
static tree_t * ch_int(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * ch_var(int id) { tree_t * n = ast_node_new(TT_VAR); n->v.ival = id; return n; }
static tree_t * ch_fnca(const char * f, int n, tree_t * const * a) {
    tree_t * g = ast_node_new(n > 0 ? TT_FNC : TT_QLIT);
    g->v.sval = (char *) f;
    for (int i = 0; i < n; i++) ast_push(g, a[i]);
    return g;
}
static tree_t * ch_f1(const char * f, tree_t * a) { tree_t * v[1] = { a }; return ch_fnca(f, 1, v); }
static tree_t * ch_f2(const char * f, tree_t * a, tree_t * b) { tree_t * v[2] = { a, b }; return ch_fnca(f, 2, v); }
static tree_t * ch_f3(const char * f, tree_t * a, tree_t * b, tree_t * c) { tree_t * v[3] = { a, b, c }; return ch_fnca(f, 3, v); }
static tree_t * ch_f5(const char * f, tree_t * a, tree_t * b, tree_t * c, tree_t * d, tree_t * e) { tree_t * v[5] = { a, b, c, d, e }; return ch_fnca(f, 5, v); }
static tree_t * ch_list(tree_t * const * a, int n, tree_t * tail) {
    tree_t * l = ast_node_new(TT_MAKELIST);
    l->v.ival = tail ? 1 : 0;
    for (int i = 0; i < n; i++) ast_push(l, a[i]);
    if (tail) ast_push(l, tail);
    return l;
}
static tree_t * ch_cons(tree_t * h, tree_t * t) { tree_t * v[1] = { h }; return ch_list(v, 1, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_clause(int ar, tree_t * const * it, int n) { tree_t * cl = ast_node_new(TT_CLAUSE); cl->v.dval = ar; for (int i = 0; i < n; i++) ast_push(cl, it[i]); return cl; }
static tree_t * ch_choice(const char * key, tree_t * cl) { tree_t * ch = ast_node_new(TT_CHOICE); ch->v.sval = (char *) lp_strdup(key); ast_push(ch, cl); return ch; }
static tree_t * ch_stmt(const tree_t * s, tree_t * subj) {
    tree_t * st = ast_node_new(TT_STMT);
    const tree_t * old = stmt_attr_find(s, ":subj");
    for (int i = 0; i < s->n; i++) ast_push(st, s->c[i] == old ? ast_attr_expr(":subj", subj) : s->c[i]);
    st->line = s->line;
    return st;
}
static const tree_t * ch_subj(const tree_t * s) { return (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL; }
static int ch_is_goal(const tree_t * d, const char * f, int n) { return d && d->t == TT_FNC && d->v.sval && d->n == n && !strcmp(d->v.sval, f); }
static int ch_is_decl(const tree_t * d) { return ch_is_goal(d, "chr_constraint", 1) || ch_is_goal(d, "constraints", 1); }
static int ch_is_consumed(const tree_t * d) { return ch_is_decl(d) || ch_is_goal(d, "chr_option", 2) || ch_is_goal(d, "chr_type", 1); }
static int ch_is_init(const tree_t * d) { return ch_is_goal(d, "initialization", 1) || ch_is_goal(d, "initialization", 2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ch_specs(const tree_t * a, ch_con_t * out, int n, int cap) {
    if (!a) return n;
    if (ch_is_goal(a, ",", 2)) return ch_specs(a->c[1], out, ch_specs(a->c[0], out, n, cap), cap);
    if (a->t == TT_MAKELIST) { for (int i = 0; i < a->n; i++) n = ch_specs(a->c[i], out, n, cap); return n; }
    const char * nm = NULL;
    int ar = 0;
    if (ch_is_goal(a, "/", 2) && a->c[0] && a->c[0]->v.sval && a->c[1] && a->c[1]->t == TT_ILIT) {
        nm = a->c[0]->v.sval;
        ar = (int) a->c[1]->v.ival;
    } else if (a->t == TT_FNC && a->v.sval) {
        nm = a->v.sval;
        ar = a->n;
    } else if ((a->t == TT_QLIT || a->t == TT_NAME) && a->v.sval) nm = a->v.sval;
    if (!nm) return n;
    for (int i = 0; i < n && out; i++) if (out[i].arity == ar && !strcmp(out[i].name, nm)) return n;
    if (out && n < cap) { out[n].name = lp_strdup(nm); out[n].arity = ar; }
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * ch_store_key(const char * mod, const ch_con_t * c) {
    char b[strlen(mod) + strlen(c->name) + 48];
    snprintf(b, sizeof b, "$chr_store_global_list_%s____%s___%d", mod, c->name, c->arity);
    return lp_strdup(b);
}
static const char * ch_occ_name(const ch_con_t * c, int j) { char b[strlen(c->name) + 40]; snprintf(b, sizeof b, "%s___%d__%d", c->name, c->arity, j); return lp_strdup(b); }
static const char * ch_key_of(const char * nm, int ar) { char b[strlen(nm) + 16]; snprintf(b, sizeof b, "%s/%d", nm, ar); return lp_strdup(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_activation(const ch_con_t * c, int idx, int k, const char * mod) {
    int n = c->arity, S = n, Id = n + 1, H = n + 2, Vs = n + 3, Old = n + 4;
    const char * key = ch_store_key(mod, c);
    tree_t * it[n + 16], * sa[n + 6], * oa[n + 1], * av[n > 0 ? n : 1];
    int m = 0, p = 0;
    for (int i = 0; i < n; i++) it[m++] = ch_var(i);
    sa[p++] = ch_var(Id);
    sa[p++] = ch_atom("active");
    sa[p++] = ch_var(H);
    sa[p++] = ch_int(0);
    for (int i = 0; i < n; i++) oa[i] = ch_var(i);
    oa[n] = ch_var(S);
    sa[p++] = ch_f2(":", ch_atom(mod), ch_fnca(ch_occ_name(c, 0), n + 1, oa));
    sa[p++] = ch_atom(c->name);
    for (int i = 0; i < n; i++) sa[p++] = ch_var(i);
    it[m++] = ch_f2("=", ch_var(S), ch_fnca("suspension", p, sa));
    for (int i = 0; i < n; i++) av[i] = ch_var(i);
    if (n > 0) { it[m++] = ch_f2("term_variables", n == 1 ? ch_var(0) : ch_list(av, n, NULL), ch_var(Vs)); it[m++] = ch_f1("chr none_locked", ch_var(Vs)); }
    it[m++] = ch_f1("chr gen_id", ch_var(Id));
    it[m++] = ch_f2("nb_getval", ch_atom(key), ch_var(Old));
    it[m++] = ch_f2("b_setval", ch_atom(key), ch_cons(ch_var(S), ch_var(Old)));
    if (n > 0) it[m++] = ch_f5("$chr_attach", ch_atom(mod), ch_int(idx), ch_int(k), ch_var(Vs), ch_var(S));
    it[m++] = ch_f3("setarg", ch_int(2), ch_var(S), ch_atom("inactive"));
    for (int i = 0; i < n; i++) oa[i] = ch_var(i);
    oa[n] = ch_var(S);
    it[m++] = ch_fnca(ch_occ_name(c, 0), n + 1, oa);
    return ch_choice(ch_key_of(c->name, n), ch_clause(n, it, m));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_occurrence_last(const ch_con_t * c, int j) {
    int n = c->arity;
    tree_t * it[n + 2];
    for (int i = 0; i <= n; i++) it[i] = ch_var(i);
    it[n + 1] = ch_f3("setarg", ch_int(2), ch_var(n), ch_atom("active"));
    return ch_choice(ch_key_of(ch_occ_name(c, j), n + 1), ch_clause(n + 1, it, n + 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_constraint_facts(const ch_con_t * cons, int k, const char * mod) {
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) "$chr_constraint/3";
    for (int i = 0; i < k; i++) {
        tree_t * it[3] = { ch_f2("/", ch_atom(cons[i].name), ch_int(cons[i].arity)), ch_int(i + 1), ch_atom(ch_store_key(mod, &cons[i])) };
        ast_push(ch, ch_clause(3, it, 3));
    }
    return ch;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_initialization(const ch_con_t * cons, int k, const char * mod) {
    tree_t * it[k > 0 ? k : 1];
    for (int i = 0; i < k; i++) it[i] = ch_f2("nb_setval", ch_atom(ch_store_key(mod, &cons[k - 1 - i])), ch_list(NULL, 0, NULL));
    return ch_choice("$chr_initialization/0", ch_clause(0, it, k));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_module_fact(const char * mod) { tree_t * it[1] = { ch_atom(mod) }; return ch_choice("$chr_module/1", ch_clause(1, it, 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_chr_rewrite(const tree_t * prog) {
    int nc = 0, decl = -1, init = -1, clauses = prog->n;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = ch_subj(prog->c[i]);
        if (ch_is_decl(d)) { nc = ch_specs(d->c[0], NULL, nc, 0); if (decl < 0) decl = i; }
        if (ch_is_init(d) && init < 0) init = i;
        if (d && (d->t == TT_CHOICE || d->t == TT_CLAUSE) && clauses == prog->n) clauses = i;
    }
    if (!nc) return prog;
    ch_con_t cons[nc];
    int k = 0;
    for (int i = 0; i < prog->n; i++) { const tree_t * d = ch_subj(prog->c[i]); if (ch_is_decl(d)) k = ch_specs(d->c[0], cons, k, nc); }
    const char * mod = "user";
    int at = (init >= 0 && init < decl) ? init : decl;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i <= prog->n; i++) {
        const tree_t * s = i < prog->n ? prog->c[i] : NULL;
        if (i == clauses) {
            const tree_t * ds = prog->c[decl];
            ast_push(np, ch_stmt(ds, ch_constraint_facts(cons, k, mod)));
            for (int q = 0; q < k; q++) { ast_push(np, ch_stmt(ds, ch_activation(&cons[q], q + 1, k, mod))); ast_push(np, ch_stmt(ds, ch_occurrence_last(&cons[q], 0))); }
            ast_push(np, ch_stmt(ds, ch_initialization(cons, k, mod)));
            ast_push(np, ch_stmt(ds, ch_module_fact(mod)));
        }
        if (!s) break;
        if (i == at) ast_push(np, ch_stmt(prog->c[decl], ch_f1("initialization", ch_atom("$chr_initialization"))));
        if (ch_is_consumed(ch_subj(s))) continue;
        ast_push(np, (tree_t *) s);
    }
    return np;
}
