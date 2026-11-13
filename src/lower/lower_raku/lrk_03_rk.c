#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "runtime/builtin_table.h"
#include <stdlib.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_08_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "lower/lower_raku/lrk_10_rk.h"
#include "lower/lower_raku/lrk_11_rk.h"
#include "lower/lower_raku/lrk_12_rk.h"
#include "lower/g_lower_rk.h"
static tree_t * rk_tv_check(rk_tvenv_t * e, tree_t * val, const char * type, const char * name, int element);
int rk_is_class_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower_rk.class_names.len; i++) if (!strcmp(CV_AT(g_lower_rk.class_names, const char *, i), nm)) return 1;
    return 0;
}
tree_t * leaf_sval2(tree_e kind, const char * s) { tree_t * n = ast_node_new(kind); n->v.sval = (char *)s; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_coerce_val(tree_t * val, const char * base) { tree_t * m = ast_node_new(TT_METHCALL); m->line = val->line; ast_push(m, val); ast_push(m, leaf_sval2(TT_QLIT, base)); return m; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_user_proc_exists(const char * nm) {
    for (int i = 0; i < g_stage2.proc_count; i++) { const char * pn = g_stage2.proc_table[i].name; if (pn && (!strcmp(pn, nm) || (pn[0] == '&' && !strcmp(pn + 1, nm)))) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * const rk_shadowable_terms[] = { "now", "time", "rand", "Empty", "Less", "Same", "More", NULL };
int rk_certainly_undeclared(const char * nm) {
    extern int rt_builtin_is_known(const char *);
    if (!isalpha((unsigned char) nm[0])) return 0;
    for (const char * p = nm; *p; p++) if (!isalnum((unsigned char) *p) && *p != '-' && *p != '_') return 0;
    extern int rk_is_core_routine(const char *);
    return !rk_is_core_routine(nm) && bt_row_of(BT_VOC_RK, nm, (unsigned) strlen(nm)) < 0 && !rk_user_proc_exists(nm) && !rk_proc_known(nm) && !rk_is_multi_name(nm) && !rk_is_class_name(nm) &&
        !rt_builtin_is_known(nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_predeclared(const char * nm, int bare) {
    if (!nm || !*nm) return 0;
    static const char * const dyn[] = { "*DISTRO", "*KERNEL", "*VM", "*RAKU", "*PERL", "*PID", "*PROGRAM", "*PROGRAM-NAME", "*CWD", "*EXECUTABLE", "*EXECUTABLE-NAME", "*HOME", "*TMPDIR", "*USER",
        "%*ENV", "@*ARGS", "?FILE", "/", "!", "Order::Less", "Order::Same", "Order::More", "Bool::True", "Bool::False", NULL };
    for (int i = 0; dyn[i]; i++) if (!strcmp(nm, dyn[i])) return 1;
    if (!bare) return 0;
    for (int i = 0; rk_shadowable_terms[i]; i++) if (!strcmp(nm, rk_shadowable_terms[i]) && !rk_is_class_name(nm) && !rk_user_proc_exists(nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_to_array_call(const tree_t * rhs) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) "__rk_to_array";
    ast_push(f, leaf_sval2(TT_VAR, "__rk_to_array"));
    ast_push(f, (tree_t *) rhs);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_to_hash_call(const tree_t * rhs) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) "__rk_to_hash";
    ast_push(f, leaf_sval2(TT_VAR, "__rk_to_hash"));
    ast_push(f, (tree_t *) rhs);
    return f;
}
static int rk_is_nil_term(const tree_t * t) { return t && t->t == TT_VAR && !t->n && (t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "Nil") && !rk_is_class_name("Nil"); }
tree_t * rk_typeobj_tree(const char * ty) { return rk_fnc2("__rk_typeobj", leaf_sval2(TT_QLIT, ty), NULL); }
const char * rk_decl_typeobj_name(const char * ty) {
    if (!ty || !isupper((unsigned char) ty[0])) return NULL;
    for (const char * p = ty; *p; p++) if (!isalnum((unsigned char) *p) && *p != '_' && !(*p == ':' && (p[1] == ':' || (p > ty && p[-1] == ':')))) return NULL;
    return ty;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_assign_rhs(const char * vn, const tree_t * rhs) {
    if (!vn || !rhs) return rhs;
    if (vn[0] == '@') return rk_to_array_call(rhs);
    if (vn[0] == '%') { if (rhs->t == TT_FNC && rhs->n > 0 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__rk_hash")) return rhs; return rk_to_hash_call(rhs); }
    if (rk_is_nil_term(rhs)) return rk_typeobj_tree("Any");
    return rhs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_fnc2(const char * nm, const tree_t * a, const tree_t * b) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) nm;
    ast_push(f, leaf_sval2(TT_VAR, nm));
    if (a) ast_push(f, (tree_t *) a);
    if (b) ast_push(f, (tree_t *) b);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_arr_operand(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '@') return 1;
    return t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_arr");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_numctx(tree_e k, const tree_t * x, const tree_t * o) {
    int num = k == TT_LT || k == TT_LE || k == TT_GT || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_ADD || k == TT_SUB || k == TT_MUL || k == TT_POW;
    int plus0 = k == TT_ADD && o && o->t == TT_ILIT && o->v.ival == 0 && x && x->t != TT_ILIT && x->t != TT_FLIT && x->t != TT_QLIT;
    return num && (plus0 || rk_yields_list(x) || rk_arr_operand(x)) ? rk_fnc2("__rk_numify", x, NULL) : x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * rk_prologue_target(const tree_t * s) {
    if (!s || s->t != TT_UNLESS || s->n < 1 || !s->c[0]) return NULL;
    const tree_t * mc = s->c[0];
    if (mc->t != TT_METHCALL || mc->n < 2 || !mc->c[0] || !mc->c[1] || !mc->c[1]->v.sval || strcmp(mc->c[1]->v.sval, "defined")) return NULL;
    return (mc->c[0]->t == TT_VAR) ? mc->c[0]->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_set_tail(tree_t * st, const char * tmp) {
    if (!st) return st;
    if (st->t == TT_SEQ_EXPR && st->n > 0) { st->c[st->n - 1] = rk_set_tail(st->c[st->n - 1], tmp); return st; }
    if (st->t == TT_IF || st->t == TT_UNLESS || st->t == TT_CASE) {
        if (st->n < 2) return st;
        for (int i = (st->t == TT_CASE ? 1 : 1); i < st->n; i++) {
            tree_t * ch = st->c[i];
            if (st->t == TT_CASE && ch && ch->t == TT_ARM && ch->n >= 1) { ch->c[ch->n - 1] = rk_set_tail(ch->c[ch->n - 1], tmp); continue; }
            if (ch && (ch->t == TT_SEQ_EXPR || ch->t == TT_IF || ch->t == TT_UNLESS || ch->t == TT_CASE)) st->c[i] = rk_set_tail(ch, tmp);
        }
        return st;
    }
    if (!rk_block_tail_is_value(st)) return st;
    tree_t * as = ast_node_new(TT_ASSIGN);
    as->line = st->line;
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, st);
    return as;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_value_wrap(tree_t * st) {
    char nb[48];
    snprintf(nb, sizeof nb, "__vf_%d", g_lower_rk.rk_case_desugar_cid++);
    const char * tmp = intern(nb);
    tree_t * seq = ast_node_new(TT_SEQ);
    seq->line = st->line;
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, rk_fnc2("__rk_undef", NULL, NULL));
    ast_push(seq, as);
    ast_push(seq, rk_set_tail(st, tmp));
    ast_push(seq, leaf_sval2(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_value_form(const tree_t * t) { return t && (t->t == TT_IF || t->t == TT_UNLESS || (t->t == TT_CASE && t->n > 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_value_forms(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_value_forms(t->c[i]);
    if (t->t == TT_ASSIGN && t->n == 2 && rk_is_value_form(t->c[1])) t->c[1] = rk_value_wrap(t->c[1]);
    if (t->t == TT_ITERATE && t->n > 0 && rk_is_value_form(t->c[0])) t->c[0] = rk_value_wrap(t->c[0]);
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%') {
        tree_t * mc = ast_node_new(TT_METHCALL);
        mc->line = t->c[0]->line;
        ast_push(mc, t->c[0]);
        ast_push(mc, leaf_sval2(TT_QLIT, "pairs"));
        t->c[0] = mc;
    }
    if (t->t == TT_FNC && t->n == 2 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_phaser_do") && t->c[1] && t->c[1]->t == TT_SEQ_EXPR && t->c[1]->n > 0 &&
        rk_is_value_form(t->c[1]->c[t->c[1]->n - 1])) t->c[1]->c[t->c[1]->n - 1] = rk_value_wrap(t->c[1]->c[t->c[1]->n - 1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_block_decl(const tree_t * prog, const char * nm) {
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (sub) d = sub; }
        if (d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strcmp(d->c[0]->v.sval, nm)) return d;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_smartmatch_code_walk(const tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_smartmatch_code_walk(prog, t->c[i]);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_smartmatch") || t->n != 3) return;
    int ok = 0;
    tree_t * bc = rk_code_call1(prog, t->c[2], t->c[1], &ok);
    if (!ok) {
        tree_t * r = t->c[2];
        int dyn = r &&
            (r->t == TT_HASH_GET || r->t == TT_ARR_GET ||
            (r->t == TT_VAR && r->v.sval && r->v.sval[0] && !strchr("&@%:", r->v.sval[0]) && !(r->v.sval[0] >= 'A' && r->v.sval[0] <= 'Z') && !rk_is_class_name(r->v.sval) && !(r->slen & 1)));
        if (!dyn) return;
        char b[64];
        snprintf(b, sizeof b, "__smdl%p", (const void *) t);
        const char * lv = lp_strdup(b);
        snprintf(b, sizeof b, "__smdr%p", (const void *) t);
        const char * rv = lp_strdup(b);
        tree_t * lhs = t->c[1];
        tree_t * iv = ast_node_new(TT_INVOKE);
        ast_push(iv, rk_tv(rv));
        ast_push(iv, rk_tv(lv));
        tree_t * tn = ast_node_new(TT_TERNARY);
        ast_push(tn, rk_fnc2("__rk_is_code", rk_tv(rv), NULL));
        ast_push(tn, rk_fnc2("__rk_mkbool", iv, NULL));
        ast_push(tn, rk_fnc2("__rk_smartmatch", rk_tv(lv), rk_tv(rv)));
        t->t = TT_SEQ_EXPR;
        t->v.sval = NULL;
        t->n = 0;
        ast_push(t, rk_gath_asg(rk_tv(lv), lhs));
        ast_push(t, rk_gath_asg(rk_tv(rv), r));
        ast_push(t, tn);
        return;
    }
    t->v.sval = (char *) "__rk_mkbool";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_mkbool");
    t->c[1] = bc;
    t->n = 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sort_cmp_walk(tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sort_cmp_walk(prog, t->c[i]);
    if (t->t != TT_METHCALL || t->n != 3 || !t->c[1] || !t->c[1]->v.sval || strcmp(t->c[1]->v.sval, "sort") || !t->c[2] || !t->c[0]) return;
    const tree_t * cl = rk_closure_block(t->c[2]) ? t->c[2] : NULL, * B = cl ? cl->c[1] : t->c[2];
    if (B->t != TT_ANON_BLOCK || !B->v.sval) return;
    if (t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%') return;
    int nref = cl ? cl->n - 2 : 0;
    const tree_t * bd = rk_block_decl(prog, B->v.sval);
    if (!bd || bd->v.ival != 2 + nref) return;
    char hn[strlen(B->v.sval) + 16];
    snprintf(hn, sizeof hn, "__rk_sortc_%s", B->v.sval);
    const char * hname = lp_strdup(hn);
    if (!rk_block_decl(prog, hname)) ast_push(prog, rk_sort_helper(hname, B->v.sval, nref));
    tree_t * recv = t->c[0];
    t->t = TT_FNC;
    t->v.sval = (char *) hname;
    t->n = 0;
    ast_push(t, rk_tv(hname));
    ast_push(t, recv);
    for (int i = 2; cl && i < cl->n; i++) ast_push(t, cl->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_var_scalar_name(const tree_t * v) {
    if (!v || v->t != TT_VAR || !v->v.sval || (v->slen & 4)) return 0;
    char c = v->v.sval[0] == '*' ? v->v.sval[1] : v->v.sval[0];
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static int rk_var_agg_name(const tree_t * v) {
    if (!v || v->t != TT_VAR || v->n || !v->v.sval || (v->slen & 4)) return 0;
    char c = v->v.sval[0], d = v->v.sval[1];
    return (c == '@' || c == '%') && ((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z') || d == '_');
}
static tree_t * rk_var_operand(tree_t * t) {
    if (!t) return NULL;
    if (t->t == TT_METHCALL && t->n == 2 && t->c[0] && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "VAR")) return t->c[0];
    if (t->t == TT_FNC && t->n == 2 && t->v.sval && !strcmp(t->v.sval, "VAR")) return t->c[1];
    return NULL;
}
typedef struct rk_vctx { const tree_t * scope; const struct rk_vctx * up; } rk_vctx;
static int rk_var_is_param(const rk_vctx * c, const char * nm) {
    for (; c; c = c->up) for (int i = 0; i < c->scope->n; i++) { const tree_t * p = c->scope->c[i]; if (p && p->t == TT_VAR && p->n == 1 && p->v.sval && !strcmp(p->v.sval, nm)) return 1; }
    return 0;
}
static void rk_var_container_walk(tree_t * t, const rk_vctx * up) {
    if (!t) return;
    rk_vctx here = { t, up };
    const rk_vctx * cx = (t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK) ? &here : up;
    if (t->t == TT_METHCALL && t->n == 2 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "name") && rk_var_agg_name(rk_var_operand(t->c[0])) &&
        !rk_var_is_param(cx, rk_var_operand(t->c[0])->v.sval)) {
        tree_t * q = leaf_sval2(TT_QLIT, rk_var_operand(t->c[0])->v.sval);
        t->t = q->t;
        t->v = q->v;
        t->slen = q->slen;
        t->n = 0;
        return;
    }
    for (int i = 0; i < t->n; i++) rk_var_container_walk(t->c[i], cx);
    tree_t * v = rk_var_operand(t);
    if (rk_var_agg_name(v)) { t->t = v->t; t->v = v->v; t->slen = v->slen; t->n = 0; return; }
    if (!rk_var_scalar_name(v)) return;
    char nb[strlen(v->v.sval) + 2];
    nb[0] = '$';
    strcpy(nb + 1, v->v.sval);
    tree_t * nm = leaf_sval2(TT_QLIT, lp_strdup(nb));
    t->t = TT_NEW;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, leaf_sval2(TT_QLIT, "Scalar"));
    ast_push(t, leaf_sval2(TT_QLIT, "name"));
    ast_push(t, nm);
    ast_push(t, leaf_sval2(TT_QLIT, "of"));
    ast_push(t, rk_fnc2("__rk_typeobj", leaf_sval2(TT_QLIT, "Mu"), NULL));
    ast_push(t, leaf_sval2(TT_QLIT, "dynamic"));
    ast_push(t, rk_fnc2("__rk_mkbool", rk_ti(v->v.sval[0] == '*'), NULL));
    ast_push(t, leaf_sval2(TT_QLIT, "value"));
    ast_push(t, v);
}
static tree_t * rk_range_call_of(tree_t * to, const char * fn, int flagged) {
    tree_t * w = ast_node_new(TT_FNC);
    w->line = to->line;
    w->v.sval = (char *) fn;
    ast_push(w, leaf_sval2(TT_VAR, fn));
    ast_push(w, to->c[0]);
    ast_push(w, to->c[1]);
    if (flagged || to->v.ival) { tree_t * fl = ast_node_new(TT_ILIT); fl->v.ival = to->v.ival; ast_push(w, fl); }
    return w;
}
static tree_t * rk_range_val_of(tree_t * to) { return rk_range_call_of(to, "__rk_range_val", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_method_name(const char * m) {
    static const char * const names[] = { "WHAT", "raku", "perl", "gist", "min", "max", "minmax", "excludes-min", "excludes-max", "bounds", NULL };
    for (int i = 0; m && names[i]; i++) if (!strcmp(m, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_xx_value_is_pure(const tree_t * e) {
    if (!e) return 1;
    if (e->t == TT_QLIT || e->t == TT_ILIT || e->t == TT_FLIT || e->t == TT_NUL || e->t == TT_VAR) return 1;
    if (e->t == TT_SEQ_EXPR && e->n == 1) return rk_xx_value_is_pure(e->c[0]);
    if (e->t == TT_TO && e->n == 2) return rk_xx_value_is_pure(e->c[0]) && rk_xx_value_is_pure(e->c[1]);
    if (e->t != TT_FNC || !e->v.sval || strcmp(e->v.sval, "__rk_arr")) return 0;
    for (int i = 1; i < e->n; i++) if (!rk_xx_value_is_pure(e->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_range_values(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_range_values(t->c[i]);
    if (t->t == TT_ASSIGN && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] != '@' && t->c[0]->v.sval[0] != '%' && t->c[0]->v.sval[0] != '&' && t->c[1] &&
        t->c[1]->t == TT_TO && t->c[1]->n == 2) t->c[1] = rk_range_val_of(t->c[1]);
    if (t->t == TT_DECL && t->n >= 2 && t->c[t->n - 1] && t->c[t->n - 1]->t == TT_TO && t->c[t->n - 1]->n == 2 && t->c[t->n - 2] && t->c[t->n - 2]->t == TT_VAR && t->c[t->n - 2]->v.sval &&
        t->c[t->n - 2]->v.sval[0] != '@' && t->c[t->n - 2]->v.sval[0] != '%') t->c[t->n - 1] = rk_range_val_of(t->c[t->n - 1]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_lit")) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) {
        if (t->n == 2) {
            tree_t * w = ast_node_new(TT_FNC);
            w->line = t->line;
            w->v.sval = (char *) "__rk_to_array";
            ast_push(w, leaf_sval2(TT_VAR, "__rk_to_array"));
            ast_push(w, rk_range_call_of(t->c[i], "__rk_range_arr", 0));
            *t = *w;
            break;
        }
        t->c[i] = rk_range_val_of(t->c[i]);
    }
    if (t->t == TT_FNC && t->v.sval && strncmp(t->v.sval, "__", 2)) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr")) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_xx") && t->n == 3 && t->c[1] && t->c[1]->t == TT_TO && t->c[1]->n == 2) t->c[1] = rk_range_val_of(t->c[1]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_pair") && t->n == 3 && t->c[2] && t->c[2]->t == TT_TO && t->c[2]->n == 2) t->c[2] = rk_range_val_of(t->c[2]);
    if (t->t == TT_METHCALL) for (int i = 2; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_INVOKE) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "__rk_in_range") || !strcmp(t->v.sval, "__rk_numify") || !strcmp(t->v.sval, "__rk_smartmatch"))) {
        for (int i = 1; i < t->n && i < (strcmp(t->v.sval, "__rk_smartmatch") ? 2 : 3); i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    }
    if (t->t == TT_SAY || t->t == TT_PRINT || t->t == TT_REVERSE || t->t == TT_SORT) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] =
        rk_range_val_of(t->c[i]);
    if (t->t == TT_METHCALL && t->n >= 2 && t->c[0] && t->c[0]->t == TT_TO && t->c[0]->n == 2 && t->c[1] && t->c[1]->t == TT_QLIT && rk_range_method_name(t->c[1]->v.sval)) t->c[0] =
        rk_range_val_of(t->c[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tv_enforced(const char * ty, const rk_ns_t * cls) {
    if (!ty) return 0;
    if (!strcmp(ty, "Int") || !strcmp(ty, "Str") || !strcmp(ty, "Num") || !strcmp(ty, "Bool") || rk_is_class_name(ty)) return 1;
    for (int i = 0; cls && i < cls->n; i++) if (!strcmp(cls->v[i], ty)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tv_push(rk_tvenv_t * e, const char * name, const char * type) {
    if (e->n >= e->cap) { e->cap = e->cap ? e->cap * 2 : 16; e->v = (rk_tv_t *) ct_grow(e->v, sizeof(rk_tv_t) * (size_t) e->cap); }
    e->v[e->n].name = name;
    e->v[e->n].type = type;
    e->n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_tv_find(rk_tvenv_t * e, const char * name) { for (int i = e->n - 1; i >= 0; i--) if (!strcmp(e->v[i].name, name)) return e->v[i].type; return NULL; }
static tree_t * rk_tv_check_plain(tree_t * val, const char * type, const char * name, int element) {
    tree_t * w = ast_node_new(TT_FNC);
    w->line = val->line;
    w->v.sval = (char *) "__rk_assign_check";
    ast_push(w, leaf_sval2(TT_VAR, "__rk_assign_check"));
    ast_push(w, leaf_sval2(TT_QLIT, type));
    ast_push(w, val);
    ast_push(w, leaf_sval2(TT_QLIT, name));
    tree_t * el = ast_node_new(TT_ILIT);
    el->v.ival = element;
    ast_push(w, el);
    return w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tv_check(rk_tvenv_t * e, tree_t * val, const char * type, const char * name, int element) {
    if (!rk_ns_has(&e->subsets, type)) return rk_tv_check_plain(val, type, name, element);
    char tn[24];
    snprintf(tn, sizeof tn, "__ssv%d", e->ssn++);
    const char * tmp = intern(tn);
    int line = val->line;
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = line;
    tree_t * as = ast_node_new(TT_ASSIGN);
    as->line = line;
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, val);
    ast_push(seq, as);
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, rk_tv_check_plain(leaf_sval2(TT_VAR, tmp), type, name, element));
    tree_t * u = ast_node_new(TT_UNLESS);
    u->line = line;
    ast_push(u, rk_ss_call(type, leaf_sval2(TT_VAR, tmp)));
    ast_push(u, blk);
    ast_push(seq, u);
    ast_push(seq, leaf_sval2(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_tv_walk(tree_t * t, rk_tvenv_t * e) {
    if (!t) return;
    int mark = e->n;
    int scope = t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK || t->t == TT_SEQ_EXPR || t->t == TT_CLASS_DECL;
    if (t->t == TT_SUB_DECL) for (int i = 1; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (c && c->t == TT_VAR && c->v.sval && (c->n == 0 || (c->n >= 1 && c->c[0] && c->c[0]->t == TT_QLIT))) rk_tv_push(e, c->v.sval, NULL);
    }
    if (t->t == TT_ANON_BLOCK) for (int i = 1; i < t->n; i++) { tree_t * c = t->c[i]; if (c && c->t == TT_VAR && c->v.sval) rk_tv_push(e, c->v.sval, NULL); }
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_DECL && c->n >= 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[1] && c->c[1]->t == TT_VAR && c->c[1]->v.sval && c->c[1]->v.sval[0] != '@' && c->c[1]->v.sval[0] != '%' &&
            rk_coerce_base(c->c[0]->v.sval)) {
            rk_tv_walk(c, e);
            const char * cty = c->c[0]->v.sval, * base = rk_coerce_base(cty), * nm = c->c[1]->v.sval;
            c->c[0]->v.sval = (char *) base;
            rk_tv_push(e, nm, cty);
            if (c->n >= 3 && c->c[2] && rk_is_nil_term(c->c[2])) c->c[2] = rk_typeobj_tree(base);
            else if (c->n >= 3 && c->c[2]) c->c[2] = rk_tv_check(e, rk_coerce_val(c->c[2], base), base, nm, 0);
            continue;
        }
        if (c->t == TT_DECL && c->n >= 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[1] && c->c[1]->t == TT_VAR && c->c[1]->v.sval && c->c[0]->v.sval && rk_tv_enforced(c->c[0]->v.sval, &e->classes)) {
            rk_tv_walk(c, e);
            const char * ty = c->c[0]->v.sval, * nm = c->c[1]->v.sval;
            rk_tv_push(e, nm, ty);
            if (c->n >= 3 && c->c[2] && nm[0] != '@' && nm[0] != '%' && rk_is_nil_term(c->c[2])) c->c[2] = rk_typeobj_tree(ty);
            else if (c->n >= 3 && c->c[2] && nm[0] != '@' && nm[0] != '%') c->c[2] = rk_tv_check(e, c->c[2], ty, nm, 0);
            continue;
        }
        if (c->t == TT_DECL && c->n >= 1) { rk_tv_walk(c, e); tree_t * v = c->c[c->n >= 3 ? 1 : 0]; if (v && v->t == TT_VAR && v->v.sval) rk_tv_push(e, v->v.sval, NULL); continue; }
        rk_tv_walk(c, e);
        if (c->t == TT_ASSIGN && c->n == 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval) {
            const char * nm = c->c[0]->v.sval, * ty = rk_tv_find(e, nm), * cb = rk_coerce_base(ty);
            if (nm[0] != '@' && nm[0] != '%' && rk_is_nil_term(c->c[1])) c->c[1] = rk_typeobj_tree(cb ? cb : ty ? ty : "Any");
            else if (cb && nm[0] != '@' && nm[0] != '%') c->c[1] = rk_tv_check(e, rk_coerce_val(c->c[1], cb), cb, nm, 0);
            else if (ty && nm[0] != '@' && nm[0] != '%') c->c[1] = rk_tv_check(e, c->c[1], ty, nm, 0);
            else if (!ty && nm[0] != '@' && nm[0] != '%' && !rk_tv_find(e, nm)) rk_tv_push(e, nm, NULL);
        }
        if ((c->t == TT_ARR_SET || c->t == TT_HASH_SET) && c->n == 3 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval) {
            const char * ty = rk_tv_find(e, c->c[0]->v.sval);
            if (rk_is_nil_term(c->c[2])) c->c[2] = rk_typeobj_tree(ty ? ty : "Any");
            else if (ty) c->c[2] = rk_tv_check(e, c->c[2], ty, c->c[0]->v.sval, 1);
        }
        if (c->t == TT_METHCALL && c->n >= 3 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && c->c[0]->v.sval[0] == '@' && c->c[1] && c->c[1]->t == TT_QLIT && c->c[1]->v.sval &&
            (!strcmp(c->c[1]->v.sval, "push") || !strcmp(c->c[1]->v.sval, "append") || !strcmp(c->c[1]->v.sval, "unshift") || !strcmp(c->c[1]->v.sval, "prepend"))) {
            const char * ty = rk_tv_find(e, c->c[0]->v.sval);
            if (ty) for (int k = 2; k < c->n; k++) if (c->c[k] && c->c[k]->t != TT_VAR && c->c[k]->t != TT_FNC) c->c[k] = rk_tv_check(e, c->c[k], ty, c->c[0]->v.sval, 1);
        }
    }
    if (scope) e->n = mark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_end_nonint(const tree_t * e) {
    if (!e) return 0;
    if (e->t == TT_FLIT) return 1;
    if (e->t == TT_QLIT && e->v.sval) { char * ep; if (!*e->v.sval) return 1; strtoll(e->v.sval, &ep, 10); return *ep != 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_has_qlit_e(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_QLIT && t->v.sval && (!strcmp(t->v.sval, "e") || !strncmp(t->v.sval, "e.", 2))) return 1;
    for (int i = 0; i < t->n; i++) if (rk_has_qlit_e(t->c[i])) return 1;
    return 0;
}
static int rk_uses_v6e(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_USE_DECL && t->v.sval && !strcmp(t->v.sval, "v6") && rk_has_qlit_e(t)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_uses_v6e(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sprintf_names(tree_t * t, int v6e) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sprintf_names(t->c[i], v6e);
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "sprintf") || !strcmp(t->v.sval, "printf")) && t->n >= 1 && !rk_proc_known(t->v.sval)) {
        const char * nn = !strcmp(t->v.sval, "sprintf") ? (v6e ? "__rk_sprintf" : "__rk_sprintf_d") : (v6e ? "__rk_printf" : "__rk_printf_d");
        t->v.sval = (char *) nn;
        if (t->c[0] && t->c[0]->t == TT_VAR) t->c[0]->v.sval = (char *) nn;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_nonint_ranges(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_nonint_ranges(c);
        if (c && c->t == TT_TO && c->n == 2 && !c->v.ival && (rk_range_end_nonint(c->c[0]) || rk_range_end_nonint(c->c[1]))) {
            tree_t * w = ast_node_new(TT_FNC);
            w->line = c->line;
            w->v.sval = (char *) "__rk_range_arr";
            ast_push(w, leaf_sval2(TT_VAR, "__rk_range_arr"));
            ast_push(w, c->c[0]);
            ast_push(w, c->c[1]);
            t->c[i] = w;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_exclusive_ranges(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_exclusive_ranges(c);
        if (!c || c->t != TT_TO || c->n != 2 || !c->v.ival) continue;
        if (c->v.ival != 2 || rk_range_end_nonint(c->c[0]) || rk_range_end_nonint(c->c[1])) { t->c[i] = rk_range_call_of(c, "__rk_range_arr", 0); continue; }
        tree_t * hi = c->c[1];
        if (hi && hi->t == TT_ILIT) {
            tree_t * h = ast_node_new(TT_ILIT);
            h->line = hi->line;
            h->v.ival = hi->v.ival - 1;
            hi = h;
        } else {
            if (hi && hi->t == TT_VAR && hi->v.sval && hi->v.sval[0] == '@') { tree_t * el = ast_node_new(TT_METHCALL); ast_push(el, hi); ast_push(el, leaf_sval2(TT_QLIT, "elems")); hi = el; }
            tree_t * ex = ast_node_new(TT_FNC);
            ex->line = c->line;
            ex->v.sval = (char *) "__rk_exend";
            ast_push(ex, leaf_sval2(TT_VAR, "__rk_exend"));
            ast_push(ex, hi);
            hi = ex;
        }
        c->c[1] = hi;
        c->v.ival = 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_ss_arg(int j) { tree_t * g = ast_node_new(TT_ARR_GET); ast_push(g, leaf_sval2(TT_VAR, "@__a")); tree_t * ix = ast_node_new(TT_ILIT); ix->v.ival = j; ast_push(g, ix); return g; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_type_terms(tree_t * t, const rk_ns_t * names) {
    if (!t) return;
    int sm = t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_smartmatch"),
        dz = t->t == TT_METHCALL && t->n == 3 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && (!strcmp(t->c[1]->v.sval, "does") || !strcmp(t->c[1]->v.sval, "isa"));
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_type_terms(c, names);
        if (t->t == TT_DECL || t->t == TT_MODULE_DECL || (sm && i == 2) || (dz && i == 2)) continue;
        if (!c || c->t != TT_VAR || c->n || !(c->slen & 1) || !c->v.sval || !rk_ns_has(names, c->v.sval)) continue;
        tree_t * to = ast_node_new(TT_FNC);
        to->line = c->line;
        to->v.sval = (char *) "__rk_typeobj";
        ast_push(to, leaf_sval2(TT_VAR, "__rk_typeobj"));
        ast_push(to, leaf_sval2(TT_QLIT, c->v.sval));
        t->c[i] = to;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sub_names(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && *t->c[0]->v.sval) {
        size_t bl = strcspn(t->c[0]->v.sval, "$");
        char b[bl + 1];
        memcpy(b, t->c[0]->v.sval, bl);
        b[bl] = 0;
        rk_ns_add(out, intern(b));
    }
    for (int i = 0; i < t->n; i++) rk_sub_names(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_indirect_lit_names(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_indirect_lit_names(t->c[i]);
    if (t->t != TT_VAR || !(t->slen & RK_VAR_INDIRECT) || t->n != 1 || !t->c[0] || t->c[0]->t != TT_QLIT || !t->c[0]->v.sval) return;
    const char * s = t->c[0]->v.sval;
    if (!isalpha((unsigned char) s[0]) || strpbrk(s, ":*?!<>[]()$@%&. ")) return;
    char nb[strlen(s) + 2];
    const char * pre = t->v.sval[0] == '$' ? "" : t->v.sval;
    snprintf(nb, sizeof nb, "%s%s", pre, s);
    t->v.sval = (char *) intern(nb);
    t->slen &= ~RK_VAR_INDIRECT;
    t->n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_builtin_refs(tree_t * t, const rk_ns_t * subs) {
    if (!t) return;
    int skip = t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK || t->t == TT_DECL;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_builtin_refs(c, subs);
        if (skip || !c || c->t != TT_VAR || c->n || !(c->slen & 1) || !c->v.sval || c->v.sval[0] != '&' || !isalpha((unsigned char) c->v.sval[1]) || strpbrk(c->v.sval + 1, ":*?!<>[]()$@% ") ||
            rk_ns_has(subs, c->v.sval + 1)) continue;
        tree_t * ab = ast_node_new(TT_ANON_BLOCK), * sq = ast_node_new(TT_SEQ_EXPR), * fc = ast_node_new(TT_FNC);
        fc->line = c->line;
        fc->v.sval = (char *) intern(c->v.sval + 1);
        ast_push(fc, leaf_sval2(TT_VAR, fc->v.sval));
        ast_push(fc, leaf_sval2(TT_VAR, "_"));
        ast_push(sq, fc);
        ast_push(ab, sq);
        t->c[i] = ab;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ctl_rhs(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_ctl_rhs(c);
        if (!c || c->t != TT_ASSIGN || c->n != 2 || !c->c[1] || (c->c[1]->t != TT_RETURN && c->c[1]->t != TT_LOOP_NEXT && c->c[1]->t != TT_LOOP_BREAK)) continue;
        tree_t * sq = ast_node_new(TT_SEQ_EXPR), * nil = ast_node_new(TT_FNC), * as = ast_node_new(TT_ASSIGN);
        sq->line = c->line;
        nil->v.sval = (char *) "__rk_undef";
        ast_push(nil, leaf_sval2(TT_VAR, "__rk_undef"));
        ast_push(as, c->c[0]);
        ast_push(as, nil);
        ast_push(sq, c->c[1]);
        ast_push(sq, as);
        t->c[i] = sq;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_place_groups(tree_t * t, int * uid) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_place_groups(t->c[i], uid);
    if (t->t != TT_FNC || !t->v.sval) return;
    if (!strcmp(t->v.sval, "__rk_destructure") && t->n >= 2) rk_pl_destructure(t, uid);
    else if (!strcmp(t->v.sval, "__rk_enum") && t->n >= 2) rk_pl_enum(t);
    else if (!strcmp(t->v.sval, "__rk_with_mod") && t->n >= 3) rk_pl_modifier(t, 0);
    else if (!strcmp(t->v.sval, "__rk_without_mod") && t->n >= 3) rk_pl_modifier(t, 1);
    else if (!strcmp(t->v.sval, "__rk_given_mod") && t->n >= 3) rk_pl_modifier(t, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *rk_stage2_core(const tree_t *prog, int reset_multi, int want_main) {
    rk_unfound_dynamics((tree_t *) prog);
    rk_supply_nodes((tree_t *) prog, rk_user_sub_named(prog, "emit"));
    { rk_ns_t rns = { 0 }; rk_relaxed_collect(prog, &rns); rk_relaxed_walk((tree_t *) prog, &rns); }
    {
        rk_ns_t ss = { 0 }, cls = { 0 };
        int mcn = 0;
        { int anon = 0; rk_name_anon_subsig((tree_t *) prog, &anon); rk_subsig_blocks((tree_t *) prog, &anon); }
        rk_amp_methcalls(prog, (tree_t *) prog);
        rk_invocant_params((tree_t *) prog, NULL);
        rk_shadow_rename((tree_t *) prog);
        rk_wrap_bodies((tree_t *) prog);
        { int pl = 0; rk_place_groups((tree_t *) prog, &pl); }
        rk_nested_sub_names((tree_t *) prog);
        rk_ss_collect(prog, &ss);
        if (ss.n) rk_ss_hoist((tree_t *) prog);
        rk_tv_classes(prog, &cls);
        rk_ss_multi((tree_t *) prog, &ss, &cls, (tree_t *) prog, &mcn);
        rk_ss_rewrite((tree_t *) prog, &ss, 0);
        rk_module_names(prog, &ss);
        rk_type_terms((tree_t *) prog, &ss);
        rk_ns_t subs = { 0 };
        rk_indirect_lit_names((tree_t *) prog);
        rk_sub_names(prog, &subs);
        rk_builtin_refs((tree_t *) prog, &subs);
        rk_ctl_rhs((tree_t *) prog);
        rk_attr_aliases((tree_t *) prog);
        rk_bool_regexes((tree_t *) prog);
        rk_sub_topic((tree_t *) prog, 0);
    }
    rk_var_container_walk((tree_t *) prog, NULL);
    rk_role_specialize((tree_t *) prog);
    rk_hoist_nested_types((tree_t *) prog);
    rk_hoist_nested_multis((tree_t *) prog);
    rk_class_default_tweaks((tree_t *) prog);
    rk_value_forms((tree_t *) prog);
    rk_tail_ifs((tree_t *) prog, 0);
    rk_typed_vars((tree_t *) prog);
    rk_sprintf_names((tree_t *) prog, rk_uses_v6e(prog));
    rk_user_terms((tree_t *) prog);
    { int iseq = 0; rk_infinite_ranges((tree_t *) prog, &iseq, 0); }
    rk_xx_thunks((tree_t *) prog);
    rk_array_single_arg((tree_t *) prog);
    rk_range_values((tree_t *) prog);
    rk_exclusive_ranges((tree_t *) prog);
    rk_bind_refs((tree_t *) prog, 0);
    rk_minmax_fns((tree_t *) prog, prog, rk_user_sub_named(prog, "max"), rk_user_sub_named(prog, "min"));
    { int rseq = 0; rk_rw_loops((tree_t *) prog, &rseq); }
    rk_rw_method_setters((tree_t *) prog);
    rk_attr_bound_refs((tree_t *) prog);
    { rk_rwmap_t rm = { 0 }; rk_rw_subs(prog, &rm); if (rm.n) { rk_rw_calls((tree_t *) prog, &rm, 0); rk_rw_method_deref((tree_t *) prog, &rm); } }
    rk_elem_store_values((tree_t *) prog);
    rk_nonint_ranges((tree_t *) prog);
    rk_nested_elem_sets((tree_t *) prog);
    rk_pkg_scope((tree_t *) prog, NULL);
    rk_pkg_refs((tree_t *) prog);
    rk_class_stubs(prog, (tree_t *) prog);
    { int cbseq = 0; rk_class_body((tree_t *) prog, &cbseq); }
    rk_place_phasers((tree_t *) prog);
    rk_rename_user_main((tree_t *) prog);
    rk_return_outside_routine((tree_t *) prog, 0);
    rk_seq_desugar((tree_t *) prog);
    rk_lazy_stages((tree_t *) prog);
    rk_gather_blockify((tree_t *) prog);
    rk_listops_to_methcalls((tree_t *) prog, 0);
    { int hseq = 0; rk_hyper_meth_walk((tree_t *) prog, &hseq); }
    rk_amp_methcalls(prog, (tree_t *) prog);
    rk_sub_placeholders((tree_t *) prog);
    rk_ph_alias((tree_t *) prog);
    rk_cap_file_scope((tree_t *) prog);
    rk_globalize_file_scope_writes((tree_t *) prog);
    rk_hoist_anon_blocks((tree_t *) prog);
    rk_gather_consumers((tree_t *) prog, NULL, 0);
    rk_sort_cmp_walk((tree_t *) prog, (tree_t *) prog);
    rk_smartmatch_topic_walk((tree_t *) prog);
    rk_smartmatch_code_walk(prog, (tree_t *) prog);
    rk_qualified_sub_refs(prog, (tree_t *) prog);
    rk_trans_flags_walk((tree_t *) prog);
    rk_subst_meth_walk((tree_t *) prog);
    { int sseq = 0; rk_subst_code_walk(prog, (tree_t *) prog, &sseq); }
    { int pseq = 0; rk_parse_actions_walk((tree_t *) prog, &pseq); }
    if (!rk_user_sub_named(prog, "make")) rk_make_walk((tree_t *) prog, "/");
    { int nseq = 0; rk_new_build_walk((tree_t *) prog, (tree_t *) prog, &nseq); }
    raku_register_program(&g_stage2, prog);
    rk_discover_grammars(prog);
    rk_register_classes(prog);
    if (!want_main) rk_eval_host_classes(prog);
    if (reset_multi) g_lower_rk.multi_names.len = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_SUB_DECL) continue;
        const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!nm) continue;
        const char * soh = strchr(nm, '$');
        if (!soh) continue;
        int bl = (int)(soh - nm);
        char base[bl + 1];
        memcpy(base, nm, bl);
        base[bl] = 0;
        rk_multi_name_add(base);
    }
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        const char * cn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cn) continue;
        for (int j = 0; j < d->n; j++) {
            const tree_t * m = d->c[j];
            if (!m || m->t != TT_SUB_DECL) continue;
            const char * mn = (m->n > 0 && m->c[0] && m->c[0]->v.sval) ? m->c[0]->v.sval : NULL;
            if (!mn) continue;
            const char * ms = strchr(mn, '$');
            if (!ms) continue;
            int ml = (int)(ms - mn);
            char mb[fmt_len("%s__%.*s", cn, ml, mn)];
            snprintf(mb, sizeof mb, "%s__%.*s", cn, ml, mn);
            rk_multi_name_add(mb);
        }
    }
    rk_discover_procs(prog);
    {
        extern int rt_binop_overload_armed_by(const char *pn);
        g_stage2.rk_infix_armed = 0;
        for (int i = 0; i < g_stage2.proc_count; i++) if (rt_binop_overload_armed_by(g_stage2.proc_table[i].name)) { g_stage2.rk_infix_armed = 1; break; }
    }
    rk_mark_generators();
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_raku_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            const char * pname = g_stage2.proc_table[pi].name;
            const char * _mu = pname ? strchr(pname, '_') : NULL;
            int is_method = (_mu && _mu != pname && _mu[1] == '_');
            int param_start = is_method ? 0 : 1;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            if (is_method) { stage2_scope_reserve(sc, sc->n + 1); sc->e[sc->n].name = lp_strdup("self"); sc->e[sc->n].slot = sc->n; sc->n++; param_start = 1; }
            for (int k = 0; k < np && (k + param_start) < proc->n; k++) {
                stage2_scope_reserve(sc, sc->n + 1);
                const tree_t *pv = proc->c[k + param_start];
                if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            g_stage2.bbp.table[bb_idx]->nparams = sc->n;
            if (!g_stage2.proc_table[pi].is_generator) { g_stage2.bbp.table[bb_idx]->entry_frame = 1; g_stage2.bbp.table[bb_idx]->smx = 1; }
            g_stage2.proc_table[pi].lex_startup = 1;
            if (sc->n > 0) {
                const char ** _pn = (const char **) ct_zalloc((size_t) sc->n, sizeof(const char *));
                if (_pn) { for (int k = 0; k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
        }
    }
    int has_main = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "main") == 0) { has_main = 1; break; }
    if (!has_main && want_main) {
        IR_graph_t * tg = IR_alloc(8192);
        rcx_t tcx;
        tcx.cond_om = NULL;
        tcx.try_depth = 0;
        tcx.g = tg;
        tcx.try_catch = NULL;
        tcx.loops = NULL;
        tcx.loop_label = NULL;
        tcx.cur_proc = NULL;
        tcx.cur_byref_mask = 0;
        tcx.cur_nparams = 0;
        tcx.cur_proc_name = "main";
        IR_t * succ = IR_node_alloc(tg, IR_SUCCEED);
        IR_t * fail = IR_node_alloc(tg, IR_FAIL);
        IR_t * sentry = succ;
        IR_t * entry = succ;
        tcx.proc_exit = succ;
        int has_rk_MAIN = 0;
        for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "MAIN") == 0) { has_rk_MAIN = 1; break; }
        if (has_rk_MAIN) {
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *)"MAIN";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *)"MAIN";
            ast_push(mc, nmv);
            IR_t * r = NULL;
            IR_t * e = lower_rv(&tcx, mc, sentry, sentry, &r);
            if (e) { entry = e; sentry = e; }
        }
        for (int i = prog->n - 1; i >= 0; i--) {
            const tree_t * s = prog->c[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_SUB_DECL || s->t == TT_CLASS_DECL || s->t == TT_ROLE_DECL || s->t == TT_GRAMMAR_DECL) continue;
            if (s->t == TT_MODULE_DECL) {
                for (int j = s->n - 1; j >= 1; j--) {
                    const tree_t * ch = s->c[j];
                    if (ch && ch->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ch); if (!sub) continue; ch = sub; }
                    if (!ch || ch->t == TT_SUB_DECL || ch->t == TT_REGEX_DECL) continue;
                    IR_t * r = NULL;
                    IR_t * e = lower_rv(&tcx, ch, sentry, sentry, &r);
                    if (e) e = trace_stmt_wrap(&tcx, trace_stmt_line(ch), e, sentry);
                    if (e) { entry = e; sentry = e; }
                }
                continue;
            }
            if ((s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
            IR_t * r = NULL;
            IR_t * e = lower_rv(&tcx, s, sentry, sentry, &r);
            if (e) e = trace_stmt_wrap(&tcx, trace_stmt_line(s), e, sentry);
            if (e) { entry = e; sentry = e; }
        }
        {
            tree_t * ta = ast_node_new(TT_ASSIGN);
            ta->v.ival = 1;
            ast_push(ta, leaf_sval2(TT_VAR, "_"));
            ast_push(ta, ast_node_new(TT_NUL));
            IR_t * tr = NULL;
            IR_t * te = lower_rv(&tcx, ta, sentry, sentry, &tr);
            if (te) { entry = te; sentry = te; }
        }
        tg->entry = trace_call_wrap(&tcx, "main", entry, fail);
        int bb_idx = bb_program_add(&g_stage2.bbp, tg);
        if (bb_idx >= 0) {
            int pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pi].name = lp_strdup("main");
            g_stage2.proc_table[pi].proc = NULL;
            g_stage2.proc_table[pi].entry_pc = -1;
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.proc_table[pi].nparams = 0;
        }
    }
    rk_reclassify_calls();
    rk_file_scope_reads_are_globals();
    rk_unassigned_dynamics_are_globals();
    rk_eval_main_globals();
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.bbp.table[bi]) {
            g_stage2.bbp.table[bi]->home_frame = 1;
            if (!g_stage2.proc_table[pi].is_generator) { g_stage2.bbp.table[bi]->entry_frame = 1; g_stage2.bbp.table[bi]->smx = 1; }
            {
                const tree_t * pr = (const tree_t *) g_stage2.proc_table[pi].proc;
                g_stage2.bbp.table[bi]->block_args = (pr && pr->t == TT_SUB_DECL && !g_stage2.proc_table[pi].is_variadic && !g_stage2.proc_table[pi].named_rest) ? 1 : 0;
            }
        }
    }
    return &g_stage2;
}
