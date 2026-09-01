/* rk_cc_test.c -- prove the lowered character classes, escapes and anchors actually DISCRIMINATE.
 *
 * ⛔ WHY EVERY ASSERTION HERE IS A NEGATIVE ONE. Rung 3 shipped these primitives as stubs that
 * consumed one character and returned "matched" for any input. Every POSITIVE test passes against
 * such a stub -- <[0-9]> "matches" '5' just fine when it matches everything. Only asking what the
 * class REFUSES can tell the two apart, so each case below pairs an accept with a reject.
 *
 * The tables are the ones tools/rakugram/nqp_cc.py actually produces for specs taken verbatim from
 * Grammar.nqp; test_gate_rakugram_precedence.sh regenerates them and checks they still agree.
 */
#include <stdio.h>
#include <string.h>
typedef struct { const char *src; int pos; int len; } RkCur;
#include "rk_cc_rt.c"

static int fails = 0;
static void ck(const char *what, int got, int want) {
    if (got != want) { fails++; printf("  FAIL %-46s got %d want %d\n", what, got, want); }
    else             { printf("  ok   %-46s\n", what); }
}
/* run one class against one input; report match and how far the cursor moved */
static int run(int mode, const RkCCItem *t, int n, const char *s, int *adv) {
    RkCur c = { s, 0, (int)strlen(s) };
    int r = rk_cc(&c, mode, t, n);
    if (adv) *adv = c.pos;
    return r;
}

/* --- tables exactly as nqp_cc.py lowers them ---------------------------------------------- */
static const RkCCItem cc_digit19[]  = { {0,'1','9'} };                    /* <[1..9]>   range */
static const RkCCItem cc_dashminus[]= { {0,45,45}, {0,8722,8722} };       /* <[-−]>  two literals */
static const RkCCItem cc_quotedash[]= { {0,39,39}, {0,45,45} };           /* <[ ' \- ]>       */
static const RkCCItem cc_ws_hash[]  = { {1,'s',0}, {0,'#','#'} };         /* <?[\s#]>  zero-width */
static const RkCCItem cc_lparen[]   = { {0,'(','('} };                    /* <![(]>    zero-width */
static const RkCCItem cc_word[]     = { {1,'w',0} };                      /* <-[\w]>   negated  */
static const RkCCItem cc_d[]        = { {1,'d',0} };                      /* \d                 */
static const RkCCItem cc_h[]        = { {1,'h',0} };                      /* \h  horizontal only */

int main(void) {
    int adv;
    printf("CHARACTER CLASSES -- the rejects are the whole point:\n");
    ck("<[1..9]> accepts '5'",            run(0, cc_digit19, 1, "5", 0), 1);
    ck("<[1..9]> REJECTS 'z'",            run(0, cc_digit19, 1, "z", 0), 0);
    ck("<[1..9]> REJECTS '0' (not in range)", run(0, cc_digit19, 1, "0", 0), 0);

    /* ⛔ `..` is the range operator in Raku; `-` is a LITERAL. Reading `-` as POSIX-style range
       would turn <[-−]> into '-'..U+2212 and swallow every ASCII letter and digit. */
    ck("<[-−]> accepts '-'",             run(0, cc_dashminus, 2, "-", 0), 1);
    ck("<[-−]> accepts U+2212 minus",    run(0, cc_dashminus, 2, "−", 0), 1);
    ck("<[-−]> REJECTS 'a' (not a range)", run(0, cc_dashminus, 2, "a", 0), 0);
    ck("<[-−]> REJECTS '5' (not a range)", run(0, cc_dashminus, 2, "5", 0), 0);
    run(0, cc_dashminus, 2, "−", &adv);
    ck("U+2212 consumed as ONE codepoint (3 bytes)", adv, 3);

    /* literal whitespace inside <[...]> is insignificant: <[ ' \- ]> is two members, not four */
    ck("<[ ' \\- ]> accepts apostrophe",  run(0, cc_quotedash, 2, "'", 0), 1);
    ck("<[ ' \\- ]> REJECTS a space",     run(0, cc_quotedash, 2, " ", 0), 0);

    printf("\nZERO-WIDTH FORMS -- <?[...]> and <![...]> must NOT advance:\n");
    ck("<?[\\s#]> accepts '#'",           run(2, cc_ws_hash, 2, "#", &adv), 1);
    ck("<?[\\s#]> did not advance",       adv, 0);
    ck("<?[\\s#]> REJECTS 'x'",           run(2, cc_ws_hash, 2, "x", 0), 0);
    ck("<![(]> accepts 'x'",              run(3, cc_lparen, 1, "x", &adv), 1);
    ck("<![(]> did not advance",          adv, 0);
    ck("<![(]> REJECTS '('",              run(3, cc_lparen, 1, "(", 0), 0);
    ck("<![(]> accepts end-of-input",     run(3, cc_lparen, 1, "", 0), 1);
    ck("<?[\\s#]> REJECTS end-of-input",  run(2, cc_ws_hash, 2, "", 0), 0);

    printf("\nNEGATED CONSUMING FORM:\n");
    ck("<-[\\w]> accepts '+'",            run(1, cc_word, 1, "+", &adv), 1);
    ck("<-[\\w]> advanced one byte",      adv, 1);
    ck("<-[\\w]> REJECTS 'a'",            run(1, cc_word, 1, "a", 0), 0);
    ck("<-[\\w]> REJECTS end-of-input",   run(1, cc_word, 1, "", 0), 0);

    printf("\nBACKSLASH CLASSES:\n");
    ck("\\d accepts '7'",                 run(0, cc_d, 1, "7", 0), 1);
    ck("\\d REJECTS 'z'",                 run(0, cc_d, 1, "z", 0), 0);
    ck("\\h accepts a space",             run(0, cc_h, 1, " ", 0), 1);
    ck("\\h accepts a tab",               run(0, cc_h, 1, "\t", 0), 1);
    /* \h is HORIZONTAL whitespace: a newline is \v, and conflating them makes a rule that should
       stop at end-of-line run on into the next one. */
    ck("\\h REJECTS a newline",           run(0, cc_h, 1, "\n", 0), 0);

    printf("\nANCHORS -- ^^ / $$ constrain and never consume:\n");
    { RkCur c = { "ab\ncd", 0, 5 };
      ck("^^ holds at position 0",        rk_anchor(&c, "^^"), 1);
      c.pos = 1; ck("^^ REJECTS mid-line", rk_anchor(&c, "^^"), 0);
      c.pos = 3; ck("^^ holds after a newline", rk_anchor(&c, "^^"), 1);
      c.pos = 2; ck("$$ holds before a newline", rk_anchor(&c, "$$"), 1);
      c.pos = 1; ck("$$ REJECTS mid-line", rk_anchor(&c, "$$"), 0);
      c.pos = 5; ck("$$ holds at end of input", rk_anchor(&c, "$$"), 1);
      c.pos = 2; rk_anchor(&c, "$$"); ck("anchor did not consume", c.pos, 2); }

    printf("\nWORD BOUNDARIES:\n");
    { RkCur c = { "ab cd", 0, 5 };
      ck("<< holds at start of a word",   rk_wb(&c, 0), 1);
      c.pos = 1; ck("<< REJECTS inside a word", rk_wb(&c, 0), 0);
      c.pos = 2; ck(">> holds at end of a word", rk_wb(&c, 1), 1);
      c.pos = 1; ck(">> REJECTS inside a word", rk_wb(&c, 1), 0); }

    printf("\n%s\n", fails ? "SOME CHECKS FAILED" : "all checks passed");
    return fails ? 1 : 0;
}
