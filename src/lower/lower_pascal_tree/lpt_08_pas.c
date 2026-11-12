#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include "ir/ast.h"
#include "lower/lower_pascal_class.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
static int pas_xr_src(const tree_t *n);
static tree_t *pas_xr_operand(const tree_t *src);
extern long double pas_xr_un_ld(long long op, long double x);
extern long double pas_xr_bin_ld(long long op, long double x, long double y);
int pas_setcons_elems(tree_t *v, tree_t *out) {
    if (!v || v->t != TT_FNC || v->n < 1 || !v->c[0] || !v->c[0]->v.sval) return 0;
    if (!strcmp(v->c[0]->v.sval, "__pas_set")) { for (int i = 1; i < v->n; i++) ast_push(out, v->c[i]); return 1; }
    return !strcmp(v->c[0]->v.sval, "__pas_setuni") && v->n == 3 && pas_setcons_elems(v->c[1], out) && pas_setcons_elems(v->c[2], out);
}
int pas_has_user_call(const tree_t *x) {
    if (!x) return 0;
    if (x->t == TT_FNC && x->n >= 1 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && (pas_is_func(x->c[0]->v.sval) || pas_is_proc(x->c[0]->v.sval))) return 1;
    if (x->t == TT_FNC && x->n >= 1 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && (!strcmp(x->c[0]->v.sval, "__pas_eof_f") || !strcmp(x->c[0]->v.sval, "__pas_eoln_f"))) return 1;
    for (int i = 0; i < x->n; i++) if (pas_has_user_call(x->c[i])) return 1;
    return 0;
}
int pas_is_func(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nfunc; i++) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) return 1;
    return 0;
}
tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_range_check"));
    ast_push(e, val);
    ast_push(e, ilit(lo));
    ast_push(e, ilit(hi));
    int isset = clause && !strcmp(clause, "set");
    const char *cl = isset ? "6.8.2.2" : clause;
    if (g_lower_pas.sem.pas_range_check_on == 2 && cl) ast_push(e, leaf_s(TT_QLIT, cl));
    else if (isset) ast_push(e, leaf_s(TT_QLIT, ""));
    if (isset) ast_push(e, leaf_s(TT_QLIT, "set"));
    return e;
}
static int pas_addr_base_offset(tree_t *e, long long *out) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { *out = pas_hash_stable(e->v.sval); return 1; }
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        long long base;
        if (!pas_addr_base_offset(e->c[0], &base)) return 0;
        const char *rt = pas_with_sel_rtype(e->c[0]);
        if (!rt) return 0;
        long long foff;
        if (!pas_rectype_field_offset(rt, (int)e->c[1]->v.ival, &foff)) return 0;
        *out = base + foff;
        return 1;
    }
    return 0;
}
tree_t *pas_addr_of_proc(tree_t *e) {
    if (!e) return e;
    const char *nm = NULL;
    if (e->t == TT_VAR && e->v.sval && (pas_is_proc(e->v.sval) || pas_is_func(e->v.sval))) nm = e->v.sval;
    else if (e->t == TT_FNC && e->n == 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_func(e->c[0]->v.sval)) nm = e->c[0]->v.sval;
    if (nm) return ilit(pas_hash_stable(nm));
    { long long addr; if (pas_addr_base_offset(e, &addr)) return ilit(addr); }
    return e;
}
void pas_proc_add(const char *name) {
    if (name) {
        if (g_lower_pas.sem.pas_nproc >= g_lower_pas.sem.pas_procs_cap) g_lower_pas.sem.pas_procs =
            pas_pool_grow(g_lower_pas.sem.pas_procs, &g_lower_pas.sem.pas_procs_cap, sizeof *g_lower_pas.sem.pas_procs, 16);
        g_lower_pas.sem.pas_procs[g_lower_pas.sem.pas_nproc].name = ct_strdup(name);
        g_lower_pas.sem.pas_procs[g_lower_pas.sem.pas_nproc].nvp = 0;
        g_lower_pas.sem.pas_procs[g_lower_pas.sem.pas_nproc].nrp = 0;
        g_lower_pas.sem.pas_procs[g_lower_pas.sem.pas_nproc].nsp = 0;
        g_lower_pas.sem.pas_nproc++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_vparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isvar = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_SUCCEED) { isvar = 1; break; }
        out[nv++] = isvar;
    }
    return nv;
}
static int pas_realparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isreal = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_FLIT) { isreal = 1; break; }
        out[nv++] = isreal;
    }
    return nv;
}
static int pas_strparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isstr = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_QLIT) { isstr = 1; break; }
        out[nv++] = isstr;
    }
    return nv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_proc_vparams(const char *name, PNodeList *params) {
    if (!name || !params) return;
    for (int i = g_lower_pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) {
        g_lower_pas.sem.pas_procs[i].nvp = pas_vparam_scan(params, g_lower_pas.sem.pas_procs[i].vp);
        g_lower_pas.sem.pas_procs[i].nrp = pas_realparam_scan(params, g_lower_pas.sem.pas_procs[i].rp);
        g_lower_pas.sem.pas_procs[i].nsp = pas_strparam_scan(params, g_lower_pas.sem.pas_procs[i].sp);
        return;
    }
    for (int i = g_lower_pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) {
        g_lower_pas.sem.pas_funcs[i].nvp = pas_vparam_scan(params, g_lower_pas.sem.pas_funcs[i].vp);
        g_lower_pas.sem.pas_funcs[i].nrp = pas_realparam_scan(params, g_lower_pas.sem.pas_funcs[i].rp);
        g_lower_pas.sem.pas_funcs[i].nsp = pas_strparam_scan(params, g_lower_pas.sem.pas_funcs[i].sp);
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_required_needs_params(const char *name) {
    const char *nm[] = { "get", "put", "reset", "rewrite", "new", "dispose", "pack", "unpack", "read", "write" };
    const char *cl[] = { "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.3", "6.6.5.3", "6.6.5.4", "6.6.5.4", "6.9.1", "6.9.3" };
    for (size_t i = 0; name && i < sizeof nm / sizeof nm[0]; i++) if (!strcmp(name, nm[i])) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: the required procedure '%s' is activated with no actual-parameters, and its activation takes at least one\n", cl[i], name);
        g_lower_pas.sem.pas_iso_errors++;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_call_arity(const char *name, PNodeList *args) {
    const char *req[] = { "abs", "arctan", "chr", "cos", "dispose", "eof", "eoln", "exp", "get", "ln", "new", "odd", "ord", "pack", "page", "pred", "put", "read", "readln", "reset", "rewrite",
        "round", "sin", "sqr", "sqrt", "succ", "trunc", "unpack", "write", "writeln" };
    if (!name || (args && args->count % 2)) return;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return;
    for (int i = 1; args && i < args->count; i += 2) if (!args->items[i] || args->items[i]->t != TT_ILIT || args->items[i]->v.ival != -1) return;
    int argc = args ? args->count / 2 : 0, nent = 0, formals = -1;
    const char *clause = NULL;
    for (int i = 0; i < g_lower_pas.sem.pas_nproc; i++) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) {
        if (g_lower_pas.sem.pas_procs[i].nvp >= 16 || g_lower_pas.sem.pas_procs[i].nvp == argc) return;
        nent++;
        formals = g_lower_pas.sem.pas_procs[i].nvp;
        clause = "6.8.2.3";
    }
    for (int i = 0; i < g_lower_pas.sem.pas_nfunc; i++) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) {
        if (g_lower_pas.sem.pas_funcs[i].nvp >= 16 || g_lower_pas.sem.pas_funcs[i].nvp == argc) return;
        nent++;
        formals = g_lower_pas.sem.pas_funcs[i].nvp;
        clause = "6.7.3";
    }
    if (!nent) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with %d actual-parameter(s), and the number of actual-parameters shall equal the number of its formal-parameters (%d)\n", clause,
        name, argc, formals);
    g_lower_pas.sem.pas_iso_errors++;
}
int pas_proc_param_is_var(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower_pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) return (idx < g_lower_pas.sem.pas_procs[i].nvp)
        ? g_lower_pas.sem.pas_procs[i].vp[idx] : 0;
    for (int i = g_lower_pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) return (idx < g_lower_pas.sem.pas_funcs[i].nvp)
        ? g_lower_pas.sem.pas_funcs[i].vp[idx] : 0;
    return 0;
}
int pas_proc_param_is_string(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower_pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) return (idx < g_lower_pas.sem.pas_procs[i].nsp)
        ? g_lower_pas.sem.pas_procs[i].sp[idx] : 0;
    for (int i = g_lower_pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) return (idx < g_lower_pas.sem.pas_funcs[i].nsp)
        ? g_lower_pas.sem.pas_funcs[i].sp[idx] : 0;
    return 0;
}
int pas_proc_param_is_real(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower_pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) return (idx < g_lower_pas.sem.pas_procs[i].nrp)
        ? g_lower_pas.sem.pas_procs[i].rp[idx] : 0;
    for (int i = g_lower_pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower_pas.sem.pas_funcs[i].name && !strcmp(g_lower_pas.sem.pas_funcs[i].name, name)) return (idx < g_lower_pas.sem.pas_funcs[i].nrp)
        ? g_lower_pas.sem.pas_funcs[i].rp[idx] : 0;
    return 0;
}
int pas_is_proc(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nproc; i++) if (g_lower_pas.sem.pas_procs[i].name && !strcmp(g_lower_pas.sem.pas_procs[i].name, name)) return 1;
    return 0;
}
static int pas_is_currencyvar(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_lower_pas.sem.pas_nscalarvartype - 1; name && i >= 0; i--)
        if (g_lower_pas.sem.pas_scalarvartype[i].vname && g_lower_pas.sem.pas_scalarvartype[i].uid == u && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, name))
        return !strcmp(g_lower_pas.sem.pas_scalarvartype[i].tname, "currency");
    return 0;
}
int pas_is_currencyexpr(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_VAR) return e->v.sval && pas_is_currencyvar(e->v.sval);
    if (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV || e->t == TT_MNS) { for (int i = 0; i < e->n; i++) if (pas_is_currencyexpr(e->c[i])) return 1; }
    return 0;
}
void pas_ordinal_fn_arg(const char *name, PNodeList *args) {
    if (!name || !args || args->count < 1 || pas_is_func(name) || pas_is_proc(name)) return;
    int is_chr = !strcmp(name, "chr");
    if (!is_chr && strcmp(name, "ord") && strcmp(name, "succ") && strcmp(name, "pred")) return;
    tree_t *a = args->items[0];
    if (!a || !(a->t == TT_FLIT || (a->t == TT_VAR && a->v.sval && pas_var_every_decl_real(a->v.sval)))) return;
    if (a->t == TT_FLIT)
        fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the real constant %g, and %s requires an expression of %s\n", name, a->v.dval, name,
        is_chr ? "integer-type" : "an ordinal-type");
    else fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the variable '%s' of type real, and %s requires an expression of %s\n", name, a->v.sval, name,
        is_chr ? "integer-type" : "an ordinal-type");
    g_lower_pas.sem.pas_iso_errors++;
}
void pas_string_assign_check(const char *vn, tree_t *rhs) {
    if (g_lower_pas.sem.pas_seen_mode_directive || !vn || !pas_is_chararr(vn)) return;
    char k = pas_expr_lit_class(rhs);
    int i = pas_chararr_row(vn);
    if (i < 0 || (k != 'c' && k != 's')) return;
    if (k == 'c') {
        fprintf(stderr,
            "pascal: ISO 7185 6.8.2.2 violation: the variable '%s' is an array and is given a char value, which is not assignment-compatible with it; a character-string of one character is of the ch"
            "ar-type (6.1.7, 6.4.6)\n", vn);
        g_lower_pas.sem.pas_iso_errors++;
        return;
    }
    long long len = (long long)strlen(rhs->v.sval);
    if (g_lower_pas.sem.pas_chararrs[i].nostr) {
        fprintf(stderr,
            "pascal: ISO 7185 6.4.3.2 violation: the variable '%s' is given a character-string of %lld characters but is not of a string-type, which is a packed array [1..n] of char with n > 1; a ch"
            "aracter-string is assignment-compatible only with a string-type (6.4.6, 6.8.2.2)\n", vn, len);
        g_lower_pas.sem.pas_iso_errors++;
    } else if (g_lower_pas.sem.pas_chararrs[i].n >= 0 && len != g_lower_pas.sem.pas_chararrs[i].n) {
        fprintf(stderr,
            "pascal: ISO 7185 6.4.6 violation: the variable '%s' is a string-type of %lld components and is given a character-string of %lld characters; assignment-compatible string-types have the s"
            "ame number of components (6.8.2.2)\n", vn, g_lower_pas.sem.pas_chararrs[i].n, len);
        g_lower_pas.sem.pas_iso_errors++;
    }
}
static int pas_boolfam_width(const char *tn0) {
    const char *tn = tn0;
    int guard = 0;
    while (tn) {
        if (!strcmp(tn, "bytebool")) return 1;
        if (!strcmp(tn, "wordbool")) return 2;
        if (!strcmp(tn, "longbool")) return 4;
        if (!strcmp(tn, "qwordbool")) return 8;
        const char *al = pas_typealias_get(tn);
        if (!al || !strcmp(al, tn) || guard++ >= 8) break;
        tn = al;
    }
    return 0;
}
int pas_var_is_boolfam(const char *name) { return name ? pas_boolfam_width(pas_scalarvartype_get(name)) > 0 : 0; }
int pas_pchar_castname(tree_t *e) {
    if (!e || e->t != TT_FNC || e->n != 2 || !e->c[0] || !e->c[0]->v.sval) return 0;
    const char *f = e->c[0]->v.sval;
    return !strcmp(f, "pchar") || !strcmp(f, "pwidechar") || !strcmp(f, "pansichar");
}
tree_t *pas_pchar_assign(tree_t *sel, tree_t *rhs) {
    if (!g_lower_pas.sem.pas_ptrcell || !sel || !rhs) return NULL;
    const pty_t *lt = pty_of_low(sel), *rt = pty_of_low(rhs);
    if (lt && pty_is_pchar(lt)) { tree_t *_se = pas_pchar_of_strelem(rhs); if (_se) return mk_assign(sel, _se); }
    if (lt && pty_is_pchar(lt) && rt && (rt->k == PTK_STRING || rt->k == PTK_ARRAY)) return mk_assign(sel, mk_fnc1("__pas_pchar_of", rhs));
    if (lt && lt->k == PTK_STRING && rt && pty_is_pchar(rt)) return mk_assign(sel, mk_fnc1("__pas_pstr", rhs));
    return NULL;
}
const char *pas_ptrcast_target(const tree_t *n) {
    const char *t = (IS(n, TT_FNC) && n->n == 2 && IS(n->c[0], TT_VAR) && n->c[0]->v.sval) ? pas_alias_resolve(n->c[0]->v.sval) : NULL;
    return (t && !pas_is_func(t) && !pas_is_proc(t)) ? pas_ptrtype_target(t) : NULL;
}
pval E_argument(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_IDX) && n->n == 2 && IS(n->c[1], TT_SUBRANGE) && n->c[1]->n == 2) {
        {
            PNodeList *_al = pnl_new();
            pnl_push(_al, mk_fnc3("arr_slice", E_expression(n->c[0]).node, E_expression(n->c[1]->c[0]).node, E_expression(n->c[1]->c[1]).node));
            pnl_push(_al, ilit(-1));
            out.list = _al;
        }
        return out;
    }
    if (IS(n, TT_FMT) && n->n >= 2 && pas_xr_src(n->c[0])) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_xr_str"));
        ast_push(e, ilit(0));
        ast_push(e, pas_xr_operand(n->c[0]));
        ast_push(e, E_expression(n->c[1]).node);
        ast_push(e, n->n == 3 ? E_expression(n->c[2]).node : ilit(-1));
        { PNodeList *_al = pnl_new(); pnl_push(_al, e); pnl_push(_al, ilit(-1)); out.list = _al; }
        return out;
    }
    if (IS(n, TT_FMT) && n->n == 2) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        if (v[3].node && ((v[3].node->t == TT_ILIT && v[3].node->v.ival < 0) || (v[3].node->t == TT_MNS && v[3].node->n == 1 && v[3].node->c[0] && v[3].node->c[0]->t == TT_ILIT))) v[3].node = ilit(0);
        { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, v[3].node); out.list = _al; }
        return out;
    }
    if (IS(n, TT_FMT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        v[5] = E_expression(n->c[2]);
        if (v[3].node && ((v[3].node->t == TT_ILIT && v[3].node->v.ival < 0) || (v[3].node->t == TT_MNS && v[3].node->n == 1 && v[3].node->c[0] && v[3].node->c[0]->t == TT_ILIT))) v[3].node = ilit(0);
        { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, ilit(-3)); pnl_push(_al, v[3].node); pnl_push(_al, v[5].node); out.list = _al; }
        return out;
    }
    if (n != NULL) { v[1] = E_expression(n); { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, ilit(-1)); out.list = _al; } return out; }
    elab_bad("argument", n);
    return out;
}
static int pas_xr_src(const tree_t *n) { return n && pty_real_rank(pty_of_src(n)) == 2; }
int pas_xr_low(tree_t *e) { return e && pty_real_rank(pty_of_low(e)) == 2; }
static int pas_xr_numeric(const tree_t *n) { const pty_t *t = n ? pty_of_src(n) : NULL; return t && (t->k == PTK_REAL || t->k == PTK_INT); }
static tree_t *pas_xr_imgnode(long double v) {
    union { long double ld; struct { uint64_t m; uint16_t se; } f; } u;
    memset(&u, 0, sizeof u);
    u.ld = v;
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_xr_cvt"));
    ast_push(e, ilit(10));
    ast_push(e, ilit((long long)u.f.m));
    ast_push(e, ilit((long long)u.f.se));
    return e;
}
static long double pas_xr_litval(const char *text, double dv) { return text && strtod(text, NULL) == dv ? strtold(text, NULL) : (long double)dv; }
static tree_t *pas_xr_litnode(const char *text, double dv) { return pas_xr_imgnode(pas_xr_litval(text, dv)); }
tree_t *pas_xr_wrap(const tree_t *src, tree_t *e) {
    if (IS(src, TT_FLIT)) return pas_xr_litnode(src->n > 0 && IS(src->c[0], TT_QLIT) ? src->c[0]->v.sval : NULL, src->v.dval);
    if (IS(src, TT_VAR) && src->v.sval && !strcasecmp(src->v.sval, "pi") && pas_xr_src(src) && !pas_is_func("pi")) return mk_fnc2("__pas_xr_un", ilit(19), ilit(0));
    return pas_xr_src(src) ? e : pas_xr_cvtn(2, e);
}
static tree_t *pas_xr_operand(const tree_t *src) { return pas_xr_wrap(src, IS(src, TT_FLIT) ? NULL : E_expression(src).node); }
static void pas_xrf_ch(pas_xrf_t *f, char ch) { if (f->n + 1 >= f->cap) { f->cap = f->cap ? f->cap * 2 : 64; f->c = ct_grow(f->c, (size_t)f->cap); } f->c[f->n++] = ch; }
static void pas_xrf_push(pas_xrf_t *f) { if (++f->sp > f->depth) f->depth = f->sp; }
static int pas_xr_pi(const tree_t *n) { return IS(n, TT_VAR) && n->v.sval && !strcasecmp(n->v.sval, "pi") && pas_xr_src(n) && !pas_scope_uid("pi") && !pas_scalarvartype_get("pi"); }
static int pas_xr_fn(const tree_t *n, int *bin, int *op) {
    static const struct {
        const char *nm;
        int op;
    } U[] = { {"sin", 4}, {"cos", 5}, {"tan", 6}, {"arctan", 7}, {"ln", 8}, {"exp", 9}, {"frac", 10}, {"int", 11}, {"arcsin", 12}, {"arccos", 13}, {"sinh", 14}, {"cosh", 15}, {"tanh", 16}, {"log10",
        17}, {"log2", 18}, {"degtorad", 20}, {"radtodeg", 21}, {"sqrt", 3}, {"abs", 1}, {"sqr", 2}, {0, 0} };
    static const struct { const char *nm; int op; } B[] = { {"arctan2", 4}, {"power", 5}, {"hypot", 6}, {"logn", 7}, {0, 0} };
    const char *nm = IS(n, TT_FNC) && n->n >= 2 && IS(n->c[0], TT_VAR) ? n->c[0]->v.sval : NULL;
    if (!nm || pas_is_func(nm) || pas_is_proc(nm) || !pas_xr_src(n)) return 0;
    if (n->n == 2) for (int i = 0; U[i].nm; i++) if (!strcmp(nm, U[i].nm)) { *bin = 0; *op = U[i].op; return 1; }
    if (n->n == 3) for (int i = 0; B[i].nm; i++) if (!strcmp(nm, B[i].nm)) { *bin = 1; *op = B[i].op; return 1; }
    return 0;
}
static int pas_xr_cval(const tree_t *n, long double *v) {
    tree_e k = n->t;
    long double a = 0, b = 0;
    int bin = 0, op = 0;
    if (IS(n, TT_ILIT)) { *v = (long double)n->v.ival; return 1; }
    if (IS(n, TT_FLIT)) { *v = pas_xr_litval(n->n > 0 && IS(n->c[0], TT_QLIT) ? n->c[0]->v.sval : NULL, n->v.dval); return 1; }
    if (pas_xr_pi(n)) { *v = pas_xr_un_ld(19, 0.0L); return 1; }
    if (!pas_xr_src(n)) return 0;
    if ((k == TT_ADD || k == TT_SUB || k == TT_MUL || k == TT_DIV) && n->n == 2) {
        if (!pas_xr_cval(n->c[0], &a) || !pas_xr_cval(n->c[1], &b)) return 0;
        *v = k == TT_ADD ? a + b : k == TT_SUB ? a - b : k == TT_MUL ? a * b : a / b;
        return 1;
    }
    if (k == TT_MNS && n->n == 1) { if (!pas_xr_cval(n->c[0], &a)) return 0; *v = -a; return 1; }
    if (pas_xr_fn(n, &bin, &op)) {
        if (!pas_xr_cval(n->c[1], &a) || (bin && !pas_xr_cval(n->c[2], &b))) return 0;
        if (!bin && ((op == 3 && a < 0) || (op == 8 && !(a > 0)))) return 0;
        *v = bin ? pas_xr_bin_ld(op, a, b) : pas_xr_un_ld(op, a);
        return 1;
    }
    return 0;
}
static void pas_xrf(const tree_t *n, pas_xrf_t *f) {
    tree_e k = n->t;
    int bin = 0, op = 0;
    long double cv = 0;
    if (IS(n, TT_FLIT) || (!IS(n, TT_ILIT) && !IS(n, TT_VAR) && pas_xr_cval(n, &cv))) {
        tree_t *lit = IS(n, TT_FLIT) ? pas_xr_litnode(n->n > 0 && IS(n->c[0], TT_QLIT) ? n->c[0]->v.sval : NULL, n->v.dval) : pas_xr_imgnode(cv);
        pnl_push(f->lv, lit->c[2]);
        pnl_push(f->lv, lit->c[3]);
        pas_xrf_ch(f, 'X');
        pas_xrf_push(f);
    } else if ((k == TT_ADD || k == TT_SUB || k == TT_MUL || k == TT_DIV) && n->n == 2 && pas_xr_src(n)) {
        pas_xrf(n->c[0], f);
        pas_xrf(n->c[1], f);
        pas_xrf_ch(f, k == TT_ADD ? '+' : k == TT_SUB ? '-' : k == TT_MUL ? '*' : '/');
        f->sp--;
    } else if (k == TT_MNS && n->n == 1 && pas_xr_src(n)) {
        pas_xrf(n->c[0], f);
        pas_xrf_ch(f, 'n');
    } else if (k == TT_PLS && n->n == 1 && pas_xr_src(n)) {
        pas_xrf(n->c[0], f);
    } else if (pas_xr_pi(n)) {
        pas_xrf_ch(f, 'P');
        pas_xrf_push(f);
    } else if (pas_xr_fn(n, &bin, &op)) {
        pas_xrf(n->c[1], f);
        if (bin) pas_xrf(n->c[2], f);
        pas_xrf_ch(f, bin ? 'b' : 'u');
        pas_xrf_ch(f, (char)('A' + op));
        if (bin) f->sp--;
    } else {
        pnl_push(f->lv, E_expression(n).node);
        pas_xrf_ch(f, 'L');
        pas_xrf_push(f);
    }
}
static tree_t *pas_xrf_build_t(const tree_t *n, char tail) {
    pas_xrf_t f;
    memset(&f, 0, sizeof f);
    f.lv = pnl_new();
    pas_xrf(n, &f);
    if (tail) pas_xrf_ch(&f, tail);
    char *code = ct_alloc((size_t)f.n + 16);
    int hl = sprintf(code, "%d;", f.depth);
    memcpy(code + hl, f.c, (size_t)f.n);
    code[hl + f.n] = '\0';
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_xr_ev"));
    ast_push(e, leaf_s(TT_QLIT, code));
    for (int i = 0; i < f.lv->count; i++) ast_push(e, f.lv->items[i]);
    return e;
}
static tree_t *pas_xrf_build(const tree_t *n) { return pas_xrf_build_t(n, 0); }
int pas_xr_elab(const tree_t *n, pval *out) {
    tree_e k = n->t;
    if ((((k == TT_ADD || k == TT_SUB || k == TT_MUL || k == TT_DIV) && n->n == 2) || ((k == TT_MNS || k == TT_PLS) && n->n == 1)) && pas_xr_src(n)) { out->node = pas_xrf_build(n); return 1; }
    if (pas_xr_pi(n)) { out->node = pas_xrf_build(n); return 1; }
    if ((k == TT_LT || k == TT_LE || k == TT_GT || k == TT_GE || k == TT_EQ || k == TT_NE) && n->n == 2 && (pas_xr_src(n->c[0]) || pas_xr_src(n->c[1])) && pas_xr_numeric(n->c[0]) &&
        pas_xr_numeric(n->c[1])) {
        tree_t *a = pas_xr_operand(n->c[0]), *b = pas_xr_operand(n->c[1]);
        out->node = bin(k, a, b);
        return 1;
    }
    return 0;
}
int pas_xr_call(const tree_t *n, pval *out) {
    int bin = 0, op = 0;
    const char *nm = IS(n, TT_FNC) && n->n >= 2 && IS(n->c[0], TT_VAR) ? n->c[0]->v.sval : NULL;
    if (!nm || pas_is_func(nm) || pas_is_proc(nm)) return 0;
    if (n->n == 2 && (!strcmp(nm, "trunc") || !strcmp(nm, "round")) && pas_xr_src(n->c[1])) { out->node = pas_xrf_build_t(n->c[1], nm[0] == 't' ? 't' : 'r'); return 1; }
    if (pas_xr_fn(n, &bin, &op)) { out->node = pas_xrf_build(n); return 1; }
    return 0;
}
int pas_routine_named(const tree_t *t, const char *nm) {
    if (!t || !nm) return 0;
    if ((IS(t, TT_PROCEDURE) || IS(t, TT_FUNCTION)) && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, nm)) return 1;
    for (int i = 0; i < t->n; i++) if (pas_routine_named(t->c[i], nm)) return 1;
    return 0;
}
int pas_takes_address(const tree_t *root, const tree_t *t) {
    if (!t) return 0;
    if (IS(t, TT_ADDR) && t->n == 1 && t->c[0] && !(t->c[0]->t == TT_VAR && t->c[0]->v.sval && pas_routine_named(root, t->c[0]->v.sval))) return 1;
    if (IS(t, TT_FNC) && t->n >= 2 && t->c[0] && t->c[0]->v.sval && !strcasecmp(t->c[0]->v.sval, "addr")) return 1;
    if (IS(t, TT_VAR) && t->v.sval && (!strcasecmp(t->v.sval, "pchar") || !strcasecmp(t->v.sval, "pansichar") || !strcasecmp(t->v.sval, "pwidechar"))) return 1;
    for (int i = 0; i < t->n; i++) if (pas_takes_address(root, t->c[i])) return 1;
    return 0;
}
int pas_user_ioflow(const tree_t *t) {
    if (!t) return 0;
    if (IS(t, TT_PART) && t->v.sval && !strcmp(t->v.sval, "units")) return 0;
    if (IS(t, TT_TRY)) return 1;
    if (IS(t, TT_VAR) && t->v.sval && !strcasecmp(t->v.sval, "ioresult")) return 1;
    if (t->t == TT_ATTR && t->v.sval && !strcmp(t->v.sval, ":pragma") && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "iocheck 0")) return 1;
    for (int i = 0; i < t->n; i++) if (pas_user_ioflow(t->c[i])) return 1;
    return 0;
}
int pas_has_unit(const tree_t *prog, const char *name) {
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t *f = prog->c[i];
        if (!IS(f, TT_PART) || !f->v.sval || strcmp(f->v.sval, "units")) continue;
        for (int k = 0; k < f->n; k++) if (f->c[k] && f->c[k]->n > 0 && f->c[k]->c[0] && f->c[k]->c[0]->v.sval && !strcasecmp(f->c[k]->c[0]->v.sval, name)) return 1;
    }
    return 0;
}
