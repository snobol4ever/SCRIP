#include "parsers/raku/rk_syntax/rks_head.h"
#include <string.h>
#include "ir/ct_arena.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
#include "parsers/raku/rk_syntax/rks_08_sh.h"
#include "parsers/raku/rk_syntax/rks_09_sh.h"
static int r_scoped(RkP *p, int pos, int scope);
static int r_declarator(RkP *p, int pos);
static int r_regex_def(RkP *p, int pos);
static int r_multi_declarator(RkP *p, int pos);
static int r_routine_declarator(RkP *p, int pos);
static int r_regex_declarator(RkP *p, int pos);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int compile_time_var_known(const char *s, int n) {
    static const char known[][20] = { "$?FILE", "$?LINE", "$?DISTRIBUTION", "$?LANG", "%?LANG", "$?NL", "$?BITS", "$?TABSTOP", "$?PACKAGE", "%?RESOURCES", "$?CHECKSUM", "$?FILES",
                                         "$?SOURCE", "$?STRICT", "$?LANGUAGE-REVISION", "%?REQUIRE-SYMBOLS", "$?CONCRETIZATION", "$?CLASS", "$?ROLE", "$?MODULE", "$?REGEX", "&?ROUTINE",
                                         "&?BLOCK", "" };
    for (int i = 0; known[i][0]; i++) if ((int) strlen(known[i]) == n && !memcmp(known[i], s, (size_t) n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_blorst(RkP *p, int pos) {
    if (ch(p, pos) == '{') { int e = r_block(p, pos); p->blk_n = -1; return e; }
    if (ch(p, pos) == ';' || pos >= p->n) panic_at(p, pos, "Missing block or statement");
    tree_t *sl = p->cur_list; int sbk = p->bk;
    tree_t *tmp = p->build ? ast_node_new(TT_SEQ_EXPR) : NULL;
    p->cur_list = tmp; if (p->bk == BK_ITEMS || p->bk == BK_MAIN || p->bk == BK_CLASS || p->bk == BK_GRAMMAR) p->bk = BK_MODULE;
    int e = r_statement(p, pos);
    p->cur_list = sl; p->bk = sbk;
    if (e < 0) panic_at(p, pos, "Missing block or statement");
    p->tv = tmp && tmp->n ? tmp->c[0] : NULL; p->blk_n = -2;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_statement_prefix(RkP *p, int pos) {
    static const char *const pre[] = { "BEGIN", "TEMP", "CHECK", "INIT", "ENTER", "FIRST", "END", "LEAVE", "KEEP", "UNDO", "NEXT", "LAST", "PRE", "POST", "CLOSE",
                                        "eager", "sink", "try", "quietly", "gather", "once", "start", "supply", "react", "do", 0 };
    for (int i = 0; pre[i]; i++) {
        int e = kok(p, pos, pre[i]);
        if (e >= 0) {
            int sd = p->sup_depth, sup = !strcmp(pre[i], "react") || !strcmp(pre[i], "supply");
            if (!strcmp(pre[i], "supply")) p->want_core = 1;
            p->sup_depth += sup;
            int f = r_blorst(p, e);
            p->sup_depth = sd;
            if (p->build) { tree_t *t = taketv(p); int isblk = p->blk_n == -1 || ch(p, e) == '{'; rkb_sprefix_term(p->B, &p->tm, pos, f, pre[i], (int) strlen(pre[i]), isblk ? t : NULL,
                isblk ? NULL : t); }
            return f;
        }
    }
    static const char *const hyp[] = { "race", "hyper", "lazy", 0 };
    for (int i = 0; hyp[i]; i++) {
        int e = kok(p, pos, hyp[i]);
        if (e >= 0) {
            int f = kok(p, e, "for");
            int g = f >= 0 ? r_statement_control(p, e) : r_blorst(p, e);
            if (p->build) { tree_t *t = taketv(p); rkb_sprefix_term(p->B, &p->tm, pos, g, hyp[i], (int) strlen(hyp[i]), ch(p, e) == '{' ? t : NULL, ch(p, e) == '{' ? NULL : t); }
            return g;
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
int r_keyword_term(RkP *p, int pos) {
    int e;
    if ((e = r_statement_prefix(p, pos)) >= 0) return e;
    static const char *const scopes[] = { "my", "our", "has", "HAS", "augment", "anon", "state", "supersede", "unit", 0 };
    for (int i = 0; scopes[i]; i++) {
        int k = lit(p, pos, scopes[i]);
        if (k >= 0 && !wordch_at(p, k) && end_keyword_ok(p, k)) { int r = r_scoped(p, k, i); if (r >= 0) return r; }
        else if (k >= 0 && !wordch_at(p, k) && (ch(p, k) == '\\' || (ch(p, k) == '(' && !user_routine_n(p, p->s + pos, k - pos)))) { int r = r_scoped(p, k, i); if (r >= 0) return r; }
    }
    if ((e = r_multi_declarator(p, pos)) >= 0) return e;
    if ((e = r_routine_declarator(p, pos)) >= 0) return e;
    if ((e = r_regex_declarator(p, pos)) >= 0) return e;
    if ((e = r_package_declarator(p, pos)) >= 0) return e;
    if ((e = r_type_declarator(p, pos)) >= 0) return e;
    if ((e = kw_end(p, pos, "self")) >= 0) { if (p->build) { rkb_var(p->B, &p->tm, pos, e, NULL, 0); p->tm.cls = 'S'; } return e; }
    if ((e = kw_end(p, pos, "now")) >= 0 && !is_name_n(p, "now", 3)) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    if ((e = kw_end(p, pos, "time")) >= 0 && !is_name_n(p, "time", 4)) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    if (((e = kw_end(p, pos, "make-temp-file")) >= 0 && !is_name_n(p, "make-temp-file", 14)) || ((e = kw_end(p, pos, "make-temp-dir")) >= 0 && !is_name_n(p, "make-temp-dir", 13))) {
        int ne = e; while (ch(p, ne) == ' ' || ch(p, ne) == '\t') ne++;
        int nc = ch(p, ne);
        if (nc == ';' || nc == ')' || nc == '}' || nc == ']' || nc == ',' || nc == '.' || nc == '\n' || nc == '\r' || nc == 0) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
    }
    if ((e = kw(p, pos, "rand")) >= 0 && ch(p, e) != '-' && ch(p, e) != '\'') {
        int h = e; if (ch(p, h) == '(') h++; h = hs(p, h);
        if (is_digit_cp(cp_at(p, h)) || ch(p, h) == '$') panic_at(p, pos, "Unsupported use of rand(N); in Raku please use N.rand for Num or (^N).pick for Int result");
        if (at_lit(p, e, "()")) panic_at(p, pos, "Unsupported use of rand(); in Raku please use rand");
        if (end_keyword_ok(p, e)) { if (p->build) rkb_name(p->B, &p->tm, pos, e, e - pos); return e; }
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
        if (at_lit(p, e, "const::")) { int x = e + 7; while (is_word_cp(cp_at(p, x))) x++; if (p->build) rkb_name(p->B, &p->tm, pos, x, x - pos); return x; }
        if (ch(p, w) == '(') { int a = r_args(p, w, 0); if (p->build) rkb_call(p->B, &p->tm, pos, a, w - pos, takelst(p), 1); return a; }
        if (p->build) rkb_name(p->B, &p->tm, pos, w, w - pos);
        return w;
    }
    if ((e = kw(p, pos, "__END__")) >= 0) panic_at(p, pos, "Unsupported use of __END__ as end of code; in Raku please use the =finish pod marker and $=finish to read");
    if ((e = kw(p, pos, "__DATA__")) >= 0) panic_at(p, pos, "Unsupported use of __DATA__ as start of data; in Raku please use the =finish pod marker and $=finish to read");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_xblock(RkP *p, int pos, int implicit) {
    int xbk = p->xb_bk; p->xb_bk = 0;
    int sg = p->goal; p->goal = GOAL_BLOCK;
    int e = r_EXPR(p, pos, 0);
    RkList *cond = takelst(p);
    p->goal = sg;
    if (e < 0) panic_at(p, pos, "Missing expression");
    e = ws(p, e);
    p->next_bk = xbk;
    int f = r_pblock(p, e, implicit);
    p->next_bk = 0;
    p->xb_cond = cond; p->xb_blk = p->tv; p->xb_sig = p->pb_sig;
    if (p->build) rkb_pointy(p->B, p->xb_blk, p->xb_sig);
    return f;
}
int r_initializer(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == '=' && ch(p, pos + 1) != '=' && ch(p, pos + 1) != '>') {
        int q = ws(p, pos + 1);
        int e = r_EXPR(p, q, p->leftsigil == '$' ? PRLE('i') : PR('e'));
        if (e < 0) panic_at(p, q, "Malformed initializer");
        p->init_op = "=";
        return e;
    }
    if (at_lit(p, pos, ":=")) {
        int q = ws(p, pos + 2);
        int e = r_EXPR(p, q, PR('e'));
        if (e < 0) panic_at(p, q, "Malformed binding");
        p->init_op = ":=";
        return e;
    }
    if (at_lit(p, pos, "::=")) panic_at(p, pos, "\"::=\" not yet implemented. Sorry.");
    if (at_lit(p, pos, ".=")) {
        int q = ws(p, pos + 2);
        int e = r_dottyop(p, q);
        if (e < 0) panic_at(p, q, "Malformed mutator method call");
        if (p->build) { RkList *x = rkb_list_new(); RkEl *el = rkb_list_el(x); rkb_method_term(p->B, &el->t0, &p->pf); el->t = el->t0.t; el->nitem = 1; x->nitem = 1; p->lst = x; }
        p->init_op = ".=";
        return e;
    }
    return -1;
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
        p->init_op = NULL;
        int i = r_initializer(p, q);
        if (i < 0) panic_at(p, q, "A sigilless term definition requires an initializer");
        if (p->build) { RkDecl *d = (RkDecl *) ct_zalloc(1, sizeof(RkDecl)); d->sigil = '$'; d->name = ct_strndup0(p->s + pos + 1, e - pos - 1); d->init_op = p->init_op; d->init = takelst(p);
            p->dcl = d; }
        return i;
    }
    if (c == '$' || c == '@' || c == '%' || c == '&') {
        p->dcl = NULL;
        int e = r_variable_declarator(p, pos);
        if (e < 0) return -1;
        RkDecl *d = p->dcl;
        int q = ws(p, e);
        int thunk = p->scope == 3 || p->scope == 4;
        if (thunk) scope_enter(p);
        p->init_op = NULL;
        int i = r_initializer(p, q);
        if (d && i >= 0) { d->init_op = p->init_op; d->init = takelst(p); }
        if (thunk) scope_leave(p);
        p->dcl = d;
        return i >= 0 ? i : e;
    }
    if (c == '(') {
        int s = r_signature(p, pos + 1, 0); tree_t *sig = p->sig; s = ws(p, s);
        if (ch(p, s) != ')') panic_at(p, s, "Unable to parse signature; couldn't find final ')'");
        int e = r_traits(p, s + 1);
        int q = ws(p, e);
        p->init_op = NULL;
        int i = r_initializer(p, q);
        if (p->build) { RkDecl *d = (RkDecl *) ct_zalloc(1, sizeof(RkDecl)); d->sigil = '('; d->sig = sig; if (i >= 0) { d->init_op = p->init_op; d->init = takelst(p); } p->dcl = d; }
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
    static const char m[][6] = { "multi", "proto", "only", "" };
    for (int i = 0; m[i][0]; i++) {
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
    if ((e = kw_end(p, pos, "sub")) >= 0 && p->scope == 8) panic_at(p, pos, "Cannot use 'supersede' with sub declaration");
    if ((e = kw_end(p, pos, "sub")) >= 0) return r_routine_def(p, ws(p, e), 0);
    if ((e = kw_end(p, pos, "method")) >= 0) return r_routine_def(p, ws(p, e), 1);
    if ((e = kw_end(p, pos, "submethod")) >= 0) return r_routine_def(p, ws(p, e), 2);
    if ((e = kw_end(p, pos, "macro")) >= 0) panic_at(p, pos, "Use of macros is experimental; please 'use experimental :macros'");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_declarator(RkP *p, int pos) {
    static const char *const k[] = { "rule", "token", "regex", 0 };
    for (int i = 0; k[i]; i++) {
        int e = kok(p, pos, k[i]);
        if (e >= 0) { p->xb_bk = i; int f = r_regex_def(p, e); return f; }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_regex_def(RkP *p, int pos) {
    int q = pos; int rkind = p->xb_bk; p->xb_bk = 0;
    int n = r_deflongname(p, q);
    int nfrom = q, nto = n;
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
    if (p->build) { memset(&p->tm, 0, sizeof p->tm); p->tm.kind = TK_BSTMT; p->tm.from = pos; p->tm.to = p->tm.core_to = e + 1;
        p->tm.t = rkb_regex_decl(p->B, rkind, nto >= 0 ? p->s + nfrom : "", nto >= 0 ? nto - nfrom : 0, q); }
    return r_ENDSTMT(p, e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_scoped(RkP *p, int pos, int scope) {
    int ss = p->scope; p->scope = scope + 1;
    int q = ws(p, pos);
    int e = -1;
    int tyf = -1, tyt = -1;
    p->dcl = NULL;
    if ((e = r_declarator(p, q)) >= 0) goto done;
    if ((e = r_regex_declarator(p, q)) >= 0) goto done;
    if ((e = r_package_declarator(p, q)) >= 0) goto done;
    {
        int t = q; int nt = 0;
        for (;;) { int tn = r_typename(p, t); if (tn < 0) break; if (!nt) { tyf = t; tyt = tn; } t = ws(p, tn); nt++; }
        if (nt && scope == 1 && (ch(p, t) == '$' || ch(p, t) == '@' || ch(p, t) == '%' || ch(p, t) == '&'))
            panic_at(p, t, "Cannot put a type constraint on an 'our'-scoped variable");
        if (nt) {
            p->dcl = NULL;
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
    if (p->build && p->dcl) { RkDecl *d = p->dcl; p->dcl = NULL; d->scope = scope; if (tyf >= 0) d->type = ct_strndup0(p->s + tyf, tyt - tyf); rkb_decl(p->B, &p->tm, pos, e, d); }
    return e;
}
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
        p->modk = NULL; p->modx = NULL; p->cmodk = p->cmodx = 0; p->nmods = 0;
        RK_GROW(p->modk, p->nmods, p->cmodk, const char *); RK_GROW(p->modx, p->nmods, p->cmodx, RkList *); p->modk[0] = cond[i]; p->modx[0] = takelst(p); p->nmods = 1;
        int t = ws(p, x);
        for (int j = 0; loop[j]; j++) { int f = kok(p, t, loop[j]); if (f >= 0) { int y = r_statement_mod_expr(p, f, loop[j]); RK_GROW(p->modk, p->nmods, p->cmodk, const char *);
            RK_GROW(p->modx, p->nmods, p->cmodx, RkList *); p->modk[1] = loop[j]; p->modx[1] = takelst(p); p->nmods = 2; return y; } }
        return x;
    }
    for (int j = 0; loop[j]; j++) {
        int f = kok(p, q, loop[j]);
        if (f >= 0) { int y = r_statement_mod_expr(p, f, loop[j]); p->modk = NULL; p->modx = NULL; p->cmodk = p->cmodx = 0; p->nmods = 0;
            RK_GROW(p->modk, p->nmods, p->cmodk, const char *); RK_GROW(p->modx, p->nmods, p->cmodx, RkList *); p->modk[0] = loop[j]; p->modx[0] = takelst(p); p->nmods = 1; return y; }
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
void stmt_end_info(RkP *p, int e, int *semi, int *last) {
    int t = ws(p, e);
    *semi = ch(p, t) == ';';
    p->last_real_semi = *semi;
    if (p->bk == BK_MAIN) { *last = t >= p->n || p->finished; if (*last && !*semi) *semi = 1; }
    else *last = t >= p->n || ch(p, t) == '}';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_statement(RkP *p, int pos) {
    int c = ch(p, pos);
    if (c == ')' || c == ']' || c == '}' || pos >= p->n) return -1;
    if (c == ';') { if (p->build) { rkb_empty(p->B, p->cur_list, p->bk); p->last_real_semi = 1; } return pos; }
    int e;
    if ((e = r_label(p, pos)) >= 0) {
        char *lb = p->build ? ct_strndup0(p->s + pos, r_identifier(p, pos) - pos) : NULL;
        int n0 = p->cur_list ? p->cur_list->n : 0;
        int f = r_statement(p, e);
        if (p->build && f >= 0 && lb && p->bk == BK_ITEMS) rkb_label_item(p->B, p->stmt_list, lb);
        else if (p->build && f >= 0 && lb && p->cur_list && p->cur_list->n > n0) rkb_label(p->B, p->cur_list, n0, lb);
        return f;
    }
    p->ctl_ns = 0; p->tv = NULL;
    if ((e = r_statement_control(p, pos)) >= 0) {
        if (p->build && p->tv) { int semi, last; tree_t *t = taketv(p); int ns = p->ctl_ns; stmt_end_info(p, e, &semi, &last); if (p->bk == BK_ITEMS) p->stmt_list = rkb_item_tree(p->B, t); else rkb_control(p->B, p->cur_list, p->bk, pos, t, ns, semi, last); }
        else if (p->build) { int semi, last; stmt_end_info(p, e, &semi, &last); }
        return e;
    }
    int si = p->invocant_ok; int sq = p->qsigil; p->qsigil = 0;
    e = r_EXPR(p, pos, 0);
    RkList *X = takelst(p);
    p->invocant_ok = si; p->qsigil = sq;
    if (e >= 0) {
        int end = e; int nm = 0; const char **mk = NULL; RkList **mx = NULL;
        if (!(marked_end(p, e) || marked_end(p, ws(p, e)))) {
            p->nmods = 0;
            int m = r_statement_mods(p, e);
            if (m >= 0) { end = m; nm = p->nmods; mk = p->modk; mx = p->modx; }
        }
        if (p->build) {
            p->stmt_list = X;
            if (p->bk != BK_ITEMS) { int semi, last; stmt_end_info(p, end, &semi, &last); rkb_statement(p->B, p->cur_list, p->bk, pos, end, X, nm, mk, mx, semi, last); }
            else if (nm) rkb_items_for(p->B, X, nm, mk, mx);
        }
        return end;
    }
    if (is_terminator_at(p, pos)) return pos;
    panic_at(p, pos, "Bogus statement");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_statement_in_regex(RkP *p, int pos) {
    int e = r_statement(p, pos);
    if (e < 0) return -1;
    int q = ws(p, e);
    if (ch(p, q) == ';') return q + 1;
    if (marked_end(p, q)) return q;
    return q;
}
