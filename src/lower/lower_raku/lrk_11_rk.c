#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_11_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_clone_tree(const tree_t * t) {
    if (!t) return NULL;
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, rk_clone_tree(t->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_class_default_tweaks(tree_t * prog) {
    for (int i = 0; i < prog->n; i++) {
        tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); d = (tree_t *) sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        tree_t * seq = ast_node_new(TT_SEQ_EXPR);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || !ch->v.sval || ch->n < 1 || !ch->c[0]) continue;
            int sig = ch->t == TT_ARR_DECL ? '@' : ch->t == TT_HASH_DECL ? '%' :
                (ch->t == TT_HAS_DECL && ch->c[0]->t != TT_ILIT && ch->c[0]->t != TT_QLIT && !(ch->c[0]->t == TT_FLIT && ch->c[0]->n == 0)) ? '$' : 0;
            if (!sig) continue;
            const char * fn = rk_fld_bare(ch->v.sval);
            if (!*fn) continue;
            tree_t * rhs = rk_clone_tree(ch->c[0]);
            if (sig != '$') {
                tree_t * w = ast_node_new(TT_FNC);
                w->v.sval = (char *) (sig == '@' ? "__rk_to_array" : "__rk_to_hash");
                ast_push(w, leaf_sval2(TT_VAR, w->v.sval));
                ast_push(w, rhs);
                rhs = w;
            }
            tree_t * f1 = ast_node_new(TT_TWIGIL_FIELD), * f2 = ast_node_new(TT_TWIGIL_FIELD);
            f1->v.sval = f2->v.sval = (char *) fn;
            tree_t * cond;
            if (sig == '$') {
                cond = ast_node_new(TT_METHCALL);
                ast_push(cond, f1);
                ast_push(cond, leaf_sval2(TT_QLIT, "defined"));
            } else {
                cond = ast_node_new(TT_FNC);
                cond->v.sval = (char *) "__rk_unset";
                ast_push(cond, leaf_sval2(TT_VAR, "__rk_unset"));
                ast_push(cond, f1);
            }
            tree_t * asg = ast_node_new(TT_ASSIGN);
            ast_push(asg, f2);
            ast_push(asg, rhs);
            tree_t * body = ast_node_new(TT_SEQ_EXPR);
            ast_push(body, asg);
            tree_t * br = ast_node_new(sig == '$' ? TT_UNLESS : TT_IF);
            ast_push(br, cond);
            ast_push(br, body);
            ast_push(seq, br);
        }
        if (seq->n == 0) continue;
        tree_t * tw = NULL;
        for (int j = 1; j < d->n && !tw; j++) { tree_t * ch = d->c[j]; if (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0] && ch->c[0]->v.sval && !strcmp(ch->c[0]->v.sval, "TWEAK")) tw = ch; }
        if (!tw) { tw = ast_node_new(TT_SUB_DECL); tw->v.ival = 1; ast_push(tw, leaf_sval2(TT_VAR, "TWEAK")); for (int k = 0; k < seq->n; k++) ast_push(tw, seq->c[k]); ast_push(d, tw); continue; }
        int at = (int) tw->v.ival;
        if (at < 1) at = 1;
        if (at > tw->n) at = tw->n;
        int nold = tw->n - at;
        tree_t ** old = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (nold + 1));
        for (int k = 0; k < nold; k++) old[k] = tw->c[at + k];
        tw->n = at;
        for (int k = 0; k < seq->n; k++) ast_push(tw, seq->c[k]);
        for (int k = 0; k < nold; k++) ast_push(tw, old[k]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_nested_elem_sets(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_nested_elem_sets(t->c[i]);
    while ((t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n == 3 && t->c[0] && (t->c[0]->t == TT_ARR_GET || t->c[0]->t == TT_HASH_GET) && t->c[0]->n >= 2) {
        tree_t * g = t->c[0];
        tree_t * put = ast_node_new(TT_FNC);
        put->v.sval = (char *) "__rk_elem_put";
        ast_push(put, leaf_sval2(TT_VAR, "__rk_elem_put"));
        ast_push(put, leaf_sval2(TT_QLIT, t->t == TT_ARR_SET ? "a" : "h"));
        ast_push(put, rk_clone_tree(g));
        ast_push(put, t->c[1]);
        ast_push(put, t->c[2]);
        t->t = g->t == TT_ARR_GET ? TT_ARR_SET : TT_HASH_SET;
        t->c[0] = g->c[0];
        t->c[1] = g->c[1];
        t->c[2] = put;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_elem_store_value(tree_t * st) {
    if (!st || st->t != TT_ARR_SET || st->n != 3 || !st->c[0] || st->c[0]->t != TT_VAR || !st->c[1]) return st;
    tree_t * ix = st->c[1];
    if (ix->t != TT_ILIT && ix->t != TT_QLIT && !(ix->t == TT_VAR && ix->n == 0)) return st;
    tree_t * get = ast_node_new(TT_ARR_GET);
    get->line = st->line;
    ast_push(get, rk_clone_tree(st->c[0]));
    ast_push(get, rk_clone_tree(ix));
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = st->line;
    ast_push(seq, st);
    ast_push(seq, get);
    return seq;
}
void rk_elem_store_values(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_elem_store_values(t->c[i]);
    int from = t->t == TT_METHCALL ? 2 : t->t == TT_FNC || t->t == TT_INVOKE ? 1 : t->t == TT_SAY || t->t == TT_PRINT || t->t == TT_RETURN ? 0 : t->t == TT_ASSIGN ? 1 : -1;
    if (from < 0) return;
    for (int i = from; i < t->n; i++) t->c[i] = rk_elem_store_value(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_scalar_name(const tree_t * v) {
    return v && v->t == TT_VAR && v->v.sval && !(v->slen & 1) && v->v.sval[0] != '@' && v->v.sval[0] != '%' && v->v.sval[0] != '&' && strncmp(v->v.sval, "__", 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_scalar_bind(const tree_t * t) { return t && t->t == TT_ASSIGN && t->v.ival == 2 && t->n > 1 && rk_is_scalar_name(t->c[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_fnc_of(const char * fn, int line) { tree_t * f = ast_node_new(TT_FNC); f->line = line; f->v.sval = (char *) fn; ast_push(f, leaf_sval2(TT_VAR, fn)); return f; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_bind_ref(tree_t * r) {
    if (rk_is_scalar_name(r) && r->n == 0) { r->slen |= RK_VF_RAW; return r; }
    int el = r && r->t == TT_ARR_GET && r->n >= 2;
    tree_t * f = rk_fnc_of(el ? "__rk_elem_ref" : "__rk_box", r ? r->line : 0);
    if (el) { ast_push(f, r->c[0]); ast_push(f, r->c[1]); } else ast_push(f, r);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_bind_refs(tree_t * t, int in_blk) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_bind_refs(t->c[i], in_blk || (t->c[i] && t->c[i]->t == TT_ANON_BLOCK));
    int ebind = t->t == TT_ARR_SET && t->v.ival == 2 && t->n == 3;
    if (in_blk) { if (rk_is_scalar_bind(t) || ebind) t->v.ival = 0; return; }
    if (rk_is_scalar_bind(t)) { t->c[0]->slen |= RK_VF_RAW; t->c[1] = rk_bind_ref(t->c[1]); return; }
    if (ebind) {
        tree_t * c0 = t->c[0], * c1 = t->c[1], * c2 = rk_bind_ref(t->c[2]);
        tree_t * f = rk_fnc_of("__rk_elem_bind", t->line);
        ast_push(f, c0);
        ast_push(f, c1);
        ast_push(f, c2);
        t->n = 0;
        if (c0->t == TT_VAR && c0->v.sval && c0->v.sval[0] == '@') {
            t->t = TT_ASSIGN;
            t->v.ival = 2;
            ast_push(t, rk_clone_tree(c0));
            ast_push(t, f);
        } else {
            t->t = TT_SEQ_EXPR;
            t->v.ival = 0;
            ast_push(t, f);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_bind_unbox(tree_t * t, const rk_ns_t * Bx) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_bind_unbox(t->c[i], Bx);
    if (t->t == TT_VAR && (t->slen & RK_VF_RAW)) t->slen &= ~RK_VF_BOXED;
    if (rk_is_scalar_bind(t) && !rk_ns_has(Bx, t->c[0]->v.sval)) { tree_t * d = rk_fnc_of("__rk_deref", t->line); ast_push(d, t->c[1]); t->c[1] = d; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rw_shadows(const tree_t * t, const char * v) {
    if (!t) return 0;
    if (t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return 1;
    if (t->t == TT_EVERY && t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) { const char * iv = t->c[0]->v.sval ? t->c[0]->v.sval : "_"; return !strcmp(iv, v); }
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->v.sval) return !strcmp(t->c[0]->v.sval, v);
    return t->t == TT_CASE && !strcmp(v, "_");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rw_assigns(const tree_t * t, const char * v) {
    if (!t || rk_rw_shadows(t, v)) return 0;
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, v)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_rw_assigns(t->c[i], v)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_write_back(tree_t * t, const char * v, const char * arr, const char * k) {
    if (!t || rk_rw_shadows(t, v)) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_rw_write_back(c, v, arr, k);
        if (!(c && c->t == TT_ASSIGN && c->n > 1 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, v))) continue;
        tree_t * st = ast_node_new(TT_ARR_SET);
        st->line = c->line;
        ast_push(st, leaf_sval2(TT_VAR, arr));
        ast_push(st, leaf_sval2(TT_VAR, k));
        ast_push(st, leaf_sval2(TT_VAR, v));
        tree_t * sq = ast_node_new(TT_SEQ_EXPR);
        sq->line = c->line;
        ast_push(sq, c);
        ast_push(sq, st);
        ast_push(sq, leaf_sval2(TT_VAR, v));
        t->c[i] = sq;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rw_loops(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_rw_loops(t->c[i], seq);
    if (t->t != TT_EVERY || t->n != 2 || !t->c[0] || t->c[0]->t != TT_ITERATE || t->c[0]->n != 1) return;
    tree_t * it = t->c[0], * src = it->c[0], * body = t->c[1];
    if (!src || src->t != TT_VAR || !src->v.sval || src->v.sval[0] != '@' || (it->slen & RK_PV_COPY)) return;
    const char * v = it->v.sval ? it->v.sval : "_";
    int asg = 0;
    for (int i = 0; body && i < body->n; i++) if (rk_rw_assigns(body->c[i], v)) asg = 1;
    if (body && body->t == TT_ASSIGN && rk_rw_assigns(body, v)) asg = 1;
    if (!asg) return;
    char kb[32];
    snprintf(kb, sizeof kb, "__rwk_%d", (*seq)++);
    const char * k = intern(kb);
    rk_rw_write_back(body, v, src->v.sval, k);
    tree_t * inc = ast_node_new(TT_ASSIGN);
    ast_push(inc, leaf_sval2(TT_VAR, k));
    tree_t * add = ast_node_new(TT_ADD);
    ast_push(add, leaf_sval2(TT_VAR, k));
    tree_t * one = ast_node_new(TT_ILIT);
    one->v.ival = 1;
    ast_push(add, one);
    ast_push(inc, add);
    tree_t * nb = ast_node_new(TT_SEQ_EXPR);
    nb->line = t->line;
    ast_push(nb, inc);
    ast_push(nb, body);
    t->c[1] = nb;
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = t->line;
    ev->slen = t->slen;
    ast_push(ev, it);
    ast_push(ev, nb);
    tree_t * init = ast_node_new(TT_ASSIGN);
    ast_push(init, leaf_sval2(TT_VAR, k));
    tree_t * m1 = ast_node_new(TT_ILIT);
    m1->v.ival = -1;
    ast_push(init, m1);
    t->t = TT_SEQ_EXPR;
    t->n = 0;
    ast_push(t, init);
    ast_push(t, ev);
}
void rk_rw_subs(const tree_t * t, rk_rwmap_t * m) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strchr(t->c[0]->v.sval, '$') && t->v.ival > 0 && t->v.ival < 64) {
        unsigned long long mk = 0;
        int np = (int) t->v.ival;
        for (int k = 0; k < np && 1 + k < t->n; k++) if (rk_param_is_rw(t->c[1 + k])) mk |= 1ULL << k;
        if (mk) {
            if (m->n >= m->cap) {
                m->cap = m->cap ? m->cap * 2 : 8;
                m->nm = (const char **) ct_grow((void *) m->nm, sizeof(const char *) * (size_t) m->cap);
                m->mask = (unsigned long long *) ct_grow((void *) m->mask, sizeof(unsigned long long) * (size_t) m->cap);
                m->np = (int *) ct_grow((void *) m->np, sizeof(int) * (size_t) m->cap);
            }
            m->nm[m->n] = t->c[0]->v.sval;
            m->mask[m->n] = mk;
            m->np[m->n++] = np;
        }
    }
    for (int i = 0; i < t->n; i++) rk_rw_subs(t->c[i], m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rw_calls(tree_t * t, const rk_rwmap_t * m, int in_blk) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_rw_calls(t->c[i], m, in_blk || (t->c[i] && t->c[i]->t == TT_ANON_BLOCK));
    if (!in_blk && t->t == TT_METHCALL && t->n >= 3 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval) {
        for (int j = 0; j < m->n; j++) {
            if (strcmp(m->nm[j], t->c[1]->v.sval)) continue;
            for (int k = 0; k < m->np[j] && 2 + k < t->n; k++) {
                if (!(m->mask[j] & (1ULL << k))) continue;
                tree_t * a = t->c[2 + k];
                if (rk_is_scalar_name(a) && a->n == 0) a->slen |= RK_VF_RAW;
                else if (a && a->t == TT_ARR_GET && a->n >= 2) t->c[2 + k] = rk_bind_ref(a);
            }
        }
        return;
    }
    if (in_blk || t->t != TT_FNC || !t->v.sval) return;
    for (int j = 0; j < m->n; j++) {
        if (strcmp(m->nm[j], t->v.sval) || t->n - 1 != m->np[j]) continue;
        for (int k = 0; k < m->np[j]; k++) {
            if (!(m->mask[j] & (1ULL << k))) continue;
            tree_t * a = t->c[1 + k];
            if (rk_is_scalar_name(a) && a->n == 0) a->slen |= RK_VF_RAW;
            else if (a && a->t == TT_ARR_GET && a->n >= 2) t->c[1 + k] = rk_bind_ref(a);
        }
        return;
    }
}
static tree_t * rk_formal_var(tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return pv && pv->t == TT_VAR && pv->v.sval ? pv : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_method_deref_one(tree_t * sd, const rk_rwmap_t * m) {
    if (!sd->n || !sd->c[0] || !sd->c[0]->v.sval) return;
    int np = (int) sd->v.ival - 1, added = 0;
    for (int j = 0; j < m->n; j++) {
        if (strcmp(m->nm[j], sd->c[0]->v.sval)) continue;
        for (int k = 0; k < np && 1 + k < sd->n; k++) {
            tree_t * pv = rk_formal_var(sd->c[1 + k]);
            if (!(m->mask[j] & (1ULL << k)) || !pv || rk_param_is_rw(pv) || (pv->slen & RK_PV_RAW) || pv->v.sval[0] == '@' || pv->v.sval[0] == '%' || pv->v.sval[0] == '&') continue;
            tree_t * d = rk_fnc_of("__rk_deref", sd->line);
            ast_push(d, leaf_sval2(TT_VAR, pv->v.sval));
            tree_t * as = ast_node_new(TT_ASSIGN);
            as->line = sd->line;
            ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
            ast_push(as, d);
            ast_push(sd, as);
            for (int i = sd->n - 1; i > 1 + np + added; i--) sd->c[i] = sd->c[i - 1];
            sd->c[1 + np + added] = as;
            added++;
        }
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rw_method_deref(tree_t * t, const rk_rwmap_t * m) {
    if (!t) return;
    int pkg = t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL;
    for (int i = 0; i < t->n; i++) { if (pkg && t->c[i] && t->c[i]->t == TT_SUB_DECL) rk_rw_method_deref_one(t->c[i], m); rk_rw_method_deref(t->c[i], m); }
}
static const char * rk_rw_tail_attr(const tree_t * sd) {
    const tree_t * c = sd->n > 1 ? sd->c[sd->n - 1] : NULL;
    if (c && c->t == TT_RETURN && c->n == 1) c = c->c[0];
    return c && c->t == TT_TWIGIL_FIELD && c->v.sval ? c->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_add_setter(tree_t * cls, const char * m, const char * attr) {
    char nm[256];
    snprintf(nm, sizeof nm, "__rwset_%s", m);
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->line = cls->line;
    sd->v.ival = 2;
    ast_push(sd, leaf_sval2(TT_VAR, intern(nm)));
    ast_push(sd, leaf_sval2(TT_VAR, "__rwv"));
    tree_t * as = ast_node_new(TT_ASSIGN);
    as->line = cls->line;
    ast_push(as, leaf_sval2(TT_TWIGIL_FIELD, attr));
    ast_push(as, leaf_sval2(TT_VAR, "__rwv"));
    ast_push(sd, as);
    ast_push(cls, sd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_collect(tree_t * t, rk_ns_t * names) {
    if (!t) return;
    if (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL) for (int i = 0; i < t->n; i++) {
        const tree_t * sd = t->c[i];
        if (sd && sd->t == TT_SUB_DECL && (sd->slen & RK_PV_RW) && sd->n && sd->c[0] && sd->c[0]->v.sval && rk_rw_tail_attr(sd)) rk_ns_add(names, sd->c[0]->v.sval);
    }
    for (int i = 0; i < t->n; i++) rk_rw_collect(t->c[i], names);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_gen_setters(tree_t * t, const rk_ns_t * names) {
    if (!t) return;
    if (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL) {
        int n0 = t->n;
        for (int i = 0; i < n0; i++) {
            const tree_t * sd = t->c[i];
            if (sd && sd->t == TT_SUB_DECL && (sd->slen & RK_PV_RW) && sd->n && sd->c[0] && sd->c[0]->v.sval && rk_rw_tail_attr(sd)) rk_rw_add_setter(t, sd->c[0]->v.sval, rk_rw_tail_attr(sd));
            else if (sd && sd->t == TT_RW_DECL && sd->v.sval && sd->v.sval[0] == '.' && rk_ns_has(names, sd->v.sval + 1)) rk_rw_add_setter(t, sd->v.sval + 1, sd->v.sval + 1);
        }
    }
    for (int i = 0; i < t->n; i++) rk_rw_gen_setters(t->c[i], names);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_assign_calls(tree_t * t, const rk_ns_t * names) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_rw_assign_calls(t->c[i], names);
    if (t->t != TT_ASSIGN || t->n < 2 || !t->c[0] || t->c[0]->t != TT_FIELD || t->c[0]->n < 1 || !t->c[0]->v.sval || !rk_ns_has(names, t->c[0]->v.sval)) return;
    char nm[256];
    snprintf(nm, sizeof nm, "__rwset_%s", t->c[0]->v.sval);
    tree_t * obj = t->c[0]->c[0], * rhs = t->c[1];
    t->t = TT_METHCALL;
    t->v.ival = 0;
    t->n = 0;
    ast_push(t, obj);
    ast_push(t, leaf_sval2(TT_QLIT, intern(nm)));
    ast_push(t, rhs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rw_method_setters(tree_t * prog) { rk_ns_t names = { 0 }; rk_rw_collect(prog, &names); if (!names.n) return; rk_rw_gen_setters(prog, &names); rk_rw_assign_calls(prog, &names); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_user_sub_named(const tree_t * prog, const char * nm) {
    size_t l = strlen(nm);
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (sub) d = sub; }
        const char * dn = (d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0]) ? d->c[0]->v.sval : NULL;
        if (dn && !strncmp(dn, nm, l) && (dn[l] == 0 || dn[l] == '$')) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_minmax_fns(tree_t * t, const tree_t * prog, int umax, int umin) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_minmax_fns(t->c[i], prog, umax, umin);
    if (t->t != TT_FNC || !t->v.sval || t->n < 2 || !t->c[0] || t->c[0]->t != TT_VAR) return;
    for (int i = 1; t->n > 2 && i < t->n; i++) {
        const tree_t * a = t->c[i];
        if (a && ((a->t == TT_VAR && a->v.sval && a->v.sval[0] == '@') || (a->t == TT_FNC && a->v.sval && !strncmp(a->v.sval, "__rk_arr", 8)))) return;
    }
    int mx = !strcmp(t->v.sval, "max"), mn = !strcmp(t->v.sval, "min");
    if ((!mx || umax) && (!mn || umin)) return;
    const char * nn = mx ? "__rk_arr_max" : "__rk_arr_min";
    t->v.sval = (char *) nn;
    t->c[0]->v.sval = (char *) nn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_ss_call(const char * nm, tree_t * arg) {
    char b[strlen(nm) + 8];
    snprintf(b, sizeof b, "__ssf_%s", nm);
    tree_t * f = rk_fnc_of(intern(b), arg ? arg->line : 0);
    ast_push(f, arg);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_param_guard(const char * ty, const char * pn, int line) {
    tree_t * chk = rk_fnc_of("__param_check", line);
    ast_push(chk, leaf_sval2(TT_QLIT, ty));
    ast_push(chk, leaf_sval2(TT_VAR, pn));
    ast_push(chk, leaf_sval2(TT_QLIT, pn));
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, chk);
    tree_t * u = ast_node_new(TT_UNLESS);
    u->line = line;
    ast_push(u, rk_ss_call(ty, leaf_sval2(TT_VAR, pn)));
    ast_push(u, blk);
    return u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_take_where(tree_t * pv) {
    for (int x = 0; x < pv->n; x++) {
        tree_t * c = pv->c[x];
        if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_where") && c->n >= 2) {
            tree_t * wx = c->c[1];
            for (int y = x + 1; y < pv->n; y++) pv->c[y - 1] = pv->c[y];
            pv->n--;
            return wx;
        }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ss_rewrite(tree_t * t, const rk_ns_t * ss, int in_pkg) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_ss_rewrite(t->c[i], ss, t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL);
    if (t->t == TT_FNC && t->n == 3 && t->v.sval && !strcmp(t->v.sval, "__rk_smartmatch") && t->c[2] && t->c[2]->t == TT_VAR && t->c[2]->n == 0 && t->c[2]->v.sval && rk_ns_has(ss, t->c[2]->v.sval)) {
        char b[strlen(t->c[2]->v.sval) + 8];
        snprintf(b, sizeof b, "__ssf_%s", t->c[2]->v.sval);
        const char * fn = intern(b);
        tree_t * arg = t->c[1];
        t->v.sval = (char *) fn;
        t->n = 0;
        ast_push(t, leaf_sval2(TT_VAR, fn));
        ast_push(t, arg);
        return;
    }
    if (t->t == TT_METHCALL && t->n == 3 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && (!strcmp(t->c[1]->v.sval, "does") || !strcmp(t->c[1]->v.sval, "isa")) && t->c[2] &&
        (t->c[2]->t == TT_QLIT || (t->c[2]->t == TT_VAR && t->c[2]->n == 0)) && t->c[2]->v.sval && rk_ns_has(ss, t->c[2]->v.sval)) {
        char b[strlen(t->c[2]->v.sval) + 8];
        snprintf(b, sizeof b, "__ssf_%s", t->c[2]->v.sval);
        const char * fn = intern(b);
        tree_t * arg = t->c[0];
        t->t = TT_FNC;
        t->v.sval = (char *) fn;
        t->n = 0;
        ast_push(t, leaf_sval2(TT_VAR, fn));
        ast_push(t, arg);
        return;
    }
    if (t->t == TT_ANON_BLOCK) { for (int k = 1; k < t->n; k++) if (t->c[k] && t->c[k]->t == TT_VAR) rk_ss_take_where(t->c[k]); return; }
    if (t->t != TT_SUB_DECL || t->n < 1 || !t->c[0]) return;
    int multi = t->c[0]->v.sval && strchr(t->c[0]->v.sval, '$') != NULL;
    int ismeth = in_pkg, first = 1, last = ismeth ? (int) t->v.ival - 1 : (int) t->v.ival, body = last + 1;
    tree_t * guards[last >= first ? 3 * (last - first + 1) : 1];
    int ng = 0;
    for (int k = first; k <= last && k < t->n; k++) {
        tree_t * pv = t->c[k];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
        tree_t * wx = rk_ss_take_where(pv);
        if (multi) continue;
        if (wx && ng < 30) {
            tree_t * as = ast_node_new(TT_ASSIGN);
            as->line = t->line;
            ast_push(as, leaf_sval2(TT_VAR, "_"));
            ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
            guards[ng++] = as;
            tree_t * chk = rk_fnc_of("__param_check", t->line);
            ast_push(chk, leaf_sval2(TT_QLIT, "where"));
            ast_push(chk, leaf_sval2(TT_VAR, pv->v.sval));
            ast_push(chk, leaf_sval2(TT_QLIT, pv->v.sval));
            tree_t * blk = ast_node_new(TT_SEQ_EXPR);
            ast_push(blk, chk);
            tree_t * u = ast_node_new(TT_UNLESS);
            u->line = t->line;
            tree_t * sm = rk_fnc_of("__rk_smartmatch", t->line);
            ast_push(sm, leaf_sval2(TT_VAR, "_"));
            ast_push(sm, wx);
            ast_push(u, sm);
            ast_push(u, blk);
            guards[ng++] = u;
        }
        if (pv->v.sval[0] == '@' || pv->v.sval[0] == '%' || pv->n < 1 || !pv->c[0] || pv->c[0]->t != TT_QLIT || !pv->c[0]->v.sval || !rk_ns_has(ss, pv->c[0]->v.sval) || ng >= 31) continue;
        guards[ng++] = rk_ss_param_guard(pv->c[0]->v.sval, pv->v.sval, t->line);
        for (int x = 1; x < pv->n; x++) pv->c[x - 1] = pv->c[x];
        pv->n--;
    }
    if (!ng) return;
    for (int g = 0; g < ng; g++) { ast_push(t, NULL); for (int y = t->n - 1; y > body + g; y--) t->c[y] = t->c[y - 1]; t->c[body + g] = guards[g]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_ss_base_of(const tree_t * t, const char * name) {
    if (!t) return NULL;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strncmp(t->c[0]->v.sval, "__ssf_", 6) && !strcmp(t->c[0]->v.sval + 6, name)) {
        for (int i = 1; i < t->n; i++) {
            const tree_t * c = t->c[i];
            if (c && c->t == TT_IF && c->n > 0 && c->c[0] && c->c[0]->t == TT_METHCALL && c->c[0]->n == 3 && c->c[0]->c[2] && c->c[0]->c[2]->v.sval) return c->c[0]->c[2]->v.sval;
        }
        return NULL;
    }
    for (int i = 0; i < t->n; i++) { const char * b = rk_ss_base_of(t->c[i], name); if (b) return b; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_rank(const tree_t * root, const rk_ns_t * ss, const char * ty, int depth) {
    if (!ty || !*ty || depth > 6) return 0;
    size_t bl = strcspn(ty, ":");
    char b[bl + 1];
    memcpy(b, ty, bl);
    b[bl] = 0;
    if (!strcmp(b, "Any") || !strcmp(b, "Mu")) return 0;
    if (!strcmp(b, "Cool")) return 1;
    if (!strcmp(b, "Numeric") || !strcmp(b, "Real") || !strcmp(b, "Stringy") || !strcmp(b, "Positional") || !strcmp(b, "Iterable") || !strcmp(b, "Associative") || !strcmp(b, "Callable")) return 2;
    if (rk_ns_has(ss, b)) return 10 + rk_ss_rank(root, ss, rk_ss_base_of(root, b), depth + 1);
    return 3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_ss_where_of(const tree_t * pv) {
    for (int x = 0; x < pv->n; x++) { const tree_t * c = pv->c[x]; if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_where") && c->n >= 2) return c->c[1]; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_if(tree_t * cond, tree_t * inner) {
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, inner);
    tree_t * f = ast_node_new(TT_IF);
    ast_push(f, cond);
    ast_push(f, blk);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_core_type(const char * b) {
    static const char * const core[] = { "Int", "Num", "Str", "Bool", "Rat", "FatRat", "List", "Array", "Hash", "Range", "Numeric", "Real", "Stringy", "Positional", "Iterable", "Seq", "Pair", "Map",
        "Set", "Bag", "Mix", NULL };
    for (int i = 0; core[i]; i++) if (!strcmp(core[i], b)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_cand_chain(const rk_ns_t * ss, const rk_ns_t * classes, tree_t * sd, const char * cname, int c) {
    tree_t * call = rk_fnc_of(cname, sd->line);
    for (int j = 0; j < c; j++) ast_push(call, rk_ss_arg(j));
    tree_t * ret = ast_node_new(TT_RETURN);
    ast_push(ret, call);
    tree_t * inner = ret;
    for (int j = c - 1; j >= 0; j--) {
        tree_t * pv = sd->c[1 + j];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        const tree_t * wx = rk_ss_where_of(pv);
        if (wx) {
            tree_t * as = ast_node_new(TT_ASSIGN);
            ast_push(as, leaf_sval2(TT_VAR, "_"));
            ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
            tree_t * sm = rk_fnc_of("__rk_smartmatch", sd->line);
            ast_push(sm, leaf_sval2(TT_VAR, "_"));
            ast_push(sm, rk_clone_tree(wx));
            tree_t * seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, as);
            ast_push(seq, rk_ss_if(sm, inner));
            inner = seq;
        }
        const char * ty = (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT) ? pv->c[0]->v.sval : NULL;
        if (ty && *ty) {
            size_t bl = strcspn(ty, ":");
            char b[bl + 1];
            memcpy(b, ty, bl);
            b[bl] = 0;
            tree_t * test = NULL;
            if (rk_ns_has(ss, b)) test = rk_ss_call(b, leaf_sval2(TT_VAR, pv->v.sval));
            else if (rk_ss_core_type(b) || rk_ns_has(classes, b)) {
                test = ast_node_new(TT_METHCALL);
                ast_push(test, leaf_sval2(TT_VAR, pv->v.sval));
                ast_push(test, leaf_sval2(TT_QLIT, "does"));
                ast_push(test, leaf_sval2(TT_QLIT, intern(b)));
            }
            if (test) inner = rk_ss_if(test, inner);
        }
    }
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    for (int j = 0; j < c; j++) {
        tree_t * pv = sd->c[1 + j];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        tree_t * as = ast_node_new(TT_ASSIGN);
        ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
        ast_push(as, rk_ss_arg(j));
        ast_push(seq, as);
    }
    ast_push(seq, inner);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_multi_list(tree_t * t, const rk_ns_t * ss, const rk_ns_t * classes, tree_t * root, int * ctr) {
    int n = t->n;
    char * done = (char *) ct_alloc((size_t) n + 1);
    memset(done, 0, (size_t) n + 1);
    for (int i = 0; i < n; i++) {
        tree_t * d0 = rk_top_subj(t->c[i]);
        const char * n0 = (d0 && d0->t == TT_SUB_DECL && d0->n > 0 && d0->c[0]) ? d0->c[0]->v.sval : NULL;
        if (done[i] || !n0 || !strchr(n0, '$')) continue;
        size_t bl = strcspn(n0, "$");
        char base[bl + 1];
        memcpy(base, n0, bl);
        base[bl] = 0;
        int ncmax = n - i > 0 ? n - i : 1;
        rk_mc_t cands[ncmax];
        int idx[ncmax], nc = 0, valid = 1, constrained = 0;
        for (int k = i; k < n && valid; k++) {
            tree_t * dk = rk_top_subj(t->c[k]);
            const char * nk = (dk && dk->t == TT_SUB_DECL && dk->n > 0 && dk->c[0]) ? dk->c[0]->v.sval : NULL;
            if (!nk) continue;
            if (!strchr(nk, '$')) { if (!strcmp(nk, base)) valid = 0; continue; }
            if (strncmp(nk, base, bl) || nk[bl] != '$') continue;
            done[k] = 1;
            if (nc >= ncmax) { valid = 0; break; }
            rk_mc_t * m = &cands[nc];
            m->sd = dk;
            m->rank = 0;
            m->nreq = (int) dk->v.ival;
            m->nmax = (int) dk->v.ival;
            if (dk->v.ival < 0 || dk->v.ival > 12) { valid = 0; break; }
            int seen_opt = 0;
            for (int q = 1; q <= (int) dk->v.ival && q < dk->n; q++) {
                tree_t * pv = dk->c[q];
                if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
                if (!pv || pv->t != TT_VAR || !pv->v.sval || pv->v.sval[0] == '@' || pv->v.sval[0] == '%' || pv->v.sval[0] == '&' || !pv->v.sval[0]) { valid = 0; break; }
                if ((pv->slen & RK_PV_NAMED) || (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && (pv->c[0]->v.sval[0] == '*' || pv->c[0]->v.sval[0] == '+'))) {
                    valid = 0;
                    break;
                }
                if (pv->slen & RK_PV_OPT) { seen_opt = 1; m->nreq--; } else if (seen_opt) { valid = 0; break; }
                const char * ty = (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT) ? pv->c[0]->v.sval : NULL;
                const tree_t * wx = rk_ss_where_of(pv);
                if (wx || (ty && rk_ns_has(ss, ty))) constrained = 1;
                m->rank += rk_ss_rank(root, ss, ty, 0) + (wx ? 5 : 0);
            }
            idx[nc++] = k;
        }
        if (!valid || !constrained || nc == 0) continue;
        for (int a = 1; a < nc; a++) { rk_mc_t key = cands[a]; int b = a - 1; while (b >= 0 && cands[b].rank < key.rank) { cands[b + 1] = cands[b]; b--; } cands[b + 1] = key; }
        char * names[ncmax];
        for (int a = 0; a < nc; a++) { char nb[bl + 24]; snprintf(nb, sizeof nb, "__mc%d_%s", (*ctr)++, base); names[a] = (char *) intern(nb); cands[a].sd->c[0]->v.sval = names[a]; }
        tree_t * disp = ast_node_new(TT_SUB_DECL);
        disp->v.ival = 1;
        disp->line = cands[0].sd->line;
        ast_push(disp, leaf_sval2(TT_VAR, intern(base)));
        tree_t * ap = leaf_sval2(TT_VAR, "@__a");
        ast_push(ap, leaf_sval2(TT_QLIT, "*@"));
        ast_push(disp, ap);
        int cmin = 99, cmax = 0;
        for (int a = 0; a < nc; a++) { if (cands[a].nreq < cmin) cmin = cands[a].nreq; if (cands[a].nmax > cmax) cmax = cands[a].nmax; }
        for (int c = cmin; c <= cmax; c++) {
            tree_t * grp = ast_node_new(TT_SEQ_EXPR);
            int any = 0;
            for (int a = 0; a < nc; a++) if (cands[a].nreq <= c && c <= cands[a].nmax) { ast_push(grp, rk_ss_cand_chain(ss, classes, cands[a].sd, names[a], c)); any = 1; }
            if (!any) continue;
            tree_t * eq = ast_node_new(TT_EQ);
            tree_t * el = ast_node_new(TT_METHCALL);
            ast_push(el, leaf_sval2(TT_VAR, "@__a"));
            ast_push(el, leaf_sval2(TT_QLIT, "elems"));
            tree_t * lit = ast_node_new(TT_ILIT);
            lit->v.ival = c;
            ast_push(eq, el);
            ast_push(eq, lit);
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, eq);
            ast_push(f, grp);
            ast_push(disp, f);
        }
        tree_t * df = ast_node_new(TT_DIE);
        char msg[bl + 64];
        snprintf(msg, sizeof msg, "Cannot resolve caller %s(...); none of these signatures matches", base);
        tree_t * nw = ast_node_new(TT_NEW);
        ast_push(nw, leaf_sval2(TT_QLIT, "X::Multi::NoMatch"));
        ast_push(nw, leaf_sval2(TT_QLIT, "message"));
        ast_push(nw, leaf_sval2(TT_QLIT, intern(msg)));
        ast_push(df, nw);
        ast_push(disp, df);
        tree_t * ins = disp;
        if (t->c[idx[0]]->t == TT_STMT) {
            extern tree_t *ast_stmt_new(tree_e kind);
            extern tree_t *ast_attr_int(const char *tag, int ival);
            extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
            ins = ast_stmt_new(TT_STMT);
            ast_push(ins, ast_attr_int(":line", 0));
            ast_push(ins, ast_attr_int(":stno", 0));
            ast_push(ins, ast_attr_expr(":subj", disp));
        }
        ast_push(t, NULL);
        for (int y = t->n - 1; y > idx[0]; y--) t->c[y] = t->c[y - 1];
        t->c[idx[0]] = ins;
        n = t->n;
        char * nd = (char *) ct_alloc((size_t) n + 1);
        memset(nd, 0, (size_t) n + 1);
        for (int y = 0; y < n - 1 && y < (int) idx[0]; y++) nd[y] = done[y];
        for (int y = idx[0]; y < n - 1; y++) nd[y + 1] = done[y];
        done = nd;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_pr_is_star(const tree_t * e) {
    if (!e || e->t != TT_ANON_BLOCK || e->n != 1 || !e->c[0] || e->c[0]->t != TT_SEQ_EXPR || e->c[0]->n != 1) return 0;
    const tree_t * v = e->c[0]->c[0];
    return v && v->t == TT_VAR && v->v.sval && !strcmp(v->v.sval, "*");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_pr_count_star(const tree_t * t) {
    int n = 0;
    for (int i = 0; t && i < t->n; i++) if (t->c[i] && t->c[i]->t != TT_SUB_DECL) n += rk_pr_is_star(t->c[i]) ? 1 : rk_pr_count_star(t->c[i]);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_pr_swap_star(tree_t * t, const char * nb, int np, const char * const * hv) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c || c->t == TT_SUB_DECL) continue;
        if (!rk_pr_is_star(c)) { rk_pr_swap_star(c, nb, np, hv); continue; }
        tree_t * call = ast_node_new(TT_FNC);
        call->v.sval = (char *) nb;
        ast_push(call, leaf_sval2(TT_VAR, nb));
        for (int q = 0; q < np; q++) ast_push(call, leaf_sval2(TT_VAR, hv[q]));
        t->c[i] = call;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_pr_param_fixed(const tree_t * pv) {
    if (!pv || pv->t != TT_VAR || !pv->v.sval || !pv->v.sval[0] || strchr("@%&", pv->v.sval[0]) || (pv->slen & (RK_PV_NAMED | RK_PV_OPT))) return 0;
    const tree_t * ty = (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT) ? pv->c[0] : NULL;
    return !(ty && ty->v.sval && (ty->v.sval[0] == '*' || ty->v.sval[0] == '+'));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_proto_list(tree_t * t, int * ctr) {
    for (int i = 0; i < t->n; i++) {
        tree_t * pd = rk_top_subj(t->c[i]);
        const char * nm = (pd && pd->t == TT_SUB_DECL && pd->n > 0 && pd->c[0]) ? pd->c[0]->v.sval : NULL;
        if (!nm || !*nm || strchr(nm, '$')) continue;
        int np = (int) pd->v.ival, ok = np >= 0 && np < pd->n, ncand = 0;
        size_t bl = strlen(nm);
        for (int q = 1; ok && q <= np; q++) ok = rk_pr_param_fixed(pd->c[q]);
        for (int k = 0; ok && k < t->n; k++) {
            const tree_t * dk = rk_top_subj(t->c[k]);
            const char * nk = (dk && dk->t == TT_SUB_DECL && dk->n > 0 && dk->c[0]) ? dk->c[0]->v.sval : NULL;
            if (nk && !strncmp(nk, nm, bl) && nk[bl] == '$') ncand++;
        }
        if (!ok || !ncand || !rk_pr_count_star(pd)) continue;
        char nb[bl + 24];
        snprintf(nb, sizeof nb, "__pr%d_%s", (*ctr)++, nm);
        const char * nbi = intern(nb);
        for (int k = 0; k < t->n; k++) {
            tree_t * dk = rk_top_subj(t->c[k]);
            const char * nk = (dk && dk->t == TT_SUB_DECL && dk->n > 0 && dk->c[0]) ? dk->c[0]->v.sval : NULL;
            if (!nk || strncmp(nk, nm, bl) || nk[bl] != '$') continue;
            char rn[strlen(nbi) + strlen(nk + bl) + 1];
            snprintf(rn, sizeof rn, "%s%s", nbi, nk + bl);
            dk->c[0]->v.sval = (char *) intern(rn);
        }
        const char * hv[np > 0 ? np : 1];
        for (int q = 0; q < np; q++) { char hn[sizeof nb + 16]; snprintf(hn, sizeof hn, "__prv%s_%d", nb + 4, q); hv[q] = intern(hn); }
        rk_pr_swap_star(pd, nbi, np, hv);
        for (int q = np - 1; q >= 0; q--) {
            tree_t * a = ast_node_new(TT_ASSIGN);
            ast_push(a, leaf_sval2(TT_VAR, hv[q]));
            ast_push(a, leaf_sval2(TT_VAR, pd->c[q + 1]->v.sval));
            ast_push(pd, a);
            for (int y = pd->n - 1; y > np + 1; y--) pd->c[y] = pd->c[y - 1];
            pd->c[np + 1] = a;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ss_multi(tree_t * t, const rk_ns_t * ss, const rk_ns_t * classes, tree_t * root, int * ctr) {
    if (!t) return;
    if (t->t != TT_CLASS_DECL && t->t != TT_ROLE_DECL && t->t != TT_GRAMMAR_DECL) { rk_ss_proto_list(t, ctr); rk_ss_multi_list(t, ss, classes, root, ctr); }
    for (int i = 0; i < t->n; i++) rk_ss_multi(t->c[i], ss, classes, root, ctr);
}
