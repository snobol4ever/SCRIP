#include "parsers/raku/rk_syntax/rks_head.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
#include "parsers/raku/rk_syntax/rks_08_sh.h"
static RkPkg *role_lookup(RkP *p, const char *name);
static int r_quote_raw(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int opener_close(int c) {
    for (unsigned i = 0; i + 1 < sizeof rk_brackets / sizeof *rk_brackets; i += 2) if (rk_brackets[i] == c) return rk_brackets[i + 1];
    return 0;
}
int line_of(RkP *p, int pos) {
    int line = 1;
    for (int i = 0; i < pos && i < p->n; i++) if (p->s[i] == '\n') line++;
    return line;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_nibble_until(RkP *p, int pos, RkLang *L, int *endpos) {
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
void lang_q(RkLang *L) { memset(L, 0, sizeof *L); L->qbs = 1; }
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
int quote_body(RkP *p, int pos, RkLang *L, const char *what) { return quote_body_at(p, pos, L, what, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quotepair(RkP *p, int pos, const char **key, int *val) {
    if (ch(p, pos) != ':') return -1;
    int q = pos + 1; *val = 1;
    if (ch(p, q) == '!') {
        int e = r_identifier(p, q + 1);
        if (e < 0) return -1;
        *key = ct_strndup0(p->s + q + 1, e - q - 1); *val = 0; return e;
    }
    if (is_digit_cp(cp_at(p, q))) {
        int d = r_decint(p, q); int e = r_identifier(p, d);
        if (e < 0) return -1;
        *key = ct_strndup0(p->s + d, e - d); return e;
    }
    int e = r_identifier(p, q);
    if (e < 0) return -1;
    *key = ct_strndup0(p->s + q, e - q);
    if (ch(p, e) == '(') { int c = r_circumfix(p, e); if (c >= 0) { *val = 2; return c; } }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int apply_quote_adverb(RkP *p, int pos, RkLang *L, const char *k, int v, int *to) {
    if (!strcmp(k, "q") || !strcmp(k, "single")) { L->qbs = 1; return 1; }
    if (!strcmp(k, "qq") || !strcmp(k, "double")) { L->bs = 1; L->qbs = 0; L->clos = 1; L->sc = 1; L->ar = 1; L->hs = 1; L->fn = 1; return 1; }
    if (!strcmp(k, "b") || !strcmp(k, "backslash")) { L->bs = v != 0; if (v) L->qbs = 0; return 1; }
    if (!strcmp(k, "s") || !strcmp(k, "scalar")) { L->sc = v != 0; return 1; }
    if (!strcmp(k, "a") || !strcmp(k, "array")) { L->ar = v != 0; return 1; }
    if (!strcmp(k, "h") || !strcmp(k, "hash")) { L->hs = v != 0; return 1; }
    if (!strcmp(k, "f") || !strcmp(k, "function")) { L->fn = v != 0; return 1; }
    if (!strcmp(k, "c") || !strcmp(k, "closure")) { L->clos = v != 0; return 1; }
    if (!strcmp(k, "w") || !strcmp(k, "words")) { L->words = 1; return 1; }
    if (!strcmp(k, "ww") || !strcmp(k, "quotewords")) { L->ww = v != 0; L->words = 1; return 1; }
    if (!strcmp(k, "x") || !strcmp(k, "exec") || !strcmp(k, "v") || !strcmp(k, "val")) { L->xv = 1; return 1; }
    if (!strcmp(k, "to") || !strcmp(k, "heredoc")) { *to = 1; return 1; }
    if (!strcmp(k, "cc")) { L->cc = 1; return 1; }
    if (!strcmp(k, "regex")) { memset(L, 0, sizeof *L); L->regex = 1; return 1; }
    if (!strcmp(k, "p") || !strcmp(k, "path")) panic_at(p, pos, "Unrecognized adverb: :%s", k);
    panic_at(p, pos, "Unrecognized adverb: :%s", k);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lang_bits(const RkLang *L) {
    return (L->bs ? RKQ_BS : 0) | (L->qbs ? RKQ_QBS : 0) | (L->sc ? RKQ_SC : 0) | (L->ar ? RKQ_AR : 0) | (L->hs ? RKQ_HS : 0) | (L->fn ? RKQ_FN : 0) | (L->clos ? RKQ_CLOS : 0)
        | (L->words ? RKQ_WORDS : 0) | (L->ww ? RKQ_WW : 0) | ((L->cc || L->regex || L->xv) ? RKQ_UNSUP : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quibble(RkP *p, int pos, RkLang *L, int to, const char *what) {
    const char *key; int v;
    for (;;) {
        int e = r_quotepair(p, pos, &key, &v);
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
    p->qs = s; p->qe = stop_at; p->qfl = lang_bits(L);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int expect_close(RkP *p, int pos, int c, const char *what, int open_pos) {
    int q = ws(p, pos);
    if (ch(p, q) != c) panic_at(p, q, "Unable to parse %s; couldn't find final '%c' (corresponding starter was at line %d)", what, c, line_of(p, open_pos));
    return q + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int words_quote(RkP *p, int pos, int open, int nrep, int qq) {
    RkLang L; if (qq) { lang_qq(&L); L.ww = 1; } else lang_q(&L);
    L.words = 1; L.start = open; L.nrep = nrep; L.stop = opener_close(open);
    int s = quote_starter(p, pos, &L);
    int stop_at = s;
    int e = quote_body_at(p, s, &L, "quote words", &stop_at);
    if (p->build) rkb_words(p->B, &p->tm, pos, e, s, stop_at);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_postcircumfix(RkP *p, int pos) {
    int sub = p->x_sub; p->x_sub = 0;
    int c = cp_at(p, pos);
    if (c == '[' || c == '{') {
        int sq = p->qsigil; p->qsigil = 0; int sg = p->goal; p->goal = 0;
        p->x_star = c == '[' && sub;
        int e = r_semilist(p, pos + 1); RkList *in = p->last_list; p->qsigil = sq; p->goal = sg;
        e = expect_close(p, e, c == '[' ? ']' : '}', "subscript", pos);
        if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = c; p->pf.from = pos; p->pf.to = e; p->pf.inner = in; }
        return e; }
    if (c == '(') {
        int sq = p->qsigil; p->qsigil = 0; int sg = p->goal; p->goal = 0;
        int e = r_arglist(p, ws(p, pos + 1));
        RkList *a = takelst(p);
        p->qsigil = sq; p->goal = sg;
        e = expect_close(p, e, ')', "argument list", pos);
        if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = '('; p->pf.from = pos; p->pf.to = e; p->pf.args = a; p->pf.form = 1; }
        return e;
    }
    if (at_lit(p, pos, "<<")) {
        RkLang L; lang_qq(&L); L.ww = 1; L.start = '<'; L.stop = '>'; L.nrep = 2;
        int e; r_nibble_until(p, pos + 2, &L, &e);
        if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '>>' (corresponding '<<' was at line %d)", line_of(p, pos));
        int f = quote_stopper(p, e, &L);
        if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = 'W'; p->pf.from = pos; p->pf.to = f; p->pf.txt = ct_strndup0(p->s + pos + 2, e - pos - 2); }
        return f;
    }
    if (c == 0xAB) {
        RkLang L; lang_qq(&L); L.ww = 1; L.start = 0xAB; L.stop = 0xBB; L.nrep = 1;
        int e; r_nibble_until(p, pos + 2, &L, &e);
        if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '\xc2\xbb'");
        int f = quote_stopper(p, e, &L);
        if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = 'W'; p->pf.from = pos; p->pf.to = f; p->pf.txt = ct_strndup0(p->s + pos + 2, e - pos - 2); }
        return f;
    }
    if (c == '<') {
        RkLang L; lang_q(&L); L.start = '<'; L.stop = '>'; L.nrep = 1;
        int e; r_nibble_until(p, pos + 1, &L, &e);
        if (e >= 0 && quote_stopper(p, e, &L) >= 0) {
            int f = quote_stopper(p, e, &L);
            if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = '<'; p->pf.from = pos; p->pf.to = f; p->pf.txt = ct_strndup0(p->s + pos + 1, e - pos - 1); }
            return f;
        }
        int q = pos + 1; while (ch(p, q) == '=') q++;
        int h = hs(p, q);
        int d = cp_at(p, h);
        if (is_digit_cp(d) || d == '$' || d == '@' || d == '%' || d == '&' || d == ':') panic_at(p, pos, "Whitespace required before %.*s operator", q - pos, p->s + pos);
        panic_at(p, pos, "Unable to parse quote-words subscript; couldn't find '>' (corresponding '<' was at line %d)", line_of(p, pos));
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int strs_has(const RkStrs *a, const char *s, int n) {
    for (int i = 0; i < a->n; i++) if ((int) strlen(a->v[i]) == n && !memcmp(a->v[i], s, (size_t) n)) return 1;
    return 0;
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
static int role_generic_is(RkP *p, const char *name, int depth) {
    RkPkg *r = role_lookup(p, name);
    if (!r || depth > 16) return 0;
    if (r->generic_is) return 1;
    for (int j = 0; j < r->roles.n; j++) if (role_generic_is(p, r->roles.v[j], depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RkPkg *role_lookup(RkP *p, const char *name) {
    for (int i = p->nroledb - 1; i >= 0; i--) if (!strcmp(p->roledb[i].name, name)) return &p->roledb[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int role_flatten(RkP *p, const char *name, RkStrs *meths, RkStrs *multis, RkStrs *stubs, int depth) {
    RkPkg *r = role_lookup(p, name);
    if (!r || r->unknown || depth > 16) return 0;
    for (int i = 0; i < r->meths.n; i++) {
        size_t need = strlen(r->meths.v[i]) + strlen(r->name) + 2;
        char *buf = (char *) ct_alloc(need); int m = snprintf(buf, need, "%s\t%s", r->meths.v[i], r->name);
        if (m > 0 && !strs_has(meths, buf, m)) strs_add(meths, buf, m);
    }
    for (int i = 0; i < r->multis.n; i++) if (!strs_has(multis, r->multis.v[i], (int) strlen(r->multis.v[i]))) strs_add(multis, r->multis.v[i], (int) strlen(r->multis.v[i]));
    for (int i = 0; i < r->stubs.n; i++) if (!strs_has(stubs, r->stubs.v[i], (int) strlen(r->stubs.v[i]))) strs_add(stubs, r->stubs.v[i], (int) strlen(r->stubs.v[i]));
    for (int j = 0; j < r->roles.n; j++) if (!role_flatten(p, r->roles.v[j], meths, multis, stubs, depth + 1)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int class_defines(const RkPkg *k, const char *n, int len) {
    if (strs_has(&k->meths, n, len) || strs_has(&k->mnames, n, len) || strs_has(&k->stubs, n, len)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check_role_composition(RkP *p, RkPkg *k) {
    int nr = k->roles.n;
    if (!nr || p->build) return;
    for (int i = 0; i < nr; i++) if (role_generic_is(p, k->roles.v[i], 0))
        panic_at(p, k->decl_pos, "Cannot find method 'instantiate_generic' on object of type Perl6::Metamodel::ClassHOW");
    RkStrs *M = (RkStrs *) ct_alloc(sizeof(RkStrs) * (size_t) nr), *K = (RkStrs *) ct_alloc(sizeof(RkStrs) * (size_t) nr), *S = (RkStrs *) ct_alloc(sizeof(RkStrs) * (size_t) nr);
    memset(M, 0, sizeof(RkStrs) * (size_t) nr); memset(K, 0, sizeof(RkStrs) * (size_t) nr); memset(S, 0, sizeof(RkStrs) * (size_t) nr);
    for (int i = 0; i < nr; i++) if (!role_flatten(p, k->roles.v[i], &M[i], &K[i], &S[i], 0)) return;
    for (int i = 0; i < nr; i++) for (int a = 0; a < M[i].n; a++) {
        const char *n = M[i].v[a]; int len = (int) (strchr(n, '\t') - n); const char *org = n + len + 1;
        if (class_defines(k, n, len)) continue;
        for (int j = i + 1; j < nr; j++) for (int b = 0; b < M[j].n; b++) {
            const char *x = M[j].v[b]; int xl = (int) (strchr(x, '\t') - x);
            if (xl == len && !memcmp(x, n, (size_t) len) && strcmp(x + xl + 1, org))
                panic_at(p, k->decl_pos, "Method '%.*s' must be resolved by class %s because it exists in multiple roles (%s, %s)", len, n, k->name, k->roles.v[j], k->roles.v[i]);
        }
    }
    for (int i = 0; i < nr; i++) for (int a = 0; a < K[i].n; a++) {
        const char *key = K[i].v[a]; int len = (int) strlen(key); int nl = (int) (strchr(key, '|') - key);
        if (class_defines(k, key, nl)) continue;
        for (int j = i + 1; j < nr; j++) if (strcmp(k->roles.v[i], k->roles.v[j]) && strs_has(&K[j], key, len))
            panic_at(p, k->decl_pos, "Multi method '%.*s' must be resolved by class %s because it exists in multiple roles (%s, %s)", nl, key, k->name, k->roles.v[j], k->roles.v[i]);
    }
    if (k->has_parent) return;
    for (int i = 0; i < nr; i++) for (int a = 0; a < S[i].n; a++) {
        const char *n = S[i].v[a]; int len = (int) strlen(n); int ok = class_defines(k, n, len);
        for (int j = 0; j < nr && !ok; j++) for (int b = 0; b < M[j].n && !ok; b++) if (!strncmp(M[j].v[b], n, (size_t) len) && M[j].v[b][len] == '\t') ok = 1;
        for (int j = 0; j < nr && !ok; j++) for (int b = 0; b < K[j].n && !ok; b++) if (!strncmp(K[j].v[b], n, (size_t) len) && K[j].v[b][len] == '|') ok = 1;
        if (!ok) panic_at(p, k->decl_pos, "Method '%s' must be implemented by %s because it is required by roles: %s.", n, k->name, k->roles.v[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void package_close(RkP *p) {
    RkPkg k = p->pkgs[--p->npkgs];
    if (!strcmp(k.kind, "role") && k.name) { RK_GROW(p->roledb, p->nroledb, p->croledb, RkPkg); p->roledb[p->nroledb++] = k; }
    if (p->build || k.unknown || !(!strcmp(k.kind, "class") || !strcmp(k.kind, "grammar") || !strcmp(k.kind, "role"))) return;
    if (k.name && (!strcmp(k.kind, "class") || !strcmp(k.kind, "grammar"))) check_role_composition(p, &k);
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
static int rx_adverbs(RkP *p, int pos, int *p5) {
    const char *key; int v;
    for (;;) {
        int e = r_quotepair(p, pos, &key, &v);
        if (e < 0) return pos;
        if (!strcmp(key, "P5") || !strcmp(key, "Perl5")) *p5 = 1;
        pos = ws(p, e);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p5_class_item(RkP *p, int q, int *cp, int *lit) {
    if (ch(p, q) == '\\') {
        int c = ch(p, q + 1);
        *lit = !strchr("dDwWsShHvVN", c) || c < 0;
        *cp = c == 'n' ? '\n' : c == 't' ? '\t' : c == 'r' ? '\r' : c == 'f' ? '\f' : c == 'e' ? 27 : c == 'a' ? 7 : (c == 'x' || c == 'c' || (c >= '0' && c <= '9')) ? -1 : cp_at(p, q + 1);
        return q + 1 + cp_len(p, q + 1);
    }
    *lit = 1; *cp = cp_at(p, q);
    return q + cp_len(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int p5_class(RkP *p, int q, RkLang *L) {
    int start = q; q++;
    if (ch(p, q) == '^') q++;
    if (ch(p, q) == ']') q++;
    while (q < p->n && ch(p, q) != ']' && quote_stopper(p, q, L) < 0) {
        int from = q, c0, l0; q = p5_class_item(p, q, &c0, &l0);
        if (ch(p, q) == '-' && ch(p, q + 1) != ']' && q + 1 < p->n) {
            int c1, l1; int e = p5_class_item(p, q + 1, &c1, &l1);
            if (!l0 || !l1) panic_at(p, e, "Illegal range endpoint in regex: %.*s", e - from, p->s + from);
            if (c0 >= 0 && c1 >= 0 && c0 > c1) panic_at(p, e, "Illegal reversed character range in regex: %.*s", e - from, p->s + from);
            q = e;
        }
    }
    return ch(p, q) == ']' ? q + 1 : (q > start ? q : start + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int p5_regex_body(RkP *p, int pos, RkLang *L) {
    int q = pos;
    int depth = 0;
    while (q < p->n) {
        if (depth == 0 && quote_stopper(p, q, L) >= 0) return q;
        int c = ch(p, q);
        if (c == '\\') { q += 1 + cp_len(p, q + 1); continue; }
        if (c == '[' && L->start != '[') { q = p5_class(p, q, L); continue; }
        if (L->start && cp_at(p, q) == L->start) depth++;
        else if (L->start && cp_at(p, q) == L->stop) depth--;
        q += cp_len(p, q);
    }
    panic_at(p, pos, "Regex not terminated.");
    return -1;
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
    int s = LEN(p, pos, 1);
    int stop_at;
    int e; r_nibble_until(p, s, &L, &e);
    if (e < 0 || quote_stopper(p, e, &L) < 0) panic_at(p, pos, "Unable to parse expression in %s; couldn't find final %s (corresponding starter was at line %d)", what, qq ? "'\"'" : "\"'\"",
        line_of(p, pos));
    stop_at = e; (void) stop_at;
    return quote_stopper(p, e, &L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_quote(RkP *p, int pos) {
    RkClosure *sq = p->qc; int sn = p->nqc, sc = p->cqc, sqs = p->qs, sqe = p->qe, sqf = p->qfl;
    p->qc = NULL; p->nqc = 0; p->cqc = 0; p->qs = -1; p->qe = -1; p->qfl = 0;
    int nh = p->nhere;
    int e = r_quote_raw(p, pos);
    if (e >= 0 && p->build && p->nhere > nh) { rkb_heredoc_stub(p->B, &p->tm, pos, e); p->here[p->nhere - 1].node = p->tm.t; }
    else if (e >= 0 && p->build) rkb_quote(p->B, &p->tm, pos, e, p->qc, p->nqc, p->qs, p->qe, p->qfl);
    p->qc = sq; p->nqc = sn; p->cqc = sc; p->qs = sqs; p->qe = sqe; p->qfl = sqf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_quote_raw(RkP *p, int pos) {
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
        int h = NSPAN(p, q, ck(is_space_cp));
        if (ch(p, h) == '/') panic_at(p, pos, "Null regex not allowed");
        RkLang L; memset(&L, 0, sizeof L); L.regex = 1; L.stop = '/'; L.nrep = 1;
        int e = r_regex_nibbler(p, q, &L);
        if (ch(p, e) != '/') panic_at(p, e, "Unable to parse regex; couldn't find final '/'");
        return old_rx_mods(p, e + 1);
    }
    if (!is_alpha_cp(c)) return -1;
    int wend = NSPAN(p, pos, ck(is_word_cp));
    if ((ch(p, wend) == '-' || ch(p, wend) == '\'') && is_alpha_cp(cp_at(p, wend + 1))) return -1;
    int wl = wend - pos; const char *w = p->s + pos;
    char *word = (char *) ct_alloc((size_t) wl + 1); memcpy(word, w, (size_t) wl); word[wl] = 0;
    if (!strcmp(word, "q") || !strcmp(word, "qq") || !strcmp(word, "Q") || (wl > 1 && (w[0] == 'q' || w[0] == 'Q') && (w[1] != 'q' || wl > 2))) {
        RkLang L; int to = 0; int base;
        if (w[0] == 'Q') { memset(&L, 0, sizeof L); base = 1; }
        else if (wl >= 2 && w[1] == 'q') { lang_qq(&L); base = 2; }
        else { lang_q(&L); base = 1; }
        int q = pos + base;
        if (q < wend) { int m = quote_mod_after(p, q, &to, &L); if (m != wend) return -1; q = m; }
        char *qw = (char *) ct_alloc((size_t) (q - pos) + 1); memcpy(qw, w, (size_t) (q - pos)); qw[q - pos] = 0;
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
