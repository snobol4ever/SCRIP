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

#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
#define PAS_STMT_STACK 512
static int pas_stmt_line_stk[PAS_STMT_STACK];
static int pas_stmt_sp = 0;
static void pas_stmt_mark(void) { if (pas_stmt_sp < PAS_STMT_STACK) pas_stmt_line_stk[pas_stmt_sp] = -1; pas_stmt_sp++; }
static void pas_stmt_line_fill(void) { if (pas_stmt_sp > 0 && pas_stmt_sp <= PAS_STMT_STACK && pas_stmt_line_stk[pas_stmt_sp - 1] < 0) pas_stmt_line_stk[pas_stmt_sp - 1] = pascal_get_lineno(); }
static int  pas_stmt_line_pop(void) { int l = -1; if (pas_stmt_sp > 0) { pas_stmt_sp--; if (pas_stmt_sp < PAS_STMT_STACK) l = pas_stmt_line_stk[pas_stmt_sp]; } return l >= 0 ? l : pascal_get_lineno(); }
int pascal_lex_wrapped(void) { int t = pascal_yylex(); pas_stmt_line_fill(); return t; }
#define pascal_yylex pascal_lex_wrapped
void pascal_yyerror(const char *msg) { fprintf(stderr, "pascal parse error line %d: %s\n", pascal_get_lineno(), msg); }
tree_t   *pascal_prog_result = NULL;
static PNodeList g_pascal_procs;
static int pas_trace_enabled(void) { extern long g_trace_budget; return g_trace_budget != 0; }
static PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)ct_zalloc(1, sizeof *l); return l; }
static void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)ct_grow(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
static PNodeList *pnl_concat(PNodeList *a, PNodeList *b) {
    if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a;
}
typedef struct { const char *name; const char *sig; const char *owner; int depth, rid, formal, uid; } PasDef;
typedef struct { const char *name; PNodeList *params; const char *sig; } PasFwd;
struct pas_pend_frame;
static struct { PasDef *defs; int n, cap; int *marks; int depth, mcap; PasFwd *fwd; int nfwd, fcap;
                PasDef *pend; int npend, cpend; PasDef *fsig; int nfsig, cfsig; PasDef *cand; int ncand, ccand; int nrid, npf, nuid; PasDef *lab; int nlab, clab; PasDef *apl; int napl, capl;
                struct pas_pend_frame *pnest; int npnest, cpnest; int anonseq; } g_pas_scope;
static PasDef *pas_scope_push(PasDef **a, int *n, int *cap, const char *name) {
    if (*n >= *cap) { *cap = *cap ? *cap * 2 : 256; *a = (PasDef *)ct_grow(*a, (size_t)*cap * sizeof(PasDef)); }
    PasDef *d = &(*a)[(*n)++]; d->name = name; d->sig = NULL; d->owner = NULL; d->depth = g_pas_scope.depth; d->rid = 0; d->formal = 0; d->uid = ++g_pas_scope.nuid; return d;
}
static void pas_scope_defined_after_use(const char *name);
static void pas_scope_define(const char *name) { if (name) { pas_scope_defined_after_use(name); pas_scope_push(&g_pas_scope.defs, &g_pas_scope.n, &g_pas_scope.cap, name); } }
static int pas_scope_uid(const char *name) { for (int i = g_pas_scope.n - 1; name && i >= 0; i--) if (g_pas_scope.defs[i].name && !strcmp(g_pas_scope.defs[i].name, name)) return g_pas_scope.defs[i].uid; return 0; }
static void pas_scope_define_list(PNodeList *ids) { for (int i = 0; ids && i < ids->count; i++) if (ids->items[i] && ids->items[i]->v.sval) pas_scope_define(ids->items[i]->v.sval); }
static void pas_scope_fwd_save(const char *name, PNodeList *params, const char *sig) {
    if (!name) return;
    if (g_pas_scope.nfwd >= g_pas_scope.fcap) { g_pas_scope.fcap = g_pas_scope.fcap ? g_pas_scope.fcap * 2 : 32; g_pas_scope.fwd = (PasFwd *)ct_grow(g_pas_scope.fwd, (size_t)g_pas_scope.fcap * sizeof(PasFwd)); }
    g_pas_scope.fwd[g_pas_scope.nfwd].name = name; g_pas_scope.fwd[g_pas_scope.nfwd].sig = sig; g_pas_scope.fwd[g_pas_scope.nfwd++].params = params;
}
static const PasFwd *pas_scope_fwd_of(const char *name, PNodeList *params) {
    if ((!params || params->count == 0) && name) for (int i = g_pas_scope.nfwd - 1; i >= 0; i--) if (g_pas_scope.fwd[i].name && !strcmp(g_pas_scope.fwd[i].name, name)) return &g_pas_scope.fwd[i];
    return NULL;
}
static PNodeList *pas_scope_fwd_params(const char *name, PNodeList *params) { const PasFwd *f = pas_scope_fwd_of(name, params); return f ? f->params : params; }
static void pas_formal_subranges(const char *sig, PNodeList *params);
static void pas_formal_chararrs(const char *sig, PNodeList *params);
static void pas_formal_strtypes(const char *sig, PNodeList *params);
static void pas_scope_enter(const char *name, PNodeList *params) {
    if (g_pas_scope.depth >= g_pas_scope.mcap) { g_pas_scope.mcap = g_pas_scope.mcap ? g_pas_scope.mcap * 2 : 16; g_pas_scope.marks = (int *)ct_grow(g_pas_scope.marks, (size_t)g_pas_scope.mcap * sizeof(int)); }
    g_pas_scope.marks[g_pas_scope.depth++] = g_pas_scope.n;
    const PasFwd *fwd = pas_scope_fwd_of(name, params); if (fwd) params = fwd->params;
    int first = g_pas_scope.n; pas_scope_define_list(params);
    for (int i = first; name && i < g_pas_scope.n; i++) for (int j = g_pas_scope.nfsig - 1; j >= 0; j--)
        if (!strcmp(g_pas_scope.fsig[j].owner, name) && !strcmp(g_pas_scope.fsig[j].name, g_pas_scope.defs[i].name)) {
            g_pas_scope.defs[i].sig = g_pas_scope.fsig[j].sig; g_pas_scope.defs[i].formal = 1; break; }
    if (first > 0 && g_pas_scope.defs[first - 1].rid && name && !strcmp(g_pas_scope.defs[first - 1].name, name)) {
        const char *sig = (fwd && fwd->sig) ? fwd->sig : g_pas_scope.defs[first - 1].sig; pas_formal_subranges(sig, params); pas_formal_chararrs(sig, params); pas_formal_strtypes(sig, params); }
}
static int pas_pf_is_formal(const char *name);
static const PasDef *pas_pf_lookup(const char *name);
static tree_t *pas_pf_callthrough(const char *name, PNodeList *args);
static const char *pas_pf_rtype(const tree_t *e) {
    if (!e || e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall") || !e->c[2]->v.sval || e->c[2]->v.sval[0] != 'F') return NULL;
    const char *r = strrchr(e->c[2]->v.sval, ')'); return (r && r[1] == ':') ? r + 2 : NULL;
}
static void pas_scope_exit(void) {
    if (g_pas_scope.depth > 0) g_pas_scope.n = g_pas_scope.marks[--g_pas_scope.depth];
    while (g_pas_scope.napl > 0 && g_pas_scope.apl[g_pas_scope.napl - 1].depth > g_pas_scope.depth) g_pas_scope.napl--;
}
static int pas_is_int_typename(const char *t);
static int pas_is_realtypename(const char *t);
static int pas_is_settype(const char *name);
static int pas_scope_has(const char *name) {
    const char *req[] = { "true", "false", "maxint", "input", "output", "nil", "char", "boolean", "text", "string", "widechar" };
    if (!name) return 1;
    if (pas_is_int_typename(name) || pas_is_realtypename(name)) return 1;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return 1;
    for (int i = g_pas_scope.n - 1; i >= 0; i--) if (g_pas_scope.defs[i].name && !strcmp(g_pas_scope.defs[i].name, name)) return 1;
    return 0;
}
static void pas_scope_require(const char *name);
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
static const char *pas_elem_rectype(tree_t *e);
static const char *pas_with_sel_rtype(tree_t *sel);
static const char *pas_recvar_field_rectype(const char *vn, long idx);
static int pas_rectype_nf(const char *rn);
static int pas_array_high_get(const char *name, long long *out);
static int pas_is_aggregate_designator(tree_t *e);
static tree_t *mk_rec_init_type(const char *rt);
static tree_t *pas_new_target_init(const char *rt);
static tree_t *mk_rec_init_type_d(const char *rt, int depth);
static long long pas_array_low(const char *name);
static long long pas_arrtype_high(const char *name);
static long long pas_arrtype_lo(const char *name);
static long long pas_index_bound(long long v, int want_hi);
static long long pas_array_decl_low(void);
static long long pas_array_nonparam_low(const char *name);
static int pas_subvar_get(const char *n, long long *lo, long long *hi);
static int pas_sizeof_lookup(const char *name, long long *out);
static int pas_sizeof_builtin_size(const char *n, long long *out);
static int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi);
static int pas_var_string_kind(const char *name);
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi);
static int pas_tfcomp_nonchar(const char *n);
static int pas_is_hdrfile(const char *n);
static void pas_assigned_add(const char *n);
static int pas_assigned_get(const char *n);
static int pas_array_is_param(const char *name);
static const char *pas_selector_base_name(tree_t *e);
extern int g_pas_iso_errors;
extern int g_pas_seen_mode_directive;
extern int g_pas_mode_iso;
extern int pascal_seen_decl_start;
extern int g_pas_min_enum_size;
extern int g_pas_pack_set_size;
extern int g_pas_range_check_on;
extern int g_pas_align_mac68k;
extern int g_pas_pack_records;
extern int g_pas_zerobased_strings;
extern int g_pas_codepage;
static int pas_rectype_total_size(const char *rn, long long *out);
static int pas_rectype_field_offset(const char *rn, int idx, long long *out);
static const char *pas_typealias_get(const char *n);
static long long g_pas_pend_sub_low;
static long long g_pas_pend_esz = -1;
static struct { char *name; long long bytes; } *g_pas_tsz; static int g_pas_ntsz, g_pas_tszcap;
static void pas_tsz_add(const char *name, long long bytes) { if (!name || bytes < 0) return; if (g_pas_ntsz >= g_pas_tszcap) { g_pas_tszcap = g_pas_tszcap ? g_pas_tszcap * 2 : 64; g_pas_tsz = (__typeof__(g_pas_tsz))ct_grow(g_pas_tsz, (size_t)g_pas_tszcap * sizeof *g_pas_tsz); } g_pas_tsz[g_pas_ntsz].name = ct_strdup(name); g_pas_tsz[g_pas_ntsz].bytes = bytes; g_pas_ntsz++; }
static int pas_tsz_get(const char *name, long long *out) { if (!name) return 0; for (int i = g_pas_ntsz - 1; i >= 0; i--) if (!strcmp(g_pas_tsz[i].name, name)) { *out = g_pas_tsz[i].bytes; return 1; } return 0; }
static int pas_array_is_zero_size(const char *name) { long long b; return pas_tsz_get(name, &b) && b == 0; }
#ifndef PAS_FIELD_MAX
#define PAS_FIELD_MAX 128
#endif
struct pas_vt { int state; char *name; tree_t *node; tree_t *tagexpr; long long mask; int tagslot; int tagfi; long long fmask[PAS_FIELD_MAX]; unsigned char fhas[PAS_FIELD_MAX]; };
static struct pas_vt **g_pas_vt; static int g_pas_nvt, g_pas_vtcap;
static struct pas_vt *pas_vt_new(int state) { if (g_pas_nvt >= g_pas_vtcap) { g_pas_vtcap = g_pas_vtcap ? g_pas_vtcap * 2 : 32; g_pas_vt = (struct pas_vt **)ct_grow(g_pas_vt, (size_t)g_pas_vtcap * sizeof *g_pas_vt); } struct pas_vt *v = (struct pas_vt *)ct_zalloc(1, sizeof *v); v->state = state; v->tagslot = -1; v->tagfi = -1; g_pas_vt[g_pas_nvt++] = v; return v; }
static void pas_vt_open(void) { pas_vt_new(0); }
static struct pas_vt *pas_vt_top_open(void) { for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 0) return g_pas_vt[i]; return NULL; }
static int pas_vt_consts(tree_t *e, long long *v, int *n, int cap) { if (!e || *n >= cap) return 0; if (e->t == TT_ADD && e->n == 2) return pas_vt_consts(e->c[0], v, n, cap) && pas_vt_consts(e->c[1], v, n, cap); if (e->t == TT_EQ && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT) { v[(*n)++] = e->c[1]->v.ival; return 1; } return 0; }
static void pas_vt_arm(int lo, int hi, tree_t *consts) { struct pas_vt *t = pas_vt_top_open(); if (!t) return; long long v[64]; int n = 0; long long m = 0; int ok = pas_vt_consts(consts, v, &n, 64) && n > 0; for (int i = 0; ok && i < n; i++) { if (v[i] < 0 || v[i] > 62) ok = 0; else m |= 1LL << v[i]; } for (int f = lo; ok && f < hi && f < PAS_FIELD_MAX; f++) { t->fmask[f] = m; t->fhas[f] = 1; } }
static void pas_vt_close(int tagfi) { struct pas_vt *t = pas_vt_top_open(); if (!t) return; t->tagfi = tagfi; t->state = 1; }
static void pas_vt_bind(const char *name) { struct pas_vt *best = NULL; for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 1) { if (g_pas_vt[i]->tagfi >= 0 && (!best || g_pas_vt[i]->tagfi > best->tagfi)) best = g_pas_vt[i]; } for (int i = 0; i < g_pas_nvt; i++) if (g_pas_vt[i]->state == 1) g_pas_vt[i]->state = 3; if (best && name) { best->state = 2; best->name = ct_strdup(name); } }
static struct pas_vt *pas_vt_find_name(const char *nm);
static int pas_rectype_slot(const char *rn, int fi);
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static int pas_var_is_real(const char *name);
static int pas_is_setvar(const char *name);
static int pas_is_singlevar(const char *name);
static int pas_is_currencyexpr(tree_t *e);
static int pas_is_qword(tree_t *e);
static int pas_is_pcharvar(const char *name);
static char *pas_pchar_off_name(const char *n);
static int pas_pchar_castname(tree_t *e);
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b);
static int g_pas_pend_isbool;
static int g_pas_pend_istfile;
static void pas_tfilevar_add(const char *name);
static int pas_is_tfilevar(const char *name);
static void pas_tfiletype_add(const char *name);
static int pas_is_tfiletype(const char *name);
static int pas_rectype_has_file(const char *rn);
static int pas_is_tfile_node(const tree_t *e);
static int pas_is_booltype(const char *name);
static void pas_boolvar_add(const char *name);
static int pas_is_boolvar(const char *name);
static int pas_is_boolexpr(tree_t *e);
static int pas_is_filevar(const char *name);
static int pas_is_filevar_elem(tree_t *e);
static int pas_is_stdstream(const char *name);
static int pas_is_hdrfile(const char *name);
static int pas_proc_param_is_var(const char *name, int idx);
static void pas_filevar_add(const char *name);
static int pas_is_chararr(const char *name);
static long long pas_chararr_lo(const char *name);
static int pas_is_strarr(const char *name);
static long long pas_strarr_lo(const char *name);
static tree_t *pas_alpha_wrap(tree_t *x);
static int pas_ca_is_read(const tree_t *e);
static tree_t *pas_trace_wrap_value(tree_t *val);
static const char *pas_rectype_field_enum_by_index(const char *rn, long idx);
static const char *pas_enumarr_get(const char *a);
static tree_t *pas_tree_clone(tree_t *e);
static const char *pas_scalarvartype_get(const char *vn);
static const char *pas_typealias_get(const char *n);
static int pas_var_is_boolfam(const char *name);
static const char *pas_curfunc_name(void);
static int pas_enumnames_idx(const char *tn);
static const char *pas_trace_enum_names_of_var(const char *vn) {
    const char *t = vn ? pas_scalarvartype_get(vn) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        int ei = pas_enumnames_idx(t);
        if (ei >= 0) return pas_enumnames_by_idx(ei);
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return NULL;
}
static const char *pas_trace_store_name(const tree_t *lhs) {
    if (!lhs) return NULL;
    if (lhs->t == TT_VAR && lhs->v.sval) return lhs->v.sval;
    const char *cf = pas_curfunc_name();
    if (cf && lhs->t == TT_FNC && lhs->n >= 1 && lhs->c[0] && lhs->c[0]->t == TT_VAR && lhs->c[0]->v.sval && !strcmp(lhs->c[0]->v.sval, cf)) return cf;
    return NULL;
}
static tree_t *pas_trace_wrap_proc(const char *pname, PNodeList *params, tree_t *body, int isfunc) {
    if (!pas_trace_enabled() || !body) return body;
    tree_t *enter_call = ast_node_new(TT_FNC);
    ast_push(enter_call, leaf_s(TT_VAR, "__trace_call"));
    ast_push(enter_call, leaf_s(TT_QLIT, pname));
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *id = params->items[i];
        if (id && id->v.sval) {
            ast_push(enter_call, leaf_s(TT_QLIT, id->v.sval));
            ast_push(enter_call, pas_trace_wrap_value(leaf_s(TT_VAR, id->v.sval)));
        }
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
static tree_t *pas_trace_wrap_value(tree_t *val) {
    if (!val) return val;
    const char *_enm = (val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
    if (!_enm && val->t == TT_VAR && val->v.sval) _enm = pas_trace_enum_names_of_var(val->v.sval);
    if (!_enm && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[1]) {
        const char *_et = NULL, *_rt = (val->c[1]->t == TT_ILIT) ? pas_with_sel_rtype(val->c[0]) : NULL;
        if (_rt) _et = pas_rectype_field_enum_by_index(_rt, (long)val->c[1]->v.ival);
        if (!_et && val->c[0]->t == TT_VAR && val->c[0]->v.sval) _et = pas_enumarr_get(val->c[0]->v.sval);
        if (_et) _enm = pas_enumnames_by_idx(pas_enumnames_idx(_et)); }
    if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); return _w; }
    if (pas_is_charexpr(val)) return mk_chr_wrap(val);
    if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, bin(TT_NE, val, ilit(0))); ast_push(_w, leaf_s(TT_QLIT, "false,true")); return _w; }
    if (val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) return pas_alpha_wrap(val);
    if (pas_ca_is_read(val)) return pas_alpha_wrap(val);
    if (val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(_w, val); ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval))); return _w; }
    return val;
}
static tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b);
static tree_t *pas_trace_lhs(tree_t *lhs) { return (pas_trace_enabled() && lhs && lhs->t != TT_VAR) ? pas_tree_clone(lhs) : lhs; }
static tree_t *pas_trace_assigned(tree_t *lhs, tree_t *asn) {
    if (!pas_trace_enabled() || !lhs) return asn;
    const char *nm = pas_trace_store_name(lhs); tree_t *tv;
    if (nm) tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, nm), pas_trace_wrap_value(leaf_s(TT_VAR, nm)));
    else tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, "<lval>"), pas_trace_wrap_value(lhs));
    PNodeList *sl = pnl_new(); pnl_push(sl, asn); pnl_push(sl, tv); return seq_of(sl);
}
static tree_t *pas_trace_wrap_for_body(const char *var, tree_t *body) {
    if (!pas_trace_enabled() || !var) return body;
    PNodeList *l = pnl_new();
    pnl_push(l, mk_fnc2("__trace_value", leaf_s(TT_QLIT, var), pas_trace_wrap_value(leaf_s(TT_VAR, var))));
    if (body) pnl_push(l, body);
    return seq_of(l);
}
static tree_t *pas_trace_prepend_tap_off(tree_t *body) {
    if (!pas_trace_enabled()) return body;
    tree_t *off = ast_node_new(TT_FNC); ast_push(off, leaf_s(TT_VAR, "__trace_tap_off"));
    tree_t *nb = ast_node_new(TT_PROGRAM);
    ast_push(nb, off);
    if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(nb, body->c[i]); }
    else if (body) ast_push(nb, body);
    return nb;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high);
static unsigned long long pas_caparm_mask(const char *name);
static long long pas_caparm_lo(const char *name, int pos);
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo);
static int pas_is_rel(tree_t *e);
static int pas_is_proc(const char *name);
static int pas_is_func(const char *name);
static tree_t *pas_addr_of_proc(tree_t *e);
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause);
static int pas_proc_param_is_real(const char *name, int idx);
static int pas_proc_param_is_string(const char *name, int idx);
static tree_t *pas_bool(tree_t *e);
static tree_t *pas_tree_clone(tree_t *e);
static tree_t *mk_deref(tree_t *ptr) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr);
    return e;
}
static tree_t *mk_fnc0(const char *fn) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); return e; }
static tree_t *mk_fnc1(const char *fn, tree_t *a) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); return e; }
static tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *mk_fnc3(const char *fn, tree_t *a, tree_t *b, tree_t *c) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); ast_push(e, c); return e; }
static int pas_ord_var_bounds(const char *name, long long *lo, long long *hi);
static tree_t *pas_ord_check(tree_t *v, long long lo, long long hi, const char *what) {
    tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_ord_check"));
    ast_push(e, v); ast_push(e, ilit(lo)); ast_push(e, ilit(hi)); ast_push(e, leaf_s(TT_QLIT, what));
    return e;
}
static int pas_is_unsigned_inttypename(const char *n) { if (!n) return 0; static const char *U[] = {"byte","word","cardinal","longword","qword","uint8","uint16","uint32","uint64","nativeuint","pointer","ptruint","codepointer"}; for (size_t i = 0; i < sizeof(U) / sizeof(U[0]); i++) if (!strcmp(n, U[i])) return 1; return 0; }
static tree_t *mk_call(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "ord") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) return a->c[1];
        if (a && a->t == TT_IDX) a->v.ival = 0;
        if (pas_is_charexpr(a) || pas_is_boolexpr(a)) return bin(TT_ADD, a, ilit(0));
        return a;
    }
    if (name && (!strcmp(name, "pack") || !strcmp(name, "unpack")) && args && args->count >= 5) {
        int ispack = !strcmp(name, "pack");
        tree_t *a = args->items[ispack ? 0 : 2]; tree_t *i = args->items[ispack ? 2 : 4]; tree_t *z = args->items[ispack ? 4 : 0];
        long long zhi = -1; long long zlo = 1;
        if (z && z->t == TT_VAR && z->v.sval) { if (!pas_array_high_get(z->v.sval, &zhi)) zhi = -1; if (pas_is_chararr(z->v.sval)) { long long _cl = pas_chararr_lo(z->v.sval); if (_cl > 0) zlo = _cl; } else { zlo = pas_array_low(z->v.sval); } }
        if (a && a->t == TT_VAR && a->v.sval && z && z->t == TT_VAR && z->v.sval && zhi >= 0) {
            long long ahi = -1; int _ahok = pas_array_high_get(a->v.sval, &ahi);
            long long alo = pas_array_low(a->v.sval);
            long long cnt = zhi - zlo + 1;
            if (i && (pas_is_charexpr(i) || (i->t == TT_FNC && i->n >= 2 && i->c[0] && i->c[0]->v.sval && !strcmp(i->c[0]->v.sval, "__pas_chrlit")))) {
                fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: the ordinal parameter of %s is not assignment-compatible with the index-type of the unpacked array '%s'\n", name, a->v.sval);
                g_pas_iso_errors++;
            } else if (_ahok && cnt > 0 && i && i->t == TT_ILIT) {
                long long st = i->v.ival;
                if (st < alo || st + cnt - 1 > ahi) {
                    fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s over the unpacked array '%s' declared [%lld..%lld] reads components [%lld..%lld], which is outside its index-type\n",
                            name, a->v.sval, alo, ahi, st, st + cnt - 1);
                    g_pas_iso_errors++;
                }
            }
        }
        { tree_t *src = ispack ? a : z;
          if (src && src->t == TT_VAR && src->v.sval && !pas_array_is_param(src->v.sval) && !pas_assigned_get(src->v.sval) && !pas_array_is_zero_size(src->v.sval)) {
              fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s reads the array '%s', whose components have never been assigned a value\n", name, src->v.sval);
              g_pas_iso_errors++;
          }
        }
        static int _pkn = 0; char _cvb[24]; snprintf(_cvb, sizeof _cvb, "__pas_pk%d", _pkn++); const char *_cv = ct_strdup(_cvb);
        tree_t *zi = ast_node_new(TT_IDX); ast_push(zi, pas_tree_clone(z)); ast_push(zi, leaf_s(TT_VAR, _cv));
        tree_t *ai = ast_node_new(TT_IDX); ast_push(ai, pas_tree_clone(a)); ast_push(ai, bin(TT_ADD, pas_tree_clone(i), bin(TT_SUB, leaf_s(TT_VAR, _cv), ilit(zlo))));
        tree_t *body = ispack ? mk_assign(zi, ai) : mk_assign(ai, zi);
        tree_t *f = ast_node_new(TT_FOR); ast_push(f, leaf_s(TT_VAR, _cv)); ast_push(f, ilit(zlo)); ast_push(f, ilit(zhi)); ast_push(f, body);
        return f;
    }
    if (name && (!strcmp(name, "get") || !strcmp(name, "put")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && pas_is_filevar(args->items[0]->v.sval) && !pas_is_stdstream(args->items[0]->v.sval)) {
        return mk_fnc1(!strcmp(name, "get") ? "__pas_tget" : "__pas_tput", args->items[0]);
    }
    if (name && args && args->count >= 1 && pas_is_tfile_node(args->items[0])) {
        tree_t *fa = args->items[0];
        if (!strcmp(name, "readln") || !strcmp(name, "writeln")) {
            fprintf(stderr, "pascal: ISO 7185 %s violation: the file parameter of '%s' shall be a textfile, but '%s' is a file of a non-text component-type\n",
                    !strcmp(name, "readln") ? "6.9.2" : "6.9.4", name, fa->v.sval ? fa->v.sval : "<file>");
            g_pas_iso_errors++;
        }
        if (!strcmp(name, "get")) return mk_fnc1("__pas_fget", fa);
        if (!strcmp(name, "put")) return mk_fnc1("__pas_fput", fa);
        if (!strcmp(name, "eof")) return mk_fnc1("__pas_feof_t", fa);
        if (!strcmp(name, "reset")) return pas_is_hdrfile(fa->v.sval)
            ? mk_assign(fa, mk_set_bin("__pas_treset", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval)))
            : mk_assign(fa, mk_fnc1("__pas_treset", pas_tree_clone(fa)));
        if (!strcmp(name, "rewrite")) return pas_is_hdrfile(fa->v.sval)
            ? mk_assign(fa, mk_set_bin("__pas_trewrite", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval)))
            : mk_assign(fa, mk_fnc1("__pas_trewrite", pas_tree_clone(fa)));
        if (!strcmp(name, "read") || !strcmp(name, "write")) {
            PNodeList *stmts = pnl_new();
            for (int i = 2; i + 1 < args->count; i += 2) {
                tree_t *v = args->items[i];
                long long _rlo, _rhi;
                if (!strcmp(name, "read") && v && v->t == TT_VAR && v->v.sval && pas_subvar_get(v->v.sval, &_rlo, &_rhi)) {
                    tree_t *rc = ast_node_new(TT_FNC); ast_push(rc, leaf_s(TT_VAR, "__pas_fread_range"));
                    ast_push(rc, pas_tree_clone(fa)); ast_push(rc, ilit(_rlo)); ast_push(rc, ilit(_rhi));
                    pnl_push(stmts, mk_assign(v, rc));
                } else if (!strcmp(name, "write") && fa && fa->t == TT_VAR && fa->v.sval && pas_tfcomp_range(fa->v.sval, &_rlo, &_rhi)) {
                    tree_t *wc = ast_node_new(TT_FNC); ast_push(wc, leaf_s(TT_VAR, "__pas_fwrite_range"));
                    ast_push(wc, pas_tree_clone(fa)); ast_push(wc, v); ast_push(wc, ilit(_rlo)); ast_push(wc, ilit(_rhi));
                    pnl_push(stmts, wc);
                } else if (!strcmp(name, "read")) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_fread", pas_tree_clone(fa))));
                else pnl_push(stmts, mk_set_bin("__pas_fwrite", pas_tree_clone(fa), v));
            }
            return seq_of(stmts);
        }
    }
    if (name && !strcmp(name, "chr") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t != TT_ILIT) a = pas_ord_check(a, 0, 255, "chr(x) of a value that is the ordinal number of no char-type value");
        return mk_fnc1("__pas_chrlit", a); }
    if (name && (!strcmp(name, "pred") || !strcmp(name, "succ")) && args && args->count >= 1) {
        int up = !strcmp(name, "succ"); long long lo, hi; tree_t *a = args->items[0];
        tree_t *v = bin(up ? TT_ADD : TT_SUB, a, ilit(1));
        if (a && a->t == TT_VAR && pas_ord_var_bounds(a->v.sval, &lo, &hi))
            v = pas_ord_check(v, lo, hi, up ? "succ(x) of the largest value of its type" : "pred(x) of the smallest value of its type");
        if (a && ((a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) || pas_is_charexpr(a))) return mk_fnc1("__pas_chrlit", v);
        if (a && pas_is_boolexpr(a)) return bin(TT_NE, v, ilit(0));
        return v; }
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1 && !(args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && pas_is_pcharvar(args->items[0]->v.sval))) {
        tree_t *v = args->items[0];
        tree_t *delta = (args->count >= 3) ? args->items[2] : ilit(1);
        tree_e op = !strcmp(name, "inc") ? TT_ADD : TT_SUB;
        return mk_assign(v, bin(op, pas_tree_clone(v), delta));
    }
    if (name && !strcmp(name, "setlength") && args && args->count >= 4) {
        tree_t *v = args->items[0]; tree_t *n = args->items[2];
        return mk_assign(v, mk_fnc2("__pas_setlength", pas_tree_clone(v), n));
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        long long hi;
        if (v && v->t == TT_VAR && v->v.sval && pas_array_high_get(v->v.sval, &hi)) {
            return ilit(!strcmp(name, "low") ? pas_array_nonparam_low(v->v.sval) : hi);
        }
        if (v && v->t == TT_VAR && v->v.sval) {
            long long lo2, hi2;
            if (pas_ordinal_bound_lookup(v->v.sval, &lo2, &hi2)) return ilit(!strcmp(name, "low") ? lo2 : hi2);
        }
        if (v && v->t == TT_VAR && v->v.sval) {
            int sk = pas_var_string_kind(v->v.sval);
            if (sk) { int _zb = sk == 1 && (g_pas_zerobased_strings & 1); return !strcmp(name, "low") ? ilit(sk == 2 || _zb ? 0 : 1) : (sk == 2 ? ilit(255) : (_zb ? bin(TT_SUB, mk_fnc1("length", pas_tree_clone(v)), ilit(1)) : mk_fnc1("length", pas_tree_clone(v)))); }
        }
    }
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && pas_is_pcharvar(args->items[0]->v.sval)) {
        tree_t *h = leaf_s(TT_VAR, pas_pchar_off_name(args->items[0]->v.sval)); tree_t *n = (args->count >= 3) ? args->items[2] : ilit(1);
        return bin(TT_ASSIGN, h, bin(name[0] == 'i' ? TT_ADD : TT_SUB, pas_tree_clone(h), n)); }
    if (name && !strcmp(name, "stringtowidechar") && args && args->count >= 6) { tree_t *x = args->items[2]; if (x && x->t == TT_IDX && x->n == 2 && x->c[0] && x->c[0]->t == TT_VAR) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_s2wide")); ast_push(f, args->items[0]); ast_push(f, x->c[0]); ast_push(f, x->c[1]); ast_push(f, args->items[4]); return f; } }
    if (name && (!strcmp(name, "widestring") || !strcmp(name, "ansistring") || !strcmp(name, "unicodestring")) && args && args->count >= 1 && pas_pchar_castname(args->items[0]) && args->items[0]->c[1] && args->items[0]->c[1]->t == TT_IDX && args->items[0]->c[1]->n == 2 && args->items[0]->c[1]->c[0] && args->items[0]->c[1]->c[0]->t == TT_VAR && !pas_var_string_kind(args->items[0]->c[1]->c[0]->v.sval)) { tree_t *x = args->items[0]->c[1]; tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_wide2s")); ast_push(f, x->c[0]); ast_push(f, x->c[1]); return f; }
    if (name && !strcmp(name, "sizeof") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_VAR && a->v.sval) {
            long long sz;
            if (pas_sizeof_lookup(a->v.sval, &sz)) return ilit(sz);
        }
    }
    if (name && !strcmp(name, "stringcodepage") && args && args->count >= 1) return ilit(g_pas_codepage);
    if (name && !strcmp(name, "ismanagedtype") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        const char *tn = (a && a->t == TT_VAR && a->v.sval) ? pas_scalarvartype_get(a->v.sval) : NULL;
        { int _guard = 0; while (tn) { const char *_al = pas_typealias_get(tn); if (!_al || !strcmp(_al, tn) || _guard++ >= 8) break; tn = _al; } }
        int managed = tn && (!strcmp(tn, "ansistring") || !strcmp(tn, "unicodestring") || !strcmp(tn, "widestring") || !strcmp(tn, "variant"));
        return ilit(managed ? 1 : 0);
    }
    if (name && !strcmp(name, "boolean") && args && args->count >= 1) return bin(TT_NE, mk_fnc2("iand", args->items[0], ilit(255)), ilit(0));
    if (name && strcmp(name, "char") && strcmp(name, "widechar") && strcmp(name, "boolean") && args && args->count >= 1) {
        long long _tsz;
        int _isboolfam = !strcmp(name, "bytebool") || !strcmp(name, "wordbool") || !strcmp(name, "longbool") || !strcmp(name, "qwordbool");
        const char *_tcn = name;
        { const char *_al = pas_typealias_get(_tcn); int _guard = 0; while (_al && strcmp(_al, _tcn) && _guard++ < 8) { _tcn = _al; _al = pas_typealias_get(_tcn); } }
        if ((!_isboolfam || pas_is_boolexpr(args->items[0])) && pas_sizeof_builtin_size(_tcn, &_tsz)) {
            if (_isboolfam || (_tsz != 1 && _tsz != 2 && _tsz != 4)) {
                tree_t *_a0 = args->items[0];
                return (_a0 && _a0->t == TT_VAR && _a0->v.sval && pas_var_is_boolfam(_a0->v.sval)) ? bin(TT_ADD, _a0, ilit(0)) : _a0;
            }
            long long _full = 1LL << (_tsz * 8); tree_t *_m = mk_fnc2("iand", args->items[0], ilit(_full - 1));
            if (pas_is_unsigned_inttypename(_tcn)) return _m;
            long long _half = _full / 2; return bin(TT_SUB, bin(TT_MOD, bin(TT_ADD, _m, ilit(_half)), ilit(_full)), ilit(_half));
        }
    }
    if (name && !strcmp(name, "swapendian") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        long long sz = 4;
        if (a && a->t == TT_VAR && a->v.sval) pas_sizeof_lookup(a->v.sval, &sz);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_swapendian"));
        ast_push(e, a);
        ast_push(e, ilit(sz));
        return e;
    }
    if (name && !strcmp(name, "addr") && args && args->count >= 1) return pas_addr_of_proc(args->items[0]);
    if (name && !strcmp(name, "fillchar") && args && args->count >= 5) {
        tree_t *dst = args->items[0]; tree_t *val = args->items[4];
        long long fhi = -1, flo = 0; tree_t *fdst = NULL;
        if (dst && dst->t == TT_VAR && dst->v.sval && pas_array_high_get(dst->v.sval, &fhi)) {
            flo = pas_array_low(dst->v.sval);
            fdst = leaf_s(TT_VAR, dst->v.sval);
        } else if (dst && dst->t == TT_FNC && dst->n >= 2 && dst->c[0] && dst->c[0]->v.sval && !strcmp(dst->c[0]->v.sval, "__pas_deref")) {
            const char *ptn = pas_ptrexpr_target(dst->c[1]);
            long long phi = ptn ? pas_arrtype_high(ptn) : -1;
            if (phi >= 0) { fhi = phi; flo = pas_arrtype_lo(ptn); fdst = mk_deref(pas_tree_clone(dst->c[1])); }
        }
        if (fdst) {
            static int _fcn = 0; char _fcb[24]; snprintf(_fcb, sizeof _fcb, "__pas_fc%d", _fcn++); const char *_fcv = ct_strdup(_fcb);
            tree_t *fidx = ast_node_new(TT_IDX); ast_push(fidx, fdst); ast_push(fidx, leaf_s(TT_VAR, _fcv));
            tree_t *fbody = mk_assign(fidx, val);
            tree_t *floop = ast_node_new(TT_FOR); ast_push(floop, leaf_s(TT_VAR, _fcv)); ast_push(floop, ilit(flo)); ast_push(floop, ilit(fhi));
            ast_push(floop, fbody);
            return floop;
        }
    }
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc2("__pas_trunc", args->items[0], ilit(1));
    if (name && !strcmp(name, "int") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
    if (name && !strcmp(name, "round") && args && args->count >= 1) return mk_fnc1("__pas_round", args->items[0]);
    if (name && !strcmp(name, "halt") && (!args || args->count == 0)) return mk_fnc0("__pas_halt");
    if (name && !strcmp(name, "halt") && args && args->count >= 1) return mk_fnc1("__pas_halt", args->items[0]);
    if (name && !strcmp(name, "frac") && args && args->count >= 1) return mk_fnc1("__pas_frac", args->items[0]);
    if (name && !strcmp(name, "assert") && args && args->count >= 1) return mk_fnc1("__pas_assert", args->items[0]);
    if (name && !strcmp(name, "mkdir") && args && args->count >= 1) return mk_fnc1("__pas_mkdir", args->items[0]);
    if (name && !strcmp(name, "rmdir") && args && args->count >= 1) return mk_fnc1("__pas_rmdir", args->items[0]);
    if (name && !strcmp(name, "pos") && args && args->count >= 3) {
        tree_t *needle = args->items[0];
        if (pas_is_charexpr(needle)) needle = mk_chr_wrap(needle);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_pos"));
        ast_push(e, needle);
        ast_push(e, args->items[2]);
        return e;
    }
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
        tree_t *fa = args->items[0]; int isf = (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) || pas_is_filevar_elem(fa);
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
    if (name && !strcmp(name, "delete") && args && args->count >= 6) {
        tree_t *sv = args->items[0]; tree_t *pos = args->items[2]; tree_t *cnt = args->items[4];
        return mk_assign(sv, mk_fnc3("__pas_str_delete", pas_tree_clone(sv), pos, cnt));
    }
    if (name && !strcmp(name, "insert") && args && args->count >= 6) {
        tree_t *src = args->items[0]; tree_t *sv = args->items[2]; tree_t *pos = args->items[4];
        if (src && src->t == TT_FNC && src->n == 2 && src->c[0] && src->c[0]->v.sval && !strcmp(src->c[0]->v.sval, "__pas_chrlit") && src->c[1] && src->c[1]->t == TT_ILIT) {
            char _cb[2]; _cb[0] = (char)src->c[1]->v.ival; _cb[1] = '\0'; src = leaf_s(TT_QLIT, ct_strdup(_cb));
        }
        return mk_assign(sv, mk_fnc3("__pas_str_insert", src, pas_tree_clone(sv), pos));
    }
    if (name && !strcmp(name, "readstr") && args && args->count >= 4) {
        tree_t *sv = args->items[0]; tree_t *v = args->items[2];
        tree_t *rhs = mk_fnc1("__pas_str_to_int", sv);
        if (g_pas_range_check_on && v && v->t == TT_VAR && v->v.sval) { long long _lo, _hi; if (pas_subvar_get(v->v.sval, &_lo, &_hi)) rhs = pas_range_wrap(rhs, _lo, _hi, NULL); }
        return mk_assign(v, rhs);
    }
    if (name && !strcmp(name, "readln") && (!args || args->count == 0)) return mk_fnc0("__pas_readln");
    if (name && (!strcmp(name, "readln") || !strcmp(name, "read")) && args && args->count >= 1) {
        int isln = !strcmp(name, "readln");
        int start = 0; tree_t *fstream = NULL;
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa)) { fstream = fa; start = 2; }
        else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) { if (!pas_is_stdstream(fa->v.sval)) fstream = fa; start = 2; }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            if (v && v->t == TT_VAR && v->v.sval && pas_is_chararr(v->v.sval)) {
                long long rlo = pas_array_low(v->v.sval); long long rhi = -1; pas_array_high_get(v->v.sval, &rhi);
                static int _rdcn = 0; char _rdcb[24]; snprintf(_rdcb, sizeof _rdcb, "__pas_rdi%d", _rdcn++); const char *_rdcv = ct_strdup(_rdcb);
                tree_t *ridx = ast_node_new(TT_IDX); ast_push(ridx, leaf_s(TT_VAR, v->v.sval)); ast_push(ridx, leaf_s(TT_VAR, _rdcv));
                tree_t *rval = fstream ? mk_fnc1("__pas_read_c_f", pas_tree_clone(fstream)) : mk_fnc0("__pas_read_c");
                tree_t *rloop = ast_node_new(TT_FOR); ast_push(rloop, leaf_s(TT_VAR, _rdcv)); ast_push(rloop, ilit(rlo)); ast_push(rloop, ilit(rhi));
                ast_push(rloop, mk_assign(ridx, rval));
                pnl_push(stmts, rloop);
                continue;
            }
            int isc = v && ((v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) || pas_is_charexpr(v));
            int isr = !isc && v && v->t == TT_VAR && v->v.sval && pas_var_is_real(v->v.sval);
            tree_t *rv = isr ? (fstream ? mk_set_bin("__pas_read_i_f", pas_tree_clone(fstream), ilit(1)) : mk_fnc1("__pas_read_i", ilit(1)))
                             : fstream ? mk_fnc1(isc ? "__pas_read_c_f" : "__pas_read_i_f", pas_tree_clone(fstream)) : mk_fnc0(isc ? "__pas_read_c" : "__pas_read_i");
            { long long _lo, _hi; if (!isr && g_pas_range_check_on && v && v->t == TT_VAR && v->v.sval && !pas_is_setvar(v->v.sval) && pas_subvar_get(v->v.sval, &_lo, &_hi)) rv = pas_range_wrap(rv, _lo, _hi, "6.9.1"); }
            pnl_push(stmts, mk_assign(v, rv));
        }
        if (isln) { if (fstream) pnl_push(stmts, mk_fnc1("__pas_readln_f", pas_tree_clone(fstream))); else pnl_push(stmts, mk_fnc0("__pas_readln")); }
        return seq_of(stmts);
    }
    if (name && (!strcmp(name, "ReadInt") || !strcmp(name, "readint")) && args && args->count >= 3) {
        tree_t *fa = args->items[0]; tree_t *fstream = NULL; int start = 0;
        if (pas_is_filevar_elem(fa)) { fstream = fa; start = 2; }
        else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) { if (!pas_is_stdstream(fa->v.sval)) fstream = fa; start = 2; }
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
        return mk_assign(fv, mk_fnc1("__pas_fassign", pas_alpha_wrap(nm)));
    }
    if (name && !strcmp(name, "rewrite") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ? ((g_pas_mode_iso && pas_is_hdrfile(fv->v.sval)) ? ilit(1) : leaf_s(TT_QLIT, fv->v.sval)) : NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_rewrite", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "append") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_append", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_append", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ? ((g_pas_mode_iso && pas_is_hdrfile(fv->v.sval)) ? ilit(0) : leaf_s(TT_QLIT, fv->v.sval)) : NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_reset", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_reset", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "close") && args && args->count >= 1) {
        return mk_fnc1("__pas_fclose", args->items[0]);
    }
    if (name && !strcmp(name, "erase") && args && args->count >= 1) {
        return mk_fnc1("__pas_ferase", args->items[0]);
    }
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv = args->items[0];
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, pas_new_target_init(rt)); }
        else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        tree_t *as = mk_assign(pv, alloc);
        if (args->count >= 4 && rt) { struct pas_vt *vt = pas_vt_find_name(rt); if (vt && vt->tagfi >= 0) {
            tree_t *tg = ast_node_new(TT_IDX); ast_push(tg, mk_deref(pas_tree_clone(pv))); ast_push(tg, ilit(pas_rectype_slot(rt, vt->tagfi)));
            tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, as); ast_push(q, mk_assign(tg, args->items[2])); ast_push(q, mk_fnc1("__pas_tagmark", pas_tree_clone(pv))); return q; } }
        return as;
    }
    if (name && !strcmp(name, "mark") && args && args->count >= 1) {
        tree_t *mk = ast_node_new(TT_FNC);
        ast_push(mk, leaf_s(TT_VAR, "__pas_mark"));
        return mk_assign(args->items[0], mk);
    }
    if (name && !strcmp(name, "release") && args && args->count >= 1) {
        return mk_fnc1("__pas_release", args->items[0]);
    }
    if (name && !strcmp(name, "dispose") && args && args->count >= 1) {
        if (args->count >= 4) { const char *drt = pas_ptrexpr_target(args->items[0]); struct pas_vt *vt = drt ? pas_vt_find_name(drt) : NULL;
            tree_t *tn = ilit(-1); if (vt && vt->tagfi >= 0) { tn = ast_node_new(TT_IDX); ast_push(tn, mk_deref(pas_tree_clone(args->items[0]))); ast_push(tn, ilit(pas_rectype_slot(drt, vt->tagfi))); }
            tree_t *dk = ast_node_new(TT_FNC); ast_push(dk, leaf_s(TT_VAR, "__pas_dispose_k")); ast_push(dk, args->items[0]); ast_push(dk, args->items[2]); ast_push(dk, tn); return dk; }
        return mk_fnc1("__pas_dispose", args->items[0]);
    }
    tree_t *e = ast_node_new(TT_FNC);
    int _wstart = 0; tree_t *_wstream = NULL;
    if (is_pas_io(map_io(name)) && args && args->count >= 2) {
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa) || (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval) && !pas_is_stdstream(fa->v.sval))) { _wstream = fa; _wstart = 2; }
        else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval)) { _wstart = 2; }
    }
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (_wstream) { ast_push(e, pas_tree_clone(_wstream)); ast_push(e, ilit(-9)); }
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = _wstart; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i]; tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val);
                const char *_enm = (val && val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
                if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); val = _w; }
                else if (is_char) { val = mk_chr_wrap(val); if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2); }
                else if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, bin(TT_NE, val, ilit(0))); ast_push(_w, leaf_s(TT_QLIT, "false,true")); val = _w; if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(5); }
                else if (val && val->t == TT_VAR && val->v.sval && pas_is_singlevar(val->v.sval)) { if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-5); }
                else if (pas_is_currencyexpr(val)) { if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-6); }
                else if (pas_is_qword(val) && val->t == TT_VAR) { if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-7); }
                else if (val && val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) { val = pas_alpha_wrap(val); }
                else if (pas_ca_is_read(val)) { val = pas_alpha_wrap(val); }
                else if (val && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(_w, val); ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval))); val = _w; }
                if (g_pas_seen_mode_directive && wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-4);
                ast_push(e, val); ast_push(e, wid);
            }
        } else {
            unsigned long long _cam = pas_caparm_mask(name);
            for (int i = 0; i < args->count; i += 2) {
                tree_t *val = args->items[i]; int _pidx = i / 2;
                if (val && val->t == TT_SEQ_EXPR && val->n == 2 && val->c[0] && val->c[0]->t == TT_FNC && val->c[0]->n >= 1 && val->c[0]->c[0] && val->c[0]->c[0]->v.sval && !strcmp(val->c[0]->c[0]->v.sval, "__pas_vcheck") && pas_proc_param_is_var(name, _pidx)) val = val->c[1];
                if (val && val->t == TT_VAR && val->v.sval && pas_is_stdstream(val->v.sval) && !pas_ptrvar_target(val->v.sval) && pas_proc_param_is_var(name, _pidx)) val = mk_fnc1("__pas_stdfile", ilit(!strcmp(val->v.sval, "input") ? 0 : 1));
                else if (val && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx) && pas_is_aggregate_designator(val)) val = mk_fnc1("__pas_arr_copy", val);
                if (val && val->t == TT_QLIT && val->v.sval && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) { long long _lo = pas_caparm_lo(name, _pidx); val = pas_str_to_alpha(val->v.sval, _lo, _lo + (long long)strlen(val->v.sval) - 1); }
                else if (pas_ca_is_read(val) && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) { tree_t *_en = ast_node_new(TT_FNC); ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode")); ast_push(_en, val); ast_push(_en, ilit(pas_caparm_lo(name, _pidx))); ast_push(_en, ilit(-1)); val = _en; }
                if (val && val->t != TT_FLIT && pas_proc_param_is_real(name, _pidx)) val = bin(TT_ADD, val, flit(0.0));
                if (val && val->t == TT_FNC && val->n == 2 && val->c[0] && val->c[0]->v.sval && !strcmp(val->c[0]->v.sval, "__pas_chrlit") && val->c[1] && val->c[1]->t == TT_ILIT && pas_proc_param_is_string(name, _pidx)) {
                    char _cb[2]; _cb[0] = (char)val->c[1]->v.ival; _cb[1] = '\0'; val = leaf_s(TT_QLIT, ct_strdup(_cb));
                }
                ast_push(e, val);
            }
        }
    }
    return e;
}
static tree_t *pas_mod(tree_t *a, tree_t *b) {
    tree_t *b1 = b;
    if (!b || b->t != TT_ILIT || b->v.ival <= 0) { b1 = pas_ord_check(b, 1, 9223372036854775807LL, "i mod j with j zero or negative"); ast_push(b1, leaf_s(TT_QLIT, "6.7.2.2")); }
    return bin(TT_MOD, bin(TT_ADD, bin(TT_MOD, a, b1), pas_tree_clone(b)), pas_tree_clone(b)); }
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
static struct { const char *name; int nvp; int vp[16]; int nrp; int rp[16]; int nsp; int sp[16]; } g_pas_funcs[256]; static int g_pas_nfunc;
static void pas_func_add(const char *name) { if (g_pas_nfunc < 256 && name) { g_pas_funcs[g_pas_nfunc].name = ct_strdup(name); g_pas_funcs[g_pas_nfunc].nvp = 0; g_pas_funcs[g_pas_nfunc].nrp = 0; g_pas_funcs[g_pas_nfunc].nsp = 0; g_pas_nfunc++; } }
static int pas_is_func(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nfunc; i++) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return 1; return 0; }
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_range_check"));
    ast_push(e, val); ast_push(e, ilit(lo)); ast_push(e, ilit(hi));
    int isset = clause && !strcmp(clause, "set"); const char *cl = isset ? "6.8.2.2" : clause;
    if (g_pas_range_check_on == 2 && cl) ast_push(e, leaf_s(TT_QLIT, cl)); else if (isset) ast_push(e, leaf_s(TT_QLIT, ""));
    if (isset) ast_push(e, leaf_s(TT_QLIT, "set"));
    return e;
}
static long long pas_hash_stable(const char *nm) {
    unsigned long long h = 1469598103934665603ULL;
    for (const char *p = nm; *p; p++) { h ^= (unsigned char)*p; h *= 1099511628211ULL; }
    unsigned long long v = (1000000000ULL + (h % 1000000000ULL)) & ~0xFULL;
    return (long long)v;
}
static int pas_addr_base_offset(tree_t *e, long long *out) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { *out = pas_hash_stable(e->v.sval); return 1; }
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        long long base; if (!pas_addr_base_offset(e->c[0], &base)) return 0;
        const char *rt = pas_with_sel_rtype(e->c[0]); if (!rt) return 0;
        long long foff; if (!pas_rectype_field_offset(rt, (int)e->c[1]->v.ival, &foff)) return 0;
        *out = base + foff; return 1;
    }
    return 0;
}
static tree_t *pas_addr_of_proc(tree_t *e) {
    if (!e) return e;
    const char *nm = NULL;
    if (e->t == TT_VAR && e->v.sval && (pas_is_proc(e->v.sval) || pas_is_func(e->v.sval))) nm = e->v.sval;
    else if (e->t == TT_FNC && e->n == 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_func(e->c[0]->v.sval)) nm = e->c[0]->v.sval;
    if (nm) return ilit(pas_hash_stable(nm));
    { long long addr; if (pas_addr_base_offset(e, &addr)) return ilit(addr); }
    return e;
}
static struct { const char *name; int nvp; int vp[16]; int nrp; int rp[16]; int nsp; int sp[16]; } g_pas_procs[256]; static int g_pas_nproc;
static void pas_proc_add(const char *name) { if (g_pas_nproc < 256 && name) { g_pas_procs[g_pas_nproc].name = ct_strdup(name); g_pas_procs[g_pas_nproc].nvp = 0; g_pas_procs[g_pas_nproc].nrp = 0; g_pas_procs[g_pas_nproc].nsp = 0; g_pas_nproc++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_vparam_scan(PNodeList *params, int *out) { int nv = 0; if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) { tree_t *it = params->items[j]; int isvar = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_SUCCEED) { isvar = 1; break; }
        out[nv++] = isvar; }
    return nv; }
static int pas_realparam_scan(PNodeList *params, int *out) { int nv = 0; if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) { tree_t *it = params->items[j]; int isreal = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_FLIT) { isreal = 1; break; }
        out[nv++] = isreal; }
    return nv; }
static int pas_strparam_scan(PNodeList *params, int *out) { int nv = 0; if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) { tree_t *it = params->items[j]; int isstr = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_QLIT) { isstr = 1; break; }
        out[nv++] = isstr; }
    return nv; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_proc_vparams(const char *name, PNodeList *params) { if (!name || !params) return;
    for (int i = g_pas_nproc - 1; i >= 0; i--) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) { g_pas_procs[i].nvp = pas_vparam_scan(params, g_pas_procs[i].vp); g_pas_procs[i].nrp = pas_realparam_scan(params, g_pas_procs[i].rp); g_pas_procs[i].nsp = pas_strparam_scan(params, g_pas_procs[i].sp); return; }
    for (int i = g_pas_nfunc - 1; i >= 0; i--) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) { g_pas_funcs[i].nvp = pas_vparam_scan(params, g_pas_funcs[i].vp); g_pas_funcs[i].nrp = pas_realparam_scan(params, g_pas_funcs[i].rp); g_pas_funcs[i].nsp = pas_strparam_scan(params, g_pas_funcs[i].sp); return; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_required_needs_params(const char *name) {
    const char *nm[] = { "get", "put", "reset", "rewrite", "new", "dispose", "pack", "unpack", "read", "write" };
    const char *cl[] = { "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.3", "6.6.5.3", "6.6.5.4", "6.6.5.4", "6.9.1", "6.9.3" };
    for (size_t i = 0; name && i < sizeof nm / sizeof nm[0]; i++) if (!strcmp(name, nm[i])) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: the required procedure '%s' is activated with no actual-parameters, and its activation takes at least one\n", cl[i], name);
        g_pas_iso_errors++; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_call_arity(const char *name, PNodeList *args) {
    const char *req[] = { "abs", "arctan", "chr", "cos", "dispose", "eof", "eoln", "exp", "get", "ln", "new", "odd", "ord", "pack", "page", "pred", "put",
                          "read", "readln", "reset", "rewrite", "round", "sin", "sqr", "sqrt", "succ", "trunc", "unpack", "write", "writeln" };
    if (!name || !args || args->count % 2) return;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return;
    for (int i = 1; i < args->count; i += 2) if (!args->items[i] || args->items[i]->t != TT_ILIT || args->items[i]->v.ival != -1) return;
    int argc = args->count / 2, nent = 0, formals = -1; const char *clause = NULL;
    for (int i = 0; i < g_pas_nproc; i++) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) {
        if (g_pas_procs[i].nvp >= 16 || g_pas_procs[i].nvp == argc) return; nent++; formals = g_pas_procs[i].nvp; clause = "6.8.2.3"; }
    for (int i = 0; i < g_pas_nfunc; i++) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) {
        if (g_pas_funcs[i].nvp >= 16 || g_pas_funcs[i].nvp == argc) return; nent++; formals = g_pas_funcs[i].nvp; clause = "6.7.3"; }
    if (!nent) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with %d actual-parameter(s), and the number of actual-parameters shall equal the number of its formal-parameters (%d)\n",
            clause, name, argc, formals);
    g_pas_iso_errors++;
}
static int pas_proc_param_is_var(const char *name, int idx) { if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_pas_nproc - 1; i >= 0; i--) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) return (idx < g_pas_procs[i].nvp) ? g_pas_procs[i].vp[idx] : 0;
    for (int i = g_pas_nfunc - 1; i >= 0; i--) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return (idx < g_pas_funcs[i].nvp) ? g_pas_funcs[i].vp[idx] : 0; return 0; }
static int pas_proc_param_is_string(const char *name, int idx) { if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_pas_nproc - 1; i >= 0; i--) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) return (idx < g_pas_procs[i].nsp) ? g_pas_procs[i].sp[idx] : 0;
    for (int i = g_pas_nfunc - 1; i >= 0; i--) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return (idx < g_pas_funcs[i].nsp) ? g_pas_funcs[i].sp[idx] : 0; return 0; }
static int pas_proc_param_is_real(const char *name, int idx) { if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_pas_nproc - 1; i >= 0; i--) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) return (idx < g_pas_procs[i].nrp) ? g_pas_procs[i].rp[idx] : 0;
    for (int i = g_pas_nfunc - 1; i >= 0; i--) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return (idx < g_pas_funcs[i].nrp) ? g_pas_funcs[i].rp[idx] : 0; return 0; }
static int pas_is_proc(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nproc; i++) if (g_pas_procs[i].name && !strcmp(g_pas_procs[i].name, name)) return 1; return 0; }
static struct { char *name; unsigned long long camask; long long lo[64]; } g_pas_caparm[256]; static int g_pas_ncaparm;
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo) { if (g_pas_ncaparm < 256 && name && m) { g_pas_caparm[g_pas_ncaparm].name = ct_strdup(name); g_pas_caparm[g_pas_ncaparm].camask = m; for (int i = 0; i < 64; i++) g_pas_caparm[g_pas_ncaparm].lo[i] = lo ? lo[i] : 0; g_pas_ncaparm++; } }
static unsigned long long pas_caparm_mask(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncaparm; i++) if (g_pas_caparm[i].name && !strcmp(g_pas_caparm[i].name, name)) return g_pas_caparm[i].camask; return 0; }
static long long pas_caparm_lo(const char *name, int pos) { if (!name || pos < 0 || pos >= 64) return 0; for (int i = 0; i < g_pas_ncaparm; i++) if (g_pas_caparm[i].name && !strcmp(g_pas_caparm[i].name, name)) return g_pas_caparm[i].lo[pos]; return 0; }
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = ct_strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; double val; } g_pas_rconsts[64]; static int g_pas_nrconst;
static void pas_rconst_add(const char *name, double v) { if (g_pas_nrconst < 64 && name) { g_pas_rconsts[g_pas_nrconst].name = ct_strdup(name); g_pas_rconsts[g_pas_nrconst].val = v; g_pas_nrconst++; } }
int pas_rconst_get(const char *name, double *out) { if (!name) return 0; for (int i = 0; i < g_pas_nrconst; i++) if (g_pas_rconsts[i].name && !strcmp(g_pas_rconsts[i].name, name)) { *out = g_pas_rconsts[i].val; return 1; } return 0; }
static struct { char *name; char *val; } g_pas_sconsts[64]; static int g_pas_nsconst;
static void pas_sconst_add(const char *name, const char *v) { if (g_pas_nsconst < 64 && name && v) { g_pas_sconsts[g_pas_nsconst].name = ct_strdup(name); g_pas_sconsts[g_pas_nsconst].val = ct_strdup(v); g_pas_nsconst++; } }
const char *pas_sconst_get(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsconst; i++) if (g_pas_sconsts[i].name && !strcmp(g_pas_sconsts[i].name, name)) return g_pas_sconsts[i].val; return 0; }
static int g_pas_level = 1;
static struct { char *name; long long high; long long ncols; int is_param; int is_local; long long low; } g_pas_arrays[256]; static int g_pas_narray; static long long g_pas_pend_arr_ncols;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = ct_strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = -1; g_pas_arrays[g_pas_narray].is_param = 0; g_pas_arrays[g_pas_narray].is_local = (g_pas_level >= 2);
    g_pas_arrays[g_pas_narray].low = pas_array_decl_low(); g_pas_narray++; } }
static void pas_array_add2d(const char *name, long long high, long long ncols) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = ct_strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = ncols; g_pas_arrays[g_pas_narray].is_param = 0; g_pas_arrays[g_pas_narray].is_local = (g_pas_level >= 2); g_pas_arrays[g_pas_narray].low = g_pas_pend_sub_low; g_pas_narray++; } }
static void pas_array_add2d_param(const char *name, long long high, long long ncols) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = ct_strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = ncols; g_pas_arrays[g_pas_narray].is_param = 1; g_pas_arrays[g_pas_narray].is_local = 0; g_pas_arrays[g_pas_narray].low = 0; g_pas_narray++; } }
static long long pas_array_ncols(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return g_pas_arrays[i].ncols; return -1; }
static long long pas_array_low(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return g_pas_arrays[i].low; return 0; }
static int pas_array_is_param(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return g_pas_arrays[i].is_param; return 0; }
static struct { char *name; } g_pas_assigned[512]; static int g_pas_nassigned;
static void pas_assigned_add(const char *n) { if (!n) return; for (int i = 0; i < g_pas_nassigned; i++) if (g_pas_assigned[i].name && !strcmp(g_pas_assigned[i].name, n)) return; if (g_pas_nassigned < 512) { g_pas_assigned[g_pas_nassigned].name = ct_strdup(n); g_pas_nassigned++; } }
static int pas_assigned_get(const char *n) { if (!n) return 0; for (int i = 0; i < g_pas_nassigned; i++) if (g_pas_assigned[i].name && !strcmp(g_pas_assigned[i].name, n)) return 1; return 0; }
static const char *pas_selector_base_name(tree_t *e) { while (e) { if (e->t == TT_VAR) return e->v.sval; if ((e->t == TT_IDX || e->t == TT_FIELD) && e->n >= 1 && e->c[0]) { e = e->c[0]; continue; } if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref") && e->c[1]) { e = e->c[1]; continue; } break; } return NULL; }
int g_pas_iso_errors = 0;
int pascal_iso_error_count(void) { return g_pas_iso_errors; }
void pascal_iso_error_reset(void) { g_pas_iso_errors = 0; }
int pas_is_agg_local(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && g_pas_arrays[i].is_local && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) return 1; return 0; }
static int pas_array_high_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) { *out = g_pas_arrays[i].high; return 1; } return 0; }
static long long g_pas_pend_sub_low;
static struct { char *name; long long high; int ndim2; long long ncols; int ischar; long long lo; } g_pas_arrtypes[64]; static int g_pas_narrtype; static int g_pas_pend_arr_ischar; static int g_pas_pend_arr_wrap;
static void pas_arrtype_add(const char *name, long long high, int ndim2, long long ncols) { if (g_pas_narrtype < 64 && name) { g_pas_arrtypes[g_pas_narrtype].name = ct_strdup(name); g_pas_arrtypes[g_pas_narrtype].high = high; g_pas_arrtypes[g_pas_narrtype].ndim2 = ndim2; g_pas_arrtypes[g_pas_narrtype].ncols = ncols; g_pas_arrtypes[g_pas_narrtype].ischar = g_pas_pend_arr_ischar; g_pas_arrtypes[g_pas_narrtype].lo = ndim2 ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : g_pas_pend_sub_low; g_pas_narrtype++; } }
static long long pas_arrtype_high(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].high; return -1; }
static long long pas_arrtype_ncols(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].ndim2 ? g_pas_arrtypes[i].ncols : -1; return -1; }
static long long pas_arrtype_lo(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].lo; return 0; }
static int pas_arrtype_ischar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_narrtype; i++) if (g_pas_arrtypes[i].name && !strcmp(g_pas_arrtypes[i].name, name)) return g_pas_arrtypes[i].ischar; return 0; }
static struct { char *name; long long high; long long min_size; } g_pas_enumtypes[64]; static int g_pas_nenum; static long long g_pas_pend_enum_max;
static long long g_pas_pend_set_hi = -1;
static void pas_enumtype_add(const char *n, long long h) { if (g_pas_nenum < 64 && n) { g_pas_enumtypes[g_pas_nenum].name = ct_strdup(n); g_pas_enumtypes[g_pas_nenum].high = h; g_pas_enumtypes[g_pas_nenum].min_size = g_pas_min_enum_size; g_pas_nenum++; } }
static long long pas_enumtype_high(const char *n) { if (!n) return -1; for (int i = 0; i < g_pas_nenum; i++) if (g_pas_enumtypes[i].name && !strcmp(g_pas_enumtypes[i].name, n)) return g_pas_enumtypes[i].high; return -1; }
static int pas_enumtype_min_size(const char *n, long long *out) { if (!n) return 0; for (int i = 0; i < g_pas_nenum; i++) if (g_pas_enumtypes[i].name && !strcmp(g_pas_enumtypes[i].name, n)) { *out = g_pas_enumtypes[i].min_size; return 1; } return 0; }
static struct { char *tname; char *names; } g_pas_enumnames[64]; static int g_pas_nenumname; static char g_pas_pend_enum_names[512];
static void pas_enumnames_add(const char *tn, const char *names) { if (g_pas_nenumname < 64 && tn && names && names[0]) { g_pas_enumnames[g_pas_nenumname].tname = ct_strdup(tn); g_pas_enumnames[g_pas_nenumname].names = ct_strdup(names); g_pas_nenumname++; } }
static int pas_enumnames_idx(const char *tn) { if (!tn) return -1; for (int i = 0; i < g_pas_nenumname; i++) if (g_pas_enumnames[i].tname && !strcmp(g_pas_enumnames[i].tname, tn)) return i; return -1; }
static const char *pas_enumnames_by_idx(int i) { return (i >= 0 && i < g_pas_nenumname) ? g_pas_enumnames[i].names : NULL; }
static struct { char *aname; char *etype; } g_pas_enumarrs[128]; static int g_pas_nenumarr;
static void pas_enumarr_add(const char *a, const char *et) { if (g_pas_nenumarr < 128 && a && et) { g_pas_enumarrs[g_pas_nenumarr].aname = ct_strdup(a); g_pas_enumarrs[g_pas_nenumarr].etype = ct_strdup(et); g_pas_nenumarr++; } }
static const char *pas_enumarr_get(const char *a) { if (!a) return NULL; for (int i = 0; i < g_pas_nenumarr; i++) if (g_pas_enumarrs[i].aname && !strcmp(g_pas_enumarrs[i].aname, a)) return g_pas_enumarrs[i].etype; return NULL; }
static struct { char *name; long long low; long long high; } g_pas_subtypes[64]; static int g_pas_nsubtype; static long long g_pas_pend_sub_low; static long long g_pas_pend_sub_high;
static void pas_subtype_add(const char *n, long long lo, long long hi) { if (g_pas_nsubtype < 64 && n) { g_pas_subtypes[g_pas_nsubtype].name = ct_strdup(n); g_pas_subtypes[g_pas_nsubtype].low = lo; g_pas_subtypes[g_pas_nsubtype].high = hi; g_pas_nsubtype++; } }
static long long pas_subtype_high(const char *n) { if (!n) return -1; for (int i = 0; i < g_pas_nsubtype; i++) if (g_pas_subtypes[i].name && !strcmp(g_pas_subtypes[i].name, n)) return g_pas_subtypes[i].high; return -1; }
static long long pas_subtype_low(const char *n) { if (!n) return 0; for (int i = 0; i < g_pas_nsubtype; i++) if (g_pas_subtypes[i].name && !strcmp(g_pas_subtypes[i].name, n)) return g_pas_subtypes[i].low; return 0; }
static struct { char *name; long long low; long long high; int uid; } g_pas_subvars[256]; static int g_pas_nsubvar;
static void pas_subvar_add(const char *n, long long lo, long long hi) { if (g_pas_nsubvar < 256 && n) { g_pas_subvars[g_pas_nsubvar].name = ct_strdup(n); g_pas_subvars[g_pas_nsubvar].uid = pas_scope_uid(n); g_pas_subvars[g_pas_nsubvar].low = lo; g_pas_subvars[g_pas_nsubvar].high = hi; g_pas_nsubvar++; } }
static int pas_subvar_get(const char *n, long long *lo, long long *hi) { if (!n) return 0; int u = pas_scope_uid(n); for (int i = g_pas_nsubvar - 1; i >= 0; i--) if (g_pas_subvars[i].name && !strcmp(g_pas_subvars[i].name, n) && g_pas_subvars[i].uid == u) { *lo = g_pas_subvars[i].low; *hi = g_pas_subvars[i].high; return 1; } return 0; }
static struct { char *name; char *target; } g_pas_typealias[64]; static int g_pas_ntypealias;
static void pas_typealias_add(const char *n, const char *t) { if (g_pas_ntypealias < 64 && n && t && strcmp(n, t)) { g_pas_typealias[g_pas_ntypealias].name = ct_strdup(n); g_pas_typealias[g_pas_ntypealias].target = ct_strdup(t); g_pas_ntypealias++; } }
static const char *pas_typealias_get(const char *n) { if (!n) return NULL; for (int i = g_pas_ntypealias - 1; i >= 0; i--) if (g_pas_typealias[i].name && !strcmp(g_pas_typealias[i].name, n)) return g_pas_typealias[i].target; return NULL; }
static int g_pas_ldepth;
static struct { char *vname; char *tname; int is_global; int uid; } g_pas_scalarvartype[512]; static int g_pas_nscalarvartype;
static void pas_scalarvartype_add(const char *vn, const char *tn) { if (g_pas_nscalarvartype < 512 && vn && tn) { g_pas_scalarvartype[g_pas_nscalarvartype].vname = ct_strdup(vn); g_pas_scalarvartype[g_pas_nscalarvartype].tname = ct_strdup(tn); g_pas_scalarvartype[g_pas_nscalarvartype].is_global = (g_pas_ldepth == 0); g_pas_scalarvartype[g_pas_nscalarvartype].uid = pas_scope_uid(vn); g_pas_nscalarvartype++; } }
static const char *pas_scalarvartype_get(const char *vn) { if (!vn) return NULL; for (int i = g_pas_nscalarvartype - 1; i >= 0; i--) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, vn)) return g_pas_scalarvartype[i].tname; return NULL; }
static int pas_string_type_kind(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) {
        if (!strcmp(t, "shortstring") || !strcmp(t, "openstring")) return 2;
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_var_string_kind(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_pas_nscalarvartype - 1; name && i >= 0; i--) if (g_pas_scalarvartype[i].vname && g_pas_scalarvartype[i].uid == u && !strcmp(g_pas_scalarvartype[i].vname, name)) return pas_string_type_kind(g_pas_scalarvartype[i].tname);
    return 0;
}
static struct { char *name; long long low; long long high; int ischar; } g_pas_tfcomp[128]; static int g_pas_ntfcomp;
static void pas_tfcomp_add(const char *n, long long lo, long long hi, int ischar) { if (g_pas_ntfcomp < 128 && n) { g_pas_tfcomp[g_pas_ntfcomp].name = ct_strdup(n); g_pas_tfcomp[g_pas_ntfcomp].low = lo; g_pas_tfcomp[g_pas_ntfcomp].high = hi; g_pas_tfcomp[g_pas_ntfcomp].ischar = ischar; g_pas_ntfcomp++; } }
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi) { if (!n) return 0; for (int i = g_pas_ntfcomp - 1; i >= 0; i--) if (g_pas_tfcomp[i].name && !strcmp(g_pas_tfcomp[i].name, n) && g_pas_tfcomp[i].high >= g_pas_tfcomp[i].low) { *lo = g_pas_tfcomp[i].low; *hi = g_pas_tfcomp[i].high; return 1; } return 0; }
static int pas_tfcomp_nonchar(const char *n) { if (!n) return 0; for (int i = g_pas_ntfcomp - 1; i >= 0; i--) if (g_pas_tfcomp[i].name && !strcmp(g_pas_tfcomp[i].name, n)) return !g_pas_tfcomp[i].ischar; return 0; }
static long long g_pas_pend_frng_lo; static long long g_pas_pend_frng_hi = -1; static int g_pas_pend_frng_ischar;
#define PAS_REC_MAX 512
#define PAS_FIELD_MAX 128
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; char *fldenum[PAS_FIELD_MAX]; char *fldrec[PAS_FIELD_MAX]; char *fldtypename[PAS_FIELD_MAX]; int fldca[PAS_FIELD_MAX]; int fldna[PAS_FIELD_MAX]; long long fldna_lo[PAS_FIELD_MAX]; long long fldna_hi[PAS_FIELD_MAX]; long long fldca_lo[PAS_FIELD_MAX]; long long fldca_hi[PAS_FIELD_MAX]; int fldchar[PAS_FIELD_MAX]; int fldfile[PAS_FIELD_MAX]; int fldov[PAS_FIELD_MAX]; int nf; int packed; int align_mac68k; int pack_n; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct pas_recvar_s { char *vname; char *fields[PAS_FIELD_MAX]; int nf; int fldchar[PAS_FIELD_MAX]; int packed; char *fldrec[PAS_FIELD_MAX]; int fldna[PAS_FIELD_MAX]; long long fldna_lo[PAS_FIELD_MAX]; long long fldna_hi[PAS_FIELD_MAX]; int fldov[PAS_FIELD_MAX]; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static char *g_pas_pend_fldenum[PAS_FIELD_MAX]; static char *g_pas_pend_fldrec[PAS_FIELD_MAX]; static char *g_pas_pend_fldtypename[PAS_FIELD_MAX]; static int g_pas_pend_fldca[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_lo[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_hi[PAS_FIELD_MAX]; static int g_pas_pend_fldchar[PAS_FIELD_MAX]; static int g_pas_pend_fldfile[PAS_FIELD_MAX]; static int g_pas_pend_fldov[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static int g_pas_recbody_depth;
struct pas_pend_frame { char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; char *fldenum[PAS_FIELD_MAX]; char *fldrec[PAS_FIELD_MAX]; char *fldtypename[PAS_FIELD_MAX]; int fldca[PAS_FIELD_MAX]; long long fldca_lo[PAS_FIELD_MAX]; long long fldca_hi[PAS_FIELD_MAX]; int fldchar[PAS_FIELD_MAX]; int fldfile[PAS_FIELD_MAX]; int fldna[PAS_FIELD_MAX]; long long fldna_lo[PAS_FIELD_MAX]; long long fldna_hi[PAS_FIELD_MAX]; int fldov[PAS_FIELD_MAX]; int nf; };
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename; static int g_pas_pend_ischar;
static int g_pas_pend_isarr; static int g_pas_pend_fldna[PAS_FIELD_MAX]; static long long g_pas_pend_fldna_lo[PAS_FIELD_MAX]; static long long g_pas_pend_fldna_hi[PAS_FIELD_MAX];
static void pas_pend_nest_save(void) {
    if (g_pas_scope.npnest >= g_pas_scope.cpnest) { g_pas_scope.cpnest = g_pas_scope.cpnest ? g_pas_scope.cpnest * 2 : 4; g_pas_scope.pnest = (struct pas_pend_frame *)ct_grow(g_pas_scope.pnest, (size_t)g_pas_scope.cpnest * sizeof *g_pas_scope.pnest); }
    struct pas_pend_frame *fr = &g_pas_scope.pnest[g_pas_scope.npnest++]; int n = g_pas_pend_nf; fr->nf = n;
    for (int i = 0; i < n; i++) { fr->fields[i] = g_pas_pend_fields[i]; fr->fldptrto[i] = g_pas_pend_fldptrto[i]; fr->fldenum[i] = g_pas_pend_fldenum[i]; fr->fldrec[i] = g_pas_pend_fldrec[i]; fr->fldtypename[i] = g_pas_pend_fldtypename[i]; fr->fldca[i] = g_pas_pend_fldca[i]; fr->fldca_lo[i] = g_pas_pend_fldca_lo[i]; fr->fldca_hi[i] = g_pas_pend_fldca_hi[i]; fr->fldchar[i] = g_pas_pend_fldchar[i]; fr->fldfile[i] = g_pas_pend_fldfile[i]; fr->fldna[i] = g_pas_pend_fldna[i]; fr->fldna_lo[i] = g_pas_pend_fldna_lo[i]; fr->fldna_hi[i] = g_pas_pend_fldna_hi[i]; fr->fldov[i] = g_pas_pend_fldov[i]; }
    g_pas_pend_nf = 0; }
static void pas_pend_nest_restore(void) { if (g_pas_scope.npnest <= 0) return; struct pas_pend_frame *fr = &g_pas_scope.pnest[--g_pas_scope.npnest]; int n = fr->nf;
    for (int i = 0; i < n; i++) { g_pas_pend_fields[i] = fr->fields[i]; g_pas_pend_fldptrto[i] = fr->fldptrto[i]; g_pas_pend_fldenum[i] = fr->fldenum[i]; g_pas_pend_fldrec[i] = fr->fldrec[i]; g_pas_pend_fldtypename[i] = fr->fldtypename[i]; g_pas_pend_fldca[i] = fr->fldca[i]; g_pas_pend_fldca_lo[i] = fr->fldca_lo[i]; g_pas_pend_fldca_hi[i] = fr->fldca_hi[i]; g_pas_pend_fldchar[i] = fr->fldchar[i]; g_pas_pend_fldfile[i] = fr->fldfile[i]; g_pas_pend_fldna[i] = fr->fldna[i]; g_pas_pend_fldna_lo[i] = fr->fldna_lo[i]; g_pas_pend_fldna_hi[i] = fr->fldna_hi[i]; g_pas_pend_fldov[i] = fr->fldov[i]; }
    g_pas_pend_nf = n; }
static struct { char *pname; char *rname; } g_pas_ptrtypes[PAS_REC_MAX]; static int g_pas_nptrtype;
static void pas_ptrtype_add(const char *p, const char *r) { if (g_pas_nptrtype < PAS_REC_MAX && p && r) { int k = g_pas_nptrtype++; g_pas_ptrtypes[k].pname = ct_strdup(p); g_pas_ptrtypes[k].rname = ct_strdup(r); } }
static const char *pas_ptrtype_target(const char *p) { if (!p) return NULL; for (int i = 0; i < g_pas_nptrtype; i++) if (g_pas_ptrtypes[i].pname && !strcmp(g_pas_ptrtypes[i].pname, p)) return g_pas_ptrtypes[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_arrptr[PAS_REC_MAX]; static int g_pas_narrptr; static char *g_pas_pend_arr_ptrto;
static void pas_arrptr_add(const char *v, const char *r) { if (g_pas_narrptr < PAS_REC_MAX && v && r) { int k = g_pas_narrptr++; g_pas_arrptr[k].vname = ct_strdup(v); g_pas_arrptr[k].rname = ct_strdup(r); } }
static const char *pas_arrptr_target(const char *v) { if (!v) return NULL; for (int i = g_pas_narrptr - 1; i >= 0; i--) if (g_pas_arrptr[i].vname && !strcmp(g_pas_arrptr[i].vname, v)) return g_pas_arrptr[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_ptrvars[PAS_REC_MAX]; static int g_pas_nptrvar;
static void pas_ptrvar_add(const char *v, const char *r) { if (g_pas_nptrvar < PAS_REC_MAX && v && r) { int k = g_pas_nptrvar++; g_pas_ptrvars[k].vname = ct_strdup(v); g_pas_ptrvars[k].rname = ct_strdup(r); } }
static const char *pas_ptrvar_target(const char *v) { if (!v) return NULL; for (int i = g_pas_nptrvar - 1; i >= 0; i--) if (g_pas_ptrvars[i].vname && !strcmp(g_pas_ptrvars[i].vname, v)) return g_pas_ptrvars[i].rname; return NULL; }
#define PAS_NEST_MAX_PV 16
static int g_pas_pvmarks[PAS_NEST_MAX_PV]; static int g_pas_npvmark;
static int g_pas_rvmarks[PAS_NEST_MAX_PV]; static int g_pas_nrvmark;
static void pas_recvar_mark(void) { if (g_pas_nrvmark < PAS_NEST_MAX_PV) g_pas_rvmarks[g_pas_nrvmark++] = g_pas_nrecvar; }
static void pas_recvar_release(void) { if (g_pas_nrvmark > 0) g_pas_nrecvar = g_pas_rvmarks[--g_pas_nrvmark]; }
static void pas_ptrvar_mark(void) { if (g_pas_npvmark < PAS_NEST_MAX_PV) g_pas_pvmarks[g_pas_npvmark++] = g_pas_nptrvar; }
static void pas_ptrvar_release(void) { if (g_pas_npvmark > 0) g_pas_nptrvar = g_pas_pvmarks[--g_pas_npvmark]; }
static struct { char *pname; char *vnames[16]; char *rnames[16]; int n; struct pas_recvar_s rvsave[16]; int rvn; PNodeList *params; } g_pas_fwdpv[64]; static int g_pas_nfwdpv;
static void pas_fwd_save(const char *pn, PNodeList *params) { if (!pn || g_pas_nfwdpv >= 64 || (g_pas_npvmark == 0 && g_pas_nrvmark == 0)) return; int k = g_pas_nfwdpv++; g_pas_fwdpv[k].pname = ct_strdup(pn); g_pas_fwdpv[k].params = params; g_pas_fwdpv[k].n = 0; g_pas_fwdpv[k].rvn = 0;
    if (g_pas_npvmark > 0) for (int i = g_pas_pvmarks[g_pas_npvmark - 1]; i < g_pas_nptrvar && g_pas_fwdpv[k].n < 16; i++) { g_pas_fwdpv[k].vnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].vname; g_pas_fwdpv[k].rnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].rname; g_pas_fwdpv[k].n++; }
    if (g_pas_nrvmark > 0) for (int i = g_pas_rvmarks[g_pas_nrvmark - 1]; i < g_pas_nrecvar && g_pas_fwdpv[k].rvn < 16; i++) g_pas_fwdpv[k].rvsave[g_pas_fwdpv[k].rvn++] = g_pas_recvars[i]; }
static PNodeList *pas_fwd_params(const char *pn, PNodeList *given) { if (given && given->count > 0) return given; if (pn) for (int i = 0; i < g_pas_nfwdpv; i++) if (g_pas_fwdpv[i].pname && !strcmp(g_pas_fwdpv[i].pname, pn) && g_pas_fwdpv[i].params) return g_pas_fwdpv[i].params; return given; }
static void pas_fwd_restore(const char *pn) { if (!pn) return; for (int i = 0; i < g_pas_nfwdpv; i++) if (g_pas_fwdpv[i].pname && !strcmp(g_pas_fwdpv[i].pname, pn)) { for (int j = 0; j < g_pas_fwdpv[i].n; j++) pas_ptrvar_add(g_pas_fwdpv[i].vnames[j], g_pas_fwdpv[i].rnames[j]);
    for (int j = 0; j < g_pas_fwdpv[i].rvn && g_pas_nrecvar < PAS_REC_MAX; j++) g_pas_recvars[g_pas_nrecvar++] = g_pas_fwdpv[i].rvsave[j]; return; } }
static void pas_pend_reset(void) { g_pas_pend_isarr = 0; g_pas_pend_isbool = 0; g_pas_pend_istfile = 0; g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ptrto = NULL; g_pas_pend_typename = NULL; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; g_pas_pend_enum_max = -1; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_wrap = 0; g_pas_pend_set_hi = -1; g_pas_pend_esz = -1; }
static long long pas_array_decl_low(void) {
    if (!g_pas_pend_isarr && g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0) return pas_arrtype_lo(g_pas_pend_typename);
    return g_pas_pend_sub_low;
}
static long long pas_array_nonparam_low(const char *name) {
    for (int i = 0; name && i < g_pas_narray; i++) if (g_pas_arrays[i].name && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) return g_pas_arrays[i].low;
    return 0;
}
static struct { int lo, hi, n; } g_pas_vcase_stk[32]; static int g_pas_vcase_depth;
static void pas_vcase_push(void) { if (g_pas_vcase_depth < 32) { g_pas_vcase_stk[g_pas_vcase_depth].lo = 0; g_pas_vcase_stk[g_pas_vcase_depth].hi = 0; g_pas_vcase_stk[g_pas_vcase_depth].n = 0; } g_pas_vcase_depth++; }
static void pas_vcase_pop(void) { if (g_pas_vcase_depth > 0) g_pas_vcase_depth--; }
static int pas_field_overlayable(int a, int b) { if (g_pas_pend_fldrec[a] || g_pas_pend_fldrec[b] || g_pas_pend_fldca[a] || g_pas_pend_fldca[b] || g_pas_pend_fldna[a] || g_pas_pend_fldna[b] || g_pas_pend_fldfile[a] || g_pas_pend_fldfile[b] || g_pas_pend_fldptrto[a] || g_pas_pend_fldptrto[b] || g_pas_pend_fldchar[a] != g_pas_pend_fldchar[b]) return 0;
    const char *ta = g_pas_pend_fldtypename[a], *tb = g_pas_pend_fldtypename[b]; if ((ta && (pas_is_realtypename(ta) || pas_is_settype(ta) || !strcmp(ta, "string"))) || (tb && (pas_is_realtypename(tb) || pas_is_settype(tb) || !strcmp(tb, "string")))) return 0; return 1; }
static void pas_vcase_arm_end(int start) { if (g_pas_vcase_depth <= 0 || g_pas_vcase_depth > 32) return; int end = g_pas_pend_nf; int d = g_pas_vcase_depth - 1;
    if (g_pas_vcase_stk[d].n++ == 0) { g_pas_vcase_stk[d].lo = start; g_pas_vcase_stk[d].hi = end; return; }
    for (int j = 0; start + j < end && g_pas_vcase_stk[d].lo + j < g_pas_vcase_stk[d].hi; j++) { int a = start + j; int c = g_pas_pend_fldov[g_pas_vcase_stk[d].lo + j]; if (c != a && pas_field_overlayable(a, c)) g_pas_pend_fldov[a] = c; } }
static void pas_pend_add(const char *f) {
    if (f && g_pas_pend_nf >= PAS_FIELD_MAX) { static int said = 0; if (!said) { said = 1;
        fprintf(stderr, "pascal: record has more than %d fields; field '%s' and any after it are not resolvable -- raise PAS_FIELD_MAX\n", (int)PAS_FIELD_MAX, f); } }
    if (g_pas_pend_nf < PAS_FIELD_MAX && f) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_pend_ptrtarget; g_pas_pend_fldenum[g_pas_pend_nf] = (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) ? g_pas_pend_typename : NULL; g_pas_pend_fldrec[g_pas_pend_nf] = (g_pas_pend_typename && pas_rectype_nf(g_pas_pend_typename) > 0) ? g_pas_pend_typename : NULL; g_pas_pend_fldtypename[g_pas_pend_nf] = g_pas_pend_typename;
    { int _ica = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename));
      long long _lo = g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : (g_pas_pend_typename ? pas_arrtype_lo(g_pas_pend_typename) : 0);
      long long _hi = g_pas_pend_arr_ischar ? g_pas_pend_sub_high : (g_pas_pend_typename ? pas_arrtype_high(g_pas_pend_typename) : -1);
      g_pas_pend_fldca[g_pas_pend_nf] = _ica; g_pas_pend_fldca_lo[g_pas_pend_nf] = _lo; g_pas_pend_fldca_hi[g_pas_pend_nf] = _hi;
      int _iar = g_pas_pend_isarr || (g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0);
      long long _nlo = g_pas_pend_isarr ? g_pas_pend_sub_low : (g_pas_pend_typename ? pas_arrtype_lo(g_pas_pend_typename) : 0);
      long long _nhi = g_pas_pend_isarr ? g_pas_pend_sub_high : (g_pas_pend_typename ? pas_arrtype_high(g_pas_pend_typename) : -1);
      g_pas_pend_fldna[g_pas_pend_nf] = (_iar && !_ica) ? 1 : 0; g_pas_pend_fldna_lo[g_pas_pend_nf] = _nlo; g_pas_pend_fldna_hi[g_pas_pend_nf] = _nhi; }
    g_pas_pend_fldchar[g_pas_pend_nf] = g_pas_pend_ischar;
    g_pas_pend_fldfile[g_pas_pend_nf] = g_pas_pend_istfile || (g_pas_pend_typename && (!strcmp(g_pas_pend_typename, "text") || pas_rectype_has_file(g_pas_pend_typename)));
    g_pas_pend_fldov[g_pas_pend_nf] = g_pas_pend_nf; g_pas_pend_fields[g_pas_pend_nf++] = ct_strdup(f); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ischar = 0; g_pas_pend_ischar = 0; g_pas_pend_isarr = 0; g_pas_pend_istfile = 0; } }
static void pas_rectype_add(const char *tn, int packed) { pas_vt_bind(tn); if (g_pas_nrectype >= PAS_REC_MAX || !tn) return; int k = g_pas_nrectype++; g_pas_rectypes[k].tname = ct_strdup(tn); g_pas_rectypes[k].nf = g_pas_pend_nf; g_pas_rectypes[k].packed = packed; g_pas_rectypes[k].align_mac68k = g_pas_align_mac68k; g_pas_rectypes[k].pack_n = g_pas_pack_records;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_rectypes[k].fldna[i] = g_pas_pend_fldna[i]; g_pas_rectypes[k].fldna_lo[i] = g_pas_pend_fldna_lo[i]; g_pas_rectypes[k].fldna_hi[i] = g_pas_pend_fldna_hi[i]; g_pas_rectypes[k].fields[i] = g_pas_pend_fields[i]; g_pas_rectypes[k].fldptrto[i] = g_pas_pend_fldptrto[i]; g_pas_rectypes[k].fldenum[i] = g_pas_pend_fldenum[i]; g_pas_rectypes[k].fldrec[i] = g_pas_pend_fldrec[i]; g_pas_rectypes[k].fldtypename[i] = g_pas_pend_fldtypename[i]; g_pas_rectypes[k].fldca[i] = g_pas_pend_fldca[i]; g_pas_rectypes[k].fldca_lo[i] = g_pas_pend_fldca_lo[i]; g_pas_rectypes[k].fldca_hi[i] = g_pas_pend_fldca_hi[i]; g_pas_rectypes[k].fldchar[i] = g_pas_pend_fldchar[i]; g_pas_rectypes[k].fldfile[i] = g_pas_pend_fldfile[i]; g_pas_rectypes[k].fldov[i] = g_pas_pend_fldov[i]; } }
static int pas_rectype_to_pend(const char *tn) { if (!tn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, tn)) {
    pas_pend_reset(); for (int j = 0; j < g_pas_rectypes[i].nf; j++) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_rectypes[i].fldptrto[j]; g_pas_pend_fldenum[g_pas_pend_nf] = g_pas_rectypes[i].fldenum[j]; g_pas_pend_fldrec[g_pas_pend_nf] = g_pas_rectypes[i].fldrec[j]; g_pas_pend_fldtypename[g_pas_pend_nf] = g_pas_rectypes[i].fldtypename[j]; g_pas_pend_fldca[g_pas_pend_nf] = g_pas_rectypes[i].fldca[j]; g_pas_pend_fldca_lo[g_pas_pend_nf] = g_pas_rectypes[i].fldca_lo[j]; g_pas_pend_fldca_hi[g_pas_pend_nf] = g_pas_rectypes[i].fldca_hi[j]; g_pas_pend_fldchar[g_pas_pend_nf] = g_pas_rectypes[i].fldchar[j]; g_pas_pend_fldfile[g_pas_pend_nf] = g_pas_rectypes[i].fldfile[j]; g_pas_pend_fldov[g_pas_pend_nf] = g_pas_rectypes[i].fldov[j]; g_pas_pend_fields[g_pas_pend_nf++] = g_pas_rectypes[i].fields[j]; } return 1; } return 0; }
static int pas_rectype_field_index(const char *rn, const char *fn) { if (!rn || !fn) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    for (int j = 0; j < g_pas_rectypes[i].nf; j++) if (g_pas_rectypes[i].fields[j] && !strcmp(g_pas_rectypes[i].fields[j], fn)) return j; return -1; } return -1; }
static int pas_rectype_has_file(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    for (int j = 0; j < g_pas_rectypes[i].nf; j++) if (g_pas_rectypes[i].fldfile[j]) return 1; return 0; } return 0; }
static int pas_rectype_nf(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return g_pas_rectypes[i].nf; return 0; }
static int pas_rectype_field_is_ca(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca[idx]; return 0; } return 0; }
static int pas_rectype_field_is_char(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldchar[idx]; return 0; } return 0; }
static int pas_rectype_field_is_na(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return (idx < g_pas_rectypes[i].nf) ? g_pas_rectypes[i].fldna[idx] : 0; return 0; }
static long long pas_rectype_field_na_lo(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return (idx < g_pas_rectypes[i].nf) ? g_pas_rectypes[i].fldna_lo[idx] : 0; return 0; }
static long long pas_rectype_field_ca_lo(const char *rn, long idx) { if (!rn || idx < 0) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca_lo[idx]; return 0; } return 0; }
static long long pas_rectype_field_ca_hi(const char *rn, long idx) { if (!rn || idx < 0) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) { if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldca_hi[idx]; return -1; } return -1; }
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldptrto[idx]; return NULL; } return NULL; }
static int pas_rectype_layout(const char *rn, long long *size_out, long long *align_out);
static int pas_rectype_field_size_align(const char *rn, int idx, long long *sz, long long *al) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_pas_nrectype; i++) {
        if (!g_pas_rectypes[i].tname || strcmp(g_pas_rectypes[i].tname, rn)) continue;
        if (idx >= g_pas_rectypes[i].nf) return 0;
        long long s, nat;
        if (g_pas_rectypes[i].fldptrto[idx]) { s = 8; nat = 8; }
        else if (g_pas_rectypes[i].fldrec[idx]) { if (!pas_rectype_layout(g_pas_rectypes[i].fldrec[idx], &s, &nat)) return 0; }
        else if (g_pas_rectypes[i].fldca[idx]) { long long cnt = g_pas_rectypes[i].fldca_hi[idx] - g_pas_rectypes[i].fldca_lo[idx] + 1; s = cnt > 0 ? cnt : 1; nat = 1; }
        else if (g_pas_rectypes[i].fldna[idx]) { long long cnt = g_pas_rectypes[i].fldna_hi[idx] - g_pas_rectypes[i].fldna_lo[idx] + 1; if (cnt < 1) cnt = 1; long long esz = 4; if (g_pas_rectypes[i].fldtypename[idx]) pas_sizeof_lookup(g_pas_rectypes[i].fldtypename[idx], &esz); s = cnt * esz; nat = (esz < 1) ? 1 : (esz > 8 ? 8 : esz); }
        else if (g_pas_rectypes[i].fldenum[idx]) { long long ms; if (!pas_enumtype_min_size(g_pas_rectypes[i].fldenum[idx], &ms)) ms = 4; s = ms; nat = (ms < 1) ? 1 : (ms > 8 ? 8 : ms); }
        else if (g_pas_rectypes[i].fldchar[idx]) { s = 1; nat = 1; }
        else if (g_pas_rectypes[i].fldtypename[idx]) { if (!pas_sizeof_lookup(g_pas_rectypes[i].fldtypename[idx], &s)) return 0; nat = (s < 1) ? 1 : (s > 8 ? 8 : s); }
        else return 0;
        *sz = s;
        if (g_pas_rectypes[i].packed) *al = 1;
        else if (g_pas_rectypes[i].align_mac68k) *al = (s == 1) ? 1 : 2;
        else if (g_pas_rectypes[i].pack_n > 0 && nat > g_pas_rectypes[i].pack_n) *al = g_pas_rectypes[i].pack_n;
        else *al = nat;
        return 1;
    }
    return 0;
}
static int pas_rectype_layout(const char *rn, long long *size_out, long long *align_out) {
    if (!rn) return 0;
    for (int i = 0; i < g_pas_nrectype; i++) {
        if (!g_pas_rectypes[i].tname || strcmp(g_pas_rectypes[i].tname, rn)) continue;
        long long off = 0, maxal = 1;
        for (int j = 0; j < g_pas_rectypes[i].nf; j++) {
            long long sz, al;
            if (!pas_rectype_field_size_align(rn, j, &sz, &al)) return 0;
            if (al > maxal) maxal = al;
            if (al > 1) off = ((off + al - 1) / al) * al;
            off += sz;
        }
        long long total;
        if (g_pas_rectypes[i].packed) total = off;
        else if (g_pas_rectypes[i].align_mac68k) total = ((off + 1) / 2) * 2;
        else total = ((off + maxal - 1) / maxal) * maxal;
        if (size_out) *size_out = total;
        if (align_out) *align_out = maxal;
        return 1;
    }
    return 0;
}
static int pas_rectype_total_size(const char *rn, long long *out) { return pas_rectype_layout(rn, out, NULL); }
static int pas_rectype_field_offset(const char *rn, int idx, long long *out) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_pas_nrectype; i++) {
        if (!g_pas_rectypes[i].tname || strcmp(g_pas_rectypes[i].tname, rn)) continue;
        if (idx >= g_pas_rectypes[i].nf) return 0;
        long long off = 0;
        for (int j = 0; j <= idx; j++) {
            long long sz, al;
            if (!pas_rectype_field_size_align(rn, j, &sz, &al)) return 0;
            if (al > 1) off = ((off + al - 1) / al) * al;
            if (j == idx) { *out = off; return 1; }
            off += sz;
        }
        return 0;
    }
    return 0;
}
static const char *pas_rectype_field_enum_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldenum[idx]; return NULL; } return NULL; }
static const char *pas_rectype_field_rectype_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldrec[idx]; return NULL; } return NULL; }
static void pas_recvar_add(const char *vn, int packed) { pas_vt_bind(vn); if (g_pas_nrecvar >= PAS_REC_MAX || !vn || g_pas_pend_nf == 0) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = ct_strdup(vn); g_pas_recvars[k].nf = g_pas_pend_nf; g_pas_recvars[k].packed = packed;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; g_pas_recvars[k].fldchar[i] = g_pas_pend_fldchar[i]; g_pas_recvars[k].fldrec[i] = g_pas_pend_fldrec[i]; g_pas_recvars[k].fldna[i] = g_pas_pend_fldna[i]; g_pas_recvars[k].fldna_lo[i] = g_pas_pend_fldna_lo[i]; g_pas_recvars[k].fldna_hi[i] = g_pas_pend_fldna_hi[i]; g_pas_recvars[k].fldov[i] = g_pas_pend_fldov[i]; } }
static void pas_recvar_add_from_type(const char *vn, const char *tn) { if (!vn || !tn) return; for (int ri = 0; ri < g_pas_nrectype; ri++) { if (!g_pas_rectypes[ri].tname || strcmp(g_pas_rectypes[ri].tname, tn)) continue; if (g_pas_nrecvar >= PAS_REC_MAX) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = ct_strdup(vn); g_pas_recvars[k].nf = g_pas_rectypes[ri].nf; g_pas_recvars[k].packed = g_pas_rectypes[ri].packed; for (int j = 0; j < g_pas_rectypes[ri].nf; j++) { g_pas_recvars[k].fields[j] = g_pas_rectypes[ri].fields[j]; g_pas_recvars[k].fldchar[j] = g_pas_rectypes[ri].fldchar[j]; g_pas_recvars[k].fldrec[j] = g_pas_rectypes[ri].fldrec[j]; g_pas_recvars[k].fldna[j] = g_pas_rectypes[ri].fldna[j]; g_pas_recvars[k].fldna_lo[j] = g_pas_rectypes[ri].fldna_lo[j]; g_pas_recvars[k].fldna_hi[j] = g_pas_rectypes[ri].fldna_hi[j]; g_pas_recvars[k].fldov[j] = g_pas_rectypes[ri].fldov[j]; } return; } }
static int pas_recvar_field_is_char(const char *vn, long idx) { if (!vn || idx < 0) return 0; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return (idx < g_pas_recvars[i].nf) ? g_pas_recvars[i].fldchar[idx] : 0; return 0; }
static int pas_recvar_field_index(const char *vn, const char *fn) { if (!vn || !fn) return -1; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) {
    for (int j = 0; j < g_pas_recvars[i].nf; j++) if (g_pas_recvars[i].fields[j] && !strcmp(g_pas_recvars[i].fields[j], fn)) return j; return -1; } return -1; }
static int pas_rectype_slot(const char *rn, int fi) { if (!rn || fi < 0 || fi >= PAS_FIELD_MAX) return fi; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return fi < g_pas_rectypes[i].nf ? g_pas_rectypes[i].fldov[fi] : fi; return fi; }
static int pas_recvar_slot(const char *vn, int fi) { if (!vn || fi < 0 || fi >= PAS_FIELD_MAX) return fi; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return fi < g_pas_recvars[i].nf ? g_pas_recvars[i].fldov[fi] : fi; return fi; }
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_selector_rectype(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return pas_elem_rectype(e); }
static const char *pas_ptrexpr_target(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) { const char *at = pas_arrptr_target(e->c[0]->v.sval); if (at) return at; }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) { const char *rt = pas_with_sel_rtype(e->c[0]); if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival); }
    return NULL; }
#define PAS_LOCAL_MAX 64
#define PAS_NEST_MAX  16
static struct { const char *names[PAS_LOCAL_MAX]; int n; int decl_level; const char *func_name; } g_pas_lstk[PAS_NEST_MAX]; static int g_pas_ldepth;
static void pas_proc_enter(void) { if (g_pas_ldepth < PAS_NEST_MAX) { g_pas_lstk[g_pas_ldepth].n = 0; g_pas_lstk[g_pas_ldepth].decl_level = g_pas_level; g_pas_lstk[g_pas_ldepth].func_name = NULL; } g_pas_ldepth++; g_pas_level++; }
static void pas_proc_exit(void) { if (g_pas_ldepth > 0) g_pas_ldepth--; if (g_pas_level > 1) g_pas_level--; }
static void pas_local_add(const char *name) { if (g_pas_level < 2 || g_pas_ldepth == 0 || g_pas_ldepth > PAS_NEST_MAX || !name) return; int d = g_pas_ldepth - 1; if (g_pas_lstk[d].n < PAS_LOCAL_MAX) g_pas_lstk[d].names[g_pas_lstk[d].n++] = ct_strdup(name); }
static void pas_curfunc_push(const char *fname) { if (g_pas_ldepth > 0 && g_pas_ldepth <= PAS_NEST_MAX) g_pas_lstk[g_pas_ldepth - 1].func_name = fname; }
static const char *pas_curfunc_name(void) { return (g_pas_ldepth > 0 && g_pas_ldepth <= PAS_NEST_MAX) ? g_pas_lstk[g_pas_ldepth - 1].func_name : NULL; }
static int pas_is_declared_local_here(const char *name) { if (!name || g_pas_ldepth <= 0) return 0; int d = g_pas_ldepth - 1; for (int i = 0; i < g_pas_lstk[d].n; i++) if (g_pas_lstk[d].names[i] && !strcmp(g_pas_lstk[d].names[i], name)) return 1; return 0; }
static struct { char *name; } g_pas_setvars[256]; static int g_pas_nsetvar;
static struct { char *name; long long hi; long long pack; } g_pas_settypes[64]; static int g_pas_nsettype;
static void pas_settype_add(const char *name, long long hi) { if (g_pas_nsettype < 64 && name) { g_pas_settypes[g_pas_nsettype].name = ct_strdup(name); g_pas_settypes[g_pas_nsettype].hi = hi; g_pas_settypes[g_pas_nsettype].pack = g_pas_pack_set_size; g_pas_nsettype++; } }
static int pas_is_settype(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsettype; i++) if (g_pas_settypes[i].name && !strcmp(g_pas_settypes[i].name, name)) return 1; return 0; }
static long long pas_settype_hi(const char *name) { if (!name) return -1; for (int i = 0; i < g_pas_nsettype; i++) if (g_pas_settypes[i].name && !strcmp(g_pas_settypes[i].name, name)) return g_pas_settypes[i].hi; return -1; }
static long long pas_settype_pack(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsettype; i++) if (g_pas_settypes[i].name && !strcmp(g_pas_settypes[i].name, name)) return g_pas_settypes[i].pack; return 0; }
static void pas_setvar_add(const char *name) { if (g_pas_nsetvar < 256 && name) { g_pas_setvars[g_pas_nsetvar++].name = ct_strdup(name); } }
static int pas_is_setvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsetvar; i++) if (g_pas_setvars[i].name && !strcmp(g_pas_setvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_charvars[256]; static int g_pas_ncharvar;
static void pas_charvar_add(const char *name) { if (g_pas_ncharvar < 256 && name) { g_pas_charvars[g_pas_ncharvar++].name = ct_strdup(name); } }
static struct { char *name; } g_pas_singlevars[256]; static int g_pas_nsinglevar;
static void pas_singlevar_add(const char *name) { if (g_pas_nsinglevar < 256 && name) { g_pas_singlevars[g_pas_nsinglevar++].name = ct_strdup(name); } }
static int pas_is_singlevar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsinglevar; i++) if (g_pas_singlevars[i].name && !strcmp(g_pas_singlevars[i].name, name)) return 1; return 0; }
static int pas_is_currencyvar(const char *name) { int u = pas_scope_uid(name); for (int i = g_pas_nscalarvartype - 1; name && i >= 0; i--) if (g_pas_scalarvartype[i].vname && g_pas_scalarvartype[i].uid == u && !strcmp(g_pas_scalarvartype[i].vname, name)) return !strcmp(g_pas_scalarvartype[i].tname, "currency"); return 0; }
static int pas_is_currencyexpr(tree_t *e) { if (!e) return 0; if (e->t == TT_VAR) return e->v.sval && pas_is_currencyvar(e->v.sval); if (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV || e->t == TT_MNS) { for (int i = 0; i < e->n; i++) if (pas_is_currencyexpr(e->c[i])) return 1; } return 0; }
static struct { char *name; } g_pas_pcharvars[256]; static int g_pas_npcharvar;
static void pas_pcharvar_add(const char *name) { if (g_pas_npcharvar < 256 && name) { g_pas_pcharvars[g_pas_npcharvar++].name = ct_strdup(name); } }
static int pas_is_pcharvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_npcharvar; i++) if (g_pas_pcharvars[i].name && !strcmp(g_pas_pcharvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_boolvars[512]; static int g_pas_nboolvar;
static struct { char *name; } g_pas_tfilevars[128]; static int g_pas_ntfilevar;
static struct { char *name; } g_pas_tfiletypes[64]; static int g_pas_ntfiletype;
static void pas_tfilevar_add(const char *name) { if (g_pas_ntfilevar < 128 && name) g_pas_tfilevars[g_pas_ntfilevar++].name = ct_strdup(name); }
static int pas_is_tfilevar(const char *name) { if (!name) return 0; for (int i = g_pas_ntfilevar - 1; i >= 0; i--) if (g_pas_tfilevars[i].name && !strcmp(g_pas_tfilevars[i].name, name)) return 1; return 0; }
static void pas_tfiletype_add(const char *name) { if (g_pas_ntfiletype < 64 && name) g_pas_tfiletypes[g_pas_ntfiletype++].name = ct_strdup(name); }
static int pas_is_tfiletype(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ntfiletype; i++) if (g_pas_tfiletypes[i].name && !strcmp(g_pas_tfiletypes[i].name, name)) return 1; return 0; }
static int pas_is_tfile_node(const tree_t *e) { return e && e->t == TT_VAR && e->v.sval && pas_is_tfilevar(e->v.sval); }
static struct { char *name; } g_pas_booltypes[64]; static int g_pas_nbooltype;
static void pas_boolvar_add(const char *name) { if (g_pas_nboolvar < 512 && name) g_pas_boolvars[g_pas_nboolvar++].name = ct_strdup(name); }
static int pas_is_boolvar(const char *name) { if (!name) return 0; for (int i = g_pas_nboolvar - 1; i >= 0; i--) if (g_pas_boolvars[i].name && !strcmp(g_pas_boolvars[i].name, name)) return 1; return 0; }
static void pas_booltype_add(const char *name) { if (g_pas_nbooltype < 64 && name) g_pas_booltypes[g_pas_nbooltype++].name = ct_strdup(name); }
static int pas_is_booltype(const char *name) { if (!name) return 0; if (!strcmp(name, "boolean")) return 1; for (int i = 0; i < g_pas_nbooltype; i++) if (g_pas_booltypes[i].name && !strcmp(g_pas_booltypes[i].name, name)) return 1; return 0; }
static int pas_is_boolexpr(tree_t *e) { if (!e) return 0; if (pas_pf_rtype(e)) return pas_is_booltype(pas_pf_rtype(e)); switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_NOT: return 1; case TT_CONJ: case TT_ALT: return 1; case TT_MUL: case TT_ADD: return e->n == 2 && pas_is_boolexpr(e->c[0]) && pas_is_boolexpr(e->c[1]); case TT_VAR: return pas_is_boolvar(e->v.sval); case TT_FNC: { const char *fn = (e->n >= 1 && e->c[0]) ? e->c[0]->v.sval : NULL; if (!fn) return 0; if (!strcmp(fn, "__pas_in") || !strcmp(fn, "__pas_eof") || !strcmp(fn, "__pas_eoln") || !strcmp(fn, "__pas_eof_f") || !strcmp(fn, "__pas_eoln_f") || !strcmp(fn, "__pas_feof_t") || !strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne") || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")) return 1; return pas_is_boolvar(fn); } default: return 0; } }
static int pas_is_charvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncharvar; i++) if (g_pas_charvars[i].name && !strcmp(g_pas_charvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_filevars[256]; static int g_pas_nfilevar;
static void pas_filevar_add(const char *name) { if (g_pas_nfilevar < 256 && name) { for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return; g_pas_filevars[g_pas_nfilevar++].name = ct_strdup(name); } }
static int pas_is_filevar(const char *name) { if (!name) return 0; if (!strcmp(name, "input") || !strcmp(name, "output")) return 1; for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return 1; return 0; }
static int pas_is_filevar_elem(tree_t *e) { long long _ah; return e && e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_filevar(e->c[0]->v.sval) && pas_array_high_get(e->c[0]->v.sval, &_ah); }
static int pas_is_stdstream(const char *name) { return name && (!strcmp(name, "input") || !strcmp(name, "output")); }
static char *g_pas_hdrfiles[32]; static int g_pas_nhdrfile;
static int pas_is_hdrfile(const char *n) { if (!n) return 0; for (int i = 0; i < g_pas_nhdrfile; i++) if (g_pas_hdrfiles[i] && !strcmp(g_pas_hdrfiles[i], n)) return 1; return 0; }
static struct { char *name; long long lo; int uid; } g_pas_chararrs[256]; static int g_pas_nchararr;
static void pas_chararr_add2(const char *name, long long lo) { if (g_pas_nchararr < 256 && name) { g_pas_chararrs[g_pas_nchararr].name = ct_strdup(name); g_pas_chararrs[g_pas_nchararr].lo = lo; g_pas_chararrs[g_pas_nchararr].uid = pas_scope_uid(name); g_pas_nchararr++; } }
static void pas_chararr_add(const char *name) { pas_chararr_add2(name, 0); }
static int pas_chararr_row(const char *name) { if (!name) return -1; int u = pas_scope_uid(name); for (int i = g_pas_nchararr - 1; i >= 0; i--) if (g_pas_chararrs[i].name && !strcmp(g_pas_chararrs[i].name, name) && g_pas_chararrs[i].uid == u) return i; return -1; }
static long long pas_chararr_lo(const char *name) { int i = pas_chararr_row(name); return i < 0 ? 0 : g_pas_chararrs[i].lo; }
static int pas_is_chararr(const char *name) { return pas_chararr_row(name) >= 0; }
static tree_t *g_pas_cafield_marks[2048]; static long long g_pas_cafield_lo[2048]; static long long g_pas_cafield_hi[2048]; static int g_pas_ncafield = 0;
static void pas_cafield_mark_add(tree_t *e, long long lo, long long hi) { if (g_pas_ncafield < 2048 && e) { g_pas_cafield_marks[g_pas_ncafield] = e; g_pas_cafield_lo[g_pas_ncafield] = lo; g_pas_cafield_hi[g_pas_ncafield] = hi; g_pas_ncafield++; } }
static int pas_is_cafield(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return 1; return 0; }
static long long pas_cafield_lo_get(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return g_pas_cafield_lo[i]; return 0; }
static long long pas_cafield_hi_get(const tree_t *e) { for (int i = 0; i < g_pas_ncafield; i++) if (g_pas_cafield_marks[i] == (tree_t *)e) return g_pas_cafield_hi[i]; return -1; }
static tree_t *g_pas_nafield_marks[2048]; static long long g_pas_nafield_lo[2048]; static int g_pas_nnafield = 0;
static void pas_nafield_mark_add(tree_t *e, long long lo) { if (g_pas_nnafield < 2048 && e) { g_pas_nafield_marks[g_pas_nnafield] = e; g_pas_nafield_lo[g_pas_nnafield] = lo; g_pas_nnafield++; } }
static int pas_is_nafield(const tree_t *e) { for (int i = 0; i < g_pas_nnafield; i++) if (g_pas_nafield_marks[i] == (tree_t *)e) return 1; return 0; }
static long long pas_nafield_lo_get(const tree_t *e) { for (int i = 0; i < g_pas_nnafield; i++) if (g_pas_nafield_marks[i] == (tree_t *)e) return g_pas_nafield_lo[i]; return 0; }
static tree_t *g_pas_cvfield_marks[2048]; static int g_pas_ncvfield = 0;
static void pas_cvfield_mark_add(tree_t *e) { if (g_pas_ncvfield < 2048 && e) g_pas_cvfield_marks[g_pas_ncvfield++] = e; }
static int pas_is_cvfield(const tree_t *e) { for (int i = 0; i < g_pas_ncvfield; i++) if (g_pas_cvfield_marks[i] == (tree_t *)e) return 1; return 0; }
static int pas_ca_is_read(const tree_t *e) { return e && e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_ca_unpack"); }
static struct { char *name; long long lo; } g_pas_strarrs[128]; static int g_pas_nstrarr;
static void pas_strarr_add2(const char *name, long long lo) { if (g_pas_nstrarr < 128 && name) { g_pas_strarrs[g_pas_nstrarr].name = ct_strdup(name); g_pas_strarrs[g_pas_nstrarr].lo = lo; g_pas_nstrarr++; } }
static void pas_strarr_add(const char *name) { pas_strarr_add2(name, 1); }
static int pas_is_strarr(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nstrarr; i++) if (g_pas_strarrs[i].name && !strcmp(g_pas_strarrs[i].name, name)) return 1; return 0; }
static long long pas_strarr_lo(const char *name) { if (!name) return 1; for (int i = 0; i < g_pas_nstrarr; i++) if (g_pas_strarrs[i].name && !strcmp(g_pas_strarrs[i].name, name)) return g_pas_strarrs[i].lo; return 1; }
static struct { char *aname; char *rname; int nf; char *fields[32]; char *fldenum[32]; char *fldtypename[32]; int fldchar[32]; char *fldrec[32]; int fldna[32]; long long fldna_lo[32]; long long fldna_hi[32]; int fldov[32]; } g_pas_arrrecs[128];
static int g_pas_narrrec;
static void pas_arrrec_add(const char *a, const char *r, int nf) { if (g_pas_narrrec < 128 && a && nf > 0) { g_pas_arrrecs[g_pas_narrrec].aname = ct_strdup(a); g_pas_arrrecs[g_pas_narrrec].rname = r ? ct_strdup(r) : NULL; g_pas_arrrecs[g_pas_narrrec].nf = nf; for (int _i = 0; _i < nf && _i < 32; _i++) { g_pas_arrrecs[g_pas_narrrec].fields[_i] = g_pas_pend_fields[_i] ? ct_strdup(g_pas_pend_fields[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldenum[_i] = g_pas_pend_fldenum[_i] ? ct_strdup(g_pas_pend_fldenum[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldchar[_i] = g_pas_pend_fldchar[_i]; g_pas_arrrecs[g_pas_narrrec].fldtypename[_i] = g_pas_pend_fldtypename[_i] ? ct_strdup(g_pas_pend_fldtypename[_i]) : NULL;
    g_pas_arrrecs[g_pas_narrrec].fldrec[_i] = g_pas_pend_fldrec[_i]; g_pas_arrrecs[g_pas_narrrec].fldna[_i] = g_pas_pend_fldna[_i]; g_pas_arrrecs[g_pas_narrrec].fldna_lo[_i] = g_pas_pend_fldna_lo[_i];
    g_pas_arrrecs[g_pas_narrrec].fldna_hi[_i] = g_pas_pend_fldna_hi[_i]; g_pas_arrrecs[g_pas_narrrec].fldov[_i] = g_pas_pend_fldov[_i]; } g_pas_narrrec++; } }
static void pas_arrrec_add_from_type(const char *a, const char *tn) { if (!a || !tn) return; pas_pend_nest_save(); if (pas_rectype_to_pend(tn)) pas_arrrec_add(a, tn, g_pas_pend_nf); pas_pend_nest_restore(); }
static const char *pas_arrrec_field_enum(const char *a, long idx) { if (!a || idx < 0 || idx >= 32) return NULL; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (idx < g_pas_arrrecs[i].nf) return g_pas_arrrecs[i].fldenum[idx]; return NULL; } return NULL; }
static int pas_arrrec_field_is_char(const char *a, long idx) { if (!a || idx < 0 || idx >= 32) return 0; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (idx < g_pas_arrrecs[i].nf) return g_pas_arrrecs[i].fldchar[idx]; return 0; } return 0; }
static int pas_arrrec_find(const char *a, const char **rn) { if (!a) return 0; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { if (rn) *rn = g_pas_arrrecs[i].rname; return g_pas_arrrecs[i].nf; } return 0; }
static int pas_arrrec_field_index(const char *a, const char *fn) { if (!a || !fn) return -1; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) { for (int j = 0; j < g_pas_arrrecs[i].nf && j < 32; j++) if (g_pas_arrrecs[i].fields[j] && !strcmp(g_pas_arrrecs[i].fields[j], fn)) return j; } return -1; }
static int pas_arrrec_slot(const char *a, long fi) { if (!a || fi < 0 || fi >= 32) return (int)fi; for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, a)) return fi < g_pas_arrrecs[i].nf ? g_pas_arrrecs[i].fldov[fi] : (int)fi; return (int)fi; }
#define PAS_FIELD_IDX_MARK 0x46494458
static tree_t *pas_arrrec_flatten(tree_t *idxsel, long long fi) {
    tree_t *e = ast_node_new(TT_IDX); ast_push(e, idxsel); ast_push(e, ilit((idxsel && idxsel->t == TT_IDX && idxsel->n == 2 && idxsel->c[0] && idxsel->c[0]->t == TT_VAR && idxsel->c[0]->v.sval) ? pas_arrrec_slot(idxsel->c[0]->v.sval, fi) : fi)); e->slen = PAS_FIELD_IDX_MARK; return e;
}
static tree_t *mk_chr_wrap(tree_t *e) { tree_t *r = ast_node_new(TT_FNC); ast_push(r, leaf_s(TT_VAR, "__pas_chr")); ast_push(r, e); return r; }
static int pas_buf_is_char(tree_t *e) { return !(!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") && e->n >= 2 && e->c[1] && e->c[1]->t == TT_VAR && pas_tfcomp_nonchar(e->c[1]->v.sval)); }
static int pas_is_charexpr(tree_t *e) { if (!e) return 0; if (pas_pf_rtype(e)) return !strcmp(pas_pf_rtype(e), "char"); if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_tbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_pchar_deref"))) return pas_buf_is_char(e); if (e->t == TT_FNC && e->n == 1 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_getbufch")) return 1; if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1; if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[1]->t == TT_ILIT && pas_recvar_field_is_char(e->c[0]->v.sval, e->c[1]->v.ival)) return 1; if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_charvar(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_chararr(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_var_string_kind(e->c[0]->v.sval) || pas_is_pcharvar(e->c[0]->v.sval))) return 1; if (e->t == TT_IDX && e->n >= 2 && e->c[0] && pas_is_cafield(e->c[0])) return 1; if (pas_is_cvfield(e)) return 1; return 0; }
static int pas_is_strtyped(tree_t *e) { if (!e) return 0; if (e->t == TT_QLIT) return 1; if (pas_ca_is_read(e)) return 1; if (e->t == TT_VAR && e->v.sval && pas_is_chararr(e->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_is_chararr(e->c[0]->v.sval) || pas_is_strarr(e->c[0]->v.sval))) return 1; return 0; }
static tree_t *pas_alpha_wrap(tree_t *x) { if (x && x->t == TT_VAR && x->v.sval && pas_is_chararr(x->v.sval)) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(f, x); ast_push(f, ilit(pas_chararr_lo(x->v.sval))); return f; } if (x && x->t == TT_IDX && x->n >= 2 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && pas_is_strarr(x->c[0]->v.sval)) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(f, x); ast_push(f, ilit(pas_strarr_lo(x->c[0]->v.sval))); return f; } return x; }
static int pas_func_returns_stringish(const char *fname) {
    const char *t = fname ? pas_scalarvartype_get(fname) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "shortstring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_is_strval(tree_t *e) { if (!e) return 0; if (e->t == TT_QLIT) return 1; if (pas_ca_is_read(e)) return 1; if (e->t == TT_VAR && e->v.sval && (pas_is_chararr(e->v.sval) || pas_is_strarr(e->v.sval))) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_strarr(e->c[0]->v.sval)) return 1; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_func_returns_stringish(e->c[0]->v.sval)) return 1; return 0; }
static tree_t *mk_set_bin(const char *fn, tree_t *a, tree_t *b);
static tree_t *pas_rel(tree_e tt, tree_t *a, tree_t *b) { int lex = pas_is_strval(a) || pas_is_strval(b); if (lex || pas_is_strtyped(a) || pas_is_strtyped(b)) { a = pas_alpha_wrap(a); b = pas_alpha_wrap(b); } return lex ? bin(tt, mk_set_bin("__pas_strcmp", a, b), ilit(0)) : bin(tt, a, b); }
static int pas_is_setexpr(tree_t *e);
static tree_t *pas_rel_or_set(tree_e tt, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : pas_rel(tt, a, b); }
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return ct_strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
#define PAS_WITH_MAX 8
static struct { tree_t *sel; const char *rtype; } g_with_stk[PAS_WITH_MAX]; static int g_with_depth;
static tree_t *pas_tree_clone(tree_t *e) { if (!e) return NULL; tree_t *c = ast_node_new(e->t); c->v = e->v; if (e->t == TT_IDX && e->slen == PAS_FIELD_IDX_MARK) c->slen = e->slen; if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = ct_strdup(e->v.sval); for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i])); return c; }
static const char *pas_with_sel_rtype(tree_t *sel) { if (!sel) return NULL; { const char *_er = pas_elem_rectype(sel); if (_er) return _er; } if (sel->t == TT_VAR && sel->v.sval) { for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, sel->v.sval)) { const char *rt = NULL; for (int j = 0; j < g_pas_nrectype; j++) { int match = 1; if (!g_pas_rectypes[j].tname) continue; if (g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { match = 0; break; } if (match) { rt = g_pas_rectypes[j].tname; break; } } return rt; } } if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) { const char *ptn = pas_ptrexpr_target(sel->c[1]); return ptn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval) { const char *_arn = NULL; if (pas_arrrec_find(sel->c[0]->v.sval, &_arn) > 0 && _arn) return _arn; }
    if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && sel->c[1] && sel->c[1]->t == TT_ILIT) {
        const char *_vfr = pas_recvar_field_rectype(sel->c[0]->v.sval, (long)sel->c[1]->v.ival); if (_vfr) return _vfr; }
    if (sel->t == TT_IDX && sel->n >= 2 && sel->c[1] && sel->c[1]->t == TT_ILIT) { const char *bt = pas_with_sel_rtype(sel->c[0]); if (bt) {
        const char *_fr = pas_rectype_field_rectype_by_index(bt, sel->c[1]->v.ival); if (_fr) return _fr; return pas_rectype_field_ptrto_by_index(bt, sel->c[1]->v.ival); } } return NULL; }
static int pas_with_field_index(const char *rtype, const char *fname) { return pas_rectype_field_index(rtype, fname); }
static int pas_with_recvar_field(const char *vname, const char *fname) { return pas_recvar_field_index(vname, fname); }
static void pas_with_push(tree_t *sel) { if (g_with_depth >= PAS_WITH_MAX || !sel) return; const char *rt = pas_with_sel_rtype(sel); g_with_stk[g_with_depth].sel = sel; g_with_stk[g_with_depth].rtype = rt; g_with_depth++; }
static void pas_with_pop(void) { if (g_with_depth > 0) g_with_depth--; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_elem_rectype(tree_t *e) {
    if (!e || e->t != TT_IDX || e->n != 2 || e->slen == PAS_FIELD_IDX_MARK || !e->c[0]) return NULL;
    tree_t *a = e->c[0];
    if (a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_deref")) {
        const char *pt = pas_ptrexpr_target(a->c[1]); return (pt && pas_arrtype_high(pt) >= 0 && pas_rectype_nf(pt) > 0) ? pt : NULL; }
    if (a->t != TT_IDX || a->n != 2 || !a->c[0] || !a->c[1] || a->c[1]->t != TT_ILIT) return NULL;
    long fi = (long)a->c[1]->v.ival; tree_t *b = a->c[0];
    if (b->t == TT_VAR && b->v.sval) { for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, b->v.sval))
            return (fi >= 0 && fi < g_pas_recvars[i].nf && g_pas_recvars[i].fldna[fi]) ? g_pas_recvars[i].fldrec[fi] : NULL; return NULL; }
    { const char *bt = pas_with_sel_rtype(b); return (bt && pas_rectype_field_is_na(bt, fi)) ? pas_rectype_field_rectype_by_index(bt, fi) : NULL; }
}
static int pas_rectype_is_packed(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return g_pas_rectypes[i].packed; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_recvar_is_packed(const char *vn) { if (!vn) return 0; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return g_pas_recvars[i].packed; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_recvar_field_rectype(const char *vn, long idx) { if (!vn || idx < 0) return NULL; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return (idx < g_pas_recvars[i].nf) ? g_pas_recvars[i].fldrec[idx] : NULL; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_designator_rectype(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) { long idx = (long)e->c[1]->v.ival; tree_t *b = e->c[0];
        if (b->t == TT_VAR && b->v.sval) return pas_recvar_field_rectype(b->v.sval, idx);
        const char *bt = pas_designator_rectype(b); return bt ? pas_rectype_field_rectype_by_index(bt, idx) : NULL; }
    if (e->t == TT_FIELD && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_VAR && e->c[1]->v.sval) { tree_t *b = e->c[0]; const char *fn = e->c[1]->v.sval;
        if (b->t == TT_VAR && b->v.sval) { int fi = pas_recvar_field_index(b->v.sval, fn); return (fi >= 0) ? pas_recvar_field_rectype(b->v.sval, fi) : NULL; }
        const char *bt = pas_designator_rectype(b); if (!bt) return NULL; int fi = pas_rectype_field_index(bt, fn); return (fi >= 0) ? pas_rectype_field_rectype_by_index(bt, fi) : NULL; }
    return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_is_aggregate_designator(tree_t *e) { if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { long long _ah; return pas_array_high_get(e->v.sval, &_ah); }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_rectype_nf(pas_ptrexpr_target(e->c[1])) > 0;
    if (e->t == TT_IDX && e->n == 2 && e->slen != PAS_FIELD_IDX_MARK && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_arrrec_find(e->c[0]->v.sval, NULL) > 0) return 1;
    if (pas_elem_rectype(e)) return 1;
    if (e->t != TT_IDX || e->n != 2 || !e->c[0] || !e->c[1] || e->c[1]->t != TT_ILIT) return 0;
    if (pas_designator_rectype(e)) return 1;
    { long idx = (long)e->c[1]->v.ival; tree_t *b = e->c[0];
      if (b->t == TT_VAR && b->v.sval) { for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, b->v.sval))
              return (idx >= 0 && idx < g_pas_recvars[i].nf) ? (g_pas_recvars[i].fldna[idx] || g_pas_recvars[i].fldrec[idx] != NULL) : 0; return 0; }
      { const char *bt = pas_with_sel_rtype(b); return bt ? (pas_rectype_field_is_na(bt, idx) || pas_rectype_field_rectype_by_index(bt, idx) != NULL) : 0; } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_actual_is_packed_component(tree_t *a) { if (!a || a->n < 2 || !a->c[0] || !a->c[1]) return 0;
    if (a->t == TT_IDX && a->c[0]->t == TT_IDX && a->c[0]->n >= 2 && a->c[0]->c[0] && a->c[0]->c[1] && a->c[0]->c[1]->t == TT_ILIT && a->c[1]->t != TT_ILIT) {
        tree_t *b = a->c[0]->c[0]; long i = (long)a->c[0]->c[1]->v.ival; const char *rt = NULL;
        if (b->t == TT_VAR && b->v.sval) { if (pas_recvar_is_packed(b->v.sval)) return 1; rt = pas_recvar_field_rectype(b->v.sval, i); }
        if (!rt) { const char *bt = pas_designator_rectype(b); rt = bt ? pas_rectype_field_rectype_by_index(bt, i) : NULL; }
        return (rt && pas_rectype_is_packed(rt)) ? 1 : 0; }
    if (!((a->t == TT_IDX && a->c[1]->t == TT_ILIT) || (a->t == TT_FIELD && a->c[1]->t == TT_VAR))) return 0;
    tree_t *base = a->c[0];
    if (base->t == TT_VAR && base->v.sval) return pas_recvar_is_packed(base->v.sval);
    const char *rt = pas_designator_rectype(base);
    return (rt && pas_rectype_is_packed(rt)) ? 1 : 0; }
static const char *pas_field_typename(tree_t *e) { if (!e || e->t != TT_IDX || e->n != 2 || !e->c[0] || !e->c[1]) return NULL; const char *rt = NULL; long long fi = -1;
    if (e->slen == PAS_FIELD_IDX_MARK && e->c[0]->t == TT_IDX && e->c[0]->n == 2 && e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR && e->c[1]->t == TT_ILIT) { fi = e->c[1]->v.ival;
        for (int i = 0; i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, e->c[0]->c[0]->v.sval)) return (fi >= 0 && fi < 32) ? g_pas_arrrecs[i].fldtypename[fi] : NULL; return NULL; }
    if (e->c[1]->t == TT_ILIT) { rt = pas_selector_rectype(e->c[0]); if (!rt) rt = pas_with_sel_rtype(e->c[0]); fi = e->c[1]->v.ival; }
    if (!rt || fi < 0) return NULL;
    for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rt)) return (fi < g_pas_rectypes[i].nf) ? g_pas_rectypes[i].fldtypename[fi] : NULL;
    return NULL; }
static int pas_is_setexpr(tree_t *e) { if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) return pas_is_setvar(e->v.sval);
    if (e->t == TT_IDX && pas_is_settype(pas_field_typename(e))) return 1;
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval) { const char *f = e->c[0]->v.sval;
        return !strcmp(f, "__pas_set") || !strcmp(f, "__pas_setrange") || !strcmp(f, "__pas_setuni") || !strcmp(f, "__pas_setint") || !strcmp(f, "__pas_setdif"); }
    return 0; }
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *pas_rdiv(tree_t *a, tree_t *b) { return bin(TT_DIV, bin(TT_MUL, a, flit(1.0)), b); }
static int pas_var_unsigned(const char *name) { if (!name) return 0; const char *tn = pas_scalarvartype_get(name); long long lo, hi; if (tn && (pas_is_unsigned_inttypename(tn) || (pas_subtype_high(tn) >= 0 && pas_subtype_low(tn) >= 0))) return 1; return pas_subvar_get(name, &lo, &hi) && lo >= 0; }
static int pas_unsigned_sum(tree_t *e, int *nv) { if (!e) return 0; if (e->t == TT_ILIT) return e->v.ival >= 0; if (e->t == TT_VAR) { if (!pas_var_unsigned(e->v.sval)) return 0; (*nv)++; return 1; } return e->t == TT_ADD && e->n == 2 && pas_unsigned_sum(e->c[0], nv) && pas_unsigned_sum(e->c[1], nv); }
static int pas_is_qword(tree_t *e) { if (!e) return 0; if (e->t == TT_ILIT) return e->v.ival < 0; if (e->t != TT_VAR || !e->v.sval) return 0; const char *tn = pas_scalarvartype_get(e->v.sval); for (int g = 0; tn && g < 8; g++) { if (!strcmp(tn, "qword") || !strcmp(tn, "uint64")) return 1; const char *al = pas_typealias_get(tn); if (!al || !strcmp(al, tn)) break; tn = al; } return 0; }
static tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) {
    if (pas_is_setexpr(a) || pas_is_setexpr(b)) return mk_set_bin(setfn, a, b);
    if (ak == TT_SUB && (g_pas_zerobased_strings & 2) && a && a->t == TT_ADD) { int nv = 0; if (pas_unsigned_sum(a, &nv) && nv >= 2) return mk_fnc1("__pas_qchk", bin(ak, a, b)); }
    return bin(ak, a, b);
}
static struct pas_vt *pas_vt_find_name(const char *nm) { if (!nm) return NULL; for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 2 && g_pas_vt[i]->name && !strcmp(g_pas_vt[i]->name, nm)) return g_pas_vt[i]; return NULL; }
static void pas_vt_mark(tree_t *e, tree_t *base, const char *rt, int fi) {
    if (!e || !base || fi < 0 || fi >= PAS_FIELD_MAX) return;
    const char *nm = rt ? rt : pas_with_sel_rtype(base); struct pas_vt *t = pas_vt_find_name(nm);
    if (!t && base->t == TT_VAR && base->v.sval) t = pas_vt_find_name(base->v.sval);
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_FNC && base->n == 2 && base->c[0] && base->c[0]->v.sval && !strcmp(base->c[0]->v.sval, "__pas_deref")) { struct pas_vt *m = pas_vt_new(5); m->node = e; m->tagexpr = pas_tree_clone(base->c[1]); return; }
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_VAR && base->v.sval) { struct pas_vt *m = pas_vt_new(7); m->node = e; m->name = ct_strdup(base->v.sval); return; }
    if (!t || !t->fhas[fi] || t->tagfi < 0) return;
    if (base->t != TT_VAR || !base->v.sval) return;
    { int asg = 0; for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 6 && g_pas_vt[i]->name && !strcmp(g_pas_vt[i]->name, base->v.sval)) { asg = 1; break; } if (!asg) return; }
    int slot = (rt || (nm && pas_vt_find_name(nm))) ? pas_rectype_slot(nm, t->tagfi) : pas_recvar_slot(base->v.sval, t->tagfi);
    tree_t *te = ast_node_new(TT_IDX); ast_push(te, pas_tree_clone(base)); ast_push(te, ilit(slot));
    struct pas_vt *m = pas_vt_new(4); m->node = e; m->tagexpr = te; m->mask = t->fmask[fi];
}
static tree_t *pas_vt_check_of(tree_t *sel) { if (!sel) return NULL; for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 4 && g_pas_vt[i]->node == sel) { tree_t *c = ast_node_new(TT_FNC); ast_push(c, leaf_s(TT_VAR, "__pas_vcheck")); ast_push(c, pas_tree_clone(g_pas_vt[i]->tagexpr)); ast_push(c, ilit(g_pas_vt[i]->mask)); return c; } return NULL; }
static tree_t *pas_vt_wrap_read(tree_t *sel) { tree_t *c = pas_vt_check_of(sel); if (!c) return sel; tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, c); ast_push(q, sel); return q; }
static tree_t *pas_vt_wrap_stmt(tree_t *sel, tree_t *rhs, tree_t *st) { for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 7 && g_pas_vt[i]->node == sel) { struct pas_vt *a = pas_vt_new(6); a->name = ct_strdup(g_pas_vt[i]->name); break; } for (int i = g_pas_nvt - 1; i >= 0; i--) if (g_pas_vt[i]->state == 5 && g_pas_vt[i]->node == sel && rhs) { tree_t *c = ast_node_new(TT_FNC); ast_push(c, leaf_s(TT_VAR, "__pas_tagset_chk")); ast_push(c, pas_tree_clone(g_pas_vt[i]->tagexpr)); ast_push(c, pas_tree_clone(sel)); ast_push(c, pas_tree_clone(rhs)); tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, c); ast_push(q, st); return q; } tree_t *c = pas_vt_check_of(sel); if (!c) return st; tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, c); ast_push(q, st); return q; }
static tree_t *pas_nested_field_resolve(tree_t *base, const char *fld) {
    const char *_brt = pas_with_sel_rtype(base);
    if (_brt) { int _nfi = pas_rectype_field_index(_brt, fld); if (_nfi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, base); ast_push(e, ilit(pas_rectype_slot(_brt, _nfi))); const char *_fe = pas_rectype_field_enum_by_index(_brt, _nfi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); }
        if (pas_rectype_field_is_ca(_brt, _nfi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_brt, _nfi), pas_rectype_field_ca_hi(_brt, _nfi));
        if (pas_rectype_field_is_na(_brt, _nfi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_brt, _nfi));
        if (pas_rectype_field_is_char(_brt, _nfi)) pas_cvfield_mark_add(e);
        return e; } }
    return bin(TT_FIELD, base, leaf_s(TT_VAR, fld));
}
static int pas_is_realtypename(const char *t) {
    return t && (!strcmp(t, "real") || !strcmp(t, "single") || !strcmp(t, "double") || !strcmp(t, "extended") || !strcmp(t, "comp"));
}
static int pas_is_int_typename(const char *t) {
    return t && (!strcmp(t, "integer") || !strcmp(t, "longint") || !strcmp(t, "shortint") || !strcmp(t, "byte") || !strcmp(t, "word") ||
                 !strcmp(t, "cardinal") || !strcmp(t, "int64") || !strcmp(t, "qword") || !strcmp(t, "smallint") || !strcmp(t, "longword"));
}
static int pas_var_typename_is(const char *name, int (*pred)(const char *)) {
    const char *t = name ? pas_scalarvartype_get(name) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        if (pred(t)) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_var_is_real(const char *name) { return pas_var_typename_is(name, pas_is_realtypename); }
static int pas_var_every_decl_real(const char *name) {
    int n = 0;
    for (int i = 0; name && i < g_pas_nscalarvartype; i++) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, name)) {
        const char *t = g_pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8 && !pas_is_realtypename(t); guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || !pas_is_realtypename(t)) return 0;
        n++; }
    return n > 0;
}
static void pas_ordinal_fn_arg(const char *name, PNodeList *args) {
    if (!name || !args || args->count < 1 || pas_is_func(name) || pas_is_proc(name)) return;
    int is_chr = !strcmp(name, "chr");
    if (!is_chr && strcmp(name, "ord") && strcmp(name, "succ") && strcmp(name, "pred")) return;
    tree_t *a = args->items[0];
    if (!a || !(a->t == TT_FLIT || (a->t == TT_VAR && a->v.sval && pas_var_every_decl_real(a->v.sval)))) return;
    if (a->t == TT_FLIT) fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the real constant %g, and %s requires an expression of %s\n",
                                 name, a->v.dval, name, is_chr ? "integer-type" : "an ordinal-type");
    else fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the variable '%s' of type real, and %s requires an expression of %s\n",
                 name, a->v.sval, name, is_chr ? "integer-type" : "an ordinal-type");
    g_pas_iso_errors++;
}
static char pas_expr_lit_class(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_FLIT) return 'r';
    if (e->t == TT_QLIT) return (e->v.sval && strlen(e->v.sval) == 1) ? 'c' : 's';
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_chrlit")) return 'c';
    if (e->t == TT_EQ && e->n == 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_ILIT && e->c[1]->t == TT_ILIT) return 'b';
    return 0;
}
static char pas_var_decl_class(const char *name) {
    char k = 0;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return 0;
    for (int i = 0; name && i < g_pas_nscalarvartype; i++) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, name)) {
        const char *t = g_pas_scalarvartype[i].tname; char c = 0;
        for (int guard = 0; t && guard < 8 && !c; guard++) {
            if (pas_is_int_typename(t)) c = 'i'; else if (pas_is_realtypename(t)) c = 'r'; else if (!strcmp(t, "char")) c = 'c';
            else if (pas_is_booltype(t)) c = 'b'; else if (pas_enumtype_high(t) >= 0) c = 'e';
            else { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; } }
        if (!c || (k && k != c)) return 0;
        k = c; }
    return k;
}
static int pas_ord_var_bounds(const char *name, long long *lo, long long *hi) {
    int n = 0; long long l = 0, h = -1;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return 0;
    for (int i = 0; i < g_pas_nscalarvartype; i++) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, name)) {
        const char *t = g_pas_scalarvartype[i].tname; long long cl = 0, ch = -1; int ok = 0;
        for (int guard = 0; t && guard < 8 && !ok; guard++) {
            if (!strcmp(t, "integer")) { cl = -2147483647LL; ch = 2147483647LL; ok = 1; }
            else if (!strcmp(t, "char")) { cl = 0; ch = 255; ok = 1; }
            else if (pas_is_booltype(t)) { cl = 0; ch = 1; ok = 1; }
            else if (pas_enumtype_high(t) >= 0 && pas_subtype_high(t) < 0) { cl = 0; ch = pas_enumtype_high(t); ok = 1; }
            else { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; } }
        if (!ok || (n && (cl != l || ch != h))) return 0;
        l = cl; h = ch; n++; }
    if (!n) return 0;
    *lo = l; *hi = h; return 1;
}
static const char *pas_class_name(char k) {
    switch (k) { case 'i': return "integer-type"; case 'r': return "real-type"; case 'c': return "char-type"; case 'b': return "Boolean-type";
                 case 'e': return "an enumerated-type"; case 's': return "a string-type"; default: return "?"; }
}
static void pas_sign_operand(tree_t *e, char op) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (!k || k == 'i' || k == 'r') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.2.2 violation: the monadic %c is applied to an operand of %s, and it takes only integer-type or real-type (table 4)\n", op, pas_class_name(k));
    g_pas_iso_errors++;
}
static void pas_set_member_ordinal(tree_t *e) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (k != 'r' && k != 's') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.1 violation: a member-designator of a set-constructor is of %s, and the type of its expressions shall be an ordinal-type\n", pas_class_name(k));
    g_pas_iso_errors++;
}
static void pas_variant_values(tree_t *e, long long *v, int *n, int cap) {
    if (!e) return;
    if (e->t == TT_ADD && e->n == 2) { pas_variant_values(e->c[0], v, n, cap); pas_variant_values(e->c[1], v, n, cap); return; }
    if (e->t == TT_EQ && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT && *n < cap) v[(*n)++] = e->c[1]->v.ival;
}
static void pas_variant_constants_in_tag_type(const char *tag_type, PNodeList *arms) {
    const char *t = tag_type; long long lo = 0, hi = -1;
    for (int guard = 0; t && guard < 8 && hi < lo; guard++) {
        if (pas_is_booltype(t)) { lo = 0; hi = 1; }
        else if (pas_subtype_high(t) >= 0) { lo = pas_subtype_low(t); hi = pas_subtype_high(t); }
        else if (pas_enumtype_high(t) >= 0) { lo = 0; hi = pas_enumtype_high(t); }
        else { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; } }
    if (hi < lo || hi - lo >= 4096 || !arms) return;
    long long v[4096]; int n = 0; unsigned char seen[4096]; memset(seen, 0, sizeof seen);
    for (int i = 0; i < arms->count; i++) pas_variant_values(arms->items[i], v, &n, 4096);
    for (int i = 0; i < n; i++) {
        if (v[i] < lo || v[i] > hi) { fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: a case-constant of the variant-part denotes %lld, which is not a value of its tag-type %s (%lld..%lld)\n", v[i], tag_type, lo, hi); g_pas_iso_errors++; return; }
        if (seen[v[i] - lo]++) { fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the value %lld of the tag-type %s is denoted by more than one case-constant of the variant-part\n", v[i], tag_type); g_pas_iso_errors++; return; } }
    for (long long x = lo; x <= hi; x++) if (!seen[x - lo]) {
        fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the case-constants of the variant-part do not denote the value %lld of its tag-type %s, and they shall denote every value of it\n", x, tag_type);
        g_pas_iso_errors++; return; }
}
static void pas_type_not_self_applied(const char *name) {
    int self = name && g_pas_pend_typename && !strcmp(g_pas_pend_typename, name) && !g_pas_pend_ptrtarget;
    for (int i = 0; name && !self && i < g_pas_pend_nf && i < PAS_FIELD_MAX; i++)
        if (g_pas_pend_fldtypename[i] && !strcmp(g_pas_pend_fldtypename[i], name) && !g_pas_pend_fldptrto[i]) self = 1;
    if (!self) return;
    fprintf(stderr, "pascal: ISO 7185 6.4.1 violation: the type-denoter of the type-definition of '%s' contains an applied occurrence of '%s' outside the domain-type of a pointer-type\n", name, name);
    g_pas_iso_errors++;
}
static int pas_typealias_is_char(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) return 0; if (!strcmp(al, "char")) return 1; t = al; }
    return 0;
}
static int pas_class_compatible(char to, char from) { return to == from || (to == 'r' && from == 'i'); }
static void pas_value_compat(const char *vn, tree_t *rhs, const char *clause, const char *what) {
    char to = pas_var_decl_class(vn), from = pas_expr_lit_class(rhs);
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: %s '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n",
            clause, what, vn, pas_class_name(from), pas_class_name(to));
    g_pas_iso_errors++;
}
static int pas_boolfam_width(const char *tn0) { const char *tn = tn0; int guard = 0; while (tn) { if (!strcmp(tn, "bytebool")) return 1; if (!strcmp(tn, "wordbool")) return 2; if (!strcmp(tn, "longbool")) return 4; if (!strcmp(tn, "qwordbool")) return 8; const char *al = pas_typealias_get(tn); if (!al || !strcmp(al, tn) || guard++ >= 8) break; tn = al; } return 0; }
static int pas_var_is_boolfam(const char *name) { return name ? pas_boolfam_width(pas_scalarvartype_get(name)) > 0 : 0; }
static char *pas_pchar_off_name(const char *n) { size_t l = strlen(n) + 12; char *h = (char *)ct_zalloc(l, 1); snprintf(h, l, "__pas_po_%s", n); return h; }
static int pas_pchar_castname(tree_t *e) { if (!e || e->t != TT_FNC || e->n != 2 || !e->c[0] || !e->c[0]->v.sval) return 0; const char *f = e->c[0]->v.sval; return !strcmp(f, "pchar") || !strcmp(f, "pwidechar") || !strcmp(f, "pansichar"); }
static tree_t *pas_pchar_assign(tree_t *sel, tree_t *rhs) {
    if (!sel || sel->t != TT_VAR || !sel->v.sval || !pas_is_pcharvar(sel->v.sval) || !rhs) return NULL;
    tree_t *in = pas_pchar_castname(rhs) ? rhs->c[1] : rhs; tree_t *base = NULL, *off = NULL;
    if (in->t == TT_IDX && in->n == 2 && in->c[0] && in->c[0]->t == TT_VAR && in->c[0]->v.sval && pas_var_string_kind(in->c[0]->v.sval)) { base = in->c[0]; off = bin(TT_SUB, in->c[1], ilit(1)); }
    else if (in->t == TT_VAR && in->v.sval && pas_is_pcharvar(in->v.sval)) { base = in; off = leaf_s(TT_VAR, pas_pchar_off_name(in->v.sval)); }
    else if (in->t == TT_VAR && in->v.sval && pas_var_string_kind(in->v.sval)) { base = in; off = ilit(0); }
    else if (in->t == TT_QLIT) { base = in; off = ilit(0); }
    if (!base) return NULL;
    tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, bin(TT_ASSIGN, sel, pas_tree_clone(base))); ast_push(q, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_pchar_off_name(sel->v.sval)), off)); return q;
}
static tree_t *pas_pchar_deref(tree_t *p) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, p); ast_push(e, bin(TT_ADD, leaf_s(TT_VAR, pas_pchar_off_name(p->v.sval)), ilit(1))); return e; }
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    { tree_t *_pc = pas_pchar_assign(sel, rhs); if (_pc) return _pc; }
    { const char *_abn = pas_selector_base_name(sel); if (_abn) pas_assigned_add(_abn); }
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_QLIT && pas_var_string_kind(sel->v.sval) && pas_is_charexpr(rhs)) rhs = mk_chr_wrap(rhs);
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_FLIT && pas_var_is_real(sel->v.sval)) rhs = bin(TT_ADD, rhs, flit(0.0));
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs) {
        int _dbf = pas_var_is_boolfam(sel->v.sval);
        int _sbf = (rhs->t == TT_VAR && rhs->v.sval) ? pas_var_is_boolfam(rhs->v.sval) : 0;
        if (_dbf && !_sbf && pas_is_boolexpr(rhs)) rhs = bin(TT_SUB, ilit(0), rhs);
        else if (!_dbf && _sbf) rhs = bin(TT_NE, rhs, ilit(0));
    }
    if (pas_is_aggregate_designator(rhs)) rhs = mk_fnc1("__pas_arr_copy", rhs);
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0]) && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_FNC && sel->c[0]->c[0]->n >= 2
        && sel->c[0]->c[0]->c[0] && sel->c[0]->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_idx_set"));
        ast_push(e, sel->c[0]->c[0]->c[1]); ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0])
        && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_VAR) {
        tree_t *base = sel->c[0]->c[0];
        tree_t *upd = ast_node_new(TT_FNC);
        ast_push(upd, leaf_s(TT_VAR, "__pas_field_idx_update"));
        ast_push(upd, pas_tree_clone(base)); ast_push(upd, pas_tree_clone(sel->c[0]->c[1])); ast_push(upd, sel->c[1]); ast_push(upd, rhs);
        return bin(TT_ASSIGN, base, upd);
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
    }
    return bin(TT_ASSIGN, sel, rhs);
}
static tree_t *mk_ident(const char *name) {
    if (pas_pf_is_formal(name) && pas_pf_lookup(name)->sig[0] == 'F') return pas_pf_callthrough(name, NULL);
    if (name && !strcmp(name, "result") && !pas_is_declared_local_here(name)) {
        const char *cf = pas_curfunc_name();
        if (cf) return leaf_s(TT_VAR, cf);
    }
    if (name && !strcmp(name, "true"))  return bin(TT_EQ, ilit(1), ilit(1));
    if (name && !strcmp(name, "false")) return bin(TT_EQ, ilit(0), ilit(1));
    if (name && !strcmp(name, "nil"))   return ilit(0);
    if (name && !strcmp(name, "hinstance")) return ilit(0);
    if (name && !strcmp(name, "eof"))   return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln"))  return mk_fnc0("__pas_eoln");
    long long cv; if (pas_const_get(name, &cv)) return pas_is_charvar(name) ? mk_fnc1("__pas_chrlit", ilit(cv)) : ilit(cv);
    if (name && !strcmp(name, "maxint") && !pas_scalarvartype_get(name) && !pas_is_func(name)) return ilit(2147483647);
    double rv; if (pas_rconst_get(name, &rv)) return flit(rv);
    const char *sv = pas_sconst_get(name); if (sv) return leaf_s(TT_QLIT, sv);
    if (pas_is_func(name)) return mk_call(name, NULL);
    for (int wi = g_with_depth - 1; wi >= 0; wi--) {
        tree_t *wsel = g_with_stk[wi].sel; const char *rt = g_with_stk[wi].rtype;
        int fi = -1;
        if (rt) fi = pas_with_field_index(rt, name);
        if (fi < 0 && wsel && wsel->t == TT_VAR && wsel->v.sval) fi = pas_with_recvar_field(wsel->v.sval, name);
        if (fi < 0 && wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && wsel->c[0]->v.sval) fi = pas_arrrec_field_index(wsel->c[0]->v.sval, name);
        if (fi >= 0) { if (wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && pas_arrrec_find(wsel->c[0]->v.sval, NULL) > 0) { tree_t *_af = pas_arrrec_flatten(pas_tree_clone(wsel), fi); if (pas_arrrec_field_is_char(wsel->c[0]->v.sval, fi)) pas_cvfield_mark_add(_af); return _af; } tree_t *e = ast_node_new(TT_IDX); ast_push(e, pas_tree_clone(wsel)); ast_push(e, ilit(rt ? pas_rectype_slot(rt, fi) : (wsel && wsel->t == TT_VAR && wsel->v.sval) ? pas_recvar_slot(wsel->v.sval, fi) : fi)); { const char *_crt = rt ? rt : pas_with_sel_rtype(wsel); if (_crt && pas_rectype_field_is_ca(_crt, fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_crt, fi), pas_rectype_field_ca_hi(_crt, fi)); if (_crt && pas_rectype_field_is_char(_crt, fi)) pas_cvfield_mark_add(e); } return e; }
    }
    pas_scope_require(name);
    return leaf_s(TT_VAR, name);
}
static void pas_scope_applied(const char *name) {
    for (int i = g_pas_scope.n - 1; name && i >= 0; i--) if (g_pas_scope.defs[i].name && !strcmp(g_pas_scope.defs[i].name, name)) {
        if (g_pas_scope.defs[i].depth < g_pas_scope.depth) pas_scope_push(&g_pas_scope.apl, &g_pas_scope.napl, &g_pas_scope.capl, name);
        return; }
}
static void pas_scope_defined_after_use(const char *name) {
    if (g_pas_recbody_depth > 0) return;
    for (int i = g_pas_scope.napl - 1; i >= 0 && g_pas_scope.apl[i].depth == g_pas_scope.depth; i--) if (!strcmp(g_pas_scope.apl[i].name, name)) {
        fprintf(stderr, "pascal: ISO 7185 6.2.2.9 violation: '%s' is used in this block before its defining-point in this block, which shall precede every applied occurrence\n", name);
        g_pas_iso_errors++; return; }
}
static void pas_scope_require(const char *name) {
    pas_scope_applied(name);
    if (pas_scope_has(name)) return;
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the identifier '%s' has no defining-point -- it is declared nowhere in a region enclosing this use\n", name);
    g_pas_iso_errors++;
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static void pas_not_a_word_symbol(const char *n) {
    const char *ws[] = { "and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", "function", "goto", "if", "in", "label",
                         "mod", "nil", "not", "of", "or", "packed", "procedure", "program", "record", "repeat", "set", "then", "to", "type", "until", "var",
                         "while", "with" };
    for (size_t i = 0; n && i < sizeof ws / sizeof ws[0]; i++) if (!strcmp(n, ws[i])) {
        fprintf(stderr, "pascal: ISO 7185 6.1.2 violation: '%s' is a word-symbol and cannot be defined as an identifier\n", n); g_pas_iso_errors++; return; }
}
static void pas_label_in_range(long long v) {
    if (v < 0 || v > 9999) { fprintf(stderr, "pascal: ISO 7185 6.1.6 violation: the label %lld is not in the closed interval 0 to 9999\n", v); g_pas_iso_errors++; }
}
static void pas_label_declare(long long v) { char b[24]; snprintf(b, sizeof b, "%lld", v); pas_scope_push(&g_pas_scope.lab, &g_pas_scope.nlab, &g_pas_scope.clab, ct_strdup(b)); }
static PasDef *pas_label_find(long long v) {
    char b[24]; snprintf(b, sizeof b, "%lld", v);
    for (int i = g_pas_scope.nlab - 1; i >= 0; i--) if (!strcmp(g_pas_scope.lab[i].name, b)) return &g_pas_scope.lab[i];
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the label %lld has no defining-point -- no label-declaration-part of this block or of an enclosing block declares it\n", v);
    g_pas_iso_errors++; return NULL;
}
static void pas_label_prefix(long long v) { PasDef *d = pas_label_find(v); if (d && d->depth == g_pas_scope.depth) d->formal++; }
static void pas_labels_close(void) {
    while (g_pas_scope.nlab > 0 && g_pas_scope.lab[g_pas_scope.nlab - 1].depth >= g_pas_scope.depth) { PasDef *d = &g_pas_scope.lab[--g_pas_scope.nlab];
        if (d->formal == 1) continue;
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: the label %s prefixes %d statements of the block that declares it, which shall closest-contain exactly one\n", d->name, d->formal);
        g_pas_iso_errors++; }
}
static void pas_real_is_not_ordinal(double v) {
    if (g_pas_case_depth > 0) fprintf(stderr, "pascal: ISO 7185 6.8.3.5 violation: the case-constant %g is of type real, which is not an ordinal-type\n", v);
    else fprintf(stderr, "pascal: ISO 7185 6.4.2.4 violation: the constant %g is of type real, and a subrange bound or a variant's case-constant (6.4.3.3) shall be of an ordinal-type\n", v);
    g_pas_iso_errors++;
}
static void pas_program_params_distinct(PNodeList *ids) {
    for (int i = 0; ids && i < ids->count; i++) for (int j = 0; j < i; j++)
        if (ids->items[i] && ids->items[j] && ids->items[i]->v.sval && ids->items[j]->v.sval && !strcmp(ids->items[i]->v.sval, ids->items[j]->v.sval)) {
            fprintf(stderr, "pascal: ISO 7185 6.10 violation: the program-parameter '%s' appears more than once in the program-heading\n", ids->items[i]->v.sval);
            g_pas_iso_errors++; break; }
}
static PNodeList *pas_var_names_add(PNodeList *seen, PNodeList *ids) {
    if (ids) for (int i = 0; i < ids->count; i++) { const char *n = ids->items[i] ? ids->items[i]->v.sval : NULL; if (!n) continue;
        for (int j = 0; j < seen->count; j++) if (seen->items[j] && seen->items[j]->v.sval && !strcmp(seen->items[j]->v.sval, n)) {
            fprintf(stderr, "pascal: ISO 7185 6.2.2.7 violation: the identifier '%s' has two defining-points in one variable-declaration-part\n", n); g_pas_iso_errors++; break; }
        pnl_push(seen, ids->items[i]); }
    return seen;
}
static void pas_for_const_bounds(const char *cv, tree_t *from, tree_t *to, int down) {
    if (!cv || !from || !to || from->t != TT_ILIT || to->t != TT_ILIT) return;
    long long lo = 0, hi = -1, a = from->v.ival, b = to->v.ival; int n = 0;
    for (int i = 0; i < g_pas_nsubvar; i++) if (g_pas_subvars[i].name && !strcmp(g_pas_subvars[i].name, cv)) {
        if (n && (g_pas_subvars[i].low != lo || g_pas_subvars[i].high != hi)) return;
        lo = g_pas_subvars[i].low; hi = g_pas_subvars[i].high; n++; }
    if (!n) return;
    for (int i = 0; i < g_pas_nscalarvartype; i++)
        if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, cv) && pas_subtype_high(g_pas_scalarvartype[i].tname) < 0) return;
    if (down ? a < b : a > b) return;
    if (a >= lo && a <= hi && b >= lo && b <= hi) return;
    fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the for-statement over '%s' (of type %lld..%lld) runs from %lld to %lld, and a value outside the"
                    " control-variable's type is not assignment-compatible with it\n", cv, lo, hi, a, b);
    g_pas_iso_errors++;
}
static tree_t *pas_cond(tree_t *e) { return (pas_is_rel(e) || (e && (e->t == TT_CONJ || e->t == TT_ALT || e->t == TT_NOT))) ? e : bin(TT_NE, e, ilit(0)); }
static const char *pas_cond_var_nonbool_type(tree_t *e) {
    if (!e || e->t != TT_VAR || !e->v.sval || pas_is_boolvar(e->v.sval)) return NULL;
    const char *found = NULL;
    for (int i = 0; i < g_pas_nscalarvartype; i++) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, e->v.sval)) {
        const char *t = g_pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || pas_is_booltype(t) || !(pas_enumtype_high(t) >= 0 || pas_is_int_typename(t) || pas_is_realtypename(t) || !strcmp(t, "char"))) return NULL;
        found = g_pas_scalarvartype[i].tname; }
    return found;
}
static tree_t *pas_cond_bool(tree_t *e, const char *stmt, const char *clause) {
    const char *t = pas_cond_var_nonbool_type(e);
    if (t) { fprintf(stderr, "pascal: ISO 7185 %s violation: the Boolean-expression of %s is the variable '%s' of type %s, which is not the required-type Boolean\n",
                     clause, stmt, e->v.sval, t); g_pas_iso_errors++; }
    return pas_cond(e);
}
static tree_t *pas_bool(tree_t *e) { return e; }
static tree_t *pas_flip_rel(tree_t *e) {
    switch (e->t) { case TT_LT: e->t = TT_GE; break; case TT_GE: e->t = TT_LT; break; case TT_LE: e->t = TT_GT; break;
                    case TT_GT: e->t = TT_LE; break; case TT_EQ: e->t = TT_NE; break; case TT_NE: e->t = TT_EQ; break;
                    case TT_CONJ: case TT_ALT: { tree_t *n = ast_node_new(TT_NOT); ast_push(n, e); return n; } default: break; }
    return e;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high) {
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = ct_strdup(s ? s : ""); return mk_fnc3("__pas_ca_encode", q, ilit(lo < 0 ? 0 : lo), ilit(high));
}
static tree_t *pas_str_padded(const char *s, long long n) {
    size_t sl = s ? strlen(s) : 0; size_t len = (n > (long long) sl) ? (size_t) n : sl; char *buf = (char *) ct_alloc(len + 1);
    for (size_t k = 0; k < len; k++) buf[k] = (k < sl) ? s[k] : ' '; buf[len] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}
static int pas_array_is_pure_num(const char *name) {
    if (!name) return 0;
    if (pas_is_chararr(name)) return 0;
    if (pas_arrrec_find(name, NULL) > 0) return 0;
    for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name)) return 0;
    return 1;
}
static int pas_array_is_real(const char *name) {
    if (pas_array_is_pure_num(name)) return 1;
    if (!name || pas_is_chararr(name) || pas_enumarr_get(name) || pas_arrrec_find(name, NULL) <= 0) return 0;
    for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name)) return 0;
    return 1;
}
static long long pas_range_fit_size(long long lo, long long hi) {
    if (lo >= 0) { if (hi <= 255) return 1; if (hi <= 65535) return 2; return 4; }
    if (lo >= -128 && hi <= 127) return 1;
    if (lo >= -32768 && hi <= 32767) return 2;
    return 4;
}
static int pas_sizeof_builtin_size(const char *n, long long *out) {
    if (!n) return 0;
    static const struct { const char *n; long long sz; } T[] = {
        {"integer",4},{"longint",4},{"shortint",1},{"byte",1},{"word",2},{"smallint",2},
        {"cardinal",4},{"longword",4},{"int64",8},{"qword",8},{"single",4},{"real",8},
        {"double",8},{"extended",10},{"comp",8},{"char",1},{"widechar",1},
        {"boolean",1},{"bytebool",1},{"wordbool",2},{"longbool",4},{"qwordbool",8},{"pointer",8},
        {"ptruint",8},{"ptrint",8},{"int8",1},{"int16",2},{"int32",4},{"uint8",1},
        {"uint16",2},{"uint32",4},{"uint64",8},{"nativeint",8},{"nativeuint",8},
        {"codepointer",8},
    };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *out = T[i].sz; return 1; }
    return 0;
}
static int pas_sizeof_lookup(const char *name, long long *out) {
    if (!name) return 0;
    if (pas_sizeof_builtin_size(name, out)) return 1;
    if (pas_arrtype_high(name) >= 0 && pas_tsz_get(name, out)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_sizeof_lookup(al, out); }
    { long long eh = pas_enumtype_high(name); if (eh >= 0) { long long ms; if (!pas_enumtype_min_size(name, &ms)) ms = 4; long long fit = pas_range_fit_size(0, eh); *out = fit > ms ? fit : ms; return 1; } }
    if (pas_is_settype(name)) { long long pk = pas_settype_pack(name); if (pk > 0) { *out = pk; return 1; } long long sh = pas_settype_hi(name); *out = (sh >= 0 && sh <= 31) ? 4 : 32; return 1; }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *out = pas_range_fit_size(pas_subtype_low(name), sh); return 1; } }
    { long long lo, hi; if (pas_subvar_get(name, &lo, &hi)) { *out = pas_range_fit_size(lo, hi); return 1; } }
    if (pas_rectype_nf(name) > 0 && pas_rectype_total_size(name, out)) return 1;
    for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name)) {
        const char *rt = pas_with_sel_rtype(leaf_s(TT_VAR, name));
        return (rt && pas_rectype_total_size(rt, out)) ? 1 : 0;
    }
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return pas_tsz_get(name, out);
    if (pas_is_charvar(name)) { *out = 1; return 1; }
    if (pas_is_boolvar(name)) { *out = 1; return 1; }
    if (pas_ptrvar_target(name)) { *out = 8; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_sizeof_lookup(tn, out); }
    return pas_tsz_get(name, out);
}
static int pas_ordinal_builtin_bound(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    static const struct { const char *n; long long lo; long long hi; } T[] = {
        {"byte",0,255},{"shortint",-128,127},{"word",0,65535},{"smallint",-32768,32767},
        {"longword",0,4294967295LL},{"cardinal",0,4294967295LL},{"uint32",0,4294967295LL},
        {"longint",-2147483648LL,2147483647LL},{"integer",-2147483648LL,2147483647LL},
        {"int32",-2147483648LL,2147483647LL},{"int8",-128,127},{"uint8",0,255},
        {"int16",-32768,32767},{"uint16",0,65535},
        {"int64",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL},
        {"qword",0,-1},{"uint64",0,-1},{"nativeint",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL},
        {"nativeuint",0,-1},{"char",0,255},{"widechar",0,255},{"boolean",0,1},{"bytebool",0,255},
    };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *lo = T[i].lo; *hi = T[i].hi; return 1; }
    return 0;
}
static int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi) {
    if (!name) return 0;
    if (pas_ordinal_builtin_bound(name, lo, hi)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_ordinal_bound_lookup(al, lo, hi); }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *lo = pas_subtype_low(name); *hi = sh; return 1; } }
    if (pas_subvar_get(name, lo, hi)) return 1;
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return 0;
    for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name)) return 0;
    if (pas_is_charvar(name)) { *lo = 0; *hi = 255; return 1; }
    if (pas_is_boolvar(name)) { *lo = 0; *hi = 1; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_ordinal_bound_lookup(tn, lo, hi); }
    return 0;
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
static tree_t *mk_rec_init_type_d(const char *rt, int depth) {
    for (int i = 0; rt && i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rt))
        return mk_rec_block(g_pas_rectypes[i].nf, g_pas_rectypes[i].fldrec, g_pas_rectypes[i].fldna, g_pas_rectypes[i].fldna_lo, g_pas_rectypes[i].fldna_hi, depth);
    return mk_fnc1("arr_make", ilit(0));
}
static tree_t *mk_rec_init_type(const char *rt) { return mk_rec_init_type_d(rt, 0); }
static tree_t *pas_new_target_init(const char *rt) {
    long long ah = pas_arrtype_high(rt);
    if (ah >= 0 && !pas_arrtype_ischar(rt) && pas_arrtype_ncols(rt) < 0)
        return pas_rectype_nf(rt) > 0 ? mk_fnc3("__pas_arr_of", ilit(pas_arrtype_lo(rt)), ilit(ah), mk_rec_init_type(rt)) : mk_fnc2("arr_make", ilit(pas_arrtype_lo(rt)), ilit(ah));
    return pas_rectype_nf(rt) > 0 ? mk_rec_init_type(rt) : ilit(0);
}
static tree_t *mk_array_init(const char *name, long long high) {
    for (int i = g_pas_nrecvar - 1; name && i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, name))
        return mk_rec_block(g_pas_recvars[i].nf, g_pas_recvars[i].fldrec, g_pas_recvars[i].fldna, g_pas_recvars[i].fldna_lo, g_pas_recvars[i].fldna_hi, 0);
    for (int i = 0; name && i < g_pas_narrrec; i++) if (g_pas_arrrecs[i].aname && !strcmp(g_pas_arrrecs[i].aname, name)) {
        tree_t *proto = (g_pas_arrrecs[i].rname && pas_rectype_nf(g_pas_arrrecs[i].rname) == g_pas_arrrecs[i].nf) ? mk_rec_init_type(g_pas_arrrecs[i].rname)
                      : mk_rec_block(g_pas_arrrecs[i].nf, g_pas_arrrecs[i].fldrec, g_pas_arrrecs[i].fldna, g_pas_arrrecs[i].fldna_lo, g_pas_arrrecs[i].fldna_hi, 0);
        return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), proto); }
    if (!pas_array_is_real(name)) return mk_fnc2("arr_make", ilit(pas_is_chararr(name) ? pas_chararr_lo(name) : 0), ilit(high));
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !g_pas_arrays[i].is_param && !strcmp(g_pas_arrays[i].name, name)) {
        if (g_pas_arrays[i].ncols < 0 && g_pas_arrays[i].low != 0 && g_pas_arrays[i].low <= high) return mk_fnc2("arr_make", ilit(g_pas_arrays[i].low), ilit(high));
        break; }
    return mk_fnc1("arr_make", ilit(high));
}
static long long pas_index_bound(long long v, int want_hi) {
    long long lo = 0, hi = -1;
    if (g_pas_pend_sub_high >= 0 && g_pas_pend_sub_high == v) { lo = g_pas_pend_sub_low; hi = v; }
    else if (g_pas_pend_typename && pas_subtype_high(g_pas_pend_typename) >= 0 && pas_subtype_high(g_pas_pend_typename) == v) { lo = pas_subtype_low(g_pas_pend_typename); hi = v; }
    else if (v >= 0) { lo = 0; hi = v; }
    else if (g_pas_pend_typename) { long long bl, bh; if (pas_ordinal_bound_lookup(g_pas_pend_typename, &bl, &bh) && bh >= bl && bh - bl < 65536) { lo = bl; hi = bh; } }
    return want_hi ? hi : lo;
}
static long long pas_decl_part_order(long long prev, long long cur) {
    const char *nm[] = { "", "label-declaration-part", "constant-definition-part", "type-definition-part", "variable-declaration-part", "procedure-and-function-declaration-part" };
    if (cur < prev) { fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block's %s shall precede its %s\n", nm[cur], nm[prev]); g_pas_iso_errors++; }
    else if (cur == prev && cur < 5) { fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block has more than one %s\n", nm[cur]); g_pas_iso_errors++; }
    return cur > prev ? cur : prev;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_sig_value_type(const char *sig, int idx, int var_too) {
    if (!sig || strlen(sig) < 3) return NULL;
    const char *p = sig + 2; int at = 0;
    while (*p && *p != ')') {
        const char *q = p; int dep = 0;
        while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
        char k = *p; int cnt = (k == 'v' || k == 'r') ? atoi(p + 1) : 1;
        if (idx < at + cnt) { const char *c = (k == 'v' || (var_too && k == 'r')) ? memchr(p, ':', (size_t)(q - p)) : NULL; if (!c) return NULL;
            char *t = (char *)ct_zalloc(1, (size_t)(q - c)); memcpy(t, c + 1, (size_t)(q - c - 1)); return t; }
        at += cnt; p = (*q == ';') ? q + 1 : q;
    }
    return NULL;
}
static void pas_formal_subranges(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) { tree_t *id = params->items[i]; if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1); if (t && pas_subtype_high(t) >= 0) pas_subvar_add(id->v.sval, pas_subtype_low(t), pas_subtype_high(t)); }
}
static void pas_formal_chararrs(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) { tree_t *id = params->items[i]; if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 0); if (t && pas_arrtype_ischar(t)) pas_chararr_add2(id->v.sval, pas_arrtype_lo(t)); }
}
static void pas_formal_strtypes(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) { tree_t *id = params->items[i]; if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1); if (t && (pas_string_type_kind(t) || !strcmp(t, "currency"))) pas_scalarvartype_add(id->v.sval, t); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_value_actuals_check(const char *callee, const PasDef *cd, PNodeList *args) {
    for (int i = 0; i + 1 < args->count; i += 2) {
        const char *t = pas_sig_value_type(cd->sig, i / 2, 0); tree_t *a = args->items[i]; const char *at = NULL; const char *rt = t ? NULL : pas_sig_value_type(cd->sig, i / 2, 1); long long _lo, _hi;
        if (rt && pas_subtype_high(rt) < 0 && strcmp(rt, "char") && !pas_is_booltype(rt) && a && a->t == TT_VAR && a->v.sval && !pas_is_setvar(a->v.sval) && pas_subvar_get(a->v.sval, &_lo, &_hi)) {
            fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: the actual-parameter %d of '%s' is a variable of the subrange type %lld..%lld, which is not the type %s of its variable formal-parameter\n",
                    i / 2 + 1, callee, _lo, _hi, rt);
            g_pas_iso_errors++; continue; }
        if (!t || !a || (strcmp(t, "integer") && strcmp(t, "real") && strcmp(t, "char") && strcmp(t, "boolean"))) continue;
        if (a->t == TT_FNC && a->n == 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) at = "char";
        else if (a->t == TT_FLIT) at = "real";
        else if (a->t == TT_QLIT && a->v.sval && strlen(a->v.sval) > 1) at = "string";
        else if (a->t == TT_ILIT && !strcmp(t, "char")) at = "non-char ordinal";
        if (!at || !strcmp(at, t)) continue;
        fprintf(stderr, "pascal: ISO 7185 6.6.3.2 violation: the actual-parameter %d of '%s' is a %s value, which is not assignment-compatible with the type %s of its value formal-parameter\n",
                i / 2 + 1, callee, at, t);
        g_pas_iso_errors++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_pf_cat3(const char *a, const char *b, const char *c) {
    size_t la = a ? strlen(a) : 0, lb = b ? strlen(b) : 0, lc = c ? strlen(c) : 0; char *s = (char *)ct_zalloc(1, la + lb + lc + 1);
    if (la) memcpy(s, a, la); if (lb) memcpy(s + la, b, lb); if (lc) memcpy(s + la + lb, c, lc); return s;
}
static const char *pas_pf_canon(const char *t) { for (int g = 0; t && g < 16 && pas_subtype_high(t) < 0; g++) { const char *a = pas_typealias_get(t); if (!a) break; t = a; } return t ? t : ""; }
static char *pas_pf_sect(char k, int n, const char *t) { char nb[24]; snprintf(nb, sizeof nb, "%c%d:", k, n); return pas_pf_cat3(nb, pas_pf_canon(t), NULL); }
static char *pas_pf_sig(char k, const char *params, const char *rtype) { char *s = pas_pf_cat3(k == 'F' ? "F(" : "P(", params, ")"); return k == 'F' ? pas_pf_cat3(s, ":", pas_pf_canon(rtype)) : s; }
static const char *pas_pf_env_name(const char *formal, int k) { char b[16]; snprintf(b, sizeof b, "__pas_pe%d_", k); return pas_pf_cat3(b, formal, NULL); }
static void pas_pf_pend(const char *formal, const char *sig) { pas_scope_push(&g_pas_scope.pend, &g_pas_scope.npend, &g_pas_scope.cpend, formal)->sig = sig; }
static void pas_pf_section(char k, PNodeList *ids, const char *t) { pas_pf_pend(NULL, pas_pf_sect(k, ids ? ids->count : 0, t)); }
static PNodeList *pas_pf_formal(const char *name, const char *sig) {
    pas_not_a_word_symbol(name); pas_pf_pend(NULL, sig); pas_pf_pend(name, sig);
    PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, name)); for (int k = 1; k <= 3; k++) pnl_push(l, leaf_s(TT_VAR, pas_pf_env_name(name, k))); return l;
}
static const char *pas_pf_heading(const char *name, PNodeList *params, char k, const char *rtype) {
    if ((!params || params->count == 0) && name) for (int i = g_pas_scope.nfwd - 1; i >= 0; i--)
        if (g_pas_scope.fwd[i].name && g_pas_scope.fwd[i].sig && !strcmp(g_pas_scope.fwd[i].name, name)) { g_pas_scope.npend = 0; return g_pas_scope.fwd[i].sig; }
    const char *ps = "";
    for (int i = 0; i < g_pas_scope.npend; i++) { PasDef *d = &g_pas_scope.pend[i];
        if (!d->name) ps = pas_pf_cat3(ps, *ps ? ";" : "", d->sig);
        else if (name) { PasDef *f = pas_scope_push(&g_pas_scope.fsig, &g_pas_scope.nfsig, &g_pas_scope.cfsig, d->name); f->sig = d->sig; f->owner = name; } }
    g_pas_scope.npend = 0;
    return pas_pf_sig(k, ps, rtype);
}
static void pas_pf_define_routine(const char *name, const char *sig) {
    if (name) { PasDef *d = pas_scope_push(&g_pas_scope.defs, &g_pas_scope.n, &g_pas_scope.cap, name); d->sig = sig; d->rid = ++g_pas_scope.nrid; }
}
static const PasDef *pas_pf_lookup(const char *name) {
    for (int i = g_pas_scope.n - 1; name && i >= 0; i--) if (g_pas_scope.defs[i].name && !strcmp(g_pas_scope.defs[i].name, name)) return g_pas_scope.defs[i].sig ? &g_pas_scope.defs[i] : NULL;
    return NULL;
}
static int pas_pf_is_formal(const char *name) { const PasDef *d = pas_pf_lookup(name); return d && d->formal; }
static char pas_pf_param(const char *sig, int idx, const char **fsig) {
    *fsig = NULL; if (!sig || strlen(sig) < 3) return 0;
    const char *p = sig + 2; int at = 0;
    while (*p && *p != ')') {
        const char *q = p; int dep = 0;
        while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
        char k = *p; int cnt = (k == 'v' || k == 'r') ? atoi(p + 1) : 1;
        if (idx < at + cnt) { if (k == 'P' || k == 'F') { char *f = (char *)ct_zalloc(1, (size_t)(q - p) + 1); memcpy(f, p, (size_t)(q - p)); *fsig = f; } return k; }
        at += cnt; p = (*q == ';') ? q + 1 : q;
    }
    return 0;
}
static int pas_pf_nactuals(const char *sig) { int n = 0; const char *f; char k; for (int i = 0; (k = pas_pf_param(sig, i, &f)) != 0; i++) n += (k == 'P' || k == 'F') ? 4 : 1; return n; }
static void pas_pf_candidate(const PasDef *d) {
    for (int i = 0; i < g_pas_scope.ncand; i++) if (g_pas_scope.cand[i].rid == d->rid) return;
    PasDef *c = pas_scope_push(&g_pas_scope.cand, &g_pas_scope.ncand, &g_pas_scope.ccand, d->name); c->sig = d->sig; c->depth = d->depth; c->rid = d->rid;
}
static void pas_pf_closure(PNodeList *out, tree_t *a, const char *fsig, const char *callee, int pos) {
    const char *nm = NULL;
    if (a && a->t == TT_VAR) nm = a->v.sval;
    else if (a && a->t == TT_FNC && a->n >= 1 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval)
        nm = (!strcmp(a->c[0]->v.sval, "__pas_pcall") && a->n == 4) ? a->c[1]->v.sval : (a->n == 1 ? a->c[0]->v.sval : NULL);
    const PasDef *d = pas_pf_lookup(nm);
    if (!d) { fprintf(stderr, "pascal: ISO 7185 6.6.3.4 violation: the actual-parameter %d of '%s' shall be a %s-identifier\n", pos, callee, *fsig == 'F' ? "function" : "procedure");
        g_pas_iso_errors++; }
    else if (strcmp(d->sig, fsig)) {
        fprintf(stderr, "pascal: ISO 7185 6.6.3.6 violation: the actual-parameter %d of '%s' is '%s', whose heading %s is not congruous with the formal-parameter's %s\n",
                pos, callee, nm, d->sig, fsig);
        g_pas_iso_errors++; }
    if (d && d->formal) { pnl_push(out, leaf_s(TT_VAR, nm)); pnl_push(out, ilit(-1));
        for (int k = 1; k <= 3; k++) { pnl_push(out, leaf_s(TT_VAR, pas_pf_env_name(nm, k))); pnl_push(out, ilit(-1)); } return; }
    int lvl = d ? d->depth + 1 : 1;
    if (lvl > 4) { fprintf(stderr, "pascal: '%s' is declared at lexical level %d; passing a routine nested deeper than level 4 as an actual-parameter is not implemented\n", nm, lvl);
        g_pas_iso_errors++; }
    if (d) pas_pf_candidate(d);
    pnl_push(out, ilit(d ? d->rid : 0)); pnl_push(out, ilit(-1));
    for (int k = 1; k <= 3; k++) { pnl_push(out, k < lvl ? mk_fnc1("__pas_display_get", ilit(k)) : ilit(0)); pnl_push(out, ilit(-1)); }
}
static PNodeList *pas_pf_actuals(const char *callee, PNodeList *args) {
    const PasDef *cd = pas_pf_lookup(callee); const char *fs;
    if (cd && args) pas_value_actuals_check(callee, cd, args);
    if (!cd || !args || !strpbrk(cd->sig + 1, "PF")) return args;
    PNodeList *out = pnl_new();
    for (int i = 0; i + 1 < args->count; i += 2) { char k = pas_pf_param(cd->sig, i / 2, &fs);
        if (k == 'P' || k == 'F') pas_pf_closure(out, args->items[i], fs, callee, i / 2 + 1); else { pnl_push(out, args->items[i]); pnl_push(out, args->items[i + 1]); } }
    return out;
}
static tree_t *pas_pf_callthrough(const char *name, PNodeList *args) {
    const PasDef *d = pas_pf_lookup(name); int na = args ? args->count / 2 : 0, nf = pas_pf_nactuals(d->sig);
    if (na != nf) { fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with the wrong number of actual-parameters for its heading %s\n",
                            d->sig[0] == 'F' ? "6.7.3" : "6.8.2.3", name, d->sig); g_pas_iso_errors++; }
    tree_t *e = mk_fnc1("__pas_pcall", leaf_s(TT_VAR, name)); ast_push(e, leaf_s(TT_QLIT, d->sig)); ast_push(e, ilit(0));
    for (int i = 0; args && i + 1 < args->count; i += 2) ast_push(e, args->items[i]);
    return e;
}
static tree_t *pas_pf_envcall(tree_t *call, const char *pp, int lvl, int n) {
    tree_t *e = mk_fnc1("__pas_envcall", ilit(lvl));
    for (int k = 1; k <= 3; k++) ast_push(e, leaf_s(TT_VAR, pas_pf_env_name(pp, k)));
    for (int k = 1; k <= 3; k++) { char b[48]; snprintf(b, sizeof b, "__pas_vptmp_pfs%d_%d", n, k); ast_push(e, leaf_s(TT_VAR, ct_strdup(b))); }
    for (int i = 0; i < call->n; i++) ast_push(e, call->c[i]);
    return e;
}
static void pas_pf_resolve(tree_t *e) {
    if (!e) return;
    for (int i = 0; i < e->n; i++) pas_pf_resolve(e->c[i]);
    if (e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall")) return;
    const char *pp = e->c[1]->v.sval, *fs = e->c[2]->v.sval; int isf = fs[0] == 'F', n = g_pas_scope.npf++;
    char tb[40]; snprintf(tb, sizeof tb, "__pas_vptmp_pf%d", n); const char *tmp = ct_strdup(tb);
    tree_t *chain = mk_fnc1("__pas_rterr", leaf_s(TT_QLIT, "6.6.3.4")); ast_push(chain, leaf_s(TT_QLIT, "a procedural or functional parameter denotes no routine"));
    for (int ci = g_pas_scope.ncand - 1; ci >= 0; ci--) { const PasDef *c = &g_pas_scope.cand[ci];
        if (strcmp(c->sig, fs)) continue;
        PNodeList *al = pnl_new(); for (int i = 4; i < e->n; i++) { pnl_push(al, pas_tree_clone(e->c[i])); pnl_push(al, ilit(-1)); }
        tree_t *call = mk_call(c->name, al);
        if (c->depth >= 1 && call && call->t == TT_FNC) call = pas_pf_envcall(call, pp, c->depth + 1, n);
        tree_t *iff = ast_node_new(TT_IF); ast_push(iff, bin(TT_EQ, leaf_s(TT_VAR, pp), ilit(c->rid)));
        ast_push(iff, isf ? mk_assign(leaf_s(TT_VAR, tmp), call) : call); ast_push(iff, chain); chain = iff; }
    if (isf) { tree_t *sq = ast_node_new(TT_SEQ_EXPR); ast_push(sq, chain); ast_push(sq, leaf_s(TT_VAR, tmp)); chain = sq; }
    *e = *chain;
}

#line 2055 "pascal.tab.c"

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
  YYSYMBOL_ATSIGN = 59,                    /* ATSIGN  */
  YYSYMBOL_INTCONST = 60,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 61,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 62,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 63,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 64,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_program = 66,                   /* program  */
  YYSYMBOL_file_id_list_opt = 67,          /* file_id_list_opt  */
  YYSYMBOL_block = 68,                     /* block  */
  YYSYMBOL_decl_part_list = 69,            /* decl_part_list  */
  YYSYMBOL_decl_part = 70,                 /* decl_part  */
  YYSYMBOL_label_list = 71,                /* label_list  */
  YYSYMBOL_const_decl_list = 72,           /* const_decl_list  */
  YYSYMBOL_const_decl = 73,                /* const_decl  */
  YYSYMBOL_constant = 74,                  /* constant  */
  YYSYMBOL_scalar_constant = 75,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 76,            /* type_decl_list  */
  YYSYMBOL_type_decl = 77,                 /* type_decl  */
  YYSYMBOL_type = 78,                      /* type  */
  YYSYMBOL_79_1 = 79,                      /* @1  */
  YYSYMBOL_80_2 = 80,                      /* @2  */
  YYSYMBOL_81_3 = 81,                      /* $@3  */
  YYSYMBOL_packed_opt = 82,                /* packed_opt  */
  YYSYMBOL_simple_type = 83,               /* simple_type  */
  YYSYMBOL_record_body = 84,               /* record_body  */
  YYSYMBOL_record_field_list = 85,         /* record_field_list  */
  YYSYMBOL_record_field = 86,              /* record_field  */
  YYSYMBOL_87_4 = 87,                      /* @4  */
  YYSYMBOL_case_arm_mark = 88,             /* case_arm_mark  */
  YYSYMBOL_case_open = 89,                 /* case_open  */
  YYSYMBOL_record_case_opt = 90,           /* record_case_opt  */
  YYSYMBOL_record_case_list = 91,          /* record_case_list  */
  YYSYMBOL_record_case_arm = 92,           /* record_case_arm  */
  YYSYMBOL_var_decl_list = 93,             /* var_decl_list  */
  YYSYMBOL_var_decl = 94,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 95,            /* procedure_decl  */
  YYSYMBOL_96_5 = 96,                      /* $@5  */
  YYSYMBOL_97_6 = 97,                      /* $@6  */
  YYSYMBOL_98_7 = 98,                      /* $@7  */
  YYSYMBOL_pv_mark = 99,                   /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 100,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 101,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 102,           /* parameter_decl  */
  YYSYMBOL_pf_params = 103,                /* pf_params  */
  YYSYMBOL_pf_sections = 104,              /* pf_sections  */
  YYSYMBOL_pf_section = 105,               /* pf_section  */
  YYSYMBOL_id_list = 106,                  /* id_list  */
  YYSYMBOL_body = 107,                     /* body  */
  YYSYMBOL_statement_list = 108,           /* statement_list  */
  YYSYMBOL_109_8 = 109,                    /* $@8  */
  YYSYMBOL_110_9 = 110,                    /* $@9  */
  YYSYMBOL_statement = 111,                /* statement  */
  YYSYMBOL_statement_no_label = 112,       /* statement_no_label  */
  YYSYMBOL_call = 113,                     /* call  */
  YYSYMBOL_call_with_args = 114,           /* call_with_args  */
  YYSYMBOL_argument_list = 115,            /* argument_list  */
  YYSYMBOL_argument = 116,                 /* argument  */
  YYSYMBOL_assignment = 117,               /* assignment  */
  YYSYMBOL_selector = 118,                 /* selector  */
  YYSYMBOL_expression_list = 119,          /* expression_list  */
  YYSYMBOL_compound_statement = 120,       /* compound_statement  */
  YYSYMBOL_goto_statement = 121,           /* goto_statement  */
  YYSYMBOL_if_statement = 122,             /* if_statement  */
  YYSYMBOL_case_statement = 123,           /* case_statement  */
  YYSYMBOL_124_10 = 124,                   /* $@10  */
  YYSYMBOL_case_list = 125,                /* case_list  */
  YYSYMBOL_case_elem = 126,                /* case_elem  */
  YYSYMBOL_constant_list = 127,            /* constant_list  */
  YYSYMBOL_while_statement = 128,          /* while_statement  */
  YYSYMBOL_repeat_statement = 129,         /* repeat_statement  */
  YYSYMBOL_for_statement = 130,            /* for_statement  */
  YYSYMBOL_with_statement = 131,           /* with_statement  */
  YYSYMBOL_with_open = 132,                /* with_open  */
  YYSYMBOL_expression = 133,               /* expression  */
  YYSYMBOL_simple_expression = 134,        /* simple_expression  */
  YYSYMBOL_term = 135,                     /* term  */
  YYSYMBOL_factor = 136,                   /* factor  */
  YYSYMBOL_set_member_list = 137,          /* set_member_list  */
  YYSYMBOL_set_member = 138                /* set_member  */
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

/* The parser invokes alloca or the compile-time arena; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include \"ct_arena.h\"
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
       && ! ((defined YYMALLOC) \
             && (defined YYFREE)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ct_alloc
#   if 0
void *ct_alloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ct_drop
#   if 0
void ct_drop(void *); /* INFRINGES ON USER NAME SPACE */
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
#define YYLAST   472

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  183
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  370

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  2020,  2020,  2041,  2042,  2045,  2048,  2049,  2052,  2053,
    2054,  2055,  2056,  2059,  2060,  2063,  2064,  2066,  2067,  2068,
    2069,  2070,  2071,  2073,  2073,  2073,  2074,  2074,  2074,  2074,
    2074,  2076,  2077,  2079,  2081,  2082,  2083,  2083,  2083,  2085,
    2086,  2086,  2087,  2088,  2089,  2097,  2097,  2099,  2106,  2107,
    2108,  2110,  2113,  2116,  2117,  2120,  2120,  2121,  2124,  2127,
    2130,  2131,  2132,  2135,  2136,  2139,  2140,  2143,  2144,  2146,
    2148,  2149,  2150,  2150,  2154,  2154,  2158,  2158,  2164,  2167,
    2168,  2171,  2172,  2175,  2176,  2177,  2178,  2181,  2182,  2185,
    2186,  2189,  2190,  2191,  2192,  2195,  2196,  2199,  2202,  2202,
    2203,  2203,  2206,  2207,  2212,  2213,  2214,  2215,  2216,  2217,
    2218,  2219,  2220,  2221,  2222,  2225,  2226,  2229,  2236,  2237,
    2240,  2241,  2242,  2245,  2280,  2285,  2289,  2290,  2293,  2294,
    2297,  2300,  2305,  2306,  2309,  2309,  2320,  2321,  2324,  2325,
    2328,  2329,  2332,  2335,  2338,  2343,  2350,  2353,  2354,  2361,
    2362,  2363,  2364,  2365,  2366,  2367,  2368,  2371,  2372,  2373,
    2374,  2375,  2376,  2379,  2380,  2381,  2382,  2383,  2384,  2387,
    2388,  2389,  2390,  2391,  2392,  2393,  2394,  2395,  2396,  2397,
    2400,  2401,  2404,  2405
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
  "LPARENT", "RPARENT", "DOTDOT", "ATSIGN", "INTCONST", "CHARCODE",
  "REALCONST", "STRINGCONST", "IDENT", "$accept", "program",
  "file_id_list_opt", "block", "decl_part_list", "decl_part", "label_list",
  "const_decl_list", "const_decl", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "@1", "@2", "$@3", "packed_opt",
  "simple_type", "record_body", "record_field_list", "record_field", "@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "pf_params", "pf_sections", "pf_section", "id_list",
  "body", "statement_list", "$@8", "$@9", "statement",
  "statement_no_label", "call", "call_with_args", "argument_list",
  "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@10", "case_list", "case_elem", "constant_list", "while_statement",
  "repeat_statement", "for_statement", "with_statement", "with_open",
  "expression", "simple_expression", "term", "factor", "set_member_list",
  "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-243)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-128)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      34,   -15,    64,    15,  -243,    30,    67,  -243,    -2,  -243,
      63,  -243,    32,   340,  -243,  -243,    70,    86,    98,    30,
     102,  -243,   115,  -243,  -243,  -243,  -243,    10,    59,    86,
    -243,  -243,    30,  -243,   162,  -243,    19,    14,   150,   115,
    -243,  -243,   130,   376,  -243,   141,  -243,   111,    13,  -243,
    -243,   156,   173,  -243,   200,   178,   200,  -243,   200,   171,
     138,  -243,  -243,  -243,  -243,  -243,    75,  -243,  -243,  -243,
    -243,  -243,  -243,  -243,  -243,   111,  -243,  -243,   145,   222,
    -243,   247,   248,   253,  -243,   266,  -243,    11,   283,  -243,
     250,   250,   210,    30,   231,  -243,   232,   233,   239,   301,
     212,  -243,  -243,   255,    14,  -243,   275,    17,   269,   269,
     269,    78,   200,   269,  -243,  -243,  -243,  -243,   252,  -243,
     127,   142,   109,    74,  -243,  -243,   127,    27,   300,    29,
     352,    44,   200,   200,   260,  -243,   200,   311,  -243,   314,
    -243,  -243,   317,  -243,  -243,  -243,  -243,  -243,   262,    30,
     263,     8,  -243,   169,   308,  -243,    94,   397,   397,   397,
    -243,   281,   315,   316,  -243,  -243,   274,  -243,   200,   200,
    -243,    74,    74,  -243,   312,    41,  -243,   332,  -243,    14,
     200,   200,   200,   200,   200,   200,   200,   269,   269,   269,
     269,   269,   269,   269,   269,    14,   178,  -243,  -243,    14,
    -243,   120,  -243,   364,   407,  -243,    51,   407,  -243,  -243,
    -243,   293,   188,   293,    11,  -243,   286,   334,  -243,  -243,
    -243,  -243,  -243,   365,   111,   365,    30,   346,   353,   258,
     407,   200,   200,  -243,  -243,  -243,   109,   109,   109,   109,
     109,   109,   109,    74,    74,    74,  -243,  -243,  -243,  -243,
    -243,  -243,   127,   397,   327,   200,  -243,   200,   200,  -243,
      25,  -243,   297,   310,  -243,  -243,  -243,   358,    99,  -243,
    -243,   341,    36,  -243,   191,  -243,   378,   200,   200,   407,
    -243,  -243,    40,  -243,   198,    14,  -243,   390,   407,   303,
      30,   324,     9,  -243,   215,  -243,   326,  -243,   365,   370,
    -243,    30,   335,  -243,  -243,   395,  -243,   159,   186,   397,
    -243,   397,    14,  -243,   200,   293,   220,   293,    25,  -243,
     337,  -243,   347,  -243,  -243,    -1,   111,  -243,   398,    14,
      14,  -243,  -243,  -243,   407,  -243,   349,   362,  -243,  -243,
     389,  -243,  -243,   351,  -243,  -243,  -243,  -243,  -243,   354,
     111,   111,   397,   396,  -243,  -243,  -243,   414,  -243,   225,
    -243,   397,   399,   397,  -243,  -243,   414,    30,   363,  -243
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    96,     0,     7,
       0,     3,     0,     0,    95,     2,     0,     0,     0,     0,
       0,   100,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    78,    11,    68,     0,    78,     0,   114,     0,    10,
      32,     8,     0,     0,    15,    80,    67,    46,    80,    98,
      97,     0,     0,   100,     0,     0,     0,   100,     0,     0,
     115,   101,   102,   105,   116,   104,     0,   106,   107,   108,
     109,   110,   111,   112,   113,    46,    31,    13,     0,     0,
      27,     0,     0,     0,    26,     0,    23,     0,     0,    45,
       0,     0,     0,     0,     0,    28,     0,    48,     0,     0,
       0,    34,    76,     0,   114,   131,     0,     0,     0,     0,
       0,     0,     0,     0,   171,   174,   172,   173,   127,   170,
     169,     0,   149,   157,   163,   127,   148,     0,     0,     0,
       0,   114,     0,     0,     0,   126,     0,     0,    30,     0,
      29,    24,     0,    25,    21,    17,    20,    22,     0,     0,
       0,     0,    82,     0,    72,    35,     0,     0,     0,     0,
      69,     0,    43,     0,    40,     7,     0,    99,     0,     0,
     176,   158,   159,   178,   182,     0,   180,     0,   177,   114,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,     0,   134,   130,   114,
     103,     0,   119,   120,   123,   125,     0,   129,    33,    18,
      19,    88,     0,    88,     0,    79,     0,     0,     7,    47,
      51,    50,    49,     0,    46,     0,    57,     0,     0,     0,
     143,     0,     0,   179,   175,   142,   150,   151,   152,   153,
     154,   155,   156,   162,   160,   161,   166,   167,   168,   164,
     165,   146,   147,   139,   132,     0,   117,     0,     0,   124,
       0,    83,     0,     0,    81,    86,    70,     0,     0,    44,
      42,     0,    62,    54,     0,    77,    74,     0,     0,   183,
     181,   141,     0,   137,     0,   114,   118,   121,   128,     0,
       0,     0,     0,    90,     0,    85,     0,    73,     0,     0,
      41,    57,     0,    52,    55,     0,     7,     0,     0,   139,
     135,     0,   114,   133,     0,    88,     0,    88,     0,    87,
       0,    84,     0,    36,    53,     0,    46,    71,     0,   114,
     114,   136,   140,   138,   122,    93,     0,     0,    89,    91,
       0,    37,    59,     0,    56,    75,   145,   144,    92,     0,
      46,    46,    66,     0,    94,    39,    38,    61,    64,     0,
      59,    66,     0,    66,    63,    58,    60,    57,     0,    65
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -243,  -243,  -243,  -159,  -243,  -243,  -243,  -243,   416,   -43,
       2,  -243,   415,   -71,  -243,  -243,  -243,  -243,  -213,    79,
    -243,   152,  -243,  -243,    96,  -243,   100,   101,  -243,   432,
    -243,  -243,  -243,  -243,   430,   418,  -243,   254,  -208,  -243,
     149,    -3,  -243,   160,  -243,  -243,   -97,   338,  -243,   -34,
    -243,   216,  -243,   -36,  -243,  -243,  -243,  -243,  -243,  -243,
    -243,   161,  -242,  -243,  -243,  -243,  -243,  -243,   -33,   199,
    -101,   -73,  -243,   240
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,    98,
      86,    39,    40,    99,   341,   351,   226,   100,   101,   271,
     272,   273,   326,   367,   352,   303,   357,   358,    32,    33,
      24,   218,   306,   165,    45,    88,   151,   152,   261,   292,
     293,   274,    25,    36,   104,    37,    61,    62,    63,   119,
     201,   202,    65,   120,   206,    67,    68,    69,    70,   253,
     282,   283,   359,    71,    72,    73,    74,   127,   174,   122,
     123,   124,   175,   176
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,    66,     8,    64,   137,   263,   227,   167,   171,   172,
     268,   284,   270,   214,   318,    41,    34,    51,   102,   126,
     342,   121,    49,   128,    49,   130,    52,    53,    54,    34,
     148,    55,    56,   169,    49,   170,   149,   195,     1,   150,
     178,   301,    50,    57,   289,   309,    58,    51,    10,     3,
     290,   343,   198,   291,   302,    11,    52,    53,    54,   267,
      42,    55,    56,   310,     4,   215,   319,   284,    66,    87,
      64,     5,     9,    57,    59,     7,    58,   196,    60,   177,
     141,   143,   235,    15,   153,   322,   243,   244,   245,     7,
     156,   232,   141,   143,     7,    66,   233,    64,   251,   203,
     204,   258,   254,   207,    43,   133,   259,   335,    60,   337,
     190,   191,   192,   108,   220,   221,   222,   246,   247,   248,
     249,   250,   193,   194,   109,   110,   134,    14,   135,   136,
      26,    89,   111,   173,   112,   229,   230,   113,   114,   115,
     116,   117,   118,    66,    10,    64,   212,   328,   187,   298,
      28,   219,   179,   269,   299,   188,   189,    90,    91,    66,
     252,    64,    31,    66,    92,    64,    35,    93,  -127,   329,
     255,    80,    94,    95,    96,    97,   180,   256,   134,    38,
     135,   136,   181,   182,   183,   184,   185,   186,   313,  -127,
      77,  -127,  -127,   180,   132,    75,   330,    87,   279,   181,
     182,   183,   184,   185,   186,    80,   138,   139,   140,    84,
     281,   153,    10,   107,    47,   333,   105,   129,   161,    10,
     180,   216,   203,   131,   287,   288,   181,   182,   183,   184,
     185,   186,   346,   347,   162,   108,   163,   106,    10,   164,
     262,    10,   125,   304,   307,   308,   109,   110,   311,    66,
     312,    64,   144,   145,   111,   344,   112,   294,   146,   113,
     114,   115,   116,   117,   118,    10,   281,   320,   332,   277,
      10,   147,   336,   278,   155,   311,    66,   362,    64,   355,
     356,   334,    80,   138,   142,   140,    84,   316,   154,   157,
     158,   -26,   180,    66,    66,    64,    64,   159,   181,   182,
     183,   184,   185,   186,   108,   168,   160,   166,   132,   281,
      80,   138,    95,   140,    84,   294,   208,   217,   281,   209,
     281,   197,   210,   111,   205,   112,   211,   213,   113,   114,
     115,   116,   117,   118,   180,   223,   224,   225,   228,   266,
     181,   182,   183,   184,   185,   186,   180,    16,    17,   260,
     265,   275,   181,   182,   183,   184,   185,   186,   276,    18,
     285,   295,   296,   297,   300,    19,   180,   315,    20,    21,
     231,    22,   181,   182,   183,   184,   185,   186,   199,   236,
     237,   238,   239,   240,   241,   242,   180,   305,   317,   234,
     321,   323,   181,   182,   183,   184,   185,   186,   180,   325,
     327,   339,   340,   345,   181,   182,   183,   184,   185,   186,
     350,    90,    91,   348,   349,   353,   257,   360,   354,   361,
     369,    93,    78,    79,   180,    80,    94,    95,    96,    97,
     181,   182,   183,   184,   185,   186,    80,    81,    82,    83,
      84,   180,   314,    90,    91,    44,   368,   181,   182,   183,
     184,   185,   186,   324,    76,   365,   363,    80,   138,    95,
     140,    84,   364,   366,    46,    48,   103,   338,   264,   200,
     331,   286,   280
};

static const yytype_int16 yycheck[] =
{
      43,    37,     5,    37,    75,   213,   165,   104,   109,   110,
     223,   253,   225,     5,     5,     5,    19,     3,     5,    55,
      21,    54,     5,    56,     5,    58,    12,    13,    14,    32,
      19,    17,    18,    16,     5,   108,    25,    10,     4,    28,
     113,     5,    23,    29,    19,     5,    32,     3,    50,    64,
      25,    52,    23,    28,    18,    57,    12,    13,    14,   218,
      50,    17,    18,    23,     0,    57,    57,   309,   104,    56,
     104,    56,     5,    29,    60,    64,    32,    50,    64,   112,
      78,    79,   179,    51,    87,   298,   187,   188,   189,    64,
      93,    50,    90,    91,    64,   131,    55,   131,   195,   132,
     133,    50,   199,   136,    45,    30,    55,   315,    64,   317,
      36,    37,    38,    35,   157,   158,   159,   190,   191,   192,
     193,   194,    48,    49,    46,    47,    51,    64,    53,    54,
      60,    20,    54,    55,    56,   168,   169,    59,    60,    61,
      62,    63,    64,   179,    50,   179,   149,   306,    39,    50,
      64,    57,    10,   224,    55,    46,    47,    46,    47,   195,
     196,   195,    64,   199,    53,   199,    64,    56,    30,    10,
      50,    60,    61,    62,    63,    64,    34,    57,    51,    64,
      53,    54,    40,    41,    42,    43,    44,    45,   285,    51,
      60,    53,    54,    34,    56,    45,    10,    56,   231,    40,
      41,    42,    43,    44,    45,    60,    61,    62,    63,    64,
     253,   214,    50,    53,    52,   312,    60,    57,     6,    50,
      34,    52,   255,    52,   257,   258,    40,    41,    42,    43,
      44,    45,   329,   330,    22,    35,    24,    64,    50,    27,
      52,    50,    64,    52,   277,   278,    46,    47,    50,   285,
      52,   285,     5,     5,    54,   326,    56,   260,     5,    59,
      60,    61,    62,    63,    64,    50,   309,    52,   311,    11,
      50,     5,    52,    15,    64,    50,   312,    52,   312,   350,
     351,   314,    60,    61,    62,    63,    64,   290,     5,    58,
      58,    58,    34,   329,   330,   329,   330,    58,    40,    41,
      42,    43,    44,    45,    35,    30,     5,    52,    56,   352,
      60,    61,    62,    63,    64,   318,     5,     9,   361,     5,
     363,    21,     5,    54,    64,    56,    64,    64,    59,    60,
      61,    62,    63,    64,    34,    54,    21,    21,    64,     5,
      40,    41,    42,    43,    44,    45,    34,     7,     8,    56,
      64,     5,    40,    41,    42,    43,    44,    45,     5,    19,
      33,    64,    52,     5,    23,    25,    34,    64,    28,    29,
      58,    31,    40,    41,    42,    43,    44,    45,    26,   180,
     181,   182,   183,   184,   185,   186,    34,     9,    64,    57,
      64,    21,    40,    41,    42,    43,    44,    45,    34,    64,
       5,    64,    55,     5,    40,    41,    42,    43,    44,    45,
      21,    46,    47,    64,    52,    64,    52,    21,    64,     5,
      57,    56,    46,    47,    34,    60,    61,    62,    63,    64,
      40,    41,    42,    43,    44,    45,    60,    61,    62,    63,
      64,    34,    52,    46,    47,    29,   367,    40,    41,    42,
      43,    44,    45,   301,    39,    56,   360,    60,    61,    62,
      63,    64,   361,   363,    32,    35,    48,   318,   214,   131,
     309,   255,   232
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    66,    64,     0,    56,    67,    64,   106,     5,
      50,    57,    68,    69,    64,    51,     7,     8,    19,    25,
      28,    29,    31,    70,    95,   107,    60,    71,    64,    72,
      73,    64,    93,    94,   106,    64,   108,   110,    64,    76,
      77,     5,    50,    45,    73,    99,    94,    52,    99,     5,
      23,     3,    12,    13,    14,    17,    18,    29,    32,    60,
      64,   111,   112,   113,   114,   117,   118,   120,   121,   122,
     123,   128,   129,   130,   131,    45,    77,    60,    46,    47,
      60,    61,    62,    63,    64,    74,    75,    56,   100,    20,
      46,    47,    53,    56,    61,    62,    63,    64,    74,    78,
      82,    83,     5,   100,   109,    60,    64,   108,    35,    46,
      47,    54,    56,    59,    60,    61,    62,    63,    64,   114,
     118,   133,   134,   135,   136,    64,   118,   132,   133,   108,
     133,    52,    56,    30,    51,    53,    54,    78,    61,    62,
      63,    75,    62,    75,     5,     5,     5,     5,    19,    25,
      28,   101,   102,   106,     5,    64,   106,    58,    58,    58,
       5,     6,    22,    24,    27,    98,    52,   111,    30,    16,
     136,   135,   135,    55,   133,   137,   138,   133,   136,    10,
      34,    40,    41,    42,    43,    44,    45,    39,    46,    47,
      36,    37,    38,    48,    49,    10,    50,    21,    23,    26,
     112,   115,   116,   133,   133,    64,   119,   133,     5,     5,
       5,    64,   106,    64,     5,    57,    52,     9,    96,    57,
      74,    74,    74,    54,    21,    21,    81,    68,    64,   133,
     133,    58,    50,    55,    57,   111,   134,   134,   134,   134,
     134,   134,   134,   135,   135,   135,   136,   136,   136,   136,
     136,   111,   118,   124,   111,    50,    57,    52,    50,    55,
      56,   103,    52,   103,   102,    64,     5,    68,    83,    78,
      83,    84,    85,    86,   106,     5,     5,    11,    15,   133,
     138,    74,   125,   126,   127,    33,   116,   133,   133,    19,
      25,    28,   104,   105,   106,    64,    52,     5,    50,    55,
      23,     5,    18,    90,    52,     9,    97,   133,   133,     5,
      23,    50,    52,   111,    52,    64,   106,    64,     5,    57,
      52,    64,    83,    21,    86,    64,    87,     5,    68,    10,
      10,   126,    74,   111,   133,   103,    52,   103,   105,    64,
      55,    79,    21,    52,    78,     5,   111,   111,    64,    52,
      21,    80,    89,    64,    64,    78,    78,    91,    92,   127,
      21,     5,    52,    89,    92,    56,    91,    88,    84,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    67,    68,    69,    69,    70,    70,
      70,    70,    70,    71,    71,    72,    72,    73,    73,    73,
      73,    73,    73,    74,    74,    74,    75,    75,    75,    75,
      75,    76,    76,    77,    78,    78,    79,    80,    78,    78,
      81,    78,    78,    78,    78,    82,    82,    83,    83,    83,
      83,    83,    84,    85,    85,    87,    86,    86,    88,    89,
      90,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      95,    95,    96,    95,    97,    95,    98,    95,    99,   100,
     100,   101,   101,   102,   102,   102,   102,   103,   103,   104,
     104,   105,   105,   105,   105,   106,   106,   107,   109,   108,
     110,   108,   111,   111,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   113,   113,   114,   115,   115,
     116,   116,   116,   117,   118,   118,   118,   118,   119,   119,
     120,   121,   122,   122,   124,   123,   125,   125,   126,   126,
     127,   127,   128,   129,   130,   130,   131,   132,   132,   133,
     133,   133,   133,   133,   133,   133,   133,   134,   134,   134,
     134,   134,   134,   135,   135,   135,   135,   135,   135,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     137,   137,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     5,     5,
       4,     4,     4,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     1,     4,     1,     2,     0,     0,     9,     9,
       0,     5,     4,     2,     4,     1,     0,     3,     1,     3,
       3,     3,     2,     3,     1,     0,     4,     0,     0,     0,
       7,     5,     0,     3,     1,     6,     0,     2,     1,     4,
       7,     9,     0,     8,     0,    10,     0,     7,     0,     3,
       0,     3,     1,     3,     5,     4,     3,     3,     0,     3,
       1,     3,     4,     3,     5,     3,     1,     3,     0,     4,
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     4,     3,     1,
       1,     3,     5,     3,     4,     3,     2,     1,     3,     1,
       3,     2,     4,     6,     0,     6,     3,     1,     3,     0,
       3,     1,     4,     4,     8,     8,     4,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     3,     2,     2,     2,     3,
       1,     3,     1,     3
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
#line 2021 "pascal.y"
        { tree_t *body = (yyvsp[-1].node);
          if (g_pas_narray > 0 || g_pas_nhdrfile > 0 || g_pas_nscalarvartype > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_narray; i++) if (!g_pas_arrays[i].is_param && !g_pas_arrays[i].is_local) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_init(g_pas_arrays[i].name, g_pas_arrays[i].high)));
              for (int i = 0; i < g_pas_nscalarvartype; i++) { const char *vn = g_pas_scalarvartype[i].vname; long long _ah;
                  if (!vn || !g_pas_scalarvartype[i].is_global || pas_is_func(vn) || pas_is_proc(vn) || pas_array_high_get(vn, &_ah)) continue;
                  if (pas_var_typename_is(vn, pas_is_int_typename)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), ilit(0)));
                  else if (pas_var_typename_is(vn, pas_is_realtypename)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), flit(0.0))); }
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          body = pas_trace_prepend_tap_off(body);
          tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0); emit_proc(&g_pascal_procs, mainp);
          for (int i = 0; i < g_pascal_procs.count; i++) pas_pf_resolve(g_pascal_procs.items[i]);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
#line 3483 "pascal.tab.c"
    break;

  case 3: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 2041 "pascal.y"
                            { pas_scope_define_list((yyvsp[-1].list)); pas_program_params_distinct((yyvsp[-1].list)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) { tree_t *id = (yyvsp[-1].list)->items[i]; if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) { pas_filevar_add(id->v.sval); if (g_pas_nhdrfile < 32) g_pas_hdrfiles[g_pas_nhdrfile++] = ct_strdup(id->v.sval); } } }
#line 3489 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 2045 "pascal.y"
                        { pas_labels_close(); (yyval.node) = (yyvsp[0].node); }
#line 3495 "pascal.tab.c"
    break;

  case 6: /* decl_part_list: decl_part_list decl_part  */
#line 2048 "pascal.y"
                             { (yyval.ival) = pas_decl_part_order((yyvsp[-1].ival), (yyvsp[0].ival)); }
#line 3501 "pascal.tab.c"
    break;

  case 7: /* decl_part_list: %empty  */
#line 2049 "pascal.y"
      { (yyval.ival) = 0; }
#line 3507 "pascal.tab.c"
    break;

  case 8: /* decl_part: LABELSY label_list SEMICOLON  */
#line 2052 "pascal.y"
                                 { (yyval.ival) = 1; }
#line 3513 "pascal.tab.c"
    break;

  case 9: /* decl_part: CONSTSY const_decl_list  */
#line 2053 "pascal.y"
                              { (yyval.ival) = 2; }
#line 3519 "pascal.tab.c"
    break;

  case 10: /* decl_part: TYPESY type_decl_list  */
#line 2054 "pascal.y"
                            { (yyval.ival) = 3; }
#line 3525 "pascal.tab.c"
    break;

  case 11: /* decl_part: VARSY var_decl_list  */
#line 2055 "pascal.y"
                          { (yyval.ival) = 4; }
#line 3531 "pascal.tab.c"
    break;

  case 12: /* decl_part: procedure_decl  */
#line 2056 "pascal.y"
                     { (yyval.ival) = 5; }
#line 3537 "pascal.tab.c"
    break;

  case 13: /* label_list: label_list COMMA INTCONST  */
#line 2059 "pascal.y"
                              { pas_label_in_range((yyvsp[0].ival)); pas_label_declare((yyvsp[0].ival)); }
#line 3543 "pascal.tab.c"
    break;

  case 14: /* label_list: INTCONST  */
#line 2060 "pascal.y"
               { pas_label_in_range((yyvsp[0].ival)); pas_label_declare((yyvsp[0].ival)); }
#line 3549 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 2066 "pascal.y"
                                           { pas_scope_define((yyvsp[-3].str)); pas_rconst_add((yyvsp[-3].str), (yyvsp[-1].dval)); }
#line 3555 "pascal.tab.c"
    break;

  case 18: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 2067 "pascal.y"
                                          { pas_scope_define((yyvsp[-4].str)); pas_rconst_add((yyvsp[-4].str), (yyvsp[-1].dval)); }
#line 3561 "pascal.tab.c"
    break;

  case 19: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 2068 "pascal.y"
                                           { pas_scope_define((yyvsp[-4].str)); pas_rconst_add((yyvsp[-4].str), -(yyvsp[-1].dval)); }
#line 3567 "pascal.tab.c"
    break;

  case 20: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 2069 "pascal.y"
                                       { pas_scope_define((yyvsp[-3].str)); if ((yyvsp[-1].str) && strlen((yyvsp[-1].str))==1) { pas_const_add((yyvsp[-3].str),(long long)(unsigned char)(yyvsp[-1].str)[0]); pas_charvar_add((yyvsp[-3].str)); } else pas_sconst_add((yyvsp[-3].str),(yyvsp[-1].str)); }
#line 3573 "pascal.tab.c"
    break;

  case 21: /* const_decl: IDENT EQOP CHARCODE SEMICOLON  */
#line 2070 "pascal.y"
                                    { pas_scope_define((yyvsp[-3].str)); pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); pas_charvar_add((yyvsp[-3].str)); }
#line 3579 "pascal.tab.c"
    break;

  case 22: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 2071 "pascal.y"
                                    { pas_scope_define((yyvsp[-3].str)); pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 3585 "pascal.tab.c"
    break;

  case 23: /* constant: scalar_constant  */
#line 2073 "pascal.y"
                    { (yyval.ival) = (yyvsp[0].ival); }
#line 3591 "pascal.tab.c"
    break;

  case 24: /* constant: PLUS scalar_constant  */
#line 2073 "pascal.y"
                                                        { (yyval.ival) = (yyvsp[0].ival); }
#line 3597 "pascal.tab.c"
    break;

  case 25: /* constant: MINUS scalar_constant  */
#line 2073 "pascal.y"
                                                                                             { (yyval.ival) = -(yyvsp[0].ival); }
#line 3603 "pascal.tab.c"
    break;

  case 26: /* scalar_constant: IDENT  */
#line 2074 "pascal.y"
                       { pas_scope_applied((yyvsp[0].str)); long long cv = 0; if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "true")) cv = 1; else if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "false")) cv = 0; else if (!pas_const_get((yyvsp[0].str), &cv) && (yyvsp[0].str) && !strcmp((yyvsp[0].str), "maxint")) cv = 2147483647; (yyval.ival) = cv; }
#line 3609 "pascal.tab.c"
    break;

  case 27: /* scalar_constant: INTCONST  */
#line 2074 "pascal.y"
                                                                                                                                                                                                                                                              { (yyval.ival) = (yyvsp[0].ival); }
#line 3615 "pascal.tab.c"
    break;

  case 28: /* scalar_constant: REALCONST  */
#line 2074 "pascal.y"
                                                                                                                                                                                                                                                                                       { pas_real_is_not_ordinal((yyvsp[0].dval)); (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 3621 "pascal.tab.c"
    break;

  case 29: /* scalar_constant: STRINGCONST  */
#line 2074 "pascal.y"
                                                                                                                                                                                                                                                                                                                                                          { (yyval.ival) = ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) ? (long long)(unsigned char)(yyvsp[0].str)[0] : 0; }
#line 3627 "pascal.tab.c"
    break;

  case 30: /* scalar_constant: CHARCODE  */
#line 2074 "pascal.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                             { (yyval.ival) = (yyvsp[0].ival); }
#line 3633 "pascal.tab.c"
    break;

  case 33: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 2079 "pascal.y"
                                     { pas_scope_define((yyvsp[-3].str)); pas_type_not_self_applied((yyvsp[-3].str)); pas_tsz_add((yyvsp[-3].str), g_pas_pend_esz); if (g_pas_pend_ischar && g_pas_pend_sub_high >= 0 && !g_pas_pend_isarr && g_pas_pend_nf == 0) pas_typealias_add((yyvsp[-3].str), "char"); long long _ty3 = ((yyvsp[-1].ival) == -4) ? -1 : (yyvsp[-1].ival); int _pk3 = ((yyvsp[-1].ival) == -4) || (g_pas_pend_typename && pas_rectype_is_packed(g_pas_pend_typename)); if (_ty3 < 0 && g_pas_pend_istfile) pas_tfiletype_add((yyvsp[-3].str)); if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_pas_pend_isbool) pas_booltype_add((yyvsp[-3].str)); if (_ty3 == -2) pas_settype_add((yyvsp[-3].str), g_pas_pend_set_hi); if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str), _pk3); if (g_pas_pend_enum_max >= 0) { pas_enumtype_add((yyvsp[-3].str), g_pas_pend_enum_max); pas_enumnames_add((yyvsp[-3].str), g_pas_pend_enum_names); } if (g_pas_pend_sub_high >= 0 && _ty3 < 0 && g_pas_pend_arr_ncols < 0) pas_subtype_add((yyvsp[-3].str), g_pas_pend_sub_low, g_pas_pend_sub_high); if (_ty3 >= 0 && !g_pas_pend_ptrtarget) { pas_arrtype_add((yyvsp[-3].str), _ty3, g_pas_pend_arr_ncols >= 0 ? 1 : 0, g_pas_pend_arr_ncols); } if (_ty3 == -1 && !g_pas_pend_ptrtarget && g_pas_pend_nf == 0 && g_pas_pend_enum_max < 0 && g_pas_pend_sub_high < 0 && g_pas_pend_typename) pas_typealias_add((yyvsp[-3].str), g_pas_pend_typename); pas_pend_reset(); }
#line 3639 "pascal.tab.c"
    break;

  case 34: /* type: simple_type  */
#line 2081 "pascal.y"
                { if (g_pas_pend_ptrtarget) { (yyval.ival) = -3; } else if ((yyvsp[0].ival) == -2) { (yyval.ival) = -2; } else if ((yyvsp[0].ival) >= 0 && g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0) { long long _tnc = pas_arrtype_ncols(g_pas_pend_typename); if (_tnc >= 0) g_pas_pend_arr_ncols = _tnc; (yyval.ival) = (yyvsp[0].ival); } else { (yyval.ival) = -1; } }
#line 3645 "pascal.tab.c"
    break;

  case 35: /* type: ARROW IDENT  */
#line 2082 "pascal.y"
                  { g_pas_pend_esz = -1; g_pas_pend_ptrtarget = ct_strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 3651 "pascal.tab.c"
    break;

  case 36: /* @1: %empty  */
#line 2083 "pascal.y"
                                                        { (yyval.ival) = pas_index_bound((yyvsp[-2].ival), 0); }
#line 3657 "pascal.tab.c"
    break;

  case 37: /* @2: %empty  */
#line 2083 "pascal.y"
                                                                                               { (yyval.ival) = pas_index_bound((yyvsp[-3].ival), 1); g_pas_pend_esz = -1; }
#line 3663 "pascal.tab.c"
    break;

  case 38: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY @1 @2 type  */
#line 2083 "pascal.y"
                                                                                                                                                                { g_pas_pend_esz = (g_pas_pend_esz >= 0 && (yyvsp[-1].ival) >= (yyvsp[-2].ival)) ? g_pas_pend_esz * ((yyvsp[-1].ival) - (yyvsp[-2].ival) + 1) : -1; int _eic = g_pas_pend_ischar; int _wr = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); g_pas_pend_arr_wrap = _wr; g_pas_pend_arr_ptrto = g_pas_pend_ptrtarget ? g_pas_pend_ptrtarget : (g_pas_pend_typename ? (char *)pas_ptrtype_target(g_pas_pend_typename) : NULL); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_ischar = _eic; g_pas_pend_isarr = 1;
        if ((yyvsp[-1].ival) >= (yyvsp[-2].ival)) { g_pas_pend_sub_low = (yyvsp[-2].ival); g_pas_pend_sub_high = (yyvsp[-1].ival); } (yyval.ival) = ((yyvsp[-5].ival) >= 0 || (yyvsp[-1].ival) < (yyvsp[-2].ival)) ? (yyvsp[-5].ival) : (yyvsp[-1].ival); }
#line 3670 "pascal.tab.c"
    break;

  case 39: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 2085 "pascal.y"
                                                                               { g_pas_pend_esz = -1; int _eic = g_pas_pend_ischar; g_pas_pend_ptrtarget = NULL; long long r = (yyvsp[-5].ival); long long c = (yyvsp[-3].ival); g_pas_pend_arr_ncols = c + 1; g_pas_pend_arr_ischar = _eic; g_pas_pend_isarr = 1; (yyval.ival) = (r + 1) * (c + 1) - 1; }
#line 3676 "pascal.tab.c"
    break;

  case 40: /* $@3: %empty  */
#line 2086 "pascal.y"
                          { g_pas_recbody_depth++; if (g_pas_recbody_depth > 1) pas_pend_nest_save(); }
#line 3682 "pascal.tab.c"
    break;

  case 41: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 2086 "pascal.y"
                                                                                                                          { if (g_pas_recbody_depth > 1) { char _anm[32]; snprintf(_anm, sizeof _anm, "$anonrec%d", ++g_pas_scope.anonseq); pas_rectype_add(_anm, (yyvsp[-4].ival) ? 1 : 0); pas_pend_nest_restore(); g_pas_pend_typename = ct_strdup(_anm); } g_pas_recbody_depth--; g_pas_pend_ptrtarget = NULL; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_enum_max = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; g_pas_pend_esz = (g_pas_recbody_depth == 0 && g_pas_pend_nf == 0) ? 0 : -1; (yyval.ival) = (yyvsp[-4].ival) ? -4 : -1; }
#line 3688 "pascal.tab.c"
    break;

  case 42: /* type: packed_opt SETSY OFSY simple_type  */
#line 2087 "pascal.y"
                                        { g_pas_pend_esz = -1; g_pas_pend_ptrtarget = NULL; g_pas_pend_set_hi = (yyvsp[0].ival); (yyval.ival) = -2; }
#line 3694 "pascal.tab.c"
    break;

  case 43: /* type: packed_opt FILESY  */
#line 2088 "pascal.y"
                        { g_pas_pend_esz = -1; g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 3700 "pascal.tab.c"
    break;

  case 44: /* type: packed_opt FILESY OFSY type  */
#line 2089 "pascal.y"
                                  { int _cf = g_pas_pend_istfile || (g_pas_pend_typename && (!strcmp(g_pas_pend_typename, "text") || pas_rectype_has_file(g_pas_pend_typename)));
        for (int _i = 0; !_cf && _i < g_pas_pend_nf; _i++) if (g_pas_pend_fldfile[_i]) _cf = 1;
        if (_cf) { fprintf(stderr, "pascal: ISO 7185 6.4.3.5 violation: the component-type of a file-type shall not be a file-type nor a structured-type having a file-type as a component -- 'file of %s'\n", g_pas_pend_typename ? g_pas_pend_typename : "<file>"); g_pas_iso_errors++; }
        if (g_pas_pend_sub_high >= 0) { g_pas_pend_frng_lo = g_pas_pend_sub_low; g_pas_pend_frng_hi = g_pas_pend_sub_high; }
        else if (g_pas_pend_typename && pas_subtype_high(g_pas_pend_typename) >= 0) { g_pas_pend_frng_lo = pas_subtype_low(g_pas_pend_typename); g_pas_pend_frng_hi = pas_subtype_high(g_pas_pend_typename); }
        g_pas_pend_frng_ischar = g_pas_pend_ischar;
        pas_pend_reset(); g_pas_pend_esz = -1; g_pas_pend_istfile = 1; (yyval.ival) = -1; }
#line 3712 "pascal.tab.c"
    break;

  case 45: /* packed_opt: PACKEDSY  */
#line 2097 "pascal.y"
                     { (yyval.ival) = 1; }
#line 3718 "pascal.tab.c"
    break;

  case 46: /* packed_opt: %empty  */
#line 2097 "pascal.y"
                                   { (yyval.ival) = 0; }
#line 3724 "pascal.tab.c"
    break;

  case 47: /* simple_type: LPARENT id_list RPARENT  */
#line 2100 "pascal.y"
        { pas_scope_define_list((yyvsp[-1].list)); int _eo = 0; g_pas_pend_enum_names[0] = '\0';
          if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) {
              tree_t *_id = (yyvsp[-1].list)->items[i];
              if (_id && _id->v.sval) { if (_eo > 0) strncat(g_pas_pend_enum_names, ",", sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); strncat(g_pas_pend_enum_names, _id->v.sval, sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); pas_const_add(_id->v.sval, (long long)(_eo++)); } }
          g_pas_pend_enum_max = (long long)(_eo - 1); g_pas_pend_esz = -1;
          (yyval.ival) = _eo > 0 ? (long long)(_eo - 1) : -1; }
#line 3735 "pascal.tab.c"
    break;

  case 48: /* simple_type: IDENT  */
#line 2106 "pascal.y"
            { pas_scope_applied((yyvsp[0].str)); long long _esz; if (!pas_sizeof_lookup((yyvsp[0].str), &_esz)) _esz = -1; g_pas_pend_typename = ct_strdup((yyvsp[0].str)); g_pas_pend_isbool = pas_is_booltype((yyvsp[0].str)); g_pas_pend_istfile = pas_is_tfiletype((yyvsp[0].str)); g_pas_pend_ischar = !strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar") || pas_typealias_is_char((yyvsp[0].str)); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = ct_strdup(_pt); (yyval.ival) = -3; } else { if (!strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar")) { (yyval.ival) = 255; } else if (pas_is_settype((yyvsp[0].str))) { (yyval.ival) = -2; } else { long long _eh = pas_enumtype_high((yyvsp[0].str)); long long _sh = pas_subtype_high((yyvsp[0].str)); long long _ah = pas_arrtype_high((yyvsp[0].str)); if (_eh >= 0) { (yyval.ival) = _eh; } else if (_sh >= 0) { (yyval.ival) = _sh; } else if (_ah >= 0) { if (g_pas_recbody_depth == 0 && pas_rectype_nf((yyvsp[0].str)) > 0) { pas_rectype_to_pend((yyvsp[0].str)); g_pas_pend_typename = ct_strdup((yyvsp[0].str)); } (yyval.ival) = _ah; } else { if (g_pas_recbody_depth == 0) pas_rectype_to_pend((yyvsp[0].str)); g_pas_pend_typename = ct_strdup((yyvsp[0].str)); (yyval.ival) = -1; } } } g_pas_pend_esz = _esz; }
#line 3741 "pascal.tab.c"
    break;

  case 49: /* simple_type: constant DOTDOT constant  */
#line 2107 "pascal.y"
                               { g_pas_pend_sub_low = (yyvsp[-2].ival); g_pas_pend_sub_high = (yyvsp[0].ival); g_pas_pend_ischar = 0; g_pas_pend_esz = ((yyvsp[0].ival) >= (yyvsp[-2].ival)) ? pas_range_fit_size((yyvsp[-2].ival), (yyvsp[0].ival)) : -1; (yyval.ival) = (yyvsp[0].ival); }
#line 3747 "pascal.tab.c"
    break;

  case 50: /* simple_type: STRINGCONST DOTDOT constant  */
#line 2108 "pascal.y"
                                  { long long _l = ((yyvsp[-2].str) && strlen((yyvsp[-2].str)) == 1) ? (long long)(unsigned char)(yyvsp[-2].str)[0] : 0;
          g_pas_pend_sub_low = _l; g_pas_pend_sub_high = (yyvsp[0].ival); g_pas_pend_ischar = 1; g_pas_pend_esz = 1; (yyval.ival) = (yyvsp[0].ival); }
#line 3754 "pascal.tab.c"
    break;

  case 51: /* simple_type: CHARCODE DOTDOT constant  */
#line 2110 "pascal.y"
                               { g_pas_pend_sub_low = (yyvsp[-2].ival); g_pas_pend_sub_high = (yyvsp[0].ival); g_pas_pend_ischar = 1; g_pas_pend_esz = 1; (yyval.ival) = (yyvsp[0].ival); }
#line 3760 "pascal.tab.c"
    break;

  case 55: /* @4: %empty  */
#line 2120 "pascal.y"
                  { (yyval.str) = g_pas_pend_typename; g_pas_pend_typename = NULL; }
#line 3766 "pascal.tab.c"
    break;

  case 56: /* record_field: id_list COLON @4 type  */
#line 2120 "pascal.y"
                                                                                      { pas_scope_define_list((yyvsp[-3].list)); int _syn = 0; if ((yyvsp[0].ival) == -2 && !g_pas_pend_typename) { char _snm[32]; snprintf(_snm, sizeof _snm, "$anonset%d", ++g_pas_scope.anonseq); pas_settype_add(_snm, g_pas_pend_set_hi); g_pas_pend_typename = ct_strdup(_snm); _syn = 1; } if ((yyvsp[-3].list)) { char *_svp = g_pas_pend_ptrtarget; int _svc = g_pas_pend_ischar; int _sva = g_pas_pend_arr_ischar; int _svr = g_pas_pend_isarr; for (int i = 0; i < (yyvsp[-3].list)->count; i++) if ((yyvsp[-3].list)->items[i] && (yyvsp[-3].list)->items[i]->v.sval) { g_pas_pend_ptrtarget = _svp; g_pas_pend_ischar = _svc; g_pas_pend_arr_ischar = _sva; g_pas_pend_isarr = _svr; pas_pend_add((yyvsp[-3].list)->items[i]->v.sval); } } if (!g_pas_pend_typename || _syn) g_pas_pend_typename = (yyvsp[-1].str); }
#line 3772 "pascal.tab.c"
    break;

  case 58: /* case_arm_mark: %empty  */
#line 2124 "pascal.y"
    { (yyval.ival) = g_pas_pend_nf; }
#line 3778 "pascal.tab.c"
    break;

  case 59: /* case_open: %empty  */
#line 2127 "pascal.y"
    { pas_vcase_push(); pas_vt_open(); }
#line 3784 "pascal.tab.c"
    break;

  case 60: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 2130 "pascal.y"
                                                             { pas_vcase_pop(); pas_scope_define((yyvsp[-5].str)); pas_variant_constants_in_tag_type((yyvsp[-3].str), (yyvsp[0].list)); if ((yyvsp[-5].str)) { g_pas_pend_typename = ct_strdup((yyvsp[-3].str)); pas_pend_add((yyvsp[-5].str)); pas_vt_close(g_pas_pend_nf - 1); } else pas_vt_close(-1); }
#line 3790 "pascal.tab.c"
    break;

  case 61: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 2131 "pascal.y"
                                                   { pas_vcase_pop(); pas_variant_constants_in_tag_type((yyvsp[-3].str), (yyvsp[0].list)); if ((yyvsp[-3].str)) pas_pend_add((yyvsp[-3].str)); pas_vt_close(-1); }
#line 3796 "pascal.tab.c"
    break;

  case 63: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 2135 "pascal.y"
                                               { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 3802 "pascal.tab.c"
    break;

  case 64: /* record_case_list: record_case_arm  */
#line 2136 "pascal.y"
                      { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 3808 "pascal.tab.c"
    break;

  case 65: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 2139 "pascal.y"
                                                                  { pas_vcase_arm_end((int)(yyvsp[-2].ival)); pas_vt_arm((int)(yyvsp[-2].ival), g_pas_pend_nf, (yyvsp[-5].node)); (yyval.node) = (yyvsp[-5].node); }
#line 3814 "pascal.tab.c"
    break;

  case 66: /* record_case_arm: %empty  */
#line 2140 "pascal.y"
      { (yyval.node) = NULL; }
#line 3820 "pascal.tab.c"
    break;

  case 67: /* var_decl_list: var_decl_list var_decl  */
#line 2143 "pascal.y"
                           { (yyval.list) = pas_var_names_add((yyvsp[-1].list), (yyvsp[0].list)); }
#line 3826 "pascal.tab.c"
    break;

  case 68: /* var_decl_list: var_decl  */
#line 2144 "pascal.y"
               { (yyval.list) = pas_var_names_add(pnl_new(), (yyvsp[0].list)); }
#line 3832 "pascal.tab.c"
    break;

  case 69: /* var_decl: id_list COLON type SEMICOLON  */
#line 2146 "pascal.y"
                                       { pas_scope_define_list((yyvsp[-3].list)); if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) if ((yyvsp[-3].list)->items[i] && (yyvsp[-3].list)->items[i]->v.sval) pas_tsz_add((yyvsp[-3].list)->items[i]->v.sval, g_pas_pend_esz); long long _ty3 = ((yyvsp[-1].ival) == -4) ? -1 : (yyvsp[-1].ival); int _pk3 = ((yyvsp[-1].ival) == -4) || (g_pas_pend_typename && pas_rectype_is_packed(g_pas_pend_typename)); if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if (_ty3 == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if (_ty3 >= 0 && g_pas_pend_nf > 0) { pas_array_add(id->v.sval, _ty3); pas_arrrec_add(id->v.sval, g_pas_pend_typename, g_pas_pend_nf); } else if (_ty3 >= 0) { long long _varnc = (g_pas_pend_arr_ncols >= 0) ? g_pas_pend_arr_ncols : pas_arrtype_ncols(g_pas_pend_typename); if (g_pas_pend_ischar && !g_pas_pend_arr_ischar && _varnc < 0 && g_pas_pend_nf == 0) { pas_charvar_add(id->v.sval); } else if (_varnc >= 0) { pas_array_add2d(id->v.sval, _ty3, _varnc); } else { pas_array_add(id->v.sval, _ty3); if (g_pas_pend_arr_ptrto) pas_arrptr_add(id->v.sval, g_pas_pend_arr_ptrto); int _aic = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); if (_aic && g_pas_pend_arr_wrap) pas_strarr_add2(id->v.sval, (g_pas_pend_typename && pas_arrtype_lo(g_pas_pend_typename) > 0) ? pas_arrtype_lo(g_pas_pend_typename) : 1); else if (_aic) pas_chararr_add2(id->v.sval, g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : pas_arrtype_lo(g_pas_pend_typename)); else if (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) pas_enumarr_add(id->v.sval, g_pas_pend_typename); } } if (_ty3 == -2) pas_setvar_add(id->v.sval); if (_ty3 < 0 && g_pas_pend_ischar) pas_charvar_add(id->v.sval); if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_pas_pend_isbool) pas_boolvar_add(id->v.sval); if (_ty3 < 0 && g_pas_pend_istfile) { pas_tfilevar_add(id->v.sval); pas_tfcomp_add(id->v.sval, g_pas_pend_frng_lo, g_pas_pend_frng_hi, g_pas_pend_frng_ischar); } if (_ty3 < 0 && !g_pas_pend_istfile && g_pas_pend_nf == 0 && !g_pas_pend_isarr && g_pas_pend_sub_high >= 0) { pas_subvar_add(id->v.sval, g_pas_pend_sub_low, g_pas_pend_sub_high); } else if (_ty3 < 0 && !g_pas_pend_istfile && g_pas_pend_nf == 0 && !g_pas_pend_isarr && g_pas_pend_typename && pas_subtype_high(g_pas_pend_typename) >= 0) { pas_subvar_add(id->v.sval, pas_subtype_low(g_pas_pend_typename), pas_subtype_high(g_pas_pend_typename)); } if (_ty3 < 0 && g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval, _pk3); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "text")) pas_filevar_add(id->v.sval); if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "pchar")) pas_pcharvar_add(id->v.sval); if (g_pas_pend_typename) pas_scalarvartype_add(id->v.sval, g_pas_pend_typename); } pas_local_add(id->v.sval); } } if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "pchar") && (yyvsp[-3].list)) { int _n0 = (yyvsp[-3].list)->count; for (int _hi = 0; _hi < _n0; _hi++) { tree_t *_id = (yyvsp[-3].list)->items[_hi]; if (_id && _id->v.sval) { char *_hn = pas_pchar_off_name(_id->v.sval); pas_scope_define(_hn); pas_local_add(_hn); pnl_push((yyvsp[-3].list), leaf_s(TT_VAR, _hn)); } } } g_pas_pend_frng_lo = 0; g_pas_pend_frng_hi = -1; g_pas_pend_frng_ischar = 0; pas_pend_reset(); (yyval.list) = (yyvsp[-3].list); }
#line 3838 "pascal.tab.c"
    break;

  case 70: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 2148 "pascal.y"
                                                                               { const char *_sg = pas_pf_heading((yyvsp[-5].str), (yyvsp[-3].list), 'P', NULL); pas_pf_define_routine((yyvsp[-5].str), _sg); pas_scope_fwd_save((yyvsp[-5].str), (yyvsp[-3].list), _sg); pas_proc_add((yyvsp[-5].str)); pas_proc_vparams((yyvsp[-5].str), (yyvsp[-3].list)); pas_fwd_save((yyvsp[-5].str), (yyvsp[-3].list)); pas_ptrvar_release(); pas_recvar_release(); }
#line 3844 "pascal.tab.c"
    break;

  case 71: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 2149 "pascal.y"
                                                                                            { const char *_sg = pas_pf_heading((yyvsp[-7].str), (yyvsp[-5].list), 'F', (yyvsp[-3].str)); pas_pf_define_routine((yyvsp[-7].str), _sg); pas_scope_fwd_save((yyvsp[-7].str), (yyvsp[-5].list), _sg); pas_func_add((yyvsp[-7].str)); pas_proc_vparams((yyvsp[-7].str), (yyvsp[-5].list)); if ((yyvsp[-3].str) && !strcmp((yyvsp[-3].str), "char")) pas_charvar_add((yyvsp[-7].str)); if (pas_is_booltype((yyvsp[-3].str))) pas_boolvar_add((yyvsp[-7].str)); if ((yyvsp[-3].str)) pas_scalarvartype_add((yyvsp[-7].str), (yyvsp[-3].str)); pas_fwd_save((yyvsp[-7].str), (yyvsp[-5].list)); pas_ptrvar_release(); pas_recvar_release(); }
#line 3850 "pascal.tab.c"
    break;

  case 72: /* $@5: %empty  */
#line 2150 "pascal.y"
                                                             { pas_pf_define_routine((yyvsp[-3].str), pas_pf_heading((yyvsp[-3].str), (yyvsp[-1].list), 'P', NULL)); pas_scope_enter((yyvsp[-3].str), (yyvsp[-1].list)); pas_proc_add((yyvsp[-3].str)); pas_proc_vparams((yyvsp[-3].str), pas_scope_fwd_params((yyvsp[-3].str), (yyvsp[-1].list))); pas_proc_enter(); pas_fwd_restore((yyvsp[-3].str)); }
#line 3856 "pascal.tab.c"
    break;

  case 73: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 2151 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-6].str), pas_fwd_params((yyvsp[-6].str), (yyvsp[-4].list)), pas_trace_wrap_proc((yyvsp[-6].str), (yyvsp[-4].list), (yyvsp[-1].node), 0), 0, dl, ln, lc); pas_proc_exit(); pas_scope_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3864 "pascal.tab.c"
    break;

  case 74: /* $@6: %empty  */
#line 2154 "pascal.y"
                                                                        { pas_pf_define_routine((yyvsp[-5].str), pas_pf_heading((yyvsp[-5].str), (yyvsp[-3].list), 'F', (yyvsp[-1].str))); pas_scope_enter((yyvsp[-5].str), (yyvsp[-3].list)); pas_func_add((yyvsp[-5].str)); pas_proc_vparams((yyvsp[-5].str), pas_scope_fwd_params((yyvsp[-5].str), (yyvsp[-3].list))); if ((yyvsp[-1].str) && !strcmp((yyvsp[-1].str), "char")) pas_charvar_add((yyvsp[-5].str)); if (pas_is_booltype((yyvsp[-1].str))) pas_boolvar_add((yyvsp[-5].str)); if ((yyvsp[-1].str)) pas_scalarvartype_add((yyvsp[-5].str), (yyvsp[-1].str)); pas_proc_enter(); pas_curfunc_push((yyvsp[-5].str)); pas_fwd_restore((yyvsp[-5].str)); }
#line 3870 "pascal.tab.c"
    break;

  case 75: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 2155 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-8].str), pas_fwd_params((yyvsp[-8].str), (yyvsp[-6].list)), pas_trace_wrap_proc((yyvsp[-8].str), (yyvsp[-6].list), (yyvsp[-1].node), 1), 1, dl, ln, lc); pas_proc_exit(); pas_scope_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3878 "pascal.tab.c"
    break;

  case 76: /* $@7: %empty  */
#line 2158 "pascal.y"
                                         { pas_pf_define_routine((yyvsp[-2].str), pas_pf_heading((yyvsp[-2].str), NULL, 'F', NULL)); pas_scope_enter((yyvsp[-2].str), NULL); pas_func_add((yyvsp[-2].str)); pas_proc_vparams((yyvsp[-2].str), pas_scope_fwd_params((yyvsp[-2].str), NULL)); pas_proc_enter(); pas_curfunc_push((yyvsp[-2].str)); pas_fwd_restore((yyvsp[-2].str)); }
#line 3884 "pascal.tab.c"
    break;

  case 77: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 2159 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), pas_fwd_params((yyvsp[-5].str), pnl_new()), pas_trace_wrap_proc((yyvsp[-5].str), NULL, (yyvsp[-1].node), 1), 1, dl, ln, lc); pas_proc_exit(); pas_scope_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3892 "pascal.tab.c"
    break;

  case 78: /* pv_mark: %empty  */
#line 2164 "pascal.y"
    { pas_ptrvar_mark(); pas_recvar_mark(); }
#line 3898 "pascal.tab.c"
    break;

  case 79: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 2167 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 3904 "pascal.tab.c"
    break;

  case 80: /* parameter_list_opt: %empty  */
#line 2168 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 3910 "pascal.tab.c"
    break;

  case 81: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 2171 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 3916 "pascal.tab.c"
    break;

  case 82: /* parameter_decl_list: parameter_decl  */
#line 2172 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 3922 "pascal.tab.c"
    break;

  case 83: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 2175 "pascal.y"
                                { (yyval.list) = pas_pf_formal((yyvsp[-1].str), pas_pf_sig('P', (yyvsp[0].str), NULL)); }
#line 3928 "pascal.tab.c"
    break;

  case 84: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 2176 "pascal.y"
                                             { (yyval.list) = pas_pf_formal((yyvsp[-3].str), pas_pf_sig('F', (yyvsp[-2].str), (yyvsp[0].str))); }
#line 3934 "pascal.tab.c"
    break;

  case 85: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 2177 "pascal.y"
                                { pas_pf_section('r', (yyvsp[-2].list), (yyvsp[0].str)); if (!strcmp((yyvsp[0].str), "text")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_filevar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0 && _ah >= 0) { pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); pas_arrrec_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); } else if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); } } } for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 3940 "pascal.tab.c"
    break;

  case 86: /* parameter_decl: id_list COLON IDENT  */
#line 2178 "pascal.y"
                          { pas_pf_section('v', (yyvsp[-2].list), (yyvsp[0].str)); if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); if (!strcmp((yyvsp[0].str), "char")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_charvar_add((yyvsp[-2].list)->items[i]->v.sval); if (!strcmp((yyvsp[0].str), "single")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_singlevar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_realtypename((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_FLIT)); if (!strcmp((yyvsp[0].str), "pchar")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) { ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_QLIT)); if ((yyvsp[-2].list)->items[i]->v.sval) pas_pcharvar_add((yyvsp[-2].list)->items[i]->v.sval); } { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0 && _ah >= 0) { pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); pas_arrrec_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); } else if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); } } } (yyval.list) = (yyvsp[-2].list); }
#line 3946 "pascal.tab.c"
    break;

  case 87: /* pf_params: LPARENT pf_sections RPARENT  */
#line 2181 "pascal.y"
                                { (yyval.str) = (yyvsp[-1].str); }
#line 3952 "pascal.tab.c"
    break;

  case 88: /* pf_params: %empty  */
#line 2182 "pascal.y"
      { (yyval.str) = ""; }
#line 3958 "pascal.tab.c"
    break;

  case 89: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 2185 "pascal.y"
                                     { (yyval.str) = pas_pf_cat3((yyvsp[-2].str), ";", (yyvsp[0].str)); }
#line 3964 "pascal.tab.c"
    break;

  case 90: /* pf_sections: pf_section  */
#line 2186 "pascal.y"
                 { (yyval.str) = (yyvsp[0].str); }
#line 3970 "pascal.tab.c"
    break;

  case 91: /* pf_section: id_list COLON IDENT  */
#line 2189 "pascal.y"
                        { (yyval.str) = pas_pf_sect('v', (yyvsp[-2].list)->count, (yyvsp[0].str)); }
#line 3976 "pascal.tab.c"
    break;

  case 92: /* pf_section: VARSY id_list COLON IDENT  */
#line 2190 "pascal.y"
                                { (yyval.str) = pas_pf_sect('r', (yyvsp[-2].list)->count, (yyvsp[0].str)); }
#line 3982 "pascal.tab.c"
    break;

  case 93: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 2191 "pascal.y"
                                  { (yyval.str) = pas_pf_sig('P', (yyvsp[0].str), NULL); }
#line 3988 "pascal.tab.c"
    break;

  case 94: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 2192 "pascal.y"
                                             { (yyval.str) = pas_pf_sig('F', (yyvsp[-2].str), (yyvsp[0].str)); }
#line 3994 "pascal.tab.c"
    break;

  case 95: /* id_list: id_list COMMA IDENT  */
#line 2195 "pascal.y"
                        { pas_not_a_word_symbol((yyvsp[0].str)); pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 4000 "pascal.tab.c"
    break;

  case 96: /* id_list: IDENT  */
#line 2196 "pascal.y"
            { pas_not_a_word_symbol((yyvsp[0].str)); PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 4006 "pascal.tab.c"
    break;

  case 97: /* body: BEGINSY statement_list ENDSY  */
#line 2199 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 4012 "pascal.tab.c"
    break;

  case 98: /* $@8: %empty  */
#line 2202 "pascal.y"
                             { pas_stmt_mark(); }
#line 4018 "pascal.tab.c"
    break;

  case 99: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 2202 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node)) { if (pas_trace_enabled() && (yyvsp[0].node)->t != TT_SUCCEED) pnl_push((yyvsp[-3].list), mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push((yyvsp[-3].list), (yyvsp[0].node)); } (yyval.list) = (yyvsp[-3].list); }
#line 4024 "pascal.tab.c"
    break;

  case 100: /* $@9: %empty  */
#line 2203 "pascal.y"
      { pas_stmt_mark(); }
#line 4030 "pascal.tab.c"
    break;

  case 101: /* statement_list: $@9 statement  */
#line 2203 "pascal.y"
                                     { int _ln = pas_stmt_line_pop(); PNodeList *l = pnl_new(); if ((yyvsp[0].node)) { if (pas_trace_enabled() && (yyvsp[0].node)->t != TT_SUCCEED) pnl_push(l, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(l, (yyvsp[0].node)); } (yyval.list) = l; }
#line 4036 "pascal.tab.c"
    break;

  case 102: /* statement: statement_no_label  */
#line 2206 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 4042 "pascal.tab.c"
    break;

  case 103: /* statement: INTCONST COLON statement_no_label  */
#line 2208 "pascal.y"
        { pas_label_in_range((yyvsp[-2].ival)); pas_label_prefix((yyvsp[-2].ival)); char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)(yyvsp[-2].ival));
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = ct_strdup(_lb); ast_push(L, (yyvsp[0].node)); (yyval.node) = L; }
#line 4049 "pascal.tab.c"
    break;

  case 104: /* statement_no_label: assignment  */
#line 2212 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4055 "pascal.tab.c"
    break;

  case 105: /* statement_no_label: call  */
#line 2213 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4061 "pascal.tab.c"
    break;

  case 106: /* statement_no_label: compound_statement  */
#line 2214 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 4067 "pascal.tab.c"
    break;

  case 107: /* statement_no_label: goto_statement  */
#line 2215 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4073 "pascal.tab.c"
    break;

  case 108: /* statement_no_label: if_statement  */
#line 2216 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4079 "pascal.tab.c"
    break;

  case 109: /* statement_no_label: case_statement  */
#line 2217 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4085 "pascal.tab.c"
    break;

  case 110: /* statement_no_label: while_statement  */
#line 2218 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4091 "pascal.tab.c"
    break;

  case 111: /* statement_no_label: repeat_statement  */
#line 2219 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 4097 "pascal.tab.c"
    break;

  case 112: /* statement_no_label: for_statement  */
#line 2220 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4103 "pascal.tab.c"
    break;

  case 113: /* statement_no_label: with_statement  */
#line 2221 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4109 "pascal.tab.c"
    break;

  case 114: /* statement_no_label: %empty  */
#line 2222 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 4115 "pascal.tab.c"
    break;

  case 115: /* call: IDENT  */
#line 2225 "pascal.y"
          { if (pas_pf_is_formal((yyvsp[0].str))) (yyval.node) = pas_pf_callthrough((yyvsp[0].str), NULL); else if (pas_is_proc((yyvsp[0].str))) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.node) = e; } else { pas_required_needs_params((yyvsp[0].str)); (yyval.node) = mk_call((yyvsp[0].str), NULL); } }
#line 4121 "pascal.tab.c"
    break;

  case 116: /* call: call_with_args  */
#line 2226 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4127 "pascal.tab.c"
    break;

  case 117: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 2229 "pascal.y"
                                        { (yyvsp[-1].list) = pas_pf_actuals((yyvsp[-3].str), (yyvsp[-1].list)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) if (pas_proc_param_is_var((yyvsp[-3].str), i) && pas_actual_is_packed_component((yyvsp[-1].list)->items[i])) {
            fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a component of a packed structure is passed as the variable parameter %d of '%s'\n", i + 1, (yyvsp[-3].str));
            g_pas_iso_errors++; }
        pas_call_arity((yyvsp[-3].str), (yyvsp[-1].list)); pas_ordinal_fn_arg((yyvsp[-3].str), (yyvsp[-1].list));
        (yyval.node) = pas_pf_is_formal((yyvsp[-3].str)) ? pas_pf_callthrough((yyvsp[-3].str), (yyvsp[-1].list)) : mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 4137 "pascal.tab.c"
    break;

  case 118: /* argument_list: argument_list COMMA argument  */
#line 2236 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 4143 "pascal.tab.c"
    break;

  case 119: /* argument_list: argument  */
#line 2237 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 4149 "pascal.tab.c"
    break;

  case 120: /* argument: expression  */
#line 2240 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 4155 "pascal.tab.c"
    break;

  case 121: /* argument: expression COLON expression  */
#line 2241 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 4161 "pascal.tab.c"
    break;

  case 122: /* argument: expression COLON expression COLON expression  */
#line 2242 "pascal.y"
                                                   { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-4].node))); pnl_push(_al, ilit(-3)); pnl_push(_al, (yyvsp[-2].node)); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 4167 "pascal.tab.c"
    break;

  case 123: /* assignment: selector BECOMES expression  */
#line 2246 "pascal.y"
        { if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) pas_value_compat((yyvsp[-2].node)->v.sval, (yyvsp[0].node), "6.8.2.2", "the variable");
          tree_t *_tl = pas_trace_lhs((yyvsp[-2].node));
          if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_FNC && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->v.sval && (!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_fbuf_get") || !strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get"))) {
              int _isqlit = (yyvsp[0].node) && ((yyvsp[0].node)->t == TT_QLIT || ((yyvsp[0].node)->t == TT_FNC && (yyvsp[0].node)->n >= 1 && (yyvsp[0].node)->c[0] && (yyvsp[0].node)->c[0]->v.sval && !strcmp((yyvsp[0].node)->c[0]->v.sval, "__pas_chrlit")));
              if (!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_fbuf_get") && (yyvsp[-2].node)->c[1] && (yyvsp[-2].node)->c[1]->t == TT_VAR && (yyvsp[-2].node)->c[1]->v.sval
                  && _isqlit && pas_tfcomp_nonchar((yyvsp[-2].node)->c[1]->v.sval)) {
                  fprintf(stderr, "pascal: ISO 7185 6.6.5.2 violation: the value assigned to the buffer-variable of '%s' is not assignment-compatible with its component-type\n", (yyvsp[-2].node)->c[1]->v.sval);
                  g_pas_iso_errors++;
              }
              (yyval.node) = pas_trace_assigned(_tl, mk_set_bin(!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get") ? "__pas_tbuf_set" : "__pas_fbuf_set", (yyvsp[-2].node)->c[1], pas_bool((yyvsp[0].node))));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval && pas_is_chararr((yyvsp[-2].node)->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _cah; if (!pas_array_high_get((yyvsp[-2].node)->v.sval, &_cah)) _cah = (long long)strlen((yyvsp[0].node)->v.sval);
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, pas_chararr_lo((yyvsp[-2].node)->v.sval), _cah)));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR && (yyvsp[-2].node)->c[0]->v.sval && pas_is_strarr((yyvsp[-2].node)->c[0]->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _slo = pas_strarr_lo((yyvsp[-2].node)->c[0]->v.sval); long long _shi = _slo + (long long)strlen((yyvsp[0].node)->v.sval) - 1;
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, _slo, _shi)));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR && (yyvsp[-2].node)->c[0]->v.sval && pas_is_strarr((yyvsp[-2].node)->c[0]->v.sval) && pas_ca_is_read((yyvsp[0].node))) {
              tree_t *_en = ast_node_new(TT_FNC); ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode")); ast_push(_en, (yyvsp[0].node)); ast_push(_en, ilit(pas_strarr_lo((yyvsp[-2].node)->c[0]->v.sval))); ast_push(_en, ilit(-1));
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), _en));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval && pas_is_chararr((yyvsp[-2].node)->v.sval) && pas_ca_is_read((yyvsp[0].node))) {
              long long _ceh; if (!pas_array_high_get((yyvsp[-2].node)->v.sval, &_ceh)) _ceh = -1;
              tree_t *_en = ast_node_new(TT_FNC); ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode")); ast_push(_en, (yyvsp[0].node)); ast_push(_en, ilit(pas_chararr_lo((yyvsp[-2].node)->v.sval))); ast_push(_en, ilit(_ceh));
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), _en));
          } else if (pas_is_cafield((yyvsp[-2].node))) {
              tree_t *_rhs; long long _flo = pas_cafield_lo_get((yyvsp[-2].node));
              if ((yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) { long long _fhi = pas_cafield_hi_get((yyvsp[-2].node)); if (_fhi < _flo) _fhi = _flo + (long long)strlen((yyvsp[0].node)->v.sval) - 1; _rhs = pas_str_padded((yyvsp[0].node)->v.sval, _fhi - _flo + 1); }
              else _rhs = pas_bool((yyvsp[0].node));
              tree_t *_pk = _rhs; if (!pas_ca_is_read(_rhs) && _rhs->t != TT_QLIT) { _pk = ast_node_new(TT_FNC); ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack")); ast_push(_pk, _rhs); ast_push(_pk, ilit(_flo)); }
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), _pk));
          } else { tree_t *_rhs0 = pas_bool((yyvsp[0].node));
              if (g_pas_range_check_on && (yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) { long long _rlo, _rhi; if (pas_subvar_get((yyvsp[-2].node)->v.sval, &_rlo, &_rhi)) _rhs0 = pas_range_wrap(_rhs0, _rlo, _rhi, pas_is_setvar((yyvsp[-2].node)->v.sval) ? "set" : "6.8.2.2"); }
              (yyval.node) = pas_trace_assigned(_tl, mk_assign((yyvsp[-2].node), _rhs0)); } (yyval.node) = pas_vt_wrap_stmt((yyvsp[-2].node), (yyvsp[0].node), (yyval.node)); }
#line 4204 "pascal.tab.c"
    break;

  case 124: /* selector: selector LBRACK expression_list RBRACK  */
#line 2280 "pascal.y"
                                           { tree_t *e = NULL; if ((yyvsp[-1].list) && (yyvsp[-1].list)->count == 2 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { long long _nc = pas_array_ncols((yyvsp[-3].node)->v.sval); if (_nc > 0) { tree_t *flat = bin(TT_ADD, bin(TT_MUL, (yyvsp[-1].list)->items[0], ilit(_nc)), (yyvsp[-1].list)->items[1]); e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); ast_push(e, flat); } } if (!e && (yyvsp[-1].list) && (yyvsp[-1].list)->count == 1 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_IDX && (yyvsp[-3].node)->n == 2 && (yyvsp[-3].node)->c[0] && (yyvsp[-3].node)->c[0]->t == TT_VAR && (yyvsp[-3].node)->c[0]->v.sval && !pas_is_nafield((yyvsp[-3].node))) {
        long long _nc2 = pas_array_ncols((yyvsp[-3].node)->c[0]->v.sval);
        if (_nc2 > 0) { tree_t *flat2 = bin(TT_ADD, bin(TT_MUL, (yyvsp[-3].node)->c[1], ilit(_nc2)), (yyvsp[-1].list)->items[0]);
          e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)->c[0]); ast_push(e, flat2); } }
      if (!e) { e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, ((yyvsp[-1].list)->count == 1 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval && pas_is_pcharvar((yyvsp[-3].node)->v.sval)) ? bin(TT_ADD, bin(TT_ADD, (yyvsp[-1].list)->items[i], leaf_s(TT_VAR, pas_pchar_off_name((yyvsp[-3].node)->v.sval))), ilit(1)) : ((yyvsp[-1].list)->count == 1 && (g_pas_zerobased_strings & 1) && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval && pas_var_string_kind((yyvsp[-3].node)->v.sval) == 1) ? bin(TT_ADD, (yyvsp[-1].list)->items[i], ilit(1)) : (yyvsp[-1].list)->items[i]); } if (e && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { const char *_et = pas_enumarr_get((yyvsp[-3].node)->v.sval); if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } (yyval.node) = e; }
#line 4214 "pascal.tab.c"
    break;

  case 125: /* selector: selector PERIOD IDENT  */
#line 2285 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi < 0 && (yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR) { const char *_arn = NULL; int _anf = pas_arrrec_find((yyvsp[-2].node)->c[0]->v.sval, &_arn); if (_anf > 0) { int _afi = _arn ? pas_rectype_field_index(_arn, (yyvsp[0].str)) : -1; if (_afi < 0) { _afi = pas_arrrec_field_index((yyvsp[-2].node)->c[0]->v.sval, (yyvsp[0].str)); } if (_afi < 0) { for (int _ri = 0; _ri < g_pas_nrectype; _ri++) { int _t = pas_rectype_field_index(g_pas_rectypes[_ri].tname, (yyvsp[0].str)); if (_t >= 0 && g_pas_rectypes[_ri].nf == _anf) { _afi = _t; break; } } } if (_afi >= 0) { (yyval.node) = pas_arrrec_flatten((yyvsp[-2].node), _afi); if (pas_arrrec_field_is_char((yyvsp[-2].node)->c[0]->v.sval, _afi) || (_arn && pas_rectype_field_is_char(_arn, _afi))) pas_cvfield_mark_add((yyval.node)); const char *_fe = pas_arrrec_field_enum((yyvsp[-2].node)->c[0]->v.sval, _afi); if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi); if (_fe && (yyval.node)) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) (yyval.node)->v.ival = (long long)(_ei + 1); } } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
        else if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_rt ? pas_rectype_slot(_rt, _fi) : ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) ? pas_recvar_slot((yyvsp[-2].node)->v.sval, _fi) : _fi)); if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } { const char *_mrt = _rt ? _rt : pas_with_sel_rtype((yyvsp[-2].node)); if (_mrt && pas_rectype_field_is_ca(_mrt, _fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_mrt, _fi), pas_rectype_field_ca_hi(_mrt, _fi)); if (_mrt && pas_rectype_field_is_char(_mrt, _fi)) pas_cvfield_mark_add(e);
            if (_mrt && pas_rectype_field_is_na(_mrt, _fi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_mrt, _fi)); } pas_vt_mark(e, (yyvsp[-2].node), _rt, _fi); (yyval.node) = e; } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
#line 4223 "pascal.tab.c"
    break;

  case 126: /* selector: selector ARROW  */
#line 2289 "pascal.y"
                     { (yyval.node) = pas_is_tfile_node((yyvsp[-1].node)) ? mk_fnc1("__pas_fbuf_get", (yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && pas_is_filevar((yyvsp[-1].node)->v.sval) && !pas_is_stdstream((yyvsp[-1].node)->v.sval)) ? mk_fnc1("__pas_tbuf_get", (yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && pas_is_pcharvar((yyvsp[-1].node)->v.sval)) ? pas_pchar_deref((yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && !strcmp((yyvsp[-1].node)->v.sval, "input") && !pas_ptrvar_target((yyvsp[-1].node)->v.sval)) ? mk_fnc0("__pas_getbufch") : mk_deref((yyvsp[-1].node))))); }
#line 4229 "pascal.tab.c"
    break;

  case 127: /* selector: IDENT  */
#line 2290 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 4235 "pascal.tab.c"
    break;

  case 128: /* expression_list: expression_list COMMA expression  */
#line 2293 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 4241 "pascal.tab.c"
    break;

  case 129: /* expression_list: expression  */
#line 2294 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 4247 "pascal.tab.c"
    break;

  case 130: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 2297 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 4253 "pascal.tab.c"
    break;

  case 131: /* goto_statement: GOTOSY INTCONST  */
#line 2301 "pascal.y"
        { pas_label_find((yyvsp[0].ival)); char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)(yyvsp[0].ival));
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = ct_strdup(_gb); G->line = pascal_get_lineno(); (yyval.node) = G; }
#line 4260 "pascal.tab.c"
    break;

  case 132: /* if_statement: IFSY expression THENSY statement  */
#line 2305 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond_bool((yyvsp[-2].node), "an if-statement", "6.8.3.4"), (yyvsp[0].node)); }
#line 4266 "pascal.tab.c"
    break;

  case 133: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 2306 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond_bool((yyvsp[-4].node), "an if-statement", "6.8.3.4")); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 4272 "pascal.tab.c"
    break;

  case 134: /* $@10: %empty  */
#line 2309 "pascal.y"
                           { pas_case_push(); }
#line 4278 "pascal.tab.c"
    break;

  case 135: /* case_statement: CASESY expression OFSY $@10 case_list ENDSY  */
#line 2310 "pascal.y"
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), (yyvsp[-4].node)));
          tree_t *chain = ast_node_new(TT_FNC); ast_push(chain, leaf_s(TT_VAR, "__pas_rterr")); ast_push(chain, leaf_s(TT_QLIT, "6.8.3.5"));
          ast_push(chain, leaf_s(TT_QLIT, "no case-constant of the case-statement is equal to the value of its case-index"));
          if ((yyvsp[-1].list)) for (int i = (yyvsp[-1].list)->count - 1; i >= 0; i--) { tree_t *e = (yyvsp[-1].list)->items[i]; if (!e) continue; ast_push(e, chain); chain = e; }
          ast_push(seq, chain);
          pas_case_pop();
          (yyval.node) = seq; }
#line 4291 "pascal.tab.c"
    break;

  case 136: /* case_list: case_list SEMICOLON case_elem  */
#line 2320 "pascal.y"
                                  { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 4297 "pascal.tab.c"
    break;

  case 137: /* case_list: case_elem  */
#line 2321 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 4303 "pascal.tab.c"
    break;

  case 138: /* case_elem: constant_list COLON statement  */
#line 2324 "pascal.y"
                                  { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4309 "pascal.tab.c"
    break;

  case 139: /* case_elem: %empty  */
#line 2325 "pascal.y"
      { (yyval.node) = NULL; }
#line 4315 "pascal.tab.c"
    break;

  case 140: /* constant_list: constant_list COMMA constant  */
#line 2328 "pascal.y"
                                 { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival)))); }
#line 4321 "pascal.tab.c"
    break;

  case 141: /* constant_list: constant  */
#line 2329 "pascal.y"
               { (yyval.node) = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival))); }
#line 4327 "pascal.tab.c"
    break;

  case 142: /* while_statement: WHILESY expression DOSY statement  */
#line 2332 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond_bool((yyvsp[-2].node), "a while-statement", "6.8.3.8"), (yyvsp[0].node)); }
#line 4333 "pascal.tab.c"
    break;

  case 143: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 2335 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond_bool((yyvsp[0].node), "a repeat-statement", "6.8.3.7")); }
#line 4339 "pascal.tab.c"
    break;

  case 144: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 2339 "pascal.y"
        { pas_scope_require((yyvsp[-6].str)); pas_value_compat((yyvsp[-6].str), (yyvsp[-4].node), "6.8.3.9", "the control-variable"); pas_value_compat((yyvsp[-6].str), (yyvsp[-2].node), "6.8.3.9", "the control-variable");
          if (pas_var_is_real((yyvsp[-6].str))) { fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", (yyvsp[-6].str)); g_pas_iso_errors++; }
          pas_for_const_bounds((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-2].node), 0);
          tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, pas_trace_wrap_for_body((yyvsp[-6].str), (yyvsp[0].node))); (yyval.node) = e; }
#line 4348 "pascal.tab.c"
    break;

  case 145: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 2344 "pascal.y"
        { pas_scope_require((yyvsp[-6].str)); pas_value_compat((yyvsp[-6].str), (yyvsp[-4].node), "6.8.3.9", "the control-variable"); pas_value_compat((yyvsp[-6].str), (yyvsp[-2].node), "6.8.3.9", "the control-variable");
          if (pas_var_is_real((yyvsp[-6].str))) { fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", (yyvsp[-6].str)); g_pas_iso_errors++; }
          pas_for_const_bounds((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-2].node), 1);
          tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, pas_trace_wrap_for_body((yyvsp[-6].str), (yyvsp[0].node))); e->v.ival = 1; (yyval.node) = e; }
#line 4357 "pascal.tab.c"
    break;

  case 146: /* with_statement: WITHSY with_open DOSY statement  */
#line 2350 "pascal.y"
                                    { long long n = (yyvsp[-2].ival); for (long long i = 0; i < n; i++) pas_with_pop(); (yyval.node) = (yyvsp[0].node); }
#line 4363 "pascal.tab.c"
    break;

  case 147: /* with_open: with_open COMMA selector  */
#line 2353 "pascal.y"
                             { pas_with_push((yyvsp[0].node)); (yyval.ival) = (yyvsp[-2].ival) + 1; }
#line 4369 "pascal.tab.c"
    break;

  case 148: /* with_open: selector  */
#line 2354 "pascal.y"
               { pas_with_push((yyvsp[0].node)); (yyval.ival) = 1; }
#line 4375 "pascal.tab.c"
    break;

  case 149: /* expression: simple_expression  */
#line 2361 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4381 "pascal.tab.c"
    break;

  case 150: /* expression: expression INOP simple_expression  */
#line 2362 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4387 "pascal.tab.c"
    break;

  case 151: /* expression: expression LTOP simple_expression  */
#line 2363 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4393 "pascal.tab.c"
    break;

  case 152: /* expression: expression LEOP simple_expression  */
#line 2364 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4399 "pascal.tab.c"
    break;

  case 153: /* expression: expression GTOP simple_expression  */
#line 2365 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4405 "pascal.tab.c"
    break;

  case 154: /* expression: expression GEOP simple_expression  */
#line 2366 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4411 "pascal.tab.c"
    break;

  case 155: /* expression: expression NEOP simple_expression  */
#line 2367 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_NE, "__pas_setne", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4417 "pascal.tab.c"
    break;

  case 156: /* expression: expression EQOP simple_expression  */
#line 2368 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_EQ, "__pas_seteq", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4423 "pascal.tab.c"
    break;

  case 157: /* simple_expression: term  */
#line 2371 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4429 "pascal.tab.c"
    break;

  case 158: /* simple_expression: PLUS term  */
#line 2372 "pascal.y"
                { pas_sign_operand((yyvsp[0].node), '+'); (yyval.node) = (yyvsp[0].node); }
#line 4435 "pascal.tab.c"
    break;

  case 159: /* simple_expression: MINUS term  */
#line 2373 "pascal.y"
                 { pas_sign_operand((yyvsp[0].node), '-'); (yyval.node) = mk_neg((yyvsp[0].node)); }
#line 4441 "pascal.tab.c"
    break;

  case 160: /* simple_expression: simple_expression PLUS term  */
#line 2374 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4447 "pascal.tab.c"
    break;

  case 161: /* simple_expression: simple_expression MINUS term  */
#line 2375 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4453 "pascal.tab.c"
    break;

  case 162: /* simple_expression: simple_expression OROP term  */
#line 2376 "pascal.y"
                                  { (yyval.node) = (pas_is_boolexpr((yyvsp[-2].node)) && pas_is_boolexpr((yyvsp[0].node))) ? bin(TT_ALT, (yyvsp[-2].node), (yyvsp[0].node)) : mk_fnc2("ior", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4459 "pascal.tab.c"
    break;

  case 163: /* term: factor  */
#line 2379 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4465 "pascal.tab.c"
    break;

  case 164: /* term: term MUL factor  */
#line 2380 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4471 "pascal.tab.c"
    break;

  case 165: /* term: term RDIV factor  */
#line 2381 "pascal.y"
                       { (yyval.node) = pas_rdiv((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4477 "pascal.tab.c"
    break;

  case 166: /* term: term IDIV factor  */
#line 2382 "pascal.y"
                       { (yyval.node) = pas_is_qword((yyvsp[-2].node)) ? mk_fnc2("__pas_udiv", (yyvsp[-2].node), (yyvsp[0].node)) : bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4483 "pascal.tab.c"
    break;

  case 167: /* term: term IMOD factor  */
#line 2383 "pascal.y"
                       { (yyval.node) = pas_is_qword((yyvsp[-2].node)) ? mk_fnc2("__pas_umod", (yyvsp[-2].node), (yyvsp[0].node)) : pas_mod((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4489 "pascal.tab.c"
    break;

  case 168: /* term: term ANDOP factor  */
#line 2384 "pascal.y"
                        { (yyval.node) = (pas_is_boolexpr((yyvsp[-2].node)) && pas_is_boolexpr((yyvsp[0].node))) ? bin(TT_CONJ, (yyvsp[-2].node), (yyvsp[0].node)) : mk_fnc2("iand", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4495 "pascal.tab.c"
    break;

  case 169: /* factor: selector  */
#line 2387 "pascal.y"
             { if (pas_is_cafield((yyvsp[0].node))) { tree_t *u = ast_node_new(TT_FNC); ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack")); ast_push(u, (yyvsp[0].node)); ast_push(u, ilit(pas_cafield_lo_get((yyvsp[0].node)))); (yyval.node) = u; } else (yyval.node) = pas_vt_wrap_read((yyvsp[0].node)); }
#line 4501 "pascal.tab.c"
    break;

  case 170: /* factor: call_with_args  */
#line 2388 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4507 "pascal.tab.c"
    break;

  case 171: /* factor: INTCONST  */
#line 2389 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 4513 "pascal.tab.c"
    break;

  case 172: /* factor: REALCONST  */
#line 2390 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 4519 "pascal.tab.c"
    break;

  case 173: /* factor: STRINGCONST  */
#line 2391 "pascal.y"
                  { if ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((long long)(unsigned char)(yyvsp[0].str)[0])); (yyval.node) = _cl; } else (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 4525 "pascal.tab.c"
    break;

  case 174: /* factor: CHARCODE  */
#line 2392 "pascal.y"
               { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((yyvsp[0].ival))); (yyval.node) = _cl; }
#line 4531 "pascal.tab.c"
    break;

  case 175: /* factor: LPARENT expression RPARENT  */
#line 2393 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4537 "pascal.tab.c"
    break;

  case 176: /* factor: NOTSY factor  */
#line 2394 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 4543 "pascal.tab.c"
    break;

  case 177: /* factor: ATSIGN factor  */
#line 2395 "pascal.y"
                    { (yyval.node) = pas_addr_of_proc((yyvsp[0].node)); }
#line 4549 "pascal.tab.c"
    break;

  case 178: /* factor: LBRACK RBRACK  */
#line 2396 "pascal.y"
                    { (yyval.node) = mk_set_ctor(NULL); }
#line 4555 "pascal.tab.c"
    break;

  case 179: /* factor: LBRACK set_member_list RBRACK  */
#line 2397 "pascal.y"
                                    { (yyval.node) = (yyvsp[-1].node); }
#line 4561 "pascal.tab.c"
    break;

  case 180: /* set_member_list: set_member  */
#line 2400 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4567 "pascal.tab.c"
    break;

  case 181: /* set_member_list: set_member_list COMMA set_member  */
#line 2401 "pascal.y"
                                       { (yyval.node) = mk_set_bin("__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4573 "pascal.tab.c"
    break;

  case 182: /* set_member: expression  */
#line 2404 "pascal.y"
               { pas_set_member_ordinal((yyvsp[0].node)); PNodeList *_l = pnl_new(); pnl_push(_l, (yyvsp[0].node)); (yyval.node) = mk_set_ctor(_l); }
#line 4579 "pascal.tab.c"
    break;

  case 183: /* set_member: expression DOTDOT expression  */
#line 2405 "pascal.y"
                                   { pas_set_member_ordinal((yyvsp[-2].node)); pas_set_member_ordinal((yyvsp[0].node)); (yyval.node) = mk_set_bin("__pas_setrange", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4585 "pascal.tab.c"
    break;


#line 4589 "pascal.tab.c"

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

#line 2411 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    g_pas_seen_mode_directive = 0; g_pas_mode_iso = 0; pascal_seen_decl_start = 0; g_pas_align_mac68k = 0; g_pas_pack_records = 0; g_pas_zerobased_strings = 0;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs); memset(&g_pas_scope, 0, sizeof g_pas_scope);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0; g_pas_ncaparm = 0; g_pas_pend_arr_ncols = -1;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0; g_pas_nsettype = 0; g_pas_ncharvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; g_pas_narrtype = 0; g_pas_nboolvar = 0; g_pas_nbooltype = 0; g_pas_pend_isbool = 0; g_pas_ntfilevar = 0; g_pas_ntfiletype = 0; g_pas_pend_istfile = 0;
    g_pas_nenum = 0; g_pas_pend_enum_max = -1; g_pas_nsubtype = 0; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0; g_with_depth = 0;
    g_pas_nchararr = 0; g_pas_pend_arr_ischar = 0; g_pas_nrconst = 0; g_pas_nsconst = 0; g_pas_narrrec = 0; g_pas_nrvmark = 0; g_pas_nnafield = 0; g_pas_ncafield = 0; g_pas_ncvfield = 0; g_pas_narrptr = 0; g_pas_pend_arr_ptrto = NULL;
    g_pas_nenumname = 0; g_pas_nenumarr = 0; g_pas_pend_enum_names[0] = '\0';
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
