#ifndef LOWER_PASCAL_TREE_H
#define LOWER_PASCAL_TREE_H
typedef struct tree_t tree_t;
tree_t *lower_pascal_tree(tree_t *pruned, const char *filename);
void pas_note_error(void);
int pas_undef_var(const char *name);
#endif
