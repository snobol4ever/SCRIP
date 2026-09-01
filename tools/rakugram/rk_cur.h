/* rk_cur.h -- THE ONE definition of the parse cursor for the Grammar.nqp port.
 *
 * ⛔ Was defined in four places (the emitter's header, rk_hll.h, prec_test.c, rk_cc_test.c). Four
 * copies of a struct agree until one gains a field, and then the ABI splits with no diagnostic.
 *
 * ⛔ REFUSAL MUST PROPAGATE AS REFUSAL. RK_UNIMPL is -1, and every generated call site tests
 * `if (!rk_x(c)) goto fail;` -- 2041 of them at rung 9a -- so a refusing subrule READS AS A MATCH at
 * its caller: the parser would go on as if the rule had matched (consuming nothing) and report a
 * verdict about a program it never actually parsed. rk_refuse() records the rule name and, when an
 * entry point has installed a jmp_buf, longjmps to it; a refusal then never returns into a caller.
 * With no jmp_buf (the compile-only gate) it returns RK_UNIMPL exactly as before.
 */
#ifndef RK_CUR_H
#define RK_CUR_H
#include <setjmp.h>
#include <string.h>
/* MARKER / MARKED -- NQP HLL::Grammar's per-name position memo (%*MARKHASH): MARKER(n) records the
 * current position under n and always succeeds, zero-width; MARKED(n) succeeds iff the recorded position
 * equals the current one. Grammar.nqp uses it for 'endstmt', 'ws', 'endarg' … (statement, eat_terminator,
 * _ws). Deterministic: no guessing, and an overflow of the tiny table REFUSES rather than dropping a mark. */
#define RK_MAXMARKS 16
typedef struct { const char *name; int pos; } RkMark;
typedef struct {
    const char *src; int pos; int len;
    jmp_buf *refuse;          /* installed by the entry point; NULL = compile-only, no propagation */
    const char *refused;      /* the rule that refused, for the three-valued verdict */
    int nmarks; RkMark marks[RK_MAXMARKS];
} RkCur;
#define RK_UNIMPL (-1)
static inline int rk_refuse(RkCur *c, const char *rule) {
    c->refused = rule;
    if (c->refuse) longjmp(*c->refuse, 1);
    return RK_UNIMPL;
}
static inline int rk_mark_set(RkCur *c, const char *name) {
    for (int i = 0; i < c->nmarks; i++) if (!strcmp(c->marks[i].name, name)) { c->marks[i].pos = c->pos; return 1; }
    if (c->nmarks >= RK_MAXMARKS) return rk_refuse(c, "MARKER: mark table full");
    c->marks[c->nmarks].name = name; c->marks[c->nmarks].pos = c->pos; c->nmarks++; return 1;
}
static inline int rk_mark_test(RkCur *c, const char *name) {
    for (int i = 0; i < c->nmarks; i++) if (!strcmp(c->marks[i].name, name)) return c->marks[i].pos == c->pos;
    return 0;                 /* never marked: MARKED fails, as in NQP (nqp::existskey false) */
}
#endif
