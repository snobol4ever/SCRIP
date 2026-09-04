%define api.prefix {pascal_yy}
%code requires {
#include "ast.h"
#include "../snobol4/scrip_cc.h"
typedef struct PNodeList { tree_t **items; int count; int cap; } PNodeList;
}
%{
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
void pascal_yyerror(const char *msg) { fprintf(stderr, "pascal parse error line %d: %s\n", pascal_get_lineno(), msg); }
tree_t   *pascal_prog_result = NULL;
static PNodeList g_pascal_procs;
static PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)calloc(1, sizeof *l); return l; }
static void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)realloc(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
static PNodeList *pnl_concat(PNodeList *a, PNodeList *b) {
    if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a;
}
static tree_t *leaf_s(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)(s ? s : ""); return e; }
static tree_t *ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *flit(double v) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = v; return e; }
static tree_t *bin(tree_e k, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *un(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); ast_push(e, a); return e; }
static tree_t *mk_neg(tree_t *a) { if (a && a->t == TT_FLIT) { a->v.dval = -a->v.dval; return a; } return un(TT_MNS, a); }
static tree_t *prog_of(PNodeList *l) {
    tree_t *e = ast_node_new(TT_PROGRAM);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static tree_t *seq_of(PNodeList *l) {
    if (l && l->count == 1) return l->items[0];
    tree_t *e = ast_node_new(TT_SEQ_EXPR);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static const char *map_io(const char *fn) {
    if (fn && !strcmp(fn, "writeln")) return "__pas_writeln";
    if (fn && !strcmp(fn, "write"))   return "__pas_write";
    if (fn && !strcmp(fn, "sqr"))     return "__pas_sqr";
    return fn;
}
static int is_pas_io(const char *fn) {
    return fn && (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write"));
}
static const char *pas_ptrvar_target(const char *v);
static const char *pas_enumnames_by_idx(int i);
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_with_sel_rtype(tree_t *sel);
static int pas_rectype_nf(const char *rn);
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static int pas_is_filevar(const char *name);
static int pas_is_stdstream(const char *name);
static void pas_filevar_add(const char *name);
static int pas_is_chararr(const char *name);
static long long pas_chararr_lo(const char *name);
static int pas_is_strarr(const char *name);
static long long pas_strarr_lo(const char *name);
static tree_t *pas_alpha_wrap(tree_t *x);
static int pas_ca_is_read(const tree_t *e);
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high);
static unsigned long long pas_caparm_mask(const char *name);
static long long pas_caparm_lo(const char *name, int pos);
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo);
static int pas_is_rel(tree_t *e);
static int pas_is_proc(const char *name);
static tree_t *pas_bool(tree_t *e);
static tree_t *pas_tree_clone(tree_t *e);
static tree_t *mk_deref(tree_t *ptr) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr);
    return e;
}
static tree_t *mk_fnc0(const char *fn) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); return e; }
static tree_t *mk_fnc1(const char *fn, tree_t *a) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); return e; }
static tree_t *mk_call(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "ord") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) return a->c[1];
        if (a && a->t == TT_IDX) a->v.ival = 0;
        return a;
    }
    if (name && !strcmp(name, "chr") && args && args->count >= 1) return mk_fnc1("__pas_chrlit", args->items[0]);
    if (name && !strcmp(name, "pred") && args && args->count >= 1) return bin(TT_SUB, args->items[0], ilit(1));
    if (name && !strcmp(name, "succ") && args && args->count >= 1) return bin(TT_ADD, args->items[0], ilit(1));
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
    if (name && !strcmp(name, "round") && args && args->count >= 1) return mk_fnc1("__pas_round", args->items[0]);
    if (name && !strcmp(name, "halt") && (!args || args->count == 0)) return mk_fnc0("__pas_halt");
    if (name && !strcmp(name, "halt") && args && args->count >= 1) return mk_fnc1("__pas_halt", args->items[0]);
    if (name && !strcmp(name, "abs") && args && args->count >= 1) return mk_fnc1("__pas_abs", args->items[0]);
    if (name && !strcmp(name, "sin") && args && args->count >= 1) return mk_fnc1("__pas_sin", args->items[0]);
    if (name && !strcmp(name, "cos") && args && args->count >= 1) return mk_fnc1("__pas_cos", args->items[0]);
    if (name && !strcmp(name, "exp") && args && args->count >= 1) return mk_fnc1("__pas_exp", args->items[0]);
    if (name && !strcmp(name, "sqrt") && args && args->count >= 1) return mk_fnc1("__pas_sqrt", args->items[0]);
    if (name && !strcmp(name, "ln") && args && args->count >= 1) return mk_fnc1("__pas_ln", args->items[0]);
    if (name && !strcmp(name, "arctan") && args && args->count >= 1) return mk_fnc1("__pas_arctan", args->items[0]);
    if (name && !strcmp(name, "odd") && args && args->count >= 1) return bin(TT_NE, bin(TT_MOD, args->items[0], ilit(2)), ilit(0));
    if (name && !strcmp(name, "eof") && (!args || args->count == 0)) return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln") && (!args || args->count == 0)) return mk_fnc0("__pas_eoln");
    if (name && (!strcmp(name, "eof") || !strcmp(name, "eoln")) && args && args->count >= 1) {
        tree_t *fa = args->items[0]; int isf = fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval);
        int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        const char *base = !strcmp(name, "eof") ? "__pas_eof" : "__pas_eoln";
        if (isf && !isstd) { const char *fn2 = !strcmp(name, "eof") ? "__pas_eof_f" : "__pas_eoln_f"; return mk_fnc1(fn2, pas_tree_clone(fa)); }
        return mk_fnc0(base);
    }
    if (name && (!strcmp(name, "GetBufCh") || !strcmp(name, "getbufch")) && args && args->count >= 1) {
        tree_t *fa = args->items[0]; int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        if (fa && fa->t == TT_VAR && fa->v.sval && !isstd) return mk_fnc1("__pas_getbufch_f", pas_tree_clone(fa));
        return mk_fnc0("__pas_getbufch");
    }
    if (name && !strcmp(name, "readln") && (!args || args->count == 0)) return mk_fnc0("__pas_readln");
    if (name && (!strcmp(name, "readln") || !strcmp(name, "read")) && args && args->count >= 1) {
        int isln = !strcmp(name, "readln");
        int start = 0; tree_t *fstream = NULL;
        tree_t *fa = args->items[0];
        if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) { if (!pas_is_stdstream(fa->v.sval)) fstream = fa; start = 2; }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            int isc = v && ((v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) || pas_is_charexpr(v));
            if (fstream) { const char *rfn = isc ? "__pas_read_c_f" : "__pas_read_i_f"; pnl_push(stmts, mk_assign(v, mk_fnc1(rfn, pas_tree_clone(fstream)))); }
            else { const char *rfn = isc ? "__pas_read_c" : "__pas_read_i"; pnl_push(stmts, mk_assign(v, mk_fnc0(rfn))); }
        }
        if (isln) { if (fstream) pnl_push(stmts, mk_fnc1("__pas_readln_f", pas_tree_clone(fstream))); else pnl_push(stmts, mk_fnc0("__pas_readln")); }
        return seq_of(stmts);
    }
    if (name && (!strcmp(name, "ReadInt") || !strcmp(name, "readint")) && args && args->count >= 3) {
        tree_t *fa = args->items[0]; tree_t *fstream = NULL; int start = 0;
        if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) { if (!pas_is_stdstream(fa->v.sval)) fstream = fa; start = 2; }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            if (fstream) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_read_i_f", pas_tree_clone(fstream))));
            else pnl_push(stmts, mk_assign(v, mk_fnc0("__pas_read_i")));
        }
        return seq_of(stmts);
    }
    if (name && !strcmp(name, "assign") && args && args->count >= 3) {
        tree_t *fv = args->items[0]; tree_t *nm = args->items[2];
        return mk_assign(fv, mk_fnc1("__pas_fassign", nm));
    }
    if (name && !strcmp(name, "rewrite") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, mk_fnc1("__pas_reset", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "close") && args && args->count >= 1) {
        return mk_fnc1("__pas_fclose", args->items[0]);
    }
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv = args->items[0];
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, ilit(pas_rectype_nf(rt))); }
        else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        return mk_assign(pv, alloc);
    }
    if (name && !strcmp(name, "dispose") && args && args->count >= 1) {
        return mk_fnc1("__pas_dispose", args->items[0]);
    }
    tree_t *e = ast_node_new(TT_FNC);
    int _wstart = 0; tree_t *_wstream = NULL;
    if (is_pas_io(map_io(name)) && args && args->count >= 2) {
        tree_t *fa = args->items[0];
        if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval) && !pas_is_stdstream(fa->v.sval)) { _wstream = fa; _wstart = 2; }
        else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval)) { _wstart = 2; }
    }
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (_wstream) { ast_push(e, pas_tree_clone(_wstream)); ast_push(e, ilit(-1)); }
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = _wstart; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i]; tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val);
                const char *_enm = (val && val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
                if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); val = _w; }
                else if (is_char) { val = mk_chr_wrap(val); if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2); }
                else if (val && val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) { val = pas_alpha_wrap(val); }
                else if (pas_ca_is_read(val)) { val = pas_alpha_wrap(val); }
                else if (val && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(_w, val); ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval))); val = _w; }
                ast_push(e, val); ast_push(e, wid);
            }
        } else {
            unsigned long long _cam = pas_caparm_mask(name);
            for (int i = 0; i < args->count; i += 2) {
                tree_t *val = args->items[i]; int _pidx = i / 2;
                if (val && val->t == TT_QLIT && val->v.sval && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) { long long _lo = pas_caparm_lo(name, _pidx); val = pas_str_to_alpha(val->v.sval, _lo, _lo + (long long)strlen(val->v.sval) - 1); }
                ast_push(e, val);
            }
        }
    }
    return e;
}
static tree_t *mk_in(tree_t *elem, tree_t *set) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_in")); ast_push(e, elem); ast_push(e, set);
    return e;
}
static tree_t *mk_set_ctor(PNodeList *elems) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_set"));
    if (elems) for (int i = 0; i < elems->count; i++) ast_push(e, elems->items[i]);
    return e;
}
static void emit_proc(PNodeList *procs, tree_t *proc) {
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
static tree_t *mk_array_fill(long long high);
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high);
static tree_t *mk_array_init(const char *name, long long high);
static int pas_array_high_get(const char *name, long long *out);
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int _li = 0; lnames && _li < lcount; _li++) { long long _hi; if (lnames[_li] && pas_array_high_get(lnames[_li], &_hi)) ast_push(body_prog, bin(TT_ASSIGN, leaf_s(TT_VAR, lnames[_li]), mk_array_init(lnames[_li], _hi))); }
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); }
    else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    unsigned long long camask = 0; long long calo[64]; for (int _z = 0; _z < 64; _z++) calo[_z] = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { if (i < 64) byref |= (1LL << i); pv->n = 0; }
        if (pv && pv->v.sval && i < 64 && pas_is_chararr(pv->v.sval)) { camask |= (1ULL << i); calo[i] = pas_chararr_lo(pv->v.sval); }
        ast_push(vlist, pv);
    }
    if (camask) pas_caparm_add(name, camask, calo);
    vlist->v.ival = byref;
    ast_push(proc, vlist);
    ast_push(proc, body_prog);
    if (is_function) ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *locals = ast_node_new(TT_VLIST);
    locals->v.ival = decl_level;
    for (int i = 0; i < lcount; i++) if (lnames[i]) ast_push(locals, leaf_s(TT_VAR, lnames[i]));
    ast_push(proc, locals);
    return proc;
}
static struct { const char *name; } g_pas_funcs[256]; static int g_pas_nfunc;
static void pas_func_add(const char *name) { if (g_pas_nfunc < 256 && name) { g_pas_funcs[g_pas_nfunc].name = strdup(name); g_pas_nfunc++; } }
static int pas_is_func(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nfunc; i++) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return 1; return 0; }
static struct { const char *name; } g_pas_procs[256]; static int g_pas_nproc;
static void pas_proc_add(const char *name) { if (g_pas_nproc < 256 && name) { g_pas_procs[g_pas_nproc].name = strdup(name); g_pas_nproc++; } }
static int pas_is_proc(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nproc; i++) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) return 1; return 0; }
static struct { char *name; unsigned long long camask; long long lo[64]; } g_pas_caparm[256]; static int g_pas_ncaparm;
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo) { if (g_pas_ncaparm < 256 && name && m) { g_pas_caparm[g_pas_ncaparm].name = strdup(name); g_pas_caparm[g_pas_ncaparm].camask = m; for (int i = 0; i < 64; i++) g_pas_caparm[g_pas_ncaparm].lo[i] = lo ? lo[i] : 0; g_pas_ncaparm++; } }
static unsigned long long pas_caparm_mask(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncaparm; i++) if (g_pas_caparm[i].name && !strcmp(g_pas_caparm[i].name, name)) return g_pas_caparm[i].camask; return 0; }
static long long pas_caparm_lo(const char *name, int pos) { if (!name || pos < 0 || pos >= 64) return 0; for (int i = 0; i < g_pas_ncaparm; i++) if (g_pas_caparm[i].name && !strcmp(g_pas_caparm[i].name, name)) return g_pas_caparm[i].lo[pos]; return 0; }
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
static int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; double val; } g_pas_rconsts[64]; static int g_pas_nrconst;
static void pas_rconst_add(const char *name, double v) { if (g_pas_nrconst < 64 && name) { g_pas_rconsts[g_pas_nrconst].name = strdup(name); g_pas_rconsts[g_pas_nrconst].val = v; g_pas_nrconst++; } }
static int pas_rconst_get(const char *name, double *out) { if (!name) return 0; for (int i = 0; i < g_pas_nrconst; i++) if (g_pas_rconsts[i].name && !strcmp(g_pas_rconsts[i].name, name)) { *out = g_pas_rconsts[i].val; return 1; } return 0; }
static struct { char *name; char *val; } g_pas_sconsts[64]; static int g_pas_nsconst;
static void pas_sconst_add(const char *name, const char *v) { if (g_pas_nsconst < 64 && name && v) { g_pas_sconsts[g_pas_nsconst].name = strdup(name); g_pas_sconsts[g_pas_nsconst].val = strdup(v); g_pas_nsconst++; } }
static const char *pas_sconst_get(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsconst; i++) if (g_pas_sconsts[i].name && !strcmp(g_pas_sconsts[i].name, name)) return g_pas_sconsts[i].val; return 0; }
static int g_pas_level = 1;
static struct { char *name; long long high; long long ncols; int is_param; int is_local; } g_pas_arrays[256]; static int g_pas_narray; static long long g_pas_pend_arr_ncols;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = -1; g_pas_arrays[g_pas_narray].is_param = 0; g_pas_arrays[g_pas_narray].is_local = (g_pas_level >= 2); g_pas_narray++; } }
static void pas_array_add2d(const char *name, long long high, long long ncols) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = ncols; g_pas_arrays[g_pas_narray].is_param = 0; g_pas_arrays[g_pas_narray].is_local = (g_pas_level >= 2); g_pas_narray++; } }
static void pas_array_add2d_param(const char *name, long long high, long long ncols) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = ncols; g_pas_arrays[g_pas_narray].is_param = 1; g_pas_arrays[g_pas_narray].is_local = 0; g_pas_narray++; } }
static long long pas_array_ncols(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return g_pas_arrays[i].ncols; return -1; }
int pas_is_agg_local(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && g_pas_arrays[i].is_local && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) return 1; return 0; }
static int pas_array_high_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) { *out = g_pas_arrays[i].high; return 1; } return 0; }
static long long g_pas_pend_sub_low;
static struct { char *name; long long high; int ndim2; long long ncols; int ischar; long long lo; } g_pas_arrtypes[64]; static int g_pas_narrtype; static int g_pas_pend_arr_ischar; static int g_pas_pend_arr_wrap;
static void pas_arrtype_add(const char *name, long long high, int ndim2, long long ncols) { if (g_pas_narrtype < 64 && name) { g_pas_arrtypes[g_pas_narrtype].name = strdup(name); g_pas_arrtypes[g_pas_narrtype].high = high; g_pas_arrtypes[g_pas_narrtype].ndim2 = ndim2; g_pas_arrtypes[g_pas_narrtype].ncols = ncols; g_pas_arrtypes[g_pas_narrtype].ischar = g_pas_pend_arr_ischar; g_pas_arrtypes[g_pas_narrtype].lo = (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0); g_pas_narrtype++; } }
static long long pas_arrtype_high(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].high; return -1; }
static long long pas_arrtype_ncols(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].ndim2 ? g_pas_arrtypes[i].ncols : -1; return -1; }
static long long pas_arrtype_lo(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].lo; return 0; }
static int pas_arrtype_ischar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].ischar; return 0; }
static struct { char *name; long long high; } g_pas_enumtypes[64]; static int g_pas_nenum; static long long g_pas_pend_enum_max;
static void pas_enumtype_add(const char *n, long long h) { if (g_pas_nenum < 64 && n) { g_pas_enumtypes[g_pas_nenum].name = strdup(n); g_pas_enumtypes[g_pas_nenum].high = h; g_pas_nenum++; } }
static long long pas_enumtype_high(const char *n) { if (!n) return -1; for (int i = 0; i < g_pas_nenum; i++) if (g_pas_enumtypes[i].name && !strcmp(g_pas_enumtypes[i].name, n)) return g_pas_enumtypes[i].high; return -1; }
static struct { char *tname; char *names; } g_pas_enumnames[64]; static int g_pas_nenumname; static char g_pas_pend_enum_names[512];
static void pas_enumnames_add(const char *tn, const char *names) { if (g_pas_nenumname < 64 && tn && names && names[0]) { g_pas_enumnames[g_pas_nenumname].tname = strdup(tn); g_pas_enumnames[g_pas_nenumname].names = strdup(names); g_pas_nenumname++; } }
static int pas_enumnames_idx(const char *tn) { if (!tn) return -1; for (int i = 0; i < g_pas_nenumname; i++) if (g_pas_enumnames[i].tname && !strcmp(g_pas_enumnames[i].tname, tn)) return i; return -1; }
static const char *pas_enumnames_by_idx(int i) { return (i >= 0 && i < g_pas_nenumname) ? g_pas_enumnames[i].names : NULL; }
static struct { char *aname; char *etype; } g_pas_enumarrs[128]; static int g_pas_nenumarr;
static void pas_enumarr_add(const char *a, const char *et) { if (g_pas_nenumarr < 128 && a && et) { g_pas_enumarrs[g_pas_nenumarr].aname = strdup(a); g_pas_enumarrs[g_pas_nenumarr].etype = strdup(et); g_pas_nenumarr++; } }
static const char *pas_enumarr_get(const char *a) { if (!a) return NULL; for (int i = 0; i < g_pas_nenumarr; i++) if (g_pas_enumarrs[i].aname && !strcmp(g_pas_enumarrs[i].aname, a)) return g_pas_enumarrs[i].etype; return NULL; }
static struct { char *name; long long low; long long high; } g_pas_subtypes[64]; static int g_pas_nsubtype; static long long g_pas_pend_sub_low; static long long g_pas_pend_sub_high;
static void pas_subtype_add(const char *n, long long lo, long long hi) { if (g_pas_nsubtype < 64 && n) { g_pas_subtypes[g_pas_nsubtype].name = strdup(n); g_pas_subtypes[g_pas_nsubtype].low = lo; g_pas_subtypes[g_pas_nsubtype].high = hi; g_pas_nsubtype++; } }
static long long pas_subtype_high(const char *n) { if (!n) return -1; for (int i = 0; i < g_pas_nsubtype; i++) if (g_pas_subtypes[i].name && !strcmp(g_pas_subtypes[i].name, n)) return g_pas_subtypes[i].high; return -1; }
#define PAS_REC_MAX 512
#define PAS_FIELD_MAX 32
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; char *fldenum[PAS_FIELD_MAX]; char *fldrec[PAS_FIELD_MAX]; int fldca[PAS_FIELD_MAX]; long long fldca_lo[PAS_FIELD_MAX]; long long fldca_hi[PAS_FIELD_MAX]; int fldchar[PAS_FIELD_MAX]; int nf; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct { char *vname; char *fields[PAS_FIELD_MAX]; int nf; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static char *g_pas_pend_fldenum[PAS_FIELD_MAX]; static char *g_pas_pend_fldrec[PAS_FIELD_MAX]; static int g_pas_pend_fldca[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_lo[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_hi[PAS_FIELD_MAX]; static int g_pas_pend_fldchar[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static int g_pas_recbody_depth;
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename; static int g_pas_pend_ischar;
static struct { char *pname; char *rname; } g_pas_ptrtypes[PAS_REC_MAX]; static int g_pas_nptrtype;
static void pas_ptrtype_add(const char *p, const char *r) { if (g_pas_nptrtype < PAS_REC_MAX && p && r) { int k = g_pas_nptrtype++; g_pas_ptrtypes[k].pname = strdup(p); g_pas_ptrtypes[k].rname = strdup(r); } }
static const char *pas_ptrtype_target(const char *p) { if (!p) return NULL; for (int i = 0; i < g_pas_nptrtype; i++) if (g_pas_ptrtypes[i].pname && !strcmp(g_pas_ptrtypes[i].pname, p)) return g_pas_ptrtypes[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_arrptr[PAS_REC_MAX]; static int g_pas_narrptr; static char *g_pas_pend_arr_ptrto;
static void pas_arrptr_add(const char *v, const char *r) { if (g_pas_narrptr < PAS_REC_MAX && v && r) { int k = g_pas_narrptr++; g_pas_arrptr[k].vname = strdup(v); g_pas_arrptr[k].rname = strdup(r); } }
static const char *pas_arrptr_target(const char *v) { if (!v) return NULL; for (int i = g_pas_narrptr - 1; i >= 0; i--) if (g_pas_arrptr[i].vname && !strcmp(g_pas_arrptr[i].vname, v)) return g_pas_arrptr[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_ptrvars[PAS_REC_MAX]; static int g_pas_nptrvar;
static void pas_ptrvar_add(const char *v, const char *r) { if (g_pas_nptrvar < PAS_REC_MAX && v && r) { int k = g_pas_nptrvar++; g_pas_ptrvars[k].vname = strdup(v); g_pas_ptrvars[k].rname = strdup(r); } }
static const char *pas_ptrvar_target(const char *v) { if (!v) return NULL; for (int i = g_pas_nptrvar - 1; i >= 0; i--) if (g_pas_ptrvars[i].vname && !strcmp(g_pas_ptrvars[i].vname, v)) return g_pas_ptrvars[i].rname; return NULL; }
#define PAS_NEST_MAX_PV 16
static int g_pas_pvmarks[PAS_NEST_MAX_PV]; static int g_pas_npvmark;
static void pas_ptrvar_mark(void) { if (g_pas_npvmark < PAS_NEST_MAX_PV) g_pas_pvmarks[g_pas_npvmark++] = g_pas_nptrvar; }
static void pas_ptrvar_release(void) { if (g_pas_npvmark > 0) g_pas_nptrvar = g_pas_pvmarks[--g_pas_npvmark]; }
static struct { char *pname; char *vnames[16]; char *rnames[16]; int n; } g_pas_fwdpv[64]; static int g_pas_nfwdpv;
static void pas_fwd_save(const char *pn) { if (!pn || g_pas_nfwdpv >= 64 || g_pas_npvmark == 0) return; int from = g_pas_pvmarks[g_pas_npvmark - 1]; int k = g_pas_nfwdpv++; g_pas_fwdpv[k].pname = strdup(pn); g_pas_fwdpv[k].n = 0; for (int i = from; i < g_pas_nptrvar && g_pas_fwdpv[k].n < 16; i++) { g_pas_fwdpv[k].vnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].vname; g_pas_fwdpv[k].rnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].rname; g_pas_fwdpv[k].n++; } }
static void pas_fwd_restore(const char *pn) { if (!pn) return; for (int i = 0; i < g_pas_nfwdpv; i++) if (g_pas_fwdpv[i].pname && !strcmp(g_pas_fwdpv[i].pname, pn)) { for (int j = 0; j < g_pas_fwdpv[i].n; j++) pas_ptrvar_add(g_pas_fwdpv[i].vnames[j], g_pas_fwdpv[i].rnames[j]); return; } }
static void pas_pend_reset(void) { g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ptrto = NULL; g_pas_pend_typename = NULL; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; g_pas_pend_enum_max = -1; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_wrap = 0; }
static void pas_pend_add(const char *f) { if (g_pas_pend_nf < PAS_FIELD_MAX && f) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_pend_ptrtarget; g_pas_pend_fldenum[g_pas_pend_nf] = (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) ? g_pas_pend_typename : NULL; g_pas_pend_fldrec[g_pas_pend_nf] = (g_pas_pend_typename && pas_rectype_nf(g_pas_pend_typename) > 0) ? g_pas_pend_typename : NULL;
    { int _ica = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename));
      long long _lo = g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : (g_pas_pend_typename ? pas_arrtype_lo(g_pas_pend_typename) : 0);
      long long _hi = g_pas_pend_arr_ischar ? g_pas_pend_sub_high : (g_pas_pend_typename ? pas_arrtype_high(g_pas_pend_typename) : -1);
      g_pas_pend_fldca[g_pas_pend_nf] = _ica; g_pas_pend_fldca_lo[g_pas_pend_nf] = _lo; g_pas_pend_fldca_hi[g_pas_pend_nf] = _hi; }
    g_pas_pend_fldchar[g_pas_pend_nf] = g_pas_pend_ischar; g_pas_pend_fields[g_pas_pend_nf++] = strdup(f); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ischar = 0; g_pas_pend_ischar = 0; } }
static void pas_rectype_add(const char *tn) { if (g_pas_nrectype >= PAS_REC_MAX || !tn) return; int k = g_pas_nrectype++; g_pas_rectypes[k].tname = strdup(tn); g_pas_rectypes[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_rectypes[k].fields[i] = g_pas_pend_fields[i]; g_pas_rectypes[k].fldptrto[i] = g_pas_pend_fldptrto[i]; g_pas_rectypes[k].fldenum[i] = g_pas_pend_fldenum[i]; g_pas_rectypes[k].fldrec[i] = g_pas_pend_fldrec[i]; g_pas_rectypes[k].fldca[i] = g_pas_pend_fldca[i]; g_pas_rectypes[k].fldca_lo[i] = g_pas_pend_fldca_lo[i]; g_pas_rectypes[k].fldca_hi[i] = g_pas_pend_fldca_hi[i]; g_pas_rectypes[k].fldchar[i] = g_pas_pend_fldchar[i]; } }
static int pas_rectype_to_pend(const char *tn) { if (!tn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, tn)) {
    pas_pend_reset(); for (int j = 0; j < g_pas_rectypes[i].nf; j++) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_rectypes[i].fldptrto[j]; g_pas_pend_fldenum[g_pas_pend_nf] = g_pas_rectypes[i].fldenum[j]; g_pas_pend_fldrec[g_pas_pend_nf] = g_pas_rectypes[i].fldrec[j]; g_pas_pend_fldca[g_pas_pend_nf] = g_pas_rectypes[i].fldca[j]; g_pas_pend_fldca_lo[g_pas_pend_nf] = g_pas_rectypes[i].fldca_lo[j]; g_pas_pend_fldca_hi[g_pas_pend_nf] = g_pas_rectypes[i].fldca_hi[j]; g_pas_pend_fldchar[g_pas_pend_nf] = g_pas_rectypes[i].fldchar[j]; g_pas_pend_fields[g_pas_pend_nf++] = g_pas_rectypes[i].fields[j]; } return 1; } return 0; }
static int pas_rectype_field_index(const char *rn, const char *fn) { if (!rn || !fn) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    for (int j = 0; j < g_pas_rectypes[i].nf; j++) if (g_pas_rectypes[i].fields[j] && !strcmp(g_pas_rectypes[i].fields[j], fn)) return j; return -1; } return -1; }
static int pas_rectype_nf(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return g_pas_rectypes[i].nf; return 0; }
static int pas_rectype_field_is_ca(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca[idx]; return 0; } return 0; }
static int pas_rectype_field_is_char(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldchar[idx]; return 0; } return 0; }
static long long pas_rectype_field_ca_lo(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca_lo[idx]; return 0; } return 0; }
static long long pas_rectype_field_ca_hi(const char *rn, long idx) { if (!rn || idx < 0) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca_hi[idx]; return -1; } return -1; }
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldptrto[idx]; return NULL; } return NULL; }
static const char *pas_rectype_field_enum_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldenum[idx]; return NULL; } return NULL; }
static const char *pas_rectype_field_rectype_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldrec[idx]; return NULL; } return NULL; }
static void pas_recvar_add(const char *vn) { if (g_pas_nrecvar >= PAS_REC_MAX || !vn || g_pas_pend_nf == 0) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = strdup(vn); g_pas_recvars[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; }
static void pas_recvar_add_from_type(const char *vn, const char *tn) { if (!vn || !tn) return; for (int ri = 0; ri < g_pas_nrectype; ri++) { if (!g_pas_rectypes[ri].tname || strcmp(g_pas_rectypes[ri].tname, tn)) continue; if (g_pas_nrecvar >= PAS_REC_MAX) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = strdup(vn); g_pas_recvars[k].nf = g_pas_rectypes[ri].nf; for (int j = 0; j < g_pas_rectypes[ri].nf; j++) g_pas_recvars[k].fields[j] = g_pas_rectypes[ri].fields[j]; return; } }
static int pas_recvar_field_index(const char *vn, const char *fn) { if (!vn || !fn) return -1; for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) {
    for (int j = 0; j < g_pas_recvars[i].nf; j++) if (g_pas_recvars[i].fields[j] && !strcmp(g_pas_recvars[i].fields[j], fn)) return j; return -1; } return -1; }
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_selector_rectype(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return NULL; }
static const char *pas_ptrexpr_target(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) { const char *at = pas_arrptr_target(e->c[0]->v.sval); if (at) return at; }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) { const char *rt = pas_with_sel_rtype(e->c[0]); if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival); }
    return NULL; }
#define PAS_LOCAL_MAX 64
#define PAS_NEST_MAX  16
static struct { const char *names[PAS_LOCAL_MAX]; int n; int decl_level; } g_pas_lstk[PAS_NEST_MAX]; static int g_pas_ldepth;
static void pas_proc_enter(void) { if (g_pas_ldepth < PAS_NEST_MAX) { g_pas_lstk[g_pas_ldepth].n = 0; g_pas_lstk[g_pas_ldepth].decl_level = g_pas_level; } g_pas_ldepth++; g_pas_level++; }
static void pas_proc_exit(void) { if (g_pas_ldepth > 0) g_pas_ldepth--; if (g_pas_level > 1) g_pas_level--; }
static void pas_local_add(const char *name) { if (g_pas_level < 2 || g_pas_ldepth == 0 || g_pas_ldepth > PAS_NEST_MAX || !name) return; int d = g_pas_ldepth - 1; if (g_pas_lstk[d].n < PAS_LOCAL_MAX) g_pas_lstk[d].names[g_pas_lstk[d].n++] = strdup(name); }
static struct { char *name; } g_pas_setvars[256]; static int g_pas_nsetvar;
static struct { char *name; } g_pas_settypes[64]; static int g_pas_nsettype;
static void pas_settype_add(const char *name) { if (g_pas_nsettype < 64 && name) { g_pas_settypes[g_pas_nsettype++].name = strdup(name); } }
static int pas_is_settype(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsettype; i++) if (g_pas_settypes[i].name && !strcmp(g_pas_settypes[i].name, name)) return 1; return 0; }
static void pas_setvar_add(const char *name) { if (g_pas_nsetvar < 256 && name) { g_pas_setvars[g_pas_nsetvar++].name = strdup(name); } }
static int pas_is_setvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsetvar; i++) if (g_pas_setvars[i].name && !strcmp(g_pas_setvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_charvars[256]; static int g_pas_ncharvar;
static void pas_charvar_add(const char *name) { if (g_pas_ncharvar < 256 && name) { g_pas_charvars[g_pas_ncharvar++].name = strdup(name); } }
static int pas_is_charvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncharvar; i++) if (g_pas_charvars[i].name && !strcmp(g_pas_charvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_filevars[256]; static int g_pas_nfilevar;
static void pas_filevar_add(const char *name) { if (g_pas_nfilevar < 256 && name) { for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return; g_pas_filevars[g_pas_nfilevar++].name = strdup(name); } }
static int pas_is_filevar(const char *name) { if (!name) return 0; if (!strcmp(name, "input") || !strcmp(name, "output")) return 1; for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return 1; return 0; }
static int pas_is_stdstream(const char *name) { return name && (!strcmp(name, "input") || !strcmp(name, "output")); }
static char *g_pas_hdrfiles[32]; static int g_pas_nhdrfile;
static struct { char *name; long long lo; } g_pas_chararrs[256]; static int g_pas_nchararr;
static void pas_chararr_add2(const char *name, long long lo) { if (g_pas_nchararr < 256 && name) { g_pas_chararrs[g_pas_nchararr].name = strdup(name); g_pas_chararrs[g_pas_nchararr].lo = lo; g_pas_nchararr++; } }
static void pas_chararr_add(const char *name) { pas_chararr_add2(name, 0); }
static long long pas_chararr_lo(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nchararr; i++) if (g_pas_chararrs[i].name && !strcmp(g_pas_chararrs[i].name, name)) return g_pas_chararrs[i].lo; return 0; }
static int pas_is_chararr(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nchararr; i++) if (g_pas_chararrs[i].name && !strcmp(g_pas_chararrs[i].name, name)) return 1; return 0; }
static tree_t *g_pas_cafield_marks[2048]; static long long g_pas_cafield_lo[2048]; static long long g_pas_cafield_hi[2048]; static int g_pas_ncafield = 0;
static void pas_cafield_mark_add(tree_t *e, long long lo, long long hi) { if (g_pas_ncafield < 2048 && e) { g_pas_cafield_marks[g_pas_ncafield] = e; g_pas_cafield_lo[g_pas_ncafield] = lo; g_pas_cafield_hi[g_pas_ncafield] = hi; g_pas_ncafield++; } }
static int pas_is_cafield(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return 1; return 0; }
static long long pas_cafield_lo_get(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return g_pas_cafield_lo[i]; return 0; }
static long long pas_cafield_hi_get(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return g_pas_cafield_hi[i]; return -1; }
static tree_t *g_pas_cvfield_marks[2048]; static int g_pas_ncvfield = 0;
static void pas_cvfield_mark_add(tree_t *e) { if (g_pas_ncvfield < 2048 && e) g_pas_cvfield_marks[g_pas_ncvfield++] = e; }
static int pas_is_cvfield(const tree_t *e) { for (int i = 0; i < g_pas_ncvfield; i++) if (g_pas_cvfield_marks[i] == (tree_t *)e) return 1; return 0; }
static int pas_ca_is_read(const tree_t *e) { return e && e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_ca_unpack"); }
static struct { char *name; long long lo; } g_pas_strarrs[128]; static int g_pas_nstrarr;
static void pas_strarr_add2(const char *name, long long lo) { if (g_pas_nstrarr < 128 && name) { g_pas_strarrs[g_pas_nstrarr].name = strdup(name); g_pas_strarrs[g_pas_nstrarr].lo = lo; g_pas_nstrarr++; } }
static void pas_strarr_add(const char *name) { pas_strarr_add2(name, 1); }
static int pas_is_strarr(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nstrarr; i++) if (g_pas_strarrs[i].name && !strcmp(g_pas_strarrs[i].name, name)) return 1; return 0; }
static long long pas_strarr_lo(const char *name) { if (!name) return 1; for (int i = 0; i < g_pas_nstrarr; i++) if (g_pas_strarrs[i].name && !strcmp(g_pas_strarrs[i].name, name)) return g_pas_strarrs[i].lo; return 1; }
static struct { char *aname; char *rname; int nf; char *fields[32]; char *fldenum[32]; int fldchar[32]; } g_pas_arrrecs[128]; static int g_pas_narrrec;
static void pas_arrrec_add(const char *a, const char *r, int nf) { if (g_pas_narrrec < 128 && a && nf > 0) { g_pas_arrrecs[g_pas_narrrec].aname = strdup(a); g_pas_arrrecs[g_pas_narrrec].rname = r ? strdup(r) : NULL; g_pas_arrrecs[g_pas_narrrec].nf = nf; for (int _i = 0; _i < nf && _i < 32; _i++) { g_pas_arrrecs[g_pas_narrrec].fields[_i] = g_pas_pend_fields[_i] ? strdup(g_pas_pend_fields[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldenum[_i] = g_pas_pend_fldenum[_i] ? strdup(g_pas_pend_fldenum[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldchar[_i] = g_pas_pend_fldchar[_i]; } g_pas_narrrec++; } }
static const char *pas_arrrec_field_enum(const char *a, long idx) { if (!a || idx < 0 || idx >= 32) return NULL; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (idx < g_pas_arrrecs[i].nf) return g_pas_arrrecs[i].fldenum[idx]; return NULL; } return NULL; }
static int pas_arrrec_field_is_char(const char *a, long idx) { if (!a || idx < 0 || idx >= 32) return 0; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (idx < g_pas_arrrecs[i].nf) return g_pas_arrrecs[i].fldchar[idx]; return 0; } return 0; }
static int pas_arrrec_find(const char *a, const char **rn) { if (!a) return 0; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (rn) *rn = g_pas_arrrecs[i].rname; return g_pas_arrrecs[i].nf; } return 0; }
static int pas_arrrec_field_index(const char *a, const char *fn) { if (!a || !fn) return -1; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { for (int j = 0; j < g_pas_arrrecs[i].nf && j < 32; j++) if (g_pas_arrrecs[i].fields[j] && !strcmp(g_pas_arrrecs[i].fields[j], fn)) return j; } return -1; }
static tree_t *pas_arrrec_flatten(tree_t *idxsel, long long fi) {
    tree_t *base = idxsel->c[0]; tree_t *sub = idxsel->c[1];
    tree_t *flat = bin(TT_ADD, bin(TT_MUL, sub, ilit(pas_arrrec_find(base->v.sval, NULL))), ilit(fi));
    tree_t *e = ast_node_new(TT_IDX); ast_push(e, base); ast_push(e, flat); return e;
}
static tree_t *mk_chr_wrap(tree_t *e) { tree_t *r = ast_node_new(TT_FNC); ast_push(r, leaf_s(TT_VAR, "__pas_chr")); ast_push(r, e); return r; }
static int pas_is_charexpr(tree_t *e) { if (!e) return 0; if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1; if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_charvar(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_chararr(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 2 && e->c[0] && pas_is_cafield(e->c[0])) return 1; if (pas_is_cvfield(e)) return 1; return 0; }
static int pas_is_strtyped(tree_t *e) { if (!e) return 0; if (e->t == TT_QLIT) return 1; if (pas_ca_is_read(e)) return 1; if (e->t == TT_VAR && e->v.sval && pas_is_chararr(e->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_is_chararr(e->c[0]->v.sval) || pas_is_strarr(e->c[0]->v.sval))) return 1; return 0; }
static tree_t *pas_alpha_wrap(tree_t *x) { if (x && x->t == TT_VAR && x->v.sval && pas_is_chararr(x->v.sval)) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(f, x); ast_push(f, ilit(pas_chararr_lo(x->v.sval))); return f; } if (x && x->t == TT_IDX && x->n >= 2 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && pas_is_strarr(x->c[0]->v.sval)) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(f, x); ast_push(f, ilit(pas_strarr_lo(x->c[0]->v.sval))); return f; } if (pas_ca_is_read(x)) { long long _lo = (x->n >= 3 && x->c[2]) ? x->c[2]->v.ival : 0; tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(f, x); ast_push(f, ilit(_lo)); return f; } return x; }
static int pas_is_strval(tree_t *e) { if (!e) return 0; if (pas_ca_is_read(e)) return 1; if (e->t == TT_VAR && e->v.sval && (pas_is_chararr(e->v.sval) || pas_is_strarr(e->v.sval))) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_strarr(e->c[0]->v.sval)) return 1; return 0; }
static tree_t *mk_set_bin(const char *fn, tree_t *a, tree_t *b);
static tree_t *pas_rel(tree_e tt, tree_t *a, tree_t *b) { int lex = pas_is_strval(a) || pas_is_strval(b); if (pas_is_strtyped(a) || pas_is_strtyped(b)) { a = pas_alpha_wrap(a); b = pas_alpha_wrap(b); } return lex ? bin(tt, mk_set_bin("__pas_strcmp", a, b), ilit(0)) : bin(tt, a, b); }
static int pas_is_setexpr(tree_t *e);
static tree_t *pas_rel_or_set(tree_e tt, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : pas_rel(tt, a, b); }
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
#define PAS_WITH_MAX 8
static struct { tree_t *sel; const char *rtype; } g_with_stk[PAS_WITH_MAX]; static int g_with_depth;
static tree_t *pas_tree_clone(tree_t *e) { if (!e) return NULL; tree_t *c = ast_node_new(e->t); c->v = e->v; if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = strdup(e->v.sval); for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i])); return c; }
static const char *pas_with_sel_rtype(tree_t *sel) { if (!sel) return NULL; if (sel->t == TT_VAR && sel->v.sval) { for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, sel->v.sval)) { const char *rt = NULL; for (int j = 0; j < g_pas_nrectype; j++) { int match = 1; if (!g_pas_rectypes[j].tname) continue; if (g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { match = 0; break; } if (match) { rt = g_pas_rectypes[j].tname; break; } } if (!rt) { for (int j = 0; j < g_pas_nrectype; j++) { int all = 1; if (!g_pas_rectypes[j].tname || g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { all = 0; break; } if (all) { rt = g_pas_rectypes[j].tname; break; } } } return rt ? rt : g_pas_recvars[i].fields[0] ? g_pas_rectypes[0].tname : NULL; } } if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) { const char *ptn = pas_ptrexpr_target(sel->c[1]); return ptn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval) { const char *_arn = NULL; if (pas_arrrec_find(sel->c[0]->v.sval, &_arn) > 0 && _arn) return _arn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[1] && sel->c[1]->t == TT_ILIT) { const char *bt = pas_with_sel_rtype(sel->c[0]); if (bt) { const char *_fr = pas_rectype_field_rectype_by_index(bt, sel->c[1]->v.ival); if (_fr) return _fr; return pas_rectype_field_ptrto_by_index(bt, sel->c[1]->v.ival); } } return NULL; }
static int pas_with_field_index(const char *rtype, const char *fname) { return pas_rectype_field_index(rtype, fname); }
static int pas_with_recvar_field(const char *vname, const char *fname) { return pas_recvar_field_index(vname, fname); }
static void pas_with_push(tree_t *sel) { if (g_with_depth >= PAS_WITH_MAX || !sel) return; const char *rt = pas_with_sel_rtype(sel); g_with_stk[g_with_depth].sel = sel; g_with_stk[g_with_depth].rtype = rt; g_with_depth++; }
static void pas_with_pop(void) { if (g_with_depth > 0) g_with_depth--; }
static int pas_is_setexpr(tree_t *e) { if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) return pas_is_setvar(e->v.sval);
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval) { const char *f = e->c[0]->v.sval;
        return !strcmp(f, "__pas_set") || !strcmp(f, "__pas_setuni") || !strcmp(f, "__pas_setint") || !strcmp(f, "__pas_setdif"); }
    return 0; }
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : bin(ak, a, b); }
static tree_t *g_pas_nrec_marks[512]; static int g_pas_nrec_nmarks = 0;
static void pas_nrec_mark_add(tree_t *e) { if (g_pas_nrec_nmarks < 512 && e) g_pas_nrec_marks[g_pas_nrec_nmarks++] = e; }
int pas_is_nrec_idx(const tree_t *e) { for (int i = 0; i < g_pas_nrec_nmarks; i++) if (g_pas_nrec_marks[i] == (tree_t *)e) return 1; return 0; }
static tree_t *pas_nested_field_resolve(tree_t *base, const char *fld) {
    const char *_brt = pas_with_sel_rtype(base);
    if (_brt) { int _nfi = pas_rectype_field_index(_brt, fld); if (_nfi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, base); ast_push(e, ilit(_nfi)); const char *_fe = pas_rectype_field_enum_by_index(_brt, _nfi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } pas_nrec_mark_add(e); return e; } }
    return bin(TT_FIELD, base, leaf_s(TT_VAR, fld));
}
static int pas_recspan_nf(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && !pas_is_nrec_idx(e)) { int nf = pas_arrrec_find(e->c[0]->v.sval, NULL); if (nf > 1) return nf; }
    if (e->t == TT_VAR && e->v.sval) { for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, e->v.sval)) return g_pas_recvars[i].nf; }
    return 0;
}
static tree_t *pas_recspan_slot(tree_t *e, long long fi) {
    if (e->t == TT_IDX && e->n == 2) return pas_arrrec_flatten(pas_tree_clone(e), fi);
    tree_t *s = ast_node_new(TT_IDX); ast_push(s, pas_tree_clone(e)); ast_push(s, ilit(fi)); return s;
}
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    { int lnf = pas_recspan_nf(sel); int rnf = pas_recspan_nf(rhs);
      int has_idx = ((sel && sel->t == TT_IDX) || (rhs && rhs->t == TT_IDX));
      if (lnf > 1 && lnf == rnf && has_idx) {
          PNodeList *cps = pnl_new();
          for (long long fi = 0; fi < lnf; fi++) pnl_push(cps, bin(TT_ASSIGN, pas_recspan_slot(sel, fi), pas_recspan_slot(rhs, fi)));
          return seq_of(cps);
      } }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && pas_is_nrec_idx(sel)) {
        tree_t *inner = sel->c[0]; tree_t *ei_nd = sel->c[1];
        if (inner && inner->t == TT_IDX && inner->n >= 2) {
            tree_t *base = inner->c[0]; tree_t *fi_nd = inner->c[1];
            if (base && base->t == TT_FNC && base->n >= 2 && base->c[0] && base->c[0]->v.sval && !strcmp(base->c[0]->v.sval, "__pas_deref")) {
                tree_t *e = ast_node_new(TT_FNC);
                ast_push(e, leaf_s(TT_VAR, "__pas_nrec_deref_set"));
                ast_push(e, base->c[1]); ast_push(e, pas_tree_clone(fi_nd)); ast_push(e, pas_tree_clone(ei_nd)); ast_push(e, rhs);
                return e;
            }
            if (base && base->t == TT_VAR && base->v.sval) {
                tree_t *upd = ast_node_new(TT_FNC);
                ast_push(upd, leaf_s(TT_VAR, "__pas_nrec_update"));
                ast_push(upd, pas_tree_clone(base)); ast_push(upd, pas_tree_clone(fi_nd)); ast_push(upd, pas_tree_clone(ei_nd)); ast_push(upd, rhs);
                return bin(TT_ASSIGN, base, upd);
            }
        }
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_FNC && sel->c[0]->c[0]->n >= 2
        && sel->c[0]->c[0]->c[0] && sel->c[0]->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_idx_set"));
        ast_push(e, sel->c[0]->c[0]->c[1]); ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        const char *_dbrt = (sel->c[1] && sel->c[1]->t == TT_ILIT) ? pas_with_sel_rtype(sel->c[0]) : NULL;
        const char *_dfrt = _dbrt ? pas_rectype_field_rectype_by_index(_dbrt, sel->c[1]->v.ival) : NULL;
        const char *_drhsrt = _dfrt ? pas_with_sel_rtype(rhs) : NULL;
        tree_t *e = ast_node_new(TT_FNC);
        if (_dfrt && _drhsrt) ast_push(e, leaf_s(TT_VAR, "__pas_nrec_pfield_set")); else ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
    }
    if (sel && sel->t == TT_IDX && sel->n == 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && sel->c[1] && sel->c[1]->t == TT_ILIT) {
        const char *_brt = pas_with_sel_rtype(sel->c[0]); const char *_frt = _brt ? pas_rectype_field_rectype_by_index(_brt, sel->c[1]->v.ival) : NULL;
        const char *_rhsrt = pas_with_sel_rtype(rhs);
        if (_frt && _rhsrt) {
            tree_t *upd = ast_node_new(TT_FNC); ast_push(upd, leaf_s(TT_VAR, "__pas_nrec_field_set"));
            ast_push(upd, pas_tree_clone(sel->c[0])); ast_push(upd, pas_tree_clone(sel->c[1])); ast_push(upd, rhs);
            return bin(TT_ASSIGN, sel->c[0], upd);
        }
    }
    return bin(TT_ASSIGN, sel, rhs);
}
static tree_t *mk_ident(const char *name) {
    if (name && !strcmp(name, "true"))  return ilit(1);
    if (name && !strcmp(name, "false")) return ilit(0);
    if (name && !strcmp(name, "nil"))   return ilit(0);
    if (name && !strcmp(name, "eof"))   return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln"))  return mk_fnc0("__pas_eoln");
    long long cv; if (pas_const_get(name, &cv)) return ilit(cv);
    double rv; if (pas_rconst_get(name, &rv)) return flit(rv);
    const char *sv = pas_sconst_get(name); if (sv) return leaf_s(TT_QLIT, sv);
    if (pas_is_func(name)) return mk_call(name, NULL);
    for (int wi = g_with_depth - 1; wi >= 0; wi--) {
        tree_t *wsel = g_with_stk[wi].sel; const char *rt = g_with_stk[wi].rtype;
        int fi = -1;
        if (rt) fi = pas_with_field_index(rt, name);
        if (fi < 0 && wsel && wsel->t == TT_VAR && wsel->v.sval) fi = pas_with_recvar_field(wsel->v.sval, name);
        if (fi < 0 && wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && wsel->c[0]->v.sval) fi = pas_arrrec_field_index(wsel->c[0]->v.sval, name);
        if (fi >= 0) { if (wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && pas_arrrec_find(wsel->c[0]->v.sval, NULL) > 0) { tree_t *_af = pas_arrrec_flatten(pas_tree_clone(wsel), fi); if (pas_arrrec_field_is_char(wsel->c[0]->v.sval, fi)) pas_cvfield_mark_add(_af); return _af; } tree_t *e = ast_node_new(TT_IDX); ast_push(e, pas_tree_clone(wsel)); ast_push(e, ilit(fi)); { const char *_crt = rt ? rt : pas_with_sel_rtype(wsel); if (_crt && pas_rectype_field_is_ca(_crt, fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_crt, fi), pas_rectype_field_ca_hi(_crt, fi)); if (_crt && pas_rectype_field_is_char(_crt, fi)) pas_cvfield_mark_add(e); } return e; }
    }
    return leaf_s(TT_VAR, name);
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
static tree_t *pas_bool(tree_t *e) { return e; }
static tree_t *pas_flip_rel(tree_t *e) {
    switch (e->t) { case TT_LT: e->t = TT_GE; break; case TT_GE: e->t = TT_LT; break; case TT_LE: e->t = TT_GT; break;
                    case TT_GT: e->t = TT_LE; break; case TT_EQ: e->t = TT_NE; break; case TT_NE: e->t = TT_EQ; break; default: break; }
    return e;
}
static tree_t *mk_array_fill(long long high) {
    long long n = high + 1; if (n < 1) n = 1;
    size_t len = (size_t)(n * 2 - 1);
    char *buf = (char *)malloc(len + 1); size_t p = 0;
    for (long long k = 0; k < n; k++) { if (k) buf[p++] = '\001'; buf[p++] = '0'; }
    buf[p] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high) {
    if (!s) s = ""; if (lo < 0) lo = 0;
    size_t sl = strlen(s); long long n = high + 1; if (n < lo + (long long)sl) n = lo + (long long)sl; if (n < 1) n = 1;
    char *buf = (char *)malloc((size_t)n * 12 + 1); size_t p = 0;
    for (long long k = 0; k < n; k++) { if (k) buf[p++] = '\001'; long long si = k - lo; unsigned ch = (si >= 0 && si < (long long)sl) ? (unsigned char)s[si] : (si >= 0 ? (unsigned)' ' : 0u); p += (size_t)snprintf(buf + p, 12, "%u", ch); }
    buf[p] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}
static int pas_array_is_pure_num(const char *name) {
    if (!name) return 0;
    if (pas_is_chararr(name)) return 0;
    if (pas_arrrec_find(name, NULL) > 0) return 0;
    if (pas_enumarr_get(name)) return 0;
    for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name)) return 0;
    return 1;
}
static tree_t *mk_array_init(const char *name, long long high) {
    if (pas_array_is_pure_num(name)) return mk_fnc1("arr_make", ilit(high));
    return mk_array_fill(high);
}
%}
%union {
    tree_t    *node;
    PNodeList *list;
    char      *str;
    long long  ival;
    double     dval;
}
%token GOTOSY PROGRAMSY SEMICOLON ARRAYSY LABELSY CONSTSY FORWARDSY
%token DOSY DOWNTOSY FORSY REPEATSY WHILESY TOSY UNTILSY WITHSY CASESY
%token PROCEDURESY PACKEDSY OFSY FILESY ENDSY SETSY VARSY THENSY RECORDSY
%token FUNCTIONSY BEGINSY BECOMES TYPESY IFSY ELSESY INOP NOTSY IDIV IMOD ANDOP OROP
%token LTOP LEOP GTOP GEOP NEOP EQOP PLUS MINUS MUL RDIV
%token COMMA PERIOD COLON ARROW LBRACK RBRACK LPARENT RPARENT DOTDOT
%token <ival> INTCONST
%token <dval> REALCONST
%token <str>  STRINGCONST IDENT
%type <node> block body statement statement_no_label compound_statement
%type <node> assignment call call_with_args if_statement while_statement
%type <node> repeat_statement for_statement with_statement case_statement goto_statement
%type <node> expression simple_expression term factor selector
%type <node> case_elem constant_list
%type <list> case_list
%type <list> statement_list argument_list expression_list expression_list_opt id_list argument
%type <list> parameter_list_opt parameter_decl_list parameter_decl
%type <list> selector_list
%type <ival> constant scalar_constant simple_type type
%type <ival> with_open
%start program
%%
program:
    PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD
        { tree_t *body = $5;
          if (g_pas_narray > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_narray; i++) if (!g_pas_arrays[i].is_param && !g_pas_arrays[i].is_local) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_init(g_pas_arrays[i].name, g_pas_arrays[i].high)));
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0); emit_proc(&g_pascal_procs, mainp);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
    ;
file_id_list_opt:
    LPARENT id_list RPARENT { if ($2) for (int i = 0; i < $2->count; i++) { tree_t *id = $2->items[i]; if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) { pas_filevar_add(id->v.sval); if (g_pas_nhdrfile < 32) g_pas_hdrfiles[g_pas_nhdrfile++] = strdup(id->v.sval); } } }
    |
    ;
block:
    decl_part_list body { $$ = $2; }
    ;
decl_part_list:
    decl_part_list decl_part
    |
    ;
decl_part:
    LABELSY label_list SEMICOLON
    | CONSTSY const_decl_list
    | TYPESY type_decl_list
    | VARSY var_decl_list
    | procedure_decl
    ;
label_list:
    label_list COMMA INTCONST
    | INTCONST
    ;
const_decl_list:
    const_decl_list const_decl
    | const_decl
    ;
const_decl: IDENT EQOP REALCONST SEMICOLON { pas_rconst_add($1, $3); }
    | IDENT EQOP PLUS REALCONST SEMICOLON { pas_rconst_add($1, $4); }
    | IDENT EQOP MINUS REALCONST SEMICOLON { pas_rconst_add($1, -$4); }
    | IDENT EQOP STRINGCONST SEMICOLON { if ($3 && strlen($3)==1) pas_const_add($1,(long long)(unsigned char)$3[0]); else pas_sconst_add($1,$3); }
    | IDENT EQOP constant SEMICOLON { pas_const_add($1, $3); } ;
constant:
    scalar_constant { $$ = $1; } | PLUS scalar_constant { $$ = $2; } | MINUS scalar_constant { $$ = -$2; } ;
scalar_constant: IDENT { long long cv = 0; pas_const_get($1, &cv); $$ = cv; } | INTCONST { $$ = $1; } | REALCONST { $$ = (long long)$1; } | STRINGCONST { $$ = ($1 && strlen($1) == 1) ? (long long)(unsigned char)$1[0] : 0; } ;
type_decl_list:
    type_decl_list type_decl
    | type_decl
    ;
type_decl: IDENT EQOP type SEMICOLON { if ($3 == -2) pas_settype_add($1); if (g_pas_pend_ptrtarget) pas_ptrtype_add($1, g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add($1); if (g_pas_pend_enum_max >= 0) { pas_enumtype_add($1, g_pas_pend_enum_max); pas_enumnames_add($1, g_pas_pend_enum_names); } if (g_pas_pend_sub_high >= 0 && $3 < 0 && g_pas_pend_arr_ncols < 0) pas_subtype_add($1, g_pas_pend_sub_low, g_pas_pend_sub_high); if ($3 >= 0 && !g_pas_pend_ptrtarget) { pas_arrtype_add($1, $3, g_pas_pend_arr_ncols >= 0 ? 1 : 0, g_pas_pend_arr_ncols); } pas_pend_reset(); } ;
type:
    simple_type { if (g_pas_pend_ptrtarget) { $$ = -3; } else if ($1 == -2) { $$ = -2; } else if ($1 >= 0 && g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0) { long long _tnc = pas_arrtype_ncols(g_pas_pend_typename); if (_tnc >= 0) g_pas_pend_arr_ncols = _tnc; $$ = $1; } else { $$ = -1; } }
    | ARROW IDENT { g_pas_pend_ptrtarget = strdup($2); $$ = -3; }
    | packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type { int _eic = g_pas_pend_ischar; int _wr = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); g_pas_pend_arr_wrap = _wr; g_pas_pend_arr_ptrto = g_pas_pend_ptrtarget ? g_pas_pend_ptrtarget : (g_pas_pend_typename ? (char *)pas_ptrtype_target(g_pas_pend_typename) : NULL); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_ischar = _eic; $$ = $4; }
    | packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type { int _eic = g_pas_pend_ischar; g_pas_pend_ptrtarget = NULL; long long r = $4; long long c = $6; g_pas_pend_arr_ncols = c + 1; g_pas_pend_arr_ischar = _eic; $$ = (r + 1) * (c + 1) - 1; }
    | packed_opt RECORDSY { g_pas_recbody_depth++; } record_body ENDSY { g_pas_recbody_depth--; g_pas_pend_ptrtarget = NULL; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_enum_max = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; $$ = -1; }
    | packed_opt SETSY OFSY simple_type { g_pas_pend_ptrtarget = NULL; $$ = -2; }
    | packed_opt FILESY { g_pas_pend_ptrtarget = NULL; $$ = -1; }
    ;
packed_opt: PACKEDSY | ;
simple_type:
    LPARENT id_list RPARENT
        { int _eo = 0; g_pas_pend_enum_names[0] = '\0';
          if ($2) for (int i = 0; i < $2->count; i++) {
              tree_t *_id = $2->items[i];
              if (_id && _id->v.sval) { if (_eo > 0) strncat(g_pas_pend_enum_names, ",", sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); strncat(g_pas_pend_enum_names, _id->v.sval, sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); pas_const_add(_id->v.sval, (long long)(_eo++)); } }
          g_pas_pend_enum_max = (long long)(_eo - 1);
          $$ = _eo > 0 ? (long long)(_eo - 1) : -1; }
    | IDENT { g_pas_pend_typename = strdup($1); g_pas_pend_ischar = !strcmp($1, "char"); const char *_pt = pas_ptrtype_target($1); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); $$ = -3; } else { if (!strcmp($1, "char")) { $$ = 255; } else if (pas_is_settype($1)) { $$ = -2; } else { long long _eh = pas_enumtype_high($1); long long _sh = pas_subtype_high($1); long long _ah = pas_arrtype_high($1); if (_eh >= 0) { $$ = _eh; } else if (_sh >= 0) { $$ = _sh; } else if (_ah >= 0) { $$ = _ah; } else { if (g_pas_recbody_depth == 0) pas_rectype_to_pend($1); g_pas_pend_typename = strdup($1); $$ = -1; } } } }
    | constant DOTDOT constant { g_pas_pend_sub_low = $1; g_pas_pend_sub_high = $3; $$ = $3; }
    ;
record_body:
    record_field_list record_case_opt
    ;
record_field_list:
    record_field_list SEMICOLON record_field
    | record_field
    ;
record_field:
    id_list COLON type { if ($1) { char *_svp = g_pas_pend_ptrtarget; int _svc = g_pas_pend_ischar; int _sva = g_pas_pend_arr_ischar; for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) { g_pas_pend_ptrtarget = _svp; g_pas_pend_ischar = _svc; g_pas_pend_arr_ischar = _sva; pas_pend_add($1->items[i]->v.sval); } } }
    |
    ;
record_case_opt:
    CASESY IDENT COLON IDENT OFSY record_case_list { if ($2) { g_pas_pend_typename = strdup($4); pas_pend_add($2); } }
    | CASESY IDENT OFSY record_case_list { if ($2) pas_pend_add($2); }
    |
    ;
record_case_list:
    record_case_list SEMICOLON record_case_arm
    | record_case_arm
    ;
record_case_arm:
    constant_list COLON LPARENT record_body RPARENT
    |
    ;
var_decl_list:
    var_decl_list var_decl
    | var_decl
    ;
var_decl: id_list COLON type SEMICOLON { if ($1) for (int i = 0; i < $1->count; i++) { tree_t *id = $1->items[i]; if (id && id->v.sval) { if ($3 == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ($3 >= 0 && g_pas_pend_nf > 0) { pas_array_add2d(id->v.sval, ($3 + 1) * g_pas_pend_nf - 1, (long long)g_pas_pend_nf); pas_arrrec_add(id->v.sval, g_pas_pend_typename, g_pas_pend_nf); } else if ($3 >= 0) { long long _varnc = (g_pas_pend_arr_ncols >= 0) ? g_pas_pend_arr_ncols : pas_arrtype_ncols(g_pas_pend_typename); if (g_pas_pend_ischar && !g_pas_pend_arr_ischar && _varnc < 0 && g_pas_pend_nf == 0) { pas_charvar_add(id->v.sval); } else if (_varnc >= 0) { pas_array_add2d(id->v.sval, $3, _varnc); } else { pas_array_add(id->v.sval, $3); if (g_pas_pend_arr_ptrto) pas_arrptr_add(id->v.sval, g_pas_pend_arr_ptrto); int _aic = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); if (_aic && g_pas_pend_arr_wrap) pas_strarr_add2(id->v.sval, (g_pas_pend_typename && pas_arrtype_lo(g_pas_pend_typename) > 0) ? pas_arrtype_lo(g_pas_pend_typename) : 1); else if (_aic) pas_chararr_add2(id->v.sval, g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : pas_arrtype_lo(g_pas_pend_typename)); else if (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) pas_enumarr_add(id->v.sval, g_pas_pend_typename); } } if ($3 == -2) pas_setvar_add(id->v.sval); if ($3 < 0 && g_pas_pend_ischar) pas_charvar_add(id->v.sval); if ($3 < 0 && g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "text")) pas_filevar_add(id->v.sval); } pas_local_add(id->v.sval); } } pas_pend_reset(); } ;
procedure_decl:
    PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON { pas_proc_add($2); pas_fwd_save($2); pas_ptrvar_release(); }
    | FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON { pas_func_add($2); if ($6 && !strcmp($6, "char")) pas_charvar_add($2); pas_fwd_save($2); pas_ptrvar_release(); }
    | PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON { pas_proc_add($2); pas_proc_enter(); pas_fwd_restore($2); } block SEMICOLON
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc($2, $4, $7, 0, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); emit_proc(&g_pascal_procs, p); }
    | FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON { pas_func_add($2); if ($6 && !strcmp($6, "char")) pas_charvar_add($2); pas_proc_enter(); pas_fwd_restore($2); } block SEMICOLON
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc($2, $4, $9, 1, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); emit_proc(&g_pascal_procs, p); }
    ;
pv_mark:
    { pas_ptrvar_mark(); }
    ;
parameter_list_opt:
    LPARENT parameter_decl_list RPARENT { $$ = $2; }
    | { $$ = pnl_new(); }
    ;
parameter_decl_list:
    parameter_decl_list SEMICOLON parameter_decl { $$ = pnl_concat($1, $3); }
    | parameter_decl { $$ = $1; }
    ;
parameter_decl:
    PROCEDURESY id_list { $$ = $2; }
    | FUNCTIONSY id_list COLON IDENT { $$ = $2; }
    | VARSY id_list COLON IDENT { if (pas_is_settype($4)) for (int i = 0; i < $2->count; i++) if ($2->items[i] && $2->items[i]->v.sval) pas_setvar_add($2->items[i]->v.sval); const char *_pt = pas_ptrtype_target($4); if (_pt) for (int i = 0; i < $2->count; i++) if ($2->items[i] && $2->items[i]->v.sval) pas_ptrvar_add($2->items[i]->v.sval, _pt); { long long _ah = pas_arrtype_high($4); long long _nc = pas_arrtype_ncols($4); int _nf = pas_rectype_nf($4); for (int i = 0; i < $2->count; i++) if ($2->items[i] && $2->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type($2->items[i]->v.sval, $4); pas_array_add2d_param($2->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param($2->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param($2->items[i]->v.sval, _ah, -1); } } } for (int i = 0; i < $2->count; i++) if ($2->items[i]) ast_push($2->items[i], ast_node_new(TT_SUCCEED)); $$ = $2; }
    | id_list COLON IDENT { if (pas_is_settype($3)) for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) pas_setvar_add($1->items[i]->v.sval); const char *_pt = pas_ptrtype_target($3); if (_pt) for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) pas_ptrvar_add($1->items[i]->v.sval, _pt); if (!strcmp($3, "char")) for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) pas_charvar_add($1->items[i]->v.sval); { long long _ah = pas_arrtype_high($3); long long _nc = pas_arrtype_ncols($3); int _nf = pas_rectype_nf($3); int _aic = pas_arrtype_ischar($3); for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type($1->items[i]->v.sval, $3); pas_array_add2d_param($1->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param($1->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param($1->items[i]->v.sval, _ah, -1); if (_aic) pas_chararr_add2($1->items[i]->v.sval, pas_arrtype_lo($3)); } } } $$ = $1; }
    ;
id_list:
    id_list COMMA IDENT { pnl_push($1, leaf_s(TT_VAR, $3)); $$ = $1; }
    | IDENT { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, $1)); $$ = l; }
    ;
body:
    BEGINSY statement_list ENDSY { $$ = prog_of($2); }
    ;
statement_list:
    statement_list SEMICOLON statement { if ($3) pnl_push($1, $3); $$ = $1; }
    | statement { PNodeList *l = pnl_new(); if ($1) pnl_push(l, $1); $$ = l; }
    ;
statement:
    statement_no_label { $$ = $1; }
    | INTCONST COLON statement_no_label
        { char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)$1);
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = strdup(_lb); ast_push(L, $3); $$ = L; }
    ;
statement_no_label:
    assignment { $$ = $1; }
    | call { $$ = $1; }
    | compound_statement { $$ = $1; }
    | goto_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | case_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | repeat_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | with_statement { $$ = $1; }
    | { $$ = ast_node_new(TT_SUCCEED); }
    ;
call:
    IDENT { if (pas_is_proc($1)) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, $1)); $$ = e; } else $$ = mk_call($1, NULL); }
    | call_with_args { $$ = $1; }
    ;
call_with_args:
    IDENT LPARENT argument_list RPARENT { $$ = mk_call($1, $3); }
    ;
argument_list:
    argument_list COMMA argument { $$ = pnl_concat($1, $3); }
    | argument { $$ = $1; }
    ;
argument:
    expression { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool($1)); pnl_push(_al, ilit(-1)); $$ = _al; }
    | expression COLON expression { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool($1)); pnl_push(_al, $3); $$ = _al; }
    | expression COLON expression COLON expression { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool($1)); pnl_push(_al, ilit(-3)); pnl_push(_al, $3); pnl_push(_al, $5); $$ = _al; }
    ;
assignment:
    selector BECOMES expression
        { if ($1 && $1->t == TT_VAR && $1->v.sval && pas_is_chararr($1->v.sval) && $3 && $3->t == TT_QLIT && $3->v.sval) {
              long long _cah; if (!pas_array_high_get($1->v.sval, &_cah)) _cah = (long long)strlen($3->v.sval);
              $$ = mk_assign($1, pas_str_to_alpha($3->v.sval, pas_chararr_lo($1->v.sval), _cah));
          } else if ($1 && $1->t == TT_IDX && $1->n == 2 && $1->c[0] && $1->c[0]->t == TT_VAR && $1->c[0]->v.sval && pas_is_strarr($1->c[0]->v.sval) && $3 && $3->t == TT_QLIT && $3->v.sval) {
              long long _slo = pas_strarr_lo($1->c[0]->v.sval); long long _shi = _slo + (long long)strlen($3->v.sval) - 1;
              $$ = mk_assign($1, pas_str_to_alpha($3->v.sval, _slo, _shi));
          } else if (pas_is_cafield($1)) {
              tree_t *_rhs;
              if ($3 && $3->t == TT_QLIT && $3->v.sval) { long long _flo = pas_cafield_lo_get($1); long long _fhi = pas_cafield_hi_get($1); if (_fhi < _flo) _fhi = _flo + (long long)strlen($3->v.sval) - 1; _rhs = pas_str_to_alpha($3->v.sval, _flo, _fhi); }
              else _rhs = pas_bool($3);
              tree_t *_pk = ast_node_new(TT_FNC); ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack")); ast_push(_pk, _rhs);
              $$ = mk_assign($1, _pk);
          } else { $$ = mk_assign($1, pas_bool($3)); } }
    ;
selector:
    selector LBRACK expression_list RBRACK { tree_t *e = NULL; if ($3 && $3->count == 2 && $1 && $1->t == TT_VAR && $1->v.sval) { long long _nc = pas_array_ncols($1->v.sval); if (_nc > 0) { tree_t *flat = bin(TT_ADD, bin(TT_MUL, $3->items[0], ilit(_nc)), $3->items[1]); e = ast_node_new(TT_IDX); ast_push(e, $1); ast_push(e, flat); } } if (!e) { e = ast_node_new(TT_IDX); ast_push(e, $1); if ($3) for (int i = 0; i < $3->count; i++) ast_push(e, $3->items[i]); } if (e && $1 && $1->t == TT_VAR && $1->v.sval) { const char *_et = pas_enumarr_get($1->v.sval); if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } $$ = e; }
    | selector PERIOD IDENT { int _fi = -1; const char *_rt = pas_selector_rectype($1); if (_rt) _fi = pas_rectype_field_index(_rt, $3); else if ($1 && $1->t == TT_VAR && $1->v.sval) _fi = pas_recvar_field_index($1->v.sval, $3);
        if (_fi < 0 && $1 && $1->t == TT_IDX && $1->n == 2 && $1->c[0] && $1->c[0]->t == TT_VAR) { const char *_arn = NULL; int _anf = pas_arrrec_find($1->c[0]->v.sval, &_arn); if (_anf > 0) { int _afi = _arn ? pas_rectype_field_index(_arn, $3) : -1; if (_afi < 0) { _afi = pas_arrrec_field_index($1->c[0]->v.sval, $3); } if (_afi < 0) { for (int _ri = 0; _ri < g_pas_nrectype; _ri++) { int _t = pas_rectype_field_index(g_pas_rectypes[_ri].tname, $3); if (_t >= 0 && g_pas_rectypes[_ri].nf == _anf) { _afi = _t; break; } } } if (_afi >= 0) { $$ = pas_arrrec_flatten($1, _afi); if (pas_arrrec_field_is_char($1->c[0]->v.sval, _afi) || (_arn && pas_rectype_field_is_char(_arn, _afi))) pas_cvfield_mark_add($$); const char *_fe = pas_arrrec_field_enum($1->c[0]->v.sval, _afi); if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi); if (_fe && $$) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) $$->v.ival = (long long)(_ei + 1); } } else { $$ = bin(TT_FIELD, $1, leaf_s(TT_VAR, $3)); } } else { $$ = pas_nested_field_resolve($1, $3); } }
        else if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, $1); ast_push(e, ilit(_fi)); if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } { const char *_mrt = _rt ? _rt : pas_with_sel_rtype($1); if (_mrt && pas_rectype_field_is_ca(_mrt, _fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_mrt, _fi), pas_rectype_field_ca_hi(_mrt, _fi)); if (_mrt && pas_rectype_field_is_char(_mrt, _fi)) pas_cvfield_mark_add(e); } $$ = e; } else { $$ = pas_nested_field_resolve($1, $3); } }
    | selector ARROW { $$ = mk_deref($1); }
    | IDENT { $$ = mk_ident($1); }
    ;
expression_list:
    expression_list COMMA expression { pnl_push($1, $3); $$ = $1; }
    | expression { PNodeList *l = pnl_new(); pnl_push(l, $1); $$ = l; }
    ;
compound_statement:
    BEGINSY statement_list ENDSY { $$ = seq_of($2); }
    ;
goto_statement:
    GOTOSY INTCONST
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)$2);
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = strdup(_gb); $$ = G; }
    ;
if_statement:
    IFSY expression THENSY statement { $$ = bin(TT_IF, pas_cond($2), $4); }
    | IFSY expression THENSY statement ELSESY statement { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond($2)); ast_push(e, $4); ast_push(e, $6); $$ = e; }
    ;
case_statement:
    CASESY expression OFSY { pas_case_push(); } case_list ENDSY
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), $2));
          tree_t *chain = NULL;
          if ($5) for (int i = $5->count - 1; i >= 0; i--) { tree_t *e = $5->items[i]; if (!e) continue; if (chain) ast_push(e, chain); chain = e; }
          ast_push(seq, chain ? chain : ast_node_new(TT_SUCCEED));
          pas_case_pop();
          $$ = seq; }
    ;
case_list:
    case_list SEMICOLON case_elem { if ($3) pnl_push($1, $3); $$ = $1; }
    | case_elem { PNodeList *l = pnl_new(); if ($1) pnl_push(l, $1); $$ = l; }
    ;
case_elem:
    constant_list COLON statement { $$ = bin(TT_IF, pas_cond($1), $3); }
    | { $$ = NULL; }
    ;
constant_list:
    constant_list COMMA constant { $$ = bin(TT_ADD, $1, bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit($3))); }
    | constant { $$ = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit($1)); }
    ;
while_statement:
    WHILESY expression DOSY statement { $$ = bin(TT_WHILE, pas_cond($2), $4); }
    ;
repeat_statement:
    REPEATSY statement_list UNTILSY expression { $$ = bin(TT_REPEAT, seq_of($2), pas_cond($4)); }
    ;
for_statement:
    FORSY IDENT BECOMES expression TOSY expression DOSY statement
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, $2)); ast_push(e, $4); ast_push(e, $6); ast_push(e, $8); $$ = e; }
    | FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, $2)); ast_push(e, $4); ast_push(e, $6); ast_push(e, $8); e->v.ival = 1; $$ = e; }
    ;
with_statement:
    WITHSY with_open DOSY statement { long long n = $2; for (long long i = 0; i < n; i++) pas_with_pop(); $$ = $4; }
    ;
with_open:
    with_open COMMA selector { pas_with_push($3); $$ = $1 + 1; }
    | selector { pas_with_push($1); $$ = 1; }
    ;
selector_list:
    selector_list COMMA selector { pnl_push($1, $3); $$ = $1; }
    | selector { PNodeList *l = pnl_new(); pnl_push(l, $1); $$ = l; }
    ;
expression:
    simple_expression { $$ = $1; }
    | expression INOP simple_expression { $$ = mk_in($1, $3); }
    | expression LTOP simple_expression { $$ = pas_rel(TT_LT, $1, $3); }
    | expression LEOP simple_expression { $$ = pas_rel_or_set(TT_LE, "__pas_subset", $1, $3); }
    | expression GTOP simple_expression { $$ = pas_rel(TT_GT, $1, $3); }
    | expression GEOP simple_expression { $$ = pas_rel_or_set(TT_GE, "__pas_super", $1, $3); }
    | expression NEOP simple_expression { $$ = pas_rel_or_set(TT_NE, "__pas_setne", $1, $3); }
    | expression EQOP simple_expression { $$ = pas_rel_or_set(TT_EQ, "__pas_seteq", $1, $3); }
    ;
simple_expression:
    term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = mk_neg($2); }
    | simple_expression PLUS term { $$ = pas_arith_or_set(TT_ADD, "__pas_setuni", $1, $3); }
    | simple_expression MINUS term { $$ = pas_arith_or_set(TT_SUB, "__pas_setdif", $1, $3); }
    | simple_expression OROP term { $$ = bin(TT_ADD, $1, $3); }
    ;
term:
    factor { $$ = $1; }
    | term MUL factor { $$ = pas_arith_or_set(TT_MUL, "__pas_setint", $1, $3); }
    | term RDIV factor { $$ = bin(TT_DIV, $1, $3); }
    | term IDIV factor { $$ = bin(TT_DIV, $1, $3); }
    | term IMOD factor { $$ = bin(TT_MOD, $1, $3); }
    | term ANDOP factor { $$ = bin(TT_MUL, $1, $3); }
    ;
factor:
    selector { if (pas_is_cafield($1)) { tree_t *u = ast_node_new(TT_FNC); ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack")); ast_push(u, $1); ast_push(u, ilit(pas_cafield_lo_get($1))); $$ = u; } else $$ = $1; }
    | call_with_args { $$ = $1; }
    | INTCONST { $$ = ilit($1); }
    | REALCONST { $$ = flit($1); }
    | STRINGCONST { if ($1 && strlen($1) == 1) { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((long long)(unsigned char)$1[0])); $$ = _cl; } else $$ = leaf_s(TT_QLIT, $1); }
    | LPARENT expression RPARENT { $$ = $2; }
    | NOTSY factor { $$ = pas_flip_rel(pas_cond($2)); }
    | LBRACK expression_list_opt RBRACK { $$ = mk_set_ctor($2); }
    ;
expression_list_opt:
    expression_list { $$ = $1; }
    | { $$ = NULL; }
    ;
%%
extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0; g_pas_ncaparm = 0; g_pas_pend_arr_ncols = -1;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0; g_pas_nsettype = 0; g_pas_ncharvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; g_pas_narrtype = 0;
    g_pas_nenum = 0; g_pas_pend_enum_max = -1; g_pas_nsubtype = 0; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0; g_with_depth = 0;
    g_pas_nchararr = 0; g_pas_pend_arr_ischar = 0; g_pas_nrconst = 0; g_pas_nsconst = 0; g_pas_narrrec = 0; g_pas_ncafield = 0; g_pas_ncvfield = 0; g_pas_narrptr = 0; g_pas_pend_arr_ptrto = NULL;
    g_pas_nenumname = 0; g_pas_nenumarr = 0; g_pas_pend_enum_names[0] = '\0'; g_pas_nrec_nmarks = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
