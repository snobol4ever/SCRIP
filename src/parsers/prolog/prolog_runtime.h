#ifndef PL_RUNTIME_H
#define PL_RUNTIME_H
#include "term.h"
typedef struct {
    Term  **stack;
    int     top;
    int     capacity;
} Trail;
void trail_init(Trail *t);
void trail_push(Trail *t, Term *term);
void trail_unwind(Trail *t, int mark);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int trail_mark(const Trail *t) { return t->top; }
typedef struct {
    int n_vars;
    int n_args;
    int trail_mark_slot;
} EnvLayout;
#endif
