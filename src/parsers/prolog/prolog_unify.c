#include "rt/rt_arena.h"
#include "prolog_runtime.h"
#include "term.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TRAIL_INIT_CAP 64
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void trail_init(Trail *t) {
    t->stack    = rt_ws_alloc(TRAIL_INIT_CAP * sizeof(Term *));
    t->top      = 0;
    t->capacity = TRAIL_INIT_CAP;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void trail_push(Trail *t, Term *term) {
    if (t->top >= t->capacity) {
        t->capacity *= 2;
        t->stack = rt_ws_realloc(t->stack, t->capacity * sizeof(Term *));
    }
    t->stack[t->top++] = term;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void trail_unwind(Trail *t, int mark) {
    while (t->top > mark) {
        Term *bound = t->stack[--t->top];
        int saved_slot = bound->saved_slot;
        bound->tag      = TERM_VAR;
        bound->var_slot = saved_slot;
    }
}
