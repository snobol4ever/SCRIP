#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include <stdlib.h>
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static const char *pas_ov_argtype(tree_t *e);
long long pas_decl_part_order(long long prev, long long cur) {
    const char *nm[] = { "", "label-declaration-part", "constant-definition-part", "type-definition-part", "variable-declaration-part", "procedure-and-function-declaration-part" };
    if (g_lower_pas.sem.pas_dialect_fpc) return cur > prev ? cur : prev;
    if (cur < prev) {
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block's %s shall precede its %s\n", nm[cur], nm[prev]);
        g_lower_pas.sem.pas_iso_errors++;
    } else if (cur == prev && cur < 5) {
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block has more than one %s\n", nm[cur]);
        g_lower_pas.sem.pas_iso_errors++;
    }
    return cur > prev ? cur : prev;
}
const char *pas_ov_psig(const tree_t *n) {
    const char *s = "";
    const tree_t *ps = (n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS) ? n->c[1] : NULL;
    for (int i = 0; ps && i < ps->n; i++) {
        const tree_t *d = ps->c[i];
        if (!d || d->t != TT_DECL || d->n < 1 || !d->c[0]) continue;
        const tree_t *ty = d->n > 1 ? d->c[1] : NULL;
        const char *k = (d->v.sval && (!strcmp(d->v.sval, "var") || !strcmp(d->v.sval, "out"))) ? "r:" : "v:";
        const char *tn = (ty && ty->t == TT_VAR && ty->v.sval) ? pas_pf_canon(ty->v.sval) : "?";
        for (int j = 0; j < d->c[0]->n; j++) s = pas_pf_cat3(s, pas_pf_cat3(k, tn, ";"), NULL);
    }
    const tree_t *rt = (n->t == TT_FUNCTION && n->n > 2 && ps) ? n->c[2] : (n->t == TT_FUNCTION && n->n > 1 && !ps) ? n->c[1] : NULL;
    return (rt && rt->t == TT_VAR && rt->v.sval) ? pas_pf_cat3(s, ">", pas_pf_canon(rt->v.sval)) : s;
}
void pas_ov_tag(const char *psig) { if (g_lower_pas.sem.pas_scope.n > 0) g_lower_pas.sem.pas_scope.defs[g_lower_pas.sem.pas_scope.n - 1].ov = psig; }
static int pas_ov_nparams(const char *psig) { int k = 0; for (const char *p = psig; p && *p && *p != '>'; p++) k += *p == ';'; return k; }
static const char *pas_ov_ptype(const char *psig, int idx, int *byref) {
    const char *p = psig;
    for (int k = 0; p && *p && *p != '>'; k++) {
        const char *e = strchr(p, ';');
        if (!e) break;
        if (k == idx) { *byref = p[0] == 'r'; char *t = (char *)ct_zalloc(1, (size_t)(e - p)); memcpy(t, p + 2, (size_t)(e - p - 2)); return t; }
        p = e + 1;
    }
    return NULL;
}
const char *pas_overload_name(const char *src, const char *psig) {
    if (!src || !g_lower_pas.sem.pas_dialect_fpc) return pas_shadow_name(src);
    int nd = 0;
    const PasDef *same = NULL;
    for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) {
        const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        if (d->depth < g_lower_pas.sem.pas_scope.depth) break;
        if (!d->name || d->fld || !d->rid || !d->ov || strcmp(d->name, src)) continue;
        nd++;
        if (!same && (!strcmp(d->ov, psig) || (!*psig && pas_scope_fwd_of(d->alias ? d->alias : d->name, NULL)))) same = d;
    }
    if (nd && same) return same->alias ? same->alias : src;
    const char *r = pas_shadow_name(src);
    if (nd) {
        size_t sz = strlen(src) + 24;
        char buf[sz];
        snprintf(buf, sz, "%s$%d", src, ++g_lower_pas.sem.pas_routine_alias_n);
        r = ct_strdup(buf);
        PasDef *a = pas_scope_push(&g_lower_pas.sem.pas_scope.defs, &g_lower_pas.sem.pas_scope.n, &g_lower_pas.sem.pas_scope.cap, src);
        a->alias = r;
        a->rid = 1;
    }
    if (r != src) pas_ov_tag(psig);
    return r;
}
static int pas_ov_realrank(const char *t) {
    return !strcmp(t, "single") ? 0 : !strcmp(t, "extended") ? 2 : (!strcmp(t, "double") || !strcmp(t, "real") || !strcmp(t, "comp") || !strcmp(t, "currency")) ? 1 : -1;
}
static int pas_ov_unsigned(const char *t) {
    return !strcmp(t, "byte") || !strcmp(t, "word") || !strcmp(t, "cardinal") || !strcmp(t, "longword") || !strcmp(t, "qword") || !strcmp(t, "uint8") || !strcmp(t, "uint16") || !strcmp(t, "uint32") ||
        !strcmp(t, "uint64") || !strcmp(t, "nativeuint") || !strcmp(t, "sizeuint") || !strcmp(t, "ptruint");
}
static int pas_ov_width(const char *t) {
    if (!strcmp(t, "integer")) return g_lower_pas.sem.pas_int16 ? 16 : 32;
    if (!strcmp(t, "byte") || !strcmp(t, "shortint") || !strcmp(t, "int8") || !strcmp(t, "uint8")) return 8;
    if (!strcmp(t, "word") || !strcmp(t, "smallint") || !strcmp(t, "int16") || !strcmp(t, "uint16")) return 16;
    if (!strcmp(t, "int64") || !strcmp(t, "qword") || !strcmp(t, "uint64") || !strcmp(t, "nativeint") || !strcmp(t, "nativeuint") || !strcmp(t, "sizeint") || !strcmp(t, "sizeuint") ||
        !strcmp(t, "ptrint") || !strcmp(t, "ptruint")) return 64;
    return 32;
}
static const char *pas_ov_litreal(double d) {
    size_t sz = 40;
    char buf[sz];
    if ((double)(float)d == d) return "single";
    for (int p = 1; p <= 17; p++) { snprintf(buf, sz, "%.*g", p, d); if (strtod(buf, NULL) == d) return strtold(buf, NULL) == (long double)d ? "double" : "extended"; }
    return "extended";
}
static int pas_ov_isint(const char *t) { return t && t[0] != '$' && pas_typename_class(t) == 'i'; }
static const char *pas_ov_arith(tree_t *e) {
    const char *a = pas_ov_argtype(e->c[0]), *b = e->n > 1 ? pas_ov_argtype(e->c[1]) : a;
    if (!a || !b) return NULL;
    int ra = pas_ov_realrank(a), rb = pas_ov_realrank(b);
    if (ra < 0 && rb < 0) return pas_ov_isint(a) && pas_ov_isint(b) ? "int64" : NULL;
    int la = e->c[0]->t == TT_FLIT, lb = e->n > 1 && e->c[1]->t == TT_FLIT;
    if (ra >= 0 && rb >= 0) { if (la != lb) return la ? b : a; return ra >= rb ? a : b; }
    return (ra >= 0 ? la : lb) ? "double" : ra >= 0 ? a : b;
}
static const char *pas_ov_argtype(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_ILIT) return (e->v.ival > 2147483647LL || e->v.ival < -2147483648LL) ? "int64" : "longint";
    if (e->t == TT_FLIT) return pas_ov_litreal(e->v.dval);
    if (e->t == TT_QLIT) return (e->v.sval && strlen(e->v.sval) == 1) ? "$char" : "$str";
    if (e->t == TT_LT || e->t == TT_LE || e->t == TT_GT || e->t == TT_GE || e->t == TT_EQ || e->t == TT_NE || e->t == TT_CONJ || e->t == TT_ALT) return "boolean";
    if (e->t == TT_VAR && e->v.sval) {
        if (!strcmp(e->v.sval, "true") || !strcmp(e->v.sval, "false")) return "boolean";
        const char *t = pas_scalarvartype_get(e->v.sval);
        if (t) return pas_pf_canon(t);
    }
    if (pas_is_stringexpr(e)) return "$str";
    if (pas_is_charexpr(e)) return "$char";
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_abs") || !strcmp(e->c[0]->v.sval, "__pas_sqr")))
        return pas_ov_argtype(e->c[1]);
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
        const PasDef *fd = pas_pf_lookup(e->c[0]->v.sval);
        const char *rp = (fd && fd->sig) ? strrchr(fd->sig, ')') : NULL;
        if (rp && rp[1] == ':') return rp + 2;
    }
    if ((e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV || e->t == TT_MOD || e->t == TT_IDIV) && e->n == 2) return pas_ov_arith(e);
    if ((e->t == TT_MNS || e->t == TT_PLS) && e->n == 1) return pas_ov_arith(e);
    if (pas_expr_is_real(e)) return "double";
    { const pty_t *_pt = pty_of_low(e); if (_pt && _pt->k == PTK_INT && _pt->name && _pt->name[0] != '$') return _pt->name; }
    { const pty_t *_pr = pty_of_low(e); if (_pr && _pr->k == PTK_REAL && pty_real_rank(_pr) == 2) return "extended"; }
    return NULL;
}
static int pas_ov_cost(const char *a, const char *p) {
    if (!a) return 30;
    if (!strcmp(a, p)) return 0;
    int lit = a[0] == '$';
    char ca = lit ? (a[1] == 'c' ? 'c' : 's') : !strcmp(a, "currency") ? 'r' : pas_string_type_kind(a) ? 's' : pas_typename_class(a);
    char cp = !strcmp(p, "currency") ? 'r' : pas_string_type_kind(p) ? 's' : pas_typename_class(p);
    if (!ca || !cp) return -1;
    if (ca == 'i' && cp == 'i') {
        int wa = pas_ov_width(a), wp = pas_ov_width(p), sa = pas_ov_unsigned(a), sp = pas_ov_unsigned(p), d = wa > wp ? wa - wp : wp - wa;
        if (wa == wp && sa == sp) return 1;
        if (wp > wa && (sa == sp || sa)) return 2 + d / 8;
        return 12 + d / 8 + (sa != sp ? 4 : 0);
    }
    if (ca == 'i' && cp == 'r') return 40 + pas_ov_realrank(p);
    if (ca == 'r' && cp == 'r') {
        if (!strcmp(a, "currency")) return !strcmp(p, "extended") ? 1 : !strcmp(p, "double") || !strcmp(p, "real") ? 2 : 3;
        int d = pas_ov_realrank(a) - pas_ov_realrank(p);
        return 1 + (d < 0 ? -d : d);
    }
    if (ca == 'c' && cp == 'c') return 1;
    if (ca == 'c' && cp == 's') return 3;
    if (ca == 's' && cp == 's') return 1;
    if (ca == 'b' && cp == 'b') return 1;
    return -1;
}
const char *pas_overload_pick(const char *src, PNodeList *args, int *np_out) {
    if (!src || !g_lower_pas.sem.pas_dialect_fpc) return NULL;
    int na = args ? args->count / 2 : 0, nc = 0, best = -1, bestc = 0, bestnp = 0, firstnp = 0;
    const char *bestn = NULL, *firstn = NULL;
    int depth = -1;
    for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) {
        const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        if (!d->name || d->fld || !d->rid || !d->ov || strcmp(d->name, src)) continue;
        if (depth < 0) depth = d->depth;
        if (d->depth != depth) break;
        nc++;
    }
    if (nc < 2) return NULL;
    for (int k = 0; k < na; k++) if (!args->items[2 * k + 1] || args->items[2 * k + 1]->t != TT_ILIT || args->items[2 * k + 1]->v.ival != -1) return NULL;
    for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) {
        const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        if (!d->name || d->fld || !d->rid || !d->ov || strcmp(d->name, src) || d->depth != depth) continue;
        int np = pas_ov_nparams(d->ov), ok = na <= np, cost = 0;
        for (int k = na; ok && k < np; k++) ok = pas_default_get(src, k) != NULL;
        if (!ok) continue;
        for (int k = 0; k < na && cost >= 0; k++) {
            int byref = 0;
            const char *pt = pas_ov_ptype(d->ov, k, &byref);
            int c = pt ? pas_ov_cost(pas_ov_argtype(args->items[2 * k]), pt) : 30;
            cost = c < 0 ? -1 : cost + c;
        }
        const char *nm = d->alias ? d->alias : d->name;
        firstn = nm;
        firstnp = np;
        if (cost >= 0 && (best < 0 || cost <= bestc)) { best = 1; bestc = cost; bestn = nm; bestnp = np; }
    }
    *np_out = bestn ? bestnp : firstnp;
    return bestn ? bestn : firstn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const pty_t *pas_formal_type(const char *name, int idx) {
    idx = name ? pas_pf_decl_index(name, idx) : idx;
    if (idx < 0) return NULL;
    if (name && idx >= 0 && g_lower_pas.sem.pas_dialect_fpc) for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) {
        const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        int sib = 0;
        if (!d->name || d->fld || !d->rid || !d->ov || strcmp(d->alias ? d->alias : d->name, name)) continue;
        for (int j = 0; j < g_lower_pas.sem.pas_scope.n; j++) {
            const PasDef *o = &g_lower_pas.sem.pas_scope.defs[j];
            if (o != d && o->name && !o->fld && o->rid && o->ov && o->depth == d->depth && !strcmp(o->name, d->name)) sib = 1;
        }
        if (sib) { int byref = 0; const char *tn = pas_ov_ptype(d->ov, idx, &byref); const pty_t *t = tn ? pty_named_type(tn) : NULL; if (t) return t; }
        break;
    }
    return pty_formal_type(name, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *pas_sem_proc_at(const tree_t *root, int k) {
    if (!root || k < 0 || k >= root->n) return NULL;
    const tree_t *w = root->c[k];
    if (!w) return NULL;
    if (w->t == TT_PROC_DECL) return w;
    for (int i = 0; i < w->n; i++) {
        const tree_t *a = w->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n > 0 && a->c[0] && a->c[0]->t == TT_PROC_DECL) return a->c[0];
    }
    return NULL;
}
static const char *pas_sem_name(const tree_t *p) { if (!p) return NULL; if (p->v.sval) return p->v.sval; return (p->n > 0 && p->c[0]) ? p->c[0]->v.sval : NULL; }
static const tree_t *pas_sem_locals(const tree_t *p) { return (p && p->n > 0) ? p->c[p->n - 1] : NULL; }
static const tree_t *pas_sem_params(const tree_t *p) { return (p && p->n > 1) ? p->c[1] : NULL; }
static const tree_t *pas_sem_body(const tree_t *p) { return (p && p->n > 2) ? p->c[2] : NULL; }
static int pas_sem_level(const tree_t *p) { const tree_t *l = pas_sem_locals(p); return l ? (int)l->v.ival : 0; }
static int pas_sem_vlist_has(const tree_t *vl, const char *name) {
    if (!vl || !name) return 0;
    for (int i = 0; i < vl->n; i++) if (vl->c[i] && vl->c[i]->v.sval && !strcmp(vl->c[i]->v.sval, name)) return 1;
    return 0;
}
static int pas_sem_shadows(const tree_t *p, const char *name) { return pas_sem_vlist_has(pas_sem_params(p), name) || pas_sem_vlist_has(pas_sem_locals(p), name); }
static const tree_t *pas_sem_find_proc(const tree_t *root, const char *name) {
    if (!root || !name) return NULL;
    for (int i = 0; i < root->n; i++) { const tree_t *p = pas_sem_proc_at(root, i); if (p) { const char *pn = pas_sem_name(p); if (pn && !strcmp(pn, name)) return p; } }
    return NULL;
}
static int pas_sem_threatens(const tree_t *root, const tree_t *node, const char *cv, const tree_t *self_for) {
    if (!node || !cv) return 0;
    if (node->t == TT_ASSIGN && node->n > 0 && node->c[0] && node->c[0]->t == TT_VAR && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FOR && node != self_for && node->n > 0 && node->c[0] && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FNC && node->n > 1 && node->c[0] && node->c[0]->v.sval) {
        const tree_t *callee = pas_sem_find_proc(root, node->c[0]->v.sval);
        if (callee) {
            const tree_t *pl = pas_sem_params(callee);
            long long mask = pl ? pl->v.ival : 0;
            for (int a = 1; a < node->n; a++) {
                int idx = a - 1;
                if (idx < 64 && (mask & (1LL << idx)) && node->c[a] && node->c[a]->t == TT_VAR && node->c[a]->v.sval && !strcmp(node->c[a]->v.sval, cv)) return node->line ? node->line : 1;
            }
        }
    }
    for (int i = 0; i < node->n; i++) { int r = pas_sem_threatens(root, node->c[i], cv, self_for); if (r) return r; }
    return 0;
}
static int pas_sem_subtree_start(const tree_t *root, int k) {
    int L = pas_sem_level(pas_sem_proc_at(root, k));
    int j = k - 1;
    while (j >= 0 && pas_sem_proc_at(root, j) && pas_sem_level(pas_sem_proc_at(root, j)) > L) j--;
    return j + 1;
}
static int pas_sem_threat_nested(const tree_t *root, int k, const char *cv) {
    int start = pas_sem_subtree_start(root, k);
    int j = k - 1;
    while (j >= start) {
        const tree_t *p = pas_sem_proc_at(root, j);
        int sub = pas_sem_subtree_start(root, j);
        if (p && pas_sem_level(p) <= pas_sem_level(pas_sem_proc_at(root, k))) break;
        if (p && pas_sem_shadows(p, cv)) { j = sub - 1; continue; }
        if (p) { int r = pas_sem_threatens(root, pas_sem_body(p), cv, NULL); if (r) return r; }
        j--;
    }
    return 0;
}
static void pas_sem_walk(const tree_t *root, int k, const tree_t *node, const char *fname, int *nerr) {
    if (!node) return;
    if (node->t == TT_FOR && node->n > 0 && node->c[0] && node->c[0]->v.sval && strncmp(node->c[0]->v.sval, "__pas_", 6) != 0) {
        const char *cv = node->c[0]->v.sval;
        const tree_t *b = pas_sem_proc_at(root, k);
        int lvl = pas_sem_level(b);
        int iso = g_lower_pas.sem.pas_mode_iso || !g_lower_pas.sem.pas_seen_mode_directive;
        if (iso && lvl >= 1 && !pas_sem_vlist_has(pas_sem_locals(b), cv)) {
            fprintf(stderr,
                "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is not declared in the"
                " variable-declaration-part of the block closest-containing the for-statement\n", fname, node->line, cv);
            (*nerr)++;
        }
        int t = (iso && node->n > 3) ? pas_sem_threatens(root, node->c[3], cv, node) : 0;
        if (!t && iso) t = pas_sem_threat_nested(root, k, cv);
        if (t) {
            const tree_t *_bp = pas_sem_proc_at(root, k);
            const char *_pn = (_bp && _bp->v.sval) ? _bp->v.sval : "(main)";
            fprintf(stderr,
                "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is threatened at line %d by the"
                " block closest-containing the for-statement (assigned to, passed as a variable parameter, or reused as a control-variable) -- in procedure %s\n", fname, node->line, cv, t, _pn);
            (*nerr)++;
        }
    }
    for (int i = 0; i < node->n; i++) pas_sem_walk(root, k, node->c[i], fname, nerr);
}
static int pas_sem_is_seq(const tree_t *n) { return n->t == TT_PROGRAM || n->t == TT_SEQ_EXPR || n->t == TT_LABEL_DEF; }
static int pas_sem_label_at(const tree_t *node, const char *lab, const pas_sem_step_t *gp, int glen, int len, int m, int root_only) {
    if (!node) return 0;
    if (node->t == TT_LABEL_DEF && node->v.sval && !strcmp(node->v.sval, lab)) return (root_only ? len == 0 : len == m) ? 1 : 2;
    for (int i = 0; i < node->n; i++) {
        int r;
        if (pas_sem_is_seq(node)) r = pas_sem_label_at(node->c[i], lab, gp, glen, len, m, root_only);
        else if (m == len && len < glen && gp[len].n == node && gp[len].i == i) r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, len + 1, root_only);
        else r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, m, root_only);
        if (r) return r;
    }
    return 0;
}
static int pas_sem_parent(const tree_t *root, int j) {
    int L = pas_sem_level(pas_sem_proc_at(root, j));
    for (int k = j + 1; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p && pas_sem_level(p) < L) return k; }
    return -1;
}
static void pas_sem_goto_check(const tree_t *root, int k, const tree_t *g, const pas_sem_step_t *gp, int glen, const char *fname, int *nerr) {
    const char *lab = g->v.sval;
    const char *pn = pas_sem_name(pas_sem_proc_at(root, k));
    int r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, k)), lab, gp, glen, 0, 0, 0);
    if (r == 2) {
        fprintf(stderr,
            "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s enters a structured statement -- label %s prefixes a statement outside every"
            " statement-sequence that contains the goto (in %s)\n", fname, g->line, lab, lab, pn ? pn : "main");
        (*nerr)++;
        return;
    }
    if (r == 1) return;
    for (int a = pas_sem_parent(root, k); a >= 0; a = pas_sem_parent(root, a)) {
        r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, a)), lab, gp, glen, 0, 0, 1);
        if (r == 1) return;
        if (r == 2) {
            const char *an = pas_sem_name(pas_sem_proc_at(root, a));
            fprintf(stderr,
                "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s leaves %s for label %s of the enclosing block %s, which does not prefix a statement"
                " of that block's outermost statement-sequence\n", fname, g->line, lab, pn ? pn : "main", lab, an ? an : "main");
            (*nerr)++;
            return;
        }
    }
    fprintf(stderr, "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s names a label that prefixes no statement of its block or of any enclosing block (in %s)\n", fname, g->line, lab,
        pn ? pn : "main");
    (*nerr)++;
}
static int pas_sem_depth(const tree_t *node) { int d = 0; for (int i = 0; node && i < node->n; i++) { int c = pas_sem_depth(node->c[i]); if (c > d) d = c; } return d + 1; }
static void pas_sem_goto_walk(const tree_t *root, int k, const tree_t *node, pas_sem_step_t *gp, int len, const char *fname, int *nerr) {
    if (!node) return;
    if (node->t == TT_GOTO_U && node->v.sval) { pas_sem_goto_check(root, k, node, gp, len, fname, nerr); return; }
    for (int i = 0; i < node->n; i++) {
        if (pas_sem_is_seq(node)) { pas_sem_goto_walk(root, k, node->c[i], gp, len, fname, nerr); continue; }
        gp[len].n = node;
        gp[len].i = i;
        pas_sem_goto_walk(root, k, node->c[i], gp, len + 1, fname, nerr);
    }
}
int pascal_sem_check(const tree_t *root, const char *filename) {
    if (!root) return 0;
    int nerr = 0;
    const char *fname = filename ? filename : "<stdin>";
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p) pas_sem_walk(root, k, pas_sem_body(p), fname, &nerr); }
    for (int k = 0; k < root->n; k++) {
        const tree_t *p = pas_sem_proc_at(root, k);
        if (!p) continue;
        pas_sem_step_t gp[pas_sem_depth(pas_sem_body(p)) + 1];
        pas_sem_goto_walk(root, k, pas_sem_body(p), gp, 0, fname, &nerr);
    }
    return nerr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_tree_has_event(const tree_t *t, const char *ev) {
    if (!t) return 0;
    if (t->t == TT_ATTR && t->v.sval && !strcmp(t->v.sval, ":pragma") && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, ev)) return 1;
    for (int i = 0; i < t->n; i++) if (pas_tree_has_event(t->c[i], ev)) return 1;
    return 0;
}
void pas_iso_empty_strings(const tree_t *t) {
    if (!t) return;
    if (t->t == TT_ATTR && t->v.sval && !strcmp(t->v.sval, ":pragma") && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strncmp(t->c[0]->v.sval, "iso_6_1_7 ", 10)) {
        fprintf(stderr, "pascal: ISO 7185 6.1.7 violation line %d: a character-string shall contain at least one string-element -- '' is not one\n", atoi(t->c[0]->v.sval + 10));
        g_lower_pas.sem.pas_iso_errors++;
    }
    for (int i = 0; i < t->n; i++) pas_iso_empty_strings(t->c[i]);
}
static int pas_sign_over(const tree_t *x) {
    if (!x || x->slen == -7) return 0;
    if (x->t == TT_CONJ && x->n == 2) return 1;
    return x->t == TT_FNC && x->n == 3 && x->c[0] && x->c[0]->v.sval && (!strcasecmp(x->c[0]->v.sval, "shl") || !strcasecmp(x->c[0]->v.sval, "shr"));
}
static tree_t *pas_sign_into(tree_t *s, tree_t *x) {
    int i = x->t == TT_CONJ ? 0 : 1;
    if (pas_sign_over(x->c[i])) x->c[i] = pas_sign_into(s, x->c[i]);
    else { s->c[0] = x->c[i]; x->c[i] = s; }
    return x;
}
tree_t *pas_sign_binds_factor(tree_t *t) {
    if (!t) return t;
    for (int i = 0; i < t->n; i++) t->c[i] = pas_sign_binds_factor(t->c[i]);
    if ((t->t == TT_MNS || t->t == TT_PLS) && t->n == 1 && pas_sign_over(t->c[0])) return pas_sign_into(t, t->c[0]);
    return t;
}
void pas_apply_event(const char *ev) {
    if (!strcmp(ev, "iso_error")) g_lower_pas.sem.pas_iso_errors++;
    else if (!strcmp(ev, "mode_iso")) g_lower_pas.sem.pas_mode_iso = 1;
    else if (!strcmp(ev, "mode_noniso")) g_lower_pas.sem.pas_mode_iso = 0;
    else if (!strcmp(ev, "int16")) g_lower_pas.sem.pas_int16 = 1;
    else if (!strcmp(ev, "seen_mode")) g_lower_pas.sem.pas_seen_mode_directive = 1;
    else if (!strncmp(ev, "fnamecall ", 10)) g_lower_pas.sem.pas_fname_is_call = atoi(ev + 10);
    else if (!strncmp(ev, "minenum ", 8)) g_lower_pas.sem.pas_min_enum_size = atoi(ev + 8);
    else if (!strncmp(ev, "packset ", 8)) g_lower_pas.sem.pas_pack_set_size = atoi(ev + 8);
    else if (!strncmp(ev, "packrec ", 8)) g_lower_pas.sem.pas_pack_records = atoi(ev + 8);
    else if (!strncmp(ev, "range ", 6)) g_lower_pas.sem.pas_range_check_on = atoi(ev + 6);
    else if (!strncmp(ev, "iocheck ", 8)) g_lower_pas.sem.pas_ioquiet = ev[8] == '0';
    else if (!strncmp(ev, "zerobased ", 10)) {
        if (ev[10] == '1') g_lower_pas.sem.pas_zerobased_strings |= 1;
        else g_lower_pas.sem.pas_zerobased_strings &= ~1;
    } else if (!strncmp(ev, "overflow ", 9)) {
        if (ev[9] == '1') g_lower_pas.sem.pas_zerobased_strings |= 2;
        else g_lower_pas.sem.pas_zerobased_strings &= ~2;
    } else if (!strncmp(ev, "align ", 6)) g_lower_pas.sem.pas_align_mac68k = (ev[6] == '1');
    else if (!strncmp(ev, "codepage ", 9)) g_lower_pas.sem.pas_codepage = atoi(ev + 9);
    else if (!strcmp(ev, "push")) {
        if (g_lower_pas.sem.pas_min_enum_stack_n >= g_lower_pas.sem.pas_pragma_cap) {
            g_lower_pas.sem.pas_pragma_cap = g_lower_pas.sem.pas_pragma_cap ? g_lower_pas.sem.pas_pragma_cap * 2 : 16;
            g_lower_pas.sem.pas_pragma_stk = ct_grow(g_lower_pas.sem.pas_pragma_stk, (size_t)g_lower_pas.sem.pas_pragma_cap * sizeof *g_lower_pas.sem.pas_pragma_stk);
        }
        g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].en = g_lower_pas.sem.pas_min_enum_size;
        g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].set = g_lower_pas.sem.pas_pack_set_size;
        g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].rec = g_lower_pas.sem.pas_pack_records;
        g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].zb = g_lower_pas.sem.pas_zerobased_strings;
        g_lower_pas.sem.pas_min_enum_stack_n++;
    } else if (!strcmp(ev, "pop")) {
        if (g_lower_pas.sem.pas_min_enum_stack_n > 0) {
            g_lower_pas.sem.pas_min_enum_stack_n--;
            g_lower_pas.sem.pas_min_enum_size = g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].en;
            g_lower_pas.sem.pas_pack_set_size = g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].set;
            g_lower_pas.sem.pas_pack_records = g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].rec;
            g_lower_pas.sem.pas_zerobased_strings = g_lower_pas.sem.pas_pragma_stk[g_lower_pas.sem.pas_min_enum_stack_n].zb;
        }
    }
}
static int pas_result_declared(const tree_t *t) {
    if (!t) return 0;
    if (IS(t, TT_DECL) && t->n > 0 && t->c[0] && IS(t->c[0], TT_VAR) && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "result")) return 1;
    if (IS(t, TT_VLIST)) { for (int i = 0; i < t->n; i++) if (t->c[i] && IS(t->c[i], TT_VAR) && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "result")) return 1; }
    if ((IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION)) && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "result")) return 1;
    for (int i = 0; i < t->n; i++) if (pas_result_declared(t->c[i])) return 1;
    return 0;
}
static int pas_result_used(const tree_t *t, int in_routine) {
    if (!t) return 0;
    if (in_routine && IS(t, TT_VAR) && t->v.sval && !strcmp(t->v.sval, "result")) return 1;
    int inside = in_routine || IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION);
    for (int i = 0; i < t->n; i++) if (pas_result_used(t->c[i], inside)) return 1;
    return 0;
}
static int pas_fname_read_in(const tree_t *t, const char *fn, int mode) {
    if (!t || IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION)) return 0;
    if (IS(t, TT_VAR)) return mode == 0 && t->v.sval && !strcmp(t->v.sval, fn);
    if (IS(t, TT_FIELD)) return t->n > 0 && pas_fname_read_in(t->c[0], fn, mode == 1 ? 1 : 0);
    for (int i = 0; i < t->n; i++) {
        int m = 0;
        if (i == 0 && IS(t, TT_ASSIGN)) m = 1;
        else if (i == 0 && IS(t, TT_FNC)) m = 2;
        else if (i == 0 && IS(t, TT_IDX)) m = mode == 1 ? 1 : 0;
        if (pas_fname_read_in(t->c[i], fn, m)) return 1;
    }
    return 0;
}
static int pas_fname_read_as_variable(const tree_t *t) {
    if (!t) return 0;
    if (IS(t, TT_FUNCTION) && t->n >= 3 && t->c[0] && t->c[0]->v.sval) {
        int has_params = 0;
        for (int i = 1; i < t->n; i++) if (IS(t->c[i], TT_PARAMS) && t->c[i]->n > 0) has_params = 1;
        if (has_params && IS(t->c[t->n - 1], TT_BLOCK) && pas_fname_read_in(t->c[t->n - 1], t->c[0]->v.sval, 0)) return 1;
    }
    for (int i = 0; i < t->n; i++) if (pas_fname_read_as_variable(t->c[i])) return 1;
    return 0;
}
int pas_result_flips_dialect(const tree_t *root) { return !pas_tree_has_event(root, "mode_iso") && ((pas_result_used(root, 0) && !pas_result_declared(root)) || pas_fname_read_as_variable(root)); }
static void pas_default_add(const char *routine, int idx, tree_t *expr) {
    if (!routine || !expr) return;
    if (g_lower_pas.sem.pas_ndefault >= g_lower_pas.sem.pas_default_cap) {
        g_lower_pas.sem.pas_default_cap = g_lower_pas.sem.pas_default_cap ? g_lower_pas.sem.pas_default_cap * 2 : 16;
        g_lower_pas.sem.pas_defaults = ct_grow(g_lower_pas.sem.pas_defaults, (size_t)g_lower_pas.sem.pas_default_cap * sizeof *g_lower_pas.sem.pas_defaults);
    }
    g_lower_pas.sem.pas_defaults[g_lower_pas.sem.pas_ndefault].routine = ct_strdup(routine);
    g_lower_pas.sem.pas_defaults[g_lower_pas.sem.pas_ndefault].idx = idx;
    g_lower_pas.sem.pas_defaults[g_lower_pas.sem.pas_ndefault].expr = expr;
    g_lower_pas.sem.pas_ndefault++;
}
tree_t *pas_default_get(const char *routine, int idx) {
    for (int i = g_lower_pas.sem.pas_ndefault - 1; routine && i >= 0; i--) if (g_lower_pas.sem.pas_defaults[i].idx == idx && !strcmp(g_lower_pas.sem.pas_defaults[i].routine, routine))
        return g_lower_pas.sem.pas_defaults[i].expr;
    return NULL;
}
PNodeList *pas_defaults_extend(const char *routine, PNodeList *args) {
    int nact = args ? args->count / 2 : 0;
    for (int idx = nact; pas_default_get(routine, idx); idx++) {
        if (!args) args = pnl_new();
        pnl_push(args, E_expression(pas_tree_clone(pas_default_get(routine, idx))).node);
        pnl_push(args, ilit(-1));
    }
    return args;
}
void pas_defaults_prepass(tree_t *t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) pas_defaults_prepass(t->c[i]);
    if (!(IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION)) || t->n < 2 || !t->c[0] || !t->c[0]->v.sval) return;
    tree_t *ps = NULL;
    for (int i = 1; i < t->n; i++) if (IS(t->c[i], TT_PARAMS)) { ps = t->c[i]; break; }
    if (!ps) return;
    int idx = 0;
    for (int i = 0; i < ps->n; i++) {
        tree_t *d = ps->c[i];
        if (!IS(d, TT_DECL)) continue;
        int names = (d->n > 0 && d->c[0] && d->c[0]->n > 0 && d->v.sval && strcmp(d->v.sval, "procedure") && strcmp(d->v.sval, "function")) ? d->c[0]->n : 1;
        if (d->n >= 3 && d->v.sval && (!strcmp(d->v.sval, "value") || !strcmp(d->v.sval, "const"))) { for (int k = 0; k < names; k++) pas_default_add(t->c[0]->v.sval, idx + k, d->c[2]); d->n = 2; }
        idx += names;
    }
}
void pas_param_prepass(tree_t *t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) pas_param_prepass(t->c[i]);
    if (!IS(t, TT_PARAMS)) return;
    for (int i = 0; i < t->n; i++) {
        tree_t *d = t->c[i];
        if (!IS(d, TT_DECL) || !d->v.sval) continue;
        if (!strcmp(d->v.sval, "const")) d->v.sval = "value";
        if (d->n >= 2 && IS(d->c[1], TT_ARRAY_TYPE) && d->c[1]->n == 1 && d->c[1]->c[0] && d->c[1]->c[0]->v.sval) {
            size_t ln = strlen(d->c[1]->c[0]->v.sval) + 8;
            char *tn = (char *)ct_zalloc(ln, 1);
            snprintf(tn, ln, "$open$%s", d->c[1]->c[0]->v.sval);
            d->c[1] = leaf_s(TT_VAR, tn);
        }
    }
}
