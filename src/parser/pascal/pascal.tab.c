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
static tree_t *mk_deref(tree_t *ptr) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr);
    return e;
}
static tree_t *mk_call(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "ord") && args && args->count >= 1) return args->items[0];
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
            for (int i = 0; i < args->count; i++) ast_push(e, args->items[i]);
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
static struct { char *name; long long high; } g_pas_arrays[256]; static int g_pas_narray;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_narray++; } }
#define PAS_REC_MAX 64
#define PAS_FIELD_MAX 32
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; int nf; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct { char *vname; char *fields[PAS_FIELD_MAX]; int nf; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename;
static struct { char *pname; char *rname; } g_pas_ptrtypes[PAS_REC_MAX]; static int g_pas_nptrtype;
static void pas_ptrtype_add(const char *p, const char *r) { if (g_pas_nptrtype < PAS_REC_MAX && p && r) { int k = g_pas_nptrtype++; g_pas_ptrtypes[k].pname = strdup(p); g_pas_ptrtypes[k].rname = strdup(r); } }
static const char *pas_ptrtype_target(const char *p) { if (!p) return NULL; for (int i = 0; i < g_pas_nptrtype; i++) if (g_pas_ptrtypes[i].pname && !strcmp(g_pas_ptrtypes[i].pname, p)) return g_pas_ptrtypes[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_ptrvars[PAS_REC_MAX]; static int g_pas_nptrvar;
static void pas_ptrvar_add(const char *v, const char *r) { if (g_pas_nptrvar < PAS_REC_MAX && v && r) { int k = g_pas_nptrvar++; g_pas_ptrvars[k].vname = strdup(v); g_pas_ptrvars[k].rname = strdup(r); } }
static const char *pas_ptrvar_target(const char *v) { if (!v) return NULL; for (int i = 0; i < g_pas_nptrvar; i++) if (g_pas_ptrvars[i].vname && !strcmp(g_pas_ptrvars[i].vname, v)) return g_pas_ptrvars[i].rname; return NULL; }
static void pas_pend_reset(void) { g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; }
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
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
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
    long long cv; if (pas_const_get(name, &cv)) return ilit(cv);
    if (pas_is_func(name)) return mk_call(name, NULL);
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

#line 302 "pascal.tab.c"

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
  YYSYMBOL_selector_list = 114,            /* selector_list  */
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
#define YYLAST   355

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  141
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  278

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
       0,   257,   257,   272,   273,   276,   279,   280,   283,   284,
     285,   286,   287,   290,   291,   294,   295,   297,   299,   299,
     299,   299,   300,   300,   300,   302,   303,   305,   307,   308,
     309,   310,   311,   312,   314,   314,   316,   322,   323,   326,
     329,   330,   333,   334,   337,   338,   340,   342,   343,   344,
     344,   348,   348,   354,   355,   358,   359,   362,   363,   364,
     365,   368,   369,   372,   375,   376,   379,   380,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   398,
     399,   402,   405,   406,   409,   410,   413,   427,   428,   430,
     431,   434,   435,   438,   441,   446,   447,   450,   450,   460,
     461,   464,   465,   468,   469,   472,   475,   478,   480,   484,
     487,   488,   491,   492,   493,   494,   495,   496,   497,   498,
     501,   502,   503,   504,   505,   506,   509,   510,   511,   512,
     513,   514,   517,   518,   519,   520,   521,   522,   523,   524,
     527,   528
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
  "with_statement", "selector_list", "expression", "simple_expression",
  "term", "factor", "expression_list_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-171)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       9,   -46,    26,    14,  -171,    22,    56,  -171,   -11,  -171,
      30,  -171,    61,   262,  -171,  -171,    66,    54,    76,    22,
     102,    15,   110,  -171,  -171,  -171,  -171,     0,   139,    54,
    -171,   133,    22,  -171,    51,   133,   126,   137,    15,   177,
     153,   177,    15,   177,   148,   -16,    25,  -171,  -171,  -171,
    -171,  -171,    29,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,   185,   110,  -171,  -171,   173,   167,  -171,     6,   230,
    -171,    71,   189,  -171,   212,    57,   199,   199,   199,   177,
     177,  -171,  -171,  -171,   190,  -171,   103,    65,    82,   114,
    -171,  -171,   103,     5,   223,    37,   274,    40,   177,    15,
    -171,   177,   183,  -171,   177,    71,  -171,  -171,    86,    86,
    -171,  -171,  -171,  -171,   247,  -171,    22,    22,    22,     7,
    -171,   130,   245,  -171,   194,    22,   204,   220,   275,   120,
    -171,   217,   177,   177,  -171,   114,   114,   232,   310,   228,
     254,    15,   177,   177,   177,   177,   177,   177,   177,   199,
     199,   199,   199,   199,   199,   199,   199,    15,   153,  -171,
    -171,    15,  -171,    -1,  -171,   286,  -171,   310,  -171,   -14,
     279,  -171,  -171,  -171,   235,   138,   142,     6,  -171,   224,
     284,  -171,  -171,    84,   167,  -171,   238,  -171,   280,    22,
     297,   176,   310,   177,  -171,  -171,  -171,    82,    82,    82,
      82,    82,    82,    82,   114,   114,   114,  -171,  -171,  -171,
    -171,  -171,  -171,   103,   167,   270,   177,  -171,   177,  -171,
    -171,   242,   243,  -171,  -171,  -171,   301,  -171,  -171,   287,
     287,   289,   302,  -171,   146,   300,   177,   177,   310,  -171,
      88,  -171,   159,    15,  -171,   310,  -171,  -171,  -171,   255,
    -171,  -171,    22,    71,   308,  -171,   125,   163,   167,  -171,
     167,    15,  -171,   303,  -171,  -171,  -171,   316,    15,    15,
    -171,  -171,  -171,    71,  -171,  -171,  -171,  -171
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    62,     0,     7,
       0,     3,     0,     0,    61,     2,     0,     0,     0,     0,
       0,    78,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    54,    11,    45,     0,    54,     0,     0,    78,     0,
       0,     0,    78,     0,     0,    79,     0,    65,    66,    69,
      80,    68,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    10,    26,     8,     0,     0,    15,     0,     0,
      44,    35,     0,    94,     0,     0,     0,     0,     0,   141,
       0,   134,   135,   136,    90,   133,   132,     0,   112,   120,
     126,    90,   111,     0,     0,     0,     0,    78,     0,    78,
      63,     0,     0,    89,     0,    35,    25,    13,     0,     0,
      23,    24,    18,    22,     0,    19,     0,     0,     0,     0,
      56,     0,    49,    34,     0,     0,    37,     0,     0,     0,
      28,     0,     0,     0,   138,   121,   122,   140,    92,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,    97,
      93,    78,    67,     0,    83,    84,    64,    86,    88,     0,
       0,    20,    21,    17,    57,     0,     0,     0,    53,     0,
       0,     7,    29,     0,     0,    46,     0,    33,     0,    43,
       0,     0,   106,     0,   139,   137,   105,   113,   114,   115,
     116,   117,   118,   119,   125,   123,   124,   129,   130,   131,
     127,   128,   109,   110,   102,    95,     0,    81,     0,    87,
      27,     0,     0,    55,    60,    47,     0,    36,    38,     0,
       0,     0,    39,    41,     0,    51,     0,     0,    91,   104,
       0,   100,     0,    78,    82,    85,    59,    58,    50,     0,
      32,    31,    43,    35,     0,     7,     0,     0,   102,    98,
       0,    78,    96,     0,    40,    42,    48,     0,    78,    78,
      99,   103,   101,    35,    52,   108,   107,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -171,  -171,  -171,  -170,  -171,  -171,  -171,  -171,   293,   -65,
      52,  -171,   261,   -98,  -171,   -53,  -171,  -171,    73,  -171,
     304,  -171,  -171,  -171,   305,  -171,   155,    -3,  -171,    62,
     -90,   240,  -171,   -18,  -171,   119,  -171,   -21,   237,  -171,
    -171,  -171,  -171,  -171,  -171,    81,  -171,  -171,  -171,  -171,
    -171,  -171,   -35,   129,   -55,   -66,  -171
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,   127,
     115,    62,    63,   128,   129,   130,   231,   232,   233,    32,
      33,    24,   181,   255,    69,   119,   120,    34,    25,    46,
      47,    48,    49,    85,   163,   164,    51,    86,   137,    53,
      54,    55,    56,   214,   240,   241,   242,    57,    58,    59,
      60,    93,   138,    88,    89,    90,   139
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,   114,     8,    50,    87,    64,    94,   170,    96,   166,
     134,   226,   177,     1,   -90,   157,     3,    52,    36,    92,
      50,    52,   135,   136,    50,   116,     4,    37,    38,    39,
      99,   117,    40,    41,   118,   -90,   193,   -90,   -90,    10,
      98,   219,    99,    36,    42,   140,    11,    43,   100,   216,
      65,   196,    37,    38,    39,   158,   217,    40,    41,   101,
     160,     9,    99,   165,   178,   121,   167,   212,     7,    42,
       5,   215,    43,   133,    44,   141,    52,    45,    52,    50,
     102,    50,   103,   104,     7,   267,   207,   208,   209,   210,
     211,   123,    14,   258,   204,   205,   206,   191,   192,   142,
      75,    10,    45,    71,    95,   143,   144,   145,   146,   147,
     148,   259,    15,   174,   175,   176,    28,   108,   109,   228,
      52,   149,   183,    50,   124,    26,   186,   125,   150,   151,
     110,   111,   112,   126,    10,   268,    52,   213,    31,    50,
      52,   227,   187,    50,   188,   110,   111,   189,   113,   239,
     152,   153,   154,   262,   102,   265,   103,   104,   238,   142,
     171,   172,   155,   156,    35,   143,   144,   145,   146,   147,
     148,   272,    61,   269,   121,   277,   249,   250,   275,   276,
      10,   165,   179,   245,    66,    73,   234,   236,    10,    68,
     221,   237,    10,   239,   222,   271,    10,   142,   253,    74,
      97,   256,   257,   143,   144,   145,   146,   147,   148,   260,
     142,   261,    76,   108,   109,    91,   143,   144,   145,   146,
     147,   148,    52,    77,    78,    50,   110,   111,   112,   113,
     105,    79,   107,    80,    76,   122,    81,    82,    83,    84,
      52,   131,   132,    50,   159,   168,    98,    52,    52,   234,
      50,    50,   173,    79,   180,    80,   182,   142,    81,    82,
      83,    84,   -22,   143,   144,   145,   146,   147,   148,    16,
      17,   197,   198,   199,   200,   201,   202,   203,   184,   190,
     185,    18,   193,   194,   220,    10,   224,    19,   142,   225,
      20,    21,   229,    22,   143,   144,   145,   146,   147,   148,
     161,   230,   235,   243,   246,   247,   248,   252,   142,   254,
     263,   195,   251,   266,   143,   144,   145,   146,   147,   148,
     142,   274,    67,   106,   273,   264,   143,   144,   145,   146,
     147,   148,   223,   108,   109,   244,    70,   162,   218,   270,
      72,   169,     0,   125,   142,     0,   110,   111,   112,   126,
     143,   144,   145,   146,   147,   148
};

static const yytype_int16 yycheck[] =
{
      21,    66,     5,    21,    39,     5,    41,   105,    43,    99,
      76,   181,     5,     4,    30,    10,    62,    38,     3,    40,
      38,    42,    77,    78,    42,    19,     0,    12,    13,    14,
       5,    25,    17,    18,    28,    51,    50,    53,    54,    50,
      56,    55,     5,     3,    29,    80,    57,    32,    23,    50,
      50,   141,    12,    13,    14,    50,    57,    17,    18,    30,
      23,     5,     5,    98,    57,    68,   101,   157,    62,    29,
      56,   161,    32,    16,    59,    10,    97,    62,    99,    97,
      51,    99,    53,    54,    62,   255,   152,   153,   154,   155,
     156,    20,    62,     5,   149,   150,   151,   132,   133,    34,
      38,    50,    62,    52,    42,    40,    41,    42,    43,    44,
      45,    23,    51,   116,   117,   118,    62,    46,    47,   184,
     141,    39,   125,   141,    53,    59,     6,    56,    46,    47,
      59,    60,    61,    62,    50,    10,   157,   158,    62,   157,
     161,    57,    22,   161,    24,    59,    60,    27,    62,   214,
      36,    37,    38,   243,    51,   253,    53,    54,   193,    34,
     108,   109,    48,    49,    62,    40,    41,    42,    43,    44,
      45,   261,    62,    10,   177,   273,   229,   230,   268,   269,
      50,   216,    52,   218,    45,    59,   189,    11,    50,    56,
      52,    15,    50,   258,    52,   260,    50,    34,    52,    62,
      52,   236,   237,    40,    41,    42,    43,    44,    45,    50,
      34,    52,    35,    46,    47,    62,    40,    41,    42,    43,
      44,    45,   243,    46,    47,   243,    59,    60,    61,    62,
      45,    54,    59,    56,    35,     5,    59,    60,    61,    62,
     261,    52,    30,   261,    21,    62,    56,   268,   269,   252,
     268,   269,     5,    54,     9,    56,    62,    34,    59,    60,
      61,    62,    58,    40,    41,    42,    43,    44,    45,     7,
       8,   142,   143,   144,   145,   146,   147,   148,    58,    62,
       5,    19,    50,    55,     5,    50,    62,    25,    34,     5,
      28,    29,    54,    31,    40,    41,    42,    43,    44,    45,
      26,    21,     5,    33,    62,    62,     5,     5,    34,     9,
      55,    57,    23,     5,    40,    41,    42,    43,    44,    45,
      34,     5,    29,    62,    21,   252,    40,    41,    42,    43,
      44,    45,   177,    46,    47,   216,    32,    97,    52,   258,
      35,   104,    -1,    56,    34,    -1,    59,    60,    61,    62,
      40,    41,    42,    43,    44,    45
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
      89,    90,     5,    20,    53,    56,    62,    72,    76,    77,
      78,    52,    30,    16,   118,   117,   117,   101,   115,   119,
     115,    10,    34,    40,    41,    42,    43,    44,    45,    39,
      46,    47,    36,    37,    38,    48,    49,    10,    50,    21,
      23,    26,    94,    97,    98,   115,    93,   115,    62,   101,
      76,    73,    73,     5,    90,    90,    90,     5,    57,    52,
       9,    85,    62,    90,    58,     5,     6,    22,    24,    27,
      62,   115,   115,    50,    55,    57,    93,   116,   116,   116,
     116,   116,   116,   116,   117,   117,   117,   118,   118,   118,
     118,   118,    93,   100,   106,    93,    50,    57,    52,    55,
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
      68,    68,    68,    69,    69,    70,    70,    71,    72,    72,
      72,    72,    73,    73,    73,    74,    74,    75,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    78,    78,    79,
      80,    80,    81,    81,    82,    82,    83,    84,    84,    85,
      84,    86,    84,    87,    87,    88,    88,    89,    89,    89,
      89,    90,    90,    91,    92,    92,    93,    93,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    95,
      95,    96,    97,    97,    98,    98,    99,   100,   100,   100,
     100,   101,   101,   102,   103,   104,   104,   106,   105,   107,
     107,   108,   108,   109,   109,   110,   111,   112,   112,   113,
     114,   114,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   116,   116,   116,   116,   116,   117,   117,   117,   117,
     117,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     119,   119
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     1,     1,
       2,     2,     1,     1,     1,     2,     1,     4,     1,     2,
       7,     4,     4,     2,     1,     0,     3,     1,     3,     1,
       3,     1,     3,     0,     2,     1,     4,     6,     8,     0,
       7,     0,     9,     3,     0,     3,     1,     2,     4,     4,
       3,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     4,     3,     1,     1,     3,     3,     4,     3,     2,
       1,     3,     1,     3,     2,     4,     6,     0,     6,     3,
       1,     3,     0,     3,     1,     4,     4,     8,     8,     4,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     3,     2,     3,
       1,     0
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
#line 258 "pascal.y"
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
#line 1635 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 276 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1641 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 297 "pascal.y"
                                          { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 1647 "pascal.tab.c"
    break;

  case 18: /* constant: STRINGCONST  */
#line 299 "pascal.y"
                { (yyval.ival) = 0; }
#line 1653 "pascal.tab.c"
    break;

  case 19: /* constant: scalar_constant  */
#line 299 "pascal.y"
                                              { (yyval.ival) = (yyvsp[0].ival); }
#line 1659 "pascal.tab.c"
    break;

  case 20: /* constant: PLUS scalar_constant  */
#line 299 "pascal.y"
                                                                                  { (yyval.ival) = (yyvsp[0].ival); }
#line 1665 "pascal.tab.c"
    break;

  case 21: /* constant: MINUS scalar_constant  */
#line 299 "pascal.y"
                                                                                                                       { (yyval.ival) = -(yyvsp[0].ival); }
#line 1671 "pascal.tab.c"
    break;

  case 22: /* scalar_constant: IDENT  */
#line 300 "pascal.y"
                       { long long cv = 0; pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 1677 "pascal.tab.c"
    break;

  case 23: /* scalar_constant: INTCONST  */
#line 300 "pascal.y"
                                                                                         { (yyval.ival) = (yyvsp[0].ival); }
#line 1683 "pascal.tab.c"
    break;

  case 24: /* scalar_constant: REALCONST  */
#line 300 "pascal.y"
                                                                                                                  { (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 1689 "pascal.tab.c"
    break;

  case 27: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 305 "pascal.y"
                                     { if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str)); pas_pend_reset(); }
#line 1695 "pascal.tab.c"
    break;

  case 28: /* type: simple_type  */
#line 307 "pascal.y"
                { (yyval.ival) = (g_pas_pend_ptrtarget ? -3 : -1); }
#line 1701 "pascal.tab.c"
    break;

  case 29: /* type: ARROW IDENT  */
#line 308 "pascal.y"
                  { g_pas_pend_ptrtarget = strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 1707 "pascal.tab.c"
    break;

  case 30: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 309 "pascal.y"
                                                             { g_pas_pend_ptrtarget = NULL; (yyval.ival) = (yyvsp[-3].ival); }
#line 1713 "pascal.tab.c"
    break;

  case 31: /* type: packed_opt RECORDSY record_body ENDSY  */
#line 310 "pascal.y"
                                            { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1719 "pascal.tab.c"
    break;

  case 32: /* type: packed_opt SETSY OFSY simple_type  */
#line 311 "pascal.y"
                                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -2; }
#line 1725 "pascal.tab.c"
    break;

  case 33: /* type: packed_opt FILESY  */
#line 312 "pascal.y"
                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1731 "pascal.tab.c"
    break;

  case 36: /* simple_type: LPARENT id_list RPARENT  */
#line 317 "pascal.y"
        { int _eo = 0;
          if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) {
              tree_t *_id = (yyvsp[-1].list)->items[i];
              if (_id && _id->v.sval) pas_const_add(_id->v.sval, (long long)(_eo++)); }
          (yyval.ival) = -1; }
#line 1741 "pascal.tab.c"
    break;

  case 37: /* simple_type: IDENT  */
#line 322 "pascal.y"
            { g_pas_pend_typename = strdup((yyvsp[0].str)); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); (yyval.ival) = -3; } else { pas_rectype_to_pend((yyvsp[0].str)); (yyval.ival) = -1; } }
#line 1747 "pascal.tab.c"
    break;

  case 38: /* simple_type: constant DOTDOT constant  */
#line 323 "pascal.y"
                               { (yyval.ival) = (yyvsp[0].ival); }
#line 1753 "pascal.tab.c"
    break;

  case 42: /* record_field: id_list COLON type  */
#line 333 "pascal.y"
                       { if ((yyvsp[-2].list)) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_pend_add((yyvsp[-2].list)->items[i]->v.sval); }
#line 1759 "pascal.tab.c"
    break;

  case 46: /* var_decl: id_list COLON type SEMICOLON  */
#line 340 "pascal.y"
                                       { if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if ((yyvsp[-1].ival) == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ((yyvsp[-1].ival) >= 0) pas_array_add(id->v.sval, (yyvsp[-1].ival)); if ((yyvsp[-1].ival) == -2) pas_setvar_add(id->v.sval); if (g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } } pas_local_add(id->v.sval); } } pas_pend_reset(); }
#line 1765 "pascal.tab.c"
    break;

  case 47: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 342 "pascal.y"
                                                                       { }
#line 1771 "pascal.tab.c"
    break;

  case 48: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 343 "pascal.y"
                                                                                    { pas_func_add((yyvsp[-6].str)); }
#line 1777 "pascal.tab.c"
    break;

  case 49: /* $@1: %empty  */
#line 344 "pascal.y"
                                                     { pas_proc_enter(); }
#line 1783 "pascal.tab.c"
    break;

  case 50: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON $@1 block SEMICOLON  */
#line 345 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), (yyvsp[-4].list), (yyvsp[-1].node), 0, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1791 "pascal.tab.c"
    break;

  case 51: /* $@2: %empty  */
#line 348 "pascal.y"
                                                                { pas_func_add((yyvsp[-4].str)); pas_proc_enter(); }
#line 1797 "pascal.tab.c"
    break;

  case 52: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON $@2 block SEMICOLON  */
#line 349 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-7].str), (yyvsp[-6].list), (yyvsp[-1].node), 1, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1805 "pascal.tab.c"
    break;

  case 53: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 354 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 1811 "pascal.tab.c"
    break;

  case 54: /* parameter_list_opt: %empty  */
#line 355 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 1817 "pascal.tab.c"
    break;

  case 55: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 358 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1823 "pascal.tab.c"
    break;

  case 56: /* parameter_decl_list: parameter_decl  */
#line 359 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 1829 "pascal.tab.c"
    break;

  case 57: /* parameter_decl: PROCEDURESY id_list  */
#line 362 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 1835 "pascal.tab.c"
    break;

  case 58: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 363 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 1841 "pascal.tab.c"
    break;

  case 59: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 364 "pascal.y"
                                { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 1847 "pascal.tab.c"
    break;

  case 60: /* parameter_decl: id_list COLON IDENT  */
#line 365 "pascal.y"
                          { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); (yyval.list) = (yyvsp[-2].list); }
#line 1853 "pascal.tab.c"
    break;

  case 61: /* id_list: id_list COMMA IDENT  */
#line 368 "pascal.y"
                        { pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 1859 "pascal.tab.c"
    break;

  case 62: /* id_list: IDENT  */
#line 369 "pascal.y"
            { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 1865 "pascal.tab.c"
    break;

  case 63: /* body: BEGINSY statement_list ENDSY  */
#line 372 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 1871 "pascal.tab.c"
    break;

  case 64: /* statement_list: statement_list SEMICOLON statement  */
#line 375 "pascal.y"
                                       { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1877 "pascal.tab.c"
    break;

  case 65: /* statement_list: statement  */
#line 376 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 1883 "pascal.tab.c"
    break;

  case 66: /* statement: statement_no_label  */
#line 379 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1889 "pascal.tab.c"
    break;

  case 67: /* statement: INTCONST COLON statement_no_label  */
#line 381 "pascal.y"
        { char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)(yyvsp[-2].ival));
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = strdup(_lb); ast_push(L, (yyvsp[0].node)); (yyval.node) = L; }
#line 1896 "pascal.tab.c"
    break;

  case 68: /* statement_no_label: assignment  */
#line 385 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1902 "pascal.tab.c"
    break;

  case 69: /* statement_no_label: call  */
#line 386 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1908 "pascal.tab.c"
    break;

  case 70: /* statement_no_label: compound_statement  */
#line 387 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1914 "pascal.tab.c"
    break;

  case 71: /* statement_no_label: goto_statement  */
#line 388 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1920 "pascal.tab.c"
    break;

  case 72: /* statement_no_label: if_statement  */
#line 389 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1926 "pascal.tab.c"
    break;

  case 73: /* statement_no_label: case_statement  */
#line 390 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1932 "pascal.tab.c"
    break;

  case 74: /* statement_no_label: while_statement  */
#line 391 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1938 "pascal.tab.c"
    break;

  case 75: /* statement_no_label: repeat_statement  */
#line 392 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1944 "pascal.tab.c"
    break;

  case 76: /* statement_no_label: for_statement  */
#line 393 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1950 "pascal.tab.c"
    break;

  case 77: /* statement_no_label: with_statement  */
#line 394 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1956 "pascal.tab.c"
    break;

  case 78: /* statement_no_label: %empty  */
#line 395 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 1962 "pascal.tab.c"
    break;

  case 79: /* call: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 1968 "pascal.tab.c"
    break;

  case 80: /* call: call_with_args  */
#line 399 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1974 "pascal.tab.c"
    break;

  case 81: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 402 "pascal.y"
                                        { (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 1980 "pascal.tab.c"
    break;

  case 82: /* argument_list: argument_list COMMA argument  */
#line 405 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1986 "pascal.tab.c"
    break;

  case 83: /* argument_list: argument  */
#line 406 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 1992 "pascal.tab.c"
    break;

  case 84: /* argument: expression  */
#line 409 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 1998 "pascal.tab.c"
    break;

  case 85: /* argument: expression COLON expression  */
#line 410 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 2004 "pascal.tab.c"
    break;

  case 86: /* assignment: selector BECOMES expression  */
#line 414 "pascal.y"
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
#line 2020 "pascal.tab.c"
    break;

  case 87: /* selector: selector LBRACK expression_list RBRACK  */
#line 427 "pascal.y"
                                           { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); (yyval.node) = e; }
#line 2026 "pascal.tab.c"
    break;

  case 88: /* selector: selector PERIOD IDENT  */
#line 428 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_fi)); (yyval.node) = e; } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } }
#line 2033 "pascal.tab.c"
    break;

  case 89: /* selector: selector ARROW  */
#line 430 "pascal.y"
                     { (yyval.node) = mk_deref((yyvsp[-1].node)); }
#line 2039 "pascal.tab.c"
    break;

  case 90: /* selector: IDENT  */
#line 431 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 2045 "pascal.tab.c"
    break;

  case 91: /* expression_list: expression_list COMMA expression  */
#line 434 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2051 "pascal.tab.c"
    break;

  case 92: /* expression_list: expression  */
#line 435 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2057 "pascal.tab.c"
    break;

  case 93: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 438 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 2063 "pascal.tab.c"
    break;

  case 94: /* goto_statement: GOTOSY INTCONST  */
#line 442 "pascal.y"
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)(yyvsp[0].ival));
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = strdup(_gb); (yyval.node) = G; }
#line 2070 "pascal.tab.c"
    break;

  case 95: /* if_statement: IFSY expression THENSY statement  */
#line 446 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2076 "pascal.tab.c"
    break;

  case 96: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 447 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond((yyvsp[-4].node))); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2082 "pascal.tab.c"
    break;

  case 97: /* $@3: %empty  */
#line 450 "pascal.y"
                           { pas_case_push(); }
#line 2088 "pascal.tab.c"
    break;

  case 98: /* case_statement: CASESY expression OFSY $@3 case_list ENDSY  */
#line 451 "pascal.y"
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), (yyvsp[-4].node)));
          tree_t *chain = NULL;
          if ((yyvsp[-1].list)) for (int i = (yyvsp[-1].list)->count - 1; i >= 0; i--) { tree_t *e = (yyvsp[-1].list)->items[i]; if (!e) continue; if (chain) ast_push(e, chain); chain = e; }
          ast_push(seq, chain ? chain : ast_node_new(TT_SUCCEED));
          pas_case_pop();
          (yyval.node) = seq; }
#line 2100 "pascal.tab.c"
    break;

  case 99: /* case_list: case_list SEMICOLON case_elem  */
#line 460 "pascal.y"
                                  { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2106 "pascal.tab.c"
    break;

  case 100: /* case_list: case_elem  */
#line 461 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2112 "pascal.tab.c"
    break;

  case 101: /* case_elem: constant_list COLON statement  */
#line 464 "pascal.y"
                                  { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2118 "pascal.tab.c"
    break;

  case 102: /* case_elem: %empty  */
#line 465 "pascal.y"
      { (yyval.node) = NULL; }
#line 2124 "pascal.tab.c"
    break;

  case 103: /* constant_list: constant_list COMMA constant  */
#line 468 "pascal.y"
                                 { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival)))); }
#line 2130 "pascal.tab.c"
    break;

  case 104: /* constant_list: constant  */
#line 469 "pascal.y"
               { (yyval.node) = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival))); }
#line 2136 "pascal.tab.c"
    break;

  case 105: /* while_statement: WHILESY expression DOSY statement  */
#line 472 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2142 "pascal.tab.c"
    break;

  case 106: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 475 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond((yyvsp[0].node))); }
#line 2148 "pascal.tab.c"
    break;

  case 107: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 479 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2154 "pascal.tab.c"
    break;

  case 108: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 481 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); e->v.ival = 1; (yyval.node) = e; }
#line 2160 "pascal.tab.c"
    break;

  case 109: /* with_statement: WITHSY selector_list DOSY statement  */
#line 484 "pascal.y"
                                        { (yyval.node) = (yyvsp[0].node); }
#line 2166 "pascal.tab.c"
    break;

  case 112: /* expression: simple_expression  */
#line 491 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2172 "pascal.tab.c"
    break;

  case 113: /* expression: expression INOP simple_expression  */
#line 492 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2178 "pascal.tab.c"
    break;

  case 114: /* expression: expression LTOP simple_expression  */
#line 493 "pascal.y"
                                        { (yyval.node) = bin(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2184 "pascal.tab.c"
    break;

  case 115: /* expression: expression LEOP simple_expression  */
#line 494 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2190 "pascal.tab.c"
    break;

  case 116: /* expression: expression GTOP simple_expression  */
#line 495 "pascal.y"
                                        { (yyval.node) = bin(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2196 "pascal.tab.c"
    break;

  case 117: /* expression: expression GEOP simple_expression  */
#line 496 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2202 "pascal.tab.c"
    break;

  case 118: /* expression: expression NEOP simple_expression  */
#line 497 "pascal.y"
                                        { (yyval.node) = bin(TT_NE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2208 "pascal.tab.c"
    break;

  case 119: /* expression: expression EQOP simple_expression  */
#line 498 "pascal.y"
                                        { (yyval.node) = bin(TT_EQ, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2214 "pascal.tab.c"
    break;

  case 120: /* simple_expression: term  */
#line 501 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2220 "pascal.tab.c"
    break;

  case 121: /* simple_expression: PLUS term  */
#line 502 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2226 "pascal.tab.c"
    break;

  case 122: /* simple_expression: MINUS term  */
#line 503 "pascal.y"
                 { (yyval.node) = un(TT_MNS, (yyvsp[0].node)); }
#line 2232 "pascal.tab.c"
    break;

  case 123: /* simple_expression: simple_expression PLUS term  */
#line 504 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2238 "pascal.tab.c"
    break;

  case 124: /* simple_expression: simple_expression MINUS term  */
#line 505 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2244 "pascal.tab.c"
    break;

  case 125: /* simple_expression: simple_expression OROP term  */
#line 506 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2250 "pascal.tab.c"
    break;

  case 126: /* term: factor  */
#line 509 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2256 "pascal.tab.c"
    break;

  case 127: /* term: term MUL factor  */
#line 510 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2262 "pascal.tab.c"
    break;

  case 128: /* term: term RDIV factor  */
#line 511 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2268 "pascal.tab.c"
    break;

  case 129: /* term: term IDIV factor  */
#line 512 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2274 "pascal.tab.c"
    break;

  case 130: /* term: term IMOD factor  */
#line 513 "pascal.y"
                       { (yyval.node) = bin(TT_MOD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2280 "pascal.tab.c"
    break;

  case 131: /* term: term ANDOP factor  */
#line 514 "pascal.y"
                        { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2286 "pascal.tab.c"
    break;

  case 132: /* factor: selector  */
#line 517 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2292 "pascal.tab.c"
    break;

  case 133: /* factor: call_with_args  */
#line 518 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2298 "pascal.tab.c"
    break;

  case 134: /* factor: INTCONST  */
#line 519 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 2304 "pascal.tab.c"
    break;

  case 135: /* factor: REALCONST  */
#line 520 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 2310 "pascal.tab.c"
    break;

  case 136: /* factor: STRINGCONST  */
#line 521 "pascal.y"
                  { (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 2316 "pascal.tab.c"
    break;

  case 137: /* factor: LPARENT expression RPARENT  */
#line 522 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2322 "pascal.tab.c"
    break;

  case 138: /* factor: NOTSY factor  */
#line 523 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 2328 "pascal.tab.c"
    break;

  case 139: /* factor: LBRACK expression_list_opt RBRACK  */
#line 524 "pascal.y"
                                        { (yyval.node) = mk_set_ctor((yyvsp[-1].list)); }
#line 2334 "pascal.tab.c"
    break;

  case 140: /* expression_list_opt: expression_list  */
#line 527 "pascal.y"
                    { (yyval.list) = (yyvsp[0].list); }
#line 2340 "pascal.tab.c"
    break;

  case 141: /* expression_list_opt: %empty  */
#line 528 "pascal.y"
      { (yyval.list) = NULL; }
#line 2346 "pascal.tab.c"
    break;


#line 2350 "pascal.tab.c"

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

#line 530 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
