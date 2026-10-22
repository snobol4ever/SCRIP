/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_ATOM_H
#define PL_ATOM_H
#include <stddef.h>
#include <limits.h>
#include "g_runtime.h"
void prolog_atom_init(void);
int prolog_atom_intern(const char *name);
int prolog_atom_intern_n(const char *s, size_t n);
int prolog_atom_intern_lit(const char *s);
size_t prolog_atom_len(int id);
const char *prolog_atom_name(int id);
int prolog_atom_count(void);
void rt_pl_atom_table_install(const long *tab);
int prolog_functor_intern(int name, int arity);
#define PROLOG_MAX_ARITY INT_MAX
#define PROLOG_MAX_PROCEDURE_ARITY 1024
int prolog_functor_name(int fid);
int prolog_functor_arity(int fid);
int prolog_functor_count(void);
void rt_pl_functor_table_install(const long *tab);
void prolog_op_col_set(int atom, int prec, const char *ty);
int prolog_op_col_get(int atom, int fix, int *prec_out, int *type_out);
int prolog_op_cols_ready(void);
int prolog_op_col_user(int atom, int fix);
void prolog_op_col_set_u(int atom, int prec, const char *ty, int user);
const char *prolog_op_type_name(int code);
#endif
