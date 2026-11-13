#include "parsers/raku/rk_tree/rkt_head.h"
#include <string.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/raku/rk_tree.h"
#include "parsers/raku/rk_tree/rkt_01_sh.h"
#include "parsers/raku/rk_tree/rkt_03_sh.h"
#include "parsers/raku/rk_tree/rkt_04_sh.h"
#include "parsers/raku/rk_tree/rkt_05_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int line_at(RkB *b, int pos) {
    int lo = 0, hi = b->nnlp; if (pos > b->len) pos = b->len;
    while (lo < hi) { int m = lo + (hi - lo) / 2; if (b->nlp[m] < pos) lo = m + 1; else hi = m; }
    return lo + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int next_tok_pos(RkB *b, int pos) {
    int q = pos;
    for (;;) {
        while (q < b->len && isspace((unsigned char) b->s[q])) q++;
        if (q < b->len && b->s[q] == '#') { while (q < b->len && b->s[q] != '\n') q++; continue; }
        return q;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *strip_sigil(const char *s) { if (s && (s[0] == '$' || s[0] == '@' || s[0] == '%')) return s + 1; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_tw_bare(const char *s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned ns_hash(const char *s) { unsigned h = 2166136261u; while (*s) h = (h ^ (unsigned char) *s++) * 16777619u; return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ns_has(const NS *t, const char *s) {
    if (!s || !t->cap) return 0;
    for (unsigned m = (unsigned) t->cap - 1, i = ns_hash(s) & m; t->k[i]; i = (i + 1) & m) if (!strcmp(t->k[i], s)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ns_put(NS *t, const char *s) { unsigned m = (unsigned) t->cap - 1, i = ns_hash(s) & m; while (t->k[i]) i = (i + 1) & m; t->k[i] = s; t->n++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ns_add(NS *t, const char *s) {
    if (!s || ns_has(t, s)) return;
    if (2 * (t->n + 1) > t->cap) {
        int cap = t->cap ? t->cap * 2 : 16; NS g = { (const char **) ct_zalloc((size_t) cap, sizeof(const char *)), 0, cap };
        for (int i = 0; i < t->cap; i++) if (t->k[i]) ns_put(&g, t->k[i]);
        *t = g;
    }
    ns_put(t, intern(s));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ns_remove(NS *t, const char *s) {
    if (!s || !t->cap || !ns_has(t, s)) return;
    NS g = { (const char **) ct_zalloc((size_t) t->cap, sizeof(const char *)), 0, t->cap };
    for (int i = 0; i < t->cap; i++) if (t->k[i] && strcmp(t->k[i], s)) ns_put(&g, t->k[i]);
    *t = g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mark_arr(RkB *b, const char *bare) { ns_add(&b->arrn, bare); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_arr(RkB *b, const char *bare) { return ns_has(&b->arrn, bare); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mark_arrlit(RkB *b, const char *bare, const tree_t *rhs) {
    if (!bare || !rhs || rhs->t != TT_FNC || !rhs->v.sval || strcmp(rhs->v.sval, "__rk_arr_lit")) return;
    ns_add(&b->als, bare);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *var_ident(const char *s) { if (s && (s[0] == '@' || s[0] == '%')) return s; return strip_sigil(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_set_st_lookup(RkB *b, const char *(*look)(void *, const char *, int), void *ctx) { b->st_look = look; b->st_ctx = ctx; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_rename_var(RkB *b, RkTerm *it, const char *name) { it->name = name; it->t = var_node(b, name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_codevar(RkB *b, const char *name) { if (name && name[0] == '&' && name[1]) ns_add(&b->codev, intern(name + 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_seq(TL *stmts) { tree_t *seq = ast_node_new(TT_SEQ_EXPR); if (stmts) for (int i = 0; i < stmts->n; i++) expr_add_child(seq, stmts->v[i]); return seq; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *seq1(tree_t *stmt) { tree_t *seq = ast_node_new(TT_SEQ_EXPR); if (stmt) expr_add_child(seq, stmt); return seq; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tail_is_statement(int t) {
    switch (t) {
    case TT_RETURN: case TT_NRETURN: case TT_PROC_FAIL: case TT_SAY: case TT_SAY_FH: case TT_PRINT: case TT_PRINT_FH:
    case TT_IF: case TT_UNLESS: case TT_WHILE: case TT_UNTIL: case TT_REPEAT: case TT_FOR: case TT_DO_WHILE: case TT_CLOOP:
    case TT_EVERY: case TT_CASE: case TT_LOOP_BREAK: case TT_LOOP_NEXT: case TT_SUB_DECL: case TT_PROC_DECL:
    case TT_CLASS_DECL: case TT_RECORD_DECL: case TT_DIE: case TT_CATCH: case TT_YADA: case TT_SEQ:
    case TT_LABEL_DEF: case TT_STMT: case TT_PROGRAM: case TT_END: case TT_GATHER: case TT_GOTO_S: case TT_GOTO_F:
    case TT_GOTO_U: case TT_GOTO_DIRECT: case TT_GLOBAL: case TT_LOCAL: case TT_STATIC_DECL: case TT_INITIAL:
        return 1;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rhs_is_aliasable(tree_t *r) {
    if (!r) return 0;
    switch (r->t) { case TT_VAR: case TT_ARR_GET: case TT_HASH_GET: case TT_FIELD: case TT_TWIGIL_FIELD: case TT_INDIRECT: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_bind(tree_t *target, tree_t *rhs) { if (rk_rhs_is_aliasable(rhs)) return NULL; return expr_binary(TT_ASSIGN, target, rhs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_logical_and(tree_t *a, tree_t *c) { tree_t *s = expr_binary(TT_SEQ, a, c); s->v.ival = 1; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_seq_is_logical_and(const tree_t *t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tail_value(TL *l) {
    if (!l || l->n <= 0) return;
    tree_t *last = l->v[l->n - 1];
    if (!last || (rk_tail_is_statement(last->t) && !rk_seq_is_logical_and(last))) return;
    tree_t *r = ast_node_new(TT_RETURN); r->line = last->line; expr_add_child(r, last); l->v[l->n - 1] = r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body) {
    tree_t *n = ast_node_new(TT_CLOOP); expr_add_child(n, init); expr_add_child(n, cond); expr_add_child(n, incr); expr_add_child(n, body); return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_tw_field(const char *name) { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(name)); return fe; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_tw_post_incdec(RkB *b, const char *var, int add) {
    const char *tmp = fmt("__twpost_%d", b->twpost_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rk_tw_field(var)));
    expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tw_field(var), expr_binary(add ? TT_ADD : TT_SUB, rk_tw_field(var), rk_ilit(1))));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_catch_when_cond(tree_t *e) { return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_defaults_prologue(TL *params, tree_t *body) {
    if (!params) return body;
    TL pro = { 0 };
    for (int i = 0; i < params->n; i++) {
        tree_t *p = params->v[i];
        if (!p || p->t != TT_ASSIGN || p->n < 2) continue;
        tree_t *pv = p->c[0]; tree_t *dv = p->c[1];
        params->v[i] = pv;
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_tree_clone(pv)); ast_push(mc, leaf_sval(TT_QLIT, "defined"));
        tree_t *un = ast_node_new(TT_UNLESS); ast_push(un, mc); ast_push(un, seq1(expr_binary(TT_ASSIGN, rk_tree_clone(pv), dv)));
        tl_add(&pro, un);
    }
    if (!pro.n) return body;
    for (int i = 0; body && i < body->n; i++) tl_add(&pro, body->c[i]);
    return make_seq(&pro);
}
RkB *rkb_new(const char *src, int len) {
    RkB *b = (RkB *) ct_zalloc(1, sizeof(RkB)); b->s = src; b->len = len;
    for (int i = 0; i < len; i++) if (src[i] == '\n') { GROW(b->nlp, b->nnlp, b->cnlp, int); b->nlp[b->nnlp++] = i; }
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_set_after_line(RkB *b, int pos) { int q = pos; while (q > 0 && isspace((unsigned char) b->s[q - 1])) q--; b->after_line = line_at(b, q > 0 ? q - 1 : 0); }
RkList *rkb_list_new(void) { return (RkList *) ct_zalloc(1, sizeof(RkList)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkEl *rkb_list_el(RkList *L) { GROW(L->v, L->n, L->cap, RkEl); RkEl *e = &L->v[L->n++]; memset(e, 0, sizeof *e); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_list_end(RkList *L) { if (L->n && L->v[L->n - 1].nitem == 1 && L->v[L->n - 1].t0.kind == TK_EMPTY) { L->n--; L->trailing = L->nitem > 1; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_pend(RkEl *e, tree_t *ph, RkDecl *d) { GROW(e->pd, e->npd, e->cpd, RkPend); e->pd[e->npd].ph = ph; e->pd[e->npd].d = d; e->npd++; }
tree_t *rkb_expr(RkB *b, RkList *L) { if (!L || !L->n) return ast_node_new(TT_NUL); return el_tree(b, &L->v[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_contextualize(RkB *b, RkTerm *it, const char *meth) {
    (void) b;
    tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, it->t ? it->t : ast_node_new(TT_NUL)); ast_push(c, leaf_sval(TT_QLIT, meth));
    it->kind = TK_TREE; it->t = c; it->in = NULL; it->cnt = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_if_value(RkB *b, tree_t *t) {
    if (!t || (t->t != TT_IF && t->t != TT_UNLESS) || t->n < 2) return t;
    tree_t *yes = t->c[1], *no = t->n > 2 ? t->c[2] : var_node(b, "Empty");
    if (no && (no->t == TT_IF || no->t == TT_UNLESS)) no = rk_if_value(b, no);
    return t->t == TT_IF ? rkb_ternary(b, t->c[0], yes, no) : rkb_ternary(b, t->c[0], no, yes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_paren(RkB *b, RkList *L, int nstmts) {
    (void) nstmts;
    if (!L || !L->nitem) return make_call("__rk_arr");
    if (L->n == 1 && !L->trailing) return rk_if_value(b, el_tree(b, &L->v[0]));
    tree_t *call = make_call("__rk_arr");
    for (int i = 0; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_paren_last(RkB *b, RkList *L) { if (!L || !L->n) return make_call("__rk_arr"); tree_t *sq = ast_node_new(TT_SEQ_EXPR); for (int i = 0; i < L->n; i++) expr_add_child(sq, el_tree(b, &L->v[i])); return sq; }
tree_t *closure_of(RkTerm *t) { if (!t || t->kind != TK_BLOCK) return NULL; return t->lop; }
int rkt_rk_reads_var(const tree_t *t, const char *name) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, name)) return 1;
    for (int i = 0; i < t->n; i++) if (rkt_rk_reads_var(t->c[i], name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pcv_release(RkB *b, tree_t *sig) {
    for (int i = 0; sig && i < sig->n; i++) {
        const tree_t *p = sig->c[i]; if (p && p->t == TT_ASSIGN && p->n) p = p->c[0];
        if (!p || p->t != TT_VAR || !p->v.sval) continue;
        size_t n = strlen(p->v.sval); if (n < 7 || strcmp(p->v.sval + n - 6, "__code")) continue;
        char *bare = trimdup(p->v.sval, (int) (n - 6)); const char *ib = intern(bare);
        for (int k = b->npcv - 1; k >= 0; k--) if (!strcmp(b->pcv[k], ib)) { ns_remove(&b->codev, ib); b->pcv[k] = b->pcv[--b->npcv]; break; }
    }
}
static void app(RkB *b, tree_t *list, tree_t *t, int from) { if (!t) return; if (!t->line) t->line = line_at(b, from); ast_push(list, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_label(RkB *b, tree_t *list, int first, const char *label) {
    (void) b;
    if (!list || list->n <= first || !label) return;
    tree_t *stmt = list->c[first];
    tree_t *lab = leaf_sval(TT_LABEL_DEF, label);
    lab->line = stmt->line;
    expr_add_child(lab, stmt);
    list->c[first] = lab;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_empty(RkB *b, tree_t *list, int bk) { (void) b; if (bk == BK_CLASS || bk == BK_GRAMMAR || bk == BK_ITEMS || !list) return; ast_push(list, ast_node_new(TT_SEQ_EXPR)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_swap_nonwhen(RkB *b, int v) { int o = b->nonwhen; b->nonwhen = v; return o; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pair_seg(RkEl *e) {
    RkTerm *t = &e->t0;
    if (e->nitem == 1 && t->kind == TK_FAT && !t->npre && !t->npost) return 1;
    if (e->nitem >= 2 && t->kind == TK_QUOTE && !t->npre && !t->npost && t->t && t->t->t == TT_QLIT && e->op1 && !strcmp(e->op1, "=>")) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pair_add(RkB *b, tree_t *h, RkEl *e) {
    RkTerm *t = &e->t0;
    if (t->kind == TK_FAT) { expr_add_child(h, leaf_sval(TT_QLIT, t->name)); expr_add_child(h, t->val); return; }
    expr_add_child(h, leaf_sval(TT_QLIT, t->t->v.sval)); expr_add_child(h, el_rest(b, e));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *paren_hash(RkB *b, RkList *in) {
    if (!in || !in->n) return NULL;
    for (int i = 0; i < in->n; i++) if (!pair_seg(&in->v[i])) return NULL;
    tree_t *h = make_call("__rk_hash");
    for (int i = 0; i < in->n; i++) pair_add(b, h, &in->v[i]);
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *brace_hash(RkB *b, RkList *in) {
    if (!in || in->n < 2 || !pair_seg(&in->v[0])) return NULL;
    tree_t *h = make_call("__rk_hash");
    for (int i = 0; i < in->n; i++) { if (pair_seg(&in->v[i])) pair_add(b, h, &in->v[i]); else expr_add_child(h, el_tree(b, &in->v[i])); }
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *hash_pairs(RkB *b, RkEl **rs, int nrs) {
    if (nrs == 1 && rs[0]->nitem == 1 && rs[0]->t0.kind == TK_PAREN) return paren_hash(b, rs[0]->t0.in);
    for (int i = 0; i < nrs; i++) if (!pair_seg(rs[i])) return NULL;
    tree_t *h = make_call("__rk_hash");
    for (int i = 0; i < nrs; i++) pair_add(b, h, rs[i]);
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_decl(RkB *b, RkTerm *it, int from, int to, RkDecl *d) { (void) b; memset(it, 0, sizeof *it); it->kind = TK_DECL; it->from = from; it->to = it->core_to = to; it->decl = d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void has_items(RkB *b, tree_t *list, RkDecl *d, int from) {
    if (d->sigil == '(' && d->sig) {
        for (int i = 0; i < d->sig->n; i++) {
            tree_t *v = d->sig->c[i];
            if (!v || v->t != TT_VAR || !v->v.sval || !*v->v.sval) continue;
            RkDecl one = *d;
            one.name = (v->v.sval[0] == '@' || v->v.sval[0] == '%' || v->v.sval[0] == '$') ? v->v.sval : fmt("$%s", v->v.sval);
            one.sigil = one.name[0];
            one.sig = NULL;
            one.init = NULL;
            one.init_op = NULL;
            has_items(b, list, &one, from);
        }
        return;
    }
    const char *nm = d->name ? d->name : "$";
    int twig = strlen(nm) > 2 && (nm[1] == '.' || nm[1] == '!');
    const char *fn = twig ? nm + 1 : strip_sigil(nm);
    const char *tw = NULL, *tn = NULL, *handles = NULL;
    for (int i = 0; i < d->ntr; i++) { if (!strcmp(d->tr[i].word, "handles")) handles = d->tr[i].name; else if (!tw) { tw = d->tr[i].word; tn = d->tr[i].name; } }
    if (handles) { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *) intern(fn); expr_add_child(fv, leaf_sval(TT_QLIT, handles)); app(b, list, fv, from); return; }
    int has_init = d->init_op && !strcmp(d->init_op, "=") && d->init;
    if (nm[0] == '@' || nm[0] == '%') {
        tree_t *fv = ast_node_new(nm[0] == '@' ? TT_ARR_DECL : TT_HASH_DECL); fv->v.sval = (char *) intern(fn);
        if (has_init) {
            RkEl **rs = NULL; int nrs = 0, crs = 0;
            for (int i = 0; i < d->init->n; i++) { GROW(rs, nrs, crs, RkEl *); rs[nrs++] = &d->init->v[i]; }
            tree_t *val;
            if (nm[0] == '@') val = nrs == 1 ? rk_arr_rhs(el_tree(b, rs[0])) : rs_list(b, rs, nrs);
            else { val = hash_pairs(b, rs, nrs); if (!val) val = el_tree(b, rs[0]); }
            expr_add_child(fv, val);
        }
        app(b, list, fv, from); return;
    }
    if (has_init) {
        tree_t *e = rkb_expr(b, d->init);
        if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *) intern(fn); app(b, list, rw, from); }
        tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *) intern(fn); expr_add_child(fv, e); app(b, list, fv, from); return;
    }
    tree_t *fv;
    if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *) intern(fn); }
    else if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *) intern(fn); }
    else fv = leaf_sval(TT_VAR, fn);
    app(b, list, fv, from);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *wrap_mod(RkB *b, const char *k, RkList *cx, tree_t *base) {
    if (!strcmp(k, "for")) { tree_t *cond = rkb_paren(b, cx, 1); tree_t *gen = expr_unary(TT_ITERATE, cond); gen->v.sval = (char *) intern("_"); return expr_binary(TT_EVERY, gen, base); }
    tree_t *cond = rkb_expr(b, cx);
    if (!strcmp(k, "if") || !strcmp(k, "when")) { tree_t *e = ast_node_new(TT_IF); expr_add_child(e, cond); expr_add_child(e, base); return e; }
    if (!strcmp(k, "unless")) { tree_t *e = ast_node_new(TT_UNLESS); ast_push(e, cond); ast_push(e, base); return e; }
    if (!strcmp(k, "while")) return expr_binary(TT_WHILE, cond, base);
    if (!strcmp(k, "until")) { tree_t *e = ast_node_new(TT_UNTIL); expr_add_child(e, cond); expr_add_child(e, base); return e; }
    if (!strcmp(k, "with")) return call2("__rk_with_mod", base, cond);
    if (!strcmp(k, "without")) return call2("__rk_without_mod", base, cond);
    return call2("__rk_given_mod", base, cond);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_has_placeholder(const tree_t *t) {
    if (!t || t->t == TT_ANON_BLOCK) return 0;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] == '^') return 1;
    for (int i = 0; i < t->n; i++) if (rk_has_placeholder(t->c[i])) return 1;
    return 0;
}
static tree_t *rk_mod_invoke(RkB *b, const char *k, RkList *cx, tree_t *blk) {
    if (strcmp(k, "if") && strcmp(k, "unless") && strcmp(k, "with") && strcmp(k, "without") && strcmp(k, "given")) return wrap_mod(b, k, cx, blk);
    tree_t *cond = rkb_expr(b, cx);
    const char *tmp = fmt("__modv_%d", b->post_uid++);
    tree_t *inv = ast_node_new(TT_INVOKE); expr_add_child(inv, blk); expr_add_child(inv, leaf_sval(TT_VAR, tmp));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, inv);
    tree_t *out = ast_node_new(TT_SEQ_EXPR); expr_add_child(out, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), cond));
    if (!strcmp(k, "given")) { expr_add_child(out, inv); return out; }
    tree_t *test = leaf_sval(TT_VAR, tmp);
    if (!strcmp(k, "with") || !strcmp(k, "without")) { test = make_call("__rk_defined"); expr_add_child(test, leaf_sval(TT_VAR, tmp)); }
    tree_t *e = ast_node_new(!strcmp(k, "if") || !strcmp(k, "with") ? TT_IF : TT_UNLESS); expr_add_child(e, test); expr_add_child(e, seq);
    expr_add_child(out, e);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_statement(RkB *b, tree_t *list, int bk, int from, int to, RkList *L, int nmods, const char **modk, RkList **modx, int semi, int last) {
    (void) to;
    if (bk == BK_ITEMS || !list || !L || !L->nitem || !L->n) return;
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    int single = L->nitem == 1;
    if (bk == BK_CLASS) {
        if (single && t0->kind == TK_DECL && t0->decl && t0->decl->scope == 2) { has_items(b, list, t0->decl, from); return; }
        if (single && t0->kind == TK_BSTMT) { app(b, list, t0->t, from); return; }
        app(b, list, stmt_plain(b, L), from); return;
    }
    if (bk == BK_GRAMMAR) { app(b, list, el_t0(b, e0), from); return; }
    int ns = 1;
    if (single && !nmods && !t0->npre && !t0->npost) {
        if (t0->kind == TK_BSTMT) ns = t0->ck == 2 ? 2 : t0->ck == 1 ? 1 : 0;
        else if (t0->kind == TK_BLOCK && t0->val) ns = 0;
    }
    if (bk == BK_CATCH) b->nonwhen++;
    if (ns != 1) {
        app(b, list, t0->kind == TK_BLOCK ? t0->val : t0->t, from);
        if (semi && ns == 0) rkb_empty(b, list, bk);
        return;
    }
    int tail = !semi && last && (bk == BK_BLOCK || bk == BK_SUB || bk == BK_METHOD || bk == BK_CATCH || bk == BK_GIVEN);
    tree_t *tr;
    if (nmods) {
        tr = assign_forms(b, L, 0, BK_MAIN);
        tree_t *pblk = (!tr && single && t0->kind == TK_BLOCK && t0->t && t0->t->t == TT_ANON_BLOCK && t0->t->n >= 1 && rk_has_placeholder(t0->t->c[0]) && !t0->npre && !t0->npost) ? t0->t : NULL;
        if (!tr) tr = pblk ? pblk : (single && t0->kind == TK_BLOCK && t0->val && !t0->npre && !t0->npost) ? t0->val : el_tree(b, e0);
        for (int i = 0; i < nmods; i++) tr = pblk ? rk_mod_invoke(b, modk[i], modx[i], tr) : wrap_mod(b, modk[i], modx[i], tr);
    }
    else if (tail) tr = stmt_tail(b, L, bk);
    else tr = stmt_plain(b, L);
    app(b, list, tr, from);
    if (tail) { b->tail_list = list; b->tail_tree = tr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_items_set(RkList *L, tree_t *t) {
    RkEl e; memset(&e, 0, sizeof e); e.t = t; e.nitem = 1; e.t0.kind = TK_TREE; e.t0.t = t;
    L->v[0] = e; L->n = 1; L->nitem = 1; L->op1 = NULL; L->trailing = 0;
    L->rg_lo = L->rg_hi = NULL; L->rg_op = NULL; L->rg_nitem = 0; memset(&L->t1, 0, sizeof L->t1);
}
void rkb_items_for(RkB *b, RkList *L, int nmods, const char **modk, RkList **modx) {
    if (!L || !L->nitem || !L->n || nmods != 1) return;
    int isif = !strcmp(modk[0], "if"), isun = !strcmp(modk[0], "unless"), isw = !strcmp(modk[0], "with"), isno = !strcmp(modk[0], "without"), isg = !strcmp(modk[0], "given");
    if (!isif && !isun && !isw && !isno && !isg && strcmp(modk[0], "for")) return;
    tree_t *tr = assign_forms(b, L, 0, BK_MAIN);
    if (!tr) tr = el_tree(b, &L->v[0]);
    if (isw || isno || isg) {
        if (L->n > 1 || L->trailing) { tree_t *call = make_call("__rk_arr"); for (int i = 0; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i])); tr = call; }
        tree_t *cond = rkb_expr(b, modx[0]), *dt = make_call("__rk_defined");
        expr_add_child(dt, leaf_sval(TT_VAR, "_"));
        int uid = b->post_uid++;
        const char *sv = fmt("__modsv_%d", uid), *rv = fmt("__modrv_%d", uid);
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, sv), leaf_sval(TT_VAR, "_")));
        expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, "_"), cond));
        expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, rv), isg ? tr : isw ? rkb_ternary(b, dt, tr, var_node(b, "Empty")) : rkb_ternary(b, dt, var_node(b, "Empty"), tr)));
        expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, "_"), leaf_sval(TT_VAR, sv)));
        expr_add_child(seq, leaf_sval(TT_VAR, rv));
        rk_items_set(L, seq);
        return;
    }
    if (isif || isun) {
        if (L->n > 1 || L->trailing) { tree_t *call = make_call("__rk_arr"); for (int i = 0; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i])); tr = call; }
        tree_t *cond = rkb_expr(b, modx[0]);
        rk_items_set(L, isif ? rkb_ternary(b, cond, tr, var_node(b, "Empty")) : rkb_ternary(b, cond, var_node(b, "Empty"), tr));
        return;
    }
    tree_t *src = rkb_paren(b, modx[0], 1), *blk = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR), *mc = ast_node_new(TT_METHCALL);
    expr_add_child(sq, tr);
    expr_add_child(blk, sq);
    ast_push(mc, src);
    ast_push(mc, leaf_sval(TT_QLIT, "map"));
    ast_push(mc, blk);
    rk_items_set(L, mc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_label_item(RkB *b, RkList *L, const char *label) {
    if (!L || !L->n || !L->v[0].t0.t || !label) return;
    tree_t *lab = leaf_sval(TT_LABEL_DEF, label);
    lab->line = L->v[0].t0.t->line;
    expr_add_child(lab, L->v[0].t0.t);
    L->v[0].t = L->v[0].t0.t = lab;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkList *rkb_item_tree(RkB *b, tree_t *t) {
    (void) b;
    RkList *L = rkb_list_new();
    RkEl *e = rkb_list_el(L);
    e->t = t;
    e->nitem = 1;
    e->t0.kind = TK_TREE;
    e->t0.t = t;
    L->nitem = 1;
    return L;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_control(RkB *b, tree_t *list, int bk, int from, tree_t *t, int ns, int semi, int last) {
    (void) last;
    if (!list || bk == BK_ITEMS) return;
    if (bk == BK_CATCH) b->nonwhen++;
    app(b, list, t, from);
    if (semi && ns == 0) rkb_empty(b, list, bk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_temp_mark(const tree_t *m) { return m && m->t == TT_FNC && m->v.sval && !strcmp(m->v.sval, "__rk_temp") && m->n >= 2 && m->c[1] && m->c[1]->t == TT_VAR && m->c[1]->v.sval; }
static int rk_has_return(const tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_RETURN) return 1;
    if (t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < t->n; i++) if (rk_has_return(t->c[i])) return 1;
    return 0;
}
static void rk_temp_expand(RkB *b, TL *st) {
    for (int i = 0; i < st->n; i++) {
        tree_t *m = st->v[i];
        if (!rk_is_temp_mark(m)) continue;
        tree_t *var = m->c[1], *as = m->n > 2 ? m->c[2] : NULL;
        int uid = b->post_uid++;
        const char *sg = var->v.sval[0] == '@' ? "@" : var->v.sval[0] == '%' ? "%" : "";
        const char *tmp = fmt("%s__temp_%d", sg, uid), *rv = fmt("__temp_rv_%d", uid);
        TL rest = { 0 }, out = { 0 };
        for (int k = i + 1; k < st->n; k++) tl_add(&rest, st->v[k]);
        for (int k = 0; k < i; k++) tl_add(&out, st->v[k]);
        rk_temp_expand(b, &rest);
        tree_t *last = rest.n ? rest.v[rest.n - 1] : NULL;
        int rt = last && last->t == TT_RETURN && last->n >= 1, inner = rt && rk_has_return(last->c[0]);
        for (int k = 0; k < rest.n - (rt ? 1 : 0); k++) if (rk_has_return(rest.v[k])) inner = 1;
        if (inner) {
            if (as) tl_add(&out, as);
            for (int k = 0; k < rest.n; k++) tl_add(&out, rest.v[k]);
            *st = out;
            return;
        }
        tl_add(&out, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rk_tree_clone(var)));
        if (as) tl_add(&out, as);
        if (rt) rest.v[rest.n - 1] = last->c[0];
        if (rest.n) tl_add(&out, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, rv), make_seq(&rest)));
        tl_add(&out, expr_binary(TT_ASSIGN, rk_tree_clone(var), leaf_sval(TT_VAR, tmp)));
        if (rest.n && rt) { tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, leaf_sval(TT_VAR, rv)); tl_add(&out, r); }
        else if (rest.n) tl_add(&out, leaf_sval(TT_VAR, rv));
        *st = out;
        return;
    }
}
tree_t *rkb_block_seq(RkB *b, tree_t *list, int bk, int yada) {
    (void) yada;
    TL st = { 0 };
    for (int i = 0; list && i < list->n; i++) tl_add(&st, list->c[i]);
    tree_t *tail = NULL;
    if (b->tail_list == list && st.n && st.v[st.n - 1] == b->tail_tree) { tail = st.v[--st.n]; b->tail_list = NULL; }
    { int ht = tail && rk_is_temp_mark(tail); for (int i = 0; i < st.n && !ht; i++) ht = rk_is_temp_mark(st.v[i]); if (ht) { if (tail) { tl_add(&st, tail); tail = NULL; } rk_temp_expand(b, &st); } }
    if (bk == BK_CLASS || bk == BK_GRAMMAR || bk == BK_MODULE) { if (tail) tl_add(&st, tail); return make_seq(&st); }
    if ((bk == BK_BLOCK || bk == BK_METHOD || bk == BK_CATCH || bk == BK_GIVEN) && tail && tail->t == TT_YADA && !st.n) { TL y = { 0 }; tl_add(&y, tail); return make_seq(&y); }
    if ((bk == BK_SUB || bk == BK_METHOD) && !tail) rk_tail_value(&st);
    if (tail) tl_add(&st, tail);
    return make_seq(&st);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_uses_topic(const tree_t *t) {
    if (!t || t->t == TT_ANON_BLOCK) return 0;
    if (t->t == TT_VAR && t->v.sval && (!strcmp(t->v.sval, "_") || t->v.sval[0] == '^')) return 1;
    for (int i = 0; i < t->n; i++) if (rk_uses_topic(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_block_term(RkB *b, RkTerm *it, int from, int to, tree_t *seq, tree_t *sig, int sub, RkList *last, int nstmts) {
    memset(it, 0, sizeof *it); it->kind = TK_BLOCK; it->from = from; it->to = it->core_to = to; it->val = (sig || sub) ? NULL : seq; it->lop = rkb_paren(b, last, nstmts);
    if (!sig && nstmts == 1 && last && last->n >= 1) { tree_t *h = paren_hash(b, last); if (!h) h = brace_hash(b, last); if (h && !rk_uses_topic(h)) { it->kind = TK_TREE; it->t = h; return; } }
    rk_pcv_release(b, sig);
    tree_t *a = ast_node_new(TT_ANON_BLOCK); expr_add_child(a, seq);
    a->slen = sub ? 1 : 0;
    for (int i = 0; sig && i < sig->n; i++) { tree_t *p = sig->c[i]; if (p && p->t == TT_ASSIGN && p->n) p = p->c[0]; expr_add_child(a, p); }
    if (!sig && rkt_rk_reads_var(seq, "@_")) { tree_t *ap = var_node(b, "@_"); expr_add_child(ap, leaf_sval(TT_QLIT, intern("*@"))); expr_add_child(a, ap); }
    if (!sig && rkt_rk_reads_var(seq, "%_")) { tree_t *hp = var_node(b, "%_"); expr_add_child(hp, leaf_sval(TT_QLIT, intern("*%"))); expr_add_child(a, hp); }
    it->t = a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_attr_prologue(TL *params, tree_t *body, int kind) {
    tree_t *pro = NULL;
    for (int i = 0; kind != 0 && i < params->n; i++) {
        tree_t *pp = params->v[i], *pv = pp && pp->t == TT_ASSIGN && pp->n ? pp->c[0] : pp;
        if (!pv || pv->t != TT_VAR || !pv->v.sval || pv->v.sval[0] != '!' || !pv->v.sval[1]) continue;
        const char *orig = pv->v.sval;
        pv->v.sval = (char *) intern(orig + 1);
        if (!pro) pro = ast_node_new(TT_SEQ_EXPR);
        tree_t *dt = make_call("__rk_defined"), *iff = ast_node_new(TT_IF);
        expr_add_child(dt, leaf_sval(TT_VAR, pv->v.sval));
        expr_add_child(iff, dt);
        expr_add_child(iff, expr_binary(TT_ASSIGN, rk_tw_field(orig), leaf_sval(TT_VAR, pv->v.sval)));
        expr_add_child(pro, iff);
    }
    if (!pro) return body;
    for (int i = 0; i < body->n; i++) expr_add_child(pro, body->c[i]);
    return pro;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_use(RkB *b, const char *name, int namelen, RkList *args) {
    char *nm = trimdup(name, namelen);
    tree_t *u = ast_node_new(TT_USE_DECL);
    char *dot = nm[0] == 'v' && isdigit((unsigned char) nm[1]) ? strchr(nm, '.') : NULL;
    if (dot) {
        *dot = 0; u->v.sval = nm;
        tree_t *acc = var_node(b, "$_");
        char *part = dot + 1;
        while (*part) { char *nx = strchr(part, '.'); if (nx) *nx = 0; tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, acc); ast_push(mc, leaf_sval(TT_QLIT, part)); acc = mc; if (!nx) break;
            part = nx + 1; }
        ast_push(u, acc);
        return u;
    }
    u->v.sval = nm;
    if (args && args->nitem) ast_push(u, rkb_expr(b, args));
    return u;
}
tree_t *rkb_require(RkB *b, const char *name, int namelen, int resolved) { (void) b; tree_t *u = ast_node_new(TT_USE_DECL); u->v.sval = trimdup(name, namelen); u->slen = resolved ? 1 : 3; return u; }
void rkb_pointy(RkB *b, tree_t *blk, tree_t *sig) {
    if (!blk || !sig || sig->n != 1 || !sig->c[0] || sig->c[0]->t != TT_VAR || !sig->c[0]->v.sval) return;
    if (b->pt_n >= b->pt_c) { b->pt_c = b->pt_c ? b->pt_c * 2 : 8; b->pt_blk = (tree_t **) ct_grow((void *) b->pt_blk, sizeof(tree_t *) * (size_t) b->pt_c); b->pt_nm = (const char **) ct_grow((void *) b->pt_nm, sizeof(const char *) * (size_t) b->pt_c); }
    b->pt_blk[b->pt_n] = blk;
    b->pt_nm[b->pt_n++] = sig->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_pointy_cond(RkB *b, tree_t *blk, tree_t *cond) {
    for (int i = 0; i < b->pt_n; i++) if (b->pt_blk[i] == blk) { tree_t *v = leaf_sval(TT_VAR, b->pt_nm[i]); v->slen |= 4; return expr_binary(TT_ASSIGN, v, cond); }
    return cond;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_defined_cond(RkB *b, tree_t *blk, tree_t *c) {
    tree_t *a = rk_pointy_cond(b, blk, c);
    if (a == c) { tree_t *t = leaf_sval(TT_VAR, "_"); a = expr_binary(TT_ASSIGN, t, c); }
    tree_t *call = make_call("__rk_defined");
    expr_add_child(call, a);
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_if(RkB *b, int n, const char **kw, RkList **cond, tree_t **blk, tree_t *els) {
    tree_t **cs = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (n ? n : 1));
    for (int i = 0; i < n; i++) {
        int def = kw && kw[i] && (!strcmp(kw[i], "with") || !strcmp(kw[i], "orwith"));
        cs[i] = def ? rk_defined_cond(b, blk[i], rkb_expr(b, cond[i])) : rk_pointy_cond(b, blk[i], rkb_expr(b, cond[i]));
    }
    tree_t *acc = els;
    for (int i = n - 1; i >= 0; i--) { tree_t *e = ast_node_new(TT_IF); expr_add_child(e, cs[i]); expr_add_child(e, blk[i]); if (acc) expr_add_child(e, acc); acc = e; }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_unless(RkB *b, RkList *cond, tree_t *blk, int defined) {
    tree_t *e = ast_node_new(TT_UNLESS);
    ast_push(e, defined ? rk_defined_cond(b, blk, rkb_expr(b, cond)) : rk_pointy_cond(b, blk, rkb_expr(b, cond)));
    ast_push(e, blk);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_while(RkB *b, int until, RkList *cond, tree_t *blk) {
    tree_t *c = rk_pointy_cond(b, blk, rkb_expr(b, cond));
    if (!until) return expr_binary(TT_WHILE, c, blk);
    tree_t *e = ast_node_new(TT_UNTIL); expr_add_child(e, c); expr_add_child(e, blk); return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_repeat(RkB *b, int until, tree_t *blk, RkList *cond) {
    tree_t *e = ast_node_new(TT_REPEAT); expr_add_child(e, blk);
    if (cond) { expr_add_child(e, rkb_expr(b, cond)); e->v.ival = until ? 2 : 1; } else e->v.ival = 0;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_loop_clause(RkB *b, RkList *L) {
    TL l = { 0 };
    for (int i = 0; i < L->n; i++) {
        RkEl *el = &L->v[i];
        if (el->t0.kind == TK_DECL && el->t0.decl) tl_add(&l, expr_binary(TT_ASSIGN, var_node(b, el->t0.decl->name), rkb_expr(b, el->t0.decl->init)));
        else tl_add(&l, el_tree(b, el));
    }
    return make_seq(&l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_loop(RkB *b, RkList *e1, RkList *e2, RkList *e3, int parens, tree_t *blk) {
    if (!parens) return expr_binary(TT_WHILE, rk_ilit(1), blk);
    tree_t *init;
    if (e1 && e1->n > 1) init = rk_loop_clause(b, e1);
    else if (e1 && e1->nitem == 1 && e1->n && e1->v[0].t0.kind == TK_DECL && e1->v[0].t0.decl) {
        RkDecl *d = e1->v[0].t0.decl;
        tree_t *v = rkb_expr(b, d->init);
        init = expr_binary(TT_ASSIGN, var_node(b, d->name), v);
    }
    else init = rkb_expr(b, e1);
    tree_t *cond = rkb_expr(b, e2);
    tree_t *incr = e3 && e3->n > 1 ? rk_loop_clause(b, e3) : rkb_expr(b, e3);
    return rk_cstyle_loop(init, cond, incr, blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_given(RkB *b, RkList *topic, tree_t *whens) {
    tree_t *ec = ast_node_new(TT_CASE); expr_add_child(ec, rkb_expr(b, topic));
    for (int i = 0; whens && i < whens->n; i++) expr_add_child(ec, whens->c[i]);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_when(RkB *b, tree_t *list, int bk, RkList *cond, tree_t *blk) {
    if (!list) return;
    tree_t *c = rkb_expr(b, cond);
    if (bk == BK_CATCH) c = rk_catch_when_cond(c);
    tree_t *w = ast_node_new(TT_ARM); expr_add_child(w, c); expr_add_child(w, blk); ast_push(list, w);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_default(RkB *b, tree_t *list, tree_t *blk) { (void) b; if (!list) return; tree_t *w = ast_node_new(TT_ARM); expr_add_child(w, blk); ast_push(list, w); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_catch(RkB *b, tree_t *body, int whens) {
    tree_t *e = ast_node_new(TT_CATCH);
    if (whens && body && body->n) {
        tree_t *ec = ast_node_new(TT_CASE); expr_add_child(ec, leaf_sval(TT_VAR, intern("_")));
        for (int i = 0; i < body->n; i++) expr_add_child(ec, body->c[i]);
        ast_push(e, seq1(ec)); return e;
    }
    ast_push(e, rkb_block_seq(b, body, BK_BLOCK, 0));
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_phaser_word(const char *w, int blk) {
    static const char *const ph[] = { "BEGIN", "CHECK", "INIT", "END", "ENTER", "LEAVE", "KEEP", "UNDO", "PRE", "POST", "FIRST", "LAST", "NEXT", "TEMP", "CLOSE", 0 };
    for (int i = 0; ph[i]; i++) if (!strcmp(ph[i], w)) return 1;
    if (blk && (!strcmp(w, "once") || !strcmp(w, "quietly") || !strcmp(w, "react") || !strcmp(w, "do"))) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_do_for_map(tree_t *st) {
    if (!st || st->t != TT_EVERY || st->n != 2 || !st->c[0] || st->c[0]->t != TT_ITERATE || st->c[0]->n != 1) return NULL;
    const char *v = st->c[0]->v.sval;
    if (v && strcmp(v, "_")) return NULL;
    tree_t *e = st->c[1];
    while (e && e->t == TT_SEQ_EXPR && e->n == 1) e = e->c[0];
    if (!e || e->t == TT_SEQ_EXPR) return NULL;
    tree_t *m = ast_node_new(TT_MAP); ast_push(m, e); ast_push(m, st->c[0]->c[0]); return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_sprefix_term(RkB *b, RkTerm *it, int from, int to, const char *word, int wlen, tree_t *blk, tree_t *stmt) {
    (void) b;
    memset(it, 0, sizeof *it); it->from = from; it->to = it->core_to = to;
    char *w = trimdup(word, wlen);
    if (is_phaser_word(w, blk != NULL)) { it->kind = TK_BSTMT; it->ck = blk ? 2 : 1; it->t = rk_phaser_mark(w, blk ? blk : seq1(stmt)); return; }
    if (!strcmp(w, "try")) { it->kind = blk ? TK_BSTMT : TK_TREE; tree_t *e = ast_node_new(TT_TRY); ast_push(e, blk ? blk : seq1(stmt)); it->t = e; return; }
    if (!strcmp(w, "gather")) { it->kind = TK_TREE; tree_t *g = ast_node_new(TT_GATHER); expr_add_child(g, blk ? blk : stmt); it->t = g; return; }
    if (!strcmp(w, "eager") && !blk && stmt && (stmt->t == TT_GATHER || stmt->t == TT_FNC || stmt->t == TT_METHCALL)) {
        tree_t *m = ast_node_new(TT_METHCALL);
        ast_push(m, stmt);
        ast_push(m, leaf_sval(TT_QLIT, "eager"));
        it->kind = TK_TREE;
        it->t = m;
        return;
    }
    if (!strcmp(w, "do") && !blk) { tree_t *m = rk_do_for_map(stmt); if (m) { it->kind = TK_TREE; it->t = m; return; } }
    it->kind = TK_TREE; it->t = blk ? blk : stmt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_arrays(RkB *b, tree_t *t) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && (is_arr(b, t->v.sval) || ns_has(&b->als, t->v.sval))) t->slen |= 2;
    for (int i = 0; i < t->n; i++) mark_arrays(b, t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_program(RkB *b, tree_t *list) {
    TL l = { 0 };
    for (int i = 0; list && i < list->n; i++) tl_add(&l, list->c[i]);
    TL *all = &l;
    tree_t *prog = ast_stmt_new(TT_PROGRAM);
    for (int i = 0; i < all->n; i++) {
        tree_t *e = all->v[i];
        if (!e) continue;
        tree_t *st = ast_stmt_new(TT_STMT);
        expr_add_child(st, ast_attr_int(":line", 0));
        expr_add_child(st, ast_attr_int(":stno", 0));
        expr_add_child(st, ast_attr_expr(":subj", e));
        expr_add_child(prog, st);
    }
    mark_arrays(b, prog);
    return prog;
}
