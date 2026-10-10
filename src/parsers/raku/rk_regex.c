#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../ir/ast.h"
#include "rk_regex.h"
#define RXF_I 1
#define RXF_M 2
#define RXF_R 4
#define RXF_S 8
#define RXF_X 16
typedef struct { const char *s; int n, i, flags, pidx, angle, err; const char *msg; } RxP;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rxn(tree_e k) { tree_t *t = ast_node_new(k); t->v.ival = 0; t->n = 0; return t; }
static tree_t *rxn_s(tree_e k, const char *s, int len) { tree_t *t = rxn(k); t->v.sval = ct_strndup(s, (size_t) len); t->slen = len; return t; }
static tree_t *rxn_i(tree_e k, long long v) { tree_t *t = rxn(k); t->v.ival = v; return t; }
static tree_t *rx_ilit(long long v) { tree_t *t = ast_node_new(TT_ILIT); t->v.ival = v; return t; }
static tree_t *rxn_sf(tree_e k, const char *s, int len, long long flags) { tree_t *t = rxn_s(k, s, len); ast_push(t, rx_ilit(flags)); return t; }
static long long rx_flags_of(const tree_t *t) { return (t && t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) ? t->c[0]->v.ival : 0; }
static int rx_peek(RxP *p) { return p->i < p->n ? (unsigned char) p->s[p->i] : 0; }
static int rx_at(RxP *p, int k) { return p->i + k < p->n ? (unsigned char) p->s[p->i + k] : 0; }
static int rx_isw(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static int rx_isidstart(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c >= 0x80; }
static int rx_hexv(int c) { return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1; }
static void rx_err(RxP *p, const char *m) { if (!p->err) { p->err = 1; p->msg = m; } }
static int rx_cplen(const unsigned char *s, int n, int i) { unsigned c = s[i]; int l = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1; return i + l <= n ? l : 1; }
static int rx_putcp(unsigned cp, char *o) {
    if (cp < 0x80) { o[0] = (char) cp; return 1; }
    if (cp < 0x800) { o[0] = (char) (0xC0 | (cp >> 6)); o[1] = (char) (0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char) (0xE0 | (cp >> 12)); o[1] = (char) (0x80 | ((cp >> 6) & 0x3F)); o[2] = (char) (0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char) (0xF0 | (cp >> 18)); o[1] = (char) (0x80 | ((cp >> 12) & 0x3F)); o[2] = (char) (0x80 | ((cp >> 6) & 0x3F)); o[3] = (char) (0x80 | (cp & 0x3F)); return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rx_skipws(RxP *p) {
    for (;;) {
        while (p->i < p->n && isspace(rx_peek(p))) p->i++;
        if (rx_peek(p) == '#') { while (p->i < p->n && rx_peek(p) != '\n') p->i++; continue; }
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_number(RxP *p, unsigned *out, int base) {
    unsigned v = 0; int any = 0;
    for (;;) {
        int c = rx_peek(p), d = base == 16 ? rx_hexv(c) : (c >= '0' && c <= '9') ? c - '0' : -1;
        if (d < 0 || d >= base) break;
        v = v * (unsigned) base + (unsigned) d; p->i++; any = 1;
    }
    *out = v;
    return any;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rx_cls_name(int c, int *neg) {
    *neg = c >= 'A' && c <= 'Z';
    switch (c | 0x20) {
        case 'd': return "digit";
        case 'w': return "word";
        case 's': return "space";
        case 'h': return "hspace";
        case 'v': return "vspace";
        case 'n': return "nl";
        default: return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_escape(RxP *p, int fl) {
    int c = rx_peek(p), neg = 0;
    const char *cls = rx_cls_name(c, &neg);
    if (cls) { p->i++; tree_t *t = rxn_sf(TT_RX_CNAME, cls, (int) strlen(cls), neg); tree_t *k = rxn_i(TT_RX_CLASS, fl); ast_push(k, t); return k; }
    if (c == 'x' || c == 'o' || c == 'c') {
        unsigned cp = 0; char b[8];
        p->i++;
        if (rx_peek(p) == '[') {
            tree_t *lit = rxn(TT_RX_LIT); char *buf = (char *) ct_alloc((size_t) (p->n - p->i) * 4 + 8); int k = 0;
            p->i++;
            for (;;) {
                while (rx_peek(p) == ' ' || rx_peek(p) == ',') p->i++;
                if (!rx_number(p, &cp, c == 'x' ? 16 : c == 'o' ? 8 : 10)) break;
                k += rx_putcp(cp, buf + k);
            }
            if (rx_peek(p) == ']') p->i++; else rx_err(p, "unterminated \\x[...]");
            lit->v.sval = buf; lit->slen = k; buf[k] = 0; ast_push(lit, rx_ilit(fl)); return lit;
        }
        if (!rx_number(p, &cp, c == 'x' ? 16 : c == 'o' ? 8 : 10)) { rx_err(p, "digits expected after \\x"); return rxn(TT_RX_FAIL); }
        { int l = rx_putcp(cp, b); return rxn_sf(TT_RX_LIT, b, l, fl); }
    }
    if (c == 't') { p->i++; return rxn_sf(TT_RX_LIT, "\t", 1, fl); }
    if (c == 'r') { p->i++; return rxn_sf(TT_RX_LIT, "\r", 1, fl); }
    if (c == 'e') { p->i++; return rxn_sf(TT_RX_LIT, "\033", 1, fl); }
    if (c == 'f') { p->i++; return rxn_sf(TT_RX_LIT, "\f", 1, fl); }
    if (c == '0') { p->i++; tree_t *t = rxn_sf(TT_RX_LIT, "", 0, fl | 32); t->slen = 1; return t; }
    { int l = rx_cplen((const unsigned char *) p->s, p->n, p->i); tree_t *t = rxn_sf(TT_RX_LIT, p->s + p->i, l, fl); p->i += l; return t; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_quoted(RxP *p, int fl) {
    int q = rx_peek(p), st;
    char *buf; int k = 0;
    p->i++;
    st = p->i;
    buf = (char *) ct_alloc((size_t) (p->n - st) + 8);
    while (p->i < p->n && rx_peek(p) != q) {
        if (rx_peek(p) == '\\' && p->i + 1 < p->n) { int e = rx_at(p, 1); buf[k++] = (char) (e == 'n' ? '\n' : e == 't' ? '\t' : e); p->i += 2; continue; }
        buf[k++] = p->s[p->i++];
    }
    if (rx_peek(p) == q) p->i++; else rx_err(p, "unterminated quote in regex");
    buf[k] = 0;
    { tree_t *t = rxn(TT_RX_LIT); t->v.sval = buf; t->slen = k; ast_push(t, rx_ilit(fl)); return t; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_set_item_bracket(RxP *p, tree_t *set, int op) {
    while (p->i < p->n && rx_peek(p) != ']') {
        unsigned lo, hi; int ll;
        if (rx_peek(p) == ' ' || rx_peek(p) == '\n' || rx_peek(p) == '\t') { p->i++; continue; }
        if (rx_peek(p) == '\\' && p->i + 1 < p->n) {
            int e = rx_at(p, 1), neg = 0; const char *cls = rx_cls_name(e, &neg);
            p->i += 2;
            if (cls) { ast_push(set, rxn_sf(TT_RX_CNAME, cls, (int) strlen(cls), neg | (op << 1))); continue; }
            if (e == 'x' || e == 'o') { if (!rx_number(p, &lo, e == 'x' ? 16 : 8)) { rx_err(p, "digits expected in class"); return 0; } }
            else lo = e == 'n' ? '\n' : e == 't' ? '\t' : e == 'r' ? '\r' : e == 'e' ? 27 : e == 'f' ? '\f' : (unsigned) e;
        } else {
            ll = rx_cplen((const unsigned char *) p->s, p->n, p->i);
            lo = 0;
            for (int k = 0; k < ll; k++) lo = (lo << 8) | (unsigned char) p->s[p->i + k];
            if (ll > 1) { unsigned c = (unsigned char) p->s[p->i]; lo = ll == 2 ? c & 0x1F : ll == 3 ? c & 0x0F : c & 0x07; for (int k = 1; k < ll; k++) lo = (lo << 6) | ((unsigned char) p->s[p->i + k] & 0x3F); }
            p->i += ll;
        }
        hi = lo;
        while (rx_peek(p) == ' ') p->i++;
        if (rx_peek(p) == '.' && rx_at(p, 1) == '.') {
            p->i += 2;
            while (rx_peek(p) == ' ') p->i++;
            if (rx_peek(p) == '\\' && p->i + 1 < p->n) { int e = rx_at(p, 1); p->i += 2; if (e == 'x' || e == 'o') { if (!rx_number(p, &hi, e == 'x' ? 16 : 8)) { rx_err(p, "range end expected"); return 0; } } else hi = (unsigned) e; }
            else { ll = rx_cplen((const unsigned char *) p->s, p->n, p->i); unsigned c = (unsigned char) p->s[p->i]; hi = ll == 1 ? c : ll == 2 ? c & 0x1F : ll == 3 ? c & 0x0F : c & 0x07; for (int k = 1; k < ll; k++) hi = (hi << 6) | ((unsigned char) p->s[p->i + k] & 0x3F); p->i += ll; }
        }
        { tree_t *r = rxn_i(TT_RX_CRANGE, op); ast_push(r, rx_ilit(lo)); ast_push(r, rx_ilit(hi)); ast_push(set, r); }
    }
    if (rx_peek(p) == ']') { p->i++; return 1; }
    rx_err(p, "unterminated [ in character class");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_set(RxP *p, int firstop, int fl) {
    tree_t *set = rxn_i(TT_RX_CLASS, fl);
    int op = firstop;
    for (;;) {
        int c;
        while (rx_peek(p) == ' ') p->i++;
        c = rx_peek(p);
        if (c == '>') { p->i++; break; }
        if (c == '+') { op = 0; p->i++; continue; }
        if (c == '-') { op = 1; p->i++; continue; }
        if (p->i >= p->n) { rx_err(p, "unterminated character class"); return set; }
        if (c == '[') { p->i++; if (!rx_set_item_bracket(p, set, op)) return set; }
        else if (c == ':') {
            int ng = 0, st;
            p->i++;
            if (rx_peek(p) == '!') { ng = 1; p->i++; }
            st = p->i;
            while (p->i < p->n && (rx_isw(rx_peek(p)) || rx_peek(p) == '=')) p->i++;
            if (rx_peek(p) == '<' || rx_peek(p) == '(') { int open = rx_peek(p), close = open == '<' ? '>' : ')', d = 0; while (p->i < p->n) { if (rx_peek(p) == open) d++; else if (rx_peek(p) == close && --d == 0) { p->i++; break; } p->i++; } }
            ast_push(set, rxn_sf(TT_RX_CPROP, p->s + st, p->i - st, ng | (op << 1)));
        } else if (rx_isidstart(c)) {
            int st = p->i;
            while (p->i < p->n && rx_isw(rx_peek(p))) p->i++;
            ast_push(set, rxn_sf(TT_RX_CNAME, p->s + st, p->i - st, op << 1));
        } else if (c == '.') p->i++;
        else { rx_err(p, "unable to parse character class"); return set; }
        op = 0;
    }
    return set;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_alt(RxP *p);
static tree_t *rx_atom(RxP *p);
static int rx_ident(RxP *p, int *st) {
    int b = p->i;
    if (!rx_isidstart(rx_peek(p))) return 0;
    while (p->i < p->n && (rx_isw(rx_peek(p)) || ((rx_peek(p) == '-' || rx_peek(p) == '\'') && rx_isidstart(rx_at(p, 1))) || rx_peek(p) == ':')) {
        if (rx_peek(p) == ':' && rx_at(p, 1) != ':' && !(rx_isw(rx_at(p, 1)))) break;
        p->i++;
    }
    *st = b;
    return p->i - b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_code_block(RxP *p, tree_e kind, int flag) {
    int d = 0, st = p->i;
    while (p->i < p->n) { if (p->s[p->i] == '{') d++; else if (p->s[p->i] == '}' && --d == 0) { p->i++; break; } p->i++; }
    return rxn_sf(kind, p->s + st, p->i - st, flag);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_angle(RxP *p) {
    int c = rx_peek(p), fl = p->flags & (RXF_I | RXF_M), pred = 0;
    if (c == '(') { p->i++; return rxn_i(TT_RX_MARK, 0); }
    if (c == ' ' || c == '\t' || c == '\n') {
        tree_t *alt = rxn_i(TT_RX_ALT, 2);
        for (;;) {
            int st;
            rx_skipws(p);
            if (p->i >= p->n) { rx_err(p, "unterminated < ... > word list"); return alt; }
            if (rx_peek(p) == '>') { p->i++; break; }
            st = p->i;
            while (p->i < p->n && !isspace(rx_peek(p)) && rx_peek(p) != '>') p->i++;
            ast_push(alt, rxn_sf(TT_RX_LIT, p->s + st, p->i - st, fl));
        }
        return alt->n == 1 ? alt->c[0] : alt;
    }
    if (c == '[') return rx_set(p, 0, fl);
    if (c == '+' || c == '-') { int op = c == '-'; p->i++; while (rx_peek(p) == ' ') p->i++; return rx_set(p, op, fl); }
    if (c == ':') return rx_set(p, 0, fl);
    if (c == '{') { tree_t *t = rx_code_block(p, TT_RX_CODE, 2); if (rx_peek(p) == '>') p->i++; else rx_err(p, "missing > after <{...}"); return t; }
    if (c == '?' || c == '!') {
        pred = c;
        p->i++;
        c = rx_peek(p);
        if (c == '>') { p->i++; return rxn(pred == '?' ? TT_RX_NULL : TT_RX_FAIL); }
        if (c == '{') { tree_t *t = rx_code_block(p, TT_RX_CODE, pred == '!' ? 1 : 0); if (rx_peek(p) == '>') p->i++; else rx_err(p, "missing > after <?{...}"); return t; }
        if (c == '?' || c == '!') { p->i++; if (rx_peek(p) == '>') p->i++; return rxn(pred == '?' ? TT_RX_NULL : TT_RX_FAIL); }
        if (c == '[' || c == ':' || c == '+' || c == '-') {
            tree_t *set = (c == '+' || c == '-') ? (p->i++, rx_set(p, c == '-', fl)) : rx_set(p, 0, fl);
            tree_t *lk = rxn_i(TT_RX_LOOK, pred == '!' ? 1 : 0);
            ast_push(lk, set);
            return lk;
        }
    }
    {
        int dot = 0, amp = 0, st, nl;
        const char *alias = NULL; int alen = 0;
        if (c == '.') { dot = 1; p->i++; if (rx_peek(p) == '.' && rx_at(p, 1) == '.') { p->i += 2; if (rx_peek(p) == '>') p->i++; return rxn(TT_RX_FAIL); } }
        else if (c == '&') { amp = 1; p->i++; }
        else if (c == '|') { p->i++; if (rx_peek(p) == 'w') { p->i++; if (rx_peek(p) == '>') p->i++; return rxn_i(TT_RX_ANCHOR, 6); } rx_err(p, "unrecognized regex assertion"); return rxn(TT_RX_FAIL); }
        nl = rx_ident(p, &st);
        if (!nl) { rx_err(p, rx_peek(p) == '$' || rx_peek(p) == '@' ? "variable interpolation inside <...>" : "unrecognized regex assertion"); return rxn(TT_RX_FAIL); }
        if (rx_peek(p) == '=') { alias = p->s + st; alen = nl; p->i++; if (rx_peek(p) == '.') { dot = 1; p->i++; } else if (rx_peek(p) == '&') { amp = 1; p->i++; } nl = rx_ident(p, &st); if (!nl) { rx_err(p, "unrecognized regex assertion"); return rxn(TT_RX_FAIL); } }
        if ((nl == 6 && !strncmp(p->s + st, "before", 6)) || (nl == 5 && !strncmp(p->s + st, "after", 5))) {
            tree_t *lk = rxn_i(TT_RX_LOOK, (pred == '!' ? 1 : 0) | (nl == 5 ? 2 : 0));
            rx_skipws(p);
            p->angle++;
            ast_push(lk, rx_alt(p));
            p->angle--;
            if (rx_peek(p) == '>') p->i++; else rx_err(p, "missing > after lookaround");
            return lk;
        }
        {
            tree_t *r = rxn_sf(TT_RX_RULE, p->s + st, nl, (dot ? 1 : 0) | (amp ? 2 : 0) | (pred == '?' ? 4 : 0) | (pred == '!' ? 8 : 0));
            if (alias) { tree_t *a = rxn_s(TT_RX_CAP, alias, alen); ast_push(a, r); r = a; }
            if (rx_peek(p) == '(' || rx_peek(p) == ':' || rx_peek(p) == ' ') {
                int d = 0, ast = p->i;
                while (p->i < p->n && !(d == 0 && rx_peek(p) == '>')) { if (rx_peek(p) == '(' || rx_peek(p) == '[' || rx_peek(p) == '{') d++; else if (rx_peek(p) == ')' || rx_peek(p) == ']' || rx_peek(p) == '}') d--; p->i++; }
                ast_push(r->t == TT_RX_CAP ? r->c[0] : r, rxn_s(TT_RX_ARGS, p->s + ast, p->i - ast));
            }
            if (rx_peek(p) == '>') p->i++; else rx_err(p, "missing > after assertion");
            return r;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rx_closure_bounds(RxP *p, int *mn, int *mx) {
    unsigned a = 0, b = 0;
    p->i++;
    rx_skipws(p);
    if (!rx_number(p, &a, 10)) { rx_err(p, "number expected in **{}"); return 0; }
    rx_skipws(p);
    *mn = (int) a; *mx = (int) a;
    if (rx_peek(p) == '.' && rx_at(p, 1) == '.') { p->i += 2; rx_skipws(p); if (rx_peek(p) == '*') { p->i++; *mx = -1; } else if (rx_number(p, &b, 10)) *mx = (int) b; else { rx_err(p, "range end expected"); return 0; } }
    rx_skipws(p);
    if (rx_peek(p) == '}') p->i++; else rx_err(p, "missing } in **{}");
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rx_modifier(RxP *p) {
    int st, nl, on = 1;
    p->i++;
    if (rx_peek(p) == '!') { on = 0; p->i++; }
    nl = rx_ident(p, &st);
    if (nl == 1 && p->s[st] == 'i') { if (on) p->flags |= RXF_I; else p->flags &= ~RXF_I; }
    else if (nl == 10 && !strncmp(p->s + st, "ignorecase", 10)) { if (on) p->flags |= RXF_I; else p->flags &= ~RXF_I; }
    else if (nl == 1 && p->s[st] == 'm') { if (on) p->flags |= RXF_M; else p->flags &= ~RXF_M; }
    else if (nl == 1 && p->s[st] == 'r') { if (on) p->flags |= RXF_R; else p->flags &= ~RXF_R; }
    else if (nl == 7 && !strncmp(p->s + st, "ratchet", 7)) { if (on) p->flags |= RXF_R; else p->flags &= ~RXF_R; }
    else if (nl == 1 && p->s[st] == 's') { if (on) p->flags |= RXF_S; else p->flags &= ~RXF_S; }
    else if (nl == 8 && !strncmp(p->s + st, "sigspace", 8)) { if (on) p->flags |= RXF_S; else p->flags &= ~RXF_S; }
    else rx_err(p, "unknown regex modifier");
    if (rx_peek(p) == '(' || rx_peek(p) == '[') { int open = rx_peek(p), close = open == '(' ? ')' : ']', d = 0; while (p->i < p->n) { if (rx_peek(p) == open) d++; else if (rx_peek(p) == close && --d == 0) { p->i++; break; } p->i++; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_group(RxP *p, int capture) {
    int save_pidx = p->pidx, idx = capture ? p->pidx++ : -1;
    tree_t *body, *g;
    p->angle++;
    body = rx_alt(p);
    p->angle--;
    if (rx_peek(p) == (capture ? ')' : ']')) p->i++; else rx_err(p, capture ? "missing )" : "missing ]");
    if (capture) { g = rxn_i(TT_RX_CAP, idx); ast_push(g, body); return g; }
    (void) save_pidx;
    g = rxn_i(TT_RX_GROUP, 0);
    ast_push(g, body);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_atom(RxP *p) {
    int fl = p->flags & (RXF_I | RXF_M), c = rx_peek(p);
    if (p->i >= p->n) return NULL;
    if (c == '\'' || c == '"') return rx_quoted(p, fl);
    if (c == '\\') { p->i++; if (p->i >= p->n) { rx_err(p, "backslash at end of regex"); return NULL; } return rx_escape(p, fl); }
    if (c == '.') { p->i++; return rxn(TT_RX_ANY); }
    if (c == '^') { p->i++; if (rx_peek(p) == '^') { p->i++; return rxn_i(TT_RX_ANCHOR, 2); } return rxn_i(TT_RX_ANCHOR, 0); }
    if (c == '$') {
        p->i++;
        if (rx_peek(p) == '$') { p->i++; return rxn_i(TT_RX_ANCHOR, 3); }
        if (rx_peek(p) == '<' || (rx_peek(p) >= '0' && rx_peek(p) <= '9')) {
            int nm = 0, nlen = 0, num = -1, after;
            if (rx_peek(p) == '<') { p->i++; nm = p->i; while (p->i < p->n && rx_peek(p) != '>') p->i++; nlen = p->i - nm; if (rx_peek(p) == '>') p->i++; }
            else { unsigned v = 0; rx_number(p, &v, 10); num = (int) v; }
            after = p->i;
            while (rx_peek(p) == ' ') p->i++;
            if (rx_peek(p) == '=' && rx_at(p, 1) != '=' && rx_at(p, 1) != '~') {
                tree_t *a, *body;
                p->i++;
                rx_skipws(p);
                body = rx_atom(p);
                if (!body) { rx_err(p, "alias needs an atom"); return rxn(TT_RX_FAIL); }
                a = num >= 0 ? rxn_i(TT_RX_CAP, num) : rxn_s(TT_RX_CAP, p->s + nm, nlen);
                ast_push(a, body);
                return a;
            }
            p->i = after;
            return num >= 0 ? rxn_i(TT_RX_BACKREF, num) : rxn_s(TT_RX_BACKREF, p->s + nm, nlen);
        }
        if (rx_isidstart(rx_peek(p))) { int st = p->i; while (p->i < p->n && (rx_isw(rx_peek(p)) || ((rx_peek(p) == '-' || rx_peek(p) == '\'') && isalpha(rx_at(p, 1))))) p->i++; return rxn_sf(TT_RX_VAR, p->s + st, p->i - st, '$'); }
        return rxn_i(TT_RX_ANCHOR, 1);
    }
    if (c == '@' && rx_isidstart(rx_at(p, 1))) { int st; p->i++; st = p->i; while (p->i < p->n && rx_isw(rx_peek(p))) p->i++; return rxn_sf(TT_RX_VAR, p->s + st, p->i - st, '@'); }
    if (c == '<') {
        if (rx_at(p, 1) == '<') { p->i += 2; return rxn_i(TT_RX_ANCHOR, 4); }
        if ((rx_at(p, 1) == '$' || rx_at(p, 1) == '@') && rx_isidstart(rx_at(p, 2))) {
            int sig = rx_at(p, 1), st;
            p->i += 2; st = p->i;
            while (p->i < p->n && rx_isw(rx_peek(p))) p->i++;
            if (rx_peek(p) == '>') { p->i++; return rxn_sf(TT_RX_VAR, p->s + st, p->i - 1 - st, sig | 256); }
            rx_err(p, "missing > after <$var>");
            return rxn(TT_RX_FAIL);
        }
        p->i++;
        return rx_angle(p);
    }
    if (c == '>') { if (p->angle > 0) return NULL; if (rx_at(p, 1) == '>') { p->i += 2; return rxn_i(TT_RX_ANCHOR, 5); } rx_err(p, "unrecognized regex metacharacter >"); return NULL; }
    if (c == 0xC2 && rx_at(p, 1) == 0xAB) { p->i += 2; return rxn_i(TT_RX_ANCHOR, 4); }
    if (c == 0xC2 && rx_at(p, 1) == 0xBB) { p->i += 2; return rxn_i(TT_RX_ANCHOR, 5); }
    if (c == '(') { p->i++; return rx_group(p, 1); }
    if (c == '[') { p->i++; return rx_group(p, 0); }
    if (c == '{') return rx_code_block(p, TT_RX_CODE, 3);
    if (c == ')' && rx_at(p, 1) == '>') { p->i += 2; return rxn_i(TT_RX_MARK, 1); }
    if (c == ')' || c == ']' || c == '|' || c == '&' || c == '~') return NULL;
    if (c == ':') { if (rx_isidstart(rx_at(p, 1)) || rx_at(p, 1) == '!') { rx_modifier(p); return rxn(TT_RX_NULL); } p->i++; while (rx_peek(p) == ':') p->i++; return rxn(TT_RX_NULL); }
    if (c == '#') { rx_skipws(p); return rxn(TT_RX_NULL); }
    if (c == '*' || c == '+' || c == '?') { rx_err(p, "quantifier quantifies nothing"); return NULL; }
    if (c == '%') { rx_err(p, "unrecognized regex metacharacter %"); return NULL; }
    if (rx_isw(c)) { int l = rx_cplen((const unsigned char *) p->s, p->n, p->i); tree_t *t = rxn_sf(TT_RX_LIT, p->s + p->i, l, fl); p->i += l; return t; }
    rx_err(p, "unrecognized regex metacharacter");
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_quant(RxP *p, tree_t *a) {
    int save0 = p->i, mn = 0, mx = 0, pre = 0, c, d, m, flags;
    tree_t *q;
    rx_skipws(p);
    c = rx_peek(p); d = rx_at(p, 1);
    if (c == '*' && d != '*') { mn = 0; mx = -1; p->i++; }
    else if (c == '+') { mn = 1; mx = -1; p->i++; }
    else if (c == '?') { mn = 0; mx = 1; p->i++; }
    else if (c == '*' && d == '*') {
        unsigned v = 0;
        p->i += 2;
        rx_skipws(p);
        if (rx_peek(p) == '?' || rx_peek(p) == '!' || rx_peek(p) == ':') { pre = rx_peek(p); p->i++; rx_skipws(p); }
        if (rx_peek(p) == '^') { unsigned w = 0; p->i++; rx_number(p, &w, 10); mn = 0; mx = (int) w - 1; }
        else if (rx_peek(p) == '{') { if (!rx_closure_bounds(p, &mn, &mx)) return a; }
        else if (rx_isw(rx_peek(p)) && rx_number(p, &v, 10)) {
            mn = (int) v; mx = mn;
            if (rx_peek(p) == '.' && rx_at(p, 1) == '.') { p->i += 2; if (rx_peek(p) == '*') { mx = -1; p->i++; } else if (rx_peek(p) == '^') { unsigned w = 0; p->i++; rx_number(p, &w, 10); mx = (int) w - 1; } else { unsigned w = 0; rx_number(p, &w, 10); mx = (int) w; } }
        } else { rx_err(p, "unrecognized quantifier after **"); return a; }
    } else { p->i = save0; return a; }
    flags = (p->flags & RXF_R) ? 2 : 0;
    m = pre ? pre : rx_peek(p);
    if (m == '?') { flags = 1; if (!pre) p->i++; }
    else if (m == '!') { flags = 0; if (!pre) p->i++; }
    else if (m == ':' && (pre || rx_at(p, 1) != ':')) { flags = 2; if (!pre) p->i++; }
    q = rxn_i(TT_RX_QUANT, flags);
    ast_push(q, a);
    ast_push(q, rx_ilit(mn));
    ast_push(q, rx_ilit(mx));
    {
        int save = p->i;
        rx_skipws(p);
        if (rx_peek(p) == '%') {
            tree_t *s;
            p->i++;
            if (rx_peek(p) == '%') { q->v.ival |= 4; p->i++; }
            rx_skipws(p);
            s = rx_atom(p);
            if (s) ast_push(q, s); else rx_err(p, "missing separator after %");
        } else p->i = save;
    }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_seq(RxP *p) {
    tree_t *s = rxn_i(TT_RX_SEQ, (p->flags & RXF_S) ? 1 : 0);
    for (;;) {
        tree_t *a;
        rx_skipws(p);
        if (p->i >= p->n) break;
        a = rx_atom(p);
        if (!a) break;
        if (a->t == TT_RX_NULL) { s->v.ival = (p->flags & RXF_S) ? 1 : 0; continue; }
        a = rx_quant(p, a);
        if (a->t == TT_RX_LIT && s->n > 0 && s->c[s->n - 1]->t == TT_RX_LIT && rx_flags_of(s->c[s->n - 1]) == rx_flags_of(a) && !(rx_flags_of(a) & 32)) {
            tree_t *l = s->c[s->n - 1];
            char *b = (char *) ct_alloc((size_t) (l->slen + a->slen) + 1);
            memcpy(b, l->v.sval, (size_t) l->slen); memcpy(b + l->slen, a->v.sval, (size_t) a->slen); b[l->slen + a->slen] = 0;
            l->v.sval = b; l->slen += a->slen;
            continue;
        }
        ast_push(s, a);
    }
    return s->n == 1 && !s->v.ival ? s->c[0] : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_conj(RxP *p) {
    tree_t *l = rx_seq(p), *cj = NULL;
    for (;;) {
        rx_skipws(p);
        if (rx_peek(p) != '&') break;
        p->i++;
        if (rx_peek(p) == '&') p->i++;
        if (!cj) { cj = rxn(TT_RX_CONJ); ast_push(cj, l); }
        ast_push(cj, rx_seq(p));
    }
    return cj ? cj : l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rx_alt(RxP *p) {
    int pidx0 = p->pidx, pmax;
    tree_t *first, *alt = NULL;
    rx_skipws(p);
    if (rx_peek(p) == '|' && rx_at(p, 1) != '|') p->i++;
    first = rx_conj(p);
    pmax = p->pidx;
    for (;;) {
        int seq;
        rx_skipws(p);
        if (rx_peek(p) != '|') break;
        p->i++;
        seq = rx_peek(p) == '|';
        if (seq) p->i++;
        if (!alt) { alt = rxn_i(TT_RX_ALT, seq ? 1 : 0); ast_push(alt, first); }
        if (seq) alt->v.ival = 1;
        p->pidx = pidx0;
        ast_push(alt, rx_conj(p));
        if (p->pidx > pmax) pmax = p->pidx;
    }
    p->pidx = pmax;
    return alt ? alt : first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_regex_parse(const char *flags, const char *s, int len) {
    RxP p;
    tree_t *root = rxn(TT_RX_ROOT), *body;
    memset(&p, 0, sizeof p);
    p.s = s; p.n = len;
    for (const char *f = flags ? flags : ""; *f; f++) { if (*f == 'i') p.flags |= RXF_I; else if (*f == 'r') p.flags |= RXF_R; else if (*f == 's') p.flags |= RXF_S; else if (*f == 'm') p.flags |= RXF_M; else if (*f == 'x' || *f == 'g' || *f == 'p' || *f == 'c' || *f == 'e' || *f == 'o' || *f == 'n') p.flags |= RXF_X; }
    body = rx_alt(&p);
    rx_skipws(&p);
    if (p.i < p.n && !p.err) rx_err(&p, "unparsed text at the end of the regex");
    root->v.sval = flags ? ct_strdup(flags) : (char *) "";
    root->v.ival = p.err ? 1 : 0;
    root->slen = p.pidx;
    if (p.err) { tree_t *e = rxn_s(TT_RX_FAIL, p.msg, (int) strlen(p.msg)); ast_push(root, e); return root; }
    ast_push(root, body);
    return root;
}
