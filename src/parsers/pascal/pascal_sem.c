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
        if (p && pas_sem_level(p) <= pas_sem_level(pas_sem_proc_at(root, k))) break;
        if (p && pas_sem_shadows(p, cv)) { j = sub - 1; continue; }
        if (p) { int r = pas_sem_threatens(root, pas_sem_body(p), cv, NULL); if (r) return r; }
        j--; }
    return 0;
}
static void pas_sem_walk(const tree_t *root, int k, const tree_t *node, const char *fname, int *nerr) {
    if (!node) return;
    if (node->t == TT_FOR && node->n > 0 && node->c[0] && node->c[0]->v.sval && strncmp(node->c[0]->v.sval, "__pas_", 6) != 0) {
        const char *cv = node->c[0]->v.sval; const tree_t *b = pas_sem_proc_at(root, k); int lvl = pas_sem_level(b);
        if (lvl >= 1 && !pas_sem_vlist_has(pas_sem_locals(b), cv) && !pas_sem_vlist_has(pas_sem_params(b), cv)) {
            fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is not declared in the"
                            " variable-declaration-part of the block closest-containing the for-statement\n", fname, node->line, cv);
            (*nerr)++; }
        int t = (node->n > 3) ? pas_sem_threatens(root, node->c[3], cv, node) : 0;
        if (!t) t = pas_sem_threat_nested(root, k, cv);
        if (t) { const tree_t *_bp = pas_sem_proc_at(root, k); const char *_pn = (_bp && _bp->v.sval) ? _bp->v.sval : "(main)";
                 fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is threatened at line %d by the"
                                 " block closest-containing the for-statement (assigned to, passed as a variable parameter, or reused as a control-variable) -- in procedure %s\n",
                         fname, node->line, cv, t, _pn); (*nerr)++; }
    }
    for (int i = 0; i < node->n; i++) pas_sem_walk(root, k, node->c[i], fname, nerr);
}
#define PAS_SEM_PATH_MAX 64
typedef struct { const tree_t *n; int i; } pas_sem_step_t;
static int pas_sem_is_seq(const tree_t *n) { return n->t == TT_PROGRAM || n->t == TT_SEQ_EXPR || n->t == TT_LABEL_DEF; }
static int pas_sem_label_at(const tree_t *node, const char *lab, const pas_sem_step_t *gp, int glen, int len, int m, int sib, int root_only) {
    if (!node) return 0;
    if (node->t == TT_LABEL_DEF && node->v.sval && !strcmp(node->v.sval, lab)) return (root_only ? len == 0 : (len == m || (len == m + 1 && sib))) ? 1 : 2;
    for (int i = 0; i < node->n; i++) { int r;
        if (pas_sem_is_seq(node)) r = pas_sem_label_at(node->c[i], lab, gp, glen, len, m, sib, root_only);
        else if (m != len) r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, m, sib, root_only);
        else if (len < glen && gp[len].n == node && gp[len].i == i) r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, len + 1, 0, root_only);
        else r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, len, len < glen && gp[len].n == node, root_only);
        if (r) return r; }
    return 0;
}
static int pas_sem_parent(const tree_t *root, int j) {
    int L = pas_sem_level(pas_sem_proc_at(root, j));
    for (int k = j + 1; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p && pas_sem_level(p) < L) return k; }
    return -1;
}
static void pas_sem_goto_check(const tree_t *root, int k, const tree_t *g, const pas_sem_step_t *gp, int glen, const char *fname, int *nerr) {
    const char *lab = g->v.sval; const char *pn = pas_sem_name(pas_sem_proc_at(root, k));
    int r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, k)), lab, gp, glen, 0, 0, 0, 0);
    if (r == 2) { fprintf(stderr, "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s enters a structured statement -- label %s prefixes a statement outside every"
                                  " statement-sequence that contains the goto (in %s)\n", fname, g->line, lab, lab, pn ? pn : "main"); (*nerr)++; return; }
    if (r == 1) return;
    for (int a = pas_sem_parent(root, k); a >= 0; a = pas_sem_parent(root, a)) {
        r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, a)), lab, gp, glen, 0, 0, 0, 1);
        if (r == 1) return;
        if (r == 2) { const char *an = pas_sem_name(pas_sem_proc_at(root, a));
            fprintf(stderr, "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s leaves %s for label %s of the enclosing block %s, which does not prefix a statement"
                            " of that block's outermost statement-sequence\n", fname, g->line, lab, pn ? pn : "main", lab, an ? an : "main"); (*nerr)++; return; } }
    fprintf(stderr, "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s names a label that prefixes no statement of its block or of any enclosing block (in %s)\n",
            fname, g->line, lab, pn ? pn : "main"); (*nerr)++;
}
static void pas_sem_goto_walk(const tree_t *root, int k, const tree_t *node, pas_sem_step_t *gp, int len, const char *fname, int *nerr) {
    if (!node || len >= PAS_SEM_PATH_MAX) return;
    if (node->t == TT_GOTO_U && node->v.sval) { pas_sem_goto_check(root, k, node, gp, len, fname, nerr); return; }
    for (int i = 0; i < node->n; i++) {
        if (pas_sem_is_seq(node)) { pas_sem_goto_walk(root, k, node->c[i], gp, len, fname, nerr); continue; }
        gp[len].n = node; gp[len].i = i; pas_sem_goto_walk(root, k, node->c[i], gp, len + 1, fname, nerr); }
}
int pascal_sem_check(const tree_t *root, const char *filename) {
    if (!root) return 0;
    int nerr = 0; const char *fname = filename ? filename : "<stdin>";
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p) pas_sem_walk(root, k, pas_sem_body(p), fname, &nerr); }
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); pas_sem_step_t gp[PAS_SEM_PATH_MAX]; if (p) pas_sem_goto_walk(root, k, pas_sem_body(p), gp, 0, fname, &nerr); }
    return nerr;
}
