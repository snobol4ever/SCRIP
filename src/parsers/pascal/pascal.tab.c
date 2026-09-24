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
static int pas_array_high_get(const char *name, long long *out);
static long long pas_array_low(const char *name);
static int pas_subvar_get(const char *n, long long *lo, long long *hi);
static int pas_sizeof_lookup(const char *name, long long *out);
static int pas_sizeof_builtin_size(const char *n, long long *out);
static int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi);
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi);
static int pas_tfcomp_nonchar(const char *n);
static int pas_is_hdrfile(const char *n);
static void pas_assigned_add(const char *n);
static int pas_assigned_get(const char *n);
static int pas_array_is_param(const char *name);
static const char *pas_selector_base_name(tree_t *e);
extern int g_pas_iso_errors;
extern int g_pas_seen_mode_directive;
extern int pascal_seen_decl_start;
extern int g_pas_min_enum_size;
extern int g_pas_pack_set_size;
extern int g_pas_range_check_on;
extern int g_pas_align_mac68k;
extern int g_pas_codepage;
static int pas_rectype_total_size(const char *rn, long long *out);
static int pas_rectype_field_offset(const char *rn, int idx, long long *out);
static const char *pas_typealias_get(const char *n);
static long long g_pas_pend_sub_low;
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static int pas_is_singlevar(const char *name);
static int pas_is_pcharvar(const char *name);
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
static int pas_is_stdstream(const char *name);
static void pas_filevar_add(const char *name);
static int pas_is_chararr(const char *name);
static long long pas_chararr_lo(const char *name);
static int pas_is_strarr(const char *name);
static long long pas_strarr_lo(const char *name);
static tree_t *pas_alpha_wrap(tree_t *x);
static int pas_ca_is_read(const tree_t *e);
static tree_t *pas_trace_wrap_value(tree_t *val);
static const char *pas_scalarvartype_get(const char *vn);
static const char *pas_typealias_get(const char *n);
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
    if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); return _w; }
    if (pas_is_charexpr(val)) return mk_chr_wrap(val);
    if (pas_is_boolexpr(val)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, "false,true")); return _w; }
    if (val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) return pas_alpha_wrap(val);
    if (pas_ca_is_read(val)) return pas_alpha_wrap(val);
    if (val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str")); ast_push(_w, val); ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval))); return _w; }
    return val;
}
static tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b);
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
static tree_t *pas_addr_of_proc(tree_t *e);
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi);
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
          if (src && src->t == TT_VAR && src->v.sval && !pas_array_is_param(src->v.sval) && !pas_assigned_get(src->v.sval)) {
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
    if (name && !strcmp(name, "chr") && args && args->count >= 1) return mk_fnc1("__pas_chrlit", args->items[0]);
    if (name && !strcmp(name, "pred") && args && args->count >= 1) return bin(TT_SUB, args->items[0], ilit(1));
    if (name && !strcmp(name, "succ") && args && args->count >= 1) return bin(TT_ADD, args->items[0], ilit(1));
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        tree_t *delta = (args->count >= 3) ? args->items[2] : ilit(1);
        tree_e op = !strcmp(name, "inc") ? TT_ADD : TT_SUB;
        return mk_assign(v, bin(op, pas_tree_clone(v), delta));
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        long long hi;
        if (v && v->t == TT_VAR && v->v.sval && pas_array_high_get(v->v.sval, &hi)) {
            return ilit(!strcmp(name, "low") ? 0 : hi);
        }
        if (v && v->t == TT_VAR && v->v.sval) {
            long long lo2, hi2;
            if (pas_ordinal_bound_lookup(v->v.sval, &lo2, &hi2)) return ilit(!strcmp(name, "low") ? lo2 : hi2);
        }
    }
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
        if ((!_isboolfam || pas_is_boolexpr(args->items[0])) && pas_sizeof_builtin_size(_tcn, &_tsz)) return args->items[0];
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
        if (dst && dst->t == TT_VAR && dst->v.sval) {
            long long fhi;
            if (pas_array_high_get(dst->v.sval, &fhi)) {
                long long flo = pas_array_low(dst->v.sval);
                static int _fcn = 0; char _fcb[24]; snprintf(_fcb, sizeof _fcb, "__pas_fc%d", _fcn++); const char *_fcv = ct_strdup(_fcb);
                tree_t *fidx = ast_node_new(TT_IDX); ast_push(fidx, leaf_s(TT_VAR, dst->v.sval)); ast_push(fidx, leaf_s(TT_VAR, _fcv));
                tree_t *fbody = mk_assign(fidx, val);
                tree_t *floop = ast_node_new(TT_FOR); ast_push(floop, leaf_s(TT_VAR, _fcv)); ast_push(floop, ilit(flo)); ast_push(floop, ilit(fhi));
                ast_push(floop, fbody);
                return floop;
            }
        }
    }
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
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
        if (g_pas_range_check_on && v && v->t == TT_VAR && v->v.sval) { long long _lo, _hi; if (pas_subvar_get(v->v.sval, &_lo, &_hi)) rhs = pas_range_wrap(rhs, _lo, _hi); }
        return mk_assign(v, rhs);
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
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_rewrite", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "append") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_append", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_append", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_reset", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_reset", pas_tree_clone(fv)));
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
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, ilit(pas_rectype_nf(rt))); }
        else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        return mk_assign(pv, alloc);
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
    if (_wstream) { ast_push(e, pas_tree_clone(_wstream)); ast_push(e, ilit(-9)); }
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = _wstart; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i]; tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val);
                const char *_enm = (val && val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
                if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); val = _w; }
                else if (is_char) { val = mk_chr_wrap(val); if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2); }
                else if (pas_is_boolexpr(val)) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, "false,true")); val = _w; if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(5); }
                else if (val && val->t == TT_VAR && val->v.sval && pas_is_singlevar(val->v.sval)) { if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-5); }
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
                if (val && val->t == TT_QLIT && val->v.sval && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) { long long _lo = pas_caparm_lo(name, _pidx); val = pas_str_to_alpha(val->v.sval, _lo, _lo + (long long)strlen(val->v.sval) - 1); }
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
static tree_t *pas_mod(tree_t *a, tree_t *b) { return bin(TT_MOD, bin(TT_ADD, bin(TT_MOD, a, b), pas_tree_clone(b)), pas_tree_clone(b)); }
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
static struct { const char *name; int nvp; int vp[16]; int nrp; int rp[16]; int nsp; int sp[16]; } g_pas_funcs[256]; static int g_pas_nfunc;
static void pas_func_add(const char *name) { if (g_pas_nfunc < 256 && name) { g_pas_funcs[g_pas_nfunc].name = ct_strdup(name); g_pas_funcs[g_pas_nfunc].nvp = 0; g_pas_funcs[g_pas_nfunc].nrp = 0; g_pas_funcs[g_pas_nfunc].nsp = 0; g_pas_nfunc++; } }
static int pas_is_func(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nfunc; i++) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return 1; return 0; }
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_range_check"));
    ast_push(e, val); ast_push(e, ilit(lo)); ast_push(e, ilit(hi));
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
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = ct_strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_arrays[g_pas_narray].ncols = -1; g_pas_arrays[g_pas_narray].is_param = 0; g_pas_arrays[g_pas_narray].is_local = (g_pas_level >= 2); g_pas_arrays[g_pas_narray].low = g_pas_pend_sub_low; g_pas_narray++; } }
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
static void pas_arrtype_add(const char *name, long long high, int ndim2, long long ncols) { if (g_pas_narrtype < 64 && name) { g_pas_arrtypes[g_pas_narrtype].name = ct_strdup(name); g_pas_arrtypes[g_pas_narrtype].high = high; g_pas_arrtypes[g_pas_narrtype].ndim2 = ndim2; g_pas_arrtypes[g_pas_narrtype].ncols = ncols; g_pas_arrtypes[g_pas_narrtype].ischar = g_pas_pend_arr_ischar; g_pas_arrtypes[g_pas_narrtype].lo = (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0); g_pas_narrtype++; } }
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
static struct { char *name; long long low; long long high; } g_pas_subvars[256]; static int g_pas_nsubvar;
static void pas_subvar_add(const char *n, long long lo, long long hi) { if (g_pas_nsubvar < 256 && n) { g_pas_subvars[g_pas_nsubvar].name = ct_strdup(n); g_pas_subvars[g_pas_nsubvar].low = lo; g_pas_subvars[g_pas_nsubvar].high = hi; g_pas_nsubvar++; } }
static int pas_subvar_get(const char *n, long long *lo, long long *hi) { if (!n) return 0; for (int i = g_pas_nsubvar - 1; i >= 0; i--) if (g_pas_subvars[i].name && !strcmp(g_pas_subvars[i].name, n)) { *lo = g_pas_subvars[i].low; *hi = g_pas_subvars[i].high; return 1; } return 0; }
static struct { char *name; char *target; } g_pas_typealias[64]; static int g_pas_ntypealias;
static void pas_typealias_add(const char *n, const char *t) { if (g_pas_ntypealias < 64 && n && t && strcmp(n, t)) { g_pas_typealias[g_pas_ntypealias].name = ct_strdup(n); g_pas_typealias[g_pas_ntypealias].target = ct_strdup(t); g_pas_ntypealias++; } }
static const char *pas_typealias_get(const char *n) { if (!n) return NULL; for (int i = g_pas_ntypealias - 1; i >= 0; i--) if (g_pas_typealias[i].name && !strcmp(g_pas_typealias[i].name, n)) return g_pas_typealias[i].target; return NULL; }
static int g_pas_ldepth;
static struct { char *vname; char *tname; int is_global; } g_pas_scalarvartype[512]; static int g_pas_nscalarvartype;
static void pas_scalarvartype_add(const char *vn, const char *tn) { if (g_pas_nscalarvartype < 512 && vn && tn) { g_pas_scalarvartype[g_pas_nscalarvartype].vname = ct_strdup(vn); g_pas_scalarvartype[g_pas_nscalarvartype].tname = ct_strdup(tn); g_pas_scalarvartype[g_pas_nscalarvartype].is_global = (g_pas_ldepth == 0); g_pas_nscalarvartype++; } }
static const char *pas_scalarvartype_get(const char *vn) { if (!vn) return NULL; for (int i = g_pas_nscalarvartype - 1; i >= 0; i--) if (g_pas_scalarvartype[i].vname && !strcmp(g_pas_scalarvartype[i].vname, vn)) return g_pas_scalarvartype[i].tname; return NULL; }
static struct { char *name; long long low; long long high; int ischar; } g_pas_tfcomp[128]; static int g_pas_ntfcomp;
static void pas_tfcomp_add(const char *n, long long lo, long long hi, int ischar) { if (g_pas_ntfcomp < 128 && n) { g_pas_tfcomp[g_pas_ntfcomp].name = ct_strdup(n); g_pas_tfcomp[g_pas_ntfcomp].low = lo; g_pas_tfcomp[g_pas_ntfcomp].high = hi; g_pas_tfcomp[g_pas_ntfcomp].ischar = ischar; g_pas_ntfcomp++; } }
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi) { if (!n) return 0; for (int i = g_pas_ntfcomp - 1; i >= 0; i--) if (g_pas_tfcomp[i].name && !strcmp(g_pas_tfcomp[i].name, n) && g_pas_tfcomp[i].high >= g_pas_tfcomp[i].low) { *lo = g_pas_tfcomp[i].low; *hi = g_pas_tfcomp[i].high; return 1; } return 0; }
static int pas_tfcomp_nonchar(const char *n) { if (!n) return 0; for (int i = g_pas_ntfcomp - 1; i >= 0; i--) if (g_pas_tfcomp[i].name && !strcmp(g_pas_tfcomp[i].name, n)) return !g_pas_tfcomp[i].ischar; return 0; }
static long long g_pas_pend_frng_lo; static long long g_pas_pend_frng_hi = -1; static int g_pas_pend_frng_ischar;
#define PAS_REC_MAX 512
#define PAS_FIELD_MAX 128
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; char *fldenum[PAS_FIELD_MAX]; char *fldrec[PAS_FIELD_MAX]; char *fldtypename[PAS_FIELD_MAX]; int fldca[PAS_FIELD_MAX]; int fldna[PAS_FIELD_MAX]; long long fldna_lo[PAS_FIELD_MAX]; long long fldna_hi[PAS_FIELD_MAX]; long long fldca_lo[PAS_FIELD_MAX]; long long fldca_hi[PAS_FIELD_MAX]; int fldchar[PAS_FIELD_MAX]; int fldfile[PAS_FIELD_MAX]; int nf; int packed; int align_mac68k; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct pas_recvar_s { char *vname; char *fields[PAS_FIELD_MAX]; int nf; int fldchar[PAS_FIELD_MAX]; int packed; char *fldrec[PAS_FIELD_MAX]; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static char *g_pas_pend_fldenum[PAS_FIELD_MAX]; static char *g_pas_pend_fldrec[PAS_FIELD_MAX]; static char *g_pas_pend_fldtypename[PAS_FIELD_MAX]; static int g_pas_pend_fldca[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_lo[PAS_FIELD_MAX]; static long long g_pas_pend_fldca_hi[PAS_FIELD_MAX]; static int g_pas_pend_fldchar[PAS_FIELD_MAX]; static int g_pas_pend_fldfile[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static int g_pas_recbody_depth;
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename; static int g_pas_pend_ischar;
static int g_pas_pend_isarr; static int g_pas_pend_fldna[PAS_FIELD_MAX]; static long long g_pas_pend_fldna_lo[PAS_FIELD_MAX]; static long long g_pas_pend_fldna_hi[PAS_FIELD_MAX];
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
static void pas_pend_reset(void) { g_pas_pend_isarr = 0; g_pas_pend_isbool = 0; g_pas_pend_istfile = 0; g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ptrto = NULL; g_pas_pend_typename = NULL; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; g_pas_pend_enum_max = -1; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_wrap = 0; g_pas_pend_set_hi = -1; }
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
    g_pas_pend_fields[g_pas_pend_nf++] = ct_strdup(f); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ischar = 0; g_pas_pend_ischar = 0; g_pas_pend_isarr = 0; g_pas_pend_istfile = 0; } }
static void pas_rectype_add(const char *tn, int packed) { if (g_pas_nrectype >= PAS_REC_MAX || !tn) return; int k = g_pas_nrectype++; g_pas_rectypes[k].tname = ct_strdup(tn); g_pas_rectypes[k].nf = g_pas_pend_nf; g_pas_rectypes[k].packed = packed; g_pas_rectypes[k].align_mac68k = g_pas_align_mac68k;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_rectypes[k].fldna[i] = g_pas_pend_fldna[i]; g_pas_rectypes[k].fldna_lo[i] = g_pas_pend_fldna_lo[i]; g_pas_rectypes[k].fldna_hi[i] = g_pas_pend_fldna_hi[i]; g_pas_rectypes[k].fields[i] = g_pas_pend_fields[i]; g_pas_rectypes[k].fldptrto[i] = g_pas_pend_fldptrto[i]; g_pas_rectypes[k].fldenum[i] = g_pas_pend_fldenum[i]; g_pas_rectypes[k].fldrec[i] = g_pas_pend_fldrec[i]; g_pas_rectypes[k].fldtypename[i] = g_pas_pend_fldtypename[i]; g_pas_rectypes[k].fldca[i] = g_pas_pend_fldca[i]; g_pas_rectypes[k].fldca_lo[i] = g_pas_pend_fldca_lo[i]; g_pas_rectypes[k].fldca_hi[i] = g_pas_pend_fldca_hi[i]; g_pas_rectypes[k].fldchar[i] = g_pas_pend_fldchar[i]; g_pas_rectypes[k].fldfile[i] = g_pas_pend_fldfile[i]; } }
static int pas_rectype_to_pend(const char *tn) { if (!tn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, tn)) {
    pas_pend_reset(); for (int j = 0; j < g_pas_rectypes[i].nf; j++) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_rectypes[i].fldptrto[j]; g_pas_pend_fldenum[g_pas_pend_nf] = g_pas_rectypes[i].fldenum[j]; g_pas_pend_fldrec[g_pas_pend_nf] = g_pas_rectypes[i].fldrec[j]; g_pas_pend_fldtypename[g_pas_pend_nf] = g_pas_rectypes[i].fldtypename[j]; g_pas_pend_fldca[g_pas_pend_nf] = g_pas_rectypes[i].fldca[j]; g_pas_pend_fldca_lo[g_pas_pend_nf] = g_pas_rectypes[i].fldca_lo[j]; g_pas_pend_fldca_hi[g_pas_pend_nf] = g_pas_rectypes[i].fldca_hi[j]; g_pas_pend_fldchar[g_pas_pend_nf] = g_pas_rectypes[i].fldchar[j]; g_pas_pend_fldfile[g_pas_pend_nf] = g_pas_rectypes[i].fldfile[j]; g_pas_pend_fields[g_pas_pend_nf++] = g_pas_rectypes[i].fields[j]; } return 1; } return 0; }
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
static void pas_recvar_add(const char *vn, int packed) { if (g_pas_nrecvar >= PAS_REC_MAX || !vn || g_pas_pend_nf == 0) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = ct_strdup(vn); g_pas_recvars[k].nf = g_pas_pend_nf; g_pas_recvars[k].packed = packed;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; g_pas_recvars[k].fldchar[i] = g_pas_pend_fldchar[i]; g_pas_recvars[k].fldrec[i] = g_pas_pend_fldrec[i]; } }
static void pas_recvar_add_from_type(const char *vn, const char *tn) { if (!vn || !tn) return; for (int ri = 0; ri < g_pas_nrectype; ri++) { if (!g_pas_rectypes[ri].tname || strcmp(g_pas_rectypes[ri].tname, tn)) continue; if (g_pas_nrecvar >= PAS_REC_MAX) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = ct_strdup(vn); g_pas_recvars[k].nf = g_pas_rectypes[ri].nf; g_pas_recvars[k].packed = g_pas_rectypes[ri].packed; for (int j = 0; j < g_pas_rectypes[ri].nf; j++) { g_pas_recvars[k].fields[j] = g_pas_rectypes[ri].fields[j]; g_pas_recvars[k].fldchar[j] = g_pas_rectypes[ri].fldchar[j]; g_pas_recvars[k].fldrec[j] = g_pas_rectypes[ri].fldrec[j]; } return; } }
static int pas_recvar_field_is_char(const char *vn, long idx) { if (!vn || idx < 0) return 0; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return (idx < g_pas_recvars[i].nf) ? g_pas_recvars[i].fldchar[idx] : 0; return 0; }
static int pas_recvar_field_index(const char *vn, const char *fn) { if (!vn || !fn) return -1; for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) {
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
static int pas_is_boolexpr(tree_t *e) { if (!e) return 0; switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_NOT: return 1; case TT_MUL: case TT_ADD: return e->n == 2 && pas_is_boolexpr(e->c[0]) && pas_is_boolexpr(e->c[1]); case TT_VAR: return pas_is_boolvar(e->v.sval); case TT_FNC: { const char *fn = (e->n >= 1 && e->c[0]) ? e->c[0]->v.sval : NULL; if (!fn) return 0; if (!strcmp(fn, "__pas_in") || !strcmp(fn, "__pas_eof") || !strcmp(fn, "__pas_eoln") || !strcmp(fn, "__pas_eof_f") || !strcmp(fn, "__pas_eoln_f") || !strcmp(fn, "__pas_feof_t") || !strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne") || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")) return 1; return pas_is_boolvar(fn); } default: return 0; } }
static int pas_is_charvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ncharvar; i++) if (g_pas_charvars[i].name && !strcmp(g_pas_charvars[i].name, name)) return 1; return 0; }
static struct { char *name; } g_pas_filevars[256]; static int g_pas_nfilevar;
static void pas_filevar_add(const char *name) { if (g_pas_nfilevar < 256 && name) { for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return; g_pas_filevars[g_pas_nfilevar++].name = ct_strdup(name); } }
static int pas_is_filevar(const char *name) { if (!name) return 0; if (!strcmp(name, "input") || !strcmp(name, "output")) return 1; for (int i = 0; i < g_pas_nfilevar; i++) if (g_pas_filevars[i].name && !strcmp(g_pas_filevars[i].name, name)) return 1; return 0; }
static int pas_is_stdstream(const char *name) { return name && (!strcmp(name, "input") || !strcmp(name, "output")); }
static char *g_pas_hdrfiles[32]; static int g_pas_nhdrfile;
static int pas_is_hdrfile(const char *n) { if (!n) return 0; for (int i = 0; i < g_pas_nhdrfile; i++) if (g_pas_hdrfiles[i] && !strcmp(g_pas_hdrfiles[i], n)) return 1; return 0; }
static struct { char *name; long long lo; } g_pas_chararrs[256]; static int g_pas_nchararr;
static void pas_chararr_add2(const char *name, long long lo) { if (g_pas_nchararr < 256 && name) { g_pas_chararrs[g_pas_nchararr].name = ct_strdup(name); g_pas_chararrs[g_pas_nchararr].lo = lo; g_pas_nchararr++; } }
static void pas_chararr_add(const char *name) { pas_chararr_add2(name, 0); }
static long long pas_chararr_lo(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nchararr; i++) if (g_pas_chararrs[i].name && !strcmp(g_pas_chararrs[i].name, name)) return g_pas_chararrs[i].lo; return 0; }
static int pas_is_chararr(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nchararr; i++) if (g_pas_chararrs[i].name && !strcmp(g_pas_chararrs[i].name, name)) return 1; return 0; }
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
static struct { char *aname; char *rname; int nf; char *fields[32]; char *fldenum[32]; int fldchar[32]; } g_pas_arrrecs[128]; static int g_pas_narrrec;
static void pas_arrrec_add(const char *a, const char *r, int nf) { if (g_pas_narrrec < 128 && a && nf > 0) { g_pas_arrrecs[g_pas_narrrec].aname = ct_strdup(a); g_pas_arrrecs[g_pas_narrrec].rname = r ? ct_strdup(r) : NULL; g_pas_arrrecs[g_pas_narrrec].nf = nf; for (int _i = 0; _i < nf && _i < 32; _i++) { g_pas_arrrecs[g_pas_narrrec].fields[_i] = g_pas_pend_fields[_i] ? ct_strdup(g_pas_pend_fields[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldenum[_i] = g_pas_pend_fldenum[_i] ? ct_strdup(g_pas_pend_fldenum[_i]) : NULL; g_pas_arrrecs[g_pas_narrrec].fldchar[_i] = g_pas_pend_fldchar[_i]; } g_pas_narrrec++; } }
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
static int pas_is_charexpr(tree_t *e) { if (!e) return 0; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_tbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_pchar_deref"))) return 1; if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1; if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[1]->t == TT_ILIT && pas_recvar_field_is_char(e->c[0]->v.sval, e->c[1]->v.ival)) return 1; if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_charvar(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_chararr(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 2 && e->c[0] && pas_is_cafield(e->c[0])) return 1; if (pas_is_cvfield(e)) return 1; return 0; }
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
static tree_t *pas_rel(tree_e tt, tree_t *a, tree_t *b) { int lex = pas_is_strval(a) || pas_is_strval(b); if (pas_is_strtyped(a) || pas_is_strtyped(b)) { a = pas_alpha_wrap(a); b = pas_alpha_wrap(b); } return lex ? bin(tt, mk_set_bin("__pas_strcmp", a, b), ilit(0)) : bin(tt, a, b); }
static int pas_is_setexpr(tree_t *e);
static tree_t *pas_rel_or_set(tree_e tt, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : pas_rel(tt, a, b); }
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return ct_strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
#define PAS_WITH_MAX 8
static struct { tree_t *sel; const char *rtype; } g_with_stk[PAS_WITH_MAX]; static int g_with_depth;
static tree_t *pas_tree_clone(tree_t *e) { if (!e) return NULL; tree_t *c = ast_node_new(e->t); c->v = e->v; if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = ct_strdup(e->v.sval); for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i])); return c; }
static const char *pas_with_sel_rtype(tree_t *sel) { if (!sel) return NULL; if (sel->t == TT_VAR && sel->v.sval) { for (int i = g_pas_nrecvar - 1; i >= 0; i--) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, sel->v.sval)) { const char *rt = NULL; for (int j = 0; j < g_pas_nrectype; j++) { int match = 1; if (!g_pas_rectypes[j].tname) continue; if (g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { match = 0; break; } if (match) { rt = g_pas_rectypes[j].tname; break; } } if (!rt) { for (int j = 0; j < g_pas_nrectype; j++) { int all = 1; if (!g_pas_rectypes[j].tname || g_pas_rectypes[j].nf != g_pas_recvars[i].nf) continue; for (int k = 0; k < g_pas_recvars[i].nf; k++) if (!g_pas_recvars[i].fields[k] || !g_pas_rectypes[j].fields[k] || strcmp(g_pas_recvars[i].fields[k], g_pas_rectypes[j].fields[k])) { all = 0; break; } if (all) { rt = g_pas_rectypes[j].tname; break; } } } return rt ? rt : g_pas_recvars[i].fields[0] ? g_pas_rectypes[0].tname : NULL; } } if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) { const char *ptn = pas_ptrexpr_target(sel->c[1]); return ptn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval) { const char *_arn = NULL; if (pas_arrrec_find(sel->c[0]->v.sval, &_arn) > 0 && _arn) return _arn; } if (sel->t == TT_IDX && sel->n >= 2 && sel->c[1] && sel->c[1]->t == TT_ILIT) { const char *bt = pas_with_sel_rtype(sel->c[0]); if (bt) { const char *_fr = pas_rectype_field_rectype_by_index(bt, sel->c[1]->v.ival); if (_fr) return _fr; return pas_rectype_field_ptrto_by_index(bt, sel->c[1]->v.ival); } } return NULL; }
static int pas_with_field_index(const char *rtype, const char *fname) { return pas_rectype_field_index(rtype, fname); }
static int pas_with_recvar_field(const char *vname, const char *fname) { return pas_recvar_field_index(vname, fname); }
static void pas_with_push(tree_t *sel) { if (g_with_depth >= PAS_WITH_MAX || !sel) return; const char *rt = pas_with_sel_rtype(sel); g_with_stk[g_with_depth].sel = sel; g_with_stk[g_with_depth].rtype = rt; g_with_depth++; }
static void pas_with_pop(void) { if (g_with_depth > 0) g_with_depth--; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
static int pas_actual_is_packed_component(tree_t *a) { if (!a || a->n < 2 || !a->c[0] || !a->c[1]) return 0;
    if (!((a->t == TT_IDX && a->c[1]->t == TT_ILIT) || (a->t == TT_FIELD && a->c[1]->t == TT_VAR))) return 0;
    tree_t *base = a->c[0];
    if (base->t == TT_VAR && base->v.sval) return pas_recvar_is_packed(base->v.sval);
    const char *rt = pas_designator_rectype(base);
    return (rt && pas_rectype_is_packed(rt)) ? 1 : 0; }
static int pas_is_setexpr(tree_t *e) { if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) return pas_is_setvar(e->v.sval);
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval) { const char *f = e->c[0]->v.sval;
        return !strcmp(f, "__pas_set") || !strcmp(f, "__pas_setuni") || !strcmp(f, "__pas_setint") || !strcmp(f, "__pas_setdif"); }
    return 0; }
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *pas_rdiv(tree_t *a, tree_t *b) { return bin(TT_DIV, bin(TT_MUL, a, flit(1.0)), b); }
static tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : bin(ak, a, b); }
static tree_t *g_pas_nrec_marks[512]; static int g_pas_nrec_nmarks = 0;
static void pas_nrec_mark_add(tree_t *e) { if (g_pas_nrec_nmarks < 512 && e) g_pas_nrec_marks[g_pas_nrec_nmarks++] = e; }
int pas_is_nrec_idx(const tree_t *e) { for (int i = 0; i < g_pas_nrec_nmarks; i++) if (g_pas_nrec_marks[i] == (tree_t *)e) return 1; return 0; }
static tree_t *pas_nested_field_resolve(tree_t *base, const char *fld) {
    const char *_brt = pas_with_sel_rtype(base);
    if (_brt) { int _nfi = pas_rectype_field_index(_brt, fld); if (_nfi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, base); ast_push(e, ilit(_nfi)); const char *_fe = pas_rectype_field_enum_by_index(_brt, _nfi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); }
        if (pas_rectype_field_is_ca(_brt, _nfi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_brt, _nfi), pas_rectype_field_ca_hi(_brt, _nfi));
        if (pas_rectype_field_is_na(_brt, _nfi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_brt, _nfi));
        if (pas_rectype_field_is_char(_brt, _nfi)) pas_cvfield_mark_add(e);
        pas_nrec_mark_add(e); return e; } }
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
static const char *pas_class_name(char k) {
    switch (k) { case 'i': return "integer-type"; case 'r': return "real-type"; case 'c': return "char-type"; case 'b': return "Boolean-type";
                 case 'e': return "an enumerated-type"; case 's': return "a string-type"; default: return "?"; }
}
static int pas_class_compatible(char to, char from) { return to == from || (to == 'r' && from == 'i'); }
static void pas_value_compat(const char *vn, tree_t *rhs, const char *clause, const char *what) {
    char to = pas_var_decl_class(vn), from = pas_expr_lit_class(rhs);
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: %s '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n",
            clause, what, vn, pas_class_name(from), pas_class_name(to));
    g_pas_iso_errors++;
}
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    { const char *_abn = pas_selector_base_name(sel); if (_abn) pas_assigned_add(_abn); }
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_FLIT && pas_var_is_real(sel->v.sval)) rhs = bin(TT_ADD, rhs, flit(0.0));
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
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
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
                    case TT_GT: e->t = TT_LE; break; case TT_EQ: e->t = TT_NE; break; case TT_NE: e->t = TT_EQ; break; default: break; }
    return e;
}
static tree_t *mk_array_fill(long long high) {
    long long n = high + 1; if (n < 1) n = 1;
    size_t len = (size_t)(n * 2 - 1);
    char *buf = (char *)ct_alloc(len + 1); size_t p = 0;
    for (long long k = 0; k < n; k++) { if (k) buf[p++] = '\001'; buf[p++] = '0'; }
    buf[p] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high) {
    if (!s) s = ""; if (lo < 0) lo = 0;
    size_t sl = strlen(s); long long n = high + 1; if (n < lo + (long long)sl) n = lo + (long long)sl; if (n < 1) n = 1;
    char *buf = (char *)ct_alloc((size_t)n * 12 + 1); size_t p = 0;
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
    for (int i = 0; i < g_pas_narray; i++) if (g_pas_arrays[i].name && !strcmp(g_pas_arrays[i].name, name)) return 0;
    if (pas_is_charvar(name)) { *out = 1; return 1; }
    if (pas_is_boolvar(name)) { *out = 1; return 1; }
    if (pas_ptrvar_target(name)) { *out = 8; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_sizeof_lookup(tn, out); }
    return 0;
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
static tree_t *mk_array_init(const char *name, long long high) {
    if (pas_array_is_pure_num(name)) return mk_fnc1("arr_make", ilit(high));
    return mk_array_fill(high);
}
static long long pas_decl_part_order(long long prev, long long cur) {
    const char *nm[] = { "", "label-declaration-part", "constant-definition-part", "type-definition-part", "variable-declaration-part", "procedure-and-function-declaration-part" };
    if (cur < prev) { fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block's %s shall precede its %s\n", nm[cur], nm[prev]); g_pas_iso_errors++; }
    else if (cur == prev && cur < 5) { fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block has more than one %s\n", nm[cur]); g_pas_iso_errors++; }
    return cur > prev ? cur : prev;
}

#line 1473 "pascal.tab.c"

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
  YYSYMBOL_REALCONST = 61,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 62,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 63,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 64,                  /* $accept  */
  YYSYMBOL_program = 65,                   /* program  */
  YYSYMBOL_file_id_list_opt = 66,          /* file_id_list_opt  */
  YYSYMBOL_block = 67,                     /* block  */
  YYSYMBOL_decl_part_list = 68,            /* decl_part_list  */
  YYSYMBOL_decl_part = 69,                 /* decl_part  */
  YYSYMBOL_label_list = 70,                /* label_list  */
  YYSYMBOL_const_decl_list = 71,           /* const_decl_list  */
  YYSYMBOL_const_decl = 72,                /* const_decl  */
  YYSYMBOL_constant = 73,                  /* constant  */
  YYSYMBOL_scalar_constant = 74,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 75,            /* type_decl_list  */
  YYSYMBOL_type_decl = 76,                 /* type_decl  */
  YYSYMBOL_type = 77,                      /* type  */
  YYSYMBOL_78_1 = 78,                      /* $@1  */
  YYSYMBOL_packed_opt = 79,                /* packed_opt  */
  YYSYMBOL_simple_type = 80,               /* simple_type  */
  YYSYMBOL_record_body = 81,               /* record_body  */
  YYSYMBOL_record_field_list = 82,         /* record_field_list  */
  YYSYMBOL_record_field = 83,              /* record_field  */
  YYSYMBOL_record_case_opt = 84,           /* record_case_opt  */
  YYSYMBOL_record_case_list = 85,          /* record_case_list  */
  YYSYMBOL_record_case_arm = 86,           /* record_case_arm  */
  YYSYMBOL_var_decl_list = 87,             /* var_decl_list  */
  YYSYMBOL_var_decl = 88,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 89,            /* procedure_decl  */
  YYSYMBOL_90_2 = 90,                      /* $@2  */
  YYSYMBOL_91_3 = 91,                      /* $@3  */
  YYSYMBOL_92_4 = 92,                      /* $@4  */
  YYSYMBOL_pv_mark = 93,                   /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 94,        /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 95,       /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 96,            /* parameter_decl  */
  YYSYMBOL_id_list = 97,                   /* id_list  */
  YYSYMBOL_body = 98,                      /* body  */
  YYSYMBOL_statement_list = 99,            /* statement_list  */
  YYSYMBOL_100_5 = 100,                    /* $@5  */
  YYSYMBOL_101_6 = 101,                    /* $@6  */
  YYSYMBOL_statement = 102,                /* statement  */
  YYSYMBOL_statement_no_label = 103,       /* statement_no_label  */
  YYSYMBOL_call = 104,                     /* call  */
  YYSYMBOL_call_with_args = 105,           /* call_with_args  */
  YYSYMBOL_argument_list = 106,            /* argument_list  */
  YYSYMBOL_argument = 107,                 /* argument  */
  YYSYMBOL_assignment = 108,               /* assignment  */
  YYSYMBOL_selector = 109,                 /* selector  */
  YYSYMBOL_expression_list = 110,          /* expression_list  */
  YYSYMBOL_compound_statement = 111,       /* compound_statement  */
  YYSYMBOL_goto_statement = 112,           /* goto_statement  */
  YYSYMBOL_if_statement = 113,             /* if_statement  */
  YYSYMBOL_case_statement = 114,           /* case_statement  */
  YYSYMBOL_115_7 = 115,                    /* $@7  */
  YYSYMBOL_case_list = 116,                /* case_list  */
  YYSYMBOL_case_elem = 117,                /* case_elem  */
  YYSYMBOL_constant_list = 118,            /* constant_list  */
  YYSYMBOL_while_statement = 119,          /* while_statement  */
  YYSYMBOL_repeat_statement = 120,         /* repeat_statement  */
  YYSYMBOL_for_statement = 121,            /* for_statement  */
  YYSYMBOL_with_statement = 122,           /* with_statement  */
  YYSYMBOL_with_open = 123,                /* with_open  */
  YYSYMBOL_expression = 124,               /* expression  */
  YYSYMBOL_simple_expression = 125,        /* simple_expression  */
  YYSYMBOL_term = 126,                     /* term  */
  YYSYMBOL_factor = 127,                   /* factor  */
  YYSYMBOL_set_member_list = 128,          /* set_member_list  */
  YYSYMBOL_set_member = 129                /* set_member  */
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
#define YYLAST   434

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  165
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  331

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   318


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
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1434,  1434,  1455,  1456,  1459,  1462,  1463,  1466,  1467,
    1468,  1469,  1470,  1473,  1474,  1477,  1478,  1480,  1481,  1482,
    1483,  1484,  1486,  1486,  1486,  1487,  1487,  1487,  1487,  1489,
    1490,  1492,  1494,  1495,  1496,  1497,  1498,  1498,  1499,  1500,
    1501,  1509,  1509,  1511,  1518,  1519,  1522,  1525,  1526,  1529,
    1530,  1533,  1534,  1535,  1538,  1539,  1542,  1543,  1546,  1547,
    1549,  1551,  1552,  1553,  1553,  1557,  1557,  1561,  1561,  1567,
    1570,  1571,  1574,  1575,  1578,  1579,  1580,  1581,  1584,  1585,
    1588,  1591,  1591,  1592,  1592,  1595,  1596,  1601,  1602,  1603,
    1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1614,  1615,
    1618,  1625,  1626,  1629,  1630,  1631,  1634,  1670,  1678,  1682,
    1683,  1686,  1687,  1690,  1693,  1698,  1699,  1702,  1702,  1713,
    1714,  1717,  1718,  1721,  1722,  1725,  1728,  1731,  1736,  1743,
    1746,  1747,  1754,  1755,  1756,  1757,  1758,  1759,  1760,  1761,
    1764,  1765,  1766,  1767,  1768,  1769,  1772,  1773,  1774,  1775,
    1776,  1777,  1780,  1786,  1787,  1788,  1789,  1790,  1791,  1792,
    1793,  1794,  1797,  1798,  1801,  1802
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
  "LPARENT", "RPARENT", "DOTDOT", "ATSIGN", "INTCONST", "REALCONST",
  "STRINGCONST", "IDENT", "$accept", "program", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "label_list", "const_decl_list",
  "const_decl", "constant", "scalar_constant", "type_decl_list",
  "type_decl", "type", "$@1", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@2", "$@3", "$@4", "pv_mark", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "id_list", "body",
  "statement_list", "$@5", "$@6", "statement", "statement_no_label",
  "call", "call_with_args", "argument_list", "argument", "assignment",
  "selector", "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@7", "case_list", "case_elem",
  "constant_list", "while_statement", "repeat_statement", "for_statement",
  "with_statement", "with_open", "expression", "simple_expression", "term",
  "factor", "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-226)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-111)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      21,    -6,    69,    16,  -226,    18,   101,  -226,   -30,  -226,
      71,  -226,    79,   117,  -226,  -226,    83,    98,   109,    18,
     118,  -226,   133,  -226,  -226,  -226,  -226,     9,    86,    98,
    -226,  -226,    18,  -226,   190,  -226,    33,    32,   115,   133,
    -226,  -226,   141,   123,  -226,   146,  -226,    66,     6,  -226,
    -226,   151,   154,  -226,   251,   164,   251,  -226,   251,   162,
      67,  -226,  -226,  -226,  -226,  -226,    22,  -226,  -226,  -226,
    -226,  -226,  -226,  -226,  -226,    66,  -226,  -226,   175,   292,
    -226,   239,   240,  -226,   244,  -226,    15,   254,  -226,   335,
     335,   200,    18,  -226,  -226,   216,   220,   276,   111,  -226,
    -226,   235,    32,  -226,   264,    14,   261,   261,   261,   229,
     251,   261,  -226,  -226,  -226,   253,  -226,   172,   122,    68,
     170,  -226,  -226,   172,     5,   213,    62,   305,    19,   251,
     251,   232,  -226,   251,   336,   339,  -226,   351,  -226,  -226,
    -226,  -226,    18,    18,    18,     8,  -226,   227,   309,  -226,
      27,   331,  -226,   322,   346,   359,  -226,  -226,   318,  -226,
     251,   251,  -226,   170,   170,  -226,   285,    -8,  -226,   259,
    -226,    32,   251,   251,   251,   251,   251,   251,   251,   261,
     261,   261,   261,   261,   261,   261,   261,    32,   164,  -226,
    -226,    32,  -226,   130,  -226,   317,   345,  -226,     3,   345,
    -226,  -226,  -226,   333,   256,   290,    15,  -226,   321,   395,
    -226,  -226,  -226,   205,    66,   205,    18,   397,   398,   188,
     345,   251,   251,  -226,  -226,  -226,    68,    68,    68,    68,
      68,    68,    68,   170,   170,   170,  -226,  -226,  -226,  -226,
    -226,  -226,   172,   331,   371,   251,  -226,   251,   251,  -226,
     342,   343,  -226,  -226,  -226,   402,    97,  -226,  -226,   385,
      23,  -226,   313,  -226,   400,   251,   251,   345,  -226,  -226,
      93,  -226,   316,    32,  -226,   330,   345,  -226,  -226,  -226,
     205,   389,  -226,    18,   348,  -226,    66,   407,  -226,   134,
     148,   331,  -226,   331,    32,  -226,   251,   358,    66,  -226,
       2,  -226,  -226,   409,    32,    32,  -226,  -226,  -226,   345,
     394,  -226,   331,   353,  -226,  -226,  -226,    66,   412,  -226,
     349,   399,  -226,   331,   362,   331,  -226,    18,   412,   364,
    -226
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    79,     0,     7,
       0,     3,     0,     0,    78,     2,     0,     0,     0,     0,
       0,    83,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    69,    11,    59,     0,    69,     0,    97,     0,    10,
      30,     8,     0,     0,    15,    71,    58,    42,    71,    81,
      80,     0,     0,    83,     0,     0,     0,    83,     0,     0,
      98,    84,    85,    88,    99,    87,     0,    89,    90,    91,
      92,    93,    94,    95,    96,    42,    29,    13,     0,     0,
      26,     0,     0,    25,     0,    22,     0,     0,    41,     0,
       0,     0,     0,    27,    28,    44,     0,     0,     0,    32,
      67,     0,    97,   114,     0,     0,     0,     0,     0,     0,
       0,     0,   154,   155,   156,   110,   153,   152,     0,   132,
     140,   146,   110,   131,     0,     0,     0,     0,    97,     0,
       0,     0,   109,     0,     0,     0,    23,     0,    24,    17,
      20,    21,     0,     0,     0,     0,    73,     0,    63,    33,
       0,     0,    60,     0,    39,     0,    36,     7,     0,    82,
       0,     0,   158,   141,   142,   160,   164,     0,   162,     0,
     159,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,   117,
     113,    97,    86,     0,   102,   103,   106,   108,     0,   112,
      31,    18,    19,    74,     0,     0,     0,    70,     0,     0,
       7,    43,    45,     0,    42,     0,    50,     0,     0,     0,
     126,     0,     0,   161,   157,   125,   133,   134,   135,   136,
     137,   138,   139,   145,   143,   144,   149,   150,   151,   147,
     148,   129,   130,   122,   115,     0,   100,     0,     0,   107,
       0,     0,    72,    77,    61,     0,     0,    40,    38,     0,
      53,    48,     0,    68,    65,     0,     0,   165,   163,   124,
       0,   120,     0,    97,   101,   104,   111,    76,    75,    64,
       0,     0,    37,    50,     0,    46,    42,     0,     7,     0,
       0,   122,   118,     0,    97,   116,     0,     0,    42,    47,
       0,    49,    62,     0,    97,    97,   119,   123,   121,   105,
       0,    34,    57,     0,    66,   128,   127,    42,    52,    55,
       0,     0,    35,    57,     0,    57,    54,    50,    51,     0,
      56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -226,  -226,  -226,  -147,  -226,  -226,  -226,  -226,   390,   -43,
     131,  -226,   383,   -74,  -226,  -226,  -206,    96,  -226,   142,
    -226,    99,   103,  -226,   396,  -226,  -226,  -226,  -226,   392,
     381,  -226,   224,     7,  -226,    52,  -226,  -226,  -100,   303,
    -226,   -32,  -226,   187,  -226,   -34,  -226,  -226,  -226,  -226,
    -226,  -226,  -226,   143,  -225,  -226,  -226,  -226,  -226,  -226,
     -50,   160,   -91,   -82,  -226,   211
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,    96,
      85,    39,    40,    97,   216,    98,    99,   259,   260,   261,
     285,   318,   319,    32,    33,    24,   210,   288,   157,    45,
      87,   145,   146,   262,    25,    36,   102,    37,    61,    62,
      63,   116,   193,   194,    65,   117,   198,    67,    68,    69,
      70,   243,   270,   271,   320,    71,    72,    73,    74,   124,
     166,   119,   120,   121,   167,   168
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      84,   134,   159,    66,   118,    64,   125,   256,   127,   258,
     217,   100,     8,   206,    41,   187,   163,   164,   272,    49,
      10,   123,    51,   312,   162,     1,    34,    11,   283,   170,
     161,    52,    53,    54,   142,    51,    55,    56,    49,    34,
     143,   284,   222,   144,    52,    53,    54,   223,    57,    55,
      56,    58,   130,   248,   313,   188,    50,     3,   249,    42,
     169,    57,    86,   255,    58,   207,   272,    49,    66,     4,
      64,   225,     5,   131,   297,   132,   133,    10,     7,   195,
     196,     7,    60,   199,   211,   190,    88,   241,   233,   234,
     235,   244,    59,   147,    66,    60,    64,  -110,   291,   150,
     236,   237,   238,   239,   240,   105,     9,   179,   212,   126,
     219,   220,    89,    90,   180,   181,   292,   153,  -110,    91,
    -110,  -110,    92,   129,    16,    17,    80,    93,    94,    95,
      15,    43,   171,   154,    14,   155,    18,    66,   156,    64,
     257,   303,    19,    26,   304,    20,    21,   280,    22,   203,
     204,   205,   281,    66,   242,    64,   172,    66,   305,    64,
      75,    28,   173,   174,   175,   176,   177,   178,   172,    78,
      79,   267,    31,   295,   173,   174,   175,   176,   177,   178,
     245,    35,   172,    80,    81,    82,    83,   246,   173,   174,
     175,   176,   177,   178,   308,   195,    38,   275,   276,   265,
     269,    77,    86,   266,   315,   316,   182,   183,   184,   136,
     138,   103,   301,   147,   128,   289,   290,   104,   185,   186,
     136,   138,   172,   131,   311,   132,   133,   122,   173,   174,
     175,   176,   177,   178,   189,    80,   135,    94,    83,    66,
      10,    64,    47,   322,   139,   140,   309,   172,   269,   141,
     307,    89,    90,   173,   174,   175,   176,   177,   178,   148,
      66,    92,    64,   149,   106,    80,    93,    94,    95,   269,
      66,    66,    64,    64,   -25,   107,   108,    10,   151,   208,
     269,   152,   269,   109,   165,   110,   106,   158,   111,   112,
     113,   114,   115,   172,   160,   197,   106,   107,   108,   173,
     174,   175,   176,   177,   178,   109,    10,   110,   250,   129,
     111,   112,   113,   114,   115,   109,   224,   110,   209,   172,
     111,   112,   113,   114,   115,   173,   174,   175,   176,   177,
     178,   191,   226,   227,   228,   229,   230,   231,   232,   172,
      10,   200,   251,   221,   201,   173,   174,   175,   176,   177,
     178,   172,    80,   137,    94,    83,   202,   173,   174,   175,
     176,   177,   178,    10,   172,   286,   293,   214,   294,   247,
     173,   174,   175,   176,   177,   178,   213,    89,    90,   172,
     215,   218,   296,    10,   253,   173,   174,   175,   176,   177,
     178,    80,    93,    94,    83,    80,    93,    94,    83,   293,
     254,   324,   263,   264,   273,   277,   278,   279,   282,   287,
     298,   300,   302,   310,   314,   317,   321,   323,   327,    44,
     325,   330,    76,   329,   328,   299,   326,    48,    46,   101,
     252,   192,   274,   268,   306
};

static const yytype_int16 yycheck[] =
{
      43,    75,   102,    37,    54,    37,    56,   213,    58,   215,
     157,     5,     5,     5,     5,    10,   107,   108,   243,     5,
      50,    55,     3,    21,   106,     4,    19,    57,     5,   111,
      16,    12,    13,    14,    19,     3,    17,    18,     5,    32,
      25,    18,    50,    28,    12,    13,    14,    55,    29,    17,
      18,    32,    30,    50,    52,    50,    23,    63,    55,    50,
     110,    29,    56,   210,    32,    57,   291,     5,   102,     0,
     102,   171,    56,    51,   280,    53,    54,    50,    63,   129,
     130,    63,    63,   133,    57,    23,    20,   187,   179,   180,
     181,   191,    60,    86,   128,    63,   128,    30,     5,    92,
     182,   183,   184,   185,   186,    53,     5,    39,   151,    57,
     160,   161,    46,    47,    46,    47,    23,     6,    51,    53,
      53,    54,    56,    56,     7,     8,    60,    61,    62,    63,
      51,    45,    10,    22,    63,    24,    19,   171,    27,   171,
     214,   288,    25,    60,    10,    28,    29,    50,    31,   142,
     143,   144,    55,   187,   188,   187,    34,   191,    10,   191,
      45,    63,    40,    41,    42,    43,    44,    45,    34,    46,
      47,   221,    63,   273,    40,    41,    42,    43,    44,    45,
      50,    63,    34,    60,    61,    62,    63,    57,    40,    41,
      42,    43,    44,    45,   294,   245,    63,   247,   248,    11,
     243,    60,    56,    15,   304,   305,    36,    37,    38,    78,
      79,    60,   286,   206,    52,   265,   266,    63,    48,    49,
      89,    90,    34,    51,   298,    53,    54,    63,    40,    41,
      42,    43,    44,    45,    21,    60,    61,    62,    63,   273,
      50,   273,    52,   317,     5,     5,   296,    34,   291,     5,
     293,    46,    47,    40,    41,    42,    43,    44,    45,     5,
     294,    56,   294,    63,    35,    60,    61,    62,    63,   312,
     304,   305,   304,   305,    58,    46,    47,    50,    58,    52,
     323,     5,   325,    54,    55,    56,    35,    52,    59,    60,
      61,    62,    63,    34,    30,    63,    35,    46,    47,    40,
      41,    42,    43,    44,    45,    54,    50,    56,    52,    56,
      59,    60,    61,    62,    63,    54,    57,    56,     9,    34,
      59,    60,    61,    62,    63,    40,    41,    42,    43,    44,
      45,    26,   172,   173,   174,   175,   176,   177,   178,    34,
      50,     5,    52,    58,     5,    40,    41,    42,    43,    44,
      45,    34,    60,    61,    62,    63,     5,    40,    41,    42,
      43,    44,    45,    50,    34,    52,    50,    21,    52,    52,
      40,    41,    42,    43,    44,    45,    54,    46,    47,    34,
      21,    63,    52,    50,    63,    40,    41,    42,    43,    44,
      45,    60,    61,    62,    63,    60,    61,    62,    63,    50,
       5,    52,     5,     5,    33,    63,    63,     5,    23,     9,
      21,    63,     5,    55,     5,    21,    63,     5,    56,    29,
      21,    57,    39,   327,   325,   283,   323,    35,    32,    48,
     206,   128,   245,   222,   291
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    65,    63,     0,    56,    66,    63,    97,     5,
      50,    57,    67,    68,    63,    51,     7,     8,    19,    25,
      28,    29,    31,    69,    89,    98,    60,    70,    63,    71,
      72,    63,    87,    88,    97,    63,    99,   101,    63,    75,
      76,     5,    50,    45,    72,    93,    88,    52,    93,     5,
      23,     3,    12,    13,    14,    17,    18,    29,    32,    60,
      63,   102,   103,   104,   105,   108,   109,   111,   112,   113,
     114,   119,   120,   121,   122,    45,    76,    60,    46,    47,
      60,    61,    62,    63,    73,    74,    56,    94,    20,    46,
      47,    53,    56,    61,    62,    63,    73,    77,    79,    80,
       5,    94,   100,    60,    63,    99,    35,    46,    47,    54,
      56,    59,    60,    61,    62,    63,   105,   109,   124,   125,
     126,   127,    63,   109,   123,   124,    99,   124,    52,    56,
      30,    51,    53,    54,    77,    61,    74,    61,    74,     5,
       5,     5,    19,    25,    28,    95,    96,    97,     5,    63,
      97,    58,     5,     6,    22,    24,    27,    92,    52,   102,
      30,    16,   127,   126,   126,    55,   124,   128,   129,   124,
     127,    10,    34,    40,    41,    42,    43,    44,    45,    39,
      46,    47,    36,    37,    38,    48,    49,    10,    50,    21,
      23,    26,   103,   106,   107,   124,   124,    63,   110,   124,
       5,     5,     5,    97,    97,    97,     5,    57,    52,     9,
      90,    57,    73,    54,    21,    21,    78,    67,    63,   124,
     124,    58,    50,    55,    57,   102,   125,   125,   125,   125,
     125,   125,   125,   126,   126,   126,   127,   127,   127,   127,
     127,   102,   109,   115,   102,    50,    57,    52,    50,    55,
      52,    52,    96,    63,     5,    67,    80,    77,    80,    81,
      82,    83,    97,     5,     5,    11,    15,   124,   129,    73,
     116,   117,   118,    33,   107,   124,   124,    63,    63,     5,
      50,    55,    23,     5,    18,    84,    52,     9,    91,   124,
     124,     5,    23,    50,    52,   102,    52,    80,    21,    83,
      63,    77,     5,    67,    10,    10,   117,    73,   102,   124,
      55,    77,    21,    52,     5,   102,   102,    21,    85,    86,
     118,    63,    77,     5,    52,    21,    86,    56,    85,    81,
      57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    66,    66,    67,    68,    68,    69,    69,
      69,    69,    69,    70,    70,    71,    71,    72,    72,    72,
      72,    72,    73,    73,    73,    74,    74,    74,    74,    75,
      75,    76,    77,    77,    77,    77,    78,    77,    77,    77,
      77,    79,    79,    80,    80,    80,    81,    82,    82,    83,
      83,    84,    84,    84,    85,    85,    86,    86,    87,    87,
      88,    89,    89,    90,    89,    91,    89,    92,    89,    93,
      94,    94,    95,    95,    96,    96,    96,    96,    97,    97,
      98,   100,    99,   101,    99,   102,   102,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     105,   106,   106,   107,   107,   107,   108,   109,   109,   109,
     109,   110,   110,   111,   112,   113,   113,   115,   114,   116,
     116,   117,   117,   118,   118,   119,   120,   121,   121,   122,
     123,   123,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   125,   125,   125,   125,   126,   126,   126,   126,
     126,   126,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   129,   129
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     5,     5,
       4,     4,     1,     2,     2,     1,     1,     1,     1,     2,
       1,     4,     1,     2,     7,     9,     0,     5,     4,     2,
       4,     1,     0,     3,     1,     3,     2,     3,     1,     3,
       0,     6,     4,     0,     3,     1,     5,     0,     2,     1,
       4,     7,     9,     0,     8,     0,    10,     0,     7,     0,
       3,     0,     3,     1,     2,     4,     4,     3,     3,     1,
       3,     0,     4,     0,     2,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       4,     3,     1,     1,     3,     5,     3,     4,     3,     2,
       1,     3,     1,     3,     2,     4,     6,     0,     6,     3,
       1,     3,     0,     3,     1,     4,     4,     8,     8,     4,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     3,     2,     2,
       2,     3,     1,     3,     1,     3
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
#line 1435 "pascal.y"
        { tree_t *body = (yyvsp[-1].node);
          if (g_pas_narray > 0 || g_pas_nhdrfile > 0 || g_pas_nscalarvartype > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_nhdrfile; i++) if (g_pas_hdrfiles[i]) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_hdrfiles[i]), leaf_s(TT_QLIT, g_pas_hdrfiles[i])));
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
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
#line 2865 "pascal.tab.c"
    break;

  case 3: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 1455 "pascal.y"
                            { pas_program_params_distinct((yyvsp[-1].list)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) { tree_t *id = (yyvsp[-1].list)->items[i]; if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) { pas_filevar_add(id->v.sval); if (g_pas_nhdrfile < 32) g_pas_hdrfiles[g_pas_nhdrfile++] = ct_strdup(id->v.sval); } } }
#line 2871 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 1459 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2877 "pascal.tab.c"
    break;

  case 6: /* decl_part_list: decl_part_list decl_part  */
#line 1462 "pascal.y"
                             { (yyval.ival) = pas_decl_part_order((yyvsp[-1].ival), (yyvsp[0].ival)); }
#line 2883 "pascal.tab.c"
    break;

  case 7: /* decl_part_list: %empty  */
#line 1463 "pascal.y"
      { (yyval.ival) = 0; }
#line 2889 "pascal.tab.c"
    break;

  case 8: /* decl_part: LABELSY label_list SEMICOLON  */
#line 1466 "pascal.y"
                                 { (yyval.ival) = 1; }
#line 2895 "pascal.tab.c"
    break;

  case 9: /* decl_part: CONSTSY const_decl_list  */
#line 1467 "pascal.y"
                              { (yyval.ival) = 2; }
#line 2901 "pascal.tab.c"
    break;

  case 10: /* decl_part: TYPESY type_decl_list  */
#line 1468 "pascal.y"
                            { (yyval.ival) = 3; }
#line 2907 "pascal.tab.c"
    break;

  case 11: /* decl_part: VARSY var_decl_list  */
#line 1469 "pascal.y"
                          { (yyval.ival) = 4; }
#line 2913 "pascal.tab.c"
    break;

  case 12: /* decl_part: procedure_decl  */
#line 1470 "pascal.y"
                     { (yyval.ival) = 5; }
#line 2919 "pascal.tab.c"
    break;

  case 13: /* label_list: label_list COMMA INTCONST  */
#line 1473 "pascal.y"
                              { pas_label_in_range((yyvsp[0].ival)); }
#line 2925 "pascal.tab.c"
    break;

  case 14: /* label_list: INTCONST  */
#line 1474 "pascal.y"
               { pas_label_in_range((yyvsp[0].ival)); }
#line 2931 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 1480 "pascal.y"
                                           { pas_rconst_add((yyvsp[-3].str), (yyvsp[-1].dval)); }
#line 2937 "pascal.tab.c"
    break;

  case 18: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 1481 "pascal.y"
                                          { pas_rconst_add((yyvsp[-4].str), (yyvsp[-1].dval)); }
#line 2943 "pascal.tab.c"
    break;

  case 19: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 1482 "pascal.y"
                                           { pas_rconst_add((yyvsp[-4].str), -(yyvsp[-1].dval)); }
#line 2949 "pascal.tab.c"
    break;

  case 20: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 1483 "pascal.y"
                                       { if ((yyvsp[-1].str) && strlen((yyvsp[-1].str))==1) { pas_const_add((yyvsp[-3].str),(long long)(unsigned char)(yyvsp[-1].str)[0]); pas_charvar_add((yyvsp[-3].str)); } else pas_sconst_add((yyvsp[-3].str),(yyvsp[-1].str)); }
#line 2955 "pascal.tab.c"
    break;

  case 21: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 1484 "pascal.y"
                                    { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 2961 "pascal.tab.c"
    break;

  case 22: /* constant: scalar_constant  */
#line 1486 "pascal.y"
                    { (yyval.ival) = (yyvsp[0].ival); }
#line 2967 "pascal.tab.c"
    break;

  case 23: /* constant: PLUS scalar_constant  */
#line 1486 "pascal.y"
                                                        { (yyval.ival) = (yyvsp[0].ival); }
#line 2973 "pascal.tab.c"
    break;

  case 24: /* constant: MINUS scalar_constant  */
#line 1486 "pascal.y"
                                                                                             { (yyval.ival) = -(yyvsp[0].ival); }
#line 2979 "pascal.tab.c"
    break;

  case 25: /* scalar_constant: IDENT  */
#line 1487 "pascal.y"
                       { long long cv = 0; if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "true")) cv = 1; else if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "false")) cv = 0; else pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 2985 "pascal.tab.c"
    break;

  case 26: /* scalar_constant: INTCONST  */
#line 1487 "pascal.y"
                                                                                                                                                                                  { (yyval.ival) = (yyvsp[0].ival); }
#line 2991 "pascal.tab.c"
    break;

  case 27: /* scalar_constant: REALCONST  */
#line 1487 "pascal.y"
                                                                                                                                                                                                           { pas_real_is_not_ordinal((yyvsp[0].dval)); (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 2997 "pascal.tab.c"
    break;

  case 28: /* scalar_constant: STRINGCONST  */
#line 1487 "pascal.y"
                                                                                                                                                                                                                                                                              { (yyval.ival) = ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) ? (long long)(unsigned char)(yyvsp[0].str)[0] : 0; }
#line 3003 "pascal.tab.c"
    break;

  case 31: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 1492 "pascal.y"
                                     { long long _ty3 = ((yyvsp[-1].ival) == -4) ? -1 : (yyvsp[-1].ival); int _pk3 = ((yyvsp[-1].ival) == -4) || (g_pas_pend_typename && pas_rectype_is_packed(g_pas_pend_typename)); if (_ty3 < 0 && g_pas_pend_istfile) pas_tfiletype_add((yyvsp[-3].str)); if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_pas_pend_isbool) pas_booltype_add((yyvsp[-3].str)); if (_ty3 == -2) pas_settype_add((yyvsp[-3].str), g_pas_pend_set_hi); if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str), _pk3); if (g_pas_pend_enum_max >= 0) { pas_enumtype_add((yyvsp[-3].str), g_pas_pend_enum_max); pas_enumnames_add((yyvsp[-3].str), g_pas_pend_enum_names); } if (g_pas_pend_sub_high >= 0 && _ty3 < 0 && g_pas_pend_arr_ncols < 0) pas_subtype_add((yyvsp[-3].str), g_pas_pend_sub_low, g_pas_pend_sub_high); if (_ty3 >= 0 && !g_pas_pend_ptrtarget) { pas_arrtype_add((yyvsp[-3].str), _ty3, g_pas_pend_arr_ncols >= 0 ? 1 : 0, g_pas_pend_arr_ncols); } if (_ty3 == -1 && !g_pas_pend_ptrtarget && g_pas_pend_nf == 0 && g_pas_pend_enum_max < 0 && g_pas_pend_sub_high < 0 && g_pas_pend_typename) pas_typealias_add((yyvsp[-3].str), g_pas_pend_typename); pas_pend_reset(); }
#line 3009 "pascal.tab.c"
    break;

  case 32: /* type: simple_type  */
#line 1494 "pascal.y"
                { if (g_pas_pend_ptrtarget) { (yyval.ival) = -3; } else if ((yyvsp[0].ival) == -2) { (yyval.ival) = -2; } else if ((yyvsp[0].ival) >= 0 && g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0) { long long _tnc = pas_arrtype_ncols(g_pas_pend_typename); if (_tnc >= 0) g_pas_pend_arr_ncols = _tnc; (yyval.ival) = (yyvsp[0].ival); } else { (yyval.ival) = -1; } }
#line 3015 "pascal.tab.c"
    break;

  case 33: /* type: ARROW IDENT  */
#line 1495 "pascal.y"
                  { g_pas_pend_ptrtarget = ct_strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 3021 "pascal.tab.c"
    break;

  case 34: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 1496 "pascal.y"
                                                             { int _eic = g_pas_pend_ischar; int _wr = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); g_pas_pend_arr_wrap = _wr; g_pas_pend_arr_ptrto = g_pas_pend_ptrtarget ? g_pas_pend_ptrtarget : (g_pas_pend_typename ? (char *)pas_ptrtype_target(g_pas_pend_typename) : NULL); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_ischar = _eic; g_pas_pend_isarr = 1; (yyval.ival) = (yyvsp[-3].ival); }
#line 3027 "pascal.tab.c"
    break;

  case 35: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 1497 "pascal.y"
                                                                               { int _eic = g_pas_pend_ischar; g_pas_pend_ptrtarget = NULL; long long r = (yyvsp[-5].ival); long long c = (yyvsp[-3].ival); g_pas_pend_arr_ncols = c + 1; g_pas_pend_arr_ischar = _eic; g_pas_pend_isarr = 1; (yyval.ival) = (r + 1) * (c + 1) - 1; }
#line 3033 "pascal.tab.c"
    break;

  case 36: /* $@1: %empty  */
#line 1498 "pascal.y"
                          { g_pas_recbody_depth++; }
#line 3039 "pascal.tab.c"
    break;

  case 37: /* type: packed_opt RECORDSY $@1 record_body ENDSY  */
#line 1498 "pascal.y"
                                                                       { g_pas_recbody_depth--; g_pas_pend_ptrtarget = NULL; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_enum_max = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; (yyval.ival) = (yyvsp[-4].ival) ? -4 : -1; }
#line 3045 "pascal.tab.c"
    break;

  case 38: /* type: packed_opt SETSY OFSY simple_type  */
#line 1499 "pascal.y"
                                        { g_pas_pend_ptrtarget = NULL; g_pas_pend_set_hi = (yyvsp[0].ival); (yyval.ival) = -2; }
#line 3051 "pascal.tab.c"
    break;

  case 39: /* type: packed_opt FILESY  */
#line 1500 "pascal.y"
                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 3057 "pascal.tab.c"
    break;

  case 40: /* type: packed_opt FILESY OFSY type  */
#line 1501 "pascal.y"
                                  { int _cf = g_pas_pend_istfile || (g_pas_pend_typename && (!strcmp(g_pas_pend_typename, "text") || pas_rectype_has_file(g_pas_pend_typename)));
        for (int _i = 0; !_cf && _i < g_pas_pend_nf; _i++) if (g_pas_pend_fldfile[_i]) _cf = 1;
        if (_cf) { fprintf(stderr, "pascal: ISO 7185 6.4.3.5 violation: the component-type of a file-type shall not be a file-type nor a structured-type having a file-type as a component -- 'file of %s'\n", g_pas_pend_typename ? g_pas_pend_typename : "<file>"); g_pas_iso_errors++; }
        if (g_pas_pend_sub_high >= 0) { g_pas_pend_frng_lo = g_pas_pend_sub_low; g_pas_pend_frng_hi = g_pas_pend_sub_high; }
        else if (g_pas_pend_typename && pas_subtype_high(g_pas_pend_typename) >= 0) { g_pas_pend_frng_lo = pas_subtype_low(g_pas_pend_typename); g_pas_pend_frng_hi = pas_subtype_high(g_pas_pend_typename); }
        g_pas_pend_frng_ischar = g_pas_pend_ischar;
        pas_pend_reset(); g_pas_pend_istfile = 1; (yyval.ival) = -1; }
#line 3069 "pascal.tab.c"
    break;

  case 41: /* packed_opt: PACKEDSY  */
#line 1509 "pascal.y"
                     { (yyval.ival) = 1; }
#line 3075 "pascal.tab.c"
    break;

  case 42: /* packed_opt: %empty  */
#line 1509 "pascal.y"
                                   { (yyval.ival) = 0; }
#line 3081 "pascal.tab.c"
    break;

  case 43: /* simple_type: LPARENT id_list RPARENT  */
#line 1512 "pascal.y"
        { int _eo = 0; g_pas_pend_enum_names[0] = '\0';
          if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) {
              tree_t *_id = (yyvsp[-1].list)->items[i];
              if (_id && _id->v.sval) { if (_eo > 0) strncat(g_pas_pend_enum_names, ",", sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); strncat(g_pas_pend_enum_names, _id->v.sval, sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); pas_const_add(_id->v.sval, (long long)(_eo++)); } }
          g_pas_pend_enum_max = (long long)(_eo - 1);
          (yyval.ival) = _eo > 0 ? (long long)(_eo - 1) : -1; }
#line 3092 "pascal.tab.c"
    break;

  case 44: /* simple_type: IDENT  */
#line 1518 "pascal.y"
            { g_pas_pend_typename = ct_strdup((yyvsp[0].str)); g_pas_pend_isbool = pas_is_booltype((yyvsp[0].str)); g_pas_pend_istfile = pas_is_tfiletype((yyvsp[0].str)); g_pas_pend_ischar = !strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar"); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = ct_strdup(_pt); (yyval.ival) = -3; } else { if (!strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar")) { (yyval.ival) = 255; } else if (pas_is_settype((yyvsp[0].str))) { (yyval.ival) = -2; } else { long long _eh = pas_enumtype_high((yyvsp[0].str)); long long _sh = pas_subtype_high((yyvsp[0].str)); long long _ah = pas_arrtype_high((yyvsp[0].str)); if (_eh >= 0) { (yyval.ival) = _eh; } else if (_sh >= 0) { (yyval.ival) = _sh; } else if (_ah >= 0) { (yyval.ival) = _ah; } else { if (g_pas_recbody_depth == 0) pas_rectype_to_pend((yyvsp[0].str)); g_pas_pend_typename = ct_strdup((yyvsp[0].str)); (yyval.ival) = -1; } } } }
#line 3098 "pascal.tab.c"
    break;

  case 45: /* simple_type: constant DOTDOT constant  */
#line 1519 "pascal.y"
                               { g_pas_pend_sub_low = (yyvsp[-2].ival); g_pas_pend_sub_high = (yyvsp[0].ival); (yyval.ival) = (yyvsp[0].ival); }
#line 3104 "pascal.tab.c"
    break;

  case 49: /* record_field: id_list COLON type  */
#line 1529 "pascal.y"
                       { if ((yyvsp[-2].list)) { char *_svp = g_pas_pend_ptrtarget; int _svc = g_pas_pend_ischar; int _sva = g_pas_pend_arr_ischar; int _svr = g_pas_pend_isarr; for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { g_pas_pend_ptrtarget = _svp; g_pas_pend_ischar = _svc; g_pas_pend_arr_ischar = _sva; g_pas_pend_isarr = _svr; pas_pend_add((yyvsp[-2].list)->items[i]->v.sval); } } }
#line 3110 "pascal.tab.c"
    break;

  case 51: /* record_case_opt: CASESY IDENT COLON IDENT OFSY record_case_list  */
#line 1533 "pascal.y"
                                                   { if ((yyvsp[-4].str)) { g_pas_pend_typename = ct_strdup((yyvsp[-2].str)); pas_pend_add((yyvsp[-4].str)); } }
#line 3116 "pascal.tab.c"
    break;

  case 52: /* record_case_opt: CASESY IDENT OFSY record_case_list  */
#line 1534 "pascal.y"
                                         { if ((yyvsp[-2].str)) pas_pend_add((yyvsp[-2].str)); }
#line 3122 "pascal.tab.c"
    break;

  case 58: /* var_decl_list: var_decl_list var_decl  */
#line 1546 "pascal.y"
                           { (yyval.list) = pas_var_names_add((yyvsp[-1].list), (yyvsp[0].list)); }
#line 3128 "pascal.tab.c"
    break;

  case 59: /* var_decl_list: var_decl  */
#line 1547 "pascal.y"
               { (yyval.list) = pas_var_names_add(pnl_new(), (yyvsp[0].list)); }
#line 3134 "pascal.tab.c"
    break;

  case 60: /* var_decl: id_list COLON type SEMICOLON  */
#line 1549 "pascal.y"
                                       { long long _ty3 = ((yyvsp[-1].ival) == -4) ? -1 : (yyvsp[-1].ival); int _pk3 = ((yyvsp[-1].ival) == -4) || (g_pas_pend_typename && pas_rectype_is_packed(g_pas_pend_typename)); if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if (_ty3 == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if (_ty3 >= 0 && g_pas_pend_nf > 0) { pas_array_add2d(id->v.sval, (_ty3 + 1) * g_pas_pend_nf - 1, (long long)g_pas_pend_nf); pas_arrrec_add(id->v.sval, g_pas_pend_typename, g_pas_pend_nf); } else if (_ty3 >= 0) { long long _varnc = (g_pas_pend_arr_ncols >= 0) ? g_pas_pend_arr_ncols : pas_arrtype_ncols(g_pas_pend_typename); if (g_pas_pend_ischar && !g_pas_pend_arr_ischar && _varnc < 0 && g_pas_pend_nf == 0) { pas_charvar_add(id->v.sval); } else if (_varnc >= 0) { pas_array_add2d(id->v.sval, _ty3, _varnc); } else { pas_array_add(id->v.sval, _ty3); if (g_pas_pend_arr_ptrto) pas_arrptr_add(id->v.sval, g_pas_pend_arr_ptrto); int _aic = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); if (_aic && g_pas_pend_arr_wrap) pas_strarr_add2(id->v.sval, (g_pas_pend_typename && pas_arrtype_lo(g_pas_pend_typename) > 0) ? pas_arrtype_lo(g_pas_pend_typename) : 1); else if (_aic) pas_chararr_add2(id->v.sval, g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : pas_arrtype_lo(g_pas_pend_typename)); else if (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) pas_enumarr_add(id->v.sval, g_pas_pend_typename); } } if (_ty3 == -2) pas_setvar_add(id->v.sval); if (_ty3 < 0 && g_pas_pend_ischar) pas_charvar_add(id->v.sval); if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_pas_pend_isbool) pas_boolvar_add(id->v.sval); if (_ty3 < 0 && g_pas_pend_istfile) { pas_tfilevar_add(id->v.sval); pas_tfcomp_add(id->v.sval, g_pas_pend_frng_lo, g_pas_pend_frng_hi, g_pas_pend_frng_ischar); } if (_ty3 < 0 && !g_pas_pend_istfile && g_pas_pend_nf == 0 && !g_pas_pend_isarr && g_pas_pend_sub_high >= 0) { pas_subvar_add(id->v.sval, g_pas_pend_sub_low, g_pas_pend_sub_high); } else if (_ty3 < 0 && !g_pas_pend_istfile && g_pas_pend_nf == 0 && !g_pas_pend_isarr && g_pas_pend_typename && pas_subtype_high(g_pas_pend_typename) >= 0) { pas_subvar_add(id->v.sval, pas_subtype_low(g_pas_pend_typename), pas_subtype_high(g_pas_pend_typename)); } if (_ty3 < 0 && g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval, _pk3); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "text")) pas_filevar_add(id->v.sval); if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "pchar")) pas_pcharvar_add(id->v.sval); if (g_pas_pend_typename) pas_scalarvartype_add(id->v.sval, g_pas_pend_typename); } pas_local_add(id->v.sval); } } g_pas_pend_frng_lo = 0; g_pas_pend_frng_hi = -1; g_pas_pend_frng_ischar = 0; pas_pend_reset(); (yyval.list) = (yyvsp[-3].list); }
#line 3140 "pascal.tab.c"
    break;

  case 61: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 1551 "pascal.y"
                                                                               { pas_proc_add((yyvsp[-5].str)); pas_proc_vparams((yyvsp[-5].str), (yyvsp[-3].list)); pas_fwd_save((yyvsp[-5].str), (yyvsp[-3].list)); pas_ptrvar_release(); pas_recvar_release(); }
#line 3146 "pascal.tab.c"
    break;

  case 62: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 1552 "pascal.y"
                                                                                            { pas_func_add((yyvsp[-7].str)); pas_proc_vparams((yyvsp[-7].str), (yyvsp[-5].list)); if ((yyvsp[-3].str) && !strcmp((yyvsp[-3].str), "char")) pas_charvar_add((yyvsp[-7].str)); if (pas_is_booltype((yyvsp[-3].str))) pas_boolvar_add((yyvsp[-7].str)); if ((yyvsp[-3].str)) pas_scalarvartype_add((yyvsp[-7].str), (yyvsp[-3].str)); pas_fwd_save((yyvsp[-7].str), (yyvsp[-5].list)); pas_ptrvar_release(); pas_recvar_release(); }
#line 3152 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 1553 "pascal.y"
                                                             { pas_proc_add((yyvsp[-3].str)); pas_proc_vparams((yyvsp[-3].str), (yyvsp[-1].list)); pas_proc_enter(); pas_fwd_restore((yyvsp[-3].str)); }
#line 3158 "pascal.tab.c"
    break;

  case 64: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@2 block SEMICOLON  */
#line 1554 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-6].str), pas_fwd_params((yyvsp[-6].str), (yyvsp[-4].list)), pas_trace_wrap_proc((yyvsp[-6].str), (yyvsp[-4].list), (yyvsp[-1].node), 0), 0, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3166 "pascal.tab.c"
    break;

  case 65: /* $@3: %empty  */
#line 1557 "pascal.y"
                                                                        { pas_func_add((yyvsp[-5].str)); pas_proc_vparams((yyvsp[-5].str), (yyvsp[-3].list)); if ((yyvsp[-1].str) && !strcmp((yyvsp[-1].str), "char")) pas_charvar_add((yyvsp[-5].str)); if (pas_is_booltype((yyvsp[-1].str))) pas_boolvar_add((yyvsp[-5].str)); if ((yyvsp[-1].str)) pas_scalarvartype_add((yyvsp[-5].str), (yyvsp[-1].str)); pas_proc_enter(); pas_curfunc_push((yyvsp[-5].str)); pas_fwd_restore((yyvsp[-5].str)); }
#line 3172 "pascal.tab.c"
    break;

  case 66: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@3 block SEMICOLON  */
#line 1558 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-8].str), pas_fwd_params((yyvsp[-8].str), (yyvsp[-6].list)), pas_trace_wrap_proc((yyvsp[-8].str), (yyvsp[-6].list), (yyvsp[-1].node), 1), 1, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3180 "pascal.tab.c"
    break;

  case 67: /* $@4: %empty  */
#line 1561 "pascal.y"
                                         { pas_func_add((yyvsp[-2].str)); pas_proc_enter(); pas_curfunc_push((yyvsp[-2].str)); pas_fwd_restore((yyvsp[-2].str)); }
#line 3186 "pascal.tab.c"
    break;

  case 68: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@4 block SEMICOLON  */
#line 1562 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), pas_fwd_params((yyvsp[-5].str), pnl_new()), pas_trace_wrap_proc((yyvsp[-5].str), NULL, (yyvsp[-1].node), 1), 1, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); pas_recvar_release(); emit_proc(&g_pascal_procs, p); }
#line 3194 "pascal.tab.c"
    break;

  case 69: /* pv_mark: %empty  */
#line 1567 "pascal.y"
    { pas_ptrvar_mark(); pas_recvar_mark(); }
#line 3200 "pascal.tab.c"
    break;

  case 70: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 1570 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 3206 "pascal.tab.c"
    break;

  case 71: /* parameter_list_opt: %empty  */
#line 1571 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 3212 "pascal.tab.c"
    break;

  case 72: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 1574 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 3218 "pascal.tab.c"
    break;

  case 73: /* parameter_decl_list: parameter_decl  */
#line 1575 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 3224 "pascal.tab.c"
    break;

  case 74: /* parameter_decl: PROCEDURESY id_list  */
#line 1578 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 3230 "pascal.tab.c"
    break;

  case 75: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 1579 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 3236 "pascal.tab.c"
    break;

  case 76: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 1580 "pascal.y"
                                { if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); } } } for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 3242 "pascal.tab.c"
    break;

  case 77: /* parameter_decl: id_list COLON IDENT  */
#line 1581 "pascal.y"
                          { if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); if (!strcmp((yyvsp[0].str), "char")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_charvar_add((yyvsp[-2].list)->items[i]->v.sval); if (!strcmp((yyvsp[0].str), "single")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_singlevar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_realtypename((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_FLIT)); if (!strcmp((yyvsp[0].str), "pchar")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) { ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_QLIT)); if ((yyvsp[-2].list)->items[i]->v.sval) pas_pcharvar_add((yyvsp[-2].list)->items[i]->v.sval); } { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); int _aic = pas_arrtype_ischar((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); if (_aic) pas_chararr_add2((yyvsp[-2].list)->items[i]->v.sval, pas_arrtype_lo((yyvsp[0].str))); } } } (yyval.list) = (yyvsp[-2].list); }
#line 3248 "pascal.tab.c"
    break;

  case 78: /* id_list: id_list COMMA IDENT  */
#line 1584 "pascal.y"
                        { pas_not_a_word_symbol((yyvsp[0].str)); pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 3254 "pascal.tab.c"
    break;

  case 79: /* id_list: IDENT  */
#line 1585 "pascal.y"
            { pas_not_a_word_symbol((yyvsp[0].str)); PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 3260 "pascal.tab.c"
    break;

  case 80: /* body: BEGINSY statement_list ENDSY  */
#line 1588 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 3266 "pascal.tab.c"
    break;

  case 81: /* $@5: %empty  */
#line 1591 "pascal.y"
                             { pas_stmt_mark(); }
#line 3272 "pascal.tab.c"
    break;

  case 82: /* statement_list: statement_list SEMICOLON $@5 statement  */
#line 1591 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node)) { if (pas_trace_enabled() && (yyvsp[0].node)->t != TT_SUCCEED) pnl_push((yyvsp[-3].list), mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push((yyvsp[-3].list), (yyvsp[0].node)); } (yyval.list) = (yyvsp[-3].list); }
#line 3278 "pascal.tab.c"
    break;

  case 83: /* $@6: %empty  */
#line 1592 "pascal.y"
      { pas_stmt_mark(); }
#line 3284 "pascal.tab.c"
    break;

  case 84: /* statement_list: $@6 statement  */
#line 1592 "pascal.y"
                                     { int _ln = pas_stmt_line_pop(); PNodeList *l = pnl_new(); if ((yyvsp[0].node)) { if (pas_trace_enabled() && (yyvsp[0].node)->t != TT_SUCCEED) pnl_push(l, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(l, (yyvsp[0].node)); } (yyval.list) = l; }
#line 3290 "pascal.tab.c"
    break;

  case 85: /* statement: statement_no_label  */
#line 1595 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3296 "pascal.tab.c"
    break;

  case 86: /* statement: INTCONST COLON statement_no_label  */
#line 1597 "pascal.y"
        { pas_label_in_range((yyvsp[-2].ival)); char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)(yyvsp[-2].ival));
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = ct_strdup(_lb); ast_push(L, (yyvsp[0].node)); (yyval.node) = L; }
#line 3303 "pascal.tab.c"
    break;

  case 87: /* statement_no_label: assignment  */
#line 1601 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3309 "pascal.tab.c"
    break;

  case 88: /* statement_no_label: call  */
#line 1602 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3315 "pascal.tab.c"
    break;

  case 89: /* statement_no_label: compound_statement  */
#line 1603 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 3321 "pascal.tab.c"
    break;

  case 90: /* statement_no_label: goto_statement  */
#line 1604 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3327 "pascal.tab.c"
    break;

  case 91: /* statement_no_label: if_statement  */
#line 1605 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3333 "pascal.tab.c"
    break;

  case 92: /* statement_no_label: case_statement  */
#line 1606 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3339 "pascal.tab.c"
    break;

  case 93: /* statement_no_label: while_statement  */
#line 1607 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3345 "pascal.tab.c"
    break;

  case 94: /* statement_no_label: repeat_statement  */
#line 1608 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3351 "pascal.tab.c"
    break;

  case 95: /* statement_no_label: for_statement  */
#line 1609 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3357 "pascal.tab.c"
    break;

  case 96: /* statement_no_label: with_statement  */
#line 1610 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3363 "pascal.tab.c"
    break;

  case 97: /* statement_no_label: %empty  */
#line 1611 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 3369 "pascal.tab.c"
    break;

  case 98: /* call: IDENT  */
#line 1614 "pascal.y"
          { if (pas_is_proc((yyvsp[0].str))) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.node) = e; } else (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 3375 "pascal.tab.c"
    break;

  case 99: /* call: call_with_args  */
#line 1615 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3381 "pascal.tab.c"
    break;

  case 100: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 1618 "pascal.y"
                                        { if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) if (pas_proc_param_is_var((yyvsp[-3].str), i) && pas_actual_is_packed_component((yyvsp[-1].list)->items[i])) {
            fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a component of a packed structure is passed as the variable parameter %d of '%s'\n", i + 1, (yyvsp[-3].str));
            g_pas_iso_errors++; }
        pas_call_arity((yyvsp[-3].str), (yyvsp[-1].list)); pas_ordinal_fn_arg((yyvsp[-3].str), (yyvsp[-1].list));
        (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 3391 "pascal.tab.c"
    break;

  case 101: /* argument_list: argument_list COMMA argument  */
#line 1625 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 3397 "pascal.tab.c"
    break;

  case 102: /* argument_list: argument  */
#line 1626 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 3403 "pascal.tab.c"
    break;

  case 103: /* argument: expression  */
#line 1629 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 3409 "pascal.tab.c"
    break;

  case 104: /* argument: expression COLON expression  */
#line 1630 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 3415 "pascal.tab.c"
    break;

  case 105: /* argument: expression COLON expression COLON expression  */
#line 1631 "pascal.y"
                                                   { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-4].node))); pnl_push(_al, ilit(-3)); pnl_push(_al, (yyvsp[-2].node)); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 3421 "pascal.tab.c"
    break;

  case 106: /* assignment: selector BECOMES expression  */
#line 1635 "pascal.y"
        { if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) pas_value_compat((yyvsp[-2].node)->v.sval, (yyvsp[0].node), "6.8.2.2", "the variable");
          if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_FNC && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->v.sval && (!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_fbuf_get") || !strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get"))) {
              int _isqlit = (yyvsp[0].node) && ((yyvsp[0].node)->t == TT_QLIT || ((yyvsp[0].node)->t == TT_FNC && (yyvsp[0].node)->n >= 1 && (yyvsp[0].node)->c[0] && (yyvsp[0].node)->c[0]->v.sval && !strcmp((yyvsp[0].node)->c[0]->v.sval, "__pas_chrlit")));
              if (!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_fbuf_get") && (yyvsp[-2].node)->c[1] && (yyvsp[-2].node)->c[1]->t == TT_VAR && (yyvsp[-2].node)->c[1]->v.sval
                  && _isqlit && pas_tfcomp_nonchar((yyvsp[-2].node)->c[1]->v.sval)) {
                  fprintf(stderr, "pascal: ISO 7185 6.6.5.2 violation: the value assigned to the buffer-variable of '%s' is not assignment-compatible with its component-type\n", (yyvsp[-2].node)->c[1]->v.sval);
                  g_pas_iso_errors++;
              }
              (yyval.node) = mk_set_bin(!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get") ? "__pas_tbuf_set" : "__pas_fbuf_set", (yyvsp[-2].node)->c[1], pas_bool((yyvsp[0].node)));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval && pas_is_chararr((yyvsp[-2].node)->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _cah; if (!pas_array_high_get((yyvsp[-2].node)->v.sval, &_cah)) _cah = (long long)strlen((yyvsp[0].node)->v.sval);
              (yyval.node) = mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, pas_chararr_lo((yyvsp[-2].node)->v.sval), _cah));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR && (yyvsp[-2].node)->c[0]->v.sval && pas_is_strarr((yyvsp[-2].node)->c[0]->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _slo = pas_strarr_lo((yyvsp[-2].node)->c[0]->v.sval); long long _shi = _slo + (long long)strlen((yyvsp[0].node)->v.sval) - 1;
              (yyval.node) = mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, _slo, _shi));
          } else if (pas_is_cafield((yyvsp[-2].node))) {
              tree_t *_rhs; long long _flo = pas_cafield_lo_get((yyvsp[-2].node));
              if ((yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) { long long _fhi = pas_cafield_hi_get((yyvsp[-2].node)); if (_fhi < _flo) _fhi = _flo + (long long)strlen((yyvsp[0].node)->v.sval) - 1; _rhs = pas_str_to_alpha((yyvsp[0].node)->v.sval, _flo, _fhi); }
              else _rhs = pas_bool((yyvsp[0].node));
              tree_t *_pk = ast_node_new(TT_FNC); ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack")); ast_push(_pk, _rhs); ast_push(_pk, ilit(_flo));
              (yyval.node) = mk_assign((yyvsp[-2].node), _pk);
          } else { tree_t *_rhs0 = pas_bool((yyvsp[0].node));
              if (g_pas_range_check_on && (yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) { long long _rlo, _rhi; if (pas_subvar_get((yyvsp[-2].node)->v.sval, &_rlo, &_rhi)) _rhs0 = pas_range_wrap(_rhs0, _rlo, _rhi); }
              tree_t *_lvalclone = (pas_trace_enabled() && (yyvsp[-2].node) && (yyvsp[-2].node)->t != TT_VAR) ? pas_tree_clone((yyvsp[-2].node)) : NULL;
              tree_t *_asn = mk_assign((yyvsp[-2].node), _rhs0);
              const char *_tsn = pas_trace_enabled() ? pas_trace_store_name((yyvsp[-2].node)) : NULL;
              if (_tsn) {
                  tree_t *_tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, _tsn), pas_trace_wrap_value(leaf_s(TT_VAR, _tsn)));
                  PNodeList *_sl = pnl_new(); pnl_push(_sl, _asn); pnl_push(_sl, _tv); (yyval.node) = seq_of(_sl);
              } else if (_lvalclone) {
                  tree_t *_tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, "<lval>"), pas_trace_wrap_value(_lvalclone));
                  PNodeList *_sl = pnl_new(); pnl_push(_sl, _asn); pnl_push(_sl, _tv); (yyval.node) = seq_of(_sl);
              } else { (yyval.node) = _asn; } } }
#line 3459 "pascal.tab.c"
    break;

  case 107: /* selector: selector LBRACK expression_list RBRACK  */
#line 1670 "pascal.y"
                                           { tree_t *e = NULL; if ((yyvsp[-1].list) && (yyvsp[-1].list)->count == 2 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { long long _nc = pas_array_ncols((yyvsp[-3].node)->v.sval); if (_nc > 0) { tree_t *flat = bin(TT_ADD, bin(TT_MUL, (yyvsp[-1].list)->items[0], ilit(_nc)), (yyvsp[-1].list)->items[1]); e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); ast_push(e, flat); } } if (!e && (yyvsp[-1].list) && (yyvsp[-1].list)->count == 1 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_IDX && (yyvsp[-3].node)->n == 2 && (yyvsp[-3].node)->c[0] && (yyvsp[-3].node)->c[0]->t == TT_VAR && (yyvsp[-3].node)->c[0]->v.sval && !pas_is_nafield((yyvsp[-3].node))) {
        long long _nc2 = pas_array_ncols((yyvsp[-3].node)->c[0]->v.sval);
        if (_nc2 > 0) { tree_t *flat2 = bin(TT_ADD, bin(TT_MUL, (yyvsp[-3].node)->c[1], ilit(_nc2)), (yyvsp[-1].list)->items[0]);
          e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)->c[0]); ast_push(e, flat2); } }
      if (!e && pas_is_nafield((yyvsp[-3].node)) && (yyvsp[-1].list) && (yyvsp[-1].list)->count == 1) { long long _nlo = pas_nafield_lo_get((yyvsp[-3].node)); tree_t *_ei = (yyvsp[-1].list)->items[0];
        if (_nlo != 0) _ei = bin(TT_SUB, _ei, ilit(_nlo));
        e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); ast_push(e, _ei); pas_nrec_mark_add(e); }
      if (!e) { e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); } if (e && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { const char *_et = pas_enumarr_get((yyvsp[-3].node)->v.sval); if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } (yyval.node) = e; }
#line 3472 "pascal.tab.c"
    break;

  case 108: /* selector: selector PERIOD IDENT  */
#line 1678 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi < 0 && (yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR) { const char *_arn = NULL; int _anf = pas_arrrec_find((yyvsp[-2].node)->c[0]->v.sval, &_arn); if (_anf > 0) { int _afi = _arn ? pas_rectype_field_index(_arn, (yyvsp[0].str)) : -1; if (_afi < 0) { _afi = pas_arrrec_field_index((yyvsp[-2].node)->c[0]->v.sval, (yyvsp[0].str)); } if (_afi < 0) { for (int _ri = 0; _ri < g_pas_nrectype; _ri++) { int _t = pas_rectype_field_index(g_pas_rectypes[_ri].tname, (yyvsp[0].str)); if (_t >= 0 && g_pas_rectypes[_ri].nf == _anf) { _afi = _t; break; } } } if (_afi >= 0) { (yyval.node) = pas_arrrec_flatten((yyvsp[-2].node), _afi); if (pas_arrrec_field_is_char((yyvsp[-2].node)->c[0]->v.sval, _afi) || (_arn && pas_rectype_field_is_char(_arn, _afi))) pas_cvfield_mark_add((yyval.node)); const char *_fe = pas_arrrec_field_enum((yyvsp[-2].node)->c[0]->v.sval, _afi); if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi); if (_fe && (yyval.node)) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) (yyval.node)->v.ival = (long long)(_ei + 1); } } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
        else if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_fi)); if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } { const char *_mrt = _rt ? _rt : pas_with_sel_rtype((yyvsp[-2].node)); if (_mrt && pas_rectype_field_is_ca(_mrt, _fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_mrt, _fi), pas_rectype_field_ca_hi(_mrt, _fi)); if (_mrt && pas_rectype_field_is_char(_mrt, _fi)) pas_cvfield_mark_add(e);
            if (_mrt && pas_rectype_field_is_na(_mrt, _fi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_mrt, _fi)); } (yyval.node) = e; } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
#line 3481 "pascal.tab.c"
    break;

  case 109: /* selector: selector ARROW  */
#line 1682 "pascal.y"
                     { (yyval.node) = pas_is_tfile_node((yyvsp[-1].node)) ? mk_fnc1("__pas_fbuf_get", (yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && pas_is_filevar((yyvsp[-1].node)->v.sval) && !pas_is_stdstream((yyvsp[-1].node)->v.sval)) ? mk_fnc1("__pas_tbuf_get", (yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && pas_is_pcharvar((yyvsp[-1].node)->v.sval)) ? mk_fnc1("__pas_pchar_deref", (yyvsp[-1].node)) : mk_deref((yyvsp[-1].node)))); }
#line 3487 "pascal.tab.c"
    break;

  case 110: /* selector: IDENT  */
#line 1683 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 3493 "pascal.tab.c"
    break;

  case 111: /* expression_list: expression_list COMMA expression  */
#line 1686 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 3499 "pascal.tab.c"
    break;

  case 112: /* expression_list: expression  */
#line 1687 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 3505 "pascal.tab.c"
    break;

  case 113: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 1690 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 3511 "pascal.tab.c"
    break;

  case 114: /* goto_statement: GOTOSY INTCONST  */
#line 1694 "pascal.y"
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)(yyvsp[0].ival));
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = ct_strdup(_gb); G->line = pascal_get_lineno(); (yyval.node) = G; }
#line 3518 "pascal.tab.c"
    break;

  case 115: /* if_statement: IFSY expression THENSY statement  */
#line 1698 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond_bool((yyvsp[-2].node), "an if-statement", "6.8.3.4"), (yyvsp[0].node)); }
#line 3524 "pascal.tab.c"
    break;

  case 116: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 1699 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond_bool((yyvsp[-4].node), "an if-statement", "6.8.3.4")); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 3530 "pascal.tab.c"
    break;

  case 117: /* $@7: %empty  */
#line 1702 "pascal.y"
                           { pas_case_push(); }
#line 3536 "pascal.tab.c"
    break;

  case 118: /* case_statement: CASESY expression OFSY $@7 case_list ENDSY  */
#line 1703 "pascal.y"
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), (yyvsp[-4].node)));
          tree_t *chain = ast_node_new(TT_FNC); ast_push(chain, leaf_s(TT_VAR, "__pas_rterr")); ast_push(chain, leaf_s(TT_QLIT, "6.8.3.5"));
          ast_push(chain, leaf_s(TT_QLIT, "no case-constant of the case-statement is equal to the value of its case-index"));
          if ((yyvsp[-1].list)) for (int i = (yyvsp[-1].list)->count - 1; i >= 0; i--) { tree_t *e = (yyvsp[-1].list)->items[i]; if (!e) continue; ast_push(e, chain); chain = e; }
          ast_push(seq, chain);
          pas_case_pop();
          (yyval.node) = seq; }
#line 3549 "pascal.tab.c"
    break;

  case 119: /* case_list: case_list SEMICOLON case_elem  */
#line 1713 "pascal.y"
                                  { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 3555 "pascal.tab.c"
    break;

  case 120: /* case_list: case_elem  */
#line 1714 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 3561 "pascal.tab.c"
    break;

  case 121: /* case_elem: constant_list COLON statement  */
#line 1717 "pascal.y"
                                  { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3567 "pascal.tab.c"
    break;

  case 122: /* case_elem: %empty  */
#line 1718 "pascal.y"
      { (yyval.node) = NULL; }
#line 3573 "pascal.tab.c"
    break;

  case 123: /* constant_list: constant_list COMMA constant  */
#line 1721 "pascal.y"
                                 { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival)))); }
#line 3579 "pascal.tab.c"
    break;

  case 124: /* constant_list: constant  */
#line 1722 "pascal.y"
               { (yyval.node) = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival))); }
#line 3585 "pascal.tab.c"
    break;

  case 125: /* while_statement: WHILESY expression DOSY statement  */
#line 1725 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond_bool((yyvsp[-2].node), "a while-statement", "6.8.3.8"), (yyvsp[0].node)); }
#line 3591 "pascal.tab.c"
    break;

  case 126: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 1728 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond_bool((yyvsp[0].node), "a repeat-statement", "6.8.3.7")); }
#line 3597 "pascal.tab.c"
    break;

  case 127: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 1732 "pascal.y"
        { pas_value_compat((yyvsp[-6].str), (yyvsp[-4].node), "6.8.3.9", "the control-variable"); pas_value_compat((yyvsp[-6].str), (yyvsp[-2].node), "6.8.3.9", "the control-variable");
          if (pas_var_is_real((yyvsp[-6].str))) { fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", (yyvsp[-6].str)); g_pas_iso_errors++; }
          pas_for_const_bounds((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-2].node), 0);
          tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, pas_trace_wrap_for_body((yyvsp[-6].str), (yyvsp[0].node))); (yyval.node) = e; }
#line 3606 "pascal.tab.c"
    break;

  case 128: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 1737 "pascal.y"
        { pas_value_compat((yyvsp[-6].str), (yyvsp[-4].node), "6.8.3.9", "the control-variable"); pas_value_compat((yyvsp[-6].str), (yyvsp[-2].node), "6.8.3.9", "the control-variable");
          if (pas_var_is_real((yyvsp[-6].str))) { fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", (yyvsp[-6].str)); g_pas_iso_errors++; }
          pas_for_const_bounds((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-2].node), 1);
          tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, pas_trace_wrap_for_body((yyvsp[-6].str), (yyvsp[0].node))); e->v.ival = 1; (yyval.node) = e; }
#line 3615 "pascal.tab.c"
    break;

  case 129: /* with_statement: WITHSY with_open DOSY statement  */
#line 1743 "pascal.y"
                                    { long long n = (yyvsp[-2].ival); for (long long i = 0; i < n; i++) pas_with_pop(); (yyval.node) = (yyvsp[0].node); }
#line 3621 "pascal.tab.c"
    break;

  case 130: /* with_open: with_open COMMA selector  */
#line 1746 "pascal.y"
                             { pas_with_push((yyvsp[0].node)); (yyval.ival) = (yyvsp[-2].ival) + 1; }
#line 3627 "pascal.tab.c"
    break;

  case 131: /* with_open: selector  */
#line 1747 "pascal.y"
               { pas_with_push((yyvsp[0].node)); (yyval.ival) = 1; }
#line 3633 "pascal.tab.c"
    break;

  case 132: /* expression: simple_expression  */
#line 1754 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3639 "pascal.tab.c"
    break;

  case 133: /* expression: expression INOP simple_expression  */
#line 1755 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3645 "pascal.tab.c"
    break;

  case 134: /* expression: expression LTOP simple_expression  */
#line 1756 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3651 "pascal.tab.c"
    break;

  case 135: /* expression: expression LEOP simple_expression  */
#line 1757 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3657 "pascal.tab.c"
    break;

  case 136: /* expression: expression GTOP simple_expression  */
#line 1758 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3663 "pascal.tab.c"
    break;

  case 137: /* expression: expression GEOP simple_expression  */
#line 1759 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3669 "pascal.tab.c"
    break;

  case 138: /* expression: expression NEOP simple_expression  */
#line 1760 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_NE, "__pas_setne", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3675 "pascal.tab.c"
    break;

  case 139: /* expression: expression EQOP simple_expression  */
#line 1761 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_EQ, "__pas_seteq", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3681 "pascal.tab.c"
    break;

  case 140: /* simple_expression: term  */
#line 1764 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3687 "pascal.tab.c"
    break;

  case 141: /* simple_expression: PLUS term  */
#line 1765 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3693 "pascal.tab.c"
    break;

  case 142: /* simple_expression: MINUS term  */
#line 1766 "pascal.y"
                 { (yyval.node) = mk_neg((yyvsp[0].node)); }
#line 3699 "pascal.tab.c"
    break;

  case 143: /* simple_expression: simple_expression PLUS term  */
#line 1767 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3705 "pascal.tab.c"
    break;

  case 144: /* simple_expression: simple_expression MINUS term  */
#line 1768 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3711 "pascal.tab.c"
    break;

  case 145: /* simple_expression: simple_expression OROP term  */
#line 1769 "pascal.y"
                                  { (yyval.node) = (pas_is_boolexpr((yyvsp[-2].node)) && pas_is_boolexpr((yyvsp[0].node))) ? bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)) : mk_fnc2("ior", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3717 "pascal.tab.c"
    break;

  case 146: /* term: factor  */
#line 1772 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3723 "pascal.tab.c"
    break;

  case 147: /* term: term MUL factor  */
#line 1773 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3729 "pascal.tab.c"
    break;

  case 148: /* term: term RDIV factor  */
#line 1774 "pascal.y"
                       { (yyval.node) = pas_rdiv((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3735 "pascal.tab.c"
    break;

  case 149: /* term: term IDIV factor  */
#line 1775 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3741 "pascal.tab.c"
    break;

  case 150: /* term: term IMOD factor  */
#line 1776 "pascal.y"
                       { (yyval.node) = pas_mod((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3747 "pascal.tab.c"
    break;

  case 151: /* term: term ANDOP factor  */
#line 1777 "pascal.y"
                        { (yyval.node) = (pas_is_boolexpr((yyvsp[-2].node)) && pas_is_boolexpr((yyvsp[0].node))) ? bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)) : mk_fnc2("iand", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3753 "pascal.tab.c"
    break;

  case 152: /* factor: selector  */
#line 1780 "pascal.y"
             { if (pas_is_nrec_idx((yyvsp[0].node)) && (yyvsp[0].node)->n >= 2 && (yyvsp[0].node)->c[0] && (yyvsp[0].node)->c[0]->t == TT_IDX && (yyvsp[0].node)->c[0]->n >= 2) {
          tree_t *g = ast_node_new(TT_FNC); ast_push(g, leaf_s(TT_VAR, "__pas_nrec_get"));
          ast_push(g, (yyvsp[0].node)->c[0]->c[0]); ast_push(g, (yyvsp[0].node)->c[0]->c[1]); ast_push(g, (yyvsp[0].node)->c[1]);
          if (pas_is_cvfield((yyvsp[0].node))) pas_cvfield_mark_add(g);
          (yyval.node) = g; }
      else if (pas_is_cafield((yyvsp[0].node))) { tree_t *u = ast_node_new(TT_FNC); ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack")); ast_push(u, (yyvsp[0].node)); ast_push(u, ilit(pas_cafield_lo_get((yyvsp[0].node)))); (yyval.node) = u; } else (yyval.node) = (yyvsp[0].node); }
#line 3764 "pascal.tab.c"
    break;

  case 153: /* factor: call_with_args  */
#line 1786 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3770 "pascal.tab.c"
    break;

  case 154: /* factor: INTCONST  */
#line 1787 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 3776 "pascal.tab.c"
    break;

  case 155: /* factor: REALCONST  */
#line 1788 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 3782 "pascal.tab.c"
    break;

  case 156: /* factor: STRINGCONST  */
#line 1789 "pascal.y"
                  { if ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((long long)(unsigned char)(yyvsp[0].str)[0])); (yyval.node) = _cl; } else (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 3788 "pascal.tab.c"
    break;

  case 157: /* factor: LPARENT expression RPARENT  */
#line 1790 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3794 "pascal.tab.c"
    break;

  case 158: /* factor: NOTSY factor  */
#line 1791 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 3800 "pascal.tab.c"
    break;

  case 159: /* factor: ATSIGN factor  */
#line 1792 "pascal.y"
                    { (yyval.node) = pas_addr_of_proc((yyvsp[0].node)); }
#line 3806 "pascal.tab.c"
    break;

  case 160: /* factor: LBRACK RBRACK  */
#line 1793 "pascal.y"
                    { (yyval.node) = mk_set_ctor(NULL); }
#line 3812 "pascal.tab.c"
    break;

  case 161: /* factor: LBRACK set_member_list RBRACK  */
#line 1794 "pascal.y"
                                    { (yyval.node) = (yyvsp[-1].node); }
#line 3818 "pascal.tab.c"
    break;

  case 162: /* set_member_list: set_member  */
#line 1797 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3824 "pascal.tab.c"
    break;

  case 163: /* set_member_list: set_member_list COMMA set_member  */
#line 1798 "pascal.y"
                                       { (yyval.node) = mk_set_bin("__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3830 "pascal.tab.c"
    break;

  case 164: /* set_member: expression  */
#line 1801 "pascal.y"
               { PNodeList *_l = pnl_new(); pnl_push(_l, (yyvsp[0].node)); (yyval.node) = mk_set_ctor(_l); }
#line 3836 "pascal.tab.c"
    break;

  case 165: /* set_member: expression DOTDOT expression  */
#line 1802 "pascal.y"
                                   { (yyval.node) = mk_set_bin("__pas_setrange", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 3842 "pascal.tab.c"
    break;


#line 3846 "pascal.tab.c"

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

#line 1808 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    g_pas_seen_mode_directive = 0; pascal_seen_decl_start = 0; g_pas_align_mac68k = 0;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0; g_pas_ncaparm = 0; g_pas_pend_arr_ncols = -1;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0; g_pas_nsettype = 0; g_pas_ncharvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; g_pas_narrtype = 0; g_pas_nboolvar = 0; g_pas_nbooltype = 0; g_pas_pend_isbool = 0; g_pas_ntfilevar = 0; g_pas_ntfiletype = 0; g_pas_pend_istfile = 0;
    g_pas_nenum = 0; g_pas_pend_enum_max = -1; g_pas_nsubtype = 0; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0; g_with_depth = 0;
    g_pas_nchararr = 0; g_pas_pend_arr_ischar = 0; g_pas_nrconst = 0; g_pas_nsconst = 0; g_pas_narrrec = 0; g_pas_nrvmark = 0; g_pas_nnafield = 0; g_pas_nrec_nmarks = 0; g_pas_ncafield = 0; g_pas_ncvfield = 0; g_pas_narrptr = 0; g_pas_pend_arr_ptrto = NULL;
    g_pas_nenumname = 0; g_pas_nenumarr = 0; g_pas_pend_enum_names[0] = '\0'; g_pas_nrec_nmarks = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
