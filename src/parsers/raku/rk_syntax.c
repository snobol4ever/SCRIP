#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>
#include "ct_arena.h"
#include "rk_syntax.h"
#include "rk_core_names.h"
/*====================================================================================================================================================================================================*/
enum { AS_LEFT = 1, AS_RIGHT, AS_NON, AS_LIST, AS_UNARY };
enum { OF_FIDDLY = 1, OF_IFFY = 2, OF_DIFFY = 4, OF_CHAIN = 8, OF_FAKE = 16, OF_DOTTY = 32, OF_COMMA = 64, OF_WORD = 128 };
#define PR(c) (((c) - 'a') * 4 + 2)
#define RK_GROW(arr, n, cap, type) do { if ((n) >= (cap)) { (cap) = (cap) ? (cap) * 2 : 16; (arr) = (type *) ct_grow((arr), sizeof(type) * (size_t) (cap)); } } while (0)
#define PRLE(c) (((c) - 'a') * 4 + 1)
typedef struct { const char *sym; int prec; int sub; unsigned char assoc; unsigned char flags; } RkOp;
typedef struct { char *sym; int len; int prec; int depth; char cat; int assoc; unsigned flags; int wordend; } RkUserOp;
typedef struct { char *name; int depth; int value; } RkName;
typedef struct { int pos; int len; int depth; } RkMyst;
typedef struct { char **v; int n; int c; } RkStrs;
typedef struct { const char *kind; char *name; RkStrs attrs; RkStrs roles; RkMyst *uses; int nuses; int cuses; int unknown; } RkPkg;
typedef struct RkLang {
    int regex; int cc; int words; int ww;
    int bs; int qbs; int clos; int sc; int ar; int hs; int fn;
    int start; int stop; int stop2; int nrep;
    const char *here; int here_len;
} RkLang;
typedef struct { char *delim; int dlen; RkLang lang; } RkHere;
typedef struct { int prec; int sub; int assoc; int from; int to; } StackOp;
typedef struct RkP {
    const char *s; int n; const char *file;
    jmp_buf jb; char msg[400]; int err_pos;
    int hw;
    int *wsmark; unsigned char *endmark;
    int goal; int qsigil; int in_meta; int in_reduce; int invocant_ok; int in_decl; int leftsigil; int in_proto; int multiness;
    int has_self; int scope; int in_regex_assert;
    const char *ustop; int ustop_len;
    int p5isms;
    char **libs; int nlibs; int clibs; int load_depth;
    char **decls; int ndecls; int cdecls;
    RkHere *here; int nhere; int chere;
    StackOp *ops; int nops; int cops;
    int *ends; int nends; int cends;
    RkUserOp *uops; int nuops; int cuops;
    RkName *names; int nnames; int cnames;
    int depth;
    char *pkg;
    int finished;
    int comp_unit_begin;
    RkMyst *myst; int nmyst; int cmyst; int myst_off; int saw_inv; int last_inv; int lax; int quote_block; int lang_e;
    RkPkg *pkgs; int npkgs; int cpkgs;
    RkPkg *roledb; int nroledb; int croledb;
    char **exports; int nexports; int cexports;
} RkP;
static const int rk_brackets[] = {
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
/*====================================================================================================================================================================================================*/
static int rk_decode(const char *s, int n, int pos, int *len) {
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
static int cp_at(RkP *p, int pos) { int l; return rk_decode(p->s, p->n, pos, &l); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cp_len(RkP *p, int pos) { int l; rk_decode(p->s, p->n, pos, &l); return l ? l : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_mark_cp(int c) {
    return (c >= 0x300 && c <= 0x36F) || (c >= 0x1AB0 && c <= 0x1AFF) || (c >= 0x1DC0 && c <= 0x1DFF) || (c >= 0x20D0 && c <= 0x20FF) || (c >= 0xFE20 && c <= 0xFE2F) || c == 0x200D ||
           (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0xE0100 && c <= 0xE01EF) || (c >= 0x1F3FB && c <= 0x1F3FF);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_digit_cp(int c) {
    if (c >= '0' && c <= '9') return 1;
    if (c < 0x660) return 0;
    static const int z[] = { 0x660,0x6F0,0x7C0,0x966,0x9E6,0xA66,0xAE6,0xB66,0xBE6,0xC66,0xCE6,0xD66,0xDE6,0xE50,0xED0,0xF20,0x1040,0x1090,0x17E0,0x1810,0x1946,0x19D0,0x1A80,0x1A90,
                             0x1B50,0x1BB0,0x1C40,0x1C50,0xA620,0xA8D0,0xA900,0xA9D0,0xA9F0,0xAA50,0xABF0,0xFF10,0x104A0,0x11066,0x1D7CE,0x1D7D8,0x1D7E2,0x1D7EC,0x1D7F6 };
    for (unsigned i = 0; i < sizeof z / sizeof *z; i++) if (c >= z[i] && c <= z[i] + 9) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_space_cp(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == 0x0B || c == 0x0C || c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 ||
           c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_vspace_cp(int c) { return c == '\n' || c == '\r' || c == 0x0B || c == 0x0C || c == 0x85 || c == 0x2028 || c == 0x2029; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_hspace_cp(int c) { return is_space_cp(c) && !is_vspace_cp(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_alpha_cp(int c) {
    if (c < 0) return 0;
    if (c < 0x80) return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    if (c < 0xC0) return c == 0xAA || c == 0xB5 || c == 0xBA;
    if (c == 0xD7 || c == 0xF7) return 0;
    if (is_digit_cp(c) || is_space_cp(c) || is_mark_cp(c)) return 0;
    if (c >= 0x2B9 && c <= 0x2FF) return c <= 0x2C1 || (c >= 0x2C6 && c <= 0x2D1) || (c >= 0x2E0 && c <= 0x2E4) || c == 0x2EC || c == 0x2EE;
    if (c == 0x37E || c == 0x387 || c == 0x3F6 || c == 0x482 || (c >= 0x55A && c <= 0x55F) || c == 0x589 || c == 0x58A || c == 0x5BE || c == 0x5C0 || c == 0x5C3 || c == 0x5C6) return 0;
    if ((c >= 0x600 && c <= 0x60F) || c == 0x61B || c == 0x61F || (c >= 0x66A && c <= 0x66D) || c == 0x6D4 || c == 0x6DD || c == 0x6DE || c == 0x6E9) return 0;
    if (c >= 0x964 && c <= 0x965) return 0;
    if (c == 0x1680 || c == 0x169B || c == 0x169C) return 0;
    if (c >= 0x2000 && c <= 0x206F) return 0;
    if (c >= 0x2070 && c <= 0x209F) return c == 0x2071 || c == 0x207F || (c >= 0x2090 && c <= 0x209C);
    if (c >= 0x20A0 && c <= 0x20CF) return 0;
    if (c >= 0x2100 && c <= 0x214F) {
        static const int let[] = { 0x2102,0x2107,0x210A,0x210B,0x210C,0x210D,0x210E,0x210F,0x2110,0x2111,0x2112,0x2113,0x2115,0x2119,0x211A,0x211B,0x211C,0x211D,0x2124,0x2126,0x2128,0x212A,0x212B,
                                   0x212C,0x212D,0x212F,0x2130,0x2131,0x2132,0x2133,0x2134,0x2135,0x2136,0x2137,0x2138,0x2139,0x213C,0x213D,0x213E,0x213F,0x2145,0x2146,0x2147,0x2148,0x2149,0x214E };
        for (unsigned i = 0; i < sizeof let / sizeof *let; i++) if (let[i] == c) return 1;
        return 0;
    }
    if (c >= 0x2150 && c <= 0x218F) return (c >= 0x2160 && c <= 0x2188) ? 0 : 0;
    if (c >= 0x2190 && c <= 0x2BFF) return 0;
    if (c >= 0x2E00 && c <= 0x2E7F) return 0;
    if (c >= 0x3000 && c <= 0x303F) return c == 0x3005 || c == 0x3006 || c == 0x3031 || c == 0x3032 || c == 0x3033 || c == 0x3034 || c == 0x3035 || c == 0x303B || c == 0x303C;
    if (c >= 0x30A0 && c <= 0x30FF) return c != 0x30A0 && c != 0x30FB;
    if (c >= 0xFD3E && c <= 0xFD3F) return 0;
    if (c >= 0xFE10 && c <= 0xFE6F) return 0;
    if (c >= 0xFF00 && c <= 0xFF65) return (c >= 0xFF21 && c <= 0xFF3A) || (c >= 0xFF41 && c <= 0xFF5A);
    if (c >= 0xFFF0 && c <= 0xFFFF) return 0;
    if (c >= 0xE000 && c <= 0xF8FF) return 0;
    if (c >= 0x1F000 && c <= 0x1FBFF) return 0;
    if (c >= 0x1D400 && c <= 0x1D7CB) return 1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_word_cp(int c) { return is_alpha_cp(c) || is_digit_cp(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_numeric_other_cp(int c) {
    return (c >= 0xB2 && c <= 0xB3) || c == 0xB9 || (c >= 0xBC && c <= 0xBE) || (c >= 0x2070 && c <= 0x2079 && c != 0x2071 && c != 0x2072 && c != 0x2073) || (c >= 0x2080 && c <= 0x2089) ||
           (c >= 0x2150 && c <= 0x2189) || (c >= 0x2460 && c <= 0x249B) || (c >= 0x24EA && c <= 0x24FF) || (c >= 0x2776 && c <= 0x2793) || (c >= 0x3007 && c <= 0x3007) ||
           (c >= 0x3021 && c <= 0x3029) || (c >= 0x3038 && c <= 0x303A) || (c >= 0x3192 && c <= 0x3195) || (c >= 0x3220 && c <= 0x3229) || (c >= 0x3248 && c <= 0x324F) ||
           (c >= 0x3251 && c <= 0x325F) || (c >= 0x3280 && c <= 0x3289) || (c >= 0x32B1 && c <= 0x32BF) || (c >= 0x10107 && c <= 0x10133) || (c >= 0x10140 && c <= 0x10178) ||
           (c >= 0x12400 && c <= 0x1246E) || (c >= 0x1369 && c <= 0x137C) || (c >= 0x1F100 && c <= 0x1F10C);
}
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
static int opener_close(int c) {
    for (unsigned i = 0; i + 1 < sizeof rk_brackets / sizeof *rk_brackets; i += 2) if (rk_brackets[i] == c) return rk_brackets[i + 1];
    return 0;
}
/*====================================================================================================================================================================================================*/
static int line_of(RkP *p, int pos) {
    int line = 1;
    for (int i = 0; i < pos && i < p->n; i++) if (p->s[i] == '\n') line++;
    return line;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void panic_at(RkP *p, int pos, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vsnprintf(p->msg, sizeof p->msg, fmt, ap); va_end(ap);
    p->err_pos = pos;
    longjmp(p->jb, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void hwm(RkP *p, int pos) { if (pos > p->hw) p->hw = pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ch(RkP *p, int pos) { hwm(p, pos); return pos < p->n ? (unsigned char) p->s[pos] : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int at_lit(RkP *p, int pos, const char *lit) {
    int l = (int) strlen(lit);
    hwm(p, pos);
    return pos + l <= p->n && memcmp(p->s + pos, lit, (size_t) l) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int lit(RkP *p, int pos, const char *s) { return at_lit(p, pos, s) ? pos + (int) strlen(s) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int asc_space(int b) { return b == ' ' || b == '\t' || b == '\n' || b == '\r' || b == '\f' || b == 0x0B; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int asc_word(int b) { return (b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z') || (b >= '0' && b <= '9') || b == '_' || b >= 0x80; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int wordch_at(RkP *p, int pos) { return pos >= 0 && pos < p->n && is_word_cp(cp_at(p, pos)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prev_cp(RkP *p, int pos) {
    if (pos <= 0) return -1;
    int q = pos - 1;
    while (q > 0 && (((unsigned char) p->s[q]) & 0xC0) == 0x80) q--;
    return cp_at(p, q);
}
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
static int kw(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0) return -1;
    if (wordch_at(p, e)) return -1;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int at_bol(RkP *p, int pos) { return pos == 0 || p->s[pos - 1] == '\n' || p->s[pos - 1] == '\r'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int at_eol(RkP *p, int pos) { return pos >= p->n || p->s[pos] == '\n' || p->s[pos] == '\r'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_ws(RkP *p, int from, int to) { if (to >= 0 && to <= p->n) p->wsmark[to] = from + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int marked_ws_from(RkP *p, int pos) { return (pos >= 0 && pos <= p->n && p->wsmark[pos]) ? p->wsmark[pos] - 1 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_end(RkP *p, int pos) { if (pos >= 0 && pos <= p->n) p->endmark[pos] = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int marked_end(RkP *p, int pos) { return pos >= 0 && pos <= p->n && p->endmark[pos]; }
/*====================================================================================================================================================================================================*/
static const RkOp rk_infix[] = {
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
static const RkOp rk_prefix[] = {
    { "++", PR('x'), 0, AS_UNARY, 0 }, { "--", PR('x'), 0, AS_UNARY, 0 }, { "++\xe2\x9a\x9b", PR('x'), 0, AS_UNARY, 0 }, { "--\xe2\x9a\x9b", PR('x'), 0, AS_UNARY, 0 },
    { "+", PR('v'), 0, AS_UNARY, 0 }, { "~", PR('v'), 0, AS_UNARY, 0 }, { "-", PR('v'), 0, AS_UNARY, 0 }, { "\xe2\x88\x92", PR('v'), 0, AS_UNARY, 0 }, { "?", PR('v'), 0, AS_UNARY, 0 },
    { "!", PR('v'), 0, AS_UNARY, 0 }, { "|", PR('v'), 0, AS_UNARY, 0 }, { "+^", PR('v'), 0, AS_UNARY, 0 }, { "~^", PR('v'), 0, AS_UNARY, 0 }, { "?^", PR('v'), 0, AS_UNARY, 0 },
    { "^", PR('v'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b", PR('v'), 0, AS_UNARY, 0 },
    { "let", PR('x'), 0, AS_UNARY, OF_WORD }, { "temp", PR('x'), 0, AS_UNARY, OF_WORD }, { "so", PR('h'), 0, AS_UNARY, OF_WORD }, { "not", PR('h'), 0, AS_UNARY, OF_WORD },
    { NULL, 0, 0, 0, 0 }
};
static const RkOp rk_postfix[] = {
    { "++", PR('x'), 0, AS_UNARY, 0 }, { "--", PR('x'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b++", PR('x'), 0, AS_UNARY, 0 }, { "\xe2\x9a\x9b--", PR('x'), 0, AS_UNARY, 0 },
    { "i", PR('y'), 0, AS_UNARY, OF_WORD },
    { NULL, 0, 0, 0, 0 }
};
/*====================================================================================================================================================================================================*/
static char *ct_strndup0(const char *s, int n) { char *r = (char *) ct_alloc((size_t) n + 1); memcpy(r, s, (size_t) n); r[n] = 0; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_name_n(RkP *p, const char *s, int n) {
    if (n <= 0) return;
    RK_GROW(p->names, p->nnames, p->cnames, RkName);
    p->names[p->nnames].name = ct_strndup0(s, n);
    p->names[p->nnames].depth = p->depth;
    p->names[p->nnames].value = 0;
    p->nnames++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_value_name(RkP *p, const char *s, int n) { add_name_n(p, s, n); if (p->nnames) p->names[p->nnames - 1].value = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_our_name(RkP *p, const char *s, int n) { add_name_n(p, s, n); if (p->nnames && p->scope != 1) p->names[p->nnames - 1].depth = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_name_all(RkP *p, const char *s, int n) {
    add_our_name(p, s, n);
    for (int i = 0; i + 1 < n; i++) if (s[i] == ':' && s[i + 1] == ':') { add_our_name(p, s, i); add_our_name(p, s + i + 2, n - i - 2); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_name_cmp(const void *a, const void *b) { return strcmp(*(const char *const *) a, *(const char *const *) b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_has(const char *const *tab, size_t n, const char *s, int len) {
    char buf[256];
    if (len <= 0 || len >= (int) sizeof buf) return 0;
    memcpy(buf, s, (size_t) len); buf[len] = 0;
    const char *key = buf;
    return bsearch(&key, tab, n, sizeof *tab, core_name_cmp) != NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_pseudo_pkg(const char *s, int n) {
    static const char *const ps[] = { "GLOBAL", "OUR", "MY", "CORE", "SETTING", "OUTER", "CALLER", "DYNAMIC", "PROCESS", "COMPILING", "UNIT", "LEXICAL", "CLIENT", "OUTERS", "CALLERS", "EXPORT", 0 };
    for (int i = 0; ps[i]; i++) if ((int) strlen(ps[i]) == n && !memcmp(ps[i], s, (size_t) n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int user_name_index(RkP *p, const char *s, int n) {
    for (int i = p->nnames - 1; i >= 0; i--) if ((int) strlen(p->names[i].name) == n && !memcmp(p->names[i].name, s, (size_t) n)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_name_n(RkP *p, const char *s, int n) {
    int i0 = 0; while (i0 + 1 < n && !(s[i0] == ':' && s[i0 + 1] == ':')) i0++;
    if (i0 + 1 >= n && is_pseudo_pkg(s, n)) return 1;
    while (n > 0) {
        int i = 0; while (i + 1 < n && !(s[i] == ':' && s[i + 1] == ':')) i++;
        if (i + 1 < n && is_pseudo_pkg(s, i)) return 1;
        break;
    }
    if (n >= 2 && s[n - 1] == ':' && s[n - 2] == ':') n -= 2;
    if (n <= 0) return 1;
    if (user_name_index(p, s, n) >= 0) return 1;
    if (core_has(rk_core_names, sizeof rk_core_names / sizeof *rk_core_names, s, n)) return 1;
    if (p->lang_e && core_has(rk_core_e_names, sizeof rk_core_e_names / sizeof *rk_core_e_names, s, n)) return 1;
    if (i0 + 1 < n && i0 > 0 && user_name_index(p, s, i0) >= 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_type_n(RkP *p, const char *s, int n) {
    static const char *const vals[] = { "True", "False", "Less", "Same", "More", "Inf", "NaN", "pi", "e", "i", "tau", "Empty", "Nil", 0 };
    for (int i = 0; vals[i]; i++) if ((int) strlen(vals[i]) == n && !memcmp(vals[i], s, (size_t) n)) return 0;
    if (n >= 2 && s[n - 1] == ':' && s[n - 2] == ':') return 0;
    int u = user_name_index(p, s, n);
    if (u >= 0 && p->names[u].value) return 0;
    return is_name_n(p, s, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_routine_name(RkP *p, const char *s, int n) {
    if (n <= 0 || n > 250) return;
    char buf[256]; buf[0] = '&'; memcpy(buf + 1, s, (size_t) n);
    add_name_n(p, buf, n + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int routine_visible(RkP *p, const char *s, int n) {
    if (n <= 0 || n > 250) return 1;
    char buf[256]; buf[0] = '&'; memcpy(buf + 1, s, (size_t) n);
    if (user_name_index(p, buf, n + 1) >= 0 || user_name_index(p, s, n) >= 0) return 1;
    if (core_has(rk_core_routines, sizeof rk_core_routines / sizeof *rk_core_routines, s, n)) return 1;
    if (p->lang_e && core_has(rk_core_e_routines, sizeof rk_core_e_routines / sizeof *rk_core_e_routines, s, n)) return 1;
    return core_has(rk_core_names, sizeof rk_core_names / sizeof *rk_core_names, s, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_mystery(RkP *p, int pos, int len) {
    int c0 = cp_at(p, pos);
    if (p->myst_off || routine_visible(p, p->s + pos, len) || !is_alpha_cp(c0) || is_numeric_other_cp(c0) || (c0 >= 0x80 && cp_len(p, pos) == len)) return;
    if (len == 9 && !memcmp(p->s + pos, "GLOBALish", 9)) return;
    for (int i = p->nmyst - 1; i >= 0; i--) if (p->myst[i].pos == pos) return;
    RK_GROW(p->myst, p->nmyst, p->cmyst, RkMyst);
    p->myst[p->nmyst].pos = pos; p->myst[p->nmyst].len = len; p->myst[p->nmyst].depth = p->depth;
    p->nmyst++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void resolve_mysteries(RkP *p, int depth) {
    int k = 0;
    for (int i = 0; i < p->nmyst; i++) {
        RkMyst m = p->myst[i];
        if (m.depth > depth) { if (routine_visible(p, p->s + m.pos, m.len)) continue; m.depth = depth; }
        p->myst[k++] = m;
    }
    p->nmyst = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scope_enter(RkP *p) { p->depth++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scope_leave(RkP *p) {
    resolve_mysteries(p, p->depth - 1);
    p->depth--;
    while (p->nnames > 0 && p->names[p->nnames - 1].depth > p->depth) p->nnames--;
    int k = 0;
    for (int i = 0; i < p->nuops; i++) if (p->uops[i].depth <= p->depth || p->uops[i].depth >= 1000) p->uops[k++] = p->uops[i];
    p->nuops = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int uniname_cmp(const void *a, const void *b) { return strcmp((const char *) a, ((const RkUniName *) b)->name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned uniname_cp(const char *s, int n) {
    char buf[128]; int k = 0;
    while (n > 0 && *s == ' ') { s++; n--; }
    while (n > 0 && s[n - 1] == ' ') n--;
    if (n <= 0 || n >= (int) sizeof buf) return 0;
    int digits = 1;
    for (int i = 0; i < n; i++) { char c = s[i]; if (c < '0' || c > '9') digits = 0; buf[k++] = (char) (c >= 'a' && c <= 'z' ? c - 32 : c); }
    buf[k] = 0;
    if (digits) return (unsigned) strtoul(buf, NULL, 10);
    const RkUniName *u = (const RkUniName *) bsearch(buf, rk_uninames, sizeof rk_uninames / sizeof *rk_uninames, sizeof *rk_uninames, uniname_cmp);
    return u ? (unsigned) u->cp : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_user_op(RkP *p, char cat, const char *sym, int len, int prec) {
    if (len <= 0) return;
    RK_GROW(p->uops, p->nuops, p->cuops, RkUserOp);
    RkUserOp *u = &p->uops[p->nuops++];
    char *d = (char *) ct_alloc((size_t) len * 4 + 1); int k = 0;
    for (int i = 0; i < len; i++) {
        if (sym[i] == '\\' && i + 2 < len && (sym[i + 1] == 'x' || sym[i + 1] == 'c') && sym[i + 2] == '[') {
            int j = i + 3; unsigned v = 0;
            if (sym[i + 1] == 'c') { while (j < len && sym[j] != ']') j++; v = uniname_cp(sym + i + 3, j - i - 3); if (!v) { d[k++] = sym[i]; continue; } }
            else while (j < len && sym[j] != ']') { char h = sym[j]; v = v * 16 + (unsigned) (h >= 'a' ? h - 'a' + 10 : h >= 'A' ? h - 'A' + 10 : h - '0'); j++; }
            if (v < 0x80) d[k++] = (char) v;
            else if (v < 0x800) { d[k++] = (char) (0xC0 | (v >> 6)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            else if (v < 0x10000) { d[k++] = (char) (0xE0 | (v >> 12)); d[k++] = (char) (0x80 | ((v >> 6) & 0x3F)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            else { d[k++] = (char) (0xF0 | (v >> 18)); d[k++] = (char) (0x80 | ((v >> 12) & 0x3F)); d[k++] = (char) (0x80 | ((v >> 6) & 0x3F)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            i = j; continue;
        }
        if (sym[i] == '\\' && i + 1 < len) i++;
        d[k++] = sym[i];
    }
    d[k] = 0;
    u->sym = d; u->len = k; u->prec = prec; u->depth = p->depth; u->cat = cat; u->assoc = AS_LEFT; u->flags = 0;
    { int q = k - 1; while (q > 0 && (((unsigned char) d[q]) & 0xC0) == 0x80) q--; int l; u->wordend = is_word_cp(rk_decode(d, k, q, &l)); }
}
/*====================================================================================================================================================================================================*/
static int r_nibble_until(RkP *p, int pos, RkLang *L, int *endpos);
static void register_user_op(RkP *p, int from, int to);
static int r_semilist(RkP *p, int pos);
static int r_block(RkP *p, int pos);
static int r_blockoid(RkP *p, int pos);
static int r_EXPR(RkP *p, int pos, int preclim);
static int r_statementlist(RkP *p, int pos);
static int r_statement(RkP *p, int pos);
static int r_eat_terminator(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int peek_delims(RkP *p, int pos, RkLang *L) {
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
static int quote_starter(RkP *p, int pos, RkLang *L) { if (!L->start) return -1; return match_rep(p, pos, L->start, L->nrep); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int here_stopper(RkP *p, int pos, RkLang *L) {
    if (!at_bol(p, pos)) return -1;
    int q = pos;
    while (q < p->n && is_hspace_cp(cp_at(p, q))) q += cp_len(p, q);
    if (q + L->here_len > p->n || memcmp(p->s + q, L->here, (size_t) L->here_len) != 0) return -1;
    q += L->here_len;
    while (q < p->n && is_hspace_cp(cp_at(p, q))) q += cp_len(p, q);
    if (!at_eol(p, q)) return -1;
    if (q < p->n && p->s[q] == '\r' && q + 1 < p->n && p->s[q + 1] == '\n') return q + 2;
    if (q < p->n) return q + cp_len(p, q);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int quote_stopper(RkP *p, int pos, RkLang *L) {
    if (L->here) return here_stopper(p, pos, L);
    int c = cp_at(p, pos);
    if (is_mark_cp(cp_at(p, pos + cp_len(p, pos)))) return -1;
    if (L->stop2 && c == L->stop2) return pos + cp_len(p, pos);
    if (L->start && L->nrep > 1) return match_rep(p, pos, L->stop, L->nrep);
    if (c == L->stop) return pos + cp_len(p, pos);
    return -1;
}
/*====================================================================================================================================================================================================*/
static int r_ws(RkP *p, int pos);
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
static int r_comment(RkP *p, int pos) {
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
    int q = pos + 1;
    while (q < p->n && !is_vspace_cp(cp_at(p, q))) q += cp_len(p, q);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int line_end(RkP *p, int pos) { while (pos < p->n && p->s[pos] != '\n') pos++; return pos < p->n ? pos + 1 : pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blank_line_at(RkP *p, int pos) {
    int q = pos;
    while (q < p->n && is_hspace_cp(cp_at(p, q))) q += cp_len(p, q);
    return q >= p->n || p->s[q] == '\n' || p->s[q] == '\r';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pod_ident(RkP *p, int pos, int *len) {
    int q = pos;
    while (q < p->n && (is_word_cp(cp_at(p, q)) || p->s[q] == '-' || p->s[q] == '\'' || p->s[q] == ':')) q += cp_len(p, q);
    *len = q - pos;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_pod(RkP *p, int pos) {
    int q = pos;
    while (q < p->n && is_hspace_cp(cp_at(p, q))) q += cp_len(p, q);
    if (ch(p, q) != '=') return -1;
    int c = cp_at(p, q + 1);
    if (!(is_word_cp(c) || c == '\\')) return -1;
    q++;
    int nl; int e = pod_ident(p, q, &nl);
    if (nl == 6 && !memcmp(p->s + q, "finish", 6)) { p->finished = 1; return p->n; }
    if (nl == 5 && !memcmp(p->s + q, "begin", 5)) {
        int r = e;
        while (r < p->n && is_hspace_cp(cp_at(p, r))) r += cp_len(p, r);
        int bl; int be = pod_ident(p, r, &bl);
        if (bl == 0) panic_at(p, q, "Pod block without a name after =begin");
        const char *name = p->s + r;
        if (bl == 6 && !memcmp(name, "finish", 6)) { p->finished = 1; return p->n; }
        int depth = 1;
        int l = line_end(p, be);
        while (l < p->n) {
            int t = l;
            while (t < p->n && is_hspace_cp(cp_at(p, t))) t += cp_len(p, t);
            if (p->s[t] == '=' && t + 1 < p->n) {
                int dl; int de = pod_ident(p, t + 1, &dl);
                int u = de; while (u < p->n && is_hspace_cp(cp_at(p, u))) u += cp_len(p, u);
                int nl2; pod_ident(p, u, &nl2);
                if (nl2 == bl && !memcmp(p->s + u, name, (size_t) bl)) {
                    if (dl == 5 && !memcmp(p->s + t + 1, "begin", 5)) depth++;
                    else if (dl == 3 && !memcmp(p->s + t + 1, "end", 3)) { if (--depth == 0) return line_end(p, u); }
                }
            }
            l = line_end(p, l);
        }
        panic_at(p, pos, "Couldn't find =end %.*s", bl, name);
    }
    if (nl == 6 && !memcmp(p->s + q, "config", 6)) {
        int l = line_end(p, e);
        while (l < p->n && !blank_line_at(p, l) && (p->s[l] == ' ' || p->s[l] == '\t') && p->s[l] != '=') l = line_end(p, l);
        return l;
    }
    int l = line_end(p, e);
    while (l < p->n && !blank_line_at(p, l)) {
        int t = l; while (t < p->n && is_hspace_cp(cp_at(p, t))) t += cp_len(p, t);
        if (p->s[t] == '=' && t + 1 < p->n && (is_word_cp(cp_at(p, t + 1)))) break;
        l = line_end(p, l);
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_heredoc_bodies(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_unsp(RkP *p, int pos);
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
static int r_unsp(RkP *p, int pos) {
    if (ch(p, pos) != '\\') return -1;
    int c = cp_at(p, pos + 1);
    if (!(is_space_cp(c) || c == '#')) return -1;
    return ws_loop(p, pos + 1, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_ws(RkP *p, int pos) {
    if (marked_ws_from(p, pos) >= 0) return pos;
    if (wordch_at(p, pos) && is_word_cp(prev_base_cp(p, pos)) && pos > 0) return -1;
    int e = ws_loop(p, pos, 1);
    if (marked_ws_from(p, e) < 0 || e != pos) mark_ws(p, pos, e);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ws(RkP *p, int pos) { int e = r_ws(p, pos); return e < 0 ? pos : e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int had_ws_before(RkP *p, int pos) { int f = marked_ws_from(p, pos); return f >= 0 && f < pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hs(RkP *p, int pos) { while (pos < p->n && is_hspace_cp(cp_at(p, pos))) pos += cp_len(p, pos); return pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_ENDSTMT(RkP *p, int pos) {
    int q = hs(p, pos);
    if (ch(p, q) == '#') { int e = r_comment(p, q); if (e >= 0 && ch(p, q + 1) != '`') q = e; else if (e >= 0) { q = hs(p, e); } }
    if (!at_eol(p, q)) return pos;
    int e = ws(p, q);
    mark_end(p, e);
    return e;
}
/*====================================================================================================================================================================================================*/
static int r_ident(RkP *p, int pos) {
    int c = cp_at(p, pos);
    hwm(p, pos);
    if (!is_alpha_cp(c)) return -1;
    pos += cp_len(p, pos);
    for (;;) { c = cp_at(p, pos); if (is_word_cp(c) || is_mark_cp(c)) pos += cp_len(p, pos); else break; }
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_identifier(RkP *p, int pos) {
    int e = r_ident(p, pos);
    if (e < 0) return -1;
    for (;;) {
        int c = ch(p, e);
        if ((c == '\'' || c == '-') && is_alpha_cp(cp_at(p, e + 1))) { e = r_ident(p, e + 1); continue; }
        break;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_morename(RkP *p, int pos) {
    int e = lit(p, pos, "::");
    if (e < 0) return -1;
    int c = cp_at(p, e);
    if (c == '(') {
        int q = ws(p, e + 1);
        int r = r_EXPR(p, q, 0);
        if (r < 0) panic_at(p, q, "Malformed indirect name");
        r = ws(p, r);
        if (ch(p, r) != ')') panic_at(p, r, "Unable to parse indirect name; couldn't find final ')'");
        return r + 1;
    }
    if (is_alpha_cp(c)) { int r = r_identifier(p, e); return r >= 0 ? r : e; }
    if (c == ':' && ch(p, e + 1) == ':') panic_at(p, e, "Name component may not be null");
    if ((c == '$' || c == '@' || c == '%' || c == '&') && is_alpha_cp(cp_at(p, e + 1))) panic_at(p, pos, "Malformed lookup of ::%c...; please use ::('%c...'), ::{'%c...'}, or ::<%c...>", c, c, c, c);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_name(RkP *p, int pos) {
    int e = r_identifier(p, pos);
    if (e >= 0) { for (;;) { int m = r_morename(p, e); if (m < 0) break; e = m; } return e; }
    e = r_morename(p, pos);
    if (e < 0) return -1;
    for (;;) { int m = r_morename(p, e); if (m < 0) break; e = m; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_colonpair(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int colonpair_ahead(RkP *p, int pos) {
    if (ch(p, pos) != ':') return 0;
    int c = cp_at(p, pos + 1);
    return is_alpha_cp(c) || c == '<' || c == '[' || c == 0xAB;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_longname(RkP *p, int pos) {
    int e = r_name(p, pos);
    if (e < 0) return -1;
    while (colonpair_ahead(p, e)) { int c = r_colonpair(p, e); if (c < 0) break; e = c; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_deflongname(RkP *p, int pos) {
    int e = r_name(p, pos);
    if (e < 0) return -1;
    while (ch(p, e) == ':') { int c = r_colonpair(p, e); if (c < 0) break; e = c; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int end_keyword_ok(RkP *p, int pos) {
    if (wordch_at(p, pos)) return 0;
    int c = ch(p, pos);
    if (c == '(' || c == '\\' || c == '\'' || c == '-') return 0;
    int q = hs(p, pos);
    if (at_lit(p, q, "=>")) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kok(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0 || !end_keyword_ok(p, e)) return -1;
    int c = cp_at(p, e);
    if (is_space_cp(c) || c == '#') return ws(p, e);
    if (is_name_n(p, w, (int) strlen(w))) return -1;
    panic_at(p, e, "Whitespace required after keyword '%s'", w);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kw_end(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0 || !end_keyword_ok(p, e)) return -1;
    return e;
}
/*====================================================================================================================================================================================================*/
static int r_decint(RkP *p, int pos) {
    if (!is_digit_cp(cp_at(p, pos))) return -1;
    while (is_digit_cp(cp_at(p, pos))) pos += cp_len(p, pos);
    while (ch(p, pos) == '_' && is_digit_cp(cp_at(p, pos + 1))) { pos++; while (is_digit_cp(cp_at(p, pos))) pos += cp_len(p, pos); }
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int unicode_digit_value(int c) {
    static const int z[] = { 0x660,0x6F0,0x7C0,0x966,0x9E6,0xA66,0xAE6,0xB66,0xBE6,0xC66,0xCE6,0xD66,0xDE6,0xE50,0xED0,0xF20,0x1040,0x1090,0x17E0,0x1810,0x1946,0x19D0,0x1A80,0x1A90,
                             0x1B50,0x1BB0,0x1C40,0x1C50,0xA620,0xA8D0,0xA900,0xA9D0,0xA9F0,0xAA50,0xABF0,0xFF10,0x104A0,0x11066,0x1D7CE,0x1D7D8,0x1D7E2,0x1D7EC,0x1D7F6 };
    for (unsigned i = 0; i < sizeof z / sizeof *z; i++) if (c >= z[i] && c <= z[i] + 9) return c - z[i];
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int digit_in_base(int c, int base) {
    int v = -1;
    if (c >= '0' && c <= '9') v = c - '0'; else if (c >= 'a' && c <= 'z') v = c - 'a' + 10; else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
    else if (c >= 0xFF21 && c <= 0xFF3A) v = c - 0xFF21 + 10; else if (c >= 0xFF41 && c <= 0xFF5A) v = c - 0xFF41 + 10; else if (c >= 0x80) v = unicode_digit_value(c);
    return v >= 0 && v < base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_baseint(RkP *p, int pos, int base) {
    if (!digit_in_base(cp_at(p, pos), base)) return -1;
    while (digit_in_base(cp_at(p, pos), base)) pos += cp_len(p, pos);
    while (ch(p, pos) == '_' && digit_in_base(cp_at(p, pos + 1), base)) { pos++; while (digit_in_base(cp_at(p, pos), base)) pos += cp_len(p, pos); }
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_terminator_at(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_integer(RkP *p, int pos) {
    int e = -1;
    if (ch(p, pos) == '0') {
        int c = ch(p, pos + 1); int q = pos + 2;
        if (c == 'b' || c == 'o' || c == 'x' || c == 'd') {
            if (ch(p, q) == '_') q++;
            int b = c == 'b' ? 2 : c == 'o' ? 8 : c == 'x' ? 16 : 10;
            e = r_baseint(p, q, b);
        }
    }
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
    int c = ch(p, pos);
    if (c != 'e' && c != 'E') return -1;
    int q = pos + 1;
    if (ch(p, q) == '+' || ch(p, q) == '-') q++; else if (at_lit(p, q, "\xe2\x88\x92")) q += 3;
    return r_decint(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_dec_number(RkP *p, int pos) {
    if (ch(p, pos) == '.') {
        int f = r_decint(p, pos + 1);
        if (f < 0) return -1;
        int x = r_escale(p, f);
        return x >= 0 ? x : f;
    }
    int i = r_decint(p, pos);
    if (i < 0) return -1;
    if (ch(p, i) == '.' && is_digit_cp(cp_at(p, i + 1))) {
        int f = r_decint(p, i + 1);
        int x = r_escale(p, f);
        return x >= 0 ? x : f;
    }
    int x = r_escale(p, i);
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_circumfix(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_rad_number(RkP *p, int pos) {
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
    if (c == '[' || c == '(') return r_circumfix(p, q);
    panic_at(p, q, "Malformed radix number");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_numish(RkP *p, int pos) {
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
static int r_version(RkP *p, int pos) {
    if (ch(p, pos) != 'v' || !is_digit_cp(cp_at(p, pos + 1))) return -1;
    int q = pos + 1;
    for (;;) {
        if (ch(p, q) == '*') q++;
        else { int e = q; while (is_word_cp(cp_at(p, e))) e += cp_len(p, e); if (e == q) return -1; q = e; }
        if (ch(p, q) == '.' && (is_word_cp(cp_at(p, q + 1)) || ch(p, q + 1) == '*')) { q++; continue; }
        break;
    }
    if (ch(p, q) == '+') q++;
    if (ch(p, q) == '-' || ch(p, q) == '\'') return -1;
    return q;
}
/*====================================================================================================================================================================================================*/
static int r_quote(RkP *p, int pos);
static int r_variable(RkP *p, int pos);
static int r_termish(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_charspec(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '[') {
        int q = pos + 1;
        while (q < p->n && p->s[q] != ']') q++;
        if (ch(p, q) != ']') panic_at(p, pos, "Unrecognized \\c character");
        return q + 1;
    }
    if (is_digit_cp(c)) return r_decint(p, pos);
    if ((c >= '?' && c <= '_')) return pos + 1;
    panic_at(p, pos, "Unrecognized \\c character");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_bracketed_ints(RkP *p, int pos, int base) {
    int q = pos + 1;
    for (;;) {
        q = hs(p, q);
        int e = r_baseint(p, q, base);
        if (e < 0) panic_at(p, q, "Malformed character code");
        q = hs(p, e);
        if (ch(p, q) == ',') { q++; continue; }
        if (ch(p, q) == ']') return q + 1;
        panic_at(p, q, "Unable to parse character code list; couldn't find final ']'");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int qq_backslash(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c < 0) return -1;
    if (c == 'q' && (ch(p, pos + 1) == 'q' || !is_word_cp(cp_at(p, pos + 1)))) { int e = r_quote(p, pos); if (e >= 0) return e; }
    if (c == '\\') return pos + 1;
    if ((L->start && c == L->start) || c == L->stop || (L->stop2 && c == L->stop2)) return pos + cp_len(p, pos);
    if (at_lit(p, pos, "rn")) return pos + 2;
    if (strchr("abefnrt0", c) && c) return pos + 1;
    if (c == 'c') return r_charspec(p, pos + 1);
    if (c == 'o') {
        if (ch(p, pos + 1) == '[') return r_bracketed_ints(p, pos + 1, 8);
        if (ch(p, pos + 1) == '{') panic_at(p, pos, "Unsupported use of curlies around escape argument; in Raku please use square brackets");
        int e = r_baseint(p, pos + 1, 8); if (e < 0) panic_at(p, pos, "Unrecognized backslash sequence: '\\o'"); return e;
    }
    if (c == 'x') {
        if (ch(p, pos + 1) == '[') return r_bracketed_ints(p, pos + 1, 16);
        if (ch(p, pos + 1) == '{') panic_at(p, pos, "Unsupported use of curlies around escape argument; in Raku please use square brackets");
        int e = r_baseint(p, pos + 1, 16); if (e < 0) panic_at(p, pos, "Unrecognized backslash sequence: '\\x'"); return e;
    }
    if (c == 'N' && ch(p, pos + 1) == '{') panic_at(p, pos, "Unsupported use of \\N{CHARNAME}; in Raku please use \\c[CHARNAME]");
    if (c >= '1' && c <= '9') panic_at(p, pos - 1, "Unrecognized backslash sequence: '\\%c'", c);
    if (is_word_cp(c)) panic_at(p, pos - 1, "Unrecognized backslash sequence: '\\%.*s'", cp_len(p, pos), p->s + pos);
    return pos + cp_len(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int q_backslash(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c < 0) return -1;
    if (c == 'q' && (ch(p, pos + 1) == 'q' || !is_word_cp(cp_at(p, pos + 1)))) { int e = r_quote(p, pos); if (e >= 0) return e; }
    return pos + cp_len(p, pos);
    (void) L;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int interp_sigil(RkP *p, int pos, int sigil) {
    int sq = p->qsigil;
    p->qsigil = sigil;
    int e = r_EXPR(p, pos, PR('y'));
    p->qsigil = sq;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quote_escape(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c == '\\') {
        if (L->bs) { int e = qq_backslash(p, pos + 1, L); if (e >= 0) return e; }
        else if (L->qbs) return q_backslash(p, pos + 1, L);
        return -1;
    }
    if (c == '{' && L->clos) { p->quote_block = 1; return r_block(p, pos); }
    if (c == '$' && L->sc) {
        int e = interp_sigil(p, pos, '$');
        if (e < 0) panic_at(p, pos, "Non-variable $ must be backslashed");
        return e;
    }
    if (c == '@' && L->ar) return interp_sigil(p, pos, '@');
    if (c == '%' && L->hs) return interp_sigil(p, pos, '%');
    if (c == '&' && L->fn) return interp_sigil(p, pos, '&');
    if (L->ww) {
        if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201A || c == 0x2019 || c == 0x201C || c == 0x201E || c == 0x201D || c == 0xFF62) return r_quote(p, pos);
        if (c == ':' && (is_alpha_cp(cp_at(p, pos + 1)) || ch(p, pos + 1) == '!' || ch(p, pos + 1) == '$' || ch(p, pos + 1) == '@')) { int e = r_colonpair(p, pos); if (e >= 0) return e; }
        if (c == '#') return r_comment(p, pos);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_nibbler(RkP *p, int pos, RkLang *L);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_nibble_until(RkP *p, int pos, RkLang *L, int *endpos) {
    if (L->regex) { int e = r_regex_nibbler(p, pos, L); *endpos = e; return e; }
    int q = pos;
    for (;;) {
        if (q >= p->n) { *endpos = -1; return -1; }
        if (quote_stopper(p, q, L) >= 0) { *endpos = q; return q; }
        int s = quote_starter(p, q, L);
        if (s >= 0) {
            int e; r_nibble_until(p, s, L, &e);
            if (e < 0) { *endpos = -1; return -1; }
            int t = quote_stopper(p, e, L);
            if (t < 0) { *endpos = -1; return -1; }
            q = t; continue;
        }
        int e = r_quote_escape(p, q, L);
        if (e >= 0) { q = e; continue; }
        q += cp_len(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lang_q(RkLang *L) { memset(L, 0, sizeof *L); L->qbs = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lang_qq(RkLang *L) { memset(L, 0, sizeof *L); L->bs = 1; L->clos = 1; L->sc = 1; L->ar = 1; L->hs = 1; L->fn = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int quote_body_at(RkP *p, int pos, RkLang *L, const char *what, int *stop_at) {
    int start = pos;
    int e; r_nibble_until(p, pos, L, &e);
    if (e < 0) panic_at(p, start, "Couldn't find terminator of %s", what);
    int s = quote_stopper(p, e, L);
    if (s < 0) panic_at(p, e, "Couldn't find terminator of %s", what);
    if (stop_at) *stop_at = e;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int quote_body(RkP *p, int pos, RkLang *L, const char *what) { return quote_body_at(p, pos, L, what, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quotepair(RkP *p, int pos, char *key, int keylen, int *val) {
    if (ch(p, pos) != ':') return -1;
    int q = pos + 1; *val = 1;
    if (ch(p, q) == '!') {
        int e = r_identifier(p, q + 1);
        if (e < 0) return -1;
        snprintf(key, (size_t) keylen, "%.*s", e - q - 1, p->s + q + 1); *val = 0; return e;
    }
    if (is_digit_cp(cp_at(p, q))) {
        int d = r_decint(p, q); int e = r_identifier(p, d);
        if (e < 0) return -1;
        snprintf(key, (size_t) keylen, "%.*s", e - d, p->s + d); return e;
    }
    int e = r_identifier(p, q);
    if (e < 0) return -1;
    snprintf(key, (size_t) keylen, "%.*s", e - q, p->s + q);
    if (ch(p, e) == '(') { int c = r_circumfix(p, e); if (c >= 0) { *val = 2; return c; } }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int apply_quote_adverb(RkP *p, int pos, RkLang *L, const char *k, int v, int *to) {
    if (!strcmp(k, "q") || !strcmp(k, "single")) { lang_q(L); return 1; }
    if (!strcmp(k, "qq") || !strcmp(k, "double")) { lang_qq(L); return 1; }
    if (!strcmp(k, "b") || !strcmp(k, "backslash")) { L->bs = v != 0; if (v) L->qbs = 0; return 1; }
    if (!strcmp(k, "s") || !strcmp(k, "scalar")) { L->sc = v != 0; return 1; }
    if (!strcmp(k, "a") || !strcmp(k, "array")) { L->ar = v != 0; return 1; }
    if (!strcmp(k, "h") || !strcmp(k, "hash")) { L->hs = v != 0; return 1; }
    if (!strcmp(k, "f") || !strcmp(k, "function")) { L->fn = v != 0; return 1; }
    if (!strcmp(k, "c") || !strcmp(k, "closure")) { L->clos = v != 0; return 1; }
    if (!strcmp(k, "w") || !strcmp(k, "words")) { L->words = 1; return 1; }
    if (!strcmp(k, "ww") || !strcmp(k, "quotewords")) { L->ww = v != 0; L->words = 1; return 1; }
    if (!strcmp(k, "x") || !strcmp(k, "exec") || !strcmp(k, "v") || !strcmp(k, "val")) return 1;
    if (!strcmp(k, "to") || !strcmp(k, "heredoc")) { *to = 1; return 1; }
    if (!strcmp(k, "cc")) { L->cc = 1; return 1; }
    if (!strcmp(k, "regex")) { memset(L, 0, sizeof *L); L->regex = 1; return 1; }
    if (!strcmp(k, "p") || !strcmp(k, "path")) panic_at(p, pos, "Unrecognized adverb: :%s", k);
    panic_at(p, pos, "Unrecognized adverb: :%s", k);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quibble(RkP *p, int pos, RkLang *L, int to, const char *what) {
    char key[64]; int v;
    for (;;) {
        int e = r_quotepair(p, pos, key, sizeof key, &v);
        if (e < 0) break;
        apply_quote_adverb(p, pos, L, key, v, &to);
        pos = ws(p, e);
    }
    peek_delims(p, pos, L);
    int s = L->start ? quote_starter(p, pos, L) : pos + cp_len(p, pos);
    int stop_at;
    int e = quote_body_at(p, s, L, what, &stop_at);
    if (to) {
        int dl = stop_at - s;
        while (dl > 0 && asc_space((unsigned char) p->s[s + dl - 1])) dl--;
        int d0 = s; while (d0 < s + dl && asc_space((unsigned char) p->s[d0])) { d0++; dl--; }
        if (dl <= 0) panic_at(p, pos, "Heredoc delimiter is empty");
        RK_GROW(p->here, p->nhere, p->chere, RkHere);
        RkHere *h = &p->here[p->nhere++];
        h->delim = ct_strndup0(p->s + d0, dl); h->dlen = dl; h->lang = *L; h->lang.start = 0; h->lang.stop = 0; h->lang.stop2 = 0; h->lang.nrep = 1;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_heredoc_bodies(RkP *p, int pos) {
    while (p->nhere) {
        RkHere h = p->here[0];
        memmove(&p->here[0], &p->here[1], sizeof(RkHere) * (size_t) (p->nhere - 1));
        p->nhere--;
        RkLang L = h.lang;
        L.here = h.delim; L.here_len = h.dlen; L.start = 0;
        int e; r_nibble_until(p, pos, &L, &e);
        if (e < 0) panic_at(p, pos, "Ending delimiter %s not found", h.delim);
        int s = here_stopper(p, e, &L);
        if (s < 0) panic_at(p, pos, "Ending delimiter %s not found", h.delim);
        pos = s;
    }
    return pos;
}
/*====================================================================================================================================================================================================*/
static int rx_ws(RkP *p, int pos) {
    for (;;) {
        int c = cp_at(p, pos);
        if (is_space_cp(c)) { pos += cp_len(p, pos); continue; }
        if (c == '#') { pos = r_comment(p, pos); continue; }
        return pos;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_stopper(RkP *p, int pos, RkLang *L) { return quote_stopper(p, pos, L) >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_infixstopper(RkP *p, int pos, RkLang *L) {
    int c = ch(p, pos);
    if (c == ')' || c == '}' || c == ']') return 1;
    if (c == '>' && ch(p, pos + 1) != '>') return 1;
    return rx_stopper(p, pos, L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_quantified_atom(RkP *p, int pos, RkLang *L);
static int rx_termseq(RkP *p, int pos, RkLang *L);
static int rx_assertion(RkP *p, int pos, RkLang *L);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rx_unrec(RkP *p, int pos) {
    if (pos >= p->n) panic_at(p, pos, "Regex not terminated.");
    int c = cp_at(p, pos);
    if (is_vspace_cp(c)) panic_at(p, pos, "Regex not terminated.");
    panic_at(p, pos, "Unrecognized regex metacharacter %.*s (must be quoted to match literally)", cp_len(p, pos), p->s + pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_nibbler(RkP *p, int pos, RkLang *L) {
    int q = rx_ws(p, pos);
    q = rx_termseq(p, q, L);
    if (rx_infixstopper(p, q, L)) return q;
    rx_unrec(p, q);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_termish(RkP *p, int pos, RkLang *L) {
    int q = pos; int n = 0;
    for (;;) { int e = rx_quantified_atom(p, q, L); if (e < 0) break; q = e; n++; }
    if (n) return q;
    int c = ch(p, pos);
    if (rx_stopper(p, pos, L) || c == '&' || c == '|' || c == '~') panic_at(p, pos, "Null regex not allowed");
    if (rx_infixstopper(p, pos, L)) panic_at(p, pos, "Null regex not allowed");
    rx_unrec(p, pos);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_level(RkP *p, int pos, RkLang *L, const char *op, int (*sub)(RkP *, int, RkLang *)) {
    int ol = (int) strlen(op);
    int q = pos;
    if (!rx_stopper(p, q, L) && at_lit(p, q, op) && !(ol == 1 && ch(p, q + 1) == op[0])) q = rx_ws(p, q + ol);
    q = sub(p, q, L);
    for (;;) {
        if (rx_infixstopper(p, q, L)) return q;
        if (!at_lit(p, q, op)) return q;
        if (ol == 1 && ch(p, q + 1) == op[0]) return q;
        q = rx_ws(p, q + ol);
        q = sub(p, q, L);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_termconj(RkP *p, int pos, RkLang *L) { return rx_level(p, pos, L, "&", rx_termish); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_termalt(RkP *p, int pos, RkLang *L) { return rx_level(p, pos, L, "|", rx_termconj); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_termconjseq(RkP *p, int pos, RkLang *L) { return rx_level(p, pos, L, "&&", rx_termalt); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_termseq(RkP *p, int pos, RkLang *L) { return rx_level(p, pos, L, "||", rx_termconjseq); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_backmod(RkP *p, int pos) {
    int q = pos;
    if (ch(p, q) == ':') q++;
    if (ch(p, q) == '?' || ch(p, q) == '!') return q + 1;
    if (ch(p, q) == ':') return -1;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_signed_int(RkP *p, int pos) {
    int q = pos;
    if (ch(p, q) == '-') q++;
    int e = r_integer(p, q);
    if (e >= 0 && q != pos) panic_at(p, pos, "Negative numbers are not allowed as quantifiers");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_quantifier(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '*' && ch(p, pos + 1) == '*') {
        int q = rx_ws(p, pos + 2);
        int b = rx_backmod(p, q); if (b >= 0) q = b;
        q = rx_ws(p, q);
        if (ch(p, q) == '{') return r_block(p, q);
        if (ch(p, q) == '^') {
            if (ch(p, q + 1) == '*') panic_at(p, q, "Unecessary use of \"** ^*\" quantifier. Did you mean to use the \"*\" quantifier");
            int e = rx_signed_int(p, q + 1); if (e < 0) panic_at(p, q, "Malformed range."); return e;
        }
        int e = rx_signed_int(p, q);
        if (e < 0) panic_at(p, q, "Malformed range.");
        int t = e;
        if (is_space_cp(cp_at(p, t))) { int u = t; while (is_space_cp(cp_at(p, u))) u++; if (at_lit(p, u, "..")) panic_at(p, e, "Spaces not allowed in bare range."); }
        if (ch(p, t) == '^') { if (!at_lit(p, t + 1, "..")) panic_at(p, t, "Malformed range."); t += 3; }
        else if (at_lit(p, t, "..")) t += 2;
        else return e;
        if (ch(p, t) == '*') return t + 1;
        if (ch(p, t) == '^') t++;
        int m = rx_signed_int(p, t);
        if (m < 0) panic_at(p, t, "Malformed range.");
        return m;
    }
    if (c == '*' || c == '+' || c == '?') { int b = rx_backmod(p, pos + 1); return b >= 0 ? b : pos + 1; }
    if (c == '{' && is_digit_cp(cp_at(p, pos + 1))) {
        int q = pos + 1; while (is_digit_cp(cp_at(p, q)) || ch(p, q) == ',') q++;
        if (ch(p, q) == '}') panic_at(p, pos, "Unsupported use of {N,M} as general quantifier; in Raku please use ** N..M (or ** N..*)");
    }
    if (c == '%') panic_at(p, pos, "Missing quantifier on the left argument of %s", ch(p, pos + 1) == '%' ? "%%" : "%");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_backslash(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c < 0) panic_at(p, pos, "Regex not terminated.");
    if (c < 128 && strchr("dDnNsSwWeEfFhHrRtTvV0", c)) return pos + 1;
    if (c == 'o' || c == 'O') { if (ch(p, pos + 1) == '[') return r_bracketed_ints(p, pos + 1, 8); int e = r_baseint(p, pos + 1, 8); if (e < 0) panic_at(p, pos,
        "Unrecognized backslash sequence: '\\o'"); return e; }
    if (c == 'x' || c == 'X') {
        if (ch(p, pos + 1) == '[') return r_bracketed_ints(p, pos + 1, 16);
        if (ch(p, pos + 1) == '{') panic_at(p, pos, "Unsupported use of curlies around escape argument; in Raku please use square brackets");
        int e = r_baseint(p, pos + 1, 16); if (e < 0) panic_at(p, pos, "Unrecognized backslash sequence: '\\x'"); return e;
    }
    if (c == 'c' || c == 'C') return r_charspec(p, pos + 1);
    if (c == 'B') panic_at(p, pos, "Unsupported use of \\B; in Raku please use <!|w> for negated word boundary");
    if (c == 'b') panic_at(p, pos, "Unsupported use of \\b; in Raku please use <|w> for word boundary (or \xc2\xab and \xc2\xbb for left/right boundaries)");
    if (c == 'K') panic_at(p, pos, "Unsupported use of \\K; in Raku please use <( for discarding text before the capture marker");
    if (c == 'A') panic_at(p, pos, "Unsupported use of \\A as beginning-of-string matcher; in Raku please use ^");
    if (c == 'z') panic_at(p, pos, "Unsupported use of \\z as end-of-string matcher; in Raku please use $");
    if (c == 'Z') panic_at(p, pos, "Unsupported use of \\Z as end-of-string matcher; in Raku please use \\n?$");
    if (c == 'Q') panic_at(p, pos, "Unsupported use of \\Q as quotemeta; in Raku please use quotes or literal variable match");
    if (c >= '1' && c <= '9') panic_at(p, pos - 1, "Unrecognized backslash sequence: '\\%c'", c);
    if (is_word_cp(c)) panic_at(p, pos - 1, "Unrecognized backslash sequence: '\\%.*s'", cp_len(p, pos), p->s + pos);
    if (is_space_cp(c)) panic_at(p, pos - 1, "No unspace allowed in regex; if you meant to match the literal character, please enclose in single quotes");
    return pos + cp_len(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_mod_internal(RkP *p, int pos) {
    int q = pos + 1;
    if (ch(p, q) == '!') q++;
    else if (is_digit_cp(cp_at(p, q))) q = r_decint(p, q);
    int e = q; while (is_word_cp(cp_at(p, e))) e += cp_len(p, e);
    if (e == q) return -1;
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
static int r_arglist(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_cclass_char(RkP *p, int pos) {
    if (ch(p, pos) == '\\') {
        int c = cp_at(p, pos + 1);
        if (c == 'o' || c == 'O') { if (ch(p, pos + 2) == '[') return r_bracketed_ints(p, pos + 2, 8); int e = r_baseint(p, pos + 2, 8); return e >= 0 ? e : pos + 2; }
        if (c == 'x' || c == 'X') { if (ch(p, pos + 2) == '[') return r_bracketed_ints(p, pos + 2, 16); int e = r_baseint(p, pos + 2, 16); return e >= 0 ? e : pos + 2; }
        if (c == 'c' || c == 'C') return r_charspec(p, pos + 2);
        if (c < 0) panic_at(p, pos, "Regex not terminated.");
        return pos + 1 + cp_len(p, pos + 1);
    }
    if (ch(p, pos) == ']' || pos >= p->n) return -1;
    return pos + cp_len(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_cclass_elem(RkP *p, int pos, RkLang *L) {
    int q = pos;
    if (ch(p, q) == '+' || ch(p, q) == '-') q++;
    q = rx_ws(p, q);
    if (ch(p, q) == '[') {
        int t = q + 1; int first = 1;
        for (;;) {
            int s = t; while (is_space_cp(cp_at(p, s))) s += cp_len(p, s);
            if (ch(p, s) == ']') { t = s + 1; break; }
            if (s >= p->n) panic_at(p, q, "Unable to parse character class; couldn't find final ']'");
            if (ch(p, s) == '-' && !first) {
                int u = s + 1; while (is_space_cp(cp_at(p, u))) u++;
                if (ch(p, u) != ']') panic_at(p, s,
                    "Unsupported use of - as character range; in Raku please use .. for range, for explicit - in character class, escape it or place it as the first or last thing");
            }
            int e = rx_cclass_char(p, s);
            if (e < 0) panic_at(p, s, "Malformed character class");
            t = e; first = 0;
            int u = t; while (is_space_cp(cp_at(p, u))) u++;
            if (at_lit(p, u, "..")) {
                u += 2; while (is_space_cp(cp_at(p, u))) u++;
                int f = rx_cclass_char(p, u);
                if (f < 0) panic_at(p, u, "Malformed range.");
                t = f;
            }
        }
        return rx_ws(p, t);
    }
    if (ch(p, q) == ':') {
        int t = q + 1;
        if (ch(p, t) == '!') t++;
        int e = r_identifier(p, t);
        if (e < 0) return -1;
        int c = ch(p, e);
        if (c == '<' || c == '(' || c == '[' || c == '{' || c == 0xC2) { int f = r_circumfix(p, e); if (f >= 0) e = f; }
        return rx_ws(p, e);
    }
    int e = r_identifier(p, q);
    if (e < 0) return -1;
    return rx_ws(p, e);
    (void) L;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_nested_nibbler(RkP *p, int pos, RkLang *L) {
    int q = rx_ws(p, pos);
    q = rx_termseq(p, q, L);
    if (rx_infixstopper(p, q, L)) return q;
    rx_unrec(p, q);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_assertion(RkP *p, int pos, RkLang *L) {
    int c = ch(p, pos);
    if (c == '?' || c == '!') {
        if (ch(p, pos + 1) == '>') return pos + 1;
        if (ch(p, pos + 1) == '{') return r_block(p, pos + 1);
        return rx_assertion(p, pos + 1, L);
    }
    if (c == '|') { int e = r_identifier(p, pos + 1); if (e < 0) panic_at(p, pos, "Malformed <|...> assertion"); return e; }
    if (c == '.') return rx_assertion(p, pos + 1, L);
    if (c == '{') return r_block(p, pos);
    if (c == '&') {
        int e = r_variable(p, pos);
        if (e < 0) return -1;
        int sqi = p->in_regex_assert; p->in_regex_assert = 1;
        if (ch(p, e) == ':') { int a = r_arglist(p, e + 1); p->in_regex_assert = sqi; return a >= 0 ? a : e + 1; }
        if (ch(p, e) == '(') { int a = r_arglist(p, e + 1); a = ws(p, a); p->in_regex_assert = sqi; if (ch(p, a) != ')') panic_at(p, a, "Unable to parse assertion arguments"); return a + 1; }
        p->in_regex_assert = sqi;
        return e;
    }
    if (c == '$' || c == '@' || c == '%') return r_variable(p, pos);
    if (at_lit(p, pos, "~~")) {
        int q = pos + 2;
        if (ch(p, q) == '>') return q;
        if (is_digit_cp(cp_at(p, q))) return r_decint(p, q);
        int e = r_longname(p, q); return e >= 0 ? e : q;
    }
    if (c == '[' || c == '+' || c == '-' || c == ':') {
        int q = pos; int n = 0;
        for (;;) {
            if (ch(p, q) == '>') break;
            int e = rx_cclass_elem(p, q, L);
            if (e < 0) break;
            q = e; n++;
        }
        if (n) return q;
        if (c == '-' || c == '+') { panic_at(p, pos, "Malformed character class"); }
        return -1;
    }
    int e = r_longname(p, pos);
    if (e < 0) return -1;
    int d = ch(p, e);
    if (d == '>') return e;
    if (d == '=') return rx_assertion(p, e + 1, L);
    if (d == ':') {
        int sqi = p->in_regex_assert; p->in_regex_assert = 1;
        int a = r_arglist(p, e + 1);
        p->in_regex_assert = sqi;
        return a >= 0 ? a : e + 1;
    }
    if (d == '(') {
        int a = r_arglist(p, e + 1); a = ws(p, a);
        if (ch(p, a) != ')') panic_at(p, a, "Unable to parse assertion arguments; couldn't find final ')'");
        return a + 1;
    }
    if (is_space_cp(cp_at(p, e)) || d == '#') return rx_nested_nibbler(p, e, L);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_group(RkP *p, int pos, RkLang *L, int close) {
    int e = rx_nested_nibbler(p, pos + 1, L);
    if (ch(p, e) != close) panic_at(p, e, "Unable to parse regex; couldn't find final '%c'", close);
    return e + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_in_regex(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_metachar(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c == ':') {
        int q = pos + 1;
        if (at_lit(p, q, "my") || at_lit(p, q, "constant") || at_lit(p, q, "state") || at_lit(p, q, "our") || at_lit(p, q, "temp") || at_lit(p, q, "let")) {
            int e = r_statement_in_regex(p, q);
            if (e >= 0) return e;
        }
        if (at_lit(p, pos, ":::")) panic_at(p, pos, "::: not yet implemented");
        if (at_lit(p, pos, "::")) panic_at(p, pos, ":: not yet implemented");
        if (is_space_cp(cp_at(p, q))) panic_at(p, pos, "Backtrack control ':' does not seem to have a preceding atom to control");
        int e = rx_mod_internal(p, pos);
        if (e >= 0) return e;
        return -1;
    }
    if (at_lit(p, pos, "{*}")) {
        int q = pos + 3;
        int h = hs(p, q);
        if (at_lit(p, h, "#= ")) { while (q < p->n && p->s[q] != '\n') q++; }
        return q;
    }
    if (c == '{') return r_block(p, pos);
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        int n1 = cp_at(p, pos + 1);
        if (c == '$' || c == '@') {
            if (n1 == '<') {
                int q = pos + 2; while (q < p->n && p->s[q] != '>') q++;
                if (ch(p, q) != '>') panic_at(p, pos, "Unable to parse named capture");
                q++;
                int t = rx_ws(p, q);
                if (ch(p, t) == '=' && ch(p, t + 1) != '=') { t = rx_ws(p, t + 1); int e = rx_quantified_atom(p, t, L); if (e >= 0) return e; }
                return q;
            }
            if (c == '$' && is_digit_cp(n1)) {
                int q = r_decint(p, pos + 1);
                int t = rx_ws(p, q);
                if (ch(p, t) == '=' && ch(p, t + 1) != '=') { t = rx_ws(p, t + 1); int e = rx_quantified_atom(p, t, L); if (e >= 0) return e; }
                return q;
            }
        }
        int tw = n1;
        int ok = is_alpha_cp(n1) || n1 == '(' || (!is_word_cp(tw) && !is_space_cp(tw) && tw >= 0 && is_alpha_cp(cp_at(p, pos + 1 + cp_len(p, pos + 1))));
        if (ok && !rx_stopper(p, pos + 1, L)) {
            int e = r_variable(p, pos);
            if (e >= 0) {
                int t = rx_ws(p, e);
                if (ch(p, t) == '=' && ch(p, t + 1) != '=' && c != '&') { t = rx_ws(p, t + 1); int f = rx_quantified_atom(p, t, L); if (f >= 0) return f; }
                return e;
            }
        }
        if (c == '$') { if (ch(p, pos + 1) == '$') return pos + 2; return pos + 1; }
        return -1;
    }
    if (c == '<' && is_space_cp(cp_at(p, pos + 1))) {
        RkLang W; lang_q(&W); W.start = '<'; W.stop = '>'; W.nrep = 1;
        return quote_body(p, pos + 1, &W, "quote words");
    }
    if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201A || c == 0x2019 || c == 0x201C || c == 0x201E || c == 0x201D || c == 0xFF62) return r_quote(p, pos);
    if (c == '[') return rx_group(p, pos, L, ']');
    if (c == '(') return rx_group(p, pos, L, ')');
    if (c == '.') return pos + 1;
    if (at_lit(p, pos, "^^")) return pos + 2;
    if (c == '^') return pos + 1;
    if (at_lit(p, pos, "<<") || c == 0xAB) return pos + cp_len(p, pos) + (c == '<' ? 1 : 0);
    if (at_lit(p, pos, ">>") || c == 0xBB) return pos + cp_len(p, pos) + (c == '>' ? 1 : 0);
    if (at_lit(p, pos, "<(")) return pos + 2;
    if (at_lit(p, pos, ")>")) return pos + 2;
    if (c == '\\') return rx_backslash(p, pos + 1);
    if (c == '~') {
        int q = rx_ws(p, pos + 1);
        int g = rx_quantified_atom(p, q, L);
        if (g < 0) panic_at(p, q, "Missing goal in ~ regex construct");
        q = rx_ws(p, g);
        int e = rx_quantified_atom(p, q, L);
        if (e < 0) panic_at(p, q, "Missing expression in ~ regex construct");
        return e;
    }
    if (c == '<') {
        int e = rx_assertion(p, pos + 1, L);
        if (e < 0) panic_at(p, pos, "Unrecognized regex metacharacter < (must be quoted to match literally)");
        if (ch(p, e) != '>') panic_at(p, e, "Unable to parse regex; couldn't find final '>'");
        return e + 1;
    }
    if ((c == '*' || c == '+' || c == '?') && !rx_stopper(p, pos, L)) panic_at(p, pos, "Quantifier quantifies nothing.");
    if (c == '%') panic_at(p, pos, "Missing quantifier on the left argument of %s", ch(p, pos + 1) == '%' ? "%%" : "%");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_atom(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (is_word_cp(c)) { int q = pos + cp_len(p, pos); while (is_mark_cp(cp_at(p, q))) q += cp_len(p, q); return q; }
    return rx_metachar(p, pos, L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_quantified_atom(RkP *p, int pos, RkLang *L) {
    if (pos >= p->n || rx_stopper(p, pos, L)) return -1;
    int c = ch(p, pos);
    if ((c == ')' && ch(p, pos + 1) != '>') || c == ']' || c == '}' || (c == '>' && ch(p, pos + 1) != '>')) return -1;
    if (c == '|' || c == '&') return -1;
    int a = rx_atom(p, pos, L);
    if (a < 0) return -1;
    int q = rx_ws(p, a);
    int e = -1;
    if (!rx_stopper(p, q, L)) e = rx_quantifier(p, q);
    if (e < 0 && ch(p, q) == ':') { int b = rx_backmod(p, q); if (b > q && !is_word_cp(cp_at(p, b))) e = b; }
    if (e >= 0) {
        q = rx_ws(p, e);
        if (ch(p, q) == '%') {
            int t = q + 1; if (ch(p, t) == '%') t++;
            t = rx_ws(p, t);
            int s = rx_quantified_atom(p, t, L);
            if (s < 0) panic_at(p, t, "Missing separator after %%");
            return rx_ws(p, s);
        }
        return q;
    }
    return q;
}
/*====================================================================================================================================================================================================*/
typedef struct { int prec; int sub; int assoc; unsigned flags; int from; int to; int nextterm; } OpInfo;
enum { NT_TERMISH = 0, NT_NULLTERMISH, NT_DOTTYOPISH };
enum { GOAL_NONE = 0, GOAL_BLOCK = '{', GOAL_BANGBANG = '!', GOAL_ENDARGS = 'e' };
static int r_term(RkP *p, int pos);
static int r_dottyop(RkP *p, int pos);
static int r_postcircumfix(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_stmt_mod_kw(RkP *p, int pos) {
    static const char *const w[] = { "if", "unless", "while", "until", "for", "given", "when", "with", "without", 0 };
    for (int i = 0; w[i]; i++) { int e = lit(p, pos, w[i]); if (e >= 0 && end_keyword_ok(p, e)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_terminator_at(RkP *p, int pos) {
    int c = ch(p, pos);
    if (p->ustop && pos + p->ustop_len <= p->n && !memcmp(p->s + pos, p->ustop, (size_t) p->ustop_len)) return 1;
    if (c == ';' || c == ')' || c == ']' || c == '}') return 1;
    if (c == '>' && p->in_regex_assert) return 1;
    if (at_lit(p, pos, "-->")) return 1;
    return is_stmt_mod_kw(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int stdstopper(RkP *p, int pos) { return marked_end(p, pos) || pos >= p->n || is_terminator_at(p, pos); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int at_lambda(RkP *p, int pos) { return at_lit(p, pos, "->") || at_lit(p, pos, "<->"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int infixstopper(RkP *p, int pos) {
    if (p->goal == GOAL_BANGBANG && at_lit(p, pos, "!!")) return 1;
    if ((ch(p, pos) == '{' || at_lambda(p, pos)) && had_ws_before(p, pos) && (p->goal == GOAL_BLOCK || p->goal == GOAL_ENDARGS)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int infix_pred_ok(RkP *p, int pos, const char *s) {
    if (!strcmp(s, "-")) return ch(p, pos + 1) != '>' || at_lit(p, pos + 1, ">>");
    if (!strcmp(s, "!=")) return is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ']';
    if (!strcmp(s, "<<") || !strcmp(s, ">>")) return 0;
    if ((!strcmp(s, "+<") || !strcmp(s, "~<")) && p->in_meta) return at_lit(p, pos + 2, "<<") || ch(p, pos + 2) != '<';
    if ((!strcmp(s, "+>") || !strcmp(s, "~>")) && p->in_meta) return at_lit(p, pos + 2, ">>") || ch(p, pos + 2) != '>';
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int op_table_longest(RkP *p, int pos, const RkOp *tab, const RkOp **best) {
    int bl = 0; *best = NULL;
    for (int i = 0; tab[i].sym; i++) {
        int l = (int) strlen(tab[i].sym);
        if (l <= bl) continue;
        if (!at_lit(p, pos, tab[i].sym)) continue;
        if ((tab[i].flags & OF_WORD) && wordch_at(p, pos + l)) continue;
        if (tab == rk_infix && !infix_pred_ok(p, pos, tab[i].sym)) continue;
        bl = l; *best = &tab[i];
    }
    return bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int user_op_longest_i(RkP *p, int pos, char cat, int *prec, int *idx) {
    int bl = 0; *idx = -1;
    for (int i = p->nuops - 1; i >= 0; i--) {
        RkUserOp *u = &p->uops[i];
        if (u->cat != cat || u->len <= bl) continue;
        if (pos + u->len > p->n || memcmp(p->s + pos, u->sym, (size_t) u->len)) continue;
        if (u->wordend && (cat == 't' || cat == 'p') && wordch_at(p, pos + u->len)) continue;
        bl = u->len; *prec = u->prec; *idx = i;
    }
    return bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int user_op_longest(RkP *p, int pos, char cat, int *prec) { int i; return user_op_longest_i(p, pos, cat, prec, &i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_op(OpInfo *o, const RkOp *r, int from, int to) {
    o->prec = r->prec; o->sub = r->sub ? r->sub : r->prec; o->assoc = r->assoc; o->flags = r->flags; o->from = from; o->to = to; o->nextterm = NT_TERMISH;
    if (r->flags & OF_COMMA) o->nextterm = NT_NULLTERMISH;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_infix_plain(RkP *p, int pos, OpInfo *o);
static int r_infixish(RkP *p, int pos, OpInfo *o, int in_meta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_ternary_rest(RkP *p, int pos) {
    int sg = p->goal; p->goal = GOAL_BANGBANG;
    int q = ws(p, pos);
    int e = r_EXPR(p, q, PR('i'));
    p->goal = sg;
    if (e < 0) panic_at(p, q, "Confused: Found ?? but no !!");
    int t = ws(p, e);
    if (at_lit(p, t, "!!")) return t + 2;
    if (at_lit(p, t, "::") && ch(p, t + 2) != '=') panic_at(p, t, "Please use !! rather than :: in the conditional operator");
    if (ch(p, t) == ':' && ch(p, t + 1) != '=' && !is_word_cp(cp_at(p, t + 1))) panic_at(p, t, "Please use !! rather than : in the conditional operator");
    OpInfo io;
    if (r_infix_plain(p, t, &io) >= 0) panic_at(p, t, "Precedence of %.*s is too loose to use inside ?? !!; please parenthesize", io.to - io.from, p->s + io.from);
    panic_at(p, t, "Confused: Found ?? but no !!");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_infix_plain(RkP *p, int pos, OpInfo *o) {
    const RkOp *best; int bl = op_table_longest(p, pos, rk_infix, &best);
    int uprec = 0; int uidx = -1; int ul = user_op_longest_i(p, pos, 'i', &uprec, &uidx);
    int c = ch(p, pos);
    if (at_lit(p, pos, "??") && ch(p, pos + 2) != '?') {
        if (ul > 2) goto user;
        int e = r_ternary_rest(p, pos + 2);
        o->prec = PR('j'); o->sub = o->prec; o->assoc = AS_RIGHT; o->flags = OF_FIDDLY; o->from = pos; o->to = e; o->nextterm = NT_TERMISH;
        return e;
    }
    if (c == '=' && !at_lit(p, pos, "==") && !at_lit(p, pos, "=>") && !at_lit(p, pos, "=:=") && !at_lit(p, pos, "=~") && !at_lit(p, pos, "===") && ul <= 1) {
        if (at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=") && !p->p5isms) panic_at(p, pos, "Unsupported use of =~ to do pattern matching; in Raku please use ~~");
        if (p->p5isms && at_lit(p, pos, "=~")) { o->prec = PR('m'); o->sub = o->prec; o->assoc = AS_LEFT; o->flags = OF_CHAIN | OF_IFFY; o->from = pos; o->to = pos + 2; o->nextterm = NT_TERMISH;
            return pos + 2; }
        int item = p->leftsigil == '$' || p->in_meta;
        o->prec = PR('i'); o->sub = item ? PR('i') : PR('e'); o->assoc = AS_RIGHT; o->flags = item ? 0 : OF_FIDDLY; o->from = pos; o->to = pos + 1; o->nextterm = NT_TERMISH;
        p->leftsigil = 0;
        return pos + 1;
    }
    if (at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=") && !p->p5isms) panic_at(p, pos, "Unsupported use of =~ to do pattern matching; in Raku please use ~~");
    if (p->p5isms && ((at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=")) || (at_lit(p, pos, "!~") && !at_lit(p, pos, "!~~")))) {
        o->prec = PR('m'); o->sub = o->prec; o->assoc = AS_LEFT; o->flags = OF_CHAIN | OF_IFFY; o->from = pos; o->to = pos + 2; o->nextterm = NT_TERMISH;
        return pos + 2;
    }
    if (at_lit(p, pos, "!~") && !at_lit(p, pos, "!~~") && is_space_cp(cp_at(p, pos + 2)) && !p->p5isms) panic_at(p, pos,
        "Unsupported use of !~ to do negated pattern matching; in Raku please use !~~");
    if (at_lit(p, pos, "::=")) panic_at(p, pos, "\"::=\" not yet implemented. Sorry.");
    if (at_lit(p, pos, ".=")) { o->prec = PR('v'); o->sub = PR('z'); o->assoc = AS_LEFT; o->flags = OF_FIDDLY | OF_DOTTY; o->from = pos; o->to = pos + 2; o->nextterm = NT_DOTTYOPISH; return pos + 2; }
    if (c == '.' && !at_lit(p, pos, "..") && ul <= 1) {
        int q = ws(p, pos + 1);
        if (p->in_reduce) return -1;
        if (!is_alpha_cp(cp_at(p, q))) {
            if (q != pos + 1) panic_at(p, pos, "Unsupported use of . to concatenate strings; in Raku please use ~");
            if (is_space_cp(prev_cp(p, pos))) panic_at(p, pos, "Malformed postfix call (only basic method calls that exclusively use a dot can be detached)");
            panic_at(p, pos, "Malformed postfix call");
        }
        o->prec = PR('v'); o->sub = PR('z'); o->assoc = AS_LEFT; o->flags = OF_FIDDLY | OF_DOTTY; o->from = pos; o->to = q; o->nextterm = NT_DOTTYOPISH;
        return q;
    }
    if (c == ':' && !at_lit(p, pos, ":=") && !at_lit(p, pos, "::=")) {
        if (p->invocant_ok && p->goal != GOAL_BANGBANG) {
            int n1 = cp_at(p, pos + 1);
            if (pos + 1 >= p->n || is_space_cp(n1) || is_terminator_at(p, pos + 1)) {
                p->invocant_ok = 0; p->saw_inv = 1;
                o->prec = PR('g'); o->sub = o->prec; o->assoc = AS_LIST; o->flags = 0; o->from = pos; o->to = pos + 1; o->nextterm = NT_NULLTERMISH;
                return pos + 1;
            }
        }
        return -1;
    }
    if (c == '?' && !at_lit(p, pos, "??") && !at_lit(p, pos, "?|") && !at_lit(p, pos, "?^") && !at_lit(p, pos, "?&")) {
        int q = pos + 1;
        while (q < p->n && p->s[q] != ';' && p->s[q] != ':') q++;
        if (ch(p, q) == ':') panic_at(p, pos, "Unsupported use of ? and : for the ternary conditional operator; in Raku please use ?? and !!");
        return -1;
    }
    if ((at_lit(p, pos, "<<") || at_lit(p, pos, ">>")) && !at_lit(p, pos, "<<==") && !at_lit(p, pos, "==>>") && !p->in_meta && is_space_cp(cp_at(p, pos + 2)) && ul < 2)
        panic_at(p, pos, "Unsupported use of %s to do %s shift; in Raku please use %s", c == '<' ? "<<" : ">>", c == '<' ? "left" : "right", c == '<' ? "+< or ~<" : "+> or ~>");
    if (best && !strcmp(best->sym, "..") && !p->in_meta && (ch(p, pos + 2) == ')' || ch(p, pos + 2) == ']')) panic_at(p, pos, "Please use ..* for indefinite range");
    if (ul > bl) goto user;
    if (!best) return -1;
    if (!strcmp(best->sym, ",")) p->invocant_ok = 0;
    set_op(o, best, pos, pos + bl);
    return pos + bl;
user:
    o->prec = uprec; o->sub = uprec; o->assoc = uidx >= 0 ? p->uops[uidx].assoc : AS_LEFT; o->flags = uidx >= 0 ? p->uops[uidx].flags : 0; o->from = pos; o->to = pos + ul; o->nextterm = NT_TERMISH;
    if (o->flags & OF_COMMA) o->nextterm = NT_NULLTERMISH;
    return pos + ul;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_infixish_core(RkP *p, int pos, OpInfo *o, int in_meta) {
    int sm = p->in_meta; p->in_meta = in_meta;
    int best = -1; OpInfo bo; memset(&bo, 0, sizeof bo);
    OpInfo t;
    int c = cp_at(p, pos);
    if (c == '[') {
        if (ch(p, pos + 1) == '&' && (is_alpha_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == '(' || strchr(".!^:*?=~", ch(p, pos + 2)))) {
            int e = r_variable(p, pos + 1);
            if (e >= 0 && ch(p, e) == ']') { t.prec = PR('t'); t.sub = t.prec; t.assoc = AS_LEFT; t.flags = 0; t.from = pos; t.to = e + 1; t.nextterm = NT_TERMISH; if (e + 1 > best) { best = e + 1;
                bo = t; } }
        }
        if (best < 0) {
            int e = r_infixish(p, pos + 1, &t, 'b');
            if (e >= 0 && ch(p, e) == ']') { t.from = pos; t.to = e + 1; best = e + 1; bo = t; }
        }
    }
    if (c == 0xAB || c == 0xBB || at_lit(p, pos, "<<") || at_lit(p, pos, ">>")) {
        int l = (c == '<' || c == '>') ? 2 : 2;
        int e = r_infixish(p, pos + l, &t, 'h');
        if (e >= 0) {
            int d = cp_at(p, e);
            int ok = (c == 0xAB || c == 0xBB) ? (d == 0xAB || d == 0xBB) : (at_lit(p, e, "<<") || at_lit(p, e, ">>"));
            if (ok) { int f = e + 2; if (f > best) { best = f; bo = t; bo.from = pos; bo.to = f; } }
            else if (c == 0xAB || c == 0xBB) panic_at(p, e, "Missing \xc2\xab or \xc2\xbb");
        }
    }
    int neg_bad = -1; int chose_meta = 0;
    if (c == '!' && ch(p, pos + 1) != '!') {
        int e = r_infixish(p, pos + 1, &t, 'n');
        if (e >= 0) {
            int is_eq = (e == pos + 2 && ch(p, pos + 1) == '=');
            if (is_eq) { t.prec = PR('m'); t.sub = t.prec; t.assoc = AS_LEFT; t.flags = OF_CHAIN | OF_IFFY; }
            else if (!(t.flags & OF_IFFY)) neg_bad = e;
            if (e > best) { best = e; bo = t; bo.from = pos; bo.to = e; chose_meta = 1; }
        }
    }
    if ((c == 'R' || c == 'S' || c == 'X' || c == 'Z') && !wordch_at(p, pos + 1)) {
        int e = r_infixish(p, pos + 1, &t, c);
        if (e >= 0 && e > best) {
            if (c == 'X' || c == 'Z') { t.prec = PR('f'); t.sub = t.prec; t.assoc = AS_LIST; t.flags = 0; }
            best = e; bo = t; bo.from = pos; bo.to = e;
        }
    }
    if ((c == 'R' || c == 'S' || c == 'X' || c == 'Z') && wordch_at(p, pos + 1)) {
        int e = r_infixish(p, pos + 1, &t, c);
        if (e >= 0 && e > best) {
            if (c == 'X' || c == 'Z') { t.prec = PR('f'); t.sub = t.prec; t.assoc = AS_LIST; t.flags = 0; }
            best = e; bo = t; bo.from = pos; bo.to = e;
        }
    }
    int e = r_infix_plain(p, pos, &t);
    if (e >= 0 && e >= best) { best = e; bo = t; chose_meta = 0; }
    p->in_meta = sm;
    if (best < 0) return -1;
    if (neg_bad >= 0 && best == neg_bad && chose_meta && c == '!') panic_at(p, pos, "Cannot negate %.*s because it is not iffy enough", neg_bad - pos - 1, p->s + pos + 1);
    if (ch(p, best) == '=' && ch(p, best + 1) != '=' && !(bo.flags & OF_DOTTY) && best - bo.from >= 1 && !(best - bo.from == 1 && ch(p, bo.from) == '=')) {
        int prevop_is_assign = (bo.prec == PR('i'));
        if (!prevop_is_assign && !(bo.prec == PR('m') && ch(p, best - 1) == '=')) {
            if (bo.flags & OF_DIFFY) panic_at(p, pos, "Cannot make assignment out of %.*s because %s operators are diffy", best - bo.from, p->s + bo.from, "structural");
            int item = bo.prec > PR('g');
            bo.prec = PR('i'); bo.sub = item ? PR('i') : PR('e'); bo.assoc = AS_RIGHT; bo.flags = 0; bo.nextterm = NT_TERMISH;
            best++; bo.to = best;
        }
    }
    *o = bo;
    return best;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_infixish(RkP *p, int pos, OpInfo *o, int in_meta) {
    if (!in_meta && (stdstopper(p, pos) || infixstopper(p, pos))) return -1;
    if (in_meta && pos >= p->n) return -1;
    if (!in_meta && !p->in_reduce && ch(p, pos) == ':' && (is_alpha_cp(cp_at(p, pos + 1)) || ch(p, pos + 1) == '!' || ch(p, pos + 1) == '<' || ch(p, pos + 1) == '(' ||
                                                           ch(p, pos + 1) == '[' || ch(p, pos + 1) == '{' || ch(p, pos + 1) == '$' || ch(p, pos + 1) == '@' || ch(p, pos + 1) == '%' ||
                                                           ch(p, pos + 1) == '&' || is_digit_cp(cp_at(p, pos + 1)) || cp_at(p, pos + 1) == 0xAB)) {
        if (!(is_digit_cp(cp_at(p, pos + 1)) && !is_alpha_cp(cp_at(p, r_decint(p, pos + 1))))) {
            int e = r_colonpair(p, pos);
            int k = ch(p, pos + 1);
            if (e >= 0 && (k == '{' || k == '[' || k == '<' || cp_at(p, pos + 1) == 0xAB)) panic_at(p, pos, "You can't adverb %.*s", e - pos, p->s + pos);
            if (e >= 0) { o->prec = PR('i'); o->sub = o->prec; o->assoc = AS_UNARY; o->flags = OF_FAKE; o->from = pos; o->to = e; o->nextterm = NT_TERMISH; return e; }
        }
    }
    if (!in_meta && (ch(p, pos) == '{' || at_lambda(p, pos))) {
        if (p->in_reduce) return -1;
        panic_at(p, pos, "Unexpected block in infix position (missing statement control word before the expression?)");
    }
    return r_infixish_core(p, pos, o, in_meta);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_prefixish(RkP *p, int pos, int *prec) {
    if (at_lambda(p, pos) || at_lit(p, pos, "???") || at_lit(p, pos, "!!!") || at_lit(p, pos, "...")) return -1;
    const RkOp *best; int bl = op_table_longest(p, pos, rk_prefix, &best);
    int uprec = 0; int ul = user_op_longest(p, pos, 'p', &uprec);
    int e = -1;
    if (ul > bl) { e = pos + ul; *prec = uprec; }
    else if (best) {
        const char *s = best->sym;
        if (!strcmp(s, "?") && at_lit(p, pos, "??")) panic_at(p, pos, "Expected a term, but found either infix ?? or redundant prefix ?");
        if (!strcmp(s, "!") && at_lit(p, pos, "!!")) return -1;
        if (!strcmp(s, "~") && at_lit(p, pos, "~~")) panic_at(p, pos, "Expected a term, but found either infix ~~ or redundant prefix ~");
        if (!strcmp(s, "^") && at_lit(p, pos, "^^")) panic_at(p, pos, "Expected a term, but found either infix ^^ or redundant prefix ^");
        if (best->flags & OF_WORD) {
            if (!end_keyword_ok(p, pos + bl)) return -1;
            e = pos + bl;
            if (!strcmp(s, "let") || !strcmp(s, "temp")) { int c = cp_at(p, e); if (!(is_space_cp(c) || c == '#')) return -1; }
        }
        else e = pos + bl;
        *prec = best->prec;
    }
    if (e < 0) return -1;
    if (cp_at(p, e) == 0xAB) e += 2;
    else if (at_lit(p, e, "<<") && !at_lit(p, e, "<<==")) e += 2;
    return ws(p, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postfix_op(RkP *p, int pos) {
    const RkOp *best; int bl = op_table_longest(p, pos, rk_postfix, &best);
    int uprec = 0; int ul = user_op_longest(p, pos, 'P', &uprec);
    if (at_lit(p, pos, "->")) {
        int c = ch(p, pos + 2);
        if (c == '[' || c == '{' || c == '(') panic_at(p, pos, "Unsupported use of ->(), ->{} or ->[] as postfix dereferencer; in Raku please use .(), .[] or .{} to deref");
        panic_at(p, pos, "Unsupported use of -> as postfix; in Raku please use either . to call a method, or whitespace to delimit a pointy block");
    }
    int c = cp_at(p, pos);
    if (c == 0x207B || c == 0x207A || c == 0xAF || c == 0x2070 || c == 0xB9 || c == 0xB2 || c == 0xB3 || (c >= 0x2074 && c <= 0x2079)) {
        int q = pos;
        if (c == 0x207B || c == 0x207A || c == 0xAF) q += cp_len(p, q);
        int d = cp_at(p, q); int n = 0;
        while (d == 0x2070 || d == 0xB9 || d == 0xB2 || d == 0xB3 || (d >= 0x2074 && d <= 0x2079)) { q += cp_len(p, q); d = cp_at(p, q); n++; }
        if (n && q - pos > bl && q - pos > ul) return q;
    }
    if (ul > bl) return pos + ul;
    if (!best) return -1;
    return pos + bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_methodop(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_dotty(RkP *p, int pos) {
    if (ch(p, pos) != '.') return -1;
    int q = pos + 1;
    int c = ch(p, q);
    if (c == '.') return -1;
    if (c == '+' || c == '*' || c == '?' || c == '=' || c == '^') {
        int t = q + 1;
        if (c == '^' && ch(p, t) == '!') t++;
        int e = r_dottyop(p, t);
        if (e >= 0) return e;
    }
    return r_dottyop(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postfixish(RkP *p, int pos) {
    if (stdstopper(p, pos)) return -1;
    int wf = marked_ws_from(p, pos);
    if (wf >= 0 && wf != pos) return -1;
    int q = pos;
    if (!p->qsigil) { int u = r_unsp(p, q); if (u >= 0) q = u; else if (ch(p, q) == '\\' && q + 1 < p->n && !is_space_cp(cp_at(p, q + 1))) q++; }
    int meta = 0;
    {
        int t = q;
        if (ch(p, t) == '.') { int u = t + 1; int v = r_unsp(p, u); if (v >= 0) u = v; if (cp_at(p, u) == 0xBB || at_lit(p, u, ">>")) t = u; }
        if (cp_at(p, t) == 0xBB || (at_lit(p, t, ">>") && !(p->qsigil && ch(p, t + 2) == '('))) {
            t += 2; meta = 1;
            int v = r_unsp(p, t); if (v >= 0) t = v;
            q = t;
        }
    }
    int e;
    if (p->nuops) {
        int prec; int idx; int l = user_op_longest_i(p, q, 'k', &prec, &idx);
        if (l > 0 && idx >= 0 && idx + 1 < p->nuops && p->uops[idx + 1].cat == 'K') {
            RkUserOp *cl = &p->uops[idx + 1];
            const char *su = p->ustop; int sl = p->ustop_len; int sq = p->qsigil;
            p->ustop = cl->sym; p->ustop_len = cl->len; p->qsigil = 0;
            int t = r_semilist(p, q + l);
            p->ustop = su; p->ustop_len = sl; p->qsigil = sq;
            t = ws(p, t);
            if (t + cl->len <= p->n && !memcmp(p->s + t, cl->sym, (size_t) cl->len)) return t + cl->len;
            panic_at(p, t, "Unable to parse postcircumfix:<%s %s>; couldn't find final %s", p->uops[idx].sym, cl->sym, cl->sym);
        }
    }
    if ((e = r_postfix_op(p, q)) >= 0) return e;
    if (ch(p, q) == '.' && !is_word_cp(cp_at(p, q + 1)) && (e = r_postfix_op(p, q + 1)) >= 0) return e;
    if ((e = r_postcircumfix(p, q)) >= 0) return e;
    if (ch(p, q) == '.' && (ch(p, q + 1) == '[' || ch(p, q + 1) == '{' || ch(p, q + 1) == '<' || ch(p, q + 1) == '(' || cp_at(p, q + 1) == 0xAB) && (e = r_postcircumfix(p, q + 1)) >= 0) return e;
    if ((e = r_dotty(p, q)) >= 0) return e;
    if (ch(p, q) == '!' && ch(p, q + 1) != '!' && ch(p, q + 1) != '=' && ch(p, q + 1) != '~' && (e = r_methodop(p, q + 1)) >= 0) return e;
    if (meta && !p->qsigil) {
        if (is_space_cp(cp_at(p, q))) panic_at(p, q, "Missing postfix");
        if (is_alpha_cp(cp_at(p, q))) panic_at(p, q, "Missing dot on method call");
        panic_at(p, q, "Malformed postfix");
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bracket_ending_at(RkP *p, int end) {
    int c = prev_cp(p, end);
    return c == ')' || c == '}' || c == ']' || c == '>' || c == 0xBB;
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
        char msg[400]; memcpy(msg, p->msg, sizeof msg); int ep = p->err_pos;
        if (!setjmp(p->jb)) { e = r_infix_plain(p, pos, &o); }
        else e = -1;
        memcpy(p->jb, save, sizeof save); memcpy(p->msg, msg, sizeof msg); p->err_pos = ep;
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
static int r_termish(RkP *p, int pos) {
    int q = pos; int npre = 0; int lastpre = pos;
    int prec;
    for (;;) {
        int e = r_prefixish(p, q, &prec);
        if (e < 0) break;
        lastpre = q; q = e; npre++;
    }
    int t = r_term(p, q);
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
        return res;
    }
    for (;;) { int e = r_postfixish(p, t); if (e < 0) break; t = e; p->leftsigil = '@'; }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_nulltermish(RkP *p, int pos) {
    if (is_terminator_at(p, pos) || pos >= p->n) return pos;
    int e = r_termish(p, pos);
    return e >= 0 ? e : pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int paren_list_term(RkP *p, int from, int to) {
    if (ch(p, from) != '(' || ch(p, to - 1) != ')') return 0;
    int d = 0, comma = 0;
    for (int k = from; k < to; k++) {
        char b = p->s[k];
        if (b == '\'' || b == '"') { char qc = b; k++; while (k < to && p->s[k] != qc) { if (p->s[k] == '\\') k++; k++; } continue; }
        if (b == '(' || b == '[' || b == '{') d++;
        else if (b == ')' || b == ']' || b == '}') { d--; if (d == 0 && k != to - 1) return 0; }
        else if (b == ',' && d == 1) comma = 1;
    }
    return comma;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_EXPR(RkP *p, int pos, int preclim) {
    int sl = p->leftsigil; p->leftsigil = 0;
    int noinfix = (preclim == PR('y'));
    int base = p->nops;
    StackOp *st; int ns = 0;
    int q = pos; int nextterm = NT_TERMISH; int first = 1;
    int last_end = -1;
    for (;;) {
        int t;
        if (nextterm == NT_DOTTYOPISH) t = r_dottyop(p, q);
        else if (nextterm == NT_NULLTERMISH) t = r_nulltermish(p, q);
        else t = r_termish(p, q);
        if (t < 0) {
            if (ns) panic_at(p, q, "Missing required term after infix");
            p->leftsigil = sl; p->nops = base;
            return first ? -1 : last_end;
        }
        first = 0; last_end = t;
        if (noinfix) break;
        int w = r_ws(p, t);
        if (w < 0) break;
        last_end = w;
        OpInfo o;
        int saved_ls = p->leftsigil;
        int e;
        for (;;) {
            e = r_infixish(p, w, &o, 0);
            if (e < 0) break;
            if (o.prec <= preclim) { e = -1; p->leftsigil = saved_ls; break; }
            st = p->ops + base;
            int ns0 = ns;
            while (ns && st[ns - 1].sub > o.prec) ns--;
            if (ns == ns0 && o.to - o.from >= 2 && at_lit(p, o.from, ":=") && paren_list_term(p, q, t)) panic_at(p, o.from, "Cannot use bind operator with this left-hand side");
            if (at_lit(p, o.from, "==>>") || at_lit(p, o.from, "<<==")) panic_at(p, o.from, "%.4s feed operator not yet implemented. Sorry.", p->s + o.from);
            if (o.flags & OF_FAKE) { last_end = e; w = r_ws(p, e); if (w < 0) { e = -1; break; } last_end = w; continue; }
            break;
        }
        if (e < 0) break;
        st = p->ops + base;
        if (ns && st[ns - 1].sub == o.prec) {
            if (o.assoc == AS_NON) panic_at(p, o.from, "Operators '%.*s' and '%.*s' are non-associative and require parentheses", st[ns - 1].to - st[ns - 1].from, p->s + st[ns - 1].from,
                o.to - o.from, p->s + o.from);
            if (o.assoc == AS_LEFT) ns--;
            else if (o.assoc == AS_LIST) {
                const char *a = p->s + st[ns - 1].from; int al = st[ns - 1].to - st[ns - 1].from;
                const char *b = p->s + o.from; int bl2 = o.to - o.from;
                while (al > 0 && asc_space((unsigned char) a[al - 1])) al--;
                while (bl2 > 0 && asc_space((unsigned char) b[bl2 - 1])) bl2--;
                if (!(al == bl2 && !memcmp(a, b, (size_t) al)) && !(al == 1 && a[0] == ':'))
                    panic_at(p, o.from, "Only identical operators may be list associative; since '%.*s' and '%.*s' differ, they are non-associative and you need to clarify with parentheses", al, a,
                        bl2, b);
            }
        }
        p->nops = base + ns;
        RK_GROW(p->ops, p->nops, p->cops, StackOp);
        st = p->ops + base;
        st[ns].prec = o.prec; st[ns].sub = o.sub; st[ns].assoc = o.assoc; st[ns].from = o.from; st[ns].to = o.to; ns++;
        p->nops = base + ns;
        nextterm = o.nextterm;
        q = ws(p, e);
    }
    p->leftsigil = sl ? sl : p->leftsigil;
    p->nops = base;
    return last_end;
}
/*====================================================================================================================================================================================================*/
static int r_semilist(RkP *p, int pos) {
    int q = ws(p, pos);
    for (;;) {
        int c = ch(p, q);
        if (c == ')' || c == ']' || c == '}' || q >= p->n) return q;
        if (p->ustop && q + p->ustop_len <= p->n && !memcmp(p->s + q, p->ustop, (size_t) p->ustop_len)) return q;
        int e = r_statement(p, q);
        if (e < 0) return q;
        q = r_eat_terminator(p, e);
        if (q < 0) return e;
        q = ws(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int expect_close(RkP *p, int pos, int c, const char *what, int open_pos) {
    int q = ws(p, pos);
    if (ch(p, q) != c) panic_at(p, q, "Unable to parse %s; couldn't find final '%c' (corresponding starter was at line %d)", what, c, line_of(p, open_pos));
    return q + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_pblock(RkP *p, int pos, int implicit);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int words_quote(RkP *p, int pos, int open, int nrep, int qq) {
    RkLang L; if (qq) { lang_qq(&L); L.ww = 1; } else lang_q(&L);
    L.words = 1; L.start = open; L.nrep = nrep; L.stop = opener_close(open);
    int s = quote_starter(p, pos, &L);
    return quote_body(p, s, &L, "quote words");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_circumfix(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c == '(') { int sa = p->invocant_ok; int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); p->invocant_ok = sa; p->goal = sg; return expect_close(p, e, ')',
        "parenthesized expression", pos); }
    if (c == '[') { int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); p->goal = sg; return expect_close(p, e, ']', "array composer", pos); }
    if (c == '{') { int sg = p->goal; p->goal = 0; int e = r_pblock(p, pos, 1); p->goal = sg; return e; }
    if (at_lit(p, pos, "<<")) return words_quote(p, pos, '<', 2, 1);
    if (c == 0xAB) return words_quote(p, pos, 0xAB, 1, 1);
    if (c == '<') {
        if (ch(p, pos + 1) == '>') panic_at(p, pos, "Unsupported use of <>; in Raku please use lines() to read input, ('') to represent a null string or () to represent an empty list");
        if (at_lit(p, pos, "<STDIN>")) panic_at(p, pos, "Unsupported use of <STDIN>; in Raku please use $*IN.lines (or add whitespace to suppress warning)");
        return words_quote(p, pos, '<', 1, 0);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postcircumfix(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c == '[') { int sq = p->qsigil; p->qsigil = 0; int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); p->qsigil = sq; p->goal = sg; return expect_close(p, e, ']', "subscript", pos); }
    if (c == '{') { int sq = p->qsigil; p->qsigil = 0; int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); p->qsigil = sq; p->goal = sg; return expect_close(p, e, '}', "subscript", pos); }
    if (c == '(') {
        int sq = p->qsigil; p->qsigil = 0; int sg = p->goal; p->goal = 0;
        int e = r_arglist(p, ws(p, pos + 1));
        p->qsigil = sq; p->goal = sg;
        return expect_close(p, e, ')', "argument list", pos);
    }
    if (at_lit(p, pos, "<<")) {
        RkLang L; lang_qq(&L); L.ww = 1; L.start = '<'; L.stop = '>'; L.nrep = 2;
        int e; r_nibble_until(p, pos + 2, &L, &e);
        if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '>>' (corresponding '<<' was at line %d)", line_of(p, pos));
        return quote_stopper(p, e, &L);
    }
    if (c == 0xAB) {
        RkLang L; lang_qq(&L); L.ww = 1; L.start = 0xAB; L.stop = 0xBB; L.nrep = 1;
        int e; r_nibble_until(p, pos + 2, &L, &e);
        if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '\xc2\xbb'");
        return quote_stopper(p, e, &L);
    }
    if (c == '<') {
        RkLang L; lang_q(&L); L.start = '<'; L.stop = '>'; L.nrep = 1;
        int e; r_nibble_until(p, pos + 1, &L, &e);
        if (e >= 0 && quote_stopper(p, e, &L) >= 0) return quote_stopper(p, e, &L);
        int q = pos + 1; while (ch(p, q) == '=') q++;
        int h = hs(p, q);
        int d = cp_at(p, h);
        if (is_digit_cp(d) || d == '$' || d == '@' || d == '%' || d == '&' || d == ':') panic_at(p, pos, "Whitespace required before %.*s operator", q - pos, p->s + pos);
        panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '>' (corresponding '<' was at line %d)", line_of(p, pos));
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_arglist(RkP *p, int pos) {
    int sg = p->goal; int sq = p->qsigil;
    p->goal = GOAL_ENDARGS; p->qsigil = 0;
    int q = ws(p, pos);
    int e = q;
    if (!stdstopper(p, q)) { int x = r_EXPR(p, q, PR('e')); if (x >= 0) e = x; }
    p->goal = sg; p->qsigil = sq;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_semiarglist(RkP *p, int pos) {
    int q = r_arglist(p, pos);
    for (;;) { int t = ws(p, q); if (ch(p, t) != ';') { q = t; break; } q = r_arglist(p, t + 1); }
    return ws(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_args(RkP *p, int pos, int invocant_ok) {
    int si = p->invocant_ok; int sg = p->goal; int sv = p->saw_inv;
    p->invocant_ok = invocant_ok; p->goal = 0; p->saw_inv = 0;
    int e = pos;
    if (ch(p, pos) == '(') { int q = r_semiarglist(p, pos + 1); if (ch(p, q) != ')') panic_at(p, q, "Unable to parse argument list; couldn't find final ')' (corresponding starter was at line %d)",
        line_of(p, pos)); e = q + 1; }
    else {
        int u = r_unsp(p, pos);
        if (u >= 0 && ch(p, u) == '(') { int q = r_semiarglist(p, u + 1); if (ch(p, q) != ')') panic_at(p, q, "Unable to parse argument list; couldn't find final ')'"); e = q + 1; }
        else if (is_space_cp(cp_at(p, pos))) { e = r_arglist(p, pos + cp_len(p, pos)); }
    }
    p->last_inv = p->saw_inv;
    p->invocant_ok = si; p->goal = sg; p->saw_inv = sv;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_methodop(RkP *p, int pos) {
    int q = -1;
    int c = cp_at(p, pos);
    if (c == '$' || c == '@' || c == '&') q = r_variable(p, pos);
    else if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201C || c == 0xFF62) {
        if (p->qsigil && c == '"') { int t = pos + 1; while (t < p->n && p->s[t] != '"' && !is_space_cp(cp_at(p, t))) t += cp_len(p, t); if (t >= p->n || p->s[t] != '"') return -1; }
        q = r_quote(p, pos);
        if (q >= 0 && !(ch(p, q) == '(' || at_lit(p, q, ".(") || ch(p, q) == '\\')) panic_at(p, q,
            "Quoted method name requires parenthesized arguments. If you meant to concatenate two strings, use '~'.");
    }
    else {
        q = r_longname(p, pos);
        if (q >= 0 && q - pos == 2 && at_lit(p, pos, "::")) panic_at(p, pos, "Malformed class-qualified postfix call");
    }
    if (q < 0) return -1;
    int u = r_unsp(p, q); if (u >= 0) q = u;
    if (ch(p, q) == '(') q = r_args(p, q, 0);
    else if (ch(p, q) == ':' && (is_space_cp(cp_at(p, q + 1)) || ch(p, q + 1) == '{') && !p->qsigil) q = r_arglist(p, q + 1);
    else if (p->qsigil && ch(p, q) != '.') return -1;
    u = r_unsp(p, q); if (u >= 0) q = u;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_dottyop(RkP *p, int pos) {
    int q = pos;
    int u = r_unsp(p, q); if (u >= 0) q = u;
    int e = r_methodop(p, q);
    if (e >= 0) return e;
    if (ch(p, q) == ':') { e = r_colonpair(p, q); if (e >= 0) return e; }
    if (!is_alpha_cp(cp_at(p, q))) {
        if ((e = r_postfix_op(p, q)) >= 0) return e;
        if ((e = r_postcircumfix(p, q)) >= 0) return e;
    }
    return -1;
}
/*====================================================================================================================================================================================================*/
static int r_desigilname(RkP *p, int pos) {
    int c = ch(p, pos);
    if ((c == '$' || c == '@' || c == '%' || c == '&') && (ch(p, pos + 1) == '$' || ch(p, pos + 1) == '@' || ch(p, pos + 1) == '%' || ch(p, pos + 1) == '&')) return r_variable(p, pos);
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        if (p->in_decl) panic_at(p, pos, "Cannot declare a variable by indirect name (use a hash instead?)");
        return r_variable(p, pos);
    }
    return r_longname(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_twigil_at(RkP *p, int pos) {
    int c = ch(p, pos);
    if (!(c == '.' || c == '!' || c == '^' || c == ':' || c == '*' || c == '?' || c == '=' || c == '~')) return 0;
    return is_word_cp(cp_at(p, pos + 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_special_variable(RkP *p, int pos) {
    if (at_lit(p, pos, "$!{")) panic_at(p, pos, "Unsupported use of %%! variable; in Raku please use $!");
    if (ch(p, pos) != '$' && ch(p, pos) != '@' && ch(p, pos) != '%') return -1;
    int c1 = ch(p, pos + 1);
    int s = ch(p, pos);
    if (s == '$') {
        if (c1 == '@' && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ';' || ch(p, pos + 2) == ',' || ch(p, pos + 2) == ')')) panic_at(p, pos,
            "Unsupported use of $@ variable; in Raku please use $!");
        if (c1 == '$' && !is_word_cp(cp_at(p, pos + 2)) && ch(p, pos + 2) != '$' && ch(p, pos + 2) != '{' && ch(p, pos + 2) != '(' && ch(p, pos + 2) != '_' && ch(p, pos + 2) != '<')
            panic_at(p, pos, "Unsupported use of $$ variable; in Raku please use $*PID");
        if (c1 == '&' && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || is_terminator_at(p, pos + 2))) panic_at(p, pos, "Unsupported use of $& variable; in Raku please use $<>");
        if (c1 == '`' && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || is_terminator_at(p, pos + 2))) panic_at(p, pos, "Unsupported use of $` variable; in Raku please use $/.prematch");
        if (c1 == '\'' && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || is_terminator_at(p, pos + 2))) panic_at(p, pos,
            "Unsupported use of $' variable; in Raku please use $/.postmatch");
        if (c1 == '#' && is_alpha_cp(cp_at(p, pos + 2))) panic_at(p, pos, "Unsupported use of $# variable; in Raku please use .end");
        if (c1 == '|' && ch(p, hs(p, pos + 2)) == '=') panic_at(p, pos, "Unsupported use of $| variable; in Raku please use :autoflush on open");
        if (c1 == ';' && ch(p, hs(p, pos + 2)) == '=') panic_at(p, pos, "Unsupported use of $; variable; in Raku please use real multidimensional hashes");
        if (c1 == '"' && ch(p, hs(p, pos + 2)) == '=') panic_at(p, pos, "Unsupported use of $\" variable; in Raku please use .join() method");
        if (c1 == ',' && ch(p, hs(p, pos + 2)) == '=') panic_at(p, pos, "Unsupported use of $, variable; in Raku please use .join() method");
        if (c1 == '.' && !is_word_cp(cp_at(p, pos + 2)) && ch(p, pos + 2) != '(' && ch(p, pos + 2) != ':' && ch(p, pos + 2) != '^') panic_at(p, pos,
            "Unsupported use of $. variable; in Raku please use the .kv method on e.g. .lines");
        if (c1 == '?' && !is_word_cp(cp_at(p, pos + 2)) && ch(p, pos + 2) != '(') panic_at(p, pos, "Unsupported use of $? variable; in Raku please use $! for handling child errors also");
        if (c1 == ']' && !is_word_cp(cp_at(p, pos + 2)) && ch(p, pos + 2) != '(') panic_at(p, pos, "Unsupported use of $] variable; in Raku please use $*RAKU.version or $*RAKU.compiler.version");
        if (c1 == '\\' && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || ch(p, pos + 2) == '=' || is_terminator_at(p, pos + 2))) panic_at(p, pos,
            "Unsupported use of $\\ variable; in Raku please use :nl-out on open");
        if (c1 == '/' && ch(p, hs(p, pos + 2)) == '=' && (ch(p, hs(p, hs(p, pos + 2) + 1)) == '"' || ch(p, hs(p, hs(p, pos + 2) + 1)) == '\'')) panic_at(p, pos,
            "Unsupported use of $/ variable as input record separator; in Raku please use the filehandle's :nl-in attribute");
        if ((c1 == '+' || c1 == '-') && ch(p, pos + 2) == '[') panic_at(p, pos, "Unsupported use of @%c variable; in Raku please use .from/.to", c1);
    }
    if (s == '@' && (c1 == '+' || c1 == '-') && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || ch(p, pos + 2) == '[' || ch(p, pos + 2) == '{' || is_terminator_at(p, pos + 2)))
        panic_at(p, pos, "Unsupported use of @%c variable; in Raku please use .from/.to", c1);
    if (s == '%' && (c1 == '+' || c1 == '-') && (is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ',' || is_terminator_at(p, pos + 2))) panic_at(p, pos,
        "Unsupported use of %%%c variable; in Raku please use $/", c1);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int name_part_len(RkP *p, int from, int to);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void strs_add(RkStrs *a, const char *s, int n) { RK_GROW(a->v, a->n, a->c, char *); a->v[a->n++] = ct_strndup0(s, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int strs_has(const RkStrs *a, const char *s, int n) {
    for (int i = 0; i < a->n; i++) if ((int) strlen(a->v[i]) == n && !memcmp(a->v[i], s, (size_t) n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void record_attribute(RkP *p, int sigil, const char *name, int n) {
    if (!p->npkgs || n <= 0 || n > 250) return;
    char buf[256]; buf[0] = (char) sigil; buf[1] = '!'; memcpy(buf + 2, name, (size_t) n);
    strs_add(&p->pkgs[p->npkgs - 1].attrs, buf, n + 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void record_does(RkP *p, int from, int to) {
    if (!p->npkgs) return;
    RkPkg *k = &p->pkgs[p->npkgs - 1];
    for (int t = from; t + 4 <= to; t++) {
        if (memcmp(p->s + t, "does", 4) || wordch_at(p, t + 4) || (t > 0 && asc_word((unsigned char) p->s[t - 1]))) continue;
        int q = ws(p, t + 4);
        int l = r_longname(p, q);
        if (l < 0) { k->unknown = 1; continue; }
        strs_add(&k->roles, p->s + q, name_part_len(p, q, l));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int role_has_attribute(RkP *p, const char *role, const char *a, int n, int depth) {
    if (depth > 16) return -1;
    for (int i = p->nroledb - 1; i >= 0; i--) {
        RkPkg *r = &p->roledb[i];
        if (strcmp(r->name, role)) continue;
        if (r->unknown) return -1;
        if (strs_has(&r->attrs, a, n)) return 1;
        int res = 0;
        for (int j = 0; j < r->roles.n; j++) { int x = role_has_attribute(p, r->roles.v[j], a, n, depth + 1); if (x) { res = x; if (x > 0) return 1; } }
        return res;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void package_close(RkP *p) {
    RkPkg k = p->pkgs[--p->npkgs];
    if (!strcmp(k.kind, "role") && k.name) { RK_GROW(p->roledb, p->nroledb, p->croledb, RkPkg); p->roledb[p->nroledb++] = k; }
    if (k.unknown || !(!strcmp(k.kind, "class") || !strcmp(k.kind, "grammar") || !strcmp(k.kind, "role"))) return;
    for (int i = 0; i < k.nuses; i++) {
        const char *a = p->s + k.uses[i].pos; int n = k.uses[i].len;
        if (strs_has(&k.attrs, a, n)) continue;
        int found = 0;
        for (int j = 0; j < k.roles.n && found <= 0; j++) { int x = role_has_attribute(p, k.roles.v[j], a, n, 0); if (x) found = x; }
        if (found) continue;
        panic_at(p, k.uses[i].pos, "Attribute %.*s not declared in %s %s", n, a, k.kind, k.name ? k.name : "<anon>");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check_lexical_variable(RkP *p, int pos, int e, int twig) {
    int c = ch(p, pos);
    int c1 = ch(p, pos + 1);
    if (!twig && (c1 == '$' || c1 == '@' || c1 == '%' || c1 == '&')) { check_lexical_variable(p, pos + 1, e, 0); return; }
    if (c == '&' || memmem(p->s + pos, (size_t) (e - pos), "::", 2)) return;
    int nb = pos + 1 + (twig ? 1 : 0);
    e = nb + name_part_len(p, nb, e);
    if (p->in_decl && (p->scope == 3 || p->scope == 4) && (twig == 0 || twig == '.' || twig == '!')) record_attribute(p, c, p->s + nb, e - nb);
    if (!p->in_decl && twig == '!' && p->npkgs) {
        RkPkg *k = &p->pkgs[p->npkgs - 1];
        RK_GROW(k->uses, k->nuses, k->cuses, RkMyst); k->uses[k->nuses].pos = pos; k->uses[k->nuses].len = e - pos; k->uses[k->nuses].depth = 0; k->nuses++;
    }
    if ((twig == '^' || twig == ':') && e - pos == 3 && p->s[pos + 2] >= 'A' && p->s[pos + 2] <= 'Z') panic_at(p, pos, "Unsupported use of %.3s variable", p->s + pos);
    if (twig == '=' && !p->in_decl && !(e - pos == 5 && !memcmp(p->s + pos + 2, "pod", 3)) && !(e - pos == 8 && !memcmp(p->s + pos + 2, "finish", 6)))
        panic_at(p, pos, "Pod variable %.*s not yet implemented. Sorry.", e - pos, p->s + pos);
    if (twig == '^' || twig == ':') { char buf[256]; int n = e - pos - 1; if (n < 1 || n > 250) return; buf[0] = (char) c; memcpy(buf + 1, p->s + pos + 2, (size_t) n - 1);
        if (user_name_index(p, buf, n) < 0) add_name_n(p, buf, n); return; }
    if (twig) return;
    if (p->in_decl) {
        add_name_n(p, p->s + pos, e - pos);
        if (p->load_depth) { RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = ct_strndup0(p->s + pos, e - pos); }
        return;
    }
    if (p->qsigil || p->myst_off || p->lax || user_name_index(p, p->s + pos, e - pos) >= 0) return;
    if (e - pos == 2 && (p->s[pos + 1] == '_' || p->s[pos + 1] == '/' || p->s[pos + 1] == '!')) return;
    panic_at(p, pos, "Variable '%.*s' is not declared", e - pos, p->s + pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int compile_time_var_known(const char *s, int n) {
    static const char *const known[] = { "$?FILE", "$?LINE", "$?DISTRIBUTION", "$?LANG", "%?LANG", "$?NL", "$?BITS", "$?TABSTOP", "$?PACKAGE", "%?RESOURCES", "$?CHECKSUM", "$?FILES",
                                         "$?SOURCE", "$?STRICT", "$?LANGUAGE-REVISION", "%?REQUIRE-SYMBOLS", "$?CONCRETIZATION", "$?CLASS", "$?ROLE", "$?MODULE", "$?REGEX", "&?ROUTINE",
                                         "&?BLOCK", 0 };
    for (int i = 0; known[i]; i++) if ((int) strlen(known[i]) == n && !memcmp(known[i], s, (size_t) n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_variable(RkP *p, int pos) {
    int c = ch(p, pos);
    int sm = p->in_meta; p->in_meta = 0;
    int e = -1;
    if (at_lit(p, pos, "&[")) {
        OpInfo o; int q = r_infixish(p, pos + 2, &o, 'b');
        if (q >= 0 && ch(p, q) == ']') e = q + 1;
    }
    if (e < 0 && (c == '$' || c == '@' || c == '%' || c == '&')) {
        int sp = r_special_variable(p, pos); (void) sp;
        int q = pos + 1;
        int twig = 0;
        if (at_lit(p, q, ".^") && is_alpha_cp(cp_at(p, q + 2))) { e = r_identifier(p, q + 2); twig = '.'; }
        else {
            if (is_twigil_at(p, q)) { twig = ch(p, q); q++; }
            int d = r_desigilname(p, q);
            if (d < 0 && twig) { twig = 0; q = pos + 1; d = r_desigilname(p, q); }
            if (d >= 0) e = d;
            if (d >= 0) check_lexical_variable(p, pos, d, twig);
            if (d >= 0 && twig == '?' && !p->in_decl && !memmem(p->s + pos, (size_t) (d - pos), "::", 2) && !compile_time_var_known(p->s + pos, d - pos))
                panic_at(p, pos, "Variable '%.*s' is not declared", d - pos, p->s + pos);
            if (d < 0 && !twig) {
                int d1 = ch(p, q);
                if (is_digit_cp(cp_at(p, q))) { if (p->in_decl) panic_at(p, pos, "Cannot declare a numeric variable"); e = r_decint(p, q); }
                else if (d1 == '<') { if (p->in_decl) panic_at(p, pos, "Cannot declare a match variable"); e = r_postcircumfix(p, q); }
                else if ((d1 == '(' || d1 == '[' || d1 == '{') && !p->in_decl && !(d1 == '[' && c == '$') && !(d1 == '{' && c == '$')) {
                    if (d1 == '(') { int s = r_semilist(p, q + 1); e = expect_close(p, s, ')', "contextualizer", q); }
                    else e = r_circumfix(p, q);
                }
                else if (d1 == '[' && c == '$' && !p->in_decl) e = r_circumfix(p, q);
                else if (d1 == '{' && c == '$' && !p->in_decl) {
                    int t = q + 1; while (t < p->n && p->s[t] != '}' && p->s[t] != '\n') t++;
                    int inner_ws = 1; for (int k = q + 1; k < t; k++) if (!asc_space((unsigned char) p->s[k])) inner_ws = 0;
                    if (ch(p, t) == '}' && !inner_ws) {
                        int w = 1; for (int k = q + 1; k < t; k++) { unsigned char x = (unsigned char) p->s[k]; if (!(is_word_cp(x) || x == ':')) w = 0; }
                        if (w && !p->in_decl) panic_at(p, pos, "Unsupported use of ${%.*s}; in Raku please use $%.*s", t - q - 1, p->s + q + 1, t - q - 1, p->s + q + 1);
                    }
                    e = r_circumfix(p, q);
                }
                else if (c == '$' && (d1 == '/' || d1 == '_' || d1 == '!' || cp_at(p, q) == 0xA2)) e = q + cp_len(p, q);
                else if (!p->qsigil) e = q;
            }
        }
        if (e >= 0 && (twig == '.') && !p->qsigil) {
            int t = e; int u = r_unsp(p, t); if (u >= 0) t = u; else if (ch(p, t) == '\\') t++;
            if (ch(p, t) == '(') { int a = r_arglist(p, t + 1); a = ws(p, a); if (ch(p, a) == ')') e = a + 1; }
            else if (ch(p, t) == ':' && (is_space_cp(cp_at(p, t + 1)) || ch(p, t + 1) == '{')) e = r_arglist(p, t + 1);
        }
        else if (e >= 0 && twig == '.' && p->qsigil && ch(p, e) == '(') {
            int a = r_arglist(p, e + 1); a = ws(p, a); if (ch(p, a) == ')') e = a + 1;
        }
    }
    p->in_meta = sm;
    if (e >= 0 && !p->leftsigil) p->leftsigil = c;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_signature(RkP *p, int pos, int allow_invocant);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_colonpair(RkP *p, int pos) {
    if (ch(p, pos) != ':') return -1;
    int q = pos + 1;
    int c = cp_at(p, q);
    if (c == '!') {
        int e = r_identifier(p, q + 1);
        if (e < 0) panic_at(p, q, "Malformed False pair; expected identifier");
        int d = ch(p, e);
        if (d == '[' || d == '(' || d == '<' || d == '{') panic_at(p, pos, "Argument not allowed on negated pair with key '%.*s'", e - q - 1, p->s + q + 1);
        return e;
    }
    if (is_digit_cp(c)) {
        int d = r_decint(p, q);
        int e = r_identifier(p, d);
        if (e < 0) return -1;
        int x = ch(p, e);
        if (x == '[' || x == '(' || x == '<' || x == '{') panic_at(p, e, "Extra argument not allowed; pair already has argument of %.*s", d - q, p->s + q);
        return e;
    }
    if (is_alpha_cp(c)) {
        int e = r_identifier(p, q);
        int t = e; int u = r_unsp(p, t); if (u >= 0) t = u;
        int d = cp_at(p, t);
        if (d == '(' || d == '[' || d == '{' || d == '<' || d == 0xAB) {
            if (at_lit(p, t, "<>")) return t + 2;
            int f = r_circumfix(p, t);
            if (f >= 0) return f;
        }
        return e;
    }
    if (c == '(') {
        int s = r_signature(p, q + 1, 1);
        s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        return s + 1;
    }
    if (c == '<' || c == '[' || c == '{' || c == 0xAB) {
        if (at_lit(p, q, "<>")) return q + 2;
        if (c == '{' && p->in_reduce) return -1;
        return r_circumfix(p, q);
    }
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        int t = q + 1;
        if (ch(p, t) == '<') { int e = r_desigilname(p, t + 1); if (e < 0 || ch(p, e) != '>') return -1; return e + 1; }
        if (is_twigil_at(p, t)) t++;
        int e = r_desigilname(p, t);
        if (e < 0) return -1;
        return e;
    }
    return -1;
}
/*====================================================================================================================================================================================================*/
static int qok(RkP *p, int pos, const char *word) {
    if (wordch_at(p, pos)) return -1;
    if (ch(p, pos) == '(') return -1;
    if (ch(p, pos) != ':' && (is_name_n(p, word, (int) strlen(word)))) return -1;
    int h = pos; while (is_space_cp(cp_at(p, h))) h += cp_len(p, h);
    if (ch(p, h) == '#') panic_at(p, h, "# not allowed as delimiter");
    if (ch(p, h) == ',' || ch(p, h) == ';' || ch(p, h) == ')' || h >= p->n || (ch(p, h) == '=' && ch(p, h + 1) == '>')) return -1;
    return ws(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_adverbs(RkP *p, int pos, int *p5) {
    char key[64]; int v;
    for (;;) {
        int e = r_quotepair(p, pos, key, sizeof key, &v);
        if (e < 0) return pos;
        if (!strcmp(key, "P5") || !strcmp(key, "Perl5")) *p5 = 1;
        pos = ws(p, e);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p5_regex_body(RkP *p, int pos, RkLang *L) {
    int q = pos;
    int depth = 0;
    while (q < p->n) {
        if (depth == 0 && quote_stopper(p, q, L) >= 0) return q;
        int c = ch(p, q);
        if (c == '\\') { q += 1 + cp_len(p, q + 1); continue; }
        if (L->start && cp_at(p, q) == L->start) depth++;
        else if (L->start && cp_at(p, q) == L->stop) depth--;
        q += cp_len(p, q);
    }
    panic_at(p, pos, "Regex not terminated.");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int regex_quibble(RkP *p, int pos, int p5) {
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
static int old_rx_mods(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c && strchr("igsmxce", c) && !wordch_at(p, pos + 1)) panic_at(p, pos, "Unsupported use of /%c; in Raku please use :%c", c, c);
    return pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_sibble(RkP *p, int pos, int p5) {
    RkLang L; memset(&L, 0, sizeof L); L.regex = 1;
    peek_delims(p, pos, &L);
    int s = L.start ? quote_starter(p, pos, &L) : pos + cp_len(p, pos);
    int e = p5 ? p5_regex_body(p, s, &L) : r_regex_nibbler(p, s, &L);
    int t = quote_stopper(p, e, &L);
    if (t < 0) panic_at(p, e, "Unable to parse regex; couldn't find final delimiter");
    if (L.start) {
        int q = ws(p, t);
        int c = ch(p, q);
        if (c == '[' || c == '{' || c == '(' || c == '<') panic_at(p, q, "Unsupported use of brackets around replacement; in Raku please use assignment syntax");
        OpInfo o; int f = r_infixish(p, q, &o, 0);
        if (f < 0) panic_at(p, q, "Missing assignment operator");
        if (!(f == q + 1 && ch(p, q) == '=') && o.prec != PR('i')) panic_at(p, q, "Malformed assignment operator");
        int r = ws(p, f);
        int x = r_EXPR(p, r, PRLE('i'));
        if (x < 0) panic_at(p, r, "Assignment operator missing its expression");
        return x;
    }
    RkLang R; lang_qq(&R); R.start = 0; R.stop = L.stop; R.nrep = 1;
    int e2; r_nibble_until(p, t, &R, &e2);
    if (e2 < 0 || quote_stopper(p, e2, &R) < 0) panic_at(p, t, "Malformed replacement part; couldn't find final %.*s", cp_len(p, s - 1), p->s + s - 1);
    return quote_stopper(p, e2, &R);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cc_nibble(RkP *p, int pos, RkLang *L) {
    int q = pos;
    for (;;) {
        if (q >= p->n) return -1;
        if (quote_stopper(p, q, L) >= 0) return q;
        int c = ch(p, q);
        if (is_space_cp(cp_at(p, q))) { q += cp_len(p, q); int h = q; while (is_space_cp(cp_at(p, h))) h += cp_len(p, h); if (ch(p, h) == '#') q = ws(p, h); continue; }
        if (c == '#') panic_at(p, q, "Please backslash # for literal char or put whitespace in front for comment");
        if (c == '\\') { int e = qq_backslash(p, q + 1, L); if (e < 0) e = q + 2; q = e; continue; }
        q += cp_len(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_tribble(RkP *p, int pos) {
    RkLang L; memset(&L, 0, sizeof L); L.cc = 1;
    peek_delims(p, pos, &L);
    int s = L.start ? quote_starter(p, pos, &L) : pos + cp_len(p, pos);
    int e = cc_nibble(p, s, &L);
    if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Couldn't find terminator of transliteration");
    int t = quote_stopper(p, e, &L);
    if (L.start) {
        int q = ws(p, t);
        int s2 = quote_starter(p, q, &L);
        if (s2 < 0) panic_at(p, q, "Couldn't find start of transliteration replacement");
        int e2 = cc_nibble(p, s2, &L);
        if (e2 < 0 || quote_stopper(p, e2, &L) < 0) panic_at(p, q, "Couldn't find terminator of transliteration");
        return quote_stopper(p, e2, &L);
    }
    int e2 = cc_nibble(p, t, &L);
    if (e2 < 0 || quote_stopper(p, e2, &L) < 0) panic_at(p, t, "Malformed replacement part; couldn't find final terminator");
    return quote_stopper(p, e2, &L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int quote_mod_after(RkP *p, int pos, int *to, RkLang *L) {
    static const char *const mods[] = { "ww", "w", "x", "to", "s", "a", "h", "f", "c", "b", 0 };
    for (int i = 0; mods[i]; i++) {
        int l = (int) strlen(mods[i]);
        if (at_lit(p, pos, mods[i]) && !wordch_at(p, pos + l)) {
            apply_quote_adverb(p, pos, L, mods[i], 1, to);
            return pos + l;
        }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int simple_quote(RkP *p, int pos, int open, int stop, int stop2, int qq, int crnr, const char *what) {
    RkLang L; if (qq) lang_qq(&L); else lang_q(&L);
    if (crnr) memset(&L, 0, sizeof L);
    L.start = (open != stop) ? open : 0; L.stop = stop; L.stop2 = stop2; L.nrep = 1;
    int s = pos + cp_len(p, pos);
    int stop_at;
    int e; r_nibble_until(p, s, &L, &e);
    if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse expression in %s; couldn't find final %s (corresponding starter was at line %d)", what, qq ? "'\"'" : "\"'\"",
        line_of(p, pos));
    stop_at = e; (void) stop_at;
    return quote_stopper(p, e, &L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quote(RkP *p, int pos) {
    int c = cp_at(p, pos);
    switch (c) {
    case '\'': return simple_quote(p, pos, '\'', '\'', 0, 0, 0, "single quotes");
    case 0x2018: return simple_quote(p, pos, 0x2018, 0x2019, 0, 0, 0, "curly single quotes");
    case 0x201A: return simple_quote(p, pos, 0x201A, 0x2019, 0x2018, 0, 0, "low curly single quotes");
    case 0x2019: return simple_quote(p, pos, 0x2019, 0x2019, 0x2018, 0, 0, "high curly single quotes");
    case '"': return simple_quote(p, pos, '"', '"', 0, 1, 0, "double quotes");
    case 0x201C: return simple_quote(p, pos, 0x201C, 0x201D, 0, 1, 0, "curly double quotes");
    case 0x201E: return simple_quote(p, pos, 0x201E, 0x201D, 0x201C, 1, 0, "low curly double quotes");
    case 0x201D: return simple_quote(p, pos, 0x201D, 0x201D, 0x201C, 1, 0, "high curly double quotes");
    case 0xFF62: return simple_quote(p, pos, 0xFF62, 0xFF63, 0, 0, 1, "corner quotes");
    default: break;
    }
    if (c == '/') {
        int q = pos + 1;
        int h = q; while (is_space_cp(cp_at(p, h))) h += cp_len(p, h);
        if (ch(p, h) == '/') panic_at(p, pos, "Null regex not allowed");
        RkLang L; memset(&L, 0, sizeof L); L.regex = 1; L.stop = '/'; L.nrep = 1;
        int e = r_regex_nibbler(p, q, &L);
        if (ch(p, e) != '/') panic_at(p, e, "Unable to parse regex; couldn't find final '/'");
        return old_rx_mods(p, e + 1);
    }
    if (!is_alpha_cp(c)) return -1;
    int wend = pos; while (is_word_cp(cp_at(p, wend))) wend += cp_len(p, wend);
    if ((ch(p, wend) == '-' || ch(p, wend) == '\'') && is_alpha_cp(cp_at(p, wend + 1))) return -1;
    int wl = wend - pos; const char *w = p->s + pos;
    char word[16]; if (wl >= (int) sizeof word) return -1; memcpy(word, w, (size_t) wl); word[wl] = 0;
    if (!strcmp(word, "q") || !strcmp(word, "qq") || !strcmp(word, "Q") || (wl > 1 && (w[0] == 'q' || w[0] == 'Q') && (w[1] != 'q' || wl > 2))) {
        RkLang L; int to = 0; int base;
        if (w[0] == 'Q') { memset(&L, 0, sizeof L); base = 1; }
        else if (wl >= 2 && w[1] == 'q') { lang_qq(&L); base = 2; }
        else { lang_q(&L); base = 1; }
        int q = pos + base;
        if (q < wend) { int m = quote_mod_after(p, q, &to, &L); if (m != wend) return -1; q = m; }
        char qw[16]; memcpy(qw, w, (size_t) (q - pos)); qw[q - pos] = 0;
        int k = qok(p, q, qw);
        if (k < 0) return -1;
        return r_quibble(p, k, &L, to, "quote");
    }
    if (!strcmp(word, "rx") || !strcmp(word, "m") || !strcmp(word, "ms")) {
        int k = qok(p, wend, word);
        if (k < 0) return -1;
        int p5 = 0;
        k = rx_adverbs(p, k, &p5);
        int e = regex_quibble(p, k, p5);
        if (!old_rx_mods(p, e)) return e;
        return e;
    }
    if (!strcmp(word, "s") || !strcmp(word, "S") || !strcmp(word, "ss") || !strcmp(word, "Ss")) {
        int k = qok(p, wend, word);
        if (k < 0) return -1;
        int p5 = 0;
        k = rx_adverbs(p, k, &p5);
        return r_sibble(p, k, p5);
    }
    if (!strcmp(word, "tr") || !strcmp(word, "TR")) {
        int k = qok(p, wend, word);
        if (k < 0) return -1;
        int p5 = 0;
        k = rx_adverbs(p, k, &p5);
        return r_tribble(p, k);
    }
    if (!strcmp(word, "y") && ch(p, hs(p, wend)) && !is_word_cp(cp_at(p, hs(p, wend))) && qok(p, wend, word) >= 0) panic_at(p, pos, "Unsupported use of y///; in Raku please use tr///");
    if (!strcmp(word, "qr") && qok(p, wend, word) >= 0) panic_at(p, pos, "Unsupported use of qr for regex quoting; in Raku please use rx//");
    return -1;
}
/*====================================================================================================================================================================================================*/
static int r_scoped(RkP *p, int pos, int scope);
static int r_declarator(RkP *p, int pos);
static int r_routine_def(RkP *p, int pos, int is_method);
static int r_regex_def(RkP *p, int pos);
static int r_package_def(RkP *p, int pos, const char *kind);
static int r_typename(RkP *p, int pos);
static int r_trait(RkP *p, int pos);
static int r_statement_control(RkP *p, int pos);
static int r_type_declarator(RkP *p, int pos);
static int r_package_declarator(RkP *p, int pos);
static int r_multi_declarator(RkP *p, int pos);
static int r_routine_declarator(RkP *p, int pos);
static int r_regex_declarator(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_blorst(RkP *p, int pos) {
    if (ch(p, pos) == '{') return r_block(p, pos);
    if (ch(p, pos) == ';' || pos >= p->n) panic_at(p, pos, "Missing block or statement");
    int e = r_statement(p, pos);
    if (e < 0) panic_at(p, pos, "Missing block or statement");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_prefix(RkP *p, int pos) {
    static const char *const pre[] = { "BEGIN", "TEMP", "CHECK", "INIT", "ENTER", "FIRST", "END", "LEAVE", "KEEP", "UNDO", "NEXT", "LAST", "PRE", "POST", "CLOSE",
                                        "eager", "sink", "try", "quietly", "gather", "once", "start", "supply", "react", "do", 0 };
    for (int i = 0; pre[i]; i++) {
        int e = kok(p, pos, pre[i]);
        if (e >= 0) return r_blorst(p, e);
    }
    static const char *const hyp[] = { "race", "hyper", "lazy", 0 };
    for (int i = 0; hyp[i]; i++) {
        int e = kok(p, pos, hyp[i]);
        if (e >= 0) {
            int f = kok(p, e, "for");
            if (f >= 0) return r_statement_control(p, e);
            return r_blorst(p, e);
        }
    }
    int e = kok(p, pos, "DOC");
    if (e >= 0) {
        int f = -1;
        if ((f = kw_end(p, e, "BEGIN")) < 0 && (f = kw_end(p, e, "CHECK")) < 0 && (f = kw_end(p, e, "INIT")) < 0) panic_at(p, e, "Malformed DOC phaser");
        return r_blorst(p, ws(p, f));
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_lambda_term(RkP *p, int pos) {
    if (!at_lambda(p, pos)) return -1;
    return r_pblock(p, pos, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_reduce(RkP *p, int pos) {
    if (ch(p, pos) != '[') return -1;
    int q = pos + 1;
    if (is_space_cp(cp_at(p, q)) || q >= p->n) return -1;
    int t = q; while (ch(p, t) == '[') t++;
    if (strchr("-+?~^", ch(p, t)) && ch(p, t) && (is_word_cp(cp_at(p, t + 1)) || ch(p, t + 1) == '$' || ch(p, t + 1) == '@')) return -1;
    int r = q; while (r < p->n && !is_space_cp(cp_at(p, r)) && p->s[r] != ']') r++;
    if (ch(p, r) != ']') { int x = q; int depth = 0; while (x < p->n && !is_space_cp(cp_at(p, x))) { if (p->s[x] == '[') depth++; if (p->s[x] == ']') { if (!depth) break; depth--; } x++; } if (ch(p,
        x) != ']') return -1; }
    int si = p->in_reduce; p->in_reduce = 1;
    int tri = 0;
    if (ch(p, q) == '\\') { tri = 1; q++; }
    OpInfo o;
    jmp_buf save; memcpy(save, p->jb, sizeof save);
    int e = -1;
    if (!setjmp(p->jb)) e = r_infixish(p, q, &o, tri ? 't' : 'r');
    else e = -1;
    memcpy(p->jb, save, sizeof save);
    if (e < 0 || ch(p, e) != ']') { p->in_reduce = si; return -1; }
    if (o.flags & OF_FIDDLY) panic_at(p, pos, "Cannot reduce with %.*s because %s operators are too fiddly", e - q, p->s + q, o.prec == PR('i') ? "item assignment" : "conditional");
    if ((o.flags & OF_DIFFY) && !(o.flags & OF_CHAIN)) panic_at(p, pos, "Cannot reduce with %.*s because %s operators are diffy and not chaining", e - q, p->s + q, "structural infix");
    p->in_reduce = si;
    return r_args(p, e + 1, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_capterm(RkP *p, int pos) {
    if (ch(p, pos) != '\\') return -1;
    int q = pos + 1;
    if (ch(p, q) == '(') { int e = r_semiarglist(p, q + 1); if (ch(p, e) != ')') panic_at(p, e, "Unable to parse capture; couldn't find final ')'"); return e + 1; }
    if (q < p->n && !is_space_cp(cp_at(p, q))) { int e = r_termish(p, q); if (e >= 0) return e; }
    panic_at(p, pos, "You can't backslash that");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_fatarrow(RkP *p, int pos) {
    int e = r_identifier(p, pos);
    if (e < 0) return -1;
    int h = hs(p, e);
    if (!at_lit(p, h, "=>")) return -1;
    int q = ws(p, h + 2);
    int v = r_EXPR(p, q, PRLE('i'));
    if (v < 0) panic_at(p, q, "Missing value after =>");
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int deftrap_kind(const char *s, int n) {
    static const char *const t1[] = { "say", "print", "abs", "chomp", "chop", "chr", "cos", "defined", "exp", "lc", "log", "mkdir", "ord", "reverse", "rmdir", "sin", "split", "sqrt", "uc", "unlink",
        "fc", 0 };
    static const char *const t2[] = { "WHAT", "WHICH", "WHERE", "HOW", "WHENCE", "WHO", "VAR", "any", "all", "none", "one", "set", "bag", "tclc", "wordcase", "put", 0 };
    for (int i = 0; t1[i]; i++) if ((int) strlen(t1[i]) == n && !memcmp(t1[i], s, (size_t) n)) return 1;
    for (int i = 0; t2[i]; i++) if ((int) strlen(t2[i]) == n && !memcmp(t2[i], s, (size_t) n)) return 2;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int name_part_len(RkP *p, int from, int to) {
    int e = from;
    while (e < to) { if (p->s[e] == ':' && !(e + 1 < to && p->s[e + 1] == ':') && !(e > from && p->s[e - 1] == ':')) break; e++; }
    return e - from;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_term_name(RkP *p, int pos) {
    int e = r_longname(p, pos);
    if (e < 0) return -1;
    int nl = name_part_len(p, pos, e);
    int starts_colons = at_lit(p, pos, "::");
    int is_n = starts_colons || is_name_n(p, p->s + pos, nl);
    if (is_n) {
        int is_t = (is_type_n(p, p->s + pos, nl) || starts_colons) && !(nl >= 2 && p->s[pos + nl - 1] == ':' && p->s[pos + nl - 2] == ':');
        int q = e; int u;
        u = r_unsp(p, q); if (u >= 0) q = u;
        if (is_t && ch(p, q) == '[') { int a = r_arglist(p, q + 1); a = ws(p, a); if (ch(p, a) != ']') panic_at(p, a, "Unable to parse type parameter; couldn't find final ']'"); e = a + 1; q = e; }
        u = r_unsp(p, q); if (u >= 0) q = u;
        if (is_t && ch(p, q) == '{' && !had_ws_before(p, q) && q == e) panic_at(p, q, "Autovivifying object closures not yet implemented. Sorry.");
        if (is_t && ch(p, e) == '(') {
            int t = ws(p, e + 1);
            int tn = -1;
            jmp_buf save; memcpy(save, p->jb, sizeof save);
            if (!setjmp(p->jb)) tn = r_typename(p, t); else tn = -1;
            memcpy(p->jb, save, sizeof save);
            int t2 = ws(p, tn >= 0 ? tn : t);
            if (ch(p, t2) == ')') e = t2 + 1;
        }
        return e;
    }
    int q = e;
    if (ch(p, q) == '\\' && ch(p, q + 1) == '(') q++;
    int a = r_args(p, q, 1);
    int na = a;
    if (a >= 0 && !p->last_inv && nl == e - pos && !memchr(p->s + pos, ':', (size_t) nl)) add_mystery(p, pos, nl);
    if (a == q) {
        int k = deftrap_kind(p->s + pos, e - pos);
        if (k && a == e) {
            int c = ch(p, e);
            if (c == '<' || c == '[' || c == '{') panic_at(p, e, "Use of non-subscript brackets after \"%.*s\" where postfix is expected; please use whitespace before any arguments", e - pos,
                p->s + pos);
            if (c == '$' || c == '@' || c == '%' || c == '&' || c == '+' || c == '-' || c == '/' || c == '*')
                if (!(c == '-' && ch(p, e + 1) == '>') && !(c == '*' && ch(p, e + 1) == '*'))
                    panic_at(p, e, "A list operator such as \"%.*s\" must have whitespace before its arguments (or use parens)", e - pos, p->s + pos);
        }
    }
    return na;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_term_identifier(RkP *p, int pos) {
    int e = r_identifier(p, pos);
    if (e < 0) return -1;
    if (is_type_n(p, p->s + pos, e - pos)) return -1;
    int q = e;
    int u = r_unsp(p, q);
    if (ch(p, q) == '(') ;
    else if (u >= 0 && ch(p, u) == '(') q = u;
    else if (ch(p, q) == '\\' && ch(p, q + 1) == '(') q++;
    else return -1;
    int a = r_args(p, q, 1);
    if (a >= 0 && !p->last_inv) add_mystery(p, pos, e - pos);
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_keyword_term(RkP *p, int pos) {
    int e;
    if ((e = r_statement_prefix(p, pos)) >= 0) return e;
    static const char *const scopes[] = { "my", "our", "has", "HAS", "augment", "anon", "state", "supersede", "unit", 0 };
    for (int i = 0; scopes[i]; i++) {
        int k = lit(p, pos, scopes[i]);
        if (k >= 0 && !wordch_at(p, k) && end_keyword_ok(p, k)) { int r = r_scoped(p, k, i); if (r >= 0) return r; }
        else if (k >= 0 && !wordch_at(p, k) && (ch(p, k) == '(' || ch(p, k) == '\\')) { int r = r_scoped(p, k, i); if (r >= 0) return r; }
    }
    if ((e = r_multi_declarator(p, pos)) >= 0) return e;
    if ((e = r_routine_declarator(p, pos)) >= 0) return e;
    if ((e = r_regex_declarator(p, pos)) >= 0) return e;
    if ((e = r_package_declarator(p, pos)) >= 0) return e;
    if ((e = r_type_declarator(p, pos)) >= 0) return e;
    if ((e = kw_end(p, pos, "self")) >= 0) return e;
    if ((e = kw_end(p, pos, "now")) >= 0 && !is_name_n(p, "now", 3)) return e;
    if ((e = kw_end(p, pos, "time")) >= 0 && !is_name_n(p, "time", 4)) return e;
    if ((e = kw(p, pos, "rand")) >= 0 && ch(p, e) != '-' && ch(p, e) != '\'') {
        int h = e; if (ch(p, h) == '(') h++; h = hs(p, h);
        if (is_digit_cp(cp_at(p, h)) || ch(p, h) == '$') panic_at(p, pos, "Unsupported use of rand(N); in Raku please use N.rand for Num or (^N).pick for Int result");
        if (at_lit(p, e, "()")) panic_at(p, pos, "Unsupported use of rand(); in Raku please use rand");
        if (end_keyword_ok(p, e)) return e;
    }
    if ((e = kw(p, pos, "undef")) >= 0 && ch(p, e) != '-' && ch(p, e) != '\'') {
        int h = hs(p, e);
        if (at_lit(p, h, "$/")) panic_at(p, pos, "Unsupported use of $/ variable as input record separator; in Raku please use the filehandle's .slurp method");
        if (ch(p, e) == '(' || ch(p, h) == '$' || ch(p, h) == '@' || ch(p, h) == '%' || ch(p, h) == '&') panic_at(p, pos,
            "Unsupported use of undef as a verb; in Raku please use undefine() or assignment of Nil");
        panic_at(p, pos, "Unsupported use of undef as a value; in Raku please use something more specific");
    }
    if ((e = lit(p, pos, "new")) >= 0 && is_hspace_cp(cp_at(p, e))) {
        int h = hs(p, e);
        int l = r_longname(p, h);
        if (l >= 0 && ch(p, hs(p, l)) != ':' && is_type_n(p, p->s + h, name_part_len(p, h, l))) {
            int t = hs(p, l);
            if (!(ch(p, t) == ',' || ch(p, t) == ';' || ch(p, t) == ')' || t >= p->n || at_lit(p, t, "=>")))
                panic_at(p, pos, "Unsupported use of C++ constructor syntax; in Raku please use method call syntax");
        }
    }
    if ((e = lit(p, pos, "nqp::")) >= 0) {
        int w = e; while (is_word_cp(cp_at(p, w))) w++;
        if (at_lit(p, e, "const::")) { int x = e + 7; while (is_word_cp(cp_at(p, x))) x++; return x; }
        if (ch(p, w) == '(') return r_args(p, w, 0);
        return w;
    }
    if ((e = kw(p, pos, "__END__")) >= 0) panic_at(p, pos, "Unsupported use of __END__ as end of code; in Raku please use the =finish pod marker and $=finish to read");
    if ((e = kw(p, pos, "__DATA__")) >= 0) panic_at(p, pos, "Unsupported use of __DATA__ as start of data; in Raku please use the =finish pod marker and $=finish to read");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_user_term(RkP *p, int pos) {
    int prec = 0;
    int l = user_op_longest(p, pos, 't', &prec);
    if (l > 0) return pos + l;
    l = user_op_longest(p, pos, 'c', &prec);
    if (l > 0) {
        int q = pos + l;
        for (int i = p->nuops - 1; i >= 0; i--) {
            RkUserOp *u = &p->uops[i];
            if (u->cat != 'c' || u->len != l || memcmp(p->s + pos, u->sym, (size_t) l)) continue;
            RkUserOp *cl = (i + 1 < p->nuops && p->uops[i + 1].cat == 'C') ? &p->uops[i + 1] : NULL;
            if (!cl) break;
            const char *su = p->ustop; int sl = p->ustop_len;
            p->ustop = cl->sym; p->ustop_len = cl->len;
            int e = r_semilist(p, q);
            p->ustop = su; p->ustop_len = sl;
            e = ws(p, e);
            if (e + cl->len <= p->n && !memcmp(p->s + e, cl->sym, (size_t) cl->len)) return e + cl->len;
            panic_at(p, e, "Unable to parse expression in circumfix:<%s %s>; couldn't find final %s", u->sym, cl->sym, cl->sym);
        }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_term(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c < 0) return -1;
    int e;
    if (p->nuops && (e = r_user_term(p, pos)) >= 0) return e;
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        if (c == '&' && ch(p, pos + 1) == '&') return -1;
        return r_variable(p, pos);
    }
    if (is_digit_cp(c) || (c == '.' && is_digit_cp(cp_at(p, pos + 1)))) return r_numish(p, pos);
    if (c == ':') {
        if (is_digit_cp(cp_at(p, pos + 1))) { int d = r_decint(p, pos + 1); int x = ch(p, d); if (x == '<' || x == '[' || x == '(' || x == '\\') return r_rad_number(p, pos); }
        if (ch(p, pos + 1) == ':') { if (at_lit(p, pos, "::?") && is_alpha_cp(cp_at(p, pos + 3))) return r_identifier(p, pos + 3); return r_term_name(p, pos); }
        return r_colonpair(p, pos);
    }
    if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201A || c == 0x2019 || c == 0x201C || c == 0x201E || c == 0x201D || c == 0xFF62 || c == '/') return r_quote(p, pos);
    if (c == '[') { e = r_reduce(p, pos); if (e >= 0) return e; return r_circumfix(p, pos); }
    if (c == '(' || c == '{' || c == 0xAB) return r_circumfix(p, pos);
    if (c == '<') {
        if (at_lambda(p, pos)) return r_lambda_term(p, pos);
        if (at_lit(p, pos, "<==")) return -1;
        return r_circumfix(p, pos);
    }
    if (c == '\\') return r_capterm(p, pos);
    if (c == '-' && at_lit(p, pos, "->")) return r_lambda_term(p, pos);
    if (c == '*') { if (at_lit(p, pos, "**")) return pos + 2; return pos + 1; }
    if (at_lit(p, pos, "...") || c == 0x2026) {
        int q = c == 0x2026 ? pos + 3 : pos + 3;
        if (at_lit(p, pos, "...^") || at_lit(p, pos, "\xe2\x80\xa6^")) return -1;
        return r_args(p, q, 0);
    }
    if (at_lit(p, pos, "???") || at_lit(p, pos, "!!!")) return r_args(p, pos + 3, 0);
    if (at_lit(p, pos, "!!") && is_space_cp(cp_at(p, pos + 2))) return pos + 2;
    if (c == '.') {
        if (at_lit(p, pos, "..")) return -1;
        return r_dotty(p, pos);
    }
    if (c == 0x2205) { int q = pos + 3; if (!(ch(p, q) == '(' || ch(p, q) == '\\' || ch(p, q) == '\'' || ch(p, q) == '-') && !at_lit(p, hs(p, q), "=>")) return q; }
    if (c == 0x221E) return pos + 3;
    if (c >= 0x80 && is_numeric_other_cp(c) && !is_alpha_cp(c)) return r_numish(p, pos);
    if (!is_alpha_cp(c)) return -1;
    if ((e = r_fatarrow(p, pos)) >= 0) return e;
    if ((e = r_quote(p, pos)) >= 0) return e;
    if ((e = r_keyword_term(p, pos)) >= 0) return e;
    if (c == 'v' && (e = r_version(p, pos)) >= 0) return e;
    if ((e = kw(p, pos, "NaN")) >= 0) return e;
    if ((e = kw(p, pos, "Inf")) >= 0) return e;
    if ((e = r_term_identifier(p, pos)) >= 0) return e;
    return r_term_name(p, pos);
}
/*====================================================================================================================================================================================================*/
static int r_typename(RkP *p, int pos) {
    int e;
    if (at_lit(p, pos, "::?") && is_alpha_cp(cp_at(p, pos + 3))) {
        e = r_identifier(p, pos + 3);
        while (ch(p, e) == ':' && ch(p, e + 1) != ':') { int c = r_colonpair(p, e); if (c < 0) break; e = c; }
    }
    else {
        e = r_longname(p, pos);
        if (e < 0) return -1;
        int nl = name_part_len(p, pos, e);
        if (!at_lit(p, pos, "::") && !is_name_n(p, p->s + pos, nl)) return -1;
        if (at_lit(p, pos, "::") && is_alpha_cp(cp_at(p, pos + 2))) add_name_n(p, p->s + pos + 2, nl - 2);
    }
    int q = e; int u = r_unsp(p, q); if (u >= 0) q = u;
    if (ch(p, q) == '[') { int a = r_arglist(p, q + 1); a = ws(p, a); if (ch(p, a) != ']') panic_at(p, a, "Unable to parse type parameters; couldn't find final ']'"); e = a + 1; }
    q = e; u = r_unsp(p, q); if (u >= 0) q = u;
    if (ch(p, q) == '(') {
        int t = ws(p, q + 1);
        int tn = r_typename(p, t);
        int t2 = ws(p, tn >= 0 ? tn : t);
        if (ch(p, t2) == ')') e = t2 + 1;
    }
    q = ws(p, e);
    if ((u = kw(p, q, "of")) >= 0) { int t = r_typename(p, ws(p, u)); if (t >= 0) e = t; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_trait(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 0;
    int e = -1; int q;
    if ((q = kw(p, pos, "is")) >= 0) {
        int t = ws(p, q);
        e = r_longname(p, t);
        if (e < 0) panic_at(p, t, "Invalid name");
        int c = ch(p, e);
        if (c == '(' || c == '[' || c == '<' || c == '{' || cp_at(p, e) == 0xAB) { int f = r_circumfix(p, e); if (f >= 0) e = f; }
    }
    else if ((q = kw(p, pos, "hides")) >= 0 || (q = kw(p, pos, "does")) >= 0 || (q = kw(p, pos, "of")) >= 0) {
        int t = ws(p, q);
        e = r_typename(p, t);
        if (e < 0) { int l = r_longname(p, t); if (l >= 0) panic_at(p, t, "Invalid typename '%.*s'", l - t, p->s + t); panic_at(p, t, "Malformed trait"); }
    }
    else if ((q = kw(p, pos, "returns")) >= 0) {
        int t = ws(p, q);
        e = r_typename(p, t);
        if (e < 0) { int l = r_longname(p, t); if (l >= 0) panic_at(p, t, "Invalid typename '%.*s'", l - t, p->s + t); panic_at(p, t, "Malformed trait"); }
    }
    else if ((q = kw(p, pos, "return")) >= 0 && 0) e = q;
    else if ((q = kw(p, pos, "will")) >= 0) {
        int t = ws(p, q);
        int i = r_identifier(p, t);
        if (i < 0) panic_at(p, t, "Invalid name");
        e = r_pblock(p, ws(p, i), 1);
    }
    else if ((q = kw(p, pos, "handles")) >= 0) {
        int t = ws(p, q);
        e = r_term(p, t);
        if (e < 0) panic_at(p, t, "Invalid term");
    }
    p->in_decl = sd;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_traits(RkP *p, int pos) {
    int q = pos;
    for (;;) { int t = ws(p, q); int e = r_trait(p, t); if (e < 0) return q; q = e; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_post_constraint(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 0;
    int e = -1; int q;
    if (ch(p, pos) == '[' || ch(p, pos) == '(') {
        int close = ch(p, pos) == '[' ? ']' : ')';
        int s = r_signature(p, pos + 1, 0); s = ws(p, s);
        if (ch(p, s) != close) panic_at(p, s, "Unable to parse signature; couldn't find final '%c'", close);
        e = s + 1;
    }
    else if ((q = kw(p, pos, "where")) >= 0) { int t = ws(p, q); e = r_EXPR(p, t, PR('i')); if (e < 0) panic_at(p, t, "Missing expression after 'where'"); }
    p->in_decl = sd;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_sublongname(RkP *p, int pos) {
    int e = r_desigilname(p, pos);
    if (e < 0) return -1;
    if (at_lit(p, e, ":(")) { int s = r_signature(p, e + 2, 1); s = ws(p, s); if (ch(p, s) == ')') e = s + 1; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_param_var(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '[' || c == '(') {
        int close = c == '[' ? ']' : ')';
        int s = r_signature(p, pos + 1, 0); s = ws(p, s);
        if (ch(p, s) != close) panic_at(p, s, "Unable to parse signature; couldn't find final '%c'", close);
        return s + 1;
    }
    if (!(c == '$' || c == '@' || c == '%' || c == '&')) return -1;
    int q = pos + 1;
    if (strchr(".!^:*?=~", ch(p, q)) && ch(p, q) && is_word_cp(cp_at(p, q + 1))) q++;
    int e = q;
    if (c == '&') {
        if (is_alpha_cp(cp_at(p, q))) { int s = r_sublongname(p, q); if (s >= 0) { e = s; add_routine_name(p, p->s + q, name_part_len(p, q, s)); } }
        else if (at_lit(p, q, ":(")) { int s = r_signature(p, q + 2, 1); s = ws(p, s); if (ch(p, s) == ')') e = s + 1; }
    }
    else if (is_alpha_cp(cp_at(p, q))) {
        e = r_identifier(p, q);
        if (e >= 0 && q == pos + 1) add_name_n(p, p->s + pos, e - pos);
        if (e >= 0 && (p->scope == 3 || p->scope == 4) && (q == pos + 1 || ch(p, pos + 1) == '.' || ch(p, pos + 1) == '!')) record_attribute(p, c, p->s + q, e - q);
    }
    else if (is_digit_cp(cp_at(p, q))) panic_at(p, pos, "Cannot declare a numeric parameter");
    else if (ch(p, q) == '/' || ch(p, q) == '!') e = q + 1;
    if (at_lit(p, e, ":(")) return e + 1;
    if (ch(p, e) == '(') panic_at(p, e, "Shape declaration with () is reserved; please use whitespace if you meant a subsignature for unpacking");
    if (ch(p, e) == '[') { int f = r_postcircumfix(p, e); if (f >= 0) e = f; }
    else if (ch(p, e) == '{' || ch(p, e) == '<' || cp_at(p, e) == 0xAB) panic_at(p, e, "Shape declaration is not yet implemented; please use whitespace if you meant something else");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_named_param(RkP *p, int pos) {
    if (ch(p, pos) != ':') return -1;
    int q = pos + 1;
    int i = r_identifier(p, q);
    if (i >= 0 && ch(p, i) == '(') {
        int t = ws(p, i + 1);
        int e = r_named_param(p, t);
        if (e < 0) e = r_param_var(p, t);
        if (e < 0) panic_at(p, t, "Unable to parse named parameter");
        e = ws(p, e);
        if (ch(p, e) != ')') panic_at(p, e, "Unable to parse named parameter; couldn't find right parenthesis");
        return e + 1;
    }
    return r_param_var(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_type_constraint(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 0;
    int e = -1; int q;
    int c = cp_at(p, pos);
    if ((q = kw(p, pos, "where")) >= 0) { int t = ws(p, q); e = r_EXPR(p, t, PR('i')); }
    else if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201C || c == 0xFF62) e = r_quote(p, pos);
    else if (is_digit_cp(c) || (c == '.' && is_digit_cp(cp_at(p, pos + 1)))) e = r_numish(p, pos);
    else if ((c == '-' || c == '+') && (is_digit_cp(cp_at(p, pos + 1)) || at_lit(p, pos + 1, "Inf"))) e = r_numish(p, pos + 1);
    else if (at_lit(p, pos, "\xe2\x88\x92") && is_digit_cp(cp_at(p, pos + 3))) e = r_numish(p, pos + 3);
    else if (c == '<' || c == 0xAB) e = r_circumfix(p, pos);
    else if (is_alpha_cp(c) || c == ':') {
        if (c == ':' && ch(p, pos + 1) != ':') e = -1;
        else {
            e = r_typename(p, pos);
            if (e < 0 && c == 'v') e = r_version(p, pos);
            if (e < 0) { int n = r_numish(p, pos); if (n >= 0 && (at_lit(p, pos, "Inf") || at_lit(p, pos, "NaN"))) e = n; }
        }
    }
    p->in_decl = sd;
    if (e < 0) return -1;
    return ws(p, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_default_value(RkP *p, int pos) {
    if (ch(p, pos) != '=' || ch(p, pos + 1) == '=' || ch(p, pos + 1) == '>') return -1;
    int sd = p->in_decl; p->in_decl = 0;
    int q = ws(p, pos + 1);
    int e = r_EXPR(p, q, PR('i'));
    p->in_decl = sd;
    if (e < 0) panic_at(p, q, "Missing default value");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_parameter(RkP *p, int pos) {
    int q = pos; int e = -1; int ntc = 0;
    for (;;) { int t = r_type_constraint(p, q); if (t < 0) break; q = t; ntc++; }
    int c = ch(p, q);
    if (at_lit(p, q, "**") || c == '*' || c == '+') {
        int l = at_lit(p, q, "**") ? 2 : 1;
        int v = r_param_var(p, q + l);
        if (v >= 0) e = v;
        else if (c == '+' && is_alpha_cp(cp_at(p, q + 1))) { e = r_identifier(p, q + 1); if (e >= 0) add_value_name(p, p->s + q + 1, e - q - 1); }
        else if (c == '+') e = q + 1;
    }
    else if (c == '\\' || c == '|') {
        int d = ch(p, q + 1);
        if (d == '$' || d == '@' || d == '%' || d == '&') {
            if (r_param_var(p, q + 1) >= 0) panic_at(p, q, "Obsolete use of | or \\ with sigil on param %.*s", 2, p->s + q + 1);
        }
        int t = r_identifier(p, q + 1);
        e = t >= 0 ? t : q + 1;
        if (t >= 0) add_value_name(p, p->s + q + 1, t - q - 1);
    }
    else {
        int v = r_param_var(p, q);
        if (v < 0) v = r_named_param(p, q);
        if (v >= 0) { e = v; if (ch(p, e) == '?' || ch(p, e) == '!') e++; }
        else if (ntc) e = q;
        else {
            int l = r_longname(p, q);
            if (l >= 0 && is_alpha_cp(cp_at(p, q))) panic_at(p, q, "Invalid typename '%.*s' in parameter declaration.", name_part_len(p, q, l), p->s + q);
            return -1;
        }
    }
    if (e < 0) return -1;
    q = ws(p, e);
    q = r_traits(p, q); q = ws(p, q);
    for (;;) { int t = r_post_constraint(p, q); if (t < 0) break; q = ws(p, t); }
    int d = r_default_value(p, q);
    if (d >= 0) {
        q = ws(p, d);
        int t = r_trait(p, q);
        if (t >= 0) panic_at(p, q, "Cannot put trait on parameter after its default value");
        if (kw(p, q, "where") >= 0) panic_at(p, q, "Cannot put post constraint on parameter after its default value");
    }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sig_end_at(RkP *p, int pos) {
    int c = ch(p, pos);
    return at_lit(p, pos, "-->") || c == ')' || c == ']' || c == '{' || (c == ':' && is_space_cp(cp_at(p, pos + 1))) || at_lit(p, pos, ";;");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_signature(RkP *p, int pos, int allow_invocant) {
    int sd = p->in_decl; p->in_decl = 1;
    int q = ws(p, pos);
    for (;;) {
        if (!sig_end_at(p, q)) {
            int e = r_parameter(p, q);
            if (e < 0) break;
            q = e;
        }
        int t = ws(p, q);
        if (ch(p, t) == ',' || (ch(p, t) == ':' && !is_space_cp(cp_at(p, t + 1)) && 0)) { q = ws(p, t + 1); continue; }
        if (ch(p, t) == ':' && allow_invocant && !at_lit(p, t, "::")) { q = ws(p, t + 1); continue; }
        if (at_lit(p, t, ";;")) { q = ws(p, t + 2); continue; }
        if (ch(p, t) == ';') { q = ws(p, t + 1); continue; }
        q = t;
        break;
    }
    q = ws(p, q);
    if (!sig_end_at(p, q)) panic_at(p, q, "Malformed parameter");
    p->in_decl = sd;
    if (at_lit(p, q, "-->")) {
        int t = ws(p, q + 3);
        int e = r_typename(p, t);
        if (e < 0) { int c = cp_at(p, t); if (c == '\'' || c == '"') e = r_quote(p, t); else if (is_digit_cp(c) || c == '-') e = r_numish(p, c == '-' ? t + 1 : t); }
        if (e < 0) { int c = ch(p, t); if (c == '$' || c == '@' || c == '%' || c == '&') panic_at(p, t, "Malformed return value"); }
        if (e < 0) { e = r_term(p, t); if (e < 0) panic_at(p, t, "Malformed return value"); }
        q = ws(p, e);
        int c = ch(p, q);
        if (!(c == '{' || c == ')')) panic_at(p, q, "Malformed return value (return constraints only allowed at the end of the signature)");
    }
    return q;
}
/*====================================================================================================================================================================================================*/
static int r_pblock(RkP *p, int pos, int implicit) {
    if (at_lambda(p, pos)) {
        int q = at_lit(p, pos, "<->") ? pos + 3 : pos + 2;
        scope_enter(p);
        int sg = p->goal; p->goal = GOAL_BLOCK;
        int s = r_signature(p, q, 0);
        p->goal = sg;
        s = ws(p, s);
        if (ch(p, s) != '{') panic_at(p, s, "Missing block");
        int e = r_blockoid(p, s);
        scope_leave(p);
        return e;
    }
    if (ch(p, pos) == '{') { scope_enter(p); int e = r_blockoid(p, pos); scope_leave(p); return e; }
    (void) implicit;
    if (pos > 0 && p->s[pos - 1] == '}') panic_at(p, pos, "Missing block (whitespace needed before curlies taken as a hash subscript?)");
    panic_at(p, pos, "Missing block");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_block(RkP *p, int pos) {
    if (ch(p, pos) != '{') {
        int m = marked_ws_from(p, pos);
        int at = m >= 0 ? m : pos;
        if (at > 0 && p->s[at - 1] == '}') panic_at(p, at, "Missing block (whitespace needed before curlies taken as a hash subscript?)");
        panic_at(p, pos, "Missing block");
    }
    scope_enter(p);
    int e = r_blockoid(p, pos);
    scope_leave(p);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_blockoid(RkP *p, int pos) {
    if (at_lit(p, pos, "{YOU_ARE_HERE}")) panic_at(p, pos, "Reserved use of {YOU_ARE_HERE} outside of a setting");
    if (ch(p, pos) != '{') panic_at(p, pos, "Missing block");
    int noend = p->quote_block; p->quote_block = 0;
    int sg = p->goal; int sq = p->qsigil; int sr = p->in_reduce; int sm = p->in_meta; int sa = p->invocant_ok; int sd = p->in_decl; int sl = p->leftsigil; int ss = p->scope;
    const char *su = p->ustop; int sul = p->ustop_len; int s5 = p->p5isms;
    p->goal = 0; p->qsigil = 0; p->in_reduce = 0; p->in_meta = 0; p->in_decl = 0; p->scope = 0; p->ustop = NULL; p->ustop_len = 0;
    int e = r_statementlist(p, pos + 1);
    p->goal = sg; p->qsigil = sq; p->in_reduce = sr; p->in_meta = sm; p->invocant_ok = sa; p->in_decl = sd; p->leftsigil = sl; p->scope = ss; p->ustop = su; p->ustop_len = sul; p->p5isms = s5;
    if (ch(p, e) != '}') {
        if (e >= p->n) panic_at(p, e, "Missing block (couldn't find final '}' of block starting at line %d)", line_of(p, pos));
        panic_at(p, e, "Confused");
    }
    return noend ? e + 1 : r_ENDSTMT(p, e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_xblock(RkP *p, int pos, int implicit) {
    int sg = p->goal; p->goal = GOAL_BLOCK;
    int e = r_EXPR(p, pos, 0);
    p->goal = sg;
    if (e < 0) panic_at(p, pos, "Missing expression");
    e = ws(p, e);
    return r_pblock(p, e, implicit);
}
/*====================================================================================================================================================================================================*/
static int r_initializer(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '=' && ch(p, pos + 1) != '=' && ch(p, pos + 1) != '>') {
        int q = ws(p, pos + 1);
        int e = r_EXPR(p, q, p->leftsigil == '$' ? PRLE('i') : PR('e'));
        if (e < 0) panic_at(p, q, "Malformed initializer");
        return e;
    }
    if (at_lit(p, pos, ":=")) {
        int q = ws(p, pos + 2);
        int e = r_EXPR(p, q, PR('e'));
        if (e < 0) panic_at(p, q, "Malformed binding");
        return e;
    }
    if (at_lit(p, pos, "::=")) panic_at(p, pos, "\"::=\" not yet implemented. Sorry.");
    if (at_lit(p, pos, ".=")) {
        int q = ws(p, pos + 2);
        int e = r_dottyop(p, q);
        if (e < 0) panic_at(p, q, "Malformed mutator method call");
        return e;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_variable_declarator(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 1;
    int e = r_variable(p, pos);
    p->in_decl = sd;
    if (e < 0) return -1;
    int c0 = ch(p, pos);
    if (c0 == '&') register_user_op(p, pos + 1, e);
    for (;;) {
        int q = e; int u = r_unsp(p, q); if (u >= 0) q = u;
        int c = ch(p, q);
        if (c == '(' && q == e) panic_at(p, q, "The () shape syntax in %s declarations is reserved", c0 == '$' ? "variable" : c0 == '@' ? "array" : c0 == '%' ? "hash" : "routine");
        if (c == '[') { int s = r_semilist(p, q + 1); e = expect_close(p, s, ']', "shape definition", q); continue; }
        if (c == '{' && q == e) { int s = r_semilist(p, q + 1); e = expect_close(p, s, '}', "shape definition", q); continue; }
        if (c == '<' && q == e) panic_at(p, q, "Shaped variable declarations not yet implemented. Sorry.");
        break;
    }
    int q = r_traits(p, e);
    for (;;) { int t = ws(p, q); int f = r_post_constraint(p, t); if (f < 0) break; q = f; }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_declarator(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '\\') {
        int e = r_identifier(p, pos + 1);
        if (e < 0) return -1;
        while (ch(p, e) == ':' && ch(p, e + 1) != ':' && ch(p, e + 1) != '=') { int f = r_colonpair(p, e); if (f < 0) break; e = f; }
        add_value_name(p, p->s + pos + 1, name_part_len(p, pos + 1, e));
        register_user_op(p, pos + 1, e);
        int q = ws(p, e);
        int i = r_initializer(p, q);
        if (i < 0) panic_at(p, q, "A sigilless term definition requires an initializer");
        return i;
    }
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        int e = r_variable_declarator(p, pos);
        if (e < 0) return -1;
        int q = ws(p, e);
        int thunk = p->scope == 3 || p->scope == 4;
        if (thunk) scope_enter(p);
        int i = r_initializer(p, q);
        if (thunk) scope_leave(p);
        return i >= 0 ? i : e;
    }
    if (c == '(') {
        int s = r_signature(p, pos + 1, 0); s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        int e = r_traits(p, s + 1);
        int q = ws(p, e);
        int i = r_initializer(p, q);
        return i >= 0 ? i : e;
    }
    if (at_lit(p, pos, ":(")) {
        int s = r_signature(p, pos + 2, 1); s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        int e = r_traits(p, s + 1);
        int q = ws(p, e);
        int i = r_initializer(p, q);
        if (i < 0) panic_at(p, q, "Cannot declare a signature without an initializer");
        return i;
    }
    int e;
    if ((e = r_routine_declarator(p, pos)) >= 0) return e;
    if ((e = r_regex_declarator(p, pos)) >= 0) return e;
    if ((e = r_type_declarator(p, pos)) >= 0) return e;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_multi_declarator(RkP *p, int pos) {
    static const char *const m[] = { "multi", "proto", "only", 0 };
    for (int i = 0; m[i]; i++) {
        int e = kok(p, pos, m[i]);
        if (e < 0) continue;
        if (ch(p, e) == '(') panic_at(p, pos, "Cannot put %s on anonymous routine", m[i]);
        int sm = p->multiness; p->multiness = i + 1; int sp = p->in_proto; if (i == 1) p->in_proto = 1;
        int d = r_declarator(p, e);
        if (d < 0) d = r_routine_def(p, e, 0);
        p->multiness = sm; p->in_proto = sp;
        if (d < 0) panic_at(p, e, "Malformed %s", m[i]);
        return d;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_routine_declarator(RkP *p, int pos) {
    int e;
    if ((e = kw_end(p, pos, "sub")) >= 0) return r_routine_def(p, ws(p, e), 0);
    if ((e = kw_end(p, pos, "method")) >= 0) return r_routine_def(p, ws(p, e), 1);
    if ((e = kw_end(p, pos, "submethod")) >= 0) return r_routine_def(p, ws(p, e), 2);
    if ((e = kw_end(p, pos, "macro")) >= 0) panic_at(p, pos, "Use of macros is experimental; please 'use experimental :macros'");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_user_op_x(RkP *p, int from, int to, int exported);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_user_op(RkP *p, int from, int to) { register_user_op_x(p, from, to, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_user_op_x(RkP *p, int from, int to, int exported) {
    int nb = p->nuops;
    int n = to - from; const char *s = p->s + from;
    add_routine_name(p, s, n);
    static const char *const cats[] = { "infix", "prefix", "postfix", "circumfix", "postcircumfix", "term", 0 };
    for (int i = 0; cats[i]; i++) {
        int l = (int) strlen(cats[i]);
        if (n > l + 1 && !memcmp(s, cats[i], (size_t) l) && s[l] == ':') {
            int q = from + l + 1;
            if (at_lit(p, q, "sym")) q += 3;
            int c = cp_at(p, q);
            int close = c == '<' ? '>' : c == 0xAB ? 0xBB : c == '[' ? ']' : c == '(' ? ')' : c == '{' ? '}' : 0;
            if (!close) return;
            int a = q + cp_len(p, q);
            int b = a; while (b < to && cp_at(p, b) != close) { if (p->s[b] == '\\' && b + 1 < to) b++; b += cp_len(p, b); }
            if (c == '<' && at_lit(p, q, "<<")) { a = q + 2; b = a; while (b < to && !at_lit(p, b, ">>")) b++; }
            int x = a; int y = b;
            if (c == '[' || c == '(') { while (x < y && (p->s[x] == '\'' || p->s[x] == '"' || asc_space((unsigned char) p->s[x]))) x++; while (y > x && (p->s[y - 1] == '\'' || p->s[y - 1] == '"' ||
                asc_space((unsigned char) p->s[y - 1]))) y--; }
            while (x < y && asc_space((unsigned char) p->s[x])) x++;
            while (y > x && asc_space((unsigned char) p->s[y - 1])) y--;
            if (i == 0) add_user_op(p, 'i', p->s + x, y - x, PR('t'));
            else if (i == 1) add_user_op(p, 'p', p->s + x, y - x, PR('v'));
            else if (i == 2) add_user_op(p, 'P', p->s + x, y - x, PR('x'));
            else if (i == 5) { add_name_n(p, p->s + x, y - x); add_user_op(p, 't', p->s + x, y - x, 0); }
            if (i == 0 || i == 1 || i == 2 || i == 5) { if (exported) for (int k = nb; k < p->nuops; k++) p->uops[k].depth = 1000; return; }
            int sp = x; while (sp < y && !asc_space((unsigned char) p->s[sp])) sp++;
            int op2 = sp; while (op2 < y && asc_space((unsigned char) p->s[op2])) op2++;
            if (i == 3) { add_user_op(p, 'c', p->s + x, sp - x, 0); add_user_op(p, 'C', p->s + op2, y - op2, 0); }
            if (i == 4) { add_user_op(p, 'k', p->s + x, sp - x, 0); add_user_op(p, 'K', p->s + op2, y - op2, 0); }
            if (exported) for (int k = nb; k < p->nuops; k++) p->uops[k].depth = 1000;
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_onlystar(RkP *p, int pos) {
    if (!p->in_proto && p->multiness != 2) return -1;
    if (ch(p, pos) != '{') return -1;
    int q = ws(p, pos + 1);
    if (ch(p, q) != '*') return -1;
    q = ws(p, q + 1);
    if (ch(p, q) != '}') return -1;
    return r_ENDSTMT(p, q + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_routine_def(RkP *p, int pos, int is_method) {
    int q = pos; int name_from = -1, name_to = -1;
    if (is_method) {
        int t = q;
        if (ch(p, t) == '!' || ch(p, t) == '^') t++;
        int n = r_longname(p, t);
        if (n >= 0) { name_from = t; name_to = n; q = ws(p, n); }
        else if ((ch(p, q) == '$' || ch(p, q) == '@' || ch(p, q) == '%' || ch(p, q) == '&') && ch(p, q + 1) == '.') {
            int c = ch(p, q + 2);
            int close = c == '(' ? ')' : c == '[' ? ']' : c == '{' ? '}' : 0;
            if (!close) panic_at(p, q, "Malformed method");
            int s = r_signature(p, q + 3, 1); s = ws(p, s);
            if (ch(p, s) != close) panic_at(p, s, "Unable to parse subscript signature");
            q = ws(p, s + 1);
        }
    }
    else {
        int n = r_deflongname(p, q);
        if (n >= 0) { name_from = q; name_to = n; q = ws(p, n); }
    }
    if (name_from >= 0) register_user_op(p, name_from, name_to);
    int nuops_before_traits = p->nuops;
    int sm = p->has_self;
    int outer_depth = p->depth;
    scope_enter(p);
    if (ch(p, q) == '(') {
        int s = r_signature(p, q + 1, is_method);
        s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        q = ws(p, s + 1);
    }
    int tstart = q;
    q = r_traits(p, q); q = ws(p, q);
    for (int t = tstart; t + 6 <= q; t++) if (!memcmp(p->s + t, "export", 6) && !wordch_at(p, t + 6) && (t == 0 || !asc_word((unsigned char) p->s[t - 1]))) {
        for (int k = 0; k < nuops_before_traits && k < p->nuops; k++) if (p->uops[k].depth == p->depth - 1 && k >= nuops_before_traits - 2) p->uops[k].depth = 1000;
        if (name_from >= 0 && !is_method) {
            int nl = name_part_len(p, name_from, name_to); char *x = (char *) ct_alloc((size_t) nl + 2); x[0] = '&'; memcpy(x + 1, p->s + name_from, (size_t) nl); x[nl + 1] = 0;
            RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = x;
        }
        break;
    }
    int e;
    if (ch(p, q) == ';' && !is_method) {
        if (name_from < 0 || !(name_to - name_from == 4 && !memcmp(p->s + name_from, "MAIN", 4))) panic_at(p, q,
            "A unit-scoped sub definition is not allowed except on a MAIN sub; please use the block form");
        if (outer_depth != 1) panic_at(p, q, "A unit-scoped sub definition is not allowed in a subscope");
        e = r_statementlist(p, q + 1);
    }
    else if ((e = r_onlystar(p, q)) >= 0) ;
    else if (ch(p, q) != '{') {
        if (!is_method && is_alpha_cp(cp_at(p, q)) && name_from >= 0 && is_type_n(p, p->s + name_from, name_to - name_from)) {
            panic_at(p, q, "Did you mean to write \"my %.*s sub %.*s\" or put \"returns %.*s\" before the block?", name_to - name_from, p->s + name_from, 10, p->s + q, name_to - name_from,
                p->s + name_from);
        }
        scope_leave(p);
        p->has_self = sm;
        if (is_method) panic_at(p, q, "Malformed method");
        panic_at(p, q, "Missing block");
        return -1;
    }
    else e = r_blockoid(p, q);
    scope_leave(p);
    p->has_self = sm;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_declarator(RkP *p, int pos) {
    static const char *const k[] = { "rule", "token", "regex", 0 };
    for (int i = 0; k[i]; i++) {
        int e = kok(p, pos, k[i]);
        if (e >= 0) return r_regex_def(p, e);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_def(RkP *p, int pos) {
    int q = pos;
    int n = r_deflongname(p, q);
    if (n >= 0) q = ws(p, n);
    scope_enter(p);
    for (;;) {
        int t = q;
        if (ch(p, t) == ':' && ch(p, t + 1) == '(') t++;
        if (ch(p, t) == '(') {
            int s = r_signature(p, t + 1, 1); s = ws(p, s);
            if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
            q = ws(p, s + 1); continue;
        }
        int tr = r_trait(p, q);
        if (tr >= 0) { q = ws(p, tr); continue; }
        break;
    }
    if (ch(p, q) != '{') { scope_leave(p); panic_at(p, q, "Malformed regex"); }
    int body = ws(p, q + 1);
    int e;
    if ((ch(p, body) == '*' || at_lit(p, body, "<...>") || at_lit(p, body, "<*>")) && p->multiness == 2) {
        int t = ws(p, body + (ch(p, body) == '*' ? 1 : 5 - (at_lit(p, body, "<*>") ? 2 : 0)));
        e = t;
    }
    else {
        RkLang L; memset(&L, 0, sizeof L); L.regex = 1; L.start = '{'; L.stop = '}'; L.nrep = 1;
        e = r_regex_nibbler(p, q + 1, &L);
    }
    if (ch(p, e) != '}') { scope_leave(p); panic_at(p, e, "Unable to parse regex; couldn't find final '}'"); }
    scope_leave(p);
    return r_ENDSTMT(p, e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_package_declarator(RkP *p, int pos) {
    static const char *const k[] = { "package", "module", "class", "grammar", "role", "knowhow", "native", "slang", 0 };
    for (int i = 0; k[i]; i++) {
        int e = kok(p, pos, k[i]);
        if (e >= 0) return r_package_def(p, e, k[i]);
    }
    for (int i = 0; i < p->ndecls; i++) {
        int e = kok(p, pos, p->decls[i]);
        if (e >= 0) return r_package_def(p, e, "class");
    }
    int e;
    if ((e = kok(p, pos, "trusts")) >= 0) {
        int t = r_typename(p, e);
        if (t < 0) { int l = r_longname(p, e); if (l >= 0) panic_at(p, e, "Undeclared type '%.*s'", l - e, p->s + e); panic_at(p, e, "Malformed trusts"); }
        return t;
    }
    if ((e = kok(p, pos, "also")) >= 0) {
        int t = r_trait(p, e);
        if (t < 0) panic_at(p, e, "No valid trait found after also");
        t = r_traits(p, t);
        record_does(p, e, t);
        return t;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_package_def(RkP *p, int pos, const char *kind) {
    int q = pos;
    if (at_lit(p, q, "::(")) p->myst_off = 1;
    int n = r_longname(p, q);
    int pkg_from = q, pkg_len = n >= 0 ? name_part_len(p, q, n) : 0;
    char *saved_pkg = p->pkg;
    if (n >= 0) {
        int nl = name_part_len(p, q, n);
        add_name_all(p, p->s + q, nl);
        int pl = p->pkg ? (int) strlen(p->pkg) : 0;
        char *full = (char *) ct_alloc((size_t) pl + (size_t) nl + 3);
        if (pl) { memcpy(full, p->pkg, (size_t) pl); memcpy(full + pl, "::", 2); memcpy(full + pl + 2, p->s + q, (size_t) nl); full[pl + 2 + nl] = 0; add_our_name(p, full, pl + 2 + nl); }
        else { memcpy(full, p->s + q, (size_t) nl); full[nl] = 0; }
        p->pkg = full;
        q = ws(p, n);
    }
    int unit = p->scope == 9;
    int saved_scope = p->scope; p->scope = 0;
    RK_GROW(p->pkgs, p->npkgs, p->cpkgs, RkPkg);
    { RkPkg *k = &p->pkgs[p->npkgs++]; memset(k, 0, sizeof *k); k->kind = kind; k->name = pkg_len ? ct_strndup0(p->s + pkg_from, pkg_len) : NULL;
      k->unknown = saved_scope == 5 || saved_scope == 8; }
    scope_enter(p);
    if (!strcmp(kind, "role") && ch(p, q) == '[') {
        int s = r_signature(p, q + 1, 0); s = ws(p, s);
        if (ch(p, s) != ']') panic_at(p, s, "Unable to parse role signature; couldn't find final ']'");
        q = ws(p, s + 1);
    }
    int tstart = q;
    q = r_traits(p, q); q = ws(p, q);
    record_does(p, tstart, q);
    if (pkg_len) for (int t = tstart; t + 6 <= q; t++) if (!memcmp(p->s + t, "export", 6) && !wordch_at(p, t + 6) && !asc_word((unsigned char) p->s[t - 1])) {
        RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = ct_strndup0(p->s + pkg_from, pkg_len); break;
    }
    int e;
    if (ch(p, q) == '{') {
        if (unit) panic_at(p, q, "Cannot use 'unit' with block form of %s", kind);
        int sh = p->has_self; p->has_self = 1;
        e = r_blockoid(p, q);
        p->has_self = sh;
    }
    else if (ch(p, q) == ';') {
        if (!unit) {
            if (!strcmp(kind, "package")) panic_at(p, q,
                "This appears to be Perl code. If you intended it to be Raku code, please use a Raku style declaration like \"unit package Foo;\" or "
                "\"unit module Foo;\", or use the block form instead of the semicolon form.");
            panic_at(p, q, "Semicolon form of '%s' without 'unit' is illegal. You probably want to use 'unit %s'", kind, kind);
        }
        if (n < 0) panic_at(p, q, "Compilation unit cannot be anonymous");
        int sh = p->has_self; p->has_self = 1;
        e = r_statementlist(p, q + 1);
        p->has_self = sh;
    }
    else { scope_leave(p); p->scope = saved_scope; panic_at(p, q, "Unable to parse %s definition", kind); return -1; }
    scope_leave(p);
    p->scope = saved_scope;
    if (!unit) p->pkg = saved_pkg;
    if (!unit) package_close(p);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_enum_value(RkP *p, const char *en, int enl, const char *v, int vl) {
    add_our_name(p, v, vl); p->names[p->nnames - 1].value = 1;
    if (enl > 0) {
        char *full = (char *) ct_alloc((size_t) enl + (size_t) vl + 3);
        memcpy(full, en, (size_t) enl); memcpy(full + enl, "::", 2); memcpy(full + enl + 2, v, (size_t) vl); full[enl + 2 + vl] = 0;
        add_our_name(p, full, enl + 2 + vl); p->names[p->nnames - 1].value = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void enum_names_from(RkP *p, int from, int to, const char *en, int enl) {
    int q = from;
    int c = cp_at(p, q);
    if (c == '<' || c == 0xAB) {
        q += (at_lit(p, q, "<<") ? 2 : cp_len(p, q));
        while (q < to) {
            while (q < to && is_space_cp(cp_at(p, q))) q += cp_len(p, q);
            int s = q; while (q < to && !is_space_cp(cp_at(p, q)) && ch(p, q) != '>' && cp_at(p, q) != 0xBB) q += cp_len(p, q);
            if (q > s) add_enum_value(p, en, enl, p->s + s, q - s);
            if (ch(p, q) == '>' || cp_at(p, q) == 0xBB) break;
        }
        return;
    }
    for (q = from; q < to; ) {
        int e = r_identifier(p, q);
        if (e >= 0 && e <= to) {
            int h = hs(p, e);
            if (at_lit(p, h, "=>") || (q > from && p->s[q - 1] == ':' && (q < 2 || p->s[q - 2] != ':'))) add_enum_value(p, en, enl, p->s + q, e - q);
            if (q > from && (p->s[q - 1] == '\'' || p->s[q - 1] == '"')) add_enum_value(p, en, enl, p->s + q, e - q);
            q = e; continue;
        }
        if (ch(p, q) == '\'' || ch(p, q) == '"') { int s = q + 1; int t = s; while (t < to && p->s[t] != p->s[q]) t++; if (t > s) add_enum_value(p, en, enl, p->s + s, t - s); q = t + 1; continue; }
        q += cp_len(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_type_declarator(RkP *p, int pos) {
    int e;
    if ((e = kok(p, pos, "enum")) >= 0) {
        int q = e;
        int sd = p->in_decl; p->in_decl = 1;
        int n = r_longname(p, q); int en = q, enl = 0;
        if (n >= 0) { enl = name_part_len(p, q, n); add_name_all(p, p->s + q, enl); q = n; }
        else if (ch(p, q) == '$' || ch(p, q) == '@' || ch(p, q) == '%' || ch(p, q) == '&') { int v = r_variable(p, q); if (v >= 0) q = v; }
        p->in_decl = sd;
        q = ws(p, q);
        q = r_traits(p, q); q = ws(p, q);
        int c = cp_at(p, q);
        if (!(c == '<' || c == '(' || c == 0xAB)) panic_at(p, q, "An enum must supply an expression using <>, \xc2\xab\xc2\xbb, or ()");
        int t = r_term(p, q);
        if (t < 0) panic_at(p, q, "An enum must supply an expression using <>, \xc2\xab\xc2\xbb, or ()");
        enum_names_from(p, q, t, p->s + en, enl);
        for (int k = q, quo = 0; k < t; k++) {
            unsigned char b = (unsigned char) p->s[k];
            if (b == '\'' || b == '"') quo = !quo;
            else if (!quo && (b == '.' || b == '~' || b == '*' || b == '+' || b == '^' || b == '$' || b == '@' || b == '&')) { p->myst_off = 1; break; }
        }
        return ws(p, t);
    }
    if ((e = kok(p, pos, "subset")) >= 0) {
        int q = e;
        int n = r_longname(p, q);
        if (n >= 0) { add_name_all(p, p->s + q, name_part_len(p, q, n)); q = ws(p, n); }
        q = r_traits(p, q); q = ws(p, q);
        int w = kw(p, q, "where");
        if (w >= 0) { int t = ws(p, w); int x = r_EXPR(p, t, PR('e')); if (x < 0) panic_at(p, t, "Malformed subset"); return x; }
        return q;
    }
    if ((e = kok(p, pos, "constant")) >= 0) {
        int q = e;
        int sd = p->in_decl; p->in_decl = 1;
        if (ch(p, q) == '\\') q++;
        int n = r_identifier(p, q);
        if (n >= 0) { while (ch(p, n) == ':' && ch(p, n + 1) != ':' && ch(p, n + 1) != '=') { int f = r_colonpair(p, n); if (f < 0) break; n = f; } add_name_n(p, p->s + q, name_part_len(p, q, n));
            register_user_op(p, q, n); q = n; }
        else if (ch(p, q) == '$' || ch(p, q) == '@' || ch(p, q) == '%' || ch(p, q) == '&') { int v = r_variable(p, q); if (v >= 0) { if (ch(p, q) == '&') register_user_op(p, q + 1, v); q = v; } }
        p->in_decl = sd;
        q = ws(p, q);
        q = r_traits(p, q); q = ws(p, q);
        int i = r_initializer(p, q);
        if (i < 0) panic_at(p, q, "Missing initializer on constant declaration");
        return i;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_scoped(RkP *p, int pos, int scope) {
    int ss = p->scope; p->scope = scope + 1;
    int q = ws(p, pos);
    int e = -1;
    if ((e = r_declarator(p, q)) >= 0) goto done;
    if ((e = r_regex_declarator(p, q)) >= 0) goto done;
    if ((e = r_package_declarator(p, q)) >= 0) goto done;
    {
        int t = q; int nt = 0;
        for (;;) { int tn = r_typename(p, t); if (tn < 0) break; t = ws(p, tn); nt++; }
        if (nt) {
            e = r_multi_declarator(p, t);
            if (e < 0) e = r_declarator(p, t);
            if (e >= 0) goto done;
            int h = t;
            if (ch(p, h) == 'w' && kw(p, h, "where") >= 0) panic_at(p, h, "Malformed %s (found type followed by constraint; did you forget a variable in between?)", "declaration");
            if (r_trait(p, h) >= 0) panic_at(p, h, "Malformed %s (found type followed by trait; did you forget a variable in between?)", "declaration");
            if (is_terminator_at(p, h) || h >= p->n) panic_at(p, h, "Malformed %s (did you forget a variable after type?)", "declaration");
        }
    }
    if ((e = r_multi_declarator(p, q)) >= 0) goto done;
    {
        int t = q;
        for (;;) { int tn = r_typename(p, t); if (tn < 0) break; t = ws(p, tn); }
        int id = r_ident(p, t);
        if (id >= 0) {
            int h = ws(p, id);
            if (ch(p, h) == '=' || at_lit(p, h, ":=") || at_lit(p, h, "::=") || is_terminator_at(p, h) || h >= p->n || r_trait(p, h) >= 0 || kw(p, h, "where") >= 0) {
                if (is_alpha_cp(cp_at(p, t)) && !is_name_n(p, p->s + t, id - t) && t == q && p->scope != 7) {
                    panic_at(p, t, "Malformed declaration (did you mean to declare a sigilless \\%.*s or $%.*s?)", id - t, p->s + t, id - t, p->s + t);
                }
            }
            if (t == q && !is_name_n(p, p->s + t, id - t)) {
                int l = r_longname(p, t);
                if (l >= 0 && is_alpha_cp(cp_at(p, t)) && (cp_at(p, t) >= 'A' && cp_at(p, t) <= 'Z')) panic_at(p, t, "Undeclared type '%.*s'", name_part_len(p, t, l), p->s + t);
            }
        }
    }
    p->scope = ss;
    panic_at(p, q, "Malformed %s",
        scope == 0 ? "my" : scope == 1 ? "our" : scope == 2 ? "has" : scope == 3 ? "HAS" : scope == 4 ? "augment" : scope == 5 ? "anon" : scope == 6 ? "state" : scope == 7 ? "supersede" : "unit");
    return -1;
done:
    p->scope = ss;
    return e;
}
/*====================================================================================================================================================================================================*/
static int r_statement_mod_expr(RkP *p, int pos, const char *k) {
    int e = r_EXPR(p, pos, 0);
    if (e < 0) panic_at(p, pos, "Missing expression for '%s' statement modifier", k);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_mods(RkP *p, int pos) {
    static const char *const cond[] = { "if", "unless", "when", "with", "without", 0 };
    static const char *const loop[] = { "while", "until", "for", "given", 0 };
    int q = ws(p, pos);
    for (int i = 0; cond[i]; i++) {
        int e = kok(p, q, cond[i]);
        if (e < 0) continue;
        int x = r_statement_mod_expr(p, e, cond[i]);
        int t = ws(p, x);
        for (int j = 0; loop[j]; j++) { int f = kok(p, t, loop[j]); if (f >= 0) return r_statement_mod_expr(p, f, loop[j]); }
        return x;
    }
    for (int j = 0; loop[j]; j++) {
        int f = kok(p, q, loop[j]);
        if (f >= 0) return r_statement_mod_expr(p, f, loop[j]);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_label(RkP *p, int pos) {
    int e = r_identifier(p, pos);
    if (e < 0 || ch(p, e) != ':' || !is_space_cp(cp_at(p, e + 1))) return -1;
    if (is_name_n(p, p->s + pos, e - pos) && 0) return -1;
    add_name_n(p, p->s + pos, e - pos);
    return ws(p, e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == ')' || c == ']' || c == '}' || pos >= p->n) return -1;
    if (c == ';') return pos;
    int e;
    if ((e = r_label(p, pos)) >= 0) return r_statement(p, e);
    if ((e = r_statement_control(p, pos)) >= 0) return e;
    int si = p->invocant_ok; int sq = p->qsigil; p->qsigil = 0;
    e = r_EXPR(p, pos, 0);
    p->invocant_ok = si; p->qsigil = sq;
    if (e >= 0) {
        if (marked_end(p, e) || marked_end(p, ws(p, e))) return e;
        int m = r_statement_mods(p, e);
        if (m >= 0) return m;
        return e;
    }
    if (is_terminator_at(p, pos)) return pos;
    panic_at(p, pos, "Bogus statement");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_in_regex(RkP *p, int pos) {
    int e = r_statement(p, pos);
    if (e < 0) return -1;
    int q = ws(p, e);
    if (ch(p, q) == ';') return q + 1;
    if (marked_end(p, q)) return q;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_eat_terminator(RkP *p, int pos) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statementlist(RkP *p, int pos) {
    int q = ws(p, pos);
    for (;;) {
        if (p->finished) return p->n;
        int c = ch(p, q);
        if (q >= p->n || c == ')' || c == ']' || c == '}') return q;
        int e = r_statement(p, q);
        if (e < 0) return q;
        q = r_eat_terminator(p, e);
        q = ws(p, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_check_into(RkP *p, const char *src, int len, const char *path, RkP *out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_use_lib(RkP *p, int from, int to) {
    int q = from;
    while (q < to) {
        int parent = 1; const char *lit_s = NULL; int lit_n = 0; int prog = 0;
        int e = q;
        if (at_lit(p, q, "$*PROGRAM")) {
            prog = 1; e = q + 9;
            while (e < to && p->s[e] != ',' && p->s[e] != ';') {
                if (at_lit(p, e, ".parent(")) { parent = atoi(p->s + e + 8); e += 8; continue; }
                if (at_lit(p, e, ".parent")) { parent = 1; e += 7; continue; }
                if (p->s[e] == '\'' || p->s[e] == '"') { char qc = p->s[e]; int b = e + 1; int t = b; while (t < to && p->s[t] != qc) t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; continue; }
                e++;
            }
        }
        else if (p->s[q] == '\'' || p->s[q] == '"') { char qc = p->s[q]; int b = q + 1; int t = b; while (t < to && p->s[t] != qc) t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; }
        else if (p->s[q] == '<') { int b = q + 1; int t = b; while (t < to && p->s[t] != '>') t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; }
        if (lit_s) {
            char buf[1024];
            if (prog) {
                char dir[1024]; snprintf(dir, sizeof dir, "%s", p->file);
                for (int k = 0; k < parent; k++) { char *sl = strrchr(dir, '/'); if (sl) *sl = 0; else snprintf(dir, sizeof dir, "."); }
                snprintf(buf, sizeof buf, "%s/%.*s", dir, lit_n, lit_s);
            }
            else if (lit_n && lit_s[0] == '/') snprintf(buf, sizeof buf, "%.*s", lit_n, lit_s);
            else { char dir[1024]; snprintf(dir, sizeof dir, "%s", p->file); char *sl = strrchr(dir, '/'); if (sl) *sl = 0; else snprintf(dir, sizeof dir, "."); snprintf(buf, sizeof buf, "%s/%.*s",
                dir, lit_n, lit_s); }
            RK_GROW(p->libs, p->nlibs, p->clibs, char *);
            p->libs[p->nlibs++] = ct_strndup0(buf, (int) strlen(buf));
        }
        q = e > q ? e : q + 1;
        while (q < to && (p->s[q] == ',' || asc_space((unsigned char) p->s[q]))) q++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int import_module(RkP *p, const char *name) {
    if (p->load_depth >= 4) return 0;
    char *rel = (char *) ct_alloc(strlen(name) + 1); int k = 0;
    for (int i = 0; name[i]; i++) { if (name[i] == ':' && name[i + 1] == ':') { rel[k++] = '/'; i++; } else rel[k++] = name[i]; }
    rel[k] = 0;
    static const char *const exts[] = { ".rakumod", ".pm6", ".pm" };
    for (int l = p->nlibs - 1; l >= 0; l--) {
        for (int x = 0; x < 6; x++) {
            const char *ext = exts[x % 3], *sub = x < 3 ? "" : "lib/";
            size_t pl = strlen(p->libs[l]) + strlen(rel) + strlen(ext) + 6;
            char *path = (char *) ct_alloc(pl); snprintf(path, pl, "%s/%s%s%s", p->libs[l], sub, rel, ext);
            FILE *f = fopen(path, "rb");
            if (!f) continue;
            fseek(f, 0, SEEK_END); long n = ftell(f); rewind(f);
            char *src = (char *) ct_alloc((size_t) n + 1);
            size_t got = fread(src, 1, (size_t) n, f); fclose(f); src[got] = 0;
            RkP *m = (RkP *) ct_alloc(sizeof(RkP));
            memset(m, 0, sizeof *m);
            m->load_depth = p->load_depth + 1;
            for (int i = 0; i < p->nlibs; i++) { RK_GROW(m->libs, m->nlibs, m->clibs, char *); m->libs[m->nlibs++] = p->libs[i]; }
            if (rk_check_into(m, src, (int) got, path, m) != 0) return 0;
            for (int i = 0; i < m->nexports; i++) add_name_n(p, m->exports[i], (int) strlen(m->exports[i]));
            for (int i = 0; i < m->nnames; i++) if (m->names[i].depth <= 1 && m->names[i].name[0] == '&' && !strcmp(m->names[i].name, "&EXPORT")) p->myst_off = 1;
            for (int i = 0; i < m->nnames; i++) {
                const char *nm = m->names[i].name;
                if (!strncmp(nm, "EXPORTHOW::DECLARE::", 20) && nm[20]) { RK_GROW(p->decls, p->ndecls, p->cdecls, char *); p->decls[p->ndecls++] = ct_strndup0(nm + 20, (int) strlen(nm + 20)); }
                if (m->names[i].depth == 0) { add_name_n(p, nm, (int) strlen(nm)); p->names[p->nnames - 1].value = m->names[i].value; }
            }
            for (int i = 0; i < m->nuops; i++) if (m->uops[i].depth >= 1000 || m->uops[i].depth <= 1) {
                RkUserOp u = m->uops[i];
                add_user_op(p, u.cat, u.sym, u.len, u.prec);
                p->uops[p->nuops - 1].assoc = u.assoc; p->uops[p->nuops - 1].flags = u.flags;
            }
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_module_name(RkP *p, int pos) {
    int e = r_longname(p, pos);
    if (e < 0) return -1;
    if (ch(p, e) == '[') { int a = r_arglist(p, e + 1); a = ws(p, a); if (ch(p, a) != ']') panic_at(p, a, "Unable to parse generic role; couldn't find final ']'"); e = a + 1; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_use_like(RkP *p, int pos, const char *what) {
    int q = pos;
    int v = r_version(p, q);
    if (v >= 0) {
        if (!strcmp(what, "use")) {
            if (!p->comp_unit_begin) panic_at(p, q, "Too late to switch language version. Must be used as the very first statement.");
            return ws(p, v);
        }
        return ws(p, v);
    }
    int m = r_module_name(p, q);
    if (m < 0) panic_at(p, q, "Malformed %s", what);
    char *name = ct_strndup0(p->s + q, name_part_len(p, q, m));
    if (!strcmp(what, "use") && !strcmp(name, "parameters")) panic_at(p, q, "use parameters not yet implemented. Sorry.");
    if (!strcmp(name, "isms")) p->p5isms = !strcmp(what, "use");
    if (!strcmp(name, "strict")) p->lax = !strcmp(what, "no");
    int e = m;
    int c = cp_at(p, e);
    int args_from = e;
    if (is_space_cp(c) || c == '#') {
        int t = ws(p, e);
        if (!stdstopper(p, t)) { int a = r_arglist(p, e); e = a; }
    }
    static const char *const pragmas[] = { "lib", "soft", "strict", "fatal", "MONKEY", "dynamic-scope", "isms", "variables", "attributes", "invocant", "parameters", "experimental", "newline",
                                           "internals", "nqp", "precompilation", "trace", "worries", "v6", "BUILDPLAN", 0 };
    int is_pragma = !strncmp(name, "MONKEY-", 7);
    for (int i = 0; pragmas[i]; i++) if (!strcmp(name, pragmas[i])) is_pragma = 1;
    if (!strcmp(what, "use") && !strcmp(name, "lib")) add_use_lib(p, ws(p, args_from), e);
    else if (!strcmp(what, "use") && !strcmp(name, "Test")) for (size_t i = 0; i < sizeof rk_test_routines / sizeof *rk_test_routines; i++) add_routine_name(p, rk_test_routines[i],
        (int) strlen(rk_test_routines[i]));
    else if (!strcmp(what, "use") && !is_pragma) { if (!import_module(p, name)) p->myst_off = 1; }
    return ws(p, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_control(RkP *p, int pos) {
    int c = ch(p, pos);
    if (!is_alpha_cp(c)) return -1;
    int e;
    if ((e = kok(p, pos, "if")) >= 0 || (e = kok(p, pos, "with")) >= 0) {
        int q = r_xblock(p, e, 0);
        for (;;) {
            int t = ws(p, q);
            int f;
            if (at_lit(p, t, "else") && (f = hs(p, t + 4)) && at_lit(p, f, "if") && !wordch_at(p, f + 2)) panic_at(p, t, "In Raku, please use \"elsif\" instead of \"else if\"");
            if ((f = kw(p, t, "elif")) >= 0) panic_at(p, t, "In Raku, please use \"elsif\" instead of \"elif\"");
            if ((f = kok(p, t, "elsif")) >= 0 || (f = kok(p, t, "orwith")) >= 0) { q = r_xblock(p, f, 0); continue; }
            if ((f = kw(p, t, "else")) >= 0) { int g = ws(p, f); return r_pblock(p, g, 0); }
            return q;
        }
    }
    if ((e = kok(p, pos, "unless")) >= 0 || (e = kok(p, pos, "without")) >= 0) {
        int q = r_xblock(p, e, 0);
        int t = ws(p, q);
        if (kw(p, t, "else") >= 0 || kw(p, t, "elsif") >= 0 || kw(p, t, "orwith") >= 0) panic_at(p, t, "\"%s\" does not take \"%.*s\", please rewrite using \"if\"", at_lit(p, pos,
            "unless") ? "unless" : "without", 4, p->s + t);
        return q;
    }
    if ((e = kok(p, pos, "while")) >= 0 || (e = kok(p, pos, "until")) >= 0) return r_xblock(p, e, 0);
    if ((e = kok(p, pos, "repeat")) >= 0) {
        int f;
        if ((f = kok(p, e, "while")) >= 0 || (f = kok(p, e, "until")) >= 0) return r_xblock(p, f, 0);
        int q = r_pblock(p, e, 0);
        int t = ws(p, q);
        if ((f = kok(p, t, "while")) < 0 && (f = kok(p, t, "until")) < 0) panic_at(p, t, "Missing \"while\" or \"until\"");
        int x = r_EXPR(p, f, 0);
        if (x < 0) panic_at(p, f, "Missing expression");
        return x;
    }
    if ((e = kok(p, pos, "for")) >= 0) {
        if (ch(p, e) == '(') {
            int d = 1; int t = e + 1; int semis = 0;
            while (t < p->n && d) { if (p->s[t] == '(') d++; else if (p->s[t] == ')') d--; else if (p->s[t] == ';' && d == 1) semis++; t++; }
            if (semis == 2) panic_at(p, pos, "Unsupported use of C-style \"for (;;)\" loop; in Raku please use \"loop (;;)\"");
        }
        return r_xblock(p, e, 2);
    }
    if ((e = kok(p, pos, "whenever")) >= 0) return r_xblock(p, e, 2);
    if ((e = kw_end(p, pos, "foreach")) >= 0) panic_at(p, pos, "Unsupported use of 'foreach'; in Raku please use 'for'");
    if ((e = kok(p, pos, "loop")) >= 0) {
        int q = e;
        if (ch(p, q) == '(') {
            int t = ws(p, q + 1); int n = 0;
            int x = r_EXPR(p, t, 0); if (x >= 0) { t = ws(p, x); n = 1; }
            if (ch(p, t) == ';') { t = ws(p, t + 1); n = 2; x = r_EXPR(p, t, 0); if (x >= 0) t = ws(p, x);
                if (ch(p, t) == ';') { t = ws(p, t + 1); n = 3; x = r_EXPR(p, t, 0); if (x >= 0) t = ws(p, x); } }
            if (n == 3 && ch(p, t) == ')') q = ws(p, t + 1);
            else if (ch(p, t) == ')') panic_at(p, t,
                n == 0 ? "Malformed loop spec (expected 3 semicolon-separated expressions)" : "Malformed loop spec (expected 3 semicolon-separated expressions but got %d)", n);
            else if (ch(p, t) == ';') panic_at(p, t, "Malformed loop spec (expected 3 semicolon-separated expressions but got more)");
            else panic_at(p, t, "Malformed loop spec");
        }
        return r_block(p, q);
    }
    if ((e = kok(p, pos, "need")) >= 0) {
        int q = e;
        for (;;) {
            if (r_version(p, q) >= 0) panic_at(p, q, "In case of using pragma, use \"use\" instead (e.g., \"use v6;\", \"use v6.c;\").");
            int m = r_module_name(p, q); if (m < 0) panic_at(p, q, "Malformed need");
            import_module(p, ct_strndup0(p->s + q, name_part_len(p, q, m)));
            q = ws(p, m);
            if (ch(p, q) == ',') { q = ws(p, q + 1); continue; }
            return q;
        }
    }
    if ((e = lit(p, pos, "import")) >= 0 && !wordch_at(p, e)) {
        int q = ws(p, e);
        int m = r_module_name(p, q);
        if (m < 0) panic_at(p, q, "Malformed import");
        if (!is_pseudo_pkg(p->s + q, name_part_len(p, q, m))) p->myst_off = 1;
        int t = m;
        if (is_space_cp(cp_at(p, t)) || ch(p, t) == '#') { int u = ws(p, t); if (!stdstopper(p, u)) t = r_arglist(p, t); }
        return ws(p, t);
    }
    if ((e = lit(p, pos, "no")) >= 0 && !wordch_at(p, e) && is_space_cp(cp_at(p, e))) return r_use_like(p, ws(p, e), "no");
    if ((e = lit(p, pos, "use")) >= 0 && !wordch_at(p, e) && is_space_cp(cp_at(p, e))) return r_use_like(p, ws(p, e), "use");
    if ((e = lit(p, pos, "DOC")) >= 0 && is_hspace_cp(cp_at(p, e))) { int u = hs(p, e); int f = lit(p, u, "use"); if (f >= 0 && !wordch_at(p, f)) return r_use_like(p, ws(p, f), "use"); }
    if ((e = kok(p, pos, "require")) >= 0) {
        p->myst_off = 1;
        int q = e;
        int m = r_module_name(p, q);
        if (m < 0) m = r_variable(p, q);
        if (m < 0 && ch(p, q) != '$' && ch(p, q) != '@') m = r_term(p, q);
        if (m < 0) panic_at(p, q, "Malformed require");
        q = ws(p, m);
        if (!stdstopper(p, q)) { int x = r_EXPR(p, q, 0); if (x >= 0) q = x; }
        return q;
    }
    if ((e = kok(p, pos, "given")) >= 0) return r_xblock(p, e, 2);
    if ((e = kok(p, pos, "when")) >= 0) return r_xblock(p, e, 0);
    if ((e = kok(p, pos, "default")) >= 0) return r_block(p, e);
    if ((e = lit(p, pos, "CATCH")) >= 0 || (e = lit(p, pos, "CONTROL")) >= 0 || (e = lit(p, pos, "QUIT")) >= 0) {
        if (wordch_at(p, e)) return -1;
        int q = ws(p, e);
        if (ch(p, q) != '{') return -1;
        return r_block(p, q);
    }
    return -1;
}
/*====================================================================================================================================================================================================*/
static int r_comp_unit(RkP *p) {
    int q = 0;
    if (at_lit(p, 0, "\xef\xbb\xbf")) q = 3;
    if (at_lit(p, q, "#!")) q = line_end(p, q);
    p->comp_unit_begin = 1;
    q = ws(p, q);
    int v = lit(p, q, "use");
    if (v >= 0 && is_space_cp(cp_at(p, v))) {
        int t = ws(p, v);
        int ve = r_version(p, t);
        if (ve >= 0) {
            if (!(ch(p, t + 1) == '6')) panic_at(p, t, "No compiler available for Raku %.*s", ve - t, p->s + t);
            if (ve - t >= 4 && p->s[t + 3] != 'c' && p->s[t + 3] != 'd') p->lang_e = 1;
            q = r_eat_terminator(p, ws(p, ve));
        }
    }
    p->comp_unit_begin = 0;
    scope_enter(p);
    int e = r_statementlist(p, q);
    e = ws(p, e);
    if (e < p->n && !p->finished) {
        int c = ch(p, e);
        if (c == ')' || c == ']' || c == '}') panic_at(p, e, "Unexpected closing bracket");
        panic_at(p, e, "Confused");
    }
    if (p->nhere) panic_at(p, p->n, "Ending delimiter %s not found", p->here[0].delim);
    while (p->npkgs) package_close(p);
    resolve_mysteries(p, 0);
    if (!p->myst_off && p->nmyst) {
        RkMyst m = p->myst[0];
        panic_at(p, m.pos, "Undeclared %s: %.*s used at line %d", (p->s[m.pos] >= 'a' || p->s[m.pos] == '&') ? "routine" : "name", m.len, p->s + m.pos, line_of(p, m.pos));
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_check_into(RkP *p, const char *src, int len, const char *path, RkP *out) {
    (void) out;
    p->s = src; p->n = len; p->file = path ? path : "<stdin>";
    p->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(p->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    p->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(p->endmark, 0, (size_t) len + 2);
    if (setjmp(p->jb)) return 1;
    r_comp_unit(p);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_syntax_check(const char *src, int len, const char *path, char *err, int errlen) {
    RkP *p = (RkP *) ct_alloc(sizeof(RkP));
    memset(p, 0, sizeof *p);
    p->s = src; p->n = len; p->file = path ? path : "<stdin>";
    p->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(p->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    p->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(p->endmark, 0, (size_t) len + 2);
    if (setjmp(p->jb)) {
        int line = line_of(p, p->err_pos);
        int col = 1; for (int i = p->err_pos - 1; i >= 0 && src[i] != '\n'; i--) col++;
        if (err && errlen > 0) snprintf(err, (size_t) errlen, "%s:%d:%d: raku syntax error: %s", p->file, line, col, p->msg);
        return 1;
    }
    r_comp_unit(p);
    if (err && errlen > 0) err[0] = 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_syntax_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", path); return 2; }
    fseek(f, 0, SEEK_END); long n = ftell(f); rewind(f);
    char *src = (char *) ct_alloc((size_t) n + 1);
    if (fread(src, 1, (size_t) n, f) != (size_t) n) { fclose(f); fprintf(stderr, "scrip: short read on '%s'\n", path); return 2; }
    src[n] = 0; fclose(f);
    char err[600];
    int rc = rk_syntax_check(src, (int) n, path, err, sizeof err);
    if (rc) fprintf(stderr, "%s\n", err);
    return rc;
}
