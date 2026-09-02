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
        unsigned long len = r.slen != 0xFFFFFFFFu ? r.slen : __builtin_strlen(r.s);
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
    for (unsigned al = 1; al <= 70; al++) {
        for (unsigned bl = 1; bl <= 70; bl++) {
            if (al > 36 && al < 64 && bl > 36 && bl < 64) continue;
            char nm[64]; snprintf(nm, sizeof nm, "len %u+%u", al, bl);
            both(nm, Sraw(big, al), Sraw(big2, bl));
        }
    }
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
    both("SNUL + int (THE idiom)",    NUL(), I(3));
    both("SNUL + int negative",       NUL(), I(-77));
    both("SNUL + int zero",           NUL(), I(0));
    both("SNUL + real (type kept)",   NUL(), R(0.5));
    both("real + SNUL (type kept)",   R(0.5), NUL());
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
