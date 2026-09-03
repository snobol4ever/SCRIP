#include "prolog_lower.h"
#include "prolog_atom.h"
#include "scrip_cc.h"
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
    char buf[256];
    snprintf(buf, sizeof buf, "%s/%d", fn, arity);
    return strdup(buf);
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
    if (from >= semi_node->n) { tree_t *f = ast_node_new(TT_QLIT); f->v.sval = strdup("fail"); return f; }
    if (from == semi_node->n - 1) return semi_node->c[from];
    tree_t *rest = ast_node_new(TT_FNC); rest->v.sval = strdup(";");
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
        { tree_t *f = ast_node_new(TT_QLIT); f->v.sval = strdup("fail"); ast_push(else_prog, f); }
        tree_t *iff = ast_node_new(TT_IF);
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
#define TR_SLOT_MAX 256
typedef struct { const char *name; int slot; } TRSlot;
typedef struct { TRSlot e[TR_SLOT_MAX]; int n; int next; } TRSlotMap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trslot_reset(TRSlotMap *m) { m->n = 0; m->next = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trslot_get(TRSlotMap *m, const char *name) {
    if (!name || strcmp(name, "_") == 0)
        return m->next++;
    for (int i = 0; i < m->n; i++)
        if (strcmp(m->e[i].name, name) == 0)
            return m->e[i].slot;
    if (m->n >= TR_SLOT_MAX) return m->next++;
    int s = m->next++;
    m->e[m->n].name = name;
    m->e[m->n].slot = s;
    m->n++;
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
    for (int i = 0; i < arity && i < ec->n && sm.n < TR_SLOT_MAX; i++) {
        tree_t *a = ec->c[i];
        if (a && a->t == TT_VAR && a->v.sval && strcmp(a->v.sval, "_") != 0) {
            int dup = 0;
            for (int j = 0; j < sm.n; j++) if (sm.e[j].name && strcmp(sm.e[j].name, a->v.sval) == 0) { dup = 1; break; }
            if (!dup) { sm.e[sm.n].name = a->v.sval; sm.e[sm.n].slot = i; sm.n++; }
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
static tree_t *pb_var(const char *nm) { tree_t *v = ast_node_new(TT_VAR); v->v.sval = strdup(nm); return v; }
static tree_t *pb_fnc2(const char *f, tree_t *a, tree_t *b) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = strdup(f); ast_push(n, a); ast_push(n, b); return n; }
static tree_t *pb_fnc1(const char *f, tree_t *a) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = strdup(f); ast_push(n, a); return n; }
static tree_t *pb_fnc3(const char *f, tree_t *a, tree_t *b, tree_t *c) { tree_t *n = ast_node_new(TT_FNC); n->v.sval = strdup(f); ast_push(n, a); ast_push(n, b); ast_push(n, c); return n; }
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
static int g_pb_fresh_ctr = 0;
static void pb_expand_goal(tree_t *t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pb_expand_bagof(tree_t *t) {
    int is_setof = !strcmp(t->v.sval, "setof");
    tree_t *T = t->c[0]; tree_t *G = t->c[1]; tree_t *L = t->c[2];
    const char *quant[64]; int nq = 0;
    tree_t *G1 = G;
    while (G1 && G1->t == TT_FNC && G1->v.sval && !strcmp(G1->v.sval, "^") && G1->n == 2) { pb_collect_names(G1->c[0], quant, &nq, 64); G1 = G1->c[1]; }
    const char *gv[128]; int ngv = 0; pb_collect_names(G1, gv, &ngv, 128);
    const char *tv[128]; int ntv = 0; pb_collect_names(T, tv, &ntv, 128);
    const char *fv[128]; int nfv = 0;
    for (int i = 0; i < ngv; i++) { int drop = 0;
        for (int j = 0; j < ntv && !drop; j++) if (!strcmp(gv[i], tv[j])) drop = 1;
        for (int j = 0; j < nq  && !drop; j++) if (!strcmp(gv[i], quant[j])) drop = 1;
        if (!drop && nfv < 128) fv[nfv++] = gv[i]; }
    char b1[32], b2[32];
    tree_t *fa; tree_t *inner;
    if (nfv == 0) {
        snprintf(b1, sizeof b1, "_$B%d", g_pb_fresh_ctr++);
        fa = pb_fnc3("findall", T, G1, pb_var(b1));
        tree_t *ne = pb_fnc2("\\==", pb_var(b1), ast_node_new(TT_MAKELIST));
        tree_t *fin = is_setof ? pb_fnc2("sort", pb_var(b1), L) : pb_fnc2("=", pb_var(b1), L);
        inner = pb_fnc2(",", ne, fin);
    } else {
        tree_t *k1 = ast_node_new(TT_FNC); k1->v.sval = strdup("$bagkey"); for (int i = 0; i < nfv; i++) ast_push(k1, pb_var(fv[i]));
        tree_t *k2 = ast_node_new(TT_FNC); k2->v.sval = strdup("$bagkey"); for (int i = 0; i < nfv; i++) ast_push(k2, pb_var(fv[i]));
        snprintf(b1, sizeof b1, "_$B%d", g_pb_fresh_ctr++);
        snprintf(b2, sizeof b2, "_$B%d", g_pb_fresh_ctr++);
        fa = pb_fnc3("findall", pb_fnc2("-", k1, T), G1, pb_var(b1));
        tree_t *prep = pb_fnc2(is_setof ? "$bag_prep_s" : "$bag_prep_b", pb_var(b1), pb_var(b2));
        tree_t *grp = pb_fnc3("$bag_group", pb_var(b2), k2, L);
        inner = pb_fnc2(",", prep, grp);
    }
    t->v.sval = strdup(","); t->n = 0;
    ast_push(t, fa); ast_push(t, inner);
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
static tree_t *lower_clause_from_tree(tree_t *tr, PredKey key, int skip_rewrite) {
    TRSlotMap sm; trslot_reset(&sm);
    if (!skip_rewrite && tr->n > 1) pb_expand_goal(tr->c[1]);
    {
        tree_t *hd = (tr->n > 0) ? tr->c[0] : NULL;
        if (hd && hd->t == TT_FNC) {
            for (int i = 0; i < hd->n && sm.n < TR_SLOT_MAX; i++) {
                tree_t *a = hd->c[i];
                if (a && a->t == TT_VAR && a->v.sval && strcmp(a->v.sval, "_") != 0) {
                    int dup = 0;
                    for (int j = 0; j < sm.n; j++) if (strcmp(sm.e[j].name, a->v.sval) == 0) { dup = 1; break; }
                    if (!dup) { sm.e[sm.n].name = a->v.sval; sm.e[sm.n].slot = i; sm.n++; }
                }
            }
        }
    }
    sm.next = key.arity;
    tr_assign_slots(tr, &sm);
    int n_vars = sm.next;
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
    return ec;
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
            c->v.sval = e->v.sval ? strdup(e->v.sval) : NULL;
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
        if ((nm->t == TT_QLIT || nm->t == TT_NAME) && nm->v.sval && ar->t == TT_ILIT) pl_dyn_mark(strdup(nm->v.sval), (int)ar->v.ival);
        return;
    }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pld_mark_spec(spec->c[0]); pld_mark_spec(spec->c[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pld_mark_clause_arg(tree_t *arg) {
    if (!arg) return;
    tree_t *h = arg;
    if (arg->t == TT_FNC && arg->v.sval && !strcmp(arg->v.sval, ":-") && arg->n == 2) h = arg->c[0];
    if (!h) return;
    if (h->t == TT_FNC && h->v.sval) pl_dyn_mark(strdup(h->v.sval), h->n);
    else if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) pl_dyn_mark(strdup(h->v.sval), 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pld_mark_scan(tree_t *t, int mark_assertz) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && t->n >= 1) {
        const char *fn = t->v.sval;
        if (mark_assertz && (!strcmp(fn,"assertz")||!strcmp(fn,"asserta")||!strcmp(fn,"assert")) && t->n == 1) pld_mark_clause_arg(t->c[0]);
        else if ((!strcmp(fn,"retract")||!strcmp(fn,"retractall")) && t->n == 1) pld_mark_clause_arg(t->c[0]);
        else if (!strcmp(fn,"abolish") && t->n == 1) pld_mark_spec(t->c[0]);
        else if (!strcmp(fn,"dynamic") && t->n == 1) pld_mark_spec(t->c[0]);
    }
    for (int i = 0; i < t->n; i++) pld_mark_scan(t->c[i], mark_assertz);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CODE_t *prolog_lower(PlProgram *pl_prog) {
    for (PlClause *mcl = pl_prog->head; mcl; mcl = mcl->next) if (mcl->tr) pld_mark_scan(mcl->tr, 0);
    CODE_t *prog = calloc(1, sizeof(CODE_t));
    tree_t *pld_seed[256]; int pld_seed_n = 0;
    #define PL_MAX_CLAUSES 2048
    char plunit_suite[PL_MAX_CLAUSES][64];
    {
        char cur_suite[64] = "";
        int ci = 0;
        for (PlClause *cl = pl_prog->head; cl && ci < PL_MAX_CLAUSES; cl = cl->next, ci++) {
            plunit_suite[ci][0] = '\0';
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
                            if (sn) strncpy(cur_suite, sn, 63);
                        } else if (strcmp(d->v.sval, "end_tests") == 0) {
                            cur_suite[0] = '\0';
                        }
                }
            } else if (is_rule && cur_suite[0]) {
                strncpy(plunit_suite[ci], cur_suite, 63);
            }
        }
    }
    #define MAX_PREDS 512
    PredKey  keys[MAX_PREDS];
    tree_t  *choices[MAX_PREDS];
    int      nkeys = 0;
    int      clause_idx = 0;
    for (PlClause *cl = pl_prog->head; cl; cl = cl->next, clause_idx++) {
        int is_rule = (cl->tr != NULL && cl->tr->n > 0 &&
                       cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL);
        if (!is_rule) continue;
        PredKey k = key_of_head_tree(cl->tr->c[0]);
        if (k.functor < 0) continue;
        if (clause_idx < PL_MAX_CLAUSES && plunit_suite[clause_idx][0] != '\0') {
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
                                    cm->v.sval = strdup(",");
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
                            body_tr->v.sval = strdup("true");
                        }
                        tree_t *name_tr = tr_dup(name_src);
                        tree_t *opts_tr = opts_src ? tr_dup(opts_src) : NULL;
                        if (!opts_tr) {
                            opts_tr = ast_node_new(TT_QLIT);
                            opts_tr->v.sval = strdup("[]");
                        }
                        tree_t *suite_tr = ast_node_new(TT_QLIT);
                        suite_tr->v.sval = strdup(plunit_suite[clause_idx]);
                        tree_t *pj_head = ast_node_new(TT_FNC);
                        pj_head->v.sval = strdup("pj_test");
                        expr_add_child(pj_head, suite_tr);
                        expr_add_child(pj_head, name_tr);
                        expr_add_child(pj_head, opts_tr);
                        expr_add_child(pj_head, body_tr);
                        tree_t *syn = ast_node_new(TT_CLAUSE);
                        expr_add_child(syn, pj_head);
                        PredKey pk2 = { prolog_atom_intern("pj_test"), 4 };
                        int found = -1;
                        for (int i = 0; i < nkeys; i++)
                            if (pred_key_eq(keys[i], pk2)) { found = i; break; }
                        if (found < 0 && nkeys < MAX_PREDS) {
                            keys[nkeys] = pk2;
                            choices[nkeys] = ast_node_new(TT_CHOICE);
                            choices[nkeys]->v.sval = pred_str(pk2.functor, pk2.arity);
                            found = nkeys++;
                        }
                        if (found >= 0) {
                            tree_t *ec = lower_clause_from_tree(syn, pk2, 0);
                            expr_add_child(choices[found], ec);
                        }
                    }
                }
            }
        }
        int found = -1;
        for (int i = 0; i < nkeys; i++)
            if (pred_key_eq(keys[i], k)) { found = i; break; }
        if (found < 0) {
            if (nkeys >= MAX_PREDS) {
                fprintf(stderr, "prolog_lower: too many predicates\n");
                continue;
            }
            keys[nkeys] = k;
            choices[nkeys] = ast_node_new(TT_CHOICE);
            choices[nkeys]->v.sval = pred_str(k.functor, k.arity);
            found = nkeys++;
        }
        tree_t *ec = lower_clause_from_tree(cl->tr, k, cl->is_dcg);
        expr_add_child(choices[found], ec);
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
                    if (aknm && pl_dyn_is_marked(aknm, ak.arity) && pld_seed_n < 256) pld_seed[pld_seed_n++] = tr_dup(goal_tr);
                    int found = -1;
                    for (int i = 0; i < nkeys; i++)
                        if (pred_key_eq(keys[i], ak)) { found = i; break; }
                    if (found < 0 && nkeys < MAX_PREDS) {
                        keys[nkeys] = ak;
                        choices[nkeys] = ast_node_new(TT_CHOICE);
                        choices[nkeys]->v.sval = pred_str(ak.functor, ak.arity);
                        found = nkeys++;
                    }
                    if (found >= 0) {
                        tree_t *ec = lower_clause_from_tree(syn, ak, 0);
                        if (prepend) {
                            expr_add_child(choices[found], ec);
                            for (int j = choices[found]->n - 1; j > 0; j--)
                                choices[found]->c[j] = choices[found]->c[j - 1];
                            choices[found]->c[0] = ec;
                        } else {
                            expr_add_child(choices[found], ec);
                        }
                        is_assert = 1;
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
                helper_head->v.sval = strdup(hname);
                tree_t *syn = ast_node_new(TT_FNC);
                syn->v.sval = strdup(":-");
                syn->n = 0;
                expr_add_child(syn, helper_head);
                expr_add_child(syn, goal_tr);
                if (nkeys < MAX_PREDS) {
                    keys[nkeys] = hk;
                    choices[nkeys] = ast_node_new(TT_CHOICE);
                    choices[nkeys]->v.sval = pred_str(hk.functor, hk.arity);
                    tree_t *ec = lower_clause_from_tree(syn, hk, 0);
                    expr_add_child(choices[nkeys], ec);
                    nkeys++;
                    tree_t *init_arg = ast_node_new(TT_QLIT);
                    init_arg->v.sval = strdup(hname);
                    tree_t *init_call = ast_node_new(TT_FNC);
                    init_call->v.sval = strdup("initialization");
                    init_call->n = 0;
                    expr_add_child(init_call, init_arg);
                    goal_tr = init_call;
                }
            }
        }
        int is_export = 0;
        if (goal_tr && goal_tr->t == TT_FNC && goal_tr->v.sval &&
            strcmp(goal_tr->v.sval, "export") == 0 && goal_tr->n == 1) {
            is_export = 1;
            tree_t *arg = goal_tr->c[0];
            const char *ename = NULL;
            if (arg && arg->t == TT_FNC && arg->v.sval &&
                strcmp(arg->v.sval, "/") == 0 && arg->n >= 1 &&
                arg->c[0] && arg->c[0]->t == TT_QLIT) {
                ename = arg->c[0]->v.sval;
            } else if (arg && arg->t == TT_QLIT) {
                ename = arg->v.sval;
            }
            if (ename) {
                ExportEntry *e = calloc(1, sizeof *e);
                e->name = strdup(ename);
                e->next = prog->exports;
                prog->exports = e;
            }
        }
        if (!is_export) {
            STMT_t *s = stmt_new();
            s->subject = goal_tr;
            s->lineno  = cl->lineno;
            if (!prog->head) prog->head = s;
            else             prog->tail->next = s;
            prog->tail = s;
            prog->nstmts++;
        }
    }
    if (pld_seed_n > 0) {
        for (int i = 0; i < nkeys; i++) {
            const char *kn = prolog_atom_name(keys[i].functor);
            if (!kn || strcmp(kn, "main") || keys[i].arity != 0) continue;
            tree_t *mchoice = choices[i];
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
    for (int i = 0; i < nkeys; i++) {
        STMT_t *s = stmt_new();
        s->subject = choices[i];
        s->lineno  = 0;
        if (!prog->head) prog->head = s;
        else             prog->tail->next = s;
        prog->tail = s;
        prog->nstmts++;
    }
    return prog;
}
