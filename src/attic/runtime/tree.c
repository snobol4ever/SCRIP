/* ATTIC: dead code excised from src/runtime/tree.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*====================================================================================================================*/
TREEBLK_t *expr_new(const char *tag, DESCR_t val) {
    TREEBLK_t *t = GC_malloc(sizeof(TREEBLK_t));
    t->tag = GC_strdup(tag ? tag : "");
    t->val = val;
    t->n   = 0;
    t->cap = 0;
    t->c   = NULL;
    return t;
}
/*--------------------------------------------------------------------------------------------------------------------*/
TREEBLK_t *tree_new0(const char *tag) {
    return expr_new(tag, NULVCL);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void _tree_ensure_cap(TREEBLK_t *x, int needed) {
    if (x->cap >= needed) return;
    int newcap = x->cap ? x->cap * 2 : 4;
    while (newcap < needed) newcap *= 2;
    TREEBLK_t **nc = GC_malloc(newcap * sizeof(TREEBLK_t *));
    if (x->c) memcpy(nc, x->c, x->n * sizeof(TREEBLK_t *));
    x->c   = nc;
    x->cap = newcap;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void tree_append(TREEBLK_t *x, TREEBLK_t *y) {
    _tree_ensure_cap(x, x->n + 1);
    x->c[x->n++] = y;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void tree_prepend(TREEBLK_t *x, TREEBLK_t *y) {
    _tree_ensure_cap(x, x->n + 1);
    memmove(x->c + 1, x->c, x->n * sizeof(TREEBLK_t *));
    x->c[0] = y;
    x->n++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void tree_insert(TREEBLK_t *x, TREEBLK_t *y, int place) {
    if (place < 1) place = 1;
    if (place > x->n + 1) place = x->n + 1;
    _tree_ensure_cap(x, x->n + 1);
    int idx = place - 1;
    memmove(x->c + idx + 1, x->c + idx, (x->n - idx) * sizeof(TREEBLK_t *));
    x->c[idx] = y;
    x->n++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
TREEBLK_t *tree_remove(TREEBLK_t *x, int place) {
    if (!x || place < 1 || place > x->n) return NULL;
    int idx = place - 1;
    TREEBLK_t *removed = x->c[idx];
    memmove(x->c + idx, x->c + idx + 1, (x->n - idx - 1) * sizeof(TREEBLK_t *));
    x->n--;
    return removed;
}
/* ATTIC (whole-file-dead residual decls) from src/runtime/tree.c */
#include "core.h"
#include "sil_macros.h"
#include <string.h>
