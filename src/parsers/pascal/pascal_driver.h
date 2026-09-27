/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PASCAL_DRIVER_H
#define PASCAL_DRIVER_H
#include "../../ir/ast.h"
void pascal_compile(const char *source, const char *filename, tree_t **out_ast);
#define PAS_NREC_IDX_MARK 0x4E524543
static inline int pas_node_nrec_marked(const tree_t *e) { return e && e->t == TT_IDX && e->slen == PAS_NREC_IDX_MARK; }
int pascal_sem_check(const tree_t *root, const char *filename);
#endif
