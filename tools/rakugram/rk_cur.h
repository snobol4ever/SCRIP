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
/* Parse-time dynamic variables ($*X) -- ONLY those whose every write in Grammar.nqp is a literal constant
 * (decided at generation time by nqp_ast.modelled_dynvars; 7 of 13 consulted ones at rung 9d). A `:my`
 * PUSHes a binding for the duration of the declaring rule's activation (unwound at every exit, success
 * or fail); `{ $*X := 'c' }` SETs the nearest binding in place (dynamic scope: the caller sees it);
 * `<?{ $*X }>` reads truthiness. A read of a variable with NO binding REFUSES -- never a default. */
#define RK_MAXDYN 128
typedef struct { const char *name; const char *val; } RkDyn;
typedef struct {
    const char *src; int pos; int len;
    jmp_buf *refuse;          /* installed by the entry point; NULL = compile-only, no propagation */
    const char *refused;      /* the rule that refused, for the three-valued verdict */
    int nmarks; RkMark marks[RK_MAXMARKS];
    int ndyn; RkDyn dyn[RK_MAXDYN];
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
static inline int rk_dyn_push(RkCur *c, const char *name, const char *val) {
    if (c->ndyn >= RK_MAXDYN) return rk_refuse(c, ":my -- dynvar stack full");
    c->dyn[c->ndyn].name = name; c->dyn[c->ndyn].val = val; c->ndyn++; return 1;
}
static inline int rk_dyn_set(RkCur *c, const char *name, const char *val) {
    for (int i = c->ndyn - 1; i >= 0; i--) if (!strcmp(c->dyn[i].name, name)) { c->dyn[i].val = val; return 1; }
    return rk_refuse(c, "assignment to an unbound $* variable");
}
/* truthiness as NQP sees a string: '' and '0' are false. -1 = unbound (the caller refuses). */
static inline int rk_dyn_truthy(RkCur *c, const char *name) {
    for (int i = c->ndyn - 1; i >= 0; i--) if (!strcmp(c->dyn[i].name, name)) return c->dyn[i].val[0] != 0 && strcmp(c->dyn[i].val, "0") != 0;
    return -1;
}
static inline int rk_mark_test(RkCur *c, const char *name) {
    for (int i = 0; i < c->nmarks; i++) if (!strcmp(c->marks[i].name, name)) return c->marks[i].pos == c->pos;
    return 0;                 /* never marked: MARKED fails, as in NQP (nqp::existskey false) */
}
#endif
