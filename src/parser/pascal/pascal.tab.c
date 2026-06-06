/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         PASCAL_YYSTYPE
/* Substitute the variable and function names.  */
#define yyparse         pascal_yyparse
#define yylex           pascal_yylex
#define yyerror         pascal_yyerror
#define yydebug         pascal_yydebug
#define yynerrs         pascal_yynerrs
#define yylval          pascal_yylval
#define yychar          pascal_yychar

/* First part of user prologue.  */
#line 7 "pascal.y"

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
static const char *pas_ptrexpr_target(tree_t *e);
static int pas_rectype_nf(const char *rn);
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static int pas_is_rel(tree_t *e);
static tree_t *pas_bool(tree_t *e);
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
        return a;
    }
    if (name && !strcmp(name, "chr") && args && args->count >= 1) return args->items[0];
    if (name && !strcmp(name, "pred") && args && args->count >= 1) return bin(TT_SUB, args->items[0], ilit(1));
    if (name && !strcmp(name, "succ") && args && args->count >= 1) return bin(TT_ADD, args->items[0], ilit(1));
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
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
    if (name && !strcmp(name, "readln") && (!args || args->count == 0)) return mk_fnc0("__pas_readln");
    if (name && !strcmp(name, "readln") && args && args->count >= 1) {
        PNodeList *stmts = pnl_new();
        for (int i = 0; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            const char *rfn = (v && v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) ? "__pas_read_c" : "__pas_read_i";
            pnl_push(stmts, mk_assign(v, mk_fnc0(rfn)));
        }
        pnl_push(stmts, mk_fnc0("__pas_readln"));
        return seq_of(stmts);
    }
    if (name && !strcmp(name, "read") && args && args->count >= 1) {
        PNodeList *stmts = pnl_new();
        for (int i = 0; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            const char *rfn = (v && v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) ? "__pas_read_c" : "__pas_read_i";
            pnl_push(stmts, mk_assign(v, mk_fnc0(rfn)));
        }
        return seq_of(stmts);
    }
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv = args->items[0];
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, ilit(pas_rectype_nf(rt))); }
        else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        return mk_assign(pv, alloc);
    }
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = 0; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i]; tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val);
                if (is_char) { val = mk_chr_wrap(val); if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2); }
                ast_push(e, val); ast_push(e, wid);
            }
        } else {
            for (int i = 0; i < args->count; i += 2) ast_push(e, args->items[i]);
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
    ast_push(st, ast_attr_int(":lang", LANG_PASCAL));
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); }
    else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { if (i < 64) byref |= (1LL << i); pv->n = 0; }
        ast_push(vlist, pv);
    }
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
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
static int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; double val; } g_pas_rconsts[64]; static int g_pas_nrconst;
static void pas_rconst_add(const char *name, double v) { if (g_pas_nrconst < 64 && name) { g_pas_rconsts[g_pas_nrconst].name = strdup(name); g_pas_rconsts[g_pas_nrconst].val = v; g_pas_nrconst++; } }
static int pas_rconst_get(const char *name, double *out) { if (!name) return 0; for (int i = 0; i < g_pas_nrconst; i++) if (g_pas_rconsts[i].name && !strcmp(g_pas_rconsts[i].name, name)) { *out = g_pas_rconsts[i].val; return 1; } return 0; }
static struct { char *name; char *val; } g_pas_sconsts[64]; static int g_pas_nsconst;
static void pas_sconst_add(const char *name, const char *v) { if (g_pas_nsconst < 64 && name && v) { g_pas_sconsts[g_pas_nsconst].name = strdup(name); g_pas_sconsts[g_pas_nsconst].val = strdup(v); g_pas_nsconst++; } }
static const char *pas_sconst_get(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsconst; i++) if (g_pas_sconsts[i].name && !strcmp(g_pas_sconsts[i].name, name)) return g_pas_sconsts[i].val; return 0; }
static struct { char *name; long long high; } g_pas_arrays[256]; static int g_pas_narray;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_narray++; } }
#define PAS_REC_MAX 64
#define PAS_FIELD_MAX 32
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; int nf; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct { char *vname; char *fields[PAS_FIELD_MAX]; int nf; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename; static int g_pas_pend_ischar;
static struct { char *pname; char *rname; } g_pas_ptrtypes[PAS_REC_MAX]; static int g_pas_nptrtype;
static void pas_ptrtype_add(const char *p, const char *r) { if (g_pas_nptrtype < PAS_REC_MAX && p && r) { int k = g_pas_nptrtype++; g_pas_ptrtypes[k].pname = strdup(p); g_pas_ptrtypes[k].rname = strdup(r); } }
static const char *pas_ptrtype_target(const char *p) { if (!p) return NULL; for (int i = 0; i < g_pas_nptrtype; i++) if (g_pas_ptrtypes[i].pname && !strcmp(g_pas_ptrtypes[i].pname, p)) return g_pas_ptrtypes[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_ptrvars[PAS_REC_MAX]; static int g_pas_nptrvar;
static void pas_ptrvar_add(const char *v, const char *r) { if (g_pas_nptrvar < PAS_REC_MAX && v && r) { int k = g_pas_nptrvar++; g_pas_ptrvars[k].vname = strdup(v); g_pas_ptrvars[k].rname = strdup(r); } }
static const char *pas_ptrvar_target(const char *v) { if (!v) return NULL; for (int i = 0; i < g_pas_nptrvar; i++) if (g_pas_ptrvars[i].vname && !strcmp(g_pas_ptrvars[i].vname, v)) return g_pas_ptrvars[i].rname; return NULL; }
static void pas_pend_reset(void) { g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; g_pas_pend_ischar = 0; }
static void pas_pend_add(const char *f) { if (g_pas_pend_nf < PAS_FIELD_MAX && f) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_pend_ptrtarget; g_pas_pend_fields[g_pas_pend_nf++] = strdup(f); } }
static void pas_rectype_add(const char *tn) { if (g_pas_nrectype >= PAS_REC_MAX || !tn) return; int k = g_pas_nrectype++; g_pas_rectypes[k].tname = strdup(tn); g_pas_rectypes[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_rectypes[k].fields[i] = g_pas_pend_fields[i]; g_pas_rectypes[k].fldptrto[i] = g_pas_pend_fldptrto[i]; } }
static int pas_rectype_to_pend(const char *tn) { if (!tn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, tn)) {
    pas_pend_reset(); for (int j = 0; j < g_pas_rectypes[i].nf; j++) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_rectypes[i].fldptrto[j]; g_pas_pend_fields[g_pas_pend_nf++] = g_pas_rectypes[i].fields[j]; } return 1; } return 0; }
static int pas_rectype_field_index(const char *rn, const char *fn) { if (!rn || !fn) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    for (int j = 0; j < g_pas_rectypes[i].nf; j++) if (g_pas_rectypes[i].fields[j] && !strcmp(g_pas_rectypes[i].fields[j], fn)) return j; return -1; } return -1; }
static int pas_rectype_nf(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return g_pas_rectypes[i].nf; return 0; }
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldptrto[idx]; return NULL; } return NULL; }
static void pas_recvar_add(const char *vn) { if (g_pas_nrecvar >= PAS_REC_MAX || !vn || g_pas_pend_nf == 0) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = strdup(vn); g_pas_recvars[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; }
static int pas_recvar_field_index(const char *vn, const char *fn) { if (!vn || !fn) return -1; for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) {
    for (int j = 0; j < g_pas_recvars[i].nf; j++) if (g_pas_recvars[i].fields[j] && !strcmp(g_pas_recvars[i].fields[j], fn)) return j; return -1; } return -1; }
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_selector_rectype(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return NULL; }
static const char *pas_ptrexpr_target(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) { const char *rt = pas_selector_rectype(e->c[0]); if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival); }
    return NULL; }
#define PAS_LOCAL_MAX 64
#define PAS_NEST_MAX  16
static int g_pas_level = 1;
static struct { const char *names[PAS_LOCAL_MAX]; int n; int decl_level; } g_pas_lstk[PAS_NEST_MAX]; static int g_pas_ldepth;
static void pas_proc_enter(void) { if (g_pas_ldepth < PAS_NEST_MAX) { g_pas_lstk[g_pas_ldepth].n = 0; g_pas_lstk[g_pas_ldepth].decl_level = g_pas_level; } g_pas_ldepth++; g_pas_level++; }
static void pas_proc_exit(void) { if (g_pas_ldepth > 0) g_pas_ldepth--; if (g_pas_level > 1) g_pas_level--; }
static void pas_local_add(const char *name) { if (g_pas_level < 2 || g_pas_ldepth == 0 || g_pas_ldepth > PAS_NEST_MAX || !name) return; int d = g_pas_ldepth - 1; if (g_pas_lstk[d].n < PAS_LOCAL_MAX) g_pas_lstk[d].names[g_pas_lstk[d].n++] = strdup(name); }
static struct { char *name; } g_pas_setvars[256]; static int g_pas_nsetvar;
static void pas_setvar_add(const char *name) { if (g_pas_nsetvar < 256 && name) { g_pas_setvars[g_pas_nsetvar++].name = strdup(name); } }
static int pas_is_setvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsetvar; i++) if (g_pas_setvars[i].name && !strcmp(g_pas_setvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_charvars[256]; static int g_pas_ncharvar;
static void pas_charvar_add(const char *name) { if (g_pas_ncharvar < 256 && name) { g_pas_charvars[g_pas_ncharvar++].name = strdup(name); } }
static int pas_is_charvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncharvar; i++) if (g_pas_charvars[i].name && !strcmp(g_pas_charvars[i].name, name)) return 1; return 0; }
static tree_t *mk_chr_wrap(tree_t *e) { tree_t *r = ast_node_new(TT_FNC); ast_push(r, leaf_s(TT_VAR, "__pas_chr")); ast_push(r, e); return r; }
static int pas_is_charexpr(tree_t *e) { if (!e) return 0; if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1; if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1; return 0; }
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
#define PAS_WITH_MAX 8
static struct { tree_t *sel; const char *rtype; } g_with_stk[PAS_WITH_MAX]; static int g_with_depth;
static tree_t *pas_tree_clone(tree_t *e) { if (!e) return NULL; tree_t *c = ast_node_new(e->t); c->v = e->v; if (e->v.sval) c->v.sval = strdup(e->v.sval); for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i])); return c; }
static const char *pas_with_sel_rtype(tree_t *sel) { if (!sel) return NULL; if (sel->t == TT_VAR && sel->v.sval) { for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, sel->v.sval)) { const char *rt = NULL; for (int j = 0; j < g_pas_nrectype; j++) { int match = 1; if (!g_pas_rectypes[j].tname) continue; if (g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { match = 0; break; } if (match) { rt = g_pas_rectypes[j].tname; break; } } if (!rt) { for (int j = 0; j < g_pas_nrectype; j++) { int all = 1; if (!g_pas_rectypes[j].tname || g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { all = 0; break; } if (all) { rt = g_pas_rectypes[j].tname; break; } } } return rt ? rt : g_pas_recvars[i].fields[0] ? g_pas_rectypes[0].tname : NULL; } } if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) { const char *ptn = pas_ptrexpr_target(sel->c[1]); return ptn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[1] && sel->c[1]->t == TT_ILIT) { const char *bt = pas_with_sel_rtype(sel->c[0]); if (bt) return pas_rectype_field_ptrto_by_index(bt, sel->c[1]->v.ival); } return NULL; }
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
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
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
        if (fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, pas_tree_clone(wsel)); ast_push(e, ilit(fi)); return e; }
    }
    return leaf_s(TT_VAR, name);
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
static tree_t *pas_bool(tree_t *e) { if (!pas_is_rel(e)) return e; tree_t *r = ast_node_new(TT_IF); ast_push(r, e); ast_push(r, ilit(1)); ast_push(r, ilit(0)); return r; }
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

#line 382 "pascal.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "pascal.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_GOTOSY = 3,                     /* GOTOSY  */
  YYSYMBOL_PROGRAMSY = 4,                  /* PROGRAMSY  */
  YYSYMBOL_SEMICOLON = 5,                  /* SEMICOLON  */
  YYSYMBOL_ARRAYSY = 6,                    /* ARRAYSY  */
  YYSYMBOL_LABELSY = 7,                    /* LABELSY  */
  YYSYMBOL_CONSTSY = 8,                    /* CONSTSY  */
  YYSYMBOL_FORWARDSY = 9,                  /* FORWARDSY  */
  YYSYMBOL_DOSY = 10,                      /* DOSY  */
  YYSYMBOL_DOWNTOSY = 11,                  /* DOWNTOSY  */
  YYSYMBOL_FORSY = 12,                     /* FORSY  */
  YYSYMBOL_REPEATSY = 13,                  /* REPEATSY  */
  YYSYMBOL_WHILESY = 14,                   /* WHILESY  */
  YYSYMBOL_TOSY = 15,                      /* TOSY  */
  YYSYMBOL_UNTILSY = 16,                   /* UNTILSY  */
  YYSYMBOL_WITHSY = 17,                    /* WITHSY  */
  YYSYMBOL_CASESY = 18,                    /* CASESY  */
  YYSYMBOL_PROCEDURESY = 19,               /* PROCEDURESY  */
  YYSYMBOL_PACKEDSY = 20,                  /* PACKEDSY  */
  YYSYMBOL_OFSY = 21,                      /* OFSY  */
  YYSYMBOL_FILESY = 22,                    /* FILESY  */
  YYSYMBOL_ENDSY = 23,                     /* ENDSY  */
  YYSYMBOL_SETSY = 24,                     /* SETSY  */
  YYSYMBOL_VARSY = 25,                     /* VARSY  */
  YYSYMBOL_THENSY = 26,                    /* THENSY  */
  YYSYMBOL_RECORDSY = 27,                  /* RECORDSY  */
  YYSYMBOL_FUNCTIONSY = 28,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 29,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 30,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 31,                    /* TYPESY  */
  YYSYMBOL_IFSY = 32,                      /* IFSY  */
  YYSYMBOL_ELSESY = 33,                    /* ELSESY  */
  YYSYMBOL_INOP = 34,                      /* INOP  */
  YYSYMBOL_NOTSY = 35,                     /* NOTSY  */
  YYSYMBOL_IDIV = 36,                      /* IDIV  */
  YYSYMBOL_IMOD = 37,                      /* IMOD  */
  YYSYMBOL_ANDOP = 38,                     /* ANDOP  */
  YYSYMBOL_OROP = 39,                      /* OROP  */
  YYSYMBOL_LTOP = 40,                      /* LTOP  */
  YYSYMBOL_LEOP = 41,                      /* LEOP  */
  YYSYMBOL_GTOP = 42,                      /* GTOP  */
  YYSYMBOL_GEOP = 43,                      /* GEOP  */
  YYSYMBOL_NEOP = 44,                      /* NEOP  */
  YYSYMBOL_EQOP = 45,                      /* EQOP  */
  YYSYMBOL_PLUS = 46,                      /* PLUS  */
  YYSYMBOL_MINUS = 47,                     /* MINUS  */
  YYSYMBOL_MUL = 48,                       /* MUL  */
  YYSYMBOL_RDIV = 49,                      /* RDIV  */
  YYSYMBOL_COMMA = 50,                     /* COMMA  */
  YYSYMBOL_PERIOD = 51,                    /* PERIOD  */
  YYSYMBOL_COLON = 52,                     /* COLON  */
  YYSYMBOL_ARROW = 53,                     /* ARROW  */
  YYSYMBOL_LBRACK = 54,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 55,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 56,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 57,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 58,                    /* DOTDOT  */
  YYSYMBOL_INTCONST = 59,                  /* INTCONST  */
  YYSYMBOL_REALCONST = 60,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 61,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 62,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 63,                  /* $accept  */
  YYSYMBOL_program = 64,                   /* program  */
  YYSYMBOL_file_id_list_opt = 65,          /* file_id_list_opt  */
  YYSYMBOL_block = 66,                     /* block  */
  YYSYMBOL_decl_part_list = 67,            /* decl_part_list  */
  YYSYMBOL_decl_part = 68,                 /* decl_part  */
  YYSYMBOL_label_list = 69,                /* label_list  */
  YYSYMBOL_const_decl_list = 70,           /* const_decl_list  */
  YYSYMBOL_const_decl = 71,                /* const_decl  */
  YYSYMBOL_constant = 72,                  /* constant  */
  YYSYMBOL_scalar_constant = 73,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 74,            /* type_decl_list  */
  YYSYMBOL_type_decl = 75,                 /* type_decl  */
  YYSYMBOL_type = 76,                      /* type  */
  YYSYMBOL_packed_opt = 77,                /* packed_opt  */
  YYSYMBOL_simple_type = 78,               /* simple_type  */
  YYSYMBOL_record_body = 79,               /* record_body  */
  YYSYMBOL_record_field_list = 80,         /* record_field_list  */
  YYSYMBOL_record_field = 81,              /* record_field  */
  YYSYMBOL_var_decl_list = 82,             /* var_decl_list  */
  YYSYMBOL_var_decl = 83,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 84,            /* procedure_decl  */
  YYSYMBOL_85_1 = 85,                      /* $@1  */
  YYSYMBOL_86_2 = 86,                      /* $@2  */
  YYSYMBOL_parameter_list_opt = 87,        /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 88,       /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 89,            /* parameter_decl  */
  YYSYMBOL_id_list = 90,                   /* id_list  */
  YYSYMBOL_body = 91,                      /* body  */
  YYSYMBOL_statement_list = 92,            /* statement_list  */
  YYSYMBOL_statement = 93,                 /* statement  */
  YYSYMBOL_statement_no_label = 94,        /* statement_no_label  */
  YYSYMBOL_call = 95,                      /* call  */
  YYSYMBOL_call_with_args = 96,            /* call_with_args  */
  YYSYMBOL_argument_list = 97,             /* argument_list  */
  YYSYMBOL_argument = 98,                  /* argument  */
  YYSYMBOL_assignment = 99,                /* assignment  */
  YYSYMBOL_selector = 100,                 /* selector  */
  YYSYMBOL_expression_list = 101,          /* expression_list  */
  YYSYMBOL_compound_statement = 102,       /* compound_statement  */
  YYSYMBOL_goto_statement = 103,           /* goto_statement  */
  YYSYMBOL_if_statement = 104,             /* if_statement  */
  YYSYMBOL_case_statement = 105,           /* case_statement  */
  YYSYMBOL_106_3 = 106,                    /* $@3  */
  YYSYMBOL_case_list = 107,                /* case_list  */
  YYSYMBOL_case_elem = 108,                /* case_elem  */
  YYSYMBOL_constant_list = 109,            /* constant_list  */
  YYSYMBOL_while_statement = 110,          /* while_statement  */
  YYSYMBOL_repeat_statement = 111,         /* repeat_statement  */
  YYSYMBOL_for_statement = 112,            /* for_statement  */
  YYSYMBOL_with_statement = 113,           /* with_statement  */
  YYSYMBOL_with_open = 114,                /* with_open  */
  YYSYMBOL_expression = 115,               /* expression  */
  YYSYMBOL_simple_expression = 116,        /* simple_expression  */
  YYSYMBOL_term = 117,                     /* term  */
  YYSYMBOL_factor = 118,                   /* factor  */
  YYSYMBOL_expression_list_opt = 119       /* expression_list_opt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined PASCAL_YYSTYPE_IS_TRIVIAL && PASCAL_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   376

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  288

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   317


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   339,   339,   354,   355,   358,   361,   362,   365,   366,
     367,   368,   369,   372,   373,   376,   377,   379,   380,   381,
     382,   383,   385,   385,   385,   386,   386,   386,   386,   388,
     389,   391,   393,   394,   395,   396,   397,   398,   400,   400,
     402,   408,   409,   412,   415,   416,   419,   420,   423,   424,
     426,   428,   429,   430,   430,   434,   434,   440,   441,   444,
     445,   448,   449,   450,   451,   454,   455,   458,   461,   462,
     465,   466,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   484,   485,   488,   491,   492,   495,   496,
     499,   513,   514,   516,   517,   520,   521,   524,   527,   532,
     533,   536,   536,   546,   547,   550,   551,   554,   555,   558,
     561,   564,   566,   570,   573,   574,   581,   582,   583,   584,
     585,   586,   587,   588,   591,   592,   593,   594,   595,   596,
     599,   600,   601,   602,   603,   604,   607,   608,   609,   610,
     611,   612,   613,   614,   617,   618
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if PASCAL_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "GOTOSY", "PROGRAMSY",
  "SEMICOLON", "ARRAYSY", "LABELSY", "CONSTSY", "FORWARDSY", "DOSY",
  "DOWNTOSY", "FORSY", "REPEATSY", "WHILESY", "TOSY", "UNTILSY", "WITHSY",
  "CASESY", "PROCEDURESY", "PACKEDSY", "OFSY", "FILESY", "ENDSY", "SETSY",
  "VARSY", "THENSY", "RECORDSY", "FUNCTIONSY", "BEGINSY", "BECOMES",
  "TYPESY", "IFSY", "ELSESY", "INOP", "NOTSY", "IDIV", "IMOD", "ANDOP",
  "OROP", "LTOP", "LEOP", "GTOP", "GEOP", "NEOP", "EQOP", "PLUS", "MINUS",
  "MUL", "RDIV", "COMMA", "PERIOD", "COLON", "ARROW", "LBRACK", "RBRACK",
  "LPARENT", "RPARENT", "DOTDOT", "INTCONST", "REALCONST", "STRINGCONST",
  "IDENT", "$accept", "program", "file_id_list_opt", "block",
  "decl_part_list", "decl_part", "label_list", "const_decl_list",
  "const_decl", "constant", "scalar_constant", "type_decl_list",
  "type_decl", "type", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "var_decl_list", "var_decl",
  "procedure_decl", "$@1", "$@2", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "id_list", "body",
  "statement_list", "statement", "statement_no_label", "call",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@3", "case_list", "case_elem",
  "constant_list", "while_statement", "repeat_statement", "for_statement",
  "with_statement", "with_open", "expression", "simple_expression", "term",
  "factor", "expression_list_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-176)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-95)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,   -27,    44,    -4,  -176,     6,    58,  -176,    -1,  -176,
      49,  -176,    69,   272,  -176,  -176,    67,    76,    84,     6,
      88,    13,    96,  -176,  -176,  -176,  -176,     5,   144,    76,
    -176,   105,     6,  -176,   116,   105,   150,   148,    13,   182,
     149,   182,    13,   182,   163,   215,    10,  -176,  -176,  -176,
    -176,  -176,   186,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,   173,    96,  -176,  -176,   160,   187,  -176,     9,   221,
    -176,    53,   175,  -176,   200,    43,   122,   122,   122,   182,
     182,  -176,  -176,  -176,   195,  -176,   143,    91,    11,    56,
    -176,  -176,   143,     3,   233,    18,   159,    48,   182,    13,
    -176,   182,   169,  -176,   182,    53,  -176,  -176,    36,   234,
    -176,   247,   250,  -176,   254,  -176,     6,     6,     6,    34,
    -176,   125,   256,  -176,   251,   251,   194,     6,  -176,  -176,
     231,   241,   287,    16,  -176,   240,   182,   182,  -176,    56,
      56,   265,   312,   259,   264,    13,   182,   182,   182,   182,
     182,   182,   182,   122,   122,   122,   122,   122,   122,   122,
     122,    13,   149,  -176,  -176,    13,  -176,    66,  -176,   282,
    -176,   312,  -176,    87,   313,   314,  -176,   315,  -176,  -176,
    -176,  -176,   267,   155,   212,     9,  -176,   266,   326,  -176,
    -176,    72,   289,  -176,   278,  -176,   316,     6,   328,   180,
     312,   182,  -176,  -176,  -176,    11,    11,    11,    11,    11,
      11,    11,    56,    56,    56,  -176,  -176,  -176,  -176,  -176,
    -176,   143,   289,   305,   182,  -176,   182,  -176,  -176,  -176,
    -176,   279,   285,  -176,  -176,  -176,   335,  -176,  -176,   283,
     283,   336,   353,  -176,   220,   351,   182,   182,   312,  -176,
      24,  -176,   238,    13,  -176,   312,  -176,  -176,  -176,   306,
    -176,  -176,     6,    53,   357,  -176,   111,   129,   289,  -176,
     289,    13,  -176,   342,  -176,  -176,  -176,   359,    13,    13,
    -176,  -176,  -176,    53,  -176,  -176,  -176,  -176
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    66,     0,     7,
       0,     3,     0,     0,    65,     2,     0,     0,     0,     0,
       0,    82,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    58,    11,    49,     0,    58,     0,     0,    82,     0,
       0,     0,    82,     0,     0,    83,     0,    69,    70,    73,
      84,    72,     0,    74,    75,    76,    77,    78,    79,    80,
      81,     0,    10,    30,     8,     0,     0,    15,     0,     0,
      48,    39,     0,    98,     0,     0,     0,     0,     0,   145,
       0,   138,   139,   140,    94,   137,   136,     0,   116,   124,
     130,    94,   115,     0,     0,     0,     0,    82,     0,    82,
      67,     0,     0,    93,     0,    39,    29,    13,     0,     0,
      26,     0,     0,    25,     0,    22,     0,     0,     0,     0,
      60,     0,    53,    38,     0,     0,     0,     0,    27,    28,
      41,     0,     0,     0,    32,     0,     0,     0,   142,   125,
     126,   144,    96,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,   101,    97,    82,    71,     0,    87,    88,
      68,    90,    92,     0,     0,     0,    23,     0,    24,    17,
      20,    21,    61,     0,     0,     0,    57,     0,     0,     7,
      33,     0,     0,    50,     0,    37,     0,    47,     0,     0,
     110,     0,   143,   141,   109,   117,   118,   119,   120,   121,
     122,   123,   129,   127,   128,   133,   134,   135,   131,   132,
     113,   114,   106,    99,     0,    85,     0,    91,    31,    18,
      19,     0,     0,    59,    64,    51,     0,    40,    42,     0,
       0,     0,    43,    45,     0,    55,     0,     0,    95,   108,
       0,   104,     0,    82,    86,    89,    63,    62,    54,     0,
      36,    35,    47,    39,     0,     7,     0,     0,   106,   102,
       0,    82,   100,     0,    44,    46,    52,     0,    82,    82,
     103,   107,   105,    39,    56,   112,   111,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -176,  -176,  -176,  -175,  -176,  -176,  -176,  -176,   337,   -62,
      40,  -176,   303,  -104,  -176,  -132,  -176,  -176,   106,  -176,
     338,  -176,  -176,  -176,   332,  -176,   184,     1,  -176,    -6,
     -91,   274,  -176,   -18,  -176,   151,  -176,   -21,   268,  -176,
    -176,  -176,  -176,  -176,  -176,   108,  -176,  -176,  -176,  -176,
    -176,  -176,   -34,   135,   -66,   -74,  -176
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,   131,
     115,    62,    63,   132,   133,   134,   241,   242,   243,    32,
      33,    24,   189,   265,    69,   119,   120,    34,    25,    46,
      47,    48,    49,    85,   167,   168,    51,    86,   141,    53,
      54,    55,    56,   222,   250,   251,   252,    57,    58,    59,
      60,    93,   142,    88,    89,    90,   143
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,   174,   138,    50,   114,    87,     8,    94,   170,    96,
      64,   139,   140,   161,   236,    99,    36,    52,     1,    92,
      50,    52,   194,    99,    50,    37,    38,    39,   116,   268,
      40,    41,    75,   100,   117,     3,    95,   118,   195,   185,
     196,   164,    42,   197,     4,    43,   144,   269,    99,    10,
     153,    36,     5,   162,   204,    65,    11,   154,   155,   137,
      37,    38,    39,     9,   169,    40,    41,   171,     7,   121,
     220,     7,    44,   123,   223,    45,    52,    42,    52,    50,
      43,    50,   215,   216,   217,   218,   219,   212,   213,   214,
     277,   186,   156,   157,   158,   110,   175,   129,   113,   124,
     125,   145,   199,   200,   159,   160,   126,   259,   260,   127,
      45,    14,   110,   128,   129,   130,   224,   182,   183,   184,
      15,   278,    10,   225,    52,   146,    26,    50,   191,   237,
     238,   147,   148,   149,   150,   151,   152,   201,    28,   279,
      52,   221,   227,    50,    52,   146,    31,    50,   176,   178,
      35,   147,   148,   149,   150,   151,   152,    76,    61,   275,
     249,    68,   272,   146,   176,   178,    10,   248,    71,   147,
     148,   149,   150,   151,   152,    10,    79,   187,    80,   287,
     282,    81,    82,    83,    84,   165,   121,   285,   286,    66,
     169,   246,   255,   146,   102,   247,   103,   104,   244,   147,
     148,   149,   150,   151,   152,    10,   249,   231,   281,    73,
      74,    91,   266,   267,   146,    97,   101,    76,   105,   107,
     147,   148,   149,   150,   151,   152,   122,   135,    77,    78,
     136,   172,    52,   108,   109,    50,    79,   102,    80,   103,
     104,    81,    82,    83,    84,   -94,   110,   111,   112,   113,
      52,    98,   179,    50,   163,   180,   190,    52,    52,   181,
      50,    50,    10,   244,   232,   188,   -94,   146,   -94,   -94,
      10,    98,   263,   147,   148,   149,   150,   151,   152,    16,
      17,   205,   206,   207,   208,   209,   210,   211,   270,   -25,
     271,    18,   193,   110,   177,   129,   113,    19,   146,   192,
      20,    21,   198,    22,   147,   148,   149,   150,   151,   152,
     110,   128,   129,   113,   202,   201,   146,    10,   228,   229,
     230,   203,   147,   148,   149,   150,   151,   152,   234,   124,
     125,   235,   239,   245,   226,   124,   125,   240,   253,   127,
     258,   256,   110,   128,   129,   130,   146,   257,   110,   128,
     129,   113,   147,   148,   149,   150,   151,   152,   262,   261,
     264,   273,   276,   283,   284,   106,    67,    72,   274,   233,
      70,   166,   173,     0,     0,   254,   280
};

static const yytype_int16 yycheck[] =
{
      21,   105,    76,    21,    66,    39,     5,    41,    99,    43,
       5,    77,    78,    10,   189,     5,     3,    38,     4,    40,
      38,    42,     6,     5,    42,    12,    13,    14,    19,     5,
      17,    18,    38,    23,    25,    62,    42,    28,    22,     5,
      24,    23,    29,    27,     0,    32,    80,    23,     5,    50,
      39,     3,    56,    50,   145,    50,    57,    46,    47,    16,
      12,    13,    14,     5,    98,    17,    18,   101,    62,    68,
     161,    62,    59,    20,   165,    62,    97,    29,    99,    97,
      32,    99,   156,   157,   158,   159,   160,   153,   154,   155,
     265,    57,    36,    37,    38,    59,    60,    61,    62,    46,
      47,    10,   136,   137,    48,    49,    53,   239,   240,    56,
      62,    62,    59,    60,    61,    62,    50,   116,   117,   118,
      51,    10,    50,    57,   145,    34,    59,   145,   127,    57,
     192,    40,    41,    42,    43,    44,    45,    50,    62,    10,
     161,   162,    55,   161,   165,    34,    62,   165,   108,   109,
      62,    40,    41,    42,    43,    44,    45,    35,    62,   263,
     222,    56,   253,    34,   124,   125,    50,   201,    52,    40,
      41,    42,    43,    44,    45,    50,    54,    52,    56,   283,
     271,    59,    60,    61,    62,    26,   185,   278,   279,    45,
     224,    11,   226,    34,    51,    15,    53,    54,   197,    40,
      41,    42,    43,    44,    45,    50,   268,    52,   270,    59,
      62,    62,   246,   247,    34,    52,    30,    35,    45,    59,
      40,    41,    42,    43,    44,    45,     5,    52,    46,    47,
      30,    62,   253,    46,    47,   253,    54,    51,    56,    53,
      54,    59,    60,    61,    62,    30,    59,    60,    61,    62,
     271,    56,     5,   271,    21,     5,    62,   278,   279,     5,
     278,   279,    50,   262,    52,     9,    51,    34,    53,    54,
      50,    56,    52,    40,    41,    42,    43,    44,    45,     7,
       8,   146,   147,   148,   149,   150,   151,   152,    50,    58,
      52,    19,     5,    59,    60,    61,    62,    25,    34,    58,
      28,    29,    62,    31,    40,    41,    42,    43,    44,    45,
      59,    60,    61,    62,    55,    50,    34,    50,     5,     5,
       5,    57,    40,    41,    42,    43,    44,    45,    62,    46,
      47,     5,    54,     5,    52,    46,    47,    21,    33,    56,
       5,    62,    59,    60,    61,    62,    34,    62,    59,    60,
      61,    62,    40,    41,    42,    43,    44,    45,     5,    23,
       9,    55,     5,    21,     5,    62,    29,    35,   262,   185,
      32,    97,   104,    -1,    -1,   224,   268
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    64,    62,     0,    56,    65,    62,    90,     5,
      50,    57,    66,    67,    62,    51,     7,     8,    19,    25,
      28,    29,    31,    68,    84,    91,    59,    69,    62,    70,
      71,    62,    82,    83,    90,    62,     3,    12,    13,    14,
      17,    18,    29,    32,    59,    62,    92,    93,    94,    95,
      96,    99,   100,   102,   103,   104,   105,   110,   111,   112,
     113,    62,    74,    75,     5,    50,    45,    71,    56,    87,
      83,    52,    87,    59,    62,    92,    35,    46,    47,    54,
      56,    59,    60,    61,    62,    96,   100,   115,   116,   117,
     118,    62,   100,   114,   115,    92,   115,    52,    56,     5,
      23,    30,    51,    53,    54,    45,    75,    59,    46,    47,
      59,    60,    61,    62,    72,    73,    19,    25,    28,    88,
      89,    90,     5,    20,    46,    47,    53,    56,    60,    61,
      62,    72,    76,    77,    78,    52,    30,    16,   118,   117,
     117,   101,   115,   119,   115,    10,    34,    40,    41,    42,
      43,    44,    45,    39,    46,    47,    36,    37,    38,    48,
      49,    10,    50,    21,    23,    26,    94,    97,    98,   115,
      93,   115,    62,   101,    76,    60,    73,    60,    73,     5,
       5,     5,    90,    90,    90,     5,    57,    52,     9,    85,
      62,    90,    58,     5,     6,    22,    24,    27,    62,   115,
     115,    50,    55,    57,    93,   116,   116,   116,   116,   116,
     116,   116,   117,   117,   117,   118,   118,   118,   118,   118,
      93,   100,   106,    93,    50,    57,    52,    55,     5,     5,
       5,    52,    52,    89,    62,     5,    66,    57,    72,    54,
      21,    79,    80,    81,    90,     5,    11,    15,   115,    72,
     107,   108,   109,    33,    98,   115,    62,    62,     5,    78,
      78,    23,     5,    52,     9,    86,   115,   115,     5,    23,
      50,    52,    93,    55,    81,    76,     5,    66,    10,    10,
     108,    72,    93,    21,     5,    93,    93,    76
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    67,    67,    68,    68,
      68,    68,    68,    69,    69,    70,    70,    71,    71,    71,
      71,    71,    72,    72,    72,    73,    73,    73,    73,    74,
      74,    75,    76,    76,    76,    76,    76,    76,    77,    77,
      78,    78,    78,    79,    80,    80,    81,    81,    82,    82,
      83,    84,    84,    85,    84,    86,    84,    87,    87,    88,
      88,    89,    89,    89,    89,    90,    90,    91,    92,    92,
      93,    93,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    95,    95,    96,    97,    97,    98,    98,
      99,   100,   100,   100,   100,   101,   101,   102,   103,   104,
     104,   106,   105,   107,   107,   108,   108,   109,   109,   110,
     111,   112,   112,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   116,   116,   116,   116,
     117,   117,   117,   117,   117,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   119,   119
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     5,     5,
       4,     4,     1,     2,     2,     1,     1,     1,     1,     2,
       1,     4,     1,     2,     7,     4,     4,     2,     1,     0,
       3,     1,     3,     1,     3,     1,     3,     0,     2,     1,
       4,     6,     8,     0,     7,     0,     9,     3,     0,     3,
       1,     2,     4,     4,     3,     3,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     4,     3,     1,     1,     3,
       3,     4,     3,     2,     1,     3,     1,     3,     2,     4,
       6,     0,     6,     3,     1,     3,     0,     3,     1,     4,
       4,     8,     8,     4,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     3,     2,     3,     1,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = PASCAL_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == PASCAL_YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use PASCAL_YYerror or PASCAL_YYUNDEF. */
#define YYERRCODE PASCAL_YYUNDEF


/* Enable debugging if requested.  */
#if PASCAL_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !PASCAL_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !PASCAL_YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = PASCAL_YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == PASCAL_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= PASCAL_YYEOF)
    {
      yychar = PASCAL_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == PASCAL_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = PASCAL_YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = PASCAL_YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 340 "pascal.y"
        { tree_t *body = (yyvsp[-1].node);
          if (g_pas_narray > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_narray; i++) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_fill(g_pas_arrays[i].high)));
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0); emit_proc(&g_pascal_procs, mainp);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
#line 1722 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 358 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1728 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 379 "pascal.y"
                                           { pas_rconst_add((yyvsp[-3].str), (yyvsp[-1].dval)); }
#line 1734 "pascal.tab.c"
    break;

  case 18: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 380 "pascal.y"
                                          { pas_rconst_add((yyvsp[-4].str), (yyvsp[-1].dval)); }
#line 1740 "pascal.tab.c"
    break;

  case 19: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 381 "pascal.y"
                                           { pas_rconst_add((yyvsp[-4].str), -(yyvsp[-1].dval)); }
#line 1746 "pascal.tab.c"
    break;

  case 20: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 382 "pascal.y"
                                       { if ((yyvsp[-1].str) && strlen((yyvsp[-1].str))==1) pas_const_add((yyvsp[-3].str),(long long)(unsigned char)(yyvsp[-1].str)[0]); else pas_sconst_add((yyvsp[-3].str),(yyvsp[-1].str)); }
#line 1752 "pascal.tab.c"
    break;

  case 21: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 383 "pascal.y"
                                    { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 1758 "pascal.tab.c"
    break;

  case 22: /* constant: scalar_constant  */
#line 385 "pascal.y"
                    { (yyval.ival) = (yyvsp[0].ival); }
#line 1764 "pascal.tab.c"
    break;

  case 23: /* constant: PLUS scalar_constant  */
#line 385 "pascal.y"
                                                        { (yyval.ival) = (yyvsp[0].ival); }
#line 1770 "pascal.tab.c"
    break;

  case 24: /* constant: MINUS scalar_constant  */
#line 385 "pascal.y"
                                                                                             { (yyval.ival) = -(yyvsp[0].ival); }
#line 1776 "pascal.tab.c"
    break;

  case 25: /* scalar_constant: IDENT  */
#line 386 "pascal.y"
                       { long long cv = 0; pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 1782 "pascal.tab.c"
    break;

  case 26: /* scalar_constant: INTCONST  */
#line 386 "pascal.y"
                                                                                         { (yyval.ival) = (yyvsp[0].ival); }
#line 1788 "pascal.tab.c"
    break;

  case 27: /* scalar_constant: REALCONST  */
#line 386 "pascal.y"
                                                                                                                  { (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 1794 "pascal.tab.c"
    break;

  case 28: /* scalar_constant: STRINGCONST  */
#line 386 "pascal.y"
                                                                                                                                                        { (yyval.ival) = ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) ? (long long)(unsigned char)(yyvsp[0].str)[0] : 0; }
#line 1800 "pascal.tab.c"
    break;

  case 31: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 391 "pascal.y"
                                     { if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str)); pas_pend_reset(); }
#line 1806 "pascal.tab.c"
    break;

  case 32: /* type: simple_type  */
#line 393 "pascal.y"
                { (yyval.ival) = (g_pas_pend_ptrtarget ? -3 : -1); }
#line 1812 "pascal.tab.c"
    break;

  case 33: /* type: ARROW IDENT  */
#line 394 "pascal.y"
                  { g_pas_pend_ptrtarget = strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 1818 "pascal.tab.c"
    break;

  case 34: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 395 "pascal.y"
                                                             { g_pas_pend_ptrtarget = NULL; (yyval.ival) = (yyvsp[-3].ival); }
#line 1824 "pascal.tab.c"
    break;

  case 35: /* type: packed_opt RECORDSY record_body ENDSY  */
#line 396 "pascal.y"
                                            { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1830 "pascal.tab.c"
    break;

  case 36: /* type: packed_opt SETSY OFSY simple_type  */
#line 397 "pascal.y"
                                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -2; }
#line 1836 "pascal.tab.c"
    break;

  case 37: /* type: packed_opt FILESY  */
#line 398 "pascal.y"
                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1842 "pascal.tab.c"
    break;

  case 40: /* simple_type: LPARENT id_list RPARENT  */
#line 403 "pascal.y"
        { int _eo = 0;
          if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) {
              tree_t *_id = (yyvsp[-1].list)->items[i];
              if (_id && _id->v.sval) pas_const_add(_id->v.sval, (long long)(_eo++)); }
          (yyval.ival) = -1; }
#line 1852 "pascal.tab.c"
    break;

  case 41: /* simple_type: IDENT  */
#line 408 "pascal.y"
            { g_pas_pend_typename = strdup((yyvsp[0].str)); g_pas_pend_ischar = !strcmp((yyvsp[0].str), "char"); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); (yyval.ival) = -3; } else { pas_rectype_to_pend((yyvsp[0].str)); (yyval.ival) = -1; } }
#line 1858 "pascal.tab.c"
    break;

  case 42: /* simple_type: constant DOTDOT constant  */
#line 409 "pascal.y"
                               { (yyval.ival) = (yyvsp[0].ival); }
#line 1864 "pascal.tab.c"
    break;

  case 46: /* record_field: id_list COLON type  */
#line 419 "pascal.y"
                       { if ((yyvsp[-2].list)) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_pend_add((yyvsp[-2].list)->items[i]->v.sval); }
#line 1870 "pascal.tab.c"
    break;

  case 50: /* var_decl: id_list COLON type SEMICOLON  */
#line 426 "pascal.y"
                                       { if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if ((yyvsp[-1].ival) == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ((yyvsp[-1].ival) >= 0) pas_array_add(id->v.sval, (yyvsp[-1].ival)); if ((yyvsp[-1].ival) == -2) pas_setvar_add(id->v.sval); if (g_pas_pend_ischar) pas_charvar_add(id->v.sval); if (g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } } pas_local_add(id->v.sval); } } pas_pend_reset(); }
#line 1876 "pascal.tab.c"
    break;

  case 51: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 428 "pascal.y"
                                                                       { }
#line 1882 "pascal.tab.c"
    break;

  case 52: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 429 "pascal.y"
                                                                                    { pas_func_add((yyvsp[-6].str)); }
#line 1888 "pascal.tab.c"
    break;

  case 53: /* $@1: %empty  */
#line 430 "pascal.y"
                                                     { pas_proc_enter(); }
#line 1894 "pascal.tab.c"
    break;

  case 54: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON $@1 block SEMICOLON  */
#line 431 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), (yyvsp[-4].list), (yyvsp[-1].node), 0, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1902 "pascal.tab.c"
    break;

  case 55: /* $@2: %empty  */
#line 434 "pascal.y"
                                                                { pas_func_add((yyvsp[-4].str)); pas_proc_enter(); }
#line 1908 "pascal.tab.c"
    break;

  case 56: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON $@2 block SEMICOLON  */
#line 435 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-7].str), (yyvsp[-6].list), (yyvsp[-1].node), 1, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1916 "pascal.tab.c"
    break;

  case 57: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 440 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 1922 "pascal.tab.c"
    break;

  case 58: /* parameter_list_opt: %empty  */
#line 441 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 1928 "pascal.tab.c"
    break;

  case 59: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 444 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1934 "pascal.tab.c"
    break;

  case 60: /* parameter_decl_list: parameter_decl  */
#line 445 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 1940 "pascal.tab.c"
    break;

  case 61: /* parameter_decl: PROCEDURESY id_list  */
#line 448 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 1946 "pascal.tab.c"
    break;

  case 62: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 449 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 1952 "pascal.tab.c"
    break;

  case 63: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 450 "pascal.y"
                                { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 1958 "pascal.tab.c"
    break;

  case 64: /* parameter_decl: id_list COLON IDENT  */
#line 451 "pascal.y"
                          { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); if (!strcmp((yyvsp[0].str), "char")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_charvar_add((yyvsp[-2].list)->items[i]->v.sval); (yyval.list) = (yyvsp[-2].list); }
#line 1964 "pascal.tab.c"
    break;

  case 65: /* id_list: id_list COMMA IDENT  */
#line 454 "pascal.y"
                        { pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 1970 "pascal.tab.c"
    break;

  case 66: /* id_list: IDENT  */
#line 455 "pascal.y"
            { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 1976 "pascal.tab.c"
    break;

  case 67: /* body: BEGINSY statement_list ENDSY  */
#line 458 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 1982 "pascal.tab.c"
    break;

  case 68: /* statement_list: statement_list SEMICOLON statement  */
#line 461 "pascal.y"
                                       { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1988 "pascal.tab.c"
    break;

  case 69: /* statement_list: statement  */
#line 462 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 1994 "pascal.tab.c"
    break;

  case 70: /* statement: statement_no_label  */
#line 465 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2000 "pascal.tab.c"
    break;

  case 71: /* statement: INTCONST COLON statement_no_label  */
#line 467 "pascal.y"
        { char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)(yyvsp[-2].ival));
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = strdup(_lb); ast_push(L, (yyvsp[0].node)); (yyval.node) = L; }
#line 2007 "pascal.tab.c"
    break;

  case 72: /* statement_no_label: assignment  */
#line 471 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2013 "pascal.tab.c"
    break;

  case 73: /* statement_no_label: call  */
#line 472 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2019 "pascal.tab.c"
    break;

  case 74: /* statement_no_label: compound_statement  */
#line 473 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2025 "pascal.tab.c"
    break;

  case 75: /* statement_no_label: goto_statement  */
#line 474 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2031 "pascal.tab.c"
    break;

  case 76: /* statement_no_label: if_statement  */
#line 475 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2037 "pascal.tab.c"
    break;

  case 77: /* statement_no_label: case_statement  */
#line 476 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2043 "pascal.tab.c"
    break;

  case 78: /* statement_no_label: while_statement  */
#line 477 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2049 "pascal.tab.c"
    break;

  case 79: /* statement_no_label: repeat_statement  */
#line 478 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2055 "pascal.tab.c"
    break;

  case 80: /* statement_no_label: for_statement  */
#line 479 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2061 "pascal.tab.c"
    break;

  case 81: /* statement_no_label: with_statement  */
#line 480 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2067 "pascal.tab.c"
    break;

  case 82: /* statement_no_label: %empty  */
#line 481 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 2073 "pascal.tab.c"
    break;

  case 83: /* call: IDENT  */
#line 484 "pascal.y"
          { (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 2079 "pascal.tab.c"
    break;

  case 84: /* call: call_with_args  */
#line 485 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2085 "pascal.tab.c"
    break;

  case 85: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 488 "pascal.y"
                                        { (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 2091 "pascal.tab.c"
    break;

  case 86: /* argument_list: argument_list COMMA argument  */
#line 491 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 2097 "pascal.tab.c"
    break;

  case 87: /* argument_list: argument  */
#line 492 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 2103 "pascal.tab.c"
    break;

  case 88: /* argument: expression  */
#line 495 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 2109 "pascal.tab.c"
    break;

  case 89: /* argument: expression COLON expression  */
#line 496 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 2115 "pascal.tab.c"
    break;

  case 90: /* assignment: selector BECOMES expression  */
#line 500 "pascal.y"
        { int _fnsel = ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_FNC && (yyvsp[-2].node)->n == 1 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR && (yyvsp[-2].node)->c[0]->v.sval);
          if ((yyvsp[-2].node) && ((yyvsp[-2].node)->t == TT_VAR || _fnsel) && pas_is_rel((yyvsp[0].node))) {
              tree_t *e = ast_node_new(TT_IF);
              ast_push(e, (yyvsp[0].node));
              ast_push(e, mk_assign((yyvsp[-2].node), ilit(1)));
              tree_t *s2;
              if (_fnsel) { s2 = ast_node_new(TT_FNC); ast_push(s2, leaf_s(TT_VAR, (yyvsp[-2].node)->c[0]->v.sval)); }
              else s2 = leaf_s(TT_VAR, (yyvsp[-2].node)->v.sval);
              ast_push(e, mk_assign(s2, ilit(0)));
              (yyval.node) = e;
          } else { (yyval.node) = mk_assign((yyvsp[-2].node), pas_bool((yyvsp[0].node))); } }
#line 2131 "pascal.tab.c"
    break;

  case 91: /* selector: selector LBRACK expression_list RBRACK  */
#line 513 "pascal.y"
                                           { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); (yyval.node) = e; }
#line 2137 "pascal.tab.c"
    break;

  case 92: /* selector: selector PERIOD IDENT  */
#line 514 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_fi)); (yyval.node) = e; } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } }
#line 2144 "pascal.tab.c"
    break;

  case 93: /* selector: selector ARROW  */
#line 516 "pascal.y"
                     { (yyval.node) = mk_deref((yyvsp[-1].node)); }
#line 2150 "pascal.tab.c"
    break;

  case 94: /* selector: IDENT  */
#line 517 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 2156 "pascal.tab.c"
    break;

  case 95: /* expression_list: expression_list COMMA expression  */
#line 520 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2162 "pascal.tab.c"
    break;

  case 96: /* expression_list: expression  */
#line 521 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2168 "pascal.tab.c"
    break;

  case 97: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 524 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 2174 "pascal.tab.c"
    break;

  case 98: /* goto_statement: GOTOSY INTCONST  */
#line 528 "pascal.y"
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)(yyvsp[0].ival));
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = strdup(_gb); (yyval.node) = G; }
#line 2181 "pascal.tab.c"
    break;

  case 99: /* if_statement: IFSY expression THENSY statement  */
#line 532 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2187 "pascal.tab.c"
    break;

  case 100: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 533 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond((yyvsp[-4].node))); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2193 "pascal.tab.c"
    break;

  case 101: /* $@3: %empty  */
#line 536 "pascal.y"
                           { pas_case_push(); }
#line 2199 "pascal.tab.c"
    break;

  case 102: /* case_statement: CASESY expression OFSY $@3 case_list ENDSY  */
#line 537 "pascal.y"
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), (yyvsp[-4].node)));
          tree_t *chain = NULL;
          if ((yyvsp[-1].list)) for (int i = (yyvsp[-1].list)->count - 1; i >= 0; i--) { tree_t *e = (yyvsp[-1].list)->items[i]; if (!e) continue; if (chain) ast_push(e, chain); chain = e; }
          ast_push(seq, chain ? chain : ast_node_new(TT_SUCCEED));
          pas_case_pop();
          (yyval.node) = seq; }
#line 2211 "pascal.tab.c"
    break;

  case 103: /* case_list: case_list SEMICOLON case_elem  */
#line 546 "pascal.y"
                                  { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2217 "pascal.tab.c"
    break;

  case 104: /* case_list: case_elem  */
#line 547 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2223 "pascal.tab.c"
    break;

  case 105: /* case_elem: constant_list COLON statement  */
#line 550 "pascal.y"
                                  { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2229 "pascal.tab.c"
    break;

  case 106: /* case_elem: %empty  */
#line 551 "pascal.y"
      { (yyval.node) = NULL; }
#line 2235 "pascal.tab.c"
    break;

  case 107: /* constant_list: constant_list COMMA constant  */
#line 554 "pascal.y"
                                 { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival)))); }
#line 2241 "pascal.tab.c"
    break;

  case 108: /* constant_list: constant  */
#line 555 "pascal.y"
               { (yyval.node) = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival))); }
#line 2247 "pascal.tab.c"
    break;

  case 109: /* while_statement: WHILESY expression DOSY statement  */
#line 558 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2253 "pascal.tab.c"
    break;

  case 110: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 561 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond((yyvsp[0].node))); }
#line 2259 "pascal.tab.c"
    break;

  case 111: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 565 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2265 "pascal.tab.c"
    break;

  case 112: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 567 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); e->v.ival = 1; (yyval.node) = e; }
#line 2271 "pascal.tab.c"
    break;

  case 113: /* with_statement: WITHSY with_open DOSY statement  */
#line 570 "pascal.y"
                                    { long long n = (yyvsp[-2].ival); for (long long i = 0; i < n; i++) pas_with_pop(); (yyval.node) = (yyvsp[0].node); }
#line 2277 "pascal.tab.c"
    break;

  case 114: /* with_open: with_open COMMA selector  */
#line 573 "pascal.y"
                             { pas_with_push((yyvsp[0].node)); (yyval.ival) = (yyvsp[-2].ival) + 1; }
#line 2283 "pascal.tab.c"
    break;

  case 115: /* with_open: selector  */
#line 574 "pascal.y"
               { pas_with_push((yyvsp[0].node)); (yyval.ival) = 1; }
#line 2289 "pascal.tab.c"
    break;

  case 116: /* expression: simple_expression  */
#line 581 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2295 "pascal.tab.c"
    break;

  case 117: /* expression: expression INOP simple_expression  */
#line 582 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2301 "pascal.tab.c"
    break;

  case 118: /* expression: expression LTOP simple_expression  */
#line 583 "pascal.y"
                                        { (yyval.node) = bin(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2307 "pascal.tab.c"
    break;

  case 119: /* expression: expression LEOP simple_expression  */
#line 584 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2313 "pascal.tab.c"
    break;

  case 120: /* expression: expression GTOP simple_expression  */
#line 585 "pascal.y"
                                        { (yyval.node) = bin(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2319 "pascal.tab.c"
    break;

  case 121: /* expression: expression GEOP simple_expression  */
#line 586 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2325 "pascal.tab.c"
    break;

  case 122: /* expression: expression NEOP simple_expression  */
#line 587 "pascal.y"
                                        { (yyval.node) = bin(TT_NE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2331 "pascal.tab.c"
    break;

  case 123: /* expression: expression EQOP simple_expression  */
#line 588 "pascal.y"
                                        { (yyval.node) = bin(TT_EQ, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2337 "pascal.tab.c"
    break;

  case 124: /* simple_expression: term  */
#line 591 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2343 "pascal.tab.c"
    break;

  case 125: /* simple_expression: PLUS term  */
#line 592 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2349 "pascal.tab.c"
    break;

  case 126: /* simple_expression: MINUS term  */
#line 593 "pascal.y"
                 { (yyval.node) = un(TT_MNS, (yyvsp[0].node)); }
#line 2355 "pascal.tab.c"
    break;

  case 127: /* simple_expression: simple_expression PLUS term  */
#line 594 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2361 "pascal.tab.c"
    break;

  case 128: /* simple_expression: simple_expression MINUS term  */
#line 595 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2367 "pascal.tab.c"
    break;

  case 129: /* simple_expression: simple_expression OROP term  */
#line 596 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2373 "pascal.tab.c"
    break;

  case 130: /* term: factor  */
#line 599 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2379 "pascal.tab.c"
    break;

  case 131: /* term: term MUL factor  */
#line 600 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2385 "pascal.tab.c"
    break;

  case 132: /* term: term RDIV factor  */
#line 601 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2391 "pascal.tab.c"
    break;

  case 133: /* term: term IDIV factor  */
#line 602 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2397 "pascal.tab.c"
    break;

  case 134: /* term: term IMOD factor  */
#line 603 "pascal.y"
                       { (yyval.node) = bin(TT_MOD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2403 "pascal.tab.c"
    break;

  case 135: /* term: term ANDOP factor  */
#line 604 "pascal.y"
                        { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2409 "pascal.tab.c"
    break;

  case 136: /* factor: selector  */
#line 607 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2415 "pascal.tab.c"
    break;

  case 137: /* factor: call_with_args  */
#line 608 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2421 "pascal.tab.c"
    break;

  case 138: /* factor: INTCONST  */
#line 609 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 2427 "pascal.tab.c"
    break;

  case 139: /* factor: REALCONST  */
#line 610 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 2433 "pascal.tab.c"
    break;

  case 140: /* factor: STRINGCONST  */
#line 611 "pascal.y"
                  { if ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((long long)(unsigned char)(yyvsp[0].str)[0])); (yyval.node) = _cl; } else (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 2439 "pascal.tab.c"
    break;

  case 141: /* factor: LPARENT expression RPARENT  */
#line 612 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2445 "pascal.tab.c"
    break;

  case 142: /* factor: NOTSY factor  */
#line 613 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 2451 "pascal.tab.c"
    break;

  case 143: /* factor: LBRACK expression_list_opt RBRACK  */
#line 614 "pascal.y"
                                        { (yyval.node) = mk_set_ctor((yyvsp[-1].list)); }
#line 2457 "pascal.tab.c"
    break;

  case 144: /* expression_list_opt: expression_list  */
#line 617 "pascal.y"
                    { (yyval.list) = (yyvsp[0].list); }
#line 2463 "pascal.tab.c"
    break;

  case 145: /* expression_list_opt: %empty  */
#line 618 "pascal.y"
      { (yyval.list) = NULL; }
#line 2469 "pascal.tab.c"
    break;


#line 2473 "pascal.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == PASCAL_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= PASCAL_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == PASCAL_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = PASCAL_YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != PASCAL_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 620 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0; g_pas_ncharvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0; g_with_depth = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
