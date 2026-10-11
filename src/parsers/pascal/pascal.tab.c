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
#line 28 "pascal.y"

#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
extern void pascal_pragma_flush(tree_t *root);
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
static tree_t *pt_new(tree_e k) { return ast_node_new(k); }
static tree_t *pt_add(tree_t *p, tree_t *c) { if (p && c) ast_push(p, c); return p; }
static tree_t *pt_cat(tree_t *p, tree_t *l) { if (p && l) for (int i = 0; i < l->n; i++) ast_push(p, l->c[i]); return p; }
static tree_t *pt_retag(tree_t *n, tree_e k) { if (n) n->t = k; return n; }
static tree_t *pt_sval(tree_t *n, const char *s) { if (n) n->v.sval = ct_strdup(s); return n; }
static tree_t *pt_kw(const char *w) { return pt_sval(pt_new(TT_KEYWORD), w); }
static tree_t *pt_part(const char *w, tree_t *first) { return pt_add(pt_sval(pt_new(TT_PART), w), first); }
static tree_t *pt_decl(const char *w, tree_t *a, tree_t *b) { return pt_add(pt_add(pt_sval(pt_new(TT_DECL), w), a), b); }
static void pt_flush_pragmas(tree_t *root) { pascal_pragma_flush(root); }
typedef struct { char *name; int kind; long long iv; double dv; char *sv; } PasCEnvE;
typedef struct { char *name; int on; int has; double val; } PasDefE;
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; int unit_mode; int dialect; PasDefE *defs; int ndefs; int capdefs; int cond_skip; int cond_depth; int cond_taken; int cond_unknown; int macro_on; int string_declared; const char *mode_macro; unsigned sw_set; unsigned sw_on; unsigned *sw_save; int sw_n; int sw_cap; } g_pascal_cenv;
static void cenv_put(const char *name, int kind, long long iv, double dv, const char *sv) {
    if (!name || (kind == 0 && g_pascal_cenv.ni >= 256) || (kind == 1 && g_pascal_cenv.nr >= 64) || (kind == 2 && g_pascal_cenv.ns >= 64)) return;
    if (g_pascal_cenv.n >= g_pascal_cenv.cap) { g_pascal_cenv.cap = g_pascal_cenv.cap ? g_pascal_cenv.cap * 2 : 64; g_pascal_cenv.e = (PasCEnvE *)ct_grow(g_pascal_cenv.e, (size_t)g_pascal_cenv.cap * sizeof(PasCEnvE)); }
    PasCEnvE *c = &g_pascal_cenv.e[g_pascal_cenv.n++]; c->name = ct_strdup(name); c->kind = kind; c->iv = iv; c->dv = dv; c->sv = sv ? ct_strdup(sv) : NULL;
    if (kind == 0) g_pascal_cenv.ni++; else if (kind == 1) g_pascal_cenv.nr++; else g_pascal_cenv.ns++;
}
static void cenv_int(const char *name, long long v) { cenv_put(name, 0, v, 0, NULL); }
static void cenv_real(const char *name, double v) { cenv_put(name, 1, 0, v, NULL); }
static void cenv_string(const char *name, const char *s) { if (s && strlen(s) == 1) cenv_int(name, (long long)(unsigned char)s[0]); else if (s) cenv_put(name, 2, 0, 0, s); }
int pascal_cenv_int(const char *name, long long *out) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 0 && !strcmp(g_pascal_cenv.e[i].name, name)) { *out = g_pascal_cenv.e[i].iv; return 1; } return 0; }
int pascal_cenv_real(const char *name, double *out) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 1 && !strcmp(g_pascal_cenv.e[i].name, name)) { *out = g_pascal_cenv.e[i].dv; return 1; } return 0; }
const char *pascal_cenv_str(const char *name) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 2 && !strcmp(g_pascal_cenv.e[i].name, name)) return g_pascal_cenv.e[i].sv; return NULL; }
void pascal_cenv_reset(void) { g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; g_pascal_cenv.unit_mode = 0; g_pascal_cenv.dialect = PAS_DIALECT_ISO_DEFAULT; g_pascal_cenv.ndefs = 0; g_pascal_cenv.cond_skip = 0; g_pascal_cenv.cond_depth = 0; g_pascal_cenv.cond_taken = 0; g_pascal_cenv.cond_unknown = 0; g_pascal_cenv.macro_on = 0; g_pascal_cenv.string_declared = 0; g_pascal_cenv.mode_macro = NULL; g_pascal_cenv.sw_set = 0; g_pascal_cenv.sw_on = 0; g_pascal_cenv.sw_n = 0; }
int pascal_unit_mode(void) { return g_pascal_cenv.unit_mode; }
void pascal_unit_mode_set(int m) { g_pascal_cenv.unit_mode = m; }
int pascal_dialect(void) { return g_pascal_cenv.dialect; }
void pascal_dialect_set(int d) { g_pascal_cenv.dialect = d; }
int pascal_def_state(const char *n) {
    for (int i = 0; n && i < g_pascal_cenv.ndefs; i++) if (!strcasecmp(g_pascal_cenv.defs[i].name, n)) return g_pascal_cenv.defs[i].on;
    return -1;
}
void pascal_def_set(const char *n, int on) {
    if (!n || !n[0]) return;
    for (int i = 0; i < g_pascal_cenv.ndefs; i++) if (!strcasecmp(g_pascal_cenv.defs[i].name, n)) { g_pascal_cenv.defs[i].on = on; return; }
    if (g_pascal_cenv.ndefs >= g_pascal_cenv.capdefs) { g_pascal_cenv.capdefs = g_pascal_cenv.capdefs ? g_pascal_cenv.capdefs * 2 : 16; g_pascal_cenv.defs = (PasDefE *)ct_grow(g_pascal_cenv.defs, (size_t)g_pascal_cenv.capdefs * sizeof(PasDefE)); }
    g_pascal_cenv.defs[g_pascal_cenv.ndefs].name = ct_strdup(n);
    g_pascal_cenv.defs[g_pascal_cenv.ndefs].has = 0;
    g_pascal_cenv.defs[g_pascal_cenv.ndefs++].on = on;
}
int pascal_def_value(const char *n, double *v) {
    for (int i = 0; n && i < g_pascal_cenv.ndefs; i++) if (!strcasecmp(g_pascal_cenv.defs[i].name, n)) { if (!g_pascal_cenv.defs[i].on || !g_pascal_cenv.defs[i].has) return 0; *v = g_pascal_cenv.defs[i].val; return 1; }
    return 0;
}
void pascal_def_setval(const char *n, double v) {
    pascal_def_set(n, 1);
    for (int i = 0; i < g_pascal_cenv.ndefs; i++) if (!strcasecmp(g_pascal_cenv.defs[i].name, n)) { g_pascal_cenv.defs[i].has = 1; g_pascal_cenv.defs[i].val = v; }
}
int *pascal_cond_unknown_p(void) { return &g_pascal_cenv.cond_unknown; }
int *pascal_macro_on_p(void) { return &g_pascal_cenv.macro_on; }
void pascal_note_type_ident(tree_t *id) { if (id && id->v.sval && !strcasecmp(id->v.sval, "string") && !g_pascal_cenv.string_declared) pascal_flip_dialect(PAS_DIALECT_FPC); }
void pascal_note_type_decl(tree_t *id) { if (id && id->v.sval && !strcasecmp(id->v.sval, "string")) g_pascal_cenv.string_declared = 1; }
unsigned *pascal_sw_set_p(void) { return &g_pascal_cenv.sw_set; }
unsigned *pascal_sw_on_p(void) { return &g_pascal_cenv.sw_on; }
void pascal_sw_push(void) {
    if (g_pascal_cenv.sw_n + 2 > g_pascal_cenv.sw_cap) { g_pascal_cenv.sw_cap = g_pascal_cenv.sw_cap ? g_pascal_cenv.sw_cap * 2 : 16; g_pascal_cenv.sw_save = (unsigned *)ct_grow(g_pascal_cenv.sw_save, (size_t)g_pascal_cenv.sw_cap * sizeof(unsigned)); }
    g_pascal_cenv.sw_save[g_pascal_cenv.sw_n++] = g_pascal_cenv.sw_set;
    g_pascal_cenv.sw_save[g_pascal_cenv.sw_n++] = g_pascal_cenv.sw_on;
}
void pascal_sw_pop(void) {
    if (g_pascal_cenv.sw_n < 2) return;
    g_pascal_cenv.sw_on = g_pascal_cenv.sw_save[--g_pascal_cenv.sw_n];
    g_pascal_cenv.sw_set = g_pascal_cenv.sw_save[--g_pascal_cenv.sw_n];
}
int *pascal_cond_skip_p(void) { return &g_pascal_cenv.cond_skip; }
int *pascal_cond_depth_p(void) { return &g_pascal_cenv.cond_depth; }
int *pascal_cond_taken_p(void) { return &g_pascal_cenv.cond_taken; }
const char *pascal_mode_macro(void) { return g_pascal_cenv.mode_macro; }
void pascal_mode_macro_set(const char *m) { g_pascal_cenv.mode_macro = m; }
static long long cenv_value(tree_t *n) {
    long long cv = 0;
    if (!n) return 0;
    switch (n->t) {
        case TT_PLS: return cenv_value(n->c[0]);
        case TT_MNS: return -cenv_value(n->c[0]);
        case TT_ILIT: case TT_CHRLIT: return n->v.ival;
        case TT_FLIT: return (long long)n->v.dval;
        case TT_QLIT: return (n->v.sval && strlen(n->v.sval) == 1) ? (long long)(unsigned char)n->v.sval[0] : 0;
        case TT_VAR:
            if (n->v.sval && !strcmp(n->v.sval, "true")) cv = 1;
            else if (n->v.sval && !strcmp(n->v.sval, "false")) cv = 0;
            else if (!pascal_cenv_int(n->v.sval, &cv) && n->v.sval && !strcmp(n->v.sval, "maxint")) cv = 2147483647;
            return cv;
        default: return 0;
    }
}
static void cenv_note(const char *name, tree_t *e) {
    if (e && e->t == TT_FLIT) cenv_real(name, e->v.dval);
    else if (e && (e->t == TT_PLS || e->t == TT_MNS) && e->n == 1 && e->c[0] && e->c[0]->t == TT_FLIT) cenv_real(name, e->t == TT_MNS ? -e->c[0]->v.dval : e->c[0]->v.dval);
    else if (e && e->t == TT_QLIT) cenv_string(name, e->v.sval);
    else cenv_int(name, cenv_value(e));
}

#line 197 "pascal.tab.c"

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
  YYSYMBOL_USESSY = 10,                    /* USESSY  */
  YYSYMBOL_UNITSY = 11,                    /* UNITSY  */
  YYSYMBOL_INTERFACESY = 12,               /* INTERFACESY  */
  YYSYMBOL_IMPLEMENTATIONSY = 13,          /* IMPLEMENTATIONSY  */
  YYSYMBOL_INITIALIZATIONSY = 14,          /* INITIALIZATIONSY  */
  YYSYMBOL_FINALIZATIONSY = 15,            /* FINALIZATIONSY  */
  YYSYMBOL_OTHERWISESY = 16,               /* OTHERWISESY  */
  YYSYMBOL_DOSY = 17,                      /* DOSY  */
  YYSYMBOL_DOWNTOSY = 18,                  /* DOWNTOSY  */
  YYSYMBOL_FORSY = 19,                     /* FORSY  */
  YYSYMBOL_REPEATSY = 20,                  /* REPEATSY  */
  YYSYMBOL_WHILESY = 21,                   /* WHILESY  */
  YYSYMBOL_TOSY = 22,                      /* TOSY  */
  YYSYMBOL_UNTILSY = 23,                   /* UNTILSY  */
  YYSYMBOL_WITHSY = 24,                    /* WITHSY  */
  YYSYMBOL_CASESY = 25,                    /* CASESY  */
  YYSYMBOL_PROCEDURESY = 26,               /* PROCEDURESY  */
  YYSYMBOL_PACKEDSY = 27,                  /* PACKEDSY  */
  YYSYMBOL_OFSY = 28,                      /* OFSY  */
  YYSYMBOL_FILESY = 29,                    /* FILESY  */
  YYSYMBOL_ENDSY = 30,                     /* ENDSY  */
  YYSYMBOL_SETSY = 31,                     /* SETSY  */
  YYSYMBOL_VARSY = 32,                     /* VARSY  */
  YYSYMBOL_THENSY = 33,                    /* THENSY  */
  YYSYMBOL_RECORDSY = 34,                  /* RECORDSY  */
  YYSYMBOL_FUNCTIONSY = 35,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 36,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 37,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 38,                    /* TYPESY  */
  YYSYMBOL_IFSY = 39,                      /* IFSY  */
  YYSYMBOL_ELSESY = 40,                    /* ELSESY  */
  YYSYMBOL_INOP = 41,                      /* INOP  */
  YYSYMBOL_NOTSY = 42,                     /* NOTSY  */
  YYSYMBOL_IDIV = 43,                      /* IDIV  */
  YYSYMBOL_IMOD = 44,                      /* IMOD  */
  YYSYMBOL_ANDOP = 45,                     /* ANDOP  */
  YYSYMBOL_OROP = 46,                      /* OROP  */
  YYSYMBOL_LTOP = 47,                      /* LTOP  */
  YYSYMBOL_LEOP = 48,                      /* LEOP  */
  YYSYMBOL_GTOP = 49,                      /* GTOP  */
  YYSYMBOL_GEOP = 50,                      /* GEOP  */
  YYSYMBOL_NEOP = 51,                      /* NEOP  */
  YYSYMBOL_EQOP = 52,                      /* EQOP  */
  YYSYMBOL_PLUS = 53,                      /* PLUS  */
  YYSYMBOL_MINUS = 54,                     /* MINUS  */
  YYSYMBOL_MUL = 55,                       /* MUL  */
  YYSYMBOL_RDIV = 56,                      /* RDIV  */
  YYSYMBOL_COMMA = 57,                     /* COMMA  */
  YYSYMBOL_PERIOD = 58,                    /* PERIOD  */
  YYSYMBOL_COLON = 59,                     /* COLON  */
  YYSYMBOL_ARROW = 60,                     /* ARROW  */
  YYSYMBOL_LBRACK = 61,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 62,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 63,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 64,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 65,                    /* DOTDOT  */
  YYSYMBOL_ATSIGN = 66,                    /* ATSIGN  */
  YYSYMBOL_ADDBECOMES = 67,                /* ADDBECOMES  */
  YYSYMBOL_SUBBECOMES = 68,                /* SUBBECOMES  */
  YYSYMBOL_MULBECOMES = 69,                /* MULBECOMES  */
  YYSYMBOL_DIVBECOMES = 70,                /* DIVBECOMES  */
  YYSYMBOL_INTCONST = 71,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 72,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 73,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 74,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 75,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 76,                  /* $accept  */
  YYSYMBOL_compilation_unit = 77,          /* compilation_unit  */
  YYSYMBOL_program = 78,                   /* program  */
  YYSYMBOL_file_id_list_opt = 79,          /* file_id_list_opt  */
  YYSYMBOL_block = 80,                     /* block  */
  YYSYMBOL_decl_part_list = 81,            /* decl_part_list  */
  YYSYMBOL_decl_part = 82,                 /* decl_part  */
  YYSYMBOL_unit = 83,                      /* unit  */
  YYSYMBOL_intf_part_list = 84,            /* intf_part_list  */
  YYSYMBOL_intf_part = 85,                 /* intf_part  */
  YYSYMBOL_init_opt = 86,                  /* init_opt  */
  YYSYMBOL_fini_opt = 87,                  /* fini_opt  */
  YYSYMBOL_label_list = 88,                /* label_list  */
  YYSYMBOL_const_decl_list = 89,           /* const_decl_list  */
  YYSYMBOL_const_decl = 90,                /* const_decl  */
  YYSYMBOL_typed_value = 91,               /* typed_value  */
  YYSYMBOL_typed_value_list = 92,          /* typed_value_list  */
  YYSYMBOL_field_value_list = 93,          /* field_value_list  */
  YYSYMBOL_constant = 94,                  /* constant  */
  YYSYMBOL_scalar_constant = 95,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 96,            /* type_decl_list  */
  YYSYMBOL_type_decl = 97,                 /* type_decl  */
  YYSYMBOL_type = 98,                      /* type  */
  YYSYMBOL_99_1 = 99,                      /* $@1  */
  YYSYMBOL_100_2 = 100,                    /* $@2  */
  YYSYMBOL_101_3 = 101,                    /* $@3  */
  YYSYMBOL_packed_opt = 102,               /* packed_opt  */
  YYSYMBOL_simple_type = 103,              /* simple_type  */
  YYSYMBOL_record_body = 104,              /* record_body  */
  YYSYMBOL_record_field_list = 105,        /* record_field_list  */
  YYSYMBOL_record_field = 106,             /* record_field  */
  YYSYMBOL_107_4 = 107,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 108,            /* case_arm_mark  */
  YYSYMBOL_case_open = 109,                /* case_open  */
  YYSYMBOL_record_case_opt = 110,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 111,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 112,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 113,            /* var_decl_list  */
  YYSYMBOL_var_decl = 114,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 115,           /* procedure_decl  */
  YYSYMBOL_116_5 = 116,                    /* $@5  */
  YYSYMBOL_117_6 = 117,                    /* $@6  */
  YYSYMBOL_118_7 = 118,                    /* $@7  */
  YYSYMBOL_pv_mark = 119,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 120,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 121,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 122,           /* parameter_decl  */
  YYSYMBOL_param_type = 123,               /* param_type  */
  YYSYMBOL_pf_params = 124,                /* pf_params  */
  YYSYMBOL_pf_sections = 125,              /* pf_sections  */
  YYSYMBOL_pf_section = 126,               /* pf_section  */
  YYSYMBOL_id_list = 127,                  /* id_list  */
  YYSYMBOL_body = 128,                     /* body  */
  YYSYMBOL_statement_list = 129,           /* statement_list  */
  YYSYMBOL_130_8 = 130,                    /* $@8  */
  YYSYMBOL_131_9 = 131,                    /* $@9  */
  YYSYMBOL_body_stmt = 132,                /* body_stmt  */
  YYSYMBOL_133_10 = 133,                   /* $@10  */
  YYSYMBOL_statement = 134,                /* statement  */
  YYSYMBOL_statement_no_label = 135,       /* statement_no_label  */
  YYSYMBOL_call = 136,                     /* call  */
  YYSYMBOL_call_with_args = 137,           /* call_with_args  */
  YYSYMBOL_argument_list = 138,            /* argument_list  */
  YYSYMBOL_argument = 139,                 /* argument  */
  YYSYMBOL_assignment = 140,               /* assignment  */
  YYSYMBOL_selector = 141,                 /* selector  */
  YYSYMBOL_expression_list = 142,          /* expression_list  */
  YYSYMBOL_compound_statement = 143,       /* compound_statement  */
  YYSYMBOL_goto_statement = 144,           /* goto_statement  */
  YYSYMBOL_if_statement = 145,             /* if_statement  */
  YYSYMBOL_case_statement = 146,           /* case_statement  */
  YYSYMBOL_147_11 = 147,                   /* $@11  */
  YYSYMBOL_case_tail = 148,                /* case_tail  */
  YYSYMBOL_case_list = 149,                /* case_list  */
  YYSYMBOL_case_elem = 150,                /* case_elem  */
  YYSYMBOL_constant_list = 151,            /* constant_list  */
  YYSYMBOL_while_statement = 152,          /* while_statement  */
  YYSYMBOL_repeat_statement = 153,         /* repeat_statement  */
  YYSYMBOL_for_statement = 154,            /* for_statement  */
  YYSYMBOL_with_statement = 155,           /* with_statement  */
  YYSYMBOL_with_open = 156,                /* with_open  */
  YYSYMBOL_expression = 157,               /* expression  */
  YYSYMBOL_simple_expression = 158,        /* simple_expression  */
  YYSYMBOL_term = 159,                     /* term  */
  YYSYMBOL_factor = 160,                   /* factor  */
  YYSYMBOL_set_member_list = 161,          /* set_member_list  */
  YYSYMBOL_set_member = 162                /* set_member  */
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
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   634

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  87
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  474

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   330


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   167,   167,   168,   171,   174,   175,   178,   181,   182,
     185,   186,   187,   188,   189,   190,   193,   196,   197,   200,
     201,   202,   203,   204,   205,   208,   209,   210,   213,   214,
     217,   218,   221,   222,   225,   226,   229,   230,   231,   232,
     235,   236,   239,   240,   243,   244,   245,   248,   249,   250,
     251,   252,   255,   256,   259,   262,   263,   264,   264,   264,
     265,   266,   267,   267,   268,   269,   270,   273,   274,   277,
     278,   279,   280,   281,   282,   285,   288,   289,   292,   292,
     293,   296,   299,   302,   303,   304,   307,   308,   311,   312,
     315,   316,   319,   320,   323,   324,   325,   325,   326,   326,
     327,   327,   330,   333,   334,   337,   338,   341,   342,   343,
     344,   345,   346,   349,   350,   353,   354,   357,   358,   361,
     362,   363,   364,   365,   368,   369,   372,   375,   375,   376,
     376,   379,   379,   382,   383,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   399,   400,   401,   404,
     405,   406,   409,   410,   413,   414,   415,   418,   419,   420,
     421,   422,   425,   426,   427,   428,   431,   432,   435,   438,
     441,   442,   445,   445,   448,   449,   450,   453,   454,   457,
     458,   461,   462,   465,   468,   471,   472,   475,   478,   479,
     486,   487,   488,   489,   490,   491,   492,   493,   496,   497,
     498,   499,   500,   501,   504,   505,   506,   507,   508,   509,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   525,   526,   529,   530
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
  "SEMICOLON", "ARRAYSY", "LABELSY", "CONSTSY", "FORWARDSY", "USESSY",
  "UNITSY", "INTERFACESY", "IMPLEMENTATIONSY", "INITIALIZATIONSY",
  "FINALIZATIONSY", "OTHERWISESY", "DOSY", "DOWNTOSY", "FORSY", "REPEATSY",
  "WHILESY", "TOSY", "UNTILSY", "WITHSY", "CASESY", "PROCEDURESY",
  "PACKEDSY", "OFSY", "FILESY", "ENDSY", "SETSY", "VARSY", "THENSY",
  "RECORDSY", "FUNCTIONSY", "BEGINSY", "BECOMES", "TYPESY", "IFSY",
  "ELSESY", "INOP", "NOTSY", "IDIV", "IMOD", "ANDOP", "OROP", "LTOP",
  "LEOP", "GTOP", "GEOP", "NEOP", "EQOP", "PLUS", "MINUS", "MUL", "RDIV",
  "COMMA", "PERIOD", "COLON", "ARROW", "LBRACK", "RBRACK", "LPARENT",
  "RPARENT", "DOTDOT", "ATSIGN", "ADDBECOMES", "SUBBECOMES", "MULBECOMES",
  "DIVBECOMES", "INTCONST", "CHARCODE", "REALCONST", "STRINGCONST",
  "IDENT", "$accept", "compilation_unit", "program", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "unit", "intf_part_list",
  "intf_part", "init_opt", "fini_opt", "label_list", "const_decl_list",
  "const_decl", "typed_value", "typed_value_list", "field_value_list",
  "constant", "scalar_constant", "type_decl_list", "type_decl", "type",
  "$@1", "$@2", "$@3", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "$@4", "case_arm_mark", "case_open",
  "record_case_opt", "record_case_list", "record_case_arm",
  "var_decl_list", "var_decl", "procedure_decl", "$@5", "$@6", "$@7",
  "pv_mark", "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "param_type", "pf_params", "pf_sections", "pf_section", "id_list",
  "body", "statement_list", "$@8", "$@9", "body_stmt", "$@10", "statement",
  "statement_no_label", "call", "call_with_args", "argument_list",
  "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@11", "case_tail", "case_list", "case_elem", "constant_list",
  "while_statement", "repeat_statement", "for_statement", "with_statement",
  "with_open", "expression", "simple_expression", "term", "factor",
  "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-307)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-148)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     150,   -20,    -8,    93,  -307,  -307,    82,   162,  -307,    78,
     175,   177,  -307,   122,  -307,  -307,   142,  -307,   163,   324,
     481,  -307,  -307,   156,   159,    78,   181,    78,   184,  -307,
     185,  -307,  -307,  -307,   159,    78,  -307,   193,    78,   205,
     185,  -307,  -307,    28,   129,   159,  -307,    33,  -307,    78,
    -307,   148,  -307,    51,    39,   190,   185,  -307,   159,    42,
     496,  -307,    78,  -307,   185,  -307,   223,   355,   172,  -307,
    -307,   257,  -307,   172,    32,  -307,  -307,   265,   246,  -307,
     355,   286,   355,  -307,   355,   292,   160,  -307,  -307,  -307,
    -307,  -307,   432,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,   172,  -307,  -307,  -307,  -307,   343,   257,   257,  -307,
     413,   413,   413,   311,   355,   413,  -307,  -307,  -307,  -307,
     303,  -307,   224,   189,   131,    96,  -307,  -307,   233,   233,
     296,    78,  -307,   322,  -307,   323,   117,   325,  -307,   337,
      48,  -307,    14,   370,    56,  -307,   332,    39,  -307,   357,
      60,   250,  -307,   235,    31,   489,   125,   543,   123,   339,
     355,   320,  -307,   355,   355,   355,   355,   355,   391,   394,
     394,  -307,   368,   396,   345,  -307,    96,    96,  -307,   501,
     213,  -307,   513,  -307,   331,  -307,   355,   355,   355,   355,
     355,   355,   355,   413,   413,   413,   413,   413,   413,   413,
     413,  -307,  -307,  -307,  -307,  -307,  -307,   145,   514,   514,
     514,   514,   378,    25,   387,   389,  -307,    78,   344,    78,
     347,    78,    20,  -307,   217,   414,  -307,   355,  -307,   349,
    -307,   355,   355,  -307,   350,  -307,   286,  -307,  -307,  -307,
    -307,  -307,   149,  -307,   555,   531,   188,   215,   531,   531,
     531,   531,   531,  -307,   394,   375,  -307,   359,   355,   355,
    -307,  -307,   373,   131,   131,   131,   131,   131,   131,   131,
      96,    96,    96,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,   376,  -307,   393,   435,   531,   172,   472,   172,   472,
      78,   229,   374,   271,   374,   276,    14,  -307,    12,   437,
    -307,   214,   438,   440,   297,   531,  -307,    39,  -307,  -307,
     235,   514,   408,   355,  -307,   355,   355,   355,  -307,  -307,
     452,   531,  -307,  -307,   170,   401,    64,   513,  -307,  -307,
     221,  -307,  -307,   430,    47,  -307,   283,    12,    36,  -307,
      12,   402,    12,  -307,   443,  -307,  -307,  -307,   458,  -307,
    -307,   461,   355,   355,  -307,  -307,   133,  -307,   284,  -307,
    -307,   568,   158,   531,  -307,   378,   378,    71,  -307,   472,
     444,  -307,    78,   398,  -307,  -307,  -307,    78,   400,    78,
     403,    68,  -307,   295,  -307,   405,  -307,   406,  -307,   477,
    -307,   262,   275,   514,  -307,  -307,   447,   514,  -307,  -307,
     355,  -307,  -307,  -307,   167,  -307,   424,   433,  -307,  -307,
      17,   172,   298,   374,   310,   374,    36,  -307,    12,  -307,
    -307,  -307,   491,  -307,  -307,  -307,   394,   394,  -307,  -307,
    -307,   531,   378,  -307,   378,   469,  -307,  -307,   423,  -307,
      12,  -307,    12,   446,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,   172,   172,   514,   480,  -307,  -307,   434,  -307,  -307,
     506,  -307,   319,  -307,  -307,   514,   449,   514,  -307,  -307,
     506,    78,   450,  -307
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     6,     0,     1,     0,
       0,     0,   125,     0,     9,    18,     0,     5,     0,     0,
       0,   124,     4,     0,     0,     0,     0,     0,     0,   129,
       0,     8,    14,     7,     0,     0,     9,     0,     0,     0,
       0,    17,    31,     0,     0,    11,    33,     0,   102,    13,
      91,     0,   102,     0,   145,     0,    12,    53,    20,     0,
      27,   102,    22,   102,    21,    10,     0,     0,    68,    32,
      15,   104,    90,    68,   104,   127,   126,     0,     0,   129,
       0,     0,     0,   129,     0,     0,   165,   130,   133,   136,
     148,   135,     0,   137,   138,   139,   140,   141,   142,   143,
     144,    68,    52,    19,   129,   129,    29,   104,   104,    30,
       0,     0,     0,     0,     0,     0,   212,   215,   213,   214,
     165,   211,   210,     0,   190,   198,   204,    67,     0,     0,
       0,     0,    48,     0,    49,     0,    70,     0,    44,     0,
       0,    55,     0,     0,     0,   100,     0,   145,   169,     0,
       0,     0,   165,   189,     0,     0,     0,     0,   145,     0,
       0,     0,   164,     0,     0,     0,     0,     0,     0,    25,
      26,   129,     0,     0,     0,   217,   199,   200,   219,   223,
       0,   221,     0,   218,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    50,    47,    45,    46,    56,     0,     0,     0,
       0,     0,     0,     0,    65,     0,    62,     0,     0,     0,
       0,   125,     0,   106,     0,    96,    92,     0,     9,     0,
     128,     0,     0,   131,     0,   131,     0,   172,   168,   131,
     134,   150,     0,   153,   154,   157,   163,     0,   167,   158,
     159,   160,   161,    54,    28,     0,    23,     0,     0,     0,
     220,   216,   163,   191,   192,   193,   194,   195,   196,   197,
     203,   201,   202,   207,   208,   209,   205,   206,    69,    74,
      73,     0,    72,     0,     0,    36,    68,     0,    68,     0,
      80,     0,   116,     0,   116,     0,     0,   103,     0,     0,
       9,     0,     0,     0,     0,   184,   183,   145,   163,   187,
     188,   180,   170,     0,   149,     0,     0,     0,   162,    16,
       0,   224,   222,    71,   165,     0,     0,    36,    35,    60,
       0,    66,    64,     0,    85,    77,     0,     0,     0,   107,
       0,     0,     0,   105,     0,   113,   110,    94,     0,    93,
     101,    98,     0,     0,   132,   182,   174,   178,     0,   131,
     152,   155,     0,   166,    24,     0,     0,     0,    38,     0,
       0,    63,    80,     0,    75,    78,   111,     0,     0,     0,
       0,     0,   118,     0,   109,     0,   112,     0,    97,     0,
       9,     0,     0,   180,   129,   129,     0,     0,   131,   171,
       0,   151,    42,    40,     0,    39,     0,     0,    57,    76,
       0,    68,     0,   116,     0,   116,     0,   115,     0,   108,
     114,    95,     0,   131,   131,   177,   176,   175,   173,   181,
     179,   156,     0,    37,     0,     0,    58,    82,     0,    79,
       0,   122,     0,     0,   117,   119,    99,   186,   185,    41,
      43,    68,    68,    89,     0,   121,   120,     0,    61,    59,
      84,    87,     0,    82,   123,    89,     0,    89,    86,    81,
      83,    80,     0,    88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -307,  -307,  -307,  -307,  -220,   479,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,   484,    92,  -274,  -307,  -307,  -196,   120,
     483,    10,   -72,  -307,  -307,  -307,  -307,  -248,    50,  -307,
     152,  -307,  -307,    57,  -307,    62,    90,   495,     8,  -307,
    -307,  -307,  -307,   111,    49,  -307,   231,  -306,  -284,  -307,
     140,    -9,  -307,   -77,  -307,  -307,  -232,  -307,  -136,   399,
    -307,   -34,   242,   256,  -307,   -49,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,   166,  -268,  -307,  -307,  -307,  -307,
    -307,   -63,   442,   -88,   -80,  -307,   312
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    10,    18,    19,    31,     5,    20,    41,
     106,   172,    43,    45,    46,   284,   404,   326,   137,   138,
      56,    57,   139,   436,   452,   290,   140,   141,   333,   334,
     335,   411,   471,   453,   374,   460,   461,    49,    50,    32,
     300,   390,   228,    71,   143,   222,   223,   346,   339,   381,
     382,    51,    33,    53,   147,    54,   306,   307,    87,    88,
      89,   121,   242,   243,    91,   122,   247,    93,    94,    95,
      96,   311,   396,   356,   357,   462,    97,    98,    99,   100,
     154,   285,   124,   125,   126,   180,   181
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   144,   150,   309,   123,    92,   156,   312,   302,   325,
     341,   230,   279,   280,   281,   282,    47,   151,   344,   155,
      90,   157,   217,   176,   177,   296,    59,   169,   170,   168,
     175,   376,   153,    65,   384,   183,   386,   145,    70,   330,
     218,   332,    77,   358,   377,   437,   219,   103,   235,   220,
     179,   182,   372,   286,   213,     6,    75,    72,    78,    79,
      80,   226,   378,    81,    82,    75,   102,     7,   379,   367,
      72,   380,   373,   416,   102,    83,   438,   214,    84,   215,
     348,    76,   216,   232,   297,    66,   287,   345,   236,   221,
      16,   402,   403,     8,   254,   142,   244,   245,    92,    16,
     248,   249,   250,   251,   252,   270,   271,   272,   227,    92,
      85,    12,   445,    90,    86,   355,   273,   274,   275,   276,
     277,   407,   207,   146,    90,   358,    77,   399,   368,   441,
      75,   443,   417,   224,   455,   405,   456,    69,   393,   196,
     197,   198,    78,    79,    80,     9,   406,    81,    82,   394,
      69,   199,   200,    12,     1,   238,   173,   174,   449,    83,
     450,     2,    84,    74,   301,  -146,   430,    11,   304,   305,
     422,   354,   107,   395,   108,  -146,  -146,   193,   210,    16,
      14,    67,   -47,  -146,   194,   195,    17,   310,    68,    15,
    -146,   447,   448,  -147,   185,   321,   179,   355,    86,   127,
    -146,   429,    16,  -147,  -147,    16,   313,    73,   291,   278,
     293,  -147,   295,   314,   329,   313,   331,    21,  -147,   349,
     327,    22,   401,   159,   432,   128,   129,    42,  -147,   365,
     186,   433,   130,   159,    44,   131,   187,   188,   189,   190,
     191,   192,   101,   132,   133,   134,   135,   136,   204,   205,
     244,   316,   361,   244,   363,   186,    48,   355,    92,    52,
      55,   187,   188,   189,   190,   191,   192,   233,    61,   355,
     259,   355,   317,    90,    16,   260,   298,   318,   369,   423,
      63,   336,   184,   370,   162,   163,    16,   224,   337,   391,
     392,   186,   424,   234,   109,   162,   163,   187,   188,   189,
     190,   191,   192,   186,   132,   201,   134,   202,   203,   187,
     188,   189,   190,   191,   192,   352,   186,   426,   427,   353,
     142,   149,   187,   188,   189,   190,   191,   192,    16,   383,
     340,    23,    24,    16,    25,   342,   148,   431,   186,   439,
      16,   397,   375,   398,   187,   188,   189,   190,   191,   192,
      26,   158,    16,   110,   418,    16,    27,   440,   171,    28,
      29,   152,    30,   336,   111,   112,   159,    16,   412,   442,
     414,   206,   113,   178,   114,   225,   397,   115,   466,   458,
     459,   110,   116,   117,   118,   119,   120,   208,   209,   212,
     211,   229,   111,   112,   231,   246,   253,   110,   255,    75,
     113,   256,   114,   241,   257,   115,   262,   383,   111,   112,
     116,   117,   118,   119,   120,   288,   113,   289,   114,   292,
     110,   115,   294,   299,   303,   308,   116,   117,   118,   119,
     120,   111,   112,   319,   320,   110,   316,   338,   323,   113,
     328,   283,   347,   350,   115,   351,   111,   112,   359,   116,
     117,   118,   119,   120,   113,   110,   283,   364,   366,   115,
     371,   385,   336,   388,   116,   117,   118,   119,   324,   160,
     389,   387,   408,   410,   113,   413,   114,   428,   415,   115,
     419,   420,   421,   434,   116,   117,   118,   119,   120,    34,
     161,    35,   162,   163,    36,   435,   446,   451,   454,   164,
     165,   166,   167,    23,    24,   457,    25,    37,   463,   464,
     104,   465,   469,    38,   473,    60,    39,   237,    58,    40,
     467,   472,    26,    64,   409,   128,   129,   343,    27,   470,
     186,    28,   105,    62,    30,   131,   187,   188,   189,   190,
     191,   192,   186,   132,   133,   134,   135,   136,   187,   188,
     189,   190,   191,   192,   186,   468,   444,   240,   362,   425,
     187,   188,   189,   190,   191,   192,   258,   128,   129,   360,
       0,   322,   186,     0,     0,     0,   239,   261,   187,   188,
     189,   190,   191,   192,   186,   132,   201,   134,   202,   203,
     187,   188,   189,   190,   191,   192,   186,     0,     0,     0,
       0,     0,   187,   188,   189,   190,   191,   192,     0,   186,
       0,     0,     0,     0,   315,   187,   188,   189,   190,   191,
     192,     0,     0,     0,     0,     0,     0,   400,   263,   264,
     265,   266,   267,   268,   269
};

static const yytype_int16 yycheck[] =
{
       9,    73,    79,   235,    67,    54,    83,   239,   228,   283,
     294,   147,   208,   209,   210,   211,    25,    80,     6,    82,
      54,    84,     8,   111,   112,     5,    35,   104,   105,   101,
     110,   337,    81,     5,   340,   115,   342,     5,     5,   287,
      26,   289,     3,   311,     8,    28,    32,     5,    17,    35,
     113,   114,     5,    28,     6,    75,     5,    49,    19,    20,
      21,     5,    26,    24,    25,     5,    56,    75,    32,     5,
      62,    35,    25,     5,    64,    36,    59,    29,    39,    31,
     300,    30,    34,    23,    64,    57,    61,    75,    57,    75,
      57,   365,   366,     0,   171,    63,   159,   160,   147,    57,
     163,   164,   165,   166,   167,   193,   194,   195,    52,   158,
      71,    75,   418,   147,    75,   311,   196,   197,   198,   199,
     200,   369,   131,    74,   158,   393,     3,   359,    64,   413,
       5,   415,    64,   142,   440,    64,   442,    45,     5,    43,
      44,    45,    19,    20,    21,    63,    75,    24,    25,    16,
      58,    55,    56,    75,     4,    30,   107,   108,   432,    36,
     434,    11,    39,    52,   227,     5,   398,     5,   231,   232,
     390,   307,    61,    40,    63,    15,    16,    46,    61,    57,
       5,    52,    65,    23,    53,    54,    64,   236,    59,    12,
      30,   423,   424,     5,     5,   258,   259,   393,    75,    27,
      40,   397,    57,    15,    16,    57,    57,    59,   217,    64,
     219,    23,   221,    64,   286,    57,   288,    75,    30,     5,
     283,    58,    64,    63,    57,    53,    54,    71,    40,    59,
      41,    64,    60,    63,    75,    63,    47,    48,    49,    50,
      51,    52,    52,    71,    72,    73,    74,    75,   128,   129,
     313,    63,   315,   316,   317,    41,    75,   453,   307,    75,
      75,    47,    48,    49,    50,    51,    52,    17,    75,   465,
      57,   467,    57,   307,    57,    62,    59,    62,    57,    17,
      75,   290,    58,    62,    60,    61,    57,   296,    59,   352,
     353,    41,    17,    58,    71,    60,    61,    47,    48,    49,
      50,    51,    52,    41,    71,    72,    73,    74,    75,    47,
      48,    49,    50,    51,    52,    18,    41,   394,   395,    22,
      63,    75,    47,    48,    49,    50,    51,    52,    57,   338,
      59,     7,     8,    57,    10,    59,    71,   400,    41,   411,
      57,    57,    59,    59,    47,    48,    49,    50,    51,    52,
      26,    59,    57,    42,    59,    57,    32,    59,    15,    35,
      36,    75,    38,   372,    53,    54,    63,    57,   377,    59,
     379,    75,    61,    62,    63,     5,    57,    66,    59,   451,
     452,    42,    71,    72,    73,    74,    75,    65,    65,    52,
      65,    59,    53,    54,    37,    75,     5,    42,    30,     5,
      61,     5,    63,    64,    59,    66,    75,   416,    53,    54,
      71,    72,    73,    74,    75,    28,    61,    28,    63,    75,
      42,    66,    75,     9,    75,    75,    71,    72,    73,    74,
      75,    53,    54,    58,    75,    42,    63,    63,    62,    61,
       5,    63,     5,     5,    66,     5,    53,    54,    40,    71,
      72,    73,    74,    75,    61,    42,    63,     5,    57,    66,
      30,    59,   471,     5,    71,    72,    73,    74,    75,    37,
       9,    28,    28,    75,    61,    75,    63,    30,    75,    66,
      75,    75,     5,    59,    71,    72,    73,    74,    75,     8,
      58,    10,    60,    61,    13,    62,     5,    28,    75,    67,
      68,    69,    70,     7,     8,    59,    10,    26,    28,    75,
      14,     5,    63,    32,    64,    36,    35,    28,    34,    38,
     463,   471,    26,    40,   372,    53,    54,   296,    32,   467,
      41,    35,    36,    38,    38,    63,    47,    48,    49,    50,
      51,    52,    41,    71,    72,    73,    74,    75,    47,    48,
      49,    50,    51,    52,    41,   465,   416,   158,   316,   393,
      47,    48,    49,    50,    51,    52,    65,    53,    54,   313,
      -1,   259,    41,    -1,    -1,    -1,    33,    64,    47,    48,
      49,    50,    51,    52,    41,    71,    72,    73,    74,    75,
      47,    48,    49,    50,    51,    52,    41,    -1,    -1,    -1,
      -1,    -1,    47,    48,    49,    50,    51,    52,    -1,    41,
      -1,    -1,    -1,    -1,    59,    47,    48,    49,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    59,   186,   187,
     188,   189,   190,   191,   192
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    77,    78,    83,    75,    75,     0,    63,
      79,     5,    75,   127,     5,    12,    57,    64,    80,    81,
      84,    75,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    82,   115,   128,     8,    10,    13,    26,    32,    35,
      38,    85,    71,    88,    75,    89,    90,   127,    75,   113,
     114,   127,    75,   129,   131,    75,    96,    97,    89,   127,
      81,    75,   113,    75,    96,     5,    57,    52,    59,    90,
       5,   119,   114,    59,   119,     5,    30,     3,    19,    20,
      21,    24,    25,    36,    39,    71,    75,   134,   135,   136,
     137,   140,   141,   143,   144,   145,   146,   152,   153,   154,
     155,    52,    97,     5,    14,    36,    86,   119,   119,    71,
      42,    53,    54,    61,    63,    66,    71,    72,    73,    74,
      75,   137,   141,   157,   158,   159,   160,    27,    53,    54,
      60,    63,    71,    72,    73,    74,    75,    94,    95,    98,
     102,   103,    63,   120,    98,     5,   120,   130,    71,    75,
     129,   157,    75,   141,   156,   157,   129,   157,    59,    63,
      37,    58,    60,    61,    67,    68,    69,    70,    98,   129,
     129,    15,    87,   120,   120,   160,   159,   159,    62,   157,
     161,   162,   157,   160,    58,     5,    41,    47,    48,    49,
      50,    51,    52,    46,    53,    54,    43,    44,    45,    55,
      56,    72,    74,    75,    95,    95,    75,   127,    65,    65,
      61,    65,    52,     6,    29,    31,    34,     8,    26,    32,
      35,    75,   121,   122,   127,     5,     5,    52,   118,    59,
     134,    37,    23,    17,    58,    17,    57,    28,    30,    33,
     135,    64,   138,   139,   157,   157,    75,   142,   157,   157,
     157,   157,   157,     5,   129,    30,     5,    59,    65,    57,
      62,    64,    75,   158,   158,   158,   158,   158,   158,   158,
     159,   159,   159,   160,   160,   160,   160,   160,    64,    94,
      94,    94,    94,    63,    91,   157,    28,    61,    28,    28,
     101,   127,    75,   127,    75,   127,     5,    64,    59,     9,
     116,   157,    80,    75,   157,   157,   132,   133,    75,   132,
     141,   147,   132,    57,    64,    59,    63,    57,    62,    58,
      75,   157,   162,    62,    75,    91,    93,   157,     5,    98,
     103,    98,   103,   104,   105,   106,   127,    59,    63,   124,
      59,   124,    59,   122,     6,    75,   123,     5,    80,     5,
       5,     5,    18,    22,   134,    94,   149,   150,   151,    40,
     139,   157,   138,   157,     5,    59,    57,     5,    64,    57,
      62,    30,     5,    25,   110,    59,   123,     8,    26,    32,
      35,   125,   126,   127,   123,    59,   123,    28,     5,     9,
     117,   157,   157,     5,    16,    40,   148,    57,    59,   132,
      59,    64,    91,    91,    92,    64,    75,   103,    28,   106,
      75,   107,   127,    75,   127,    75,     5,    64,    59,    75,
      75,     5,    80,    17,    17,   150,   129,   129,    30,    94,
     132,   157,    57,    64,    59,    62,    99,    28,    59,    98,
      59,   124,    59,   124,   126,   123,     5,   132,   132,    91,
      91,    28,   100,   109,    75,   123,   123,    59,    98,    98,
     111,   112,   151,    28,    75,     5,    59,   109,   112,    63,
     111,   108,   104,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    77,    78,    79,    79,    80,    81,    81,
      82,    82,    82,    82,    82,    82,    83,    84,    84,    85,
      85,    85,    85,    85,    85,    86,    86,    86,    87,    87,
      88,    88,    89,    89,    90,    90,    91,    91,    91,    91,
      92,    92,    93,    93,    94,    94,    94,    95,    95,    95,
      95,    95,    96,    96,    97,    98,    98,    99,   100,    98,
      98,    98,   101,    98,    98,    98,    98,   102,   102,   103,
     103,   103,   103,   103,   103,   104,   105,   105,   107,   106,
     106,   108,   109,   110,   110,   110,   111,   111,   112,   112,
     113,   113,   114,   114,   115,   115,   116,   115,   117,   115,
     118,   115,   119,   120,   120,   121,   121,   122,   122,   122,
     122,   122,   122,   123,   123,   124,   124,   125,   125,   126,
     126,   126,   126,   126,   127,   127,   128,   130,   129,   131,
     129,   133,   132,   134,   134,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   136,   136,   136,   137,
     137,   137,   138,   138,   139,   139,   139,   140,   140,   140,
     140,   140,   141,   141,   141,   141,   142,   142,   143,   144,
     145,   145,   147,   146,   148,   148,   148,   149,   149,   150,
     150,   151,   151,   152,   153,   154,   154,   155,   156,   156,
     157,   157,   157,   157,   157,   157,   157,   157,   158,   158,
     158,   158,   158,   158,   159,   159,   159,   159,   159,   159,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   161,   161,   162,   162
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     3,     0,     2,     2,     0,
       3,     2,     2,     2,     1,     3,    11,     2,     0,     3,
       2,     2,     2,     5,     7,     2,     2,     0,     2,     0,
       3,     1,     2,     1,     4,     6,     1,     5,     3,     4,
       1,     3,     3,     5,     1,     2,     2,     1,     1,     1,
       1,     1,     2,     1,     4,     1,     2,     0,     0,     9,
       4,     9,     0,     5,     4,     2,     4,     1,     0,     3,
       1,     4,     3,     3,     3,     2,     3,     1,     0,     4,
       0,     0,     0,     7,     5,     0,     3,     1,     6,     0,
       2,     1,     4,     6,     7,     9,     0,     8,     0,    10,
       0,     7,     0,     3,     0,     3,     1,     3,     5,     4,
       3,     4,     4,     1,     3,     3,     0,     3,     1,     3,
       4,     4,     3,     5,     3,     1,     3,     0,     4,     0,
       2,     0,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     3,     1,     4,
       3,     6,     3,     1,     1,     3,     5,     3,     3,     3,
       3,     3,     4,     3,     2,     1,     3,     1,     3,     2,
       4,     6,     0,     7,     0,     2,     2,     3,     1,     3,
       0,     3,     1,     4,     4,     8,     8,     4,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     3,     2,     2,     2,
       3,     1,     3,     1,     3
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
  case 2: /* compilation_unit: program  */
#line 167 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1718 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 168 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1724 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 171 "pascal.y"
                                                            { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1730 "pascal.tab.c"
    break;

  case 5: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 174 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1736 "pascal.tab.c"
    break;

  case 6: /* file_id_list_opt: %empty  */
#line 175 "pascal.y"
     { (yyval.node) = NULL; }
#line 1742 "pascal.tab.c"
    break;

  case 7: /* block: decl_part_list body  */
#line 178 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1748 "pascal.tab.c"
    break;

  case 8: /* decl_part_list: decl_part_list decl_part  */
#line 181 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1754 "pascal.tab.c"
    break;

  case 9: /* decl_part_list: %empty  */
#line 182 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1760 "pascal.tab.c"
    break;

  case 10: /* decl_part: LABELSY label_list SEMICOLON  */
#line 185 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1766 "pascal.tab.c"
    break;

  case 11: /* decl_part: CONSTSY const_decl_list  */
#line 186 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1772 "pascal.tab.c"
    break;

  case 12: /* decl_part: TYPESY type_decl_list  */
#line 187 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1778 "pascal.tab.c"
    break;

  case 13: /* decl_part: VARSY var_decl_list  */
#line 188 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1784 "pascal.tab.c"
    break;

  case 14: /* decl_part: procedure_decl  */
#line 189 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1790 "pascal.tab.c"
    break;

  case 15: /* decl_part: USESSY id_list SEMICOLON  */
#line 190 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1796 "pascal.tab.c"
    break;

  case 16: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 193 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1802 "pascal.tab.c"
    break;

  case 17: /* intf_part_list: intf_part_list intf_part  */
#line 196 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1808 "pascal.tab.c"
    break;

  case 18: /* intf_part_list: %empty  */
#line 197 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1814 "pascal.tab.c"
    break;

  case 19: /* intf_part: USESSY id_list SEMICOLON  */
#line 200 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1820 "pascal.tab.c"
    break;

  case 20: /* intf_part: CONSTSY const_decl_list  */
#line 201 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1826 "pascal.tab.c"
    break;

  case 21: /* intf_part: TYPESY type_decl_list  */
#line 202 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1832 "pascal.tab.c"
    break;

  case 22: /* intf_part: VARSY var_decl_list  */
#line 203 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1838 "pascal.tab.c"
    break;

  case 23: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 204 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1844 "pascal.tab.c"
    break;

  case 24: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 205 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1850 "pascal.tab.c"
    break;

  case 25: /* init_opt: INITIALIZATIONSY statement_list  */
#line 208 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1856 "pascal.tab.c"
    break;

  case 26: /* init_opt: BEGINSY statement_list  */
#line 209 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1862 "pascal.tab.c"
    break;

  case 27: /* init_opt: %empty  */
#line 210 "pascal.y"
     { (yyval.node) = NULL; }
#line 1868 "pascal.tab.c"
    break;

  case 28: /* fini_opt: FINALIZATIONSY statement_list  */
#line 213 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1874 "pascal.tab.c"
    break;

  case 29: /* fini_opt: %empty  */
#line 214 "pascal.y"
     { (yyval.node) = NULL; }
#line 1880 "pascal.tab.c"
    break;

  case 30: /* label_list: label_list COMMA INTCONST  */
#line 217 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1886 "pascal.tab.c"
    break;

  case 31: /* label_list: INTCONST  */
#line 218 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1892 "pascal.tab.c"
    break;

  case 32: /* const_decl_list: const_decl_list const_decl  */
#line 221 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1898 "pascal.tab.c"
    break;

  case 33: /* const_decl_list: const_decl  */
#line 222 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1904 "pascal.tab.c"
    break;

  case 34: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 225 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 1910 "pascal.tab.c"
    break;

  case 35: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 226 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1916 "pascal.tab.c"
    break;

  case 36: /* typed_value: expression  */
#line 229 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1922 "pascal.tab.c"
    break;

  case 37: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 230 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1928 "pascal.tab.c"
    break;

  case 38: /* typed_value: LPARENT field_value_list RPARENT  */
#line 231 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 1934 "pascal.tab.c"
    break;

  case 39: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 232 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 1940 "pascal.tab.c"
    break;

  case 40: /* typed_value_list: typed_value  */
#line 235 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 1946 "pascal.tab.c"
    break;

  case 41: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 236 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1952 "pascal.tab.c"
    break;

  case 42: /* field_value_list: IDENT COLON typed_value  */
#line 239 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 1958 "pascal.tab.c"
    break;

  case 43: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 240 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 1964 "pascal.tab.c"
    break;

  case 44: /* constant: scalar_constant  */
#line 243 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1970 "pascal.tab.c"
    break;

  case 45: /* constant: PLUS scalar_constant  */
#line 244 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 1976 "pascal.tab.c"
    break;

  case 46: /* constant: MINUS scalar_constant  */
#line 245 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 1982 "pascal.tab.c"
    break;

  case 47: /* scalar_constant: IDENT  */
#line 248 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1988 "pascal.tab.c"
    break;

  case 48: /* scalar_constant: INTCONST  */
#line 249 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1994 "pascal.tab.c"
    break;

  case 49: /* scalar_constant: REALCONST  */
#line 250 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2000 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: STRINGCONST  */
#line 251 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2006 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: CHARCODE  */
#line 252 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2012 "pascal.tab.c"
    break;

  case 52: /* type_decl_list: type_decl_list type_decl  */
#line 255 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2018 "pascal.tab.c"
    break;

  case 53: /* type_decl_list: type_decl  */
#line 256 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2024 "pascal.tab.c"
    break;

  case 54: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 259 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2030 "pascal.tab.c"
    break;

  case 55: /* type: simple_type  */
#line 262 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2036 "pascal.tab.c"
    break;

  case 56: /* type: ARROW IDENT  */
#line 263 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2042 "pascal.tab.c"
    break;

  case 57: /* $@1: %empty  */
#line 264 "pascal.y"
                                                       { }
#line 2048 "pascal.tab.c"
    break;

  case 58: /* $@2: %empty  */
#line 264 "pascal.y"
                                                           { }
#line 2054 "pascal.tab.c"
    break;

  case 59: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 264 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2060 "pascal.tab.c"
    break;

  case 60: /* type: packed_opt ARRAYSY OFSY type  */
#line 265 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2066 "pascal.tab.c"
    break;

  case 61: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 266 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2072 "pascal.tab.c"
    break;

  case 62: /* $@3: %empty  */
#line 267 "pascal.y"
                         { }
#line 2078 "pascal.tab.c"
    break;

  case 63: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 267 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2084 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt SETSY OFSY simple_type  */
#line 268 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2090 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt FILESY  */
#line 269 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2096 "pascal.tab.c"
    break;

  case 66: /* type: packed_opt FILESY OFSY type  */
#line 270 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2102 "pascal.tab.c"
    break;

  case 67: /* packed_opt: PACKEDSY  */
#line 273 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2108 "pascal.tab.c"
    break;

  case 68: /* packed_opt: %empty  */
#line 274 "pascal.y"
     { (yyval.node) = NULL; }
#line 2114 "pascal.tab.c"
    break;

  case 69: /* simple_type: LPARENT id_list RPARENT  */
#line 277 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2120 "pascal.tab.c"
    break;

  case 70: /* simple_type: IDENT  */
#line 278 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2126 "pascal.tab.c"
    break;

  case 71: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 279 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2132 "pascal.tab.c"
    break;

  case 72: /* simple_type: constant DOTDOT constant  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2138 "pascal.tab.c"
    break;

  case 73: /* simple_type: STRINGCONST DOTDOT constant  */
#line 281 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2144 "pascal.tab.c"
    break;

  case 74: /* simple_type: CHARCODE DOTDOT constant  */
#line 282 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2150 "pascal.tab.c"
    break;

  case 75: /* record_body: record_field_list record_case_opt  */
#line 285 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2156 "pascal.tab.c"
    break;

  case 76: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 288 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2162 "pascal.tab.c"
    break;

  case 77: /* record_field_list: record_field  */
#line 289 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2168 "pascal.tab.c"
    break;

  case 78: /* $@4: %empty  */
#line 292 "pascal.y"
                  { }
#line 2174 "pascal.tab.c"
    break;

  case 79: /* record_field: id_list COLON $@4 type  */
#line 292 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2180 "pascal.tab.c"
    break;

  case 80: /* record_field: %empty  */
#line 293 "pascal.y"
     { (yyval.node) = NULL; }
#line 2186 "pascal.tab.c"
    break;

  case 81: /* case_arm_mark: %empty  */
#line 296 "pascal.y"
    { }
#line 2192 "pascal.tab.c"
    break;

  case 82: /* case_open: %empty  */
#line 299 "pascal.y"
    { }
#line 2198 "pascal.tab.c"
    break;

  case 83: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 302 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2204 "pascal.tab.c"
    break;

  case 84: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 303 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2210 "pascal.tab.c"
    break;

  case 85: /* record_case_opt: %empty  */
#line 304 "pascal.y"
     { (yyval.node) = NULL; }
#line 2216 "pascal.tab.c"
    break;

  case 86: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 307 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2222 "pascal.tab.c"
    break;

  case 87: /* record_case_list: record_case_arm  */
#line 308 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2228 "pascal.tab.c"
    break;

  case 88: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 311 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2234 "pascal.tab.c"
    break;

  case 89: /* record_case_arm: %empty  */
#line 312 "pascal.y"
     { (yyval.node) = NULL; }
#line 2240 "pascal.tab.c"
    break;

  case 90: /* var_decl_list: var_decl_list var_decl  */
#line 315 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2246 "pascal.tab.c"
    break;

  case 91: /* var_decl_list: var_decl  */
#line 316 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2252 "pascal.tab.c"
    break;

  case 92: /* var_decl: id_list COLON type SEMICOLON  */
#line 319 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2258 "pascal.tab.c"
    break;

  case 93: /* var_decl: id_list COLON type EQOP expression SEMICOLON  */
#line 320 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2264 "pascal.tab.c"
    break;

  case 94: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 323 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2270 "pascal.tab.c"
    break;

  case 95: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 324 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2276 "pascal.tab.c"
    break;

  case 96: /* $@5: %empty  */
#line 325 "pascal.y"
                                                            { }
#line 2282 "pascal.tab.c"
    break;

  case 97: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 325 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2288 "pascal.tab.c"
    break;

  case 98: /* $@6: %empty  */
#line 326 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); }
#line 2294 "pascal.tab.c"
    break;

  case 99: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 326 "pascal.y"
                                                                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2300 "pascal.tab.c"
    break;

  case 100: /* $@7: %empty  */
#line 327 "pascal.y"
                                        { }
#line 2306 "pascal.tab.c"
    break;

  case 101: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 327 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2312 "pascal.tab.c"
    break;

  case 102: /* pv_mark: %empty  */
#line 330 "pascal.y"
    { }
#line 2318 "pascal.tab.c"
    break;

  case 103: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 333 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2324 "pascal.tab.c"
    break;

  case 104: /* parameter_list_opt: %empty  */
#line 334 "pascal.y"
     { (yyval.node) = NULL; }
#line 2330 "pascal.tab.c"
    break;

  case 105: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 337 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2336 "pascal.tab.c"
    break;

  case 106: /* parameter_decl_list: parameter_decl  */
#line 338 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2342 "pascal.tab.c"
    break;

  case 107: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 341 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2348 "pascal.tab.c"
    break;

  case 108: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 342 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2354 "pascal.tab.c"
    break;

  case 109: /* parameter_decl: VARSY id_list COLON param_type  */
#line 343 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2360 "pascal.tab.c"
    break;

  case 110: /* parameter_decl: id_list COLON param_type  */
#line 344 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2366 "pascal.tab.c"
    break;

  case 111: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 345 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2372 "pascal.tab.c"
    break;

  case 112: /* parameter_decl: IDENT id_list COLON param_type  */
#line 346 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2378 "pascal.tab.c"
    break;

  case 113: /* param_type: IDENT  */
#line 349 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2384 "pascal.tab.c"
    break;

  case 114: /* param_type: ARRAYSY OFSY IDENT  */
#line 350 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 2390 "pascal.tab.c"
    break;

  case 115: /* pf_params: LPARENT pf_sections RPARENT  */
#line 353 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2396 "pascal.tab.c"
    break;

  case 116: /* pf_params: %empty  */
#line 354 "pascal.y"
     { (yyval.node) = NULL; }
#line 2402 "pascal.tab.c"
    break;

  case 117: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 357 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2408 "pascal.tab.c"
    break;

  case 118: /* pf_sections: pf_section  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2414 "pascal.tab.c"
    break;

  case 119: /* pf_section: id_list COLON param_type  */
#line 361 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2420 "pascal.tab.c"
    break;

  case 120: /* pf_section: VARSY id_list COLON param_type  */
#line 362 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2426 "pascal.tab.c"
    break;

  case 121: /* pf_section: CONSTSY id_list COLON param_type  */
#line 363 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2432 "pascal.tab.c"
    break;

  case 122: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 364 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2438 "pascal.tab.c"
    break;

  case 123: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 365 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2444 "pascal.tab.c"
    break;

  case 124: /* id_list: id_list COMMA IDENT  */
#line 368 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2450 "pascal.tab.c"
    break;

  case 125: /* id_list: IDENT  */
#line 369 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2456 "pascal.tab.c"
    break;

  case 126: /* body: BEGINSY statement_list ENDSY  */
#line 372 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2462 "pascal.tab.c"
    break;

  case 127: /* $@8: %empty  */
#line 375 "pascal.y"
                             { pas_stmt_mark(); }
#line 2468 "pascal.tab.c"
    break;

  case 128: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 375 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2474 "pascal.tab.c"
    break;

  case 129: /* $@9: %empty  */
#line 376 "pascal.y"
     { pas_stmt_mark(); }
#line 2480 "pascal.tab.c"
    break;

  case 130: /* statement_list: $@9 statement  */
#line 376 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2486 "pascal.tab.c"
    break;

  case 131: /* $@10: %empty  */
#line 379 "pascal.y"
    { pas_stmt_mark(); }
#line 2492 "pascal.tab.c"
    break;

  case 132: /* body_stmt: $@10 statement  */
#line 379 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2498 "pascal.tab.c"
    break;

  case 133: /* statement: statement_no_label  */
#line 382 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2504 "pascal.tab.c"
    break;

  case 134: /* statement: INTCONST COLON statement_no_label  */
#line 383 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2510 "pascal.tab.c"
    break;

  case 135: /* statement_no_label: assignment  */
#line 386 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2516 "pascal.tab.c"
    break;

  case 136: /* statement_no_label: call  */
#line 387 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2522 "pascal.tab.c"
    break;

  case 137: /* statement_no_label: compound_statement  */
#line 388 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2528 "pascal.tab.c"
    break;

  case 138: /* statement_no_label: goto_statement  */
#line 389 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2534 "pascal.tab.c"
    break;

  case 139: /* statement_no_label: if_statement  */
#line 390 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2540 "pascal.tab.c"
    break;

  case 140: /* statement_no_label: case_statement  */
#line 391 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2546 "pascal.tab.c"
    break;

  case 141: /* statement_no_label: while_statement  */
#line 392 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2552 "pascal.tab.c"
    break;

  case 142: /* statement_no_label: repeat_statement  */
#line 393 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2558 "pascal.tab.c"
    break;

  case 143: /* statement_no_label: for_statement  */
#line 394 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2564 "pascal.tab.c"
    break;

  case 144: /* statement_no_label: with_statement  */
#line 395 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2570 "pascal.tab.c"
    break;

  case 145: /* statement_no_label: %empty  */
#line 396 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2576 "pascal.tab.c"
    break;

  case 146: /* call: IDENT  */
#line 399 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2582 "pascal.tab.c"
    break;

  case 147: /* call: selector PERIOD IDENT  */
#line 400 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2588 "pascal.tab.c"
    break;

  case 148: /* call: call_with_args  */
#line 401 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2594 "pascal.tab.c"
    break;

  case 149: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 404 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2600 "pascal.tab.c"
    break;

  case 150: /* call_with_args: IDENT LPARENT RPARENT  */
#line 405 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 2606 "pascal.tab.c"
    break;

  case 151: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 406 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2612 "pascal.tab.c"
    break;

  case 152: /* argument_list: argument_list COMMA argument  */
#line 409 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2618 "pascal.tab.c"
    break;

  case 153: /* argument_list: argument  */
#line 410 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2624 "pascal.tab.c"
    break;

  case 154: /* argument: expression  */
#line 413 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2630 "pascal.tab.c"
    break;

  case 155: /* argument: expression COLON expression  */
#line 414 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2636 "pascal.tab.c"
    break;

  case 156: /* argument: expression COLON expression COLON expression  */
#line 415 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2642 "pascal.tab.c"
    break;

  case 157: /* assignment: selector BECOMES expression  */
#line 418 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2648 "pascal.tab.c"
    break;

  case 158: /* assignment: selector ADDBECOMES expression  */
#line 419 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 2654 "pascal.tab.c"
    break;

  case 159: /* assignment: selector SUBBECOMES expression  */
#line 420 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 2660 "pascal.tab.c"
    break;

  case 160: /* assignment: selector MULBECOMES expression  */
#line 421 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 2666 "pascal.tab.c"
    break;

  case 161: /* assignment: selector DIVBECOMES expression  */
#line 422 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 2672 "pascal.tab.c"
    break;

  case 162: /* selector: selector LBRACK expression_list RBRACK  */
#line 425 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2678 "pascal.tab.c"
    break;

  case 163: /* selector: selector PERIOD IDENT  */
#line 426 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2684 "pascal.tab.c"
    break;

  case 164: /* selector: selector ARROW  */
#line 427 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2690 "pascal.tab.c"
    break;

  case 165: /* selector: IDENT  */
#line 428 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2696 "pascal.tab.c"
    break;

  case 166: /* expression_list: expression_list COMMA expression  */
#line 431 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 167: /* expression_list: expression  */
#line 432 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2708 "pascal.tab.c"
    break;

  case 168: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 435 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2714 "pascal.tab.c"
    break;

  case 169: /* goto_statement: GOTOSY INTCONST  */
#line 438 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2720 "pascal.tab.c"
    break;

  case 170: /* if_statement: IFSY expression THENSY body_stmt  */
#line 441 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2726 "pascal.tab.c"
    break;

  case 171: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 442 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2732 "pascal.tab.c"
    break;

  case 172: /* $@11: %empty  */
#line 445 "pascal.y"
                           { }
#line 2738 "pascal.tab.c"
    break;

  case 173: /* case_statement: CASESY expression OFSY $@11 case_list case_tail ENDSY  */
#line 445 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2744 "pascal.tab.c"
    break;

  case 174: /* case_tail: %empty  */
#line 448 "pascal.y"
    { (yyval.node) = NULL; }
#line 2750 "pascal.tab.c"
    break;

  case 175: /* case_tail: ELSESY statement_list  */
#line 449 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 2756 "pascal.tab.c"
    break;

  case 176: /* case_tail: OTHERWISESY statement_list  */
#line 450 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 2762 "pascal.tab.c"
    break;

  case 177: /* case_list: case_list SEMICOLON case_elem  */
#line 453 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2768 "pascal.tab.c"
    break;

  case 178: /* case_list: case_elem  */
#line 454 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 179: /* case_elem: constant_list COLON body_stmt  */
#line 457 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2780 "pascal.tab.c"
    break;

  case 180: /* case_elem: %empty  */
#line 458 "pascal.y"
     { (yyval.node) = NULL; }
#line 2786 "pascal.tab.c"
    break;

  case 181: /* constant_list: constant_list COMMA constant  */
#line 461 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2792 "pascal.tab.c"
    break;

  case 182: /* constant_list: constant  */
#line 462 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 183: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 465 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2804 "pascal.tab.c"
    break;

  case 184: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 468 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2810 "pascal.tab.c"
    break;

  case 185: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 471 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 2816 "pascal.tab.c"
    break;

  case 186: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 472 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 2822 "pascal.tab.c"
    break;

  case 187: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 475 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2828 "pascal.tab.c"
    break;

  case 188: /* with_open: with_open COMMA selector  */
#line 478 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2834 "pascal.tab.c"
    break;

  case 189: /* with_open: selector  */
#line 479 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2840 "pascal.tab.c"
    break;

  case 190: /* expression: simple_expression  */
#line 486 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2846 "pascal.tab.c"
    break;

  case 191: /* expression: expression INOP simple_expression  */
#line 487 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2852 "pascal.tab.c"
    break;

  case 192: /* expression: expression LTOP simple_expression  */
#line 488 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2858 "pascal.tab.c"
    break;

  case 193: /* expression: expression LEOP simple_expression  */
#line 489 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2864 "pascal.tab.c"
    break;

  case 194: /* expression: expression GTOP simple_expression  */
#line 490 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2870 "pascal.tab.c"
    break;

  case 195: /* expression: expression GEOP simple_expression  */
#line 491 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2876 "pascal.tab.c"
    break;

  case 196: /* expression: expression NEOP simple_expression  */
#line 492 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2882 "pascal.tab.c"
    break;

  case 197: /* expression: expression EQOP simple_expression  */
#line 493 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2888 "pascal.tab.c"
    break;

  case 198: /* simple_expression: term  */
#line 496 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2894 "pascal.tab.c"
    break;

  case 199: /* simple_expression: PLUS term  */
#line 497 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2900 "pascal.tab.c"
    break;

  case 200: /* simple_expression: MINUS term  */
#line 498 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2906 "pascal.tab.c"
    break;

  case 201: /* simple_expression: simple_expression PLUS term  */
#line 499 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2912 "pascal.tab.c"
    break;

  case 202: /* simple_expression: simple_expression MINUS term  */
#line 500 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2918 "pascal.tab.c"
    break;

  case 203: /* simple_expression: simple_expression OROP term  */
#line 501 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2924 "pascal.tab.c"
    break;

  case 204: /* term: factor  */
#line 504 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2930 "pascal.tab.c"
    break;

  case 205: /* term: term MUL factor  */
#line 505 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2936 "pascal.tab.c"
    break;

  case 206: /* term: term RDIV factor  */
#line 506 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2942 "pascal.tab.c"
    break;

  case 207: /* term: term IDIV factor  */
#line 507 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2948 "pascal.tab.c"
    break;

  case 208: /* term: term IMOD factor  */
#line 508 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2954 "pascal.tab.c"
    break;

  case 209: /* term: term ANDOP factor  */
#line 509 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2960 "pascal.tab.c"
    break;

  case 210: /* factor: selector  */
#line 512 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2966 "pascal.tab.c"
    break;

  case 211: /* factor: call_with_args  */
#line 513 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2972 "pascal.tab.c"
    break;

  case 212: /* factor: INTCONST  */
#line 514 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2978 "pascal.tab.c"
    break;

  case 213: /* factor: REALCONST  */
#line 515 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2984 "pascal.tab.c"
    break;

  case 214: /* factor: STRINGCONST  */
#line 516 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2990 "pascal.tab.c"
    break;

  case 215: /* factor: CHARCODE  */
#line 517 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2996 "pascal.tab.c"
    break;

  case 216: /* factor: LPARENT expression RPARENT  */
#line 518 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3002 "pascal.tab.c"
    break;

  case 217: /* factor: NOTSY factor  */
#line 519 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 3008 "pascal.tab.c"
    break;

  case 218: /* factor: ATSIGN factor  */
#line 520 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 3014 "pascal.tab.c"
    break;

  case 219: /* factor: LBRACK RBRACK  */
#line 521 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 3020 "pascal.tab.c"
    break;

  case 220: /* factor: LBRACK set_member_list RBRACK  */
#line 522 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 3026 "pascal.tab.c"
    break;

  case 221: /* set_member_list: set_member  */
#line 525 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3032 "pascal.tab.c"
    break;

  case 222: /* set_member_list: set_member_list COMMA set_member  */
#line 526 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3038 "pascal.tab.c"
    break;

  case 223: /* set_member: expression  */
#line 529 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3044 "pascal.tab.c"
    break;

  case 224: /* set_member: expression DOTDOT expression  */
#line 530 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3050 "pascal.tab.c"
    break;


#line 3054 "pascal.tab.c"

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

#line 536 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
extern void pascal_lex_reset(void);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    pascal_lex_reset(); pascal_cenv_reset();
    pas_stmt_sp = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
