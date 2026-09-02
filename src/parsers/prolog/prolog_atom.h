/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_ATOM_H
#define PL_ATOM_H
void prolog_atom_init(void);
int prolog_atom_intern(const char *name);
const char *prolog_atom_name(int id);
int prolog_atom_count(void);
extern int ATOM_DOT;
extern int ATOM_NIL;
extern int ATOM_TRUE;
extern int ATOM_FAIL;
extern int ATOM_CUT;
#endif
