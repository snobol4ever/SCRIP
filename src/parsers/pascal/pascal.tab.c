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
#line 24 "pascal.y"

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
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; int unit_mode; int dialect; PasDefE *defs; int ndefs; int capdefs; int cond_skip; int cond_depth; int cond_taken; int cond_unknown; int macro_on; const char *mode_macro; unsigned sw_set; unsigned sw_on; unsigned *sw_save; int sw_n; int sw_cap; } g_pascal_cenv;
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
void pascal_cenv_reset(void) { g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; g_pascal_cenv.unit_mode = 0; g_pascal_cenv.dialect = PAS_DIALECT_ISO_DEFAULT; g_pascal_cenv.ndefs = 0; g_pascal_cenv.cond_skip = 0; g_pascal_cenv.cond_depth = 0; g_pascal_cenv.cond_taken = 0; g_pascal_cenv.cond_unknown = 0; g_pascal_cenv.macro_on = 0; g_pascal_cenv.mode_macro = NULL; g_pascal_cenv.sw_set = 0; g_pascal_cenv.sw_on = 0; g_pascal_cenv.sw_n = 0; }
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

#line 189 "pascal.tab.c"

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
  YYSYMBOL_DOSY = 16,                      /* DOSY  */
  YYSYMBOL_DOWNTOSY = 17,                  /* DOWNTOSY  */
  YYSYMBOL_FORSY = 18,                     /* FORSY  */
  YYSYMBOL_REPEATSY = 19,                  /* REPEATSY  */
  YYSYMBOL_WHILESY = 20,                   /* WHILESY  */
  YYSYMBOL_TOSY = 21,                      /* TOSY  */
  YYSYMBOL_UNTILSY = 22,                   /* UNTILSY  */
  YYSYMBOL_WITHSY = 23,                    /* WITHSY  */
  YYSYMBOL_CASESY = 24,                    /* CASESY  */
  YYSYMBOL_PROCEDURESY = 25,               /* PROCEDURESY  */
  YYSYMBOL_PACKEDSY = 26,                  /* PACKEDSY  */
  YYSYMBOL_OFSY = 27,                      /* OFSY  */
  YYSYMBOL_FILESY = 28,                    /* FILESY  */
  YYSYMBOL_ENDSY = 29,                     /* ENDSY  */
  YYSYMBOL_SETSY = 30,                     /* SETSY  */
  YYSYMBOL_VARSY = 31,                     /* VARSY  */
  YYSYMBOL_THENSY = 32,                    /* THENSY  */
  YYSYMBOL_RECORDSY = 33,                  /* RECORDSY  */
  YYSYMBOL_FUNCTIONSY = 34,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 35,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 36,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 37,                    /* TYPESY  */
  YYSYMBOL_IFSY = 38,                      /* IFSY  */
  YYSYMBOL_ELSESY = 39,                    /* ELSESY  */
  YYSYMBOL_INOP = 40,                      /* INOP  */
  YYSYMBOL_NOTSY = 41,                     /* NOTSY  */
  YYSYMBOL_IDIV = 42,                      /* IDIV  */
  YYSYMBOL_IMOD = 43,                      /* IMOD  */
  YYSYMBOL_ANDOP = 44,                     /* ANDOP  */
  YYSYMBOL_OROP = 45,                      /* OROP  */
  YYSYMBOL_LTOP = 46,                      /* LTOP  */
  YYSYMBOL_LEOP = 47,                      /* LEOP  */
  YYSYMBOL_GTOP = 48,                      /* GTOP  */
  YYSYMBOL_GEOP = 49,                      /* GEOP  */
  YYSYMBOL_NEOP = 50,                      /* NEOP  */
  YYSYMBOL_EQOP = 51,                      /* EQOP  */
  YYSYMBOL_PLUS = 52,                      /* PLUS  */
  YYSYMBOL_MINUS = 53,                     /* MINUS  */
  YYSYMBOL_MUL = 54,                       /* MUL  */
  YYSYMBOL_RDIV = 55,                      /* RDIV  */
  YYSYMBOL_COMMA = 56,                     /* COMMA  */
  YYSYMBOL_PERIOD = 57,                    /* PERIOD  */
  YYSYMBOL_COLON = 58,                     /* COLON  */
  YYSYMBOL_ARROW = 59,                     /* ARROW  */
  YYSYMBOL_LBRACK = 60,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 61,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 62,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 63,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 64,                    /* DOTDOT  */
  YYSYMBOL_ATSIGN = 65,                    /* ATSIGN  */
  YYSYMBOL_INTCONST = 66,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 67,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 68,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 69,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 70,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 71,                  /* $accept  */
  YYSYMBOL_compilation_unit = 72,          /* compilation_unit  */
  YYSYMBOL_program = 73,                   /* program  */
  YYSYMBOL_file_id_list_opt = 74,          /* file_id_list_opt  */
  YYSYMBOL_block = 75,                     /* block  */
  YYSYMBOL_decl_part_list = 76,            /* decl_part_list  */
  YYSYMBOL_decl_part = 77,                 /* decl_part  */
  YYSYMBOL_unit = 78,                      /* unit  */
  YYSYMBOL_intf_part_list = 79,            /* intf_part_list  */
  YYSYMBOL_intf_part = 80,                 /* intf_part  */
  YYSYMBOL_init_opt = 81,                  /* init_opt  */
  YYSYMBOL_fini_opt = 82,                  /* fini_opt  */
  YYSYMBOL_label_list = 83,                /* label_list  */
  YYSYMBOL_const_decl_list = 84,           /* const_decl_list  */
  YYSYMBOL_const_decl = 85,                /* const_decl  */
  YYSYMBOL_constant = 86,                  /* constant  */
  YYSYMBOL_scalar_constant = 87,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 88,            /* type_decl_list  */
  YYSYMBOL_type_decl = 89,                 /* type_decl  */
  YYSYMBOL_type = 90,                      /* type  */
  YYSYMBOL_91_1 = 91,                      /* $@1  */
  YYSYMBOL_92_2 = 92,                      /* $@2  */
  YYSYMBOL_93_3 = 93,                      /* $@3  */
  YYSYMBOL_packed_opt = 94,                /* packed_opt  */
  YYSYMBOL_simple_type = 95,               /* simple_type  */
  YYSYMBOL_record_body = 96,               /* record_body  */
  YYSYMBOL_record_field_list = 97,         /* record_field_list  */
  YYSYMBOL_record_field = 98,              /* record_field  */
  YYSYMBOL_99_4 = 99,                      /* $@4  */
  YYSYMBOL_case_arm_mark = 100,            /* case_arm_mark  */
  YYSYMBOL_case_open = 101,                /* case_open  */
  YYSYMBOL_record_case_opt = 102,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 103,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 104,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 105,            /* var_decl_list  */
  YYSYMBOL_var_decl = 106,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 107,           /* procedure_decl  */
  YYSYMBOL_108_5 = 108,                    /* $@5  */
  YYSYMBOL_109_6 = 109,                    /* $@6  */
  YYSYMBOL_110_7 = 110,                    /* $@7  */
  YYSYMBOL_pv_mark = 111,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 112,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 113,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 114,           /* parameter_decl  */
  YYSYMBOL_pf_params = 115,                /* pf_params  */
  YYSYMBOL_pf_sections = 116,              /* pf_sections  */
  YYSYMBOL_pf_section = 117,               /* pf_section  */
  YYSYMBOL_id_list = 118,                  /* id_list  */
  YYSYMBOL_body = 119,                     /* body  */
  YYSYMBOL_statement_list = 120,           /* statement_list  */
  YYSYMBOL_121_8 = 121,                    /* $@8  */
  YYSYMBOL_122_9 = 122,                    /* $@9  */
  YYSYMBOL_body_stmt = 123,                /* body_stmt  */
  YYSYMBOL_124_10 = 124,                   /* $@10  */
  YYSYMBOL_statement = 125,                /* statement  */
  YYSYMBOL_statement_no_label = 126,       /* statement_no_label  */
  YYSYMBOL_call = 127,                     /* call  */
  YYSYMBOL_call_with_args = 128,           /* call_with_args  */
  YYSYMBOL_argument_list = 129,            /* argument_list  */
  YYSYMBOL_argument = 130,                 /* argument  */
  YYSYMBOL_assignment = 131,               /* assignment  */
  YYSYMBOL_selector = 132,                 /* selector  */
  YYSYMBOL_expression_list = 133,          /* expression_list  */
  YYSYMBOL_compound_statement = 134,       /* compound_statement  */
  YYSYMBOL_goto_statement = 135,           /* goto_statement  */
  YYSYMBOL_if_statement = 136,             /* if_statement  */
  YYSYMBOL_case_statement = 137,           /* case_statement  */
  YYSYMBOL_138_11 = 138,                   /* $@11  */
  YYSYMBOL_case_list = 139,                /* case_list  */
  YYSYMBOL_case_elem = 140,                /* case_elem  */
  YYSYMBOL_constant_list = 141,            /* constant_list  */
  YYSYMBOL_while_statement = 142,          /* while_statement  */
  YYSYMBOL_repeat_statement = 143,         /* repeat_statement  */
  YYSYMBOL_for_statement = 144,            /* for_statement  */
  YYSYMBOL_with_statement = 145,           /* with_statement  */
  YYSYMBOL_with_open = 146,                /* with_open  */
  YYSYMBOL_expression = 147,               /* expression  */
  YYSYMBOL_simple_expression = 148,        /* simple_expression  */
  YYSYMBOL_term = 149,                     /* term  */
  YYSYMBOL_factor = 150,                   /* factor  */
  YYSYMBOL_set_member_list = 151,          /* set_member_list  */
  YYSYMBOL_set_member = 152                /* set_member  */
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
#define YYLAST   536

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  205
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  426

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   325


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
      65,    66,    67,    68,    69,    70
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   152,   152,   153,   156,   159,   160,   163,   166,   167,
     170,   171,   172,   173,   174,   175,   178,   181,   182,   185,
     186,   187,   188,   189,   190,   193,   194,   195,   198,   199,
     202,   203,   206,   207,   210,   211,   212,   213,   214,   215,
     218,   219,   220,   223,   224,   225,   226,   227,   230,   231,
     234,   237,   238,   239,   239,   239,   240,   241,   241,   242,
     243,   244,   247,   248,   251,   252,   253,   254,   255,   258,
     261,   262,   265,   265,   266,   269,   272,   275,   276,   277,
     280,   281,   284,   285,   288,   289,   292,   293,   296,   297,
     298,   298,   299,   299,   300,   300,   303,   306,   307,   310,
     311,   314,   315,   316,   317,   320,   321,   324,   325,   328,
     329,   330,   331,   334,   335,   338,   341,   341,   342,   342,
     345,   345,   348,   349,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   365,   366,   367,   370,   371,
     374,   375,   378,   379,   380,   383,   386,   387,   388,   389,
     392,   393,   396,   399,   402,   403,   406,   406,   409,   410,
     413,   414,   417,   418,   421,   424,   427,   428,   431,   434,
     435,   442,   443,   444,   445,   446,   447,   448,   449,   452,
     453,   454,   455,   456,   457,   460,   461,   462,   463,   464,
     465,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   481,   482,   485,   486
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
  "FINALIZATIONSY", "DOSY", "DOWNTOSY", "FORSY", "REPEATSY", "WHILESY",
  "TOSY", "UNTILSY", "WITHSY", "CASESY", "PROCEDURESY", "PACKEDSY", "OFSY",
  "FILESY", "ENDSY", "SETSY", "VARSY", "THENSY", "RECORDSY", "FUNCTIONSY",
  "BEGINSY", "BECOMES", "TYPESY", "IFSY", "ELSESY", "INOP", "NOTSY",
  "IDIV", "IMOD", "ANDOP", "OROP", "LTOP", "LEOP", "GTOP", "GEOP", "NEOP",
  "EQOP", "PLUS", "MINUS", "MUL", "RDIV", "COMMA", "PERIOD", "COLON",
  "ARROW", "LBRACK", "RBRACK", "LPARENT", "RPARENT", "DOTDOT", "ATSIGN",
  "INTCONST", "CHARCODE", "REALCONST", "STRINGCONST", "IDENT", "$accept",
  "compilation_unit", "program", "file_id_list_opt", "block",
  "decl_part_list", "decl_part", "unit", "intf_part_list", "intf_part",
  "init_opt", "fini_opt", "label_list", "const_decl_list", "const_decl",
  "constant", "scalar_constant", "type_decl_list", "type_decl", "type",
  "$@1", "$@2", "$@3", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "$@4", "case_arm_mark", "case_open",
  "record_case_opt", "record_case_list", "record_case_arm",
  "var_decl_list", "var_decl", "procedure_decl", "$@5", "$@6", "$@7",
  "pv_mark", "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "pf_params", "pf_sections", "pf_section", "id_list", "body",
  "statement_list", "$@8", "$@9", "body_stmt", "$@10", "statement",
  "statement_no_label", "call", "call_with_args", "argument_list",
  "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@11", "case_list", "case_elem", "constant_list", "while_statement",
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

#define YYPACT_NINF (-287)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-150)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     151,   -35,     0,   110,  -287,  -287,    35,   182,  -287,    72,
     183,   180,  -287,   195,  -287,  -287,   103,  -287,   152,   289,
     366,  -287,  -287,   154,   153,    72,   167,    72,   177,  -287,
     178,  -287,  -287,  -287,   153,    72,  -287,   186,    72,   187,
     178,  -287,  -287,    15,   201,   153,  -287,    17,  -287,    72,
    -287,    66,  -287,    36,    24,   202,   178,  -287,   153,    27,
     256,  -287,    72,  -287,   178,  -287,   199,   372,  -287,  -287,
     206,  -287,   137,    19,  -287,  -287,   207,   216,  -287,   265,
     222,   265,  -287,   265,   246,   107,  -287,  -287,  -287,  -287,
    -287,   134,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
     137,  -287,  -287,  -287,  -287,   300,   206,   206,  -287,   271,
     291,  -287,   311,   317,   323,  -287,   324,  -287,    21,   339,
    -287,   326,   326,   275,    72,   284,  -287,   285,   286,   288,
      23,    28,  -287,  -287,   308,    24,  -287,   310,    58,   315,
     315,   315,   302,   265,   315,  -287,  -287,  -287,  -287,   316,
    -287,    41,    98,   123,   217,  -287,  -287,   179,    20,   359,
      40,   404,   116,   265,   265,   303,  -287,   265,   382,   384,
     384,  -287,   361,   393,   330,  -287,   396,  -287,  -287,   397,
    -287,  -287,  -287,  -287,  -287,   334,    72,   342,    14,  -287,
      94,   406,  -287,   211,   430,   430,   430,  -287,   265,   353,
     389,   399,  -287,  -287,   357,  -287,   265,   265,  -287,   217,
     217,  -287,   371,    65,  -287,   383,  -287,   358,  -287,   265,
     265,   265,   265,   265,   265,   265,   315,   315,   315,   315,
     315,   315,   315,   315,   367,  -287,   222,  -287,  -287,  -287,
    -287,   226,  -287,   416,   444,   138,    97,   444,  -287,   384,
     386,  -287,   375,  -287,  -287,   385,   227,   385,    21,  -287,
     378,   452,  -287,  -287,  -287,  -287,  -287,   132,   159,   137,
     159,    72,   453,   454,   229,   444,   265,   265,  -287,  -287,
     387,  -287,    24,   123,   123,   123,   123,   123,   123,   123,
     217,   217,   217,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
     179,   430,   421,   265,  -287,   265,   265,   265,  -287,  -287,
     456,    26,  -287,   398,   412,  -287,  -287,  -287,   466,  -287,
     100,  -287,  -287,   443,    48,  -287,   245,  -287,   464,   265,
     265,   444,  -287,  -287,  -287,    70,  -287,   249,  -287,  -287,
     429,   232,   444,  -287,   411,    72,   415,    16,  -287,   254,
    -287,   418,  -287,   159,   459,  -287,    72,   419,  -287,  -287,
     503,  -287,   168,   194,   430,  -287,   430,  -287,  -287,   265,
    -287,   385,   255,   385,    26,  -287,   439,  -287,   449,  -287,
    -287,     6,   137,  -287,   506,  -287,  -287,  -287,  -287,  -287,
     444,  -287,   442,   455,  -287,  -287,   487,  -287,  -287,   445,
    -287,  -287,  -287,  -287,  -287,   446,   137,   137,   430,   490,
    -287,  -287,  -287,   513,  -287,   263,  -287,   430,   457,   430,
    -287,  -287,   513,    72,   458,  -287
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     6,     0,     1,     0,
       0,     0,   114,     0,     9,    18,     0,     5,     0,     0,
       0,   113,     4,     0,     0,     0,     0,     0,     0,   118,
       0,     8,    14,     7,     0,     0,     9,     0,     0,     0,
       0,    17,    31,     0,     0,    11,    33,     0,    96,    13,
      85,     0,    96,     0,   134,     0,    12,    49,    20,     0,
      27,    96,    22,    96,    21,    10,     0,     0,    32,    15,
      98,    84,    63,    98,   116,   115,     0,     0,   118,     0,
       0,     0,   118,     0,     0,   135,   119,   122,   125,   137,
     124,     0,   126,   127,   128,   129,   130,   131,   132,   133,
      63,    48,    19,   118,   118,    29,    98,    98,    30,     0,
       0,    44,     0,     0,     0,    43,     0,    40,     0,     0,
      62,     0,     0,     0,     0,     0,    45,     0,    65,     0,
       0,     0,    51,    94,     0,   134,   153,     0,     0,     0,
       0,     0,     0,     0,     0,   193,   196,   194,   195,   149,
     192,   191,     0,   171,   179,   185,   149,   170,     0,     0,
       0,     0,   134,     0,     0,     0,   148,     0,     0,    25,
      26,   118,     0,     0,     0,    47,     0,    46,    41,     0,
      42,    38,    34,    37,    39,     0,     0,     0,     0,   100,
       0,    90,    52,     0,     0,     0,     0,    86,     0,     0,
      60,     0,    57,     9,     0,   117,     0,     0,   198,   180,
     181,   200,   204,     0,   202,     0,   199,     0,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,   156,   152,   120,
     123,     0,   141,   142,   145,   136,     0,   151,    50,    28,
       0,    23,     0,    35,    36,   106,     0,   106,     0,    97,
       0,     0,     9,    64,    68,    67,    66,     0,     0,    63,
       0,    74,     0,     0,     0,   165,     0,     0,   201,   197,
     147,   164,   134,   172,   173,   174,   175,   176,   177,   178,
     184,   182,   183,   188,   189,   190,   186,   187,   147,   168,
     169,   161,   154,     0,   138,     0,     0,     0,   146,    16,
       0,     0,   101,     0,     0,    99,   104,    88,     0,    87,
       0,    61,    59,     0,    79,    71,     0,    95,    92,     0,
       0,   205,   203,   121,   163,     0,   159,     0,   120,   140,
     143,     0,   150,    24,     0,     0,     0,     0,   108,     0,
     103,     0,    91,     0,     0,    58,    74,     0,    69,    72,
       0,     9,     0,     0,   161,   157,     0,   120,   155,     0,
     139,   106,     0,   106,     0,   105,     0,   102,     0,    53,
      70,     0,    63,    89,     0,   120,   120,   158,   162,   160,
     144,   111,     0,     0,   107,   109,     0,    54,    76,     0,
      73,    93,   167,   166,   110,     0,    63,    63,    83,     0,
     112,    56,    55,    78,    81,     0,    76,    83,     0,    83,
      80,    75,    77,    74,     0,    82
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -287,  -287,  -287,  -287,  -196,   484,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,   488,    37,   -66,   -72,   483,    29,   -98,
    -287,  -287,  -287,  -287,  -245,   101,  -287,   169,  -287,  -287,
     111,  -287,   109,   112,   492,    71,  -287,  -287,  -287,  -287,
     161,   -19,  -287,   268,  -246,  -287,   157,    -9,  -287,   -64,
    -287,  -287,  -226,  -287,  -125,   370,  -287,   -49,   228,   230,
    -287,   -51,  -287,  -287,  -287,  -287,  -287,  -287,  -287,   171,
    -286,  -287,  -287,  -287,  -287,  -287,   -75,   282,  -110,  -127,
    -287,   259
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    10,    18,    19,    31,     5,    20,    41,
     105,   172,    43,    45,    46,   129,   117,    56,    57,   130,
     397,   407,   271,   131,   132,   323,   324,   325,   382,   423,
     408,   358,   413,   414,    49,    50,    32,   262,   361,   203,
      70,   119,   188,   189,   312,   347,   348,    51,    33,    53,
     135,    54,   281,   282,    86,    87,    88,   150,   241,   242,
      90,   151,   246,    92,    93,    94,    95,   301,   335,   336,
     415,    96,    97,    98,    99,   158,   243,   153,   154,   155,
     213,   214
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   116,   168,    91,   152,    89,   159,   272,   161,   299,
     205,   314,   208,   302,   138,   337,    47,   216,   160,   258,
      65,   374,    69,   320,   133,   322,    59,    76,   197,   157,
     209,   210,   102,   398,   199,     6,   235,   178,   180,   169,
     170,    74,    77,    78,    79,    74,   185,    80,    81,   178,
     180,   344,   186,   356,   134,   187,   200,   345,   201,    82,
     346,   202,    83,    74,   399,    75,   318,   212,   215,   238,
       7,    66,   357,    16,   198,   364,   236,   259,   337,   375,
     207,   118,    68,    16,    91,   101,    89,   173,   174,   244,
      84,    12,   247,   101,    85,    68,    12,     9,   217,   365,
     166,   167,   293,   294,   295,   296,   297,   249,   378,   190,
       8,    91,   368,    89,   218,   193,   290,   291,   292,    76,
      71,   277,    16,   267,    72,   391,   278,   393,   264,   265,
     266,   274,   275,    71,    77,    78,    79,   319,   219,    80,
      81,   389,    12,  -149,   220,   221,   222,   223,   224,   225,
      16,    82,   260,   307,    83,     1,   353,   333,   308,   402,
     403,   354,     2,   120,  -149,   384,  -149,  -149,   226,   163,
     164,   321,   219,    21,  -147,   227,   228,   256,   220,   221,
     222,   223,   224,   225,   385,   300,    85,    11,    14,   121,
     122,   165,    15,   166,   167,  -147,   123,  -147,  -147,   124,
     306,   331,   212,   111,   125,   126,   127,   128,   219,    22,
     386,   121,   122,    73,   220,   221,   222,   223,   224,   225,
      42,   124,   106,    44,   107,   111,   125,   126,   127,   128,
     340,    91,   342,    89,   219,   334,   234,    48,   166,   167,
     220,   221,   222,   223,   224,   225,   329,    52,    55,   190,
     330,    16,    67,   100,   362,   363,    61,    63,    17,   229,
     230,   231,   326,    23,    24,   108,    25,    16,   118,   219,
     103,   232,   233,   136,   263,   220,   221,   222,   223,   224,
     225,    26,   303,    16,   400,   313,   137,    27,   303,   304,
      28,   104,   156,    30,   390,   370,    23,    24,   334,    25,
     388,    16,   349,   359,   162,   366,   139,   367,   411,   412,
      16,    16,   376,   392,    26,   171,   181,   140,   141,   366,
      27,   418,   182,    28,    29,   142,    30,   143,   183,   184,
     144,   145,   146,   147,   148,   149,   372,   111,   175,   176,
     177,   115,   334,   139,   191,   192,   206,   326,   194,   195,
     -43,   334,   196,   334,   140,   141,   139,   111,   175,   179,
     177,   115,   142,   211,   143,   349,   204,   144,   145,   146,
     147,   148,   149,   245,    34,   142,    35,   143,   163,    36,
     144,   145,   146,   147,   148,   149,   237,   248,   252,    74,
     250,    37,   111,   175,   126,   177,   115,    38,   251,   219,
      39,   253,   254,    40,   255,   220,   221,   222,   223,   224,
     225,   219,   257,   268,   326,   261,   269,   220,   221,   222,
     223,   224,   225,   219,   109,   110,   270,   273,   280,   220,
     221,   222,   223,   224,   225,   276,   239,   298,   111,   112,
     113,   114,   115,   309,   219,   310,   279,   311,   316,   306,
     220,   221,   222,   223,   224,   225,   219,   317,   327,   328,
     338,   343,   220,   221,   222,   223,   224,   225,   350,   219,
     351,   352,   355,   360,   305,   220,   221,   222,   223,   224,
     225,   371,   121,   122,   219,   373,   379,   369,   377,   381,
     220,   221,   222,   223,   224,   225,   111,   175,   126,   177,
     115,   283,   284,   285,   286,   287,   288,   289,   383,   395,
     396,   401,   404,   405,   406,   409,   410,   416,   417,   421,
      60,   425,    58,    64,   424,   380,   315,   419,   422,   420,
      62,   394,   240,   339,   341,   387,   332
};

static const yytype_int16 yycheck[] =
{
       9,    67,   100,    54,    79,    54,    81,   203,    83,   235,
     135,   257,   139,   239,    78,   301,    25,   144,    82,     5,
       5,     5,     5,   268,     5,   270,    35,     3,     5,    80,
     140,   141,     5,    27,     6,    70,    16,   109,   110,   103,
     104,     5,    18,    19,    20,     5,    25,    23,    24,   121,
     122,    25,    31,     5,    73,    34,    28,    31,    30,    35,
      34,    33,    38,     5,    58,    29,   262,   142,   143,    29,
      70,    56,    24,    56,    51,     5,    56,    63,   364,    63,
      22,    62,    45,    56,   135,    56,   135,   106,   107,   164,
      66,    70,   167,    64,    70,    58,    70,    62,    57,    29,
      59,    60,   229,   230,   231,   232,   233,   171,   353,   118,
       0,   162,   338,   162,    16,   124,   226,   227,   228,     3,
      49,    56,    56,   198,    58,   371,    61,   373,   194,   195,
     196,   206,   207,    62,    18,    19,    20,     5,    40,    23,
      24,   367,    70,    36,    46,    47,    48,    49,    50,    51,
      56,    35,    58,    56,    38,     4,    56,   282,    61,   385,
     386,    61,    11,    26,    57,   361,    59,    60,    45,    62,
      36,   269,    40,    70,    36,    52,    53,   186,    46,    47,
      48,    49,    50,    51,    16,   236,    70,     5,     5,    52,
      53,    57,    12,    59,    60,    57,    59,    59,    60,    62,
      62,   276,   277,    66,    67,    68,    69,    70,    40,    57,
      16,    52,    53,    52,    46,    47,    48,    49,    50,    51,
      66,    62,    61,    70,    63,    66,    67,    68,    69,    70,
     305,   282,   307,   282,    40,   301,    57,    70,    59,    60,
      46,    47,    48,    49,    50,    51,    17,    70,    70,   258,
      21,    56,    51,    51,   329,   330,    70,    70,    63,    42,
      43,    44,   271,     7,     8,    66,    10,    56,    62,    40,
      14,    54,    55,    66,    63,    46,    47,    48,    49,    50,
      51,    25,    56,    56,   382,    58,    70,    31,    56,    63,
      34,    35,    70,    37,   369,    63,     7,     8,   364,    10,
     366,    56,   311,    58,    58,    56,    41,    58,   406,   407,
      56,    56,    58,    58,    25,    15,     5,    52,    53,    56,
      31,    58,     5,    34,    35,    60,    37,    62,     5,     5,
      65,    66,    67,    68,    69,    70,   345,    66,    67,    68,
      69,    70,   408,    41,     5,    70,    36,   356,    64,    64,
      64,   417,    64,   419,    52,    53,    41,    66,    67,    68,
      69,    70,    60,    61,    62,   374,    58,    65,    66,    67,
      68,    69,    70,    70,     8,    60,    10,    62,    62,    13,
      65,    66,    67,    68,    69,    70,    27,     5,    58,     5,
      29,    25,    66,    67,    68,    69,    70,    31,     5,    40,
      34,     5,     5,    37,    70,    46,    47,    48,    49,    50,
      51,    40,    70,    60,   423,     9,    27,    46,    47,    48,
      49,    50,    51,    40,    52,    53,    27,    70,    70,    46,
      47,    48,    49,    50,    51,    64,    32,    70,    66,    67,
      68,    69,    70,    57,    40,    70,    63,    62,    70,    62,
      46,    47,    48,    49,    50,    51,    40,     5,     5,     5,
      39,     5,    46,    47,    48,    49,    50,    51,    70,    40,
      58,     5,    29,     9,    58,    46,    47,    48,    49,    50,
      51,    70,    52,    53,    40,    70,    27,    58,    70,    70,
      46,    47,    48,    49,    50,    51,    66,    67,    68,    69,
      70,   219,   220,   221,   222,   223,   224,   225,     5,    70,
      61,     5,    70,    58,    27,    70,    70,    27,     5,    62,
      36,    63,    34,    40,   423,   356,   258,   416,   419,   417,
      38,   374,   162,   303,   306,   364,   277
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    72,    73,    78,    70,    70,     0,    62,
      74,     5,    70,   118,     5,    12,    56,    63,    75,    76,
      79,    70,    57,     7,     8,    10,    25,    31,    34,    35,
      37,    77,   107,   119,     8,    10,    13,    25,    31,    34,
      37,    80,    66,    83,    70,    84,    85,   118,    70,   105,
     106,   118,    70,   120,   122,    70,    88,    89,    84,   118,
      76,    70,   105,    70,    88,     5,    56,    51,    85,     5,
     111,   106,    58,   111,     5,    29,     3,    18,    19,    20,
      23,    24,    35,    38,    66,    70,   125,   126,   127,   128,
     131,   132,   134,   135,   136,   137,   142,   143,   144,   145,
      51,    89,     5,    14,    35,    81,   111,   111,    66,    52,
      53,    66,    67,    68,    69,    70,    86,    87,    62,   112,
      26,    52,    53,    59,    62,    67,    68,    69,    70,    86,
      90,    94,    95,     5,   112,   121,    66,    70,   120,    41,
      52,    53,    60,    62,    65,    66,    67,    68,    69,    70,
     128,   132,   147,   148,   149,   150,    70,   132,   146,   147,
     120,   147,    58,    62,    36,    57,    59,    60,    90,   120,
     120,    15,    82,   112,   112,    67,    68,    69,    87,    68,
      87,     5,     5,     5,     5,    25,    31,    34,   113,   114,
     118,     5,    70,   118,    64,    64,    64,     5,    51,     6,
      28,    30,    33,   110,    58,   125,    36,    22,   150,   149,
     149,    61,   147,   151,   152,   147,   150,    57,    16,    40,
      46,    47,    48,    49,    50,    51,    45,    52,    53,    42,
      43,    44,    54,    55,    57,    16,    56,    27,    29,    32,
     126,   129,   130,   147,   147,    70,   133,   147,     5,   120,
      29,     5,    58,     5,     5,    70,   118,    70,     5,    63,
      58,     9,   108,    63,    86,    86,    86,   147,    60,    27,
      27,    93,    75,    70,   147,   147,    64,    56,    61,    63,
      70,   123,   124,   148,   148,   148,   148,   148,   148,   148,
     149,   149,   149,   150,   150,   150,   150,   150,    70,   123,
     132,   138,   123,    56,    63,    58,    62,    56,    61,    57,
      70,    62,   115,    58,   115,   114,    70,     5,    75,     5,
      95,    90,    95,    96,    97,    98,   118,     5,     5,    17,
      21,   147,   152,   125,    86,   139,   140,   141,    39,   130,
     147,   129,   147,     5,    25,    31,    34,   116,   117,   118,
      70,    58,     5,    56,    61,    29,     5,    24,   102,    58,
       9,   109,   147,   147,     5,    29,    56,    58,   123,    58,
      63,    70,   118,    70,     5,    63,    58,    70,    95,    27,
      98,    70,    99,     5,    75,    16,    16,   140,    86,   123,
     147,   115,    58,   115,   117,    70,    61,    91,    27,    58,
      90,     5,   123,   123,    70,    58,    27,    92,   101,    70,
      70,    90,    90,   103,   104,   141,    27,     5,    58,   101,
     104,    62,   103,   100,    96,    63
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    72,    73,    74,    74,    75,    76,    76,
      77,    77,    77,    77,    77,    77,    78,    79,    79,    80,
      80,    80,    80,    80,    80,    81,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    85,    85,
      86,    86,    86,    87,    87,    87,    87,    87,    88,    88,
      89,    90,    90,    91,    92,    90,    90,    93,    90,    90,
      90,    90,    94,    94,    95,    95,    95,    95,    95,    96,
      97,    97,    99,    98,    98,   100,   101,   102,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   107,   107,
     108,   107,   109,   107,   110,   107,   111,   112,   112,   113,
     113,   114,   114,   114,   114,   115,   115,   116,   116,   117,
     117,   117,   117,   118,   118,   119,   121,   120,   122,   120,
     124,   123,   125,   125,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   127,   127,   127,   128,   128,
     129,   129,   130,   130,   130,   131,   132,   132,   132,   132,
     133,   133,   134,   135,   136,   136,   138,   137,   139,   139,
     140,   140,   141,   141,   142,   143,   144,   144,   145,   146,
     146,   147,   147,   147,   147,   147,   147,   147,   147,   148,
     148,   148,   148,   148,   148,   149,   149,   149,   149,   149,
     149,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   151,   151,   152,   152
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     3,     0,     2,     2,     0,
       3,     2,     2,     2,     1,     3,    11,     2,     0,     3,
       2,     2,     2,     5,     7,     2,     2,     0,     2,     0,
       3,     1,     2,     1,     4,     5,     5,     4,     4,     4,
       1,     2,     2,     1,     1,     1,     1,     1,     2,     1,
       4,     1,     2,     0,     0,     9,     9,     0,     5,     4,
       2,     4,     1,     0,     3,     1,     3,     3,     3,     2,
       3,     1,     0,     4,     0,     0,     0,     7,     5,     0,
       3,     1,     6,     0,     2,     1,     4,     6,     7,     9,
       0,     8,     0,    10,     0,     7,     0,     3,     0,     3,
       1,     3,     5,     4,     3,     3,     0,     3,     1,     3,
       4,     3,     5,     3,     1,     3,     0,     4,     0,     2,
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,     1,     4,     6,
       3,     1,     1,     3,     5,     3,     4,     3,     2,     1,
       3,     1,     3,     2,     4,     6,     0,     6,     3,     1,
       3,     0,     3,     1,     4,     4,     8,     8,     4,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     3,     2,     2,
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
  case 2: /* compilation_unit: program  */
#line 152 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1656 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 153 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1662 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 156 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1668 "pascal.tab.c"
    break;

  case 5: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 159 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1674 "pascal.tab.c"
    break;

  case 6: /* file_id_list_opt: %empty  */
#line 160 "pascal.y"
     { (yyval.node) = NULL; }
#line 1680 "pascal.tab.c"
    break;

  case 7: /* block: decl_part_list body  */
#line 163 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1686 "pascal.tab.c"
    break;

  case 8: /* decl_part_list: decl_part_list decl_part  */
#line 166 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1692 "pascal.tab.c"
    break;

  case 9: /* decl_part_list: %empty  */
#line 167 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1698 "pascal.tab.c"
    break;

  case 10: /* decl_part: LABELSY label_list SEMICOLON  */
#line 170 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1704 "pascal.tab.c"
    break;

  case 11: /* decl_part: CONSTSY const_decl_list  */
#line 171 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1710 "pascal.tab.c"
    break;

  case 12: /* decl_part: TYPESY type_decl_list  */
#line 172 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1716 "pascal.tab.c"
    break;

  case 13: /* decl_part: VARSY var_decl_list  */
#line 173 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1722 "pascal.tab.c"
    break;

  case 14: /* decl_part: procedure_decl  */
#line 174 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1728 "pascal.tab.c"
    break;

  case 15: /* decl_part: USESSY id_list SEMICOLON  */
#line 175 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1734 "pascal.tab.c"
    break;

  case 16: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 178 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1740 "pascal.tab.c"
    break;

  case 17: /* intf_part_list: intf_part_list intf_part  */
#line 181 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1746 "pascal.tab.c"
    break;

  case 18: /* intf_part_list: %empty  */
#line 182 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1752 "pascal.tab.c"
    break;

  case 19: /* intf_part: USESSY id_list SEMICOLON  */
#line 185 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1758 "pascal.tab.c"
    break;

  case 20: /* intf_part: CONSTSY const_decl_list  */
#line 186 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1764 "pascal.tab.c"
    break;

  case 21: /* intf_part: TYPESY type_decl_list  */
#line 187 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1770 "pascal.tab.c"
    break;

  case 22: /* intf_part: VARSY var_decl_list  */
#line 188 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1776 "pascal.tab.c"
    break;

  case 23: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 189 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1782 "pascal.tab.c"
    break;

  case 24: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 190 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1788 "pascal.tab.c"
    break;

  case 25: /* init_opt: INITIALIZATIONSY statement_list  */
#line 193 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1794 "pascal.tab.c"
    break;

  case 26: /* init_opt: BEGINSY statement_list  */
#line 194 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1800 "pascal.tab.c"
    break;

  case 27: /* init_opt: %empty  */
#line 195 "pascal.y"
     { (yyval.node) = NULL; }
#line 1806 "pascal.tab.c"
    break;

  case 28: /* fini_opt: FINALIZATIONSY statement_list  */
#line 198 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1812 "pascal.tab.c"
    break;

  case 29: /* fini_opt: %empty  */
#line 199 "pascal.y"
     { (yyval.node) = NULL; }
#line 1818 "pascal.tab.c"
    break;

  case 30: /* label_list: label_list COMMA INTCONST  */
#line 202 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1824 "pascal.tab.c"
    break;

  case 31: /* label_list: INTCONST  */
#line 203 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1830 "pascal.tab.c"
    break;

  case 32: /* const_decl_list: const_decl_list const_decl  */
#line 206 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1836 "pascal.tab.c"
    break;

  case 33: /* const_decl_list: const_decl  */
#line 207 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1842 "pascal.tab.c"
    break;

  case 34: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 210 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_real((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1848 "pascal.tab.c"
    break;

  case 35: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 211 "pascal.y"
                                         { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_PLS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1854 "pascal.tab.c"
    break;

  case 36: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 212 "pascal.y"
                                          { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_MNS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, -(yyvsp[-1].node)->v.dval); }
#line 1860 "pascal.tab.c"
    break;

  case 37: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 213 "pascal.y"
                                      { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_string((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.sval); }
#line 1866 "pascal.tab.c"
    break;

  case 38: /* const_decl: IDENT EQOP CHARCODE SEMICOLON  */
#line 214 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.ival); }
#line 1872 "pascal.tab.c"
    break;

  case 39: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 215 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, cenv_value((yyvsp[-1].node))); }
#line 1878 "pascal.tab.c"
    break;

  case 40: /* constant: scalar_constant  */
#line 218 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1884 "pascal.tab.c"
    break;

  case 41: /* constant: PLUS scalar_constant  */
#line 219 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 1890 "pascal.tab.c"
    break;

  case 42: /* constant: MINUS scalar_constant  */
#line 220 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 1896 "pascal.tab.c"
    break;

  case 43: /* scalar_constant: IDENT  */
#line 223 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1902 "pascal.tab.c"
    break;

  case 44: /* scalar_constant: INTCONST  */
#line 224 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1908 "pascal.tab.c"
    break;

  case 45: /* scalar_constant: REALCONST  */
#line 225 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1914 "pascal.tab.c"
    break;

  case 46: /* scalar_constant: STRINGCONST  */
#line 226 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1920 "pascal.tab.c"
    break;

  case 47: /* scalar_constant: CHARCODE  */
#line 227 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1926 "pascal.tab.c"
    break;

  case 48: /* type_decl_list: type_decl_list type_decl  */
#line 230 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1932 "pascal.tab.c"
    break;

  case 49: /* type_decl_list: type_decl  */
#line 231 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 1938 "pascal.tab.c"
    break;

  case 50: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 234 "pascal.y"
                              { (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 1944 "pascal.tab.c"
    break;

  case 51: /* type: simple_type  */
#line 237 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1950 "pascal.tab.c"
    break;

  case 52: /* type: ARROW IDENT  */
#line 238 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 1956 "pascal.tab.c"
    break;

  case 53: /* $@1: %empty  */
#line 239 "pascal.y"
                                                       { }
#line 1962 "pascal.tab.c"
    break;

  case 54: /* $@2: %empty  */
#line 239 "pascal.y"
                                                           { }
#line 1968 "pascal.tab.c"
    break;

  case 55: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 239 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 1974 "pascal.tab.c"
    break;

  case 56: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 240 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1980 "pascal.tab.c"
    break;

  case 57: /* $@3: %empty  */
#line 241 "pascal.y"
                         { }
#line 1986 "pascal.tab.c"
    break;

  case 58: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 241 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 1992 "pascal.tab.c"
    break;

  case 59: /* type: packed_opt SETSY OFSY simple_type  */
#line 242 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1998 "pascal.tab.c"
    break;

  case 60: /* type: packed_opt FILESY  */
#line 243 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2004 "pascal.tab.c"
    break;

  case 61: /* type: packed_opt FILESY OFSY type  */
#line 244 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2010 "pascal.tab.c"
    break;

  case 62: /* packed_opt: PACKEDSY  */
#line 247 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2016 "pascal.tab.c"
    break;

  case 63: /* packed_opt: %empty  */
#line 248 "pascal.y"
     { (yyval.node) = NULL; }
#line 2022 "pascal.tab.c"
    break;

  case 64: /* simple_type: LPARENT id_list RPARENT  */
#line 251 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2028 "pascal.tab.c"
    break;

  case 65: /* simple_type: IDENT  */
#line 252 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2034 "pascal.tab.c"
    break;

  case 66: /* simple_type: constant DOTDOT constant  */
#line 253 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2040 "pascal.tab.c"
    break;

  case 67: /* simple_type: STRINGCONST DOTDOT constant  */
#line 254 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2046 "pascal.tab.c"
    break;

  case 68: /* simple_type: CHARCODE DOTDOT constant  */
#line 255 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2052 "pascal.tab.c"
    break;

  case 69: /* record_body: record_field_list record_case_opt  */
#line 258 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2058 "pascal.tab.c"
    break;

  case 70: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 261 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2064 "pascal.tab.c"
    break;

  case 71: /* record_field_list: record_field  */
#line 262 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2070 "pascal.tab.c"
    break;

  case 72: /* $@4: %empty  */
#line 265 "pascal.y"
                  { }
#line 2076 "pascal.tab.c"
    break;

  case 73: /* record_field: id_list COLON $@4 type  */
#line 265 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2082 "pascal.tab.c"
    break;

  case 74: /* record_field: %empty  */
#line 266 "pascal.y"
     { (yyval.node) = NULL; }
#line 2088 "pascal.tab.c"
    break;

  case 75: /* case_arm_mark: %empty  */
#line 269 "pascal.y"
    { }
#line 2094 "pascal.tab.c"
    break;

  case 76: /* case_open: %empty  */
#line 272 "pascal.y"
    { }
#line 2100 "pascal.tab.c"
    break;

  case 77: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 275 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2106 "pascal.tab.c"
    break;

  case 78: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 276 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2112 "pascal.tab.c"
    break;

  case 79: /* record_case_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2118 "pascal.tab.c"
    break;

  case 80: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 280 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2124 "pascal.tab.c"
    break;

  case 81: /* record_case_list: record_case_arm  */
#line 281 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2130 "pascal.tab.c"
    break;

  case 82: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 284 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2136 "pascal.tab.c"
    break;

  case 83: /* record_case_arm: %empty  */
#line 285 "pascal.y"
     { (yyval.node) = NULL; }
#line 2142 "pascal.tab.c"
    break;

  case 84: /* var_decl_list: var_decl_list var_decl  */
#line 288 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2148 "pascal.tab.c"
    break;

  case 85: /* var_decl_list: var_decl  */
#line 289 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2154 "pascal.tab.c"
    break;

  case 86: /* var_decl: id_list COLON type SEMICOLON  */
#line 292 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2160 "pascal.tab.c"
    break;

  case 87: /* var_decl: id_list COLON type EQOP expression SEMICOLON  */
#line 293 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2166 "pascal.tab.c"
    break;

  case 88: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 296 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2172 "pascal.tab.c"
    break;

  case 89: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 297 "pascal.y"
                                                                                           { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2178 "pascal.tab.c"
    break;

  case 90: /* $@5: %empty  */
#line 298 "pascal.y"
                                                            { }
#line 2184 "pascal.tab.c"
    break;

  case 91: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 298 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2190 "pascal.tab.c"
    break;

  case 92: /* $@6: %empty  */
#line 299 "pascal.y"
                                                                       { }
#line 2196 "pascal.tab.c"
    break;

  case 93: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 299 "pascal.y"
                                                                                           { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2202 "pascal.tab.c"
    break;

  case 94: /* $@7: %empty  */
#line 300 "pascal.y"
                                        { }
#line 2208 "pascal.tab.c"
    break;

  case 95: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 300 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2214 "pascal.tab.c"
    break;

  case 96: /* pv_mark: %empty  */
#line 303 "pascal.y"
    { }
#line 2220 "pascal.tab.c"
    break;

  case 97: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 306 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2226 "pascal.tab.c"
    break;

  case 98: /* parameter_list_opt: %empty  */
#line 307 "pascal.y"
     { (yyval.node) = NULL; }
#line 2232 "pascal.tab.c"
    break;

  case 99: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 310 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2238 "pascal.tab.c"
    break;

  case 100: /* parameter_decl_list: parameter_decl  */
#line 311 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2244 "pascal.tab.c"
    break;

  case 101: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 314 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2250 "pascal.tab.c"
    break;

  case 102: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 315 "pascal.y"
                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2256 "pascal.tab.c"
    break;

  case 103: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 316 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2262 "pascal.tab.c"
    break;

  case 104: /* parameter_decl: id_list COLON IDENT  */
#line 317 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2268 "pascal.tab.c"
    break;

  case 105: /* pf_params: LPARENT pf_sections RPARENT  */
#line 320 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2274 "pascal.tab.c"
    break;

  case 106: /* pf_params: %empty  */
#line 321 "pascal.y"
     { (yyval.node) = NULL; }
#line 2280 "pascal.tab.c"
    break;

  case 107: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 324 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2286 "pascal.tab.c"
    break;

  case 108: /* pf_sections: pf_section  */
#line 325 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2292 "pascal.tab.c"
    break;

  case 109: /* pf_section: id_list COLON IDENT  */
#line 328 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2298 "pascal.tab.c"
    break;

  case 110: /* pf_section: VARSY id_list COLON IDENT  */
#line 329 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2304 "pascal.tab.c"
    break;

  case 111: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 330 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2310 "pascal.tab.c"
    break;

  case 112: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 331 "pascal.y"
                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2316 "pascal.tab.c"
    break;

  case 113: /* id_list: id_list COMMA IDENT  */
#line 334 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2322 "pascal.tab.c"
    break;

  case 114: /* id_list: IDENT  */
#line 335 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2328 "pascal.tab.c"
    break;

  case 115: /* body: BEGINSY statement_list ENDSY  */
#line 338 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2334 "pascal.tab.c"
    break;

  case 116: /* $@8: %empty  */
#line 341 "pascal.y"
                             { pas_stmt_mark(); }
#line 2340 "pascal.tab.c"
    break;

  case 117: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 341 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2346 "pascal.tab.c"
    break;

  case 118: /* $@9: %empty  */
#line 342 "pascal.y"
     { pas_stmt_mark(); }
#line 2352 "pascal.tab.c"
    break;

  case 119: /* statement_list: $@9 statement  */
#line 342 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2358 "pascal.tab.c"
    break;

  case 120: /* $@10: %empty  */
#line 345 "pascal.y"
    { pas_stmt_mark(); }
#line 2364 "pascal.tab.c"
    break;

  case 121: /* body_stmt: $@10 statement  */
#line 345 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2370 "pascal.tab.c"
    break;

  case 122: /* statement: statement_no_label  */
#line 348 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2376 "pascal.tab.c"
    break;

  case 123: /* statement: INTCONST COLON statement_no_label  */
#line 349 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2382 "pascal.tab.c"
    break;

  case 124: /* statement_no_label: assignment  */
#line 352 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2388 "pascal.tab.c"
    break;

  case 125: /* statement_no_label: call  */
#line 353 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2394 "pascal.tab.c"
    break;

  case 126: /* statement_no_label: compound_statement  */
#line 354 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2400 "pascal.tab.c"
    break;

  case 127: /* statement_no_label: goto_statement  */
#line 355 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2406 "pascal.tab.c"
    break;

  case 128: /* statement_no_label: if_statement  */
#line 356 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2412 "pascal.tab.c"
    break;

  case 129: /* statement_no_label: case_statement  */
#line 357 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2418 "pascal.tab.c"
    break;

  case 130: /* statement_no_label: while_statement  */
#line 358 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2424 "pascal.tab.c"
    break;

  case 131: /* statement_no_label: repeat_statement  */
#line 359 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2430 "pascal.tab.c"
    break;

  case 132: /* statement_no_label: for_statement  */
#line 360 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2436 "pascal.tab.c"
    break;

  case 133: /* statement_no_label: with_statement  */
#line 361 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2442 "pascal.tab.c"
    break;

  case 134: /* statement_no_label: %empty  */
#line 362 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2448 "pascal.tab.c"
    break;

  case 135: /* call: IDENT  */
#line 365 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2454 "pascal.tab.c"
    break;

  case 136: /* call: selector PERIOD IDENT  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2460 "pascal.tab.c"
    break;

  case 137: /* call: call_with_args  */
#line 367 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2466 "pascal.tab.c"
    break;

  case 138: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 370 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2472 "pascal.tab.c"
    break;

  case 139: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 371 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2478 "pascal.tab.c"
    break;

  case 140: /* argument_list: argument_list COMMA argument  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2484 "pascal.tab.c"
    break;

  case 141: /* argument_list: argument  */
#line 375 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2490 "pascal.tab.c"
    break;

  case 142: /* argument: expression  */
#line 378 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2496 "pascal.tab.c"
    break;

  case 143: /* argument: expression COLON expression  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2502 "pascal.tab.c"
    break;

  case 144: /* argument: expression COLON expression COLON expression  */
#line 380 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2508 "pascal.tab.c"
    break;

  case 145: /* assignment: selector BECOMES expression  */
#line 383 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2514 "pascal.tab.c"
    break;

  case 146: /* selector: selector LBRACK expression_list RBRACK  */
#line 386 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2520 "pascal.tab.c"
    break;

  case 147: /* selector: selector PERIOD IDENT  */
#line 387 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2526 "pascal.tab.c"
    break;

  case 148: /* selector: selector ARROW  */
#line 388 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2532 "pascal.tab.c"
    break;

  case 149: /* selector: IDENT  */
#line 389 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2538 "pascal.tab.c"
    break;

  case 150: /* expression_list: expression_list COMMA expression  */
#line 392 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2544 "pascal.tab.c"
    break;

  case 151: /* expression_list: expression  */
#line 393 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2550 "pascal.tab.c"
    break;

  case 152: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 396 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2556 "pascal.tab.c"
    break;

  case 153: /* goto_statement: GOTOSY INTCONST  */
#line 399 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2562 "pascal.tab.c"
    break;

  case 154: /* if_statement: IFSY expression THENSY body_stmt  */
#line 402 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2568 "pascal.tab.c"
    break;

  case 155: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 403 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2574 "pascal.tab.c"
    break;

  case 156: /* $@11: %empty  */
#line 406 "pascal.y"
                           { }
#line 2580 "pascal.tab.c"
    break;

  case 157: /* case_statement: CASESY expression OFSY $@11 case_list ENDSY  */
#line 406 "pascal.y"
                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2586 "pascal.tab.c"
    break;

  case 158: /* case_list: case_list SEMICOLON case_elem  */
#line 409 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2592 "pascal.tab.c"
    break;

  case 159: /* case_list: case_elem  */
#line 410 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2598 "pascal.tab.c"
    break;

  case 160: /* case_elem: constant_list COLON body_stmt  */
#line 413 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2604 "pascal.tab.c"
    break;

  case 161: /* case_elem: %empty  */
#line 414 "pascal.y"
     { (yyval.node) = NULL; }
#line 2610 "pascal.tab.c"
    break;

  case 162: /* constant_list: constant_list COMMA constant  */
#line 417 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2616 "pascal.tab.c"
    break;

  case 163: /* constant_list: constant  */
#line 418 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2622 "pascal.tab.c"
    break;

  case 164: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 421 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2628 "pascal.tab.c"
    break;

  case 165: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 424 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2634 "pascal.tab.c"
    break;

  case 166: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 427 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 2640 "pascal.tab.c"
    break;

  case 167: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 428 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 2646 "pascal.tab.c"
    break;

  case 168: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 431 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2652 "pascal.tab.c"
    break;

  case 169: /* with_open: with_open COMMA selector  */
#line 434 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2658 "pascal.tab.c"
    break;

  case 170: /* with_open: selector  */
#line 435 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2664 "pascal.tab.c"
    break;

  case 171: /* expression: simple_expression  */
#line 442 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2670 "pascal.tab.c"
    break;

  case 172: /* expression: expression INOP simple_expression  */
#line 443 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2676 "pascal.tab.c"
    break;

  case 173: /* expression: expression LTOP simple_expression  */
#line 444 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2682 "pascal.tab.c"
    break;

  case 174: /* expression: expression LEOP simple_expression  */
#line 445 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2688 "pascal.tab.c"
    break;

  case 175: /* expression: expression GTOP simple_expression  */
#line 446 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2694 "pascal.tab.c"
    break;

  case 176: /* expression: expression GEOP simple_expression  */
#line 447 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2700 "pascal.tab.c"
    break;

  case 177: /* expression: expression NEOP simple_expression  */
#line 448 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2706 "pascal.tab.c"
    break;

  case 178: /* expression: expression EQOP simple_expression  */
#line 449 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2712 "pascal.tab.c"
    break;

  case 179: /* simple_expression: term  */
#line 452 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2718 "pascal.tab.c"
    break;

  case 180: /* simple_expression: PLUS term  */
#line 453 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2724 "pascal.tab.c"
    break;

  case 181: /* simple_expression: MINUS term  */
#line 454 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2730 "pascal.tab.c"
    break;

  case 182: /* simple_expression: simple_expression PLUS term  */
#line 455 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2736 "pascal.tab.c"
    break;

  case 183: /* simple_expression: simple_expression MINUS term  */
#line 456 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2742 "pascal.tab.c"
    break;

  case 184: /* simple_expression: simple_expression OROP term  */
#line 457 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2748 "pascal.tab.c"
    break;

  case 185: /* term: factor  */
#line 460 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2754 "pascal.tab.c"
    break;

  case 186: /* term: term MUL factor  */
#line 461 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2760 "pascal.tab.c"
    break;

  case 187: /* term: term RDIV factor  */
#line 462 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2766 "pascal.tab.c"
    break;

  case 188: /* term: term IDIV factor  */
#line 463 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2772 "pascal.tab.c"
    break;

  case 189: /* term: term IMOD factor  */
#line 464 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2778 "pascal.tab.c"
    break;

  case 190: /* term: term ANDOP factor  */
#line 465 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2784 "pascal.tab.c"
    break;

  case 191: /* factor: selector  */
#line 468 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2790 "pascal.tab.c"
    break;

  case 192: /* factor: call_with_args  */
#line 469 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2796 "pascal.tab.c"
    break;

  case 193: /* factor: INTCONST  */
#line 470 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2802 "pascal.tab.c"
    break;

  case 194: /* factor: REALCONST  */
#line 471 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2808 "pascal.tab.c"
    break;

  case 195: /* factor: STRINGCONST  */
#line 472 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2814 "pascal.tab.c"
    break;

  case 196: /* factor: CHARCODE  */
#line 473 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2820 "pascal.tab.c"
    break;

  case 197: /* factor: LPARENT expression RPARENT  */
#line 474 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2826 "pascal.tab.c"
    break;

  case 198: /* factor: NOTSY factor  */
#line 475 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 2832 "pascal.tab.c"
    break;

  case 199: /* factor: ATSIGN factor  */
#line 476 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 2838 "pascal.tab.c"
    break;

  case 200: /* factor: LBRACK RBRACK  */
#line 477 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 2844 "pascal.tab.c"
    break;

  case 201: /* factor: LBRACK set_member_list RBRACK  */
#line 478 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 2850 "pascal.tab.c"
    break;

  case 202: /* set_member_list: set_member  */
#line 481 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2856 "pascal.tab.c"
    break;

  case 203: /* set_member_list: set_member_list COMMA set_member  */
#line 482 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2862 "pascal.tab.c"
    break;

  case 204: /* set_member: expression  */
#line 485 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2868 "pascal.tab.c"
    break;

  case 205: /* set_member: expression DOTDOT expression  */
#line 486 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2874 "pascal.tab.c"
    break;


#line 2878 "pascal.tab.c"

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

#line 492 "pascal.y"

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
