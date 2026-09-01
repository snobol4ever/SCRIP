/* rk_glue.c -- RUNG 10: the entry point. One translation unit that links the three halves of the port:
 *   the hand-written HLL layer (rk_hll.c: ws, ident, int literals, lookahead, diagnostics),
 *   rung 4's Pratt parser over the runtime-mutable operator table (rk_prec_gen.c: rk_EXPR(c, prec)),
 *   and the generated recursive descent (rk_gen.c, emitted with --provided=EXPR,O,ws,… so no stub
 *   shadows what this file supplies).
 *
 * THE VERDICT IS THREE-VALUED. rk_parse() returns RK_MATCH, RK_NOMATCH (with the position), or
 * RK_REFUSED with the NAME of the rule that could not decide. A refusal reaches here by longjmp from
 * rk_refuse() -- it never returns into a caller as a fake match (see rk_cur.h). Refusals are the
 * instrument: each names the next construct the port needs for the program it was given.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rk_cur.h"
/* ⛔ Grammar.nqp DEFINES `identifier` itself, and its bare `<before>`/`<after>` calls take no operand, so the
 * generated file owns those three names (static). The HLL layer's same-named helpers are renamed inside
 * this translation unit only -- rk_hll.c compiled on its own still exports them for its test driver. */
#define rk_identifier rk_hll_identifier
#define rk_before     rk_hll_before
#define rk_after      rk_hll_after
#include "rk_hll.h"
#undef rk_identifier
#undef rk_before
#undef rk_after
struct RkNode { char buf[64]; };
typedef struct RkNode RkNode;
#define rk_EXPR rk_EXPR_pratt                       /* rung 4 names its precedence climber rk_EXPR */
#include "rk_prec_gen.c"
#undef rk_EXPR
static int rk_termish(RkCur *c);                    /* generated below; the Pratt term hook calls it */
RkNode *rk_parse_term(RkCur *c) {
    static RkNode t;                                /* the Pratt loop only tests for NULL */
    return rk_termish(c) ? &t : NULL;
}
RkNode *rk_mk_binop(const RkOp *o, RkNode *a, RkNode *b) { (void)o; (void)b; return a; }
RkNode *rk_mk_unop(const RkOp *o, RkNode *a) { (void)o; return a; }
void rk_skip_ws(RkCur *c) { rk_ws(c); }
/* Grammar.nqp's <EXPR> is the operator-precedence parser; the generated grammar calls it as a plain rule. */
static int rk_EXPR(RkCur *c) { return rk_EXPR_pratt(c, "b=") != NULL; }
static int rk_O(RkCur *c) { (void)c; return 1; }    /* <O(...)> attaches precedence metadata: no input */
#include "rk_gen.c"
enum { RK_MATCH = 0, RK_NOMATCH = 1, RK_REFUSED = 2 };
int rk_parse(const char *src, int len, int *pos_out, const char **refused_out) {
    RkCur c; memset(&c, 0, sizeof c);
    c.src = src; c.len = len;
    jmp_buf jb; c.refuse = &jb;
    if (setjmp(jb)) { *refused_out = c.refused; *pos_out = c.pos; return RK_REFUSED; }
    rk_ops_init_builtin();
    int r = rk_comp_unit(&c);
    *pos_out = c.pos; *refused_out = 0;
    return (r == 1 && c.pos == c.len) ? RK_MATCH : RK_NOMATCH;
}
