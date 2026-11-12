#include "parsers/raku/rk_syntax/rks_head.h"
#include <stdio.h>
#include <string.h>
#include "ir/ct_arena.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
#include "parsers/raku/rk_syntax/rks_08_sh.h"
static int r_heredoc_bodies(RkP *p, int pos);
const int rk_brackets[] = {
    0x0028,0x0029,0x003C,0x003E,0x005B,0x005D,0x007B,0x007D,0x00AB,0x00BB,0x0F3A,0x0F3B,0x0F3C,0x0F3D,0x169B,0x169C,0x2018,0x2019,0x201A,0x2019,0x201B,0x2019,0x201C,0x201D,0x201E,0x201D,
    0x201F,0x201D,0x2039,0x203A,0x2045,0x2046,0x207D,0x207E,0x208D,0x208E,0x2208,0x220B,0x2209,0x220C,0x220A,0x220D,0x2215,0x29F5,0x223C,0x223D,0x2243,0x22CD,0x2252,0x2253,0x2254,0x2255,
    0x2264,0x2265,0x2266,0x2267,0x2268,0x2269,0x226A,0x226B,0x226E,0x226F,0x2270,0x2271,0x2272,0x2273,0x2274,0x2275,0x2276,0x2277,0x2278,0x2279,0x227A,0x227B,0x227C,0x227D,0x227E,0x227F,
    0x2280,0x2281,0x2282,0x2283,0x2284,0x2285,0x2286,0x2287,0x2288,0x2289,0x228A,0x228B,0x228F,0x2290,0x2291,0x2292,0x2298,0x29B8,0x22A2,0x22A3,0x22A6,0x2ADE,0x22A8,0x2AE4,0x22A9,0x2AE3,
    0x22AB,0x2AE5,0x22B0,0x22B1,0x22B2,0x22B3,0x22B4,0x22B5,0x22B6,0x22B7,0x22C9,0x22CA,0x22CB,0x22CC,0x22D0,0x22D1,0x22D6,0x22D7,0x22D8,0x22D9,0x22DA,0x22DB,0x22DC,0x22DD,0x22DE,0x22DF,
    0x22E0,0x22E1,0x22E2,0x22E3,0x22E4,0x22E5,0x22E6,0x22E7,0x22E8,0x22E9,0x22EA,0x22EB,0x22EC,0x22ED,0x22F0,0x22F1,0x22F2,0x22FA,0x22F3,0x22FB,0x22F4,0x22FC,0x22F6,0x22FD,0x22F7,0x22FE,
    0x2308,0x2309,0x230A,0x230B,0x2329,0x232A,0x23B4,0x23B5,0x2768,0x2769,0x276A,0x276B,0x276C,0x276D,0x276E,0x276F,0x2770,0x2771,0x2772,0x2773,0x2774,0x2775,0x27C3,0x27C4,0x27C5,0x27C6,
    0x27D5,0x27D6,0x27DD,0x27DE,0x27E2,0x27E3,0x27E4,0x27E5,0x27E6,0x27E7,0x27E8,0x27E9,0x27EA,0x27EB,0x2983,0x2984,0x2985,0x2986,0x2987,0x2988,0x2989,0x298A,0x298B,0x298C,0x298D,0x2990,
    0x298F,0x298E,0x2991,0x2992,0x2993,0x2994,0x2995,0x2996,0x2997,0x2998,0x29C0,0x29C1,0x29C4,0x29C5,0x29CF,0x29D0,0x29D1,0x29D2,0x29D4,0x29D5,0x29D8,0x29D9,0x29DA,0x29DB,0x29F8,0x29F9,
    0x29FC,0x29FD,0x2A2B,0x2A2C,0x2A2D,0x2A2E,0x2A34,0x2A35,0x2A3C,0x2A3D,0x2A64,0x2A65,0x2A79,0x2A7A,0x2A7D,0x2A7E,0x2A7F,0x2A80,0x2A81,0x2A82,0x2A83,0x2A84,0x2A8B,0x2A8C,0x2A91,0x2A92,
    0x2A93,0x2A94,0x2A95,0x2A96,0x2A97,0x2A98,0x2A99,0x2A9A,0x2A9B,0x2A9C,0x2AA1,0x2AA2,0x2AA6,0x2AA7,0x2AA8,0x2AA9,0x2AAA,0x2AAB,0x2AAC,0x2AAD,0x2AAF,0x2AB0,0x2AB3,0x2AB4,0x2ABB,0x2ABC,
    0x2ABD,0x2ABE,0x2ABF,0x2AC0,0x2AC1,0x2AC2,0x2AC3,0x2AC4,0x2AC5,0x2AC6,0x2ACD,0x2ACE,0x2ACF,0x2AD0,0x2AD1,0x2AD2,0x2AD3,0x2AD4,0x2AD5,0x2AD6,0x2AEC,0x2AED,0x2AF7,0x2AF8,0x2AF9,0x2AFA,
    0x2E02,0x2E03,0x2E04,0x2E05,0x2E09,0x2E0A,0x2E0C,0x2E0D,0x2E1C,0x2E1D,0x2E20,0x2E21,0x2E28,0x2E29,0x3008,0x3009,0x300A,0x300B,0x300C,0x300D,0x300E,0x300F,0x3010,0x3011,0x3014,0x3015,
    0x3016,0x3017,0x3018,0x3019,0x301A,0x301B,0x301D,0x301E,0xFE17,0xFE18,0xFE35,0xFE36,0xFE37,0xFE38,0xFE39,0xFE3A,0xFE3B,0xFE3C,0xFE3D,0xFE3E,0xFE3F,0xFE40,0xFE41,0xFE42,0xFE43,0xFE44,
    0xFE47,0xFE48,0xFE59,0xFE5A,0xFE5B,0xFE5C,0xFE5D,0xFE5E,0xFF08,0xFF09,0xFF1C,0xFF1E,0xFF3B,0xFF3D,0xFF5B,0xFF5D,0xFF5F,0xFF60,0xFF62,0xFF63,0x27EE,0x27EF,0x2E24,0x2E25,0x27EC,0x27ED,
    0x2E22,0x2E23,0x2E26,0x2E27
};
int rk_decode(const char *s, int n, int pos, int *len) {
    const unsigned char *u = (const unsigned char *) s;
    if (pos >= n) { *len = 0; return -1; }
    unsigned c = u[pos];
    if (c < 0x80) { *len = 1; return (int) c; }
    if ((c & 0xE0) == 0xC0 && pos + 1 < n) { *len = 2; return (int) (((c & 0x1F) << 6) | (u[pos + 1] & 0x3F)); }
    if ((c & 0xF0) == 0xE0 && pos + 2 < n) { *len = 3; return (int) (((c & 0x0F) << 12) | ((u[pos + 1] & 0x3F) << 6) | (u[pos + 2] & 0x3F)); }
    if ((c & 0xF8) == 0xF0 && pos + 3 < n) { *len = 4; return (int) (((c & 0x07) << 18) | ((u[pos + 1] & 0x3F) << 12) | ((u[pos + 2] & 0x3F) << 6) | (u[pos + 3] & 0x3F)); }
    *len = 1; return (int) c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cp_at(RkP *p, int pos) { int l; return rk_decode(p->s, p->n, pos, &l); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkList *takelst(RkP *p) { RkList *x = p->lst; p->lst = NULL; return x; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *taketv(RkP *p) { tree_t *t = p->tv; p->tv = NULL; return t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cp_len(RkP *p, int pos) { int l; rk_decode(p->s, p->n, pos, &l); return l ? l : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_mark_cp(int c) {
    return (c >= 0x300 && c <= 0x36F) || (c >= 0x1AB0 && c <= 0x1AFF) || (c >= 0x1DC0 && c <= 0x1DFF) || (c >= 0x20D0 && c <= 0x20FF) || (c >= 0xFE20 && c <= 0xFE2F) || c == 0x200D ||
           (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0xE0100 && c <= 0xE01EF) || (c >= 0x1F3FB && c <= 0x1F3FF);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_digit_cp(int c) {
    if (c >= '0' && c <= '9') return 1;
    if (c < 0x660) return 0;
    static const int z[] = { 0x660,0x6F0,0x7C0,0x966,0x9E6,0xA66,0xAE6,0xB66,0xBE6,0xC66,0xCE6,0xD66,0xDE6,0xE50,0xED0,0xF20,0x1040,0x1090,0x17E0,0x1810,0x1946,0x19D0,0x1A80,0x1A90,
                             0x1B50,0x1BB0,0x1C40,0x1C50,0xA620,0xA8D0,0xA900,0xA9D0,0xA9F0,0xAA50,0xABF0,0xFF10,0x104A0,0x11066,0x1D7CE,0x1D7D8,0x1D7E2,0x1D7EC,0x1D7F6 };
    for (unsigned i = 0; i < sizeof z / sizeof *z; i++) if (c >= z[i] && c <= z[i] + 9) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_space_cp(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == 0x0B || c == 0x0C || c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 ||
           c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_word_sep_at(const char *s, int n, int pos, int *len) {
    int c = rk_decode(s, n, pos, len);
    if (c >= 0x80 && *len == 1) return 0;
    return (c >= 0x09 && c <= 0x0D) || c == 0x20 || c == 0x85 || c == 0x1680 || (c >= 0x2000 && c <= 0x2006) || (c >= 0x2008 && c <= 0x200A) || c == 0x2028 ||
           c == 0x2029 || c == 0x205F || c == 0x3000;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_vspace_cp(int c) { return c == '\n' || c == '\r' || c == 0x0B || c == 0x0C || c == 0x85 || c == 0x2028 || c == 0x2029; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_hspace_cp(int c) { return is_space_cp(c) && !is_vspace_cp(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_word_cp(int c) { return is_alpha_cp(c) || is_digit_cp(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_punct_cp(int c) {
    if (c < 0x80) return (c > ' ' && c < 0x7F) && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
    return !is_word_cp(c) && !is_space_cp(c) && !is_mark_cp(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bracket_index(int c) {
    for (unsigned i = 0; i < sizeof rk_brackets / sizeof *rk_brackets; i++) if (rk_brackets[i] == c) return (int) i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void panic_at(RkP *p, int pos, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); int n = vfmt_len(fmt, ap);
    if (n > p->msgcap) { int nc = p->msgcap * 2 > n ? p->msgcap * 2 : n; p->msg = (char *) ct_grow(p->msg, (size_t) nc); p->msgcap = nc; }
    vsnprintf(p->msg, (size_t) n, fmt, ap); va_end(ap);
    p->err_pos = pos;
    longjmp(p->jb, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void hwm(RkP *p, int pos) { if (pos > p->hw) p->hw = pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ch(RkP *p, int pos) { hwm(p, pos); return pos < p->n ? (unsigned char) p->s[pos] : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int at_lit(RkP *p, int pos, const char *lit) {
    int l = (int) strlen(lit);
    hwm(p, pos);
    return pos + l <= p->n && memcmp(p->s + pos, lit, (size_t) l) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lit(RkP *p, int pos, const char *s) { return at_lit(p, pos, s) ? pos + (int) strlen(s) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int wordch_at(RkP *p, int pos) { return pos >= 0 && pos < p->n && is_word_cp(cp_at(p, pos)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prev_base_cp(RkP *p, int pos) {
    int q = pos;
    for (;;) {
        if (q <= 0) return -1;
        int r = q - 1;
        while (r > 0 && (((unsigned char) p->s[r]) & 0xC0) == 0x80) r--;
        int c = cp_at(p, r);
        if (!is_mark_cp(c)) return c;
        q = r;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int word_boundary_right(RkP *p, int pos) { return !(wordch_at(p, pos)) || !is_word_cp(prev_base_cp(p, pos)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int at_word_end(RkP *p, int pos) { return is_word_cp(prev_base_cp(p, pos)) && !wordch_at(p, pos); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int at_eol(RkP *p, int pos) { return pos >= p->n || is_vspace_cp(cp_at(p, pos)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_ws(RkP *p, int from, int to) { if (to >= 0 && to <= p->n) p->wsmark[to] = from + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int marked_ws_from(RkP *p, int pos) { return (pos >= 0 && pos <= p->n && p->wsmark[pos]) ? p->wsmark[pos] - 1 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_end(RkP *p, int pos) { if (pos >= 0 && pos <= p->n) p->endmark[pos] = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int marked_end(RkP *p, int pos) { return pos >= 0 && pos <= p->n && p->endmark[pos]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkSet cs(const char *chars) { RkSet k = { chars, NULL }; return k; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkSet ck(int (*cls)(int)) { RkSet k = { NULL, cls }; return k; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int in_set(RkSet k, int c) {
    if (c < 0) return 0;
    if (k.cls) return k.cls(c);
    int n = (int) strlen(k.chars);
    for (int i = 0, l = 1; i < n; i += l ? l : 1) if (rk_decode(k.chars, n, i, &l) == c) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int LEN(RkP *p, int pos, int n) { for (; n > 0; n--) { if (pos >= p->n) return -1; pos += cp_len(p, pos); } return pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int LIT(RkP *p, int pos, const char *s) { return lit(p, pos, s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ANY(RkP *p, int pos, RkSet k) { return in_set(k, cp_at(p, pos)) ? pos + cp_len(p, pos) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int NOTANY(RkP *p, int pos, RkSet k) { int c = cp_at(p, pos); return c >= 0 && !in_set(k, c) ? pos + cp_len(p, pos) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int NSPAN(RkP *p, int pos, RkSet k) { while (in_set(k, cp_at(p, pos))) pos += cp_len(p, pos); return pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int SPAN(RkP *p, int pos, RkSet k) { int e = NSPAN(p, pos, k); return e > pos ? e : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int BREAK(RkP *p, int pos, RkSet k) { while (pos < p->n && !in_set(k, cp_at(p, pos))) pos += cp_len(p, pos); return pos < p->n ? pos : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int REM(RkP *p, int pos) { (void) pos; return p->n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ALT(int e, int otherwise) { return e >= 0 ? e : otherwise; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_ident_rest_cp(int c) { return is_word_cp(c) || is_mark_cp(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_pod_ident_cp(int c) { return is_word_cp(c) || c == '-' || c == '\'' || c == ':'; }
const RkOp rk_infix[] = {
    { "**", PR('w'), 0, AS_RIGHT, 0 },
    { "*", PR('u'), 0, AS_LEFT, 0 }, { "\xc3\x97", PR('u'), 0, AS_LEFT, 0 }, { "/", PR('u'), 0, AS_LEFT, 0 }, { "\xc3\xb7", PR('u'), 0, AS_LEFT, 0 },
    { "div", PR('u'), 0, AS_LEFT, OF_WORD }, { "gcd", PR('u'), 0, AS_LEFT, OF_WORD }, { "lcm", PR('u'), 0, AS_LEFT, OF_WORD }, { "%", PR('u'), 0, AS_LEFT, 0 },
    { "mod", PR('u'), 0, AS_LEFT, OF_WORD }, { "%%", PR('u'), 0, AS_LEFT, OF_IFFY }, { "+&", PR('u'), 0, AS_LEFT, 0 }, { "~&", PR('u'), 0, AS_LEFT, 0 }, { "?&", PR('u'), 0, AS_LEFT, OF_IFFY },
    { "+<", PR('u'), 0, AS_LEFT, 0 }, { "+>", PR('u'), 0, AS_LEFT, 0 }, { "~<", PR('u'), 0, AS_LEFT, 0 }, { "~>", PR('u'), 0, AS_LEFT, 0 },
    { "+", PR('t'), 0, AS_LEFT, 0 }, { "-", PR('t'), 0, AS_LEFT, 0 }, { "\xe2\x88\x92", PR('t'), 0, AS_LEFT, 0 }, { "+|", PR('t'), 0, AS_LEFT, 0 }, { "+^", PR('t'), 0, AS_LEFT, 0 },
    { "~|", PR('t'), 0, AS_LEFT, 0 }, { "~^", PR('t'), 0, AS_LEFT, 0 }, { "?|", PR('t'), 0, AS_LEFT, OF_IFFY }, { "?^", PR('t'), 0, AS_LEFT, OF_IFFY },
    { "x", PR('s'), 0, AS_LEFT, OF_WORD }, { "xx", PR('s'), 0, AS_LEFT, OF_WORD },
    { "~", PR('r'), 0, AS_LEFT, 0 }, { "\xe2\x88\x98", PR('r'), 0, AS_LEFT, 0 }, { "o", PR('r'), 0, AS_LEFT, 0 },
    { "&", PR('q'), 0, AS_LIST, OF_IFFY }, { "(&)", PR('q'), 0, AS_LIST, 0 }, { "\xe2\x88\xa9", PR('q'), 0, AS_LIST, 0 }, { "(.)", PR('q'), 0, AS_LIST, 0 }, { "\xe2\x8a\x8d", PR('q'), 0, AS_LIST, 0 },
    { "|", PR('p'), 0, AS_LIST, OF_IFFY }, { "^", PR('p'), 0, AS_LIST, OF_IFFY }, { "(|)", PR('p'), 0, AS_LIST, 0 }, { "\xe2\x88\xaa", PR('p'), 0, AS_LIST, 0 }, { "(^)", PR('p'), 0, AS_LIST, 0 },
    { "\xe2\x8a\x96", PR('p'), 0, AS_LIST, 0 }, { "(+)", PR('p'), 0, AS_LIST, 0 }, { "\xe2\x8a\x8e", PR('p'), 0, AS_LIST, 0 }, { "(-)", PR('p'), 0, AS_LIST, 0 }, { "\xe2\x88\x96", PR('p'), 0,
        AS_LIST, 0 },
    { "..", PR('n'), 0, AS_NON, OF_DIFFY }, { "^..", PR('n'), 0, AS_NON, OF_DIFFY }, { "..^", PR('n'), 0, AS_NON, OF_DIFFY }, { "^..^", PR('n'), 0, AS_NON, OF_DIFFY },
    { "leg", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD }, { "cmp", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD }, { "unicmp", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD },
    { "coll", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD }, { "<=>", PR('n'), 0, AS_NON, OF_DIFFY }, { "but", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD }, { "does", PR('n'), 0, AS_NON, OF_DIFFY | OF_WORD },
    { "=~=", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\x85", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "==", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\xa9\xb5", PR('m'), 0,
        AS_LEFT, OF_CHAIN | OF_IFFY },
    { "!=", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xa0", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "<=", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xa4", PR('m'), 0,
        AS_LEFT, OF_CHAIN | OF_IFFY },
    { ">=", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xa5", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "<", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { ">", PR('m'), 0, AS_LEFT,
        OF_CHAIN | OF_IFFY },
    { "eq", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "ne", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "le", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "ge",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD },
    { "lt", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "gt", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "=:=", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "===", PR('m'), 0,
        AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\xa9\xb6", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "eqv", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "before", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD },
    { "after", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY | OF_WORD }, { "~~", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "!~~", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(elem)", PR('m'), 0,
        AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\x88\x88", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x88\x8a", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x88\x89", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(cont)",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\x88\x8b", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x88\x8d", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x88\x8c", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(<)",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\x8a\x82", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x8a\x84", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(>)", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x8a\x83",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\x8a\x85", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(==)", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xa1", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xa2",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "(<=)", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x8a\x86", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x8a\x88", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(>=)", PR('m'),
        0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "\xe2\x8a\x87", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x8a\x89", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "(<+)", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xbc",
        PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "(>+)", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY }, { "\xe2\x89\xbd", PR('m'), 0, AS_LEFT, OF_CHAIN | OF_IFFY },
    { "&&", PR('l'), 0, AS_LEFT, OF_IFFY }, { "||", PR('k'), 0, AS_LEFT, OF_IFFY }, { "^^", PR('k'), 0, AS_LIST, OF_IFFY }, { "//", PR('k'), 0, AS_LEFT, 0 },
    { "min", PR('k'), 0, AS_LIST, OF_WORD }, { "max", PR('k'), 0, AS_LIST, OF_WORD },
    { "ff", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "^ff", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "ff^", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "^ff^", PR('j'), 0, AS_RIGHT, OF_FIDDLY },
    { "fff", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "^fff", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "fff^", PR('j'), 0, AS_RIGHT, OF_FIDDLY }, { "^fff^", PR('j'), 0, AS_RIGHT, OF_FIDDLY },
    { ":=", PR('i'), PR('e'), AS_RIGHT, OF_FIDDLY }, { "=>", PR('i'), 0, AS_RIGHT, 0 }, { "\xe2\x87\x92", PR('i'), 0, AS_RIGHT, 0 },
    { "\xe2\x9a\x9b=", PR('i'), 0, AS_RIGHT, 0 }, { "\xe2\x9a\x9b+=", PR('i'), 0, AS_RIGHT, 0 }, { "\xe2\x9a\x9b-=", PR('i'), 0, AS_RIGHT, 0 }, { "\xe2\x9a\x9b\xe2\x88\x92=", PR('i'), 0, AS_RIGHT,
        0 },
    { ",", PR('g'), 0, AS_LIST, OF_COMMA },
    { "Z", PR('f'), 0, AS_LIST, 0 }, { "X", PR('f'), 0, AS_LIST, 0 }, { "minmax", PR('f'), 0, AS_LIST, OF_WORD },
    { "...", PR('f'), 0, AS_LIST, 0 }, { "\xe2\x80\xa6", PR('f'), 0, AS_LIST, 0 }, { "...^", PR('f'), 0, AS_LIST, 0 }, { "\xe2\x80\xa6^", PR('f'), 0, AS_LIST, 0 },
    { "^...", PR('f'), 0, AS_LIST, 0 }, { "^\xe2\x80\xa6", PR('f'), 0, AS_LIST, 0 }, { "^...^", PR('f'), 0, AS_LIST, 0 }, { "^\xe2\x80\xa6^", PR('f'), 0, AS_LIST, 0 },
    { "and", PR('d'), 0, AS_LEFT, OF_IFFY | OF_WORD }, { "andthen", PR('d'), 0, AS_LIST, OF_WORD }, { "notandthen", PR('d'), 0, AS_LIST, OF_WORD },
    { "or", PR('c'), 0, AS_LEFT, OF_IFFY | OF_WORD }, { "xor", PR('c'), 0, AS_LIST, OF_IFFY | OF_WORD }, { "orelse", PR('c'), 0, AS_LIST, OF_WORD },
    { "<==", PR('b'), 0, AS_LIST, 0 }, { "==>", PR('b'), 0, AS_LIST, 0 }, { "<<==", PR('b'), 0, AS_LIST, 0 }, { "==>>", PR('b'), 0, AS_LIST, 0 },
    { NULL, 0, 0, 0, 0 }
};
const RkOp rk_prefix[] = {
    { "++", PR('x'), 0, AS_UNARY, 0 }, { "--", PR('x'), 0, AS_UNARY, 0 }, { "++\xe2\x9a\x9b", PR('x'), 0, AS_UNARY, 0 }, { "--\xe2\x9a\x9b", PR('x'), 0, AS_UNARY, 0 },
    { "+", PR('v'), 0, AS_UNARY, 0 }, { "~", PR('v'), 0, AS_UNARY, 0 }, { "-", PR('v'), 0, AS_UNARY, 0 }, { "\xe2\x88\x92", PR('v'), 0, AS_UNARY, 0 }, { "?", PR('v'), 0, AS_UNARY, 0 },
    { "!", PR('v'), 0, AS_UNARY, 0 }, { "|", PR('v'), 0, AS_UNARY, 0 }, { "+^", PR('v'), 0, AS_UNARY, 0 }, { "~^", PR('v'), 0, AS_UNARY, 0 }, { "?^", PR('v'), 0, AS_UNARY, 0 },
    { "^", PR('v'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b", PR('v'), 0, AS_UNARY, 0 },
    { "let", PR('x'), 0, AS_UNARY, OF_WORD }, { "temp", PR('x'), 0, AS_UNARY, OF_WORD }, { "so", PR('h'), 0, AS_UNARY, OF_WORD }, { "not", PR('h'), 0, AS_UNARY, OF_WORD },
    { NULL, 0, 0, 0, 0 }
};
const RkOp rk_postfix[] = {
    { "++", PR('x'), 0, AS_UNARY, 0 }, { "--", PR('x'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b++", PR('x'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b--", PR('x'), 0, AS_UNARY, 0 },
    { "i", PR('y'), 0, AS_UNARY, OF_WORD },
    { NULL, 0, 0, 0, 0 }
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int peek_delims(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c < 0) panic_at(p, pos, "Couldn't find delimiter");
    if (bracket_index(c) < 0 && (!is_punct_cp(c) || c == '_')) {
        if (is_word_cp(c)) panic_at(p, pos, "Alphanumeric character is not allowed as a delimiter");
        if (is_space_cp(c)) panic_at(p, pos, "Whitespace character is not allowed as a delimiter");
    }
    int bi = bracket_index(c);
    L->start = 0; L->stop = c; L->stop2 = 0; L->nrep = 1;
    if (bi >= 0) {
        if (bi % 2) panic_at(p, pos, "Use of a closing delimiter for an opener is reserved");
        L->start = c; L->stop = rk_brackets[bi + 1];
        int q = pos + cp_len(p, pos);
        while (cp_at(p, q) == c) { L->nrep++; q += cp_len(p, q); }
        if (c == 0x201A || c == 0x201B) L->stop2 = 0x2018;
        if (c == 0x201E || c == 0x201F) L->stop2 = 0x201C;
    }
    else if (c == ':') panic_at(p, pos, "Colons may not be used to delimit quoting constructs");
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int match_rep(RkP *p, int pos, int c, int n) {
    for (int i = 0; i < n; i++) { if (cp_at(p, pos) != c) return -1; pos += cp_len(p, pos); }
    if (is_mark_cp(cp_at(p, pos))) return -1;
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int quote_starter(RkP *p, int pos, RkLang *L) { if (!L->start) return -1; return match_rep(p, pos, L->start, L->nrep); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int here_stopper(RkP *p, int pos, RkLang *L) {
    if (!at_bol(p, pos)) return -1;
    int q = pos;
    q = NSPAN(p, q, ck(is_hspace_cp));
    if (q + L->here_len > p->n || memcmp(p->s + q, L->here, (size_t) L->here_len) != 0) return -1;
    q += L->here_len;
    q = NSPAN(p, q, ck(is_hspace_cp));
    if (!at_eol(p, q)) return -1;
    if (q < p->n && p->s[q] == '\r' && q + 1 < p->n && p->s[q + 1] == '\n') return q + 2;
    if (q < p->n) return q + cp_len(p, q);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int quote_stopper(RkP *p, int pos, RkLang *L) {
    if (L->here) return here_stopper(p, pos, L);
    int c = cp_at(p, pos);
    if (is_mark_cp(cp_at(p, pos + cp_len(p, pos)))) return -1;
    if (L->stop2 && c == L->stop2) return pos + cp_len(p, pos);
    if (L->start && L->nrep > 1) return match_rep(p, pos, L->stop, L->nrep);
    if (c == L->stop) return pos + cp_len(p, pos);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int skip_balanced_raw(RkP *p, int pos) {
    RkLang L; memset(&L, 0, sizeof L);
    peek_delims(p, pos, &L);
    int q = quote_starter(p, pos, &L);
    int depth = 1;
    while (q < p->n) {
        int s = quote_stopper(p, q, &L);
        if (s >= 0) { if (--depth == 0) return s; q = s; continue; }
        int t = quote_starter(p, q, &L);
        if (t >= 0) { depth++; q = t; continue; }
        q += cp_len(p, q);
    }
    panic_at(p, pos, "Couldn't find terminator of embedded comment");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_comment(RkP *p, int pos) {
    if (ch(p, pos) != '#') return -1;
    int c1 = cp_at(p, pos + 1);
    if (c1 == '`') {
        int c2 = cp_at(p, pos + 2);
        int bi = bracket_index(c2);
        if (bi >= 0 && bi % 2 == 0) return skip_balanced_raw(p, pos + 2);
        panic_at(p, pos, "Opening bracket required for #` comment");
    }
    if (c1 == '|' || c1 == '=') {
        int c2 = cp_at(p, pos + 2);
        int bi = bracket_index(c2);
        if (bi >= 0 && bi % 2 == 0) return skip_balanced_raw(p, pos + 2);
    }
    return ALT(BREAK(p, pos + 1, ck(is_vspace_cp)), REM(p, pos + 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pod_ident(RkP *p, int pos, int *len) {
    int q = NSPAN(p, pos, ck(is_pod_ident_cp));
    *len = q - pos;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ws_loop(RkP *p, int pos, int allow_heredoc) {
    for (;;) {
        if (pos >= p->n || p->finished) return p->finished ? p->n : pos;
        int c = cp_at(p, pos);
        if (is_vspace_cp(c)) {
            if (c == '\r' && ch(p, pos + 1) == '\n') pos++;
            pos += cp_len(p, pos);
            if (allow_heredoc && p->nhere) pos = r_heredoc_bodies(p, pos);
            if (at_bol(p, pos)) { int e = r_pod(p, pos); if (e >= 0) { pos = e; continue; } }
            continue;
        }
        if (pos == 0 || at_bol(p, pos)) { int e = r_pod(p, pos); if (e >= 0) { pos = e; continue; } }
        if (is_hspace_cp(c)) { pos += cp_len(p, pos); continue; }
        if (c == 0xFEFF && pos == 0) { pos += 3; continue; }
        if (c == '#') { pos = r_comment(p, pos); continue; }
        if (c == '\\') { int e = r_unsp(p, pos); if (e >= 0) { pos = e; continue; } }
        if (c == '<' && at_bol(p, pos) && at_lit(p, pos, "<<<<<<<")) { pos = line_end(p, pos); continue; }
        if (c == '=' && at_bol(p, pos) && at_lit(p, pos, "=======")) {
            int l = line_end(p, pos);
            while (l < p->n && !at_lit(p, l, ">>>>>>>")) l = line_end(p, l);
            pos = line_end(p, l); continue;
        }
        return pos;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_unsp(RkP *p, int pos) {
    if (ch(p, pos) != '\\') return -1;
    int c = cp_at(p, pos + 1);
    if (!(is_space_cp(c) || c == '#')) return -1;
    return ws_loop(p, pos + 1, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_ws(RkP *p, int pos) {
    if (marked_ws_from(p, pos) >= 0) return pos;
    if (wordch_at(p, pos) && is_word_cp(prev_base_cp(p, pos)) && pos > 0) return -1;
    int e = ws_loop(p, pos, 1);
    if (marked_ws_from(p, e) < 0 || e != pos) mark_ws(p, pos, e);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ws(RkP *p, int pos) { int e = r_ws(p, pos); return e < 0 ? pos : e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int had_ws_before(RkP *p, int pos) { int f = marked_ws_from(p, pos); return f >= 0 && f < pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int hs(RkP *p, int pos) { return NSPAN(p, pos, ck(is_hspace_cp)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_ENDSTMT(RkP *p, int pos) {
    int q = hs(p, pos);
    if (ch(p, q) == '#') { int e = r_comment(p, q); if (e >= 0 && ch(p, q + 1) != '`') q = e; else if (e >= 0) { q = hs(p, e); } }
    if (!at_eol(p, q)) return pos;
    int e = ws(p, q);
    mark_end(p, e);
    return e;
}
int r_ident(RkP *p, int pos) {
    hwm(p, pos);
    int e = ANY(p, pos, ck(is_alpha_cp));
    return e < 0 ? -1 : NSPAN(p, e, ck(is_ident_rest_cp));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_identifier(RkP *p, int pos) {
    int e = r_ident(p, pos);
    for (int d; e >= 0 && (d = ANY(p, e, cs("'-"))) >= 0 && ANY(p, d, ck(is_alpha_cp)) >= 0; ) e = r_ident(p, d);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int end_keyword_ok(RkP *p, int pos) {
    if (wordch_at(p, pos)) return 0;
    int c = ch(p, pos);
    if (c == '(' || c == '\\' || c == '\'' || c == '-') return 0;
    int q = hs(p, pos);
    if (at_lit(p, q, "=>")) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kok(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0 || !end_keyword_ok(p, e)) return -1;
    int c = cp_at(p, e);
    if (is_space_cp(c) || c == '#') return ws(p, e);
    if (is_name_n(p, w, (int) strlen(w))) return -1;
    panic_at(p, e, "Whitespace required after keyword '%s'", w);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kw_end(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0 || !end_keyword_ok(p, e)) return -1;
    return e;
}
int r_digits(RkP *p, int pos, RkSet k) {
    int e = SPAN(p, pos, k);
    for (int u; e >= 0 && (u = LIT(p, e, "_")) >= 0 && (u = SPAN(p, u, k)) >= 0; ) e = u;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_decint(RkP *p, int pos) { return r_digits(p, pos, ck(is_digit_cp)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_integer(RkP *p, int pos) {
    int e = -1, z = LIT(p, pos, "0"), q = z >= 0 ? ANY(p, z, cs("bodx")) : -1;
    if (q >= 0) { int c = ch(p, z); e = r_baseint(p, ALT(LIT(p, q, "_"), q), c == 'b' ? 2 : c == 'o' ? 8 : c == 'x' ? 16 : 10); }
    if (e < 0) e = r_decint(p, pos);
    if (e < 0) return -1;
    if (ch(p, e) == '.' && ch(p, e + 1) != '.') {
        int c = cp_at(p, e + 1);
        if (e + 1 >= p->n || is_space_cp(c) || c == ',' || c == '=' || c == ';' || c == ')' || c == ']' || c == '}') panic_at(p, e, "Decimal point must be followed by digit");
    }
    if (ch(p, e) == '_' && ch(p, e + 1) == '_') panic_at(p, e, "Only isolated underscores are allowed inside numbers");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_escale(RkP *p, int pos) {
    int q = ANY(p, pos, cs("eE"));
    return q < 0 ? -1 : r_decint(p, ALT(ANY(p, q, cs("+-\xe2\x88\x92")), q));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_dec_number(RkP *p, int pos) {
    int f = LIT(p, pos, ".");
    if (f >= 0) { f = r_decint(p, f); return f < 0 ? -1 : ALT(r_escale(p, f), f); }
    int i = r_decint(p, pos);
    if (i < 0) return -1;
    if ((f = LIT(p, i, ".")) >= 0 && ANY(p, f, ck(is_digit_cp)) >= 0) { f = r_decint(p, f); return ALT(r_escale(p, f), f); }
    return r_escale(p, i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_rad_number(RkP *p, int pos) {
    if (ch(p, pos) != ':' || !is_digit_cp(cp_at(p, pos + 1))) return -1;
    int q = r_decint(p, pos + 1);
    int u = r_unsp(p, q); if (u >= 0) q = u;
    int c = ch(p, q);
    if (c == '<') {
        int r = q + 1;
        while (r < p->n && p->s[r] != '>') { if (p->s[r] == '\n') break; r++; }
        if (ch(p, r) != '>') panic_at(p, q, "Malformed radix number");
        return r + 1;
    }
    if (c == '[' || c == '(') { int f = r_circumfix(p, q); if (f >= 0 && p->build) rkb_set_rad_inner(p->B, p->tm.t); return f; }
    panic_at(p, q, "Malformed radix number");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_numish(RkP *p, int pos) {
    int e;
    if ((e = kw(p, pos, "NaN")) >= 0) return e;
    if ((e = kw(p, pos, "Inf")) >= 0) return e;
    if (at_lit(p, pos, "\xe2\x88\x9e")) return pos + 3;
    int d = r_dec_number(p, pos);
    int i = r_integer(p, pos);
    if (d >= 0 && d > i) return d;
    if (i >= 0) return i;
    if ((e = r_rad_number(p, pos)) >= 0) return e;
    int c = cp_at(p, pos);
    if (c >= 0x80 && is_numeric_other_cp(c)) return pos + cp_len(p, pos);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_version(RkP *p, int pos) {
    if (ch(p, pos) != 'v' || !is_digit_cp(cp_at(p, pos + 1))) return -1;
    int q = pos + 1;
    for (;;) {
        int s = LIT(p, q, "*");
        q = s >= 0 ? s : SPAN(p, q, ck(is_word_cp));
        if (q < 0) return -1;
        if (ch(p, q) == '.' && (is_word_cp(cp_at(p, q + 1)) || ch(p, q + 1) == '*')) { q++; continue; }
        break;
    }
    if (ch(p, q) == '+') q++;
    if (ch(p, q) == '-' || ch(p, q) == '\'') return -1;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_heredoc_bodies(RkP *p, int pos) {
    while (p->nhere) {
        RkHere h = p->here[0];
        memmove(&p->here[0], &p->here[1], sizeof(RkHere) * (size_t) (p->nhere - 1));
        p->nhere--;
        RkLang L = h.lang;
        L.here = h.delim; L.here_len = h.dlen; L.start = 0;
        RkClosure *sq = p->qc; int sn = p->nqc, sc = p->cqc;
        p->qc = NULL; p->nqc = 0; p->cqc = 0;
        int e; r_nibble_until(p, pos, &L, &e);
        if (e < 0) panic_at(p, pos, "Ending delimiter %s not found", h.delim);
        int s = here_stopper(p, e, &L);
        if (s < 0) panic_at(p, pos, "Ending delimiter %s not found", h.delim);
        int icols = 0;
        if (p->build) for (int ic = e, ie = NSPAN(p, e, ck(is_hspace_cp)); ic < ie; ic++) icols = p->s[ic] == '\t' ? (icols / 8 + 1) * 8 : icols + 1;
        if (p->build && h.node) rkb_heredoc_body(p->B, h.node, pos, e, icols, L.sc || L.clos, p->qc, p->nqc);
        p->qc = sq; p->nqc = sn; p->cqc = sc;
        pos = s;
    }
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rx_mod_internal(RkP *p, int pos) {
    int q = pos + 1;
    if (ch(p, q) == '!') q++;
    else if (is_digit_cp(cp_at(p, q))) q = r_decint(p, q);
    int e = SPAN(p, q, ck(is_word_cp));
    if (e < 0) return -1;
    int n = e - q; const char *w = p->s + q;
    int ok = (n == 1 && strchr("imrs", w[0])) || (n == 10 && !memcmp(w, "ignorecase", 10)) || (n == 10 && !memcmp(w, "ignoremark", 10)) || (n == 7 && !memcmp(w, "ratchet", 7)) ||
             (n == 8 && !memcmp(w, "sigspace", 8)) || (n == 3 && !memcmp(w, "dba", 3));
    if (!ok) panic_at(p, pos, "Unrecognized regex modifier :%.*s", n, w);
    if (ch(p, e) == '(') {
        int t = e + 1;
        if (is_digit_cp(cp_at(p, t))) t = r_decint(p, t); else t = r_quote(p, t);
        if (t < 0 || ch(p, t) != ')') panic_at(p, e, "Malformed regex modifier argument");
        return t + 1;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rks_rx_atom(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (is_word_cp(c)) { return NSPAN(p, pos + cp_len(p, pos), ck(is_mark_cp)); }
    return rx_metachar(p, pos, L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_stmt_mod_kw(RkP *p, int pos) {
    static const char *const w[] = { "if", "unless", "while", "until", "for", "given", "when", "with", "without", 0 };
    for (int i = 0; w[i]; i++) { int e = lit(p, pos, w[i]); if (e >= 0 && end_keyword_ok(p, e)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_terminator_at(RkP *p, int pos) {
    int c = ch(p, pos);
    if (p->ustop && pos + p->ustop_len <= p->n && !memcmp(p->s + pos, p->ustop, (size_t) p->ustop_len)) return 1;
    if (c == ';' || c == ')' || c == ']' || c == '}') return 1;
    if (c == '>' && p->in_regex_assert) return 1;
    if (at_lit(p, pos, "-->")) return 1;
    return is_stmt_mod_kw(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stdstopper(RkP *p, int pos) { return marked_end(p, pos) || pos >= p->n || is_terminator_at(p, pos); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int at_lambda(RkP *p, int pos) { return at_lit(p, pos, "->") || at_lit(p, pos, "<->"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int infixstopper(RkP *p, int pos) {
    if (p->goal == GOAL_BANGBANG && at_lit(p, pos, "!!")) return 1;
    if ((ch(p, pos) == '{' || at_lambda(p, pos)) && had_ws_before(p, pos) && (p->goal == GOAL_BLOCK || p->goal == GOAL_ENDARGS)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int infix_in_term_position(RkP *p, int pos) {
    if (p->qsigil || stdstopper(p, pos)) return 0;
    OpInfo o;
    int sp = p->invocant_ok; int sl = p->leftsigil; int sg = p->goal;
    int e = -1;
    int c = ch(p, pos);
    if (c == ',' || c == '=' || (c == '*' && ch(p, pos + 1) == '*' && 0) || c == '/' || c == '?' || c == '&' || c == '|' || c == '<' || c == '>' || c == '%' || c == 'x' || c == 'o' || c == 'a' ||
        c == '.' || c == '!' || c == '~' || c == '+' || c == '-' || c == '^' || c == '=' || c == 'e' || c == 'n' || c == 'l' || c == 'g' || c == 'c' || c == 'X' || c == 'Z' || c == 'R' || c == 'S' ||
        c == 'd' || c == 'm' || c == 'b' || c == 'f' || c == 'u' || c >= 0x80) {
        jmp_buf save; memcpy(save, p->jb, sizeof save);
        size_t ml = p->msg ? strlen(p->msg) + 1 : 1; char msg[ml]; memcpy(msg, p->msg ? p->msg : "", ml); int ep = p->err_pos;
        if (!setjmp(p->jb)) { e = r_infix_plain(p, pos, &o); }
        else e = -1;
        memcpy(p->jb, save, sizeof save); if (p->msg) memcpy(p->msg, msg, ml); p->err_pos = ep;
    }
    p->invocant_ok = sp; p->leftsigil = sl; p->goal = sg;
    if (e < 0) return 0;
    if (c == '.' || c == '<' || c == '-' || c == '+' || c == '!' || c == '~' || c == '^' || c == '?' || c == '|' || c == '&' || c == '%' || c == '/') {
        if (e == pos + 1 && (c == '.' || c == '<' || c == '-' || c == '+' || c == '!' || c == '~' || c == '^' || c == '?' || c == '|' || c == '&' || c == '%' || c == '/')) return 0;
    }
    if (is_alpha_cp(c)) { if (!end_keyword_ok(p, e) && wordch_at(p, e)) return 0; if (!(o.flags & OF_WORD) && c != 'X' && c != 'Z' && c != 'o') return 0; return c != 'X' && c != 'Z' && c != 'R' &&
        c != 'S' && c != 'o' ? 1 : 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int autoincrement_postfix_at(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (at_lit(p, pos, "++") || at_lit(p, pos, "--") || at_lit(p, pos, "\xe2\x9a\x9b++") || at_lit(p, pos, "\xe2\x9a\x9b--")) return 1;
    return c == 0x207B || c == 0x207A || c == 0xAF || c == 0x2070 || c == 0xB9 || c == 0xB2 || c == 0xB3 || (c >= 0x2074 && c <= 0x2079);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_termish(RkP *p, int pos) {
    int q = pos; int npre = 0; int lastpre = pos;
    int prec; int lastprec = 0;
    int *pf_ = NULL, *pt_ = NULL; int cpf = 0, cpt = 0;
    for (;;) {
        int e = r_prefixish(p, q, &prec);
        if (e < 0) break;
        RK_GROW(pf_, npre, cpf, int); RK_GROW(pt_, npre, cpt, int); pf_[npre] = q; pt_[npre] = e;
        lastpre = q; q = e; npre++; lastprec = prec;
    }
    int t = r_term(p, q);
    RkTerm it; memset(&it, 0, sizeof it);
    if (t >= 0 && p->build) { it = p->tm; for (int i = 0; i < npre; i++) rkb_prefix(&it, p->s + pf_[i], pt_[i] - pf_[i]); }
    if (t < 0) {
        if (npre) {
            int c = ch(p, lastpre);
            if (c == '|' || c == '~' || c == '+' || c == '-' || c == '?' || c == '!' || c == '^') {
                int k = lastpre; int e2 = r_prefixish(p, k, &prec); (void) e2;
            }
            panic_at(p, q, "Prefix %.*s requires an argument, but no valid term found", cp_len(p, lastpre), p->s + lastpre);
        }
        if (!p->qsigil && infix_in_term_position(p, pos)) panic_at(p, pos, "Preceding context expects a term, but found infix %.*s instead", cp_len(p, pos), p->s + pos);
        return -1;
    }
    if (p->qsigil) {
        int base = p->nends; int r = t;
        for (;;) { int e = r_postfixish(p, r); if (e < 0) break; r = e; RK_GROW(p->ends, p->nends, p->cends, int); p->ends[p->nends++] = e; }
        int k = p->nends - 1;
        while (k >= base && !bracket_ending_at(p, p->ends[k])) k--;
        int res = k >= base ? p->ends[k] : (p->qsigil == '$' ? t : -1);
        p->nends = base;
        if (p->build && res >= 0) {
            int r2 = t;
            for (;;) {
                p->x_sub = !it.npost && it.kind == TK_VAR && it.cls == 'A';
                int e = r_postfixish(p, r2);
                p->x_sub = 0;
                if (e < 0 || e > res) break;
                rkb_postfix(p->B, &it, &p->pf);
                r2 = e;
            }
            p->tm = it;
        }
        return res;
    }
    int tq = q;
    for (;;) {
        p->x_sub = p->build && !it.npost && it.kind == TK_VAR && it.cls == 'A';
        int e = r_postfixish(p, t);
        p->x_sub = 0;
        if (e < 0) break;
        if (p->build) rkb_postfix(p->B, &it, &p->pf);
        if (npre && lastprec == PR('x') && autoincrement_postfix_at(p, t))
            panic_at(p, t, "Operators '%.*s' and '%.*s' are non-associative and require parentheses", tq - lastpre, p->s + lastpre, e - t, p->s + t);
        t = e; p->leftsigil = '@';
    }
    if (p->build) p->tm = it;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_arglist(RkP *p, int pos) {
    int sg = p->goal; int sq = p->qsigil;
    p->goal = GOAL_ENDARGS; p->qsigil = 0;
    int q = ws(p, pos);
    int e = q;
    RkList *a = NULL;
    if (!stdstopper(p, q)) { int x = r_EXPR(p, q, PR('e')); if (x >= 0) { e = x; a = takelst(p); } }
    p->goal = sg; p->qsigil = sq;
    p->lst = a;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_semiarglist(RkP *p, int pos) {
    int q = r_arglist(p, pos);
    RkList *a = takelst(p);
    for (;;) { int t = ws(p, q); if (ch(p, t) != ';') { q = t; break; } q = r_arglist(p, t + 1); }
    p->lst = a;
    return ws(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_args(RkP *p, int pos, int invocant_ok) {
    const char *su = p->x_subst; int sul = p->x_subst_len; p->x_subst = NULL;
    int si = p->invocant_ok; int sg = p->goal; int sv = p->saw_inv;
    p->invocant_ok = invocant_ok; p->goal = 0; p->saw_inv = 0;
    int e = pos;
    RkList *a = NULL; int form = 0;
    if (ch(p, pos) == '(') { int q = r_semiarglist(p, pos + 1); a = takelst(p); form = 1; if (ch(p, q) != ')') panic_at(p, q,
        "Unable to parse argument list; couldn't find final ')' (corresponding starter was at line %d)", line_of(p, pos)); e = q + 1; }
    else {
        int u = r_unsp(p, pos);
        if (u >= 0 && ch(p, u) == '(') { int q = r_semiarglist(p, u + 1); a = takelst(p); form = 1; if (ch(p, q) != ')') panic_at(p, q, "Unable to parse argument list; couldn't find final ')'");
            e = q + 1; }
        else if (is_space_cp(cp_at(p, pos))) {
            p->x_subst = su; p->x_subst_len = sul;
            e = r_arglist(p, pos + cp_len(p, pos));
            p->x_subst = NULL; a = takelst(p); form = a ? 2 : 0;
        }
    }
    p->last_inv = p->saw_inv;
    p->invocant_ok = si; p->goal = sg; p->saw_inv = sv;
    p->lst = a; p->args_form = form;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_methodop(RkP *p, int pos) {
    int q = -1;
    int c = cp_at(p, pos);
    RkList *ma = NULL; int mform = 0;
    tree_t *qname = NULL;
    if (c == '$' || c == '@' || c == '&') q = r_variable(p, pos);
    else if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201C || c == 0xFF62) {
        if (p->qsigil && c == '"') { int t = pos + 1; while (t < p->n && p->s[t] != '"' && !is_space_cp(cp_at(p, t))) t += cp_len(p, t); if (t >= p->n || p->s[t] != '"') return -1; }
        q = r_quote(p, pos);
        if (q >= 0 && p->build) qname = p->tm.t;
        if (q >= 0 && !(ch(p, q) == '(' || at_lit(p, q, ".(") || ch(p, q) == '\\')) panic_at(p, q,
            "Quoted method name requires parenthesized arguments. If you meant to concatenate two strings, use '~'.");
    }
    else {
        q = r_longname(p, pos);
        if (q >= 0 && q - pos == 2 && at_lit(p, pos, "::")) panic_at(p, pos, "Malformed class-qualified postfix call");
    }
    if (q < 0) return -1;
    int name_end = q;
    int u = r_unsp(p, q); if (u >= 0) q = u;
    if (ch(p, q) == '(') { q = r_args(p, q, 0); ma = takelst(p); mform = 1; }
    else if (ch(p, q) == ':' && (is_space_cp(cp_at(p, q + 1)) || ch(p, q + 1) == '{') && !p->qsigil) { q = r_arglist(p, q + 1); ma = takelst(p); mform = 2; }
    else if (p->qsigil && ch(p, q) != '.') return -1;
    u = r_unsp(p, q); if (u >= 0) q = u;
    if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = 'M'; p->pf.from = pos; p->pf.to = q; p->pf.txt = ct_strndup0(p->s + pos, name_end - pos); p->pf.args = ma; p->pf.form = mform; p->pf.qname = qname; }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int regex_quibble(RkP *p, int pos, int p5) {
    RkLang L; memset(&L, 0, sizeof L); L.regex = 1;
    peek_delims(p, pos, &L);
    int s = L.start ? quote_starter(p, pos, &L) : pos + cp_len(p, pos);
    int e;
    if (p5) e = p5_regex_body(p, s, &L); else e = r_regex_nibbler(p, s, &L);
    int t = quote_stopper(p, e, &L);
    if (t < 0) panic_at(p, e, "Unable to parse regex; couldn't find final '%.*s'", cp_len(p, e), p->s + e);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cc_nibble(RkP *p, int pos, RkLang *L) {
    int q = pos;
    for (;;) {
        if (q >= p->n) return -1;
        if (quote_stopper(p, q, L) >= 0) return q;
        int c = ch(p, q);
        if (is_space_cp(cp_at(p, q))) { q += cp_len(p, q); int h = NSPAN(p, q, ck(is_space_cp)); if (ch(p, h) == '#') q = ws(p, h); continue; }
        if (c == '#') panic_at(p, q, "Please backslash # for literal char or put whitespace in front for comment");
        if (c == '\\') { int e = qq_backslash(p, q + 1, L); if (e < 0) e = q + 2; q = e; continue; }
        q += cp_len(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_capterm(RkP *p, int pos) {
    if (ch(p, pos) != '\\') return -1;
    int q = pos + 1;
    if (ch(p, q) == '(') { int e = r_semiarglist(p, q + 1); if (ch(p, e) != ')') panic_at(p, e, "Unable to parse capture; couldn't find final ')'"); return e + 1; }
    if (NOTANY(p, q, ck(is_space_cp)) >= 0) { int e = r_termish(p, q); if (e >= 0) return e; }
    panic_at(p, pos, "You can't backslash that");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_fatarrow(RkP *p, int pos) {
    int e = r_identifier(p, pos);
    if (e < 0) return -1;
    int h = hs(p, e);
    if (!at_lit(p, h, "=>")) return -1;
    int q = ws(p, h + 2);
    int v = r_EXPR(p, q, PRLE('i'));
    if (v < 0) panic_at(p, q, "Missing value after =>");
    if (p->build) rkb_fatarrow(p->B, &p->tm, pos, v, pos, e, takelst(p));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sig_end_at(RkP *p, int pos) {
    int c = ch(p, pos);
    return at_lit(p, pos, "-->") || c == ')' || c == ']' || c == '{' || (c == ':' && is_space_cp(cp_at(p, pos + 1))) || at_lit(p, pos, ";;");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_onlystar(RkP *p, int pos) {
    if (!p->in_proto && p->multiness != 2) return -1;
    if (ch(p, pos) != '{') return -1;
    int q = ws(p, pos + 1);
    if (ch(p, q) != '*') return -1;
    q = ws(p, q + 1);
    if (ch(p, q) != '}') return -1;
    return r_ENDSTMT(p, q + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_eat_terminator(RkP *p, int pos) {
    int q = ws(p, pos);
    if (ch(p, q) == ';') return q + 1;
    if (marked_end(p, pos) || marked_end(p, q)) return q;
    if (p->ustop && q + p->ustop_len <= p->n && !memcmp(p->s + q, p->ustop, (size_t) p->ustop_len)) return q;
    int c = ch(p, q);
    if (c == ')' || c == ']' || c == '}' || q >= p->n) return q;
    if (is_terminator_at(p, q) && !is_stmt_mod_kw(p, q)) return q;
    static const char *const kws[] = { "if", "while", "for", "loop", "repeat", "given", "when", 0 };
    for (int i = 0; kws[i]; i++) { int e = lit(p, q, kws[i]); if (e >= 0 && !wordch_at(p, e)) panic_at(p, q, "Confused: Missing semicolon"); }
    panic_at(p, q, "Confused");
    return -1;
}
