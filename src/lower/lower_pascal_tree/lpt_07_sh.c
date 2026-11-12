#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static int pas_array_rows(const char *name, long long *rb);
static void pu_rename(tree_t *t, const tree_t *from, const tree_t *to, const tree_t *shadow);
tree_t *leaf_s(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)(s ? s : ""); return e; }
tree_t *flit(double v) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = v; return e; }
tree_t *un(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); ast_push(e, a); return e; }
static char *pas_neg_text(const char *s) { size_t n = strlen(s); char *r = ct_alloc(n + 2); if (s[0] == '-') { memcpy(r, s + 1, n); return r; } r[0] = '-'; memcpy(r + 1, s, n + 1); return r; }
static tree_t *mk_neg(tree_t *a) {
    if (a && a->t == TT_FLIT) { a->v.dval = -a->v.dval; if (a->n > 0 && IS(a->c[0], TT_QLIT)) a->c[0]->v.sval = pas_neg_text(a->c[0]->v.sval); return a; }
    if (a && pas_expr_is_real(a)) return bin(TT_MUL, a, flit(-1.0));
    return un(TT_MNS, a);
}
const char *map_io(const char *fn) {
    if (fn && !strcmp(fn, "writeln")) return "__pas_writeln";
    if (fn && !strcmp(fn, "write")) return "__pas_write";
    if (fn && !strcmp(fn, "sqr")) return "__pas_sqr";
    return fn;
}
int is_pas_io(const char *fn) { return fn && (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write")); }
tree_t *pas_trace_prepend_tap_off(tree_t *body) {
    if (!pas_trace_enabled()) return body;
    tree_t *off = ast_node_new(TT_FNC);
    ast_push(off, leaf_s(TT_VAR, "__trace_tap_off"));
    tree_t *nb = ast_node_new(TT_PROGRAM);
    ast_push(nb, off);
    if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(nb, body->c[i]); } else if (body) ast_push(nb, body);
    return nb;
}
tree_t *mk_deref(tree_t *ptr) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr); return e; }
tree_t *mk_fnc0(const char *fn) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); return e; }
tree_t *mk_fnc1(const char *fn, tree_t *a) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); return e; }
tree_t *mk_chrlit(tree_t *a) { tree_t *e = mk_fnc1("__pas_chrlit", a); pty_note_kind(e, PTK_CHAR); return e; }
tree_t *mk_fnc4(const char *fn, tree_t *a, tree_t *b, tree_t *c, tree_t *d) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, fn));
    ast_push(e, a);
    ast_push(e, b);
    ast_push(e, c);
    ast_push(e, d);
    return e;
}
tree_t *pas_ord_check(tree_t *v, long long lo, long long hi, const char *what) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_ord_check"));
    ast_push(e, v);
    ast_push(e, ilit(lo));
    ast_push(e, ilit(hi));
    ast_push(e, leaf_s(TT_QLIT, what));
    return e;
}
int pas_is_unsigned_inttypename(const char *n) {
    if (!n) return 0;
    static const char *const U[] = {"byte","word","cardinal","longword","qword","uint8","uint16","uint32","uint64","nativeuint","pointer","ptruint","codepointer"};
    for (size_t i = 0; i < sizeof(U) / sizeof(U[0]); i++) if (!strcmp(n, U[i])) return 1;
    return 0;
}
int pas_expr_is_real(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_FLIT || e->t == TT_DIV) return 1;
    if (e->t == TT_VAR && e->v.sval) { const pty_t *vt; return pas_var_is_real(e->v.sval) || ((vt = pty_name_type(e->v.sval)) && vt->k == PTK_REAL); }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
        static const char *const rf[] = { "__pas_sin", "__pas_cos", "__pas_exp", "__pas_sqrt", "__pas_ln", "__pas_arctan", 0 };
        for (int i = 0; rf[i]; i++) if (!strcmp(e->c[0]->v.sval, rf[i])) return 1;
    }
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
        const char *fn = e->c[0]->v.sval;
        if (!strcmp(fn, "__pas_xr_ev") || !strcmp(fn, "__pas_xr_un") || !strcmp(fn, "__pas_xr_bin")) return 1;
        if (!strcmp(fn, "__pas_xr_cvt") && e->c[1] && e->c[1]->t == TT_ILIT) { long long kd = e->c[1]->v.ival; return kd <= 5 || kd == 10; }
    }
    if ((e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL) && e->n == 2) return pas_expr_is_real(e->c[0]) || pas_expr_is_real(e->c[1]);
    if ((e->t == TT_MNS || e->t == TT_PLS) && e->n == 1) return pas_expr_is_real(e->c[0]);
    if (e->t == TT_IDX && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT) {
        const char *rt = pas_with_sel_rtype(e->c[0]), *tn = rt ? pas_rectype_field_typename(rt, e->c[1]->v.ival) : NULL;
        return tn && pas_is_realtypename(tn);
    }
    return 0;
}
tree_t *pas_split_writes(tree_t *e) {
    int i0 = (e->n >= 3 && e->c[2] && e->c[2]->t == TT_ILIT && e->c[2]->v.ival == -9) ? 3 : 1;
    int gs[e->n + 1], gcall[e->n + 1], ng = 0, needs = 0;
    for (int i = i0; i + 1 < e->n;) {
        int g = (e->c[i + 1] && e->c[i + 1]->t == TT_ILIT && (e->c[i + 1]->v.ival == -10 || e->c[i + 1]->v.ival == -11 || e->c[i + 1]->v.ival == -3)) ? 4 : 2;
        if (i + g > e->n) g = e->n - i;
        gs[ng] = i;
        gcall[ng] = 0;
        for (int k = i; k < i + g; k++) if (pas_has_user_call(e->c[k])) gcall[ng] = 1;
        if (gcall[ng] && ng > 0) needs = 1;
        ng++;
        i += g;
    }
    if (!needs) return e;
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    int start = 0;
    for (int j = 0; j <= ng; j++) {
        if (j < ng && !(gcall[j] && j > start)) continue;
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, j == ng ? e->c[0]->v.sval : "__pas_write"));
        if (i0 == 3) { ast_push(c, pas_tree_clone(e->c[1])); ast_push(c, ilit(-9)); }
        for (int k = gs[start]; k < (j == ng ? e->n : gs[j]); k++) ast_push(c, e->c[k]);
        ast_push(seq, c);
        start = j;
    }
    return seq;
}
tree_t *pas_crt_route(tree_t *t) {
    if (!IS(t, TT_FNC) || t->n < 1 || !t->c[0] || !is_pas_io(t->c[0]->v.sval)) return t;
    int nl = !strcmp(t->c[0]->v.sval, "__pas_writeln");
    t->c[0] = leaf_s(TT_VAR, "__pas_writestr");
    if (nl) { ast_push(t, leaf_s(TT_QLIT, "\n")); ast_push(t, ilit(-1)); }
    return mk_fnc1("crt.crtwrite", t);
}
static tree_t *pas_mod(tree_t *a, tree_t *b) {
    if (g_lower_pas.sem.pas_dialect_fpc) return bin(TT_MOD, a, b);
    tree_t *b1 = b;
    if (!b || b->t != TT_ILIT || b->v.ival <= 0) { b1 = pas_ord_check(b, 1, 9223372036854775807LL, "i mod j with j zero or negative"); ast_push(b1, leaf_s(TT_QLIT, "6.7.2.2")); }
    return bin(TT_MOD, bin(TT_ADD, bin(TT_MOD, a, b1), pas_tree_clone(b)), pas_tree_clone(b));
}
static tree_t *mk_in(tree_t *elem, tree_t *set) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_in")); ast_push(e, elem); ast_push(e, set); return e; }
static int pas_array_rows(const char *name, long long *rb) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    if (!t || t->k != PTK_ARRAY || t->dyn || t->open || t->nix != 2 || !t->ix || !t->ix[0] || !t->ix[0]->bounded) return 0;
    rb[0] = t->ix[0]->lo;
    rb[1] = t->ix[0]->hi;
    return 1;
}
static int pas_fixed_arr_bounds(const char *name, long long *lo, long long *hi) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    if (!t || t->k != PTK_ARRAY || t->dyn || t->open || t->nix != 1 || !t->ix || !t->ix[0] || !t->ix[0]->bounded) return 0;
    *lo = t->ix[0]->lo;
    *hi = t->ix[0]->hi;
    return 1;
}
int pas_tfcomp_nonchar(const char *n) {
    if (!n) return 0;
    for (int i = g_lower_pas.sem.pas_ntfcomp - 1; i >= 0; i--) if (g_lower_pas.sem.pas_tfcomp[i].name && !strcmp(g_lower_pas.sem.pas_tfcomp[i].name, n)) return !g_lower_pas.sem.pas_tfcomp[i].ischar;
    return 0;
}
tree_t *mk_chr_wrap(tree_t *e) { tree_t *r = ast_node_new(TT_FNC); ast_push(r, leaf_s(TT_VAR, "__pas_chr")); ast_push(r, e); return r; }
static int pas_buf_is_char(tree_t *e) { return !(!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") && e->n >= 2 && e->c[1] && e->c[1]->t == TT_VAR && pas_tfcomp_nonchar(e->c[1]->v.sval)); }
tree_t *pas_pf_callthrough(const char *name, PNodeList *args) {
    const PasDef *d = pas_pf_lookup(name);
    int na = args ? args->count / 2 : 0, nf = pas_pf_nactuals(d->sig);
    if (na != nf) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with the wrong number of actual-parameters for its heading %s\n", d->sig[0] == 'F' ? "6.7.3" : "6.8.2.3", name, d->sig);
        g_lower_pas.sem.pas_iso_errors++;
    }
    tree_t *e = mk_fnc1("__pas_pcall", leaf_s(TT_VAR, name));
    ast_push(e, leaf_s(TT_QLIT, d->sig));
    ast_push(e, ilit(0));
    for (int i = 0; args && i + 1 < args->count; i += 2) ast_push(e, args->items[i]);
    return e;
}
const char *pas_pf_field_sig(const tree_t *f) {
    const pty_t *r = (f && f->t == TT_FIELD && f->n == 2 && f->c[1] && f->c[1]->v.sval) ? pty_of_src(f->c[0]) : NULL;
    if (r && r->k == PTK_POINTER) r = pty_pointee(r);
    for (int i = 0; r && r->k == PTK_RECORD && i < pty_rec_nf(r); i++) {
        struct pty_fld o;
        const char *tn;
        if (pty_rec_fld(r, i, &o) && o.name && !strcasecmp(o.name, f->c[1]->v.sval)) return (tn = pty_fld_typename(r, i)) ? pas_proctype_get(tn) :
            (pty_name_of(r) ? pas_proctype_get(pas_pf_cat3(pty_name_of(r), ".", o.name)) : NULL);
    }
    return NULL;
}
tree_t *pas_pf_field_assign(const tree_t *n) {
    const char *sg = pas_pf_field_sig(n->c[0]), *fn = n->c[0]->c[1]->v.sval;
    tree_t *rhs = ilit(0);
    if (!(IS(n->c[1], TT_VAR) && n->c[1]->v.sval && !strcasecmp(n->c[1]->v.sval, "nil"))) {
        PNodeList *c = pnl_new();
        tree_t *rv = E_expression(n->c[1]).node;
        int fr = rv && rv->t == TT_VAR && rv->v.sval && pas_pf_is_formal(rv->v.sval);
        pas_pf_closure(c, rv, sg, fn, 1);
        rhs = c->items[0];
        for (int k = 1; k <= 3 && !fr; k++) if (c->items[2 * k]->t != TT_ILIT) {
            fprintf(stderr, "pascal: the procedural field '%s' holds a routine id alone: a nested routine or a method cannot be assigned to it\n", fn);
            g_lower_pas.sem.pas_iso_errors++;
            break;
        }
    }
    return mk_assign(E_selector(n->c[0]).node, rhs);
}
tree_t *pas_tm_call(const char *name) {
    char b[40];
    snprintf(b, sizeof b, "__pas_vptmp_tm%d", g_lower_pas.sem.pas_scope.npf++);
    const char *pp = ct_strdup(b);
    tree_t *code = mk_fnc1("arr_get", leaf_s(TT_VAR, name));
    ast_push(code, ilit(0));
    tree_t *data = mk_fnc1("arr_get", leaf_s(TT_VAR, name));
    ast_push(data, ilit(1));
    tree_t *sq = ast_node_new(TT_SEQ_EXPR);
    ast_push(sq, mk_assign(leaf_s(TT_VAR, pp), code));
    ast_push(sq, mk_assign(leaf_s(TT_VAR, pas_pf_env_name(pp, 1)), data));
    tree_t *pc = mk_fnc1("__pas_pcall", leaf_s(TT_VAR, pp));
    ast_push(pc, leaf_s(TT_QLIT, "P()"));
    ast_push(pc, ilit(1));
    ast_push(sq, pc);
    return sq;
}
tree_t *pas_format_call(const tree_t *n) {
    const tree_t *st = n->c[2];
    if (IS(n->c[1], TT_FMT)) return NULL;
    for (int i = 0; i < st->n; i++) if (IS(st->c[i], TT_SUBRANGE) || IS(st->c[i], TT_FMT)) return NULL;
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_format"));
    tree_t *f = E_expression(n->c[1]).node;
    ast_push(e, pas_is_charexpr(f) ? mk_chr_wrap(f) : f);
    for (int i = 0; i < st->n; i++) { tree_t *x = E_expression(st->c[i]).node; ast_push(e, pas_is_charexpr(x) ? mk_chr_wrap(x) : x); }
    return e;
}
pval E_expression_raw(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    { pval _xo; memset(&_xo, 0, sizeof _xo); if (pas_xr_elab(n, &_xo)) return _xo; }
    if ((IS(n, TT_EQ) || IS(n, TT_NE)) && n->n == 2) {
        int _ln = IS(n->c[1], TT_VAR) && n->c[1]->v.sval && !strcasecmp(n->c[1]->v.sval, "nil"), _rn = IS(n->c[0], TT_VAR) && n->c[0]->v.sval && !strcasecmp(n->c[0]->v.sval, "nil");
        const tree_t *_x = _ln ? n->c[0] : _rn ? n->c[1] : NULL;
        const pty_t *_xt = _x ? pty_of_src(_x) : NULL;
        if (_xt && _xt->k == PTK_ARRAY && _xt->dyn) return E_expression_raw(bin(IS(n, TT_EQ) ? TT_EQ : TT_NE, pas_src_call("length", (tree_t *)_x, NULL), ilit(0)));
    }
    if (IS(n, TT_IN)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = mk_in(v[1].node, v[3].node); } return out; }
    if (IS(n, TT_LT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_ord(TT_LT, v[1].node, v[3].node); if (!out.node) out.node = pas_rel(TT_LT, v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_LE)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_ord(TT_LE, v[1].node, v[3].node); if (!out.node) out.node = pas_rel_or_set(TT_LE, "__pas_subset", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_GT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_ord(TT_GT, v[1].node, v[3].node); if (!out.node) out.node = pas_rel(TT_GT, v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_GE)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_ord(TT_GE, v[1].node, v[3].node); if (!out.node) out.node = pas_rel_or_set(TT_GE, "__pas_super", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_NE)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_rel(TT_NE, v[1].node, v[3].node); if (!out.node) out.node = pas_rel_or_set(TT_NE, "__pas_setne", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_EQ)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_rel(TT_EQ, v[1].node, v[3].node); if (!out.node) out.node = pas_rel_or_set(TT_EQ, "__pas_seteq", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_PLS)) { v[2] = E_expression(n->c[0]); { pas_sign_operand(v[2].node, '+'); out.node = v[2].node; } return out; }
    if (IS(n, TT_MNS)) { v[2] = E_expression(n->c[0]); { pas_sign_operand(v[2].node, '-'); out.node = mk_neg(v[2].node); } return out; }
    if (IS(n, TT_ADD)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_arith(TT_ADD, v[1].node, v[3].node); if (!out.node) out.node = pas_arith_or_set(TT_ADD, "__pas_setuni", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_SUB)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_ptr_arith(TT_SUB, v[1].node, v[3].node); if (!out.node) out.node = pas_arith_or_set(TT_SUB, "__pas_setdif", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_ALT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = (pas_is_boolexpr(v[1].node) && pas_is_boolexpr(v[3].node)) ? bin(TT_ALT, v[1].node, v[3].node) : mk_fnc2("ior", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_MUL)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_arith_or_set(TT_MUL, "__pas_setint", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_DIV)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rdiv(v[1].node, v[3].node); } return out; }
    if (IS(n, TT_IDIV)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        v[3].node = pas_divchk_wrap(v[3].node);
        { out.node = pas_is_qword(v[1].node) ? mk_fnc2("__pas_udiv", v[1].node, v[3].node) : bin(TT_DIV, v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_MOD)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        v[3].node = pas_divchk_wrap(v[3].node);
        { out.node = pas_is_qword(v[1].node) ? mk_fnc2("__pas_umod", v[1].node, v[3].node) : pas_mod(v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_POW) && n->n == 2) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = mk_fnc2("__pas_pow", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_CONJ)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = (pas_is_boolexpr(v[1].node) && pas_is_boolexpr(v[3].node)) ? bin(TT_CONJ, v[1].node, v[3].node) : mk_fnc2("iand", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_FIELD) && n->n == 2 && pas_pf_field_sig(n) && pas_pf_field_sig(n)[0] == 'F' && pas_pf_nactuals(pas_pf_field_sig(n)) == 0) {
        out.node = mk_fnc1("__pas_pcall", E_selector(n).node);
        ast_push(out.node, leaf_s(TT_QLIT, pas_pf_field_sig(n)));
        ast_push(out.node, ilit(0));
        return out;
    }
    if (IS(n, TT_VAR) || IS(n, TT_IDX) || IS(n, TT_FIELD) || IS(n, TT_DEREF)) {
        v[1] = E_selector(n);
        {
            if (pas_is_cafield(v[1].node)) {
                tree_t *u = ast_node_new(TT_FNC);
                ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack"));
                ast_push(u, v[1].node);
                ast_push(u, ilit(pas_cafield_lo_get(v[1].node)));
                out.node = u;
            } else out.node = pas_vt_wrap_read(v[1].node);
        }
        return out;
    }
    if (IS(n, TT_FNC)) { v[1] = E_call_with_args(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_ILIT)) { v[1].ival = tok_i(n); { out.node = ilit(v[1].ival); } return out; }
    if (IS(n, TT_FLIT)) { v[1].dval = tok_d(n); out.node = flit(v[1].dval); if (n->n > 0 && IS(n->c[0], TT_QLIT)) ast_push(out.node, leaf_s(TT_QLIT, n->c[0]->v.sval)); return out; }
    if (IS(n, TT_QLIT)) {
        v[1].str = tok_s(n);
        {
            if (v[1].str && strlen(v[1].str) == 1) {
                tree_t *_cl = ast_node_new(TT_FNC);
                ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit"));
                ast_push(_cl, ilit((long long)(unsigned char)v[1].str[0]));
                out.node = _cl;
            } else out.node = leaf_s(TT_QLIT, v[1].str);
        }
        return out;
    }
    if (IS(n, TT_CHRLIT)) { v[1].ival = tok_i(n); { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit(v[1].ival)); out.node = _cl; } return out; }
    if (IS(n, TT_NOT)) { v[2] = E_expression(n->c[0]); { out.node = pas_is_signed_intexpr(v[2].node) ? bin(TT_SUB, ilit(-1), v[2].node) : pas_flip_rel(lpt_pas_cond(v[2].node)); } return out; }
    if (IS(n, TT_ADDR)) {
        int _ai = g_lower_pas.sem.pas_addr_int;
        g_lower_pas.sem.pas_addr_int = 0;
        v[2] = E_expression(n->c[0]);
        { out.node = _ai ? pas_addr_of_proc(v[2].node) : pas_addr_of(v[2].node); }
        return out;
    }
    if (IS(n, TT_SET) && n->n == 0) { { out.node = mk_set_ctor(NULL); } return out; }
    if (IS(n, TT_SET)) { v[2] = E_set_member_list(n, 0, n->n); { out.node = v[2].node; } return out; }
    elab_bad("expression", n);
    return out;
}
pval E_for_statement(const tree_t *n) {
    pval v[11];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FOR) && !strcmp(n->v.sval, "in") && n->n == 3) {
        const char *x = tok_s(n->c[0]);
        v[4] = E_expression(n->c[1]);
        v[8] = E_body_stmt(n->c[2]);
        pas_scope_require(x);
        tree_t *coll = v[4].node, *pre = NULL, *lo = NULL, *hi = NULL, *elem = NULL;
        char *iv = (char *)ct_zalloc(32, 1);
        snprintf(iv, 32, "__pas_catmp_%d", ++g_lower_pas.sem.pas_catmp_seq);
        pas_local_add(iv);
        long long ah;
        if (pas_is_setexpr(coll)) {
            lo = ilit(0);
            hi = ilit(255);
            elem = pas_is_charvar(x) ? mk_chrlit(leaf_s(TT_VAR, iv)) : leaf_s(TT_VAR, iv);
            tree_t *seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, mk_assign(leaf_s(TT_VAR, x), elem));
            ast_push(seq, v[8].node);
            tree_t *test = ast_node_new(TT_IF);
            ast_push(test, mk_in(pas_is_charvar(x) ? mk_chrlit(leaf_s(TT_VAR, iv)) : leaf_s(TT_VAR, iv), coll));
            ast_push(test, seq);
            v[8].node = test;
            elem = NULL;
        } else {
            tree_t *base = coll;
            if (!(coll->t == TT_VAR && coll->v.sval)) {
                char *cv = (char *)ct_zalloc(32, 1);
                snprintf(cv, 32, "__pas_catmp_%d", ++g_lower_pas.sem.pas_catmp_seq);
                pas_local_add(cv);
                pre = bin(TT_ASSIGN, leaf_s(TT_VAR, cv), coll);
                base = leaf_s(TT_VAR, cv);
            }
            long long _rb[4], _fl = 0, _fh = 0;
            if (coll->t == TT_VAR && coll->v.sval && pas_array_ncols(coll->v.sval) > 0 && pas_array_rows(coll->v.sval, _rb)) {
                lo = ilit(_rb[0]);
                hi = ilit(_rb[1]);
            } else if (coll->t == TT_VAR && coll->v.sval && pas_array_high_get(coll->v.sval, &ah)) {
                lo = ilit(pas_array_nonparam_low(coll->v.sval));
                hi = ilit(ah);
            } else if (coll->t == TT_VAR && coll->v.sval && pas_fixed_arr_bounds(coll->v.sval, &_fl, &_fh)) {
                lo = ilit(_fl);
                hi = ilit(_fh);
            } else if (pas_is_dynexpr(coll) || (pty_of_low(coll) && pty_of_low(coll)->k == PTK_ARRAY && pty_of_low(coll)->dyn)) {
                lo = ilit(0);
                hi = bin(TT_SUB, mk_fnc1("__pas_dynlen", pas_tree_clone(base)), ilit(1));
            } else {
                lo = ilit(1);
                hi = mk_fnc1("length", pas_tree_clone(base));
            }
            tree_t *ix = ast_node_new(TT_IDX);
            ast_push(ix, pas_tree_clone(base));
            ast_push(ix, leaf_s(TT_VAR, iv));
            tree_t *seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, mk_assign(leaf_s(TT_VAR, x), ix));
            ast_push(seq, v[8].node);
            v[8].node = seq;
        }
        tree_t *e = ast_node_new(TT_FOR);
        ast_push(e, leaf_s(TT_VAR, iv));
        ast_push(e, lo);
        ast_push(e, hi);
        ast_push(e, v[8].node);
        if (pre) { tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, pre); ast_push(q, e); out.node = q; } else out.node = e;
        return out;
    }
    if (IS(n, TT_FOR) && !strcmp(n->v.sval, "to")) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_expression(n->c[1]);
        v[6] = E_expression(n->c[2]);
        v[8] = E_body_stmt(n->c[3]);
        {
            pas_scope_require(v[2].str);
            pas_value_compat(v[2].str, v[4].node, "6.8.3.9", "the control-variable");
            pas_value_compat(v[2].str, v[6].node, "6.8.3.9", "the control-variable");
            if (pas_var_is_real(v[2].str)) {
                fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", v[2].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            pas_for_const_bounds(v[2].str, v[4].node, v[6].node, 0);
            tree_t *e = ast_node_new(TT_FOR);
            ast_push(e, leaf_s(TT_VAR, v[2].str));
            ast_push(e, v[4].node);
            ast_push(e, v[6].node);
            ast_push(e, pas_trace_wrap_for_body(v[2].str, v[8].node));
            if (!g_lower_pas.sem.pas_seen_mode_directive) e->v.ival |= 2;
            out.node = pas_for_once(e);
        }
        return out;
    }
    if (IS(n, TT_FOR)) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_expression(n->c[1]);
        v[6] = E_expression(n->c[2]);
        v[8] = E_body_stmt(n->c[3]);
        {
            pas_scope_require(v[2].str);
            pas_value_compat(v[2].str, v[4].node, "6.8.3.9", "the control-variable");
            pas_value_compat(v[2].str, v[6].node, "6.8.3.9", "the control-variable");
            if (pas_var_is_real(v[2].str)) {
                fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", v[2].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            pas_for_const_bounds(v[2].str, v[4].node, v[6].node, 1);
            tree_t *e = ast_node_new(TT_FOR);
            ast_push(e, leaf_s(TT_VAR, v[2].str));
            ast_push(e, v[4].node);
            ast_push(e, v[6].node);
            ast_push(e, pas_trace_wrap_for_body(v[2].str, v[8].node));
            e->v.ival = 1;
            if (!g_lower_pas.sem.pas_seen_mode_directive) e->v.ival |= 2;
            out.node = pas_for_once(e);
        }
        return out;
    }
    elab_bad("for_statement", n);
    return out;
}
static int pu_is_part(const tree_t *t, const char *w) { return IS(t, TT_PART) && t->v.sval && !strcmp(t->v.sval, w); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pu_has(const tree_t *names, const char *s) { for (int i = 0; names && s && i < names->n; i++) if (names->c[i]->v.sval && !strcasecmp(names->c[i]->v.sval, s)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_add(tree_t *names, const tree_t *v) { if (IS(v, TT_VAR) && v->v.sval && !pu_has(names, v->v.sval)) ast_push(names, leaf_s(TT_VAR, v->v.sval)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_decl_names(const tree_t *d, tree_t *names) {
    if (IS(d, TT_PROCEDURE) || IS(d, TT_FUNCTION)) { if (d->n > 0) pu_add(names, d->c[0]); return; }
    if (!IS(d, TT_DECL) || d->n < 1) return;
    if (IS(d->c[0], TT_VLIST)) for (int i = 0; i < d->c[0]->n; i++) pu_add(names, d->c[0]->c[i]);
    else pu_add(names, d->c[0]);
    if (d->n > 1 && IS(d->c[1], TT_ENUM_TYPE)) for (int i = 0; i < d->c[1]->n; i++) pu_add(names, d->c[1]->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_part_names(const tree_t *part, tree_t *names) {
    for (int i = 0; part && i < part->n; i++) {
        const tree_t *p = part->c[i];
        if (IS(p, TT_PART) && !pu_is_part(p, "uses")) for (int j = 0; j < p->n; j++) pu_decl_names(p->c[j], names);
        else pu_decl_names(p, names);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_rename_routine(tree_t *r, const tree_t *from, const tree_t *to, const tree_t *shadow) {
    tree_t *inner = ast_node_new(TT_VLIST);
    for (int i = 0; shadow && i < shadow->n; i++) ast_push(inner, shadow->c[i]);
    if (r->n > 0) pu_rename(r->c[0], from, to, shadow);
    for (int i = 1; i < r->n; i++) {
        tree_t *c = r->c[i];
        if (IS(c, TT_PARAMS)) { for (int j = 0; j < c->n; j++) { pu_decl_names(c->c[j], inner); if (IS(c->c[j], TT_DECL) && c->c[j]->n > 1) pu_rename(c->c[j]->c[1], from, to, shadow); } }
        if (IS(c, TT_BLOCK)) pu_part_names(c, inner);
    }
    for (int i = 1; i < r->n; i++) if (!IS(r->c[i], TT_PARAMS)) pu_rename(r->c[i], from, to, inner);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_rename(tree_t *t, const tree_t *from, const tree_t *to, const tree_t *shadow) {
    if (!t) return;
    if (IS(t, TT_VAR)) {
        if (!t->v.sval || pu_has(shadow, t->v.sval)) return;
        for (int i = 0; i < from->n; i++) if (!strcasecmp(from->c[i]->v.sval, t->v.sval)) { t->v.sval = to->c[i]->v.sval; return; }
        return;
    }
    if (IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION)) { pu_rename_routine(t, from, to, shadow); return; }
    if (IS(t, TT_FIELD)) { if (t->n > 0) pu_rename(t->c[0], from, to, shadow); return; }
    if (IS(t, TT_DECL) && t->v.sval && !strcmp(t->v.sval, "field")) { for (int i = 1; i < t->n; i++) pu_rename(t->c[i], from, to, shadow); return; }
    for (int i = 0; i < t->n; i++) pu_rename(t->c[i], from, to, shadow);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pu_qualify(tree_t *t, const tree_t *units, const tree_t *qfrom, const tree_t *qto) {
    if (!t) return t;
    if (IS(t, TT_FIELD) && t->n == 2 && IS(t->c[0], TT_VAR) && IS(t->c[1], TT_VAR) && t->c[0]->v.sval && t->c[1]->v.sval && pu_has(units, t->c[0]->v.sval)) {
        size_t l = strlen(t->c[0]->v.sval) + strlen(t->c[1]->v.sval) + 2;
        char *q = ct_alloc(l);
        snprintf(q, l, "%s.%s", t->c[0]->v.sval, t->c[1]->v.sval);
        tree_t *lf = leaf_s(TT_VAR, t->c[1]->v.sval);
        for (int i = 0; i < qfrom->n; i++) if (!strcasecmp(qfrom->c[i]->v.sval, q)) { lf->v.sval = qto->c[i]->v.sval; break; }
        lf->slen = PAS_UNIT_QUAL_MARK;
        return lf;
    }
    for (int i = 0; i < t->n; i++) t->c[i] = pu_qualify(t->c[i], units, qfrom, qto);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_merge_part(tree_t **slot, const char *kind, const tree_t *from) {
    for (int i = 0; from && i < from->n; i++) {
        const tree_t *p = from->c[i];
        if (!pu_is_part(p, kind)) continue;
        if (!*slot) *slot = leaf_s(TT_PART, kind);
        for (int j = 0; j < p->n; j++) ast_push(*slot, p->c[j]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_stmts(tree_t *seq, const tree_t *part) {
    for (int i = 0; part && i < part->n; i++) { tree_t *s = part->c[i]; if (IS(s, TT_SEQ_EXPR)) for (int j = 0; j < s->n; j++) ast_push(seq, s->c[j]); else if (s) ast_push(seq, s); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *pu_child_part(const tree_t *u, const char *w) { for (int i = 1; u && i < u->n; i++) if (pu_is_part(u->c[i], w)) return u->c[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pu_fnc0(const char *fn) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long long pu_part_rank(const tree_t *p) {
    static const char *nm[] = { "label", "const", "type", "var" };
    if (IS(p, TT_PROCEDURE) || IS(p, TT_FUNCTION)) return 5;
    for (int i = 0; i < 4; i++) if (pu_is_part(p, nm[i])) return i + 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pu_qname(const char *un, const char *nm) { size_t l = strlen(un) + strlen(nm) + 2; char *q = ct_alloc(l); snprintf(q, l, "%s.%s", un, nm); return q; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_map(tree_t *from, tree_t *to, const char *nm, const char *q) { if (pu_has(from, nm)) return; ast_push(from, leaf_s(TT_VAR, nm)); ast_push(to, leaf_s(TT_VAR, q)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *pu_uses(const tree_t *part) { for (int i = 0; part && i < part->n; i++) if (pu_is_part(part->c[i], "uses") && part->c[i]->n > 0) return part->c[i]->c[0]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_map_uses(tree_t *from, tree_t *to, const tree_t *conf, const tree_t *own, const tree_t *forest, const tree_t *pubs, const tree_t *u1, const tree_t *u2) {
    for (int i = 0; i < conf->n; i++) {
        const char *nm = conf->c[i]->v.sval, *q = NULL;
        if (pu_has(own, nm)) continue;
        for (int pass = 0; pass < 2; pass++) {
            const tree_t *ul = pass ? u2 : u1;
            for (int j = 0; ul && j < ul->n; j++) for (int k = 0; k < forest->n; k++) {
                const char *un = forest->c[k]->c[0]->v.sval;
                if (ul->c[j]->v.sval && !strcasecmp(un, ul->c[j]->v.sval) && pu_has(pubs->c[k], nm)) q = pu_qname(un, nm);
            }
        }
        if (q) pu_map(from, to, nm, q);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pu_apply_pragmas(tree_t *t) {
    if (!t) return;
    if (t->t == TT_ATTR && t->v.sval && !strcmp(t->v.sval, ":pragma") && t->slen == 0 && t->n > 0 && t->c[0] && t->c[0]->v.sval) { t->slen = 1; pas_apply_event(t->c[0]->v.sval); }
    for (int i = 0; i < t->n; i++) pu_apply_pragmas(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pu_routine_name(const tree_t *r) { return (r && r->n > 0 && IS(r->c[0], TT_VAR)) ? r->c[0]->v.sval : NULL; }
static int pu_listed(const tree_t *list, const tree_t *r) { for (int i = 0; list && i < list->n; i++) if (list->c[i] == r) return 1; return 0; }
static void pu_idents(const tree_t *t, tree_t *names) {
    if (!t) return;
    if (IS(t, TT_VAR) && t->v.sval && !pu_has(names, t->v.sval)) ast_push(names, leaf_s(TT_VAR, t->v.sval));
    for (int i = 0; i < t->n; i++) pu_idents(t->c[i], names);
}
static int pu_strhlp_ref(const tree_t *refs, const char *nm) {
    if (!nm || strncmp(nm, "__strhlp_", 9)) return 0;
    for (int i = 0; i < refs->n; i++) { const char *r = refs->c[i]->v.sval; size_t l = r ? strlen(r) : 0; if (l && !strncasecmp(nm + 9, r, l) && (!nm[9 + l] || nm[9 + l] == '_')) return 1; }
    return 0;
}
static void pu_prune_routines(tree_t *nb, tree_t *ur) {
    tree_t *cand = ast_node_new(TT_VLIST), *refs = ast_node_new(TT_VLIST), *keep = ast_node_new(TT_VLIST);
    for (int i = 0; i < ur->n; i++) { const char *nm = pu_routine_name(ur->c[i]); if (nm && !strstr(nm, "__pc_")) ast_push(cand, ur->c[i]); }
    for (int i = 0; i < nb->n; i++) if (!pu_listed(cand, nb->c[i])) pu_idents(nb->c[i], refs);
    if (g_lower_pas.sem.pas_ioflow) { pu_add(refs, leaf_s(TT_VAR, "__sh_iocheck")); pu_add(refs, leaf_s(TT_VAR, "__sh_iochecke")); }
    if (g_lower_pas.sem.pas_divchk) { pu_add(refs, leaf_s(TT_VAR, "__sh_divchk")); pu_add(refs, leaf_s(TT_VAR, "__sh_rdiv")); }
    if (g_lower_pas.sem.pas_crt_out) pu_add(refs, leaf_s(TT_VAR, "crt.crtwrite"));
    if (pu_has(refs, "min") || pu_has(refs, "max")) { pu_add(refs, leaf_s(TT_VAR, "fpcmindouble")); pu_add(refs, leaf_s(TT_VAR, "fpcmaxdouble")); }
    for (int grew = 1; grew;) {
        grew = 0;
        for (int i = 0; i < cand->n; i++) {
            const char *nm = pu_routine_name(cand->c[i]);
            if (pu_listed(keep, cand->c[i]) || !(pu_has(refs, nm) || pu_strhlp_ref(refs, nm))) continue;
            ast_push(keep, cand->c[i]);
            for (int j = 1; j < cand->c[i]->n; j++) pu_idents(cand->c[i]->c[j], refs);
            grew = 1;
        }
    }
    int w = 0;
    for (int i = 0; i < nb->n; i++) if (!pu_listed(cand, nb->c[i]) || pu_listed(keep, nb->c[i])) nb->c[w++] = nb->c[i];
    nb->n = w;
}
tree_t *pas_flatten_units(tree_t *prog) {
    if (!IS(prog, TT_PROGRAM)) return prog;
    tree_t *forest = NULL;
    int bi = -1;
    for (int i = 0; i < prog->n; i++) { if (pu_is_part(prog->c[i], "units")) forest = prog->c[i]; else if (IS(prog->c[i], TT_BLOCK)) bi = i; }
    if (bi < 0 || !forest) return prog;
    tree_t *block = prog->c[bi];
    for (int i = 0; i < block->n; i++) if (pu_is_part(block->c[i], "uses")) pu_apply_pragmas(block->c[i]);
    long long ord = 0;
    for (int i = 0; i < block->n - 1; i++) if (pu_part_rank(block->c[i])) ord = pas_decl_part_order(ord, pu_part_rank(block->c[i]));
    int nu = forest->n;
    tree_t *unames = ast_node_new(TT_VLIST), *qfrom = ast_node_new(TT_VLIST), *qto = ast_node_new(TT_VLIST), *pnames = ast_node_new(TT_VLIST);
    tree_t *pubs = ast_node_new(TT_VLIST), *seen = ast_node_new(TT_VLIST), *conf = ast_node_new(TT_VLIST);
    for (int k = 0; k < nu; k++) pu_add(unames, forest->c[k]->c[0]);
    for (int k = 0; k < nu; k++) if (forest->c[k]->c[0]->v.sval && !strcasecmp(forest->c[k]->c[0]->v.sval, "crt")) g_lower_pas.sem.pas_crt_out = 1;
    pu_part_names(block, pnames);
    for (int k = 0; k < nu; k++) {
        tree_t *pub = ast_node_new(TT_VLIST);
        pu_part_names(forest->c[k]->c[1], pub);
        ast_push(pubs, pub);
        for (int i = 0; i < pub->n; i++) { if (pu_has(seen, pub->c[i]->v.sval) || pu_has(pnames, pub->c[i]->v.sval)) pu_add(conf, pub->c[i]); pu_add(seen, pub->c[i]); }
    }
    for (int k = 0; k < nu; k++) {
        tree_t *u = forest->c[k], *all = ast_node_new(TT_VLIST), *from = ast_node_new(TT_VLIST), *to = ast_node_new(TT_VLIST);
        pu_part_names(u->c[1], all);
        pu_part_names(u->c[2], all);
        for (int i = 0; i < all->n; i++) {
            const char *nm = all->c[i]->v.sval, *q = pu_qname(u->c[0]->v.sval, nm);
            int hide = !pu_has(pubs->c[k], nm) || pu_has(conf, nm);
            if (hide) pu_map(from, to, nm, q);
            ast_push(qfrom, leaf_s(TT_VAR, q));
            ast_push(qto, leaf_s(TT_VAR, hide ? q : nm));
        }
        pu_map_uses(from, to, conf, all, forest, pubs, pu_uses(u->c[1]), pu_uses(u->c[2]));
        for (int i = 1; i < u->n; i++) pu_rename(u->c[i], from, to, NULL);
    }
    tree_t *pfrom = ast_node_new(TT_VLIST), *pto = ast_node_new(TT_VLIST);
    pu_map_uses(pfrom, pto, conf, pnames, forest, pubs, pu_uses(block), NULL);
    pu_rename(block, pfrom, pto, NULL);
    for (int k = 0; k < nu; k++) for (int i = 1; i < forest->c[k]->n; i++) forest->c[k]->c[i] = pu_qualify(forest->c[k]->c[i], unames, qfrom, qto);
    block = pu_qualify(block, unames, qfrom, qto);
    tree_t *nb = ast_node_new(TT_BLOCK), *lab = NULL, *con = NULL, *typ = NULL, *var = NULL, *body = ast_node_new(TT_SEQ_EXPR), *ur = ast_node_new(TT_VLIST);
    pu_merge_part(&lab, "label", block);
    for (int k = 0; k < nu; k++) for (int s = 1; s <= 2; s++) pu_merge_part(&con, "const", forest->c[k]->c[s]);
    for (int k = 0; k < nu; k++) for (int s = 1; s <= 2; s++) pu_merge_part(&typ, "type", forest->c[k]->c[s]);
    for (int k = 0; k < nu; k++) for (int s = 1; s <= 2; s++) pu_merge_part(&var, "var", forest->c[k]->c[s]);
    if (lab) ast_push(nb, lab);
    if (con) ast_push(nb, con);
    if (typ) ast_push(nb, typ);
    if (var) ast_push(nb, var);
    for (int i = 0; i < block->n - 1; i++) if (pu_is_part(block->c[i], "const") || pu_is_part(block->c[i], "type") || pu_is_part(block->c[i], "var")) ast_push(nb, block->c[i]);
    for (int k = 0; k < nu; k++) {
        tree_t *intf = forest->c[k]->c[1];
        for (int i = 0; i < intf->n; i++) {
            if (!IS(intf->c[i], TT_PROCEDURE) && !IS(intf->c[i], TT_FUNCTION)) continue;
            ast_push(intf->c[i], leaf_s(TT_KEYWORD, "forward"));
            ast_push(nb, intf->c[i]);
            ast_push(ur, intf->c[i]);
        }
    }
    for (int k = 0; k < nu; k++) {
        tree_t *impl = forest->c[k]->c[2];
        for (int i = 0; i < impl->n; i++) if (IS(impl->c[i], TT_PROCEDURE) || IS(impl->c[i], TT_FUNCTION)) { ast_push(nb, impl->c[i]); ast_push(ur, impl->c[i]); }
    }
    for (int i = 0; i < block->n - 1; i++) if (IS(block->c[i], TT_PROCEDURE) || IS(block->c[i], TT_FUNCTION)) ast_push(nb, block->c[i]);
    for (int k = 0; k < nu; k++) pu_stmts(body, pu_child_part(forest->c[k], "initialization"));
    tree_t *main_seq = block->n > 0 ? block->c[block->n - 1] : NULL;
    if (IS(main_seq, TT_SEQ_EXPR)) for (int j = 0; j < main_seq->n; j++) ast_push(body, main_seq->c[j]);
    int fini = 0;
    for (int k = 0; k < nu; k++) if (pu_child_part(forest->c[k], "finalization")) fini = 1;
    if (fini) ast_push(body, pu_fnc0("fini.mute"));
    for (int k = nu - 1; k >= 0; k--) pu_stmts(body, pu_child_part(forest->c[k], "finalization"));
    ast_push(nb, body);
    pu_prune_routines(nb, ur);
    tree_t *np = ast_node_new(TT_PROGRAM);
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) { if (i == bi) ast_push(np, nb); else if (prog->c[i] != forest) ast_push(np, prog->c[i]); }
    return np;
}
static tree_t *pas_agg_type_named(const char *name, struct pas_blk *chain) {
    for (struct pas_blk *c = chain; c && name; c = c->up) for (int i = 0; c->b && i < c->b->n; i++) {
        tree_t *part = c->b->c[i];
        if (!IS(part, TT_PART) || !part->v.sval || strcmp(part->v.sval, "type")) continue;
        for (int j = 0; j < part->n; j++) {
            tree_t *d = part->c[j];
            if (IS(d, TT_DECL) && d->n >= 2 && d->c[0] && d->c[0]->t == TT_VAR && d->c[0]->v.sval && !strcmp(d->c[0]->v.sval, name)) return d->c[1];
        }
    }
    return NULL;
}
static tree_t *pas_agg_type_resolve(tree_t *t, struct pas_blk *chain) {
    for (int guard = 0; t && t->t == TT_VAR && t->v.sval && guard < 8; guard++) { tree_t *r = pas_agg_type_named(t->v.sval, chain); if (!r) break; t = r; }
    return t;
}
static tree_t *pas_agg_index(tree_t *x, int k, struct pas_blk *chain) {
    x = pas_agg_type_resolve(x, chain);
    if (!x) return NULL;
    if (x->t == TT_VAR && x->v.sval && !strcmp(x->v.sval, "boolean")) return leaf_s(TT_VAR, k ? "true" : "false");
    if (x->t == TT_VAR && x->v.sval && !strcmp(x->v.sval, "char")) { char *b = (char *)ct_zalloc(2, 1); b[0] = (char)k; return leaf_s(TT_QLIT, b); }
    if (x->t == TT_ENUM_TYPE) return k < x->n && x->c[k] && x->c[k]->v.sval ? leaf_s(TT_VAR, x->c[k]->v.sval) : NULL;
    if (x->t != TT_SUBRANGE || x->n < 1 || !x->c[0]) return NULL;
    tree_t *lo = x->c[0];
    if (lo->t == TT_ILIT) return ilit(lo->v.ival + k);
    if (lo->t == TT_MNS && lo->n == 1 && lo->c[0] && lo->c[0]->t == TT_ILIT) return ilit(-lo->c[0]->v.ival + k);
    if (lo->t == TT_PLS && lo->n == 1 && lo->c[0] && lo->c[0]->t == TT_ILIT) return ilit(lo->c[0]->v.ival + k);
    if ((lo->t == TT_QLIT && lo->v.sval && strlen(lo->v.sval) == 1) || lo->t == TT_CHRLIT) {
        char *b = (char *)ct_zalloc(2, 1);
        b[0] = (char)((lo->t == TT_CHRLIT ? (int)lo->v.ival : (unsigned char)lo->v.sval[0]) + k);
        return leaf_s(TT_QLIT, b);
    }
    tree_t *e = pas_tree_clone(lo);
    for (int i = 0; i < k; i++) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "succ")); ast_push(f, e); e = f; }
    return e;
}
static tree_t *pas_agg_field_type(tree_t *rt, const char *fname) {
    if (!rt || rt->t != TT_RECORD) return NULL;
    for (int i = 0; i < rt->n; i++) {
        tree_t *fl = rt->c[i];
        if (!IS(fl, TT_FIELDS)) continue;
        for (int j = 0; j < fl->n; j++) {
            tree_t *d = fl->c[j];
            if (!IS(d, TT_DECL) || d->n < 2 || !IS(d->c[0], TT_VLIST)) continue;
            for (int k = 0; k < d->c[0]->n; k++) if (d->c[0]->c[k] && d->c[0]->c[k]->v.sval && !strcmp(d->c[0]->c[k]->v.sval, fname)) return d->c[1];
        }
    }
    return NULL;
}
static int pas_agg_assign(tree_t *lhs, tree_t *type, int dim, tree_t *val, tree_t *out, struct pas_blk *chain) {
    if (val && val->t == TT_VLIST && val->v.sval && !strcmp(val->v.sval, "aggregate")) {
        tree_t *at = pas_agg_type_resolve(type, chain);
        if (!at || at->t != TT_ARRAY_TYPE) return 0;
        int nidx = 0, first = -1;
        tree_t *elem = at->n > 0 ? at->c[at->n - 1] : NULL;
        for (int i = 0; i < at->n - 1; i++) if (at->c[i] && at->c[i]->t != TT_KEYWORD) { if (first < 0) first = i; nidx++; }
        if (nidx == 0 && dim == 0 && elem) {
            tree_t *sl = ast_node_new(TT_FNC);
            ast_push(sl, leaf_s(TT_VAR, "setlength"));
            ast_push(sl, pas_tree_clone(lhs));
            ast_push(sl, ilit(val->n));
            ast_push(out, sl);
            for (int k = 0; k < val->n; k++) {
                tree_t *sel = ast_node_new(TT_IDX);
                ast_push(sel, pas_tree_clone(lhs));
                ast_push(sel, ilit(k));
                tree_t *ev = val->c[k], *et = pas_agg_type_resolve(elem, chain);
                if (et && et->t == TT_ARRAY_TYPE && !(ev && ev->t == TT_VLIST && ev->v.sval && !strcmp(ev->v.sval, "aggregate"))) {
                    tree_t *one = ast_node_new(TT_VLIST);
                    one->v.sval = "aggregate";
                    ast_push(one, ev);
                    ev = one;
                }
                if (!pas_agg_assign(sel, elem, 0, ev, out, chain)) return 0;
            }
            return 1;
        }
        if (dim >= nidx || !elem || first < 0) return 0;
        for (int k = 0; k < val->n; k++) {
            tree_t *ix = pas_agg_index(at->c[first + dim], k, chain);
            if (!ix) return 0;
            tree_t *sel = ast_node_new(TT_IDX);
            ast_push(sel, pas_tree_clone(lhs));
            ast_push(sel, ix);
            if (dim + 1 < nidx) { if (!pas_agg_assign(sel, at, dim + 1, val->c[k], out, chain)) return 0; } else if (!pas_agg_assign(sel, elem, 0, val->c[k], out, chain)) return 0;
        }
        return 1;
    }
    if (val && val->t == TT_VLIST && val->v.sval && !strcmp(val->v.sval, "record aggregate")) {
        tree_t *rt = pas_agg_type_resolve(type, chain);
        for (int k = 0; k < val->n; k++) {
            tree_t *f = val->c[k];
            if (!IS(f, TT_FIELD) || f->n < 2 || !f->c[0] || !f->c[0]->v.sval) return 0;
            tree_t *sel = ast_node_new(TT_FIELD);
            ast_push(sel, pas_tree_clone(lhs));
            ast_push(sel, leaf_s(TT_VAR, f->c[0]->v.sval));
            if (!pas_agg_assign(sel, pas_agg_field_type(rt, f->c[0]->v.sval), 0, f->c[1], out, chain)) return 0;
        }
        return 1;
    }
    ast_push(out, bin(TT_ASSIGN, lhs, val));
    return 1;
}
void pas_var_inits_in(tree_t *t, struct pas_blk *chain) {
    if (!t) return;
    struct pas_blk me = { t, chain };
    struct pas_blk *inner = IS(t, TT_BLOCK) ? &me : chain;
    for (int i = 0; i < t->n; i++) pas_var_inits_in(t->c[i], inner);
    if (!IS(t, TT_BLOCK) || t->n == 0 || !IS(t->c[t->n - 1], TT_SEQ_EXPR)) return;
    tree_t *inits = ast_node_new(TT_SEQ_EXPR), *body = t->c[t->n - 1];
    for (int i = 0; i < t->n - 1; i++) {
        tree_t *part = t->c[i];
        if (!IS(part, TT_PART) || !part->v.sval) continue;
        if (!strcmp(part->v.sval, "const")) {
            for (int j = 0; j < part->n; j++) {
                tree_t *d = part->c[j];
                if (!IS(d, TT_DECL) || !d->v.sval || strcmp(d->v.sval, "tconst") || d->n < 3 || !IS(d->c[0], TT_VLIST) || d->c[0]->n < 1) continue;
                tree_t *seq = ast_node_new(TT_SEQ_EXPR);
                if (pas_agg_assign(leaf_s(TT_VAR, d->c[0]->c[0]->v.sval), d->c[1], 0, d->c[2], seq, &me)) { for (int k = 0; k < seq->n; k++) ast_push(inits, seq->c[k]); d->n = 2; }
            }
            continue;
        }
        if (strcmp(part->v.sval, "var")) continue;
        for (int j = 0; j < part->n; j++) {
            tree_t *d = part->c[j];
            if (!IS(d, TT_DECL) || d->n < 3 || !IS(d->c[0], TT_VLIST)) continue;
            if (IS(d->c[2], TT_VLIST) && d->c[2]->v.sval && (!strcmp(d->c[2]->v.sval, "aggregate") || !strcmp(d->c[2]->v.sval, "record aggregate"))) {
                tree_t *seq = ast_node_new(TT_SEQ_EXPR);
                int ok = 1;
                for (int k = 0; ok && k < d->c[0]->n; k++) ok = pas_agg_assign(leaf_s(TT_VAR, d->c[0]->c[k]->v.sval), d->c[1], 0, k ? pas_tree_clone(d->c[2]) : d->c[2], seq, &me);
                if (!ok) continue;
                for (int k = 0; k < seq->n; k++) ast_push(inits, seq->c[k]);
                d->n = 2;
                continue;
            }
            for (int k = 0; k < d->c[0]->n; k++) ast_push(inits, bin(TT_ASSIGN, leaf_s(TT_VAR, d->c[0]->c[k]->v.sval), k ? pas_tree_clone(d->c[2]) : d->c[2]));
            d->n = 2;
        }
    }
    if (inits->n == 0) return;
    for (int i = 0; i < body->n; i++) ast_push(inits, body->c[i]);
    t->c[t->n - 1] = inits;
}
