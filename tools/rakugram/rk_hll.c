/* rk_hll.c -- the inherited NQP HLL::Grammar rules, hand-written.
 *
 * RUNG 6 of the Grammar.nqp -> recursive-descent port. These are the rules Grammar.nqp REFERENCES but
 * does not DEFINE: they live in NQP's HLL::Grammar and cannot be generated from Grammar.nqp at all.
 * `nqp_reach.py` says 48 of them are reachable from comp_unit; 13 are diagnostics and 2 (EXPR, O) are
 * rung 4, leaving 33 real ones. This file starts with the highest-leverage.
 *
 * ⛔ AN UNIMPLEMENTED RULE MUST REFUSE, NEVER RETURN "no match". A rule that reports failure when it
 * was never written is indistinguishable from one that ran and correctly declined, and the parser is
 * then confidently wrong instead of loudly incomplete.
 */
#include <string.h>
#include "rk_hll.h"

/* ---- ws: 25 distinct callers, the single highest-leverage rule in the inherited set ------------
 * ⭐ `rule` (as opposed to `token`) inserts <.ws> IMPLICITLY between every atom, so nothing parses
 * until this exists -- which is why it is first rather than because it is interesting.
 * Raku whitespace includes # line comments and =begin/=end pod blocks. */
int rk_ws(RkCur *c) {
    for (;;) {
        while (c->pos < c->len && (c->src[c->pos] == ' '  || c->src[c->pos] == '\t' ||
                                   c->src[c->pos] == '\n' || c->src[c->pos] == '\r' ||
                                   c->src[c->pos] == '\f')) c->pos++;
        if (c->pos < c->len && c->src[c->pos] == '#') {          /* line comment to EOL */
            while (c->pos < c->len && c->src[c->pos] != '\n') c->pos++;
            continue;
        }
        /* pod block: =begin ... =end, only when '=' starts a line */
        if (c->pos < c->len && c->src[c->pos] == '=' &&
            (c->pos == 0 || c->src[c->pos - 1] == '\n') &&
            c->pos + 6 <= c->len && !memcmp(c->src + c->pos, "=begin", 6)) {
            const char *e = c->src + c->pos;
            const char *end = 0;
            for (const char *p = e; p + 4 <= c->src + c->len; p++)
                if ((p == c->src || p[-1] == '\n') && !memcmp(p, "=end", 4)) { end = p; break; }
            if (!end) { c->pos = c->len; return 1; }             /* unterminated pod eats the rest */
            c->pos = (int)(end - c->src);
            while (c->pos < c->len && c->src[c->pos] != '\n') c->pos++;
            continue;
        }
        return 1;                                               /* ws always succeeds, possibly empty */
    }
}
/* ---- identifiers ------------------------------------------------------------------------------ */
static int is_id_start(int ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'; }
static int is_id_cont(int ch)  { return is_id_start(ch) || (ch >= '0' && ch <= '9'); }

int rk_ident(RkCur *c) {
    if (c->pos >= c->len || !is_id_start((unsigned char)c->src[c->pos])) return 0;
    c->pos++;
    while (c->pos < c->len && is_id_cont((unsigned char)c->src[c->pos])) c->pos++;
    return 1;
}
/* Raku identifiers admit embedded - and ' when followed by a letter: `sub foo-bar`, `isn't`.
 * ⛔ The lookahead is required, not cosmetic: without it `a - b` lexes as the identifier `a-b`. */
int rk_identifier(RkCur *c) {
    if (!rk_ident(c)) return 0;
    for (;;) {
        if (c->pos + 1 < c->len && (c->src[c->pos] == '-' || c->src[c->pos] == '\'')
            && is_id_start((unsigned char)c->src[c->pos + 1])) {
            c->pos++; rk_ident(c);
        } else return 1;
    }
}
/* ---- integer literals ------------------------------------------------------------------------- */
static int digits_in_base(RkCur *c, int base) {
    int n = 0;
    for (;;) {
        if (c->pos >= c->len) break;
        int ch = (unsigned char)c->src[c->pos], v;
        if      (ch == '_' && n) { c->pos++; continue; }   /* 1_000 -- separators, never leading */
        else if (ch >= '0' && ch <= '9') v = ch - '0';
        else if (ch >= 'a' && ch <= 'f') v = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F') v = ch - 'A' + 10;
        else break;
        if (v >= base) break;
        c->pos++; n++;
    }
    return n;
}
int rk_decint(RkCur *c) { return digits_in_base(c, 10) > 0; }
int rk_hexint(RkCur *c) { return digits_in_base(c, 16) > 0; }
int rk_octint(RkCur *c) { return digits_in_base(c,  8) > 0; }
int rk_binint(RkCur *c) { return digits_in_base(c,  2) > 0; }

/* ---- lookahead --------------------------------------------------------------------------------
 * ⛔ Both restore the cursor whatever the inner rule did. A lookahead that consumes is not a
 * lookahead, and the failure is silent: the parse simply continues from the wrong place. */
int rk_before(RkCur *c, RkRule r) { int s = c->pos; int m = r(c); c->pos = s; return m; }
int rk_after (RkCur *c, RkRule r) { int s = c->pos; int m = r(c); c->pos = s; return !m; }

/* ---- diagnostics: 13 of the 48 reachable inherited rules ---------------------------------------
 * These change NO parse decision -- they report. A parser stubs them; only a full compiler needs the
 * message. Kept as distinct symbols so the generated code links unchanged. */
int rk_panic(RkCur *c)        { (void)c; return 0; }
int rk_typed_panic(RkCur *c)  { (void)c; return 0; }
int rk_sorry(RkCur *c)        { (void)c; return 1; }
int rk_worry(RkCur *c)        { (void)c; return 1; }
int rk_obs(RkCur *c)          { (void)c; return 0; }
int rk_malformed(RkCur *c)    { (void)c; return 0; }
int rk_missing(RkCur *c)      { (void)c; return 0; }
int rk_NYI(RkCur *c)          { (void)c; return 0; }
/* Braid / cursor bookkeeping (HLL::Grammar methods): they set parse STATE -- the language braid, attached pod,
 * meta-op admissibility, deferred sorrows -- and consume no input; for MATCHING each is a no-op. can_meta is the
 * one that could gate (it decides whether an operator admits a meta-op); it is admitted here as always-yes and
 * recorded as an over-accept, never an over-reject. */
int rk_set_braid_from(RkCur *c)     { (void)c; return 1; }
int rk_attach_leading_docs(RkCur *c){ (void)c; return 1; }
int rk_can_meta(RkCur *c)           { (void)c; return 1; }
int rk_cry_sorrows(RkCur *c)        { (void)c; return 1; }
int rk_explain_mystery(RkCur *c)    { (void)c; return 1; }
