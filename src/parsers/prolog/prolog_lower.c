#include "prolog_lower.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "rt/prolog_atom.h"
#include "scrip_cc.h"
#include "stage2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int functor;
    int arity;
} PredKey;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pred_key_eq(PredKey a, PredKey b) {
    return a.functor == b.functor && a.arity == b.arity;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_clause_assign_dense_slots(tree_t *ec, int arity);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pred_str(int functor, int arity) {
    const char *fn = prolog_atom_name(functor);
    if (!fn) fn = "?";
    char buf[fmt_len("%s/%d", fn, arity)];
    snprintf(buf, sizeof buf, "%s/%d", fn, arity);
    return ct_strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pred_add(cv_t *keys, cv_t *choices, PredKey k) {
    tree_t *ch = ast_node_new(TT_CHOICE);
    ch->v.sval = pred_str(k.functor, k.arity);
    CV_PUSH(*keys, PredKey) = k;
    CV_PUSH(*choices, tree_t *) = ch;
    return (int) keys->len - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pred_slot(cv_t *keys, cv_t *choices, PredKey k) {
    for (uint32_t i = 0; i < keys->len; i++) if (pred_key_eq(CV_AT(*keys, PredKey, i), k)) return (int) i;
    return pl_pred_add(keys, choices, k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_flatten_conj(tree_t *t, tree_t *prog) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && strcmp(t->v.sval, ",") == 0) {
        for (int i = 0; i < t->n; i++)
            pl_flatten_conj(t->c[i], prog);
        return;
    }
    ast_push(prog, t);
}
static tree_t *pl_rewrite_control(tree_t *t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_arrow_then_prog(tree_t *arrow) {
    tree_t *then_prog = ast_node_new(TT_PROGRAM);
    for (int i = 1; i < arrow->n; i++) pl_flatten_conj(arrow->c[i], then_prog);
    for (int i = 0; i < then_prog->n; i++) then_prog->c[i] = pl_rewrite_control(then_prog->c[i]);
    return then_prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_disj_of_rest(tree_t *semi_node, int from) {
    if (from >= semi_node->n) { tree_t *f = ast_node_new(TT_QLIT); f->v.sval = ct_strdup("fail"); return f; }
    if (from == semi_node->n - 1) return semi_node->c[from];
    tree_t *rest = ast_node_new(TT_FNC); rest->v.sval = ct_strdup(";");
    for (int i = from; i < semi_node->n; i++) ast_push(rest, semi_node->c[i]);
    return rest;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_arrow(const tree_t *t) {
    return t && t->t == TT_FNC && t->v.sval && strcmp(t->v.sval, "->") == 0 && t->n >= 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_rewrite_control(tree_t *t) {
    if (!t) return t;
    if (pl_is_arrow(t)) {
        tree_t *then_prog = pl_arrow_then_prog(t);
        tree_t *else_prog = ast_node_new(TT_PROGRAM);
        { tree_t *f = ast_node_new(TT_QLIT); f->v.sval = ct_strdup("fail"); ast_push(else_prog, f); }
        tree_t *iff = ast_node_new(TT_IF);
        iff->v.ival = 1;
        ast_push(iff, pl_rewrite_control(t->c[0]));
        ast_push(iff, then_prog->n == 1 ? then_prog->c[0] : then_prog);
        ast_push(iff, else_prog->n == 1 ? else_prog->c[0] : else_prog);
        return iff;
    }
    if (t->t == TT_FNC && t->v.sval && strcmp(t->v.sval, ";") == 0 && t->n >= 2 && pl_is_arrow(t->c[0])) {
        tree_t *arrow = t->c[0];
        tree_t *then_prog = pl_arrow_then_prog(arrow);
        tree_t *else_src  = pl_disj_of_rest(t, 1);
        tree_t *else_prog = ast_node_new(TT_PROGRAM);
        pl_flatten_conj(else_src, else_prog);
        for (int i = 0; i < else_prog->n; i++) else_prog->c[i] = pl_rewrite_control(else_prog->c[i]);
        tree_t *iff = ast_node_new(TT_IF);
        ast_push(iff, pl_rewrite_control(arrow->c[0]));
        ast_push(iff, then_prog->n == 1 ? then_prog->c[0] : then_prog);
        ast_push(iff, else_prog->n == 1 ? else_prog->c[0] : else_prog);
        return iff;
    }
    if (t->t == TT_FNC && t->v.sval && (strcmp(t->v.sval, ";") == 0 || strcmp(t->v.sval, ",") == 0)) {
        for (int i = 0; i < t->n; i++) t->c[i] = pl_rewrite_control(t->c[i]);
        return t;
    }
    return t;
}
typedef struct { const char *name; int slot; } TRSlot;
typedef struct { cv_t e; int next; } TRSlotMap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trslot_reset(TRSlotMap *m) { memset(m, 0, sizeof *m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trslot_put(TRSlotMap *m, const char *name, int slot) { TRSlot t; t.name = name; t.slot = slot; CV_PUSH(m->e, TRSlot) = t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trslot_get(TRSlotMap *m, const char *name) {
    if (!name || strcmp(name, "_") == 0)
        return m->next++;
    for (uint32_t i = 0; i < m->e.len; i++)
        if (strcmp(CV_AT(m->e, TRSlot, i).name, name) == 0)
            return CV_AT(m->e, TRSlot, i).slot;
    int s = m->next++;
    trslot_put(m, name, s);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void tr_assign_slots(tree_t *t, TRSlotMap *m) {
    if (!t) return;
    if (t->t == TT_VAR) {
        t->v.ival = trslot_get(m, t->v.sval);
        return;
    }
    for (int i = 0; i < t->n; i++)
        tr_assign_slots(t->c[i], m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_clause_assign_dense_slots(tree_t *ec, int arity) {
    if (!ec) return;
    TRSlotMap sm; trslot_reset(&sm);
    for (int i = 0; i < arity && i < ec->n; i++) {
        tree_t *a = ec->c[i];
        if (a && a->t == TT_VAR && a->v.sval && strcmp(a->v.sval, "_") != 0) {
            int dup = 0;
            for (uint32_t j = 0; j < sm.e.len; j++) if (CV_AT(sm.e, TRSlot, j).name && strcmp(CV_AT(sm.e, TRSlot, j).name, a->v.sval) == 0) { dup = 1; break; }
            if (!dup) trslot_put(&sm, a->v.sval, i);
        }
    }
    sm.next = arity;
    for (int i = 0; i < ec->n; i++) tr_assign_slots(ec->c[i], &sm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void tr_head_key(tree_t *head, const char **fn_out, int *arity_out) {
    *fn_out    = NULL;
    *arity_out = 0;
    if (!head) return;
    if (head->t == TT_FNC) {
        *fn_out    = head->v.sval;
        *arity_out = head->n;
    } else if (head->t == TT_QLIT) {
        *fn_out    = head->v.sval;
        *arity_out = 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pb_var(const char *nm) { tree_t *v = ast_node_new(TT_VAR); v->v.sval = ct_strdup(nm); return v; }
static tree_t *pb_fnc2(const char *f, tree_t *a, tree_t *b) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = ct_strdup(f); ast_push(n, a); ast_push(n, b); return n; }
static tree_t *pb_fnc1(const char *f, tree_t *a) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = ct_strdup(f); ast_push(n, a); return n; }
static tree_t *pb_copy(const tree_t *t) { if (!t) return NULL; tree_t *n = ast_node_new(t->t); n->v = t->v; n->line = t->line; n->slen = t->slen; for (int i = 0; i < t->n; i++) ast_push(n, pb_copy(t->c[i])); return n; }
static tree_t *pb_fnc3(const char *f, tree_t *a, tree_t *b, tree_t *c) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = ct_strdup(f); ast_push(n, a); ast_push(n, b); ast_push(n, c); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pb_collect_names(const tree_t *t, const char **names, int *n, int cap) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && strcmp(t->v.sval, "_") != 0) {
        for (int i = 0; i < *n; i++) if (!strcmp(names[i], t->v.sval)) return;
        if (*n < cap) names[(*n)++] = t->v.sval;
        return;
    }
    for (int i = 0; i < t->n; i++) pb_collect_names(t->c[i], names, n, cap);
}
static int pb_count_vars(const tree_t *t) { if (!t) return 0; int n = (t->t == TT_VAR) ? 1 : 0; for (int i = 0; i < t->n; i++) n += pb_count_vars(t->c[i]); return n; }
static int g_pb_fresh_ctr = 0;
static void pb_expand_goal(tree_t *t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pb_expand_bagof(tree_t *t) {
    int is_setof = !strcmp(t->v.sval, "setof");
    tree_t *T = t->c[0]; tree_t *G = t->c[1]; tree_t *L = t->c[2];
    int nvg = pb_count_vars(G) + 1, nvt = pb_count_vars(T) + 1;
    const char *quant[nvg]; int nq = 0;
    tree_t *G1 = G;
    while (G1 && G1->t == TT_FNC && G1->v.sval && !strcmp(G1->v.sval, "^") && G1->n == 2) { pb_collect_names(G1->c[0], quant, &nq, nvg); G1 = G1->c[1]; }
    const char *gv[nvg]; int ngv = 0; pb_collect_names(G1, gv, &ngv, nvg);
    const char *tv[nvt]; int ntv = 0; pb_collect_names(T, tv, &ntv, nvt);
    const char *fv[nvg]; int nfv = 0;
    for (int i = 0; i < ngv; i++) { int drop = 0;
        for (int j = 0; j < ntv && !drop; j++) if (!strcmp(gv[i], tv[j])) drop = 1;
        for (int j = 0; j < nq  && !drop; j++) if (!strcmp(gv[i], quant[j])) drop = 1;
        if (!drop) fv[nfv++] = gv[i]; }
    char b1[32], b2[32];
    tree_t *fa; tree_t *inner;
    if (nfv == 0) {
        snprintf(b1, sizeof b1, "_$B%d", g_pb_fresh_ctr++);
        fa = pb_fnc3("findall", T, G1, pb_var(b1));
        tree_t *ne = pb_fnc2("\\==", pb_var(b1), ast_node_new(TT_MAKELIST));
        tree_t *fin = is_setof ? pb_fnc2("sort", pb_var(b1), L) : pb_fnc2("=", pb_var(b1), L);
        inner = pb_fnc2(",", ne, fin);
    } else {
        return;
    }
    t->v.sval = ct_strdup(","); t->n = 0;
    ast_push(t, pb_fnc1("$pl_list_guard", pb_copy(L))); ast_push(t, pb_fnc2(",", fa, inner));
    pb_expand_goal(fa->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pb_expand_goal(tree_t *t) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval) {
        const char *f = t->v.sval;
        if ((!strcmp(f, "bagof") || !strcmp(f, "setof")) && t->n == 3) { pb_expand_bagof(t); return; }
        if ((!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "->")) && t->n == 2) { pb_expand_goal(t->c[0]); pb_expand_goal(t->c[1]); return; }
        if ((!strcmp(f, "\\+") || !strcmp(f, "not") || !strcmp(f, "once") || !strcmp(f, "ignore") || !strcmp(f, "call")) && t->n >= 1) { pb_expand_goal(t->c[0]); return; }
        if ((!strcmp(f, "findall") || !strcmp(f, "aggregate_all")) && t->n == 3) { pb_expand_goal(t->c[1]); return; }
        if (!strcmp(f, "forall") && t->n == 2) { pb_expand_goal(t->c[0]); pb_expand_goal(t->c[1]); return; }
        if (!strcmp(f, "catch") && t->n == 3) { pb_expand_goal(t->c[0]); pb_expand_goal(t->c[2]); return; }
        if (!strcmp(f, "setup_call_cleanup") && t->n == 3) { pb_expand_goal(t->c[0]); pb_expand_goal(t->c[1]); pb_expand_goal(t->c[2]); return; }
        if (!strcmp(f, "^") && t->n == 2) { pb_expand_goal(t->c[1]); return; }
        return;
    }
    if (t->t == TT_PROGRAM || t->t == TT_IF) for (int i = 0; i < t->n; i++) pb_expand_goal(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lower_clause_from_tree(tree_t *tr, PredKey key, int skip_rewrite, TRSlotMap *smp) {
    TRSlotMap sm = *smp; sm.e.len = 0; sm.next = 0;
    if (!skip_rewrite && tr->n > 1) pb_expand_goal(tr->c[1]);
    {
        tree_t *hd = (tr->n > 0) ? tr->c[0] : NULL;
        if (hd && hd->t == TT_FNC) {
            for (int i = 0; i < hd->n; i++) {
                tree_t *a = hd->c[i];
                if (a && a->t == TT_VAR && a->v.sval && strcmp(a->v.sval, "_") != 0) {
                    int dup = 0;
                    for (uint32_t j = 0; j < sm.e.len; j++) if (strcmp(CV_AT(sm.e, TRSlot, j).name, a->v.sval) == 0) { dup = 1; break; }
                    if (!dup) trslot_put(&sm, a->v.sval, i);
                }
            }
        }
    }
    sm.next = key.arity;
    tr_assign_slots(tr, &sm);
    int n_vars = sm.next;
    *smp = sm;
    (void) n_vars;
    tree_t *ec = ast_node_new(TT_CLAUSE);
    ec->v.sval = pred_str(key.functor, key.arity);
    ec->v.dval = (double)key.arity;
    tree_t *head = (tr->n > 0) ? tr->c[0] : NULL;
    tree_t *raw_body = (tr->n > 1) ? tr->c[1] : NULL;
    if (head && head->t == TT_FNC) {
        for (int i = 0; i < head->n; i++)
            expr_add_child(ec, head->c[i]);
    }
    tree_t *body_prog;
    if (raw_body && raw_body->t == TT_PROGRAM) {
        body_prog = raw_body;
    } else {
        body_prog = ast_node_new(TT_PROGRAM);
        if (raw_body) pl_flatten_conj(raw_body, body_prog);
    }
    if (!skip_rewrite)
        for (int i = 0; i < body_prog->n; i++)
            body_prog->c[i] = pl_rewrite_control(body_prog->c[i]);
    for (int i = 0; i < body_prog->n; i++)
        expr_add_child(ec, body_prog->c[i]);
    if (body_prog != raw_body) { if (body_prog->c) ct_drop((char *)body_prog->c - sizeof(size_t)); ct_drop(body_prog); }
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_ssu_arrow(tree_t *tr) {
    tree_t *a = (tr && tr->n == 1) ? tr->c[0] : (tree_t *)0;
    return (a && a->t == TT_FNC && a->n == 2 && a->v.sval && !strcmp(a->v.sval, "=>")) ? a : (tree_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_ssu_head(tree_t *arrow) {
    tree_t *lhs = arrow->c[0];
    return (lhs && lhs->t == TT_FNC && lhs->n == 2 && lhs->v.sval && !strcmp(lhs->v.sval, ",")) ? lhs->c[0] : lhs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lower_ssu_clause_from_tree(tree_t *arrow, PredKey key, TRSlotMap *smp) {
    tree_t *lhs = arrow->c[0], *head = pl_ssu_head(arrow), *guard = (head == lhs) ? (tree_t *)0 : lhs->c[1];
    tree_t *syn = ast_node_new(TT_CLAUSE), *conj = (tree_t *)0;
    expr_add_child(syn, head);
    if (guard) {
        conj = ast_node_new(TT_FNC);
        conj->v.sval = ct_strdup(",");
        expr_add_child(conj, guard);
        expr_add_child(conj, arrow->c[1]);
        expr_add_child(syn, conj);
    } else expr_add_child(syn, arrow->c[1]);
    tree_t *ec = lower_clause_from_tree(syn, key, 0, smp);
    tree_t *gp = ast_node_new(TT_PROGRAM), *bp = ast_node_new(TT_PROGRAM), *cnt = ast_node_new(TT_PROGRAM);
    if (conj) pl_flatten_conj(conj->c[0], cnt);
    int ng = cnt->n;
    tree_t *nc = ast_node_new(TT_CLAUSE);
    nc->v = ec->v;
    for (int i = 0; i < key.arity && i < ec->n; i++) expr_add_child(nc, ec->c[i]);
    for (int i = key.arity; i < ec->n; i++) expr_add_child(i < key.arity + ng ? gp : bp, ec->c[i]);
    tree_t *neck = ast_node_new(TT_FNC);
    neck->v.sval = ct_strdup("=>");
    expr_add_child(neck, gp);
    expr_add_child(neck, bp);
    expr_add_child(nc, neck);
    return nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static PredKey key_of_head_tree(tree_t *head) {
    PredKey k = {-1, 0};
    if (!head) return k;
    const char *fn = NULL;
    int arity = 0;
    tr_head_key(head, &fn, &arity);
    if (!fn) return k;
    k.functor = prolog_atom_intern(fn);
    k.arity   = arity;
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *tr_dup(const tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t);
    c->v.ival = e->v.ival;
    c->v.dval = e->v.dval;
    switch (e->t) {
        case TT_QLIT: case TT_VAR: case TT_KEYWORD: case TT_FNC:
        case TT_IDX:  case TT_CSET: case TT_ATTR:
            c->v.sval = e->v.sval ? ct_strdup(e->v.sval) : NULL;
            break;
        default:
            break;
    }
    for (int i = 0; i < e->n; i++)
        expr_add_child(c, tr_dup(e->c[i]));
    return c;
}
extern void pl_dyn_mark(const char *name, int arity);
extern int  pl_dyn_is_marked(const char *name, int arity);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pld_mark_spec(tree_t *spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && spec->c[1]) {
        tree_t *nm = spec->c[0], *ar = spec->c[1];
        if ((nm->t == TT_QLIT || nm->t == TT_NAME) && nm->v.sval && ar->t == TT_ILIT) pl_dyn_mark(ct_strdup(nm->v.sval), (int)ar->v.ival);
        return;
    }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pld_mark_spec(spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "as") && spec->n == 2) { pld_mark_spec(spec->c[0]); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pld_mark_spec(spec->c[0]); pld_mark_spec(spec->c[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pld_mark_clause_arg(tree_t *arg) {
    if (!arg) return;
    tree_t *h = arg;
    if (arg->t == TT_FNC && arg->v.sval && !strcmp(arg->v.sval, ":-") && arg->n == 2) h = arg->c[0];
    if (!h) return;
    if (h->t == TT_FNC && h->v.sval) pl_dyn_mark(ct_strdup(h->v.sval), h->n);
    else if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) pl_dyn_mark(ct_strdup(h->v.sval), 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pld_mark_scan(tree_t *t, int mark_assertz) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && t->n >= 1) {
        const char *fn = t->v.sval;
        if (mark_assertz && (!strcmp(fn,"assertz")||!strcmp(fn,"asserta")||!strcmp(fn,"assert")) && t->n == 1) pld_mark_clause_arg(t->c[0]);
        else if ((!strcmp(fn,"retract")||!strcmp(fn,"retractall")) && t->n == 1) pld_mark_clause_arg(t->c[0]);
        else if (!strcmp(fn,"abolish") && t->n == 1) pld_mark_spec(t->c[0]);
        else if (!strcmp(fn,"dynamic") && (t->n == 1 || t->n == 2)) pld_mark_spec(t->c[0]);
    }
    for (int i = 0; i < t->n; i++) pld_mark_scan(t->c[i], mark_assertz);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *pl_runtime_clause_tree(tree_t *raw) {
    tree_t *syn; tree_t *head; tree_t *body = (tree_t *)0; PredKey k;
    if (!raw) return (tree_t *)0;
    head = raw;
    if (raw->t == TT_FNC && raw->v.sval && !strcmp(raw->v.sval, ":-") && raw->n == 2) { head = raw->c[0]; body = raw->c[1]; }
    k = key_of_head_tree(head);
    if (k.functor < 0) return (tree_t *)0;
    syn = ast_node_new(TT_CLAUSE);
    expr_add_child(syn, head);
    if (body) expr_add_child(syn, body);
    { TRSlotMap sm; tree_t *ec; trslot_reset(&sm); ec = lower_clause_from_tree(syn, k, 0, &sm); if (syn->c) ct_drop((char *)syn->c - sizeof(size_t)); ct_drop(syn); return ec; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_pieces(const tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_CAT) return t->n == 2 && pf_pieces(t->c[0]) && pf_pieces(t->c[1]);
    return t->t == TT_ESC || t->t == TT_QLIT;
}
static int pf_len(const tree_t *t) {
    if (t->t == TT_CAT) return pf_len(t->c[0]) + pf_len(t->c[1]);
    if (t->t == TT_ESC) return 4;
    return t->v.sval ? (int)strlen(t->v.sval) : 0;
}
static int pf_put(const tree_t *t, char *o) {
    extern int prolog_escape_bytes(const char *, char *, int); int n;
    if (t->t == TT_CAT) { n = pf_put(t->c[0], o); return n + pf_put(t->c[1], o + n); }
    if (t->t == TT_ESC) return prolog_escape_bytes(t->v.sval, o, 4);
    n = t->v.sval ? (int)strlen(t->v.sval) : 0; memcpy(o, t->v.sval, (size_t)n); return n;
}
void prolog_fold_pieces(tree_t *t) {
    if (!t) return;
    if ((t->t == TT_CAT || t->t == TT_ESC) && pf_pieces(t)) {
        char *b = (char *)ct_alloc((size_t)pf_len(t) + 1); int n = pf_put(t, b); b[n] = 0;
        t->t = TT_QLIT; t->v.sval = b; t->n = 0; return;
    }
    for (int i = 0; i < t->n; i++) prolog_fold_pieces(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prolog_dq_flag_mode(const char *v) {
    if (!v) return -1;
    return !strcmp(v, "atom") ? 0 : !strcmp(v, "chars") ? 1 : !strcmp(v, "codes") ? 2 : !strcmp(v, "string") ? 3 : -1;
}
static int pl_dq_directive_mode(const tree_t *g) {
    const tree_t *f, *v;
    if (!g || g->t != TT_FNC || !g->v.sval || strcmp(g->v.sval, "set_prolog_flag") || g->n != 2) return -1;
    f = g->c[0]; v = g->c[1];
    if (!f || !v || f->t != TT_QLIT || v->t != TT_QLIT || !f->v.sval || strcmp(f->v.sval, "double_quotes")) return -1;
    return prolog_dq_flag_mode(v->v.sval);
}
void prolog_dq_lower(tree_t *t, int mode) {
    if (!t) return;
    if (t->t == TT_DQLIT) {
        const char *b = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : ""; size_t n = strlen(b);
        if (mode == 3) return;
        if (mode == 0) { t->t = TT_QLIT; t->v.sval = (char *)b; t->n = 0; return; }
        t->t = TT_MAKELIST; t->v.ival = 0; t->n = 0;
        for (size_t i = 0; i < n; i++) {
            tree_t *e;
            if (mode == 2) { e = ast_node_new(TT_ILIT); e->v.ival = (long long)(unsigned char)b[i]; }
            else { char one[2]; one[0] = b[i]; one[1] = 0; e = ast_node_new(TT_QLIT); e->v.sval = ct_strdup(one); }
            ast_push(t, e);
        }
        return;
    }
    for (int i = 0; i < t->n; i++) prolog_dq_lower(t->c[i], mode);
}
static void pl_dq_lower_program(PlProgram *pl_prog) {
    int mode = g_stage2.pl_dq_mode ? g_stage2.pl_dq_mode - 1 : 2;
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next) {
        tree_t *tr = cl->tr; int m;
        if (tr && tr->t == TT_CLAUSE && tr->n == 2 && tr->c[0] && tr->c[0]->t == TT_NUL && (m = pl_dq_directive_mode(tr->c[1])) >= 0) { mode = m; continue; }
        prolog_dq_lower(tr, cl->dq_forced ? cl->dq_forced - 1 : mode);
    }
    g_stage2.pl_dq_mode = mode + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_stmt_push(tree_t *prog, tree_t *subj, int lineno, const char *file) {
    tree_t *st = ast_node_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", lineno)); ast_push(st, ast_attr_int(":lline", lineno)); ast_push(st, ast_attr_int(":stno", 0)); ast_push(st, ast_attr_expr(":subj", subj));
    if (file && file[0]) ast_push(st, ast_attr_leaf(":file", file));
    ast_push(prog, st);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *mod; const char *name; int ar; } pl_mkey_t;
typedef struct { cv_t defs; cv_t exps; cv_t trans; cv_t metas; cv_t dyns; cv_t foreign; } pl_mods_t;
typedef struct { const char *nm; int ar; const char *spec; } pl_mspec_t;
static const pl_mspec_t pl_mod_meta_builtins[] = { { "call", 1, "0" }, { "call", 2, "1-" }, { "call", 3, "2--" }, { "call", 4, "3---" }, { "call", 5, "4----" }, { "call", 6, "5-----" },
    { "call", 7, "6------" }, { "call", 8, "7-------" }, { "findall", 3, "-0-" }, { "findall", 4, "-0--" }, { "forall", 2, "00" }, { "\\+", 1, "0" }, { "not", 1, "0" }, { "once", 1, "0" },
    { "ignore", 1, "0" }, { "catch", 3, "0-0" }, { "bagof", 3, "-^-" }, { "setof", 3, "-^-" }, { "aggregate_all", 3, "-0-" }, { "aggregate_all", 4, "--0-" }, { "call_cleanup", 2, "00" },
    { "setup_call_cleanup", 3, "000" }, { "time", 1, "0" }, { "with_output_to", 2, "-0" }, { "maplist", 2, "1-" }, { "maplist", 3, "2--" }, { "maplist", 4, "3---" }, { "maplist", 5, "4----" },
    { "maplist", 6, "5-----" }, { "maplist", 7, "6------" }, { "foldl", 4, "3---" }, { "foldl", 5, "4----" }, { "foldl", 6, "5-----" }, { "foldl", 7, "6------" }, { "include", 3, "1--" },
    { "exclude", 3, "1--" }, { "partition", 4, "1---" }, { "partition", 6, "2-----" }, { "convlist", 3, "2--" }, { "phrase", 2, "2-" }, { "phrase", 3, "2--" }, { "initialization", 1, "0" },
    { "initialization", 2, "0-" }, { "assertion", 1, "0" }, { "call_nth", 2, "0-" }, { "limit", 2, "-0" }, { "offset", 2, "-0" }, { "distinct", 1, "0" }, { "distinct", 2, "-0" },
    { "snapshot", 1, "0" }, { "transaction", 1, "0" }, { "tnot", 1, "0" }, { "call_with_depth_limit", 3, "0--" }, { 0, 0, 0 } };
static int pl_mk_has(const cv_t *v, const char *mod, const char *name, int ar) {
    for (uint32_t i = 0; i < v->len; i++) {
        const pl_mkey_t *e = &CV_AT(*v, pl_mkey_t, i);
        if (e->ar == ar && !strcmp(e->name, name) && (!mod || (e->mod && !strcmp(e->mod, mod)))) return 1;
    }
    return 0;
}
static void pl_mk_add(cv_t *v, const char *mod, const char *name, int ar) { if (!pl_mk_has(v, mod, name, ar)) { pl_mkey_t k; k.mod = mod; k.name = name; k.ar = ar; CV_PUSH(*v, pl_mkey_t) = k; } }
static const char *pl_mod_qname(const char *mod, const char *nm) { char b[strlen(mod) + strlen(nm) + 2]; snprintf(b, sizeof b, "%s:%s", mod, nm); (void)prolog_atom_intern(b); return ct_strdup(b); }
static int pl_mod_atomish(const tree_t *t) { return t && (t->t == TT_QLIT || t->t == TT_NAME || (t->t == TT_FNC && t->n == 0)) && t->v.sval; }
static int pl_mod_never_renamed(const char *nm, int ar, int in_unit) {
    if (!nm || nm[0] == '$') return 1;
    if ((!strcmp(nm, "attr_unify_hook") && ar == 2) || (!strcmp(nm, "attribute_goals") && ar == 3) || (!strcmp(nm, "term_expansion") && ar == 2)) return 1;
    if ((!strcmp(nm, "goal_expansion") && ar == 2) || (!strcmp(nm, "portray") && ar == 1) || (!strcmp(nm, "message_hook") && ar == 3)) return 1;
    return in_unit && !strcmp(nm, "test") && (ar == 1 || ar == 2);
}
static tree_t *pl_mod_directive_goal(tree_t *tr) {
    tree_t *bp;
    if (!tr || tr->t != TT_CLAUSE || tr->n < 2 || !tr->c[0] || tr->c[0]->t != TT_NUL) return (tree_t *)0;
    bp = tr->c[1];
    if (bp && bp->t == TT_PROGRAM && bp->n > 0) return bp->c[0];
    return bp;
}
static void pl_mod_pi_each(tree_t *spec, const char *mod, cv_t *into) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && (!strcmp(spec->v.sval, ",") || !strcmp(spec->v.sval, "as")) && spec->n == 2) {
        pl_mod_pi_each(spec->c[0], mod, into);
        if (!strcmp(spec->v.sval, ",")) pl_mod_pi_each(spec->c[1], mod, into);
        return;
    }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_mod_pi_each(spec->c[i], mod, into); return; }
    if (spec->t == TT_FNC && spec->v.sval && (!strcmp(spec->v.sval, "/") || !strcmp(spec->v.sval, "//")) && spec->n == 2 && pl_mod_atomish(spec->c[0]) && spec->c[1] && spec->c[1]->t == TT_ILIT)
        pl_mk_add(into, mod, spec->c[0]->v.sval, (int)spec->c[1]->v.ival + (spec->v.sval[1] == '/' ? 2 : 0));
}
static const char *pl_mod_resolve(pl_mods_t *ms, const char *m1, const char *m2, const char *nm, int ar) {
    if (!nm) return (const char *)0;
    if (m1 && pl_mk_has(&ms->defs, m1, nm, ar)) return pl_mod_qname(m1, nm);
    if (m2 && pl_mk_has(&ms->defs, m2, nm, ar)) return pl_mod_qname(m2, nm);
    return (const char *)0;
}
static tree_t *pl_mod_ctx_node(const char *m1, int transparent) {
    tree_t *c = ast_node_new(transparent ? TT_VAR : TT_QLIT);
    c->v.sval = ct_strdup(transparent ? "$ctx" : (m1 ? m1 : "user"));
    return c;
}
static void pl_mod_prepend(tree_t *g, tree_t *first) {
    ast_push(g, first);
    for (int j = g->n - 1; j > 0; j--) g->c[j] = g->c[j - 1];
    g->c[0] = first;
}
static void pl_mod_head(pl_mods_t *ms, const char *m1, const char *m2, tree_t *h) {
    const char *q;
    if (!h) return;
    if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && pl_mod_atomish(h->c[0])) {
        tree_t *in = h->c[1];
        const char *m = h->c[0]->v.sval;
        if (in && pl_mod_atomish(in) && (q = pl_mod_resolve(ms, m, (const char *)0, in->v.sval, 0))) in->v.sval = (char *)q;
        else if (in && in->t == TT_FNC && in->v.sval && (q = pl_mod_resolve(ms, m, (const char *)0, in->v.sval, in->n))) in->v.sval = (char *)q;
        return;
    }
    if (pl_mod_atomish(h) && (q = pl_mod_resolve(ms, m1, m2, h->v.sval, 0))) h->v.sval = (char *)q;
    else if (h->t == TT_FNC && h->v.sval && (q = pl_mod_resolve(ms, m1, m2, h->v.sval, h->n))) h->v.sval = (char *)q;
}
static void pl_mod_goal(pl_mods_t *ms, const char *m1, const char *m2, tree_t *g, int trans);
static void pl_mod_clause_term(pl_mods_t *ms, const char *m1, const char *m2, tree_t *t, int trans) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ":-") && t->n == 2) { pl_mod_head(ms, m1, m2, t->c[0]); pl_mod_goal(ms, m1, m2, t->c[1], trans); return; }
    pl_mod_head(ms, m1, m2, t);
}
static void pl_mod_pi(pl_mods_t *ms, const char *m1, const char *m2, tree_t *spec) {
    const char *q;
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && (!strcmp(spec->v.sval, ",") || !strcmp(spec->v.sval, "as")) && spec->n == 2) {
        pl_mod_pi(ms, m1, m2, spec->c[0]);
        if (!strcmp(spec->v.sval, ",")) pl_mod_pi(ms, m1, m2, spec->c[1]);
        return;
    }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_mod_pi(ms, m1, m2, spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && (!strcmp(spec->v.sval, "/") || !strcmp(spec->v.sval, "//")) && spec->n == 2 && pl_mod_atomish(spec->c[0]) && spec->c[1] && spec->c[1]->t == TT_ILIT
        && (q = pl_mod_resolve(ms, m1, m2, spec->c[0]->v.sval, (int)spec->c[1]->v.ival + (spec->v.sval[1] == '/' ? 2 : 0))))
        spec->c[0]->v.sval = (char *)q;
}
static void pl_mod_closure(pl_mods_t *ms, const char *m1, const char *m2, tree_t *a, int extra) {
    const char *q;
    if (!a) return;
    if (pl_mod_atomish(a) && (q = pl_mod_resolve(ms, m1, m2, a->v.sval, extra))) a->v.sval = (char *)q;
    else if (a->t == TT_FNC && a->v.sval && strcmp(a->v.sval, ":") && (q = pl_mod_resolve(ms, m1, m2, a->v.sval, a->n + extra))) a->v.sval = (char *)q;
}
static const char *pl_mod_meta_spec(pl_mods_t *ms, const char *nm, int ar) {
    for (int i = 0; pl_mod_meta_builtins[i].nm; i++) if (pl_mod_meta_builtins[i].ar == ar && !strcmp(pl_mod_meta_builtins[i].nm, nm)) return pl_mod_meta_builtins[i].spec;
    for (uint32_t i = 0; i < ms->metas.len; i++) { const tree_t *m = CV_AT(ms->metas, const tree_t *, i); if (m->n == ar && m->v.sval && !strcmp(m->v.sval, nm)) return (const char *)m; }
    return (const char *)0;
}
static void pl_mod_args(pl_mods_t *ms, const char *m1, const char *m2, tree_t *g, int trans, const char *nm) {
    const char *spec = pl_mod_meta_spec(ms, nm, g->n);
    int declared = 0;
    if (!spec) return;
    for (uint32_t i = 0; i < ms->metas.len; i++) if ((const char *)CV_AT(ms->metas, const tree_t *, i) == spec) declared = 1;
    for (int i = 0; i < g->n; i++) {
        char k = '-';
        if (declared) {
            const tree_t *ds = ((const tree_t *)spec)->c[i];
            if (ds && ds->t == TT_ILIT && ds->v.ival >= 0 && ds->v.ival <= 9) k = (char)('0' + ds->v.ival);
            else if (ds && pl_mod_atomish(ds) && (!strcmp(ds->v.sval, ":") || !strcmp(ds->v.sval, "^"))) k = ds->v.sval[0];
        } else k = spec[i];
        if (k == '0') pl_mod_goal(ms, m1, m2, g->c[i], trans);
        else if (k == '^') { tree_t *a = g->c[i]; while (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "^") && a->n == 2) a = a->c[1]; pl_mod_goal(ms, m1, m2, a, trans); }
        else if (k >= '1' && k <= '9') pl_mod_closure(ms, m1, m2, g->c[i], k - '0');
        else if (k == ':' && g->c[i] && g->c[i]->t != TT_VAR && !(g->c[i]->t == TT_FNC && g->c[i]->v.sval && !strcmp(g->c[i]->v.sval, ":") && g->c[i]->n == 2) && m1 && strcmp(m1, "user")) {
            tree_t *w = ast_node_new(TT_FNC);
            w->v.sval = ct_strdup(":");
            ast_push(w, pl_mod_ctx_node(m1, trans));
            ast_push(w, g->c[i]);
            g->c[i] = w;
        }
    }
}
static void pl_mod_goal(pl_mods_t *ms, const char *m1, const char *m2, tree_t *g, int trans) {
    const char *nm, *q;
    int ar;
    if (!g) return;
    if (g->t == TT_PROGRAM) { for (int i = 0; i < g->n; i++) pl_mod_goal(ms, m1, m2, g->c[i], trans); return; }
    if (!pl_mod_atomish(g) && g->t != TT_FNC) return;
    nm = g->v.sval;
    ar = (g->t == TT_FNC) ? g->n : 0;
    if (!nm) return;
    if (ar == 2 && (!strcmp(nm, ",") || !strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "*->"))) { pl_mod_goal(ms, m1, m2, g->c[0], trans); pl_mod_goal(ms, m1, m2, g->c[1], trans); return; }
    if (ar == 2 && !strcmp(nm, ":") && pl_mod_atomish(g->c[0]) && g->c[1]) {
        tree_t *in = g->c[1];
        const char *m = g->c[0]->v.sval;
        if (pl_mod_atomish(in) && (q = pl_mod_resolve(ms, m, (const char *)0, in->v.sval, 0))) in->v.sval = (char *)q;
        else if (in->t == TT_FNC && in->v.sval && (q = pl_mod_resolve(ms, m, (const char *)0, in->v.sval, in->n))) in->v.sval = (char *)q;
        return;
    }
    if (ar == 1 && !strcmp(nm, "context_module")) {
        g->v.sval = ct_strdup("=");
        ast_push(g, pl_mod_ctx_node(m1, trans));
        return;
    }
    if ((ar == 1 || ar == 2) && (!strcmp(nm, "assert") || !strcmp(nm, "asserta") || !strcmp(nm, "assertz") || (ar == 1 && !strcmp(nm, "retract")))) { pl_mod_clause_term(ms, m1, m2, g->c[0], trans); return; }
    if ((ar == 1 && !strcmp(nm, "retractall")) || (ar == 2 && (!strcmp(nm, "clause") || !strcmp(nm, "predicate_property")))) { pl_mod_head(ms, m1, m2, g->c[0]); return; }
    if (ar == 1 && (!strcmp(nm, "abolish") || !strcmp(nm, "current_predicate"))) { pl_mod_pi(ms, m1, m2, g->c[0]); return; }
    pl_mod_args(ms, m1, m2, g, trans, nm);
    q = pl_mod_resolve(ms, m1, m2, nm, ar);
    if (pl_mk_has(&ms->trans, (const char *)0, q ? q : nm, ar)) {
        char b[strlen(q ? q : nm) + 5];
        snprintf(b, sizeof b, "$mt %s", q ? q : nm);
        (void)prolog_atom_intern(b);
        if (g->t != TT_FNC) { g->t = TT_FNC; g->n = 0; }
        g->v.sval = ct_strdup(b);
        pl_mod_prepend(g, pl_mod_ctx_node(m1, trans));
        return;
    }
    if (q) g->v.sval = (char *)q;
}
static void pl_mod_opt1(pl_mods_t *ms, const char *m1, const char *m2, tree_t *o, int trans) {
    if (o && o->t == TT_FNC && o->n == 1 && o->v.sval && (!strcmp(o->v.sval, "setup") || !strcmp(o->v.sval, "cleanup") || !strcmp(o->v.sval, "condition") || !strcmp(o->v.sval, "forall")))
        pl_mod_goal(ms, m1, m2, o->c[0], trans);
}
static void pl_mod_opts(pl_mods_t *ms, const char *m1, const char *m2, tree_t *opts, int trans) {
    if (!opts) return;
    if (opts->t != TT_MAKELIST) { pl_mod_opt1(ms, m1, m2, opts, trans); return; }
    for (int i = 0; i < opts->n; i++) pl_mod_opt1(ms, m1, m2, opts->c[i], trans);
}
static void pl_mod_meta_collect(tree_t *e, cv_t *metas) {
    if (!e) return;
    if (e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, ",") && e->n == 2) { pl_mod_meta_collect(e->c[0], metas); pl_mod_meta_collect(e->c[1], metas); return; }
    if (e->t == TT_MAKELIST) { for (int j = 0; j < e->n; j++) pl_mod_meta_collect(e->c[j], metas); return; }
    if (e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, ":") && e->n == 2) e = e->c[1];
    if (e && e->t == TT_FNC && e->v.sval && e->n > 0) CV_PUSH(*metas, const tree_t *) = e;
}
static void pl_module_flatten(PlProgram *pl_prog) {
    pl_mods_t ms;
    int n = 0, ci = 0, any = 0;
    memset(&ms, 0, sizeof ms);
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next) n++;
    if (!n) return;
    const char *m1v[n], *m2v[n];
    const char *fmod = "user", *unit = (const char *)0;
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next, ci++) {
        tree_t *d = pl_mod_directive_goal(cl->tr);
        int prelude = (cl->lineno == 0);
        m1v[ci] = prelude ? "user" : (unit ? unit : fmod);
        m2v[ci] = (prelude || !unit) ? (const char *)0 : fmod;
        if (prelude) continue;
        if (d && d->t == TT_FNC && d->v.sval) {
            const char *f = d->v.sval;
            if (!strcmp(f, "module") && d->n == 2 && pl_mod_atomish(d->c[0])) { fmod = d->c[0]->v.sval; m1v[ci] = fmod; pl_mod_pi_each(d->c[1], fmod, &ms.exps); any = 1; }
            else if (!strcmp(f, "$module_restore") && d->n == 1 && pl_mod_atomish(d->c[0])) fmod = d->c[0]->v.sval;
            else if (!strcmp(f, "export") && d->n == 1) pl_mod_pi_each(d->c[0], fmod, &ms.exps);
            else if (!strcmp(f, "begin_tests") && (d->n == 1 || d->n == 2) && pl_mod_atomish(d->c[0])) {
                char b[strlen(d->c[0]->v.sval) + 8];
                snprintf(b, sizeof b, "plunit_%s", d->c[0]->v.sval);
                unit = ct_strdup(b);
                m1v[ci] = unit;
                m2v[ci] = fmod;
                any = 1;
            }
            else if (!strcmp(f, "end_tests")) unit = (const char *)0;
            else if (!strcmp(f, "module_transparent")) for (int k = 0; k < d->n; k++) pl_mod_pi_each(d->c[k], m1v[ci], &ms.trans);
            else if (!strcmp(f, "meta_predicate")) for (int k = 0; k < d->n; k++) pl_mod_meta_collect(d->c[k], &ms.metas);
            else if (!strcmp(f, "dynamic") || !strcmp(f, "discontiguous") || !strcmp(f, "table")) {
                if (strcmp(m1v[ci], "user")) for (int k = 0; k < (d->n == 2 && !strcmp(f, "dynamic") ? 1 : d->n); k++) {
                    pl_mod_pi_each(d->c[k], m1v[ci], &ms.defs);
                    if (!strcmp(f, "dynamic")) pl_mod_pi_each(d->c[k], m1v[ci], &ms.dyns);
                }
            }
            continue;
        }
        if (!cl->tr || cl->tr->t != TT_CLAUSE || cl->tr->n < 1 || !cl->tr->c[0] || cl->tr->c[0]->t == TT_NUL) continue;
        {
            tree_t *h = cl->tr->c[0];
            const char *dm = m1v[ci];
            int foreign = 0;
            if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && pl_mod_atomish(h->c[0])) { dm = h->c[0]->v.sval; h = h->c[1]; foreign = 1; }
            if (!h || !dm || !strcmp(dm, "user")) continue;
            if (h->t == TT_FNC && h->n == 2 && h->v.sval && (!strcmp(h->v.sval, ":-") || !strcmp(h->v.sval, ",") || !strcmp(h->v.sval, ";") || !strcmp(h->v.sval, "->") || !strcmp(h->v.sval, ":"))) continue;
            {
                const char *hn = h->v.sval;
                int ha = (h->t == TT_FNC) ? h->n : 0;
                if (!hn || (h->t != TT_FNC && !pl_mod_atomish(h)) || pl_mod_never_renamed(hn, ha, unit != (const char *)0)) continue;
                pl_mk_add(&ms.defs, dm, hn, ha);
                if (foreign && strcmp(dm, m1v[ci])) pl_mk_add(&ms.foreign, dm, hn, ha);
            }
        }
    }
    if (!any) return;
    {
        cv_t keep = { 0 };
        for (uint32_t i = 0; i < ms.defs.len; i++) {
            const pl_mkey_t *e = &CV_AT(ms.defs, pl_mkey_t, i);
            if (strncmp(e->mod, "plunit_", 7) && pl_mk_has(&ms.exps, e->mod, e->name, e->ar)) continue;
            CV_PUSH(keep, pl_mkey_t) = *e;
        }
        ms.defs = keep;
    }
    {
        cv_t tf = { 0 };
        for (uint32_t i = 0; i < ms.trans.len; i++) {
            const pl_mkey_t *e = &CV_AT(ms.trans, pl_mkey_t, i);
            const char *q = pl_mk_has(&ms.defs, e->mod, e->name, e->ar) ? pl_mod_qname(e->mod, e->name) : e->name;
            pl_mk_add(&tf, (const char *)0, q, e->ar);
        }
        ms.trans = tf;
    }
    ci = 0;
    PlClause *last = (PlClause *)0;
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next, ci++) {
        tree_t *d = pl_mod_directive_goal(cl->tr);
        const char *m1 = m1v[ci], *m2 = m2v[ci];
        last = cl;
        if (cl->lineno == 0) continue;
        if (d) {
            const char *f = (d->t == TT_FNC) ? d->v.sval : (const char *)0;
            if (f && (!strcmp(f, "dynamic") || !strcmp(f, "discontiguous") || !strcmp(f, "table") || !strcmp(f, "module_transparent"))) { for (int k = 0; k < d->n; k++) pl_mod_pi(&ms, m1, m2, d->c[k]); continue; }
            if (f && !strcmp(f, "meta_predicate")) continue;
            if (f && !strcmp(f, "begin_tests") && d->n == 2) { pl_mod_opts(&ms, m1, m2, d->c[1], 0); continue; }
            if (f && (!strcmp(f, "module") || !strcmp(f, "$module_restore") || !strcmp(f, "end_tests") || !strcmp(f, "begin_tests") || !strcmp(f, "use_module") || !strcmp(f, "ensure_loaded")
                || !strcmp(f, "multifile") || !strcmp(f, "op") || !strcmp(f, "set_prolog_flag"))) continue;
            pl_mod_goal(&ms, m1, m2, cl->tr->c[1], 0);
            continue;
        }
        if (!cl->tr || cl->tr->t != TT_CLAUSE || cl->tr->n < 1 || !cl->tr->c[0]) continue;
        {
            tree_t *h = cl->tr->c[0];
            int trans;
            const char *hn0, *q;
            int ha;
            if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2) {
                tree_t *in = h->c[1];
                if (!in || !in->v.sval || pl_mod_never_renamed(in->v.sval, in->t == TT_FNC ? in->n : 0, m1 && !strncmp(m1, "plunit_", 7)) || (in->t == TT_FNC && in->n == 2 && (!strcmp(in->v.sval, ":-")
                    || !strcmp(in->v.sval, ",") || !strcmp(in->v.sval, ";") || !strcmp(in->v.sval, "->") || !strcmp(in->v.sval, ":")))) {
                    if (cl->tr->n >= 2) pl_mod_goal(&ms, m1, m2, cl->tr->c[1], 0);
                    continue;
                }
                pl_mod_head(&ms, m1, m2, h);
                cl->tr->c[0] = in;
                h = in;
            } else if (h->v.sval && m1 && strcmp(m1, "user") && h->t == TT_FNC && ((!strcmp(h->v.sval, "attr_unify_hook") && h->n == 2) || (!strcmp(h->v.sval, "attribute_goals") && h->n == 3))) {
                tree_t *q = ast_node_new(TT_FNC);
                q->v.sval = ct_strdup(":");
                ast_push(q, pl_mod_ctx_node(m1, 0));
                ast_push(q, h);
                cl->tr->c[0] = q;
                if (cl->tr->n >= 2) pl_mod_goal(&ms, m1, m2, cl->tr->c[1], 0);
                continue;
            } else pl_mod_head(&ms, m1, m2, h);
            hn0 = h->v.sval;
            ha = (h->t == TT_FNC) ? h->n : 0;
            trans = hn0 && pl_mk_has(&ms.trans, (const char *)0, hn0, ha);
            if (trans) {
                char b[strlen(hn0) + 5];
                snprintf(b, sizeof b, "$mt %s", hn0);
                (void)prolog_atom_intern(b);
                if (h->t != TT_FNC) { h->t = TT_FNC; h->n = 0; }
                q = ct_strdup(b);
                h->v.sval = (char *)q;
                pl_mod_prepend(h, pl_mod_ctx_node(m1, 1));
            }
            if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, "test") && (h->n == 2) && m1 && !strncmp(m1, "plunit_", 7)) pl_mod_opts(&ms, m1, m2, h->c[1], 0);
            if (cl->tr->n >= 2) pl_mod_goal(&ms, m1, m2, cl->tr->c[1], trans);
        }
    }
    {
        cv_t bare = { 0 };
        for (PlClause *cl = pl_prog->head; cl; cl = cl->next) {
            tree_t *h = (cl->tr && cl->tr->t == TT_CLAUSE && cl->tr->n >= 1) ? cl->tr->c[0] : (tree_t *)0;
            if (!h || h->t == TT_NUL || !h->v.sval || strchr(h->v.sval, ':')) continue;
            pl_mk_add(&bare, (const char *)0, h->v.sval, h->t == TT_FNC ? h->n : 0);
        }
        for (uint32_t i = 0; i < ms.defs.len && last; i++) {
            const pl_mkey_t *e = &CV_AT(ms.defs, pl_mkey_t, i);
            int dup = pl_mk_has(&ms.foreign, e->mod, e->name, e->ar);
            for (uint32_t j = 0; j < ms.defs.len; j++) {
                const pl_mkey_t *o = &CV_AT(ms.defs, pl_mkey_t, j);
                if (j != i && o->ar == e->ar && !strcmp(o->name, e->name) && !pl_mk_has(&ms.foreign, o->mod, o->name, o->ar)) dup = 1;
            }
            if (dup || pl_mk_has(&bare, (const char *)0, e->name, e->ar) || pl_mk_has(&ms.dyns, e->mod, e->name, e->ar) || pl_mk_has(&ms.trans, (const char *)0, pl_mod_qname(e->mod, e->name), e->ar)) continue;
            {
                PlClause *w = ct_zalloc(1, sizeof(PlClause));
                tree_t *cl = ast_node_new(TT_CLAUSE), *hd = ast_node_new(e->ar > 0 ? TT_FNC : TT_QLIT), *call = ast_node_new(e->ar > 0 ? TT_FNC : TT_QLIT);
                hd->v.sval = ct_strdup(e->name);
                call->v.sval = (char *)pl_mod_qname(e->mod, e->name);
                for (int k = 0; k < e->ar; k++) {
                    char vn[16];
                    snprintf(vn, sizeof vn, "$a%d", k);
                    tree_t *v1 = ast_node_new(TT_VAR), *v2 = ast_node_new(TT_VAR);
                    v1->v.sval = ct_strdup(vn);
                    v2->v.sval = ct_strdup(vn);
                    ast_push(hd, v1);
                    ast_push(call, v2);
                }
                ast_push(cl, hd);
                ast_push(cl, call);
                w->tr = cl;
                w->lineno = last->lineno > 0 ? last->lineno : 1;
                w->next = last->next;
                last->next = w;
                if (pl_prog->tail == last) pl_prog->tail = w;
                last = w;
                pl_prog->nclauses++;
            }
        }
    }
    for (uint32_t i = 0; i < ms.trans.len && last; i++) {
        const pl_mkey_t *e = &CV_AT(ms.trans, pl_mkey_t, i);
        PlClause *w = ct_zalloc(1, sizeof(PlClause));
        tree_t *cl = ast_node_new(TT_CLAUSE), *hd = ast_node_new(e->ar > 0 ? TT_FNC : TT_QLIT), *call = ast_node_new(TT_FNC), *u = ast_node_new(TT_QLIT);
        char b[strlen(e->name) + 5];
        snprintf(b, sizeof b, "$mt %s", e->name);
        (void)prolog_atom_intern(b);
        hd->v.sval = ct_strdup(e->name);
        call->v.sval = ct_strdup(b);
        u->v.sval = ct_strdup("user");
        ast_push(call, u);
        for (int k = 0; k < e->ar; k++) {
            char vn[16];
            snprintf(vn, sizeof vn, "$a%d", k);
            tree_t *v1 = ast_node_new(TT_VAR), *v2 = ast_node_new(TT_VAR);
            v1->v.sval = ct_strdup(vn);
            v2->v.sval = ct_strdup(vn);
            ast_push(hd, v1);
            ast_push(call, v2);
        }
        ast_push(cl, hd);
        ast_push(cl, call);
        w->tr = cl;
        w->lineno = last->lineno > 0 ? last->lineno : 1;
        w->next = last->next;
        last->next = w;
        if (pl_prog->tail == last) pl_prog->tail = w;
        last = w;
        pl_prog->nclauses++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *prolog_lower(PlProgram *pl_prog) {
    for (PlClause *fcl = pl_prog->head; fcl; fcl = fcl->next) prolog_fold_pieces(fcl->tr);
    pl_dq_lower_program(pl_prog);
    for (PlClause *dcl = pl_prog->head; dcl; dcl = dcl->next) prolog_dcg_expand(dcl);
    pl_module_flatten(pl_prog);
    pl_dyn_mark(ct_strdup("$db_registry"), 0);
    for (PlClause *mcl = pl_prog->head; mcl; mcl = mcl->next) if (mcl->tr) { int _isdir = (mcl->tr->n > 0 && mcl->tr->c[0] && mcl->tr->c[0]->t == TT_NUL); pld_mark_scan(mcl->tr, 1); (void) _isdir; }
    tree_t *prog = ast_node_new(TT_PROGRAM);
    tree_t *pld_seed[256]; int pld_seed_n = 0;
    int n_clauses = 0; for (PlClause *cl = pl_prog->head; cl; cl = cl->next) n_clauses++;
    const char *plunit_suite[n_clauses > 0 ? n_clauses : 1];
    {
        const char *cur_suite = "";
        int ci = 0;
        for (PlClause *cl = pl_prog->head; cl; cl = cl->next, ci++) {
            plunit_suite[ci] = "";
            int is_directive = (cl->tr && cl->tr->n > 0 && cl->tr->c[0] && cl->tr->c[0]->t == TT_NUL);
            int is_rule      = (cl->tr != NULL && cl->tr->n > 0 && cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL);
            if (is_directive) {
                tree_t *bp = cl->tr->c[1];
                tree_t *d = NULL;
                if (bp && bp->t == TT_PROGRAM && bp->n > 0) {
                    d = bp->c[0];
                } else if (bp) {
                    d = (bp->t == TT_FNC && bp->v.sval && strcmp(bp->v.sval, ",") == 0 && bp->n > 0)
                        ? bp->c[0] : bp;
                }
                if (d && d->t == TT_FNC && d->v.sval && d->n >= 1) {
                        if (strcmp(d->v.sval, "begin_tests") == 0) {
                            tree_t *a = d->c[0];
                            const char *sn = NULL;
                            if (a && a->t == TT_QLIT) sn = a->v.sval;
                            if (a && a->t == TT_FNC)  sn = a->v.sval;
                            if (sn) cur_suite = sn;
                        } else if (strcmp(d->v.sval, "end_tests") == 0) {
                            cur_suite = "";
                        }
                }
            } else if (is_rule && cur_suite[0]) {
                plunit_suite[ci] = cur_suite;
            }
        }
    }
    cv_t     keys = {0}, choices = {0};
    TRSlotMap csm; trslot_reset(&csm);
    int      clause_idx = 0;
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next, clause_idx++) {
        int is_rule = (cl->tr != NULL && cl->tr->n > 0 &&
                       cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL);
        if (!is_rule) continue;
        tree_t *ssu = pl_ssu_arrow(cl->tr);
        PredKey k = key_of_head_tree(ssu ? pl_ssu_head(ssu) : cl->tr->c[0]);
        if (k.functor < 0) continue;
        if (plunit_suite[clause_idx][0] != '\0') {
            const char *fn = prolog_atom_name(k.functor);
            if (fn && strcmp(fn, "test") == 0 && (k.arity == 1 || k.arity == 2)) {
                if (cl->tr) {
                    tree_t *hd_tr = cl->tr->c[0];
                    if (hd_tr && hd_tr->t == TT_FNC && hd_tr->n >= 1) {
                        tree_t *name_src = hd_tr->c[0];
                        tree_t *opts_src = (k.arity == 2 && hd_tr->n >= 2) ? hd_tr->c[1] : NULL;
                        tree_t *body_src = NULL;
                        if (cl->tr->n >= 2 && cl->tr->c[1] && cl->tr->c[1]->t != TT_NUL)
                            body_src = cl->tr->c[1];
                        tree_t *body_tr = NULL;
                        if (body_src && body_src->t == TT_PROGRAM) {
                            if (body_src->n == 1) {
                                body_tr = tr_dup(body_src->c[0]);
                            } else if (body_src->n > 1) {
                                tree_t *acc = tr_dup(body_src->c[body_src->n - 1]);
                                for (int bi = body_src->n - 2; bi >= 0; bi--) {
                                    tree_t *cm = ast_node_new(TT_FNC);
                                    cm->v.sval = ct_strdup(",");
                                    expr_add_child(cm, tr_dup(body_src->c[bi]));
                                    expr_add_child(cm, acc);
                                    acc = cm;
                                }
                                body_tr = acc;
                            }
                        } else if (body_src) {
                            body_tr = tr_dup(body_src);
                        }
                        if (!body_tr) {
                            body_tr = ast_node_new(TT_QLIT);
                            body_tr->v.sval = ct_strdup("true");
                        }
                        tree_t *name_tr = tr_dup(name_src);
                        tree_t *opts_tr = opts_src ? tr_dup(opts_src) : NULL;
                        if (!opts_tr) {
                            opts_tr = ast_node_new(TT_QLIT);
                            opts_tr->v.sval = ct_strdup("[]");
                        }
                        if (opts_tr->t != TT_MAKELIST && !((opts_tr->t == TT_QLIT || opts_tr->t == TT_NAME || opts_tr->t == TT_FNC) && opts_tr->v.sval && !strcmp(opts_tr->v.sval, "[]") && opts_tr->n == 0)) {
                            tree_t *one = ast_node_new(TT_MAKELIST); one->v.ival = 0; expr_add_child(one, opts_tr); opts_tr = one;
                        }
                        tree_t *suite_tr = ast_node_new(TT_QLIT);
                        suite_tr->v.sval = ct_strdup(plunit_suite[clause_idx]);
                        tree_t *pj_head = ast_node_new(TT_FNC);
                        pj_head->v.sval = ct_strdup("pj_test");
                        expr_add_child(pj_head, suite_tr);
                        expr_add_child(pj_head, name_tr);
                        expr_add_child(pj_head, opts_tr);
                        expr_add_child(pj_head, body_tr);
                        tree_t *syn = ast_node_new(TT_CLAUSE);
                        expr_add_child(syn, pj_head);
                        PredKey pk2 = { prolog_atom_intern("pj_test"), 4 };
                        int found = pl_pred_slot(&keys, &choices, pk2);
                        {
                            tree_t *ec = lower_clause_from_tree(syn, pk2, 0, &csm);
                            ec->line = cl->lineno;
                            expr_add_child(CV_AT(choices, tree_t *, found), ec);
                        }
                    }
                }
            }
        }
        int found = pl_pred_slot(&keys, &choices, k);
        tree_t *ec = ssu ? lower_ssu_clause_from_tree(ssu, k, &csm) : lower_clause_from_tree(cl->tr, k, cl->is_dcg, &csm);
        ec->line = cl->lineno;
        expr_add_child(CV_AT(choices, tree_t *, found), ec);
    }
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next) {
        if (cl->tr && cl->tr->n > 0 && cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL) continue;
        if (!cl->tr || cl->tr->n < 2) continue;
        tree_t *raw_body = cl->tr->c[1];
        tree_t *goal_tr = NULL;
        if (raw_body && raw_body->t == TT_PROGRAM && raw_body->n > 0) {
            goal_tr = raw_body->c[0];
        } else if (raw_body) {
            goal_tr = raw_body;
        }
        if (!goal_tr) continue;
        int is_assert = 0;
        if (goal_tr->t == TT_FNC && goal_tr->v.sval && goal_tr->n == 1
                && (strcmp(goal_tr->v.sval, "assertz") == 0
                    || strcmp(goal_tr->v.sval, "asserta") == 0)) {
            int prepend = (strcmp(goal_tr->v.sval, "asserta") == 0);
            tree_t *arg = goal_tr->c[0];
            tree_t *a_head = arg, *a_body = NULL;
            if (arg && arg->t == TT_FNC && arg->v.sval && strcmp(arg->v.sval, ":-") == 0 && arg->n == 2) {
                a_head = arg->c[0];
                a_body = arg->c[1];
            }
            if (a_head) {
                tree_t *syn = ast_node_new(TT_CLAUSE);
                expr_add_child(syn, a_head);
                if (a_body) expr_add_child(syn, a_body);
                PredKey ak = key_of_head_tree(a_head);
                if (ak.functor >= 0) {
                    const char *aknm = prolog_atom_name(ak.functor);
                    if (aknm && pl_dyn_is_marked(aknm, ak.arity)) { (void) pld_seed; (void) pld_seed_n; }
                    else {
                    int found = pl_pred_slot(&keys, &choices, ak);
                    {
                        tree_t *ec = lower_clause_from_tree(syn, ak, 0, &csm);
                        tree_t *fc = CV_AT(choices, tree_t *, found);
                        if (prepend) {
                            expr_add_child(fc, ec);
                            for (int j = fc->n - 1; j > 0; j--)
                                fc->c[j] = fc->c[j - 1];
                            fc->c[0] = ec;
                        } else {
                            expr_add_child(fc, ec);
                        }
                        is_assert = 1;
                    }
                    }
                }
            }
        }
        if (is_assert) continue;
        if (goal_tr->t == TT_FNC && goal_tr->v.sval && goal_tr->n > 0) {
            const char *gn = goal_tr->v.sval; int ga = goal_tr->n;
            int callable_with_args =
                  (strcmp(gn,"begin_tests")==0   && (ga==1||ga==2))
               || (strcmp(gn,"end_tests")==0     && ga==1)
               || (strcmp(gn,"nb_setval")==0     && ga==2);
            if (callable_with_args) {
                static int pj_dir_seq = 0;
                char hname[64]; snprintf(hname, sizeof hname, "pj_dir_%d", pj_dir_seq++);
                int hfn = prolog_atom_intern(hname);
                PredKey hk = { hfn, 0 };
                tree_t *helper_head = ast_node_new(TT_QLIT);
                helper_head->v.sval = ct_strdup(hname);
                tree_t *syn = ast_node_new(TT_FNC);
                syn->v.sval = ct_strdup(":-");
                syn->n = 0;
                expr_add_child(syn, helper_head);
                expr_add_child(syn, goal_tr);
                {
                    int hi = pl_pred_add(&keys, &choices, hk);
                    tree_t *ec = lower_clause_from_tree(syn, hk, 0, &csm);
                    expr_add_child(CV_AT(choices, tree_t *, hi), ec);
                    tree_t *init_arg = ast_node_new(TT_QLIT);
                    init_arg->v.sval = ct_strdup(hname);
                    tree_t *init_call = ast_node_new(TT_FNC);
                    init_call->v.sval = ct_strdup("initialization");
                    init_call->n = 0;
                    expr_add_child(init_call, init_arg);
                    goal_tr = init_call;
                }
            }
        }
        if (goal_tr && goal_tr->t == TT_FNC && goal_tr->v.sval && strcmp(goal_tr->v.sval, "export") == 0 && goal_tr->n == 1) continue;
        pl_stmt_push(prog, goal_tr, cl->lineno, pl_prog->filename);
    }
    if (pld_seed_n > 0) {
        for (uint32_t i = 0; i < keys.len; i++) {
            const char *kn = prolog_atom_name(CV_AT(keys, PredKey, i).functor);
            if (!kn || strcmp(kn, "main") || CV_AT(keys, PredKey, i).arity != 0) continue;
            tree_t *mchoice = CV_AT(choices, tree_t *, i);
            tree_t *mclause = (mchoice && mchoice->t == TT_CHOICE && mchoice->n >= 1) ? mchoice->c[0] : mchoice;
            if (!mclause || mclause->t != TT_CLAUSE) break;
            int arity0 = (int)mclause->v.dval; if (arity0 < 0) arity0 = 0;
            for (int sj = 0; sj < pld_seed_n; sj++) {
                tree_t *g = pl_rewrite_control(pld_seed[sj]);
                expr_add_child(mclause, g);
                for (int j = mclause->n - 1; j > arity0 + sj; j--) mclause->c[j] = mclause->c[j - 1];
                mclause->c[arity0 + sj] = g;
            }
            break;
        }
    }
    for (uint32_t i = 0; i < keys.len; i++) pl_stmt_push(prog, CV_AT(choices, tree_t *, i), 0, pl_prog->filename);
    return prog;
}
