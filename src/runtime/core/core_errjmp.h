#ifndef CORE_ERRJMP_H
#define CORE_ERRJMP_H
#include <setjmp.h>
typedef struct core_errjmp { jmp_buf jb; struct core_errjmp *prev; } core_errjmp_t;
extern core_errjmp_t *g_core_errjmp_stk; extern int g_core_errjmp_n;
static inline void core_errjmp_push(core_errjmp_t *e) { e->prev = g_core_errjmp_stk; g_core_errjmp_stk = e; }
static inline void core_errjmp_pop(core_errjmp_t *e, int my) { g_core_errjmp_stk = e->prev; g_core_errjmp_n = my; }
static inline core_errjmp_t *core_errjmp_at(int k) { core_errjmp_t *p = g_core_errjmp_stk; for (int i = g_core_errjmp_n - 1; i > k && p; i--) p = p->prev; return p; }
#endif
