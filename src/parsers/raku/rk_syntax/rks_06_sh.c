#include "parsers/raku/rk_syntax/rks_head.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
static int r_package_def(RkP *p, int pos, const char *kind);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int asc_word(int b) { return (b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z') || (b >= '0' && b <= '9') || b == '_' || b >= 0x80; }
char *ct_strndup0(const char *s, int n) { char *r = (char *) ct_alloc((size_t) n + 1); memcpy(r, s, (size_t) n); r[n] = 0; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_name_n(RkP *p, const char *s, int n) {
    if (n <= 0) return;
    RK_GROW(p->names, p->nnames, p->cnames, RkName);
    p->names[p->nnames].name = ct_strndup0(s, n);
    p->names[p->nnames].depth = p->depth;
    p->names[p->nnames].value = 0;
    p->names[p->nnames].st = NULL;
    p->nnames++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_value_name(RkP *p, const char *s, int n) { add_name_n(p, s, n); if (p->nnames) p->names[p->nnames - 1].value = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_our_name(RkP *p, const char *s, int n) { add_name_n(p, s, n); if (p->nnames && p->scope != 1) p->names[p->nnames - 1].depth = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_name_all(RkP *p, const char *s, int n) {
    add_our_name(p, s, n);
    for (int i = 0; i + 1 < n; i++) if (s[i] == ':' && s[i + 1] == ':') { add_our_name(p, s, i); add_our_name(p, s + i + 2, n - i - 2); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int user_name_index(RkP *p, const char *s, int n) {
    for (int i = p->nnames - 1; i >= 0; i--) if ((int) strlen(p->names[i].name) == n && !memcmp(p->names[i].name, s, (size_t) n)) return i;
    return -1;
}
const char *state_name_of(void *ctx, const char *s, int n) { RkP *p = (RkP *) ctx; int i = user_name_index(p, s, n); return i >= 0 ? p->names[i].st : NULL; }
int is_type_n(RkP *p, const char *s, int n) {
    static const char *const vals[] = { "True", "False", "Less", "Same", "More", "Inf", "NaN", "pi", "e", "i", "tau", "Empty", "Nil", 0 };
    for (int i = 0; vals[i]; i++) if ((int) strlen(vals[i]) == n && !memcmp(vals[i], s, (size_t) n)) return 0;
    if (n >= 2 && s[n - 1] == ':' && s[n - 2] == ':') return 0;
    int u = user_name_index(p, s, n);
    if (u >= 0 && p->names[u].value) return 0;
    return is_name_n(p, s, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_routine_name(RkP *p, const char *s, int n) {
    if (n <= 0) return;
    char *buf = (char *) ct_alloc((size_t) n + 1); buf[0] = '&'; memcpy(buf + 1, s, (size_t) n);
    add_name_n(p, buf, n + 1);
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
static int colonpair_ahead(RkP *p, int pos) {
    if (ch(p, pos) != ':') return 0;
    int c = cp_at(p, pos + 1);
    return is_alpha_cp(c) || c == '<' || c == '[' || c == 0xAB;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_longname(RkP *p, int pos) {
    int e = r_name(p, pos);
    if (e < 0) return -1;
    while (colonpair_ahead(p, e)) { int c = r_colonpair(p, e); if (c < 0) break; e = c; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_deflongname(RkP *p, int pos) {
    int e = r_name(p, pos);
    if (e < 0) return -1;
    while (ch(p, e) == ':') { int c = r_colonpair(p, e); if (c < 0) break; e = c; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_nulltermish(RkP *p, int pos) {
    if (is_terminator_at(p, pos) || pos >= p->n) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_EMPTY; return pos; }
    int e = r_termish(p, pos);
    if (e < 0) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_EMPTY; }
    return e >= 0 ? e : pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_EXPR(RkP *p, int pos, int preclim) {
    RkX x; memset(&x, 0, sizeof x);
    x.sl = p->leftsigil; p->leftsigil = 0;
    x.preclim = preclim; x.noinfix = preclim == PR('y'); x.base = p->nops; x.q = pos; x.nextterm = NT_TERMISH; x.last_end = -1;
    x.L = rkb_list_new();
    x.star = p->x_star; x.subst = p->x_subst; x.subst_len = p->x_subst_len;
    p->x_star = 0; p->x_subst = NULL;
    int ncur = p->build ? rkb_nocurry(p->B, x.star) : 0;
    x_list(p, &x);
    if (p->build) rkb_nocurry(p->B, ncur);
    p->nops = x.base; p->lst = x.L;
    if (x.fail) { p->leftsigil = x.sl; return -1; }
    p->leftsigil = x.sl ? x.sl : p->leftsigil;
    return x.last_end;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void strs_add(RkStrs *a, const char *s, int n) { RK_GROW(a->v, a->n, a->c, char *); a->v[a->n++] = ct_strndup0(s, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void record_attribute(RkP *p, int sigil, const char *name, int n, int pub) {
    if (!p->npkgs || n <= 0) return;
    if (pub) strs_add(&p->pkgs[p->npkgs - 1].meths, name, n);
    char *buf = (char *) ct_alloc((size_t) n + 2); buf[0] = (char) sigil; buf[1] = '!'; memcpy(buf + 2, name, (size_t) n);
    strs_add(&p->pkgs[p->npkgs - 1].attrs, buf, n + 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void record_does(RkP *p, int from, int to) {
    if (!p->npkgs) return;
    RkPkg *k = &p->pkgs[p->npkgs - 1];
    for (int t = from; t + 7 <= to; t++) if (!memcmp(p->s + t, "handles", 7) && !wordch_at(p, t + 7) && (t == 0 || !asc_word((unsigned char) p->s[t - 1]))) k->has_parent = 1;
    for (int t = from; t + 2 <= to; t++) {
        if (memcmp(p->s + t, "is", 2) || wordch_at(p, t + 2) || (t > 0 && asc_word((unsigned char) p->s[t - 1]))) continue;
        int q = ws(p, t + 2); int l = r_longname(p, q);
        if (l >= 0 && is_type_n(p, p->s + q, name_part_len(p, q, l))) k->has_parent = 1;
    }
    for (int t = from; t + 4 <= to; t++) {
        if (memcmp(p->s + t, "does", 4) || wordch_at(p, t + 4) || (t > 0 && asc_word((unsigned char) p->s[t - 1]))) continue;
        int q = ws(p, t + 4);
        int l = r_longname(p, q);
        if (l < 0) { k->unknown = 1; continue; }
        strs_add(&k->roles, p->s + q, name_part_len(p, q, l));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void check_lexical_variable(RkP *p, int pos, int e, int twig) {
    int c = ch(p, pos);
    int c1 = ch(p, pos + 1);
    if (!twig && (c1 == '$' || c1 == '@' || c1 == '%' || c1 == '&')) { check_lexical_variable(p, pos + 1, e, 0); return; }
    if (c == '&' || memmem(p->s + pos, (size_t) (e - pos), "::", 2)) return;
    int nb = pos + 1 + (twig ? 1 : 0);
    e = nb + name_part_len(p, nb, e);
    if (p->in_decl && (p->scope == 3 || p->scope == 4) && (twig == 0 || twig == '.' || twig == '!')) record_attribute(p, c, p->s + nb, e - nb, twig == '.');
    if (!p->in_decl && twig == '!' && p->npkgs) {
        RkPkg *k = &p->pkgs[p->npkgs - 1];
        RK_GROW(k->uses, k->nuses, k->cuses, RkMyst); k->uses[k->nuses].pos = pos; k->uses[k->nuses].len = e - pos; k->uses[k->nuses].depth = 0; k->nuses++;
    }
    if ((twig == '^' || twig == ':') && e - pos == 3 && p->s[pos + 2] >= 'A' && p->s[pos + 2] <= 'Z' && !p->p5isms) panic_at(p, pos, "Unsupported use of %.3s variable", p->s + pos);
    if (twig == '=' && !p->in_decl && !(e - pos == 5 && !memcmp(p->s + pos + 2, "pod", 3)) && !(e - pos == 8 && !memcmp(p->s + pos + 2, "finish", 6)))
        panic_at(p, pos, "Pod variable %.*s not yet implemented. Sorry.", e - pos, p->s + pos);
    if (twig == '^' || twig == ':') { int n = e - pos - 1; if (n < 1) return; char *buf = (char *) ct_alloc((size_t) n); buf[0] = (char) c; memcpy(buf + 1, p->s + pos + 2, (size_t) n - 1);
        if (user_name_index(p, buf, n) < 0) add_name_n(p, buf, n); return; }
    if (twig) return;
    if (p->in_decl) {
        add_name_n(p, p->s + pos, e - pos);
        if (p->scope == 7 && (c == '$' || c == '@' || c == '%')) {
            int L = e - pos + 32; char *st = (char *) ct_alloc((size_t) L);
            snprintf(st, (size_t) L, "%c__rk_st%d_%.*s", c, ++p->st_uid, e - pos - 1, p->s + pos + 1); p->names[p->nnames - 1].st = st;
        }
        if (p->load_depth) { RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = ct_strndup0(p->s + pos, e - pos); }
        return;
    }
    if (p->qsigil || p->myst_off || p->lax || user_name_index(p, p->s + pos, e - pos) >= 0) return;
    if (e - pos == 2 && (p->s[pos + 1] == '_' || p->s[pos + 1] == '/' || p->s[pos + 1] == '!')) return;
    panic_at(p, pos, "Variable '%.*s' is not declared", e - pos, p->s + pos);
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
int name_part_len(RkP *p, int from, int to) {
    int e = from;
    while (e < to) { if (p->s[e] == ':' && !(e + 1 < to && p->s[e + 1] == ':') && !(e > from && p->s[e - 1] == ':')) break; e++; }
    return e - from;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_capture(RkP *p, const char *s, int n) {
    if (n <= 0) return 0;
    char *buf = (char *) ct_alloc((size_t) n + 2);
    buf[0] = ':'; buf[1] = ':'; memcpy(buf + 2, s, (size_t) n);
    return user_name_index(p, buf, n + 2) >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_term_name(RkP *p, int pos) {
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
            if (!setjmp(p->jb)) {
                int ie = r_longname(p, t);
                tn = ie - t == 3 && (!memcmp(p->s + t, "Inf", 3) || !memcmp(p->s + t, "NaN", 3)) ? -1 : r_typename(p, t);
            } else tn = -1;
            memcpy(p->jb, save, sizeof save);
            int t2 = ws(p, tn >= 0 ? tn : t);
            if (ch(p, t2) == ')') e = t2 + 1;
        }
        if (p->build) { int ui = user_name_index(p, p->s + pos, nl); if (!is_t && ui >= 0 && p->names[ui].value) rkb_user_name(p->B, &p->tm, pos, e, nl); else rkb_name(p->B, &p->tm, pos, e, nl); }
        return e;
    }
    int q = e;
    if (ch(p, q) == '\\' && ch(p, q + 1) == '(') q++;
    if (p->build && rkb_listop_substitutes(p->s + pos, e - pos)) { p->x_subst = p->s + pos; p->x_subst_len = e - pos; }
    int a = r_args(p, q, 1);
    p->x_subst = NULL;
    RkList *args = takelst(p); int form = p->args_form;
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
    if (p->build) rkb_call(p->B, &p->tm, pos, na, e - pos, args, form);
    return na;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_term_identifier(RkP *p, int pos) {
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
    RkList *args = takelst(p);
    if (a >= 0 && !p->last_inv) add_mystery(p, pos, e - pos);
    if (p->build) rkb_call(p->B, &p->tm, pos, a, e - pos, args, 1);
    return a;
}
int r_typename(RkP *p, int pos) {
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
        if (at_lit(p, pos, "::") && is_alpha_cp(cp_at(p, pos + 2))) { add_name_n(p, p->s + pos + 2, nl - 2); if (!memmem(p->s + pos + 2, (size_t) nl - 2, "::", 2)) add_name_n(p, p->s + pos, nl); }
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
int r_trait(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 0;
    int e = -1; int q;
    RkTrait rt = { NULL, NULL };
    if ((q = kw(p, pos, "is")) >= 0) {
        int t = ws(p, q);
        e = r_longname(p, t);
        if (e < 0) panic_at(p, t, "Invalid name");
        rt.word = "is"; rt.name = ct_strndup0(p->s + t, name_part_len(p, t, e));
        int c = ch(p, e);
        if (c == '(' || c == '[' || c == '<' || c == '{' || cp_at(p, e) == 0xAB) { int f = r_circumfix(p, e); if (f >= 0) { if (p->build && c == '(' && !strcmp(rt.name, "default")) rt.args = p->tm.t; e = f; } }
    }
    else if ((q = kw(p, pos, "hides")) >= 0 || (q = kw(p, pos, "does")) >= 0 || (q = kw(p, pos, "of")) >= 0) {
        int t = ws(p, q);
        e = r_typename(p, t);
        if (e >= 0) { rt.word = ct_strndup0(p->s + pos, q - pos); rt.name = ct_strndup0(p->s + t, e - t); }
        if (e >= 0 && p->build) {
            const char *br = memchr(p->s + t, '[', (size_t) (e - t));
            if (br) {
                int sg = p->goal; p->goal = 0;
                int s = r_semilist(p, (int) (br - p->s) + 1);
                RkList *in = p->last_list; int nst = p->last_nstmts;
                p->goal = sg;
                if (s >= 0 && ch(p, s) == ']') rt.args = rkb_paren(p->B, in, nst);
            }
        }
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
        rt.word = "handles"; rt.name = ch(p, t) == '<' ? ct_strndup0(p->s + t + 1, e - t - 2) : ct_strndup0(p->s + t, e - t);
    }
    p->in_decl = sd;
    if (e >= 0 && rt.word) { p->tr1 = rt; p->has_tr1 = 1; } else p->has_tr1 = 0;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_traits(RkP *p, int pos) {
    int q = pos;
    RkTrait *tr = NULL; int n = 0, c = 0;
    for (;;) { int t = ws(p, q); p->has_tr1 = 0; int e = r_trait(p, t); if (e < 0) break; if (p->has_tr1) { RK_GROW(tr, n, c, RkTrait); tr[n++] = p->tr1; } q = e; }
    p->tr = tr; p->ntr = n;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_param_var(RkP *p, int pos) {
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
        if (e >= 0 && (p->scope == 3 || p->scope == 4) && (q == pos + 1 || ch(p, pos + 1) == '.' || ch(p, pos + 1) == '!')) record_attribute(p, c, p->s + q, e - q, ch(p, pos + 1) == '.');
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
static void check_is_traits(RkP *p, int from, int to, const char *const *known, const char *what) {
    if (p->myst_off || user_name_index(p, "&trait_mod:<is>", 15) >= 0) return;
    for (int t = from; t + 2 <= to; t++) {
        if (p->s[t] == '\'' || p->s[t] == '"' || p->s[t] == '(' || p->s[t] == '[' || p->s[t] == '{') return;
        if (memcmp(p->s + t, "is", 2) || wordch_at(p, t + 2) || (t > 0 && asc_word((unsigned char) p->s[t - 1]))) continue;
        int q = ws(p, t + 2); int e = r_identifier(p, q);
        if (e < 0 || e > to || at_lit(p, e, "::")) continue;
        int n = e - q, ok = is_type_n(p, p->s + q, n);
        for (int i = 0; !ok && known[i]; i++) if ((int) strlen(known[i]) == n && !memcmp(known[i], p->s + q, (size_t) n)) ok = 1;
        if (!ok) panic_at(p, t, "Can't use unknown trait 'is' -> '%.*s' in %s declaration.", n, p->s + q, what);
        t = e;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_variable_declarator(RkP *p, int pos) {
    int sd = p->in_decl; p->in_decl = 1;
    int e = r_variable(p, pos);
    p->in_decl = sd;
    if (e < 0) return -1;
    int var_end = e;
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
    RkTrait *vtr = p->tr; int nvtr = p->ntr;
    static const char *const vtraits[] = { "default", "dynamic", "export", 0 };
    if (p->scope != 3 && p->scope != 4) check_is_traits(p, e, q, vtraits, " variable");
    if ((p->scope == 3 || p->scope == 4) && p->npkgs) for (int t = e; t + 2 <= q; t++) {
        if (memcmp(p->s + t, "is", 2) || wordch_at(p, t + 2) || (t > 0 && asc_word((unsigned char) p->s[t - 1]))) continue;
        int a = ws(p, t + 2); int b = r_identifier(p, a);
        if (b > a && !wordch_at(p, b) && ch(p, b) != '(' && is_capture(p, p->s + a, b - a)) p->pkgs[p->npkgs - 1].generic_is = 1;
    }
    for (;;) { int t = ws(p, q); int f = r_post_constraint(p, t); if (f < 0) break; q = f; }
    if (p->build) {
        RkDecl *d = (RkDecl *) ct_zalloc(1, sizeof(RkDecl));
        d->sigil = ch(p, pos); d->name = ct_strndup0(p->s + pos, var_end - pos);
        { int ni = p->scope == 7 ? user_name_index(p, p->s + pos, var_end - pos) : -1; if (ni >= 0 && p->names[ni].st) d->name = p->names[ni].st; }
        if (d->sigil == '&') rkb_codevar(p->B, d->name);
        d->tr = vtr; d->ntr = nvtr;
        p->dcl = d;
    }
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void record_method(RkP *p, int name_from, int name_to, int sig_from, int sig_to, int body) {
    RkPkg *k = &p->pkgs[p->npkgs - 1];
    int nf = name_from > 0 && (p->s[name_from - 1] == '!' || p->s[name_from - 1] == '^') ? name_from - 1 : name_from;
    int nl = (name_to - name_from) + (name_from - nf);
    int b = ch(p, body) == '{' ? ws(p, body + 1) : -1;
    int stub = b >= 0 && (at_lit(p, b, "...") || at_lit(p, b, "!!!") || at_lit(p, b, "???")) && ch(p, ws(p, b + 3)) == '}';
    if (p->scope == 2) { if (strs_has(&k->ours, p->s + nf, nl)) panic_at(p, name_from, "Redeclaration of method '%.*s'.", nl, p->s + nf); strs_add(&k->ours, p->s + nf, nl); }
    if (stub && p->multiness != 1) {
        if (strs_has(&k->explicit, p->s + nf, nl) && k->name)
            panic_at(p, name_from, "Package '%s' already has a method '%.*s' (did you mean to declare a multi method?)", k->name, nl, p->s + nf);
        strs_add(&k->explicit, p->s + nf, nl);
    }
    if (stub) { strs_add(&k->stubs, p->s + nf, nl); return; }
    if (p->multiness != 1 && p->multiness != 2) {
        if (strs_has(&k->explicit, p->s + nf, nl) && k->name)
            panic_at(p, name_from, "Package '%s' already has a method '%.*s' (did you mean to declare a multi method?)", k->name, nl, p->s + nf);
        strs_add(&k->explicit, p->s + nf, nl);
        strs_add(&k->meths, p->s + nf, nl); return;
    }
    if (p->multiness == 2) { strs_add(&k->mnames, p->s + nf, nl); return; }
    if (strs_has(&k->explicit, p->s + nf, nl))
        panic_at(p, name_from, "Cannot have a multi candidate for '%.*s' when an only method is also in the package '%s'", nl, p->s + nf, k->name ? k->name : "<anon>");
    strs_add(&k->mnames, p->s + nf, nl);
    char *buf = (char *) ct_alloc((size_t) nl + 1 + (size_t) (sig_to - sig_from)); int m = 0;
    for (int i = nf; i < nf + nl; i++) buf[m++] = p->s[i];
    buf[m++] = '|';
    for (int i = sig_from; i < sig_to; i++) if (!asc_space((unsigned char) p->s[i])) buf[m++] = p->s[i];
    strs_add(&k->multis, buf, m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void apply_op_traits(RkP *p, int ui, int from, int to) {
    static const char names[][8] = { "equiv", "tighter", "looser", "" };
    for (int t = from; t < to; t++) for (int i = 0; names[i][0]; i++) {
        int l = (int) strlen(names[i]);
        if (t + l + 1 > to || memcmp(p->s + t, names[i], (size_t) l) || p->s[t + l] != '(') continue;
        if (t > from && asc_word((unsigned char) p->s[t - 1])) continue;
        int c = t + l + 1; while (c < to && p->s[c] != ')') c++;
        int s = t + l + 1; if (!at_lit(p, s, "&infix:<")) continue;
        s += 8; int e = s; while (e < c && p->s[e] != '>') e++;
        if (e >= c) continue;
        int xp = -1, xa = AS_LEFT;
        for (int k = p->nuops - 1; k >= 0 && xp < 0; k--) {
            RkUserOp *v = &p->uops[k];
            if (v->cat == 'i' && v->len == e - s && !memcmp(v->sym, p->s + s, (size_t) (e - s))) { xp = v->prec; xa = v->assoc; }
        }
        for (int k = 0; xp < 0 && rk_infix[k].sym; k++) {
            int n = (int) strlen(rk_infix[k].sym);
            if (n == e - s && !memcmp(rk_infix[k].sym, p->s + s, (size_t) n)) { xp = rk_infix[k].prec; xa = rk_infix[k].assoc; }
        }
        if (xp < 0) continue;
        if (i == 0) { p->uops[ui].prec = xp; p->uops[ui].assoc = xa; }
        else p->uops[ui].prec = i == 1 ? xp + 1 : xp - 1;
    }
    RkUserOp *u = &p->uops[ui];
    if (p->B && rkb_user_op_find(p->B, 'i', u->sym) >= 0) rkb_user_op(p->B, 'i', u->sym, u->len, u->sym, u->len, u->prec);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_routine_def(RkP *p, int pos, int is_method) {
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
        if (n >= 0) { name_from = q; name_to = n; q = ws(p, n); check_category(p, name_from, name_to); }
    }
    int nuops_reg0 = p->nuops;
    if (name_from >= 0) register_user_op(p, name_from, name_to);
    int nuops_before_traits = p->nuops;
    int sm = p->has_self;
    int outer_depth = p->depth;
    scope_enter(p);
    int sig_from = q, sig_to = q;
    tree_t *rsig = NULL; int multi = p->multiness;
    if (ch(p, q) == '(') {
        int s = r_signature(p, q + 1, is_method);
        rsig = p->sig;
        s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        sig_to = s + 1;
        q = ws(p, s + 1);
    }
    int tstart = q;
    q = r_traits(p, q); q = ws(p, q);
    RkTrait *rtr = p->tr; int nrtr = p->ntr;
    if (!is_method && nuops_before_traits > nuops_reg0 && p->uops[nuops_before_traits - 1].cat == 'i') apply_op_traits(p, nuops_before_traits - 1, tstart, q);
    static const char *const rtraits[] = { "assoc", "DEPRECATED", "export", "hidden-from-backtrace", "hidden-from-USAGE", "implementation-detail", "leading_docs", "nodal",
                                           "pure", "trailing_docs", "default", "equiv", "looser", "tighter", "inlinable", "onlystar", "prec", "raw", "rw", "revision-gated",
                                           "test-assertion", "cached", 0 };
    check_is_traits(p, tstart, q, rtraits, is_method ? "method" : "sub");
    for (int t = tstart; t + 6 <= q; t++) if (!memcmp(p->s + t, "export", 6) && !wordch_at(p, t + 6) && (t == 0 || !asc_word((unsigned char) p->s[t - 1]))) {
        for (int k = 0; k < nuops_before_traits && k < p->nuops; k++) if (p->uops[k].depth == p->depth - 1 && k >= nuops_before_traits - 2) p->uops[k].depth = 1000;
        if (name_from >= 0 && !is_method) {
            int nl = name_part_len(p, name_from, name_to); char *x = (char *) ct_alloc((size_t) nl + 2); x[0] = '&'; memcpy(x + 1, p->s + name_from, (size_t) nl); x[nl + 1] = 0;
            const char *from_pkg = p->pkg ? p->pkg : "";
            for (int i = 0; i < p->exported.n; i++) if (!strcmp(p->exported.v[i], x) && strcmp(p->exported_from.v[i], from_pkg))
                panic_at(p, name_from, "A symbol '%s' has already been exported", x);
            strs_add(&p->exported, x, nl + 1); strs_add(&p->exported_from, from_pkg, (int) strlen(from_pkg));
            RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = x;
        }
        break;
    }
    int e, onlystar = 0;
    if (ch(p, q) == ';' && !is_method) {
        if (name_from < 0 || !(name_to - name_from == 4 && !memcmp(p->s + name_from, "MAIN", 4))) panic_at(p, q,
            "A unit-scoped sub definition is not allowed except on a MAIN sub;\nPlease use the block form. If you did not mean to declare a unit-scoped sub,\nperhaps you accidentally placed a semicolon after routine's definition?");
        if (outer_depth != 1) panic_at(p, q, "A unit-scoped sub definition is not allowed in a subscope");
        e = r_statementlist(p, q + 1);
    }
    else if ((e = r_onlystar(p, q)) >= 0) onlystar = 1;
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
    else { p->next_bk = name_from < 0 ? BK_BLOCK : is_method ? BK_METHOD : BK_SUB; e = r_blockoid(p, q); }
    tree_t *body = p->build ? taketv(p) : NULL;
    RkList *bl = p->blk_last; int bn = p->blk_n;
    scope_leave(p);
    p->has_self = sm;
    if (is_method && name_from >= 0 && p->npkgs && !(is_method == 2 && p->lang_e)) record_method(p, name_from, name_to, sig_from, sig_to, q);
    if (p->build) {
        if (name_from < 0 && !is_method) rkb_block_term(p->B, &p->tm, pos, e, body, rsig, 1, bl, bn);
        else if (name_from < 0 && is_method == 1 && !onlystar) rkb_block_term(p->B, &p->tm, pos, e, body, rkb_self_sig(p->B, rsig), 1, bl, bn);
        else { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_BSTMT; p->tm.from = pos; p->tm.to = p->tm.core_to = e;
            p->tm.t = onlystar && is_method && multi == 2 ? ast_node_new(TT_SEQ_EXPR) : rkb_routine(p->B, is_method ? 1 : 0, multi, name_from >= 0 ? p->s + name_from : "", name_from >= 0 ? name_to - name_from : 0, 0, rsig, sig_to > sig_from, body, rtr, nrtr); }
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_package_declarator(RkP *p, int pos) {
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
    if (p->scope == 8 && pkg_len && user_name_index(p, p->s + q, pkg_len) >= 0) panic_at(p, n, "Redeclaration of symbol '%.*s'.", pkg_len, p->s + q);
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
      k->unknown = saved_scope == 5 || saved_scope == 8; k->decl_pos = pkg_from; }
    scope_enter(p);
    tree_t *rsig = NULL;
    if (!strcmp(kind, "role") && ch(p, q) == '[') {
        int s = r_signature(p, q + 1, 0); s = ws(p, s);
        if (p->build) rsig = p->sig;
        if (ch(p, s) != ']') panic_at(p, s, "Unable to parse role signature; couldn't find final ']'");
        q = ws(p, s + 1);
    }
    int tstart = q;
    q = r_traits(p, q); q = ws(p, q);
    RkTrait *ktr = p->tr; int nktr = p->ntr;
    int kbk = !strcmp(kind, "class") || !strcmp(kind, "role") ? BK_CLASS : !strcmp(kind, "grammar") ? BK_GRAMMAR : BK_MODULE;
    tree_t *kbody = NULL;
    record_does(p, tstart, q);
    if (pkg_len) for (int t = tstart; t + 6 <= q; t++) if (!memcmp(p->s + t, "export", 6) && !wordch_at(p, t + 6) && !asc_word((unsigned char) p->s[t - 1])) {
        RK_GROW(p->exports, p->nexports, p->cexports, char *); p->exports[p->nexports++] = ct_strndup0(p->s + pkg_from, pkg_len); break;
    }
    int e;
    if (ch(p, q) == '{') {
        if (unit) panic_at(p, q, "Cannot use 'unit' with block form of %s", kind);
        int sh = p->has_self; p->has_self = 1;
        p->next_bk = kbk;
        e = r_blockoid(p, q);
        kbody = p->build ? taketv(p) : NULL;
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
        tree_t *scl = p->cur_list; int sbk = p->bk;
        kbody = p->build ? ast_node_new(TT_SEQ_EXPR) : NULL; p->cur_list = kbody; p->bk = kbk;
        e = r_statementlist(p, q + 1);
        p->cur_list = scl; p->bk = sbk;
        p->has_self = sh;
    }
    else { scope_leave(p); p->scope = saved_scope; panic_at(p, q, "Unable to parse %s definition", kind); return -1; }
    scope_leave(p);
    p->scope = saved_scope;
    if (!unit) p->pkg = saved_pkg;
    if (!unit) package_close(p);
    if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_BSTMT; p->tm.from = pos; p->tm.to = p->tm.core_to = e;
        p->tm.t = rkb_package(p->B, kind, n >= 0 ? p->s + pkg_from : "", pkg_len, ktr, nktr, kbody, rsig); }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int import_module_src(RkP *p, const char *src, int got, const char *path) {
    RkP *m = (RkP *) ct_alloc(sizeof(RkP));
    memset(m, 0, sizeof *m);
    m->load_depth = p->load_depth + 1;
    for (int i = 0; i < p->nlibs; i++) { RK_GROW(m->libs, m->nlibs, m->clibs, char *); m->libs[m->nlibs++] = p->libs[i]; }
    if (rk_check_into(m, src, got, path, m) != 0) return 0;
    if (p->build) rk_build_module(p, src, got, path);
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
