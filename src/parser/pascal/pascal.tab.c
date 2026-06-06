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

#line 297 "pascal.tab.c"

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
  YYSYMBOL_case_list = 106,                /* case_list  */
  YYSYMBOL_case_elem = 107,                /* case_elem  */
  YYSYMBOL_constant_list = 108,            /* constant_list  */
  YYSYMBOL_while_statement = 109,          /* while_statement  */
  YYSYMBOL_repeat_statement = 110,         /* repeat_statement  */
  YYSYMBOL_for_statement = 111,            /* for_statement  */
  YYSYMBOL_with_statement = 112,           /* with_statement  */
  YYSYMBOL_selector_list = 113,            /* selector_list  */
  YYSYMBOL_expression = 114,               /* expression  */
  YYSYMBOL_simple_expression = 115,        /* simple_expression  */
  YYSYMBOL_term = 116,                     /* term  */
  YYSYMBOL_factor = 117,                   /* factor  */
  YYSYMBOL_expression_list_opt = 118       /* expression_list_opt  */
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
#define YYLAST   353

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  140
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  277

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
       0,   250,   250,   265,   266,   269,   272,   273,   276,   277,
     278,   279,   280,   283,   284,   287,   288,   290,   292,   292,
     292,   292,   293,   293,   293,   295,   296,   298,   300,   301,
     302,   303,   304,   305,   307,   307,   309,   310,   311,   314,
     317,   318,   321,   322,   325,   326,   328,   330,   331,   332,
     332,   336,   336,   342,   343,   346,   347,   350,   351,   352,
     353,   356,   357,   360,   363,   364,   367,   368,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   384,
     385,   388,   391,   392,   395,   396,   399,   413,   414,   416,
     417,   420,   421,   424,   427,   430,   431,   434,   437,   438,
     441,   442,   445,   446,   449,   452,   455,   457,   461,   464,
     465,   468,   469,   470,   471,   472,   473,   474,   475,   478,
     479,   480,   481,   482,   483,   486,   487,   488,   489,   490,
     491,   494,   495,   496,   497,   498,   499,   500,   501,   504,
     505
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
  "if_statement", "case_statement", "case_list", "case_elem",
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

#define YYPACT_NINF (-169)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      27,    33,    58,   -34,  -169,    48,    62,  -169,    84,  -169,
      60,  -169,    65,   262,  -169,  -169,    66,    80,    95,    48,
      99,    43,   109,  -169,  -169,  -169,  -169,    -3,   132,    80,
    -169,   119,    48,  -169,   -36,   119,   143,   158,    43,   183,
     164,   183,    43,   183,   171,   161,    10,  -169,  -169,  -169,
    -169,  -169,    29,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,   191,   109,  -169,  -169,   157,   263,  -169,    49,   208,
    -169,    71,   186,  -169,   210,    13,   200,   200,   200,   183,
     183,  -169,  -169,  -169,   185,  -169,   168,   125,    -9,   114,
    -169,  -169,   168,     2,   234,    18,   274,    22,   183,    43,
    -169,   183,   184,  -169,   183,    71,  -169,  -169,    11,    11,
    -169,  -169,  -169,  -169,   242,  -169,    48,    48,    48,     5,
    -169,    76,   239,  -169,   195,    48,   205,   206,   253,    26,
    -169,   203,   183,   183,  -169,   114,   114,   216,   299,   212,
     254,    43,   183,   183,   183,   183,   183,   183,   183,   200,
     200,   200,   200,   200,   200,   200,   200,    43,   164,   263,
    -169,    43,  -169,   124,  -169,   286,  -169,   299,  -169,   105,
     266,  -169,  -169,  -169,   222,   130,   151,    49,  -169,   211,
     275,  -169,  -169,   133,   263,  -169,   228,  -169,   264,    48,
     278,   104,   299,   183,  -169,  -169,  -169,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,   114,   114,   114,  -169,  -169,  -169,
    -169,  -169,  -169,   168,  -169,    21,  -169,   181,   251,   183,
    -169,   183,  -169,  -169,   224,   227,  -169,  -169,  -169,   287,
    -169,  -169,    47,    47,   289,   308,  -169,   182,   312,   183,
     183,   299,   263,  -169,   263,    43,    43,  -169,   299,  -169,
    -169,  -169,   277,  -169,  -169,    48,    71,   329,  -169,   154,
     166,  -169,  -169,  -169,  -169,   314,  -169,  -169,  -169,   331,
      43,    43,    71,  -169,  -169,  -169,  -169
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
      44,    35,     0,    94,     0,     0,     0,     0,     0,   140,
       0,   133,   134,   135,    90,   132,   131,     0,   111,   119,
     125,    90,   110,     0,     0,     0,     0,    78,     0,    78,
      63,     0,     0,    89,     0,    35,    25,    13,     0,     0,
      23,    24,    18,    22,     0,    19,     0,     0,     0,     0,
      56,     0,    49,    34,     0,     0,    37,     0,     0,     0,
      28,     0,     0,     0,   137,   120,   121,   139,    92,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,   101,
      93,    78,    67,     0,    83,    84,    64,    86,    88,     0,
       0,    20,    21,    17,    57,     0,     0,     0,    53,     0,
       0,     7,    29,     0,     0,    46,     0,    33,     0,    43,
       0,     0,   105,     0,   138,   136,   104,   112,   113,   114,
     115,   116,   117,   118,   124,   122,   123,   128,   129,   130,
     126,   127,   108,   109,   103,     0,    99,     0,    95,     0,
      81,     0,    87,    27,     0,     0,    55,    60,    47,     0,
      36,    38,     0,     0,     0,    39,    41,     0,    51,     0,
       0,    91,   101,    97,     0,    78,    78,    82,    85,    59,
      58,    50,     0,    32,    31,    43,    35,     0,     7,     0,
       0,    98,   102,   100,    96,     0,    40,    42,    48,     0,
      78,    78,    35,    52,   107,   106,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,  -169,  -168,  -169,  -169,  -169,  -169,   316,   -55,
     -66,  -169,   284,  -100,  -169,   -40,  -169,  -169,    82,  -169,
     315,  -169,  -169,  -169,   313,  -169,   172,    -4,  -169,    54,
     -92,   255,  -169,   -18,  -169,   131,  -169,   -21,   247,  -169,
    -169,  -169,  -169,  -169,   111,  -169,  -169,  -169,  -169,  -169,
    -169,   -35,   159,   -50,   -67,  -169
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,   127,
     115,    62,    63,   128,   129,   130,   234,   235,   236,    32,
      33,    24,   181,   258,    69,   119,   120,    34,    25,    46,
      47,    48,    49,    85,   163,   164,    51,    86,   137,    53,
      54,    55,    56,   215,   216,   217,    57,    58,    59,    60,
      93,   138,    88,    89,    90,   139
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,     8,    64,    50,    87,   170,    94,   166,    96,   134,
     177,   114,   157,   229,    10,    99,    71,    52,    99,    92,
      50,    52,     5,    99,    50,    36,   242,   135,   136,   133,
     149,     1,   186,   100,    37,    38,    39,   150,   151,    40,
      41,   160,   171,   172,   243,   140,    36,    65,   187,   196,
     188,    42,   158,   189,    43,    37,    38,    39,     4,   101,
      40,    41,   178,   165,   121,   212,   167,     9,   116,   218,
     110,   111,    42,   113,   117,    43,    52,   118,    52,    50,
     102,    50,   103,   104,    45,   207,   208,   209,   210,   211,
     269,   123,    75,   108,   109,     3,    95,   191,   192,   204,
     205,   206,    44,   125,   214,    45,   110,   111,   112,   126,
       7,     7,   174,   175,   176,   239,    15,   108,   109,   240,
      52,   183,    14,    50,   124,    26,    10,   125,   179,   231,
     110,   111,   112,   126,    10,   141,    52,   213,   142,    50,
      52,    11,    28,    50,   143,   144,   145,   146,   147,   148,
     152,   153,   154,   263,   264,   193,   267,    31,   241,   142,
     222,    35,   155,   156,   270,   143,   144,   145,   146,   147,
     148,    61,   276,   121,   219,    68,   271,    66,   274,   275,
      10,   220,   224,    10,   165,   237,   248,   214,   142,   262,
     230,   -90,   252,   253,   143,   144,   145,   146,   147,   148,
     142,    10,    73,   225,   259,   260,   143,   144,   145,   146,
     147,   148,   -90,   122,   -90,   -90,   107,    98,    76,   102,
      74,   103,   104,    97,    52,    52,    91,    50,    50,    77,
      78,   244,    10,   245,   256,    76,   105,    79,   131,    80,
     132,    98,    81,    82,    83,    84,   168,   173,   180,    52,
      52,   237,    50,    50,    79,   159,    80,   182,   185,    81,
      82,    83,    84,   -22,   184,   190,   193,   194,   142,    16,
      17,   223,    10,   227,   143,   144,   145,   146,   147,   148,
     228,    18,   232,   238,   246,   233,   249,    19,   142,   250,
      20,    21,   251,    22,   143,   144,   145,   146,   147,   148,
     161,   197,   198,   199,   200,   201,   202,   203,   142,   108,
     109,   195,   254,   255,   143,   144,   145,   146,   147,   148,
     142,   257,   110,   111,   112,   113,   143,   144,   145,   146,
     147,   148,   265,   142,   268,   272,   273,   266,   221,   143,
     144,   145,   146,   147,   148,    67,   106,    70,    72,   226,
     247,   169,   162,   261
};

static const yytype_int16 yycheck[] =
{
      21,     5,     5,    21,    39,   105,    41,    99,    43,    76,
       5,    66,    10,   181,    50,     5,    52,    38,     5,    40,
      38,    42,    56,     5,    42,     3,     5,    77,    78,    16,
      39,     4,     6,    23,    12,    13,    14,    46,    47,    17,
      18,    23,   108,   109,    23,    80,     3,    50,    22,   141,
      24,    29,    50,    27,    32,    12,    13,    14,     0,    30,
      17,    18,    57,    98,    68,   157,   101,     5,    19,   161,
      59,    60,    29,    62,    25,    32,    97,    28,    99,    97,
      51,    99,    53,    54,    62,   152,   153,   154,   155,   156,
     258,    20,    38,    46,    47,    62,    42,   132,   133,   149,
     150,   151,    59,    56,   159,    62,    59,    60,    61,    62,
      62,    62,   116,   117,   118,    11,    51,    46,    47,    15,
     141,   125,    62,   141,    53,    59,    50,    56,    52,   184,
      59,    60,    61,    62,    50,    10,   157,   158,    34,   157,
     161,    57,    62,   161,    40,    41,    42,    43,    44,    45,
      36,    37,    38,   245,   246,    50,   256,    62,   193,    34,
      55,    62,    48,    49,    10,    40,    41,    42,    43,    44,
      45,    62,   272,   177,    50,    56,    10,    45,   270,   271,
      50,    57,    52,    50,   219,   189,   221,   242,    34,   244,
      57,    30,   232,   233,    40,    41,    42,    43,    44,    45,
      34,    50,    59,    52,   239,   240,    40,    41,    42,    43,
      44,    45,    51,     5,    53,    54,    59,    56,    35,    51,
      62,    53,    54,    52,   245,   246,    62,   245,   246,    46,
      47,    50,    50,    52,    52,    35,    45,    54,    52,    56,
      30,    56,    59,    60,    61,    62,    62,     5,     9,   270,
     271,   255,   270,   271,    54,    21,    56,    62,     5,    59,
      60,    61,    62,    58,    58,    62,    50,    55,    34,     7,
       8,     5,    50,    62,    40,    41,    42,    43,    44,    45,
       5,    19,    54,     5,    33,    21,    62,    25,    34,    62,
      28,    29,     5,    31,    40,    41,    42,    43,    44,    45,
      26,   142,   143,   144,   145,   146,   147,   148,    34,    46,
      47,    57,    23,     5,    40,    41,    42,    43,    44,    45,
      34,     9,    59,    60,    61,    62,    40,    41,    42,    43,
      44,    45,    55,    34,     5,    21,     5,   255,    52,    40,
      41,    42,    43,    44,    45,    29,    62,    32,    35,   177,
     219,   104,    97,   242
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
      96,    99,   100,   102,   103,   104,   105,   109,   110,   111,
     112,    62,    74,    75,     5,    50,    45,    71,    56,    87,
      83,    52,    87,    59,    62,    92,    35,    46,    47,    54,
      56,    59,    60,    61,    62,    96,   100,   114,   115,   116,
     117,    62,   100,   113,   114,    92,   114,    52,    56,     5,
      23,    30,    51,    53,    54,    45,    75,    59,    46,    47,
      59,    60,    61,    62,    72,    73,    19,    25,    28,    88,
      89,    90,     5,    20,    53,    56,    62,    72,    76,    77,
      78,    52,    30,    16,   117,   116,   116,   101,   114,   118,
     114,    10,    34,    40,    41,    42,    43,    44,    45,    39,
      46,    47,    36,    37,    38,    48,    49,    10,    50,    21,
      23,    26,    94,    97,    98,   114,    93,   114,    62,   101,
      76,    73,    73,     5,    90,    90,    90,     5,    57,    52,
       9,    85,    62,    90,    58,     5,     6,    22,    24,    27,
      62,   114,   114,    50,    55,    57,    93,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   116,   117,   117,   117,
     117,   117,    93,   100,    72,   106,   107,   108,    93,    50,
      57,    52,    55,     5,    52,    52,    89,    62,     5,    66,
      57,    72,    54,    21,    79,    80,    81,    90,     5,    11,
      15,   114,     5,    23,    50,    52,    33,    98,   114,    62,
      62,     5,    78,    78,    23,     5,    52,     9,    86,   114,
     114,   107,    72,    93,    93,    55,    81,    76,     5,    66,
      10,    10,    21,     5,    93,    93,    76
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
     100,   101,   101,   102,   103,   104,   104,   105,   106,   106,
     107,   107,   108,   108,   109,   110,   111,   111,   112,   113,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   115,
     115,   115,   115,   115,   115,   116,   116,   116,   116,   116,
     116,   117,   117,   117,   117,   117,   117,   117,   117,   118,
     118
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
       1,     3,     1,     3,     2,     4,     6,     5,     3,     1,
       3,     0,     3,     1,     4,     4,     8,     8,     4,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     3,     2,     3,     1,
       0
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
#line 251 "pascal.y"
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
#line 1629 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 269 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1635 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 290 "pascal.y"
                                          { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 1641 "pascal.tab.c"
    break;

  case 18: /* constant: STRINGCONST  */
#line 292 "pascal.y"
                { (yyval.ival) = 0; }
#line 1647 "pascal.tab.c"
    break;

  case 19: /* constant: scalar_constant  */
#line 292 "pascal.y"
                                              { (yyval.ival) = (yyvsp[0].ival); }
#line 1653 "pascal.tab.c"
    break;

  case 20: /* constant: PLUS scalar_constant  */
#line 292 "pascal.y"
                                                                                  { (yyval.ival) = (yyvsp[0].ival); }
#line 1659 "pascal.tab.c"
    break;

  case 21: /* constant: MINUS scalar_constant  */
#line 292 "pascal.y"
                                                                                                                       { (yyval.ival) = -(yyvsp[0].ival); }
#line 1665 "pascal.tab.c"
    break;

  case 22: /* scalar_constant: IDENT  */
#line 293 "pascal.y"
                       { long long cv = 0; pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 1671 "pascal.tab.c"
    break;

  case 23: /* scalar_constant: INTCONST  */
#line 293 "pascal.y"
                                                                                         { (yyval.ival) = (yyvsp[0].ival); }
#line 1677 "pascal.tab.c"
    break;

  case 24: /* scalar_constant: REALCONST  */
#line 293 "pascal.y"
                                                                                                                  { (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 1683 "pascal.tab.c"
    break;

  case 27: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 298 "pascal.y"
                                     { if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str)); pas_pend_reset(); }
#line 1689 "pascal.tab.c"
    break;

  case 28: /* type: simple_type  */
#line 300 "pascal.y"
                { (yyval.ival) = (g_pas_pend_ptrtarget ? -3 : -1); }
#line 1695 "pascal.tab.c"
    break;

  case 29: /* type: ARROW IDENT  */
#line 301 "pascal.y"
                  { g_pas_pend_ptrtarget = strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 1701 "pascal.tab.c"
    break;

  case 30: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 302 "pascal.y"
                                                             { g_pas_pend_ptrtarget = NULL; (yyval.ival) = (yyvsp[-3].ival); }
#line 1707 "pascal.tab.c"
    break;

  case 31: /* type: packed_opt RECORDSY record_body ENDSY  */
#line 303 "pascal.y"
                                            { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1713 "pascal.tab.c"
    break;

  case 32: /* type: packed_opt SETSY OFSY simple_type  */
#line 304 "pascal.y"
                                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -2; }
#line 1719 "pascal.tab.c"
    break;

  case 33: /* type: packed_opt FILESY  */
#line 305 "pascal.y"
                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 1725 "pascal.tab.c"
    break;

  case 36: /* simple_type: LPARENT id_list RPARENT  */
#line 309 "pascal.y"
                            { (yyval.ival) = -1; }
#line 1731 "pascal.tab.c"
    break;

  case 37: /* simple_type: IDENT  */
#line 310 "pascal.y"
            { g_pas_pend_typename = strdup((yyvsp[0].str)); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); (yyval.ival) = -3; } else { pas_rectype_to_pend((yyvsp[0].str)); (yyval.ival) = -1; } }
#line 1737 "pascal.tab.c"
    break;

  case 38: /* simple_type: constant DOTDOT constant  */
#line 311 "pascal.y"
                               { (yyval.ival) = (yyvsp[0].ival); }
#line 1743 "pascal.tab.c"
    break;

  case 42: /* record_field: id_list COLON type  */
#line 321 "pascal.y"
                       { if ((yyvsp[-2].list)) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_pend_add((yyvsp[-2].list)->items[i]->v.sval); }
#line 1749 "pascal.tab.c"
    break;

  case 46: /* var_decl: id_list COLON type SEMICOLON  */
#line 328 "pascal.y"
                                       { if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if ((yyvsp[-1].ival) == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ((yyvsp[-1].ival) >= 0) pas_array_add(id->v.sval, (yyvsp[-1].ival)); if ((yyvsp[-1].ival) == -2) pas_setvar_add(id->v.sval); if (g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } } pas_local_add(id->v.sval); } } pas_pend_reset(); }
#line 1755 "pascal.tab.c"
    break;

  case 47: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 330 "pascal.y"
                                                                       { }
#line 1761 "pascal.tab.c"
    break;

  case 48: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 331 "pascal.y"
                                                                                    { pas_func_add((yyvsp[-6].str)); }
#line 1767 "pascal.tab.c"
    break;

  case 49: /* $@1: %empty  */
#line 332 "pascal.y"
                                                     { pas_proc_enter(); }
#line 1773 "pascal.tab.c"
    break;

  case 50: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON $@1 block SEMICOLON  */
#line 333 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), (yyvsp[-4].list), (yyvsp[-1].node), 0, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1781 "pascal.tab.c"
    break;

  case 51: /* $@2: %empty  */
#line 336 "pascal.y"
                                                                { pas_func_add((yyvsp[-4].str)); pas_proc_enter(); }
#line 1787 "pascal.tab.c"
    break;

  case 52: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON $@2 block SEMICOLON  */
#line 337 "pascal.y"
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-7].str), (yyvsp[-6].list), (yyvsp[-1].node), 1, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
#line 1795 "pascal.tab.c"
    break;

  case 53: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 342 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 1801 "pascal.tab.c"
    break;

  case 54: /* parameter_list_opt: %empty  */
#line 343 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 1807 "pascal.tab.c"
    break;

  case 55: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 346 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1813 "pascal.tab.c"
    break;

  case 56: /* parameter_decl_list: parameter_decl  */
#line 347 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 1819 "pascal.tab.c"
    break;

  case 57: /* parameter_decl: PROCEDURESY id_list  */
#line 350 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 1825 "pascal.tab.c"
    break;

  case 58: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 351 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 1831 "pascal.tab.c"
    break;

  case 59: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 352 "pascal.y"
                                { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 1837 "pascal.tab.c"
    break;

  case 60: /* parameter_decl: id_list COLON IDENT  */
#line 353 "pascal.y"
                          { const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); (yyval.list) = (yyvsp[-2].list); }
#line 1843 "pascal.tab.c"
    break;

  case 61: /* id_list: id_list COMMA IDENT  */
#line 356 "pascal.y"
                        { pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 1849 "pascal.tab.c"
    break;

  case 62: /* id_list: IDENT  */
#line 357 "pascal.y"
            { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 1855 "pascal.tab.c"
    break;

  case 63: /* body: BEGINSY statement_list ENDSY  */
#line 360 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 1861 "pascal.tab.c"
    break;

  case 64: /* statement_list: statement_list SEMICOLON statement  */
#line 363 "pascal.y"
                                       { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1867 "pascal.tab.c"
    break;

  case 65: /* statement_list: statement  */
#line 364 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 1873 "pascal.tab.c"
    break;

  case 66: /* statement: statement_no_label  */
#line 367 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1879 "pascal.tab.c"
    break;

  case 67: /* statement: INTCONST COLON statement_no_label  */
#line 368 "pascal.y"
                                        { (yyval.node) = (yyvsp[0].node); }
#line 1885 "pascal.tab.c"
    break;

  case 68: /* statement_no_label: assignment  */
#line 371 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1891 "pascal.tab.c"
    break;

  case 69: /* statement_no_label: call  */
#line 372 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1897 "pascal.tab.c"
    break;

  case 70: /* statement_no_label: compound_statement  */
#line 373 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1903 "pascal.tab.c"
    break;

  case 71: /* statement_no_label: goto_statement  */
#line 374 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1909 "pascal.tab.c"
    break;

  case 72: /* statement_no_label: if_statement  */
#line 375 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1915 "pascal.tab.c"
    break;

  case 73: /* statement_no_label: case_statement  */
#line 376 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1921 "pascal.tab.c"
    break;

  case 74: /* statement_no_label: while_statement  */
#line 377 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1927 "pascal.tab.c"
    break;

  case 75: /* statement_no_label: repeat_statement  */
#line 378 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1933 "pascal.tab.c"
    break;

  case 76: /* statement_no_label: for_statement  */
#line 379 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1939 "pascal.tab.c"
    break;

  case 77: /* statement_no_label: with_statement  */
#line 380 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1945 "pascal.tab.c"
    break;

  case 78: /* statement_no_label: %empty  */
#line 381 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 1951 "pascal.tab.c"
    break;

  case 79: /* call: IDENT  */
#line 384 "pascal.y"
          { (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 1957 "pascal.tab.c"
    break;

  case 80: /* call: call_with_args  */
#line 385 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1963 "pascal.tab.c"
    break;

  case 81: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 388 "pascal.y"
                                        { (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 1969 "pascal.tab.c"
    break;

  case 82: /* argument_list: argument_list COMMA argument  */
#line 391 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1975 "pascal.tab.c"
    break;

  case 83: /* argument_list: argument  */
#line 392 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 1981 "pascal.tab.c"
    break;

  case 84: /* argument: expression  */
#line 395 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 1987 "pascal.tab.c"
    break;

  case 85: /* argument: expression COLON expression  */
#line 396 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 1993 "pascal.tab.c"
    break;

  case 86: /* assignment: selector BECOMES expression  */
#line 400 "pascal.y"
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
#line 2009 "pascal.tab.c"
    break;

  case 87: /* selector: selector LBRACK expression_list RBRACK  */
#line 413 "pascal.y"
                                           { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); (yyval.node) = e; }
#line 2015 "pascal.tab.c"
    break;

  case 88: /* selector: selector PERIOD IDENT  */
#line 414 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_fi)); (yyval.node) = e; } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } }
#line 2022 "pascal.tab.c"
    break;

  case 89: /* selector: selector ARROW  */
#line 416 "pascal.y"
                     { (yyval.node) = mk_deref((yyvsp[-1].node)); }
#line 2028 "pascal.tab.c"
    break;

  case 90: /* selector: IDENT  */
#line 417 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 2034 "pascal.tab.c"
    break;

  case 91: /* expression_list: expression_list COMMA expression  */
#line 420 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2040 "pascal.tab.c"
    break;

  case 92: /* expression_list: expression  */
#line 421 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2046 "pascal.tab.c"
    break;

  case 93: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 424 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 2052 "pascal.tab.c"
    break;

  case 94: /* goto_statement: GOTOSY INTCONST  */
#line 427 "pascal.y"
                    { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 2058 "pascal.tab.c"
    break;

  case 95: /* if_statement: IFSY expression THENSY statement  */
#line 430 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2064 "pascal.tab.c"
    break;

  case 96: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 431 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond((yyvsp[-4].node))); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2070 "pascal.tab.c"
    break;

  case 97: /* case_statement: CASESY expression OFSY case_list ENDSY  */
#line 434 "pascal.y"
                                           { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 2076 "pascal.tab.c"
    break;

  case 104: /* while_statement: WHILESY expression DOSY statement  */
#line 449 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2082 "pascal.tab.c"
    break;

  case 105: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 452 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond((yyvsp[0].node))); }
#line 2088 "pascal.tab.c"
    break;

  case 106: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 456 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2094 "pascal.tab.c"
    break;

  case 107: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 458 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); e->v.ival = 1; (yyval.node) = e; }
#line 2100 "pascal.tab.c"
    break;

  case 108: /* with_statement: WITHSY selector_list DOSY statement  */
#line 461 "pascal.y"
                                        { (yyval.node) = (yyvsp[0].node); }
#line 2106 "pascal.tab.c"
    break;

  case 111: /* expression: simple_expression  */
#line 468 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2112 "pascal.tab.c"
    break;

  case 112: /* expression: expression INOP simple_expression  */
#line 469 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2118 "pascal.tab.c"
    break;

  case 113: /* expression: expression LTOP simple_expression  */
#line 470 "pascal.y"
                                        { (yyval.node) = bin(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2124 "pascal.tab.c"
    break;

  case 114: /* expression: expression LEOP simple_expression  */
#line 471 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2130 "pascal.tab.c"
    break;

  case 115: /* expression: expression GTOP simple_expression  */
#line 472 "pascal.y"
                                        { (yyval.node) = bin(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2136 "pascal.tab.c"
    break;

  case 116: /* expression: expression GEOP simple_expression  */
#line 473 "pascal.y"
                                        { (yyval.node) = pas_arith_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2142 "pascal.tab.c"
    break;

  case 117: /* expression: expression NEOP simple_expression  */
#line 474 "pascal.y"
                                        { (yyval.node) = bin(TT_NE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2148 "pascal.tab.c"
    break;

  case 118: /* expression: expression EQOP simple_expression  */
#line 475 "pascal.y"
                                        { (yyval.node) = bin(TT_EQ, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2154 "pascal.tab.c"
    break;

  case 119: /* simple_expression: term  */
#line 478 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2160 "pascal.tab.c"
    break;

  case 120: /* simple_expression: PLUS term  */
#line 479 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2166 "pascal.tab.c"
    break;

  case 121: /* simple_expression: MINUS term  */
#line 480 "pascal.y"
                 { (yyval.node) = un(TT_MNS, (yyvsp[0].node)); }
#line 2172 "pascal.tab.c"
    break;

  case 122: /* simple_expression: simple_expression PLUS term  */
#line 481 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2178 "pascal.tab.c"
    break;

  case 123: /* simple_expression: simple_expression MINUS term  */
#line 482 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2184 "pascal.tab.c"
    break;

  case 124: /* simple_expression: simple_expression OROP term  */
#line 483 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2190 "pascal.tab.c"
    break;

  case 125: /* term: factor  */
#line 486 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2196 "pascal.tab.c"
    break;

  case 126: /* term: term MUL factor  */
#line 487 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2202 "pascal.tab.c"
    break;

  case 127: /* term: term RDIV factor  */
#line 488 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2208 "pascal.tab.c"
    break;

  case 128: /* term: term IDIV factor  */
#line 489 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2214 "pascal.tab.c"
    break;

  case 129: /* term: term IMOD factor  */
#line 490 "pascal.y"
                       { (yyval.node) = bin(TT_MOD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2220 "pascal.tab.c"
    break;

  case 130: /* term: term ANDOP factor  */
#line 491 "pascal.y"
                        { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2226 "pascal.tab.c"
    break;

  case 131: /* factor: selector  */
#line 494 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2232 "pascal.tab.c"
    break;

  case 132: /* factor: call_with_args  */
#line 495 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2238 "pascal.tab.c"
    break;

  case 133: /* factor: INTCONST  */
#line 496 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 2244 "pascal.tab.c"
    break;

  case 134: /* factor: REALCONST  */
#line 497 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 2250 "pascal.tab.c"
    break;

  case 135: /* factor: STRINGCONST  */
#line 498 "pascal.y"
                  { (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 2256 "pascal.tab.c"
    break;

  case 136: /* factor: LPARENT expression RPARENT  */
#line 499 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2262 "pascal.tab.c"
    break;

  case 137: /* factor: NOTSY factor  */
#line 500 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 2268 "pascal.tab.c"
    break;

  case 138: /* factor: LBRACK expression_list_opt RBRACK  */
#line 501 "pascal.y"
                                        { (yyval.node) = mk_set_ctor((yyvsp[-1].list)); }
#line 2274 "pascal.tab.c"
    break;

  case 139: /* expression_list_opt: expression_list  */
#line 504 "pascal.y"
                    { (yyval.list) = (yyvsp[0].list); }
#line 2280 "pascal.tab.c"
    break;

  case 140: /* expression_list_opt: %empty  */
#line 505 "pascal.y"
      { (yyval.list) = NULL; }
#line 2286 "pascal.tab.c"
    break;


#line 2290 "pascal.tab.c"

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

#line 507 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL;
    g_pas_level = 1; g_pas_ldepth = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
