#include "lower/lower_prolog/lpl_head.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_06_pl.h"
static tree_t * pl_cutto_goal(tree_t * g, const tree_t * cl, int body0);
int pl_var_nodes(const tree_t * t) { int n; if (!t) return 0; n = (t->t == TT_VAR); for (int i = 0; i < t->n; i++) n += pl_var_nodes(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int max_var_slot(const tree_t * t, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot(t->c[i], mx);
    return mx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_collect_vars(const tree_t * t, int * slots, int * n, int cap) {
    if (!t || *n >= cap) return;
    if (t->t == TT_VAR) { int sl = (int) t->v.ival; for (int i = 0; i < *n; i++) if (slots[i] == sl) return; if (*n < cap) slots[(*n)++] = sl; return; }
    for (int i = 0; i < t->n; i++) pl_collect_vars(t->c[i], slots, n, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_free_vars(const tree_t * tmpl, const tree_t * goal, int * out, int cap) {
    int nvn = pl_var_nodes(tmpl) + pl_var_nodes(goal) + 1;
    int bound[nvn], nb = 0, gv[nvn], ng = 0, n = 0;
    const tree_t * g = goal;
    pl_collect_vars(tmpl, bound, &nb, nvn);
    while (g && g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "^") && g->n == 2) { pl_collect_vars(g->c[0], bound, &nb, nvn); g = g->c[1]; }
    pl_collect_vars(g, gv, &ng, nvn);
    for (int i = 0; i < ng && n < cap; i++) { int f = 0; for (int j = 0; j < nb; j++) if (bound[j] == gv[i]) { f = 1; break; } if (!f) out[n++] = gv[i]; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_var_count(const tree_t * t, int slot) {
    if (!t) return 0;
    if (t->t == TT_VAR) return (int) t->v.ival == slot;
    { int n = 0; for (int i = 0; i < t->n; i++) n += pl_var_count(t->c[i], slot); return n; }
}
static const char * const pl_rung6_builtins[] = { "=..", "==", "@<", "@=<", "@>", "@>=", "\\==", "acyclic_term", "arg", "atom", "atom_chars", "atom_codes", "atom_concat", "atom_length", "atom_number",
    "atom_string", "atomic", "atomic_concat", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term", "downcase_atom", "float", "format", "functor", "ground", "integer",
    "is_list", "msort", "name", "nonvar", "number", "number_chars", "number_codes", "number_string", "numbervars", "plus", "print", "sort", "string", "string_chars", "string_codes", "string_concat",
    "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "tab", "term_string", "term_to_atom", "term_variables", "upcase_atom", "var", "write_canonical", "writeln", "writeq",
    "put_char", "halt", "flush_output", "read", "read_term", "get_char", "peek_char", "nl", "write", NULL };
static const char * const pl_rung7_builtins[] = { "between", "repeat", "sub_atom", "for", "current_op", "current_predicate", "predicate_property", "current_prolog_flag", "current_stream",
    "stream_property", NULL };
static const char * const pl_rung8_builtins[] = { "findall", "bagof", "setof", "aggregate_all", NULL };
static const char * const pl_rung9_builtins[] = { NULL };
static const char * const pl_rung10_builtins[] = { "call", "assert", "asserta", "assertz", "retractall", "abolish", "clause", "retract", "dynamic", "b_setval", "b_getval", "phrase", "with_output_to",
    "setup_call_cleanup", "setup_call_catcher_cleanup", "use_module", "ensure_loaded", "module", "set_prolog_flag", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_name_in(const char * nm, const char * const * lst) { if (!nm) return 0; for (int i = 0; lst[i]; i++) if (!strcmp(nm, lst[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rung_of(const char * nm) {
    if (pl_name_in(nm, pl_rung10_builtins)) return 10;
    if (pl_name_in(nm, pl_rung9_builtins)) return 9;
    if (pl_name_in(nm, pl_rung8_builtins)) return 8;
    if (pl_name_in(nm, pl_rung7_builtins)) return 7;
    if (pl_name_in(nm, pl_rung6_builtins)) return 6;
    return 0;
}
static void pl_kill_co(const tree_t * t, int nv, int * seen, int * ns) {
    if (!t) return;
    if (t->t == TT_VAR && (int) t->v.ival >= 0 && (int) t->v.ival < nv) { int v = (int) t->v.ival, dup = 0; for (int j = 0; j < *ns; j++) if (seen[j] == v) dup = 1; if (!dup) seen[(*ns)++] = v; }
    for (int i = 0; i < t->n; i++) pl_kill_co(t->c[i], nv, seen, ns);
}
void pl_kill_co_goal(const tree_t * const * ts, int nt, int gi, int * co, int nv) {
    int nn = 1;
    for (int i = 0; i < nt; i++) nn += pl_var_nodes(ts[i]);
    int seen[nn], ns = 0;
    for (int i = 0; i < nt; i++) pl_kill_co(ts[i], nv, seen, &ns);
    if (ns > 1) for (int j = 0; j < ns; j++) if (co[seen[j]] > gi) co[seen[j]] = gi;
}
void pl_kill_scan(const tree_t * t, int gi, int * first, int * last, int nv) {
    if (!t) return;
    if (t->t == TT_VAR && (int) t->v.ival >= 0 && (int) t->v.ival < nv) { int v = (int) t->v.ival; if (first[v] == -2) first[v] = gi; last[v] = gi; }
    for (int i = 0; i < t->n; i++) pl_kill_scan(t->c[i], gi, first, last, nv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_table_stmt(const tree_t * s, tree_t * subj) {
    tree_t * st = ast_node_new(TT_STMT);
    const tree_t * old = stmt_attr_find(s, ":subj");
    for (int i = 0; i < s->n; i++) ast_push(st, s->c[i] == old ? ast_attr_expr(":subj", subj) : s->c[i]);
    st->line = s->line;
    return st;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_is(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval &&
        ((t->n == 3 && !strcmp(t->v.sval, "setup_call_cleanup")) || (t->n == 4 && !strcmp(t->v.sval, "setup_call_catcher_cleanup")) || (t->n == 2 && !strcmp(t->v.sval, "call_cleanup")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_count(const tree_t * t) { int n = 0; if (!t) return 0; if (pl_scc_is(t)) n++; for (int i = 0; i < t->n; i++) n += pl_scc_count(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_vars(const tree_t * t, int * v, int n, int cap) {
    if (!t) return n;
    if (t->t == TT_VAR) { int s = (int) t->v.ival; for (int i = 0; i < n; i++) if (v[i] == s) return n; if (n < cap) v[n] = s; return n + 1; }
    for (int i = 0; i < t->n; i++) n = pl_scc_vars(t->c[i], v, n, cap);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_scc_remap(const tree_t * t, const int * v, int n) {
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    if (t->t == TT_VAR) { for (int i = 0; i < n; i++) if (v[i] == (int) t->v.ival) c->v.ival = i; return c; }
    for (int i = 0; i < t->n; i++) ast_push(c, pl_scc_remap(t->c[i], v, n));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_var_cap(const tree_t * t) { int n = (t && t->t == TT_VAR) ? 1 : 0; if (!t) return 0; for (int i = 0; i < t->n; i++) n += pl_scc_var_cap(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_scc_aux(const tree_t * g, tree_t ** work, int * nw, int cap) {
    int vc = pl_scc_var_cap(g) + 1;
    int v[vc];
    int n = pl_scc_vars(g, v, 0, vc);
    char nm[32], key[48];
    snprintf(nm, sizeof nm, "$scc_g%d", g_stage2.pl_scc_aux_n++);
    snprintf(key, sizeof key, "%s/%d", nm, n);
    tree_t * call = ast_node_new(n > 0 ? TT_FNC : TT_QLIT);
    call->v.sval = (char *) lp_strdup(nm);
    for (int i = 0; i < n; i++) { tree_t * a = ast_node_new(TT_VAR); a->v.ival = v[i]; ast_push(call, a); }
    tree_t * cl = ast_node_new(TT_CLAUSE);
    cl->v.dval = n;
    for (int i = 0; i < n; i++) { tree_t * a = ast_node_new(TT_VAR); a->v.ival = i; ast_push(cl, a); }
    ast_push(cl, g->t == TT_VAR ? pl_cc_fnc1("call", pl_scc_remap(g, v, n)) : pl_scc_remap(g, v, n));
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) lp_strdup(key);
    ast_push(ch, cl);
    if (*nw >= cap) pl_refuse("setup_call_cleanup lift count in", key, 2);
    work[(*nw)++] = ch;
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_named(const tree_t * t, const char * nm, int n) { return t && t->t == TT_FNC && t->v.sval && t->n == n && !strcmp(t->v.sval, nm); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_scc_liftable(const tree_t * g) { return g && (g->t == TT_FNC || g->t == TT_QLIT || g->t == TT_VAR); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_scc_walk(tree_t * t, tree_t ** work, int * nw, int cap, int ccdef) {
    if (!t || t->t != TT_FNC || !t->v.sval) return;
    const char * nm = t->v.sval;
    int n = t->n;
    if (n == 2 && (!strcmp(nm, ",") || !strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "*->") || !strcmp(nm, "forall"))) {
        pl_scc_walk(t->c[0], work, nw, cap, ccdef);
        pl_scc_walk(t->c[1], work, nw, cap, ccdef);
        return;
    }
    if (n == 1 && (!strcmp(nm, "\\+") || !strcmp(nm, "not") || !strcmp(nm, "once") || !strcmp(nm, "ignore") || !strcmp(nm, "call"))) { pl_scc_walk(t->c[0], work, nw, cap, ccdef); return; }
    if (n == 3 && !strcmp(nm, "catch")) { pl_scc_walk(t->c[0], work, nw, cap, ccdef); pl_scc_walk(t->c[2], work, nw, cap, ccdef); return; }
    if (((n == 3 || n == 4) && !strcmp(nm, "findall")) || (n == 3 && !strcmp(nm, "aggregate_all"))) { pl_scc_walk(t->c[1], work, nw, cap, ccdef); return; }
    if (n == 3 && (!strcmp(nm, "bagof") || !strcmp(nm, "setof"))) { tree_t * g = t->c[1]; while (pl_scc_named(g, "^", 2)) g = g->c[1]; pl_scc_walk(g, work, nw, cap, ccdef); return; }
    if ((n == 3 && !strcmp(nm, "setup_call_cleanup")) || (n == 4 && !strcmp(nm, "setup_call_catcher_cleanup"))) {
        pl_scc_walk(t->c[0], work, nw, cap, ccdef);
        if (pl_scc_liftable(t->c[1])) t->c[1] = pl_scc_aux(t->c[1], work, nw, cap);
        pl_scc_walk(t->c[n - 1], work, nw, cap, ccdef);
        return;
    }
    if (n == 2 && !strcmp(nm, "call_cleanup") && !ccdef) { if (pl_scc_liftable(t->c[0])) t->c[0] = pl_scc_aux(t->c[0], work, nw, cap); pl_scc_walk(t->c[1], work, nw, cap, ccdef); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_scc_clauses(tree_t * ch, tree_t ** work, int * nw, int cap, int ccdef) {
    int nc = ch->t == TT_CHOICE ? ch->n : 1;
    for (int k = 0; k < nc; k++) {
        tree_t * cl = ch->t == TT_CHOICE ? ch->c[k] : ch;
        if (!cl || cl->t != TT_CLAUSE) continue;
        for (int i = (int) cl->v.dval; i < cl->n; i++) pl_scc_walk(cl->c[i], work, nw, cap, ccdef);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_is(const tree_t * g, const char * f) { return g && g->t == TT_FNC && g->n == 1 && g->v.sval && !strcmp(g->v.sval, f); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_mark(const tree_t * g) { return pl_cutto_is(g, "prolog_current_choice") && g->c[0] && g->c[0]->t == TT_VAR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_conj_n(const tree_t * g) { return (g && g->t == TT_FNC && g->n == 2 && g->v.sval && !strcmp(g->v.sval, ",")) ? pl_cutto_conj_n(g->c[0]) + pl_cutto_conj_n(g->c[1]) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_cutto_conj_fill(tree_t * g, tree_t ** out, int * k) {
    if (g && g->t == TT_FNC && g->n == 2 && g->v.sval && !strcmp(g->v.sval, ",")) { pl_cutto_conj_fill(g->c[0], out, k); pl_cutto_conj_fill(g->c[1], out, k); return; }
    out[(*k)++] = g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cutto_conj_of(tree_t ** g, int n) {
    if (n <= 0) return (tree_t *) pl_atom_goal("true");
    tree_t * r = g[n - 1];
    for (int i = n - 2; i >= 0; i--) r = pl_cc_fnc2(",", g[i], r);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_body_count(const tree_t * cl, int body0, int slot) { int n = 0; for (int i = body0; i < cl->n; i++) n += pl_var_count(cl->c[i], slot); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_list(tree_t ** g, int n, const tree_t * cl, int body0) {
    for (int i = 0; i < n; i++) {
        int j = i + 1;
        if (!pl_cutto_mark(g[i]) || pl_cutto_body_count(cl, body0, (int) g[i]->c[0]->v.ival) != 2) continue;
        while (j < n && !(pl_cutto_is(g[j], "prolog_cut_to") && g[j]->c[0] && g[j]->c[0]->t == TT_VAR && g[j]->c[0]->v.ival == g[i]->c[0]->v.ival)) j++;
        if (j >= n) continue;
        tree_t * ite = ast_node_new(TT_IF);
        ite->line = g[i]->line;
        ast_push(ite, pl_cutto_conj_of(g + i + 1, j - i - 1));
        ast_push(ite, (tree_t *) pl_atom_goal("true"));
        ast_push(ite, (tree_t *) pl_atom_goal("fail"));
        g[i] = ite;
        for (int k = j + 1; k < n; k++) g[i + k - j] = g[k];
        n -= j - i;
    }
    for (int i = 0; i < n; i++) g[i] = pl_cutto_goal(g[i], cl, body0);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutto_ctl(const tree_t * g, int i) {
    static const char * const all1[] = { "\\+", "once", "ignore", "call", NULL }, * const all2[] = { ";", "->", "*->", "forall", NULL };
    const char * f = g->v.sval;
    if (g->t == TT_IF) return 1;
    if (g->t != TT_FNC || !f) return 0;
    for (int k = 0; g->n == 1 && all1[k]; k++) if (!strcmp(f, all1[k])) return 1;
    for (int k = 0; g->n == 2 && all2[k]; k++) if (!strcmp(f, all2[k])) return 1;
    if (((g->n == 3 || g->n == 4) && !strcmp(f, "findall")) || (g->n == 3 && !strcmp(f, "aggregate_all"))) return i == 1;
    return g->n == 3 && !strcmp(f, "catch") && i != 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cutto_goal(tree_t * g, const tree_t * cl, int body0) {
    if (!g) return g;
    if (pl_cutto_is(g, "prolog_cut_to")) return pl_cc_throw_ar(pl_cc_fnc1("system_error", pl_cc_fnc1("unsupported", pl_cc_pi_ar("prolog_cut_to", 1))), "prolog_cut_to", 1);
    if (g->t == TT_FNC && g->n == 2 && g->v.sval && !strcmp(g->v.sval, ",")) {
        int n = pl_cutto_conj_n(g), k = 0;
        tree_t * gs[n];
        pl_cutto_conj_fill(g, gs, &k);
        n = pl_cutto_list(gs, n, cl, body0);
        return pl_cutto_conj_of(gs, n);
    }
    for (int i = 0; i < g->n; i++) if (pl_cutto_ctl(g, i)) g->c[i] = pl_cutto_goal(g->c[i], cl, body0);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cutto_clause(const tree_t * c, int ar, int head_ok) {
    tree_t * cl = pl_tree_copy(c);
    int n = cl->n - ar;
    if (n <= 0) return cl;
    tree_t * gs[n];
    for (int i = 0; i < n; i++) gs[i] = cl->c[ar + i];
    n = pl_cutto_list(gs, n, cl, head_ok ? ar : 0);
    cl->n = ar;
    for (int i = 0; i < n; i++) ast_push(cl, gs[i]);
    return cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_cutto_subj(const tree_t * subj, int ar, int head_ok) {
    if (subj->t == TT_CLAUSE) return pl_cutto_clause(subj, ar, head_ok);
    tree_t * rn = ast_node_new(TT_CHOICE);
    rn->v = subj->v;
    rn->line = subj->line;
    for (int k = 0; k < subj->n; k++) ast_push(rn, pl_cutto_clause(subj->c[k], ar, head_ok));
    return rn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_cutto_rewrite(const tree_t * prog) {
    int any = 0;
    for (int i = 0; i < prog->n && !any; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && pl_tree_calls(subj, "prolog_cut_to", 1)) any = 1;
    }
    if (!any) return prog;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * sl = subj && subj->v.sval ? strrchr(subj->v.sval, '/') : NULL;
        if (!subj || (subj->t != TT_CHOICE && subj->t != TT_CLAUSE) || !sl || !pl_tree_calls(subj, "prolog_cut_to", 1)) { ast_push(np, (tree_t *) s); continue; }
        ast_push(np, pl_table_stmt(s, pl_cutto_subj(subj, atoi(sl + 1), 0)));
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_scc_rewrite(const tree_t * prog) {
    int total = 0, ccdef = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!subj || (subj->t != TT_CHOICE && subj->t != TT_CLAUSE)) continue;
        if (subj->v.sval && !strcmp(subj->v.sval, "call_cleanup/2")) ccdef = 1;
        total += pl_scc_count(subj);
    }
    if (!total) return prog;
    tree_t * work[total];
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!subj || (subj->t != TT_CHOICE && subj->t != TT_CLAUSE) || !pl_scc_count(subj)) { ast_push(np, (tree_t *) s); continue; }
        tree_t * rn = pl_tree_copy(subj);
        int nw = 0, done = 0;
        pl_scc_clauses(rn, work, &nw, total, ccdef);
        ast_push(np, pl_table_stmt(s, rn));
        while (done < nw) { tree_t * aux = work[done++]; pl_scc_clauses(aux, work, &nw, total, ccdef); ast_push(np, pl_table_stmt(s, aux)); }
    }
    return np;
}
static int pl_mq_colon(const tree_t * t) { return t && (t->t == TT_QLIT || t->t == TT_NAME || (t->t == TT_FNC && t->n == 0)) && t->v.sval && !strcmp(t->v.sval, ":"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_mq_specs(const tree_t * a, const char ** keys, const char ** masks, int n, int cap) {
    if (!a) return n;
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, ",") && a->n == 2) return pl_mq_specs(a->c[1], keys, masks, pl_mq_specs(a->c[0], keys, masks, n, cap), cap);
    if (a->t == TT_MAKELIST) { for (int i = 0; i < a->n; i++) n = pl_mq_specs(a->c[i], keys, masks, n, cap); return n; }
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, ":") && a->n == 2) return pl_mq_specs(a->c[1], keys, masks, n, cap);
    if (a->t != TT_FNC || !a->v.sval || a->n <= 0) return n;
    { int any = 0; for (int i = 0; i < a->n; i++) if (pl_mq_colon(a->c[i])) any = 1; if (!any) return n; }
    if (keys && n < cap) {
        char b[strlen(a->v.sval) + 24], m[a->n + 1];
        snprintf(b, sizeof b, "%s/%d", a->v.sval, a->n);
        for (int i = 0; i < a->n; i++) m[i] = pl_mq_colon(a->c[i]) ? '1' : '0';
        m[a->n] = 0;
        keys[n] = lp_strdup(b);
        masks[n] = lp_strdup(m);
    }
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_mq_dir_specs(const tree_t * s, const char ** keys, const char ** masks, int n, int cap) {
    const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
    if (!subj || subj->t != TT_FNC || !subj->v.sval || (strcmp(subj->v.sval, "meta_predicate") && strcmp(subj->v.sval, "$meta_spec"))) return n;
    for (int k = 0; k < subj->n; k++) n = pl_mq_specs(subj->c[k], keys, masks, n, cap);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_mq_var(int slot) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = slot; return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_mq_wrapper(const char * key, const char * mask, const char * mod) {
    const char * sl = strrchr(key, '/');
    int ar = atoi(sl + 1), q = ar;
    char tn[sl - key + 6];
    snprintf(tn, sizeof tn, "$mq %.*s", (int)(sl - key), key);
    tree_t * cl = ast_node_new(TT_CLAUSE);
    cl->v.dval = ar;
    for (int i = 0; i < ar; i++) ast_push(cl, pl_mq_var(i));
    tree_t * call = ast_node_new(ar > 0 ? TT_FNC : TT_QLIT);
    call->v.sval = (char *) lp_strdup(tn);
    for (int i = 0; i < ar; i++) {
        if (mask[i] != '1') { ast_push(call, pl_mq_var(i)); continue; }
        tree_t * g = ast_node_new(TT_FNC);
        g->v.sval = (char *) "$mq_arg";
        ast_push(g, (tree_t *) pl_atom_goal(mod));
        ast_push(g, pl_mq_var(i));
        ast_push(g, pl_mq_var(q));
        ast_push(cl, g);
        ast_push(call, pl_mq_var(q++));
    }
    ast_push(cl, call);
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) lp_strdup(key);
    ast_push(ch, cl);
    return ch;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_mq_rewrite(const tree_t * prog) {
    int nk = 0;
    for (int i = 0; i < prog->n; i++) nk = pl_mq_dir_specs(prog->c[i], NULL, NULL, nk, 0);
    if (!nk) return prog;
    const char * keys[nk];
    const char * masks[nk];
    const char * emitted[prog->n + 1];
    int nf = 0, ne = 0;
    for (int i = 0; i < prog->n; i++) nf = pl_mq_dir_specs(prog->c[i], keys, masks, nf, nk);
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        int k = -1;
        const char * mod = "user";
        char mb[subj && subj->v.sval ? strlen(subj->v.sval) + 1 : 1];
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && subj->v.sval) {
            const char * colon = strchr(subj->v.sval, ':');
            for (int j = 0; j < nf; j++) {
                if (!strcmp(keys[j], subj->v.sval)) k = j;
                else if (colon && colon > subj->v.sval && !strcmp(colon + 1, keys[j])) { k = j; snprintf(mb, sizeof mb, "%.*s", (int)(colon - subj->v.sval), subj->v.sval); mod = mb; }
            }
        }
        if (k < 0) { ast_push(np, (tree_t *) s); continue; }
        tree_t * rn = pl_tree_copy(subj);
        char b[strlen(subj->v.sval) + 6];
        snprintf(b, sizeof b, "$mq %s", subj->v.sval);
        rn->v.sval = (char *) lp_strdup(b);
        ast_push(np, pl_table_stmt(s, rn));
        int dup = 0;
        for (int e = 0; e < ne; e++) if (!strcmp(emitted[e], subj->v.sval)) dup = 1;
        if (!dup) { emitted[ne++] = lp_strdup(subj->v.sval); ast_push(np, pl_table_stmt(s, pl_mq_wrapper(subj->v.sval, masks[k], mod))); }
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_stmt_file(const tree_t * s) { const char * v = stmt_attr_str(stmt_attr_find(s, ":file")); return v ? v : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_tree_calls(const tree_t * t, const char * f, int n) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && t->n == n && !strcmp(t->v.sval, f)) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_calls(t->c[i], f, n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_meta_real_name(const tree_t * prog, const char * name, int ar) {
    size_t nl = strlen(name);
    char want[nl + 24];
    snprintf(want, sizeof want, "%s/%d", name, ar);
    size_t wl = strlen(want);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * key = (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE)) ? subj->v.sval : NULL;
        size_t kl = key ? strlen(key) : 0;
        if (!key) continue;
        if (!strcmp(key, want)) return name;
        if (kl > wl && key[kl - wl - 1] == ':' && !strcmp(key + kl - wl, want)) {
            char * rn = (char *) ct_alloc(kl - (wl - nl) + 1);
            memcpy(rn, key, kl - (wl - nl));
            rn[kl - (wl - nl)] = 0;
            return rn;
        }
    }
    return name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_meta_rt_specs(const tree_t * prog, const tree_t * s, const tree_t * spec, tree_t * np) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_meta_rt_specs(prog, s, spec->c[0], np); pl_meta_rt_specs(prog, s, spec->c[1], np); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_meta_rt_specs(prog, s, spec->c[i], np); return; }
    if (spec->t != TT_FNC || !spec->v.sval || spec->n <= 0) return;
    {
        tree_t * cp = pl_tree_copy(spec);
        tree_t * g = ast_node_new(TT_FNC);
        cp->v.sval = (char *) pl_meta_real_name(prog, spec->v.sval, spec->n);
        g->v.sval = (char *) "$meta_add";
        ast_push(g, cp);
        ast_push(np, pl_table_stmt(s, g));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_meta_rt_rewrite(const tree_t * prog) {
    int any = 0;
    for (int i = 0; i < prog->n && !any; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && (pl_tree_calls(subj, "@", 2) || (subj->v.sval && !strcmp(subj->v.sval, "goal_expansion/2")))) any = 1;
    }
    if (!any) return prog;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        ast_push(np, (tree_t *) s);
        if (subj && subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "meta_predicate")) for (int k = 0; k < subj->n; k++) pl_meta_rt_specs(prog, s, subj->c[k], np);
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_prog_defines(const tree_t * prog, const char * key) {
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && subj->v.sval && !strcmp(subj->v.sval, key)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_src_file_rewrite(const tree_t * prog) {
    int any = 0, nk = 0;
    for (int i = 0; i < prog->n && !any; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && pl_tree_calls(subj, "source_file", 2)) any = 1;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && (pl_tree_calls(subj, "load_files", 1) || pl_tree_calls(subj, "load_files", 2))) any = 1;
        if (subj && subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "load_files")) any = 1;
    }
    if (!pl_prog_defines(prog, "$src_file_add/3")) return prog;
    if (!any) return prog;
    const char * seen[prog->n + 1];
    const char * lastfile = "";
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * key = (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE)) ? subj->v.sval : NULL;
        const char * file = pl_stmt_file(s);
        const char * sl = key ? strrchr(key, '/') : NULL;
        int dup = 0;
        if (file[0] && strcmp(file, lastfile)) {
            const size_t dpn = PATH_MAX;
            char dpb[dpn];
            const char * dp = realpath(file, dpb);
            tree_t * dg = ast_node_new(TT_FNC);
            dg->v.sval = (char *) "$src_file_add";
            ast_push(dg, (tree_t *) pl_atom_goal("$srcdir"));
            ast_push(dg, pl_cc_ilit(0));
            ast_push(dg, (tree_t *) pl_atom_goal(dp ? lp_strdup(dp) : file));
            ast_push(np, pl_table_stmt(s, dg));
            lastfile = file;
        }
        ast_push(np, (tree_t *) s);
        if (!sl || sl == key || key[0] == '$' || !file[0]) continue;
        for (int k = 0; k < nk; k++) if (!strcmp(seen[k], key)) dup = 1;
        if (dup) continue;
        seen[nk++] = key;
        {
            const size_t rpn = PATH_MAX;
            char rpb[rpn];
            const char * rp = realpath(file, rpb);
            char * nm = (char *) ct_alloc((size_t) (sl - key) + 1);
            tree_t * g = ast_node_new(TT_FNC);
            memcpy(nm, key, (size_t) (sl - key));
            nm[sl - key] = 0;
            g->v.sval = (char *) "$src_file_add";
            ast_push(g, (tree_t *) pl_atom_goal(nm));
            ast_push(g, pl_cc_ilit(atoi(sl + 1)));
            ast_push(g, (tree_t *) pl_atom_goal(rp ? lp_strdup(rp) : file));
            ast_push(np, pl_table_stmt(s, g));
        }
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_load_hook_key(const char * k) { return k && (!strcmp(k, "term_expansion/2") || !strcmp(k, "goal_expansion/2")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_load_nclauses(const tree_t * subj) { return subj->t == TT_CHOICE ? subj->n : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_load_clause(const tree_t * subj, int i) { return subj->t == TT_CHOICE ? subj->c[i] : subj; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_load_hook_line(const tree_t * prog, const char * file) {
    int h = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!subj || (subj->t != TT_CHOICE && subj->t != TT_CLAUSE) || !pl_load_hook_key(subj->v.sval) || (file && strcmp(pl_stmt_file(s), file))) continue;
        for (int j = 0; j < pl_load_nclauses(subj); j++) { const tree_t * c = pl_load_clause(subj, j); if (c && c->line > 0 && (!h || c->line < h)) h = c->line; }
    }
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_load_dir_is_exec(const tree_t * subj) {
    const char * f = (subj->t == TT_FNC) ? subj->v.sval : NULL;
    if (!f) return 1;
    if (!strcmp(f, "initialization") && subj->n >= 1) { const tree_t * gt = subj->c[0]; if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval) return 0; }
    if (!strcmp(f, "dynamic") || !strcmp(f, "multifile") || !strcmp(f, "discontiguous") || !strcmp(f, "meta_predicate") || pl_name_in(f, pl_decl_directives)) return 0;
    if ((!strcmp(f, "op") && subj->n == 3) || (!strcmp(f, "set_prolog_flag") && subj->n == 2)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_load_unit_of(const tree_t * subj, int * is_unit) {
    *is_unit = 0;
    if (!subj || subj->t != TT_FNC || !subj->v.sval || subj->n < 1) return NULL;
    if (!strcmp(subj->v.sval, "end_tests") && subj->n == 1) { *is_unit = 1; return NULL; }
    if (strcmp(subj->v.sval, "begin_tests") || subj->n > 2) return NULL;
    *is_unit = 1;
    return (subj->c[0] && (subj->c[0]->t == TT_QLIT || subj->c[0]->t == TT_NAME || subj->c[0]->t == TT_FNC)) ? subj->c[0]->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_load_item_cmp(const void * a, const void * b) {
    const pl_load_item_t * x = (const pl_load_item_t *) a;
    const pl_load_item_t * y = (const pl_load_item_t *) b;
    if (x->fo != y->fo) return x->fo < y->fo ? -1 : 1;
    if (x->line != y->line) return x->line < y->line ? -1 : 1;
    return x->ord < y->ord ? -1 : (x->ord > y->ord);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_load_capacity(const tree_t * prog) {
    int n = 1;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        n += subj ? ((subj->t == TT_CHOICE || subj->t == TT_CLAUSE) ? pl_load_nclauses(subj) : 1) : 0;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_load_defer(const tree_t * prog, pl_load_item_t * it, int * nit) {
    const char * files[prog->n + 1];
    int fh[prog->n + 1], ffo[prog->n + 1], nf = 0;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * file = subj ? pl_stmt_file(s) : "";
        int k = 0, h;
        while (k < nf && strcmp(files[k], file)) k++;
        if (k == nf) { files[nf] = file; fh[nf] = pl_load_hook_line(prog, file); ffo[nf] = i; nf++; }
        h = fh[k];
        if (!subj || h <= 0) { ast_push(np, (tree_t *) s); continue; }
        if (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) {
            const char * key = subj->v.sval;
            const char * sl = key ? strrchr(key, '/') : NULL;
            int late = 0, pj = key && !strcmp(key, "pj_test/4");
            for (int j = 0; j < pl_load_nclauses(subj); j++) if (pl_load_clause(subj, j)->line > h) late = 1;
            if (!late || !sl || pl_load_hook_key(key)) { ast_push(np, (tree_t *) s); continue; }
            char * pn = (char *) ct_alloc((size_t) (sl - key) + 1);
            memcpy(pn, key, (size_t) (sl - key));
            pn[sl - key] = 0;
            for (int j = 0; j < pl_load_nclauses(subj); j++) {
                const tree_t * c = pl_load_clause(subj, j);
                if (pj && c->line > h) continue;
                it[*nit] = (pl_load_item_t) { c, pn, file, atoi(sl + 1), c->line, ffo[k], *nit, c->line > h };
                (*nit)++;
            }
            continue;
        }
        int ln = lp_s_int(s, ":line");
        if (ln > h && pl_load_dir_is_exec(subj)) { it[*nit] = (pl_load_item_t) { subj, NULL, file, -1, ln, ffo[k], *nit, 1 }; (*nit)++; continue; }
        ast_push(np, (tree_t *) s);
    }
    qsort(it, (size_t) *nit, sizeof *it, pl_load_item_cmp);
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_load_directive_term(const tree_t * subj) {
    tree_t * tg = pl_cc_fnc1(":-", pl_body_term(subj));
    int occ = pl_var_nodes(tg) + 1;
    const char * names[occ];
    long long slots[occ];
    int nn = 0, n = 0;
    pl_dir_number_vars(tg, names, &nn, 0);
    pl_tree_renumber_vars(tg, slots, &n, occ, g_lower.pl.seed_var_base);
    g_lower.pl.seed_var_base += (n > 0 ? n : 1);
    return tg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_load_unit_at(const tree_t * prog, const char * file, int line) {
    const char * u = NULL;
    int best = -1;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        int is_unit = 0, ln;
        const char * nu;
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE || strcmp(pl_stmt_file(s), file)) continue;
        nu = pl_load_unit_of(subj, &is_unit);
        ln = lp_s_int(s, ":line");
        if (is_unit && ln < line && ln > best) { best = ln; u = nu; }
    }
    return u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_load_module_of(const tree_t * prog, const char * file) {
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!subj || subj->t != TT_FNC || !subj->v.sval || strcmp(subj->v.sval, "module") || subj->n != 2 || strcmp(pl_stmt_file(s), file)) continue;
        if (subj->c[0] && (subj->c[0]->t == TT_QLIT || subj->c[0]->t == TT_NAME) && subj->c[0]->v.sval) return subj->c[0]->v.sval;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_load_procs(const tree_t * prog, const pl_load_item_t * it, int nit, cv_t * procs) {
    for (int lo = 0; lo < nit && procs->len < PL_SEED_PROCS_MAX; lo += PL_SEED_CHUNK) {
        int hi = lo + PL_SEED_CHUNK < nit ? lo + PL_SEED_CHUNK : nit, m = 0, save_base = g_lower.pl.seed_var_base;
        const tree_t ** sg = (const tree_t **) ct_zalloc((size_t) (hi - lo), sizeof(const tree_t *));
        g_lower.pl.seed_var_base = 0;
        for (int i = lo; i < hi; i++) {
            const char * u = it[i].expand ? pl_load_unit_at(prog, it[i].file, it[i].line) : NULL;
            tree_t * t = it[i].pn ? pl_static_clause_term(it[i].t, it[i].pn, it[i].ar, 0) : pl_load_directive_term(it[i].t);
            if (it[i].pn && it[i].t->t == TT_CLAUSE && it[i].t->n <= it[i].ar && t->t == TT_FNC && t->n == 2) t = t->c[0];
            tree_t * g = pl_cc_fnc3("$load_item", t, pl_cc_ilit(it[i].line), u ? (tree_t *) pl_atom_goal(u) : pl_cc_ilit(0));
            ast_push(g, pl_cc_ilit(it[i].expand));
            { const char * mo = (it[i].expand && !u) ? pl_load_module_of(prog, it[i].file) : NULL; ast_push(g, mo ? (tree_t *) pl_atom_goal(mo) : pl_cc_ilit(0)); }
            ast_push(g, pl_seed_file_term(it[i].file));
            sg[m++] = g;
        }
        g_lower.pl.seed_var_base = save_base;
        {
            IR_graph_t * g = pl_body_graph(sg, m);
            int bb_idx = bb_program_add(&g_stage2.bbp, g);
            if (bb_idx >= 0) {
                char * pn = pl_pi_text("$load_deferred", (int) procs->len, 0);
                char * rk = pl_pi_text(pn, 0, 1);
                pl_bb_register(rk, 0, bb_idx);
                pl_new_proc(rk, 0, bb_idx);
                CV_PUSH(*procs, const char *) = pn;
            }
        }
        ct_drop(sg);
    }
}
