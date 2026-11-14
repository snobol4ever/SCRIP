#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir/ct_arena.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast_cap.h"
typedef struct tree_t tree_t;
#ifdef __cplusplus
extern "C" {
#endif
struct tree_t { tree_e t; int start; union { char * sval; long long ival; double dval; } v; int n; int end; tree_t ** c; int line; int slen; };
static inline void ast_push(tree_t * p, tree_t * child) {
    size_t cap = p->c ? AST_CAP(p) : 0;
    if ((size_t)p->n >= cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        char * block = (char *)ct_grow(p->c ? (char *)p->c - sizeof(size_t) : NULL, sizeof(size_t) + new_cap * sizeof(tree_t *));
        p->c = (tree_t **)(block + sizeof(size_t));
        AST_SET_CAP(p, new_cap);
    }
    p->c[p->n++] = child;
}
static inline tree_t * ast_pop(tree_t * p) { tree_t * child; if (p->n == 0) return NULL; child = p->c[--p->n]; return child; }
static inline void ast_tree_free(tree_t *p) { if (!p) return; for (int _i = 0; _i < p->n; _i++) ast_tree_free(p->c[_i]); if (p->c) ct_drop((char *)p->c - sizeof(size_t)); ct_drop(p); }
#ifdef __cplusplus
}
#endif
#endif
