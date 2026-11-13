#include "parsers/raku/rk_syntax/rks_head.h"
#include <string.h>
#include "ir/ct_arena.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
#include "parsers/raku/rk_syntax/rks_08_sh.h"
#include "parsers/raku/rk_syntax/rks_09_sh.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_alpha_cp(int c) {
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
int kw(RkP *p, int pos, const char *w) {
    int e = lit(p, pos, w);
    if (e < 0) return -1;
    if (wordch_at(p, e)) return -1;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int at_bol(RkP *p, int pos) {
    if (pos == 0) return 1;
    unsigned char b = (unsigned char) p->s[pos - 1];
    if (b == '\n' || b == '\r' || b == 0x0B || b == 0x0C) return 1;
    if (b == 0x85 && pos >= 2 && (unsigned char) p->s[pos - 2] == 0xC2) return 1;
    return (b == 0xA8 || b == 0xA9) && pos >= 3 && (unsigned char) p->s[pos - 2] == 0x80 && (unsigned char) p->s[pos - 3] == 0xE2;
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
    int sq = p->qsigil; int sb = p->build;
    p->qsigil = sigil; p->build = 0;
    int e = r_EXPR(p, pos, PR('y'));
    p->qsigil = sq; p->build = sb;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void interp_call(RkP *p, int pos, int sigil) {
    int sq = p->qsigil;
    p->qsigil = sigil;
    int e = r_EXPR(p, pos, PR('y'));
    p->qsigil = sq;
    RkList *X = takelst(p);
    if (e < 0 || !X) return;
    RK_GROW(p->qc, p->nqc, p->cqc, RkClosure);
    p->qc[p->nqc].pos = pos; p->qc[p->nqc].last = X; p->qc[p->nqc].cnt = 1; p->qc[p->nqc].end = e; p->nqc++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int interp_subscripted(RkP *p, int pos, int e) {
    int c = cp_at(p, pos + 1);
    if (!is_alpha_cp(c) && c != '_') return 0;
    for (int i = pos; i < e; i++) if (p->s[i] == '[' || p->s[i] == '{' || p->s[i] == '<') return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_quote_escape(RkP *p, int pos, RkLang *L) {
    int c = cp_at(p, pos);
    if (c == '\\') {
        if (L->bs) { int e = qq_backslash(p, pos + 1, L); if (e >= 0) return e; }
        else if (L->qbs) return q_backslash(p, pos + 1, L);
        return -1;
    }
    if (c == '{' && L->clos) {
        p->quote_block = 1; int e = r_block(p, pos);
        if (e >= 0 && p->build) { RK_GROW(p->qc, p->nqc, p->cqc, RkClosure); p->qc[p->nqc].pos = pos; p->qc[p->nqc].last = p->blk_last; p->qc[p->nqc].cnt = p->blk_n; p->qc[p->nqc].end = e; p->nqc++; }
        return e;
    }
    if (c == '$' && L->sc) {
        int e = interp_sigil(p, pos, '$');
        if (e < 0) panic_at(p, pos, "Non-variable $ must be backslashed");
        if (p->build && (memchr(p->s + pos, '(', (size_t) (e - pos)) || interp_subscripted(p, pos, e))) interp_call(p, pos, '$');
        return e;
    }
    if (c == '@' && L->ar) { int e = interp_sigil(p, pos, '@'); if (e >= 0 && p->build) interp_call(p, pos, '@'); return e; }
    if (c == '%' && L->hs) { int e = interp_sigil(p, pos, '%'); if (e >= 0 && p->build) interp_call(p, pos, '%'); return e; }
    if (c == '&' && L->fn) { int e = interp_sigil(p, pos, '&'); if (e >= 0 && p->build) interp_call(p, pos, '&'); return e; }
    if (L->ww) {
        if (c == '\'' || c == '"' || c == 0x2018 || c == 0x201A || c == 0x2019 || c == 0x201C || c == 0x201E || c == 0x201D || c == 0xFF62) return r_quote(p, pos);
        if (c == ':' && (is_alpha_cp(cp_at(p, pos + 1)) || ch(p, pos + 1) == '!' || ch(p, pos + 1) == '$' || ch(p, pos + 1) == '@')) { int e = r_colonpair(p, pos); if (e >= 0) return e; }
        if (c == '#') return r_comment(p, pos);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_circumfix(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c == '(') { int sa = p->invocant_ok; int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); RkList *in = p->last_list; int n = p->last_nstmts; p->invocant_ok = sa;
        p->goal = sg; e = expect_close(p, e, ')', "parenthesized expression", pos);
        if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_PAREN; p->tm.from = pos; p->tm.to = p->tm.core_to = e; p->tm.cnt = n; p->tm.in = in; if (in && in->tc && in->n == 1) in->trailing = 1; p->tm.t = rkb_paren(p->B, in, n); }
        return e; }
    if (c == '[') { int sg = p->goal; p->goal = 0; int e = r_semilist(p, pos + 1); RkList *in = p->last_list; p->goal = sg; e = expect_close(p, e, ']', "array composer", pos);
        if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE; p->tm.from = pos; p->tm.to = p->tm.core_to = e; p->tm.t = rkb_bracket(p->B, in); }
        return e; }
    if (c == '{') { int sg = p->goal; p->goal = 0; int e = r_pblock(p, pos, 1); p->goal = sg;
        if (p->build) rkb_block_term(p->B, &p->tm, pos, e, p->tv, p->pb_sig, 0, p->blk_last, p->blk_n);
        return e; }
    if (at_lit(p, pos, "<<")) return words_quote(p, pos, '<', 2, 1);
    if (c == 0xAB) return words_quote(p, pos, 0xAB, 1, 1);
    if (c == '<') {
        if (ch(p, pos + 1) == '>') panic_at(p, pos, "Unsupported use of <>; in Raku please use lines() to read input, ('') to represent a null string or () to represent an empty list");
        if (at_lit(p, pos, "<STDIN>")) panic_at(p, pos, "Unsupported use of <STDIN>; in Raku please use $*IN.lines (or add whitespace to suppress warning)");
        return words_quote(p, pos, '<', 1, 0);
    }
    return -1;
}
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
int r_variable(RkP *p, int pos) {
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
            if (d >= 0 && twig == '*' && !p->in_decl) note_core_use(p, pos, d - pos);
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
int r_colonpair(RkP *p, int pos) {
    if (ch(p, pos) != ':') return -1;
    int q = pos + 1;
    int c = cp_at(p, q);
    if (c == '!') {
        int e = r_identifier(p, q + 1);
        if (e < 0) panic_at(p, q, "Malformed False pair; expected identifier");
        int d = ch(p, e);
        if (d == '[' || d == '(' || d == '<' || d == '{') panic_at(p, pos, "Argument not allowed on negated pair with key '%.*s'", e - q - 1, p->s + q + 1);
        if (p->build) rkb_colonpair(p->B, &p->tm, pos, e, '!', q + 1, e, NULL, 0, 0, 0);
        return e;
    }
    if (is_digit_cp(c)) {
        int d = r_decint(p, q);
        int e = r_identifier(p, d);
        if (e < 0) return -1;
        int x = ch(p, e);
        if (x == '[' || x == '(' || x == '<' || x == '{') panic_at(p, e, "Extra argument not allowed; pair already has argument of %.*s", d - q, p->s + q);
        if (p->build) rkb_colonpair(p->B, &p->tm, pos, e, 'd', d, e, NULL, 0, 0, 0);
        return e;
    }
    if (is_alpha_cp(c)) {
        int e = r_identifier(p, q);
        int t = e; int u = r_unsp(p, t); if (u >= 0) t = u;
        int d = cp_at(p, t);
        if (d == '(' || d == '[' || d == '{' || d == '<' || d == 0xAB) {
            if (at_lit(p, t, "<>")) { if (p->build) rkb_colonpair(p->B, &p->tm, pos, t + 2, 'v', q, e, NULL, t + 1, t + 1, 'W'); return t + 2; }
            int f = r_circumfix(p, t);
            if (f >= 0) {
                if (p->build) {
                    RkTerm v = p->tm; int vk = d == '(' ? 'P' : d == '<' || d == 0xAB ? 'W' : d == '[' ? 'B' : 'C';
                    tree_t *vt = v.t; if (vk == 'P' && v.cnt == 0) vt = NULL;
                    int wf = t + (d == 0xAB ? 2 : at_lit(p, t, "<<") ? 2 : 1), wt = f - (d == 0xAB ? 2 : at_lit(p, t, "<<") ? 2 : 1);
                    rkb_colonpair(p->B, &p->tm, pos, f, 'v', q, e, vt, wf, wt, vk);
                }
                return f;
            }
        }
        if (p->build) rkb_colonpair(p->B, &p->tm, pos, e, 'n', q, e, NULL, 0, 0, 0);
        return e;
    }
    if (c == '(') {
        int s = r_signature(p, q + 1, 1);
        s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        if (p->build) rkb_colonpair(p->B, &p->tm, pos, s + 1, 's', pos, pos, NULL, 0, 0, 0);
        return s + 1;
    }
    if (c == '<' || c == '[' || c == '{' || c == 0xAB) {
        if (at_lit(p, q, "<>")) { if (p->build) rkb_colonpair(p->B, &p->tm, pos, q + 2, 'c', pos, pos, NULL, 0, 0, 0); return q + 2; }
        if (c == '{' && p->in_reduce) return -1;
        int f = r_circumfix(p, q);
        if (f >= 0 && p->build) { tree_t *vt = p->tm.t; rkb_colonpair(p->B, &p->tm, pos, f, 'c', pos, pos, vt, 0, 0, 'P'); }
        return f;
    }
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        int t = q + 1;
        if (ch(p, t) == '<') { int e = r_desigilname(p, t + 1); if (e < 0 || ch(p, e) != '>') return -1; if (p->build) rkb_colonpair(p->B, &p->tm, pos, e + 1, '<', t + 1, e, NULL, 0, 0, 0);
            return e + 1; }
        if (is_twigil_at(p, t)) t++;
        int e = r_desigilname(p, t);
        if (e < 0) return -1;
        if (p->build) { RkTerm v; rkb_var(p->B, &v, q, e, NULL, 0); rkb_colonpair(p->B, &p->tm, pos, e, '$', t, e, v.t, 0, 0, 0); }
        return e;
    }
    if (p->build) rkb_colonpair(p->B, &p->tm, pos, pos + 1, 'c', pos, pos, NULL, 0, 0, 0);
    return -1;
}
int user_routine_n(RkP *p, const char *s, int n) {
    char *buf = (char *) ct_alloc((size_t) n + 1);
    buf[0] = '&';
    memcpy(buf + 1, s, (size_t) n);
    return user_name_index(p, buf, n + 1) >= 0;
}
int qok(RkP *p, int pos, const char *word) {
    if (wordch_at(p, pos)) return -1;
    if (ch(p, pos) == '(') return -1;
    if (ch(p, pos) != ':' && (is_name_n(p, word, (int) strlen(word)) || user_routine_n(p, word, (int) strlen(word)))) return -1;
    int h = NSPAN(p, pos, ck(is_space_cp));
    if (ch(p, h) == '#') panic_at(p, h, "# not allowed as delimiter");
    if (ch(p, h) == ')' || h >= p->n || (ch(p, h) == '=' && ch(p, h + 1) == '>')) return -1;
    return ws(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_lambda_term(RkP *p, int pos) {
    if (!at_lambda(p, pos)) return -1;
    int e = r_pblock(p, pos, 2);
    if (e >= 0 && p->build) rkb_block_term(p->B, &p->tm, pos, e, p->tv, p->pb_sig, 0, p->blk_last, p->blk_n);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_term(RkP *p, int pos) {
    int c = cp_at(p, pos);
    if (c < 0) return -1;
    int e;
    if (p->nuops && (e = r_user_term(p, pos)) >= 0) { if (p->build) { if (p->ucf_c) { p->ucf_c = 0; rkb_circumfix_call(p->B, &p->tm, pos, e, p->ucf_open, p->ucf_in); } else rkb_user_term(p->B, &p->tm, pos, e); } return e; }
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        if (c == '&' && ch(p, pos + 1) == '&') return -1;
        if (c == '@' && ch(p, pos + 1) == '(' && !p->in_decl && !p->qsigil) {
            e = r_circumfix(p, pos + 1);
            if (e >= 0 && p->build) { rkb_contextualize(p->B, &p->tm, "list"); p->tm.from = pos; }
            return e;
        }
        if ((c == '$' || c == '%') && ch(p, pos + 1) == '(' && !p->in_decl && !p->qsigil) {
            e = r_circumfix(p, pos + 1);
            if (e >= 0 && p->build) { if (c == '$' && p->tm.kind == TK_PAREN && p->tm.cnt > 1) p->tm.t = rkb_paren_last(p->B, p->tm.in); rkb_contextualize(p->B, &p->tm, c == '$' ? "item" : "hash"); p->tm.from = pos; }
            return e;
        }
        if (c == '$' && ch(p, pos + 1) == '[' && !p->in_decl && !p->qsigil) {
            e = r_circumfix(p, pos + 1);
            if (e >= 0 && p->build) { rkb_contextualize(p->B, &p->tm, "item"); p->tm.from = pos; }
            return e;
        }
        if ((c == '@' || c == '%') && ch(p, pos + 1) == '$' && (is_alpha_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == '_') && !p->in_decl && !p->qsigil) {
            e = r_variable(p, pos + 1);
            if (e >= 0 && p->build) { rkb_var(p->B, &p->tm, pos + 1, e, NULL, 0); rkb_contextualize(p->B, &p->tm, c == '@' ? "list" : "hash"); p->tm.from = pos; }
            return e;
        }
        e = r_variable(p, pos);
        if (e >= 0 && p->build && c == '$' && ch(p, pos + 1) == '{' && !p->in_decl && !p->qsigil) { rkb_contextualize(p->B, &p->tm, "item"); p->tm.from = pos; return e; }
        if (e >= 0 && p->build) { if ((c == '$' || c == '@') && ch(p, pos + 1) == '<') rkb_var(p->B, &p->tm, pos, e, p->s + pos + 2, e - pos - 3); else rkb_var(p->B, &p->tm, pos, e, NULL, 0); }
        if (e >= 0 && p->build && c == '&' && p->ind_t && p->ind_end == e && at_lit(p, pos + 1, "::(")) { rkb_indirect_code(p->B, &p->tm, p->ind_t); p->ind_t = NULL; return e; }
        if (e >= 0 && p->build && (c == '$' || c == '@' || c == '%') && p->ind_t && p->ind_t->t == TT_QLIT && p->ind_end == e && at_lit(p, pos + 1, "::(")) {
            rkb_indirect_var(p->B, &p->tm, p->ind_t, (char) c);
            p->ind_t = NULL;
            return e;
        }
        if (e >= 0 && p->build && p->tm.kind == TK_VAR && p->tm.t && p->tm.t->t == TT_VAR) { int ni = user_name_index(p, p->s + pos, e - pos); if (ni >= 0 && p->names[ni].st) rkb_rename_var(p->B, &p->tm, p->names[ni].st); }
        return e;
    }
    if (is_digit_cp(c) || (c == '.' && is_digit_cp(cp_at(p, pos + 1)))) { e = r_numish(p, pos); if (e >= 0 && p->build) rkb_number(p->B, &p->tm, pos, e); return e; }
    if (c == ':') {
        if (is_digit_cp(cp_at(p, pos + 1))) { int d = r_decint(p, pos + 1); int x = ch(p, d); if (x == '<' || x == '[' || x == '(' || x == '\\') { e = r_rad_number(p, pos); if (e >= 0 && p->build)
            rkb_number(p->B, &p->tm, pos, e); return e; } }
        if (ch(p, pos + 1) == ':') { if (at_lit(p, pos, "::?") && is_alpha_cp(cp_at(p, pos + 3))) { e = r_identifier(p, pos + 3); if (e >= 0 && p->build) rkb_name(p->B, &p->tm, pos, e, e - pos);
            return e; }
            return r_term_name(p, pos); }
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
    if (c == '*') { e = at_lit(p, pos, "**") ? pos + 2 : pos + 1; if (p->build) { rkb_name(p->B, &p->tm, pos, e, e - pos); p->tm.kind = TK_STAR; } return e; }
    if (at_lit(p, pos, "...") || c == 0x2026) {
        int q = c == 0x2026 ? pos + 3 : pos + 3;
        if (at_lit(p, pos, "...^") || at_lit(p, pos, "\xe2\x80\xa6^")) return -1;
        e = r_args(p, q, 0);
        if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE; p->tm.from = pos; p->tm.to = p->tm.core_to = e; p->tm.t = ast_node_new(TT_YADA); }
        return e;
    }
    if (at_lit(p, pos, "???") || at_lit(p, pos, "!!!")) { e = r_args(p, pos + 3, 0); if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE; p->tm.from = pos;
        p->tm.to = p->tm.core_to = e;
        p->tm.t = ast_node_new(TT_YADA); } return e; }
    if (at_lit(p, pos, "!!") && is_space_cp(cp_at(p, pos + 2))) { if (p->build) rkb_name(p->B, &p->tm, pos, pos + 2, 2); return pos + 2; }
    if (c == '.') {
        if (at_lit(p, pos, "..")) return -1;
        e = r_dotty(p, pos);
        if (e >= 0 && p->build) rkb_method_term(p->B, &p->tm, &p->pf);
        return e;
    }
    if (c == 0x2205) { int q = pos + 3; if (!(ch(p, q) == '(' || ch(p, q) == '\\' || ch(p, q) == '\'' || ch(p, q) == '-') && !at_lit(p, hs(p, q), "=>")) { if (p->build) rkb_name(p->B, &p->tm, pos, q,
        q - pos); return q; } }
    if (c == 0x221E) { if (p->build) rkb_name(p->B, &p->tm, pos, pos + 3, 3); return pos + 3; }
    if (c >= 0x80 && is_numeric_other_cp(c) && !is_alpha_cp(c)) { e = r_numish(p, pos); if (e >= 0 && p->build) rkb_number(p->B, &p->tm, pos, e); return e; }
    if (!is_alpha_cp(c)) return -1;
    if ((e = r_fatarrow(p, pos)) >= 0) return e;
    if ((e = r_quote(p, pos)) >= 0) return e;
    if ((e = r_keyword_term(p, pos)) >= 0) return e;
    if (c == 'v' && (e = r_version(p, pos)) >= 0) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    if ((e = kw(p, pos, "NaN")) >= 0) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    if ((e = kw(p, pos, "Inf")) >= 0) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    if ((e = r_term_identifier(p, pos)) >= 0) return e;
    return r_term_name(p, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_post_constraint(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 0;
    int e = -1; int q;
    if (ch(p, pos) == '[' || ch(p, pos) == '(') {
        int close = ch(p, pos) == '[' ? ']' : ')';
        int s = r_signature(p, pos + 1, 0); s = ws(p, s);
        if (ch(p, s) != close) panic_at(p, s, "Unable to parse signature; couldn't find final '%c'", close);
        e = s + 1;
    }
    else if ((q = kw(p, pos, "where")) >= 0) {
        int t = ws(p, q);
        e = r_EXPR(p, t, PR('i'));
        if (e < 0) panic_at(p, t, "Missing expression after 'where'");
        if (p->build) { tree_t *wx = rkb_expr(p->B, takelst(p)); if (!p->pwhere) p->pwhere = wx; }
    }
    p->in_decl = sd;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_sublongname(RkP *p, int pos) {
    int e = r_desigilname(p, pos);
    if (e < 0) return -1;
    if (at_lit(p, e, ":(")) { int s = r_signature(p, e + 2, 1); s = ws(p, s); if (ch(p, s) == ')') e = s + 1; }
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
static tree_t *param_var_of(tree_t *tv) { if (tv && tv->t == TT_ASSIGN && tv->n) tv = tv->c[0]; return tv && tv->t == TT_VAR ? tv : NULL; }
static void mark_param_traits(tree_t *tv, const RkTrait *tr, int n, int pref) {
    tree_t *v = param_var_of(tv);
    if (!v) return;
    for (int i = 0; i < n; i++) if (tr[i].word && tr[i].name && !strcmp(tr[i].word, "is")) { if (!strcmp(tr[i].name, "copy")) v->slen |= RK_PV_COPY; if (!strcmp(tr[i].name, "rw")) v->slen |= RK_PV_RW; }
    if (pref == '\\') v->slen |= RK_PV_RW | RK_PV_RAW;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_parameter(RkP *p, int pos) {
    int q = pos; int e = -1; int ntc = 0;
    int *tf = NULL, *tt = NULL, ctf = 0, ctt = 0; int pref = 0, vfrom = -1, vto = -1, suffix = 0, named = 0, keyfrom = -1, keyto = -1;
    for (;;) { int t = r_type_constraint(p, q); if (t < 0) break; RK_GROW(tf, ntc, ctf, int); RK_GROW(tt, ntc, ctt, int); tf[ntc] = q;
        { int te = t; while (te > q && asc_space((unsigned char) p->s[te - 1])) te--; tt[ntc] = te; } q = t; ntc++;
        }
    int c = ch(p, q);
    if (at_lit(p, q, "**") || c == '*' || c == '+') {
        int l = at_lit(p, q, "**") ? 2 : 1;
        int v = r_param_var(p, q + l);
        pref = l == 2 ? 'L' : c;
        if (v >= 0) { e = v; vfrom = q + l; vto = v; }
        else if (c == '+' && is_alpha_cp(cp_at(p, q + 1))) { e = r_identifier(p, q + 1); if (e >= 0) { add_value_name(p, p->s + q + 1, e - q - 1); vfrom = q + 1; vto = e; } }
        else if (c == '+') e = q + 1;
    }
    else if (c == '\\' || c == '|') {
        int d = ch(p, q + 1);
        if (d == '$' || d == '@' || d == '%' || d == '&') {
            if (r_param_var(p, q + 1) >= 0) panic_at(p, q, "Obsolete use of | or \\ with sigil on param %.*s", 2, p->s + q + 1);
        }
        int t = r_identifier(p, q + 1);
        e = t >= 0 ? t : q + 1;
        pref = c; vfrom = q + 1; vto = e;
        if (t >= 0) add_value_name(p, p->s + q + 1, t - q - 1);
    }
    else {
        int v = r_param_var(p, q);
        if (v >= 0) { vfrom = q; vto = v; }
        if (v < 0) { v = r_named_param(p, q); if (v >= 0) { named = 1; int k = q; while (k < v && p->s[k] != '$' && p->s[k] != '@' && p->s[k] != '%' && p->s[k] != '&') k++; vfrom = k; vto = v;
            while (vto > vfrom && p->s[vto - 1] == ')') vto--;
            int ki = r_identifier(p, q + 1); if (ki >= 0 && ch(p, ki) == '(') { keyfrom = q + 1; keyto = ki; } } }
        if (v >= 0) { e = v; if (ch(p, e) == '?' || ch(p, e) == '!') { suffix = ch(p, e); e++; } }
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
    RkTrait *ptr = p->tr; int nptr = p->ntr;
    p->pwhere = NULL;
    tree_t *subsig = NULL;
    for (;;) { int sgn = ch(p, q) == '[' || ch(p, q) == '('; if (sgn) p->sig = NULL; int t = r_post_constraint(p, q); if (t < 0) break; if (sgn && p->sig) subsig = p->sig; q = ws(p, t); }
    tree_t *pw = p->pwhere;
    p->pwhere = NULL;
    int d = r_default_value(p, q);
    RkList *dflt = d >= 0 ? takelst(p) : NULL;
    if (p->build) { p->tv = rkb_param(p->B, pref, vfrom >= 0 ? p->s + vfrom : NULL, vfrom >= 0 ? vto - vfrom : 0, tf, tt, ntc, dflt, suffix, named); mark_param_traits(p->tv, ptr, nptr, pref); if (keyfrom >= 0) rkb_param_key(p->B, p->tv, p->s + keyfrom, keyto - keyfrom); if (subsig) { tree_t *sv = param_var_of(p->tv); if (sv) ast_push(sv, subsig); } if (pw) { tree_t *wv = param_var_of(p->tv); if (wv) { tree_t *wc = ast_node_new(TT_FNC); tree_t *wn = ast_node_new(TT_VAR); wc->v.sval = (char *) "__rk_where"; wn->v.sval = (char *) "__rk_where"; ast_push(wc, wn); ast_push(wc, pw); ast_push(wv, wc); } } }
    if (d >= 0) {
        q = ws(p, d);
        int t = r_trait(p, q);
        if (t >= 0) panic_at(p, q, "Cannot put trait on parameter after its default value");
        if (kw(p, q, "where") >= 0) panic_at(p, q, "Cannot put post constraint on parameter after its default value");
    }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_signature(RkP *p, int pos, int allow_invocant) {
    int sd = p->in_decl; p->in_decl = 1;
    int q = ws(p, pos);
    tree_t *S = p->build ? ast_node_new(TT_SEQ_EXPR) : NULL;
    for (;;) {
        if (!sig_end_at(p, q)) {
            p->tv = NULL;
            int e = r_parameter(p, q);
            if (e < 0) break;
            if (S && p->tv) ast_push(S, taketv(p));
            q = e;
        }
        int t = ws(p, q);
        if (ch(p, t) == ',' || (ch(p, t) == ':' && !is_space_cp(cp_at(p, t + 1)) && 0)) { q = ws(p, t + 1); continue; }
        if (ch(p, t) == ':' && allow_invocant && !at_lit(p, t, "::")) { if (S && S->n == 1) { tree_t *iv = param_var_of(S->c[0]); if (iv) iv->slen |= RK_PV_INV; } q = ws(p, t + 1); continue; }
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
    p->sig = S;
    return q;
}
int r_pblock(RkP *p, int pos, int implicit) {
    if (at_lambda(p, pos)) {
        int q = at_lit(p, pos, "<->") ? pos + 3 : pos + 2;
        scope_enter(p);
        int sg = p->goal; p->goal = GOAL_BLOCK;
        int s = r_signature(p, q, 0);
        tree_t *sig = p->sig;
        if (sig && at_lit(p, pos, "<->")) for (int i = 0; i < sig->n; i++) { tree_t *v = param_var_of(sig->c[i]); if (v) v->slen |= RK_PV_RW; }
        p->goal = sg;
        s = ws(p, s);
        if (ch(p, s) != '{') panic_at(p, s, "Missing block");
        int e = r_blockoid(p, s);
        p->pb_sig = sig;
        scope_leave(p);
        return e;
    }
    if (ch(p, pos) == '{') { scope_enter(p); int e = r_blockoid(p, pos); p->pb_sig = NULL; scope_leave(p); return e; }
    (void) implicit;
    if (pos > 0 && p->s[pos - 1] == '}') panic_at(p, pos, "Missing block (whitespace needed before curlies taken as a hash subscript?)");
    panic_at(p, pos, "Missing block");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_type_declarator(RkP *p, int pos) {
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
        tree_t *et = NULL;
        if (p->build) et = p->tm.kind == TK_WORDS && p->tm.name ? rkb_enum(p->B, p->tm.name, (int) strlen(p->tm.name), en, en + enl) : p->tm.t ? rkb_enum_tree(p->B, p->tm.t, en, en + enl) : ast_node_new(TT_SEQ_EXPR);
        enum_names_from(p, q, t, p->s + en, enl);
        for (int k = q, quo = 0; k < t; k++) {
            unsigned char b = (unsigned char) p->s[k];
            if (b == '\'' || b == '"') quo = !quo;
            else if (!quo && (b == '.' || b == '~' || b == '*' || b == '+' || b == '^' || b == '$' || b == '@' || b == '&')) { p->myst_off = 1; break; }
        }
        if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE; p->tm.from = pos; p->tm.to = p->tm.core_to = t; p->tm.t = et; }
        return ws(p, t);
    }
    if ((e = kok(p, pos, "subset")) >= 0) {
        int q = e;
        int n = r_longname(p, q);
        const char *sname = NULL; int snlen = 0;
        if (n >= 0) { sname = p->s + q; snlen = name_part_len(p, q, n); add_name_all(p, sname, snlen); q = ws(p, n); }
        q = r_traits(p, q); q = ws(p, q);
        const char *sbase = NULL;
        for (int i = 0; i < p->ntr; i++) if (p->tr[i].word && p->tr[i].name && !strcmp(p->tr[i].word, "of")) sbase = p->tr[i].name;
        int w = kw(p, q, "where");
        if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE; p->tm.t = ast_node_new(TT_NUL); }
        if (w >= 0) { int t = ws(p, w); int x = r_EXPR(p, t, PR('e')); if (x < 0) panic_at(p, t, "Malformed subset");
            if (p->build) { tree_t *ex = rkb_expr(p->B, takelst(p)); p->tm.kind = TK_TREE; p->tm.t = sname ? rkb_subset(p->B, sname, snlen, sbase, ex) : ast_node_new(TT_NUL); }
            return x; }
        if (p->build && sname) { p->tm.kind = TK_TREE; p->tm.t = rkb_subset(p->B, sname, snlen, sbase, NULL); }
        return q;
    }
    if ((e = kok(p, pos, "constant")) >= 0) {
        int q = e;
        int sd = p->in_decl; p->in_decl = 1;
        if (ch(p, q) == '\\') q++;
        int cnf = q;
        int n = r_identifier(p, q);
        if (n >= 0) { while (ch(p, n) == ':' && ch(p, n + 1) != ':' && ch(p, n + 1) != '=') { int f = r_colonpair(p, n); if (f < 0) break; n = f; } add_name_n(p, p->s + q, name_part_len(p, q, n));
            { char *cn = ct_strndup0(p->s + q, name_part_len(p, q, n)); if (rk_is_core_type(cn)) p->names[p->nnames - 1].value = 1; }
            register_user_op(p, q, n); q = n; }
        else if (ch(p, q) == '$' || ch(p, q) == '@' || ch(p, q) == '%' || ch(p, q) == '&') { int v = r_variable(p, q); if (v >= 0) { if (ch(p, q) == '&') register_user_op(p, q + 1, v); q = v; } }
        p->in_decl = sd;
        q = ws(p, q);
        q = r_traits(p, q); q = ws(p, q);
        int cnt_ = n >= 0 ? n : q;
        int i = r_initializer(p, q);
        if (i < 0) panic_at(p, q, "Missing initializer on constant declaration");
        if (p->build) { RkList *ci = takelst(p); int ce = cnt_; while (ce > cnf && asc_space((unsigned char) p->s[ce - 1])) ce--; memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_TREE;
            p->tm.from = pos;
            p->tm.to = p->tm.core_to = i; p->tm.t = rkb_constant(p->B, p->s + cnf, ce - cnf, ci); }
        return i;
    }
    return -1;
}
