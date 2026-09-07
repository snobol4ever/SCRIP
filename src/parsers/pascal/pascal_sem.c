#include "pascal_driver.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
static const tree_t *pas_sem_proc_at(const tree_t *root, int k) {
    if (!root || k < 0 || k >= root->n) return NULL;
    const tree_t *w = root->c[k];
    if (!w) return NULL;
    if (w->t == TT_PROC_DECL) return w;
    for (int i = 0; i < w->n; i++) { const tree_t *a = w->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n > 0 && a->c[0] && a->c[0]->t == TT_PROC_DECL) return a->c[0]; }
    return NULL; }
static const char *pas_sem_name(const tree_t *p) { if (!p) return NULL; if (p->v.sval) return p->v.sval; return (p->n > 0 && p->c[0]) ? p->c[0]->v.sval : NULL; }
static const tree_t *pas_sem_locals(const tree_t *p) { return (p && p->n > 0) ? p->c[p->n - 1] : NULL; }
static const tree_t *pas_sem_params(const tree_t *p) { return (p && p->n > 1) ? p->c[1] : NULL; }
static const tree_t *pas_sem_body(const tree_t *p) { return (p && p->n > 2) ? p->c[2] : NULL; }
static int pas_sem_level(const tree_t *p) { const tree_t *l = pas_sem_locals(p); return l ? (int)l->v.ival : 0; }
static int pas_sem_vlist_has(const tree_t *vl, const char *name) {
    if (!vl || !name) return 0;
    for (int i = 0; i < vl->n; i++) if (vl->c[i] && vl->c[i]->v.sval && !strcmp(vl->c[i]->v.sval, name)) return 1;
    return 0; }
static int pas_sem_shadows(const tree_t *p, const char *name) { return pas_sem_vlist_has(pas_sem_params(p), name) || pas_sem_vlist_has(pas_sem_locals(p), name); }
static const tree_t *pas_sem_find_proc(const tree_t *root, const char *name) {
    if (!root || !name) return NULL;
    for (int i = 0; i < root->n; i++) { const tree_t *p = pas_sem_proc_at(root, i);
        if (p) { const char *pn = pas_sem_name(p); if (pn && !strcmp(pn, name)) return p; } }
    return NULL; }
static int pas_sem_threatens(const tree_t *root, const tree_t *node, const char *cv, const tree_t *self_for) {
    if (!node || !cv) return 0;
    if (node->t == TT_ASSIGN && node->n > 0 && node->c[0] && node->c[0]->t == TT_VAR && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FOR && node != self_for && node->n > 0 && node->c[0] && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FNC && node->n > 1 && node->c[0] && node->c[0]->v.sval) {
        const tree_t *callee = pas_sem_find_proc(root, node->c[0]->v.sval);
        if (callee) { const tree_t *pl = pas_sem_params(callee); long long mask = pl ? pl->v.ival : 0;
            for (int a = 1; a < node->n; a++) { int idx = a - 1;
                if (idx < 64 && (mask & (1LL << idx)) && node->c[a] && node->c[a]->t == TT_VAR
                    && node->c[a]->v.sval && !strcmp(node->c[a]->v.sval, cv)) return node->line ? node->line : 1; } }
    }
    for (int i = 0; i < node->n; i++) { int r = pas_sem_threatens(root, node->c[i], cv, self_for); if (r) return r; }
    return 0;
}
static int pas_sem_subtree_start(const tree_t *root, int k) {
    int L = pas_sem_level(pas_sem_proc_at(root, k)); int j = k - 1;
    while (j >= 0 && pas_sem_proc_at(root, j) && pas_sem_level(pas_sem_proc_at(root, j)) > L) j--;
    return j + 1; }
static int pas_sem_threat_nested(const tree_t *root, int k, const char *cv) {
    int start = pas_sem_subtree_start(root, k); int j = k - 1;
    while (j >= start) { const tree_t *p = pas_sem_proc_at(root, j); int sub = pas_sem_subtree_start(root, j);
        if (p && pas_sem_shadows(p, cv)) { j = sub - 1; continue; }
        if (p) { int r = pas_sem_threatens(root, pas_sem_body(p), cv, NULL); if (r) return r; }
        j--; }
    return 0;
}
static void pas_sem_walk(const tree_t *root, int k, const tree_t *node, const char *fname, int *nerr) {
    if (!node) return;
    if (node->t == TT_FOR && node->n > 0 && node->c[0] && node->c[0]->v.sval) {
        const char *cv = node->c[0]->v.sval; const tree_t *b = pas_sem_proc_at(root, k); int lvl = pas_sem_level(b);
        if (lvl >= 1 && !pas_sem_vlist_has(pas_sem_locals(b), cv) && !pas_sem_vlist_has(pas_sem_params(b), cv)) {
            fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is not declared in the"
                            " variable-declaration-part of the block closest-containing the for-statement\n", fname, node->line, cv);
            (*nerr)++; }
        int t = (node->n > 3) ? pas_sem_threatens(root, node->c[3], cv, node) : 0;
        if (!t) t = pas_sem_threat_nested(root, k, cv);
        if (t) { fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is threatened at line %d by the"
                                 " block closest-containing the for-statement (assigned to, passed as a variable parameter, or reused as a control-variable)\n",
                         fname, node->line, cv, t); (*nerr)++; }
    }
    for (int i = 0; i < node->n; i++) pas_sem_walk(root, k, node->c[i], fname, nerr);
}
int pascal_sem_check(const tree_t *root, const char *filename) {
    if (!root) return 0;
    int nerr = 0; const char *fname = filename ? filename : "<stdin>";
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p) pas_sem_walk(root, k, pas_sem_body(p), fname, &nerr); }
    return nerr;
}
