/* Acceptance for rung 6 (the hand-written HLL::Grammar layer). Asserts the SUBTLE cases -- the ones
   where a wrong implementation still parses plausibly and only misparses later. */
#include <stdio.h>
#include <string.h>
#include "rk_hll.h"
static int fails;
static void chk(const char *what, int got, int want) {
    if (got != want) { fails++; printf("  FAIL %-46s got %d want %d\n", what, got, want); }
    else               printf("  ok   %-46s %d\n", what, got);
}
static int pos_after(int (*r)(RkCur *), const char *s) { RkCur c = { .src = s, .pos = 0, .len = (int)strlen(s) }; r(&c); return c.pos; }
int main(void) {
    printf("ws -- 25 callers; `rule` inserts it implicitly between every atom\n");
    chk("ws skips spaces/tabs/newlines",      pos_after(rk_ws, "  \t\n x"), 5);
    chk("ws skips a # line comment",          pos_after(rk_ws, "# hi\nx"), 5);
    chk("ws skips =begin..=end pod",          pos_after(rk_ws, "=begin X\nbody\n=end X\ny"), 21);  /* 9 + 5 + 7 = 21, i.e. at 'y' */
    chk("ws succeeds on empty (never fails)", pos_after(rk_ws, "abc"), 0);

    printf("\nidentifier -- the hyphen rule is the trap\n");
    chk("plain ident",                        pos_after(rk_identifier, "foo "), 3);
    chk("hyphenated sub name foo-bar",        pos_after(rk_identifier, "foo-bar "), 7);
    chk("apostrophe form isn't",              pos_after(rk_identifier, "isn't "), 5);
    /* ⛔ `a - b` must NOT lex as the identifier `a-b`: the hyphen needs a letter after it. */
    chk("a - b does NOT become a-b",          pos_after(rk_identifier, "a - b"), 1);
    chk("trailing hyphen not absorbed",       pos_after(rk_identifier, "foo- "), 3);

    printf("\ninteger literals\n");
    chk("plain 1234",                         pos_after(rk_decint, "1234x"), 4);
    chk("underscore separator 1_000",         pos_after(rk_decint, "1_000 "), 5);
    /* ⛔ a LEADING underscore is not a number; digits_in_base only accepts _ after a digit. */
    chk("_5 is not a decint",                 pos_after(rk_decint, "_5"), 0);
    chk("hex ff stops before g",              pos_after(rk_hexint, "ffg"), 2);
    chk("octal 17 stops at 8",                pos_after(rk_octint, "178"), 2);
    chk("binary 1011 stops at 2",             pos_after(rk_binint, "10112"), 4);
    /* base bounds actually bind: 9 is not octal, 2 is not binary */
    chk("decint accepts 9, octint does not",  pos_after(rk_octint, "9"), 0);

    printf("\nlookahead -- must NOT consume\n");
    { RkCur c = { .src = "foo", .pos = 0, .len = 3 }; int m = rk_before(&c, rk_ident);
      chk("before(ident) matches", m, 1); chk("before left pos at 0", c.pos, 0); }
    { RkCur c = { .src = "123", .pos = 0, .len = 3 }; int m = rk_after(&c, rk_ident);
      chk("after(ident) is negative lookahead", m, 1); chk("after left pos at 0", c.pos, 0); }
    printf("\n%s\n", fails ? "SOME CHECKS FAILED" : "all checks passed");
    return fails ? 1 : 0;
}
