#ifndef G_RUNTIME_ATOMS_H
#define G_RUNTIME_ATOMS_H
#include <stddef.h>
typedef struct { char *key; int id; } g_atom_hentry_t;
typedef struct { int name; int arity; } g_atom_fentry_t;
typedef struct { unsigned short p; unsigned char t; unsigned char u; } g_atom_opcol_t;
typedef struct { g_atom_opcol_t pre; g_atom_opcol_t in; g_atom_opcol_t post; } g_atom_opcols_t;
typedef struct {
    int atom_dot;
    int atom_nil;
    int atom_true;
    int atom_fail;
    int atom_cut;
    int functor_dot2;
    char **atom_names;
    int atom_len;
    int atom_cap;
    g_atom_hentry_t *ht;
    int ht_size;
    int ht_used;
    char *name_pool;
    size_t name_pool_left;
    g_atom_fentry_t *functors;
    int functor_len;
    int functor_cap;
    int *fht;
    int fht_size;
    int fht_used;
    g_atom_opcols_t *opcols;
    int opcols_cap;
    int opcols_ready;
} g_atoms_t;
#endif
