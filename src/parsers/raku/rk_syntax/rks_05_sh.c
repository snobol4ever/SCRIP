#include "parsers/raku/rk_syntax/rks_head.h"
#include <string.h>
#include "ir/ct_arena.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
static tree_t *x_expr(RkP *p, RkX *x);
static tree_t *x_tern(RkP *p, RkX *x);
static int x_seqinfix(RkX *x);
static void x_cross(RkP *p, RkX *x);
static void x_seq(RkP *p, RkX *x);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int asc_space(int b) { return b == ' ' || b == '\t' || b == '\n' || b == '\r' || b == '\f' || b == 0x0B; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int paren_list_term(RkP *p, int from, int to) {
    if (ch(p, from) != '(' || ch(p, to - 1) != ')') return 0;
    int d = 0, comma = 0;
    for (int k = from; k < to; k++) {
        char b = p->s[k];
        if (b == '\'' || b == '"') { char qc = b; k++; while (k < to && p->s[k] != qc) { if (p->s[k] == '\\') k++; k++; } continue; }
        if (b == '(' || b == '[' || b == '{') d++;
        else if (b == ')' || b == ']' || b == '}') { d--; if (d == 0 && k != to - 1) return 0; }
        else if (b == ',' && d == 1) comma = 1;
    }
    return comma;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *op_text(RkP *p, int from, int to) {
    while (to > from && asc_space((unsigned char) p->s[to - 1])) to--;
    while (from < to && asc_space((unsigned char) p->s[from])) from++;
    return ct_strndup0(p->s + from, to - from);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x_term(RkP *p, RkX *x, RkTerm *out) {
    int t;
    if (x->nextterm == NT_DOTTYOPISH) t = r_dottyop(p, x->q);
    else if (x->nextterm == NT_NULLTERMISH) t = r_nulltermish(p, x->q);
    else t = r_termish(p, x->q);
    if (t < 0) {
        if (x->ns) panic_at(p, x->q, "Missing required term after infix");
        x->fail = x->done = 1;
        return 0;
    }
    memset(out, 0, sizeof *out);
    if (p->build) {
        if (x->nextterm == NT_DOTTYOPISH) rkb_method_term(p->B, out, &p->pf);
        else if (p->tm.kind == TK_EMPTY) out->kind = TK_EMPTY;
        else *out = p->tm;
    }
    if (!x->nterm) x->el->t0 = *out;
    x->nterm++; x->L->nitem++;
    if (x->L->nitem == 2) x->L->t1 = *out;
    x->last_end = t; x->tq = x->q; x->tt = t; x->pk = 0; x->cur = out->t;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x_peek(RkP *p, RkX *x) {
    if (x->pk) return x->pk_e >= 0;
    x->pk = 1; x->pk_e = -1;
    if (x->done) return 0;
    if (x->noinfix) { x->done = 1; return 0; }
    int w = r_ws(p, x->tt);
    if (w < 0) { x->done = 1; return 0; }
    x->last_end = w;
    OpInfo o; int saved_ls = p->leftsigil; int e;
    StackOp *st;
    for (;;) {
        e = r_infixish(p, w, &o, 0);
        if (e < 0) break;
        if (o.prec <= x->preclim) { e = -1; p->leftsigil = saved_ls; break; }
        st = p->ops + x->base;
        int ns0 = x->ns;
        while (x->ns && st[x->ns - 1].sub > o.prec) x->ns--;
        if (x->ns == ns0 && o.to - o.from >= 2 && at_lit(p, o.from, ":=") && paren_list_term(p, x->tq, x->tt)) panic_at(p, o.from, "Cannot use bind operator with this left-hand side");
        if (at_lit(p, o.from, "==>>") || at_lit(p, o.from, "<<==")) panic_at(p, o.from, "%.4s feed operator not yet implemented. Sorry.", p->s + o.from);
        if (o.flags & OF_FAKE) { if (p->build && p->tm.kind == TK_CP) rkb_adverb(p->B, x->cur, &p->tm); x->last_end = e; w = r_ws(p, e); if (w < 0) { e = -1; break; } x->last_end = w;
            continue; }
        break;
    }
    if (e < 0) { x->done = 1; return 0; }
    st = p->ops + x->base;
    if (x->ns && st[x->ns - 1].sub == o.prec) {
        if (o.assoc == AS_NON) panic_at(p, o.from, "Operators '%.*s' and '%.*s' are non-associative and require parentheses", st[x->ns - 1].to - st[x->ns - 1].from, p->s + st[x->ns - 1].from,
            o.to - o.from, p->s + o.from);
        if (o.assoc == AS_LEFT) x->ns--;
        else if (o.assoc == AS_LIST) {
            const char *a = p->s + st[x->ns - 1].from; int al = st[x->ns - 1].to - st[x->ns - 1].from;
            const char *b = p->s + o.from; int bl2 = o.to - o.from;
            while (al > 0 && asc_space((unsigned char) a[al - 1])) al--;
            while (bl2 > 0 && asc_space((unsigned char) b[bl2 - 1])) bl2--;
            if (!(al == bl2 && !memcmp(a, b, (size_t) al)) && !(al == 1 && a[0] == ':'))
                panic_at(p, o.from, "Only identical operators may be list associative; since '%.*s' and '%.*s' differ, they are non-associative and you need to clarify with parentheses", al, a,
                    bl2, b);
        }
    }
    p->nops = x->base + x->ns;
    RK_GROW(p->ops, p->nops, p->cops, StackOp);
    st = p->ops + x->base;
    st[x->ns].prec = o.prec; st[x->ns].sub = o.sub; st[x->ns].assoc = o.assoc; st[x->ns].from = o.from; st[x->ns].to = o.to; x->ns++;
    p->nops = x->base + x->ns;
    x->pk_e = e; x->pk_o = o; x->pk_txt = o.mid ? "??" : op_text(p, o.from, o.to);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x_take(RkP *p, RkX *x) {
    x->nextterm = x->pk_o.nextterm; x->q = ws(p, x->pk_e); x->pk = 0;
    if (!x->nops++) x->L->op1 = x->pk_txt;
    if (x->nops - x->el_nops == 1 && strcmp(x->pk_txt, ",")) x->el->op1 = x->pk_txt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_lv_of_prec(int prec) { return prec == PR('u') ? LV_MUL : prec == PR('t') ? LV_ADDSUB : prec == PR('s') ? LV_REPL : prec == PR('r') ? LV_CAT : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x_in(RkP *p, RkX *x, int lv) {
    if (!x_peek(p, x)) return -1;
    int rv = 0, hy = 0;
    int k = rkb_op_index_rev(lv, x->pk_txt, &rv);
    if (k < 0) k = rkb_hyper_index(lv, x->pk_txt, &rv, &hy);
    int ng = 0;
    if (k < 0) { k = rkb_neg_index(lv, x->pk_txt, &rv); ng = k >= 0; }
    if (k < 0 && p->B && p->nuops) { int ui = rkb_user_op_find(p->B, 'i', x->pk_txt); if (ui >= 0 && rk_lv_of_prec(rkb_user_op_prec(p->B, ui)) == lv) { k = RK_UK + ui; rv = hy = ng = 0; } }
    x->rev = rv; x->hyper = hy; x->neg = ng;
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_after(RkP *p, RkX *x) { int cap = x->nops - x->el_nops == 1; tree_t *r = x_expr(p, x); if (cap) x->el->rest = r; return r; }
static tree_t *x_unary(RkP *p, RkX *x) {
    RkTerm tm, *t = &tm;
    int first = !x->nterm && !x->pend && !x->eli;
    if (x->pend) { t = x->pend; x->pend = NULL; } else if (!x_term(p, x, &tm)) return NULL;
    tree_t *base = NULL; int np = t->npre;
    const char *loose = (p->build && np && (!strcmp(t->pre[0], "so") || !strcmp(t->pre[0], "not"))) ? t->pre[0] : NULL;
    x->lastcls = 0;
    if (p->build) {
        if (np && (!strcmp(t->pre[np - 1], "++") || !strcmp(t->pre[np - 1], "--")) && t->kind == TK_VAR && t->cls == 'S' && !t->npost) { base = rkb_incdec(p->B, t->name, t->pre[np - 1][0] == '+');
            np--; }
        else if (np && (!strcmp(t->pre[np - 1], "++") || !strcmp(t->pre[np - 1], "--")) && t->kind == TK_VAR && t->npost && t->t
                 && (t->t->t == TT_ARR_GET || t->t->t == TT_HASH_GET || (t->t->t == TT_METHCALL && t->t->n == 2 && t->t->c[1] && t->t->c[1]->t == TT_QLIT))) { base = rkb_elem_incdec(p->B, t->t, t->pre[np - 1][0] == '+', 0); np--; }
        else if (t->kind == TK_DECL && !t->t) { base = ast_node_new(TT_NUL); rkb_pend(x->el, base, t->decl); }
        else base = t->t ? t->t : ast_node_new(TT_NUL);
        if (t->kind == TK_VAR && !t->npre && !t->npost) { x->lastcls = t->cls; x->lastname = t->name; }
        if (first && x->subst && t->kind == TK_PAREN && !t->npre && !t->npost && x_peek(p, x) && strcmp(x->pk_txt, ",")) {
            base = x->subst_call = rkb_listop_call(p->B, x->subst, x->subst_len, t); x->subst_paren = t->t; x->L->subst = 1; x->lastcls = 0;
        }
        if (first && x->star && t->kind == TK_STAR && !t->npost && x_peek(p, x) && (!strcmp(x->pk_txt, "-") || !strcmp(x->pk_txt, "+"))) {
            x_take(p, x); x_after(p, x); x->lastcls = 0; return ast_node_new(TT_NUL);
        }
    }
    if (x_in(p, x, LV_POW) >= 0) { int rv = x->rev, hy = x->hyper; x_take(p, x); tree_t *r = x_unary(p, x); if (p->build) base = hy ? rkb_hyper(p->B, LV_POW, 0, hy, base, r) : rv ? rkb_binop(p->B, LV_POW, 0, r, base) : rkb_binop(p->B, LV_POW, 0, base, r); x->lastcls = 0; }
    if (p->build) for (int i = np - 1; i >= (loose ? 1 : 0); i--) base = rkb_prefix_apply(p->B, t->pre[i], base);
    while (x_peek(p, x) && (x->pk_o.flags & OF_DOTTY) && !strcmp(x->pk_txt, ".")) {
        int nops0 = x->nops; const char *op10 = x->L->op1, *elop10 = x->el->op1;
        x_take(p, x);
        x->nops = nops0; x->L->op1 = op10; x->el->op1 = elop10;
        RkTerm dt;
        if (!x_term(p, x, &dt)) break;
        x->nterm--; x->L->nitem--;
        if (p->build && dt.kind == TK_DOTTY && dt.t && dt.t->t == TT_METHCALL && dt.t->n > 0) { dt.t->c[0] = base; base = dt.t; }
        x->lastcls = 0;
    }
    if (loose) {
        RkTerm lt; memset(&lt, 0, sizeof lt); lt.kind = TK_TREE; lt.t = base; lt.from = t->from; lt.to = lt.core_to = t->to;
        x->pend = &lt; x->lastcls = 0;
        tree_t *r = x_tern(p, x);
        x->lastcls = 0;
        return rkb_prefix_apply(p->B, loose, r);
    }
    return base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_rchain(RkP *p, RkX *x, int lv, tree_t *(*right)(RkP *, RkX *), tree_t *a, int k) {
    tree_t *r = right(p, x);
    int k2 = x_in(p, x, lv);
    if (k2 >= 0 && x->rev) { x_take(p, x); r = x_rchain(p, x, lv, right, r, k2); }
    return p->build ? rkb_binop(p->B, lv, k, r, a) : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_level(RkP *p, RkX *x, int lv, tree_t *(*lower)(RkP *, RkX *), tree_t *(*right)(RkP *, RkX *)) {
    tree_t *l = lower(p, x); int k;
    if (x->fail) return NULL;
    while ((k = x_in(p, x, lv)) >= 0) {
        int rv = x->rev, hy = x->hyper; x_take(p, x);
        if (rv) { l = x_rchain(p, x, lv, right, l, k); x->lastcls = 0; continue; }
        tree_t *r = right(p, x); if (p->build) l = hy ? rkb_hyper(p->B, lv, k, hy, l, r) : rkb_binop(p->B, lv, k, l, r); x->lastcls = 0;
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_band(RkP *p, RkX *x, int lo, int hi, int minp, tree_t *(*lower)(RkP *, RkX *)) {
    tree_t *l = lower(p, x);
    while (!x->fail && p->B && p->nuops && x_peek(p, x)) {
        int ui = rkb_user_op_find(p->B, 'i', x->pk_txt), q = ui >= 0 ? rkb_user_op_prec(p->B, ui) : -1;
        if (q <= lo || q >= hi || q < minp) break;
        x_take(p, x);
        tree_t *r = x_band(p, x, lo, hi, q + 1, lower);
        if (p->build && !x->fail) l = rkb_binop(p->B, LV_ADDSUB, RK_UK + ui, l, r);
    }
    return x->fail ? NULL : l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_ub_mul(RkP *p, RkX *x) { return x_band(p, x, PR('u'), PR('v'), 0, x_unary); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_mul(RkP *p, RkX *x) { return x_level(p, x, LV_MUL, x_ub_mul, x_ub_mul); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_ub_add(RkP *p, RkX *x) { return x_band(p, x, PR('t'), PR('u'), 0, x_mul); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_addsub(RkP *p, RkX *x) { return x_level(p, x, LV_ADDSUB, x_ub_add, x_ub_add); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_ub_repl(RkP *p, RkX *x) { return x_band(p, x, PR('s'), PR('t'), 0, x_addsub); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_repl(RkP *p, RkX *x) { return x_level(p, x, LV_REPL, x_ub_repl, x_ub_repl); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_ub_cat(RkP *p, RkX *x) { return x_band(p, x, PR('r'), PR('s'), 0, x_repl); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_cat(RkP *p, RkX *x) { return x_level(p, x, LV_CAT, x_ub_cat, x_ub_cat); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_range(RkP *p, RkX *x) {
    int top = !x->nterm && !x->eli;
    tree_t *l = x_cat(p, x); int k;
    if (x->fail) return NULL;
    if ((k = x_in(p, x, LV_RANGE1)) >= 0) {
        const char *op = x->pk_txt; x_take(p, x); tree_t *r = x_cat(p, x);
        if (top) { x->L->rg_lo = l; x->L->rg_hi = r; x->L->rg_op = op; x->L->rg_nitem = x->L->nitem; }
        if (p->build) l = rkb_binop(p->B, LV_RANGE1, k, l, r);
        x->lastcls = 0;
    }
    while ((k = x_in(p, x, LV_RANGE2)) >= 0) { x_take(p, x); tree_t *r = x_cat(p, x); if (p->build) l = rkb_binop(p->B, LV_RANGE2, k, l, r); x->lastcls = 0; }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_dor(RkP *p, RkX *x) { return x_level(p, x, LV_DOR, x_range, x_range); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_jct(RkP *p, RkX *x) { return x_level(p, x, LV_JCT, x_dor, x_range); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_divis(RkP *p, RkX *x) { return x_level(p, x, LV_DIVIS, x_jct, x_jct); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_smartmatch(RkP *p, RkX *x, tree_t *l) {
    RkTerm tm;
    if (!x_term(p, x, &tm)) return NULL;
    if (p->build) { tree_t *m = rkb_smartmatch_term(p->B, l, &tm); if (m) return m; }
    x->pend = &tm;
    tree_t *r = x_divis(p, x);
    return p->build ? rkb_smartmatch(p->B, l, r) : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_cmp(RkP *p, RkX *x) {
    tree_t *l = x_divis(p, x); int k;
    if (x->fail) return NULL;
    while ((k = x_in(p, x, LV_CMP)) >= 0) {
        int rv = x->rev, hy = x->hyper, ng = x->neg, sm = !strcmp(x->pk_txt, "~~");
        x_take(p, x); x->lastcls = 0;
        if (sm) { l = x_smartmatch(p, x, l); continue; }
        tree_t *r = x_divis(p, x);
        if (p->build) {
            l = hy ? rkb_hyper(p->B, LV_CMP, k, hy, l, r) : rv ? rkb_binop(p->B, LV_CMP, k, r, l) : rkb_binop(p->B, LV_CMP, k, l, r);
            if (ng) l = rkb_prefix_apply(p->B, "!", l);
        }
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_and(RkP *p, RkX *x) { return x_level(p, x, LV_AND, x_cmp, x_cmp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_or(RkP *p, RkX *x) { return x_level(p, x, LV_OR, x_and, x_and); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_tern(RkP *p, RkX *x) {
    tree_t *l = x_or(p, x);
    if (x->fail) return NULL;
    if (x_in(p, x, LV_TERN) >= 0) { tree_t *mid = x->pk_o.mid; x_take(p, x); tree_t *r = x_tern(p, x); if (p->build) l = rkb_ternary(p->B, l, mid, r); x->lastcls = 0; }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_expr(RkP *p, RkX *x) {
    int n0 = x->nterm;
    tree_t *l = x_tern(p, x);
    if (x->fail) return NULL;
    if (x->nterm == n0 + 1 && x->lastcls && x_peek(p, x)) {
        int cls = x->lastcls; const char *nm = x->lastname;
        int eq = !strcmp(x->pk_txt, "="), k = rkb_op_index(LV_COMPOUND, x->pk_txt), clv, ck;
        int gen = !eq && k < 0 && cls == 'S' && x->pk_o.prec == PR('i') && rkb_compound_base(x->pk_txt, &clv, &ck);
        int dot = !strcmp(x->pk_txt, ".=") && (cls == 'S' || cls == 'A' || cls == 'H');
        if ((eq && (cls == 'S' || cls == 'A')) || (k >= 0 && cls == 'S') || gen || dot) {
            const char *op = x->pk_txt;
            x_take(p, x); tree_t *r = x_after(p, x);
            if (!p->build) return NULL;
            if (dot) return rkb_dotassign(p->B, cls, nm, r);
            return gen ? rkb_assign_op(p->B, nm, op, r) : rkb_assign(p->B, cls, nm, eq ? -1 : k, r);
        }
    }
    if (x_in(p, x, LV_PAIR) >= 0) { x_take(p, x); tree_t *r = x_after(p, x); return p->build ? rkb_binop(p->B, LV_PAIR, 0, l, r) : NULL; }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x_listinfix(RkX *x) {
    const char *tx = x->pk_txt;
    if (!strcmp(tx, "X") || !strcmp(tx, "Z")) return 1;
    int lv, k, rv;
    return (tx[0] == 'X' || tx[0] == 'Z') && tx[1] && rkb_cross_inner(tx + 1, &lv, &k, &rv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x_loose_kind(const char *t) {
    return !strcmp(t, "and") ? 1 : !strcmp(t, "andthen") ? 2 : !strcmp(t, "notandthen") ? 3 : !strcmp(t, "or") ? 4 : !strcmp(t, "xor") ? 5 : !strcmp(t, "orelse") ? 6 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_loose_and(RkP *p, RkX *x, tree_t *l) {
    int k;
    while (x_peek(p, x) && (k = x_loose_kind(x->pk_txt)) >= 1 && k <= 3) {
        x_take(p, x);
        tree_t *r = x_after(p, x);
        if (x->fail) return l;
        if (p->build) l = rkb_loose(p->B, k, l, r);
        x->lastcls = 0;
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *x_loose(RkP *p, RkX *x, tree_t *l) {
    int k;
    l = x_loose_and(p, x, l);
    while (x_peek(p, x) && (k = x_loose_kind(x->pk_txt)) >= 4) {
        x_take(p, x);
        tree_t *r = x_after(p, x);
        if (x->fail) return l;
        r = x_loose_and(p, x, r);
        if (p->build) l = rkb_loose(p->B, k, l, r);
        x->lastcls = 0;
    }
    return l;
}
static tree_t *x_elem(RkP *p, RkX *x) {
    x->nterm = 0; x->el_nops = x->nops;
    tree_t *t = x_expr(p, x);
    if (x->fail) return NULL;
    t = x_loose(p, x, t);
    while (x_peek(p, x) && strcmp(x->pk_txt, ",") && !x_listinfix(x) && !x_seqinfix(x)) {
        int ui = p->build && p->B && p->nuops ? rkb_user_op_find(p->B, 'i', x->pk_txt) : -1;
        x_take(p, x);
        tree_t *r = x_after(p, x);
        if (ui >= 0 && !x->fail) t = rkb_binop(p->B, LV_ADDSUB, RK_UK + ui, t, r);
    }
    return t;
}
static int x_seqinfix(RkX *x) { const char *t = x->pk_txt; return !strcmp(t, "...") || !strcmp(t, "...^") || !strcmp(t, "\xe2\x80\xa6") || !strcmp(t, "\xe2\x80\xa6^"); }
void x_list(RkP *p, RkX *x) {
    for (x->eli = 0;; x->eli++) {
        x->el = rkb_list_el(x->L);
        tree_t *t = x_elem(p, x);
        if (x->fail) { x->L->n--; return; }
        x->el->t = t; x->el->nitem = x->nterm;
        if (x_peek(p, x) && x_seqinfix(x)) { if (!x->xstop) x_seq(p, x); break; }
        if (x_peek(p, x) && x_listinfix(x)) { if (!x->xstop) x_cross(p, x); break; }
        if (!x_peek(p, x) || strcmp(x->pk_txt, ",")) break;
        x_take(p, x);
    }
    rkb_list_end(x->L);
    if (x->L->subst && x->L->n != 1) { *x->subst_call = *x->subst_paren; x->L->subst = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x_cross(RkP *p, RkX *x) {
    const char *op = x->pk_txt; RkList *L = x->L; int eli = x->eli;
    RkList **rs = NULL; int nr = 0, cr = 0;
    while (!x->fail && x_peek(p, x) && !strcmp(x->pk_txt, op)) {
        x_take(p, x);
        RkList *R = rkb_list_new(); x->L = R; x->xstop = 1;
        x_list(p, x);
        x->xstop = 0;
        RK_GROW(rs, nr, cr, RkList *); rs[nr++] = R;
    }
    x->L = L; x->eli = eli;
    if (p->build && !x->fail && nr) rkb_cross(p->B, op, L, rs, nr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x_seq(RkP *p, RkX *x) {
    RkList *L = x->L; int eli = x->eli;
    while (!x->fail && x_peek(p, x) && x_seqinfix(x)) {
        const char *op = x->pk_txt;
        x_take(p, x);
        RkList *R = rkb_list_new(); x->L = R; x->xstop = 1;
        x_list(p, x);
        x->xstop = 0;
        if (p->build && !x->fail) rkb_sequence(p->B, op, L, R);
    }
    x->L = L; x->eli = eli;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void check_category(RkP *p, int from, int to) {
    int i = r_identifier(p, from);
    if (i < 0 || i >= to || ch(p, i) != ':' || ch(p, i + 1) == ':') return;
    int k = i + 1;
    if (at_lit(p, k, "sym") && !p->lang_e) k += 3;
    if (!(ch(p, k) == '<' || cp_at(p, k) == 0xAB)) return;
    static const char *const cats[] = { "infix", "prefix", "postfix", "circumfix", "postcircumfix", "term", "trait_mod", "METAOP_TEST_ASSIGN", "METAOP_TEST_ASSIGN_VALUE", 0 };
    for (int c = 0; cats[c]; c++) if ((int) strlen(cats[c]) == i - from && !memcmp(cats[c], p->s + from, (size_t) (i - from))) return;
    panic_at(p, from, "Cannot add tokens of category '%.*s'", i - from, p->s + from);
}
