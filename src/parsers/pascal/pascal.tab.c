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
  YYSYMBOL_pf_params = 123,                /* pf_params  */
  YYSYMBOL_pf_sections = 124,              /* pf_sections  */
  YYSYMBOL_pf_section = 125,               /* pf_section  */
  YYSYMBOL_id_list = 126,                  /* id_list  */
  YYSYMBOL_body = 127,                     /* body  */
  YYSYMBOL_statement_list = 128,           /* statement_list  */
  YYSYMBOL_129_8 = 129,                    /* $@8  */
  YYSYMBOL_130_9 = 130,                    /* $@9  */
  YYSYMBOL_body_stmt = 131,                /* body_stmt  */
  YYSYMBOL_132_10 = 132,                   /* $@10  */
  YYSYMBOL_statement = 133,                /* statement  */
  YYSYMBOL_statement_no_label = 134,       /* statement_no_label  */
  YYSYMBOL_call = 135,                     /* call  */
  YYSYMBOL_call_with_args = 136,           /* call_with_args  */
  YYSYMBOL_argument_list = 137,            /* argument_list  */
  YYSYMBOL_argument = 138,                 /* argument  */
  YYSYMBOL_assignment = 139,               /* assignment  */
  YYSYMBOL_selector = 140,                 /* selector  */
  YYSYMBOL_expression_list = 141,          /* expression_list  */
  YYSYMBOL_compound_statement = 142,       /* compound_statement  */
  YYSYMBOL_goto_statement = 143,           /* goto_statement  */
  YYSYMBOL_if_statement = 144,             /* if_statement  */
  YYSYMBOL_case_statement = 145,           /* case_statement  */
  YYSYMBOL_146_11 = 146,                   /* $@11  */
  YYSYMBOL_case_tail = 147,                /* case_tail  */
  YYSYMBOL_case_list = 148,                /* case_list  */
  YYSYMBOL_case_elem = 149,                /* case_elem  */
  YYSYMBOL_constant_list = 150,            /* constant_list  */
  YYSYMBOL_while_statement = 151,          /* while_statement  */
  YYSYMBOL_repeat_statement = 152,         /* repeat_statement  */
  YYSYMBOL_for_statement = 153,            /* for_statement  */
  YYSYMBOL_with_statement = 154,           /* with_statement  */
  YYSYMBOL_with_open = 155,                /* with_open  */
  YYSYMBOL_expression = 156,               /* expression  */
  YYSYMBOL_simple_expression = 157,        /* simple_expression  */
  YYSYMBOL_term = 158,                     /* term  */
  YYSYMBOL_factor = 159,                   /* factor  */
  YYSYMBOL_set_member_list = 160,          /* set_member_list  */
  YYSYMBOL_set_member = 161                /* set_member  */
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
#define YYLAST   600

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  86
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  456

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
       0,   166,   166,   167,   170,   173,   174,   177,   180,   181,
     184,   185,   186,   187,   188,   189,   192,   195,   196,   199,
     200,   201,   202,   203,   204,   207,   208,   209,   212,   213,
     216,   217,   220,   221,   224,   225,   228,   229,   230,   231,
     234,   235,   238,   239,   242,   243,   244,   247,   248,   249,
     250,   251,   254,   255,   258,   261,   262,   263,   263,   263,
     264,   265,   265,   266,   267,   268,   271,   272,   275,   276,
     277,   278,   279,   280,   283,   286,   287,   290,   290,   291,
     294,   297,   300,   301,   302,   305,   306,   309,   310,   313,
     314,   317,   318,   321,   322,   323,   323,   324,   324,   325,
     325,   328,   331,   332,   335,   336,   339,   340,   341,   342,
     345,   346,   349,   350,   353,   354,   355,   356,   359,   360,
     363,   366,   366,   367,   367,   370,   370,   373,   374,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     390,   391,   392,   395,   396,   397,   400,   401,   404,   405,
     406,   409,   410,   411,   412,   413,   416,   417,   418,   419,
     422,   423,   426,   429,   432,   433,   436,   436,   439,   440,
     441,   444,   445,   448,   449,   452,   453,   456,   459,   462,
     463,   466,   469,   470,   477,   478,   479,   480,   481,   482,
     483,   484,   487,   488,   489,   490,   491,   492,   495,   496,
     497,   498,   499,   500,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   516,   517,   520,   521
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
  "pf_params", "pf_sections", "pf_section", "id_list", "body",
  "statement_list", "$@8", "$@9", "body_stmt", "$@10", "statement",
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

#define YYPACT_NINF (-293)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-142)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     229,   -26,   -20,    61,  -293,  -293,    29,   101,  -293,    69,
     103,   144,  -293,   104,  -293,  -293,   109,  -293,   114,   454,
     469,  -293,  -293,   105,   115,    69,   126,    69,   169,  -293,
     171,  -293,  -293,  -293,   115,    69,  -293,   179,    69,   180,
     171,  -293,  -293,    19,   191,   115,  -293,    33,  -293,    69,
    -293,   214,  -293,   127,    20,   135,   171,  -293,   115,    42,
     449,  -293,    69,  -293,   171,  -293,   197,   342,   152,  -293,
    -293,   221,  -293,   152,    26,  -293,  -293,   222,   236,  -293,
     342,   242,   342,  -293,   342,   259,   112,  -293,  -293,  -293,
    -293,  -293,   113,  -293,  -293,  -293,  -293,  -293,  -293,  -293,
    -293,   152,  -293,  -293,  -293,  -293,   306,   221,   221,  -293,
     277,   277,   277,   293,   342,   277,  -293,  -293,  -293,  -293,
     261,  -293,   225,   148,   157,   186,  -293,  -293,   425,   425,
     257,    69,  -293,   268,  -293,   280,   235,   292,  -293,   308,
     185,  -293,    45,   356,    55,  -293,   319,    20,  -293,   345,
      49,   215,  -293,   276,    31,   419,   134,   510,    43,   327,
     342,   311,  -293,   342,   342,   342,   342,   342,   389,   392,
     392,  -293,   382,   405,   361,  -293,   186,   186,  -293,   461,
      72,  -293,   473,  -293,   346,  -293,   342,   342,   342,   342,
     342,   342,   342,   277,   277,   277,   277,   277,   277,   277,
     277,  -293,  -293,  -293,  -293,  -293,  -293,   194,   241,   241,
     241,   241,   365,   362,   396,   397,  -293,   354,    69,   355,
       6,  -293,   282,   418,  -293,   342,  -293,   357,  -293,   342,
     342,  -293,   360,  -293,   242,  -293,  -293,  -293,  -293,  -293,
     195,  -293,   522,   548,   125,   145,   548,   548,   548,   548,
     548,  -293,   392,   384,  -293,   370,   342,   342,  -293,  -293,
     385,   157,   157,   157,   157,   157,   157,   157,   186,   186,
     186,  -293,  -293,  -293,  -293,  -293,  -293,  -293,  -293,   387,
    -293,   380,   445,   548,   475,   152,   475,    69,   395,   285,
     395,    45,  -293,   390,   467,  -293,   187,   468,   471,   279,
     548,  -293,    20,  -293,  -293,   276,   241,   434,   342,  -293,
     342,   342,   342,  -293,  -293,   478,   548,  -293,  -293,   239,
     421,    10,   473,  -293,   160,  -293,  -293,   458,    32,  -293,
     320,   111,  -293,   416,   435,  -293,  -293,  -293,   488,  -293,
    -293,   494,   342,   342,  -293,  -293,    70,  -293,   326,  -293,
    -293,   535,   212,   548,  -293,   365,   365,    -6,  -293,   475,
     477,  -293,    69,   431,  -293,  -293,   440,    69,   441,    17,
    -293,   330,  -293,   442,  -293,   513,  -293,   240,   258,   241,
    -293,  -293,   489,   241,  -293,  -293,   342,  -293,  -293,  -293,
     213,  -293,   472,   465,  -293,  -293,    24,   152,   395,   347,
     395,   111,  -293,   455,  -293,  -293,   527,  -293,  -293,  -293,
     392,   392,  -293,  -293,  -293,   548,   365,  -293,   365,   505,
    -293,  -293,   459,  -293,  -293,   460,   480,  -293,  -293,  -293,
    -293,  -293,  -293,  -293,   152,   152,   241,   508,  -293,   466,
    -293,  -293,   537,  -293,   352,  -293,  -293,   241,   481,   241,
    -293,  -293,   537,    69,   476,  -293
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     6,     0,     1,     0,
       0,     0,   119,     0,     9,    18,     0,     5,     0,     0,
       0,   118,     4,     0,     0,     0,     0,     0,     0,   123,
       0,     8,    14,     7,     0,     0,     9,     0,     0,     0,
       0,    17,    31,     0,     0,    11,    33,     0,   101,    13,
      90,     0,   101,     0,   139,     0,    12,    53,    20,     0,
      27,   101,    22,   101,    21,    10,     0,     0,    67,    32,
      15,   103,    89,    67,   103,   121,   120,     0,     0,   123,
       0,     0,     0,   123,     0,     0,   159,   124,   127,   130,
     142,   129,     0,   131,   132,   133,   134,   135,   136,   137,
     138,    67,    52,    19,   123,   123,    29,   103,   103,    30,
       0,     0,     0,     0,     0,     0,   206,   209,   207,   208,
     159,   205,   204,     0,   184,   192,   198,    66,     0,     0,
       0,     0,    48,     0,    49,     0,    69,     0,    44,     0,
       0,    55,     0,     0,     0,    99,     0,   139,   163,     0,
       0,     0,   159,   183,     0,     0,     0,     0,   139,     0,
       0,     0,   158,     0,     0,     0,     0,     0,     0,    25,
      26,   123,     0,     0,     0,   211,   193,   194,   213,   217,
       0,   215,     0,   212,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    50,    47,    45,    46,    56,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    61,     0,     0,     0,
       0,   105,     0,    95,    91,     0,     9,     0,   122,     0,
       0,   125,     0,   125,     0,   166,   162,   125,   128,   144,
       0,   147,   148,   151,   157,     0,   161,   152,   153,   154,
     155,    54,    28,     0,    23,     0,     0,     0,   214,   210,
     157,   185,   186,   187,   188,   189,   190,   191,   197,   195,
     196,   201,   202,   203,   199,   200,    68,    73,    72,     0,
      71,     0,     0,    36,     0,    67,     0,    79,   111,     0,
     111,     0,   102,     0,     0,     9,     0,     0,     0,     0,
     178,   177,   139,   157,   181,   182,   174,   164,     0,   143,
       0,     0,     0,   156,    16,     0,   218,   216,    70,   159,
       0,     0,    36,    35,     0,    65,    63,     0,    84,    76,
       0,     0,   106,     0,     0,   104,   109,    93,     0,    92,
     100,    97,     0,     0,   126,   176,   168,   172,     0,   125,
     146,   149,     0,   160,    24,     0,     0,     0,    38,     0,
       0,    62,    79,     0,    74,    77,     0,     0,     0,     0,
     113,     0,   108,     0,    96,     0,     9,     0,     0,   174,
     123,   123,     0,     0,   125,   165,     0,   145,    42,    40,
       0,    39,     0,     0,    57,    75,     0,    67,   111,     0,
     111,     0,   110,     0,   107,    94,     0,   125,   125,   171,
     170,   169,   167,   175,   173,   150,     0,    37,     0,     0,
      58,    81,     0,    78,   116,     0,     0,   112,   114,    98,
     180,   179,    41,    43,    67,    67,    88,     0,   115,     0,
      60,    59,    83,    86,     0,    81,   117,    88,     0,    88,
      85,    80,    82,    79,     0,    87
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -293,  -293,  -293,  -293,  -217,   509,  -293,  -293,  -293,  -293,
    -293,  -293,  -293,   518,     8,  -271,  -293,  -293,  -175,    92,
     514,     9,   -72,  -293,  -293,  -293,  -293,  -266,   100,  -293,
     193,  -293,  -293,   119,  -293,   107,   118,   528,    89,  -293,
    -293,  -293,  -293,    97,    62,  -293,   284,  -277,  -293,   166,
      -9,  -293,   -77,  -293,  -293,  -230,  -293,  -139,   410,  -293,
     -42,   266,   270,  -293,   -49,  -293,  -293,  -293,  -293,  -293,
    -293,  -293,  -293,   200,  -292,  -293,  -293,  -293,  -293,  -293,
     -63,   184,   -69,   -85,  -293,   323
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    10,    18,    19,    31,     5,    20,    41,
     106,   172,    43,    45,    46,   282,   390,   321,   137,   138,
      56,    57,   139,   420,   435,   287,   140,   141,   327,   328,
     329,   397,   453,   436,   364,   442,   443,    49,    50,    32,
     295,   376,   226,    71,   143,   220,   221,   332,   369,   370,
      51,    33,    53,   147,    54,   301,   302,    87,    88,    89,
     121,   240,   241,    91,   122,   245,    93,    94,    95,    96,
     306,   382,   346,   347,   444,    97,    98,    99,   100,   154,
     283,   124,   125,   126,   180,   181
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   144,   150,   304,   123,    92,   156,   307,   228,   297,
     320,   291,    90,   334,   348,   357,    47,   151,   324,   155,
     326,   157,   401,    77,    65,   175,    59,   169,   170,   168,
     183,   145,   153,   277,   278,   279,   280,   362,    70,    78,
      79,    80,   176,   177,    81,    82,    77,   103,   233,     6,
     179,   182,   421,    69,    75,     7,    83,   363,   391,    84,
     224,     8,    78,    79,    80,   102,    69,    81,    82,   392,
     292,   217,   230,   102,   358,   379,    66,   218,   338,    83,
     219,   402,    84,   422,   388,   389,   380,   348,   234,   142,
      16,    85,     9,   393,   252,    86,   242,   243,    92,    16,
     246,   247,   248,   249,   250,    90,    11,   225,    14,    92,
     381,   271,   272,   273,   274,   275,    90,  -140,    86,   385,
      12,   424,   207,   426,   268,   269,   270,  -140,  -140,   257,
    -141,   345,    75,   222,   258,  -140,   146,   366,    72,    75,
    -141,  -141,  -140,   367,    12,   432,   368,   433,  -141,    74,
     160,    72,  -140,   185,   414,  -141,    15,    76,   107,   406,
     108,    16,   296,   344,   236,  -141,   299,   300,    17,   173,
     174,   161,    22,   162,   163,   159,    42,   430,   431,   127,
     164,   165,   166,   167,    21,   305,    12,   101,   311,   186,
      44,   213,   339,   316,   179,   187,   188,   189,   190,   191,
     192,    48,   312,   193,   345,   128,   129,   313,   413,   289,
     194,   195,   130,   325,   214,   131,   215,   359,   322,   216,
     204,   205,   360,   132,   133,   134,   135,   136,   186,   196,
     197,   198,   231,     1,   187,   188,   189,   190,   191,   192,
       2,   199,   200,    67,    52,   242,    55,   351,   242,   353,
      68,    16,   308,    92,    61,    63,   186,   407,   276,   309,
      90,   345,   187,   188,   189,   190,   191,   192,   109,   308,
     416,    16,   345,    73,   345,   408,   387,   417,   330,   377,
     378,   186,   222,   184,   142,   162,   163,   187,   188,   189,
     190,   191,   192,   148,   128,   129,   210,   342,   355,   186,
     -47,   343,   159,   410,   411,   187,   188,   189,   190,   191,
     192,   149,   132,   201,   134,   202,   203,   152,   158,   110,
     186,   171,   371,   415,   159,   423,   187,   188,   189,   190,
     191,   192,   206,   208,   232,   110,   162,   163,   113,    16,
     114,   293,    16,   115,   333,   209,   111,   112,   116,   117,
     118,   119,   120,   330,   113,   178,   114,   211,   399,   115,
     212,   223,   440,   441,   116,   117,   118,   119,   120,   110,
     261,   262,   263,   264,   265,   266,   267,    16,   227,   365,
     111,   112,   229,   383,   110,   384,   244,    16,   113,   403,
     114,   239,   371,   115,   251,   111,   112,    75,   116,   117,
     118,   119,   120,   113,    16,   114,   425,   110,   115,   383,
     254,   448,   253,   116,   117,   118,   119,   120,   111,   112,
     255,   260,   110,   284,   285,   286,   113,   294,   281,   288,
     290,   115,   298,   111,   112,   303,   116,   117,   118,   119,
     120,   113,   314,   281,   330,   315,   115,   235,   311,   318,
     323,   116,   117,   118,   119,   319,    23,    24,   331,    25,
     186,    23,    24,   104,    25,   336,   187,   188,   189,   190,
     191,   192,   337,   340,   349,    26,   341,    34,   356,    35,
      26,    27,    36,   354,    28,   105,    27,    30,   361,    28,
      29,   372,    30,   374,   373,    37,   132,   201,   134,   202,
     203,    38,   186,   375,    39,   394,   396,    40,   187,   188,
     189,   190,   191,   192,   186,   398,   400,   404,   405,   412,
     187,   188,   189,   190,   191,   192,   256,   419,   128,   129,
     428,   418,   429,   434,   437,   438,   445,   259,   131,   439,
     455,   446,   447,   237,   451,    60,   132,   133,   134,   135,
     136,   186,    58,   454,    64,   395,   452,   187,   188,   189,
     190,   191,   192,   186,   449,   450,    62,   427,   238,   187,
     188,   189,   190,   191,   192,   335,   186,   352,   350,   409,
     317,   310,   187,   188,   189,   190,   191,   192,     0,   186,
       0,     0,     0,     0,   386,   187,   188,   189,   190,   191,
     192
};

static const yytype_int16 yycheck[] =
{
       9,    73,    79,   233,    67,    54,    83,   237,   147,   226,
     281,     5,    54,   290,   306,     5,    25,    80,   284,    82,
     286,    84,     5,     3,     5,   110,    35,   104,   105,   101,
     115,     5,    81,   208,   209,   210,   211,     5,     5,    19,
      20,    21,   111,   112,    24,    25,     3,     5,    17,    75,
     113,   114,    28,    45,     5,    75,    36,    25,    64,    39,
       5,     0,    19,    20,    21,    56,    58,    24,    25,    75,
      64,    26,    23,    64,    64,     5,    57,    32,   295,    36,
      35,    64,    39,    59,   355,   356,    16,   379,    57,    63,
      57,    71,    63,   359,   171,    75,   159,   160,   147,    57,
     163,   164,   165,   166,   167,   147,     5,    52,     5,   158,
      40,   196,   197,   198,   199,   200,   158,     5,    75,   349,
      75,   398,   131,   400,   193,   194,   195,    15,    16,    57,
       5,   306,     5,   142,    62,    23,    74,    26,    49,     5,
      15,    16,    30,    32,    75,   416,    35,   418,    23,    52,
      37,    62,    40,     5,   384,    30,    12,    30,    61,   376,
      63,    57,   225,   302,    30,    40,   229,   230,    64,   107,
     108,    58,    58,    60,    61,    63,    71,   407,   408,    27,
      67,    68,    69,    70,    75,   234,    75,    52,    63,    41,
      75,     6,     5,   256,   257,    47,    48,    49,    50,    51,
      52,    75,    57,    46,   379,    53,    54,    62,   383,   218,
      53,    54,    60,   285,    29,    63,    31,    57,   281,    34,
     128,   129,    62,    71,    72,    73,    74,    75,    41,    43,
      44,    45,    17,     4,    47,    48,    49,    50,    51,    52,
      11,    55,    56,    52,    75,   308,    75,   310,   311,   312,
      59,    57,    57,   302,    75,    75,    41,    17,    64,    64,
     302,   436,    47,    48,    49,    50,    51,    52,    71,    57,
      57,    57,   447,    59,   449,    17,    64,    64,   287,   342,
     343,    41,   291,    58,    63,    60,    61,    47,    48,    49,
      50,    51,    52,    71,    53,    54,    61,    18,    59,    41,
      65,    22,    63,   380,   381,    47,    48,    49,    50,    51,
      52,    75,    71,    72,    73,    74,    75,    75,    59,    42,
      41,    15,   331,   386,    63,   397,    47,    48,    49,    50,
      51,    52,    75,    65,    58,    42,    60,    61,    61,    57,
      63,    59,    57,    66,    59,    65,    53,    54,    71,    72,
      73,    74,    75,   362,    61,    62,    63,    65,   367,    66,
      52,     5,   434,   435,    71,    72,    73,    74,    75,    42,
     186,   187,   188,   189,   190,   191,   192,    57,    59,    59,
      53,    54,    37,    57,    42,    59,    75,    57,    61,    59,
      63,    64,   401,    66,     5,    53,    54,     5,    71,    72,
      73,    74,    75,    61,    57,    63,    59,    42,    66,    57,
       5,    59,    30,    71,    72,    73,    74,    75,    53,    54,
      59,    75,    42,    61,    28,    28,    61,     9,    63,    75,
      75,    66,    75,    53,    54,    75,    71,    72,    73,    74,
      75,    61,    58,    63,   453,    75,    66,    28,    63,    62,
       5,    71,    72,    73,    74,    75,     7,     8,    63,    10,
      41,     7,     8,    14,    10,    75,    47,    48,    49,    50,
      51,    52,     5,     5,    40,    26,     5,     8,    57,    10,
      26,    32,    13,     5,    35,    36,    32,    38,    30,    35,
      36,    75,    38,     5,    59,    26,    71,    72,    73,    74,
      75,    32,    41,     9,    35,    28,    75,    38,    47,    48,
      49,    50,    51,    52,    41,    75,    75,    75,     5,    30,
      47,    48,    49,    50,    51,    52,    65,    62,    53,    54,
      75,    59,     5,    28,    75,    75,    28,    64,    63,    59,
      64,    75,     5,    33,    63,    36,    71,    72,    73,    74,
      75,    41,    34,   453,    40,   362,   449,    47,    48,    49,
      50,    51,    52,    41,   445,   447,    38,   401,   158,    47,
      48,    49,    50,    51,    52,   291,    41,   311,   308,   379,
     257,    59,    47,    48,    49,    50,    51,    52,    -1,    41,
      -1,    -1,    -1,    -1,    59,    47,    48,    49,    50,    51,
      52
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    77,    78,    83,    75,    75,     0,    63,
      79,     5,    75,   126,     5,    12,    57,    64,    80,    81,
      84,    75,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    82,   115,   127,     8,    10,    13,    26,    32,    35,
      38,    85,    71,    88,    75,    89,    90,   126,    75,   113,
     114,   126,    75,   128,   130,    75,    96,    97,    89,   126,
      81,    75,   113,    75,    96,     5,    57,    52,    59,    90,
       5,   119,   114,    59,   119,     5,    30,     3,    19,    20,
      21,    24,    25,    36,    39,    71,    75,   133,   134,   135,
     136,   139,   140,   142,   143,   144,   145,   151,   152,   153,
     154,    52,    97,     5,    14,    36,    86,   119,   119,    71,
      42,    53,    54,    61,    63,    66,    71,    72,    73,    74,
      75,   136,   140,   156,   157,   158,   159,    27,    53,    54,
      60,    63,    71,    72,    73,    74,    75,    94,    95,    98,
     102,   103,    63,   120,    98,     5,   120,   129,    71,    75,
     128,   156,    75,   140,   155,   156,   128,   156,    59,    63,
      37,    58,    60,    61,    67,    68,    69,    70,    98,   128,
     128,    15,    87,   120,   120,   159,   158,   158,    62,   156,
     160,   161,   156,   159,    58,     5,    41,    47,    48,    49,
      50,    51,    52,    46,    53,    54,    43,    44,    45,    55,
      56,    72,    74,    75,    95,    95,    75,   126,    65,    65,
      61,    65,    52,     6,    29,    31,    34,    26,    32,    35,
     121,   122,   126,     5,     5,    52,   118,    59,   133,    37,
      23,    17,    58,    17,    57,    28,    30,    33,   134,    64,
     137,   138,   156,   156,    75,   141,   156,   156,   156,   156,
     156,     5,   128,    30,     5,    59,    65,    57,    62,    64,
      75,   157,   157,   157,   157,   157,   157,   157,   158,   158,
     158,   159,   159,   159,   159,   159,    64,    94,    94,    94,
      94,    63,    91,   156,    61,    28,    28,   101,    75,   126,
      75,     5,    64,    59,     9,   116,   156,    80,    75,   156,
     156,   131,   132,    75,   131,   140,   146,   131,    57,    64,
      59,    63,    57,    62,    58,    75,   156,   161,    62,    75,
      91,    93,   156,     5,   103,    98,   103,   104,   105,   106,
     126,    63,   123,    59,   123,   122,    75,     5,    80,     5,
       5,     5,    18,    22,   133,    94,   148,   149,   150,    40,
     138,   156,   137,   156,     5,    59,    57,     5,    64,    57,
      62,    30,     5,    25,   110,    59,    26,    32,    35,   124,
     125,   126,    75,    59,     5,     9,   117,   156,   156,     5,
      16,    40,   147,    57,    59,   131,    59,    64,    91,    91,
      92,    64,    75,   103,    28,   106,    75,   107,    75,   126,
      75,     5,    64,    59,    75,     5,    80,    17,    17,   149,
     128,   128,    30,    94,   131,   156,    57,    64,    59,    62,
      99,    28,    59,    98,   123,    59,   123,   125,    75,     5,
     131,   131,    91,    91,    28,   100,   109,    75,    75,    59,
      98,    98,   111,   112,   150,    28,    75,     5,    59,   109,
     112,    63,   111,   108,   104,    64
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
      98,   101,    98,    98,    98,    98,   102,   102,   103,   103,
     103,   103,   103,   103,   104,   105,   105,   107,   106,   106,
     108,   109,   110,   110,   110,   111,   111,   112,   112,   113,
     113,   114,   114,   115,   115,   116,   115,   117,   115,   118,
     115,   119,   120,   120,   121,   121,   122,   122,   122,   122,
     123,   123,   124,   124,   125,   125,   125,   125,   126,   126,
     127,   129,   128,   130,   128,   132,   131,   133,   133,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     135,   135,   135,   136,   136,   136,   137,   137,   138,   138,
     138,   139,   139,   139,   139,   139,   140,   140,   140,   140,
     141,   141,   142,   143,   144,   144,   146,   145,   147,   147,
     147,   148,   148,   149,   149,   150,   150,   151,   152,   153,
     153,   154,   155,   155,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   157,   157,   157,   157,   157,   158,   158,
     158,   158,   158,   158,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   160,   160,   161,   161
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
       9,     0,     5,     4,     2,     4,     1,     0,     3,     1,
       4,     3,     3,     3,     2,     3,     1,     0,     4,     0,
       0,     0,     7,     5,     0,     3,     1,     6,     0,     2,
       1,     4,     6,     7,     9,     0,     8,     0,    10,     0,
       7,     0,     3,     0,     3,     1,     3,     5,     4,     3,
       3,     0,     3,     1,     3,     4,     3,     5,     3,     1,
       3,     0,     4,     0,     2,     0,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     3,     1,     4,     3,     6,     3,     1,     1,     3,
       5,     3,     3,     3,     3,     3,     4,     3,     2,     1,
       3,     1,     3,     2,     4,     6,     0,     7,     0,     2,
       2,     3,     1,     3,     0,     3,     1,     4,     4,     8,
       8,     4,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     2,     3,     1,     3,     1,     3
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
#line 166 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1702 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 167 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1708 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 170 "pascal.y"
                                                            { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1714 "pascal.tab.c"
    break;

  case 5: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 173 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1720 "pascal.tab.c"
    break;

  case 6: /* file_id_list_opt: %empty  */
#line 174 "pascal.y"
     { (yyval.node) = NULL; }
#line 1726 "pascal.tab.c"
    break;

  case 7: /* block: decl_part_list body  */
#line 177 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1732 "pascal.tab.c"
    break;

  case 8: /* decl_part_list: decl_part_list decl_part  */
#line 180 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1738 "pascal.tab.c"
    break;

  case 9: /* decl_part_list: %empty  */
#line 181 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1744 "pascal.tab.c"
    break;

  case 10: /* decl_part: LABELSY label_list SEMICOLON  */
#line 184 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1750 "pascal.tab.c"
    break;

  case 11: /* decl_part: CONSTSY const_decl_list  */
#line 185 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1756 "pascal.tab.c"
    break;

  case 12: /* decl_part: TYPESY type_decl_list  */
#line 186 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1762 "pascal.tab.c"
    break;

  case 13: /* decl_part: VARSY var_decl_list  */
#line 187 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1768 "pascal.tab.c"
    break;

  case 14: /* decl_part: procedure_decl  */
#line 188 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1774 "pascal.tab.c"
    break;

  case 15: /* decl_part: USESSY id_list SEMICOLON  */
#line 189 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1780 "pascal.tab.c"
    break;

  case 16: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 192 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1786 "pascal.tab.c"
    break;

  case 17: /* intf_part_list: intf_part_list intf_part  */
#line 195 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1792 "pascal.tab.c"
    break;

  case 18: /* intf_part_list: %empty  */
#line 196 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1798 "pascal.tab.c"
    break;

  case 19: /* intf_part: USESSY id_list SEMICOLON  */
#line 199 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1804 "pascal.tab.c"
    break;

  case 20: /* intf_part: CONSTSY const_decl_list  */
#line 200 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1810 "pascal.tab.c"
    break;

  case 21: /* intf_part: TYPESY type_decl_list  */
#line 201 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1816 "pascal.tab.c"
    break;

  case 22: /* intf_part: VARSY var_decl_list  */
#line 202 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1822 "pascal.tab.c"
    break;

  case 23: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 203 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1828 "pascal.tab.c"
    break;

  case 24: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 204 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1834 "pascal.tab.c"
    break;

  case 25: /* init_opt: INITIALIZATIONSY statement_list  */
#line 207 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1840 "pascal.tab.c"
    break;

  case 26: /* init_opt: BEGINSY statement_list  */
#line 208 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1846 "pascal.tab.c"
    break;

  case 27: /* init_opt: %empty  */
#line 209 "pascal.y"
     { (yyval.node) = NULL; }
#line 1852 "pascal.tab.c"
    break;

  case 28: /* fini_opt: FINALIZATIONSY statement_list  */
#line 212 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1858 "pascal.tab.c"
    break;

  case 29: /* fini_opt: %empty  */
#line 213 "pascal.y"
     { (yyval.node) = NULL; }
#line 1864 "pascal.tab.c"
    break;

  case 30: /* label_list: label_list COMMA INTCONST  */
#line 216 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1870 "pascal.tab.c"
    break;

  case 31: /* label_list: INTCONST  */
#line 217 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1876 "pascal.tab.c"
    break;

  case 32: /* const_decl_list: const_decl_list const_decl  */
#line 220 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1882 "pascal.tab.c"
    break;

  case 33: /* const_decl_list: const_decl  */
#line 221 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1888 "pascal.tab.c"
    break;

  case 34: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 224 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 1894 "pascal.tab.c"
    break;

  case 35: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 225 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1900 "pascal.tab.c"
    break;

  case 36: /* typed_value: expression  */
#line 228 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1906 "pascal.tab.c"
    break;

  case 37: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 229 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1912 "pascal.tab.c"
    break;

  case 38: /* typed_value: LPARENT field_value_list RPARENT  */
#line 230 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 1918 "pascal.tab.c"
    break;

  case 39: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 231 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 1924 "pascal.tab.c"
    break;

  case 40: /* typed_value_list: typed_value  */
#line 234 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 1930 "pascal.tab.c"
    break;

  case 41: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 235 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1936 "pascal.tab.c"
    break;

  case 42: /* field_value_list: IDENT COLON typed_value  */
#line 238 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 1942 "pascal.tab.c"
    break;

  case 43: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 239 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 1948 "pascal.tab.c"
    break;

  case 44: /* constant: scalar_constant  */
#line 242 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1954 "pascal.tab.c"
    break;

  case 45: /* constant: PLUS scalar_constant  */
#line 243 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 1960 "pascal.tab.c"
    break;

  case 46: /* constant: MINUS scalar_constant  */
#line 244 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 1966 "pascal.tab.c"
    break;

  case 47: /* scalar_constant: IDENT  */
#line 247 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1972 "pascal.tab.c"
    break;

  case 48: /* scalar_constant: INTCONST  */
#line 248 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1978 "pascal.tab.c"
    break;

  case 49: /* scalar_constant: REALCONST  */
#line 249 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1984 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: STRINGCONST  */
#line 250 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1990 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: CHARCODE  */
#line 251 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1996 "pascal.tab.c"
    break;

  case 52: /* type_decl_list: type_decl_list type_decl  */
#line 254 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2002 "pascal.tab.c"
    break;

  case 53: /* type_decl_list: type_decl  */
#line 255 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2008 "pascal.tab.c"
    break;

  case 54: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 258 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2014 "pascal.tab.c"
    break;

  case 55: /* type: simple_type  */
#line 261 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2020 "pascal.tab.c"
    break;

  case 56: /* type: ARROW IDENT  */
#line 262 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2026 "pascal.tab.c"
    break;

  case 57: /* $@1: %empty  */
#line 263 "pascal.y"
                                                       { }
#line 2032 "pascal.tab.c"
    break;

  case 58: /* $@2: %empty  */
#line 263 "pascal.y"
                                                           { }
#line 2038 "pascal.tab.c"
    break;

  case 59: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 263 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2044 "pascal.tab.c"
    break;

  case 60: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 264 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2050 "pascal.tab.c"
    break;

  case 61: /* $@3: %empty  */
#line 265 "pascal.y"
                         { }
#line 2056 "pascal.tab.c"
    break;

  case 62: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 265 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2062 "pascal.tab.c"
    break;

  case 63: /* type: packed_opt SETSY OFSY simple_type  */
#line 266 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2068 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt FILESY  */
#line 267 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2074 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt FILESY OFSY type  */
#line 268 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2080 "pascal.tab.c"
    break;

  case 66: /* packed_opt: PACKEDSY  */
#line 271 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2086 "pascal.tab.c"
    break;

  case 67: /* packed_opt: %empty  */
#line 272 "pascal.y"
     { (yyval.node) = NULL; }
#line 2092 "pascal.tab.c"
    break;

  case 68: /* simple_type: LPARENT id_list RPARENT  */
#line 275 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2098 "pascal.tab.c"
    break;

  case 69: /* simple_type: IDENT  */
#line 276 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2104 "pascal.tab.c"
    break;

  case 70: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 277 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2110 "pascal.tab.c"
    break;

  case 71: /* simple_type: constant DOTDOT constant  */
#line 278 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2116 "pascal.tab.c"
    break;

  case 72: /* simple_type: STRINGCONST DOTDOT constant  */
#line 279 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2122 "pascal.tab.c"
    break;

  case 73: /* simple_type: CHARCODE DOTDOT constant  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2128 "pascal.tab.c"
    break;

  case 74: /* record_body: record_field_list record_case_opt  */
#line 283 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2134 "pascal.tab.c"
    break;

  case 75: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 286 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2140 "pascal.tab.c"
    break;

  case 76: /* record_field_list: record_field  */
#line 287 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2146 "pascal.tab.c"
    break;

  case 77: /* $@4: %empty  */
#line 290 "pascal.y"
                  { }
#line 2152 "pascal.tab.c"
    break;

  case 78: /* record_field: id_list COLON $@4 type  */
#line 290 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2158 "pascal.tab.c"
    break;

  case 79: /* record_field: %empty  */
#line 291 "pascal.y"
     { (yyval.node) = NULL; }
#line 2164 "pascal.tab.c"
    break;

  case 80: /* case_arm_mark: %empty  */
#line 294 "pascal.y"
    { }
#line 2170 "pascal.tab.c"
    break;

  case 81: /* case_open: %empty  */
#line 297 "pascal.y"
    { }
#line 2176 "pascal.tab.c"
    break;

  case 82: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 300 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2182 "pascal.tab.c"
    break;

  case 83: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 301 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2188 "pascal.tab.c"
    break;

  case 84: /* record_case_opt: %empty  */
#line 302 "pascal.y"
     { (yyval.node) = NULL; }
#line 2194 "pascal.tab.c"
    break;

  case 85: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 305 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2200 "pascal.tab.c"
    break;

  case 86: /* record_case_list: record_case_arm  */
#line 306 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2206 "pascal.tab.c"
    break;

  case 87: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 309 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2212 "pascal.tab.c"
    break;

  case 88: /* record_case_arm: %empty  */
#line 310 "pascal.y"
     { (yyval.node) = NULL; }
#line 2218 "pascal.tab.c"
    break;

  case 89: /* var_decl_list: var_decl_list var_decl  */
#line 313 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2224 "pascal.tab.c"
    break;

  case 90: /* var_decl_list: var_decl  */
#line 314 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2230 "pascal.tab.c"
    break;

  case 91: /* var_decl: id_list COLON type SEMICOLON  */
#line 317 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2236 "pascal.tab.c"
    break;

  case 92: /* var_decl: id_list COLON type EQOP expression SEMICOLON  */
#line 318 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2242 "pascal.tab.c"
    break;

  case 93: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 321 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2248 "pascal.tab.c"
    break;

  case 94: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 322 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2254 "pascal.tab.c"
    break;

  case 95: /* $@5: %empty  */
#line 323 "pascal.y"
                                                            { }
#line 2260 "pascal.tab.c"
    break;

  case 96: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 323 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2266 "pascal.tab.c"
    break;

  case 97: /* $@6: %empty  */
#line 324 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); }
#line 2272 "pascal.tab.c"
    break;

  case 98: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 324 "pascal.y"
                                                                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2278 "pascal.tab.c"
    break;

  case 99: /* $@7: %empty  */
#line 325 "pascal.y"
                                        { }
#line 2284 "pascal.tab.c"
    break;

  case 100: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 325 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2290 "pascal.tab.c"
    break;

  case 101: /* pv_mark: %empty  */
#line 328 "pascal.y"
    { }
#line 2296 "pascal.tab.c"
    break;

  case 102: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 331 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2302 "pascal.tab.c"
    break;

  case 103: /* parameter_list_opt: %empty  */
#line 332 "pascal.y"
     { (yyval.node) = NULL; }
#line 2308 "pascal.tab.c"
    break;

  case 104: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 335 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2314 "pascal.tab.c"
    break;

  case 105: /* parameter_decl_list: parameter_decl  */
#line 336 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2320 "pascal.tab.c"
    break;

  case 106: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 339 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2326 "pascal.tab.c"
    break;

  case 107: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 340 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2332 "pascal.tab.c"
    break;

  case 108: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 341 "pascal.y"
                               { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2338 "pascal.tab.c"
    break;

  case 109: /* parameter_decl: id_list COLON IDENT  */
#line 342 "pascal.y"
                         { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2344 "pascal.tab.c"
    break;

  case 110: /* pf_params: LPARENT pf_sections RPARENT  */
#line 345 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2350 "pascal.tab.c"
    break;

  case 111: /* pf_params: %empty  */
#line 346 "pascal.y"
     { (yyval.node) = NULL; }
#line 2356 "pascal.tab.c"
    break;

  case 112: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 349 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2362 "pascal.tab.c"
    break;

  case 113: /* pf_sections: pf_section  */
#line 350 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2368 "pascal.tab.c"
    break;

  case 114: /* pf_section: id_list COLON IDENT  */
#line 353 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2374 "pascal.tab.c"
    break;

  case 115: /* pf_section: VARSY id_list COLON IDENT  */
#line 354 "pascal.y"
                               { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2380 "pascal.tab.c"
    break;

  case 116: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 355 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2386 "pascal.tab.c"
    break;

  case 117: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 356 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2392 "pascal.tab.c"
    break;

  case 118: /* id_list: id_list COMMA IDENT  */
#line 359 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2398 "pascal.tab.c"
    break;

  case 119: /* id_list: IDENT  */
#line 360 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2404 "pascal.tab.c"
    break;

  case 120: /* body: BEGINSY statement_list ENDSY  */
#line 363 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2410 "pascal.tab.c"
    break;

  case 121: /* $@8: %empty  */
#line 366 "pascal.y"
                             { pas_stmt_mark(); }
#line 2416 "pascal.tab.c"
    break;

  case 122: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 366 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2422 "pascal.tab.c"
    break;

  case 123: /* $@9: %empty  */
#line 367 "pascal.y"
     { pas_stmt_mark(); }
#line 2428 "pascal.tab.c"
    break;

  case 124: /* statement_list: $@9 statement  */
#line 367 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2434 "pascal.tab.c"
    break;

  case 125: /* $@10: %empty  */
#line 370 "pascal.y"
    { pas_stmt_mark(); }
#line 2440 "pascal.tab.c"
    break;

  case 126: /* body_stmt: $@10 statement  */
#line 370 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2446 "pascal.tab.c"
    break;

  case 127: /* statement: statement_no_label  */
#line 373 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2452 "pascal.tab.c"
    break;

  case 128: /* statement: INTCONST COLON statement_no_label  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2458 "pascal.tab.c"
    break;

  case 129: /* statement_no_label: assignment  */
#line 377 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2464 "pascal.tab.c"
    break;

  case 130: /* statement_no_label: call  */
#line 378 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2470 "pascal.tab.c"
    break;

  case 131: /* statement_no_label: compound_statement  */
#line 379 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2476 "pascal.tab.c"
    break;

  case 132: /* statement_no_label: goto_statement  */
#line 380 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2482 "pascal.tab.c"
    break;

  case 133: /* statement_no_label: if_statement  */
#line 381 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2488 "pascal.tab.c"
    break;

  case 134: /* statement_no_label: case_statement  */
#line 382 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2494 "pascal.tab.c"
    break;

  case 135: /* statement_no_label: while_statement  */
#line 383 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2500 "pascal.tab.c"
    break;

  case 136: /* statement_no_label: repeat_statement  */
#line 384 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2506 "pascal.tab.c"
    break;

  case 137: /* statement_no_label: for_statement  */
#line 385 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2512 "pascal.tab.c"
    break;

  case 138: /* statement_no_label: with_statement  */
#line 386 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2518 "pascal.tab.c"
    break;

  case 139: /* statement_no_label: %empty  */
#line 387 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2524 "pascal.tab.c"
    break;

  case 140: /* call: IDENT  */
#line 390 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2530 "pascal.tab.c"
    break;

  case 141: /* call: selector PERIOD IDENT  */
#line 391 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2536 "pascal.tab.c"
    break;

  case 142: /* call: call_with_args  */
#line 392 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2542 "pascal.tab.c"
    break;

  case 143: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 395 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2548 "pascal.tab.c"
    break;

  case 144: /* call_with_args: IDENT LPARENT RPARENT  */
#line 396 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 2554 "pascal.tab.c"
    break;

  case 145: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 397 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2560 "pascal.tab.c"
    break;

  case 146: /* argument_list: argument_list COMMA argument  */
#line 400 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2566 "pascal.tab.c"
    break;

  case 147: /* argument_list: argument  */
#line 401 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2572 "pascal.tab.c"
    break;

  case 148: /* argument: expression  */
#line 404 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2578 "pascal.tab.c"
    break;

  case 149: /* argument: expression COLON expression  */
#line 405 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2584 "pascal.tab.c"
    break;

  case 150: /* argument: expression COLON expression COLON expression  */
#line 406 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2590 "pascal.tab.c"
    break;

  case 151: /* assignment: selector BECOMES expression  */
#line 409 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2596 "pascal.tab.c"
    break;

  case 152: /* assignment: selector ADDBECOMES expression  */
#line 410 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 2602 "pascal.tab.c"
    break;

  case 153: /* assignment: selector SUBBECOMES expression  */
#line 411 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 2608 "pascal.tab.c"
    break;

  case 154: /* assignment: selector MULBECOMES expression  */
#line 412 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 2614 "pascal.tab.c"
    break;

  case 155: /* assignment: selector DIVBECOMES expression  */
#line 413 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 2620 "pascal.tab.c"
    break;

  case 156: /* selector: selector LBRACK expression_list RBRACK  */
#line 416 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2626 "pascal.tab.c"
    break;

  case 157: /* selector: selector PERIOD IDENT  */
#line 417 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2632 "pascal.tab.c"
    break;

  case 158: /* selector: selector ARROW  */
#line 418 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2638 "pascal.tab.c"
    break;

  case 159: /* selector: IDENT  */
#line 419 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2644 "pascal.tab.c"
    break;

  case 160: /* expression_list: expression_list COMMA expression  */
#line 422 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2650 "pascal.tab.c"
    break;

  case 161: /* expression_list: expression  */
#line 423 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2656 "pascal.tab.c"
    break;

  case 162: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 426 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2662 "pascal.tab.c"
    break;

  case 163: /* goto_statement: GOTOSY INTCONST  */
#line 429 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2668 "pascal.tab.c"
    break;

  case 164: /* if_statement: IFSY expression THENSY body_stmt  */
#line 432 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2674 "pascal.tab.c"
    break;

  case 165: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 433 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2680 "pascal.tab.c"
    break;

  case 166: /* $@11: %empty  */
#line 436 "pascal.y"
                           { }
#line 2686 "pascal.tab.c"
    break;

  case 167: /* case_statement: CASESY expression OFSY $@11 case_list case_tail ENDSY  */
#line 436 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2692 "pascal.tab.c"
    break;

  case 168: /* case_tail: %empty  */
#line 439 "pascal.y"
    { (yyval.node) = NULL; }
#line 2698 "pascal.tab.c"
    break;

  case 169: /* case_tail: ELSESY statement_list  */
#line 440 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 2704 "pascal.tab.c"
    break;

  case 170: /* case_tail: OTHERWISESY statement_list  */
#line 441 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 2710 "pascal.tab.c"
    break;

  case 171: /* case_list: case_list SEMICOLON case_elem  */
#line 444 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2716 "pascal.tab.c"
    break;

  case 172: /* case_list: case_elem  */
#line 445 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2722 "pascal.tab.c"
    break;

  case 173: /* case_elem: constant_list COLON body_stmt  */
#line 448 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2728 "pascal.tab.c"
    break;

  case 174: /* case_elem: %empty  */
#line 449 "pascal.y"
     { (yyval.node) = NULL; }
#line 2734 "pascal.tab.c"
    break;

  case 175: /* constant_list: constant_list COMMA constant  */
#line 452 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2740 "pascal.tab.c"
    break;

  case 176: /* constant_list: constant  */
#line 453 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2746 "pascal.tab.c"
    break;

  case 177: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 456 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2752 "pascal.tab.c"
    break;

  case 178: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 459 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2758 "pascal.tab.c"
    break;

  case 179: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 462 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 2764 "pascal.tab.c"
    break;

  case 180: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 463 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 2770 "pascal.tab.c"
    break;

  case 181: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 466 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2776 "pascal.tab.c"
    break;

  case 182: /* with_open: with_open COMMA selector  */
#line 469 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2782 "pascal.tab.c"
    break;

  case 183: /* with_open: selector  */
#line 470 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2788 "pascal.tab.c"
    break;

  case 184: /* expression: simple_expression  */
#line 477 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2794 "pascal.tab.c"
    break;

  case 185: /* expression: expression INOP simple_expression  */
#line 478 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2800 "pascal.tab.c"
    break;

  case 186: /* expression: expression LTOP simple_expression  */
#line 479 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2806 "pascal.tab.c"
    break;

  case 187: /* expression: expression LEOP simple_expression  */
#line 480 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2812 "pascal.tab.c"
    break;

  case 188: /* expression: expression GTOP simple_expression  */
#line 481 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2818 "pascal.tab.c"
    break;

  case 189: /* expression: expression GEOP simple_expression  */
#line 482 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2824 "pascal.tab.c"
    break;

  case 190: /* expression: expression NEOP simple_expression  */
#line 483 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2830 "pascal.tab.c"
    break;

  case 191: /* expression: expression EQOP simple_expression  */
#line 484 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2836 "pascal.tab.c"
    break;

  case 192: /* simple_expression: term  */
#line 487 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2842 "pascal.tab.c"
    break;

  case 193: /* simple_expression: PLUS term  */
#line 488 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2848 "pascal.tab.c"
    break;

  case 194: /* simple_expression: MINUS term  */
#line 489 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2854 "pascal.tab.c"
    break;

  case 195: /* simple_expression: simple_expression PLUS term  */
#line 490 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2860 "pascal.tab.c"
    break;

  case 196: /* simple_expression: simple_expression MINUS term  */
#line 491 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2866 "pascal.tab.c"
    break;

  case 197: /* simple_expression: simple_expression OROP term  */
#line 492 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2872 "pascal.tab.c"
    break;

  case 198: /* term: factor  */
#line 495 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2878 "pascal.tab.c"
    break;

  case 199: /* term: term MUL factor  */
#line 496 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2884 "pascal.tab.c"
    break;

  case 200: /* term: term RDIV factor  */
#line 497 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2890 "pascal.tab.c"
    break;

  case 201: /* term: term IDIV factor  */
#line 498 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2896 "pascal.tab.c"
    break;

  case 202: /* term: term IMOD factor  */
#line 499 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2902 "pascal.tab.c"
    break;

  case 203: /* term: term ANDOP factor  */
#line 500 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2908 "pascal.tab.c"
    break;

  case 204: /* factor: selector  */
#line 503 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2914 "pascal.tab.c"
    break;

  case 205: /* factor: call_with_args  */
#line 504 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2920 "pascal.tab.c"
    break;

  case 206: /* factor: INTCONST  */
#line 505 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2926 "pascal.tab.c"
    break;

  case 207: /* factor: REALCONST  */
#line 506 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2932 "pascal.tab.c"
    break;

  case 208: /* factor: STRINGCONST  */
#line 507 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2938 "pascal.tab.c"
    break;

  case 209: /* factor: CHARCODE  */
#line 508 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2944 "pascal.tab.c"
    break;

  case 210: /* factor: LPARENT expression RPARENT  */
#line 509 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2950 "pascal.tab.c"
    break;

  case 211: /* factor: NOTSY factor  */
#line 510 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 2956 "pascal.tab.c"
    break;

  case 212: /* factor: ATSIGN factor  */
#line 511 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 2962 "pascal.tab.c"
    break;

  case 213: /* factor: LBRACK RBRACK  */
#line 512 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 2968 "pascal.tab.c"
    break;

  case 214: /* factor: LBRACK set_member_list RBRACK  */
#line 513 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 2974 "pascal.tab.c"
    break;

  case 215: /* set_member_list: set_member  */
#line 516 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2980 "pascal.tab.c"
    break;

  case 216: /* set_member_list: set_member_list COMMA set_member  */
#line 517 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2986 "pascal.tab.c"
    break;

  case 217: /* set_member: expression  */
#line 520 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2992 "pascal.tab.c"
    break;

  case 218: /* set_member: expression DOTDOT expression  */
#line 521 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2998 "pascal.tab.c"
    break;


#line 3002 "pascal.tab.c"

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

#line 527 "pascal.y"

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
