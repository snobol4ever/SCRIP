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
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b);
static int g_pas_pend_isbool;
static int g_pas_pend_istfile;
static void pas_tfilevar_add(const char *name);
static int pas_is_tfilevar(const char *name);
static void pas_tfiletype_add(const char *name);
static int pas_is_tfiletype(const char *name);
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
        if (pas_is_charexpr(a) || pas_is_boolexpr(a)) return bin(TT_ADD, a, ilit(0));
        return a;
    }
    if (name && (!strcmp(name, "pack") || !strcmp(name, "unpack")) && args && args->count >= 5) {
        int ispack = !strcmp(name, "pack");
        tree_t *a = args->items[ispack ? 0 : 2]; tree_t *i = args->items[ispack ? 2 : 4]; tree_t *z = args->items[ispack ? 4 : 0];
        long long zhi = -1; long long zlo = 1;
        if (z && z->t == TT_VAR && z->v.sval) { if (!pas_array_high_get(z->v.sval, &zhi)) zhi = -1; if (pas_is_chararr(z->v.sval)) zlo = pas_chararr_lo(z->v.sval); }
        static int _pkn = 0; char _cvb[24]; snprintf(_cvb, sizeof _cvb, "__pas_pk%d", _pkn++); const char *_cv = strdup(_cvb);
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
        if (!strcmp(name, "get")) return mk_fnc1("__pas_fget", fa);
        if (!strcmp(name, "put")) return mk_fnc1("__pas_fput", fa);
        if (!strcmp(name, "eof")) return mk_fnc1("__pas_feof_t", fa);
        if (!strcmp(name, "reset")) return mk_assign(fa, mk_set_bin("__pas_treset", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval)));
        if (!strcmp(name, "rewrite")) return mk_assign(fa, mk_set_bin("__pas_trewrite", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval)));
        if (!strcmp(name, "read") || !strcmp(name, "write")) {
            PNodeList *stmts = pnl_new();
            for (int i = 2; i + 1 < args->count; i += 2) {
                tree_t *v = args->items[i];
                if (!strcmp(name, "read")) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_fread", pas_tree_clone(fa))));
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
    }
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
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
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_rewrite", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_reset", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_reset", pas_tree_clone(fv)));
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
    if (name && (!strcmp(name, "mark") || !strcmp(name, "release")) && args && args->count >= 1) {
        return mk_fnc1("__pas_dispose", args->items[0]);
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
static struct { char *vname; char *fields[PAS_FIELD_MAX]; int nf; int fldchar[PAS_FIELD_MAX]; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
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
static struct { char *pname; char *vnames[16]; char *rnames[16]; int n; PNodeList *params; } g_pas_fwdpv[64]; static int g_pas_nfwdpv;
static void pas_fwd_save(const char *pn, PNodeList *params) { if (!pn || g_pas_nfwdpv >= 64 || g_pas_npvmark == 0) return; int from = g_pas_pvmarks[g_pas_npvmark - 1]; int k = g_pas_nfwdpv++; g_pas_fwdpv[k].pname = strdup(pn); g_pas_fwdpv[k].params = params; g_pas_fwdpv[k].n = 0; for (int i = from; i < g_pas_nptrvar && g_pas_fwdpv[k].n < 16; i++) { g_pas_fwdpv[k].vnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].vname; g_pas_fwdpv[k].rnames[g_pas_fwdpv[k].n] = g_pas_ptrvars[i].rname; g_pas_fwdpv[k].n++; } }
static PNodeList *pas_fwd_params(const char *pn, PNodeList *given) { if (given && given->count > 0) return given; if (pn) for (int i = 0; i < g_pas_nfwdpv; i++) if (g_pas_fwdpv[i].pname && !strcmp(g_pas_fwdpv[i].pname, pn) && g_pas_fwdpv[i].params) return g_pas_fwdpv[i].params; return given; }
static void pas_fwd_restore(const char *pn) { if (!pn) return; for (int i = 0; i < g_pas_nfwdpv; i++) if (g_pas_fwdpv[i].pname && !strcmp(g_pas_fwdpv[i].pname, pn)) { for (int j = 0; j < g_pas_fwdpv[i].n; j++) pas_ptrvar_add(g_pas_fwdpv[i].vnames[j], g_pas_fwdpv[i].rnames[j]); return; } }
static void pas_pend_reset(void) { g_pas_pend_isbool = 0; g_pas_pend_istfile = 0; g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ptrto = NULL; g_pas_pend_typename = NULL; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; g_pas_pend_enum_max = -1; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_wrap = 0; }
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
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; g_pas_recvars[k].fldchar[i] = g_pas_pend_fldchar[i]; } }
static void pas_recvar_add_from_type(const char *vn, const char *tn) { if (!vn || !tn) return; for (int ri = 0; ri < g_pas_nrectype; ri++) { if (!g_pas_rectypes[ri].tname || strcmp(g_pas_rectypes[ri].tname, tn)) continue; if (g_pas_nrecvar >= PAS_REC_MAX) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = strdup(vn); g_pas_recvars[k].nf = g_pas_rectypes[ri].nf; for (int j = 0; j < g_pas_rectypes[ri].nf; j++) { g_pas_recvars[k].fields[j] = g_pas_rectypes[ri].fields[j]; g_pas_recvars[k].fldchar[j] = g_pas_rectypes[ri].fldchar[j]; } return; } }
static int pas_recvar_field_is_char(const char *vn, long idx) { if (!vn || idx < 0) return 0; for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) return (idx < g_pas_recvars[i].nf) ? g_pas_recvars[i].fldchar[idx] : 0; return 0; }
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
static struct { char *name; } g_pas_boolvars[512]; static int g_pas_nboolvar;
static struct { char *name; } g_pas_tfilevars[128]; static int g_pas_ntfilevar;
static struct { char *name; } g_pas_tfiletypes[64]; static int g_pas_ntfiletype;
static void pas_tfilevar_add(const char *name) { if (g_pas_ntfilevar < 128 && name) g_pas_tfilevars[g_pas_ntfilevar++].name = strdup(name); }
static int pas_is_tfilevar(const char *name) { if (!name) return 0; for (int i = g_pas_ntfilevar - 1; i >= 0; i--) if (g_pas_tfilevars[i].name && !strcmp(g_pas_tfilevars[i].name, name)) return 1; return 0; }
static void pas_tfiletype_add(const char *name) { if (g_pas_ntfiletype < 64 && name) g_pas_tfiletypes[g_pas_ntfiletype++].name = strdup(name); }
static int pas_is_tfiletype(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_ntfiletype; i++) if (g_pas_tfiletypes[i].name && !strcmp(g_pas_tfiletypes[i].name, name)) return 1; return 0; }
static int pas_is_tfile_node(const tree_t *e) { return e && e->t == TT_VAR && e->v.sval && pas_is_tfilevar(e->v.sval); }
static struct { char *name; } g_pas_booltypes[64]; static int g_pas_nbooltype;
static void pas_boolvar_add(const char *name) { if (g_pas_nboolvar < 512 && name) g_pas_boolvars[g_pas_nboolvar++].name = strdup(name); }
static int pas_is_boolvar(const char *name) { if (!name) return 0; for (int i = g_pas_nboolvar - 1; i >= 0; i--) if (g_pas_boolvars[i].name && !strcmp(g_pas_boolvars[i].name, name)) return 1; return 0; }
static void pas_booltype_add(const char *name) { if (g_pas_nbooltype < 64 && name) g_pas_booltypes[g_pas_nbooltype++].name = strdup(name); }
static int pas_is_booltype(const char *name) { if (!name) return 0; if (!strcmp(name, "boolean")) return 1; for (int i = 0; i < g_pas_nbooltype; i++) if (g_pas_booltypes[i].name && !strcmp(g_pas_booltypes[i].name, name)) return 1; return 0; }
static int pas_is_boolexpr(tree_t *e) { if (!e) return 0; switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_NOT: return 1; case TT_MUL: case TT_ADD: return e->n == 2 && pas_is_boolexpr(e->c[0]) && pas_is_boolexpr(e->c[1]); case TT_VAR: return pas_is_boolvar(e->v.sval); case TT_FNC: { const char *fn = (e->n >= 1 && e->c[0]) ? e->c[0]->v.sval : NULL; if (!fn) return 0; if (!strcmp(fn, "__pas_in") || !strcmp(fn, "__pas_eof") || !strcmp(fn, "__pas_eoln") || !strcmp(fn, "__pas_eof_f") || !strcmp(fn, "__pas_eoln_f") || !strcmp(fn, "__pas_feof_t") || !strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne") || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")) return 1; return pas_is_boolvar(fn); } default: return 0; } }
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
static int pas_is_charexpr(tree_t *e) { if (!e) return 0; if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1; if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[1]->t == TT_ILIT && pas_recvar_field_is_char(e->c[0]->v.sval, e->c[1]->v.ival)) return 1; if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1; if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_charvar(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_chararr(e->c[0]->v.sval)) return 1; if (e->t == TT_IDX && e->n >= 2 && e->c[0] && pas_is_cafield(e->c[0])) return 1; if (pas_is_cvfield(e)) return 1; return 0; }
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
static tree_t *pas_rdiv(tree_t *a, tree_t *b) { return bin(TT_DIV, bin(TT_MUL, a, flit(1.0)), b); }
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
    if (name && !strcmp(name, "true"))  return bin(TT_EQ, ilit(1), ilit(1));
    if (name && !strcmp(name, "false")) return bin(TT_EQ, ilit(0), ilit(1));
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

#line 746 "pascal.tab.c"

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
  YYSYMBOL_77_1 = 77,                      /* $@1  */
  YYSYMBOL_packed_opt = 78,                /* packed_opt  */
  YYSYMBOL_simple_type = 79,               /* simple_type  */
  YYSYMBOL_record_body = 80,               /* record_body  */
  YYSYMBOL_record_field_list = 81,         /* record_field_list  */
  YYSYMBOL_record_field = 82,              /* record_field  */
  YYSYMBOL_record_case_opt = 83,           /* record_case_opt  */
  YYSYMBOL_record_case_list = 84,          /* record_case_list  */
  YYSYMBOL_record_case_arm = 85,           /* record_case_arm  */
  YYSYMBOL_var_decl_list = 86,             /* var_decl_list  */
  YYSYMBOL_var_decl = 87,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 88,            /* procedure_decl  */
  YYSYMBOL_89_2 = 89,                      /* $@2  */
  YYSYMBOL_90_3 = 90,                      /* $@3  */
  YYSYMBOL_91_4 = 91,                      /* $@4  */
  YYSYMBOL_pv_mark = 92,                   /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 93,        /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 94,       /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 95,            /* parameter_decl  */
  YYSYMBOL_id_list = 96,                   /* id_list  */
  YYSYMBOL_body = 97,                      /* body  */
  YYSYMBOL_statement_list = 98,            /* statement_list  */
  YYSYMBOL_statement = 99,                 /* statement  */
  YYSYMBOL_statement_no_label = 100,       /* statement_no_label  */
  YYSYMBOL_call = 101,                     /* call  */
  YYSYMBOL_call_with_args = 102,           /* call_with_args  */
  YYSYMBOL_argument_list = 103,            /* argument_list  */
  YYSYMBOL_argument = 104,                 /* argument  */
  YYSYMBOL_assignment = 105,               /* assignment  */
  YYSYMBOL_selector = 106,                 /* selector  */
  YYSYMBOL_expression_list = 107,          /* expression_list  */
  YYSYMBOL_compound_statement = 108,       /* compound_statement  */
  YYSYMBOL_goto_statement = 109,           /* goto_statement  */
  YYSYMBOL_if_statement = 110,             /* if_statement  */
  YYSYMBOL_case_statement = 111,           /* case_statement  */
  YYSYMBOL_112_5 = 112,                    /* $@5  */
  YYSYMBOL_case_list = 113,                /* case_list  */
  YYSYMBOL_case_elem = 114,                /* case_elem  */
  YYSYMBOL_constant_list = 115,            /* constant_list  */
  YYSYMBOL_while_statement = 116,          /* while_statement  */
  YYSYMBOL_repeat_statement = 117,         /* repeat_statement  */
  YYSYMBOL_for_statement = 118,            /* for_statement  */
  YYSYMBOL_with_statement = 119,           /* with_statement  */
  YYSYMBOL_with_open = 120,                /* with_open  */
  YYSYMBOL_expression = 121,               /* expression  */
  YYSYMBOL_simple_expression = 122,        /* simple_expression  */
  YYSYMBOL_term = 123,                     /* term  */
  YYSYMBOL_factor = 124,                   /* factor  */
  YYSYMBOL_set_member_list = 125,          /* set_member_list  */
  YYSYMBOL_set_member = 126                /* set_member  */
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
#define YYLAST   434

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  162
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  327

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
       0,   704,   704,   720,   721,   724,   727,   728,   731,   732,
     733,   734,   735,   738,   739,   742,   743,   745,   746,   747,
     748,   749,   751,   751,   751,   752,   752,   752,   752,   754,
     755,   757,   759,   760,   761,   762,   763,   763,   764,   765,
     766,   768,   768,   770,   777,   778,   781,   784,   785,   788,
     789,   792,   793,   794,   797,   798,   801,   802,   805,   806,
     808,   810,   811,   812,   812,   816,   816,   820,   820,   826,
     829,   830,   833,   834,   837,   838,   839,   840,   843,   844,
     847,   850,   851,   854,   855,   860,   861,   862,   863,   864,
     865,   866,   867,   868,   869,   870,   873,   874,   877,   880,
     881,   884,   885,   886,   889,   907,   908,   911,   912,   915,
     916,   919,   922,   927,   928,   931,   931,   941,   942,   945,
     946,   949,   950,   953,   956,   959,   961,   965,   968,   969,
     976,   977,   978,   979,   980,   981,   982,   983,   986,   987,
     988,   989,   990,   991,   994,   995,   996,   997,   998,   999,
    1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,  1013,
    1014,  1017,  1018
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
  "type_decl", "type", "$@1", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@2", "$@3", "$@4", "pv_mark", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "id_list", "body",
  "statement_list", "statement", "statement_no_label", "call",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@5", "case_list", "case_elem",
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

#define YYPACT_NINF (-219)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-109)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      27,   -19,    84,    37,  -219,     5,    89,  -219,   -15,  -219,
      42,  -219,    59,   308,  -219,  -219,    66,    82,   104,     5,
     105,    20,   109,  -219,  -219,  -219,  -219,    11,   129,    82,
    -219,  -219,     5,  -219,   100,  -219,   119,   124,    20,   245,
     127,   245,    20,   245,   142,    65,    13,  -219,  -219,  -219,
    -219,  -219,   108,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,   159,   109,  -219,  -219,   146,   309,  -219,   154,  -219,
      95,     6,  -219,   181,     9,    70,    70,    70,   223,   245,
    -219,  -219,  -219,   176,  -219,   167,   139,    67,    97,  -219,
    -219,   167,    40,   253,    17,   320,    41,   245,    20,  -219,
     245,   182,  -219,   245,    95,  -219,  -219,   269,   288,  -219,
     238,   242,  -219,   243,  -219,    46,   244,  -219,   349,   349,
     209,     5,  -219,  -219,   231,   232,   276,    24,  -219,  -219,
     248,   245,   245,  -219,    97,    97,  -219,   268,    57,  -219,
     300,    20,   245,   245,   245,   245,   245,   245,   245,    70,
      70,    70,    70,    70,    70,    70,    70,    20,   127,  -219,
    -219,    20,  -219,   113,  -219,   280,  -219,   362,  -219,    72,
     362,   298,   312,  -219,   313,  -219,  -219,  -219,  -219,     5,
       5,     5,     7,  -219,   135,   310,  -219,   115,   339,  -219,
     281,   317,   330,  -219,  -219,   290,   211,   362,   245,   245,
    -219,  -219,  -219,    67,    67,    67,    67,    67,    67,    67,
      97,    97,    97,  -219,  -219,  -219,  -219,  -219,  -219,   167,
     339,   325,   245,  -219,   245,   245,  -219,  -219,  -219,  -219,
     303,   145,   164,    46,  -219,   297,   373,  -219,  -219,  -219,
     333,    95,   333,     5,   376,   377,   245,   245,   362,  -219,
    -219,    86,  -219,   175,    20,  -219,   332,   362,   305,   321,
    -219,  -219,  -219,   382,   103,  -219,  -219,   365,    63,  -219,
     178,  -219,   381,   158,   197,   339,  -219,   339,    20,  -219,
     245,  -219,  -219,  -219,   333,   370,  -219,     5,   335,  -219,
      95,   407,  -219,    20,    20,  -219,  -219,  -219,   362,   358,
      95,  -219,     8,  -219,  -219,   409,  -219,  -219,   394,  -219,
     339,   354,  -219,    95,   412,  -219,   236,   397,  -219,   339,
     363,   339,  -219,     5,   412,   364,  -219
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    79,     0,     7,
       0,     3,     0,     0,    78,     2,     0,     0,     0,     0,
       0,    95,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    69,    11,    59,     0,    69,     0,     0,    95,     0,
       0,     0,    95,     0,     0,    96,     0,    82,    83,    86,
      97,    85,     0,    87,    88,    89,    90,    91,    92,    93,
      94,     0,    10,    30,     8,     0,     0,    15,    71,    58,
      42,    71,   112,     0,     0,     0,     0,     0,     0,     0,
     152,   153,   154,   108,   151,   150,     0,   130,   138,   144,
     108,   129,     0,     0,     0,     0,    95,     0,    95,    80,
       0,     0,   107,     0,    42,    29,    13,     0,     0,    26,
       0,     0,    25,     0,    22,     0,     0,    41,     0,     0,
       0,     0,    27,    28,    44,     0,     0,     0,    32,    67,
       0,     0,     0,   156,   139,   140,   157,   161,     0,   159,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,   115,
     111,    95,    84,     0,   100,   101,    81,   104,   106,     0,
     110,     0,     0,    23,     0,    24,    17,    20,    21,     0,
       0,     0,     0,    73,     0,    63,    33,     0,     0,    60,
       0,    39,     0,    36,     7,     0,     0,   124,     0,     0,
     158,   155,   123,   131,   132,   133,   134,   135,   136,   137,
     143,   141,   142,   147,   148,   149,   145,   146,   127,   128,
     120,   113,     0,    98,     0,     0,   105,    31,    18,    19,
      74,     0,     0,     0,    70,     0,     0,     7,    43,    45,
       0,    42,     0,    50,     0,     0,     0,     0,   162,   160,
     122,     0,   118,     0,    95,    99,   102,   109,     0,     0,
      72,    77,    61,     0,     0,    40,    38,     0,    53,    48,
       0,    68,    65,     0,     0,   120,   116,     0,    95,   114,
       0,    76,    75,    64,     0,     0,    37,    50,     0,    46,
      42,     0,     7,    95,    95,   117,   121,   119,   103,     0,
      42,    47,     0,    49,    62,     0,   126,   125,     0,    34,
      57,     0,    66,    42,    52,    55,     0,     0,    35,    57,
       0,    57,    54,    50,    51,     0,    56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -219,  -219,  -219,  -190,  -219,  -219,  -219,  -219,   391,   -60,
     116,  -219,   360,   -94,  -219,  -219,  -201,   101,  -219,   136,
    -219,   106,   107,  -219,   393,  -219,  -219,  -219,  -219,   395,
     357,  -219,   196,    -4,  -219,    54,   -85,   336,  -219,   -18,
    -219,   212,  -219,   -21,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,   156,  -218,  -219,  -219,  -219,  -219,  -219,   -34,   120,
     -50,   -67,  -219,   234
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,   125,
     114,    62,    63,   126,   243,   127,   128,   267,   268,   269,
     289,   314,   315,    32,    33,    24,   237,   292,   194,    68,
     116,   182,   183,   270,    25,    46,    47,    48,    49,    84,
     163,   164,    51,    85,   169,    53,    54,    55,    56,   220,
     251,   252,   316,    57,    58,    59,    60,    92,   137,    87,
      88,    89,   138,   139
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,     8,   253,    50,   244,    86,   113,    93,   133,    95,
     171,   129,   233,   166,    98,    34,    64,    52,    98,    91,
      50,    52,    98,    36,    50,   132,   134,   135,    34,   310,
     190,     1,    37,    38,    39,    10,    99,    40,    41,   264,
     160,   266,    11,     3,    36,   140,   191,   263,   192,    42,
     157,   193,    43,    37,    38,    39,   202,   253,    40,    41,
     311,    65,   115,   165,   234,   179,   167,     7,   287,   170,
      42,   180,   218,    43,   181,    52,   221,    52,    50,    44,
      50,   288,    45,   299,     4,   213,   214,   215,   216,   217,
     158,   275,    74,     5,     9,  -108,    94,   196,   197,   210,
     211,   212,   305,    45,    14,    75,   149,   199,     7,   276,
      15,   184,   200,   150,   151,   117,  -108,   187,  -108,  -108,
      52,    97,   225,    50,    78,    26,    79,   226,   239,    80,
      81,    82,    83,   152,   153,   154,    52,   219,   100,    50,
      52,   118,   119,    50,    28,   155,   156,   265,   120,   141,
      10,   121,    70,   284,   109,   122,   123,   124,   285,   101,
     250,   102,   103,   222,   248,    10,    31,    35,   293,   279,
     223,    61,   238,   142,    66,   230,   231,   232,    72,   143,
     144,   145,   146,   147,   148,    10,    73,   235,   165,    90,
     256,   257,   142,   297,    96,    10,   303,   258,   143,   144,
     145,   146,   147,   148,   104,   106,   309,   294,   306,   307,
     115,   131,   273,   274,    10,   250,   259,   296,   101,   318,
     102,   103,   246,   173,   175,   277,   247,   278,    10,   184,
     290,   142,    97,    52,   173,   175,    50,   143,   144,   145,
     146,   147,   148,   176,   168,   142,   298,   177,   178,   185,
     250,   143,   144,   145,   146,   147,   148,    52,    75,   250,
      50,   250,   203,   204,   205,   206,   207,   208,   209,    76,
      77,   186,    52,    52,   159,    50,    50,    78,   136,    79,
      75,   189,    80,    81,    82,    83,   277,   142,   320,   -25,
     188,    76,    77,   143,   144,   145,   146,   147,   148,    78,
     195,    79,   142,   227,    80,    81,    82,    83,   143,   144,
     145,   146,   147,   148,   142,    16,    17,   228,   229,   236,
     143,   144,   145,   146,   147,   148,   198,    18,   109,   172,
     123,   112,   224,    19,   142,   240,    20,    21,   241,    22,
     143,   144,   145,   146,   147,   148,   161,   109,   174,   123,
     112,   242,   245,    10,   142,   107,   108,   201,   254,   261,
     143,   144,   145,   146,   147,   148,   142,   281,   109,   110,
     111,   112,   143,   144,   145,   146,   147,   148,   262,   118,
     119,   271,   272,   282,   280,   118,   119,   283,   286,   121,
     291,   300,   109,   122,   123,   124,   142,   302,   109,   122,
     123,   112,   143,   144,   145,   146,   147,   148,   109,   122,
     123,   112,   304,   308,   312,   313,   317,   319,   321,   323,
      67,   326,   105,   301,   325,    69,   322,   324,   130,   260,
      71,   295,   162,   249,   255
};

static const yytype_int16 yycheck[] =
{
      21,     5,   220,    21,   194,    39,    66,    41,    75,    43,
     104,     5,     5,    98,     5,    19,     5,    38,     5,    40,
      38,    42,     5,     3,    42,    16,    76,    77,    32,    21,
       6,     4,    12,    13,    14,    50,    23,    17,    18,   240,
      23,   242,    57,    62,     3,    79,    22,   237,    24,    29,
      10,    27,    32,    12,    13,    14,   141,   275,    17,    18,
      52,    50,    56,    97,    57,    19,   100,    62,     5,   103,
      29,    25,   157,    32,    28,    96,   161,    98,    96,    59,
      98,    18,    62,   284,     0,   152,   153,   154,   155,   156,
      50,     5,    38,    56,     5,    30,    42,   131,   132,   149,
     150,   151,   292,    62,    62,    35,    39,    50,    62,    23,
      51,   115,    55,    46,    47,    20,    51,   121,    53,    54,
     141,    56,    50,   141,    54,    59,    56,    55,   188,    59,
      60,    61,    62,    36,    37,    38,   157,   158,    30,   157,
     161,    46,    47,   161,    62,    48,    49,   241,    53,    10,
      50,    56,    52,    50,    59,    60,    61,    62,    55,    51,
     220,    53,    54,    50,   198,    50,    62,    62,    10,   254,
      57,    62,    57,    34,    45,   179,   180,   181,    59,    40,
      41,    42,    43,    44,    45,    50,    62,    52,   222,    62,
     224,   225,    34,   278,    52,    50,   290,    52,    40,    41,
      42,    43,    44,    45,    45,    59,   300,    10,   293,   294,
      56,    30,   246,   247,    50,   275,    52,   277,    51,   313,
      53,    54,    11,   107,   108,    50,    15,    52,    50,   233,
      52,    34,    56,   254,   118,   119,   254,    40,    41,    42,
      43,    44,    45,     5,    62,    34,   280,     5,     5,     5,
     310,    40,    41,    42,    43,    44,    45,   278,    35,   319,
     278,   321,   142,   143,   144,   145,   146,   147,   148,    46,
      47,    62,   293,   294,    21,   293,   294,    54,    55,    56,
      35,     5,    59,    60,    61,    62,    50,    34,    52,    58,
      58,    46,    47,    40,    41,    42,    43,    44,    45,    54,
      52,    56,    34,     5,    59,    60,    61,    62,    40,    41,
      42,    43,    44,    45,    34,     7,     8,     5,     5,     9,
      40,    41,    42,    43,    44,    45,    58,    19,    59,    60,
      61,    62,    52,    25,    34,    54,    28,    29,    21,    31,
      40,    41,    42,    43,    44,    45,    26,    59,    60,    61,
      62,    21,    62,    50,    34,    46,    47,    57,    33,    62,
      40,    41,    42,    43,    44,    45,    34,    62,    59,    60,
      61,    62,    40,    41,    42,    43,    44,    45,     5,    46,
      47,     5,     5,    62,    52,    46,    47,     5,    23,    56,
       9,    21,    59,    60,    61,    62,    34,    62,    59,    60,
      61,    62,    40,    41,    42,    43,    44,    45,    59,    60,
      61,    62,     5,    55,     5,    21,    62,     5,    21,    56,
      29,    57,    62,   287,   323,    32,   319,   321,    71,   233,
      35,   275,    96,   199,   222
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    64,    62,     0,    56,    65,    62,    96,     5,
      50,    57,    66,    67,    62,    51,     7,     8,    19,    25,
      28,    29,    31,    68,    88,    97,    59,    69,    62,    70,
      71,    62,    86,    87,    96,    62,     3,    12,    13,    14,
      17,    18,    29,    32,    59,    62,    98,    99,   100,   101,
     102,   105,   106,   108,   109,   110,   111,   116,   117,   118,
     119,    62,    74,    75,     5,    50,    45,    71,    92,    87,
      52,    92,    59,    62,    98,    35,    46,    47,    54,    56,
      59,    60,    61,    62,   102,   106,   121,   122,   123,   124,
      62,   106,   120,   121,    98,   121,    52,    56,     5,    23,
      30,    51,    53,    54,    45,    75,    59,    46,    47,    59,
      60,    61,    62,    72,    73,    56,    93,    20,    46,    47,
      53,    56,    60,    61,    62,    72,    76,    78,    79,     5,
      93,    30,    16,   124,   123,   123,    55,   121,   125,   126,
     121,    10,    34,    40,    41,    42,    43,    44,    45,    39,
      46,    47,    36,    37,    38,    48,    49,    10,    50,    21,
      23,    26,   100,   103,   104,   121,    99,   121,    62,   107,
     121,    76,    60,    73,    60,    73,     5,     5,     5,    19,
      25,    28,    94,    95,    96,     5,    62,    96,    58,     5,
       6,    22,    24,    27,    91,    52,   121,   121,    58,    50,
      55,    57,    99,   122,   122,   122,   122,   122,   122,   122,
     123,   123,   123,   124,   124,   124,   124,   124,    99,   106,
     112,    99,    50,    57,    52,    50,    55,     5,     5,     5,
      96,    96,    96,     5,    57,    52,     9,    89,    57,    72,
      54,    21,    21,    77,    66,    62,    11,    15,   121,   126,
      72,   113,   114,   115,    33,   104,   121,   121,    52,    52,
      95,    62,     5,    66,    79,    76,    79,    80,    81,    82,
      96,     5,     5,   121,   121,     5,    23,    50,    52,    99,
      52,    62,    62,     5,    50,    55,    23,     5,    18,    83,
      52,     9,    90,    10,    10,   114,    72,    99,   121,    79,
      21,    82,    62,    76,     5,    66,    99,    99,    55,    76,
      21,    52,     5,    21,    84,    85,   115,    62,    76,     5,
      52,    21,    85,    56,    84,    80,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    67,    67,    68,    68,
      68,    68,    68,    69,    69,    70,    70,    71,    71,    71,
      71,    71,    72,    72,    72,    73,    73,    73,    73,    74,
      74,    75,    76,    76,    76,    76,    77,    76,    76,    76,
      76,    78,    78,    79,    79,    79,    80,    81,    81,    82,
      82,    83,    83,    83,    84,    84,    85,    85,    86,    86,
      87,    88,    88,    89,    88,    90,    88,    91,    88,    92,
      93,    93,    94,    94,    95,    95,    95,    95,    96,    96,
      97,    98,    98,    99,    99,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   101,   101,   102,   103,
     103,   104,   104,   104,   105,   106,   106,   106,   106,   107,
     107,   108,   109,   110,   110,   112,   111,   113,   113,   114,
     114,   115,   115,   116,   117,   118,   118,   119,   120,   120,
     121,   121,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   123,   123,   123,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   125,
     125,   126,   126
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
       3,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     4,     3,
       1,     1,     3,     5,     3,     4,     3,     2,     1,     3,
       1,     3,     2,     4,     6,     0,     6,     3,     1,     3,
       0,     3,     1,     4,     4,     8,     8,     4,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     3,     2,     2,     3,     1,
       3,     1,     3
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
#line 705 "pascal.y"
        { tree_t *body = (yyvsp[-1].node);
          if (g_pas_narray > 0 || g_pas_nhdrfile > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_nhdrfile; i++) if (g_pas_hdrfiles[i]) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_hdrfiles[i]), leaf_s(TT_QLIT, g_pas_hdrfiles[i])));
              for (int i = 0; i < g_pas_narray; i++) if (!g_pas_arrays[i].is_param && !g_pas_arrays[i].is_local) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_init(g_pas_arrays[i].name, g_pas_arrays[i].high)));
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0); emit_proc(&g_pascal_procs, mainp);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
#line 2127 "pascal.tab.c"
    break;

  case 3: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 720 "pascal.y"
                            { if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) { tree_t *id = (yyvsp[-1].list)->items[i]; if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) { pas_filevar_add(id->v.sval); if (g_pas_nhdrfile < 32) g_pas_hdrfiles[g_pas_nhdrfile++] = strdup(id->v.sval); } } }
#line 2133 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 724 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2139 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 745 "pascal.y"
                                           { pas_rconst_add((yyvsp[-3].str), (yyvsp[-1].dval)); }
#line 2145 "pascal.tab.c"
    break;

  case 18: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 746 "pascal.y"
                                          { pas_rconst_add((yyvsp[-4].str), (yyvsp[-1].dval)); }
#line 2151 "pascal.tab.c"
    break;

  case 19: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 747 "pascal.y"
                                           { pas_rconst_add((yyvsp[-4].str), -(yyvsp[-1].dval)); }
#line 2157 "pascal.tab.c"
    break;

  case 20: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 748 "pascal.y"
                                       { if ((yyvsp[-1].str) && strlen((yyvsp[-1].str))==1) pas_const_add((yyvsp[-3].str),(long long)(unsigned char)(yyvsp[-1].str)[0]); else pas_sconst_add((yyvsp[-3].str),(yyvsp[-1].str)); }
#line 2163 "pascal.tab.c"
    break;

  case 21: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 749 "pascal.y"
                                    { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 2169 "pascal.tab.c"
    break;

  case 22: /* constant: scalar_constant  */
#line 751 "pascal.y"
                    { (yyval.ival) = (yyvsp[0].ival); }
#line 2175 "pascal.tab.c"
    break;

  case 23: /* constant: PLUS scalar_constant  */
#line 751 "pascal.y"
                                                        { (yyval.ival) = (yyvsp[0].ival); }
#line 2181 "pascal.tab.c"
    break;

  case 24: /* constant: MINUS scalar_constant  */
#line 751 "pascal.y"
                                                                                             { (yyval.ival) = -(yyvsp[0].ival); }
#line 2187 "pascal.tab.c"
    break;

  case 25: /* scalar_constant: IDENT  */
#line 752 "pascal.y"
                       { long long cv = 0; if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "true")) cv = 1; else if ((yyvsp[0].str) && !strcmp((yyvsp[0].str), "false")) cv = 0; else pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 2193 "pascal.tab.c"
    break;

  case 26: /* scalar_constant: INTCONST  */
#line 752 "pascal.y"
                                                                                                                                                                                  { (yyval.ival) = (yyvsp[0].ival); }
#line 2199 "pascal.tab.c"
    break;

  case 27: /* scalar_constant: REALCONST  */
#line 752 "pascal.y"
                                                                                                                                                                                                           { (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 2205 "pascal.tab.c"
    break;

  case 28: /* scalar_constant: STRINGCONST  */
#line 752 "pascal.y"
                                                                                                                                                                                                                                                 { (yyval.ival) = ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) ? (long long)(unsigned char)(yyvsp[0].str)[0] : 0; }
#line 2211 "pascal.tab.c"
    break;

  case 31: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 757 "pascal.y"
                                     { if ((yyvsp[-1].ival) < 0 && g_pas_pend_istfile) pas_tfiletype_add((yyvsp[-3].str)); if ((yyvsp[-1].ival) < 0 && (yyvsp[-1].ival) != -2 && (yyvsp[-1].ival) != -3 && g_pas_pend_isbool) pas_booltype_add((yyvsp[-3].str)); if ((yyvsp[-1].ival) == -2) pas_settype_add((yyvsp[-3].str)); if (g_pas_pend_ptrtarget) pas_ptrtype_add((yyvsp[-3].str), g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add((yyvsp[-3].str)); if (g_pas_pend_enum_max >= 0) { pas_enumtype_add((yyvsp[-3].str), g_pas_pend_enum_max); pas_enumnames_add((yyvsp[-3].str), g_pas_pend_enum_names); } if (g_pas_pend_sub_high >= 0 && (yyvsp[-1].ival) < 0 && g_pas_pend_arr_ncols < 0) pas_subtype_add((yyvsp[-3].str), g_pas_pend_sub_low, g_pas_pend_sub_high); if ((yyvsp[-1].ival) >= 0 && !g_pas_pend_ptrtarget) { pas_arrtype_add((yyvsp[-3].str), (yyvsp[-1].ival), g_pas_pend_arr_ncols >= 0 ? 1 : 0, g_pas_pend_arr_ncols); } pas_pend_reset(); }
#line 2217 "pascal.tab.c"
    break;

  case 32: /* type: simple_type  */
#line 759 "pascal.y"
                { if (g_pas_pend_ptrtarget) { (yyval.ival) = -3; } else if ((yyvsp[0].ival) == -2) { (yyval.ival) = -2; } else if ((yyvsp[0].ival) >= 0 && g_pas_pend_typename && pas_arrtype_high(g_pas_pend_typename) >= 0) { long long _tnc = pas_arrtype_ncols(g_pas_pend_typename); if (_tnc >= 0) g_pas_pend_arr_ncols = _tnc; (yyval.ival) = (yyvsp[0].ival); } else { (yyval.ival) = -1; } }
#line 2223 "pascal.tab.c"
    break;

  case 33: /* type: ARROW IDENT  */
#line 760 "pascal.y"
                  { g_pas_pend_ptrtarget = strdup((yyvsp[0].str)); (yyval.ival) = -3; }
#line 2229 "pascal.tab.c"
    break;

  case 34: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 761 "pascal.y"
                                                             { int _eic = g_pas_pend_ischar; int _wr = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); g_pas_pend_arr_wrap = _wr; g_pas_pend_arr_ptrto = g_pas_pend_ptrtarget ? g_pas_pend_ptrtarget : (g_pas_pend_typename ? (char *)pas_ptrtype_target(g_pas_pend_typename) : NULL); g_pas_pend_ptrtarget = NULL; g_pas_pend_arr_ncols = -1; g_pas_pend_arr_ischar = _eic; (yyval.ival) = (yyvsp[-3].ival); }
#line 2235 "pascal.tab.c"
    break;

  case 35: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 762 "pascal.y"
                                                                               { int _eic = g_pas_pend_ischar; g_pas_pend_ptrtarget = NULL; long long r = (yyvsp[-5].ival); long long c = (yyvsp[-3].ival); g_pas_pend_arr_ncols = c + 1; g_pas_pend_arr_ischar = _eic; (yyval.ival) = (r + 1) * (c + 1) - 1; }
#line 2241 "pascal.tab.c"
    break;

  case 36: /* $@1: %empty  */
#line 763 "pascal.y"
                          { g_pas_recbody_depth++; }
#line 2247 "pascal.tab.c"
    break;

  case 37: /* type: packed_opt RECORDSY $@1 record_body ENDSY  */
#line 763 "pascal.y"
                                                                       { g_pas_recbody_depth--; g_pas_pend_ptrtarget = NULL; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1; g_pas_pend_enum_max = -1; g_pas_pend_arr_ncols = -1; g_pas_pend_ischar = 0; g_pas_pend_arr_ischar = 0; (yyval.ival) = -1; }
#line 2253 "pascal.tab.c"
    break;

  case 38: /* type: packed_opt SETSY OFSY simple_type  */
#line 764 "pascal.y"
                                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -2; }
#line 2259 "pascal.tab.c"
    break;

  case 39: /* type: packed_opt FILESY  */
#line 765 "pascal.y"
                        { g_pas_pend_ptrtarget = NULL; (yyval.ival) = -1; }
#line 2265 "pascal.tab.c"
    break;

  case 40: /* type: packed_opt FILESY OFSY type  */
#line 766 "pascal.y"
                                  { pas_pend_reset(); g_pas_pend_istfile = 1; (yyval.ival) = -1; }
#line 2271 "pascal.tab.c"
    break;

  case 43: /* simple_type: LPARENT id_list RPARENT  */
#line 771 "pascal.y"
        { int _eo = 0; g_pas_pend_enum_names[0] = '\0';
          if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) {
              tree_t *_id = (yyvsp[-1].list)->items[i];
              if (_id && _id->v.sval) { if (_eo > 0) strncat(g_pas_pend_enum_names, ",", sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); strncat(g_pas_pend_enum_names, _id->v.sval, sizeof g_pas_pend_enum_names - strlen(g_pas_pend_enum_names) - 1); pas_const_add(_id->v.sval, (long long)(_eo++)); } }
          g_pas_pend_enum_max = (long long)(_eo - 1);
          (yyval.ival) = _eo > 0 ? (long long)(_eo - 1) : -1; }
#line 2282 "pascal.tab.c"
    break;

  case 44: /* simple_type: IDENT  */
#line 777 "pascal.y"
            { g_pas_pend_typename = strdup((yyvsp[0].str)); g_pas_pend_isbool = pas_is_booltype((yyvsp[0].str)); g_pas_pend_istfile = pas_is_tfiletype((yyvsp[0].str)); g_pas_pend_ischar = !strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar"); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); (yyval.ival) = -3; } else { if (!strcmp((yyvsp[0].str), "char") || !strcmp((yyvsp[0].str), "widechar")) { (yyval.ival) = 255; } else if (pas_is_settype((yyvsp[0].str))) { (yyval.ival) = -2; } else { long long _eh = pas_enumtype_high((yyvsp[0].str)); long long _sh = pas_subtype_high((yyvsp[0].str)); long long _ah = pas_arrtype_high((yyvsp[0].str)); if (_eh >= 0) { (yyval.ival) = _eh; } else if (_sh >= 0) { (yyval.ival) = _sh; } else if (_ah >= 0) { (yyval.ival) = _ah; } else { if (g_pas_recbody_depth == 0) pas_rectype_to_pend((yyvsp[0].str)); g_pas_pend_typename = strdup((yyvsp[0].str)); (yyval.ival) = -1; } } } }
#line 2288 "pascal.tab.c"
    break;

  case 45: /* simple_type: constant DOTDOT constant  */
#line 778 "pascal.y"
                               { g_pas_pend_sub_low = (yyvsp[-2].ival); g_pas_pend_sub_high = (yyvsp[0].ival); (yyval.ival) = (yyvsp[0].ival); }
#line 2294 "pascal.tab.c"
    break;

  case 49: /* record_field: id_list COLON type  */
#line 788 "pascal.y"
                       { if ((yyvsp[-2].list)) { char *_svp = g_pas_pend_ptrtarget; int _svc = g_pas_pend_ischar; int _sva = g_pas_pend_arr_ischar; for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { g_pas_pend_ptrtarget = _svp; g_pas_pend_ischar = _svc; g_pas_pend_arr_ischar = _sva; pas_pend_add((yyvsp[-2].list)->items[i]->v.sval); } } }
#line 2300 "pascal.tab.c"
    break;

  case 51: /* record_case_opt: CASESY IDENT COLON IDENT OFSY record_case_list  */
#line 792 "pascal.y"
                                                   { if ((yyvsp[-4].str)) { g_pas_pend_typename = strdup((yyvsp[-2].str)); pas_pend_add((yyvsp[-4].str)); } }
#line 2306 "pascal.tab.c"
    break;

  case 52: /* record_case_opt: CASESY IDENT OFSY record_case_list  */
#line 793 "pascal.y"
                                         { if ((yyvsp[-2].str)) pas_pend_add((yyvsp[-2].str)); }
#line 2312 "pascal.tab.c"
    break;

  case 60: /* var_decl: id_list COLON type SEMICOLON  */
#line 808 "pascal.y"
                                       { if ((yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) { if ((yyvsp[-1].ival) == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ((yyvsp[-1].ival) >= 0 && g_pas_pend_nf > 0) { pas_array_add2d(id->v.sval, ((yyvsp[-1].ival) + 1) * g_pas_pend_nf - 1, (long long)g_pas_pend_nf); pas_arrrec_add(id->v.sval, g_pas_pend_typename, g_pas_pend_nf); } else if ((yyvsp[-1].ival) >= 0) { long long _varnc = (g_pas_pend_arr_ncols >= 0) ? g_pas_pend_arr_ncols : pas_arrtype_ncols(g_pas_pend_typename); if (g_pas_pend_ischar && !g_pas_pend_arr_ischar && _varnc < 0 && g_pas_pend_nf == 0) { pas_charvar_add(id->v.sval); } else if (_varnc >= 0) { pas_array_add2d(id->v.sval, (yyvsp[-1].ival), _varnc); } else { pas_array_add(id->v.sval, (yyvsp[-1].ival)); if (g_pas_pend_arr_ptrto) pas_arrptr_add(id->v.sval, g_pas_pend_arr_ptrto); int _aic = g_pas_pend_arr_ischar || (g_pas_pend_typename && pas_arrtype_ischar(g_pas_pend_typename)); if (_aic && g_pas_pend_arr_wrap) pas_strarr_add2(id->v.sval, (g_pas_pend_typename && pas_arrtype_lo(g_pas_pend_typename) > 0) ? pas_arrtype_lo(g_pas_pend_typename) : 1); else if (_aic) pas_chararr_add2(id->v.sval, g_pas_pend_arr_ischar ? (g_pas_pend_sub_low > 0 ? g_pas_pend_sub_low : 0) : pas_arrtype_lo(g_pas_pend_typename)); else if (g_pas_pend_typename && pas_enumnames_idx(g_pas_pend_typename) >= 0) pas_enumarr_add(id->v.sval, g_pas_pend_typename); } } if ((yyvsp[-1].ival) == -2) pas_setvar_add(id->v.sval); if ((yyvsp[-1].ival) < 0 && g_pas_pend_ischar) pas_charvar_add(id->v.sval); if ((yyvsp[-1].ival) < 0 && (yyvsp[-1].ival) != -2 && (yyvsp[-1].ival) != -3 && g_pas_pend_isbool) pas_boolvar_add(id->v.sval); if ((yyvsp[-1].ival) < 0 && g_pas_pend_istfile) pas_tfilevar_add(id->v.sval); if ((yyvsp[-1].ival) < 0 && g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } if (g_pas_pend_typename && !strcmp(g_pas_pend_typename, "text")) pas_filevar_add(id->v.sval); } pas_local_add(id->v.sval); } } pas_pend_reset(); }
#line 2318 "pascal.tab.c"
    break;

  case 61: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 810 "pascal.y"
                                                                               { pas_proc_add((yyvsp[-5].str)); pas_fwd_save((yyvsp[-5].str), (yyvsp[-3].list)); pas_ptrvar_release(); }
#line 2324 "pascal.tab.c"
    break;

  case 62: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 811 "pascal.y"
                                                                                            { pas_func_add((yyvsp[-7].str)); if ((yyvsp[-3].str) && !strcmp((yyvsp[-3].str), "char")) pas_charvar_add((yyvsp[-7].str)); if (pas_is_booltype((yyvsp[-3].str))) pas_boolvar_add((yyvsp[-7].str)); pas_fwd_save((yyvsp[-7].str), (yyvsp[-5].list)); pas_ptrvar_release(); }
#line 2330 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 812 "pascal.y"
                                                             { pas_proc_add((yyvsp[-3].str)); pas_proc_enter(); pas_fwd_restore((yyvsp[-3].str)); }
#line 2336 "pascal.tab.c"
    break;

  case 64: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@2 block SEMICOLON  */
#line 813 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-6].str), pas_fwd_params((yyvsp[-6].str), (yyvsp[-4].list)), (yyvsp[-1].node), 0, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); emit_proc(&g_pascal_procs, p); }
#line 2344 "pascal.tab.c"
    break;

  case 65: /* $@3: %empty  */
#line 816 "pascal.y"
                                                                        { pas_func_add((yyvsp[-5].str)); if ((yyvsp[-1].str) && !strcmp((yyvsp[-1].str), "char")) pas_charvar_add((yyvsp[-5].str)); if (pas_is_booltype((yyvsp[-1].str))) pas_boolvar_add((yyvsp[-5].str)); pas_proc_enter(); pas_fwd_restore((yyvsp[-5].str)); }
#line 2350 "pascal.tab.c"
    break;

  case 66: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@3 block SEMICOLON  */
#line 817 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-8].str), pas_fwd_params((yyvsp[-8].str), (yyvsp[-6].list)), (yyvsp[-1].node), 1, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); emit_proc(&g_pascal_procs, p); }
#line 2358 "pascal.tab.c"
    break;

  case 67: /* $@4: %empty  */
#line 820 "pascal.y"
                                         { pas_func_add((yyvsp[-2].str)); pas_proc_enter(); pas_fwd_restore((yyvsp[-2].str)); }
#line 2364 "pascal.tab.c"
    break;

  case 68: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@4 block SEMICOLON  */
#line 821 "pascal.y"
        { int d = g_pas_ldepth - 1; int d_ok = (d >= 0 && d < PAS_NEST_MAX); int dl = d_ok ? g_pas_lstk[d].decl_level : 1;
          const char **ln = d_ok ? g_pas_lstk[d].names : NULL; int lc = d_ok ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc((yyvsp[-5].str), pas_fwd_params((yyvsp[-5].str), pnl_new()), (yyvsp[-1].node), 1, dl, ln, lc); pas_proc_exit(); pas_ptrvar_release(); emit_proc(&g_pascal_procs, p); }
#line 2372 "pascal.tab.c"
    break;

  case 69: /* pv_mark: %empty  */
#line 826 "pascal.y"
    { pas_ptrvar_mark(); }
#line 2378 "pascal.tab.c"
    break;

  case 70: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 829 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 2384 "pascal.tab.c"
    break;

  case 71: /* parameter_list_opt: %empty  */
#line 830 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 2390 "pascal.tab.c"
    break;

  case 72: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 833 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 2396 "pascal.tab.c"
    break;

  case 73: /* parameter_decl_list: parameter_decl  */
#line 834 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 2402 "pascal.tab.c"
    break;

  case 74: /* parameter_decl: PROCEDURESY id_list  */
#line 837 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 2408 "pascal.tab.c"
    break;

  case 75: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 838 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 2414 "pascal.tab.c"
    break;

  case 76: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 839 "pascal.y"
                                { if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); } } } for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 2420 "pascal.tab.c"
    break;

  case 77: /* parameter_decl: id_list COLON IDENT  */
#line 840 "pascal.y"
                          { if (pas_is_booltype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_boolvar_add((yyvsp[-2].list)->items[i]->v.sval); if (pas_is_settype((yyvsp[0].str))) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_setvar_add((yyvsp[-2].list)->items[i]->v.sval); const char *_pt = pas_ptrtype_target((yyvsp[0].str)); if (_pt) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_ptrvar_add((yyvsp[-2].list)->items[i]->v.sval, _pt); if (!strcmp((yyvsp[0].str), "char")) for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) pas_charvar_add((yyvsp[-2].list)->items[i]->v.sval); { long long _ah = pas_arrtype_high((yyvsp[0].str)); long long _nc = pas_arrtype_ncols((yyvsp[0].str)); int _nf = pas_rectype_nf((yyvsp[0].str)); int _aic = pas_arrtype_ischar((yyvsp[0].str)); for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i] && (yyvsp[-2].list)->items[i]->v.sval) { if (_nf > 0) { pas_recvar_add_from_type((yyvsp[-2].list)->items[i]->v.sval, (yyvsp[0].str)); pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, (long long)(_nf - 1), -1); } else if (_ah >= 0) { if (_nc >= 0) pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, _nc); else pas_array_add2d_param((yyvsp[-2].list)->items[i]->v.sval, _ah, -1); if (_aic) pas_chararr_add2((yyvsp[-2].list)->items[i]->v.sval, pas_arrtype_lo((yyvsp[0].str))); } } } (yyval.list) = (yyvsp[-2].list); }
#line 2426 "pascal.tab.c"
    break;

  case 78: /* id_list: id_list COMMA IDENT  */
#line 843 "pascal.y"
                        { pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 2432 "pascal.tab.c"
    break;

  case 79: /* id_list: IDENT  */
#line 844 "pascal.y"
            { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 2438 "pascal.tab.c"
    break;

  case 80: /* body: BEGINSY statement_list ENDSY  */
#line 847 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 2444 "pascal.tab.c"
    break;

  case 81: /* statement_list: statement_list SEMICOLON statement  */
#line 850 "pascal.y"
                                       { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2450 "pascal.tab.c"
    break;

  case 82: /* statement_list: statement  */
#line 851 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2456 "pascal.tab.c"
    break;

  case 83: /* statement: statement_no_label  */
#line 854 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2462 "pascal.tab.c"
    break;

  case 84: /* statement: INTCONST COLON statement_no_label  */
#line 856 "pascal.y"
        { char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)(yyvsp[-2].ival));
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = strdup(_lb); ast_push(L, (yyvsp[0].node)); (yyval.node) = L; }
#line 2469 "pascal.tab.c"
    break;

  case 85: /* statement_no_label: assignment  */
#line 860 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2475 "pascal.tab.c"
    break;

  case 86: /* statement_no_label: call  */
#line 861 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2481 "pascal.tab.c"
    break;

  case 87: /* statement_no_label: compound_statement  */
#line 862 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2487 "pascal.tab.c"
    break;

  case 88: /* statement_no_label: goto_statement  */
#line 863 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2493 "pascal.tab.c"
    break;

  case 89: /* statement_no_label: if_statement  */
#line 864 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2499 "pascal.tab.c"
    break;

  case 90: /* statement_no_label: case_statement  */
#line 865 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2505 "pascal.tab.c"
    break;

  case 91: /* statement_no_label: while_statement  */
#line 866 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2511 "pascal.tab.c"
    break;

  case 92: /* statement_no_label: repeat_statement  */
#line 867 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2517 "pascal.tab.c"
    break;

  case 93: /* statement_no_label: for_statement  */
#line 868 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2523 "pascal.tab.c"
    break;

  case 94: /* statement_no_label: with_statement  */
#line 869 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2529 "pascal.tab.c"
    break;

  case 95: /* statement_no_label: %empty  */
#line 870 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 2535 "pascal.tab.c"
    break;

  case 96: /* call: IDENT  */
#line 873 "pascal.y"
          { if (pas_is_proc((yyvsp[0].str))) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.node) = e; } else (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 2541 "pascal.tab.c"
    break;

  case 97: /* call: call_with_args  */
#line 874 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2547 "pascal.tab.c"
    break;

  case 98: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 877 "pascal.y"
                                        { (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 2553 "pascal.tab.c"
    break;

  case 99: /* argument_list: argument_list COMMA argument  */
#line 880 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 2559 "pascal.tab.c"
    break;

  case 100: /* argument_list: argument  */
#line 881 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 2565 "pascal.tab.c"
    break;

  case 101: /* argument: expression  */
#line 884 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[0].node))); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 2571 "pascal.tab.c"
    break;

  case 102: /* argument: expression COLON expression  */
#line 885 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-2].node))); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 2577 "pascal.tab.c"
    break;

  case 103: /* argument: expression COLON expression COLON expression  */
#line 886 "pascal.y"
                                                   { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool((yyvsp[-4].node))); pnl_push(_al, ilit(-3)); pnl_push(_al, (yyvsp[-2].node)); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 2583 "pascal.tab.c"
    break;

  case 104: /* assignment: selector BECOMES expression  */
#line 890 "pascal.y"
        { if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_FNC && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->v.sval && (!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_fbuf_get") || !strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get"))) {
              (yyval.node) = mk_set_bin(!strcmp((yyvsp[-2].node)->c[0]->v.sval, "__pas_tbuf_get") ? "__pas_tbuf_set" : "__pas_fbuf_set", (yyvsp[-2].node)->c[1], pas_bool((yyvsp[0].node)));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval && pas_is_chararr((yyvsp[-2].node)->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _cah; if (!pas_array_high_get((yyvsp[-2].node)->v.sval, &_cah)) _cah = (long long)strlen((yyvsp[0].node)->v.sval);
              (yyval.node) = mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, pas_chararr_lo((yyvsp[-2].node)->v.sval), _cah));
          } else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR && (yyvsp[-2].node)->c[0]->v.sval && pas_is_strarr((yyvsp[-2].node)->c[0]->v.sval) && (yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) {
              long long _slo = pas_strarr_lo((yyvsp[-2].node)->c[0]->v.sval); long long _shi = _slo + (long long)strlen((yyvsp[0].node)->v.sval) - 1;
              (yyval.node) = mk_assign((yyvsp[-2].node), pas_str_to_alpha((yyvsp[0].node)->v.sval, _slo, _shi));
          } else if (pas_is_cafield((yyvsp[-2].node))) {
              tree_t *_rhs;
              if ((yyvsp[0].node) && (yyvsp[0].node)->t == TT_QLIT && (yyvsp[0].node)->v.sval) { long long _flo = pas_cafield_lo_get((yyvsp[-2].node)); long long _fhi = pas_cafield_hi_get((yyvsp[-2].node)); if (_fhi < _flo) _fhi = _flo + (long long)strlen((yyvsp[0].node)->v.sval) - 1; _rhs = pas_str_to_alpha((yyvsp[0].node)->v.sval, _flo, _fhi); }
              else _rhs = pas_bool((yyvsp[0].node));
              tree_t *_pk = ast_node_new(TT_FNC); ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack")); ast_push(_pk, _rhs);
              (yyval.node) = mk_assign((yyvsp[-2].node), _pk);
          } else { (yyval.node) = mk_assign((yyvsp[-2].node), pas_bool((yyvsp[0].node))); } }
#line 2603 "pascal.tab.c"
    break;

  case 105: /* selector: selector LBRACK expression_list RBRACK  */
#line 907 "pascal.y"
                                           { tree_t *e = NULL; if ((yyvsp[-1].list) && (yyvsp[-1].list)->count == 2 && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { long long _nc = pas_array_ncols((yyvsp[-3].node)->v.sval); if (_nc > 0) { tree_t *flat = bin(TT_ADD, bin(TT_MUL, (yyvsp[-1].list)->items[0], ilit(_nc)), (yyvsp[-1].list)->items[1]); e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); ast_push(e, flat); } } if (!e) { e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); } if (e && (yyvsp[-3].node) && (yyvsp[-3].node)->t == TT_VAR && (yyvsp[-3].node)->v.sval) { const char *_et = pas_enumarr_get((yyvsp[-3].node)->v.sval); if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } (yyval.node) = e; }
#line 2609 "pascal.tab.c"
    break;

  case 106: /* selector: selector PERIOD IDENT  */
#line 908 "pascal.y"
                            { int _fi = -1; const char *_rt = pas_selector_rectype((yyvsp[-2].node)); if (_rt) _fi = pas_rectype_field_index(_rt, (yyvsp[0].str)); else if ((yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_VAR && (yyvsp[-2].node)->v.sval) _fi = pas_recvar_field_index((yyvsp[-2].node)->v.sval, (yyvsp[0].str));
        if (_fi < 0 && (yyvsp[-2].node) && (yyvsp[-2].node)->t == TT_IDX && (yyvsp[-2].node)->n == 2 && (yyvsp[-2].node)->c[0] && (yyvsp[-2].node)->c[0]->t == TT_VAR) { const char *_arn = NULL; int _anf = pas_arrrec_find((yyvsp[-2].node)->c[0]->v.sval, &_arn); if (_anf > 0) { int _afi = _arn ? pas_rectype_field_index(_arn, (yyvsp[0].str)) : -1; if (_afi < 0) { _afi = pas_arrrec_field_index((yyvsp[-2].node)->c[0]->v.sval, (yyvsp[0].str)); } if (_afi < 0) { for (int _ri = 0; _ri < g_pas_nrectype; _ri++) { int _t = pas_rectype_field_index(g_pas_rectypes[_ri].tname, (yyvsp[0].str)); if (_t >= 0 && g_pas_rectypes[_ri].nf == _anf) { _afi = _t; break; } } } if (_afi >= 0) { (yyval.node) = pas_arrrec_flatten((yyvsp[-2].node), _afi); if (pas_arrrec_field_is_char((yyvsp[-2].node)->c[0]->v.sval, _afi) || (_arn && pas_rectype_field_is_char(_arn, _afi))) pas_cvfield_mark_add((yyval.node)); const char *_fe = pas_arrrec_field_enum((yyvsp[-2].node)->c[0]->v.sval, _afi); if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi); if (_fe && (yyval.node)) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) (yyval.node)->v.ival = (long long)(_ei + 1); } } else { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); } } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
        else if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-2].node)); ast_push(e, ilit(_fi)); if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } } { const char *_mrt = _rt ? _rt : pas_with_sel_rtype((yyvsp[-2].node)); if (_mrt && pas_rectype_field_is_ca(_mrt, _fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_mrt, _fi), pas_rectype_field_ca_hi(_mrt, _fi)); if (_mrt && pas_rectype_field_is_char(_mrt, _fi)) pas_cvfield_mark_add(e); } (yyval.node) = e; } else { (yyval.node) = pas_nested_field_resolve((yyvsp[-2].node), (yyvsp[0].str)); } }
#line 2617 "pascal.tab.c"
    break;

  case 107: /* selector: selector ARROW  */
#line 911 "pascal.y"
                     { (yyval.node) = pas_is_tfile_node((yyvsp[-1].node)) ? mk_fnc1("__pas_fbuf_get", (yyvsp[-1].node)) : (((yyvsp[-1].node) && (yyvsp[-1].node)->t == TT_VAR && (yyvsp[-1].node)->v.sval && pas_is_filevar((yyvsp[-1].node)->v.sval) && !pas_is_stdstream((yyvsp[-1].node)->v.sval)) ? mk_fnc1("__pas_tbuf_get", (yyvsp[-1].node)) : mk_deref((yyvsp[-1].node))); }
#line 2623 "pascal.tab.c"
    break;

  case 108: /* selector: IDENT  */
#line 912 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 2629 "pascal.tab.c"
    break;

  case 109: /* expression_list: expression_list COMMA expression  */
#line 915 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2635 "pascal.tab.c"
    break;

  case 110: /* expression_list: expression  */
#line 916 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2641 "pascal.tab.c"
    break;

  case 111: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 919 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 2647 "pascal.tab.c"
    break;

  case 112: /* goto_statement: GOTOSY INTCONST  */
#line 923 "pascal.y"
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)(yyvsp[0].ival));
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = strdup(_gb); (yyval.node) = G; }
#line 2654 "pascal.tab.c"
    break;

  case 113: /* if_statement: IFSY expression THENSY statement  */
#line 927 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2660 "pascal.tab.c"
    break;

  case 114: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 928 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond((yyvsp[-4].node))); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2666 "pascal.tab.c"
    break;

  case 115: /* $@5: %empty  */
#line 931 "pascal.y"
                           { pas_case_push(); }
#line 2672 "pascal.tab.c"
    break;

  case 116: /* case_statement: CASESY expression OFSY $@5 case_list ENDSY  */
#line 932 "pascal.y"
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), (yyvsp[-4].node)));
          tree_t *chain = NULL;
          if ((yyvsp[-1].list)) for (int i = (yyvsp[-1].list)->count - 1; i >= 0; i--) { tree_t *e = (yyvsp[-1].list)->items[i]; if (!e) continue; if (chain) ast_push(e, chain); chain = e; }
          ast_push(seq, chain ? chain : ast_node_new(TT_SUCCEED));
          pas_case_pop();
          (yyval.node) = seq; }
#line 2684 "pascal.tab.c"
    break;

  case 117: /* case_list: case_list SEMICOLON case_elem  */
#line 941 "pascal.y"
                                  { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 2690 "pascal.tab.c"
    break;

  case 118: /* case_list: case_elem  */
#line 942 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 2696 "pascal.tab.c"
    break;

  case 119: /* case_elem: constant_list COLON statement  */
#line 945 "pascal.y"
                                  { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 120: /* case_elem: %empty  */
#line 946 "pascal.y"
      { (yyval.node) = NULL; }
#line 2708 "pascal.tab.c"
    break;

  case 121: /* constant_list: constant_list COMMA constant  */
#line 949 "pascal.y"
                                 { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival)))); }
#line 2714 "pascal.tab.c"
    break;

  case 122: /* constant_list: constant  */
#line 950 "pascal.y"
               { (yyval.node) = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit((yyvsp[0].ival))); }
#line 2720 "pascal.tab.c"
    break;

  case 123: /* while_statement: WHILESY expression DOSY statement  */
#line 953 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2726 "pascal.tab.c"
    break;

  case 124: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 956 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond((yyvsp[0].node))); }
#line 2732 "pascal.tab.c"
    break;

  case 125: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 960 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 2738 "pascal.tab.c"
    break;

  case 126: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 962 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); e->v.ival = 1; (yyval.node) = e; }
#line 2744 "pascal.tab.c"
    break;

  case 127: /* with_statement: WITHSY with_open DOSY statement  */
#line 965 "pascal.y"
                                    { long long n = (yyvsp[-2].ival); for (long long i = 0; i < n; i++) pas_with_pop(); (yyval.node) = (yyvsp[0].node); }
#line 2750 "pascal.tab.c"
    break;

  case 128: /* with_open: with_open COMMA selector  */
#line 968 "pascal.y"
                             { pas_with_push((yyvsp[0].node)); (yyval.ival) = (yyvsp[-2].ival) + 1; }
#line 2756 "pascal.tab.c"
    break;

  case 129: /* with_open: selector  */
#line 969 "pascal.y"
               { pas_with_push((yyvsp[0].node)); (yyval.ival) = 1; }
#line 2762 "pascal.tab.c"
    break;

  case 130: /* expression: simple_expression  */
#line 976 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2768 "pascal.tab.c"
    break;

  case 131: /* expression: expression INOP simple_expression  */
#line 977 "pascal.y"
                                        { (yyval.node) = mk_in((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 132: /* expression: expression LTOP simple_expression  */
#line 978 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2780 "pascal.tab.c"
    break;

  case 133: /* expression: expression LEOP simple_expression  */
#line 979 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_LE, "__pas_subset", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2786 "pascal.tab.c"
    break;

  case 134: /* expression: expression GTOP simple_expression  */
#line 980 "pascal.y"
                                        { (yyval.node) = pas_rel(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2792 "pascal.tab.c"
    break;

  case 135: /* expression: expression GEOP simple_expression  */
#line 981 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_GE, "__pas_super", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 136: /* expression: expression NEOP simple_expression  */
#line 982 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_NE, "__pas_setne", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2804 "pascal.tab.c"
    break;

  case 137: /* expression: expression EQOP simple_expression  */
#line 983 "pascal.y"
                                        { (yyval.node) = pas_rel_or_set(TT_EQ, "__pas_seteq", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2810 "pascal.tab.c"
    break;

  case 138: /* simple_expression: term  */
#line 986 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2816 "pascal.tab.c"
    break;

  case 139: /* simple_expression: PLUS term  */
#line 987 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2822 "pascal.tab.c"
    break;

  case 140: /* simple_expression: MINUS term  */
#line 988 "pascal.y"
                 { (yyval.node) = mk_neg((yyvsp[0].node)); }
#line 2828 "pascal.tab.c"
    break;

  case 141: /* simple_expression: simple_expression PLUS term  */
#line 989 "pascal.y"
                                  { (yyval.node) = pas_arith_or_set(TT_ADD, "__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2834 "pascal.tab.c"
    break;

  case 142: /* simple_expression: simple_expression MINUS term  */
#line 990 "pascal.y"
                                   { (yyval.node) = pas_arith_or_set(TT_SUB, "__pas_setdif", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2840 "pascal.tab.c"
    break;

  case 143: /* simple_expression: simple_expression OROP term  */
#line 991 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2846 "pascal.tab.c"
    break;

  case 144: /* term: factor  */
#line 994 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2852 "pascal.tab.c"
    break;

  case 145: /* term: term MUL factor  */
#line 995 "pascal.y"
                      { (yyval.node) = pas_arith_or_set(TT_MUL, "__pas_setint", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2858 "pascal.tab.c"
    break;

  case 146: /* term: term RDIV factor  */
#line 996 "pascal.y"
                       { (yyval.node) = pas_rdiv((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2864 "pascal.tab.c"
    break;

  case 147: /* term: term IDIV factor  */
#line 997 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2870 "pascal.tab.c"
    break;

  case 148: /* term: term IMOD factor  */
#line 998 "pascal.y"
                       { (yyval.node) = pas_mod((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2876 "pascal.tab.c"
    break;

  case 149: /* term: term ANDOP factor  */
#line 999 "pascal.y"
                        { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2882 "pascal.tab.c"
    break;

  case 150: /* factor: selector  */
#line 1002 "pascal.y"
             { if (pas_is_cafield((yyvsp[0].node))) { tree_t *u = ast_node_new(TT_FNC); ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack")); ast_push(u, (yyvsp[0].node)); ast_push(u, ilit(pas_cafield_lo_get((yyvsp[0].node)))); (yyval.node) = u; } else (yyval.node) = (yyvsp[0].node); }
#line 2888 "pascal.tab.c"
    break;

  case 151: /* factor: call_with_args  */
#line 1003 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2894 "pascal.tab.c"
    break;

  case 152: /* factor: INTCONST  */
#line 1004 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 2900 "pascal.tab.c"
    break;

  case 153: /* factor: REALCONST  */
#line 1005 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 2906 "pascal.tab.c"
    break;

  case 154: /* factor: STRINGCONST  */
#line 1006 "pascal.y"
                  { if ((yyvsp[0].str) && strlen((yyvsp[0].str)) == 1) { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit((long long)(unsigned char)(yyvsp[0].str)[0])); (yyval.node) = _cl; } else (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 2912 "pascal.tab.c"
    break;

  case 155: /* factor: LPARENT expression RPARENT  */
#line 1007 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2918 "pascal.tab.c"
    break;

  case 156: /* factor: NOTSY factor  */
#line 1008 "pascal.y"
                   { (yyval.node) = pas_flip_rel(pas_cond((yyvsp[0].node))); }
#line 2924 "pascal.tab.c"
    break;

  case 157: /* factor: LBRACK RBRACK  */
#line 1009 "pascal.y"
                    { (yyval.node) = mk_set_ctor(NULL); }
#line 2930 "pascal.tab.c"
    break;

  case 158: /* factor: LBRACK set_member_list RBRACK  */
#line 1010 "pascal.y"
                                    { (yyval.node) = (yyvsp[-1].node); }
#line 2936 "pascal.tab.c"
    break;

  case 159: /* set_member_list: set_member  */
#line 1013 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2942 "pascal.tab.c"
    break;

  case 160: /* set_member_list: set_member_list COMMA set_member  */
#line 1014 "pascal.y"
                                       { (yyval.node) = mk_set_bin("__pas_setuni", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2948 "pascal.tab.c"
    break;

  case 161: /* set_member: expression  */
#line 1017 "pascal.y"
               { PNodeList *_l = pnl_new(); pnl_push(_l, (yyvsp[0].node)); (yyval.node) = mk_set_ctor(_l); }
#line 2954 "pascal.tab.c"
    break;

  case 162: /* set_member: expression DOTDOT expression  */
#line 1018 "pascal.y"
                                   { (yyval.node) = mk_set_bin("__pas_setrange", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2960 "pascal.tab.c"
    break;


#line 2964 "pascal.tab.c"

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

#line 1024 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0; g_pas_ncaparm = 0; g_pas_pend_arr_ncols = -1;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0; g_pas_nsettype = 0; g_pas_ncharvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; g_pas_narrtype = 0; g_pas_nboolvar = 0; g_pas_nbooltype = 0; g_pas_pend_isbool = 0; g_pas_ntfilevar = 0; g_pas_ntfiletype = 0; g_pas_pend_istfile = 0;
    g_pas_nenum = 0; g_pas_pend_enum_max = -1; g_pas_nsubtype = 0; g_pas_pend_sub_low = 0; g_pas_pend_sub_high = -1;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0; g_with_depth = 0;
    g_pas_nchararr = 0; g_pas_pend_arr_ischar = 0; g_pas_nrconst = 0; g_pas_nsconst = 0; g_pas_narrrec = 0; g_pas_ncafield = 0; g_pas_ncvfield = 0; g_pas_narrptr = 0; g_pas_pend_arr_ptrto = NULL;
    g_pas_nenumname = 0; g_pas_nenumarr = 0; g_pas_pend_enum_names[0] = '\0'; g_pas_nrec_nmarks = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
