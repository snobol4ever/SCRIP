/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rx.h"
#include "rt/rt_arena.h"
#define RX_ALLOC(n) rx_alloc0(n)
#define RX_REALLOC(p, n) rt_wsb_realloc((p), (n))
static void *rx_alloc0(size_t n) { void *p = rt_wsb_alloc(n); memset(p, 0, n ? n : 1); return p; }
typedef enum {
    N_LIT, N_ANY, N_SET, N_BOS, N_EOS, N_BOL, N_EOL, N_WB, N_NWB, N_LWB, N_RWB, N_WW, N_NWW, N_SEQ, N_ALT, N_CONJ, N_GROUP, N_QUANT, N_LOOK, N_BACKREF, N_RULE, N_NULL, N_FAIL, N_ATOMIC, N_WS, N_CODE,
        N_SPAN, N_NLN, N_MARK
} NK;
enum { CL_DIGIT = 1, CL_WORD, CL_SPACE, CL_HSPACE, CL_VSPACE, CL_ALPHA, CL_ALNUM, CL_UPPER, CL_LOWER, CL_PUNCT, CL_XDIGIT, CL_CNTRL, CL_PRINT, CL_GRAPH, CL_BLANK, CL_NL, CL_ASCII };
typedef struct RxSetItem { char op, kind; int cls, neg, nrng, plen; unsigned *rng; const char *prop; } RxSetItem;
typedef struct RxSet { int n; RxSetItem *it; } RxSet;
typedef struct RxNode {
    unsigned char kind, fl, neg, lazy, behind, ltm, ratchet, trail, silent, looked;
    int nch, min, max, slen;
    struct RxNode **ch, *sep;
    const char *s;
    const RxCap *cap;
    RxSet *set;
    struct RxProg *sub;
} RxNode;
typedef struct CapList { RxCap **v; int n, cap; } CapList;
struct RxProg { RxNode *root; int flags; const RxEnv *env; CapList tops; };
static RxCap rx_silent_cap = { 'h', 0, "", 0, 0, 0, 0, 0, 0 };
typedef struct Ctx { const RxEnv *env; const unsigned char *s; int slen, tend, nev, cev; RxEv *ev; int depth, mfrom, mto; } Ctx;
typedef struct Cont Cont;
struct Cont { int (*fn)(Ctx *, const Cont *, int); const RxNode *n; int a, b, c; const Cont *up; };
typedef struct P { const char *s; int n, i, flags, pidx, angle, sigok, esc_more, esc_end, nmark, force_rep; const char *err; const RxEnv *env; CapList *cur; } P;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rx_cp(const unsigned char *s, int n, int i, int *len) {
    unsigned char c = s[i];
    if (c < 0x80) { *len = 1; return c; }
    int k = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
    if (k == 1 || i + k > n) { *len = 1; return c; }
    unsigned v = c & (0x7Fu >> k);
    for (int j = 1; j < k; j++) { if ((s[i + j] & 0xC0) != 0x80) { *len = 1; return c; } v = (v << 6) | (s[i + j] & 0x3Fu); }
    *len = k;
    return v;
}
static int rx_putcp(unsigned cp, char *o) {
    if (cp < 0x80) { o[0] = (char) cp; return 1; }
    if (cp < 0x800) { o[0] = (char) (0xC0 | (cp >> 6)); o[1] = (char) (0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char) (0xE0 | (cp >> 12)); o[1] = (char) (0x80 | ((cp >> 6) & 0x3F)); o[2] = (char) (0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char) (0xF0 | ((cp >> 18) & 7));
    o[1] = (char) (0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char) (0x80 | ((cp >> 6) & 0x3F));
    o[3] = (char) (0x80 | (cp & 0x3F));
    return 4;
}
static int rx_prev(const unsigned char *s, int i) { int p = i - 1; while (p > 0 && i - p < 4 && (s[p] & 0xC0) == 0x80) p--; return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rx_fold(const RxEnv *e, unsigned cp, int mode) {
    if (e && e->fold) return e->fold(e->ud, cp, mode);
    if (mode == 1) return cp >= 'a' && cp <= 'z' ? cp - 32 : cp;
    return cp >= 'A' && cp <= 'Z' ? cp + 32 : cp;
}
static int rx_isdigit_u(unsigned c) {
    static const unsigned nd[] = { 0x30, 0x660, 0x6F0, 0x7C0, 0x966, 0x9E6, 0xA66, 0xAE6, 0xB66, 0xBE6, 0xC66, 0xCE6, 0xD66, 0xDE6, 0xE50, 0xED0, 0xF20, 0x1040, 0x1090, 0x17E0, 0x1810, 0x1946, 0x19D0,
        0x1A80, 0x1A90, 0x1B50, 0x1BB0, 0x1C40, 0x1C50, 0xA620, 0xA8D0, 0xA900, 0xA9D0, 0xA9F0, 0xAA50, 0xABF0, 0xFF10, 0x104A0, 0x1D7CE, 0x1D7D8, 0x1D7E2, 0x1D7EC, 0x1D7F6 };
    if (c < 0x80) return c >= '0' && c <= '9';
    for (size_t k = 0; k < sizeof nd / sizeof *nd; k++) if (c >= nd[k] && c < nd[k] + 10) return 1;
    return 0;
}
static int rx_isalpha_u(const RxEnv *e, unsigned c) {
    if (c < 0x80) return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    if (e && e->prop) { int r = e->prop(e->ud, "L", 1, c); if (r >= 0) return r; }
    if (c == 0xAA || c == 0xB5 || c == 0xBA) return 1;
    if (c < 0xC0) return 0;
    if (c == 0xD7 || c == 0xF7) return 0;
    if (c >= 0x2000 && c <= 0x2BFF) return 0;
    if (c >= 0x3000 && c <= 0x303F) return 0;
    if (c >= 0xFE00 && c <= 0xFE6F) return 0;
    if (c >= 0xFF00 && c <= 0xFF20) return 0;
    if (c >= 0x1F000 && c <= 0x1FFFF) return 0;
    return 1;
}
static int rx_isspace_u(unsigned c) {
    return (c >= 9 && c <= 13) || c == 32 || c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}
static int rx_ishspace_u(unsigned c) { return c == 9 || c == 32 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x202F || c == 0x205F || c == 0x3000; }
static int rx_isvspace_u(unsigned c) { return (c >= 10 && c <= 13) || c == 0x85 || c == 0x2028 || c == 0x2029; }
static int rx_isword_u(const RxEnv *e, unsigned c) { return c == '_' || rx_isdigit_u(c) || rx_isalpha_u(e, c); }
static int rx_cls(const RxEnv *e, int cls, unsigned c) {
    switch (cls) {
        case CL_DIGIT:
        return rx_isdigit_u(c);
        case CL_WORD:
        return rx_isword_u(e, c);
        case CL_SPACE:
        return rx_isspace_u(c);
        case CL_HSPACE:
        return rx_ishspace_u(c);
        case CL_BLANK:
        return rx_ishspace_u(c);
        case CL_VSPACE:
        return rx_isvspace_u(c);
        case CL_ASCII:
        return c < 0x80;
        case CL_NL:
        return c == 10 || c == 13 || c == 0x85 || c == 0x2028 || c == 0x2029;
        case CL_ALPHA:
        return c == '_' || rx_isalpha_u(e, c);
        case CL_ALNUM:
        return c == '_' || rx_isdigit_u(c) || rx_isalpha_u(e, c);
        case CL_UPPER:
        return c < 0x80 ? (c >= 'A' && c <= 'Z') : (rx_isalpha_u(e, c) && rx_fold(e, c, 2) != c);
        case CL_LOWER:
        return c < 0x80 ? (c >= 'a' && c <= 'z') : (rx_isalpha_u(e, c) && rx_fold(e, c, 1) != c);
        case CL_PUNCT:
        return c < 0x80 ? ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126)) && c != '$' && c != '+' && c != '<' && c != '=' && c != '>' && c != '^' &&
            c != '`' && c != '|' && c != '~' : (c >= 0x2010 && c <= 0x2027) || (c >= 0x3001 && c <= 0x3003) || c == 0xA1 || c == 0xA7 || c == 0xAB || c == 0xB6 || c == 0xB7 || c == 0xBB || c == 0xBF;
        case CL_XDIGIT:
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        case CL_CNTRL:
        return c < 32 || (c >= 0x7F && c < 0xA0);
        case CL_PRINT:
        return c >= 32 && c != 0x7F && !(c >= 0x80 && c < 0xA0);
        case CL_GRAPH:
        return c > 32 && c != 0x7F && !(c >= 0x80 && c < 0xA0) && !rx_isspace_u(c);
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_set_item_test(const RxEnv *e, const RxSetItem *it, unsigned c, int fl) {
    int r = 0;
    if (it->kind == 'R') {
        for (int k = 0; k < it->nrng && !r; k++) r = c >= it->rng[2 * k] && c <= it->rng[2 * k + 1];
        if (!r && (fl & RXF_I)) {
            unsigned u = rx_fold(e, c, 1), l = rx_fold(e, c, 2);
            for (int k = 0; k < it->nrng && !r; k++) r = (u >= it->rng[2 * k] && u <= it->rng[2 * k + 1]) || (l >= it->rng[2 * k] && l <= it->rng[2 * k + 1]);
        }
    } else if (it->kind == 'C') r = rx_cls(e, it->cls, c);
    else if (it->kind == 'P') { int v = e && e->prop ? e->prop(e->ud, it->prop, it->plen, c) : -1; r = v > 0; }
    return it->neg ? !r : r;
}
static int rx_set_test(const RxEnv *e, const RxSet *st, unsigned c, int fl) {
    int in = st->n > 0 && st->it[0].op == '-';
    for (int k = 0; k < st->n; k++) { int t = rx_set_item_test(e, &st->it[k], c, fl); if (st->it[k].op == '-') in = in && !t; else in = in || t; }
    return in;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *nn(NK k) { RxNode *n = (RxNode *) RX_ALLOC(sizeof *n); n->kind = (unsigned char) k; n->max = -1; return n; }
static void nadd(RxNode *p, RxNode *c) { p->ch = (RxNode **) (p->ch ? RX_REALLOC(p->ch, sizeof(RxNode *) * (size_t) (p->nch + 1)) : RX_ALLOC(sizeof(RxNode *))); p->ch[p->nch++] = c; }
static RxNode *nlit(const char *s, int n, int fl) {
    RxNode *x = nn(N_LIT);
    char *b = (char *) RX_ALLOC((size_t) n + 1);
    memcpy(b, s, (size_t) n);
    x->s = b;
    x->slen = n;
    x->fl = (unsigned char) fl;
    return x;
}
static RxNode *ncls(int cls, int neg) {
    RxNode *x = nn(N_SET);
    x->set = (RxSet *) RX_ALLOC(sizeof(RxSet));
    x->set->n = 1;
    x->set->it = (RxSetItem *) RX_ALLOC(sizeof(RxSetItem));
    x->set->it[0].op = '+';
    x->set->it[0].kind = 'C';
    x->set->it[0].cls = cls;
    x->set->it[0].neg = neg;
    return x;
}
static RxNode *ncp(unsigned cp, int neg, int fl) {
    RxNode *x = nn(N_SET);
    x->set = (RxSet *) RX_ALLOC(sizeof(RxSet));
    x->set->n = 1;
    x->set->it = (RxSetItem *) RX_ALLOC(sizeof(RxSetItem));
    x->set->it[0].op = '+';
    x->set->it[0].kind = 'R';
    x->set->it[0].neg = neg;
    x->set->it[0].nrng = 1;
    x->set->it[0].rng = (unsigned *) RX_ALLOC(2 * sizeof(unsigned));
    x->set->it[0].rng[0] = x->set->it[0].rng[1] = cp;
    x->fl = (unsigned char) fl;
    return x;
}
static void perr(P *p, const char *m) { if (!p->err) p->err = m; }
static int peekc(P *p) { return p->i < p->n ? (unsigned char) p->s[p->i] : 0; }
static int isw(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c >= 0x80; }
static int isidstart(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c >= 0x80; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void skipws(P *p) {
    for (;;) {
        int c = peekc(p);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') { p->i++; continue; }
        if (c == '#') {
            if (p->i + 1 < p->n && p->s[p->i + 1] == '`') {
                int j = p->i + 2;
                char o = j < p->n ? p->s[j] : 0, cl = o == '(' ? ')' : o == '[' ? ']' : o == '{' ? '}' : o == '<' ? '>' : 0;
                int d = 0;
                if (cl) { while (j < p->n) { if (p->s[j] == o) d++; else if (p->s[j] == cl && --d == 0) { j++; break; } j++; } p->i = j; continue; }
            }
            while (p->i < p->n && p->s[p->i] != '\n') p->i++;
            continue;
        }
        if ((unsigned char) c == 0xC2 && p->i + 1 < p->n && (unsigned char) p->s[p->i + 1] == 0xA0) { p->i += 2; continue; }
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_alt(P *p);
static RxNode *p_seq(P *p);
static int hexv(int c) { return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p_number(P *p, unsigned *out, int base) {
    unsigned v = 0;
    int any = 0;
    while (p->i < p->n) { int d = hexv((unsigned char) p->s[p->i]); if (d < 0 || d >= base) break; v = v * (unsigned) base + (unsigned) d; p->i++; any = 1; }
    *out = v;
    return any;
}
static int p_escape(P *p, unsigned *cp, int *cls, int *neg) {
    int c = peekc(p);
    p->esc_more = 0;
    p->i++;
    *neg = 0;
    *cls = 0;
    int up = c >= 'A' && c <= 'Z';
    switch (c) {
        case 'd':
        case 'D':
        *cls = CL_DIGIT;
        *neg = up;
        return 1;
        case 'w':
        case 'W':
        *cls = CL_WORD;
        *neg = up;
        return 1;
        case 's':
        case 'S':
        *cls = CL_SPACE;
        *neg = up;
        return 1;
        case 'h':
        case 'H':
        *cls = CL_HSPACE;
        *neg = up;
        return 1;
        case 'v':
        case 'V':
        *cls = CL_VSPACE;
        *neg = up;
        return 1;
        case 'N':
        *cls = CL_NL;
        *neg = 1;
        return 1;
        case 'n':
        *cls = CL_NL;
        return 1;
        case 't':
        case 'T':
        *cp = 9;
        *neg = up;
        return 0;
        case 'r':
        case 'R':
        *cp = 13;
        *neg = up;
        return 0;
        case 'f':
        case 'F':
        *cp = 12;
        *neg = up;
        return 0;
        case 'e':
        case 'E':
        *cp = 27;
        *neg = up;
        return 0;
        case '0':
        *cp = 0;
        return 0;
        case 'x':
        case 'X':
        case 'o':
        case 'O':
        {
            int base = (c == 'x' || c == 'X') ? 16 : 8;
            if (peekc(p) == '[') {
                p->i++;
                p->esc_more = p->i;
                unsigned v = 0;
                p_number(p, &v, base);
                *cp = v;
                while (peekc(p) && peekc(p) != ']') p->i++;
                p->esc_end = p->i;
                if (peekc(p) == ']') p->i++;
            } else {
                unsigned v = 0;
                if (!p_number(p, &v, base)) { perr(p, "Missing hexadecimal digits after \\x"); return 0; }
                *cp = v;
            }
            *neg = c == 'X' || c == 'O';
            return 0;
        }
        case 'c':
        case 'C':
        {
            unsigned v = 0;
            if (peekc(p) == '[') { p->i++; p_number(p, &v, 10); while (peekc(p) && peekc(p) != ']') p->i++; if (peekc(p) == ']') p->i++; } else p_number(p, &v, 10);
            *cp = v;
            *neg = c == 'C';
            return 0;
        }
        default:
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '1' && c <= '9')) { perr(p, "Unrecognized backslash sequence"); *cp = (unsigned) c; return 0; }
        *cp = (unsigned) c;
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_quoted(P *p, int fl) {
    int q = peekc(p);
    p->i++;
    char *b = (char *) RX_ALLOC((size_t) (p->n - p->i) + 2);
    int k = 0;
    while (p->i < p->n && p->s[p->i] != q) {
        if (p->s[p->i] == '\\' && p->i + 1 < p->n) {
            int d = (unsigned char) p->s[p->i + 1];
            if (d == q || d == '\\') { b[k++] = (char) d; p->i += 2; continue; }
            if (q == '"') {
                if (d == 'n') { b[k++] = '\n'; p->i += 2; continue; }
                if (d == 't') { b[k++] = '\t'; p->i += 2; continue; }
                if (d == 'r') { b[k++] = '\r'; p->i += 2; continue; }
                if (d == '0') { b[k++] = 0; p->i += 2; continue; }
                if (d == 'e') { b[k++] = 27; p->i += 2; continue; }
                if (d == 'x') { p->i += 2; unsigned v = 0; if (peekc(p) == '[') p->i++; p_number(p, &v, 16); if (peekc(p) == ']') p->i++; k += rx_putcp(v, b + k); continue; }
            }
        }
        b[k++] = p->s[p->i++];
    }
    if (p->i < p->n) p->i++;
    else perr(p, "Couldn't find terminator for quote");
    RxNode *x = nlit(b, k, fl);
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p_cls_name(const char *nm, int n) {
    static const struct {
        const char *n;
        int c;
    } t[] = { { "alpha", CL_ALPHA }, { "digit", CL_DIGIT }, { "alnum", CL_ALNUM }, { "upper", CL_UPPER }, { "lower", CL_LOWER }, { "space", CL_SPACE }, { "blank", CL_BLANK }, { "punct", CL_PUNCT },
        { "xdigit", CL_XDIGIT }, { "cntrl", CL_CNTRL }, { "print", CL_PRINT }, { "graph", CL_GRAPH }, { "word", CL_WORD }, { "ascii", CL_ASCII }, { 0, 0 } };
    for (int k = 0; t[k].n; k++) if ((int) strlen(t[k].n) == n && !strncmp(t[k].n, nm, (size_t) n)) return t[k].c;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p_setitem_bracket(P *p, RxSetItem *it) {
    int cap = 8, nr = 0;
    unsigned *rng = (unsigned *) RX_ALLOC(sizeof(unsigned) * 2 * (size_t) cap);
    int extra_n = 0, extra_cap = 0;
    RxSetItem *extra = NULL;
    for (;;) {
        while (peekc(p) == ' ' || peekc(p) == '\t' || peekc(p) == '\n') p->i++;
        int c = peekc(p);
        if (p->i >= p->n) { perr(p, "Unable to parse character class; couldn't find final ']'"); return 0; }
        if (c == ']') { p->i++; break; }
        unsigned a;
        if (c == '\\') {
            p->i++;
            unsigned cp = 0;
            int cls = 0, neg = 0;
            int isc = p_escape(p, &cp, &cls, &neg);
            if (isc) {
                if (extra_n == extra_cap) {
                    extra_cap = extra_cap ? extra_cap * 2 : 4;
                    extra = (RxSetItem *) (extra ? RX_REALLOC(extra, sizeof *extra * (size_t) extra_cap) : RX_ALLOC(sizeof *extra * (size_t) extra_cap));
                }
                memset(&extra[extra_n], 0, sizeof *extra);
                extra[extra_n].op = '+';
                extra[extra_n].kind = 'C';
                extra[extra_n].cls = cls;
                extra[extra_n].neg = neg;
                extra_n++;
                continue;
            }
            a = cp;
        } else {
            int l;
            a = rx_cp((const unsigned char *) p->s, p->n, p->i, &l);
            p->i += l;
        }
        unsigned b = a;
        {
            int j = p->i;
            while (j < p->n && p->s[j] == ' ') j++;
            if (j < p->n && p->s[j] == '-') {
                int k = j + 1;
                while (k < p->n && p->s[k] == ' ') k++;
                if (k < p->n && p->s[k] != ']') { perr(p, "Unsupported use of - as character range; in Raku please use .."); return 0; }
            }
        }
        int save = p->i;
        while (peekc(p) == ' ') p->i++;
        if (peekc(p) == '.' && p->i + 1 < p->n && p->s[p->i + 1] == '.') {
            p->i += 2;
            while (peekc(p) == ' ') p->i++;
            if (peekc(p) == '\\') {
                p->i++;
                unsigned cp = 0;
                int cls = 0, neg = 0;
                p_escape(p, &cp, &cls, &neg);
                b = cp;
            } else {
                int l;
                b = rx_cp((const unsigned char *) p->s, p->n, p->i, &l);
                p->i += l;
            }
            if (b < a) { perr(p, "Illegal reversed character range in regex"); return 0; }
        } else p->i = save;
        if (nr == cap) { cap *= 2; rng = (unsigned *) RX_REALLOC(rng, sizeof(unsigned) * 2 * (size_t) cap); }
        rng[2 * nr] = a;
        rng[2 * nr + 1] = b;
        nr++;
    }
    it->kind = 'R';
    it->rng = rng;
    it->nrng = nr;
    if (extra_n) { it->plen = extra_n; it->prop = (const char *) extra; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_set(P *p, int firstop) {
    RxNode *x = nn(N_SET);
    x->fl = (unsigned char) (p->flags & (RXF_I | RXF_M));
    x->set = (RxSet *) RX_ALLOC(sizeof(RxSet));
    int cap = 4;
    x->set->it = (RxSetItem *) RX_ALLOC(sizeof(RxSetItem) * (size_t) cap);
    int op = firstop;
    for (;;) {
        while (peekc(p) == ' ') p->i++;
        int c = peekc(p);
        if (c == '>' ) { p->i++; break; }
        if (c == '+') { op = '+'; p->i++; continue; }
        if (c == '-') { op = '-'; p->i++; continue; }
        if (p->i >= p->n) { perr(p, "Unable to parse character class; couldn't find final '>'"); return x; }
        if (x->set->n + 8 >= cap) { cap *= 2; x->set->it = (RxSetItem *) RX_REALLOC(x->set->it, sizeof(RxSetItem) * (size_t) cap); }
        RxSetItem *it = &x->set->it[x->set->n];
        memset(it, 0, sizeof *it);
        it->op = (char) op;
        if (c == '[') {
            p->i++;
            if (!p_setitem_bracket(p, it)) return x;
            int en = it->plen;
            RxSetItem *ex = (RxSetItem *) it->prop;
            it->plen = 0;
            it->prop = NULL;
            x->set->n++;
            for (int k = 0; k < en; k++) { x->set->it[x->set->n] = ex[k]; x->set->it[x->set->n].op = (char) op; x->set->n++; }
            if (en && op == '-') { } else if (en) { }
        } else if (c == ':') {
            p->i++;
            int ng = 0;
            if (peekc(p) == '!') { ng = 1; p->i++; }
            int st = p->i;
            while (p->i < p->n && (isw(peekc(p)) || peekc(p) == '=')) p->i++;
            if (peekc(p) == '<' || peekc(p) == '(') {
                int open = peekc(p), close = open == '<' ? '>' : ')', d = 0;
                while (p->i < p->n) { if (peekc(p) == open) d++; else if (peekc(p) == close && --d == 0) { p->i++; break; } p->i++; }
            }
            it->kind = 'P';
            it->prop = p->s + st;
            it->plen = p->i - st;
            it->neg = ng;
            x->set->n++;
        } else if (isidstart(c)) {
            int st = p->i;
            while (p->i < p->n && isw(peekc(p))) p->i++;
            int cls = p_cls_name(p->s + st, p->i - st);
            if (!cls) { perr(p, "Unrecognized character class name"); return x; }
            it->kind = 'C';
            it->cls = cls;
            x->set->n++;
        } else if (c == '.') {
            p->i++;
        } else {
            perr(p, "Unable to parse character class");
            return x;
        }
        op = '+';
    }
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void caplist_add(CapList *l, RxCap *c) {
    if (l->n == l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->v = (RxCap **) (l->v ? RX_REALLOC(l->v, sizeof(RxCap *) * (size_t) l->cap) : RX_ALLOC(sizeof(RxCap *) * (size_t) l->cap)); }
    l->v[l->n++] = c;
}
static RxCap *mkcap(P *p, char kind, int idx, const char *name, int nlen) {
    RxCap *c = (RxCap *) RX_ALLOC(sizeof *c);
    c->kind = kind;
    c->idx = idx;
    c->name = name;
    c->nlen = nlen;
    if (p->cur) caplist_add(p->cur, c);
    return c;
}
static void mark_rep(RxNode *n) {
    if (!n) return;
    if (n->cap && n->cap != &rx_silent_cap) { ((RxCap *) n->cap)->rep = 1; if (n->kind == N_GROUP) return; }
    if (n->kind == N_GROUP) return;
    for (int k = 0; k < n->nch; k++) mark_rep(n->ch[k]);
    if (n->sep) mark_rep(n->sep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_regex_until(P *p, int close) { RxNode *r = p_alt(p); (void) close; return r; }
static int p_ident(P *p, int *st) {
    int s0 = p->i;
    if (!isidstart(peekc(p))) return 0;
    while (p->i < p->n &&
        (isw(peekc(p)) || ((peekc(p) == '-' || peekc(p) == '\'') && p->i + 1 < p->n && isidstart((unsigned char) p->s[p->i + 1])) ||
        (peekc(p) == ':' && p->i + 2 < p->n && p->s[p->i + 1] == ':' && isidstart((unsigned char) p->s[p->i + 2])))) {
        if (peekc(p) == ':') p->i += 2;
        else p->i++;
    }
    *st = s0;
    return p->i - s0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_angle(P *p) {
    int c = peekc(p);
    int fl = p->flags;
    if (c == '(') { p->i++; p->nmark++; return nn(N_MARK); }
    if (c == ' ' || c == '\t' || c == '\n') {
        RxNode *alt = nn(N_ALT);
        alt->ltm = 1;
        for (;;) {
            skipws(p);
            if (p->i >= p->n) { perr(p, "Unable to parse regex; couldn't find final '>'"); return alt; }
            if (peekc(p) == '>') { p->i++; break; }
            int st = p->i;
            while (p->i < p->n && !isspace(peekc(p)) && peekc(p) != '>') p->i++;
            nadd(alt, nlit(p->s + st, p->i - st, fl & (RXF_I | RXF_M)));
        }
        if (alt->nch == 1) return alt->ch[0];
        return alt;
    }
    if (c == '[') return p_set(p, '+');
    if (c == '+' || c == '-') { int op = c; p->i++; while (peekc(p) == ' ') p->i++; return p_set(p, op); }
    if (c == ':') { RxNode *x = p_set(p, '+'); return x; }
    if (c == '{') {
        int d = 0, st = p->i;
        while (p->i < p->n) { if (p->s[p->i] == '{') d++; else if (p->s[p->i] == '}' && --d == 0) { p->i++; break; } p->i++; }
        RxNode *x = nn(N_CODE);
        x->s = p->s + st;
        x->slen = p->i - st;
        x->neg = 2;
        if (peekc(p) == '>') p->i++;
        else perr(p, "Missing > after <{...}");
        return x;
    }
    int pred = 0;
    if (c == '?' || c == '!') {
        pred = c;
        p->i++;
        c = peekc(p);
        if (c == '>') { p->i++; RxNode *x = nn(pred == '?' ? N_NULL : N_FAIL); return x; }
        if (c == '{') {
            int d = 0, st = p->i;
            while (p->i < p->n) { if (p->s[p->i] == '{') d++; else if (p->s[p->i] == '}' && --d == 0) { p->i++; break; } p->i++; }
            RxNode *x = nn(N_CODE);
            x->s = p->s + st;
            x->slen = p->i - st;
            x->neg = pred == '!' ? 1 : 0;
            if (peekc(p) == '>') p->i++;
            else perr(p, "Missing > after <?{...}");
            return x;
        }
        if (c == '?' || c == '!') { p->i++; if (peekc(p) == '>') p->i++; return nn(pred == '?' ? N_NULL : N_FAIL); }
        if (c == '[' || c == ':' || c == '+' || c == '-') {
            RxNode *set;
            if (c == '+' || c == '-') { int op = c; p->i++; while (peekc(p) == ' ') p->i++; set = p_set(p, op); } else set = p_set(p, '+');
            RxNode *lk = nn(N_LOOK);
            lk->neg = pred == '!';
            lk->fl = (unsigned char) fl;
            nadd(lk, set);
            return lk;
        }
    }
    int dot = 0, amp = 0;
    if (c == '.') {
        dot = 1;
        p->i++;
        if (peekc(p) == '.' && p->i + 1 < p->n && p->s[p->i + 1] == '.') { p->i += 2; if (peekc(p) == '>') p->i++; return nn(N_FAIL); }
    } else if (c == '&') {
        amp = 1;
        p->i++;
    } else if (c == '|') {
        p->i++;
        if (peekc(p) == 'w') { p->i++; if (peekc(p) == '>') p->i++; return nn(N_WB); }
        perr(p, "Unrecognized regex assertion");
        return nn(N_FAIL);
    }
    int st;
    int nl = p_ident(p, &st);
    if (!nl) {
        if (peekc(p) == '$' || peekc(p) == '@') { perr(p, "variable interpolation inside <...> is resolved before the engine"); return nn(N_FAIL); }
        perr(p, "Unrecognized regex assertion");
        return nn(N_FAIL);
    }
    const char *nm = p->s + st;
    const char *alias = NULL;
    int alen = 0;
    if (peekc(p) == '=') {
        alias = nm;
        alen = nl;
        p->i++;
        if (peekc(p) == '.') { dot = 1; p->i++; } else if (peekc(p) == '&') { amp = 1; p->i++; }
        nl = p_ident(p, &st);
        if (!nl) { perr(p, "Unrecognized regex assertion"); return nn(N_FAIL); }
        nm = p->s + st;
    }
    RxNode *x;
    if (nl == 6 && !strncmp(nm, "before", 6) && !alias) {
        x = nn(N_LOOK);
        x->neg = pred == '!';
        x->behind = 0;
    } else if (nl == 5 && !strncmp(nm, "after", 5) && !alias) {
        x = nn(N_LOOK);
        x->neg = pred == '!';
        x->behind = 1;
    } else x = NULL;
    if (x) {
        if (peekc(p) != '>') {
            if (peekc(p) == ' ' || peekc(p) == ':') { if (peekc(p) == ':') p->i++; }
            p->angle++;
            RxNode *in = p_regex_until(p, '>');
            p->angle--;
            nadd(x, in);
        } else nadd(x, nn(N_NULL));
        if (peekc(p) == '>') p->i++;
        else perr(p, "Unable to parse regex; couldn't find final '>'");
        x->fl = (unsigned char) fl;
        return x;
    }
    if (!pred && !amp && nl == 2 && !strncmp(nm, "ws", 2) && !alias && peekc(p) == '>') { p->i++; RxNode *w = nn(N_WS); if (!dot) w->cap = mkcap(p, 'n', 0, nm, 2); return w; }
    if (nl == 2 && !strncmp(nm, "wb", 2) && peekc(p) == '>') { p->i++; x = nn(N_WB); if (!dot && !pred) x->cap = mkcap(p, 'n', 0, nm, 2); x->neg = pred == '!'; return x; }
    if (nl == 2 && !strncmp(nm, "ww", 2) && peekc(p) == '>') { p->i++; x = nn(pred == '!' ? N_NWW : N_WW); if (!dot && !pred) x->cap = mkcap(p, 'n', 0, nm, 2); return x; }
    if (nl == 4 && !strncmp(nm, "null", 4) && peekc(p) == '>') { p->i++; return nn(N_NULL); }
    x = nn(N_RULE);
    x->s = nm;
    x->slen = nl;
    x->fl = (unsigned char) fl;
    x->neg = pred == '!';
    x->behind = pred == '?' || pred == '!';
    int cl = p_cls_name(nm, nl);
    if (cl) { x->min = cl; }
    if (peekc(p) == '(' ) {
        int d = 0;
        while (p->i < p->n) { if (p->s[p->i] == '(') d++; else if (p->s[p->i] == ')' && --d == 0) { p->i++; break; } p->i++; }
    } else if (peekc(p) == ':' || peekc(p) == ' ') {
        while (p->i < p->n && peekc(p) != '>') p->i++;
    }
    if (peekc(p) == '>') p->i++;
    else perr(p, "Unable to parse regex; couldn't find final '>'");
    if (!dot && !pred && !(amp && !alias)) {
        x->cap = alias ? mkcap(p, 'n', 0, alias, alen) : mkcap(p, 'n', 0, nm, nl);
        if (alias) { ((RxCap *) x->cap)->name2 = nm; ((RxCap *) x->cap)->nlen2 = nl; }
    } else if (alias) x->cap = mkcap(p, 'n', 0, alias, alen);
    else x->silent = 1;
    if (amp) x->trail = 1;
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_atom(P *p);
static RxNode *p_quant(P *p, RxNode *a);
static RxNode *p_alias(P *p, char kind, int idx, const char *name, int nlen) {
    skipws(p);
    int save_pidx = p->pidx;
    if (kind == 'p') p->pidx = idx;
    RxNode *a = p_atom(p);
    if (!a) { perr(p, "Missing atom after capture alias"); return nn(N_NULL); }
    a = p_quant(p, a);
    RxNode *base = a->kind == N_QUANT ? a->ch[0] : a;
    if (base->kind == N_GROUP && base->cap && base->cap->kind == 'p') {
        RxCap *cap = (RxCap *) base->cap;
        cap->kind = kind;
        cap->idx = idx;
        cap->name = name;
        cap->nlen = nlen;
        if (p->force_rep) cap->rep = 1;
        p->pidx = kind == 'p' ? idx + 1 : save_pidx;
        return a;
    }
    if (kind == 'n' && base->kind == N_RULE && base->cap) { RxCap *cap = (RxCap *) base->cap; cap->name2 = cap->name; cap->nlen2 = cap->nlen; cap->name = name; cap->nlen = nlen; return a; }
    RxNode *sp = nn(N_SPAN);
    sp->cap = mkcap(p, kind, idx, name, nlen);
    if (p->force_rep) ((RxCap *) sp->cap)->rep = 1;
    nadd(sp, a);
    p->pidx = kind == 'p' ? idx + 1 : save_pidx;
    return sp;
}
static int p_closure_bounds(P *p, int *mn, int *mx) {
    int d = 0, st = p->i;
    while (p->i < p->n) { if (p->s[p->i] == '{') d++; else if (p->s[p->i] == '}' && --d == 0) { p->i++; break; } p->i++; }
    char *t = (char *) RX_ALLOC((size_t) (p->i - st) + 1);
    memcpy(t, p->s + st + 1, (size_t) (p->i - st - 2));
    const char *q = strstr(t, "..");
    *mn = atoi(t);
    *mx = *mn;
    if (q) { const char *r = q + 2; if (*r == '^') r++; *mx = *r == '*' ? -1 : atoi(r) - (q[2] == '^' ? 1 : 0); }
    return 1;
}
static RxNode *p_quant(P *p, RxNode *a) {
    int save0 = p->i, mn = 0, mx = 0, pre = 0;
    skipws(p);
    int c = peekc(p), d = p->i + 1 < p->n ? (unsigned char) p->s[p->i + 1] : 0;
    if (c == '*' && d != '*') {
        mn = 0;
        mx = -1;
        p->i++;
    } else if (c == '+') {
        mn = 1;
        mx = -1;
        p->i++;
    } else if (c == '?') {
        mn = 0;
        mx = 1;
        p->i++;
    } else if (c == '*' && d == '*') {
        unsigned v = 0;
        p->i += 2;
        skipws(p);
        if (peekc(p) == '?' || peekc(p) == '!' || peekc(p) == ':') { pre = peekc(p); p->i++; skipws(p); }
        if (peekc(p) == '^') { p->i++; unsigned w = 0; p_number(p, &w, 10); mn = 0; mx = (int) w - 1; } else if (peekc(p) == '{') p_closure_bounds(p, &mn, &mx);
        else if (isw(peekc(p)) && p_number(p, &v, 10)) {
            mn = (int) v;
            mx = mn;
            if (peekc(p) == '.' && p->i + 1 < p->n && p->s[p->i + 1] == '.') {
                p->i += 2;
                if (peekc(p) == '*') {
                    mx = -1;
                    p->i++;
                } else if (peekc(p) == '^') {
                    p->i++;
                    unsigned w = 0;
                    p_number(p, &w, 10);
                    mx = (int) w - 1;
                } else {
                    unsigned w = 0;
                    p_number(p, &w, 10);
                    mx = (int) w;
                }
            }
        } else {
            perr(p, "Unrecognized quantifier after **");
            return a;
        }
    } else {
        p->i = save0;
        return a;
    }
    RxNode *q = nn(N_QUANT);
    q->min = mn;
    q->max = mx;
    nadd(q, a);
    q->ratchet = (p->flags & RXF_R) ? 1 : 0;
    int m = pre ? pre : peekc(p);
    if (m == '?') {
        q->lazy = 1;
        q->ratchet = 0;
        if (!pre) p->i++;
    } else if (m == '!') {
        q->ratchet = 0;
        if (!pre) p->i++;
    } else if (m == ':' && (pre || !(p->i + 1 < p->n && p->s[p->i + 1] == ':'))) {
        q->ratchet = 1;
        if (!pre) p->i++;
    }
    if (mx < 0 || mx > 1) mark_rep(a);
    int save = p->i;
    skipws(p);
    if (peekc(p) == '%') {
        int tr = 0;
        p->i++;
        if (peekc(p) == '%') { tr = 1; p->i++; }
        skipws(p);
        RxNode *s = p_atom(p);
        if (s) { q->sep = s; q->trail = (unsigned char) tr; mark_rep(a); } else perr(p, "Missing separator after %");
    } else p->i = save;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void p_modifier(P *p) {
    p->i++;
    int neg = 0;
    if (peekc(p) == '!') { neg = 1; p->i++; }
    int st = p->i;
    while (p->i < p->n && isw(peekc(p))) p->i++;
    int nl = p->i - st;
    const char *nm = p->s + st;
    int bit = 0;
    if ((nl == 1 && nm[0] == 'i') || (nl == 2 && !strncmp(nm, "ii", 2)) || (nl == 10 && !strncmp(nm, "ignorecase", 10))) bit = RXF_I;
    else if ((nl == 1 && nm[0] == 'm') || (nl == 2 && !strncmp(nm, "mm", 2)) || (nl == 10 && !strncmp(nm, "ignoremark", 10))) bit = RXF_M;
    else if ((nl == 1 && nm[0] == 's') || (nl == 2 && !strncmp(nm, "ss", 2)) || (nl == 8 && !strncmp(nm, "sigspace", 8))) bit = RXF_S;
    else if ((nl == 1 && nm[0] == 'r') || (nl == 7 && !strncmp(nm, "ratchet", 7))) bit = RXF_R;
    else if ((nl == 2 && !strncmp(nm, "P5", 2)) || (nl == 5 && !strncmp(nm, "Perl5", 5))) { perr(p, "Perl 5 regex syntax is not supported"); return; }
    if (peekc(p) == '(') {
        int d = 0, st2 = p->i;
        while (p->i < p->n) { if (p->s[p->i] == '(') d++; else if (p->s[p->i] == ')' && --d == 0) { p->i++; break; } p->i++; }
        if (p->s[st2 + 1] == '0' ) neg = !neg;
    }
    if (bit) { if (neg) p->flags &= ~bit; else p->flags |= bit; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_group(P *p, int capture) {
    int save_flags = p->flags;
    int idx = -1, save_pidx = p->pidx;
    RxCap *cap = NULL;
    CapList kl = { 0, 0, 0 }, *save_cur = p->cur;
    if (capture) { idx = p->pidx; cap = mkcap(p, 'p', idx, NULL, 0); p->pidx = 0; p->cur = &kl; }
    RxNode *in = p_alt(p);
    if (peekc(p) != (capture ? ')' : ']')) perr(p, capture ? "Unable to parse regex; couldn't find final ')'" : "Unable to parse regex; couldn't find final ']'");
    else p->i++;
    p->flags = save_flags;
    if (capture) { p->pidx = save_pidx + 1; p->cur = save_cur; cap->kids = kl.v; cap->nkids = kl.n; }
    if (!capture) return in;
    RxNode *g = nn(N_GROUP);
    g->cap = cap;
    nadd(g, in);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RxNode *p_atom(P *p) {
    int fl = p->flags & (RXF_I | RXF_M);
    int c = peekc(p);
    if (p->i >= p->n) return NULL;
    if (c == '\'' || c == '"') return p_quoted(p, fl);
    if (c == '\\') {
        p->i++;
        if (p->i >= p->n) { perr(p, "Backslash at end of regex"); return NULL; }
        unsigned cp = 0;
        int cls = 0, neg = 0;
        int isc = p_escape(p, &cp, &cls, &neg);
        if (isc && cls == CL_NL && !neg) return nn(N_NLN);
        if (isc) return ncls(cls, neg);
        if (neg) return ncp(cp, 1, fl);
        if (p->esc_more) {
            char *b = (char *) RX_ALLOC((size_t) (p->esc_end - p->esc_more) * 4 + 8);
            int k = 0, j = p->esc_more;
            while (j < p->esc_end) {
                while (j < p->esc_end && (p->s[j] == ',' || p->s[j] == ' ')) j++;
                unsigned v = 0;
                int any = 0;
                while (j < p->esc_end && hexv((unsigned char) p->s[j]) >= 0) { v = v * 16 + (unsigned) hexv((unsigned char) p->s[j]); j++; any = 1; }
                if (!any) break;
                k += rx_putcp(v, b + k);
            }
            return nlit(b, k, fl);
        }
        char b[8];
        int l = rx_putcp(cp, b);
        return nlit(b, l, fl);
    }
    if (c == '.') { p->i++; RxNode *x = nn(N_ANY); return x; }
    if (c == '^') { p->i++; if (peekc(p) == '^') { p->i++; return nn(N_BOL); } return nn(N_BOS); }
    if (c == '$') {
        p->i++;
        if (peekc(p) == '$') { p->i++; return nn(N_EOL); }
        if (peekc(p) == '<' || (peekc(p) >= '0' && peekc(p) <= '9')) {
            int st = p->i, nm = 0, nlen = 0, num = -1;
            if (peekc(p) == '<') {
                p->i++;
                nm = p->i;
                while (p->i < p->n && peekc(p) != '>') p->i++;
                nlen = p->i - nm;
                if (peekc(p) == '>') p->i++;
            } else {
                unsigned v = 0;
                p_number(p, &v, 10);
                num = (int) v;
            }
            int after = p->i;
            while (peekc(p) == ' ') p->i++;
            if (peekc(p) == '=' && !(p->i + 1 < p->n && (p->s[p->i + 1] == '=' || p->s[p->i + 1] == '~'))) { p->i++; return p_alias(p, num >= 0 ? 'p' : 'n', num >= 0 ? num : 0, p->s + nm, nlen); }
            p->i = after;
            (void) st;
            RxNode *x = nn(N_BACKREF);
            if (num < 0) { x->s = p->s + nm; x->slen = nlen; x->min = -1; } else x->min = num;
            x->fl = (unsigned char) fl;
            return x;
        }
        return nn(N_EOS);
    }
    if ((c == '@' || c == '%') && p->i + 1 < p->n && p->s[p->i + 1] == '<') {
        int st = p->i + 2, j = st;
        while (j < p->n && p->s[j] != '>') j++;
        if (j < p->n) {
            int k = j + 1;
            while (k < p->n && p->s[k] == ' ') k++;
            if (k < p->n && p->s[k] == '=') { p->i = k + 1; p->force_rep = 1; RxNode *r = p_alias(p, 'n', 0, p->s + st, j - st); p->force_rep = 0; return r; }
        }
    }
    if (c == '<') { if (p->i + 1 < p->n && p->s[p->i + 1] == '<') { p->i += 2; return nn(N_LWB); } p->i++; return p_angle(p); }
    if (c == '>') {
        if (p->angle > 0) return NULL;
        if (p->i + 1 < p->n && p->s[p->i + 1] == '>') { p->i += 2; return nn(N_RWB); }
        perr(p, "Unrecognized regex metacharacter > (must be quoted to match literally)");
        return NULL;
    }
    if (c == 0xC2 && p->i + 1 < p->n && (unsigned char) p->s[p->i + 1] == 0xAB) { p->i += 2; return nn(N_LWB); }
    if (c == 0xC2 && p->i + 1 < p->n && (unsigned char) p->s[p->i + 1] == 0xBB) { p->i += 2; return nn(N_RWB); }
    if (c == '(') { p->i++; return p_group(p, 1); }
    if (c == '[') { p->i++; return p_group(p, 0); }
    if (c == '{') {
        int d = 0, st = p->i;
        while (p->i < p->n) { if (p->s[p->i] == '{') d++; else if (p->s[p->i] == '}' && --d == 0) { p->i++; break; } p->i++; }
        RxNode *x = nn(N_CODE);
        x->s = p->s + st;
        x->slen = p->i - st;
        x->neg = 3;
        return x;
    }
    if (c == ')' && p->nmark > 0 && p->i + 1 < p->n && p->s[p->i + 1] == '>') { p->i += 2; p->nmark--; RxNode *m = nn(N_MARK); m->neg = 1; return m; }
    if (c == ')' || c == ']' || c == '|' || c == '&') return NULL;
    if (c == ':') {
        if (p->i + 1 < p->n && (isidstart((unsigned char) p->s[p->i + 1]) || p->s[p->i + 1] == '!')) { p_modifier(p); return nn(N_NULL); }
        p->i++;
        while (peekc(p) == ':') p->i++;
        return nn(N_NULL);
    }
    if (c == '#') { skipws(p); return nn(N_NULL); }
    if (c == '*' || c == '+' || c == '?') { perr(p, "Quantifier quantifies nothing"); return NULL; }
    if (c == '%') { perr(p, "Unrecognized regex metacharacter % (must be quoted to match literally)"); return NULL; }
    if (c == '~') return NULL;
    if (isw(c)) { int l; rx_cp((const unsigned char *) p->s, p->n, p->i, &l); RxNode *x = nlit(p->s + p->i, l, fl); p->i += l; return x; }
    {
        char m[96];
        snprintf(m, sizeof m, "Unrecognized regex metacharacter %c (must be quoted to match literally)", c);
        perr(p, m[0] ? (const char *) strcpy((char *) RX_ALLOC(strlen(m) + 1), m) : "Unrecognized regex metacharacter");
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void seq_push(RxNode *s, RxNode *x) {
    if (x->kind == N_LIT && s->nch > 0) {
        RxNode *l = s->ch[s->nch - 1];
        if (l->kind == N_LIT && l->fl == x->fl) {
            char *b = (char *) RX_ALLOC((size_t) (l->slen + x->slen) + 1);
            memcpy(b, l->s, (size_t) l->slen);
            memcpy(b + l->slen, x->s, (size_t) x->slen);
            l->s = b;
            l->slen += x->slen;
            return;
        }
    }
    nadd(s, x);
}
static RxNode *p_seq(P *p) {
    RxNode *s = nn(N_SEQ);
    int atstart = 1;
    for (;;) {
        int had_ws = 0;
        { int before = p->i; skipws(p); had_ws = p->i > before; }
        if ((p->flags & RXF_S) && had_ws && !atstart) { RxNode *w = nn(N_WS); nadd(s, w); }
        if (p->i >= p->n || p->err) break;
        RxNode *a = p_atom(p);
        if (!a) {
            if (peekc(p) == '~') {
                p->i++;
                skipws(p);
                RxNode *goal = p_atom(p);
                if (!goal) { perr(p, "Missing goal after ~"); break; }
                goal = p_quant(p, goal);
                skipws(p);
                RxNode *inner = p_atom(p);
                if (!inner) { perr(p, "Missing inner atom after ~ goal"); break; }
                inner = p_quant(p, inner);
                nadd(s, inner);
                nadd(s, goal);
                atstart = 0;
                continue;
            }
            break;
        }
        a = p_quant(p, a);
        seq_push(s, a);
        atstart = 0;
    }
    if ((p->flags & RXF_S) && s->nch > 0 && 0) { }
    if (s->nch == 1) return s->ch[0];
    return s;
}
static int amp1_at(P *p) { return peekc(p) == '&' && !(p->i + 1 < p->n && p->s[p->i + 1] == '&'); }
static int amp2_at(P *p) { return peekc(p) == '&' && p->i + 1 < p->n && p->s[p->i + 1] == '&'; }
static RxNode *p_conj1(P *p) {
    RxNode *first = p_seq(p);
    skipws(p);
    if (!amp1_at(p)) return first;
    RxNode *cj = nn(N_CONJ);
    nadd(cj, first);
    while (amp1_at(p)) { p->i++; nadd(cj, p_seq(p)); skipws(p); }
    return cj;
}
static RxNode *p_conj(P *p, int dbl) {
    (void) dbl;
    RxNode *first = p_conj1(p);
    skipws(p);
    if (!amp2_at(p)) return first;
    RxNode *cj = nn(N_CONJ);
    nadd(cj, first);
    while (amp2_at(p)) { p->i += 2; nadd(cj, p_conj1(p)); skipws(p); }
    return cj;
}
static RxNode *p_ltm(P *p) {
    int base = p->pidx, mx = base;
    if (peekc(p) == ' ') skipws(p);
    int lead = peekc(p) == '|' && !(p->i + 1 < p->n && p->s[p->i + 1] == '|');
    if (lead) p->i++;
    RxNode *first;
    p->pidx = base;
    { RxNode *a = p_conj(p, 0); if (p->pidx > mx) mx = p->pidx; first = a; }
    skipws(p);
    if (peekc(p) == '|' && !(p->i + 1 < p->n && p->s[p->i + 1] == '|')) {
        RxNode *alt = nn(N_ALT);
        alt->ltm = 1;
        nadd(alt, first);
        while (peekc(p) == '|' && !(p->i + 1 < p->n && p->s[p->i + 1] == '|')) { p->i++; p->pidx = base; RxNode *b = p_conj(p, 0); if (p->pidx > mx) mx = p->pidx; nadd(alt, b); skipws(p); }
        p->pidx = mx;
        return alt;
    }
    p->pidx = mx;
    return first;
}
static RxNode *p_alt(P *p) {
    int base = p->pidx, mx = base;
    skipws(p);
    int lead = peekc(p) == '|' && p->i + 1 < p->n && p->s[p->i + 1] == '|';
    if (lead) p->i += 2;
    RxNode *first = p_ltm(p);
    if (p->pidx > mx) mx = p->pidx;
    skipws(p);
    if (peekc(p) == '|' && p->i + 1 < p->n && p->s[p->i + 1] == '|') {
        RxNode *alt = nn(N_ALT);
        nadd(alt, first);
        while (peekc(p) == '|' && p->i + 1 < p->n && p->s[p->i + 1] == '|') { p->i += 2; p->pidx = base; RxNode *b = p_ltm(p); if (p->pidx > mx) mx = p->pidx; nadd(alt, b); skipws(p); }
        p->pidx = mx;
        return alt;
    }
    p->pidx = mx;
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_ratchet(RxNode *n) { if (!n) return; for (int k = 0; k < n->nch; k++) set_ratchet(n->ch[k]); if (n->sep) set_ratchet(n->sep); if (n->kind == N_ALT) n->ratchet = 1; }
RxProg *rx_compile(const char *src, int flags, const RxEnv *env, const char **err) {
    P p;
    memset(&p, 0, sizeof p);
    p.s = src;
    p.n = (int) strlen(src);
    p.flags = flags;
    p.env = env;
    RxProg *pr = (RxProg *) RX_ALLOC(sizeof *pr);
    pr->flags = flags;
    pr->env = env;
    p.cur = &pr->tops;
    pr->root = p_alt(&p);
    if (!p.err && p.i < p.n) {
        if (src[p.i] == ')') p.err = "Unmatched ) in regex; couldn't find proper opening parenthesis";
        else if (src[p.i] == ']') p.err = "Unmatched ] in regex; couldn't find proper opening bracket";
        else p.err = "Unable to parse regex";
    }
    if (p.err) { if (err) *err = p.err; return NULL; }
    if (flags & RXF_R) set_ratchet(pr->root);
    return pr;
}
const RxCap *const *rx_tops(const RxProg *p, int *n) { *n = p->tops.n; return (const RxCap *const *) p->tops.v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int mnode(Ctx *c, const RxNode *n, int pos, const Cont *k);
static int k_final(Ctx *c, const Cont *k, int pos) { (void) k; c->tend = pos; return 1; }
static int k_exact(Ctx *c, const Cont *k, int pos) { (void) c; return pos == k->a; }
static int k_conj(Ctx *c, const Cont *k, int pos) {
    const RxNode *n = k->n;
    int save = c->nev;
    for (int i = 1; i < n->nch; i++) { Cont ek = { k_exact, n, pos, 0, 0, 0 }; if (!mnode(c, n->ch[i], k->a, &ek)) { c->nev = save; return 0; } }
    int r = k->up->fn(c, k->up, pos);
    if (!r) c->nev = save;
    return r;
}
static int k_final_full(Ctx *c, const Cont *k, int pos) { (void) k; if (pos != c->slen) return 0; c->tend = pos; return 1; }
static int run(Ctx *c, const Cont *k, int pos) { return k->fn(c, k, pos); }
static int ev_push(Ctx *c, const RxCap *cap, int from, int to, int sub) {
    if (c->nev == c->cev) { c->cev = c->cev ? c->cev * 2 : 64; c->ev = (RxEv *) (c->ev ? RX_REALLOC(c->ev, sizeof(RxEv) * (size_t) c->cev) : RX_ALLOC(sizeof(RxEv) * (size_t) c->cev)); }
    c->ev[c->nev].cap = cap;
    c->ev[c->nev].from = from;
    c->ev[c->nev].to = to;
    c->ev[c->nev].sub = sub;
    return c->nev++;
}
static int is_wordpos_before(Ctx *c, int pos) { if (pos <= 0) return 0; int pp = rx_prev(c->s, pos); int l; unsigned cp = rx_cp(c->s, c->slen, pp, &l); return rx_isword_u(c->env, cp); }
static int is_wordpos_at(Ctx *c, int pos) { if (pos >= c->slen) return 0; int l; unsigned cp = rx_cp(c->s, c->slen, pos, &l); return rx_isword_u(c->env, cp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int k_seq(Ctx *c, const Cont *k, int pos) {
    const RxNode *n = k->n;
    if (k->a >= n->nch) return run(c, k->up, pos);
    Cont nk = { k_seq, n, k->a + 1, 0, 0, k->up };
    return mnode(c, n->ch[k->a], pos, &nk);
}
static int k_group(Ctx *c, const Cont *k, int pos) { int idx = ev_push(c, k->n->cap, k->a, pos, k->b); int r = run(c, k->up, pos); if (!r) c->nev = idx; return r; }
static int k_span(Ctx *c, const Cont *k, int pos) { int idx = ev_push(c, k->n->cap, k->a, pos, c->nev); int r = run(c, k->up, pos); if (!r) c->nev = idx; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int quant_try(Ctx *c, const RxNode *n, int count, int pos, const Cont *up);
static int k_qiter(Ctx *c, const Cont *k, int pos) { if (pos == k->b && k->a > k->n->min) return 0; return quant_try(c, k->n, k->a, pos, k->up); }
static int quant_step(Ctx *c, const RxNode *n, int count, int pos, const Cont *up) {
    Cont ik = { k_qiter, n, count + 1, pos, 0, up };
    if (count > 0 && n->sep) {
        Cont sk = { 0, n, 0, 0, 0, 0 };
        (void) sk;
        RxNode two;
        memset(&two, 0, sizeof two);
        RxNode *chs[2] = { n->sep, n->ch[0] };
        two.kind = N_SEQ;
        two.nch = 2;
        two.ch = chs;
        return mnode(c, &two, pos, &ik);
    }
    return mnode(c, n->ch[0], pos, &ik);
}
static int quant_try(Ctx *c, const RxNode *n, int count, int pos, const Cont *up) {
    int can_more = n->max < 0 || count < n->max;
    if (n->lazy) { if (count >= n->min) { if (run(c, up, pos)) return 1; } if (can_more) return quant_step(c, n, count, pos, up); return 0; }
    if (n->ratchet) {
        if (can_more) {
            int save = c->nev;
            Cont stop = { k_final, n, 0, 0, 0, 0 };
            RxNode one = *n;
            one.min = 0;
            one.max = 1;
            one.ratchet = 0;
            (void) one;
            if (quant_step(c, n, count, pos, &stop)) { int end = c->tend; if (run(c, up, end)) return 1; c->nev = save; return 0; }
            c->nev = save;
        }
        if (count >= n->min) return run(c, up, pos);
        return 0;
    }
    if (can_more) { if (quant_step(c, n, count, pos, up)) return 1; }
    if (n->trail && n->sep && count > 0 && count >= n->min) { Cont tk = { 0, n, 0, 0, 0, up }; (void) tk; if (mnode(c, n->sep, pos, up)) return 1; }
    if (count >= n->min) return run(c, up, pos);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int single_width(const RxNode *n) {
    if (n->kind == N_ANY || n->kind == N_SET) return 1;
    if (n->kind == N_LIT) { int l; (void) rx_cp((const unsigned char *) n->s, n->slen, 0, &l); return l == n->slen && !(n->fl & RXF_I); }
    return 0;
}
static int match1(Ctx *c, const RxNode *n, int pos) {
    if (pos >= c->slen) return -1;
    if (n->kind == N_LIT) { int l; (void) rx_cp(c->s, c->slen, pos, &l); if (l != n->slen || memcmp(c->s + pos, n->s, (size_t) l)) return -1; return l; }
    int l;
    unsigned cp = rx_cp(c->s, c->slen, pos, &l);
    if (n->kind == N_ANY) return l;
    if (rx_set_test(c->env, n->set, cp, n->fl)) return l;
    return -1;
}
static int quant_simple(Ctx *c, const RxNode *n, int pos, const Cont *k) {
    const RxNode *a = n->ch[0];
    int cnt = 0, p = pos;
    if (n->lazy) { for (;;) { if (cnt >= n->min && run(c, k, p)) return 1; if (n->max >= 0 && cnt >= n->max) return 0; int w = match1(c, a, p); if (w < 0) return 0; p += w; cnt++; } }
    while (n->max < 0 || cnt < n->max) { int w = match1(c, a, p); if (w < 0) break; p += w; cnt++; }
    if (cnt < n->min) return 0;
    if (n->ratchet) return run(c, k, p);
    for (;;) { if (run(c, k, p)) return 1; if (cnt <= n->min) return 0; p = rx_prev(c->s, p); cnt--; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_text(Ctx *c, const RxNode *n, int *from, int *to) {
    for (int i = c->nev - 1; i >= 0; i--) {
        const RxCap *cp = c->ev[i].cap;
        if (cp == &rx_silent_cap) continue;
        int hit = n->min >= 0 ? (cp->kind == 'p' && cp->idx == n->min) : (cp->kind == 'n' && cp->nlen == n->slen && !strncmp(cp->name, n->s, (size_t) n->slen));
        if (hit) { *from = c->ev[i].from; *to = c->ev[i].to; return 1; }
    }
    return 0;
}
static int lit_match(Ctx *c, const RxNode *n, int pos) {
    if (!(n->fl & RXF_I)) { if (pos + n->slen > c->slen || memcmp(c->s + pos, n->s, (size_t) n->slen)) return -1; return pos + n->slen; }
    int p = pos, q = 0;
    while (q < n->slen) {
        if (p >= c->slen) return -1;
        int l1, l2;
        unsigned a = rx_cp(c->s, c->slen, p, &l1), b = rx_cp((const unsigned char *) n->s, n->slen, q, &l2);
        if (a != b && rx_fold(c->env, a, 2) != rx_fold(c->env, b, 2) && rx_fold(c->env, a, 0) != rx_fold(c->env, b, 0)) return -1;
        p += l1;
        q += l2;
    }
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *builtin_rule_src(const char *nm, int n) {
    if (n == 5 && !strncmp(nm, "ident", 5)) return "<.alpha> \\w*";
    if (n == 2 && !strncmp(nm, "sp", 2)) return "\\s";
    if (n == 4 && !strncmp(nm, "same", 4)) return "<?>";
    return NULL;
}
static int k_rule_end(Ctx *c, const Cont *k, int pos) {
    const RxNode *n = k->n;
    int idx = ev_push(c, n->cap ? n->cap : &rx_silent_cap, k->a, pos, k->b);
    int r = run(c, k->up, pos);
    if (!r) c->nev = idx;
    return r;
}
static int call_rule(Ctx *c, const RxNode *n, int pos, const Cont *k) {
    const RxEnv *e = c->env;
    if (!n->sub && !n->looked) {
        int fl = 0;
        const char *src = e && e->rule ? e->rule(e->ud, n->s, n->slen, &fl) : NULL;
        ((RxNode *) n)->looked = 1;
        if (src) { const char *err = NULL; ((RxNode *) n)->sub = rx_compile(src, fl, e, &err); if (!n->sub) return 0; }
    }
    if (!n->sub && n->min) {
        int w = -1;
        if (pos < c->slen) { int l; unsigned cp = rx_cp(c->s, c->slen, pos, &l); if (rx_cls(e, n->min, cp) ^ (n->neg ? 1 : 0)) w = l; }
        if (n->behind) { int ok = w >= 0; if (n->neg) ok = !ok; return ok ? run(c, k, pos) : 0; }
        if (w < 0) return 0;
        if (n->silent || !n->cap) return run(c, k, pos + w);
        int idx = ev_push(c, n->cap, pos, pos + w, c->nev);
        int r = run(c, k, pos + w);
        if (!r) c->nev = idx;
        return r;
    }
    if (!n->sub) {
        const char *src = builtin_rule_src(n->s, n->slen);
        if (!src) { if (n->behind && n->neg) return run(c, k, pos); return 0; }
        const char *err = NULL;
        ((RxNode *) n)->sub = rx_compile(src, RXF_R, e, &err);
        if (!n->sub) return 0;
    }
    if (c->depth > 4000) return 0;
    int before = c->nev;
    c->depth++;
    int r;
    if (n->behind) {
        Cont stop = { k_final, n, 0, 0, 0, 0 };
        int ok = mnode(c, n->sub->root, pos, &stop);
        c->nev = before;
        if (n->neg) ok = !ok;
        r = ok ? run(c, k, pos) : 0;
    } else {
        Cont re = { k_rule_end, n, pos, before, 0, k };
        r = mnode(c, n->sub->root, pos, &re);
    }
    c->depth--;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int look_behind(Ctx *c, const RxNode *n, int pos) {
    Cont stop = { k_final, n, 0, 0, 0, 0 };
    int save = c->nev;
    for (int p = pos; p >= 0; p = p > 0 ? rx_prev(c->s, p) : -1) { if (mnode(c, n->ch[0], p, &stop) && c->tend == pos) { c->nev = save; return 1; } c->nev = save; if (p == 0) break; }
    return 0;
}
static int mnode(Ctx *c, const RxNode *n, int pos, const Cont *k) {
    switch (n->kind) {
        case N_LIT:
        { int e = lit_match(c, n, pos); if (e < 0) return 0; return run(c, k, e); }
        case N_ANY:
        { if (pos >= c->slen) return 0; int l; (void) rx_cp(c->s, c->slen, pos, &l); return run(c, k, pos + l); }
        case N_SET:
        { if (pos >= c->slen) return 0; int l; unsigned cp = rx_cp(c->s, c->slen, pos, &l); if (!rx_set_test(c->env, n->set, cp, n->fl)) return 0; return run(c, k, pos + l); }
        case N_BOS:
        return pos == 0 ? run(c, k, pos) : 0;
        case N_EOS:
        return pos == c->slen ? run(c, k, pos) : 0;
        case N_BOL:
        return (pos == 0 || c->s[pos - 1] == '\n') ? run(c, k, pos) : 0;
        case N_EOL:
        return (pos == c->slen || c->s[pos] == '\n') ? run(c, k, pos) : 0;
        case N_WB:
        {
            int a = is_wordpos_before(c, pos), b = is_wordpos_at(c, pos);
            int at = a != b;
            if (n->neg) at = !at;
            if (!at) return 0;
            if (n->cap) { int idx = ev_push(c, n->cap, pos, pos, c->nev); int r = run(c, k, pos); if (!r) c->nev = idx; return r; }
            return run(c, k, pos);
        }
        case N_LWB:
        return (!is_wordpos_before(c, pos) && is_wordpos_at(c, pos)) ? run(c, k, pos) : 0;
        case N_RWB:
        return (is_wordpos_before(c, pos) && !is_wordpos_at(c, pos)) ? run(c, k, pos) : 0;
        case N_WW:
        case N_NWW:
        {
            int ww = is_wordpos_before(c, pos) && is_wordpos_at(c, pos);
            if ((n->kind == N_WW) != ww) return 0;
            if (n->cap) { int idx = ev_push(c, n->cap, pos, pos, c->nev); int r = run(c, k, pos); if (!r) c->nev = idx; return r; }
            return run(c, k, pos);
        }
        case N_NULL:
        return run(c, k, pos);
        case N_FAIL:
        return 0;
        case N_SPAN:
        { Cont sk = { k_span, n, pos, 0, 0, k }; return mnode(c, n->ch[0], pos, &sk); }
        case N_NLN:
        {
            if (pos >= c->slen) return 0;
            if (c->s[pos] == '\r' && pos + 1 < c->slen && c->s[pos + 1] == '\n') return run(c, k, pos + 2);
            int l;
            unsigned cp = rx_cp(c->s, c->slen, pos, &l);
            return rx_cls(c->env, CL_NL, cp) ? run(c, k, pos + l) : 0;
        }
        case N_CODE:
        return n->neg == 1 ? 0 : run(c, k, pos);
        case N_WS:
        {
            if (is_wordpos_before(c, pos) && is_wordpos_at(c, pos)) return 0;
            int p = pos;
            while (p < c->slen) { int l; unsigned cp = rx_cp(c->s, c->slen, p, &l); if (!rx_isspace_u(cp)) break; p += l; }
            if (n->cap) { int idx = ev_push(c, n->cap, pos, p, c->nev); int r = run(c, k, p); if (!r) c->nev = idx; return r; }
            return run(c, k, p);
        }
        case N_SEQ:
        { if (n->nch == 0) return run(c, k, pos); Cont nk = { k_seq, n, 1, 0, 0, k }; return mnode(c, n->ch[0], pos, &nk); }
        case N_ALT:
        {
            int save = c->nev;
            if (n->ltm) {
                int order[n->nch], endp[n->nch];
                Cont stop = { k_final, n, 0, 0, 0, 0 };
                int m = 0;
                for (int i = 0; i < n->nch; i++) {
                    if (mnode(c, n->ch[i], pos, &stop)) {
                        endp[i] = c->tend;
                        int j = m;
                        while (j > 0 && endp[order[j - 1]] < endp[i]) { order[j] = order[j - 1]; j--; }
                        order[j] = i;
                        m++;
                    } else endp[i] = -1;
                    c->nev = save;
                }
                if (n->ratchet) { if (m == 0) return 0; int i = order[0]; if (mnode(c, n->ch[i], pos, &stop)) { int e = c->tend; if (run(c, k, e)) return 1; } c->nev = save; return 0; }
                for (int q = 0; q < m; q++) { if (mnode(c, n->ch[order[q]], pos, k)) return 1; c->nev = save; }
                return 0;
            }
            if (n->ratchet) {
                Cont stop = { k_final, n, 0, 0, 0, 0 };
                for (int i = 0; i < n->nch; i++) { if (mnode(c, n->ch[i], pos, &stop)) { int e = c->tend; if (run(c, k, e)) return 1; c->nev = save; return 0; } c->nev = save; }
                return 0;
            }
            for (int i = 0; i < n->nch; i++) { if (mnode(c, n->ch[i], pos, k)) return 1; c->nev = save; }
            return 0;
        }
        case N_CONJ:
        { Cont cj = { k_conj, n, pos, 0, 0, k }; return mnode(c, n->ch[0], pos, &cj); }
        case N_MARK:
        { int old = n->neg ? c->mto : c->mfrom; if (n->neg) c->mto = pos; else c->mfrom = pos; int r = run(c, k, pos); if (!r) { if (n->neg) c->mto = old; else c->mfrom = old; } return r; }
        case N_GROUP:
        { Cont gk = { k_group, n, pos, c->nev, 0, k }; return mnode(c, n->ch[0], pos, &gk); }
        case N_QUANT:
        { const RxNode *a = n->ch[0]; if (!n->sep && single_width(a)) return quant_simple(c, n, pos, k); return quant_try(c, n, 0, pos, k); }
        case N_LOOK:
        {
            int save = c->nev;
            int ok;
            if (n->behind) ok = look_behind(c, n, pos);
            else { Cont stop = { k_final, n, 0, 0, 0, 0 }; ok = mnode(c, n->ch[0], pos, &stop); if (n->neg || !ok) c->nev = save; }
            if (n->neg) ok = !ok;
            if (!ok) return 0;
            int r = run(c, k, pos);
            if (!r) c->nev = save;
            return r;
        }
        case N_BACKREF:
        { int f, t; if (!cap_text(c, n, &f, &t)) return 0; int len = t - f; if (pos + len > c->slen) return 0; if (memcmp(c->s + pos, c->s + f, (size_t) len)) return 0; return run(c, k, pos + len); }
        case N_RULE:
        return call_rule(c, n, pos, k);
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rx_exec(const RxProg *pr, const char *subj, int slen, int start, int anchored, RxMatch *m) {
    Ctx c;
    memset(&c, 0, sizeof c);
    c.env = pr->env;
    c.s = (const unsigned char *) subj;
    c.slen = slen;
    memset(m, 0, sizeof *m);
    m->prog = pr;
    const RxNode *first = pr->root;
    while (first->kind == N_SEQ && first->nch > 0) first = first->ch[0];
    for (int pos = start; pos <= slen; ) {
        if (first->kind == N_LIT && !(first->fl & RXF_I) && first->slen > 0 && !(anchored & 1)) {
            const void *q = memchr(subj + pos, first->s[0], (size_t) (slen - pos));
            if (!q) break;
            pos = (int) ((const char *) q - subj);
        }
        c.nev = 0;
        c.mfrom = c.mto = -1;
        Cont fin = { (anchored & 2) ? k_final_full : k_final, pr->root, 0, 0, 0, 0 };
        if (mnode(&c, pr->root, pos, &fin)) { m->ok = 1; m->from = c.mfrom >= 0 ? c.mfrom : pos; m->to = c.mto >= 0 ? c.mto : c.tend; m->nev = c.nev; m->ev = c.ev; return 1; }
        if ((anchored & 1) || pos >= slen) break;
        int l;
        (void) rx_cp(c.s, slen, pos, &l);
        pos += l;
    }
    return 0;
}
