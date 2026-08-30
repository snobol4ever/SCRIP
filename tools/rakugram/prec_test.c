/* Driver for the generated Pratt parser: proves PRECEDENCE and ASSOCIATIVITY, not just "it parses".
   Associativity is the property that a syntax-only test cannot see -- a parser that folds 1-2-3 as
   1-(2-3) accepts exactly the same language as one that folds it (1-2)-3. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct { const char *src; int pos; int len; } RkCur;
struct RkNode { char buf[256]; };
typedef struct RkNode RkNode;
#include "rk_prec_gen.c"
static RkNode *mk(const char *s) { RkNode *n = malloc(sizeof *n); snprintf(n->buf, sizeof n->buf, "%s", s); return n; }
void rk_skip_ws(RkCur *c) { while (c->pos < c->len && (c->src[c->pos]==' '||c->src[c->pos]=='\t')) c->pos++; }
RkNode *rk_parse_term(RkCur *c) {
    rk_skip_ws(c);
    int st = c->pos;
    while (c->pos < c->len && ((c->src[c->pos]>='0'&&c->src[c->pos]<='9')||(c->src[c->pos]>='a'&&c->src[c->pos]<='z'))) c->pos++;
    if (c->pos == st) return 0;
    char t[64]; int n = c->pos-st; if (n > 63) n = 63; memcpy(t, c->src+st, (size_t)n); t[n]=0;
    return mk(t);
}
RkNode *rk_mk_binop(const RkOp *o, RkNode *a, RkNode *b) {
    RkNode *n = malloc(sizeof *n); snprintf(n->buf, sizeof n->buf, "(%s %s %s)", a->buf, o->sym, b->buf); return n;
}
RkNode *rk_mk_unop(const RkOp *o, RkNode *a) {
    RkNode *n = malloc(sizeof *n); snprintf(n->buf, sizeof n->buf, "(%s %s)", o->sym, a->buf); return n;
}
static int fails = 0;
static void chk(const char *expr, const char *want) {
    RkCur c = { expr, 0, (int)strlen(expr) };
    RkNode *r = rk_EXPR(&c, "b=");
    const char *got = r ? r->buf : "<null>";
    int ok = !strcmp(got, want);
    if (!ok) fails++;
    printf("  %-5s %-18s -> %-30s %s\n", ok?"ok":"FAIL", expr, got, ok?"":("want "));
    if (!ok) printf("        want %s\n", want);
}
int main(void) {
    rk_ops_init_builtin();
    printf("operators installed from Grammar.nqp: %d\n\n", g_rk_nops);
    printf("PRECEDENCE (multiplicative u= binds tighter than additive t=):\n");
    chk("2 + 3 * 4",  "(2 + (3 * 4))");
    chk("2 * 3 + 4",  "((2 * 3) + 4)");
    printf("\nASSOCIATIVITY (invisible to any syntax-only test):\n");
    chk("1 - 2 - 3",  "((1 - 2) - 3)");        /* additive is LEFT */
    chk("2 ** 3 ** 2", "(2 ** (3 ** 2))");     /* exponentiation is RIGHT */
    printf("\nPARSE-TIME EXTENSION -- the property that makes Raku non-LALR:\n");
    printf("  before install: "); { RkCur c={"1 fo 2",0,6}; RkNode*r=rk_EXPR(&c,"b="); printf("%-14s (stops at the unknown operator)\n", r?r->buf:"<null>"); }
    rk_op_install("fo", RK_INFIX, "tA=", RK_LEFT, "user");   /* between additive t= and multiplicative u= */
    printf("  after  install: "); chk("1 + 2 fo 3", "(1 + (2 fo 3))");
    printf("\n%s\n", fails ? "SOME CHECKS FAILED" : "all checks passed");
    return fails ? 1 : 0;
}
