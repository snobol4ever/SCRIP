#ifndef TREE_BLK_H
#define TREE_BLK_H
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _TREEBLK_t { char *tag; DESCR_t val; int n; int cap; struct _TREEBLK_t **c; } TREEBLK_t;
TREEBLK_t *expr_new(const char *tag, DESCR_t val);
TREEBLK_t *tree_new0(const char *tag);
void tree_append(TREEBLK_t *x, TREEBLK_t *y);
void tree_prepend(TREEBLK_t *x, TREEBLK_t *y);
void tree_insert(TREEBLK_t *x, TREEBLK_t *y, int place);
TREEBLK_t *tree_remove(TREEBLK_t *x, int place);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char *t(TREEBLK_t *x) { return x ? x->tag : ""; }
static inline DESCR_t v(TREEBLK_t *x) { return x ? x->val : NULVCL; }
static inline int n(TREEBLK_t *x) { return x ? x->n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline TREEBLK_t *c_i(TREEBLK_t *x, int i) { if (!x || i < 1 || i > x->n) return NULL; return x->c[i-1]; }
#ifdef __cplusplus
}
#endif
#endif
