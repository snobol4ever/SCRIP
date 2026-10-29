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
typedef struct { const char * name; int line; int dir; const tree_t * lhs; const tree_t * guard; const tree_t * body; } ch_rule_t;
typedef struct { long long old; int nw; } ch_vp_t;
typedef struct { ch_vp_t * p; int n; int next; } ch_vm_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ch_nodes(const tree_t * t) { int n = 1; if (!t) return 0; for (int i = 0; i < t->n; i++) n += ch_nodes(t->c[i]); return n; }
static int ch_vm_find(const ch_vm_t * m, long long old) { for (int i = 0; i < m->n; i++) if (m->p[i].old == old) return m->p[i].nw; return -1; }
static void ch_vm_bind(ch_vm_t * m, long long old, int nw) { m->p[m->n].old = old; m->p[m->n].nw = nw; m->n++; }
static int ch_vm_id(ch_vm_t * m, long long old) { int id = ch_vm_find(m, old); if (id < 0) { id = m->next++; ch_vm_bind(m, old, id); } return id; }
static tree_t * ch_copy(const tree_t * t, ch_vm_t * m) {
    if (!t) return NULL;
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    if (t->t == TT_VAR) c->v.ival = ch_vm_id(m, t->v.ival);
    for (int i = 0; i < t->n; i++) ast_push(c, ch_copy(t->c[i], m));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ch_is_arrow(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "->") && t->n >= 2; }
static void ch_flatten(tree_t * t, tree_t * prog) {
    while (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",") && t->n > 0) { for (int i = 0; i + 1 < t->n; i++) ch_flatten(t->c[i], prog); t = t->c[t->n - 1]; }
    if (!t || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ","))) return;
    ast_push(prog, t);
}
static tree_t * ch_control(tree_t * t);
static tree_t * ch_branch(tree_t * const * c, int from, int to) {
    tree_t * prog = ast_node_new(TT_PROGRAM);
    for (int i = from; i < to; i++) ch_flatten(c[i], prog);
    for (int i = 0; i < prog->n; i++) prog->c[i] = ch_control(prog->c[i]);
    return prog;
}
static tree_t * ch_fail(void) { return ch_atom("fail"); }
static tree_t * ch_control(tree_t * t) {
    if (!t) return t;
    if (ch_is_arrow(t)) {
        tree_t * then = ch_branch(t->c, 1, t->n), * iff = ast_node_new(TT_IF);
        iff->v.ival = 1;
        ast_push(iff, ch_control(t->c[0]));
        ast_push(iff, then->n == 1 ? then->c[0] : then);
        ast_push(iff, ch_fail());
        return iff;
    }
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ";") && t->n >= 2 && ch_is_arrow(t->c[0])) {
        tree_t * arrow = t->c[0], * then = ch_branch(arrow->c, 1, arrow->n), * els = ast_node_new(TT_PROGRAM), * iff = ast_node_new(TT_IF), * rest = t->c[1];
        if (t->n > 2) { rest = ast_node_new(TT_FNC); rest->v.sval = (char *) ";"; for (int i = 1; i < t->n; i++) ast_push(rest, t->c[i]); }
        ch_flatten(rest, els);
        for (int i = 0; i < els->n; i++) els->c[i] = ch_control(els->c[i]);
        ast_push(iff, ch_control(arrow->c[0]));
        ast_push(iff, then->n == 1 ? then->c[0] : then);
        ast_push(iff, els->n == 1 ? els->c[0] : els);
        return iff;
    }
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, ","))) for (int i = 0; i < t->n; i++) t->c[i] = ch_control(t->c[i]);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ch_rule_kind(const char * key) {
    const char * c = key ? strchr(key, ':') : NULL;
    if (c && c > key && c[1]) key = c + 1;
    return !key ? 0 : !strcmp(key, "<=>/2") ? 1 : !strcmp(key, "==>/2") ? 2 : !strcmp(key, "@/2") ? 3 : 0;
}
static int ch_rule_of(const tree_t * cl, int kind, ch_rule_t * r) {
    const tree_t * rule = cl->c[1];
    memset(r, 0, sizeof *r);
    r->line = cl->line;
    if (kind == 3) {
        if (cl->c[0] && cl->c[0]->v.sval) r->name = cl->c[0]->v.sval;
        if (ch_is_goal(rule, "pragma", 2)) rule = rule->c[0];
        if (!ch_is_goal(rule, "<=>", 2) && !ch_is_goal(rule, "==>", 2)) return 0;
        r->dir = ch_is_goal(rule, "==>", 2);
        r->lhs = rule->c[0];
        rule = rule->c[1];
    } else {
        r->dir = kind == 2;
        r->lhs = cl->c[0];
    }
    if (ch_is_goal(rule, "|", 2)) { r->guard = rule->c[0]; r->body = rule->c[1]; } else r->body = rule;
    return 1;
}
static int ch_head_count(const tree_t * t) { return (ch_is_goal(t, ",", 2) || ch_is_goal(t, "\\", 2)) ? ch_head_count(t->c[0]) + ch_head_count(t->c[1]) : 1; }
static const tree_t * ch_head_at(const tree_t * t, int i) {
    if (!(ch_is_goal(t, ",", 2) || ch_is_goal(t, "\\", 2))) return t;
    int a = ch_head_count(t->c[0]);
    return i < a ? ch_head_at(t->c[0], i) : ch_head_at(t->c[1], i - a);
}
static int ch_find(const ch_con_t * cons, int k, const tree_t * h) {
    const char * nm = (h && h->v.sval) ? h->v.sval : NULL;
    int ar = (h && h->t == TT_FNC) ? h->n : 0;
    for (int q = 0; nm && q < k; q++) if (cons[q].arity == ar && !strcmp(cons[q].name, nm)) return q;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { tree_t ** it; int n; } ch_out_t;
static void ch_put(ch_out_t * o, tree_t * g) { o->it[o->n++] = g; }
static int ch_pattern_atomic(const tree_t * p) { return !(p->t == TT_VAR || (p->t == TT_FNC && p->n > 0) || (p->t == TT_MAKELIST && p->n > 0)); }
static void ch_match(ch_vm_t * m, const tree_t * p, int v, ch_out_t * o) {
    if (p->t == TT_VAR) { int id = ch_vm_find(m, p->v.ival); if (id < 0) ch_vm_bind(m, p->v.ival, v); else ch_put(o, ch_f2("==", ch_var(v), ch_var(id))); return; }
    if (ch_pattern_atomic(p)) { ch_put(o, ch_f2("==", ch_var(v), ch_copy(p, m))); return; }
    int sub[p->n];
    tree_t * sk = ast_node_new(p->t);
    sk->v = p->v;
    sk->line = p->line;
    ch_put(o, ch_f1("nonvar", ch_var(v)));
    for (int i = 0; i < p->n; i++) {
        sub[i] = m->next++;
        ast_push(sk, ch_var(sub[i]));
        if (p->c[i]->t == TT_VAR && ch_vm_find(m, p->c[i]->v.ival) < 0) ch_vm_bind(m, p->c[i]->v.ival, sub[i]);
        else sub[i] = -sub[i] - 1;
    }
    ch_put(o, ch_f2("=", ch_var(v), sk));
    for (int i = 0; i < p->n; i++) if (sub[i] < 0) ch_match(m, p->c[i], -sub[i] - 1, o);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ch_removed(const ch_rule_t * r, int i) { if (r->dir) return 0; if (ch_is_goal(r->lhs, "\\", 2)) return i >= ch_head_count(r->lhs->c[0]); return 1; }
static int ch_occ_head(const ch_rule_t * r, int nh, int o) {
    int nrem = 0;
    for (int i = 0; i < nh; i++) nrem += ch_removed(r, i);
    for (int i = 0, seen = 0; i < nh; i++) if (ch_removed(r, i) == (o < nrem)) { if (seen == (o < nrem ? o : o - nrem)) return i; seen++; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_conj(tree_t * const * g, int n) { if (n == 0) return ch_atom("true"); tree_t * c = g[n - 1]; for (int i = n - 2; i >= 0; i--) c = ch_f2(",", g[i], c); return c; }
static tree_t * ch_goals_node(tree_t * const * g, int n) {
    if (n == 1) return g[0];
    tree_t * p = ast_node_new(TT_PROGRAM);
    for (int i = 0; i < n; i++) ast_push(p, g[i]);
    if (n == 0) ast_push(p, ch_atom("true"));
    return p;
}
static tree_t * ch_if(tree_t * c, tree_t * t, tree_t * e) { tree_t * iff = ast_node_new(TT_IF); ast_push(iff, c); ast_push(iff, t); ast_push(iff, e); return iff; }
static const char * ch_level_name(const ch_con_t * c, int j, int l) { char b[strlen(c->name) + 56]; snprintf(b, sizeof b, "%s___%d__%d__%d", c->name, c->arity, j, l); return lp_strdup(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ch_remove(ch_out_t * o, ch_vm_t * m, const ch_con_t * c, int idx, int k, const char * mod, int sv, const int * av) {
    int n = c->arity, Vs = m->next++, St = m->next++, St1 = m->next++, w[n > 0 ? n : 1];
    const char * key = ch_store_key(mod, c);
    tree_t * wa[n > 0 ? n : 1];
    if (!av) {
        tree_t * sa[n + 6];
        for (int i = 0; i < 6; i++) sa[i] = ch_var(m->next++);
        for (int i = 0; i < n; i++) { w[i] = m->next++; sa[6 + i] = ch_var(w[i]); }
        ch_put(o, ch_f2("=", ch_var(sv), ch_fnca("suspension", n + 6, sa)));
        av = w;
    }
    for (int i = 0; i < n; i++) wa[i] = ch_var(av[i]);
    ch_put(o, ch_f3("setarg", ch_int(2), ch_var(sv), ch_atom("removed")));
    if (n > 0) ch_put(o, ch_f2("term_variables", n == 1 ? ch_var(av[0]) : ch_list(wa, n, NULL), ch_var(Vs)));
    ch_put(o, ch_f2("nb_getval", ch_atom(key), ch_var(St)));
    ch_put(o, ch_f3("chr sbag_del_element", ch_var(St), ch_var(sv), ch_var(St1)));
    ch_put(o, ch_f2("b_setval", ch_atom(key), ch_var(St1)));
    if (n > 0) ch_put(o, ch_f5("$chr_detach", ch_atom(mod), ch_int(idx), ch_int(k), ch_var(Vs), ch_var(sv)));
}
static void ch_goals(ch_out_t * o, const tree_t * t, ch_vm_t * m) {
    if (!t) return;
    tree_t * prog = ast_node_new(TT_PROGRAM);
    ch_flatten(ch_copy(t, m), prog);
    for (int i = 0; i < prog->n; i++) { tree_t * g = ch_control(prog->c[i]); if (!(g->t == TT_QLIT && g->v.sval && !strcmp(g->v.sval, "true"))) ch_put(o, g); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_args_call(const char * f, int n, int extra) { tree_t * a[n + 1]; for (int i = 0; i < n; i++) a[i] = ch_var(i); a[n] = ch_var(extra); return ch_fnca(f, n + 1, a); }
static tree_t * ch_fall_through(const ch_con_t * c, int j) {
    int n = c->arity;
    tree_t * it[n + 2];
    for (int i = 0; i <= n; i++) it[i] = ch_var(i);
    it[n + 1] = ch_args_call(ch_occ_name(c, j + 1), n, n);
    return ch_clause(n + 1, it, n + 2);
}
static tree_t * ch_simplification(const ch_con_t * c, int idx, int k, const char * mod, const ch_rule_t * r, const tree_t * head) {
    int n = c->arity, nv = ch_nodes(r->lhs) + ch_nodes(r->guard) + ch_nodes(r->body) + n + 8, av[n > 0 ? n : 1];
    ch_vp_t mp[nv];
    ch_vm_t m = { mp, 0, n + 1 };
    tree_t * it[3 * nv + 3 * n + 16];
    ch_out_t o = { it, 0 };
    for (int i = 0; i <= n; i++) ch_put(&o, ch_var(i));
    for (int i = 0; i < n; i++) { ch_match(&m, head->c[i], i, &o); av[i] = i; }
    ch_goals(&o, r->guard, &m);
    ch_put(&o, ast_node_new(TT_CUT));
    ch_remove(&o, &m, c, idx, k, mod, n, av);
    ch_goals(&o, r->body, &m);
    return ch_clause(n + 1, it, o.n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const ch_con_t * cons; int k; const char * mod; const ch_rule_t * r; int nh; int self; int j; int q; } ch_occ_t;
typedef struct { int nr, n, nc, ne, rbase, sid, abase, cbase, ebase, nfree, total; } ch_lay_t;
static ch_lay_t ch_layout(int nr, int n, int nc, int ne) { ch_lay_t y = { nr, n, nc, ne, 2, 2 + nr, 3 + nr, 3 + nr + n, 3 + nr + n + nc, 3 + nr + n + nc + ne, 2 + nr + n + nc + ne }; return y; }
static void ch_ids(int * ids, int first, int count) { for (int i = 0; i < count; i++) ids[i] = first + i; }
static tree_t * ch_call(const char * f, tree_t * list, int nr, const int * rv, int sv, int n, const int * av, int nc, const int * cv, int ne, const int * ev) {
    int total = 1 + nr + 1 + n + nc + ne, p = 0;
    tree_t * a[total];
    a[p++] = list;
    for (int i = 0; i < nr; i++) a[p++] = ch_var(rv[i]);
    a[p++] = ch_var(sv);
    for (int i = 0; i < n; i++) a[p++] = ch_var(av[i]);
    for (int i = 0; i < nc; i++) a[p++] = ch_var(cv[i]);
    for (int i = 0; i < ne; i++) a[p++] = ch_var(ev[i]);
    return ch_fnca(f, total, a);
}
static tree_t * ch_susp_pattern(ch_vm_t * m, int cand, int arity, int * xs) {
    tree_t * sa[arity + 6];
    sa[0] = ch_var(m->next++);
    sa[1] = ch_atom("active");
    for (int i = 2; i < 6; i++) sa[i] = ch_var(m->next++);
    for (int i = 0; i < arity; i++) { xs[i] = m->next++; sa[6 + i] = ch_var(xs[i]); }
    return ch_f2("=", ch_var(cand), ch_fnca("suspension", arity + 6, sa));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_level_base(const ch_occ_t * oc, int level, const int * ne, const ch_lay_t * y) {
    const ch_con_t * c = &oc->cons[oc->q];
    int n = y->n, nr = y->nr, nc = y->nc, nei = y->ne, hn = 0, f = 0, sid;
    tree_t * hb[y->total + 2];
    int rid[nr + 1], aid[n + 1], cid[nc + 1], eid[nei + 1];
    hb[hn++] = ch_list(NULL, 0, NULL);
    for (int i = 0; i < nr; i++) { rid[i] = f++; hb[hn++] = ch_var(rid[i]); }
    sid = f++;
    hb[hn++] = ch_var(sid);
    for (int i = 0; i < n; i++) { aid[i] = f++; hb[hn++] = ch_var(aid[i]); }
    for (int i = 0; i < nc; i++) { cid[i] = f++; hb[hn++] = ch_var(cid[i]); }
    for (int i = 0; i < nei; i++) { eid[i] = f++; hb[hn++] = ch_var(eid[i]); }
    if (level == 1) {
        tree_t * ca[n + 1];
        for (int i = 0; i < n; i++) ca[i] = ch_var(aid[i]);
        ca[n] = ch_var(sid);
        hb[hn++] = ch_fnca(ch_occ_name(c, oc->j + 1), n + 1, ca);
    } else hb[hn++] = ch_call(ch_level_name(c, oc->j, level - 1), ch_var(rid[nr - 1]), nr - 1, rid, sid, n, aid, nc - 1, cid, ne[level - 1], eid);
    return ch_clause(y->total, hb, hn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_level_loop(const ch_occ_t * oc, int level, const int * pq, const int * pidx, long long * env, int * ne, int nv, const ch_lay_t * y) {
    const ch_con_t * c = &oc->cons[oc->q], * pc = &oc->cons[pq[level - 1]];
    const ch_rule_t * r = oc->r;
    const tree_t * ph = ch_head_at(r->lhs, pidx[level - 1]);
    int n = y->n, m = oc->nh - 1, nr = y->nr, nc = y->nc, nei = y->ne, S = y->sid, selfrem = ch_removed(r, oc->self), cap = 4 * nv + 16 * oc->nh + 4 * n + 64;
    int rv[nr + 1], av[n + 1], cv[nc + 1], ev[nei + 1], cand = 0, rest = 1, xs[pc->arity + 1], hn = 0;
    ch_ids(rv, y->rbase, nr);
    ch_ids(av, y->abase, n);
    ch_ids(cv, y->cbase, nc);
    ch_ids(ev, y->ebase, nei);
    ch_vp_t mp[nv + 1];
    ch_vm_t mm = { mp, 0, y->nfree };
    tree_t * cb[cap], * tb[cap], * hb[y->total + 2], * cons1[1] = { ch_var(cand) };
    ch_out_t co = { cb, 0 }, to = { tb, 0 };
    for (int i = 0; i < nei; i++) ch_vm_bind(&mm, env[level * nv + i], ev[i]);
    ch_put(&co, ch_susp_pattern(&mm, cand, pc->arity, xs));
    for (int i = 0; i < level - 1; i++) if (pq[i] == pq[level - 1]) ch_put(&co, ch_f2("\\==", ch_var(cand), ch_var(cv[i])));
    for (int a = 0; a < pc->arity; a++) ch_match(&mm, ph->c[a], xs[a], &co);
    if (level == m) ch_goals(&co, r->guard, &mm);
    hb[hn++] = ch_list(cons1, 1, ch_var(rest));
    for (int i = 0; i < nr; i++) hb[hn++] = ch_var(rv[i]);
    hb[hn++] = ch_var(S);
    for (int i = 0; i < n; i++) hb[hn++] = ch_var(av[i]);
    for (int i = 0; i < nc; i++) hb[hn++] = ch_var(cv[i]);
    for (int i = 0; i < nei; i++) hb[hn++] = ch_var(ev[i]);
    if (level < m) {
        int nout = mm.n, L = mm.next++, r2[nr + 1], c2[nc + 1], e2[nout + 1];
        for (int i = 0; i < nr; i++) r2[i] = rv[i];
        r2[nr] = rest;
        for (int i = 0; i < nc; i++) c2[i] = cv[i];
        c2[nc] = cand;
        for (int i = 0; i < nout; i++) { e2[i] = mm.p[i].nw; env[(level + 1) * nv + i] = mm.p[i].old; }
        ne[level + 1] = nout;
        ch_put(&to, ch_f2("nb_getval", ch_atom(ch_store_key(oc->mod, &oc->cons[pq[level]])), ch_var(L)));
        ch_put(&to, ch_call(ch_level_name(c, oc->j, level + 1), ch_var(L), nr + 1, r2, S, n, av, nc + 1, c2, nout, e2));
    } else {
        for (int i = 1; i <= m; i++) if (ch_removed(r, pidx[i - 1])) ch_remove(&to, &mm, &oc->cons[pq[i - 1]], pq[i - 1] + 1, oc->k, oc->mod, i == m ? cand : cv[i - 1], NULL);
        int G0 = 0, G = 0;
        if (selfrem) ch_remove(&to, &mm, c, oc->q + 1, oc->k, oc->mod, S, av);
        else {
            G0 = mm.next++;
            G = mm.next++;
            ch_put(&to, ch_f3("arg", ch_int(4), ch_var(S), ch_var(G0)));
            ch_put(&to, ch_f2("is", ch_var(G), ch_f2("+", ch_var(G0), ch_int(1))));
            ch_put(&to, ch_f3("setarg", ch_int(4), ch_var(S), ch_var(G)));
            ch_put(&to, ch_f3("setarg", ch_int(2), ch_var(S), ch_atom("active")));
        }
        ch_goals(&to, r->body, &mm);
        if (!selfrem) {
            tree_t * alive[2] = { ch_f3("arg", ch_int(2), ch_var(S), ch_atom("active")), ch_f3("arg", ch_int(4), ch_var(S), ch_var(G)) }, * th[2], * ck[2 * m + 1];
            int nrem = 0;
            for (int i = 1; i < m; i++) {
                if (!ch_removed(r, pidx[i - 1])) continue;
                int st = mm.next++;
                ck[nrem++] = ch_f3("arg", ch_int(2), ch_var(cv[i - 1]), ch_var(st));
                ck[nrem++] = ch_f2("\\==", ch_var(st), ch_atom("removed"));
            }
            tree_t * again = ch_call(ch_level_name(c, oc->j, level), ch_var(rest), nr, rv, S, n, av, nc, cv, nei, ev);
            if (nrem) again = ch_if(ch_conj(ck, nrem), again, ch_call(ch_level_name(c, oc->j, level), ch_list(NULL, 0, NULL), nr, rv, S, n, av, nc, cv, nei, ev));
            th[0] = ch_f3("setarg", ch_int(2), ch_var(S), ch_atom("inactive"));
            th[1] = again;
            ch_put(&to, ch_if(ch_conj(alive, 2), ch_goals_node(th, 2), ch_atom("true")));
        }
    }
    hb[hn++] = ch_if(ch_conj(cb, co.n), ch_goals_node(tb, to.n), ch_call(ch_level_name(c, oc->j, level), ch_var(rest), nr, rv, S, n, av, nc, cv, nei, ev));
    return ch_clause(y->total, hb, hn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ch_emit_multi(tree_t * np, const tree_t * ds, const ch_occ_t * oc) {
    const ch_con_t * c = &oc->cons[oc->q];
    const ch_rule_t * r = oc->r;
    int n = c->arity, nh = oc->nh, m = nh - 1, nv = ch_nodes(r->lhs) + ch_nodes(r->guard) + ch_nodes(r->body) + 2 * nh + n + 16, cap = 4 * nv + 16 * nh + 4 * n + 64;
    int pidx[m], pq[m], ne[m + 2], av[n + 1], ev[nv + 1], L;
    long long env[(m + 2) * nv];
    for (int i = 0, p = 0; i < nh; i++) if (i != oc->self) { pidx[p] = i; pq[p] = ch_find(oc->cons, oc->k, ch_head_at(r->lhs, i)); p++; }
    ch_vp_t mp[nv];
    ch_vm_t mm = { mp, 0, n + 1 };
    tree_t * it[cap];
    ch_out_t o = { it, 0 };
    const tree_t * sh = ch_head_at(r->lhs, oc->self);
    for (int i = 0; i <= n; i++) ch_put(&o, ch_var(i));
    for (int i = 0; i < n; i++) { ch_match(&mm, sh->c[i], i, &o); av[i] = i; }
    ch_put(&o, ast_node_new(TT_CUT));
    L = mm.next++;
    ch_put(&o, ch_f2("nb_getval", ch_atom(ch_store_key(oc->mod, &oc->cons[pq[0]])), ch_var(L)));
    ne[1] = mm.n;
    for (int i = 0; i < mm.n; i++) { env[nv + i] = mm.p[i].old; ev[i] = mm.p[i].nw; }
    ch_put(&o, ch_call(ch_level_name(c, oc->j, 1), ch_var(L), 0, NULL, n, n, av, 0, NULL, ne[1], ev));
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) ch_key_of(ch_occ_name(c, oc->j), n + 1);
    ast_push(ch, ch_clause(n + 1, it, o.n));
    ast_push(ch, ch_fall_through(c, oc->j));
    ast_push(np, ch_stmt(ds, ch));
    for (int level = 1; level <= m; level++) {
        ch_lay_t y = ch_layout(level - 1, n, level - 1, ne[level]);
        tree_t * lv = ast_node_new(TT_CHOICE);
        lv->v.sval = (char *) ch_key_of(ch_level_name(c, oc->j, level), y.total);
        ast_push(lv, ch_level_base(oc, level, ne, &y));
        ast_push(lv, ch_level_loop(oc, level, pq, pidx, env, ne, nv, &y));
        ast_push(np, ch_stmt(ds, lv));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * ch_occurrence(const ch_con_t * c, int idx, int k, const char * mod, int j, const ch_rule_t * r, const tree_t * head) {
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) ch_key_of(ch_occ_name(c, j), c->arity + 1);
    if (r) ast_push(ch, ch_simplification(c, idx, k, mod, r, head));
    ast_push(ch, ch_fall_through(c, j));
    return ch;
}
static int ch_heads_declared(const ch_con_t * cons, int k, const ch_rule_t * r, int nh) { for (int i = 0; i < nh; i++) if (ch_find(cons, k, ch_head_at(r->lhs, i)) < 0) return 0; return 1; }
static int ch_any_removed(const ch_rule_t * r, int nh) { for (int i = 0; i < nh; i++) if (ch_removed(r, i)) return 1; return 0; }
static void ch_emit_occurrence(tree_t * np, const tree_t * ds, const ch_con_t * cons, int k, const char * mod, int q, int j, const ch_rule_t * r, int nh, int self) {
    ch_occ_t oc = { cons, k, mod, r, nh, self, j, q };
    if (nh == 1 && ch_removed(r, 0)) ast_push(np, ch_stmt(ds, ch_occurrence(&cons[q], q + 1, k, mod, j, r, ch_head_at(r->lhs, 0))));
    else if (nh >= 2 && ch_any_removed(r, nh) && ch_heads_declared(cons, k, r, nh)) ch_emit_multi(np, ds, &oc);
    else ast_push(np, ch_stmt(ds, ch_occurrence(&cons[q], q + 1, k, mod, j, NULL, NULL)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * ch_module_of(const tree_t * prog) {
    for (int i = 0; i < prog->n; i++) { const tree_t * d = ch_subj(prog->c[i]); if (ch_is_goal(d, "module", 2) && d->c[0] && d->c[0]->v.sval) return d->c[0]->v.sval; }
    return "user";
}
static tree_t * ch_unify_hook(const char * mod) {
    tree_t * it[3] = { ch_var(0), ch_var(1), ch_f3("$chr_unify", ch_atom(mod), ch_var(0), ch_var(1)) };
    return ch_choice("attr_unify_hook/2", ch_clause(2, it, 3));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_chr_rewrite(const tree_t * prog) {
    int nc = 0, decl = -1, init = -1, clauses = prog->n, nr = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = ch_subj(prog->c[i]);
        if (ch_is_decl(d)) { nc = ch_specs(d->c[0], NULL, nc, 0); if (decl < 0) decl = i; }
        if (ch_is_init(d) && init < 0) init = i;
        if (d && (d->t == TT_CHOICE || d->t == TT_CLAUSE) && clauses == prog->n) clauses = i;
        if (d && d->t == TT_CHOICE && ch_rule_kind(d->v.sval)) nr += d->n;
    }
    if (!nc) return prog;
    ch_con_t cons[nc];
    ch_rule_t rules[nr + 1];
    int k = 0;
    nr = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = ch_subj(prog->c[i]);
        if (ch_is_decl(d)) k = ch_specs(d->c[0], cons, k, nc);
        int kind = (d && d->t == TT_CHOICE) ? ch_rule_kind(d->v.sval) : 0;
        for (int q = 0; kind && q < d->n; q++) if (d->c[q]->n == 2 && ch_rule_of(d->c[q], kind, &rules[nr])) nr++;
    }
    for (int a = 1; a < nr; a++) for (int b = a; b > 0 && rules[b].line < rules[b - 1].line; b--) { ch_rule_t t = rules[b]; rules[b] = rules[b - 1]; rules[b - 1] = t; }
    int total = 0;
    for (int r = 0; r < nr; r++) total += ch_head_count(rules[r].lhs);
    int ocon[total + 1], orule[total + 1], ohead[total + 1], no = 0;
    for (int r = 0; r < nr; r++) {
        for (int o = 0, nh = ch_head_count(rules[r].lhs); o < nh; o++) {
            int hi = ch_occ_head(&rules[r], nh, o), q = ch_find(cons, k, ch_head_at(rules[r].lhs, hi));
            if (q >= 0) { ocon[no] = q; orule[no] = r; ohead[no] = hi; no++; }
        }
    }
    const char * mod = ch_module_of(prog);
    int at = (init >= 0 && init < decl) ? init : decl;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i <= prog->n; i++) {
        const tree_t * s = i < prog->n ? prog->c[i] : NULL;
        if (i == clauses) {
            const tree_t * ds = prog->c[decl];
            ast_push(np, ch_stmt(ds, ch_constraint_facts(cons, k, mod)));
            for (int q = 0; q < k; q++) {
                int j = 0;
                ast_push(np, ch_stmt(ds, ch_activation(&cons[q], q + 1, k, mod)));
                for (int o = 0; o < no; o++) if (ocon[o] == q) ch_emit_occurrence(np, ds, cons, k, mod, q, j++, &rules[orule[o]], ch_head_count(rules[orule[o]].lhs), ohead[o]);
                ast_push(np, ch_stmt(ds, ch_occurrence_last(&cons[q], j)));
            }
            ast_push(np, ch_stmt(ds, ch_unify_hook(mod)));
            ast_push(np, ch_stmt(ds, ch_initialization(cons, k, mod)));
            ast_push(np, ch_stmt(ds, ch_module_fact(mod)));
        }
        if (!s) break;
        if (i == at) ast_push(np, ch_stmt(prog->c[decl], ch_f1("initialization", ch_atom("$chr_initialization"))));
        const tree_t * sd = ch_subj(s);
        if (ch_is_consumed(sd) || (sd && sd->t == TT_CHOICE && ch_rule_kind(sd->v.sval))) continue;
        ast_push(np, (tree_t *) s);
    }
    return np;
}
