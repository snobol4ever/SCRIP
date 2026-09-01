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
typedef struct {
    const char *src; int pos; int len;
    jmp_buf *refuse;          /* installed by the entry point; NULL = compile-only, no propagation */
    const char *refused;      /* the rule that refused, for the three-valued verdict */
} RkCur;
#define RK_UNIMPL (-1)
static inline int rk_refuse(RkCur *c, const char *rule) {
    c->refused = rule;
    if (c->refuse) longjmp(*c->refuse, 1);
    return RK_UNIMPL;
}
#endif
