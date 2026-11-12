#include "parsers/raku/rk_tree/rkt_head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/raku/rk_tree.h"
#include "parsers/raku/rk_syntax.h"
#include "parsers/raku/rk_tree/rkt_01_sh.h"
#include "parsers/raku/rk_tree/rkt_02_sh.h"
#include "parsers/raku/rk_tree/rkt_03_sh.h"
#include "parsers/raku/rk_tree/rkt_04_sh.h"
#include "parsers/raku/rk_tree/rkt_05_rk.h"
char *fmt(const char *f, ...) {
    va_list a; va_start(a, f); int n = vsnprintf(NULL, 0, f, a); va_end(a);
    char *r = (char *) ct_alloc((size_t) n + 1); va_start(a, f); vsnprintf(r, (size_t) n + 1, f, a); va_end(a); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *spn(RkB *b, int from, int to) { if (to < from) to = from; char *r = (char *) ct_alloc((size_t) (to - from) + 1); memcpy(r, b->s + from, (size_t) (to - from)); r[to - from] = 0; return r;
    }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *trimdup(const char *s, int n) {
    while (n > 0 && isspace((unsigned char) s[n - 1])) n--;
    while (n > 0 && isspace((unsigned char) *s)) { s++; n--; }
    char *r = (char *) ct_alloc((size_t) n + 1); memcpy(r, s, (size_t) n); r[n] = 0; return r;
}
tree_t *leaf_sval(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *var_node(RkB *b, const char *name) {
    if (name && name[0] == '&' && ns_has(&b->codev, name + 1)) return leaf_sval(TT_VAR, fmt("%s__code", name + 1));
    const char *id = var_ident(name);
    if (name && name[0] == '@') mark_arr(b, id);
    tree_t *e = leaf_sval(TT_VAR, id);
    if (name && name[0] != '$' && name[0] != '@' && name[0] != '%') e->slen = 1;
    else if (name && name[1] && strstr(name + 1, "::") && !strchr("*?!.^=~:", name[1] == ':' ? 'a' : name[1])) e->slen |= RK_VAR_PKG;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *make_call(const char *name) { tree_t *e = leaf_sval(TT_FNC, name); tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name); expr_add_child(e, n); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *call2(const char *fn, tree_t *l, tree_t *r) { tree_t *c = make_call(fn); expr_add_child(c, l); expr_add_child(c, r); return c; }
tree_t *rk_ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_adverb_bool(int v) { tree_t *b = make_call("__rk_mkbool"); expr_add_child(b, rk_ilit(v)); return b; }
static tree_t *rk_adverb_bool_n(int v, const char *name) { tree_t *b = rk_adverb_bool(v); if (name && *name) expr_add_child(b, leaf_sval(TT_QLIT, name)); return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_multi_mangle(const char *base, TL *params, int kind) {
    int np = params ? params->n : 0, nnamed = 0;
    for (int i = 0; kind == 0 && i < np; i++) if (params->v[i] && (params->v[i]->slen & RK_PV_NAMED)) nnamed++;
    SB b = { 0 };
    for (const char *c = fmt("%s$%d", base, np - nnamed); *c; c++) sb_c(&b, *c);
    for (int pass = 0; pass < (nnamed ? 2 : 1); pass++) for (int i = 0; i < np; i++) {
        tree_t *p = params->v[i];
        int named = nnamed && p && (p->slen & RK_PV_NAMED);
        if (named != pass) continue;
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : (p && p->v.sval && p->v.sval[0] == '@') ? "Positional" : (p && p->v.sval && p->v.sval[0] == '%') ? "Associative" : (p && p->v.sval && p->v.sval[0] == '&') ? "Callable" : "Any";
        if (!strcmp(ty, "*@") || !strcmp(ty, "**@") || !strcmp(ty, "+@") || !strcmp(ty, "+")) ty = "Slurpy";
        if (!strcmp(ty, "@")) ty = "Positional";
        else if (!strcmp(ty, "Any") && p && p->v.sval && strlen(p->v.sval) > 6 && !strcmp(p->v.sval + strlen(p->v.sval) - 6, "__code")) ty = "Callable";
        sb_c(&b, '$');
        if (named) { for (const char *c = fmt("N_%s_%c", p->v.sval ? p->v.sval : "", (p->slen & RK_PV_REQ) ? 'R' : 'O'); *c; c++) sb_c(&b, *c); continue; }
        for (const char *c = ty; *c; c++) sb_c(&b, (*c == ':') ? '_' : *c);
    }
    return sb_str(&b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_param(RkB *b, const char *type, const char *name) { tree_t *p = var_node(b, name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_phaser_mark(const char *name, tree_t *body) {
    tree_t *m = make_call(fmt("__rk_phaser_%s", name ? name : "")); expr_add_child(m, body ? body : ast_node_new(TT_SEQ_EXPR)); return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_quiet_store(tree_t *target, tree_t *rhs) { tree_t *a = expr_binary(TT_ASSIGN, target, rhs); a->v.ival = 1; return a; }
tree_t *rk_container_bind(tree_t *target, tree_t *rhs) { tree_t *a = expr_binary(TT_ASSIGN, target, rhs); a->v.ival = 2; return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_incdec(RkB *b, const char *var, int add) { return rk_quiet_store(var_node(b, var), expr_binary(add ? TT_ADD : TT_SUB, var_node(b, var), rk_ilit(1))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_post_incdec(RkB *b, const char *var, int add) {
    const char *tmp = fmt("__post_%d", b->post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), call2("__rk_dor", var_node(b, var), rk_ilit(0))));
    expr_add_child(seq, rk_quiet_store(var_node(b, var), expr_binary(add ? TT_ADD : TT_SUB, var_node(b, var), rk_ilit(1))));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_destructure(RkB *b, TL *targets, tree_t *rhs_arr) {
    (void) b;
    tree_t *c = make_call("__rk_destructure");
    expr_add_child(c, rhs_arr);
    for (int i = 0; targets && i < targets->n; i++) expr_add_child(c, targets->v[i]);
    return c;
}
tree_t *rk_for_multi(RkB *b, TL *vars, TL *acc, tree_t *list, tree_t *body, int destr) {
    const char *av = fmt("__fm_a_%d", b->fm_uid), *iv = fmt("__fm_i_%d", b->fm_uid); b->fm_uid++;
    int k = vars ? vars->n : 0;
    if (list && list->t == TT_TO && list->n >= 2) list = rk_arr_rhs(list);
    tree_t *hoist = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(av)), list);
    tree_t *init = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), rk_ilit(0));
    tree_t *el = make_call("elems"); expr_add_child(el, leaf_sval(TT_VAR, intern(av)));
    tree_t *cond = expr_binary(TT_LT, leaf_sval(TT_VAR, intern(iv)), el);
    tree_t *incr = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), expr_binary(TT_ADD, leaf_sval(TT_VAR, intern(iv)), rk_ilit(destr ? 1 : k)));
    tree_t *seq = ast_node_new(TT_SEQ);
    for (int i = 0; i < k; i++) {
        tree_t *idx = leaf_sval(TT_VAR, intern(iv));
        if (i && !destr) idx = expr_binary(TT_ADD, idx, rk_ilit(i));
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, intern(av))); expr_add_child(get, idx);
        if (destr && acc && i < acc->n && acc->v[i]) { tree_t *mc = ast_node_new(TT_METHCALL); expr_add_child(mc, get); expr_add_child(mc, acc->v[i]); get = mc; }
        else if (destr) { tree_t *inner = make_call("__rk_arr_at"); expr_add_child(inner, get); expr_add_child(inner, rk_ilit(i)); get = inner; }
        if (vars->v[i] && vars->v[i]->t == TT_VAR) vars->v[i]->slen |= 4;
        expr_add_child(seq, expr_binary(TT_ASSIGN, vars->v[i], get));
    }
    expr_add_child(seq, body);
    tree_t *outer = ast_node_new(TT_SEQ);
    expr_add_child(outer, hoist); expr_add_child(outer, rk_cstyle_loop(init, cond, incr, seq));
    return outer;
}
void rk_range_fl_arg(tree_t *call, const tree_t *to) { if (to->v.ival) expr_add_child(call, rk_ilit(to->v.ival)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_arr_index(RkB *b, const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(b, arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]); rk_range_fl_arg(call, idx);
        return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(b, arr)); ast_push(c, idx); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_dec(tree_t *hi) { if (hi && hi->t == TT_ILIT) return rk_ilit(hi->v.ival - 1); return expr_binary(TT_SUB, hi, rk_ilit(1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_arr_end_index(RkB *b, const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(b, arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(b, arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_arr_all(RkB *b, const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(b, arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(b, arr)); expr_add_child(call, rk_ilit(0)); expr_add_child(call, rk_dec(el));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_named_call(const char *fname, TL *pos, TL *named) {
    tree_t *c = make_call("__rk_named_call");
    expr_add_child(c, leaf_sval(TT_QLIT, fname));
    expr_add_child(c, rk_ilit(pos ? pos->n : 0));
    if (pos) for (int i = 0; i < pos->n; i++) expr_add_child(c, pos->v[i]);
    if (named) for (int i = 0; i < named->n; i++) expr_add_child(c, named->v[i]);
    return c;
}
static int rq_closer(int o) { return o == '(' ? ')' : o == '[' ? ']' : o == '{' ? '}' : o == '<' ? '>' : o; }
static int rq_find_close(const char *s, int i, int to, int open, int close) {
    int depth = 1;
    for (; i < to; i++) {
        if (s[i] == '\\' && i + 1 < to) { i++; continue; }
        if (open != close && s[i] == open) depth++;
        else if (s[i] == close && --depth == 0) return i;
    }
    return -1;
}
static int rq_ordinal(const char *n, int len) {
    if (len < 3) return 0;
    const char *suf = n + len - 2;
    if (strncmp(suf, "st", 2) && strncmp(suf, "nd", 2) && strncmp(suf, "rd", 2) && strncmp(suf, "th", 2)) return 0;
    for (int i = 0; i < len - 2; i++) if (!isdigit((unsigned char) n[i])) return 0;
    return atoi(n);
}
static int rq_is(const char *name, int nlen, const char *lit) { return (size_t) nlen == strlen(lit) && !memcmp(name, lit, (size_t) nlen); }
static const char *rq_word_lit(const char *w, int wl) {
    static const char *const lits[] = { "m", "rx", "ms", "mm", "s", "S", "ss", "Ss", "tr", "TR" };
    for (size_t k = 0; k < sizeof lits / sizeof *lits; k++) if (rq_is(w, wl, lits[k])) return lits[k];
    return NULL;
}
static void rq_addflag(RkRq *q, const char *f) { if (strlen(q->rxflags) + strlen(f) + 2 < (size_t) q->rxcap) { strcat(q->rxflags, f); strcat(q->rxflags, " "); } }
static void rq_apply(RkRq *q, const char *name, int nlen, int neg, const char *arg, int alen) {
    int ord = rq_ordinal(name, nlen);
    if (ord) { if (q->nth_n < q->nth_cap) q->nth[q->nth_n++] = ord; q->has_adv = 1; return; }
    if (rq_is(name, nlen, "i") || rq_is(name, nlen, "ignorecase")) { rq_addflag(q, neg ? ":!i" : ":i"); return; }
    if (rq_is(name, nlen, "ii")) { rq_addflag(q, ":ii"); return; }
    if (rq_is(name, nlen, "m") || rq_is(name, nlen, "ignoremark")) { rq_addflag(q, neg ? ":!m" : ":m"); return; }
    if (rq_is(name, nlen, "mm")) { rq_addflag(q, ":mm"); return; }
    if (rq_is(name, nlen, "s") || rq_is(name, nlen, "sigspace")) { rq_addflag(q, neg ? ":!s" : ":s"); return; }
    if (rq_is(name, nlen, "ss")) { rq_addflag(q, ":ss"); return; }
    if (rq_is(name, nlen, "r") || rq_is(name, nlen, "ratchet")) { rq_addflag(q, neg ? ":!r" : ":r"); return; }
    if (rq_is(name, nlen, "g") || rq_is(name, nlen, "global")) { q->g = !neg; q->has_adv = !neg; return; }
    if (rq_is(name, nlen, "ov") || rq_is(name, nlen, "overlap")) { q->ov = !neg; q->has_adv = !neg; return; }
    if (rq_is(name, nlen, "ex") || rq_is(name, nlen, "exhaustive")) { q->ex = !neg; q->has_adv = !neg; return; }
    if (rq_is(name, nlen, "c") || rq_is(name, nlen, "continue")) { q->cont = 1; q->c_val = alen ? atoi(arg) : 0; q->has_adv = 1; return; }
    if (rq_is(name, nlen, "p") || rq_is(name, nlen, "pos")) { q->posn = 1; q->p_val = alen ? atoi(arg) : 0; q->has_adv = 1; return; }
    if (rq_is(name, nlen, "x")) {
        if (!alen) { q->bad = 1; return; }
        const char *dd = strstr(arg, "..");
        q->x_min = atoi(arg);
        q->x_max = dd ? (dd[2] == '*' ? -1 : atoi(dd[2] == '^' ? dd + 3 : dd + 2)) : q->x_min;
        q->has_adv = 1;
        return;
    }
    if (rq_is(name, nlen, "nth")) {
        if (!alen) { q->bad = 1; return; }
        const char *p = arg;
        while (p < arg + alen && q->nth_n < q->nth_cap) {
            while (p < arg + alen && (*p == ',' || *p == ' ')) p++;
            if (p >= arg + alen) break;
            q->nth[q->nth_n++] = atoi(p);
            while (p < arg + alen && isdigit((unsigned char) *p)) p++;
            if (p + 1 < arg + alen && p[0] == '.' && p[1] == '.') { q->bad = 1; return; }
        }
        q->has_adv = 1;
        return;
    }
    if (rq_is(name, nlen, "Perl5") || rq_is(name, nlen, "P5")) { rq_addflag(q, ":P5"); return; }
    q->bad = 1;
}
static int rk_rq_parse(const char *s, int from, int to, RkRq *q) {
    char *rxbuf = q->rxflags;
    int *nthbuf = q->nth, rxcap = q->rxcap, nthcap = q->nth_cap;
    memset(q, 0, sizeof *q);
    q->rxflags = rxbuf; q->rxcap = rxcap; q->nth = nthbuf; q->nth_cap = nthcap;
    if (rxcap > 0) rxbuf[0] = 0;
    q->x_min = q->x_max = -2;
    q->word = "";
    int i = from, wl = 0;
    if (isalpha((unsigned char) s[i])) {
        while (i + wl < to && isalnum((unsigned char) s[i + wl])) wl++;
        q->word = rq_word_lit(s + i, wl);
        if (!q->word) return 0;
        i += wl;
    }
    if (!strcmp(q->word, "ms") || !strcmp(q->word, "ss") || !strcmp(q->word, "Ss")) rq_addflag(q, ":s");
    if (!strcmp(q->word, "mm")) rq_addflag(q, ":m");
    while (i < to && s[i] == ':') {
        int j = i + 1, neg = 0;
        if (j < to && s[j] == '!') { neg = 1; j++; }
        int st = j;
        while (j < to && (isalnum((unsigned char) s[j]) || s[j] == '_')) j++;
        if (j == st) break;
        const char *arg = NULL;
        int nl = j - st, al = 0;
        if (j < to && (s[j] == '(' || s[j] == '<' || s[j] == '[')) { int c = rq_find_close(s, j + 1, to, s[j], rq_closer(s[j])); if (c < 0) return 0; arg = s + j + 1; al = c - j - 1; j = c + 1; }
        if (!strcmp(q->word, "tr") || !strcmp(q->word, "TR")) {
            const char *nm = s + st;
            if (rq_is(nm, nl, "c") || rq_is(nm, nl, "complement")) q->trf |= 1;
            else if (rq_is(nm, nl, "d") || rq_is(nm, nl, "delete")) q->trf |= 2;
            else if (rq_is(nm, nl, "s") || rq_is(nm, nl, "squash")) q->trf |= 4;
            else if (rq_is(nm, nl, "r")) q->trf |= 8;
            else q->bad = 1;
            i = j;
            continue;
        }
        rq_apply(q, s + st, nl, neg, arg, al);
        i = j;
    }
    while (i < to && isspace((unsigned char) s[i])) i++;
    if (i >= to) return 0;
    q->open = (unsigned char) s[i]; q->close = rq_closer(q->open);
    int c1 = rq_find_close(s, i + 1, to, q->open, q->close);
    if (c1 < 0) return 0;
    q->ps = i + 1; q->pe = c1;
    int e = c1 + 1;
    int two = !strcmp(q->word, "s") || !strcmp(q->word, "S") || !strcmp(q->word, "ss") || !strcmp(q->word, "Ss") || !strcmp(q->word, "tr") || !strcmp(q->word, "TR");
    if (two) {
        if (q->open == q->close) { int c2 = rq_find_close(s, e, to, q->open, q->close); if (c2 < 0) return 0; q->rs = e; q->re = (s[to - 1] == q->close && to - 1 >= e) ? to - 1 : c2; }
        else {
            while (e < to && isspace((unsigned char) s[e])) e++;
            if (e >= to) return 0;
            int o2 = (unsigned char) s[e], c2c = rq_closer(o2);
            int c2 = rq_find_close(s, e + 1, to, o2, c2c);
            if (c2 < 0) return 0;
            q->rs = e + 1; q->re = c2;
        }
    }
    return 1;
}
tree_t *rx_var(RkB *b, int sig, const char *name) {
    if (sig == '@') return var_node(b, fmt("@%s", name));
    const char *st = b->st_look ? b->st_look(b->st_ctx, fmt("$%s", name), (int) strlen(name) + 1) : NULL;
    return leaf_sval(TT_VAR, st ? st + 1 : name);
}
tree_t *rx_wrap(const char *fn, tree_t *x) { tree_t *c = make_call(fn); expr_add_child(c, x); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *closure_expr(RkB *b, RkClosure *c) { if (!c) return leaf_sval(TT_QLIT, ""); return rkb_paren(b, c->last, c->cnt); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_dq(RkB *b, RkClosure *cl, int ncl, int from, int to) { return b_dq_fl(b, cl, ncl, from, to, RKQ_QQ); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_words(RkB *b, const char *s, int n, int bs) {
    tree_t *call = make_call("__rk_arr"); int wc = 0;
    int i = 0, l = 1;
    while (i < n) {
        while (i < n && rk_word_sep_at(s, n, i, &l)) i += l;
        if (i >= n) break;
        int w = i; while (i < n && !rk_word_sep_at(s, n, i, &l)) i += l;
        SB t = { 0 };
        for (int j = w; j < i; j++) { if (bs && s[j] == '\\' && j + 1 < i && s[j + 1] == '\\') { sb_c(&t, '\\'); j++; } else sb_c(&t, s[j]); }
        expr_add_child(call, leaf_sval(TT_QLIT, sb_str(&t))); wc++;
    }
    if (wc == 1) { tree_t *only = call->c[1]; return only; }
    (void) b;
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_adv_pair(const char *name) { tree_t *pr = make_call("__rk_pair"); expr_add_child(pr, leaf_sval(TT_QLIT, name)); expr_add_child(pr, rk_adverb_bool(1)); return pr; }
static int named_form(RkTerm *t, int first) {
    if (!t || t->npre || t->npost) return 0;
    if (t->kind == TK_FAT) return 1;
    if (t->kind == TK_CP && t->ck == 'v' && t->cnt == 'P') return 1;
    if (t->kind == TK_CP && (t->ck == 'n' || t->ck == '!') && first) return 1;
    if (t->kind == TK_CP && t->ck == '$' && t->val) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_named(RkTerm *t, TL *named) {
    tl_add(named, leaf_sval(TT_QLIT, t->name));
    if (t->kind == TK_CP && (t->ck == 'n' || t->ck == '!')) {
        tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *) "__rk_mkbool"; tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *) "__rk_mkbool"; ast_push(tb, tn); ast_push(tb, rk_ilit(t->ck == 'n'));
        tl_add(named, tb); return;
    }
    tl_add(named, t->val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pos_arg(RkB *b, RkEl *e, int keep) {
    if (e->nitem == 1) {
        RkTerm *t = &e->t0;
        if (t->kind == TK_CP && !t->npre && !t->npost) {
            if (t->ck == 'n') return rk_adverb_bool_n(1, t->name);
            if (t->ck == '!') return rk_adverb_bool_n(0, t->name);
            if (t->ck == 'v' && t->cnt == 'P' && keep && t->val) { tree_t *pr = make_call("__rk_pair"); expr_add_child(pr, leaf_sval(TT_QLIT, t->name)); expr_add_child(pr, t->val); return pr; }
            if (t->ck == 'v' && t->cnt == 'P') return t->val ? t->val : make_call("__rk_undef");
            if (t->ck == 'v' && t->cnt == 'W') return t->val;
        }
    }
    return el_tree(b, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int arglist(RkB *b, RkList *L, int ctx, TL *pos, TL *named) {
    int innamed = 0;
    if (!L) return 0;
    for (int i = 0; i < L->n; i++) {
        RkEl *e = &L->v[i];
        int boolform = e->t0.kind == TK_CP && (e->t0.ck == 'n' || e->t0.ck == '!');
        int can = ctx == 1 || ((ctx == 2 || ctx == 3) && (i == 0 || innamed));
        int relaxed = !can && ctx == 2 && !boolform;
        if (named && (can || relaxed) && e->nitem == 1 && named_form(&e->t0, 1)) {
            if (can) innamed = 1;
            add_named(&e->t0, named);
            if (relaxed && named->n >= 2 && named->v[named->n - 2] && named->v[named->n - 2]->t == TT_QLIT) named->v[named->n - 2]->slen = 0x4E42;
            continue;
        }
        tl_add(pos, pos_arg(b, e, ctx == 8));
    }
    return innamed;
}
static int var_cls_of(const char *t, int n) {
    if (n < 1) return 0;
    char s0 = t[0];
    if (s0 == '$') {
        if (n == 1) return 0;
        char c1 = t[1];
        if ((n == 7 && !strncmp(t, "$*STDIN", 7)) || (n == 8 && (!strncmp(t, "$*STDOUT", 8) || !strncmp(t, "$*STDERR", 8))) || (n == 4 && !strncmp(t, "$*IN", 4)) || (n == 5 && (!strncmp(t, "$*OUT", 5) || !strncmp(t, "$*ERR", 5)))) return 'F';
        if (isdigit((unsigned char) c1)) return 'P';
        if (n == 6 && !strncmp(t, "$?LINE", 6)) return 'L';
        if ((c1 == '.' || c1 == '!') && n > 2 && (isalpha((unsigned char) t[2]) || t[2] == '_')) return 'T';
        return 'S';
    }
    if (s0 == '@') { if (n > 2 && (t[1] == '.' || t[1] == '!')) return 'U'; return 'A'; }
    if (s0 == '%') { if (n > 2 && (t[1] == '.' || t[1] == '!')) return 'W'; return 'H'; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_var(RkB *b, RkTerm *it, int from, int to, const char *nc, int nclen) {
    memset(it, 0, sizeof *it); it->kind = TK_VAR; it->from = from; it->to = it->core_to = to;
    if (nc) { tree_t *c = ast_node_new(TT_NAMED_CAPTURE); ast_push(c, leaf_sval(TT_QLIT, trimdup(nc, nclen))); it->t = c; it->cls = 'N'; return; }
    char *name = spn(b, from, to); if (!strcmp(name, "$")) name = fmt("$__rk_st_anon%d", from); it->name = name;
    if (!strcmp(name, "@$/") || !strcmp(name, "%$/")) { tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, leaf_sval(TT_VAR, "/")); ast_push(mc, leaf_sval(TT_QLIT, name[0] == '@' ? "list" : "hash")); it->t = mc; it->cls = 'V'; return; }
    int n = (int) strlen(name);
    it->cls = var_cls_of(name, n);
    switch (it->cls) {
    case 'F': { tree_t *c = ast_node_new(TT_FH_CAPTURE); ast_push(c, rk_ilit((!strcmp(name, "$*STDIN") || !strcmp(name, "$*IN")) ? 0 : (!strcmp(name, "$*STDOUT") || !strcmp(name, "$*OUT")) ? 1 : 2)); it->t = c; return; }
    case 'P': { tree_t *c = ast_node_new(TT_CAPTURE); ast_push(c, rk_ilit(atoi(name + 1))); it->t = c; return; }
    case 'L': it->t = rk_ilit(line_at(b, from)); return;
    case 'T': case 'U': case 'W': { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(name + 1)); it->t = fe; return; }
    default: { tree_t *orf = name[0] == '&' ? rk_opref(b, name) : NULL; if (!orf && name[0] == '&' && strchr(name, ':')) { char *cn = rk_op_name_canon(name + 1); if (cn != name + 1 && rk_user_canon_known(b, cn)) name = fmt("&%s", cn); } it->t = orf ? orf : var_node(b, name); return; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_wordquote(RkB *b, RkTerm *it, int from, int to, RkClosure *cl, int ncl, int cf, int ct) {
    const unsigned char *u = (const unsigned char *) b->s + from;
    int wl = 0;
    while (from + wl < to && (isalnum(u[wl]) || u[wl] == '_')) wl++;
    int kind = (wl == 1 && u[0] == 'q') ? 'q' : (wl == 2 && u[0] == 'q' && u[1] == 'q') ? 'Q' : (wl == 1 && u[0] == 'Q') ? 'R' : 0;
    if (!kind || cf < 0 || ct < cf) return 0;
    int k = wl;
    while (from + k < to && isspace(u[k])) k++;
    unsigned char d = u[k];
    if (d == ':' || isalnum(d) || d == '_') return 0;
    if (kind == 'Q') it->t = b_dq(b, cl, ncl, cf, ct);
    else if (kind == 'R') it->t = leaf_sval(TT_QLIT, spn(b, cf, ct));
    else {
        SB buf = { 0 };
        char open = (unsigned char) b->s[cf - 1] < 0x80 ? b->s[cf - 1] : 0, close = (unsigned char) b->s[ct] < 0x80 ? b->s[ct] : 0;
        for (int i = cf; i < ct; ) {
            if (b->s[i] == '\\' && i + 1 < ct && (b->s[i + 1] == '\\' || (open && b->s[i + 1] == open) || (close && b->s[i + 1] == close))) { sb_c(&buf, b->s[i + 1]); i += 2; continue; }
            sb_c(&buf, b->s[i]); i++;
        }
        it->t = leaf_sval(TT_QLIT, sb_str(&buf));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_quote(RkB *b, RkTerm *it, int from, int to, RkClosure *cl, int ncl, int bf, int bt, int qf) {
    memset(it, 0, sizeof *it); it->kind = TK_QUOTE; it->from = from; it->to = it->core_to = to;
    const unsigned char *u = (const unsigned char *) b->s + from;
    if (u[0] == '"') it->t = b_dq(b, cl, ncl, from + 1, to - 1);
    else if (u[0] == '\'') it->t = b_sq(b, from + 1, to - 1);
    else if (u[0] == 0xEF && u[1] == 0xBD && u[2] == 0xA2) it->t = leaf_sval(TT_QLIT, spn(b, from + 3, to - 3));
    else if ((u[0] == 'q' || u[0] == 'Q') && rk_wordquote(b, it, from, to, cl, ncl, bf, bt)) { }
    else if ((u[0] == 'q' || u[0] == 'Q') && bf >= 0 && b_quote_fl(b, it, cl, ncl, bf, bt, qf)) { }
    else {
        RkRq q;
        char rxbuf[2 * (to - from) + 8];
        int nthbuf[to - from + 1];
        q.rxflags = rxbuf; q.rxcap = (int) sizeof rxbuf; q.nth = nthbuf; q.nth_cap = to - from + 1;
        tree_t *rt = NULL;
        if (rk_rq_parse(b->s, from, to, &q) && !q.bad) rt = rk_rq_term(b, &q, cl, ncl);
        it->t = rt ? rt : leaf_sval(TT_QLIT, spn(b, from, to));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *mkbool_lit(int v) { tree_t *bl = ast_node_new(TT_FNC); bl->v.sval = (char *) intern("__rk_mkbool"); ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, rk_ilit(v)); return bl;
    }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_user_name(RkB *b, RkTerm *it, int from, int to, int namelen) {
    memset(it, 0, sizeof *it); it->kind = TK_NAME; it->from = from; it->to = it->core_to = to;
    char *nm = spn(b, from, from + namelen); it->name = nm;
    it->t = var_node(b, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_name(RkB *b, RkTerm *it, int from, int to, int namelen) {
    memset(it, 0, sizeof *it); it->kind = TK_NAME; it->from = from; it->to = it->core_to = to;
    char *nm = spn(b, from, from + namelen); it->name = nm;
    if (!strcmp(nm, "True") || !strcmp(nm, "False")) { it->t = mkbool_lit(!strcmp(nm, "True")); return; }
    if (rk_is_core_type(nm)) { tree_t *c = make_call("__rk_typeobj"); expr_add_child(c, leaf_sval(TT_QLIT, intern(nm))); it->t = c; return; }
    if (!strcmp(nm, "make-temp-file") || !strcmp(nm, "make-temp-dir")) { it->t = make_call(nm); return; }
    it->t = var_node(b, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_flat_call(TL *pos) {
    tree_t *c = make_call("__rk_flat");
    for (int i = 0; i < pos->n; i++) { tree_t *a = pos->v[i]; expr_add_child(c, a && a->t == TT_TO && a->n >= 2 ? rk_arr_rhs(a) : a); }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_await_call(TL *pos) {
    if (pos->n == 1) return pos->v[0];
    tree_t *c = make_call("__rk_arr"); for (int i = 0; i < pos->n; i++) expr_add_child(c, pos->v[i]); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_listop_substitutes(const char *name, int namelen) {
    static const char *const own[] = { "say", "print", "put", "take", "return", "fail", "exit", "die", "join", "map", "grep", "sort", "reverse", "exists", "delete", 0 };
    char *nm = trimdup(name, namelen);
    for (int i = 0; own[i]; i++) if (!strcmp(own[i], nm)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_listop_call(RkB *b, const char *name, int namelen, RkTerm *paren) {
    TL pos = { 0 }, kw = { 0 };
    char *nm = trimdup(name, namelen);
    int named = arglist(b, paren->in, 1, &pos, &kw);
    if (named) return rk_named_call(nm, pos.n ? &pos : NULL, &kw);
    if (!strcmp(nm, "flat")) return rk_flat_call(&pos);
    if (!strcmp(nm, "await")) return rk_await_call(&pos);
    if (!strcmp(nm, "defined") && pos.n == 1) { tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, pos.v[0]); ast_push(c, leaf_sval(TT_QLIT, "defined")); return c; }
    tree_t *call = make_call(nm); for (int i = 0; i < pos.n; i++) expr_add_child(call, pos.v[i]);
    return call;
}
void rkb_call(RkB *b, RkTerm *it, int from, int to, int namelen, RkList *args, int form) {
    memset(it, 0, sizeof *it); it->kind = TK_CALL; it->from = from; it->to = it->core_to = to;
    char *nm = spn(b, from, from + namelen);
    if (nm[0] != '&' && strchr(nm, ':')) { char *cn = rk_op_name_canon(nm); if (cn != nm && rk_user_canon_known(b, cn)) nm = cn; }
    it->name = nm;
    if (!args || !args->nitem) args = NULL;
    if (!strcmp(nm, "make") && args && args->nitem == 1) {
        TL mk = { 0 };
        arglist(b, args, 0, &mk, NULL);
        if (mk.n == 1) { tree_t *c = make_call("__rk_make"); expr_add_child(c, leaf_sval(TT_VAR, "/")); expr_add_child(c, mk.v[0]); it->t = c; return; }
    }
    if (ns_has(&b->codev, nm)) {
        tree_t *c = ast_node_new(TT_INVOKE); expr_add_child(c, leaf_sval(TT_VAR, fmt("%s__code", nm)));
        if (args) { TL cp = { 0 }; arglist(b, args, 0, &cp, NULL); for (int i = 0; i < cp.n; i++) expr_add_child(c, cp.v[i]); }
        it->t = c; return;
    }
    if (form == 0 || !args) {
        if (!strcmp(nm, "True") || !strcmp(nm, "False")) { it->kind = TK_NAME; it->t = mkbool_lit(!strcmp(nm, "True")); return; }
        if (!strcmp(nm, "last")) { it->t = ast_node_new(TT_LOOP_BREAK); return; }
        if (!strcmp(nm, "next")) { it->t = ast_node_new(TT_LOOP_NEXT); return; }
        if (!strcmp(nm, "return")) { it->t = ast_node_new(TT_RETURN); return; }
        if (!strcmp(nm, "fail")) { tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, make_call("__rk_failure")); it->t = r; return; }
        if (!strcmp(nm, "exit")) { tree_t *c = make_call("__rk_exit"); expr_add_child(c, rk_ilit(0)); it->t = c; return; }
        if (!strcmp(nm, "callsame") || !strcmp(nm, "samewith") || !strcmp(nm, "callwith") || !strcmp(nm, "nextcallee") || !strcmp(nm, "redo")) { it->t = make_call(nm); return; }
        if (!strcmp(nm, "nextsame")) { tree_t *r = ast_node_new(TT_RETURN); ast_push(r, make_call("callsame")); it->t = r; return; }
        if (form == 0 && rk_is_core_routine(nm)) { it->t = make_call(nm); return; }
        if (form == 0) { it->t = var_node(b, nm); return; }
        it->t = make_call(nm); return;
    }
    TL pos = { 0 }, named = { 0 };
    if (!strcmp(nm, "nextwith")) {
        arglist(b, args, 0, &pos, NULL);
        tree_t *c = make_call("callwith");
        for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
        tree_t *r = ast_node_new(TT_RETURN);
        ast_push(r, c);
        it->t = r;
        return;
    }
    if (!strcmp(nm, "say") || !strcmp(nm, "print") || !strcmp(nm, "put")) {
        arglist(b, args, 0, &pos, NULL); if (form == 1) flatten_paren_args(args, &pos);
        tree_t *c = ast_node_new(nm[0] == 's' ? TT_SAY : TT_PRINT); for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
        if (nm[1] == 'u') expr_add_child(c, leaf_sval(TT_QLIT, "\n"));
        it->t = c; return;
    }
    if (!strcmp(nm, "take")) {
        arglist(b, args, 0, &pos, NULL);
        if (pos.n == 1) { it->t = expr_unary(TT_SUSPEND, pos.v[0]); return; }
        tree_t *call = make_call("__rk_arr"); for (int i = 0; i < pos.n; i++) expr_add_child(call, pos.v[i]); it->t = expr_unary(TT_SUSPEND, call); return;
    }
    if (!strcmp(nm, "return")) {
        arglist(b, args, 0, &pos, NULL);
        tree_t *r = ast_node_new(TT_RETURN);
        if (pos.n > 1) { tree_t *call = make_call("__rk_arr"); for (int i = 0; i < pos.n; i++) expr_add_child(call, pos.v[i]); expr_add_child(r, call); }
        else if (pos.n) expr_add_child(r, pos.v[0]);
        it->t = r;
        return;
    }
    if (!strcmp(nm, "fail")) {
        arglist(b, args, 0, &pos, NULL);
        tree_t *r = ast_node_new(TT_RETURN), *fc = make_call("__rk_failure");
        if (pos.n) expr_add_child(fc, pos.v[0]);
        expr_add_child(r, fc);
        it->t = r;
        return;
    }
    if (!strcmp(nm, "exit")) { arglist(b, args, 0, &pos, NULL); tree_t *c = make_call("__rk_exit"); if (pos.n) expr_add_child(c, pos.v[0]); it->t = c; return; }
    if (!strcmp(nm, "die")) { arglist(b, args, 0, &pos, NULL); tree_t *d = ast_node_new(TT_DIE); if (pos.n) expr_add_child(d, pos.v[0]); it->t = d; return; }
    if (!strcmp(nm, "flat")) { arglist(b, args, 0, &pos, NULL); it->t = rk_flat_call(&pos); return; }
    if (!strcmp(nm, "await")) { arglist(b, args, 0, &pos, NULL); it->t = rk_await_call(&pos); return; }
    if (!strcmp(nm, "defined")) { arglist(b, args, 0, &pos, NULL); if (pos.n == 1) { tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, pos.v[0]); ast_push(c, leaf_sval(TT_QLIT, "defined")); it->t = c; return; } }
    if (args && args->nitem == 1 && form >= 1 && (!strcmp(nm, "Int") || !strcmp(nm, "Num") || !strcmp(nm, "Str") || !strcmp(nm, "Rat") || !strcmp(nm, "FatRat") || !strcmp(nm, "Bool") || !strcmp(nm, "Real") || !strcmp(nm, "Numeric"))) {
        arglist(b, args, 0, &pos, NULL);
        if (pos.n == 1) { tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, pos.v[0]); ast_push(c, leaf_sval(TT_QLIT, nm)); it->t = c; return; }
    }
    if (!strcmp(nm, "join") && form == 2) { arglist(b, args, 0, &pos, NULL); tree_t *e = make_call("join"); for (int i = 0; i < pos.n; i++) expr_add_child(e, pos.v[i]); it->t = e; return; }
    if (!strcmp(nm, "map") || !strcmp(nm, "grep") || !strcmp(nm, "sort")) {
        tree_e k = nm[0] == 'm' ? TT_MAP : nm[0] == 'g' ? TT_GREP : TT_SORT;
        if (k == TT_SORT && args->n >= 2 && args->v[0].t0.kind == TK_BLOCK && args->v[0].nitem == 1 && !args->v[0].t0.npre && !args->v[0].t0.npost) {
            tree_t *lst = args->n == 2 ? el_tree(b, &args->v[1]) : make_call("__rk_arr");
            if (args->n > 2) for (int i = 1; i < args->n; i++) expr_add_child(lst, el_tree(b, &args->v[i]));
            tree_t *m = ast_node_new(TT_METHCALL); ast_push(m, lst); ast_push(m, leaf_sval(TT_QLIT, nm)); ast_push(m, el_tree(b, &args->v[0])); it->t = m; return;
        }
        tree_t *c = ast_node_new(k);
        int lo = 0;
        if (args->n && args->v[0].t0.kind == TK_BLOCK && args->v[0].nitem == 1) { ast_push(c, closure_of(&args->v[0].t0)); lo = 1; }
        RkTerm *f0 = args->n >= 2 ? &args->v[0].t0 : NULL;
        if (!lo && k != TT_SORT && f0 && args->v[0].nitem == 1 && f0->kind == TK_VAR && !f0->npost && !f0->npre && (f0->cls == 'S' || (f0->name && f0->name[0] == '&' && ns_has(&b->codev, f0->name + 1)))) {
            tree_t *lst = args->n == 2 ? el_tree(b, &args->v[1]) : make_call("__rk_arr");
            if (args->n > 2) for (int i = 1; i < args->n; i++) expr_add_child(lst, el_tree(b, &args->v[i]));
            tree_t *m = ast_node_new(TT_METHCALL); ast_push(m, lst); ast_push(m, leaf_sval(TT_QLIT, nm)); ast_push(m, el_tree(b, &args->v[0])); it->t = m; return;
        }
        for (int i = lo; i < args->n; i++) ast_push(c, el_tree(b, &args->v[i]));
        if (form == 1 && lo == 0 && args->n > 1) { tree_t *a = make_call("__rk_arr"); for (int i = 0; i < c->n; i++) expr_add_child(a, c->c[i]); c->n = 0; ast_push(c, a); }
        it->t = c; return;
    }
    if (!strcmp(nm, "reverse")) { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, rkb_paren(b, args, 1)); it->t = c; return; }
    if (!strcmp(nm, "exists") || !strcmp(nm, "delete")) {
        tree_t *a = rkb_expr(b, args);
        if (a && a->t == TT_HASH_GET && nm[0] == 'e' && a->n >= 2 && rk_is_slice_key(a->c[1])) { tree_t *hc = make_call("hash_exists"); expr_add_child(hc, a->c[0]); expr_add_child(hc, a->c[1]); it->t = hc; return; }
        if (a && a->t == TT_HASH_GET) { a->t = nm[0] == 'e' ? TT_HASH_EXISTS : TT_HASH_DELETE; it->t = a; return; }
        tree_t *c = make_call(nm); expr_add_child(c, a); it->t = c; return;
    }
    if (form == 2 && args->subst) { it->t = rkb_expr(b, args); return; }
    int hn = arglist(b, args, 1, &pos, &named);
    if (hn) { it->t = rk_named_call(nm, pos.n ? &pos : NULL, &named); return; }
    if (pos.n >= 1 && pos.v[0] && pos.v[0]->t == TT_TWIGIL_FIELD) {
        static const char *const mut[] = { "push", "unshift", "append", "prepend", "pop", "shift", "splice", NULL };
        for (int i = 0; mut[i]; i++) if (!strcmp(nm, mut[i])) {
            tree_t *m = ast_node_new(TT_METHCALL); ast_push(m, pos.v[0]); ast_push(m, leaf_sval(TT_QLIT, intern(nm)));
            for (int k = 1; k < pos.n; k++) ast_push(m, pos.v[k]);
            it->t = m; return;
        }
    }
    tree_t *e = make_call(nm); for (int i = 0; i < pos.n; i++) expr_add_child(e, pos.v[i]); it->t = e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_colonpair(RkB *b, RkTerm *it, int from, int to, int ck, int kfrom, int kto, tree_t *val, int vfrom, int vto, int vkind) {
    memset(it, 0, sizeof *it); it->kind = TK_CP; it->from = from; it->to = it->core_to = to; it->ck = ck; it->cnt = vkind;
    it->name = spn(b, kfrom, kto);
    if (vkind == 'W') it->val = leaf_sval(TT_QLIT, trimdup(b->s + vfrom, vto - vfrom)); else it->val = val;
    it->t = rk_adverb_bool(ck != '!');
    if ((ck == '$' || (ck == 'v' && (vkind == 'P' || vkind == 'W'))) && it->val) { tree_t *pr = make_call("__rk_pair"); expr_add_child(pr, leaf_sval(TT_QLIT, it->name)); expr_add_child(pr, it->val); it->t = pr; }
    else if ((ck == 'n' || ck == '!') && it->name && *it->name) { tree_t *pr = make_call("__rk_pair"); expr_add_child(pr, leaf_sval(TT_QLIT, it->name)); expr_add_child(pr, rk_adverb_bool(ck == 'n')); it->t = pr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_fatarrow(RkB *b, RkTerm *it, int from, int to, int kfrom, int kto, RkList *val) {
    memset(it, 0, sizeof *it); it->kind = TK_FAT; it->from = from; it->to = it->core_to = to;
    it->name = spn(b, kfrom, kto); it->val = rkb_expr(b, val);
    tree_t *c = make_call("__rk_pair"); expr_add_child(c, leaf_sval(TT_QLIT, it->name)); expr_add_child(c, it->val); it->t = c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *method_call(RkB *b, tree_t *inv, RkPf *pf) {
    char *nm = trimdup(pf->txt ? pf->txt : "", pf->txt ? (int) strlen(pf->txt) : 0);
    tree_t *qn = pf->qname;
    if (qn && qn->t == TT_QLIT && qn->v.sval) { nm = (char *) qn->v.sval; qn = NULL; }
    if (pf->mod == '^') { char *x = (char *) ct_alloc(strlen(nm) + 2); x[0] = '^'; strcpy(x + 1, nm); nm = x; }
    if (pf->mod == '?') { char *x = (char *) ct_alloc(strlen(nm) + 2); x[0] = '?'; strcpy(x + 1, nm); nm = x; }
    if (!qn && !strcmp(nm, "take") && (!pf->args || !pf->args->nitem)) { tree_t *tk = expr_unary(TT_SUSPEND, inv); tk->v.ival = 1; return tk; }
    tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, inv); ast_push(c, qn ? qn : leaf_sval(TT_QLIT, nm));
    if (pf->args) {
        if (pf->form == 2 && pf->args->n == 1 && pf->args->nitem > 1 && pf->args->op1) { ast_push(c, stmt_plain(b, pf->args)); return c; }
        TL pos = { 0 }, named = { 0 };
        arglist(b, pf->args, !strcmp(nm, "subst") ? 8 : pf->form == 2 ? 4 : 2, &pos, &named);
        static const char *const iom[] = { "spurt", "open", "lines", "words", "mkdir", "copy", "rename", "move", "chmod", "slurp", "dir", "readchars", "seek", "first", "rotor", NULL }; int isio = 0;
        for (int k = 0; iom[k]; k++) if (!strcmp(nm, iom[k])) isio = 1;
        if (isio) for (int i = 0; i < pos.n; i++) { tree_t *a = pos.v[i];
            if (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_mkbool") && a->n >= 3 && a->c[2] && a->c[2]->t == TT_QLIT) { tree_t *pc = make_call("__rk_pair"); expr_add_child(pc, a->c[2]); tree_t *bv = rk_adverb_bool(a->c[1] && a->c[1]->v.ival ? 1 : 0); expr_add_child(pc, bv); pos.v[i] = pc; } }
        for (int i = 0; i < pos.n; i++) ast_push(c, pos.v[i]);
        for (int i = 0; i < named.n; i++) { if ((i & 1) == 0 && named.v[i] && named.v[i]->t == TT_QLIT && !named.v[i]->slen) named.v[i]->slen = 0x4E41; ast_push(c, named.v[i]); }
    }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_method_term(RkB *b, RkTerm *it, RkPf *pf) {
    memset(it, 0, sizeof *it); it->from = pf->from; it->to = it->core_to = pf->to;
    if (pf->k == 'M') { it->kind = TK_DOTTY; it->name = pf->txt; it->t = method_call(b, var_node(b, "$_"), pf); return; }
    if (pf->k == '[' && pf->inner) { tree_t *g = ast_node_new(TT_ARR_GET); ast_push(g, var_node(b, "$_")); ast_push(g, rkb_expr(b, pf->inner)); it->kind = TK_TREE; it->t = g; return; }
    if ((pf->k == '{' && pf->inner) || (pf->k == '<' && pf->txt)) { tree_t *g = ast_node_new(TT_HASH_GET); ast_push(g, var_node(b, "$_")); ast_push(g, pf->k == '<' ? leaf_sval(TT_QLIT, trimdup(pf->txt, (int) strlen(pf->txt))) : rkb_expr(b, pf->inner)); it->kind = TK_TREE; it->t = g; return; }
    if (pf->k == '(') { tree_t *c = ast_node_new(TT_INVOKE); expr_add_child(c, var_node(b, "$_")); TL pos = { 0 }; arglist(b, pf->args, 0, &pos, NULL); for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]); it->kind = TK_TREE; it->t = c; return; }
    it->kind = TK_TREE; it->t = ast_node_new(TT_NUL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_param(RkB *b, int prefix, const char *var, int varlen, int *tf, int *tt, int ntypes, RkList *dflt, int suffix, int named) {
    char *v = trimdup(var ? var : "", var ? varlen : 0);
    tree_t *p;
    if (v[0] == '&' && v[1] && prefix != '*' && prefix != 'L') { const char *bare = intern(v + 1); if (!ns_has(&b->codev, bare)) { GROW(b->pcv, b->npcv, b->cpcv, const char *); b->pcv[b->npcv++] = bare; } rkb_codevar(b, v); }
    if (prefix == '*' || prefix == 'L' || prefix == '|') p = var_node(b, v), expr_add_child(p, leaf_sval(TT_QLIT, intern(prefix != '*' ? "**@" : v[0] == '%' ? "*%" : "*@")));
    else if (prefix == '+' && !ntypes) { p = var_node(b, v); expr_add_child(p, leaf_sval(TT_QLIT, v[0] == '@' ? "+@" : "+")); }
    else if (v[0] == '@' && !ntypes) { p = var_node(b, v); expr_add_child(p, leaf_sval(TT_QLIT, "@")); }
    else if (ntypes) p = rk_typed_param(b, spn(b, tf[0], tt[0]), v);
    else p = var_node(b, v);
    if (named) p->slen |= RK_PV_NAMED;
    if (suffix == '!') p->slen |= RK_PV_REQ;
    if (suffix == '?' || dflt) p->slen |= RK_PV_OPT;
    if (dflt) p = expr_binary(TT_ASSIGN, p, rkb_expr(b, dflt));
    return p;
}
void rkb_param_key(RkB *b, tree_t *tv, const char *key, int len) {
    (void) b;
    tree_t *v = tv && tv->t == TT_ASSIGN && tv->n ? tv->c[0] : tv;
    if (!v || v->t != TT_VAR || !v->v.sval) return;
    tree_t *al = leaf_sval(TT_VAR, v->v.sval);
    al->slen |= RK_PV_ALIAS;
    v->v.sval = trimdup(key, len);
    ast_push(v, al);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *decl_node(RkB *b, const char *type, tree_t *var, tree_t *val) {
    tree_t *e = ast_node_new(TT_DECL); ast_push(e, leaf_sval(TT_VAR, type)); ast_push(e, var); if (val) ast_push(e, val); (void) b; return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *build_decl_raw(RkB *b, RkDecl *d, RkList *outer, int ofirst) {
    if (d->scope == 6 && d->sigil != '(' && d->name && d->name[0]) {
        d->scope = 0; tree_t *once = build_decl(b, d, outer, ofirst); d->scope = 6;
        tree_t *st = ast_node_new(TT_STATIC_DECL); ast_push(st, var_node(b, d->name)); ast_push(st, once); return st;
    }
    RkEl **rs = NULL; int nrs = 0, crs = 0;
    if (d->init) for (int i = 0; i < d->init->n; i++) { GROW(rs, nrs, crs, RkEl *); rs[nrs++] = &d->init->v[i]; }
    if (outer) for (int i = ofirst; i < outer->n; i++) { GROW(rs, nrs, crs, RkEl *); rs[nrs++] = &outer->v[i]; }
    const char *name = d->name ? d->name : "$";
    const char *type = d->type;
    int sig = d->sigil;
    if (sig == '(') {
        tree_t *rhs = nrs == 1 ? el_tree(b, rs[0]) : rs_list(b, rs, nrs);
        TL targets = { 0 };
        for (int i = 0; d->sig && i < d->sig->n; i++) tl_add(&targets, d->sig->c[i]);
        return rk_destructure(b, &targets, rhs);
    }
    if (!d->init_op || !nrs) {
        tree_t *var = var_node(b, name);
        if (sig == '$') for (int i = 0; i < d->ntr; i++) if (d->tr[i].word && d->tr[i].name && d->tr[i].args && !strcmp(d->tr[i].word, "is") && !strcmp(d->tr[i].name, "default")) {
            tree_t *dv = d->tr[i].args;
            if (dv->t == TT_FNC && dv->v.sval && !strcmp(dv->v.sval, "__rk_arr") && dv->n == 2) dv = dv->c[1];
            return type ? decl_node(b, type, var, dv) : expr_binary(TT_ASSIGN, var, dv);
        }
        if (type) return decl_node(b, type, var, NULL);
        if (sig == '$') { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; return expr_binary(TT_ASSIGN, var, nul); }
        return expr_binary(TT_ASSIGN, var, make_call("__rk_undef"));
    }
    if (!strcmp(d->init_op, ":=")) {
        tree_t *e = nrs > 1 || (d->init ? d->init->trailing : outer && outer->trailing) ? rs_list(b, rs, nrs) : el_tree(b, rs[0]);
        tree_t *var = var_node(b, name);
        if (sig != '$') return rk_container_bind(var, e);
        tree_t *bd = rk_bind(var, rk_scalar_rhs(e));
        if (!bd) bd = rk_container_bind(var, e);
        if (type) return decl_node(b, type, var_node(b, name), bd->c[1]);
        return bd;
    }
    if (sig == '@') {
        tree_t *val;
        if (nrs == 1 && !(d->init ? d->init->trailing : outer && outer->trailing)) { tree_t *e = el_tree(b, rs[0]); val = rk_arr_rhs(e); }
        else val = rs_list(b, rs, nrs);
        tree_t *var = var_node(b, name);
        return type ? decl_node(b, type, var, val) : expr_binary(TT_ASSIGN, var, val);
    }
    if (sig == '%') {
        tree_t *val = type ? NULL : hash_pairs(b, rs, nrs);
        if (!val) val = nrs == 1 ? el_tree(b, rs[0]) : rs_list(b, rs, nrs);
        tree_t *var = var_node(b, name);
        return type ? decl_node(b, type, var, val) : expr_binary(TT_ASSIGN, var, val);
    }
    tree_t *e = el_tree(b, rs[0]);
    if (type && d->init_op && !strcmp(d->init_op, ".=") && e && e->t == TT_METHCALL && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "_") &&
        e->c[1] && e->c[1]->t == TT_QLIT && e->c[1]->v.sval) {
        tree_t *r;
        if (!strcmp(e->c[1]->v.sval, "new")) { r = ast_node_new(TT_NEW); ast_push(r, leaf_sval(TT_QLIT, type)); for (int i = 2; i < e->n; i++) ast_push(r, e->c[i]); }
        else { r = e; r->c[0] = leaf_sval(TT_VAR, type); }
        return decl_node(b, type, var_node(b, name), r);
    }
    if (type) return decl_node(b, type, var_node(b, name), e);
    mark_arrlit(b, strip_sigil(name), e);
    return expr_binary(TT_ASSIGN, var_node(b, name), rk_scalar_rhs(e));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_group_first(const tree_t *t) {
    if (!t || t->t != TT_FNC || !t->v.sval) return 0;
    if (!strcmp(t->v.sval, "__rk_destructure")) return 2;
    return !strcmp(t->v.sval, "__rk_enum") ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mark_declared(tree_t *t) {
    if (!t) return;
    if (t->t == TT_ASSIGN && t->n && t->c[0] && t->c[0]->t == TT_VAR) { t->c[0]->slen |= 4; return; }
    if (t->t == TT_DECL && t->n >= 2 && t->c[1] && t->c[1]->t == TT_VAR) { t->c[1]->slen |= 4; return; }
    if (rk_group_first(t)) { for (int i = rk_group_first(t); i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) t->c[i]->slen |= 4; return; }
    if (t->t == TT_SEQ_EXPR || t->t == TT_UNLESS) for (int i = 0; i < t->n; i++) rk_mark_declared(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mark_our(tree_t *t) {
    if (!t) return;
    if (t->t == TT_ASSIGN && t->n && t->c[0] && t->c[0]->t == TT_VAR) { t->c[0]->slen |= RK_VAR_OUR; return; }
    if (rk_group_first(t)) { for (int i = rk_group_first(t); i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) t->c[i]->slen |= RK_VAR_OUR; return; }
    if (t->t == TT_SEQ_EXPR || t->t == TT_UNLESS) for (int i = 0; i < t->n; i++) rk_mark_our(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *build_decl(RkB *b, RkDecl *d, RkList *outer, int ofirst) { tree_t *t = build_decl_raw(b, d, outer, ofirst); rk_mark_declared(t); if (d->scope == 1 && d->sigil != '(') rk_mark_our(t); return t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_self_sig(RkB *b, tree_t *sig) {
    tree_t *ns = ast_node_new(sig ? sig->t : TT_SEQ_EXPR);
    expr_add_child(ns, var_node(b, "self"));
    for (int i = 0; sig && i < sig->n; i++) expr_add_child(ns, sig->c[i]);
    return ns;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_routine(RkB *b, int kind, int multi, const char *name, int namelen, int prefix, tree_t *sig, int has_parens, tree_t *body, RkTrait *tr, int ntr) {
    (void) prefix; (void) has_parens; (void) tr; (void) ntr;
    TL params = { 0 };
    for (int i = 0; sig && i < sig->n; i++) tl_add(&params, sig->c[i]);
    rk_pcv_release(b, sig);
    if (!body) body = ast_node_new(TT_SEQ_EXPR);
    if (rk_slash_param(&params)) {
        for (int i = 0; i < params.n; i++) { tree_t *pp = params.v[i]; if (pp && pp->t == TT_ASSIGN && pp->n) pp = pp->c[0]; if (pp && pp->t == TT_VAR && pp->v.sval && !strcmp(pp->v.sval, "/")) pp->v.sval = (char *) intern("__slash"); }
        rk_slash_rename(body);
    }
    if (params.n == 0 && !has_parens && rkt_rk_reads_var(body, "@_")) { tree_t *ap = var_node(b, "@_"); expr_add_child(ap, leaf_sval(TT_QLIT, intern("*@"))); tl_add(&params, ap); }
    if (!has_parens && (params.n == 0 || (params.n == 1 && params.v[0] && params.v[0]->v.sval && !strcmp(params.v[0]->v.sval, "@_"))) && rkt_rk_reads_var(body, "%_")) { tree_t *hp = var_node(b, "%_"); expr_add_child(hp, leaf_sval(TT_QLIT, intern("*%"))); tl_add(&params, hp); }
    body = rk_attr_prologue(&params, body, kind);
    tree_t *rkbody = rk_defaults_prologue(&params, body);
    char *nm = trimdup(name ? name : "", name ? namelen : 0);
    nm = rk_op_name_canon(nm);
    const char *mn = multi == 1 ? rk_multi_mangle(nm, &params, kind) : intern(nm);
    tree_t *e;
    if (kind == 0) { e = leaf_sval(TT_SUB_DECL, mn); e->v.ival = params.n; }
    else { e = ast_node_new(TT_SUB_DECL); e->v.ival = params.n + 1; }
    tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mn); expr_add_child(e, nn);
    for (int i = 0; i < params.n; i++) expr_add_child(e, params.v[i]);
    for (int i = 0; i < rkbody->n; i++) expr_add_child(e, rkbody->c[i]);
    (void) b;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_subset(RkB *b, const char *name, int namelen, const char *base, tree_t *where) {
    (void) b;
    char *nm = trimdup(name, namelen);
    tree_t *sd = leaf_sval(TT_SUB_DECL, intern(fmt("__ssf_%s", nm)));
    sd->v.ival = 1;
    expr_add_child(sd, leaf_sval(TT_VAR, intern(fmt("__ssf_%s", nm))));
    expr_add_child(sd, leaf_sval(TT_VAR, "__sv"));
    expr_add_child(sd, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, "_"), leaf_sval(TT_VAR, "__sv")));
    tree_t *ret = ast_node_new(TT_RETURN);
    expr_add_child(ret, mkbool_lit(1));
    tree_t *inner = ret;
    if (where) {
        tree_t *thn = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(thn, ret);
        tree_t *ifw = ast_node_new(TT_IF);
        expr_add_child(ifw, call2("__rk_smartmatch", leaf_sval(TT_VAR, "_"), where));
        expr_add_child(ifw, thn);
        inner = ifw;
    }
    if (base && *base && strcmp(base, "Any") && strcmp(base, "Mu")) {
        tree_t *dm = ast_node_new(TT_METHCALL);
        ast_push(dm, leaf_sval(TT_VAR, "__sv"));
        ast_push(dm, leaf_sval(TT_QLIT, "does"));
        ast_push(dm, leaf_sval(TT_QLIT, intern(base)));
        tree_t *thn = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(thn, inner);
        tree_t *ifb = ast_node_new(TT_IF);
        expr_add_child(ifb, dm);
        expr_add_child(ifb, thn);
        inner = ifb;
    }
    expr_add_child(sd, inner);
    tree_t *rf = ast_node_new(TT_RETURN);
    expr_add_child(rf, mkbool_lit(0));
    expr_add_child(sd, rf);
    return sd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_regex_decl(RkB *b, int kind, const char *name, int namelen, int brace) {
    tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = kind == 1 ? 0 : kind == 0 ? 1 : 2;
    ast_push(rd, leaf_sval(TT_VAR, trimdup(name ? name : "", name ? namelen : 0)));
    SB s = { 0 }; int depth = 1;
    for (int i = brace + 1; i < b->len; i++) {
        char c = b->s[i];
        if (c == '\\' && i + 1 < b->len) { sb_c(&s, c); sb_c(&s, b->s[i + 1]); i++; continue; }
        if (c == '{') { depth++; sb_c(&s, c); continue; }
        if (c == '}') { if (--depth == 0) break; sb_c(&s, c); continue; }
        sb_c(&s, c);
    }
    ast_push(rd, leaf_sval(TT_QLIT, sb_str(&s)));
    return rd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_enum_call(RkB *b, int from, int to) {
    tree_t *g = make_call("__rk_enum");
    if (to > from) { char *en = spn(b, from, to); tree_t *tv = var_node(b, en); expr_add_child(tv, leaf_sval(TT_QLIT, en)); expr_add_child(g, tv); }
    else expr_add_child(g, ast_node_new(TT_NUL));
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_enum(RkB *b, const char *text, int n, int from, int to) {
    tree_t *g = rk_enum_call(b, from, to);
    int i = 0;
    while (i < n) {
        while (i < n && (text[i] == ' ' || text[i] == '\t' || text[i] == '\n')) i++;
        if (i >= n) break;
        int w = i; while (i < n && text[i] != ' ' && text[i] != '\t' && text[i] != '\n') i++;
        expr_add_child(g, var_node(b, trimdup(text + w, i - w)));
    }
    rk_mark_declared(g);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_enum_items(RkB *b, tree_t *t, tree_t *g) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr")) { for (int i = 1; i < t->n; i++) rk_enum_items(b, t->c[i], g); return; }
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_pair") && t->n == 3 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval) {
        tree_t *v = var_node(b, t->c[1]->v.sval);
        expr_add_child(v, t->c[2]);
        expr_add_child(g, v);
        return;
    }
    if ((t->t == TT_QLIT || t->t == TT_VAR) && t->v.sval && *t->v.sval) expr_add_child(g, var_node(b, t->v.sval));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_enum_tree(RkB *b, tree_t *list, int from, int to) {
    tree_t *g = rk_enum_call(b, from, to);
    rk_enum_items(b, list, g);
    rk_mark_declared(g);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_constant(RkB *b, const char *name, int namelen, RkList *init) {
    tree_t *e = rkb_expr(b, init);
    tree_t *t = expr_binary(TT_ASSIGN, var_node(b, trimdup(name, namelen)), e);
    rk_mark_declared(t);
    return t;
}
