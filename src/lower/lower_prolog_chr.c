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
static void ch_removal(ch_out_t * o, ch_vm_t * m, const ch_con_t * c, int idx, int k, const char * mod) {
    int n = c->arity, S = n, Vs = m->next++, St = m->next++, St1 = m->next++;
    const char * key = ch_store_key(mod, c);
    tree_t * av[n > 0 ? n : 1];
    for (int i = 0; i < n; i++) av[i] = ch_var(i);
    ch_put(o, ch_f3("setarg", ch_int(2), ch_var(S), ch_atom("removed")));
    if (n > 0) ch_put(o, ch_f2("term_variables", n == 1 ? ch_var(0) : ch_list(av, n, NULL), ch_var(Vs)));
    ch_put(o, ch_f2("nb_getval", ch_atom(key), ch_var(St)));
    ch_put(o, ch_f3("chr sbag_del_element", ch_var(St), ch_var(S), ch_var(St1)));
    ch_put(o, ch_f2("b_setval", ch_atom(key), ch_var(St1)));
    if (n > 0) ch_put(o, ch_f5("$chr_detach", ch_atom(mod), ch_int(idx), ch_int(k), ch_var(Vs), ch_var(S)));
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
    int n = c->arity, nv = ch_nodes(r->lhs) + ch_nodes(r->guard) + ch_nodes(r->body) + n + 8;
    ch_vp_t mp[nv];
    ch_vm_t m = { mp, 0, n + 1 };
    tree_t * it[3 * nv + 3 * n + 16];
    ch_out_t o = { it, 0 };
    for (int i = 0; i <= n; i++) ch_put(&o, ch_var(i));
    for (int i = 0; i < n; i++) ch_match(&m, head->c[i], i, &o);
    ch_goals(&o, r->guard, &m);
    ch_put(&o, ast_node_new(TT_CUT));
    ch_removal(&o, &m, c, idx, k, mod);
    ch_goals(&o, r->body, &m);
    return ch_clause(n + 1, it, o.n);
}
static tree_t * ch_occurrence(const ch_con_t * c, int idx, int k, const char * mod, int j, const ch_rule_t * r, const tree_t * head) {
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) ch_key_of(ch_occ_name(c, j), c->arity + 1);
    if (r) ast_push(ch, ch_simplification(c, idx, k, mod, r, head));
    ast_push(ch, ch_fall_through(c, j));
    return ch;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * ch_module_of(const tree_t * prog) {
    for (int i = 0; i < prog->n; i++) { const tree_t * d = ch_subj(prog->c[i]); if (ch_is_goal(d, "module", 2) && d->c[0] && d->c[0]->v.sval) return d->c[0]->v.sval; }
    return "user";
}
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
        for (int h = 0, hc = ch_head_count(rules[r].lhs); h < hc; h++) { int q = ch_find(cons, k, ch_head_at(rules[r].lhs, h)); if (q >= 0) { ocon[no] = q; orule[no] = r; ohead[no] = h; no++; } }
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
                for (int o = 0; o < no; o++) {
                    if (ocon[o] != q) continue;
                    const ch_rule_t * r = &rules[orule[o]];
                    int simple = r->dir == 0 && ch_head_count(r->lhs) == 1;
                    ast_push(np, ch_stmt(ds, ch_occurrence(&cons[q], q + 1, k, mod, j++, simple ? r : NULL, simple ? ch_head_at(r->lhs, 0) : NULL)));
                }
                ast_push(np, ch_stmt(ds, ch_occurrence_last(&cons[q], j)));
            }
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
