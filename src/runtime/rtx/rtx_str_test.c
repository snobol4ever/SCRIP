/* rtx_str_test.c — RTX-3 differential battery: asm str_concat_d vs c_str_concat_d.
 *
 * Concatenation IS comparable input-for-input, unlike the allocator: the two calls return
 * different POINTERS (each carves its own block) but must agree on everything that is
 * semantically visible — the result tag, the result slen, and the result BYTES including the
 * terminating NUL. Those three are compared for every case.
 *
 * The fourth thing compared is invisible from the result but load-bearing: the SXT
 * extend-in-place OWNERSHIP TOKEN. After a concat, C arms {owner,len} on the fresh block so a
 * following `S = S CH` can extend in place instead of recopying. If the asm forgot to arm it,
 * every append loop in the corpus would silently go quadratic while every test still passed.
 * g_sxt_fr is hidden, so the token is probed through the exported rt_sxt_match(), which returns
 * the armed length for the owning pointer and -1 otherwise — exactly the observable C uses.
 *
 * Every case runs ASM-then-C and again C-then-ASM: a bug that only appears on one side of an
 * alternation (a token left armed from the previous call, say) cannot hide behind ordering.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "descr.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
DESCR_t c_str_concat_d(DESCR_t a, DESCR_t b);
long rt_sxt_match(const char *s);
typedef struct { int v; unsigned slen; long tok; unsigned long nbytes; char bytes[512]; } obs_t;
static int fails = 0, n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static obs_t take(DESCR_t (*fn)(DESCR_t, DESCR_t), DESCR_t a, DESCR_t b) {
    obs_t o; memset(&o, 0, sizeof o);
    DESCR_t r = fn(a, b);
    o.v = (int)r.v; o.slen = r.slen;
    o.tok = (r.v == DT_S && r.s) ? rt_sxt_match(r.s) : -2;
    if ((r.v == DT_S || r.v == DT_SNUL) && r.s) {
        unsigned long len = r.slen && r.slen != 0xFFFFFFFFu ? r.slen : strlen(r.s);
        if (len > sizeof o.bytes - 1) len = sizeof o.bytes - 1;
        o.nbytes = len; memcpy(o.bytes, r.s, len); o.bytes[len] = (char)0;
    } else if (r.v == DT_I) { o.nbytes = 8; memcpy(o.bytes, &r.i, 8); }
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cmp(const char *what, obs_t x, obs_t c) {
    n++;
    if (x.v == c.v && x.slen == c.slen && x.tok == c.tok && x.nbytes == c.nbytes && memcmp(x.bytes, c.bytes, x.nbytes) == 0) return;
    fails++;
    printf("  MISMATCH %-30s asm{v=%d slen=%u tok=%ld n=%lu \"%.40s\"}\n", what, x.v, x.slen, x.tok, x.nbytes, x.bytes);
    printf("  %-39s   c{v=%d slen=%u tok=%ld n=%lu \"%.40s\"}\n", "", c.v, c.slen, c.tok, c.nbytes, c.bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void both(const char *what, DESCR_t a, DESCR_t b) {
    cmp(what, take(str_concat_d, a, b), take(c_str_concat_d, a, b));
    obs_t c2 = take(c_str_concat_d, a, b); cmp(what, take(str_concat_d, a, b), c2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t S(const char *s) { DESCR_t d; d.v = DT_S; d.slen = (uint32_t)strlen(s); d.s = (char *)s; return d; }
static DESCR_t Sraw(const char *s, uint32_t slen) { DESCR_t d; d.v = DT_S; d.slen = slen; d.s = (char *)s; return d; }
static DESCR_t I(int64_t i) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I; d.i = i; return d; }
static DESCR_t R(double r) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_R; d.r = r; return d; }
static DESCR_t NUL(void) { DESCR_t d; d.v = DT_SNUL; d.slen = 0; d.s = (char *)""; return d; }
static DESCR_t FL(void) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_FAIL; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static char big[600], big2[600];
    for (int i = 0; i < 599; i++) { big[i] = (char)('a' + (i % 26)); big2[i] = (char)('A' + (i % 26)); }
    big[599] = big2[599] = (char)0;
    printf("RTX-3 STR differential battery (asm str_concat_d vs c_str_concat_d)\n");
    /* every RTX_MEMCPY size class on BOTH operands: 1,2,3 / 4-7 / 8-15 / 16-32 / >32 */
    for (unsigned al = 1; al <= 70; al++) {
        for (unsigned bl = 1; bl <= 70; bl++) {
            if (al > 36 && al < 64 && bl > 36 && bl < 64) continue;
            char nm[64]; snprintf(nm, sizeof nm, "len %u+%u", al, bl);
            both(nm, Sraw(big, al), Sraw(big2, bl));
        }
    }
    /* guard boundaries — each of these MUST route to C and match it exactly */
    both("slen0 left (strlen path)",  Sraw("hello", 0), S("world"));
    both("slen0 right",               S("hello"), Sraw("world", 0));
    both("slen0 both",                Sraw("hello", 0), Sraw("world", 0));
    both("CSET left (slen ~0)",       Sraw("abc", 0xFFFFFFFFu), S("d"));
    both("CSET right",                S("d"), Sraw("abc", 0xFFFFFFFFu));
    both("null ptr left",             Sraw((char *)0, 3), S("xyz"));
    both("null ptr right",            S("xyz"), Sraw((char *)0, 3));
    both("SNUL left  (identity)",     NUL(), S("kept"));
    both("SNUL right (identity)",     S("kept"), NUL());
    both("SNUL both",                 NUL(), NUL());
    both("int left  (coerce)",        I(1863), S(" years"));
    both("int right (coerce)",        S("year "), I(-77));
    both("int + SNUL (type kept)",    I(3), NUL());
    /* RTX-3b null-identity arm. SNUL+I is THE idiom shape -- `N = LT(N,lim) N + 1` makes
     * 10M of these in var_access/func_call -- and the battery was blind to it until now:
     * SNUL+S, S+SNUL, SNUL+SNUL and I+SNUL were all present, so symmetry made it LOOK
     * covered. Manual v3.7 p.22: the other operand is returned UNCHANGED, not coerced. */
    both("SNUL + int (THE idiom)",    NUL(), I(3));
    both("SNUL + int negative",       NUL(), I(-77));
    both("SNUL + int zero",           NUL(), I(0));
    both("SNUL + real (type kept)",   NUL(), R(0.5));
    both("real + SNUL (type kept)",   R(0.5), NUL());
    /* guard precedence: FAIL and slen0-DT_S must still beat the null arm, both orders */
    both("SNUL + FAIL (FAIL wins)",   NUL(), FL());
    both("FAIL + SNUL (FAIL wins)",   FL(), NUL());
    both("slen0 left + SNUL",         Sraw("hello", 0), NUL());
    both("SNUL + slen0 right",        NUL(), Sraw("hello", 0));
    both("CSET + SNUL",               Sraw("abc", 0xFFFFFFFFu), NUL());
    both("SNUL + CSET",               NUL(), Sraw("abc", 0xFFFFFFFFu));
    both("real left (coerce)",        R(0.5), S("x"));
    both("real right",                S("x"), R(100.0));
    both("FAIL left",                 FL(), S("x"));
    both("FAIL right",                S("x"), FL());
    both("FAIL both",                 FL(), FL());
    both("empty-content slen1",       Sraw("\0zz", 1), S("q"));
    both("embedded NUL in payload",   Sraw("a\0c", 3), S("Q"));
    both("aliased operand a==b",      Sraw(big, 20), Sraw(big, 20));
    both("huge 599+599",              Sraw(big, 599), Sraw(big2, 599));
    /* SXT token interaction: the armed token from a previous concat must be honored, i.e. the
     * asm must decline the fast path when the left operand IS the owner and let C extend. */
    for (int rep = 0; rep < 3; rep++) {
        DESCR_t acc = S("seed");
        for (int k = 0; k < 6; k++) {
            DESCR_t x = str_concat_d(acc, S("+ch"));
            DESCR_t y = c_str_concat_d(acc, S("+ch"));
            n++;
            if (x.v != y.v || x.slen != y.slen || strcmp(x.s, y.s) != 0) {
                fails++; printf("  MISMATCH append-loop k=%d asm{%u \"%s\"} c{%u \"%s\"}\n", k, x.slen, x.s, y.slen, y.s);
            }
            acc = x;
        }
    }
    printf("RTX-3 STR: %d cases, %d mismatches\n", n, fails);
    if (fails) { printf("RTX STR UNIT: FAIL\n"); return 1; }
    printf("RTX STR UNIT: PASS\n");
    return 0;
}
