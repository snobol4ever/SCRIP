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

#line 191 "pascal.tab.c"

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
  YYSYMBOL_INTCONST = 67,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 68,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 69,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 70,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 71,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 72,                  /* $accept  */
  YYSYMBOL_compilation_unit = 73,          /* compilation_unit  */
  YYSYMBOL_program = 74,                   /* program  */
  YYSYMBOL_file_id_list_opt = 75,          /* file_id_list_opt  */
  YYSYMBOL_block = 76,                     /* block  */
  YYSYMBOL_decl_part_list = 77,            /* decl_part_list  */
  YYSYMBOL_decl_part = 78,                 /* decl_part  */
  YYSYMBOL_unit = 79,                      /* unit  */
  YYSYMBOL_intf_part_list = 80,            /* intf_part_list  */
  YYSYMBOL_intf_part = 81,                 /* intf_part  */
  YYSYMBOL_init_opt = 82,                  /* init_opt  */
  YYSYMBOL_fini_opt = 83,                  /* fini_opt  */
  YYSYMBOL_label_list = 84,                /* label_list  */
  YYSYMBOL_const_decl_list = 85,           /* const_decl_list  */
  YYSYMBOL_const_decl = 86,                /* const_decl  */
  YYSYMBOL_constant = 87,                  /* constant  */
  YYSYMBOL_scalar_constant = 88,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 89,            /* type_decl_list  */
  YYSYMBOL_type_decl = 90,                 /* type_decl  */
  YYSYMBOL_type = 91,                      /* type  */
  YYSYMBOL_92_1 = 92,                      /* $@1  */
  YYSYMBOL_93_2 = 93,                      /* $@2  */
  YYSYMBOL_94_3 = 94,                      /* $@3  */
  YYSYMBOL_packed_opt = 95,                /* packed_opt  */
  YYSYMBOL_simple_type = 96,               /* simple_type  */
  YYSYMBOL_record_body = 97,               /* record_body  */
  YYSYMBOL_record_field_list = 98,         /* record_field_list  */
  YYSYMBOL_record_field = 99,              /* record_field  */
  YYSYMBOL_100_4 = 100,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 101,            /* case_arm_mark  */
  YYSYMBOL_case_open = 102,                /* case_open  */
  YYSYMBOL_record_case_opt = 103,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 104,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 105,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 106,            /* var_decl_list  */
  YYSYMBOL_var_decl = 107,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 108,           /* procedure_decl  */
  YYSYMBOL_109_5 = 109,                    /* $@5  */
  YYSYMBOL_110_6 = 110,                    /* $@6  */
  YYSYMBOL_111_7 = 111,                    /* $@7  */
  YYSYMBOL_pv_mark = 112,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 113,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 114,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 115,           /* parameter_decl  */
  YYSYMBOL_pf_params = 116,                /* pf_params  */
  YYSYMBOL_pf_sections = 117,              /* pf_sections  */
  YYSYMBOL_pf_section = 118,               /* pf_section  */
  YYSYMBOL_id_list = 119,                  /* id_list  */
  YYSYMBOL_body = 120,                     /* body  */
  YYSYMBOL_statement_list = 121,           /* statement_list  */
  YYSYMBOL_122_8 = 122,                    /* $@8  */
  YYSYMBOL_123_9 = 123,                    /* $@9  */
  YYSYMBOL_body_stmt = 124,                /* body_stmt  */
  YYSYMBOL_125_10 = 125,                   /* $@10  */
  YYSYMBOL_statement = 126,                /* statement  */
  YYSYMBOL_statement_no_label = 127,       /* statement_no_label  */
  YYSYMBOL_call = 128,                     /* call  */
  YYSYMBOL_call_with_args = 129,           /* call_with_args  */
  YYSYMBOL_argument_list = 130,            /* argument_list  */
  YYSYMBOL_argument = 131,                 /* argument  */
  YYSYMBOL_assignment = 132,               /* assignment  */
  YYSYMBOL_selector = 133,                 /* selector  */
  YYSYMBOL_expression_list = 134,          /* expression_list  */
  YYSYMBOL_compound_statement = 135,       /* compound_statement  */
  YYSYMBOL_goto_statement = 136,           /* goto_statement  */
  YYSYMBOL_if_statement = 137,             /* if_statement  */
  YYSYMBOL_case_statement = 138,           /* case_statement  */
  YYSYMBOL_139_11 = 139,                   /* $@11  */
  YYSYMBOL_case_tail = 140,                /* case_tail  */
  YYSYMBOL_case_list = 141,                /* case_list  */
  YYSYMBOL_case_elem = 142,                /* case_elem  */
  YYSYMBOL_constant_list = 143,            /* constant_list  */
  YYSYMBOL_while_statement = 144,          /* while_statement  */
  YYSYMBOL_repeat_statement = 145,         /* repeat_statement  */
  YYSYMBOL_for_statement = 146,            /* for_statement  */
  YYSYMBOL_with_statement = 147,           /* with_statement  */
  YYSYMBOL_with_open = 148,                /* with_open  */
  YYSYMBOL_expression = 149,               /* expression  */
  YYSYMBOL_simple_expression = 150,        /* simple_expression  */
  YYSYMBOL_term = 151,                     /* term  */
  YYSYMBOL_factor = 152,                   /* factor  */
  YYSYMBOL_set_member_list = 153,          /* set_member_list  */
  YYSYMBOL_set_member = 154                /* set_member  */
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
#define YYLAST   543

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  83
/* YYNRULES -- Number of rules.  */
#define YYNRULES  209
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  434

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   326


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
      65,    66,    67,    68,    69,    70,    71
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   158,   158,   159,   162,   165,   166,   169,   172,   173,
     176,   177,   178,   179,   180,   181,   184,   187,   188,   191,
     192,   193,   194,   195,   196,   199,   200,   201,   204,   205,
     208,   209,   212,   213,   216,   217,   218,   219,   220,   221,
     224,   225,   226,   229,   230,   231,   232,   233,   236,   237,
     240,   243,   244,   245,   245,   245,   246,   247,   247,   248,
     249,   250,   253,   254,   257,   258,   259,   260,   261,   262,
     265,   268,   269,   272,   272,   273,   276,   279,   282,   283,
     284,   287,   288,   291,   292,   295,   296,   299,   300,   303,
     304,   305,   305,   306,   306,   307,   307,   310,   313,   314,
     317,   318,   321,   322,   323,   324,   327,   328,   331,   332,
     335,   336,   337,   338,   341,   342,   345,   348,   348,   349,
     349,   352,   352,   355,   356,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   372,   373,   374,   377,
     378,   381,   382,   385,   386,   387,   390,   393,   394,   395,
     396,   399,   400,   403,   406,   409,   410,   413,   413,   416,
     417,   418,   421,   422,   425,   426,   429,   430,   433,   436,
     439,   440,   443,   446,   447,   454,   455,   456,   457,   458,
     459,   460,   461,   464,   465,   466,   467,   468,   469,   472,
     473,   474,   475,   476,   477,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   493,   494,   497,   498
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
  "RPARENT", "DOTDOT", "ATSIGN", "INTCONST", "CHARCODE", "REALCONST",
  "STRINGCONST", "IDENT", "$accept", "compilation_unit", "program",
  "file_id_list_opt", "block", "decl_part_list", "decl_part", "unit",
  "intf_part_list", "intf_part", "init_opt", "fini_opt", "label_list",
  "const_decl_list", "const_decl", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "$@1", "$@2", "$@3", "packed_opt",
  "simple_type", "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "pf_params", "pf_sections", "pf_section", "id_list",
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

#define YYPACT_NINF (-292)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-151)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     211,   -49,   -16,    79,  -292,  -292,    50,    95,  -292,    33,
     111,   115,  -292,    83,  -292,  -292,    89,  -292,   122,   300,
     314,  -292,  -292,    99,   124,    33,   127,    33,   147,  -292,
     180,  -292,  -292,  -292,   124,    33,  -292,   191,    33,   219,
     180,  -292,  -292,    20,   117,   124,  -292,    30,  -292,    33,
    -292,   162,  -292,    42,    17,   214,   180,  -292,   124,    34,
     285,  -292,    33,  -292,   180,  -292,   112,   216,  -292,  -292,
     228,  -292,   121,    23,  -292,  -292,   234,   242,  -292,   327,
     247,   327,  -292,   327,   246,    98,  -292,  -292,  -292,  -292,
    -292,   104,  -292,  -292,  -292,  -292,  -292,  -292,  -292,  -292,
     121,  -292,  -292,  -292,  -292,   297,   228,   228,  -292,   205,
     274,  -292,   328,   329,   332,  -292,   343,  -292,    26,   359,
    -292,   286,   286,   248,    33,   260,  -292,   313,   155,   319,
      28,   108,  -292,  -292,   306,    17,  -292,   349,    73,   340,
     340,   340,   305,   327,   340,  -292,  -292,  -292,  -292,   336,
    -292,    85,   159,   132,   158,  -292,  -292,   171,    37,   376,
      59,   421,    24,   327,   327,   321,  -292,   327,   386,   395,
     395,  -292,   372,   407,   346,  -292,   408,  -292,  -292,   409,
    -292,  -292,  -292,  -292,  -292,   344,    33,   345,    18,  -292,
     206,   410,  -292,   173,   389,   389,   389,   389,  -292,   327,
     357,   392,   393,  -292,  -292,   360,  -292,   327,   327,  -292,
     158,   158,  -292,   388,   232,  -292,   400,  -292,   361,  -292,
     327,   327,   327,   327,   327,   327,   327,   340,   340,   340,
     340,   340,   340,   340,   340,   362,  -292,   247,  -292,  -292,
    -292,  -292,   203,  -292,   433,   459,   136,   241,   459,  -292,
     395,   387,  -292,   363,  -292,  -292,   367,   271,   367,    26,
    -292,   373,   441,  -292,  -292,  -292,  -292,   399,  -292,   103,
     186,   121,   186,    33,   450,   458,   230,   459,   327,   327,
    -292,  -292,   402,  -292,    17,   132,   132,   132,   132,   132,
     132,   132,   158,   158,   158,  -292,  -292,  -292,  -292,  -292,
    -292,  -292,   171,   389,   426,   327,  -292,   327,   327,   327,
    -292,  -292,   462,    39,  -292,   404,   417,  -292,  -292,  -292,
     472,  -292,  -292,   252,  -292,  -292,   448,    41,  -292,   272,
    -292,   470,   327,   327,   459,  -292,  -292,  -292,    35,  -292,
     304,  -292,  -292,   446,   204,   459,  -292,   415,    33,   418,
      19,  -292,   320,  -292,   419,  -292,   186,   460,  -292,    33,
     420,  -292,  -292,   494,  -292,   176,   195,   389,  -292,  -292,
     471,   389,  -292,  -292,   327,  -292,   367,   326,   367,    39,
    -292,   431,  -292,   442,  -292,  -292,    22,   121,  -292,   498,
    -292,  -292,  -292,   395,   395,  -292,  -292,  -292,   459,  -292,
     449,   463,  -292,  -292,   491,  -292,  -292,   452,  -292,  -292,
    -292,  -292,  -292,   453,   121,   121,   389,   493,  -292,  -292,
    -292,   520,  -292,   330,  -292,   389,   464,   389,  -292,  -292,
     520,    33,   465,  -292
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     6,     0,     1,     0,
       0,     0,   115,     0,     9,    18,     0,     5,     0,     0,
       0,   114,     4,     0,     0,     0,     0,     0,     0,   119,
       0,     8,    14,     7,     0,     0,     9,     0,     0,     0,
       0,    17,    31,     0,     0,    11,    33,     0,    97,    13,
      86,     0,    97,     0,   135,     0,    12,    49,    20,     0,
      27,    97,    22,    97,    21,    10,     0,     0,    32,    15,
      99,    85,    63,    99,   117,   116,     0,     0,   119,     0,
       0,     0,   119,     0,     0,   136,   120,   123,   126,   138,
     125,     0,   127,   128,   129,   130,   131,   132,   133,   134,
      63,    48,    19,   119,   119,    29,    99,    99,    30,     0,
       0,    44,     0,     0,     0,    43,     0,    40,     0,     0,
      62,     0,     0,     0,     0,     0,    45,     0,    65,     0,
       0,     0,    51,    95,     0,   135,   154,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   200,   198,   199,   150,
     196,   195,     0,   175,   183,   189,   150,   174,     0,     0,
       0,     0,   135,     0,     0,     0,   149,     0,     0,    25,
      26,   119,     0,     0,     0,    47,     0,    46,    41,     0,
      42,    38,    34,    37,    39,     0,     0,     0,     0,   101,
       0,    91,    52,     0,     0,     0,     0,     0,    87,     0,
       0,    60,     0,    57,     9,     0,   118,     0,     0,   202,
     184,   185,   204,   208,     0,   206,     0,   203,     0,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,     0,   157,   153,
     121,   124,     0,   142,   143,   146,   137,     0,   152,    50,
      28,     0,    23,     0,    35,    36,   107,     0,   107,     0,
      98,     0,     0,     9,    64,    69,    68,     0,    67,     0,
       0,    63,     0,    75,     0,     0,     0,   169,     0,     0,
     205,   201,   148,   168,   135,   176,   177,   178,   179,   180,
     181,   182,   188,   186,   187,   192,   193,   194,   190,   191,
     148,   172,   173,   165,   155,     0,   139,     0,     0,     0,
     147,    16,     0,     0,   102,     0,     0,   100,   105,    89,
       0,    66,    88,     0,    61,    59,     0,    80,    72,     0,
      96,    93,     0,     0,   209,   207,   122,   167,   159,   163,
       0,   121,   141,   144,     0,   151,    24,     0,     0,     0,
       0,   109,     0,   104,     0,    92,     0,     0,    58,    75,
       0,    70,    73,     0,     9,     0,     0,   165,   119,   119,
       0,     0,   121,   156,     0,   140,   107,     0,   107,     0,
     106,     0,   103,     0,    53,    71,     0,    63,    90,     0,
     121,   121,   162,   161,   160,   158,   166,   164,   145,   112,
       0,     0,   108,   110,     0,    54,    77,     0,    74,    94,
     171,   170,   111,     0,    63,    63,    84,     0,   113,    56,
      55,    79,    82,     0,    77,    84,     0,    84,    81,    76,
      78,    75,     0,    83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -292,  -292,  -292,  -292,  -201,   490,  -292,  -292,  -292,  -292,
    -292,  -292,  -292,   496,    12,   -65,    61,   488,     3,   -99,
    -292,  -292,  -292,  -292,  -251,   100,  -292,   174,  -292,  -292,
     110,  -292,   105,   113,   497,    49,  -292,  -292,  -292,  -292,
      65,     0,  -292,   277,  -240,  -292,   160,    -9,  -292,   -72,
    -292,  -292,  -223,  -292,  -127,   375,  -292,   -43,   233,   235,
    -292,   -50,  -292,  -292,  -292,  -292,  -292,  -292,  -292,  -292,
     175,  -291,  -292,  -292,  -292,  -292,  -292,   -74,   292,  -126,
    -110,  -292,   264
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    10,    18,    19,    31,     5,    20,    41,
     105,   172,    43,    45,    46,   129,   117,    56,    57,   130,
     405,   415,   273,   131,   132,   326,   327,   328,   387,   431,
     416,   361,   421,   422,    49,    50,    32,   263,   364,   204,
      70,   119,   188,   189,   314,   350,   351,    51,    33,    53,
     135,    54,   283,   284,    86,    87,    88,   150,   242,   243,
      90,   151,   247,    92,    93,    94,    95,   303,   370,   338,
     339,   423,    96,    97,    98,    99,   158,   244,   153,   154,
     155,   214,   215
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   168,   116,   274,    91,   152,   138,   159,   206,   161,
     160,    89,   340,   301,   210,   211,    47,   304,   316,   323,
      76,   325,     6,   259,   379,    65,    59,    76,   133,   209,
     157,   169,   170,   198,   217,    69,    77,    78,    79,   102,
     367,    80,    81,    77,    78,    79,   359,    74,    80,    81,
     406,   368,   185,    82,   236,     7,    83,    68,   186,   101,
      82,   187,   320,    83,    74,   347,   360,   101,   213,   216,
      68,   348,    75,   134,   349,   369,   340,    66,    74,     8,
     199,   407,   260,   380,    84,    91,   118,    16,    85,   239,
     245,    16,    89,   248,   237,    85,   208,    12,    71,   250,
      11,   292,   293,   294,    12,   383,   173,   174,   322,   190,
      12,    71,    91,     9,   200,   193,    14,    73,   373,    89,
     295,   296,   297,   298,   299,   269,   106,    15,   107,   265,
     266,   267,   268,   276,   277,  -150,   399,   201,   401,   202,
      16,   164,   203,   218,   220,   166,   167,    17,   120,   397,
     221,   222,   223,   224,   225,   226,  -150,   336,  -150,  -150,
      21,   163,   165,   389,   166,   167,    42,   410,   411,    67,
     178,   180,   324,  -148,   121,   122,   219,   257,   227,   108,
      22,   123,   178,   180,   124,   228,   229,   302,   111,   125,
     126,   127,   128,   390,  -148,    44,  -148,  -148,    48,   308,
     220,   230,   231,   232,   334,   213,   221,   222,   223,   224,
     225,   226,   391,   233,   234,     1,   196,   220,    52,    16,
     -43,    72,     2,   221,   222,   223,   224,   225,   226,   235,
      16,   166,   167,   343,    91,   345,   220,   264,   337,   121,
     122,    89,   221,   222,   223,   224,   225,   226,   332,   124,
     190,    55,   333,   111,   125,   126,   127,   128,   365,   366,
     305,   305,    61,    16,   329,   261,   100,   306,   375,   109,
     110,   220,   111,   175,   176,   177,   115,   221,   222,   223,
     224,   225,   226,   111,   112,   113,   114,   115,   408,   279,
      63,   118,    23,    24,   280,    25,   393,   394,   309,   103,
     398,   136,   337,   310,   352,   162,   396,    23,    24,   356,
      25,    26,   171,   137,   357,   419,   420,    27,   156,   192,
      28,   104,    34,    30,    35,   194,    26,    36,    16,    16,
     315,   362,    27,   181,   182,    28,    29,   183,    30,   377,
      37,   111,   175,   179,   177,   115,    38,   139,   184,    39,
     329,   337,    40,   111,   175,   126,   177,   115,   140,   141,
     337,   371,   337,   372,   191,   205,   142,   212,   143,   139,
     352,   144,   145,   146,   147,   148,   149,    16,   195,   381,
     140,   141,   139,    16,   197,   400,   207,   371,   142,   426,
     143,   249,   246,   144,   145,   146,   147,   148,   149,   163,
      74,   142,   251,   143,   238,   253,   144,   145,   146,   147,
     148,   149,   252,   254,   255,   256,   258,   220,   270,   262,
     271,   272,   329,   221,   222,   223,   224,   225,   226,   220,
     313,   275,   282,   300,   312,   221,   222,   223,   224,   225,
     226,   220,   121,   122,   318,   311,   319,   221,   222,   223,
     224,   225,   226,   278,   240,   330,   111,   175,   126,   177,
     115,   321,   220,   331,   281,   308,   341,   346,   221,   222,
     223,   224,   225,   226,   220,   353,   354,   355,   358,   363,
     221,   222,   223,   224,   225,   226,   376,   220,   384,   378,
     382,   386,   307,   221,   222,   223,   224,   225,   226,   388,
     220,   395,   403,   409,   404,   374,   221,   222,   223,   224,
     225,   226,   285,   286,   287,   288,   289,   290,   291,   414,
     412,   424,   413,   417,   418,   425,    60,   429,    64,   433,
      58,   432,   430,   385,   427,    62,   317,   241,   428,   402,
     342,   344,   392,   335
};

static const yytype_int16 yycheck[] =
{
       9,   100,    67,   204,    54,    79,    78,    81,   135,    83,
      82,    54,   303,   236,   140,   141,    25,   240,   258,   270,
       3,   272,    71,     5,     5,     5,    35,     3,     5,   139,
      80,   103,   104,     5,   144,     5,    19,    20,    21,     5,
       5,    24,    25,    19,    20,    21,     5,     5,    24,    25,
      28,    16,    26,    36,    17,    71,    39,    45,    32,    56,
      36,    35,   263,    39,     5,    26,    25,    64,   142,   143,
      58,    32,    30,    73,    35,    40,   367,    57,     5,     0,
      52,    59,    64,    64,    67,   135,    63,    57,    71,    30,
     164,    57,   135,   167,    57,    71,    23,    71,    49,   171,
       5,   227,   228,   229,    71,   356,   106,   107,     5,   118,
      71,    62,   162,    63,     6,   124,     5,    52,   341,   162,
     230,   231,   232,   233,   234,   199,    61,    12,    63,   194,
     195,   196,   197,   207,   208,    37,   376,    29,   378,    31,
      57,    37,    34,    58,    41,    60,    61,    64,    27,   372,
      47,    48,    49,    50,    51,    52,    58,   284,    60,    61,
      71,    63,    58,   364,    60,    61,    67,   390,   391,    52,
     109,   110,   271,    37,    53,    54,    17,   186,    46,    67,
      58,    60,   121,   122,    63,    53,    54,   237,    67,    68,
      69,    70,    71,    17,    58,    71,    60,    61,    71,    63,
      41,    43,    44,    45,   278,   279,    47,    48,    49,    50,
      51,    52,    17,    55,    56,     4,    61,    41,    71,    57,
      65,    59,    11,    47,    48,    49,    50,    51,    52,    58,
      57,    60,    61,   307,   284,   309,    41,    64,   303,    53,
      54,   284,    47,    48,    49,    50,    51,    52,    18,    63,
     259,    71,    22,    67,    68,    69,    70,    71,   332,   333,
      57,    57,    71,    57,   273,    59,    52,    64,    64,    53,
      54,    41,    67,    68,    69,    70,    71,    47,    48,    49,
      50,    51,    52,    67,    68,    69,    70,    71,   387,    57,
      71,    63,     7,     8,    62,    10,   368,   369,    57,    14,
     374,    67,   367,    62,   313,    59,   371,     7,     8,    57,
      10,    26,    15,    71,    62,   414,   415,    32,    71,    71,
      35,    36,     8,    38,    10,    65,    26,    13,    57,    57,
      59,    59,    32,     5,     5,    35,    36,     5,    38,   348,
      26,    67,    68,    69,    70,    71,    32,    42,     5,    35,
     359,   416,    38,    67,    68,    69,    70,    71,    53,    54,
     425,    57,   427,    59,     5,    59,    61,    62,    63,    42,
     379,    66,    67,    68,    69,    70,    71,    57,    65,    59,
      53,    54,    42,    57,    65,    59,    37,    57,    61,    59,
      63,     5,    71,    66,    67,    68,    69,    70,    71,    63,
       5,    61,    30,    63,    28,    59,    66,    67,    68,    69,
      70,    71,     5,     5,     5,    71,    71,    41,    61,     9,
      28,    28,   431,    47,    48,    49,    50,    51,    52,    41,
      63,    71,    71,    71,    71,    47,    48,    49,    50,    51,
      52,    41,    53,    54,    71,    58,     5,    47,    48,    49,
      50,    51,    52,    65,    33,     5,    67,    68,    69,    70,
      71,    62,    41,     5,    64,    63,    40,     5,    47,    48,
      49,    50,    51,    52,    41,    71,    59,     5,    30,     9,
      47,    48,    49,    50,    51,    52,    71,    41,    28,    71,
      71,    71,    59,    47,    48,    49,    50,    51,    52,     5,
      41,    30,    71,     5,    62,    59,    47,    48,    49,    50,
      51,    52,   220,   221,   222,   223,   224,   225,   226,    28,
      71,    28,    59,    71,    71,     5,    36,    63,    40,    64,
      34,   431,   427,   359,   424,    38,   259,   162,   425,   379,
     305,   308,   367,   279
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    73,    74,    79,    71,    71,     0,    63,
      75,     5,    71,   119,     5,    12,    57,    64,    76,    77,
      80,    71,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    78,   108,   120,     8,    10,    13,    26,    32,    35,
      38,    81,    67,    84,    71,    85,    86,   119,    71,   106,
     107,   119,    71,   121,   123,    71,    89,    90,    85,   119,
      77,    71,   106,    71,    89,     5,    57,    52,    86,     5,
     112,   107,    59,   112,     5,    30,     3,    19,    20,    21,
      24,    25,    36,    39,    67,    71,   126,   127,   128,   129,
     132,   133,   135,   136,   137,   138,   144,   145,   146,   147,
      52,    90,     5,    14,    36,    82,   112,   112,    67,    53,
      54,    67,    68,    69,    70,    71,    87,    88,    63,   113,
      27,    53,    54,    60,    63,    68,    69,    70,    71,    87,
      91,    95,    96,     5,   113,   122,    67,    71,   121,    42,
      53,    54,    61,    63,    66,    67,    68,    69,    70,    71,
     129,   133,   149,   150,   151,   152,    71,   133,   148,   149,
     121,   149,    59,    63,    37,    58,    60,    61,    91,   121,
     121,    15,    83,   113,   113,    68,    69,    70,    88,    69,
      88,     5,     5,     5,     5,    26,    32,    35,   114,   115,
     119,     5,    71,   119,    65,    65,    61,    65,     5,    52,
       6,    29,    31,    34,   111,    59,   126,    37,    23,   152,
     151,   151,    62,   149,   153,   154,   149,   152,    58,    17,
      41,    47,    48,    49,    50,    51,    52,    46,    53,    54,
      43,    44,    45,    55,    56,    58,    17,    57,    28,    30,
      33,   127,   130,   131,   149,   149,    71,   134,   149,     5,
     121,    30,     5,    59,     5,     5,    71,   119,    71,     5,
      64,    59,     9,   109,    64,    87,    87,    87,    87,   149,
      61,    28,    28,    94,    76,    71,   149,   149,    65,    57,
      62,    64,    71,   124,   125,   150,   150,   150,   150,   150,
     150,   150,   151,   151,   151,   152,   152,   152,   152,   152,
      71,   124,   133,   139,   124,    57,    64,    59,    63,    57,
      62,    58,    71,    63,   116,    59,   116,   115,    71,     5,
      76,    62,     5,    96,    91,    96,    97,    98,    99,   119,
       5,     5,    18,    22,   149,   154,   126,    87,   141,   142,
     143,    40,   131,   149,   130,   149,     5,    26,    32,    35,
     117,   118,   119,    71,    59,     5,    57,    62,    30,     5,
      25,   103,    59,     9,   110,   149,   149,     5,    16,    40,
     140,    57,    59,   124,    59,    64,    71,   119,    71,     5,
      64,    59,    71,    96,    28,    99,    71,   100,     5,    76,
      17,    17,   142,   121,   121,    30,    87,   124,   149,   116,
      59,   116,   118,    71,    62,    92,    28,    59,    91,     5,
     124,   124,    71,    59,    28,    93,   102,    71,    71,    91,
      91,   104,   105,   143,    28,     5,    59,   102,   105,    63,
     104,   101,    97,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    74,    75,    75,    76,    77,    77,
      78,    78,    78,    78,    78,    78,    79,    80,    80,    81,
      81,    81,    81,    81,    81,    82,    82,    82,    83,    83,
      84,    84,    85,    85,    86,    86,    86,    86,    86,    86,
      87,    87,    87,    88,    88,    88,    88,    88,    89,    89,
      90,    91,    91,    92,    93,    91,    91,    94,    91,    91,
      91,    91,    95,    95,    96,    96,    96,    96,    96,    96,
      97,    98,    98,   100,    99,    99,   101,   102,   103,   103,
     103,   104,   104,   105,   105,   106,   106,   107,   107,   108,
     108,   109,   108,   110,   108,   111,   108,   112,   113,   113,
     114,   114,   115,   115,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   118,   119,   119,   120,   122,   121,   123,
     121,   125,   124,   126,   126,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   128,   128,   128,   129,
     129,   130,   130,   131,   131,   131,   132,   133,   133,   133,
     133,   134,   134,   135,   136,   137,   137,   139,   138,   140,
     140,   140,   141,   141,   142,   142,   143,   143,   144,   145,
     146,   146,   147,   148,   148,   149,   149,   149,   149,   149,
     149,   149,   149,   150,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   151,   151,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   153,   153,   154,   154
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
       2,     4,     1,     0,     3,     1,     4,     3,     3,     3,
       2,     3,     1,     0,     4,     0,     0,     0,     7,     5,
       0,     3,     1,     6,     0,     2,     1,     4,     6,     7,
       9,     0,     8,     0,    10,     0,     7,     0,     3,     0,
       3,     1,     3,     5,     4,     3,     3,     0,     3,     1,
       3,     4,     3,     5,     3,     1,     3,     0,     4,     0,
       2,     0,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     3,     1,     4,
       6,     3,     1,     1,     3,     5,     3,     4,     3,     2,
       1,     3,     1,     3,     2,     4,     6,     0,     7,     0,
       2,     2,     3,     1,     3,     0,     3,     1,     4,     4,
       8,     8,     4,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     1,     2,     2,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     3,     2,     2,     2,     3,     1,     3,     1,     3
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
#line 158 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1665 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 159 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1671 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 162 "pascal.y"
                                                            { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1677 "pascal.tab.c"
    break;

  case 5: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 165 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1683 "pascal.tab.c"
    break;

  case 6: /* file_id_list_opt: %empty  */
#line 166 "pascal.y"
     { (yyval.node) = NULL; }
#line 1689 "pascal.tab.c"
    break;

  case 7: /* block: decl_part_list body  */
#line 169 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1695 "pascal.tab.c"
    break;

  case 8: /* decl_part_list: decl_part_list decl_part  */
#line 172 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1701 "pascal.tab.c"
    break;

  case 9: /* decl_part_list: %empty  */
#line 173 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1707 "pascal.tab.c"
    break;

  case 10: /* decl_part: LABELSY label_list SEMICOLON  */
#line 176 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1713 "pascal.tab.c"
    break;

  case 11: /* decl_part: CONSTSY const_decl_list  */
#line 177 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1719 "pascal.tab.c"
    break;

  case 12: /* decl_part: TYPESY type_decl_list  */
#line 178 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1725 "pascal.tab.c"
    break;

  case 13: /* decl_part: VARSY var_decl_list  */
#line 179 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1731 "pascal.tab.c"
    break;

  case 14: /* decl_part: procedure_decl  */
#line 180 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1737 "pascal.tab.c"
    break;

  case 15: /* decl_part: USESSY id_list SEMICOLON  */
#line 181 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1743 "pascal.tab.c"
    break;

  case 16: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 184 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1749 "pascal.tab.c"
    break;

  case 17: /* intf_part_list: intf_part_list intf_part  */
#line 187 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1755 "pascal.tab.c"
    break;

  case 18: /* intf_part_list: %empty  */
#line 188 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1761 "pascal.tab.c"
    break;

  case 19: /* intf_part: USESSY id_list SEMICOLON  */
#line 191 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1767 "pascal.tab.c"
    break;

  case 20: /* intf_part: CONSTSY const_decl_list  */
#line 192 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1773 "pascal.tab.c"
    break;

  case 21: /* intf_part: TYPESY type_decl_list  */
#line 193 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1779 "pascal.tab.c"
    break;

  case 22: /* intf_part: VARSY var_decl_list  */
#line 194 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1785 "pascal.tab.c"
    break;

  case 23: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 195 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1791 "pascal.tab.c"
    break;

  case 24: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 196 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1797 "pascal.tab.c"
    break;

  case 25: /* init_opt: INITIALIZATIONSY statement_list  */
#line 199 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1803 "pascal.tab.c"
    break;

  case 26: /* init_opt: BEGINSY statement_list  */
#line 200 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1809 "pascal.tab.c"
    break;

  case 27: /* init_opt: %empty  */
#line 201 "pascal.y"
     { (yyval.node) = NULL; }
#line 1815 "pascal.tab.c"
    break;

  case 28: /* fini_opt: FINALIZATIONSY statement_list  */
#line 204 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1821 "pascal.tab.c"
    break;

  case 29: /* fini_opt: %empty  */
#line 205 "pascal.y"
     { (yyval.node) = NULL; }
#line 1827 "pascal.tab.c"
    break;

  case 30: /* label_list: label_list COMMA INTCONST  */
#line 208 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1833 "pascal.tab.c"
    break;

  case 31: /* label_list: INTCONST  */
#line 209 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1839 "pascal.tab.c"
    break;

  case 32: /* const_decl_list: const_decl_list const_decl  */
#line 212 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1845 "pascal.tab.c"
    break;

  case 33: /* const_decl_list: const_decl  */
#line 213 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1851 "pascal.tab.c"
    break;

  case 34: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 216 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_real((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1857 "pascal.tab.c"
    break;

  case 35: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 217 "pascal.y"
                                         { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_PLS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1863 "pascal.tab.c"
    break;

  case 36: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 218 "pascal.y"
                                          { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_MNS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, -(yyvsp[-1].node)->v.dval); }
#line 1869 "pascal.tab.c"
    break;

  case 37: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 219 "pascal.y"
                                      { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_string((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.sval); }
#line 1875 "pascal.tab.c"
    break;

  case 38: /* const_decl: IDENT EQOP CHARCODE SEMICOLON  */
#line 220 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.ival); }
#line 1881 "pascal.tab.c"
    break;

  case 39: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 221 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, cenv_value((yyvsp[-1].node))); }
#line 1887 "pascal.tab.c"
    break;

  case 40: /* constant: scalar_constant  */
#line 224 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1893 "pascal.tab.c"
    break;

  case 41: /* constant: PLUS scalar_constant  */
#line 225 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 1899 "pascal.tab.c"
    break;

  case 42: /* constant: MINUS scalar_constant  */
#line 226 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 1905 "pascal.tab.c"
    break;

  case 43: /* scalar_constant: IDENT  */
#line 229 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1911 "pascal.tab.c"
    break;

  case 44: /* scalar_constant: INTCONST  */
#line 230 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1917 "pascal.tab.c"
    break;

  case 45: /* scalar_constant: REALCONST  */
#line 231 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1923 "pascal.tab.c"
    break;

  case 46: /* scalar_constant: STRINGCONST  */
#line 232 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1929 "pascal.tab.c"
    break;

  case 47: /* scalar_constant: CHARCODE  */
#line 233 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1935 "pascal.tab.c"
    break;

  case 48: /* type_decl_list: type_decl_list type_decl  */
#line 236 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1941 "pascal.tab.c"
    break;

  case 49: /* type_decl_list: type_decl  */
#line 237 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 1947 "pascal.tab.c"
    break;

  case 50: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 240 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 1953 "pascal.tab.c"
    break;

  case 51: /* type: simple_type  */
#line 243 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1959 "pascal.tab.c"
    break;

  case 52: /* type: ARROW IDENT  */
#line 244 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 1965 "pascal.tab.c"
    break;

  case 53: /* $@1: %empty  */
#line 245 "pascal.y"
                                                       { }
#line 1971 "pascal.tab.c"
    break;

  case 54: /* $@2: %empty  */
#line 245 "pascal.y"
                                                           { }
#line 1977 "pascal.tab.c"
    break;

  case 55: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 245 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 1983 "pascal.tab.c"
    break;

  case 56: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 246 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1989 "pascal.tab.c"
    break;

  case 57: /* $@3: %empty  */
#line 247 "pascal.y"
                         { }
#line 1995 "pascal.tab.c"
    break;

  case 58: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 247 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2001 "pascal.tab.c"
    break;

  case 59: /* type: packed_opt SETSY OFSY simple_type  */
#line 248 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2007 "pascal.tab.c"
    break;

  case 60: /* type: packed_opt FILESY  */
#line 249 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2013 "pascal.tab.c"
    break;

  case 61: /* type: packed_opt FILESY OFSY type  */
#line 250 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2019 "pascal.tab.c"
    break;

  case 62: /* packed_opt: PACKEDSY  */
#line 253 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2025 "pascal.tab.c"
    break;

  case 63: /* packed_opt: %empty  */
#line 254 "pascal.y"
     { (yyval.node) = NULL; }
#line 2031 "pascal.tab.c"
    break;

  case 64: /* simple_type: LPARENT id_list RPARENT  */
#line 257 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2037 "pascal.tab.c"
    break;

  case 65: /* simple_type: IDENT  */
#line 258 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2043 "pascal.tab.c"
    break;

  case 66: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 259 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2049 "pascal.tab.c"
    break;

  case 67: /* simple_type: constant DOTDOT constant  */
#line 260 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2055 "pascal.tab.c"
    break;

  case 68: /* simple_type: STRINGCONST DOTDOT constant  */
#line 261 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2061 "pascal.tab.c"
    break;

  case 69: /* simple_type: CHARCODE DOTDOT constant  */
#line 262 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2067 "pascal.tab.c"
    break;

  case 70: /* record_body: record_field_list record_case_opt  */
#line 265 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2073 "pascal.tab.c"
    break;

  case 71: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 268 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2079 "pascal.tab.c"
    break;

  case 72: /* record_field_list: record_field  */
#line 269 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2085 "pascal.tab.c"
    break;

  case 73: /* $@4: %empty  */
#line 272 "pascal.y"
                  { }
#line 2091 "pascal.tab.c"
    break;

  case 74: /* record_field: id_list COLON $@4 type  */
#line 272 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2097 "pascal.tab.c"
    break;

  case 75: /* record_field: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2103 "pascal.tab.c"
    break;

  case 76: /* case_arm_mark: %empty  */
#line 276 "pascal.y"
    { }
#line 2109 "pascal.tab.c"
    break;

  case 77: /* case_open: %empty  */
#line 279 "pascal.y"
    { }
#line 2115 "pascal.tab.c"
    break;

  case 78: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 282 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2121 "pascal.tab.c"
    break;

  case 79: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 283 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2127 "pascal.tab.c"
    break;

  case 80: /* record_case_opt: %empty  */
#line 284 "pascal.y"
     { (yyval.node) = NULL; }
#line 2133 "pascal.tab.c"
    break;

  case 81: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 287 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2139 "pascal.tab.c"
    break;

  case 82: /* record_case_list: record_case_arm  */
#line 288 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2145 "pascal.tab.c"
    break;

  case 83: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 291 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2151 "pascal.tab.c"
    break;

  case 84: /* record_case_arm: %empty  */
#line 292 "pascal.y"
     { (yyval.node) = NULL; }
#line 2157 "pascal.tab.c"
    break;

  case 85: /* var_decl_list: var_decl_list var_decl  */
#line 295 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2163 "pascal.tab.c"
    break;

  case 86: /* var_decl_list: var_decl  */
#line 296 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2169 "pascal.tab.c"
    break;

  case 87: /* var_decl: id_list COLON type SEMICOLON  */
#line 299 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2175 "pascal.tab.c"
    break;

  case 88: /* var_decl: id_list COLON type EQOP expression SEMICOLON  */
#line 300 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2181 "pascal.tab.c"
    break;

  case 89: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 303 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2187 "pascal.tab.c"
    break;

  case 90: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 304 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2193 "pascal.tab.c"
    break;

  case 91: /* $@5: %empty  */
#line 305 "pascal.y"
                                                            { }
#line 2199 "pascal.tab.c"
    break;

  case 92: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 305 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2205 "pascal.tab.c"
    break;

  case 93: /* $@6: %empty  */
#line 306 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); }
#line 2211 "pascal.tab.c"
    break;

  case 94: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 306 "pascal.y"
                                                                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2217 "pascal.tab.c"
    break;

  case 95: /* $@7: %empty  */
#line 307 "pascal.y"
                                        { }
#line 2223 "pascal.tab.c"
    break;

  case 96: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 307 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2229 "pascal.tab.c"
    break;

  case 97: /* pv_mark: %empty  */
#line 310 "pascal.y"
    { }
#line 2235 "pascal.tab.c"
    break;

  case 98: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 313 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2241 "pascal.tab.c"
    break;

  case 99: /* parameter_list_opt: %empty  */
#line 314 "pascal.y"
     { (yyval.node) = NULL; }
#line 2247 "pascal.tab.c"
    break;

  case 100: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 317 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2253 "pascal.tab.c"
    break;

  case 101: /* parameter_decl_list: parameter_decl  */
#line 318 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2259 "pascal.tab.c"
    break;

  case 102: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 321 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2265 "pascal.tab.c"
    break;

  case 103: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 322 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2271 "pascal.tab.c"
    break;

  case 104: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 323 "pascal.y"
                               { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2277 "pascal.tab.c"
    break;

  case 105: /* parameter_decl: id_list COLON IDENT  */
#line 324 "pascal.y"
                         { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2283 "pascal.tab.c"
    break;

  case 106: /* pf_params: LPARENT pf_sections RPARENT  */
#line 327 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2289 "pascal.tab.c"
    break;

  case 107: /* pf_params: %empty  */
#line 328 "pascal.y"
     { (yyval.node) = NULL; }
#line 2295 "pascal.tab.c"
    break;

  case 108: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 331 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2301 "pascal.tab.c"
    break;

  case 109: /* pf_sections: pf_section  */
#line 332 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2307 "pascal.tab.c"
    break;

  case 110: /* pf_section: id_list COLON IDENT  */
#line 335 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2313 "pascal.tab.c"
    break;

  case 111: /* pf_section: VARSY id_list COLON IDENT  */
#line 336 "pascal.y"
                               { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2319 "pascal.tab.c"
    break;

  case 112: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 337 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2325 "pascal.tab.c"
    break;

  case 113: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 338 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2331 "pascal.tab.c"
    break;

  case 114: /* id_list: id_list COMMA IDENT  */
#line 341 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2337 "pascal.tab.c"
    break;

  case 115: /* id_list: IDENT  */
#line 342 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2343 "pascal.tab.c"
    break;

  case 116: /* body: BEGINSY statement_list ENDSY  */
#line 345 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2349 "pascal.tab.c"
    break;

  case 117: /* $@8: %empty  */
#line 348 "pascal.y"
                             { pas_stmt_mark(); }
#line 2355 "pascal.tab.c"
    break;

  case 118: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 348 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2361 "pascal.tab.c"
    break;

  case 119: /* $@9: %empty  */
#line 349 "pascal.y"
     { pas_stmt_mark(); }
#line 2367 "pascal.tab.c"
    break;

  case 120: /* statement_list: $@9 statement  */
#line 349 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2373 "pascal.tab.c"
    break;

  case 121: /* $@10: %empty  */
#line 352 "pascal.y"
    { pas_stmt_mark(); }
#line 2379 "pascal.tab.c"
    break;

  case 122: /* body_stmt: $@10 statement  */
#line 352 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2385 "pascal.tab.c"
    break;

  case 123: /* statement: statement_no_label  */
#line 355 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2391 "pascal.tab.c"
    break;

  case 124: /* statement: INTCONST COLON statement_no_label  */
#line 356 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2397 "pascal.tab.c"
    break;

  case 125: /* statement_no_label: assignment  */
#line 359 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2403 "pascal.tab.c"
    break;

  case 126: /* statement_no_label: call  */
#line 360 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2409 "pascal.tab.c"
    break;

  case 127: /* statement_no_label: compound_statement  */
#line 361 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2415 "pascal.tab.c"
    break;

  case 128: /* statement_no_label: goto_statement  */
#line 362 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2421 "pascal.tab.c"
    break;

  case 129: /* statement_no_label: if_statement  */
#line 363 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2427 "pascal.tab.c"
    break;

  case 130: /* statement_no_label: case_statement  */
#line 364 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2433 "pascal.tab.c"
    break;

  case 131: /* statement_no_label: while_statement  */
#line 365 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2439 "pascal.tab.c"
    break;

  case 132: /* statement_no_label: repeat_statement  */
#line 366 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2445 "pascal.tab.c"
    break;

  case 133: /* statement_no_label: for_statement  */
#line 367 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2451 "pascal.tab.c"
    break;

  case 134: /* statement_no_label: with_statement  */
#line 368 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2457 "pascal.tab.c"
    break;

  case 135: /* statement_no_label: %empty  */
#line 369 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2463 "pascal.tab.c"
    break;

  case 136: /* call: IDENT  */
#line 372 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2469 "pascal.tab.c"
    break;

  case 137: /* call: selector PERIOD IDENT  */
#line 373 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2475 "pascal.tab.c"
    break;

  case 138: /* call: call_with_args  */
#line 374 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2481 "pascal.tab.c"
    break;

  case 139: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 377 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2487 "pascal.tab.c"
    break;

  case 140: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 378 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2493 "pascal.tab.c"
    break;

  case 141: /* argument_list: argument_list COMMA argument  */
#line 381 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2499 "pascal.tab.c"
    break;

  case 142: /* argument_list: argument  */
#line 382 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2505 "pascal.tab.c"
    break;

  case 143: /* argument: expression  */
#line 385 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2511 "pascal.tab.c"
    break;

  case 144: /* argument: expression COLON expression  */
#line 386 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2517 "pascal.tab.c"
    break;

  case 145: /* argument: expression COLON expression COLON expression  */
#line 387 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2523 "pascal.tab.c"
    break;

  case 146: /* assignment: selector BECOMES expression  */
#line 390 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2529 "pascal.tab.c"
    break;

  case 147: /* selector: selector LBRACK expression_list RBRACK  */
#line 393 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2535 "pascal.tab.c"
    break;

  case 148: /* selector: selector PERIOD IDENT  */
#line 394 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2541 "pascal.tab.c"
    break;

  case 149: /* selector: selector ARROW  */
#line 395 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2547 "pascal.tab.c"
    break;

  case 150: /* selector: IDENT  */
#line 396 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2553 "pascal.tab.c"
    break;

  case 151: /* expression_list: expression_list COMMA expression  */
#line 399 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2559 "pascal.tab.c"
    break;

  case 152: /* expression_list: expression  */
#line 400 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2565 "pascal.tab.c"
    break;

  case 153: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 403 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2571 "pascal.tab.c"
    break;

  case 154: /* goto_statement: GOTOSY INTCONST  */
#line 406 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2577 "pascal.tab.c"
    break;

  case 155: /* if_statement: IFSY expression THENSY body_stmt  */
#line 409 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2583 "pascal.tab.c"
    break;

  case 156: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 410 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2589 "pascal.tab.c"
    break;

  case 157: /* $@11: %empty  */
#line 413 "pascal.y"
                           { }
#line 2595 "pascal.tab.c"
    break;

  case 158: /* case_statement: CASESY expression OFSY $@11 case_list case_tail ENDSY  */
#line 413 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2601 "pascal.tab.c"
    break;

  case 159: /* case_tail: %empty  */
#line 416 "pascal.y"
    { (yyval.node) = NULL; }
#line 2607 "pascal.tab.c"
    break;

  case 160: /* case_tail: ELSESY statement_list  */
#line 417 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 2613 "pascal.tab.c"
    break;

  case 161: /* case_tail: OTHERWISESY statement_list  */
#line 418 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 2619 "pascal.tab.c"
    break;

  case 162: /* case_list: case_list SEMICOLON case_elem  */
#line 421 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2625 "pascal.tab.c"
    break;

  case 163: /* case_list: case_elem  */
#line 422 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2631 "pascal.tab.c"
    break;

  case 164: /* case_elem: constant_list COLON body_stmt  */
#line 425 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2637 "pascal.tab.c"
    break;

  case 165: /* case_elem: %empty  */
#line 426 "pascal.y"
     { (yyval.node) = NULL; }
#line 2643 "pascal.tab.c"
    break;

  case 166: /* constant_list: constant_list COMMA constant  */
#line 429 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2649 "pascal.tab.c"
    break;

  case 167: /* constant_list: constant  */
#line 430 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2655 "pascal.tab.c"
    break;

  case 168: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 433 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2661 "pascal.tab.c"
    break;

  case 169: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 436 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2667 "pascal.tab.c"
    break;

  case 170: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 439 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 2673 "pascal.tab.c"
    break;

  case 171: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 440 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 2679 "pascal.tab.c"
    break;

  case 172: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 443 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2685 "pascal.tab.c"
    break;

  case 173: /* with_open: with_open COMMA selector  */
#line 446 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2691 "pascal.tab.c"
    break;

  case 174: /* with_open: selector  */
#line 447 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2697 "pascal.tab.c"
    break;

  case 175: /* expression: simple_expression  */
#line 454 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2703 "pascal.tab.c"
    break;

  case 176: /* expression: expression INOP simple_expression  */
#line 455 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2709 "pascal.tab.c"
    break;

  case 177: /* expression: expression LTOP simple_expression  */
#line 456 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2715 "pascal.tab.c"
    break;

  case 178: /* expression: expression LEOP simple_expression  */
#line 457 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2721 "pascal.tab.c"
    break;

  case 179: /* expression: expression GTOP simple_expression  */
#line 458 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2727 "pascal.tab.c"
    break;

  case 180: /* expression: expression GEOP simple_expression  */
#line 459 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2733 "pascal.tab.c"
    break;

  case 181: /* expression: expression NEOP simple_expression  */
#line 460 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2739 "pascal.tab.c"
    break;

  case 182: /* expression: expression EQOP simple_expression  */
#line 461 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2745 "pascal.tab.c"
    break;

  case 183: /* simple_expression: term  */
#line 464 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2751 "pascal.tab.c"
    break;

  case 184: /* simple_expression: PLUS term  */
#line 465 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2757 "pascal.tab.c"
    break;

  case 185: /* simple_expression: MINUS term  */
#line 466 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2763 "pascal.tab.c"
    break;

  case 186: /* simple_expression: simple_expression PLUS term  */
#line 467 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2769 "pascal.tab.c"
    break;

  case 187: /* simple_expression: simple_expression MINUS term  */
#line 468 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2775 "pascal.tab.c"
    break;

  case 188: /* simple_expression: simple_expression OROP term  */
#line 469 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2781 "pascal.tab.c"
    break;

  case 189: /* term: factor  */
#line 472 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2787 "pascal.tab.c"
    break;

  case 190: /* term: term MUL factor  */
#line 473 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2793 "pascal.tab.c"
    break;

  case 191: /* term: term RDIV factor  */
#line 474 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2799 "pascal.tab.c"
    break;

  case 192: /* term: term IDIV factor  */
#line 475 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2805 "pascal.tab.c"
    break;

  case 193: /* term: term IMOD factor  */
#line 476 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2811 "pascal.tab.c"
    break;

  case 194: /* term: term ANDOP factor  */
#line 477 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2817 "pascal.tab.c"
    break;

  case 195: /* factor: selector  */
#line 480 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2823 "pascal.tab.c"
    break;

  case 196: /* factor: call_with_args  */
#line 481 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2829 "pascal.tab.c"
    break;

  case 197: /* factor: INTCONST  */
#line 482 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2835 "pascal.tab.c"
    break;

  case 198: /* factor: REALCONST  */
#line 483 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2841 "pascal.tab.c"
    break;

  case 199: /* factor: STRINGCONST  */
#line 484 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2847 "pascal.tab.c"
    break;

  case 200: /* factor: CHARCODE  */
#line 485 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2853 "pascal.tab.c"
    break;

  case 201: /* factor: LPARENT expression RPARENT  */
#line 486 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2859 "pascal.tab.c"
    break;

  case 202: /* factor: NOTSY factor  */
#line 487 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 2865 "pascal.tab.c"
    break;

  case 203: /* factor: ATSIGN factor  */
#line 488 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 2871 "pascal.tab.c"
    break;

  case 204: /* factor: LBRACK RBRACK  */
#line 489 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 2877 "pascal.tab.c"
    break;

  case 205: /* factor: LBRACK set_member_list RBRACK  */
#line 490 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 2883 "pascal.tab.c"
    break;

  case 206: /* set_member_list: set_member  */
#line 493 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2889 "pascal.tab.c"
    break;

  case 207: /* set_member_list: set_member_list COMMA set_member  */
#line 494 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2895 "pascal.tab.c"
    break;

  case 208: /* set_member: expression  */
#line 497 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2901 "pascal.tab.c"
    break;

  case 209: /* set_member: expression DOTDOT expression  */
#line 498 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2907 "pascal.tab.c"
    break;


#line 2911 "pascal.tab.c"

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

#line 504 "pascal.y"

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
