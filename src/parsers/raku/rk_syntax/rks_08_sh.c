#include "parsers/raku/rk_syntax/rks_head.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_08_sh.h"
#include "parsers/raku/rk_syntax/rks_09_sh.h"
static int rx_quantified_atom(RkP *p, int pos, RkLang *L);
static int rx_termseq(RkP *p, int pos, RkLang *L);
static int rx_assertion(RkP *p, int pos, RkLang *L);
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
static int bin_digit_cp(int c) { return digit_in_base(c, 2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int oct_digit_cp(int c) { return digit_in_base(c, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dec_digit_cp(int c) { return digit_in_base(c, 10); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hex_digit_cp(int c) { return digit_in_base(c, 16); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_baseint(RkP *p, int pos, int base) {
    return r_digits(p, pos, ck(base == 2 ? bin_digit_cp : base == 8 ? oct_digit_cp : base == 16 ? hex_digit_cp : dec_digit_cp));
}
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
int qq_backslash(RkP *p, int pos, RkLang *L) {
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
static void rx_unrec(RkP *p, int pos) {
    if (pos >= p->n) panic_at(p, pos, "Regex not terminated.");
    int c = cp_at(p, pos);
    if (is_vspace_cp(c)) panic_at(p, pos, "Regex not terminated.");
    panic_at(p, pos, "Unrecognized regex metacharacter %.*s (must be quoted to match literally)", cp_len(p, pos), p->s + pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_regex_nibbler(RkP *p, int pos, RkLang *L) {
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
            int s = NSPAN(p, t, ck(is_space_cp));
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
    if (!p->myst_off) {
        int nl = name_part_len(p, pos, e), last = -1;
        for (int i = pos; i + 1 < pos + nl; i++) if (p->s[i] == ':' && p->s[i + 1] == ':') last = i;
        if (last > pos && !is_name_n(p, p->s + pos, last - pos)) panic_at(p, pos, "Could not locate compile-time value for symbol %.*s", last - pos, p->s + pos);
    }
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
int rx_metachar(RkP *p, int pos, RkLang *L) {
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
static int rx_quantified_atom(RkP *p, int pos, RkLang *L) {
    if (pos >= p->n || rx_stopper(p, pos, L)) return -1;
    int c = ch(p, pos);
    if ((c == ')' && ch(p, pos + 1) != '>') || c == ']' || c == '}' || (c == '>' && ch(p, pos + 1) != '>')) return -1;
    if (c == '|' || c == '&') return -1;
    int a = rks_rx_atom(p, pos, L);
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
