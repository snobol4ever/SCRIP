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
#define YYSTYPE         RAKU_YYSTYPE
/* Substitute the variable and function names.  */
#define yyparse         raku_yyparse
#define yylex           raku_yylex
#define yyerror         raku_yyerror
#define yydebug         raku_yydebug
#define yynerrs         raku_yynerrs
#define yylval          raku_yylval
#define yychar          raku_yychar

/* First part of user prologue.  */
#line 13 "raku.y"

#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "raku.tab.h"
#include "raku_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  raku_yylex(void);
extern int  raku_get_lineno(void);
void raku_yyerror(const char *msg) {
    fprintf(stderr, "raku parse error line %d: %s\n", raku_get_lineno(), msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *testop_rt(const char *s) {
    if (!s) return "__rk_test_ok";
    if (!strcmp(s, "plan")) return "__rk_test_plan";
    if (!strcmp(s, "ok")) return "__rk_test_ok";
    if (!strcmp(s, "nok")) return "__rk_test_nok";
    if (!strcmp(s, "is")) return "__rk_test_is";
    if (!strcmp(s, "isnt")) return "__rk_test_isnt";
    if (!strcmp(s, "done-testing")) return "__rk_test_done";
    if (!strcmp(s, "skip-rest")) return "__rk_test_skip_rest";
    if (!strcmp(s, "skip")) return "__rk_test_skip";
    if (!strcmp(s, "todo")) return "__rk_test_todo";
    if (!strcmp(s, "diag")) return "__rk_test_diag";
    if (!strcmp(s, "pass")) return "__rk_test_pass";
    if (!strcmp(s, "flunk")) return "__rk_test_flunk";
    return "__rk_test_ok";
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_call(const char *name) {
    tree_t *e = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(e, n);
    return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_junction(const char *flav, tree_t *l, tree_t *r) {
    tree_t *e = make_call(flav);
    if (l && l->t == TT_FNC && l->v.sval && strcmp(l->v.sval, flav) == 0) {
        for (int i = 1; i < l->n; i++) expr_add_child(e, l->c[i]);
    } else {
        expr_add_child(e, l);
    }
    expr_add_child(e, r);
    return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rk_multi_mangle(const char *base, ExprList *params) {
    static char buf[512]; int np = params ? params->count : 0;
    int pos = snprintf(buf, sizeof buf, "%s$%d", base, np);
    for (int i = 0; i < np; i++) { tree_t *p = params->items[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        char safe[64]; int j = 0;
        for (const char *c = ty; *c && j < 63; c++, j++) safe[j] = (*c == ':') ? '_' : *c; safe[j] = 0;
        pos += snprintf(buf + pos, sizeof buf - pos, "$%s", safe); }
    return intern(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_param(const char *type, const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_def_param(const char *type, const char *def, const char *name) {
    char buf[160]; snprintf(buf, sizeof buf, "%s%s", type, def);
    return rk_typed_param(intern(buf), name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_seq(ExprList *stmts) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *seq1(tree_t *stmt) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmt) expr_add_child(seq, stmt);
    return seq;
}
static tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body) {
    tree_t *n = ast_node_new(TT_CLOOP);
    expr_add_child(n, init); expr_add_child(n, cond); expr_add_child(n, incr); expr_add_child(n, body);
    return n;
}
static tree_t *rk_incdec(const char *var, int add) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_ASSIGN, var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one));
}
static tree_t *rk_destructure(ExprList *targets, tree_t *rhs_arr) {
    static int __destr_uid = 0;
    char tmp[32]; snprintf(tmp, sizeof tmp, "__destr_%d", __destr_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    tree_t *bind = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rhs_arr); expr_add_child(seq, bind);
    int n = targets ? targets->count : 0;
    for (int i = 0; i < n; i++) {
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, tmp));
        tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = i; expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, targets->items[i], get));
    }
    if (targets) exprlist_free(targets);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_range_ex(tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return expr_binary(TT_TO, lo, d); }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_is_chain_cmp(tree_e k) {
    return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return expr_binary(TT_SEQ, left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *lower_interp_str(const char *s) {
    int len = s ? (int)strlen(s) : 0;
    tree_t *result = NULL;
    char litbuf[4096]; int litpos = 0, i = 0;
    while (i < len) {
        if (s[i]=='$' && i+1<len &&
            (s[i+1]=='_'||(s[i+1]>='A'&&s[i+1]<='Z')||(s[i+1]>='a'&&s[i+1]<='z'))) {
            if (litpos>0) { litbuf[litpos]='\0';
                tree_t *lit=leaf_sval(TT_QLIT,litbuf);
                result=result?expr_binary(TT_CAT,result,lit):lit; litpos=0; }
            i++;
            char vname[256]; int vlen=0;
            while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
                { if(vlen<255) vname[vlen++]=s[i]; i++; }
            vname[vlen]='\0';
            tree_t *var=leaf_sval(TT_VAR,vname);
            result=result?expr_binary(TT_CAT,result,var):var;
        } else { if(litpos<4095) litbuf[litpos++]=s[i]; i++; }
    }
    if (litpos>0) { litbuf[litpos]='\0';
        tree_t *lit=leaf_sval(TT_QLIT,litbuf);
        result=result?expr_binary(TT_CAT,result,lit):lit; }
    return result ? result : leaf_sval(TT_QLIT,"");
}
/*--------------------------------------------------------------------------------------------------------------------*/
tree_t *raku_prog_result = NULL;
static void add_proc(tree_t *e) {
    if (!e) return;
    if (!raku_prog_result) raku_prog_result = ast_stmt_new(TT_PROGRAM);
    tree_t *st = ast_stmt_new(TT_STMT);
    expr_add_child(st, ast_attr_int(":line", 0));
    expr_add_child(st, ast_attr_int(":stno", 0));
    expr_add_child(st, ast_attr_expr(":subj", e));
    expr_add_child(raku_prog_result, st);
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RAKU_METH_MAX 256
typedef struct { char key[128]; char procname[128]; } RakuMethEntry;
static RakuMethEntry raku_meth_table[RAKU_METH_MAX];
static int           raku_meth_ntypes = 0;
static void raku_meth_register(const char *classname, const char *methname, const char *procname) {
    if (raku_meth_ntypes >= RAKU_METH_MAX) return;
    RakuMethEntry *e = &raku_meth_table[raku_meth_ntypes++];
    snprintf(e->key,      sizeof e->key,      "%s::%s", classname, methname);
    snprintf(e->procname, sizeof e->procname,  "%s",     procname);
}
/*--------------------------------------------------------------------------------------------------------------------*/
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/

#line 328 "raku.tab.c"

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

#include "raku.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LIT_INT = 3,                    /* LIT_INT  */
  YYSYMBOL_LIT_FLOAT = 4,                  /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 5,                    /* LIT_STR  */
  YYSYMBOL_LIT_INTERP_STR = 6,             /* LIT_INTERP_STR  */
  YYSYMBOL_LIT_REGEX = 7,                  /* LIT_REGEX  */
  YYSYMBOL_LIT_MATCH_GLOBAL = 8,           /* LIT_MATCH_GLOBAL  */
  YYSYMBOL_LIT_SUBST = 9,                  /* LIT_SUBST  */
  YYSYMBOL_VAR_SCALAR = 10,                /* VAR_SCALAR  */
  YYSYMBOL_VAR_ARRAY = 11,                 /* VAR_ARRAY  */
  YYSYMBOL_VAR_HASH = 12,                  /* VAR_HASH  */
  YYSYMBOL_VAR_TWIGIL = 13,                /* VAR_TWIGIL  */
  YYSYMBOL_IDENT = 14,                     /* IDENT  */
  YYSYMBOL_VAR_ARRAY_TWIGIL = 15,          /* VAR_ARRAY_TWIGIL  */
  YYSYMBOL_VAR_HASH_TWIGIL = 16,           /* VAR_HASH_TWIGIL  */
  YYSYMBOL_CARET = 17,                     /* CARET  */
  YYSYMBOL_VAR_CAPTURE = 18,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 19,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 20,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 21,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 22,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 23,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 24,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 25,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 26,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 27,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 28,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 29,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 30,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 31,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 32,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 33,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 34,                 /* KW_RETURN  */
  YYSYMBOL_KW_CONSTANT = 35,               /* KW_CONSTANT  */
  YYSYMBOL_KW_GIVEN = 36,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 37,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 38,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 39,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 40,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 41,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 42,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 43,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 44,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 45,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 46,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 47,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 48,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 49,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 50,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 51,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 52,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 53,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 54,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 55,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 56,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 57,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 58,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 59,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 60,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 61,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 62,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 63,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 64,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 65,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 66,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 67,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 68,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 69,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 70,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 71,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 72,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 73,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 74,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 75,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 76,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 77,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 78,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 79,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 80,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 81,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 82,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 83,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 84,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 85,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 86,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 87,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 88,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 89,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 90,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 91,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 92,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 93,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 94,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 95,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 96,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 97,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 98,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 99,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 100,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 101,                   /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 102,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 103,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 104,                   /* OP_POW  */
  YYSYMBOL_105_ = 105,                     /* '='  */
  YYSYMBOL_106_ = 106,                     /* '!'  */
  YYSYMBOL_107_ = 107,                     /* '<'  */
  YYSYMBOL_108_ = 108,                     /* '>'  */
  YYSYMBOL_109_ = 109,                     /* '|'  */
  YYSYMBOL_110_ = 110,                     /* '&'  */
  YYSYMBOL_111_ = 111,                     /* '~'  */
  YYSYMBOL_112_ = 112,                     /* '+'  */
  YYSYMBOL_113_ = 113,                     /* '-'  */
  YYSYMBOL_114_ = 114,                     /* '*'  */
  YYSYMBOL_115_ = 115,                     /* '/'  */
  YYSYMBOL_116_ = 116,                     /* '%'  */
  YYSYMBOL_UMINUS = 117,                   /* UMINUS  */
  YYSYMBOL_118_ = 118,                     /* '.'  */
  YYSYMBOL_119_ = 119,                     /* ';'  */
  YYSYMBOL_120_ = 120,                     /* '('  */
  YYSYMBOL_121_ = 121,                     /* ')'  */
  YYSYMBOL_122_ = 122,                     /* ','  */
  YYSYMBOL_123_ = 123,                     /* '['  */
  YYSYMBOL_124_ = 124,                     /* ']'  */
  YYSYMBOL_125_ = 125,                     /* '{'  */
  YYSYMBOL_126_ = 126,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 127,                 /* $accept  */
  YYSYMBOL_program = 128,                  /* program  */
  YYSYMBOL_stmt_list = 129,                /* stmt_list  */
  YYSYMBOL_stmt = 130,                     /* stmt  */
  YYSYMBOL_if_stmt = 131,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 132,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 133,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 134,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 135,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 136,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 137,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 138,                /* loop_incr  */
  YYSYMBOL_for_stmt = 139,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 140,               /* given_stmt  */
  YYSYMBOL_when_list = 141,                /* when_list  */
  YYSYMBOL_sub_decl = 142,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 143,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 144,                 /* sub_body  */
  YYSYMBOL_method_body = 145,              /* method_body  */
  YYSYMBOL_class_decl = 146,               /* class_decl  */
  YYSYMBOL_role_decl = 147,                /* role_decl  */
  YYSYMBOL_is_clauses = 148,               /* is_clauses  */
  YYSYMBOL_class_body_list = 149,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 150,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 151,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 152,           /* named_arg_list  */
  YYSYMBOL_pair_list = 153,                /* pair_list  */
  YYSYMBOL_param_list = 154,               /* param_list  */
  YYSYMBOL_block = 155,                    /* block  */
  YYSYMBOL_closure = 156,                  /* closure  */
  YYSYMBOL_expr = 157,                     /* expr  */
  YYSYMBOL_tern_expr = 158,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 159,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 160,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 161,                 /* dor_expr  */
  YYSYMBOL_range_expr = 162,               /* range_expr  */
  YYSYMBOL_add_expr = 163,                 /* add_expr  */
  YYSYMBOL_repl_expr = 164,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 165,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 166,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 167,               /* unary_expr  */
  YYSYMBOL_pow_expr = 168,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 169,              /* scalar_list  */
  YYSYMBOL_meth_name = 170,                /* meth_name  */
  YYSYMBOL_postfix_expr = 171,             /* postfix_expr  */
  YYSYMBOL_call_expr = 172,                /* call_expr  */
  YYSYMBOL_arg_list = 173,                 /* arg_list  */
  YYSYMBOL_atom = 174                      /* atom  */
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
         || (defined RAKU_YYSTYPE_IS_TRIVIAL && RAKU_YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3758

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  412
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1051

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   360


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
       2,     2,     2,   106,     2,     2,     2,   116,   110,     2,
     120,   121,   114,   112,   122,   113,   118,   115,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   119,
     107,   105,   108,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   123,     2,   124,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   125,   109,   126,   111,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     117
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   338,   338,   349,   350,   353,   355,   357,   359,   363,
     365,   367,   369,   373,   376,   380,   382,   384,   386,   388,
     390,   394,   398,   400,   402,   404,   406,   408,   410,   412,
     414,   416,   418,   420,   422,   424,   426,   429,   432,   434,
     437,   440,   442,   444,   446,   448,   453,   457,   461,   466,
     470,   471,   473,   475,   477,   479,   482,   484,   486,   488,
     491,   494,   497,   500,   503,   505,   507,   509,   511,   514,
     516,   518,   520,   523,   526,   530,   533,   535,   537,   539,
     542,   545,   548,   551,   554,   557,   560,   563,   566,   569,
     572,   573,   574,   575,   576,   577,   578,   580,   582,   584,
     586,   587,   588,   589,   590,   591,   592,   594,   596,   598,
     600,   601,   602,   603,   606,   608,   610,   612,   614,   616,
     618,   620,   624,   626,   628,   630,   632,   634,   638,   640,
     644,   646,   648,   650,   654,   656,   660,   664,   666,   668,
     672,   673,   674,   675,   676,   679,   685,   691,   696,   702,
     707,   712,   720,   731,   732,   738,   746,   752,   758,   766,
     772,   778,   787,   794,   803,   812,   821,   828,   835,   836,
     839,   842,   844,   846,   848,   850,   852,   854,   856,   859,
     861,   863,   865,   868,   871,   877,   878,   879,   882,   885,
     887,   889,   891,   893,   895,   897,   899,   902,   904,   906,
     908,   911,   914,   920,   936,   951,   952,   964,   978,   979,
     982,   985,   988,   991,   994,   997,  1000,  1003,  1006,  1009,
    1012,  1015,  1022,  1029,  1036,  1043,  1047,  1051,  1055,  1059,
    1066,  1073,  1080,  1087,  1097,  1105,  1113,  1122,  1129,  1136,
    1146,  1154,  1164,  1179,  1180,  1185,  1190,  1197,  1201,  1207,
    1209,  1211,  1213,  1217,  1218,  1219,  1220,  1221,  1222,  1223,
    1224,  1227,  1228,  1229,  1231,  1233,  1235,  1237,  1239,  1242,
    1244,  1246,  1248,  1250,  1253,  1256,  1260,  1263,  1266,  1269,
    1271,  1275,  1278,  1281,  1284,  1287,  1290,  1292,  1294,  1296,
    1298,  1300,  1302,  1304,  1306,  1310,  1313,  1314,  1319,  1322,
    1324,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,
    1336,  1337,  1338,  1339,  1340,  1341,  1347,  1353,  1359,  1362,
    1363,  1364,  1367,  1369,  1372,  1373,  1374,  1377,  1378,  1381,
    1382,  1385,  1386,  1387,  1390,  1391,  1392,  1393,  1394,  1397,
    1398,  1399,  1400,  1403,  1404,  1407,  1408,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1423,  1425,
    1430,  1431,  1436,  1438,  1444,  1450,  1454,  1458,  1463,  1470,
    1475,  1481,  1487,  1494,  1501,  1506,  1511,  1518,  1523,  1528,
    1530,  1532,  1534,  1536,  1538,  1541,  1542,  1545,  1546,  1547,
    1548,  1556,  1557,  1558,  1559,  1560,  1564,  1568,  1571,  1573,
    1575,  1577,  1579,  1581,  1582,  1586,  1590,  1594,  1595,  1596,
    1598,  1601,  1602
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if RAKU_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LIT_INT", "LIT_FLOAT",
  "LIT_STR", "LIT_INTERP_STR", "LIT_REGEX", "LIT_MATCH_GLOBAL",
  "LIT_SUBST", "VAR_SCALAR", "VAR_ARRAY", "VAR_HASH", "VAR_TWIGIL",
  "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET", "VAR_CAPTURE",
  "VAR_FH", "VAR_NAMED_CAPTURE", "KW_USE", "TESTOP", "KW_MY", "KW_SAY",
  "KW_PRINT", "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR",
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_CONSTANT", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_ROLE",
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT",
  "OP_SLE", "OP_SGT", "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt",
  "loop_incr", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "scalar_methcall", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "jct_expr", "dor_expr", "range_expr",
  "add_expr", "repl_expr", "addsub_expr", "mul_expr", "unary_expr",
  "pow_expr", "scalar_list", "meth_name", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-756)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-412)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -756,    64,  1610,  -756,  -756,  -756,  -756,  -756,   520,   -70,
     100,    13,   244,  -756,  -756,   859,  -756,  -756,  -756,    69,
    1744,    47,  2896,  2963,  3030,  3097,  3164,    -6,     4,  3164,
    1811,   125,  3164,    87,   136,  3231,  3298,     4,   238,    53,
      90,    44,    44,  3365,     4,     4,  3164,   235,   277,    94,
    -756,   293,   330,   335,   859,   859,  -756,  1878,   195,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,   511,
    -756,  -756,  -756,  1481,   562,  -756,   315,   347,   275,  -756,
     148,   294,   -94,   297,  -756,  -756,   320,   332,   346,   460,
     329,   357,  3164,  3164,  3164,  3164,  3164,  3164,   484,  1946,
    3164,   486,  3164,  3164,    55,  2014,   393,   395,   103,  -756,
       4,  -756,  -756,   408,   128,  -756,  2082,  -756,   -42,   204,
     216,   217,   424,   530,   161,   560,  1878,   481,  1878,    19,
    1878,     4,  1878,     4,   -36,   158,   270,  -756,  -756,   465,
    -756,   476,   531,   555,   496,   119,   149,  1878,     4,  1878,
       4,  -756,  2150,  -756,  3164,  3164,  -756,  3164,  3164,  -756,
    3164,  3164,  3164,  2217,  3164,  -756,   620,  -756,  -756,  -756,
     573,   544,    26,   589,   610,   645,  -756,  -756,  -756,   366,
     653,  1067,  3164,  3164,  3164,  3164,  3164,  3164,  3164,  3164,
    -756,  3164,  3164,  3164,  3164,  3164,  3164,  3164,  3164,  -756,
     859,   859,   859,   859,   859,   859,   859,   859,   859,   859,
     859,   859,   859,   859,   859,   746,   859,   859,   859,   859,
     859,   859,   859,   859,   859,   859,   859,   859,   859,   859,
     524,   792,   151,  -756,  -756,   662,   671,   680,   688,   710,
     721,   137,  -756,   487,   678,   736,   720,   730,   740,   842,
     762,  -756,   588,  3164,   875,  3164,  -756,  3164,   788,   366,
     618,  -756,  3164,  3164,  -756,  3432,  -756,  3499,  -756,   246,
     251,   252,   279,   819,   854,  -756,   629,   706,  3164,  3164,
    3164,  3164,  3164,  3164,  3164,  -756,  3164,   717,  3164,  3164,
    -756,  3164,   731,   652,   733,  -756,   950,  3164,  -756,   859,
     859,    -1,  -756,  -756,  -756,  -756,  3164,  3164,  -756,   948,
    3164,   949,  3164,   737,   937,   759,  -756,   959,   851,   852,
     855,   856,   857,   847,  -756,  -756,  -756,     4,    -8,  -756,
      15,   858,   860,  -756,  -756,  -756,  -756,  2284,  -756,   554,
     862,   165,   872,  2896,  2963,  3164,   672,    89,   187,  -756,
     288,   863,   864,   867,   868,   869,   870,   871,   873,   874,
     876,   877,   878,   879,   880,   889,   890,   586,   586,   586,
     586,   586,   586,   892,   586,   586,   586,   586,   586,   586,
     586,   586,  -756,  -756,  -756,  -756,  -756,  -756,   883,   883,
     294,   -94,   297,   297,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
     891,   977,   893,  -756,  2352,  -756,  -756,  -756,  -756,  -756,
    -756,  3164,  2420,  -756,   905,   907,   909,  -756,  2488,  -756,
    2556,  -756,   894,   908,   896,  -756,  -756,   898,  -756,   904,
    1878,   -52,   943,   127,  2624,   318,   906,  3164,  -756,  3566,
    -756,  3164,  -756,    21,  -756,  3164,  3164,   919,  1016,  2284,
     910,   911,   912,   914,   915,   916,   917,   344,  2284,   918,
     921,   390,     4,   -14,  3633,  -756,     4,     4,    12,   -27,
     -26,  -756,    37,   902,   764,  1240,   922,   926,   -22,   924,
     923,   931,   925,     4,     4,     4,   941,  3164,  -756,  -756,
    -756,  -756,  -756,  -756,  1014,  1033,  -756,    50,   902,   766,
      22,   236,     5,  -756,   771,  1034,  3164,  3164,  3164,   259,
     101,   220,  -756,   248,  3164,  3164,  -756,  3164,  3164,  -756,
    3164,  3164,  3164,  3164,  3164,  3164,  3164,  3164,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,   859,  2692,  -756,  2760,   933,
     773,   934,  -756,   775,  3164,  3164,  3164,  -756,   777,   134,
    -756,   780,   782,  -756,  -756,  -756,  -756,  -756,   784,  3164,
    -756,  3164,  3164,  3164,   790,  -756,   200,  -756,   935,  1878,
     434,   936,   902,   793,   938,   939,  3164,  -756,   795,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,   797,  -756,  -756,
    -756,   748,  -756,  -756,  1878,     4,  -756,  -756,  1040,  -756,
    1046,  1049,  -756,  1050,  1051,  -756,   902,   301,  2896,  2963,
    1425,  -756,   290,  -756,  -756,  3164,     4,  -756,  -756,  -756,
     945,   946,  1035,  -756,  -756,  3164,   947,  -756,  -756,   117,
     683,   808,  1012,  -756,  -756,   902,   902,   799,   805,  1055,
    1060,  1061,  -756,  -756,   153,   952,   968,   258,  3164,  3164,
    3164,  3164,  3164,  3164,  -756,  -756,  -756,  -756,   261,   289,
     291,   299,   307,   333,   342,   356,   360,   371,   382,   417,
    -756,  -756,   807,  -756,   809,   811,  -756,   976,  -756,  -756,
     985,   986,   987,  -756,  3164,  -756,  1093,  -756,  2284,  1004,
     477,  -756,  -756,  1005,  1053,  1056,  -756,   813,  -756,  3164,
    -756,  -756,   902,  -756,  -756,   488,  1010,  1011,   -14,  -756,
     816,   840,     4,     4,     4,  -756,  -756,  -756,  -756,   202,
     343,   174,  -756,   433,  3164,  3164,  3164,  3164,  3164,  3164,
    3164,  3164,  -756,     4,  1006,  -756,  -756,     4,  1017,  1677,
    -756,   280,   326,    46,    49,   835,  1018,  1019,   524,  -756,
    -756,   902,   902,  1132,  1133,  1134,  3164,  2828,  1037,  1038,
    -756,   442,   450,   452,   453,   464,   467,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  1072,   818,  -756,
    -756,  -756,  3164,  3164,  2284,   501,  -756,  -756,  3164,  -756,
    -756,  -756,  -756,     4,     4,  -756,  -756,  -756,  -756,  -756,
    1135,  1136,  3164,  3164,  3164,  -756,  -756,  -756,   478,   503,
     505,   513,   516,   535,   536,   537,  -756,  -756,  -756,  3164,
     256,  1137,  1138,  1028,  -756,    28,  1083,  -756,    36,  -756,
    1139,  1140,  1032,  3164,  -756,  1141,  1142,  1039,  3164,  -756,
      59,    83,  1043,  1044,  -756,  -756,   353,  -756,  -756,  -756,
    -756,  -756,   539,  1026,   821,  3164,  3164,  -756,  -756,  -756,
    -756,  -756,  -756,  3164,  1045,  -756,  -756,   823,  -756,   522,
     920,  -756,  -756,  -756,   574,   575,   580,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  1047,  -756,  -756,  -756,  -756,
       4,  1042,   828,  1031,  1358,  1042,   830,  1052,  1057,  -756,
    1058,  1059,  1062,  -756,  1063,  1145,  1151,  1064,  3164,  -756,
    1154,  1155,  1065,  3164,  -756,  -756,  -756,    42,  -756,  -756,
    -756,  1048,   592,   593,  -756,  -756,  1066,  -756,     4,  -756,
    -756,  -756,  -756,  1677,  -756,  -756,  1042,  -756,  2896,  2963,
    1492,  -756,   441,  -756,  1042,  -756,  -756,  -756,  -756,  -756,
    -756,  1069,  1070,  -756,  1071,  1075,  1076,  -756,  1077,  1042,
     832,  -756,  -756,  -756,  -756,  -756,  1054,  -756,   463,   175,
    -756,   601,  3164,  3164,  3164,  3164,  3164,  3164,  3164,  3164,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  1042,
       4,  3164,  3164,  3164,  -756,  -756,  -756,   609,   611,   612,
     654,   658,   670,   674,   675,  -756,  -756,   689,   693,   694,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   387,   388,   389,   391,   392,   393,
     394,   404,   403,   405,   406,     0,   395,   396,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     390,     0,     0,     0,     0,     0,    91,     0,     3,     4,
      92,    93,   100,   101,   102,   103,    94,    95,   110,     0,
     111,   112,   113,    99,     0,   298,   300,   318,   321,   323,
     326,   328,   330,   333,   338,   342,   344,   358,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   393,   394,   404,
       0,   411,   341,     0,   392,    31,     0,   385,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,     0,   412,   297,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   136,     0,   137,     0,     0,   104,     0,     0,   105,
       0,     0,     0,     3,     0,   382,    96,    98,   379,   205,
       0,     0,     0,     0,     0,     0,   340,   339,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,     0,     0,     0,     0,     0,
     296,     0,   362,     0,     0,     0,     0,     0,   367,     0,
     366,   360,     0,     0,     0,     0,    26,     0,   407,     0,
       0,    34,     0,     0,     6,     0,     9,     0,    10,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      38,     0,     0,   117,     0,   129,     0,     0,   150,     0,
       0,     0,     3,   157,    41,    42,     0,     0,   153,     0,
       0,     0,     0,     0,   132,     0,   135,     0,     0,     0,
       0,     0,     0,     0,   380,   381,   383,     0,     0,   208,
       0,     0,     0,   243,    88,    89,   408,     0,   262,   392,
     393,   394,   404,     0,     0,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   304,   307,
     308,   301,   302,     0,   305,   306,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   319,   320,   322,   324,   325,
     327,   329,   331,   332,   337,   334,   335,   336,   343,   347,
     357,   351,   352,   353,   354,   355,   356,   349,   350,   348,
     378,     0,   375,    47,     0,    81,    82,    83,    84,    85,
      44,     0,     0,   361,   398,   399,   400,    49,     0,   370,
       0,   359,     0,     0,     0,   296,    33,     0,   386,     0,
       0,     0,   389,   403,     0,     0,     0,     0,    23,     0,
      24,     0,    25,     0,   160,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   408,     0,     0,   120,   408,     0,     0,   324,
     325,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   408,     0,   408,     0,     0,   106,   107,
     108,   109,   295,    97,     0,     0,   208,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,     0,
       0,     0,   288,     0,     0,     0,   289,     0,     0,   290,
       0,     0,     0,     0,     0,     0,     0,     0,   263,    51,
      53,    55,    58,    56,    57,    52,    54,    64,    66,    68,
      71,    69,    70,    65,    67,     0,     0,   371,     0,     0,
       0,     0,   167,     0,     0,     0,     0,   369,     0,   403,
     365,     0,     0,   398,   399,   400,    32,     5,     0,     0,
      11,     0,     0,     0,     0,    16,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,    72,
      75,    74,    78,    76,    77,    73,    36,     0,    79,    80,
      39,   114,   119,   118,     0,     0,   128,   147,     0,   149,
       0,     0,   254,     0,     0,   156,     0,     0,     0,     0,
       0,   168,     0,    28,    27,     0,     0,   151,   401,   402,
       0,     0,   130,   133,   134,     0,     0,   206,   207,     0,
       0,     0,     0,   204,   165,     0,     0,     0,     0,     0,
       0,     0,   242,   410,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   272,   279,   286,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     299,   377,     0,   374,     0,     0,    46,     0,    48,   166,
       0,     0,     0,   368,     0,   363,     0,   364,     0,     0,
       0,   250,   249,     0,     0,     0,    18,     0,    19,     0,
      22,   159,     0,    30,    29,     0,   410,   410,     0,   121,
       0,   122,     0,     0,     0,   255,   256,   155,   257,     0,
       0,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,    62,    63,     0,     0,     0,
     203,     0,     0,     0,     0,     0,     0,     0,     0,   164,
     162,     0,     0,     0,     0,     0,     0,     0,   398,   400,
     283,     0,     0,     0,     0,     0,     0,   291,   292,   293,
     294,   264,   266,   268,   271,   269,   270,   265,   267,   376,
     373,   372,    45,    59,    60,    61,   247,     0,     0,    13,
      12,    17,     0,     0,     0,     0,   158,     7,     0,    37,
      40,   116,   115,   408,     0,   126,   148,   145,   146,   258,
       0,     0,     0,     0,     0,   172,   173,   170,     0,     0,
       0,     0,     0,     0,     0,     0,   154,   152,   131,     0,
     392,     0,     0,     0,   140,     0,     3,   238,     0,   235,
       0,     0,     0,     0,   214,     0,     0,     0,     0,   209,
       0,     0,     0,     0,   210,   211,     0,   161,   163,   244,
     245,   246,     0,   167,     0,     0,     0,   273,   275,   278,
     276,   277,   274,     0,   410,   252,   251,     0,    20,     0,
     123,   124,   259,   260,     0,     0,     0,   174,   176,   178,
     181,   179,   180,   175,   177,     0,   141,   142,   143,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,   215,   212,   213,     0,   241,   282,
     281,   166,     0,     0,   248,    14,   410,     8,     0,   127,
     182,   184,   183,     0,   139,   237,     0,   186,     0,     0,
       0,   185,     0,   234,     0,   223,   224,   218,   221,   222,
     217,     0,     0,   227,     0,     0,     0,   228,     0,     0,
       0,   280,   284,   285,    21,   125,     0,   236,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,   233,   231,   232,   220,   229,   230,   219,   240,     0,
       0,     0,     0,     0,   190,   191,   188,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   138,     0,     0,     0,
     192,   194,   196,   199,   197,   198,   193,   195,   200,   202,
     201
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -756,  -756,     1,  -756,  -462,  -608,  -756,  -756,  -756,  -756,
    -756,   207,  -756,  -756,  -756,  -756,  -756,  -267,  -755,  -756,
    -756,  -756,   666,  -756,  -756,   621,   747,  -313,   310,   913,
      -2,  -202,  -756,   558,  -756,   619,   -16,   978,   975,   734,
      11,  -756,  -756,  -229,  -756,  -756,   -62,  -756
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   181,    59,    60,   475,    61,    62,    63,    64,
      65,   853,    66,    67,   488,    68,    69,   303,   857,    70,
      71,   328,   507,    72,   512,   571,   445,   484,   111,   161,
     117,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   276,   410,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   412,   729,   373,   454,   504,   859,   136,   481,
     135,   612,    24,   482,   505,   635,   636,   509,   223,   224,
     127,   129,   131,   133,   134,   481,   112,   139,   141,   482,
     144,   481,   481,   148,   150,   482,   482,   243,   481,   296,
     331,   165,   482,   252,   168,   288,   481,   622,   620,   621,
     482,   579,   481,   100,   260,   179,   482,   119,   120,   121,
     860,   122,   289,   865,     3,   176,   177,   580,   861,   248,
     581,   866,   249,   935,   659,   660,   661,   261,   123,   154,
     262,   936,   124,   113,   221,   221,   297,   618,   332,    58,
     235,   236,   237,   238,   239,   240,   155,   940,   244,   145,
     246,   247,   623,   624,   637,   941,   650,   651,   171,   862,
     652,    58,   867,   250,   259,   524,   157,   506,   103,    58,
     483,   948,   937,   825,   277,   172,   287,   288,   292,    58,
     294,   662,   525,   158,   262,   142,   508,    58,   290,   143,
     593,   291,   592,   656,   289,   313,   942,   315,   146,   921,
     318,   863,   319,   320,   868,   321,   322,   925,   323,   324,
     325,   323,   326,   989,   938,   864,   965,   125,   869,   160,
     973,   273,   156,   650,   651,   274,   653,   652,   939,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   943,   359,
     360,   361,   362,   363,   364,   365,   366,   657,   658,   583,
     288,   288,   944,   388,   389,   714,   704,   101,   156,   159,
     254,   997,   829,   527,   715,   526,   625,   289,   289,  1011,
     290,   219,   220,   291,   467,   102,   309,   675,   255,   471,
     528,   299,   300,   257,  1018,   478,   394,   395,   396,   397,
     398,   654,   421,   760,   310,   104,   481,   105,    99,   169,
     482,   432,   104,   434,   105,   435,   311,   422,   776,   221,
     438,   439,   180,   441,  1035,   446,   821,   830,   831,   221,
     413,   414,   101,   777,   312,   514,   460,   461,   462,   463,
     464,   465,   466,   479,   480,   668,   469,   470,   279,   669,
     517,   170,   959,   290,   290,   670,   291,   291,   671,   672,
     836,  1025,   673,   485,   486,   487,   159,   173,   490,   263,
     492,   738,    73,   529,   530,   739,   744,   531,   532,   745,
     746,   265,   267,   264,   533,   721,   747,   534,   535,   748,
     749,   536,   537,   750,   751,   266,   268,   137,   138,   304,
     174,   519,   520,   521,   523,   175,   676,   151,   153,   916,
     917,   447,   560,   690,   166,   167,   449,   451,   152,   737,
     563,   257,   104,    58,   105,   448,   568,   305,   572,   832,
     450,   452,   279,   833,   677,   218,    99,   427,   285,   281,
     498,   286,   282,   283,   780,   674,   834,   787,   769,   770,
     301,   200,   201,   202,   203,   302,   222,   598,   225,   453,
     855,   204,   205,   206,   302,   856,   607,   199,   499,   199,
     500,   226,   227,   228,   538,   788,   752,   789,   501,   561,
     137,   762,   207,   208,   229,   790,   547,   209,   210,   211,
     212,   213,   214,   791,   269,   270,   271,   585,   578,   215,
     586,   293,   179,   295,   298,   588,   858,   590,   233,   591,
     230,   856,   548,   594,   595,   816,   216,   217,   314,   792,
     316,   549,   285,   606,   231,   286,   262,  1002,   793,   835,
    1003,  1004,   615,   947,   232,   550,   234,  1005,   856,   551,
    1006,  1007,   794,   632,  1008,  1009,   795,   336,   337,  1021,
     552,    73,   279,  1022,   692,   646,   695,   796,   241,   281,
     245,   553,   282,   283,   877,   878,  1023,   278,   797,   610,
     279,   280,   262,    99,   665,   666,   667,   281,   253,   710,
     282,   283,   678,   679,   284,   680,   681,   256,   682,   683,
     684,   685,   686,   687,   688,   689,   554,   182,   399,   876,
     183,   184,   922,   798,   272,   926,   400,   185,   401,   402,
     186,   187,   305,   718,   188,   189,   719,   403,   404,   837,
     199,   599,   700,   701,   702,   405,   406,  1010,   887,   601,
     275,   602,   603,   407,   408,   409,   888,   709,   889,   890,
     711,   712,   285,   604,   304,   286,   605,   717,   191,  1024,
     891,   192,   193,   892,   725,   305,   810,   547,   194,   262,
     285,   195,   196,   286,   907,   197,   198,   817,   423,   262,
     818,    89,   730,    90,    91,    92,    93,    94,    95,    96,
     898,   308,   548,   262,   549,    97,   740,   741,   743,   908,
     190,   909,   550,   753,   990,   551,   306,   503,    98,   910,
      99,   957,   911,   758,   262,    89,   808,    90,    91,    92,
      93,    94,    95,    96,   552,   553,   554,   815,   698,    97,
     307,   912,   913,   914,   330,   949,   781,   782,   783,   784,
     785,   786,   515,   327,    99,     4,     5,     6,     7,   473,
     474,   199,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,   599,   601,   216,   217,   399,   329,   605,
     960,   961,   806,   110,    28,   400,   962,   401,   402,   431,
     262,   803,   805,    33,   333,   884,   403,   404,   992,   993,
     305,    41,    42,    43,   405,   406,    46,  1026,   547,   334,
     548,   549,   407,   408,   409,  1040,    50,  1041,  1042,   437,
     262,   761,   838,   839,   840,   841,   842,   843,   844,   845,
     457,   458,   897,   382,   383,   384,   899,   854,   367,   368,
     369,   370,   371,   372,   335,   374,   375,   376,   377,   378,
     379,   380,   381,   550,   882,   728,   474,   551,    54,   338,
    1043,   415,   611,   613,  1044,    55,   616,   617,   619,   552,
     416,   140,    57,   553,   554,    73,  1045,    58,   522,   417,
    1046,  1047,   424,   642,   643,   644,   399,   418,   599,   411,
     895,   896,   601,   605,   400,  1048,   401,   402,   763,  1049,
    1050,   764,   765,   766,   767,   403,   404,   336,   459,   419,
     904,   905,   906,   405,   406,   385,   386,   387,   336,   468,
     420,   407,   408,   409,   425,   870,   426,   915,   871,   427,
     872,   873,   472,   337,   476,   337,   429,   924,   493,   337,
     428,   930,     4,     5,     6,     7,   934,   824,   474,   106,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
     495,   337,   430,   952,   953,   626,   627,   655,   627,   433,
     110,   954,   663,   262,   697,   262,   699,   262,   703,   262,
      33,   705,   706,   707,   262,   336,   708,   436,    41,    42,
      43,   713,   586,    46,   722,   627,   726,   262,   727,   262,
     771,   627,   972,    50,   455,   731,   772,   627,   799,   262,
     800,   706,   801,   262,   336,   814,   984,   823,   337,   894,
     262,   988,   951,   262,   956,   262,   754,   958,   474,   966,
     627,   974,   627,  1019,   627,   162,   164,   392,   393,   456,
     477,   854,   489,   491,   494,    54,   998,   999,  1001,   496,
     497,   498,    55,   502,   499,   500,   501,   518,   510,    57,
     511,   555,   539,   540,    58,   516,   541,   542,   543,   544,
     545,   557,   546,   547,   221,   548,   549,   550,   551,   552,
    1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,   553,   554,
     564,   556,   565,   558,   566,   582,   574,   576,   573,  1037,
    1038,  1039,   575,   577,   596,   587,   597,   302,   647,   599,
     600,   601,   638,   602,   603,   604,   605,   608,   822,   640,
     609,   633,   826,   827,   828,   634,   645,   648,   664,   639,
     732,   641,   696,   698,   716,   720,   733,   723,   724,   734,
     735,   736,   757,   846,   755,   756,   759,   848,   768,   773,
       4,     5,     6,     7,   774,   775,   778,   339,   340,   341,
     342,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   343,   344,    24,   779,   802,    25,    26,    27,    28,
     345,   346,    31,    32,   803,   804,   805,   807,    33,    34,
      35,    36,    37,    38,   347,   348,    41,    42,    43,    44,
      45,    46,    47,   809,   811,   812,    48,    49,   813,   819,
     820,    50,   847,   900,   901,    51,   849,   874,   875,   879,
     880,   881,   885,   886,   893,   902,   903,   918,   919,   920,
     923,   929,   950,   927,   928,   931,   932,   967,   933,   981,
      52,    53,   945,   946,   955,   982,   963,   856,   985,   986,
     996,   975,   649,    54,   991,  1020,   976,   977,   978,   694,
      55,   979,   980,   983,   987,   994,    56,    57,  1012,  1013,
    1014,   584,    58,   349,  1015,  1016,  1017,   391,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     964,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   628,   629,    24,     0,   995,    25,
      26,    27,    28,    29,   630,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,     0,    50,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1036,     0,     0,    52,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,    56,
      57,     4,     5,     6,     7,    58,   631,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   968,   969,    24,     0,     0,    25,    26,    27,
      28,    29,   970,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,    48,    49,     0,
       0,     0,    50,     0,     0,     0,    51,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,    56,    57,    46,
       0,     0,     0,    58,   971,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
    -411,  -411,     0,   110,    28,     0,     0,     0,     0,     0,
       0,    54,     0,    33,     0,     0,     0,     0,    55,     0,
       0,    41,    42,    43,   140,    57,    46,     0,     0,     0,
      58,   742,     0,     0,  -411,  -411,    50,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
       0,     0,     0,  -411,     0,     0,     0,     0,     0,     0,
       0,  -411,  -411,  -411,     0,  -411,     0,     0,  -411,  -411,
    -411,  -411,  -411,  -411,     0,  -411,  -411,  -411,    54,  -411,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,   140,    57,     4,     5,     6,     7,    58,  1000,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
      26,    27,    28,    29,    30,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,     0,    50,     0,     0,     0,    51,     0,
       4,     5,     6,     7,     0,     0,     0,   850,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    52,    53,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,    56,
      57,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
     851,   852,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,     0,    57,    46,     0,
       0,     0,    58,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,   115,   116,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
     140,    57,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   178,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   242,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   251,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   258,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,     0,
      57,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,   180,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,     0,    57,    46,     0,
       0,     0,    58,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   513,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   559,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   562,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
     569,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   567,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,   442,
       7,     0,     0,     0,   114,   107,   108,   109,   443,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   570,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   178,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,   569,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   691,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   693,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   883,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,     0,   126,    46,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,    54,
       0,    33,     0,     0,     0,     0,    55,     0,     0,    41,
      42,    43,     0,   128,    46,     0,     0,     0,    58,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,     0,
     130,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,     0,   132,    46,     0,
       0,     0,    58,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,     0,    57,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
       0,   147,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,     0,   149,    46,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,    54,     0,    33,     0,     0,     0,     0,    55,     0,
       0,    41,    42,    43,     0,    57,    46,     0,     0,     0,
     163,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     4,     5,   442,     7,     0,     0,     0,   114,
     107,   108,   109,   443,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,    54,     0,
      33,     0,     0,     0,     0,    55,     0,     0,    41,    42,
      43,     0,   440,    46,     0,     0,     0,    58,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,     0,   444,
      46,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,     0,   589,    46,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,   614,     0,     0,     0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   231,   611,   206,   272,    14,   762,    14,    10,
      26,   473,    26,    14,    22,    37,    38,   330,   112,   113,
      22,    23,    24,    25,    26,    10,    15,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    99,    10,    75,
      14,    43,    14,   105,    46,    26,    10,    10,    75,    75,
      14,   103,    10,   123,   116,    57,    14,    10,    11,    12,
      14,    14,    43,    14,     0,    54,    55,   119,    22,    14,
     122,    22,    17,    14,    69,    70,    71,   119,    31,    26,
     122,    22,    35,    14,   111,   111,   122,    75,    62,   125,
      92,    93,    94,    95,    96,    97,    43,    14,   100,    12,
     102,   103,    65,    66,   126,    22,    56,    57,    14,    63,
      60,   125,    63,    58,   116,    26,    26,   125,   105,   125,
     121,   876,    63,   731,   126,    31,   128,    26,   130,   125,
     132,   126,    43,    43,   122,    10,   121,   125,   119,    14,
     453,   122,   121,   121,    43,   147,    63,   149,    12,   121,
     152,   105,   154,   155,   105,   157,   158,   121,   160,   161,
     162,   163,   164,   121,   105,   119,   921,   120,   119,   125,
     925,    10,   119,    56,    57,    14,   126,    60,   119,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   105,   191,
     192,   193,   194,   195,   196,   197,   198,   510,   511,    72,
      26,    26,   119,   219,   220,     5,    72,   107,   119,   119,
     107,   966,    10,    26,    14,   126,   483,    43,    43,   974,
     119,    73,    74,   122,   286,   125,   107,   126,   125,   291,
      43,    73,    74,   105,   989,   297,   225,   226,   227,   228,
     229,   508,   105,   126,   125,   118,    10,   120,   120,    14,
      14,   253,   118,   255,   120,   257,   107,   120,   105,   111,
     262,   263,    67,   265,  1019,   267,   728,    65,    66,   111,
     119,   120,   107,   120,   125,   337,   278,   279,   280,   281,
     282,   283,   284,   299,   300,    26,   288,   289,    29,    30,
     125,    14,   900,   119,   119,    36,   122,   122,    39,    40,
     126,   126,    43,   302,   306,   307,   119,    14,   310,   105,
     312,    10,     2,   126,    26,    14,    26,    29,    30,    29,
      30,   105,   105,   119,    36,   592,    36,    39,    40,    39,
      40,    43,    44,    43,    44,   119,   119,    27,    28,   119,
      10,   343,   344,   345,   346,    10,   126,    37,    38,    93,
      94,   105,   414,   555,    44,    45,   105,   105,   120,   626,
     422,   105,   118,   125,   120,   119,   428,   119,   430,    26,
     119,   119,    29,    30,   126,   100,   120,   119,   119,    36,
     119,   122,    39,    40,   126,   126,    43,   126,   655,   656,
     120,    76,    77,    78,    79,   125,   102,   459,   101,   120,
     120,    86,    87,    88,   125,   125,   468,   119,   119,   119,
     119,   114,   115,   116,   126,   126,   126,   126,   119,   421,
     110,   650,   107,   108,   104,   126,   119,    80,    81,    82,
      83,    84,    85,   126,    10,    11,    12,   119,   440,    92,
     122,   131,   444,   133,   134,   447,   120,   449,   119,   451,
     118,   125,   119,   455,   456,   722,   109,   110,   148,   126,
     150,   119,   119,   119,   118,   122,   122,    26,   126,   126,
      29,    30,   474,   120,    14,   119,   119,    36,   125,   119,
      39,    40,   126,   485,    43,    44,   126,   121,   122,    26,
     119,   181,    29,    30,   556,   497,   558,   126,    14,    36,
      14,   119,    39,    40,   771,   772,    43,    26,   126,   119,
      29,    30,   122,   120,   516,   517,   518,    36,   123,   581,
      39,    40,   524,   525,    43,   527,   528,   119,   530,   531,
     532,   533,   534,   535,   536,   537,   119,    26,    14,   768,
      29,    30,   855,   126,    14,   858,    22,    36,    24,    25,
      39,    40,   119,   119,    43,    44,   122,    33,    34,   126,
     119,   119,   564,   565,   566,    41,    42,   126,   126,   119,
      10,   119,   119,    49,    50,    51,   126,   579,   126,   126,
     582,   583,   119,   119,   119,   122,   119,   589,    26,   126,
     126,    29,    30,   126,   596,   119,   119,   119,    36,   122,
     119,    39,    40,   122,   126,    43,    44,   119,   121,   122,
     122,    91,   614,    93,    94,    95,    96,    97,    98,    99,
     119,   125,   119,   122,   119,   105,   628,   629,   630,   126,
     119,   126,   119,   635,   947,   119,   105,   327,   118,   126,
     120,   119,   126,   645,   122,    91,   708,    93,    94,    95,
      96,    97,    98,    99,   119,   119,   119,   719,   119,   105,
     105,   126,   126,   126,   120,   126,   668,   669,   670,   671,
     672,   673,   118,    53,   120,     3,     4,     5,     6,    27,
      28,   119,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   119,   119,   109,   110,    14,   125,   119,
     126,   126,   704,    31,    32,    22,   126,    24,    25,   121,
     122,   119,   119,    41,   125,   777,    33,    34,   126,   126,
     119,    49,    50,    51,    41,    42,    54,   126,   119,   119,
     119,   119,    49,    50,    51,   126,    64,   126,   126,   121,
     122,    58,   744,   745,   746,   747,   748,   749,   750,   751,
     121,   122,   814,     7,     8,     9,   818,   759,   200,   201,
     202,   203,   204,   205,   119,   207,   208,   209,   210,   211,
     212,   213,   214,   119,   776,    27,    28,   119,   106,   126,
     126,   119,   472,   473,   126,   113,   476,   477,   478,   119,
     119,   119,   120,   119,   119,   485,   126,   125,   126,   119,
     126,   126,   124,   493,   494,   495,    14,   119,   119,    17,
     812,   813,   119,   119,    22,   126,    24,    25,    10,   126,
     126,    13,    14,    15,    16,    33,    34,   121,   122,   119,
     832,   833,   834,    41,    42,   216,   217,   218,   121,   122,
     119,    49,    50,    51,   108,    10,   126,   849,    13,   119,
      15,    16,   121,   122,   121,   122,    14,   856,   121,   122,
     120,   863,     3,     4,     5,     6,   868,    27,    28,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     121,   122,   120,   885,   886,   121,   122,   121,   122,    14,
      31,   893,   121,   122,   121,   122,   121,   122,   121,   122,
      41,   121,   122,   121,   122,   121,   122,   119,    49,    50,
      51,   121,   122,    54,   121,   122,   121,   122,   121,   122,
     121,   122,   924,    64,   105,   615,   121,   122,   121,   122,
     121,   122,   121,   122,   121,   122,   938,   121,   122,   121,
     122,   943,   121,   122,   121,   122,   636,    27,    28,   121,
     122,   121,   122,   121,   122,    42,    43,   223,   224,   105,
      10,   963,    14,    14,    27,   106,   968,   969,   970,    10,
     119,   119,   113,   126,   119,   119,   119,   105,   120,   120,
     120,    89,   119,   119,   125,   123,   119,   119,   119,   119,
     119,    14,   119,   119,   111,   119,   119,   119,   119,   119,
    1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,   119,   119,
     105,   120,   105,   120,   105,    72,   108,   119,   124,  1021,
    1022,  1023,   126,   119,   105,   119,    10,   125,    14,   119,
     119,   119,   108,   119,   119,   119,   119,   119,   728,   108,
     119,   119,   732,   733,   734,   119,   105,    14,    14,   126,
      10,   126,   119,   119,   119,   119,    10,   119,   119,    10,
      10,    10,    27,   753,   119,   119,   119,   757,    56,    14,
       3,     4,     5,     6,    14,    14,   124,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   126,   119,    29,    30,    31,    32,
      33,    34,    35,    36,   119,   119,   119,    14,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,   119,   119,    72,    59,    60,    72,   119,
     119,    64,   126,   823,   824,    68,   119,   119,   119,     7,
       7,     7,   105,   105,    72,    10,    10,    10,    10,   121,
      67,   119,   126,    14,    14,    14,    14,   126,   119,    14,
      93,    94,   119,   119,   119,    14,   119,   125,    14,    14,
     963,   119,   506,   106,   126,   121,   119,   119,   119,   558,
     113,   119,   119,   119,   119,   119,   119,   120,   119,   119,
     119,   444,   125,   126,   119,   119,   119,   222,    -1,   221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     920,    -1,    -1,    -1,   924,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,   958,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1020,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,
     120,     3,     4,     5,     6,   125,   126,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,   119,   120,    54,
      -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    40,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    49,    50,    51,   119,   120,    54,    -1,    -1,    -1,
     125,   126,    -1,    -1,    73,    74,    64,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,   106,   118,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,   119,   120,     3,     4,     5,     6,   125,   126,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,   119,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,   119,   120,    54,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,
     119,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,
      50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,
      -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,    54,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,
      51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,
      54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   128,   129,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      64,    68,    93,    94,   106,   113,   119,   120,   125,   130,
     131,   133,   134,   135,   136,   137,   139,   140,   142,   143,
     146,   147,   150,   155,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   171,   172,   174,    91,
      93,    94,    95,    96,    97,    98,    99,   105,   118,   120,
     123,   107,   125,   105,   118,   120,    10,    11,    12,    13,
      31,   155,   167,    14,    10,   119,   120,   157,   173,    10,
      11,    12,    14,    31,    35,   120,   120,   157,   120,   157,
     120,   157,   120,   157,   157,   163,    14,   155,   155,   157,
     119,   157,    10,    14,   157,    12,    12,   120,   157,   120,
     157,   155,   120,   155,    26,    43,   119,    26,    43,   119,
     125,   156,   156,   125,   156,   157,   155,   155,   157,    14,
      14,    14,    31,    14,    10,    10,   167,   167,   121,   157,
      67,   129,    26,    29,    30,    36,    39,    40,    43,    44,
     119,    26,    29,    30,    36,    39,    40,    43,    44,   119,
      76,    77,    78,    79,    86,    87,    88,   107,   108,    80,
      81,    82,    83,    84,    85,    92,   109,   110,   100,    73,
      74,   111,   102,   112,   113,   101,   114,   115,   116,   104,
     118,   118,    14,   119,   119,   157,   157,   157,   157,   157,
     157,    14,   121,   173,   157,    14,   157,   157,    14,    17,
      58,   121,   173,   123,   107,   125,   119,   105,   121,   157,
     173,   119,   122,   105,   119,   105,   119,   105,   119,    10,
      11,    12,    14,    10,    14,    10,   169,   157,    26,    29,
      30,    36,    39,    40,    43,   119,   122,   157,    26,    43,
     119,   122,   157,   155,   157,   155,    75,   122,   155,    73,
      74,   120,   125,   144,   119,   119,   105,   105,   125,   107,
     125,   107,   125,   157,   155,   157,   155,    23,   157,   157,
     157,   157,   157,   157,   157,   157,   157,    53,   148,   125,
     120,    14,    62,   125,   119,   119,   121,   122,   126,    10,
      11,    12,    13,    24,    25,    33,    34,    47,    48,   126,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   160,   160,   160,
     160,   160,   160,   158,   160,   160,   160,   160,   160,   160,
     160,   160,     7,     8,     9,   162,   162,   162,   163,   163,
     164,   165,   166,   166,   167,   167,   167,   167,   167,    14,
      22,    24,    25,    33,    34,    41,    42,    49,    50,    51,
     170,    17,   170,   119,   120,   119,   119,   119,   119,   119,
     119,   105,   120,   121,   124,   108,   126,   119,   120,    14,
     120,   121,   157,    14,   157,   157,   119,   121,   157,   157,
     120,   157,     5,    14,   120,   153,   157,   105,   119,   105,
     119,   105,   119,   120,   144,   105,   105,   121,   122,   122,
     157,   157,   157,   157,   157,   157,   157,   173,   122,   157,
     157,   173,   121,    27,    28,   132,   121,    10,   173,   163,
     163,    10,    14,   121,   154,   129,   157,   157,   141,    14,
     157,    14,   157,   121,    27,   121,    10,   119,   119,   119,
     119,   119,   126,   155,    14,    22,   125,   149,   121,   154,
     120,   120,   151,   121,   173,   118,   123,   125,   105,   157,
     157,   157,   126,   157,    26,    43,   126,    26,    43,   126,
      26,    29,    30,    36,    39,    40,    43,    44,   126,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,    89,   120,    14,   120,   121,
     173,   157,   121,   173,   105,   105,   105,   121,   173,    14,
     121,   152,   173,   124,   108,   126,   119,   119,   157,   103,
     119,   122,    72,    72,   153,   119,   122,   119,   157,   120,
     157,   157,   121,   154,   157,   157,   105,    10,   173,   119,
     119,   119,   119,   119,   119,   119,   119,   173,   119,   119,
     119,   155,   131,   155,   120,   157,   155,   155,    75,   155,
      75,    75,    10,    65,    66,   144,   121,   122,    24,    25,
      34,   126,   157,   119,   119,    37,    38,   126,   108,   126,
     108,   126,   155,   155,   155,   105,   157,    14,    14,   149,
      56,    57,    60,   126,   144,   121,   121,   154,   154,    69,
      70,    71,   126,   121,    14,   157,   157,   157,    26,    30,
      36,    39,    40,    43,   126,   126,   126,   126,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     158,   121,   173,   121,   152,   173,   119,   121,   119,   121,
     157,   157,   157,   121,    72,   121,   122,   121,   122,   157,
     173,   157,   157,   121,     5,    14,   119,   157,   119,   122,
     119,   144,   121,   119,   119,   157,   121,   121,    27,   132,
     157,   155,    10,    10,    10,    10,    10,   144,    10,    14,
     157,   157,   126,   157,    26,    29,    30,    36,    39,    40,
      43,    44,   126,   157,   155,   119,   119,    27,   157,   119,
     126,    58,   170,    10,    13,    14,    15,    16,    56,   144,
     144,   121,   121,    14,    14,    14,   105,   120,   124,   126,
     126,   157,   157,   157,   157,   157,   157,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   121,
     121,   121,   119,   119,   119,   119,   157,    14,   173,   119,
     119,   119,    72,    72,   122,   173,   144,   119,   122,   119,
     119,   131,   155,   121,    27,   132,   155,   155,   155,    10,
      65,    66,    26,    30,    43,   126,   126,   126,   157,   157,
     157,   157,   157,   157,   157,   157,   155,   126,   155,   119,
      10,    93,    94,   138,   157,   120,   125,   145,   120,   145,
      14,    22,    63,   105,   119,    14,    22,    63,   105,   119,
      10,    13,    15,    16,   119,   119,   170,   144,   144,     7,
       7,     7,   157,   121,   173,   105,   105,   126,   126,   126,
     126,   126,   126,    72,   121,   157,   157,   173,   119,   173,
     155,   155,    10,    10,   157,   157,   157,   126,   126,   126,
     126,   126,   126,   126,   126,   157,    93,    94,    10,    10,
     121,   121,   154,    67,   129,   121,   154,    14,    14,   119,
     157,    14,    14,   119,   157,    14,    22,    63,   105,   119,
      14,    22,    63,   105,   119,   119,   119,   120,   145,   126,
     126,   121,   157,   157,   157,   119,   121,   119,    27,   132,
     126,   126,   126,   119,   155,   145,   121,   126,    24,    25,
      34,   126,   157,   145,   121,   119,   119,   119,   119,   119,
     119,    14,    14,   119,   157,    14,    14,   119,   157,   121,
     154,   126,   126,   126,   119,   155,   138,   145,   157,   157,
     126,   157,    26,    29,    30,    36,    39,    40,    43,    44,
     126,   145,   119,   119,   119,   119,   119,   119,   145,   121,
     121,    26,    30,    43,   126,   126,   126,   157,   157,   157,
     157,   157,   157,   157,   157,   145,   155,   157,   157,   157,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   127,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   132,   132,   132,   132,   132,   132,   133,   133,
     134,   134,   134,   134,   135,   135,   136,   137,   137,   137,
     138,   138,   138,   138,   138,   139,   139,   139,   139,   139,
     139,   140,   140,   141,   141,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   143,   143,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   146,   147,   148,   148,   148,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   150,   151,   151,   151,   151,   152,   152,   153,
     153,   153,   153,   154,   154,   154,   154,   154,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   156,   157,   157,   157,   158,
     158,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   160,
     160,   160,   161,   161,   162,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   166,   166,   166,   166,   166,   167,
     167,   167,   167,   168,   168,   169,   169,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   171,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   173,   173,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     5,     5,     7,     6,     6,
       8,    10,     6,     4,     4,     4,     3,     5,     5,     6,
       6,     2,     5,     4,     3,     3,     5,     7,     3,     5,
       7,     3,     3,     2,     4,     7,     6,     4,     6,     4,
       2,     4,     4,     4,     4,     4,     4,     4,     4,     7,
       7,     7,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     4,     4,     4,     4,     4,     3,     3,     3,     3,
       2,     1,     1,     1,     1,     1,     2,     4,     2,     1,
       1,     1,     1,     1,     2,     2,     4,     4,     4,     4,
       1,     1,     1,     1,     5,     7,     7,     3,     5,     5,
       4,     6,     3,     5,     5,     7,     4,     6,     5,     3,
       5,     7,     3,     5,     5,     3,     2,     2,    12,     9,
       1,     2,     2,     2,     2,     7,     7,     5,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     3,     7,     6,
       4,     7,     6,     7,     6,     5,     6,     5,     3,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     3,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     6,     5,     0,     3,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     4,     7,     6,     4,     8,
       7,     5,     5,     0,     4,     4,     4,     3,     5,     3,
       3,     5,     5,     1,     2,     3,     3,     3,     4,     5,
       5,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       9,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     4,     3,     6,     6,     5,     3,     3,     6,     5,
       4,     4,     6,     6,     5,     3,     6,     5,     3,     2,
       3,     3,     2,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     5,     5,     1,     1,     1,     1,     2,     3,     4,
       5,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = RAKU_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == RAKU_YYEMPTY)                                        \
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
   Use RAKU_YYerror or RAKU_YYUNDEF. */
#define YYERRCODE RAKU_YYUNDEF


/* Enable debugging if requested.  */
#if RAKU_YYDEBUG

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
#else /* !RAKU_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !RAKU_YYDEBUG */


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

  yychar = RAKU_YYEMPTY; /* Cause a token to be read.  */

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
  if (yychar == RAKU_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= RAKU_YYEOF)
    {
      yychar = RAKU_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == RAKU_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = RAKU_YYUNDEF;
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
  yychar = RAKU_YYEMPTY;
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
  case 2: /* program: stmt_list  */
#line 339 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2718 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 349 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2724 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 350 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2730 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2736 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 356 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2742 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 358 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2748 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 360 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2756 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 364 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2762 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 366 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2768 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 368 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2774 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 370 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2782 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 374 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2789 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 377 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2797 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 381 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2803 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 383 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2809 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 385 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2815 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 387 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2821 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 389 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2827 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 391 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2835 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 395 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2843 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 399 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2849 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 401 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2855 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 403 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2861 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 405 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2867 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 407 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2873 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 409 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2879 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2885 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2891 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 415 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2897 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 417 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2903 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 419 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2909 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 421 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2915 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 423 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2921 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 425 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2927 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 427 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2934 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 430 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2941 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 433 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2947 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 435 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2954 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 438 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2961 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 441 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2967 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 443 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2973 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 445 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2979 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 447 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2985 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 449 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2994 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 454 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3002 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 458 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3010 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 462 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3019 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 467 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3027 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 470 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3033 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 472 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3039 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3045 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 476 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3051 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 478 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3057 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 480 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3064 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 483 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3070 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 485 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3076 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3082 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 489 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3089 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 492 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3096 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 495 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3103 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 498 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3110 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 501 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3117 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3123 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3129 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 508 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3135 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 510 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3141 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 512 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3148 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 515 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3154 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 517 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3160 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 519 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3166 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 521 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3173 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3180 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 527 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3188 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 531 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3195 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 534 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3201 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 536 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3207 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 538 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3213 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 540 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3220 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 543 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3227 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 546 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3234 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 549 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3241 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 552 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3248 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 555 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3255 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 558 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3262 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 561 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3269 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 564 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3276 "raku.tab.c"
    break;

  case 88: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 567 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3283 "raku.tab.c"
    break;

  case 89: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 570 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3290 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 572 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3296 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 573 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3302 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 574 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3308 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 575 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3314 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 576 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3320 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 577 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3326 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3332 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3338 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3344 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 585 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3350 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 586 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3356 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 587 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3362 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 588 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3368 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 589 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3374 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 590 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3380 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 591 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3386 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 593 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3392 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 595 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3398 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 597 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3404 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3410 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 600 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3416 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 601 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3422 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 602 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3428 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 603 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3434 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 607 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3440 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 609 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3446 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 611 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3452 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 613 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3458 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3464 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3470 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3476 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3482 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3488 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3494 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 629 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3500 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3506 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 633 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3512 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3518 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 639 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3524 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 641 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3530 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 645 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3536 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3542 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 649 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3548 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3554 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 655 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3560 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3566 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3572 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP block  */
#line 665 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3578 "raku.tab.c"
    break;

  case 138: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 667 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3584 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 669 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3590 "raku.tab.c"
    break;

  case 140: /* loop_incr: expr  */
#line 672 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3596 "raku.tab.c"
    break;

  case 141: /* loop_incr: VAR_SCALAR OP_INC  */
#line 673 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),1); }
#line 3602 "raku.tab.c"
    break;

  case 142: /* loop_incr: VAR_SCALAR OP_DEC  */
#line 674 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),0); }
#line 3608 "raku.tab.c"
    break;

  case 143: /* loop_incr: OP_INC VAR_SCALAR  */
#line 675 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 3614 "raku.tab.c"
    break;

  case 144: /* loop_incr: OP_DEC VAR_SCALAR  */
#line 676 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 3620 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 680 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3630 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 686 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3640 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 692 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3649 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 697 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3659 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 703 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3668 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr block  */
#line 708 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3675 "raku.tab.c"
    break;

  case 151: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 713 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3687 "raku.tab.c"
    break;

  case 152: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 721 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3700 "raku.tab.c"
    break;

  case 153: /* when_list: %empty  */
#line 731 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3706 "raku.tab.c"
    break;

  case 154: /* when_list: when_list KW_WHEN expr block  */
#line 733 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3714 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 739 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3726 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 747 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT sub_body  */
#line 753 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 759 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3758 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 767 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3768 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 773 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3778 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 779 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3791 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 788 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3802 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 795 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3815 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 804 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3828 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 813 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3839 "raku.tab.c"
    break;

  case 166: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 822 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3850 "raku.tab.c"
    break;

  case 167: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 829 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3859 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list '}'  */
#line 835 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3865 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr '}'  */
#line 837 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3872 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 840 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3879 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 843 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3885 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 845 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3891 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 847 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3897 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 849 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3903 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3909 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 853 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3915 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3921 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 857 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3928 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 860 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3934 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 862 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3940 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 864 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3946 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 866 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3953 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 869 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3960 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 872 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3968 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list '}'  */
#line 877 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3974 "raku.tab.c"
    break;

  case 186: /* method_body: '{' YADA '}'  */
#line 878 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3980 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr '}'  */
#line 880 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3987 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 883 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3994 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 886 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4000 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 888 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4006 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 890 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4012 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 892 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4018 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 894 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4024 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 896 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4030 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 898 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4036 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 900 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4043 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 903 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4049 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 905 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4055 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 907 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4061 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 909 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4068 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 912 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4075 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 915 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4083 "raku.tab.c"
    break;

  case 203: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 921 "raku.y"
        {
            const char *cname = intern((yyvsp[-4].sval)); free((yyvsp[-4].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *cd = ast_node_new(TT_CLASS_DECL);
            if ((yyvsp[-3].sval)) cd->v.sval = (yyvsp[-3].sval);
            ast_push(cd, leaf_sval(TT_VAR, cname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(cd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = cd;
        }
#line 4101 "raku.tab.c"
    break;

  case 204: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 937 "raku.y"
        {
            const char *rname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *rd = ast_node_new(TT_ROLE_DECL);
            ast_push(rd, leaf_sval(TT_VAR, rname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(rd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = rd;
        }
#line 4118 "raku.tab.c"
    break;

  case 205: /* is_clauses: %empty  */
#line 951 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4124 "raku.tab.c"
    break;

  case 206: /* is_clauses: is_clauses IDENT IDENT  */
#line 953 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 4140 "raku.tab.c"
    break;

  case 207: /* is_clauses: is_clauses TESTOP IDENT  */
#line 965 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 4156 "raku.tab.c"
    break;

  case 208: /* class_body_list: %empty  */
#line 978 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4162 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 980 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4169 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 983 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4176 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 986 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4183 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 989 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4190 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 992 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4197 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 995 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4204 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 998 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4211 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1001 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4218 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1004 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4225 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1007 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4232 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1010 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4239 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1013 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4246 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1016 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4257 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1023 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4268 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1030 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4279 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1037 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4290 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1044 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4298 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1048 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4306 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1052 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4314 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1056 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4322 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1060 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4333 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1067 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4344 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1074 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4355 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1081 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4366 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1088 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4380 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1098 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4392 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1106 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4404 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1114 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4417 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1123 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4428 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1130 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4439 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1137 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4453 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1147 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4465 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1155 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4477 "raku.tab.c"
    break;

  case 242: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1165 "raku.y"
        {
            const char *gname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *gd = ast_node_new(TT_GRAMMAR_DECL);
            ast_push(gd, leaf_sval(TT_VAR, gname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(gd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = gd;
        }
#line 4494 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: %empty  */
#line 1179 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4500 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1181 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4509 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1186 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4518 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1191 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4527 "raku.tab.c"
    break;

  case 247: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1198 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4535 "raku.tab.c"
    break;

  case 248: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1202 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4543 "raku.tab.c"
    break;

  case 249: /* pair_list: IDENT OP_FATARROW expr  */
#line 1208 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4549 "raku.tab.c"
    break;

  case 250: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1210 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4555 "raku.tab.c"
    break;

  case 251: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1212 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4561 "raku.tab.c"
    break;

  case 252: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1214 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4567 "raku.tab.c"
    break;

  case 253: /* param_list: VAR_SCALAR  */
#line 1217 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4573 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT VAR_SCALAR  */
#line 1218 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4579 "raku.tab.c"
    break;

  case 255: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1219 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4585 "raku.tab.c"
    break;

  case 256: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1220 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4591 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' VAR_SCALAR  */
#line 1221 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4597 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1222 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4603 "raku.tab.c"
    break;

  case 259: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1223 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4609 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1224 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4615 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list '}'  */
#line 1227 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4621 "raku.tab.c"
    break;

  case 262: /* block: '{' YADA '}'  */
#line 1228 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4627 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr '}'  */
#line 1230 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4633 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1232 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1234 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4645 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1236 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4651 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1238 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4657 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1240 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4664 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1243 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4670 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1245 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4676 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1247 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4682 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1249 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4688 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1251 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4695 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1254 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4702 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1257 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4710 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1261 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4717 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1264 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4724 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1267 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4731 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1270 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4737 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1272 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4745 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1276 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4752 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1279 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4759 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1282 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4766 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1285 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4773 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1288 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4780 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1291 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4786 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1293 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4792 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1295 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4798 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST '}'  */
#line 1297 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4804 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1299 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4810 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1301 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4816 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1303 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4822 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1305 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4828 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1307 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4834 "raku.tab.c"
    break;

  case 295: /* closure: '{' expr '}'  */
#line 1310 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4840 "raku.tab.c"
    break;

  case 296: /* expr: VAR_SCALAR '=' expr  */
#line 1313 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4846 "raku.tab.c"
    break;

  case 297: /* expr: KW_GATHER block  */
#line 1314 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4856 "raku.tab.c"
    break;

  case 298: /* expr: tern_expr  */
#line 1319 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4862 "raku.tab.c"
    break;

  case 299: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1323 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4868 "raku.tab.c"
    break;

  case 300: /* tern_expr: cmp_expr  */
#line 1324 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4874 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1327 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4880 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1328 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4886 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1329 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4892 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1330 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4898 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1331 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4904 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1332 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4910 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1333 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4916 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1334 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4922 "raku.tab.c"
    break;

  case 309: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1335 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4928 "raku.tab.c"
    break;

  case 310: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1336 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4934 "raku.tab.c"
    break;

  case 311: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1337 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4940 "raku.tab.c"
    break;

  case 312: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1338 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4946 "raku.tab.c"
    break;

  case 313: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1339 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4952 "raku.tab.c"
    break;

  case 314: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1340 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4958 "raku.tab.c"
    break;

  case 315: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1342 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4968 "raku.tab.c"
    break;

  case 316: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1348 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4978 "raku.tab.c"
    break;

  case 317: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1354 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4988 "raku.tab.c"
    break;

  case 318: /* cmp_expr: jct_expr  */
#line 1359 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4994 "raku.tab.c"
    break;

  case 319: /* jct_expr: jct_expr '|' range_expr  */
#line 1362 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5000 "raku.tab.c"
    break;

  case 320: /* jct_expr: jct_expr '&' range_expr  */
#line 1363 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5006 "raku.tab.c"
    break;

  case 321: /* jct_expr: dor_expr  */
#line 1364 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5012 "raku.tab.c"
    break;

  case 322: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1368 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5018 "raku.tab.c"
    break;

  case 323: /* dor_expr: range_expr  */
#line 1369 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5024 "raku.tab.c"
    break;

  case 324: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1372 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5030 "raku.tab.c"
    break;

  case 325: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1373 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5036 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr  */
#line 1374 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5042 "raku.tab.c"
    break;

  case 327: /* add_expr: add_expr '~' repl_expr  */
#line 1377 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5048 "raku.tab.c"
    break;

  case 328: /* add_expr: repl_expr  */
#line 1378 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5054 "raku.tab.c"
    break;

  case 329: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1381 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5060 "raku.tab.c"
    break;

  case 330: /* repl_expr: addsub_expr  */
#line 1382 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5066 "raku.tab.c"
    break;

  case 331: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1385 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5072 "raku.tab.c"
    break;

  case 332: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1386 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5078 "raku.tab.c"
    break;

  case 333: /* addsub_expr: mul_expr  */
#line 1387 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5084 "raku.tab.c"
    break;

  case 334: /* mul_expr: mul_expr '*' unary_expr  */
#line 1390 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5090 "raku.tab.c"
    break;

  case 335: /* mul_expr: mul_expr '/' unary_expr  */
#line 1391 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5096 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '%' unary_expr  */
#line 1392 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5102 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1393 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5108 "raku.tab.c"
    break;

  case 338: /* mul_expr: unary_expr  */
#line 1394 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5114 "raku.tab.c"
    break;

  case 339: /* unary_expr: '-' unary_expr  */
#line 1397 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5120 "raku.tab.c"
    break;

  case 340: /* unary_expr: '!' unary_expr  */
#line 1398 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5126 "raku.tab.c"
    break;

  case 341: /* unary_expr: CARET unary_expr  */
#line 1399 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5132 "raku.tab.c"
    break;

  case 342: /* unary_expr: pow_expr  */
#line 1400 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5138 "raku.tab.c"
    break;

  case 343: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1403 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5144 "raku.tab.c"
    break;

  case 344: /* pow_expr: postfix_expr  */
#line 1404 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5150 "raku.tab.c"
    break;

  case 345: /* scalar_list: VAR_SCALAR  */
#line 1407 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5156 "raku.tab.c"
    break;

  case 346: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1408 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5162 "raku.tab.c"
    break;

  case 347: /* meth_name: IDENT  */
#line 1411 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5168 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_SORT  */
#line 1412 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5174 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_MAP  */
#line 1413 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5180 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_GREP  */
#line 1414 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5186 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_SAY  */
#line 1415 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5192 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_PRINT  */
#line 1416 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5198 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_TAKE  */
#line 1417 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5204 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_RETURN  */
#line 1418 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5210 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_EXISTS  */
#line 1419 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5216 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_DELETE  */
#line 1420 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5222 "raku.tab.c"
    break;

  case 357: /* meth_name: TESTOP  */
#line 1421 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5228 "raku.tab.c"
    break;

  case 358: /* postfix_expr: call_expr  */
#line 1423 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5234 "raku.tab.c"
    break;

  case 359: /* call_expr: IDENT '(' arg_list ')'  */
#line 1426 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5243 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '(' ')'  */
#line 1430 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5249 "raku.tab.c"
    break;

  case 361: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1432 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5258 "raku.tab.c"
    break;

  case 362: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1437 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5264 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1439 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5274 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1445 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5284 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1451 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5292 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' KW_NEW  */
#line 1455 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5300 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' IDENT  */
#line 1459 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5309 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1464 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5320 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1471 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5329 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' CARET IDENT  */
#line 1476 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5339 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' CARET IDENT  */
#line 1482 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5349 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1488 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5360 "raku.tab.c"
    break;

  case 373: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1495 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5371 "raku.tab.c"
    break;

  case 374: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1502 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5380 "raku.tab.c"
    break;

  case 375: /* call_expr: atom '.' meth_name  */
#line 1507 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5389 "raku.tab.c"
    break;

  case 376: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1512 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5400 "raku.tab.c"
    break;

  case 377: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1519 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5409 "raku.tab.c"
    break;

  case 378: /* call_expr: call_expr '.' meth_name  */
#line 1524 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5418 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_DIE expr  */
#line 1529 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5424 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_MAP closure expr  */
#line 1531 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5430 "raku.tab.c"
    break;

  case 381: /* call_expr: KW_GREP closure expr  */
#line 1533 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5436 "raku.tab.c"
    break;

  case 382: /* call_expr: KW_SORT expr  */
#line 1535 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5442 "raku.tab.c"
    break;

  case 383: /* call_expr: KW_SORT closure expr  */
#line 1537 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5448 "raku.tab.c"
    break;

  case 384: /* call_expr: atom  */
#line 1538 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5454 "raku.tab.c"
    break;

  case 385: /* arg_list: expr  */
#line 1541 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5460 "raku.tab.c"
    break;

  case 386: /* arg_list: arg_list ',' expr  */
#line 1542 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5466 "raku.tab.c"
    break;

  case 387: /* atom: LIT_INT  */
#line 1545 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5472 "raku.tab.c"
    break;

  case 388: /* atom: LIT_FLOAT  */
#line 1546 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5478 "raku.tab.c"
    break;

  case 389: /* atom: LIT_STR  */
#line 1547 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5484 "raku.tab.c"
    break;

  case 390: /* atom: WORDLIST  */
#line 1549 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5496 "raku.tab.c"
    break;

  case 391: /* atom: LIT_INTERP_STR  */
#line 1556 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5502 "raku.tab.c"
    break;

  case 392: /* atom: VAR_SCALAR  */
#line 1557 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5508 "raku.tab.c"
    break;

  case 393: /* atom: VAR_ARRAY  */
#line 1558 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5514 "raku.tab.c"
    break;

  case 394: /* atom: VAR_HASH  */
#line 1559 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5520 "raku.tab.c"
    break;

  case 395: /* atom: VAR_CAPTURE  */
#line 1561 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5528 "raku.tab.c"
    break;

  case 396: /* atom: VAR_FH  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5536 "raku.tab.c"
    break;

  case 397: /* atom: VAR_NAMED_CAPTURE  */
#line 1569 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5543 "raku.tab.c"
    break;

  case 398: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1572 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5549 "raku.tab.c"
    break;

  case 399: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1574 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5555 "raku.tab.c"
    break;

  case 400: /* atom: VAR_HASH '{' expr '}'  */
#line 1576 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5561 "raku.tab.c"
    break;

  case 401: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1578 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5567 "raku.tab.c"
    break;

  case 402: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1580 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5573 "raku.tab.c"
    break;

  case 403: /* atom: IDENT  */
#line 1581 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5579 "raku.tab.c"
    break;

  case 404: /* atom: VAR_TWIGIL  */
#line 1583 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5587 "raku.tab.c"
    break;

  case 405: /* atom: VAR_ARRAY_TWIGIL  */
#line 1587 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5595 "raku.tab.c"
    break;

  case 406: /* atom: VAR_HASH_TWIGIL  */
#line 1591 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5603 "raku.tab.c"
    break;

  case 407: /* atom: '(' ')'  */
#line 1594 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5609 "raku.tab.c"
    break;

  case 408: /* atom: '(' expr ')'  */
#line 1595 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5615 "raku.tab.c"
    break;

  case 409: /* atom: '(' expr ',' ')'  */
#line 1597 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5621 "raku.tab.c"
    break;

  case 410: /* atom: '(' expr ',' arg_list ')'  */
#line 1599 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5628 "raku.tab.c"
    break;

  case 411: /* atom: block  */
#line 1601 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5634 "raku.tab.c"
    break;

  case 412: /* atom: KW_SUB block  */
#line 1602 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5640 "raku.tab.c"
    break;


#line 5644 "raku.tab.c"

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
  yytoken = yychar == RAKU_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= RAKU_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == RAKU_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = RAKU_YYEMPTY;
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
  if (yychar != RAKU_YYEMPTY)
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

#line 1604 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
