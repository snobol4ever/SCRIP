#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include <stdlib.h>
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_pascal_tree.h"
#include "lower/lower_pascal_class.h"
#include "lower/lower_pascal_generics.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_09_sh.h"
#include "lower/lower_pascal_tree/lpt_10_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static int pas_dyn_elem_is_string(tree_t *base);
static long long pas_wrap_mask_of_type(const char *t);
static int pas_is_strelem_arr(const char *name);
static tree_t *mk_rec_init_type_d(const char *rt, int depth);
static pval E_program(const tree_t *n);
static pval E_set_member(const tree_t *n);
const pty_t *pas_art(const char *a) {
    const pty_t *t = a ? pty_name_type(a) : NULL;
    if (!t || t->k != PTK_ARRAY) return NULL;
    while (t && t->k == PTK_ARRAY) t = t->base;
    return t && t->k == PTK_RECORD && t->lay ? t : NULL;
}
int pas_arf(const char *a, long idx, struct pty_fld *f) { const pty_t *t = pas_art(a); return t && idx >= 0 && pty_rec_fld(t, (int)idx, f); }
tree_t *ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
tree_t *bin(tree_e k, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
tree_t *pas_trace_wrap_proc(const char *pname, PNodeList *params, tree_t *body, int isfunc) {
    if (!pas_trace_enabled() || !body) return body;
    tree_t *enter_call = ast_node_new(TT_FNC);
    ast_push(enter_call, leaf_s(TT_VAR, "__trace_call"));
    ast_push(enter_call, leaf_s(TT_QLIT, pname));
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *id = params->items[i];
        if (id && id->v.sval) { ast_push(enter_call, leaf_s(TT_QLIT, id->v.sval)); ast_push(enter_call, pas_trace_wrap_value(leaf_s(TT_VAR, id->v.sval))); }
    }
    tree_t *exit_call = ast_node_new(TT_FNC);
    ast_push(exit_call, leaf_s(TT_VAR, "__trace_return"));
    ast_push(exit_call, leaf_s(TT_QLIT, pname));
    if (isfunc) ast_push(exit_call, pas_trace_wrap_value(leaf_s(TT_VAR, pname)));
    tree_t *nb = ast_node_new(TT_PROGRAM);
    ast_push(nb, enter_call);
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *id = params->items[i];
        if (id && id->v.sval) {
            tree_t *bind = ast_node_new(TT_FNC);
            ast_push(bind, leaf_s(TT_VAR, "__trace_value"));
            ast_push(bind, leaf_s(TT_QLIT, id->v.sval));
            ast_push(bind, pas_trace_wrap_value(leaf_s(TT_VAR, id->v.sval)));
            ast_push(nb, bind);
        }
    }
    for (int i = 0; i < body->n; i++) ast_push(nb, body->c[i]);
    ast_push(nb, exit_call);
    return nb;
}
tree_t *pas_trace_wrap_value(tree_t *val) {
    if (!val) return val;
    const char *_enm = (val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
    if (!_enm && val->t == TT_VAR && val->v.sval) _enm = pas_trace_enum_names_of_var(val->v.sval);
    if (!_enm && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[1]) {
        const char *_et = NULL, *_rt = (val->c[1]->t == TT_ILIT) ? pas_with_sel_rtype(val->c[0]) : NULL;
        if (_rt) _et = pas_rectype_field_enum_by_index(_rt, (long)val->c[1]->v.ival);
        if (!_et && val->c[0]->t == TT_VAR && val->c[0]->v.sval) _et = pas_enumarr_of(val->c[0]);
        if (_et) _enm = pas_enumnames_by_idx(pas_enumnames_idx(_et));
    }
    if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); return _w; }
    if (pas_is_charexpr(val)) return mk_chr_wrap(val);
    if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) {
        tree_t *_w = ast_node_new(TT_FNC);
        ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
        ast_push(_w, val->t == TT_IDX ? val : bin(TT_NE, val, ilit(0)));
        ast_push(_w, leaf_s(TT_QLIT, "false,true"));
        return _w;
    }
    if (val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) return pas_alpha_wrap(val);
    if (pas_ca_is_read(val)) return pas_alpha_wrap(val);
    if (val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) {
        tree_t *_w = ast_node_new(TT_FNC);
        ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(_w, val);
        ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval)));
        return _w;
    }
    return val;
}
tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); return e; }
tree_t *mk_fnc3(const char *fn, tree_t *a, tree_t *b, tree_t *c) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); ast_push(e, c); return e; }
static int pas_is_u64_typename(const char *t) { return t && (!strcmp(t, "qword") || !strcmp(t, "uint64") || !strcmp(t, "nativeuint") || !strcmp(t, "sizeuint") || !strcmp(t, "ptruint")); }
int pas_dyn_depth(tree_t *e, const char **elem) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { int d = pas_array_isdyn(e->v.sval); const char *t = pas_scalarvartype_get(e->v.sval); if (d > 0 && elem) *elem = t; return d > 0 ? d : 0; }
    if (e->t != TT_IDX || e->n != 2) return 0;
    if (e->c[1] && e->c[1]->t == TT_ILIT) {
        const char *rt = pas_with_sel_rtype(e->c[0]), *tn = rt ? pas_rectype_field_typename(rt, e->c[1]->v.ival) : NULL;
        if (tn && !strncmp(tn, "$dyn", 4)) { if (elem) *elem = strchr(tn + 4, '$') + 1; return atoi(tn + 4); }
        if (tn && pas_arrtype_isdyn(tn)) { if (elem) *elem = tn; return pas_arrtype_isdyn(tn); }
    }
    int d = pas_dyn_depth(e->c[0], elem);
    return d > 1 ? d - 1 : 0;
}
static int pas_dyn_elem_is_string(tree_t *base) {
    const char *el = NULL;
    return base && base->t == TT_IDX && pas_dyn_depth(base, &el) == 1 && el && (pas_string_type_kind(el) || !strcmp(el, "string"));
}
tree_t *lpt_mk_call(const char *name, PNodeList *args) {
    tree_t *c = mk_call_raw(name, args);
    if (!g_lower_pas.sem.pas_ioflow || g_lower_pas.sem.pas_ioquiet || !name || !c || pas_is_proc(name) || pas_is_func(name)) return c;
    static const char *const AFTER[] = { "reset", "rewrite", "append", "close", "erase", "rename", "seek", "truncate", "flush", "blockread", "blockwrite", 0 };
    static const char *const AFTERW[] = { "write", "writeln", "read", "readln", 0 };
    static const char *const BEFORE[] = { 0 };
    int after = 0, before = 0;
    for (int i = 0; AFTER[i]; i++) if (!strcmp(name, AFTER[i])) after = 1;
    if (g_lower_pas.sem.pas_iofull) for (int i = 0; AFTERW[i]; i++) if (!strcmp(name, AFTERW[i])) after = 1;
    for (int i = 0; BEFORE[i]; i++) if (!strcmp(name, BEFORE[i])) before = 1;
    if (!after && !before) return c;
    tree_t *chk = mk_call_raw(pas_is_proc("__sh_iochecke") ? "__sh_iochecke" : "__sh_iocheck", NULL);
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, after ? c : chk);
    ast_push(q, after ? chk : c);
    return q;
}
tree_t *mk_set_ctor(PNodeList *elems) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_set"));
    if (elems) for (int i = 0; i < elems->count; i++) ast_push(e, elems->items[i]);
    return e;
}
void emit_proc(PNodeList *procs, tree_t *proc) {
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
int pas_ptr_typed(const tree_t *low) { const pty_t *t = g_lower_pas.sem.pas_ptrcell && low ? pty_of_low(low) : NULL; return t && t->k == PTK_POINTER; }
static int pas_ptr_or_nil(const tree_t *low) { const pty_t *t = g_lower_pas.sem.pas_ptrcell && low ? pty_of_low(low) : NULL; return t && (t->k == PTK_POINTER || t->k == PTK_NIL); }
tree_t *pas_pchar_of_strelem(tree_t *e) {
    if (!e || e->t != TT_IDX || e->n != 2 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval || !pas_var_string_kind(e->c[0]->v.sval)) return NULL;
    tree_t *cell = pas_addr_of_cell(e->c[0]);
    if (cell) return mk_fnc2("__pas_pstrview", cell, bin(TT_SUB, e->c[1], ilit(1)));
    return mk_fnc2("__pas_ptr_inc", mk_fnc1("__pas_pchar_of", e->c[0]), bin(TT_SUB, e->c[1], ilit(1)));
}
int pas_pchar_typed(const tree_t *low) { const pty_t *t = g_lower_pas.sem.pas_ptrcell && low ? pty_of_low(low) : NULL; return t && pty_is_pchar(t); }
tree_t *pas_ptr_arith(tree_e tt, tree_t *a, tree_t *b) {
    if (pas_ptr_typed(a) && pas_ptr_typed(b)) return tt == TT_SUB ? mk_fnc2("__pas_ptr_diff", a, b) : NULL;
    if (!pas_ptr_typed(a) || (tt == TT_SUB && pas_ptr_typed(b))) return NULL;
    return mk_fnc2("__pas_ptr_inc", a, tt == TT_ADD ? b : bin(TT_SUB, ilit(0), b));
}
tree_t *pas_addr_of_cell(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[1] && !(e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_var_string_kind(e->c[0]->v.sval)) && e->c[0]->t != TT_QLIT) {
        long long d2[4];
        if (pas_row_designator(e, d2)) return mk_fnc2("__pas_addr", e->c[0], bin(TT_MUL, pas_dense_sub(e->c[1], d2[0]), ilit(d2[3] - d2[2] + 1)));
        return mk_fnc2("__pas_addr", e->c[0], e->c[1]);
    }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return e->c[1];
    if (e->t == TT_VAR && e->v.sval) { int k = pty_name_kind(e->v.sval); if (k == 'v' || k == 'p' || k == 'r' || k == 'R') return mk_fnc1("__pas_addr_var", e); }
    return NULL;
}
tree_t *pas_addr_of(tree_t *e) { tree_t *c = pas_addr_of_cell(e); if (!c && g_lower_pas.sem.pas_ptrcell) c = pas_pchar_of_strelem(e); return c ? c : pas_addr_of_proc(e); }
tree_t *pas_ptr_ord(tree_e tt, tree_t *a, tree_t *b) { return pas_ptr_typed(a) && pas_ptr_typed(b) ? bin(tt, mk_fnc2("__pas_ptr_diff", a, b), ilit(0)) : NULL; }
tree_t *pas_ptr_rel(tree_e tt, tree_t *a, tree_t *b) {
    if (!(pas_ptr_typed(a) || pas_ptr_typed(b)) || !pas_ptr_or_nil(a) || !pas_ptr_or_nil(b)) return NULL;
    return bin(tt == TT_EQ ? TT_NE : TT_EQ, mk_fnc2("__pas_ptr_eq", a, b), ilit(0));
}
long long pas_array_ncols(const char *name) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    return t && t->k == PTK_ARRAY && !t->dyn && !t->open && t->ix && t->ix[0] && t->ix[1] ? t->ix[1]->hi - t->ix[1]->lo + 1 : -1;
}
int pas_array_isdyn(const char *name) { int d = name ? pty_name_dyn(name) : 0; return d > 0 ? d : 0; }
int pas_is_dynexpr(const tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_IDX) { const pty_t *dt = pty_of_low((tree_t *)e); if (dt && dt->k == PTK_ARRAY && dt->dyn) return 1; }
    if (e->t == TT_VAR && e->v.sval) return pas_array_isdyn(e->v.sval) > 0;
    int k = 0;
    while (e && e->t == TT_IDX && e->n >= 2) { e = e->c[0]; k++; }
    return e && e->t == TT_VAR && e->v.sval && pas_array_isdyn(e->v.sval) > k;
}
int pas_array_high_get(const char *name, long long *out) { return name ? pty_name_high(name, out) : 0; }
int pas_array_dims(const char *name, long long *out) { return name ? pty_name_dims(name, out) : 0; }
long long pas_arrtype_high(const char *name) { return pty_tn_high(name); }
long long pas_arrtype_ncols(const char *name) { return pty_tn_ncols(name); }
long long pas_arrtype_lo(const char *name) { return pty_tn_lo(name); }
int pas_arrtype_ischar(const char *name) { return pty_tn_ischar(name); }
const char *pas_enumnames_by_idx(int i) { return (i >= 0 && i < g_lower_pas.sem.pas_nenumname) ? g_lower_pas.sem.pas_enumnames[i].names : NULL; }
const char *pas_enumarr_of(const tree_t *n) {
    const pty_t *t = n ? pty_of_low(n) : NULL;
    if (!t || t->k != PTK_ARRAY) return NULL;
    while (t && t->k == PTK_ARRAY) t = t->base;
    return t && t->k == PTK_ENUM ? pty_name_of(t) : NULL;
}
void pas_subtype_add(const char *n, long long lo, long long hi) {
    if (n) {
        if (g_lower_pas.sem.pas_nsubtype >= g_lower_pas.sem.pas_subtypes_cap) g_lower_pas.sem.pas_subtypes =
            pas_pool_grow(g_lower_pas.sem.pas_subtypes, &g_lower_pas.sem.pas_subtypes_cap, sizeof *g_lower_pas.sem.pas_subtypes, 16);
        g_lower_pas.sem.pas_subtypes[g_lower_pas.sem.pas_nsubtype].name = ct_strdup(n);
        g_lower_pas.sem.pas_subtypes[g_lower_pas.sem.pas_nsubtype].low = lo;
        g_lower_pas.sem.pas_subtypes[g_lower_pas.sem.pas_nsubtype].high = hi;
        g_lower_pas.sem.pas_subtypes[g_lower_pas.sem.pas_nsubtype].isenum = g_lower_pas.sem.pas_last_const_enum;
        g_lower_pas.sem.pas_nsubtype++;
    }
}
int pas_string_type_kind(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) {
        if (pas_arrtype_high(t) >= 0) return 0;
        if (!strcmp(t, "shortstring") || !strcmp(t, "openstring")) return 2;
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_decl_string_kind(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_lower_pas.sem.pas_nscalarvartype - 1; name && i >= 0; i--)
        if (g_lower_pas.sem.pas_scalarvartype[i].vname && g_lower_pas.sem.pas_scalarvartype[i].uid == u && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, name))
        return pas_string_type_kind(g_lower_pas.sem.pas_scalarvartype[i].tname);
    return 0;
}
static int pas_array_scoped(const char *name) { return name && pty_name_agg(name) > 0; }
static int pas_is_strelem_arr(const char *name) { const pty_t *t = name ? pty_name_type(name) : NULL; return t && t->k == PTK_ARRAY && t->base && t->base->k == PTK_STRING; }
int pas_var_string_kind(const char *name) { return pas_is_strelem_arr(name) ? 0 : pas_decl_string_kind(name); }
int pas_is_settype(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsettype; i++) if (g_lower_pas.sem.pas_settypes[i].name && !strcmp(g_lower_pas.sem.pas_settypes[i].name, name)) return 1;
    return 0;
}
int pas_is_booltype(const char *name) {
    if (!name) return 0;
    if (!strcmp(name, "boolean")) return 1;
    for (int i = 0; i < g_lower_pas.sem.pas_nbooltype; i++) if (g_lower_pas.sem.pas_booltypes[i].name && !strcmp(g_lower_pas.sem.pas_booltypes[i].name, name)) return 1;
    return 0;
}
int pas_is_boolexpr(tree_t *e) { int t = pty_answer(e, PTK_BOOL); if (g_lower_pas.dump_types) pty_census("bool", e, t); return t > 0; }
int pas_is_charvar(const char *name) { return pty_name_is(name, PTK_CHAR) > 0; }
static int pas_std_shadowed(const char *name) { return pas_decl_string_kind(name) > 0 || pas_is_charvar(name); }
int pas_is_filevar(const char *name) { const pty_t *t = pty_name_type(name); return t && (t->k == PTK_TEXT || (t->k == PTK_ARRAY && t->base && t->base->k == PTK_TEXT)); }
int pas_is_filevar_elem(tree_t *e) {
    long long _ah;
    return e && e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_filevar(e->c[0]->v.sval) && pas_array_high_get(e->c[0]->v.sval, &_ah);
}
int pas_is_stdstream(const char *name) { return name && (!strcmp(name, "input") || !strcmp(name, "output")) && !pas_std_shadowed(name); }
int pas_hdrfile_index(const char *n) {
    for (int i = 0; n && i < g_lower_pas.sem.pas_nhdrfile; i++) if (g_lower_pas.sem.pas_hdrfiles[i] && !strcmp(g_lower_pas.sem.pas_hdrfiles[i], n)) return i + 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_is_hdrfile(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nhdrfile; i++) if (g_lower_pas.sem.pas_hdrfiles[i] && !strcmp(g_lower_pas.sem.pas_hdrfiles[i], n)) return 1;
    return 0;
}
void pas_chararr_add2(const char *name, long long lo) {
    if (name) {
        if (g_lower_pas.sem.pas_nchararr >= g_lower_pas.sem.pas_chararrs_cap) g_lower_pas.sem.pas_chararrs =
            pas_pool_grow(g_lower_pas.sem.pas_chararrs, &g_lower_pas.sem.pas_chararrs_cap, sizeof *g_lower_pas.sem.pas_chararrs, 16);
        g_lower_pas.sem.pas_chararrs[g_lower_pas.sem.pas_nchararr].name = ct_strdup(name);
        g_lower_pas.sem.pas_chararrs[g_lower_pas.sem.pas_nchararr].lo = lo;
        g_lower_pas.sem.pas_chararrs[g_lower_pas.sem.pas_nchararr].uid = pas_scope_uid(name);
        g_lower_pas.sem.pas_chararrs[g_lower_pas.sem.pas_nchararr].nostr = 0;
        g_lower_pas.sem.pas_chararrs[g_lower_pas.sem.pas_nchararr].n = -1;
        g_lower_pas.sem.pas_nchararr++;
    }
}
static void pas_chararr_add(const char *name) { pas_chararr_add2(name, 0); }
int pas_chararr_row(const char *name) {
    if (!name) return -1;
    int u = pas_scope_uid(name);
    for (int i = g_lower_pas.sem.pas_nchararr - 1; i >= 0; i--)
        if (g_lower_pas.sem.pas_chararrs[i].name && !strcmp(g_lower_pas.sem.pas_chararrs[i].name, name) && g_lower_pas.sem.pas_chararrs[i].uid == u) return i;
    return -1;
}
long long pas_chararr_lo(const char *name) { int i = pas_chararr_row(name); return i < 0 ? 0 : g_lower_pas.sem.pas_chararrs[i].lo; }
int pas_is_chararr(const char *name) { return pas_chararr_row(name) >= 0; }
void pas_chararr_shape(const char *name, int nostr, long long n) {
    int i = pas_chararr_row(name);
    if (i >= 0) { g_lower_pas.sem.pas_chararrs[i].nostr = nostr; g_lower_pas.sem.pas_chararrs[i].n = n; }
}
int pas_arrtype_nostr(const char *name) { return pas_arrtype_ischar(name) & PAS_CA_NOSTR; }
int pas_flat_marked(const tree_t *e) { for (int i = 0; i < g_lower_pas.sem.pas_flat_marks.count; i++) if (g_lower_pas.sem.pas_flat_marks.items[i] == e) return 1; return 0; }
int pas_is_deref_call(const tree_t *x) { return x && x->t == TT_FNC && x->n == 2 && x->c[0] && x->c[0]->v.sval && !strcmp(x->c[0]->v.sval, "__pas_deref"); }
int pas_row_designator(const tree_t *e, long long *d2) {
    if (!e || e->t != TT_IDX || e->n != 2 || !e->c[0] || pas_flat_marked(e)) return 0;
    if (!(e->c[0]->t == TT_VAR ? e->c[0]->v.sval != NULL : pas_is_deref_call(e->c[0]))) return 0;
    const pty_t *t = e->c[0]->t == TT_VAR ? pty_name_type(e->c[0]->v.sval) : pty_of_low(e->c[0]);
    if (!t || t->k != PTK_ARRAY || t->dyn || t->open || t->nix != 2 || !t->ix || !t->ix[0] || !t->ix[1] || !t->ix[0]->bounded || !t->ix[1]->bounded || !t->base || t->base->k == PTK_CHAR) return 0;
    d2[0] = t->ix[0]->lo;
    d2[1] = t->ix[0]->hi;
    d2[2] = t->ix[1]->lo;
    d2[3] = t->ix[1]->hi;
    return 1;
}
tree_t *pas_dense_sub(tree_t *i, long long lo) { return lo ? bin(TT_SUB, i, ilit(lo)) : i; }
tree_t *mk_row_call(const char *fn, const tree_t *e, const long long *d2, tree_t *row) {
    tree_t *f = ast_node_new(TT_FNC);
    ast_push(f, leaf_s(TT_VAR, fn));
    ast_push(f, pas_tree_clone(e->c[0]));
    ast_push(f, pas_dense_sub(pas_tree_clone(e->c[1]), d2[0]));
    ast_push(f, ilit(d2[3] - d2[2] + 1));
    ast_push(f, ilit(d2[2]));
    ast_push(f, ilit(d2[3]));
    if (row) ast_push(f, row);
    return f;
}
int pas_ca_is_read(const tree_t *e) { return e && e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_ca_unpack"); }
int pas_is_strarr(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nstrarr; i++) if (g_lower_pas.sem.pas_strarrs[i].name && !strcmp(g_lower_pas.sem.pas_strarrs[i].name, name)) return 1;
    return 0;
}
long long pas_strarr_lo(const char *name) {
    if (!name) return 1;
    for (int i = 0; i < g_lower_pas.sem.pas_nstrarr; i++) if (g_lower_pas.sem.pas_strarrs[i].name && !strcmp(g_lower_pas.sem.pas_strarrs[i].name, name)) return g_lower_pas.sem.pas_strarrs[i].lo;
    return 1;
}
const char *pas_arrrec_field_enum(const char *a, long idx) { struct pty_fld f; return pas_arf(a, idx, &f) && f.en && f.ft ? pty_name_of(f.ft) : NULL; }
static int pas_arrrec_field_is_char(const char *a, long idx) { struct pty_fld f; return pas_arf(a, idx, &f) ? f.ch == 1 : 0; }
int pas_arrrec_find(const char *a, const char **rn) { const pty_t *t = pas_art(a); if (!t) return 0; if (rn) *rn = pty_name_of(t); return pty_rec_nf(t); }
int pas_arrrec_field_index(const char *a, const char *fn) {
    const pty_t *t = pas_art(a);
    struct pty_fld g;
    for (int j = 0; fn && t && j < pty_rec_nf(t); j++) if (pty_rec_fld(t, j, &g) && g.name && !strcmp(g.name, fn)) return j;
    return -1;
}
static int pas_arrrec_slot(const char *a, long fi) { struct pty_fld f; return pas_arf(a, fi, &f) ? (int)(f.ov & 0xFFFF) : (int)fi; }
tree_t *pas_arrrec_flatten(tree_t *idxsel, long long fi) {
    tree_t *e = ast_node_new(TT_IDX);
    ast_push(e, idxsel);
    ast_push(e, ilit((idxsel && idxsel->t == TT_IDX && idxsel->n == 2 && idxsel->c[0] && idxsel->c[0]->t == TT_VAR && idxsel->c[0]->v.sval) ? pas_arrrec_slot(idxsel->c[0]->v.sval, fi) : fi));
    e->slen = PAS_FIELD_IDX_MARK;
    return e;
}
tree_t *pas_vt_unwrap_read(tree_t *e) {
    if (e && e->t == TT_SEQ_EXPR && e->n >= 2 && e->c[0] && e->c[0]->t == TT_FNC && e->c[0]->n >= 1 && e->c[0]->c[0] && e->c[0]->c[0]->v.sval &&
        (!strcmp(e->c[0]->c[0]->v.sval, "__pas_vcheck") || !strcmp(e->c[0]->c[0]->v.sval, "__pas_tagwhole"))) return e->c[e->n - 1];
    return e;
}
int pas_is_charexpr(tree_t *e) { int t = pty_answer(e, PTK_CHAR); if (g_lower_pas.dump_types) pty_census("char", e, t); return t > 0; }
static int pas_is_strtyped(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_QLIT) return 1;
    if (pas_ca_is_read(e)) return 1;
    if (e->t == TT_VAR && e->v.sval && pas_is_chararr(e->v.sval)) return 1;
    if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_is_chararr(e->c[0]->v.sval) || pas_is_strarr(e->c[0]->v.sval))) return 1;
    return 0;
}
tree_t *pas_alpha_wrap(tree_t *x) {
    if (x && x->t == TT_VAR && x->v.sval && pas_is_chararr(x->v.sval)) {
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(f, x);
        ast_push(f, ilit(pas_chararr_lo(x->v.sval)));
        if (g_lower_pas.sem.pas_dialect_fpc) ast_push(f, ilit(1));
        return f;
    }
    if (x && x->t == TT_IDX && x->n >= 2 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && pas_is_strarr(x->c[0]->v.sval)) {
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(f, x);
        ast_push(f, ilit(pas_strarr_lo(x->c[0]->v.sval)));
        return f;
    }
    return x;
}
static int pas_is_strval(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_QLIT || e->t == TT_CAT) return 1;
    if (pas_ca_is_read(e)) return 1;
    if (e->t == TT_VAR && e->v.sval && (pas_is_chararr(e->v.sval) || pas_is_strarr(e->v.sval))) return 1;
    if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_strarr(e->c[0]->v.sval)) return 1;
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && pas_is_strelem_arr(e->c[0]->v.sval)) return 1;
    if (e->t == TT_IDX && e->n == 2 && pas_dyn_elem_is_string(e->c[0])) return 1;
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_func_returns_stringish(e->c[0]->v.sval)) return 1;
    return 0;
}
static int pas_is_strvar(tree_t *e) { return e && e->t == TT_VAR && e->v.sval && pas_var_string_kind(e->v.sval) > 0; }
tree_t *pas_rel(tree_e tt, tree_t *a, tree_t *b) {
    if ((pas_is_stringexpr(a) && pas_is_charexpr(b)) || (pas_is_charexpr(a) && pas_is_stringexpr(b))) return bin(tt, mk_set_bin("__pas_strcmp", pas_concat_operand(a), pas_concat_operand(b)), ilit(0));
    if ((tt == TT_LT || tt == TT_LE || tt == TT_GT || tt == TT_GE) && ((pas_is_qword(a) && a->t != TT_ILIT) || (pas_is_qword(b) && b->t != TT_ILIT)))
        return bin(tt, mk_fnc2("ixor", a, ilit(INT64_MIN)), mk_fnc2("ixor", b, ilit(INT64_MIN)));
    int lex = pas_is_strval(a) || pas_is_strval(b) || pas_is_strvar(a) || pas_is_strvar(b);
    if (lex || pas_is_strtyped(a) || pas_is_strtyped(b)) { a = pas_alpha_wrap(a); b = pas_alpha_wrap(b); }
    return lex ? bin(tt, mk_set_bin("__pas_strcmp", a, b), ilit(0)) : bin(tt, a, b);
}
tree_t *pas_rel_or_set(tree_e tt, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : pas_rel(tt, a, b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_assigns_result(tree_t *t, const char *f) {
    if (!t) return 0;
    if (t->t == TT_ASSIGN && t->n >= 1 && t->c[0]) { tree_t *l = t->c[0]->t == TT_FNC && t->c[0]->n >= 1 ? t->c[0]->c[0] : t->c[0]; return l && l->t == TT_VAR && l->v.sval && !strcmp(l->v.sval, f); }
    if (t->t == TT_FNC && t->n >= 1 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__pas_rterr")) return 1;
    if (t->t == TT_IF) return t->n == 3 && pas_assigns_result(t->c[1], f) && pas_assigns_result(t->c[2], f);
    if (t->t == TT_PROGRAM || t->t == TT_SEQ_EXPR) { for (int i = 0; i < t->n; i++) if (pas_assigns_result(t->c[i], f)) return 1; }
    return 0;
}
tree_t *pas_result_check(const char *fname, tree_t *body) {
    if (g_lower_pas.sem.pas_seen_mode_directive || !fname || !body || body->t != TT_PROGRAM || pas_func_returns_stringish(fname) || pas_assigns_result(body, fname)) return body;
    size_t ml = strlen(fname) + 80;
    char *msg = (char *)ct_zalloc(ml, 1);
    snprintf(msg, ml, "the result of the function '%s' is undefined upon completion of its algorithm", fname);
    tree_t *chain = ast_node_new(TT_FNC);
    ast_push(chain, leaf_s(TT_VAR, "__pas_rterr"));
    ast_push(chain, leaf_s(TT_QLIT, "6.6.2"));
    ast_push(chain, leaf_s(TT_QLIT, msg));
    ast_push(body, bin(TT_IF, bin(TT_EQ, mk_fnc1("__pas_resundef", leaf_s(TT_VAR, fname)), ilit(1)), chain));
    return body;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_src_mentions(const tree_t *t, const char *v) {
    if (!t) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcasecmp(t->v.sval, v)) return 1;
    for (int i = 0; i < t->n; i++) if (pas_src_mentions(t->c[i], v)) return 1;
    return 0;
}
static int pas_src_assigns(const tree_t *t, const char *v) {
    if (!t) return 0;
    if (t->t == TT_ASSIGN && t->n >= 1 && pas_src_mentions(t->c[0], v)) return 1;
    if (t->t == TT_FOR && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcasecmp(t->c[0]->v.sval, v)) return 1;
    if (t->t == TT_WITH && t->n > 1) for (int i = 0; i + 1 < t->n; i++) if (pas_src_mentions(t->c[i], v)) return 1;
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && strcasecmp(t->c[0]->v.sval, "write") && strcasecmp(t->c[0]->v.sval, "writeln"))
        for (int i = 1; i < t->n; i++) if (pas_src_mentions(t->c[i], v)) return 1;
    for (int i = 0; i < t->n; i++) if (pas_src_assigns(t->c[i], v)) return 1;
    return 0;
}
static int pas_src_vlist_count(const tree_t *t, const char *v) {
    int c = 0;
    if (!t) return 0;
    if (t->t == TT_VLIST) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR && t->c[i]->v.sval && !strcasecmp(t->c[i]->v.sval, v)) c++;
    for (int i = 0; i < t->n; i++) c += pas_src_vlist_count(t->c[i], v);
    return c;
}
static void pas_undef_vars_scan(const tree_t *flat) {
    if (g_lower_pas.sem.pas_seen_mode_directive || !flat) return;
    for (int b = 0; b < flat->n; b++) {
        const tree_t *blk = flat->c[b];
        if (!blk || blk->t != TT_BLOCK) continue;
        for (int pi = 0; pi < blk->n; pi++) {
            const tree_t *pt = blk->c[pi];
            if (!pt || pt->t != TT_PART || !pt->v.sval || strcmp(pt->v.sval, "var")) continue;
            for (int di = 0; di < pt->n; di++) {
                const tree_t *d = pt->c[di], *ids = d && d->n > 1 ? d->c[0] : NULL, *ty = d && d->n > 1 ? d->c[1] : NULL;
                if (!ids || ids->t != TT_VLIST || !ty || ty->t != TT_VAR || !ty->v.sval) continue;
                if (strcasecmp(ty->v.sval, "integer") && strcasecmp(ty->v.sval, "real") && strcasecmp(ty->v.sval, "boolean") && strcasecmp(ty->v.sval, "char")) continue;
                for (int k = 0; k < ids->n; k++) {
                    const char *nm = ids->c[k] && ids->c[k]->t == TT_VAR ? ids->c[k]->v.sval : NULL;
                    if (!nm || pas_src_vlist_count(flat, nm) != 1 || pas_src_assigns(flat, nm)) continue;
                    g_lower_pas.sem.pas_undef = (const char **)ct_grow(g_lower_pas.sem.pas_undef, (size_t)(g_lower_pas.sem.pas_nundef + 1) * sizeof(const char *));
                    g_lower_pas.sem.pas_undef[g_lower_pas.sem.pas_nundef++] = nm;
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_undef_var(const char *name) { for (int i = 0; name && i < g_lower_pas.sem.pas_nundef; i++) if (!strcasecmp(g_lower_pas.sem.pas_undef[i], name)) return 1; return 0; }
int pas_actual_is_tagfield(tree_t *a) {
    for (int i = g_lower_pas.sem.pas_nvt - 1; a && i >= 0; i--)
        if ((g_lower_pas.sem.pas_vt[i]->state == 7 || g_lower_pas.sem.pas_vt[i]->state == 5 || g_lower_pas.sem.pas_vt[i]->state == 8) && g_lower_pas.sem.pas_vt[i]->node == a) return 1;
    return 0;
}
int pas_is_setexpr(tree_t *e) { int t = pty_answer(e, PTK_SET); if (g_lower_pas.dump_types) pty_census("set", e, t); return t > 0; }
tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
int pas_is_qword(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_ILIT) return e->v.ival < 0;
    { const pty_t *ty = pty_of_low(e); if (ty && ty->k == PTK_INT && ty->uns) return 1; }
    if (e->t != TT_VAR || !e->v.sval) return 0;
    const char *tn = pas_scalarvartype_get(e->v.sval);
    for (int g = 0; tn && g < 8; g++) { if (pas_is_u64_typename(tn)) return 1; const char *al = pas_typealias_get(tn); if (!al || !strcmp(al, tn)) break; tn = al; }
    return 0;
}
int pas_is_stringexpr(tree_t *e) { int t = pty_answer(e, PTK_STRING); if (g_lower_pas.dump_types) pty_census("string", e, t); return t > 0; }
tree_t *pas_concat_operand(tree_t *e) {
    if (!e || pas_is_stringexpr(e) || !pas_is_charexpr(e)) return e;
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_chrlit") && e->c[1] && e->c[1]->t == TT_ILIT && e->c[1]->v.ival != 0) {
        char *cb = (char *)ct_zalloc(2, 1);
        cb[0] = (char)e->c[1]->v.ival;
        tree_t *q = ast_node_new(TT_QLIT);
        q->v.sval = cb;
        return q;
    }
    return mk_chr_wrap(e);
}
tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) {
    if (pas_is_setexpr(a) || pas_is_setexpr(b)) return mk_set_bin(setfn, a, b);
    if (ak == TT_ADD && (pas_is_stringexpr(a) || pas_is_stringexpr(b) || (g_lower_pas.sem.pas_seen_mode_directive && pas_is_charexpr(a) && pas_is_charexpr(b))))
        return bin(TT_CAT, pas_concat_operand(a), pas_concat_operand(b));
    if (ak == TT_SUB && (g_lower_pas.sem.pas_zerobased_strings & 2) && a && a->t == TT_ADD) { int nv = 0; if (pas_unsigned_sum(a, &nv) && nv >= 2) return mk_fnc1("__pas_qchk", bin(ak, a, b)); }
    return bin(ak, a, b);
}
const pty_vt_t *pas_vt_find_name(const char *nm) { const pty_t *r = nm ? pty_rec_by_name(nm) : NULL; if (!r) r = pas_rvt(nm); if (!r) r = pas_art(nm); return r ? pty_rec_vt(r) : NULL; }
tree_t *pas_field_init_tree(const char *rn, const char *vn, int fi) {
    const pty_t *rt = rn ? pty_rec_by_name(rn) : NULL;
    struct pty_fld f;
    if (rt && pty_rec_nf(rt) <= 0) rt = NULL;
    if (rt ? !pty_rec_fld(rt, fi, &f) : !(vn && pas_rvf(vn, fi, &f))) return NULL;
    const char *frn = pas_fld_rectype(&f);
    if (f.na && f.nahi >= f.nalo && frn) return mk_fnc3("__pas_arr_of", ilit(f.nalo), ilit(f.nahi), mk_rec_init_type_d(frn, 1));
    if (f.na && f.nahi >= f.nalo) return mk_fnc2("arr_make", ilit(f.nalo), ilit(f.nahi));
    if (!f.na && frn) return mk_rec_init_type_d(frn, 1);
    return NULL;
}
tree_t *pas_region_elem(tree_t *f, tree_t *idx) {
    long long ek = f->c[3]->v.ival, es = (ek >> 8) & 0xFF, off = f->c[2]->v.ival, lo = f->c[4]->v.ival, rsz = f->c[5]->v.ival;
    tree_t *d = ast_node_new(TT_FNC);
    ast_push(d, leaf_s(TT_VAR, "__pas_rdecode"));
    ast_push(d, f->c[1]);
    ast_push(d, bin(TT_ADD, ilit(off), bin(TT_MUL, bin(TT_SUB, idx, ilit(lo)), ilit(es))));
    ast_push(d, ilit(ek));
    ast_push(d, ilit(rsz));
    return d;
}
static long long pas_wrap_mask_of_type(const char *t) {
    for (int g = 0; t && g < 8; g++) {
        if (!strcmp(t, "byte") || !strcmp(t, "uint8")) return 0xFFLL;
        if (!strcmp(t, "word") || !strcmp(t, "uint16")) return 0xFFFFLL;
        if (!strcmp(t, "longword") || !strcmp(t, "cardinal") || !strcmp(t, "uint32") || !strcmp(t, "dword")) return 0xFFFFFFFFLL;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *pas_wrap_to_tn(tree_t *rhs, const char *tn, int arr) {
    long long _wm = pas_wrap_mask_of_type(tn);
    if (_wm) rhs = mk_fnc2("iand", rhs, ilit(_wm));
    int _sb = arr ? 0 : pas_wrap_sbits_of_type(tn);
    if (_sb) rhs = bin(TT_SUB, mk_fnc2("ixor", mk_fnc2("iand", rhs, ilit((1LL << _sb) - 1)), ilit(1LL << (_sb - 1))), ilit(1LL << (_sb - 1)));
    return rhs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    if (!g_lower_pas.sem.pas_dialect_fpc && sel && sel->t == TT_IDX && sel->n >= 1 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 1 && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_VAR &&
        sel->c[0]->c[0]->v.sval && pas_curfunc_name() && !strcmp(sel->c[0]->c[0]->v.sval, pas_curfunc_name())) {
        fprintf(stderr,
            "pascal: ISO 7185 6.8.2.2 violation: a component of the function '%s' is assigned; an assignment-statement assigns a variable-access or the"
            " function-identifier itself, never a component of the function's result\n", sel->c[0]->c[0]->v.sval);
        g_lower_pas.sem.pas_iso_errors++;
    }
    { long long _rd[4]; if (pas_row_designator(rhs, _rd)) rhs = mk_row_call("__pas_row_get", rhs, _rd, NULL); if (pas_row_designator(sel, _rd)) return mk_row_call("__pas_row_set", sel, _rd, rhs); }
    int _rpatch = rhs && rhs->t == TT_FNC && rhs->n >= 1 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__pas_rpatch");
    if (rhs && rhs->t != TT_ILIT && !_rpatch && !(sel && sel->t == TT_VAR && sel->v.sval && pas_array_isdyn(sel->v.sval))) {
        const char *_tn = pas_store_tn(sel);
        long long _ad[4], _ah;
        int _arr = sel && sel->t == TT_VAR && sel->v.sval && (pas_array_dims(sel->v.sval, _ad) || pas_array_high_get(sel->v.sval, &_ah));
        if (_tn) rhs = pas_wrap_to_tn(rhs, _tn, _arr);
    }
    if (sel && sel->t == TT_FNC && sel->n == 5 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_rdecode") && sel->c[1]) {
        tree_t *e0 = sel->c[1];
        tree_t *pt = ast_node_new(TT_FNC);
        ast_push(pt, leaf_s(TT_VAR, "__pas_rpatch"));
        ast_push(pt, pas_tree_clone(e0));
        ast_push(pt, sel->c[2]);
        ast_push(pt, sel->c[3]);
        ast_push(pt, sel->c[4]);
        ast_push(pt, rhs);
        return mk_assign(e0, pt);
    }
    if (g_lower_pas.sem.pas_ptrcell && sel && rhs) {
        const pty_t *_lt = pty_of_low(sel), *_rt = pty_of_low(rhs);
        if (_lt && _lt->k == PTK_POINTER && _lt->base && _rt && _rt->k == PTK_POINTER && !_rt->base) rhs = mk_fnc1("__pas_pchar_of", rhs);
    }
    { tree_t *_pc = pas_pchar_assign(sel, rhs); if (_pc) return _pc; }
    rhs = pas_pun_view(sel, rhs);
    { const char *_abn = pas_selector_base_name(sel); if (_abn) pas_assigned_add(_abn); }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[1] && sel->c[1]->t == TT_ILIT && rhs && rhs->t != TT_QLIT && pas_is_charexpr(rhs)) {
        const char *_frt = pas_with_sel_rtype(sel->c[0]);
        const char *_ftn = _frt ? pas_rectype_field_typename(_frt, sel->c[1]->v.ival) : NULL;
        if (_ftn && pas_string_type_kind(_ftn)) {
            if (rhs->t == TT_FNC && rhs->n == 2 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__pas_chrlit") && rhs->c[1] && rhs->c[1]->t == TT_ILIT) {
                char *_cb = (char *)ct_zalloc(2, 1);
                _cb[0] = (char)rhs->c[1]->v.ival;
                tree_t *_q = ast_node_new(TT_QLIT);
                _q->v.sval = _cb;
                rhs = _q;
            } else rhs = mk_chr_wrap(rhs);
        }
    }
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_QLIT && pas_var_string_kind(sel->v.sval) && pas_is_charexpr(rhs)) rhs = mk_chr_wrap(rhs);
    if (sel && sel->t == TT_IDX && sel->n == 2 && sel->c[0] && sel->c[0]->t == TT_VAR && pas_is_strelem_arr(sel->c[0]->v.sval) && rhs && rhs->t != TT_QLIT && pas_is_charexpr(rhs)) rhs =
        mk_chr_wrap(rhs);
    if (sel && sel->t == TT_IDX && rhs && rhs->t != TT_QLIT && !(rhs->t == TT_FNC && rhs->n == 2 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__pas_chr")) && pas_is_charexpr(rhs))
        {
        const pty_t *_st = pty_of_low(sel);
        if (_st && _st->k == PTK_STRING) rhs = mk_chr_wrap(rhs);
    }
    if (sel && sel->t == TT_FNC && sel->n == 1 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && rhs && rhs->t != TT_QLIT && pas_func_returns_stringish(sel->c[0]->v.sval) &&
        pas_is_charexpr(rhs)) rhs = pas_concat_operand(rhs);
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_FLIT && pas_var_is_real(sel->v.sval) && !pas_array_isdyn(sel->v.sval) && !pas_expr_is_real(rhs)) rhs =
        bin(TT_ADD, rhs, flit(0.0));
    if (sel && sel->t == TT_FNC && sel->n == 1 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && rhs && rhs->t != TT_FLIT && pas_var_is_real(sel->c[0]->v.sval) && !pas_expr_is_real(rhs))
        rhs = bin(TT_ADD, rhs, flit(0.0));
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs) {
        int _dbf = pas_var_is_boolfam(sel->v.sval);
        int _sbf = (rhs->t == TT_VAR && rhs->v.sval) ? pas_var_is_boolfam(rhs->v.sval) : 0;
        if (_dbf && !_sbf && pas_is_boolexpr(rhs)) rhs = bin(TT_SUB, ilit(0), rhs);
        else if (!_dbf && _sbf) rhs = bin(TT_NE, rhs, ilit(0));
    }
    if (pas_is_aggregate_designator(rhs)) rhs = mk_fnc1("__pas_arr_copy", rhs);
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0]) && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_FNC &&
        sel->c[0]->c[0]->n >= 2 && sel->c[0]->c[0]->c[0] && sel->c[0]->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_idx_set"));
        ast_push(e, sel->c[0]->c[0]->c[1]);
        ast_push(e, sel->c[0]->c[1]);
        ast_push(e, sel->c[1]);
        ast_push(e, rhs);
        return e;
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0]) && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2 && sel->c[0]->c[0] &&
        (sel->c[0]->c[0]->t == TT_VAR || sel->c[0]->c[0]->t == TT_IDX)) {
        tree_t *base = sel->c[0]->c[0];
        tree_t *upd = ast_node_new(TT_FNC);
        ast_push(upd, leaf_s(TT_VAR, "__pas_field_idx_update"));
        ast_push(upd, base->t == TT_VAR ? pas_tree_clone(base) : base);
        ast_push(upd, pas_tree_clone(sel->c[0]->c[1]));
        ast_push(upd, sel->c[1]);
        ast_push(upd, rhs);
        return base->t == TT_VAR ? bin(TT_ASSIGN, base, upd) : upd;
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval &&
        !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]);
        ast_push(e, sel->c[1]);
        ast_push(e, rhs);
        return e;
    }
    return bin(TT_ASSIGN, sel, rhs);
}
tree_t *mk_ident(const char *name) {
    if (pas_pf_is_formal(name) && pas_pf_lookup(name)->sig[0] == 'F' && pas_pf_nactuals(pas_pf_lookup(name)->sig) == 0) return pas_pf_callthrough(name, NULL);
    if (name && !strcmp(name, "result") && !pas_is_declared_local_here(name)) { const char *cf = pas_curfunc_name(); if (cf) return leaf_s(TT_VAR, cf); }
    if (name && pas_curfunc_name()) {
        const char *cf = pas_curfunc_name();
        size_t nl = strlen(name);
        if (!strncmp(cf, name, nl) && cf[nl] == '$' && cf[nl + 1] && cf[nl + 1 + strspn(cf + nl + 1, "0123456789")] == 0 && !pas_is_declared_local_here(name)) return leaf_s(TT_VAR, cf);
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !g_lower_pas.sem.pas_fname_is_call && pas_curfunc_name()) {
        const char *cf = pas_curfunc_name(), *sep = strstr(cf, "__pc_") == cf ? strrchr(cf, '_') : NULL;
        if (!strcmp(name, cf)) return leaf_s(TT_VAR, name);
        if (sep && sep > cf + 1 && sep[-1] == '_' && !strcmp(sep + 1, name) && !pas_is_declared_local_here(name)) return leaf_s(TT_VAR, cf);
    }
    if (name && !strcmp(name, "true")) return bin(TT_EQ, ilit(1), ilit(1));
    if (name && !strcmp(name, "false")) return bin(TT_EQ, ilit(0), ilit(1));
    if (name && !strcmp(name, "nil")) return ilit(0);
    if (name && !strcmp(name, "hinstance")) return ilit(0);
    if (name && !strcmp(name, "eof")) return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln")) return mk_fnc0("__pas_eoln");
    if (name && !strcmp(name, "paramcount") && !pas_is_func(name) && !pas_scalarvartype_get(name)) return mk_fnc0("__pas_paramcount");
    long long cv;
    if (pas_const_get(name, &cv)) return pas_is_charvar(name) ? mk_chrlit(ilit(cv)) : ilit(cv);
    if (name && !strcmp(name, "maxint") && !pas_scalarvartype_get(name) && !pas_is_func(name)) return ilit(g_lower_pas.sem.pas_int16 ? 32767 : 2147483647);
    double rv;
    if (pas_rconst_get(name, &rv)) return flit(rv);
    const char *sv = pas_sconst_get(name);
    if (sv) return leaf_s(TT_QLIT, sv);
    { tree_t *sc = pas_setconst_get(name); if (sc) return pas_tree_clone(sc); }
    if (name && !strcmp(name, "pi") && !pas_scope_uid("pi") && !pas_scalarvartype_get(name)) return flit(3.141592653589793);
    if (name && !strcmp(name, "random") && !pas_scope_uid("random") && !pas_scalarvartype_get(name) && g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso)
        return mk_fnc0("__pas_random");
    for (int wi = g_lower_pas.sem.with_depth - 1; wi >= 0; wi--) {
        tree_t *wsel = g_lower_pas.sem.with_stk[wi].sel;
        const char *rt = g_lower_pas.sem.with_stk[wi].rtype;
        int fi = -1;
        if (rt) fi = pas_with_field_index(rt, name);
        if (fi < 0 && wsel && wsel->t == TT_VAR && wsel->v.sval) fi = pas_with_recvar_field(wsel->v.sval, name);
        if (fi < 0 && wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && wsel->c[0]->v.sval) fi = pas_arrrec_field_index(wsel->c[0]->v.sval, name);
        if (fi >= 0) {
            if (wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && pas_arrrec_find(wsel->c[0]->v.sval, NULL) > 0) {
                tree_t *_af = pas_arrrec_flatten(pas_tree_clone(wsel), fi);
                const char *_wrn = NULL;
                _af = pas_arrrec_region(_af, wsel->c[0]->v.sval, fi);
                pas_arrrec_find(wsel->c[0]->v.sval, &_wrn);
                return _af;
            }
            tree_t *e = ast_node_new(TT_IDX);
            ast_push(e, pas_tree_clone(wsel));
            ast_push(e, ilit(rt ? pas_rectype_slot(rt, fi) : (wsel && wsel->t == TT_VAR && wsel->v.sval) ? pas_recvar_slot(wsel->v.sval, fi) : fi));
            { const char *_crt = rt ? rt : pas_with_sel_rtype(wsel); }
            {
                const pty_vt_t *_vt = pas_vt_find_name(rt ? rt : pas_with_sel_rtype(wsel));
                if (!_vt && wsel && wsel->t == TT_VAR && wsel->v.sval) _vt = pas_vt_find_name(wsel->v.sval);
                if (_vt && _vt->tagfi >= 0 && fi == _vt->tagfi) pas_vt_new(8)->node = e;
            }
            return pas_field_region(wsel, rt, fi, e);
        }
    }
    if (pas_is_func(name) && !pas_name_is_data(name)) return lpt_mk_call(name, pas_defaults_extend(name, NULL));
    pas_scope_require(name);
    return leaf_s(TT_VAR, name);
}
tree_t *lpt_pas_cond(tree_t *e) { return (pas_is_rel(e) || (e && (e->t == TT_CONJ || e->t == TT_ALT || e->t == TT_NOT))) ? e : bin(TT_NE, e, ilit(0)); }
tree_t *pas_flip_rel(tree_t *e) {
    switch (e->t) {
        case TT_LT:
        e->t = TT_GE;
        break;
        case TT_GE:
        e->t = TT_LT;
        break;
        case TT_LE:
        e->t = TT_GT;
        break;
        case TT_GT:
        e->t = TT_LE;
        break;
        case TT_EQ:
        e->t = TT_NE;
        break;
        case TT_NE:
        e->t = TT_EQ;
        break;
        case TT_CONJ:
        case TT_ALT:
        { tree_t *n = ast_node_new(TT_NOT); ast_push(n, e); return n; }
        default:
        break;
    }
    return e;
}
tree_t *pas_str_to_alpha(const char *s, long long lo, long long high) {
    tree_t *q = ast_node_new(TT_QLIT);
    q->v.sval = ct_strdup(s ? s : "");
    return mk_fnc3("__pas_ca_encode", q, ilit(lo < 0 ? 0 : lo), ilit(high));
}
tree_t *pas_str_padded(const char *s, long long n) {
    size_t sl = s ? strlen(s) : 0;
    size_t len = (n > (long long) sl) ? (size_t) n : sl;
    char *buf = (char *) ct_alloc(len + 1);
    for (size_t k = 0; k < len; k++) buf[k] = (k < sl) ? s[k] : ' ';
    buf[len] = '\0';
    tree_t *q = ast_node_new(TT_QLIT);
    q->v.sval = buf;
    return q;
}
static int pas_array_is_pure_num(const char *name) { if (!name) return 0; if (pas_is_chararr(name)) return 0; if (pas_arrrec_find(name, NULL) > 0) return 0; if (pas_rvt(name)) return 0; return 1; }
static int pas_array_is_real(const char *name) {
    if (pas_array_is_pure_num(name)) return 1;
    if (!name || pas_is_chararr(name) || pas_arrrec_find(name, NULL) <= 0) return 0;
    if (pas_rvt(name)) return 0;
    return 1;
}
static tree_t *mk_rec_block(int nf, char *const *fldrec, const int *fldna, const long long *na_lo, const long long *na_hi, int depth) {
    tree_t *e = mk_fnc1("arr_make", ilit(nf > 0 ? nf - 1 : 0));
    for (int j = 0; j < nf && depth < 16; j++) {
        tree_t *sub = NULL;
        if (fldna[j] && na_hi[j] >= na_lo[j] && fldrec[j]) sub = mk_fnc3("__pas_arr_of", ilit(na_lo[j]), ilit(na_hi[j]), mk_rec_init_type_d(fldrec[j], depth + 1));
        else if (fldna[j] && na_hi[j] >= na_lo[j]) sub = mk_fnc2("arr_make", ilit(na_lo[j]), ilit(na_hi[j]));
        else if (!fldna[j] && fldrec[j]) sub = mk_rec_init_type_d(fldrec[j], depth + 1);
        if (sub) e = mk_fnc3("arr_set_pure", e, ilit(j), sub);
    }
    return e;
}
static tree_t *mk_row_value(const pty_t *t, int depth) {
    long long lo, hi;
    if (depth > 8 || !pty_array_bounds(t, &lo, &hi)) return NULL;
    if (t->base && t->base->k == PTK_CHAR) return mk_fnc2("arr_make", ilit(lo), ilit(hi));
    tree_t *sub = mk_row_value(t->base, depth + 1);
    return sub ? mk_fnc3("__pas_arr_of", ilit(lo), ilit(hi), sub) : NULL;
}
tree_t *mk_rec_block_of(const pty_t *r, int depth) {
    int nf = pty_rec_nf(r), m = nf > 0 ? nf : 1;
    const char *fr[m];
    int fn[m];
    long long lo[m], hi[m];
    for (int j = 0; j < nf; j++) { struct pty_fld f; pty_rec_fld(r, j, &f); fr[j] = pas_fld_rectype(&f); fn[j] = f.na != 0; lo[j] = f.nalo; hi[j] = f.nahi; }
    tree_t *e = mk_rec_block(nf, (char *const *)fr, fn, lo, hi, depth);
    for (int j = 0; j < nf; j++) {
        struct pty_fld f;
        pty_rec_fld(r, j, &f);
        tree_t *row = f.ft && f.ft->k == PTK_ARRAY && f.ft->base && f.ft->base->k == PTK_ARRAY ? mk_row_value(f.ft, 0) : NULL;
        if (row) e = mk_fnc3("arr_set_pure", e, ilit(j), row);
    }
    return e;
}
static tree_t *mk_rec_init_type_d(const char *rt, int depth) {
    const pty_t *t = rt ? pty_rec_by_name(rt) : NULL;
    return t && pty_rec_nf(t) > 0 ? mk_rec_block_of(t, depth) : mk_fnc1("arr_make", ilit(0));
}
tree_t *mk_rec_init_type(const char *rt) { return mk_rec_init_type_d(rt, 0); }
tree_t *pas_new_target_init(const char *rt) {
    long long ah = pas_arrtype_high(rt);
    if (ah >= 0 && (!pas_arrtype_ischar(rt) || g_lower_pas.sem.pas_dialect_fpc) && pas_arrtype_ncols(rt) < 0) return pas_rectype_nf(rt) > 0 ?
        mk_fnc3("__pas_arr_of", ilit(pas_arrtype_lo(rt)), ilit(ah), mk_rec_init_type(rt)) : mk_fnc2("arr_make", ilit(pas_arrtype_lo(rt)), ilit(ah));
    return pas_rectype_nf(rt) > 0 ? mk_rec_init_type(rt) : ilit(0);
}
static int pas_static_of_dyn(const char *name) { const pty_t *t = name ? pty_name_type(name) : NULL; return t && t->k == PTK_ARRAY && !t->dyn && t->base && t->base->k == PTK_ARRAY && t->base->dyn; }
tree_t *mk_array_init(const char *name, long long high) {
    if (pas_array_isdyn(name)) {
        long long rw, rs;
        const pty_t *rt = pty_name_type(name);
        if (rt && rt->k == PTK_ARRAY && rt->dyn && pas_raw_int_w(rt->base, &rw, &rs)) return mk_fnc4("arr_make_raw", ilit(0), ilit(-1), ilit(rw), ilit(rs));
        return mk_fnc2("arr_make", ilit(0), ilit(-1));
    }
    if (pas_static_of_dyn(name)) return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), mk_fnc2("arr_make", ilit(0), ilit(-1)));
    if (pas_rvt(name)) return mk_rec_block_of(pas_rvt(name), 0);
    if (pas_art(name)) return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), mk_rec_block_of(pas_art(name), 0));
    {
        const pty_t *pt = name ? pty_name_type(name) : NULL;
        long long blo, bhi;
        tree_t *row = pt && pt->base && pt->base->k == PTK_ARRAY && pty_array_bounds(pt, &blo, &bhi) ? mk_row_value(pt->base, 0) : NULL;
        if (row) return mk_fnc3("__pas_arr_of", ilit(blo), ilit(bhi), row);
    }
    if (name && pas_is_strarr(name)) {
        const char *et = pas_scalarvartype_get(name);
        long long eh = et ? pas_arrtype_high(et) : -1;
        if (eh >= 0) return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), mk_fnc2("arr_make", ilit(pas_arrtype_lo(et)), ilit(eh)));
    }
    { long long rlo, rhi; if (name && pas_rows_get(name, 0, &rlo, &rhi)) return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), mk_fnc2("arr_make", ilit(rlo), ilit(rhi))); }
    {
        long long rw, rs;
        const pty_t *rt = name ? pty_name_type(name) : NULL;
        if (rt && rt->k == PTK_ARRAY && !(rt->packed && rt->base && rt->base->k == PTK_CHAR) && pas_array_ncols(name) < 0 && pas_raw_int_w(rt->base, &rw, &rs))
            return mk_fnc4("arr_make_raw", ilit(pas_array_low(name)), ilit(high), ilit(rw), ilit(rs));
    }
    if (!pas_array_is_real(name)) return mk_fnc2("arr_make", ilit(pas_is_chararr(name) ? pas_chararr_lo(name) : 0), ilit(high));
    { long long lo = pas_array_low(name); if (pas_array_ncols(name) < 0 && lo != 0 && lo <= high) return mk_fnc2("arr_make", ilit(lo), ilit(high)); }
    return mk_fnc1("arr_make", ilit(high));
}
void pas_formal_chararrs(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && pas_arrtype_ischar(t)) { pas_chararr_add2(id->v.sval, pas_arrtype_lo(t)); pas_chararr_shape(id->v.sval, pas_arrtype_nostr(t), pas_arrtype_high(t)); }
    }
}
void pas_formal_strtypes(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t) pas_scalarvartype_add(id->v.sval, t);
    }
}
const PasDef *pas_pf_lookup(const char *name) {
    for (int i = g_lower_pas.sem.pas_scope.n - 1; name && i >= 0; i--) if (g_lower_pas.sem.pas_scope.defs[i].name && !strcmp(g_lower_pas.sem.pas_scope.defs[i].name, name))
        return g_lower_pas.sem.pas_scope.defs[i].sig ? &g_lower_pas.sem.pas_scope.defs[i] : NULL;
    return NULL;
}
int pas_pf_is_formal(const char *name) { const PasDef *d = pas_pf_lookup(name); return d && d->formal; }
tree_t *pas_xr_cvtn(int kind, tree_t *x) { return mk_fnc2("__pas_xr_cvt", ilit(kind), x); }
static pval E_program(const tree_t *n) { const void *pc = pty_enter(n); pval o = E_program_in(n); pty_leave(pc); return o; }
static const char *pas_vr_wrapper(const tree_t *m) {
    const pty_t *t = pty_of_src(m);
    const char *nm = t ? pty_name_of(t) : NULL;
    if (!t) return "__pc_vr_obj";
    if (t->k == PTK_INT || t->k == PTK_ENUM) return nm && !strcasecmp(nm, "int64") ? "__pc_vr_int64" : "__pc_vr_int";
    if (t->k == PTK_BOOL) return "__pc_vr_bool";
    if (t->k == PTK_CHAR) return "__pc_vr_char";
    if (t->k == PTK_REAL) return "__pc_vr_ext";
    if (t->k == PTK_STRING) return nm && !strcasecmp(nm, "shortstring") ? "__pc_vr_sstr" : "__pc_vr_str";
    if (t->k == PTK_POINTER && !pty_pointee(t)) return "__pc_vr_ptr";
    return "__pc_vr_obj";
}
static pval E_set_member(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SUBRANGE)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { pas_set_member_ordinal(v[1].node); pas_set_member_ordinal(v[3].node); out.node = mk_set_bin("__pas_setrange", v[1].node, v[3].node); }
        return out;
    }
    if (n != NULL && g_lower_pas.sem.pas_arrctor == 2 && pas_pf_lookup(pas_vr_wrapper(n))) {
        tree_t *w = ast_node_new(TT_FNC);
        ast_push(w, leaf_s(TT_VAR, pas_vr_wrapper(n)));
        ast_push(w, (tree_t *)n);
        { int _sv = g_lower_pas.sem.pas_arrctor; PNodeList *_l = pnl_new(); pnl_push(_l, E_expression(w).node); g_lower_pas.sem.pas_arrctor = _sv; out.node = mk_set_ctor(_l); }
        return out;
    }
    if (n != NULL) {
        v[1] = E_expression(n);
        { if (!g_lower_pas.sem.pas_arrctor) pas_set_member_ordinal(v[1].node); PNodeList *_l = pnl_new(); pnl_push(_l, v[1].node); out.node = mk_set_ctor(_l); }
        return out;
    }
    elab_bad("set_member", n);
    return out;
}
pval E_set_member_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_set_member(ce); { out.node = v[1].node; } } else { v[1] = out; v[3] = E_set_member(ce); { out.node = mk_set_bin("__pas_setuni", v[1].node, v[3].node); } }
    }
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pu_is_shortstring_type(const tree_t *c) { return IS(c, TT_IDX) && c->n == 2 && IS(c->c[0], TT_VAR) && c->c[0]->v.sval && !strcasecmp(c->c[0]->v.sval, "string"); }
static void pas_shortstrings(tree_t *t) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *c = t->c[i];
        int typepos = (IS(t, TT_DECL) && i >= 1) || ((IS(t, TT_ARRAY_TYPE) || IS(t, TT_FILE_TYPE) || IS(t, TT_SET_TYPE) || IS(t, TT_PTR_TYPE)) && i == t->n - 1);
        if (typepos && pu_is_shortstring_type(c)) {
            pty_note_shortstring(c->c[0], c->c[1]);
            t->c[i] = c->c[0];
        } else if (typepos && IS(c, TT_FIELD) && c->n == 2 && IS(c->c[0], TT_VAR) && IS(c->c[1], TT_VAR)) t->c[i] = c->c[1];
        else pas_shortstrings(c);
    }
}
static void pas_var_inits(tree_t *t) { pas_var_inits_in(t, NULL); }
static int pas_addr_of_routine(const tree_t *root, const tree_t *t) {
    if (!t) return 0;
    if (IS(t, TT_ADDR) && t->n == 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && pas_routine_named(root, t->c[0]->v.sval)) return 1;
    for (int i = 0; i < t->n; i++) if (pas_addr_of_routine(root, t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *lower_pascal_tree(tree_t *pruned, const char *filename) {
    if (!pruned) return NULL;
    if (!filename) filename = "<stdin>";
    memset(&g_lower_pas.sem, 0, sizeof g_lower_pas.sem);
    g_lower_pas.sem.pas_min_enum_size = 4;
    g_lower_pas.sem.pas_range_check_on = 2;
    g_lower_pas.sem.pas_pend_esz = -1;
    g_lower_pas.sem.pas_level = 1;
    g_lower_pas.sem.pas_pend_set_hi = -1;
    g_lower_pas.sem.pas_pend_frng_hi = -1;
    g_lower_pas.sem.pas_pend_arr_ncols = -1;
    g_lower_pas.sem.pas_pend_enum_max = -1;
    g_lower_pas.sem.pas_pend_sub_high = -1;
    g_lower_pas.sem.pas_fname_is_call = 0;
    g_lower_pas.sem.pas_limit_errors = 0;
    g_lower_pas.sem.pas_dialect_fpc = g_lower_pas.sem.pas_seen_mode_directive = pas_tree_has_event(pruned, "seen_mode") || pas_result_flips_dialect(pruned);
    if (!g_lower_pas.sem.pas_dialect_fpc) pas_iso_empty_strings(pruned);
    if (g_lower_pas.sem.pas_dialect_fpc) pruned = pas_sign_binds_factor(pruned);
    int _iofull = pas_user_ioflow(pruned), _su = pas_has_unit(pruned, "ioerr");
    g_lower_pas.sem.pas_iofull = _iofull;
    g_lower_pas.sem.pas_divchk = _su;
    g_lower_pas.sem.pas_ioflow = _iofull || _su;
    pruned = pas_generics_prepass(pruned);
    if (!pruned) return NULL;
    tree_t *flat = pas_flatten_units(pruned);
    pas_forin_types(flat, flat, NULL);
    pas_absolute_vars(flat);
    pas_lvalue_casts(flat, flat);
    pas_call_casts(flat, flat);
    pas_shortstrings(flat);
    flat = pas_classes_prepass(flat);
    pas_untyped_formals(flat);
    pas_defaults_prepass(flat);
    pas_param_prepass(flat);
    if (!g_lower_pas.sem.pas_mode_iso && pas_addr_of_routine(flat, flat)) g_lower_pas.sem.pas_seen_mode_directive = 1;
    g_lower_pas.sem.pas_ptrcell = pas_takes_address(flat, flat);
    g_lower_pas.sem.pas_ioquiet = 0;
    pas_var_inits(flat);
    pas_undef_vars_scan(flat);
    pty_run(flat, g_lower_pas.sem.pas_dialect_fpc);
    if (g_lower_pas.sem.pas_dialect_fpc) { pas_scope_define("returnnilifgrowheapfails"); pas_scalarvartype_add("returnnilifgrowheapfails", "boolean"); }
    tree_t *root = E_program(flat).node;
    pty_census_report();
    int nsem = pascal_sem_check(root, filename) + g_lower_pas.sem.pas_iso_errors, nlim = g_lower_pas.sem.pas_limit_errors;
    if (nlim > 0) fprintf(stderr, "pascal: %d static-storage limit error(s) in %s -- no code generated\n", nlim, filename);
    if (nsem > 0) fprintf(stderr, "pascal: %d ISO 7185 violation(s) in %s -- no code generated\n", nsem, filename);
    if (nsem + nlim > 0) return NULL;
    return root;
}
