#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/lib/bn_head_gs_bq.h"
#include "runtime/lib/bn_head_gt_punct.h"
#include "runtime/lib/bn_head_pl_gs.h"
#include "runtime/lib/bn_head_gp_general.h"
#include "runtime/lib/bn_head_pl_gp.h"
#include "runtime/rt/prolog_atom.h"
#include "driver/driver_globals.h"
#include "runtime/core/utf8.h"
#include <ctype.h>
#include <fcntl.h>
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
#include "runtime/lib/bn_34_pl.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_hb_aggb.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
static int pl_gp_term(pl_gp_t *p, int cur, int ctx, int cp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rb_room(char **bp, size_t *np, size_t need) {
    size_t nn = *np ? *np : 256;
    char *q;
    if (need < *np && *bp) return 1;
    while (nn <= need) nn *= 2;
    q = (char *)rt_gcheap_grow_block((void *)*bp, (uint16_t)HB_WSB, (uint64_t)nn);
    if (!q) return 0;
    *bp = q;
    *np = nn;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pl_utf8_ok_at(const unsigned char *s, size_t n) {
    size_t w = s[0] < 0x80 ? 1 : (size_t)utf8_seqlen(s[0]);
    if (s[0] >= 0x80 && w < 2) return 0;
    for (size_t i = 1; i < w; i++) if (i >= n || (s[i] & 0xC0) != 0x80) return 0;
    return w;
}
int pl_text_fffd(char **bp, size_t *np) {
    const unsigned char *s = (const unsigned char *)*bp;
    size_t len = strlen(*bp), m = 0, k = 0, on = 0, w;
    char *o = (char *)0;
    for (size_t i = 0; i < len; i += w ? w : 1) if (!(w = pl_utf8_ok_at(s + i, len - i))) m++;
    if (!m) return 1;
    if (!pl_rb_room(&o, &on, len + 2 * m + 1)) return 0;
    for (size_t i = 0; i < len; i += w ? w : 1) { if ((w = pl_utf8_ok_at(s + i, len - i))) { memcpy(o + k, s + i, w); k += w; } else { memcpy(o + k, "\xEF\xBF\xBD", 3); k += 3; } }
    o[k] = 0;
    *bp = o;
    *np = on;
    return 1;
}
static int pl_rb_put(char **bp, size_t *np, size_t *k, int c) { if (!pl_rb_room(bp, np, *k + 2)) return 0; (*bp)[(*k)++] = (char)c; return 1; }
int pl_read_term_text_raw(char **bp, size_t *np) {
    extern FILE *fh_cur_in_fp(void);
    FILE *in = fh_cur_in_fp();
    size_t k = 0;
    int c, q = 0, seen = 0, lit = 0, wlit;
    if (!pl_rb_room(bp, np, 2)) return -1;
    while ((c = fgetc(in)) != EOF) {
        if (!seen && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) continue;
        if (!q && c == '%') {
            if (seen && !pl_rb_put(bp, np, &k, c)) return -1;
            while ((c = fgetc(in)) != EOF && c != '\n') if (seen && !pl_rb_put(bp, np, &k, c)) return -1;
            if (seen && !pl_rb_put(bp, np, &k, '\n')) return -1;
            continue;
        }
        if (!q && c == '/') {
            int d = fgetc(in);
            if (d == '*') {
                int p = 0, e2, closed = 0;
                if (seen && !(pl_rb_put(bp, np, &k, c) && pl_rb_put(bp, np, &k, d))) return -1;
                while ((e2 = fgetc(in)) != EOF) { if (seen && !pl_rb_put(bp, np, &k, e2)) return -1; if (p == '*' && e2 == '/') { closed = 1; break; } p = e2; }
                if (!closed) { (*bp)[k] = 0; return 2; }
                if (seen) lit = 1;
                continue;
            }
            if (d != EOF) ungetc(d, in);
        }
        if (!seen) { extern void fh_note_read_start(int); extern int fh_current_input(void); fh_note_read_start(fh_current_input()); }
        seen = 1;
        if (!pl_rb_room(bp, np, k + 2)) return -1;
        wlit = lit;
        lit = 0;
        (*bp)[k++] = (char)c;
        if (q) { if (c == '\\') { int r = pl_read_esc_body(in, bp, np, &k); if (r) { (*bp)[k] = 0; return r; } } else if (c == q) { q = 0; lit = 1; } continue; }
        if (c == '\'' && k >= 2 && (*bp)[k - 2] == '0' && (k < 3 || !(isalnum((unsigned char)(*bp)[k - 3]) || (*bp)[k - 3] == '_'))) {
            int d = fgetc(in);
            if (d == EOF) { (*bp)[k] = 0; return 2; }
            if (!pl_rb_room(bp, np, k + 2)) return -1;
            (*bp)[k++] = (char)d;
            if (d == '\\') {
                int r = pl_read_esc_body(in, bp, np, &k);
                if (r) { (*bp)[k] = 0; return r; }
            } else if (d == '\'') {
                int e2 = fgetc(in);
                if (e2 == '\'') { if (!pl_rb_room(bp, np, k + 2)) return -1; (*bp)[k++] = (char)e2; } else if (e2 != EOF) ungetc(e2, in);
            }
            lit = 1;
            continue;
        }
        if (c == '\'' || c == '"') { q = c; continue; }
        if (c == '.' && (wlit || !(k >= 2 && pl_tok_is_graphic((unsigned char)(*bp)[k - 2])))) {
            int d = fgetc(in);
            if (d != EOF) ungetc(d, in);
            if (d == EOF || d == ' ' || d == '\t' || d == '\n' || d == '\r' || d == '%') { (*bp)[k] = 0; return 1; }
        }
    }
    (*bp)[k] = 0;
    return seen ? 2 : 0;
}
static int pl_gs_class(int c) {
    if (c == EOF) return 0;
    if (c >= 128) return GS_EX;
    if (c <= ' ' || c == 127) return GS_LA;
    if (c >= '0' && c <= '9') return GS_DI;
    if (c >= 'a' && c <= 'z') return GS_SL;
    if (c >= 'A' && c <= 'Z') return GS_CL;
    if (c == '_') return GS_UL;
    if (c == '!' || c == ',' || c == ';') return GS_SC;
    if (c == '"') return GS_DQ;
    if (c == '\'') return GS_QT;
    if (c == '`') return GS_BQ;
    if (c == '%') return GS_CM;
    if (strchr("()[]{}|", c)) return GS_PC;
    return GS_GR;
}
static int pl_gs_get(pl_gs_t *s) { s->c = fgetc(s->in); s->ty = pl_gs_class(s->c); s->eofn = s->c == EOF ? s->eofn + 1 : 0; return s->c; }
static void pl_gs_unget(pl_gs_t *s) { if (s->c != EOF) ungetc(s->c, s->in); else if (s->eofn > 0) s->eofn--; }
static int pl_gs_peek(pl_gs_t *s) { int d = fgetc(s->in); if (d != EOF) ungetc(d, s->in); return d; }
static void pl_gs_put(pl_gs_t *s, int ch) { if (!pl_rb_room(&s->b, &s->bn, s->bk + 2)) { s->oom = 1; return; } s->b[s->bk++] = (char)ch; s->b[s->bk] = 0; }
static void pl_gs_at(pl_gs_t *s, int plus) {
    extern int fh_in_position(int, long *, long *, long *);
    long ch = 0, ln = 0, lp = 0, off = ftell(s->in), p, n, col = 0;
    char b[512];
    s->tl = 0;
    s->tc = 0;
    if (off < 0 || fh_in_position(s->idx, &ch, &ln, &lp) != 1) return;
    for (p = off; p > 0; p -= n) {
        n = p > (long)sizeof b ? (long)sizeof b : p;
        if (pread(fileno(s->in), b, (size_t)n, (off_t)(p - n)) != (ssize_t)n) break;
        for (long i = n - 1; i >= 0; i--) { if (b[i] == '\n') { p = 0; break; } col++; }
        if (!p) break;
    }
    s->tl = ln + 1;
    s->tc = col + plus + s->eofn;
}
static void pl_gs_fail(pl_gs_t *s, const char *msg, int plus) { if (s->err) return; s->err = msg; pl_gs_at(s, plus); }
static int pl_gs_qchar(pl_gs_t *s, int q, int noesc) {
    const char *p;
    long long x;
    int rad;
    pl_gs_get(s);
    if (s->c == q) { if (pl_gs_peek(s) != q) return -1; pl_gs_get(s); return s->c; }
    if (s->c == EOF) { if (!s->err) { pl_gs_unget(s); pl_gs_fail(s, "closing quote expected here", 1); } return -3; }
    if (s->c == '\n') { if (!s->err) { pl_gs_unget(s); pl_gs_fail(s, "unexpected newline", 1); } return -4; }
    if (s->c == '\t') { if (!s->err) { pl_gs_unget(s); pl_gs_fail(s, "unexpected tab", 1); } return -5; }
    if (s->c != '\\' || noesc) return s->c;
    pl_gs_get(s);
    if (s->c == '\n') return -2;
    if (s->c > 0 && strchr("\\'\"`", s->c)) return s->c;
    if (s->c > 0 && (p = strchr("abfnrtv", s->c))) return "\a\b\f\n\r\t\v"[p - "abfnrtv"];
    if (s->c != 'x' && (s->c < '0' || s->c > '7')) { pl_gs_fail(s, "unknown escape sequence", 0); return -6; }
    rad = s->c == 'x' ? 16 : 8;
    x = s->c == 'x' ? 0 : s->c - '0';
    for (pl_gs_get(s); s->c > 0 && (rad == 16 ? isxdigit(s->c) : (s->c >= '0' && s->c <= '7')); pl_gs_get(s)) {
        if (x <= 0x10FFFF) x = x * rad + (isdigit(s->c) ? s->c - '0' : (tolower(s->c) - 'a' + 10));
    }
    if (x < 1 || x > 255) pl_gs_fail(s, "invalid character code in \\constant\\ sequence", 0);
    else if (s->c == '\\') return (int)x;
    else pl_gs_fail(s, "\\ expected in \\constant\\ sequence", 0);
    while (s->c != '\\' && s->c != q && s->c != EOF && s->c != '\n') pl_gs_get(s);
    if (s->c == q) pl_gs_unget(s);
    return -6;
}
static void pl_gs_quoted(pl_gs_t *s) {
    int q = s->c, ef = 0, ch;
    s->tk = s->ty == GS_QT ? GT_NAME : s->ty == GS_DQ ? GT_STR : GT_BQ;
    s->q = s->ty == GS_QT;
    s->bk = 0;
    for (;;) {
        ch = pl_gs_qchar(s, q, s->tk == GT_BQ);
        if (ch == -1) { if (ef) break; return; }
        if (ch == -2) continue;
        if (ch == -3 || ch == -4) { s->tk = GT_STOP; return; }
        if (ch == -5 || ch == -6) { ef = 1; continue; }
        if (!ef) pl_gs_put(s, ch);
    }
    if (s->err) return;
    pl_gs_unget(s);
    pl_gs_fail(s, s->tk == GT_NAME ? "quote character expected here" : s->tk == GT_BQ ? "back quote character expected here" : "double quote character expected here", 1);
}
static void pl_gs_number(pl_gs_t *s) {
    int rc, ch, e0, d;
    size_t lg;
    do { pl_gs_put(s, s->c); pl_gs_get(s); } while (s->ty == GS_DI);
    lg = s->bk;
    d = s->c == '.' ? pl_gs_peek(s) : EOF;
    if (d >= '0' && d <= '9') {
        s->tk = GT_FLOAT;
        pl_gs_put(s, '.');
        for (pl_gs_get(s); s->ty == GS_DI; pl_gs_get(s)) pl_gs_put(s, s->c);
        if (s->c == 'e' || s->c == 'E') {
            e0 = s->c;
            pl_gs_get(s);
            d = (s->c == '+' || s->c == '-') ? pl_gs_peek(s) : EOF;
            if (!(s->ty == GS_DI || (d >= '0' && d <= '9'))) {
                pl_gs_unget(s);
                s->c = e0;
            } else {
                pl_gs_put(s, 'e');
                pl_gs_put(s, s->c);
                for (pl_gs_get(s); s->ty == GS_DI; pl_gs_get(s)) pl_gs_put(s, s->c);
            }
        }
        s->num = REALVAL(strtod(s->b, (char **)0));
        pl_gs_unget(s);
        return;
    }
    s->tk = GT_INT;
    if (!s->b || !pl_parse_number(s->b, &s->num)) s->num = INTVAL(0);
    if (lg != 1 || s->b[0] != '0' || !(s->c == '\'' || s->c == 'b' || s->c == 'o' || s->c == 'x')) { pl_gs_unget(s); return; }
    if (s->c == '\'') {
        ch = pl_gs_qchar(s, '\'', 0);
        if (ch == -1) { pl_gs_fail(s, "quote character expected here", 1); return; }
        if (ch == -2) { pl_gs_unget(s); ungetc('\\', s->in); ungetc('\'', s->in); return; }
        if (ch < 0) { pl_gs_unget(s); s->tk = GT_STOP; s->err = (const char *)0; pl_gs_fail(s, "character expected here", 1); return; }
        s->num = INTVAL(ch);
        return;
    }
    rc = s->c;
    s->bk = 0;
    pl_gs_put(s, '0');
    pl_gs_put(s, rc);
    for (pl_gs_get(s); s->c > 0 && (rc == 'x' ? isxdigit(s->c) : rc == 'o' ? (s->c >= '0' && s->c <= '7') : (s->c == '0' || s->c == '1')); pl_gs_get(s)) pl_gs_put(s, s->c);
    if (s->bk == 2) { pl_gs_unget(s); ungetc(rc, s->in); return; }
    if (!pl_parse_number(s->b, &s->num)) s->num = INTVAL(0);
    pl_gs_unget(s);
}
static void pl_gs_token(pl_gs_t *s, int comma_punct) {
    int lb = 0, c0;
    s->err = (const char *)0;
    s->q = 0;
    for (;;) {
        do { pl_gs_get(s); if (s->ty == GS_LA) lb = 1; } while (s->ty == GS_LA);
        pl_gs_at(s, 0);
        s->bk = 0;
        if (s->c == EOF) { s->tk = GT_EOF; return; }
        if (s->ty & (GS_SL | GS_UL | GS_CL)) {
            s->tk = s->ty == GS_SL ? GT_NAME : GT_VAR;
            do { pl_gs_put(s, s->c); pl_gs_get(s); } while (s->ty & (GS_UL | GS_CL | GS_SL | GS_DI));
            pl_gs_unget(s);
            return;
        }
        if (s->ty == GS_DI) { pl_gs_number(s); return; }
        if (s->ty & (GS_QT | GS_DQ | GS_BQ)) { pl_gs_quoted(s); return; }
        if (s->ty == GS_CM) { do pl_gs_get(s); while (s->c != '\n' && s->c != EOF); lb = 1; continue; }
        if (s->ty == GS_PC) { s->tk = (s->c == '(' && !lb) ? GT_IOPEN : GT_PUNCT; s->punct = s->c; return; }
        if (s->ty == GS_SC) { s->tk = (s->c == ',' && comma_punct) ? GT_PUNCT : GT_NAME; s->punct = s->c; pl_gs_put(s, s->c); return; }
        if (s->ty == GS_EX) { s->tk = GT_EXT; pl_gs_put(s, s->c); return; }
        c0 = s->c;
        pl_gs_get(s);
        if (c0 == '.' && (s->c == EOF || (s->ty & (GS_LA | GS_CM)))) { pl_gs_unget(s); s->tk = GT_STOP; return; }
        if (c0 == '/' && s->c == '*') {
            pl_gs_get(s);
            if (s->c != EOF) do { c0 = s->c; pl_gs_get(s); } while (s->c != EOF && (c0 != '*' || s->c != '/'));
            if (s->c == EOF) { s->tk = GT_EOF; pl_gs_at(s, 0); s->err = "*/ expected here for /*...*/ comment"; return; }
            lb = 1;
            continue;
        }
        s->tk = GT_NAME;
        pl_gs_put(s, c0);
        while (s->ty == GS_GR) { pl_gs_put(s, s->c); pl_gs_get(s); }
        pl_gs_unget(s);
        return;
    }
}
static void pl_gs_next_atom(pl_gs_t *s) {
    s->err = (const char *)0;
    s->bk = 0;
    do pl_gs_get(s);
    while (s->ty == GS_LA);
    pl_gs_at(s, 0);
    s->tk = GT_NAME;
    if (s->ty == GS_SL) {
        do { pl_gs_put(s, s->c); pl_gs_get(s); } while (s->ty & (GS_UL | GS_CL | GS_SL | GS_DI));
        pl_gs_unget(s);
    } else if (s->ty & (GS_QT | GS_BQ)) {
        pl_gs_quoted(s);
        s->tk = GT_NAME;
    } else if (s->ty == GS_GR) {
        while (s->ty == GS_GR) { pl_gs_put(s, s->c); pl_gs_get(s); }
        pl_gs_unget(s);
    } else if (s->ty == GS_SC) pl_gs_put(s, s->c);
    else { pl_gs_unget(s); s->err = "cannot start an atom (use quotes ?)"; }
}
static DESCR_t pl_gs_term(pl_gs_t *s) {
    char p[2] = { (char)s->punct, 0 };
    DESCR_t a;
    if (s->tk == GT_INT || s->tk == GT_FLOAT) return s->num;
    if (s->tk == GT_IOPEN || s->tk == GT_PUNCT) { if (s->tk == GT_IOPEN) p[0] = '('; return pl_gnu_fn1("punct", pl_mk_atom_dup(p, 1)); }
    if (s->tk == GT_STOP) return pl_gnu_fn1("punct", pl_mk_atom_dup("full_stop", 9));
    if (s->tk == GT_EOF) return pl_gnu_fn1("punct", pl_mk_atom_dup("end_of_file", 11));
    a = pl_mk_atom_dup(s->b ? s->b : "", s->bk);
    if (s->tk == GT_VAR) return pl_gnu_fn1("var", a);
    if (s->tk == GT_STR) return pl_gnu_fn1("string", a);
    if (s->tk == GT_BQ) return pl_gnu_fn1("back_quotes", a);
    if (s->tk == GT_EXT) return pl_gnu_fn1("extend", a);
    return a;
}
static DESCR_t pl_gs_read_number(pl_gs_t *s, int intonly) {
    extern DESCR_t rt_num_neg(DESCR_t);
    int neg = 0;
    pl_gs_token(s, 0);
    if (!s->err && s->tk == GT_NAME && s->bk == 1 && s->b[0] == '-') { neg = 1; pl_gs_token(s, 0); }
    if (s->err) return FAILDESCR;
    if (s->tk == GT_INT || (!intonly && s->tk == GT_FLOAT)) return neg ? rt_num_neg(s->num) : s->num;
    s->err = "invalid start of number";
    return FAILDESCR;
}
static void pl_gp_swap(pl_gp_t *p) { char *b = p->s.b; size_t n = p->s.bn; p->s.b = p->ub; p->s.bn = p->ubn; p->ub = b; p->ubn = n; }
static void pl_gp_unget(pl_gp_t *p) {
    size_t k = p->s.bk;
    pl_gp_swap(p);
    p->ubk = k;
    p->utk = p->s.tk;
    p->upunct = p->s.punct;
    p->uq = p->s.q;
    p->utl = p->s.tl;
    p->utc = p->s.tc;
    p->unum = p->s.num;
    p->have = 1;
}
static void pl_gp_setname(pl_gp_t *p, const char *nm) { p->s.bk = 0; for (; *nm; nm++) pl_gs_put(&p->s, *nm); }
static void pl_gp_fail(pl_gp_t *p, const char *msg) { if (!p->s.err) p->s.err = msg; }
static int pl_gp_is(pl_gp_t *p, int pc) { return p->s.tk == GT_PUNCT && p->s.punct == pc; }
static void pl_gp_next(pl_gp_t *p, int cp) {
    if (!p->have) { pl_gs_token(&p->s, cp); return; }
    pl_gp_swap(p);
    p->s.bk = p->ubk;
    p->s.tk = p->utk;
    p->s.punct = p->upunct;
    p->s.q = p->uq;
    p->s.tl = p->utl;
    p->s.tc = p->utc;
    p->s.num = p->unum;
    p->have = 0;
    if (cp && p->s.tk == GT_NAME && !p->s.q && p->s.bk == 1 && p->s.b[0] == ',') {
        p->s.tk = GT_PUNCT;
        p->s.punct = ',';
    } else if (!cp && p->s.tk == GT_PUNCT && p->s.punct == ',') {
        p->s.tk = GT_NAME;
        p->s.q = 0;
        pl_gp_setname(p, ",");
    }
}
static int pl_gp_op(const char *nm, int fix, int *prec, int *left, int *right) {
    extern int prolog_op_col_get(int, int, int *, int *);
    extern int prolog_atom_intern(const char *);
    int pr = 0, ty = 0;
    if (!prolog_op_col_get(prolog_atom_intern(nm), fix, &pr, &ty)) return 0;
    *prec = pr;
    *left = (ty == 3 || ty == 6) ? pr : pr - 1;
    *right = (ty == 2 || ty == 4) ? pr : pr - 1;
    return 1;
}
static int pl_gp_list(pl_gp_t *p, int can_be_empty) {
    int car = pl_gp_term(p, 999, GP_GENERAL, 1);
    if (p->s.err) return 0;
    pl_gp_next(p, 1);
    if (p->s.err) return 0;
    if (!car) { if (!can_be_empty) pl_gp_fail(p, "expression expected in list"); else if (!pl_gp_is(p, ']')) pl_gp_fail(p, "expression or ] expected in list"); return 0; }
    if (!pl_gp_is(p, ',') && !pl_gp_is(p, '|') && !pl_gp_is(p, ']')) { pl_gp_fail(p, ", | ] or operator expected in list"); return 0; }
    if (pl_gp_is(p, ',')) { pl_gp_list(p, 0); return 1; }
    if (pl_gp_is(p, '|')) {
        if (!pl_gp_term(p, 999, GP_GENERAL, 0)) { pl_gp_fail(p, "expression expected in list"); return 0; }
        pl_gp_next(p, 1);
        if (!p->s.err && !pl_gp_is(p, ']')) pl_gp_fail(p, "] or operator expected in list");
    }
    return 1;
}
static int pl_gp_bracketed(pl_gp_t *p, int pc) {
    int t;
    if (pc == '[') return pl_gp_list(p, 1);
    t = pl_gp_term(p, 1200, GP_GENERAL, 0);
    if (p->s.err) return 0;
    if (pc == '(' && !t) { pl_gp_fail(p, "expression expected"); return 0; }
    pl_gp_next(p, 0);
    if (!p->s.err && !pl_gp_is(p, pc == '(' ? ')' : '}')) pl_gp_fail(p, pc == '(' ? ") or operator expected" : "} or operator expected");
    return pc == '(' ? 1 : t;
}
static int pl_gp_args(pl_gp_t *p) {
    int n = 0;
    pl_gp_next(p, 0);
    do {
        if (p->s.err) return 0;
        if (n++ >= 255) { pl_gp_fail(p, "too big compound term (exceeds max_arity)"); return 0; }
        if (!pl_gp_term(p, 999, GP_GENERAL, 1)) { pl_gp_fail(p, "expression expected"); return 0; }
        pl_gp_next(p, 1);
    } while (!p->s.err && pl_gp_is(p, ','));
    if (!p->s.err && !pl_gp_is(p, ')')) pl_gp_fail(p, ", or ) expected");
    return 1;
}
static int pl_gp_name(pl_gp_t *p, int cur, int ctx, int cp, int *cur_left, int *left_is_op) {
    char nm[p->s.bk + 1];
    int pr, lf, rt, t;
    long sl, sc;
    memcpy(nm, p->s.b ? p->s.b : "", p->s.bk);
    nm[p->s.bk] = 0;
    if (pl_gs_peek(&p->s) == '(' && (strcmp(nm, ",") || p->s.q)) return pl_gp_args(p);
    if (!strcmp(nm, "-")) {
        sl = p->s.tl;
        sc = p->s.tc;
        pl_gp_next(p, 0);
        if (p->s.err) return 0;
        if (p->s.tk == GT_INT || p->s.tk == GT_FLOAT) return 1;
        pl_gp_unget(p);
        p->s.tk = GT_NAME;
        pl_gp_setname(p, "-");
        p->s.tl = sl;
        p->s.tc = sc;
    }
    if (pl_gp_op(nm, 0, &pr, &lf, &rt) && cur >= pr) {
        *cur_left = pr;
        t = pl_gp_term(p, rt, GP_PREFIX, cp);
        if (p->s.err) return 0;
        if (t) return 1;
        if (ctx != GP_GENERAL) { pl_gp_fail(p, "expression expected or previous operator needs brackets"); return 0; }
    }
    *left_is_op = pl_gp_op(nm, 0, &pr, &lf, &rt) || pl_gp_op(nm, 1, &pr, &lf, &rt) || pl_gp_op(nm, 2, &pr, &lf, &rt);
    if (*left_is_op && ctx != GP_GENERAL) {
        pl_gp_fail(p, (pl_gp_op(nm, 1, &pr, &lf, &rt) || pl_gp_op(nm, 2, &pr, &lf, &rt)) ? "current or previous operator needs brackets" : "current operator needs brackets");
        return 0;
    }
    return 1;
}
static int pl_gp_term(pl_gp_t *p, int cur, int ctx, int cp) {
    int t = 0, pr = 0, lf = 0, rt = 0, infix, cur_left = 0, left_is_op = 0, pc;
    pl_gp_next(p, ctx != GP_PREFIX || cp);
    if (p->s.err) return 0;
    if (p->s.tk == GT_VAR || p->s.tk == GT_INT || p->s.tk == GT_FLOAT || p->s.tk == GT_STR) t = 1;
    else if (p->s.tk == GT_NAME || p->s.tk == GT_BQ) t = pl_gp_name(p, cur, ctx, cp, &cur_left, &left_is_op);
    else if (p->s.tk == GT_IOPEN || (p->s.tk == GT_PUNCT && (p->s.punct == '(' || p->s.punct == '{' || p->s.punct == '['))) {
        pc = p->s.tk == GT_IOPEN ? '(' : p->s.punct;
        t = pl_gp_bracketed(p, pc);
        if (!t && !p->s.err) { p->s.tk = GT_NAME; p->s.q = 0; pl_gp_setname(p, pc == '{' ? "{}" : "[]"); t = pl_gp_name(p, cur, ctx, cp, &cur_left, &left_is_op); }
    } else {
        pl_gp_unget(p);
        return 0;
    }
    if (p->s.err) return 0;
    for (;;) {
        pl_gp_next(p, cp);
        if (p->s.err) return 0;
        if (pl_gp_is(p, '|') && pl_gp_op("|", 1, &pr, &lf, &rt) && pr > 1000 && cur >= pr) infix = 1;
        else {
            char nm[p->s.bk + 1];
            if (p->s.tk != GT_NAME) break;
            memcpy(nm, p->s.b ? p->s.b : "", p->s.bk);
            nm[p->s.bk] = 0;
            if (pl_gp_op(nm, 1, &pr, &lf, &rt)) infix = 1;
            else if (pl_gp_op(nm, 2, &pr, &lf, &rt)) infix = 0;
            else break;
        }
        if (left_is_op) { pl_gp_fail(p, "previous operator needs brackets"); return 0; }
        if (cur < pr || cur_left > lf) break;
        if (infix && !pl_gp_term(p, rt, GP_INSIDE, cp)) { pl_gp_fail(p, "right operand expected for infix operator"); return 0; }
        cur_left = pr;
    }
    pl_gp_unget(p);
    return t;
}
static const char *pl_gp_locate(pl_gp_t *p) {
    int t = pl_gp_term(p, 1200, GP_GENERAL, 0);
    if (!p->s.err) pl_gp_next(p, 0);
    if (p->s.err) return p->s.err;
    if (!t && p->s.tk == GT_EOF) return (const char *)0;
    if (!t) pl_gp_fail(p, "expression expected");
    else if (p->s.tk != GT_STOP) pl_gp_fail(p, ". or operator expected after expression");
    return p->s.err;
}
static void pl_gp_recover(pl_gs_t *s) {
    int c0, dot, out;
    if (feof(s->in)) return;
    for (;;) {
        if (pl_gs_get(s) == EOF) return;
        if (s->c == '.') { if (pl_gs_get(s) == EOF) return; if (s->ty & (GS_LA | GS_CM)) { pl_gs_unget(s); return; } }
        if (!(s->ty & (GS_QT | GS_DQ | GS_BQ))) continue;
        c0 = s->c;
        dot = 0;
        out = 0;
        while (!out) {
            if (pl_gs_get(s) == EOF) return;
            if (s->c == c0) break;
            if (s->c == '.') dot = 1;
            else if (!(s->ty & (GS_LA | GS_CM))) dot = 0;
            if (s->c == '\n') { if (dot) return; break; }
            if (s->c != '\\') continue;
            if (pl_gs_get(s) == EOF) return;
            if (s->c == '\n') continue;
            if (s->c == '.') dot = 1;
            if (s->c > 0 && strchr("\\'\"`abfnrtv", s->c)) continue;
            if (s->c != 'x' && (s->c < '0' || s->c > '7')) continue;
            for (;;) { if (pl_gs_get(s) == EOF) return; if (s->c == c0) { out = 1; break; } if (s->c == '\\' || !isxdigit(s->c)) break; }
        }
    }
}
static const char *pl_gs_msg(const char *fn, long l, long c, const char *e) {
    extern int prolog_atom_intern(const char *);
    extern const char *prolog_atom_name(int);
    int n = *fn ? snprintf((char *)0, 0, "%s:%ld (char:%ld) %s", fn, l, c, e) : snprintf((char *)0, 0, "(char:%ld) %s", c, e);
    char *b = pl_atom_carve_cx((size_t)n, (pl_tr_ctx_t *)0);
    if (!b) return e;
    if (*fn) snprintf(b, (size_t)n + 1, "%s:%ld (char:%ld) %s", fn, l, c, e);
    else snprintf(b, (size_t)n + 1, "(char:%ld) %s", c, e);
    return prolog_atom_name(prolog_atom_carved(b, (size_t)n, (size_t)n));
}
static void pl_gs_record(const char *fn, long l, long c, const char *e) {
    pl_flag_t *fl = pl_flag_find_or_create("$gnu_synerr");
    DESCR_t a[4];
    if (!fl) return;
    a[0] = pl_mk_atom_dup(fn, strlen(fn));
    a[1] = INTVAL(l);
    a[2] = INTVAL(c);
    a[3] = pl_mk_atom_dup(e, strlen(e));
    fl->term = pl_gnu_fn("$gnu_b_synerr", 4, a);
    fl->is_term = 1;
    fl->val = "$gnu_b_synerr";
}
static const char *pl_gs_file(int idx) { return (idx >= 0 && g_fh[idx].name) ? g_fh[idx].name : idx == 0 ? "user_input" : ""; }
const char *pl_gp_read_err(long off0) {
    extern FILE *fh_cur_in_fp(void);
    extern int fh_current_input(void);
    FILE *in = fh_cur_in_fp();
    long off1 = ftell(in);
    pl_gp_t p;
    const char *e;
    if (!pl_gnu_dialect() || off0 < 0 || off1 < 0 || fseek(in, off0, SEEK_SET) != 0) return (const char *)0;
    memset(&p, 0, sizeof p);
    p.s.in = in;
    p.s.idx = fh_current_input();
    e = pl_gp_locate(&p);
    if (!e || p.s.oom) { fseek(in, off1, SEEK_SET); return (const char *)0; }
    if (p.s.tk != GT_STOP) pl_gp_recover(&p.s);
    pl_gs_record(pl_gs_file(p.s.idx), p.s.tl, p.s.tc, e);
    return pl_gs_msg(pl_gs_file(p.s.idx), p.s.tl, p.s.tc, e);
}
int pl_gs_read(const char *op, DESCR_t res, pl_tr_ctx_t *cx) {
    extern FILE *fh_cur_in_fp(void);
    extern int fh_current_input(void);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    pl_gs_t s;
    DESCR_t t = FAILDESCR;
    const char *fn;
    int named = op[5] != 'i' && op[5] != 'n';
    memset(&s, 0, sizeof s);
    s.in = fh_cur_in_fp();
    s.idx = fh_current_input();
    if (op[5] == 't') {
        pl_gs_token(&s, 0);
        if (!s.err) t = pl_gs_term(&s);
    } else if (op[5] == 'a') {
        pl_gs_next_atom(&s);
        if (!s.err) t = pl_mk_atom_dup(s.b ? s.b : "", s.bk);
    } else t = pl_gs_read_number(&s, op[5] == 'i');
    if (s.oom) { cx->ball = rt_pl_ball_kind1("resource_error", "memory"); return 0; }
    if (!s.err) return plw_unify_vals(res, pl_gnu_fn1("ok", t), cx);
    fn = named ? pl_gs_file(s.idx) : "";
    pl_gs_record(fn, s.tl, s.tc, s.err);
    fn = pl_gs_msg(fn, s.tl, s.tc, s.err);
    return plw_unify_vals(res, pl_gnu_fn1("err", pl_mk_atom_dup(fn, strlen(fn))), cx);
}
