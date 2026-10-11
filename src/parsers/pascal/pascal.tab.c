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
int pascal_lex_wrapped(void);
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
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; int unit_mode; int dialect; PasDefE *defs; int ndefs; int capdefs; int cond_skip; int cond_depth; int cond_taken; int cond_unknown; int macro_on; int string_declared; const char *mode_macro; unsigned sw_set; unsigned sw_on; unsigned *sw_save; int sw_n; int sw_cap; int last_tok; } g_pascal_cenv;
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
void pascal_cenv_reset(void) { g_pascal_cenv.last_tok = 0; g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; g_pascal_cenv.unit_mode = 0; g_pascal_cenv.dialect = PAS_DIALECT_ISO_DEFAULT; g_pascal_cenv.ndefs = 0; g_pascal_cenv.cond_skip = 0; g_pascal_cenv.cond_depth = 0; g_pascal_cenv.cond_taken = 0; g_pascal_cenv.cond_unknown = 0; g_pascal_cenv.macro_on = 0; g_pascal_cenv.string_declared = 0; g_pascal_cenv.mode_macro = NULL; g_pascal_cenv.sw_set = 0; g_pascal_cenv.sw_on = 0; g_pascal_cenv.sw_n = 0; }
int pascal_unit_mode(void) { return g_pascal_cenv.unit_mode; }
void pascal_unit_mode_set(int m) { g_pascal_cenv.unit_mode = m; }
int pascal_dialect(void) { return g_pascal_cenv.dialect; }
int pascal_lex_wrapped(void) {
#undef pascal_yylex
    int t = pascal_yylex();
#define pascal_yylex pascal_lex_wrapped
    pas_stmt_line_fill();
    int lt = g_pascal_cenv.last_tok;
    if (t == IDENT && pascal_yylval.node && pascal_yylval.node->v.sval && (lt == IDENT || lt == INTCONST || lt == REALCONST || lt == STRINGCONST || lt == CHARCODE || lt == RPARENT || lt == RBRACK)) {
        const char *w = pascal_yylval.node->v.sval;
        if (!strcmp(w, "shl") || !strcmp(w, "shr")) { pascal_flip_dialect(PAS_DIALECT_FPC); t = SHIFTOP; }
        else if (!strcmp(w, "xor")) { pascal_flip_dialect(PAS_DIALECT_FPC); t = XOROP; }
    }
    g_pascal_cenv.last_tok = t;
    return t;
}
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

#line 211 "pascal.tab.c"

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
  YYSYMBOL_SHIFTOP = 76,                   /* SHIFTOP  */
  YYSYMBOL_XOROP = 77,                     /* XOROP  */
  YYSYMBOL_POWOP = 78,                     /* POWOP  */
  YYSYMBOL_YYACCEPT = 79,                  /* $accept  */
  YYSYMBOL_compilation_unit = 80,          /* compilation_unit  */
  YYSYMBOL_program = 81,                   /* program  */
  YYSYMBOL_program_name = 82,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 83,          /* file_id_list_opt  */
  YYSYMBOL_block = 84,                     /* block  */
  YYSYMBOL_decl_part_list = 85,            /* decl_part_list  */
  YYSYMBOL_decl_part = 86,                 /* decl_part  */
  YYSYMBOL_unit = 87,                      /* unit  */
  YYSYMBOL_intf_part_list = 88,            /* intf_part_list  */
  YYSYMBOL_intf_part = 89,                 /* intf_part  */
  YYSYMBOL_init_opt = 90,                  /* init_opt  */
  YYSYMBOL_fini_opt = 91,                  /* fini_opt  */
  YYSYMBOL_label_list = 92,                /* label_list  */
  YYSYMBOL_const_decl_list = 93,           /* const_decl_list  */
  YYSYMBOL_const_decl = 94,                /* const_decl  */
  YYSYMBOL_typed_value = 95,               /* typed_value  */
  YYSYMBOL_typed_value_list = 96,          /* typed_value_list  */
  YYSYMBOL_field_value_list = 97,          /* field_value_list  */
  YYSYMBOL_constant = 98,                  /* constant  */
  YYSYMBOL_scalar_constant = 99,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 100,           /* type_decl_list  */
  YYSYMBOL_type_decl = 101,                /* type_decl  */
  YYSYMBOL_type = 102,                     /* type  */
  YYSYMBOL_103_1 = 103,                    /* $@1  */
  YYSYMBOL_104_2 = 104,                    /* $@2  */
  YYSYMBOL_105_3 = 105,                    /* $@3  */
  YYSYMBOL_packed_opt = 106,               /* packed_opt  */
  YYSYMBOL_simple_type = 107,              /* simple_type  */
  YYSYMBOL_lo_bound = 108,                 /* lo_bound  */
  YYSYMBOL_lo_term = 109,                  /* lo_term  */
  YYSYMBOL_b_expr = 110,                   /* b_expr  */
  YYSYMBOL_b_term = 111,                   /* b_term  */
  YYSYMBOL_b_factor = 112,                 /* b_factor  */
  YYSYMBOL_record_body = 113,              /* record_body  */
  YYSYMBOL_record_field_list = 114,        /* record_field_list  */
  YYSYMBOL_record_field = 115,             /* record_field  */
  YYSYMBOL_116_4 = 116,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 117,            /* case_arm_mark  */
  YYSYMBOL_case_open = 118,                /* case_open  */
  YYSYMBOL_record_case_opt = 119,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 120,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 121,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 122,            /* var_decl_list  */
  YYSYMBOL_var_decl = 123,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 124,           /* procedure_decl  */
  YYSYMBOL_125_5 = 125,                    /* $@5  */
  YYSYMBOL_126_6 = 126,                    /* $@6  */
  YYSYMBOL_127_7 = 127,                    /* $@7  */
  YYSYMBOL_pv_mark = 128,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 129,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 130,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 131,           /* parameter_decl  */
  YYSYMBOL_param_type = 132,               /* param_type  */
  YYSYMBOL_routine_directives = 133,       /* routine_directives  */
  YYSYMBOL_pf_params = 134,                /* pf_params  */
  YYSYMBOL_pf_sections = 135,              /* pf_sections  */
  YYSYMBOL_pf_section = 136,               /* pf_section  */
  YYSYMBOL_id_list = 137,                  /* id_list  */
  YYSYMBOL_body = 138,                     /* body  */
  YYSYMBOL_statement_list = 139,           /* statement_list  */
  YYSYMBOL_140_8 = 140,                    /* $@8  */
  YYSYMBOL_141_9 = 141,                    /* $@9  */
  YYSYMBOL_body_stmt = 142,                /* body_stmt  */
  YYSYMBOL_143_10 = 143,                   /* $@10  */
  YYSYMBOL_statement = 144,                /* statement  */
  YYSYMBOL_statement_no_label = 145,       /* statement_no_label  */
  YYSYMBOL_call = 146,                     /* call  */
  YYSYMBOL_call_with_args = 147,           /* call_with_args  */
  YYSYMBOL_argument_list = 148,            /* argument_list  */
  YYSYMBOL_argument = 149,                 /* argument  */
  YYSYMBOL_assignment = 150,               /* assignment  */
  YYSYMBOL_selector = 151,                 /* selector  */
  YYSYMBOL_expression_list = 152,          /* expression_list  */
  YYSYMBOL_compound_statement = 153,       /* compound_statement  */
  YYSYMBOL_goto_statement = 154,           /* goto_statement  */
  YYSYMBOL_if_statement = 155,             /* if_statement  */
  YYSYMBOL_case_statement = 156,           /* case_statement  */
  YYSYMBOL_157_11 = 157,                   /* $@11  */
  YYSYMBOL_case_tail = 158,                /* case_tail  */
  YYSYMBOL_case_list = 159,                /* case_list  */
  YYSYMBOL_case_elem = 160,                /* case_elem  */
  YYSYMBOL_constant_list = 161,            /* constant_list  */
  YYSYMBOL_case_label = 162,               /* case_label  */
  YYSYMBOL_while_statement = 163,          /* while_statement  */
  YYSYMBOL_repeat_statement = 164,         /* repeat_statement  */
  YYSYMBOL_for_statement = 165,            /* for_statement  */
  YYSYMBOL_with_statement = 166,           /* with_statement  */
  YYSYMBOL_with_open = 167,                /* with_open  */
  YYSYMBOL_expression = 168,               /* expression  */
  YYSYMBOL_simple_expression = 169,        /* simple_expression  */
  YYSYMBOL_term = 170,                     /* term  */
  YYSYMBOL_power = 171,                    /* power  */
  YYSYMBOL_factor = 172,                   /* factor  */
  YYSYMBOL_set_member_list = 173,          /* set_member_list  */
  YYSYMBOL_set_member = 174                /* set_member  */
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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   734

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  266
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  549

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   333


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
      75,    76,    77,    78
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   185,   185,   186,   189,   190,   193,   194,   197,   198,
     201,   204,   205,   208,   209,   210,   211,   212,   213,   216,
     219,   220,   223,   224,   225,   226,   227,   228,   231,   232,
     233,   236,   237,   240,   241,   242,   243,   246,   247,   250,
     251,   254,   255,   256,   257,   260,   261,   264,   265,   268,
     269,   270,   273,   274,   275,   276,   277,   280,   281,   284,
     287,   288,   289,   289,   289,   290,   291,   292,   293,   294,
     294,   295,   296,   297,   300,   301,   304,   305,   306,   307,
     310,   311,   312,   313,   314,   317,   318,   319,   320,   321,
     324,   325,   326,   327,   328,   331,   332,   333,   334,   335,
     338,   339,   342,   345,   346,   349,   349,   350,   353,   356,
     359,   360,   361,   364,   365,   368,   369,   372,   373,   376,
     377,   380,   381,   382,   382,   383,   383,   384,   384,   387,
     390,   391,   392,   395,   396,   399,   400,   401,   402,   403,
     404,   405,   406,   409,   410,   413,   414,   424,   425,   426,
     429,   430,   433,   434,   435,   436,   437,   440,   441,   444,
     447,   447,   448,   448,   451,   451,   454,   455,   456,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     472,   473,   474,   477,   478,   479,   482,   483,   486,   487,
     488,   491,   492,   493,   494,   495,   498,   499,   500,   501,
     504,   505,   508,   511,   512,   515,   516,   519,   519,   522,
     523,   524,   527,   528,   531,   532,   535,   536,   539,   540,
     543,   546,   549,   550,   551,   554,   557,   558,   565,   566,
     567,   568,   569,   570,   571,   572,   575,   576,   577,   578,
     579,   580,   581,   584,   585,   586,   587,   588,   589,   590,
     593,   594,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   610,   611,   614,   615
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
  "IDENT", "SHIFTOP", "XOROP", "POWOP", "$accept", "compilation_unit",
  "program", "program_name", "file_id_list_opt", "block", "decl_part_list",
  "decl_part", "unit", "intf_part_list", "intf_part", "init_opt",
  "fini_opt", "label_list", "const_decl_list", "const_decl", "typed_value",
  "typed_value_list", "field_value_list", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "$@1", "$@2", "$@3", "packed_opt",
  "simple_type", "lo_bound", "lo_term", "b_expr", "b_term", "b_factor",
  "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "param_type", "routine_directives", "pf_params",
  "pf_sections", "pf_section", "id_list", "body", "statement_list", "$@8",
  "$@9", "body_stmt", "$@10", "statement", "statement_no_label", "call",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@11", "case_tail", "case_list",
  "case_elem", "constant_list", "case_label", "while_statement",
  "repeat_statement", "for_statement", "with_statement", "with_open",
  "expression", "simple_expression", "term", "power", "factor",
  "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-326)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-182)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     154,   -10,    34,   144,  -326,    97,   574,  -326,  -326,   218,
     168,  -326,  -326,    92,   104,   113,   127,   113,   130,  -326,
     133,  -326,  -326,  -326,   141,   113,   171,   223,  -326,  -326,
      32,   125,   104,  -326,  -326,    44,  -326,   113,  -326,   160,
    -326,    40,    48,   192,   133,  -326,  -326,   150,  -326,  -326,
    -326,   166,   462,   283,  -326,  -326,   172,   199,  -326,   283,
      27,  -326,  -326,   309,   188,  -326,   462,   197,   462,  -326,
     462,   242,   122,  -326,  -326,  -326,  -326,  -326,   350,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,   283,  -326,  -326,
     234,   554,  -326,  -326,   374,   374,   374,   422,   462,   374,
    -326,  -326,  -326,  -326,   257,  -326,   233,   146,   428,   297,
     243,  -326,   269,  -326,   269,   553,   553,   287,   113,  -326,
    -326,  -326,  -326,   221,  -326,   311,   255,  -326,   115,    98,
    -326,    51,   390,    33,  -326,   340,    48,  -326,  -326,   344,
      38,   278,  -326,   289,    35,   570,   134,   644,    55,    55,
     437,   462,   325,  -326,   462,   462,   462,   462,   462,   396,
    -326,   104,   113,  -326,   337,   113,   363,   133,  -326,  -326,
     297,   297,  -326,   582,   277,  -326,   594,  -326,   364,  -326,
     462,   462,   462,   462,   462,   462,   462,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   114,  -326,
     384,  -326,    98,    98,  -326,   161,   607,   485,   100,   423,
     425,  -326,   394,   394,   596,   394,   394,   394,   394,   113,
     395,   113,   405,  -326,   113,    24,  -326,   276,   477,  -326,
     485,  -326,   417,  -326,   462,   462,   462,  -326,   424,  -326,
     197,  -326,  -326,  -326,   282,  -326,  -326,  -326,   164,  -326,
     656,   682,   308,   303,   682,   682,   682,   682,   682,  -326,
     104,    45,   569,  -326,   113,  -326,   133,   462,   462,  -326,
    -326,   408,   428,   428,   428,   428,   428,   428,   428,   297,
     297,   297,   297,   243,   243,   243,   243,   243,   243,  -326,
     113,   427,   113,   431,  -326,    29,  -326,   375,   432,  -326,
     553,   553,   451,  -326,   500,   509,   682,   283,   601,   283,
     601,   113,   596,  -326,   354,  -326,   354,   394,   394,    -6,
     354,  -326,  -326,  -326,  -326,   385,   269,   397,   269,   430,
     124,  -326,    25,   524,   465,   539,   544,   550,   411,   327,
     682,  -326,    48,  -326,  -326,   289,   596,   525,   462,  -326,
     462,   462,   462,  -326,  -326,  -326,  -326,   555,   199,   199,
     682,  -326,   467,   269,   473,   269,   140,  -326,    25,  -326,
    -326,  -326,  -326,   323,   511,    31,   594,  -326,  -326,   307,
    -326,  -326,   548,    57,  -326,   484,     2,   394,   394,   394,
     394,   354,   354,   394,   394,    25,  -326,    25,   510,    25,
    -326,   557,  -326,   535,  -326,   583,  -326,  -326,  -326,   581,
     462,   462,  -326,  -326,   117,    41,  -326,   488,  -326,  -326,
    -326,   669,   174,   682,   586,   586,  -326,   563,   589,   537,
      25,  -326,    25,   538,  -326,  -326,   485,   485,    -4,  -326,
     601,   571,  -326,   113,   527,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,   354,   354,   551,  -326,   533,  -326,   540,   462,
    -326,   608,   609,   465,   342,   355,  -326,   596,   596,  -326,
    -326,   606,   596,  -326,  -326,   462,  -326,   586,   558,  -326,
     562,  -326,  -326,   564,  -326,  -326,   179,  -326,   579,   578,
    -326,  -326,   101,   283,   462,  -326,  -326,   682,  -326,  -326,
    -326,  -326,  -326,    -6,  -326,   586,   586,  -326,  -326,  -326,
     682,  -326,   643,  -326,   485,  -326,   485,   623,  -326,  -326,
     577,  -326,   682,   648,  -326,  -326,  -326,  -326,  -326,   283,
     283,   596,   628,  -326,  -326,  -326,   652,  -326,   493,  -326,
     596,   599,   596,  -326,  -326,   652,   113,   602,  -326
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,   162,
       0,    11,    17,    10,     0,     0,     0,     0,    35,    36,
       0,     0,    14,    38,   158,     0,   129,    16,   118,     0,
     129,     0,   179,     0,    15,    58,     7,     0,    12,    21,
      13,     0,     0,    75,    37,    18,     0,   132,   117,    75,
     132,   160,   159,     0,     0,   162,     0,     0,     0,   162,
       0,     0,   199,   163,   166,   170,   182,   169,     0,   171,
     172,   173,   174,   175,   176,   177,   178,    75,    57,     8,
       0,     0,    33,    34,     0,     0,     0,     0,     0,     0,
     254,   257,   255,   256,   199,   253,   252,     0,   228,   236,
     243,   250,   149,    74,   149,     0,     0,     0,     0,    53,
      56,    54,    55,    52,    85,     0,     0,    60,     0,    80,
     157,     0,     0,     0,   127,     0,   179,   203,   204,     0,
       0,     0,   199,   227,     0,     0,     0,     0,   179,   179,
       0,     0,     0,   198,     0,     0,     0,     0,     0,     0,
       4,     0,     0,    12,     0,     0,     0,     0,    20,   259,
     237,   238,   261,   265,     0,   263,     0,   260,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    52,    81,    82,    61,     0,     0,     0,     0,    72,
       0,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,   158,     0,   134,     0,   145,   119,
       0,    12,     0,   161,     0,     0,     0,   164,     0,   164,
       0,   207,   202,   164,   199,   167,   168,   184,     0,   187,
     188,   191,   197,     0,   201,   192,   193,   194,   195,    59,
      23,     0,    30,   129,    25,   129,    24,     0,     0,   262,
     258,   197,   229,   230,   231,   232,   233,   234,   235,   241,
     239,   240,   242,   246,   247,   248,   244,   245,   249,   251,
       0,     0,     0,     0,   148,     0,   151,     0,     0,    76,
       0,     0,     0,    49,     0,     0,    41,    75,     0,    75,
       0,   107,     0,   100,    83,    95,    84,     0,     0,    79,
      90,    87,    88,    86,    89,     0,   149,     0,   149,     0,
       0,   130,     0,     0,   123,     0,     0,     0,     0,     0,
     221,   220,   179,   197,   225,   226,   215,   205,     0,   183,
       0,     0,     0,   196,    22,   162,   162,    32,   132,   132,
     266,   264,     0,   149,     0,   149,     0,   147,     0,    67,
      50,    51,    78,   199,     0,     0,    41,    40,    65,     0,
      73,    71,     0,   112,   104,     0,     0,     0,     0,     0,
       0,    91,    92,     0,     0,     0,   135,     0,     0,     0,
     133,     0,   143,   138,   121,     0,    12,   120,   128,   145,
       0,     0,   164,   165,   218,   209,   213,     0,   217,   164,
     186,   189,     0,   200,    28,    29,   162,     0,     0,     0,
       0,   155,     0,     0,   150,   152,     0,     0,     0,    43,
       0,     0,    70,   107,     0,   102,   105,   101,    97,    98,
      96,    99,    93,    94,   139,   137,     0,   142,     0,     0,
     146,     0,     0,   125,     0,     0,   224,     0,   215,   162,
     162,     0,     0,   164,   206,     0,   185,    31,     0,    26,
       0,   154,   153,     0,    47,    45,     0,    44,     0,     0,
      62,   103,     0,    75,     0,   136,   144,   140,   124,   122,
      12,   164,   164,   219,   212,   211,   210,   208,   216,   214,
     190,    19,     0,   156,     0,    42,     0,     0,    63,   109,
       0,   106,   141,     0,   223,   222,    27,    46,    48,    75,
      75,   116,     0,   126,    68,    64,   111,   114,     0,   109,
     116,     0,   116,   113,   108,   110,   107,     0,   115
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -326,  -326,  -326,  -326,  -326,   -45,   502,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,   522,   -27,  -226,  -326,  -326,  -326,
     -52,   496,   -21,   -57,  -326,  -326,  -326,  -326,  -283,  -326,
     138,  -208,  -193,  -119,   142,  -326,   244,  -326,  -326,   145,
    -326,   147,   158,   521,   -25,  -326,  -326,  -326,  -326,   -23,
     -42,  -326,   360,  -165,   290,   -99,  -326,   334,   -15,  -326,
     -56,  -326,  -326,  -199,  -326,  -120,   131,  -326,   -31,   351,
     353,  -326,   -28,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,   245,  -325,   237,  -326,  -326,  -326,  -326,  -326,   -44,
     241,   -54,   326,   -66,  -326,   443
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    26,     5,     6,    21,     7,    91,
     168,   357,   427,    30,    32,    33,   305,   486,   375,   302,
     313,    44,    45,   125,   518,   530,   311,   126,   127,   128,
     129,   414,   320,   315,   382,   383,   384,   493,   546,   531,
     445,   536,   537,    37,    38,    22,   406,   500,   231,    57,
     132,   225,   226,   403,   334,   199,   295,   296,    39,    23,
      41,   136,    42,   341,   342,    73,    74,    75,   105,   248,
     249,    77,   106,   253,    79,    80,    81,    82,   346,   471,
     415,   416,   538,   418,    83,    84,    85,    86,   144,   306,
     108,   109,   110,   111,   174,   175
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,   124,   133,    90,   335,    54,   319,   124,   107,   140,
      47,    76,    58,   146,    78,   200,   233,    60,   135,   314,
     316,   417,   141,    88,   145,   379,   147,   381,   169,   330,
     159,   401,   134,   177,   366,   124,   438,    50,   229,   143,
     344,   170,   171,    61,   347,    61,   468,   393,   394,    55,
     354,    63,   239,   173,   176,   393,   394,   469,    63,   219,
     487,   236,   443,   124,   124,     8,   447,    64,    65,    66,
      62,   488,    67,    68,    64,    65,    66,   220,   374,    67,
      68,   470,   444,   221,    69,   230,   222,    70,   331,    51,
     131,    69,   240,   367,    70,   439,   321,   322,   323,   324,
     402,    56,    56,   205,   386,    76,   250,   251,    78,    10,
     254,   255,   256,   257,   258,   223,   227,    76,    76,    71,
      78,    78,   290,    72,   391,   392,   224,  -180,   307,   519,
     244,   289,   219,   279,   280,   281,   282,  -180,  -180,    61,
     291,   215,   216,   417,    11,  -180,   292,   261,   290,   293,
     220,   179,  -180,   217,   303,    12,   221,   489,     1,   222,
     520,   308,  -180,    28,   242,     2,   291,    29,   212,   213,
     393,   394,   292,    27,   218,   293,    48,    52,   294,    31,
     214,   149,   467,   297,    53,   150,   336,   180,    34,    34,
     338,   339,   340,   181,   182,   183,   184,   185,   186,   224,
     452,   453,    36,   435,   325,    40,   327,    56,    43,   329,
     484,   485,   345,   466,    89,    34,    46,    56,    56,    59,
     474,   348,   413,   360,   173,   299,   -77,   396,   349,   398,
     454,   348,   455,    54,   457,    49,   514,    92,   476,    58,
     358,    93,   359,   515,    87,    88,   -77,   130,   370,   371,
     378,   -77,   380,   202,   203,   124,   124,   124,   124,   503,
     376,   208,   131,   139,   431,   481,   433,   482,   448,   449,
     450,   451,   142,   -77,   509,   362,    24,   364,   -77,   245,
     246,    25,   206,   -77,   209,   -77,   210,  -180,   527,   211,
     528,   178,   160,   153,   154,   237,   385,  -180,  -180,   424,
     425,   148,   524,   525,   250,  -180,   421,   250,   423,   112,
     113,    76,  -180,  -181,    78,   227,   428,   429,   114,   180,
     150,   197,  -180,  -181,  -181,   181,   182,   183,   184,   185,
     186,  -181,   198,    56,   268,   332,   115,   116,  -181,   269,
     191,   192,   193,   117,   412,   150,   118,   238,  -181,   153,
     154,   297,   194,   195,   119,   120,   121,   122,   123,   501,
     352,   461,   204,   207,   440,   353,   464,   465,   180,   441,
     477,   351,   502,   196,   181,   182,   183,   184,   185,   186,
     137,   234,   436,   180,   138,   235,   150,   151,   124,   181,
     182,   183,   184,   185,   186,   228,   180,   387,   388,   232,
     252,   259,   181,   182,   183,   184,   185,   186,   152,   389,
     153,   154,   263,   505,   506,   497,    94,   155,   156,   157,
     158,   272,   273,   274,   275,   276,   277,   278,   385,   410,
     390,   510,    56,   411,   368,    97,   521,    98,   265,   271,
      99,   124,    56,   298,   395,   100,   101,   102,   103,   104,
     522,   309,   180,   310,    56,   523,   397,   312,   181,   182,
     183,   184,   185,   186,    94,   119,   120,   121,   122,   201,
     326,   351,   534,   535,   187,    95,    96,   124,   124,    94,
     328,   188,   189,    97,   172,    98,   333,    56,    99,   399,
      95,    96,   337,   100,   101,   102,   103,   104,    97,   343,
      98,   247,   363,    99,    94,   190,   365,   369,   100,   101,
     102,   103,   104,   372,   377,    95,    96,   283,   284,   285,
     286,   287,   288,    97,    56,    98,   430,    94,    99,   404,
      56,   385,   432,   100,   101,   102,   103,   104,    95,    96,
     405,    56,    94,   446,   407,   472,    97,   473,   304,   408,
     472,    99,   541,    95,    96,   409,   100,   101,   102,   103,
     104,    97,   161,   304,   162,   419,    99,   163,   437,   456,
     426,   100,   101,   102,   103,   373,    13,    14,   442,    15,
     164,    13,    14,   355,    15,   458,   165,   459,   460,   166,
     462,    61,   167,   478,   479,    16,   480,   483,   241,   490,
      16,    17,   492,   494,    18,   356,    17,    20,   495,    18,
      19,   180,    20,   498,   499,   496,   511,   181,   182,   183,
     184,   185,   186,   180,   119,   120,   121,   122,   201,   181,
     182,   183,   184,   185,   186,   180,   507,   512,   516,   513,
     517,   181,   182,   183,   184,   185,   186,   267,   526,   317,
     318,   529,   532,   533,   115,   116,   539,   540,   270,   312,
     300,   301,   544,   266,   118,   262,   548,   119,   120,   121,
     122,   201,   119,   120,   121,   122,   123,   243,   119,   120,
     121,   122,   201,   260,   542,   180,   264,   491,   547,   545,
     400,   181,   182,   183,   184,   185,   186,   180,   543,   463,
     434,   420,   422,   181,   182,   183,   184,   185,   186,   508,
     180,   361,     0,   504,     0,   350,   181,   182,   183,   184,
     185,   186,     0,   180,     0,     0,     0,     0,   475,   181,
     182,   183,   184,   185,   186
};

static const yytype_int16 yycheck[] =
{
      15,    53,    59,    48,   230,    32,   214,    59,    52,    65,
      25,    42,    37,    69,    42,   114,   136,    40,    60,   212,
     213,   346,    66,    44,    68,   308,    70,   310,    94,     5,
      87,     6,     5,    99,     5,    87,     5,     5,     5,    67,
     239,    95,    96,     5,   243,     5,     5,    53,    54,     5,
       5,     3,    17,    97,    98,    53,    54,    16,     3,     8,
      64,    23,     5,   115,   116,    75,    64,    19,    20,    21,
      30,    75,    24,    25,    19,    20,    21,    26,   304,    24,
      25,    40,    25,    32,    36,    52,    35,    39,    64,    57,
      63,    36,    57,    64,    39,    64,   215,   216,   217,   218,
      75,    57,    57,   118,   312,   136,   150,   151,   136,    75,
     154,   155,   156,   157,   158,    64,   131,   148,   149,    71,
     148,   149,     8,    75,   317,   318,    75,     5,    28,    28,
      75,   197,     8,   187,   188,   189,   190,    15,    16,     5,
      26,    43,    44,   468,     0,    23,    32,   162,     8,    35,
      26,     5,    30,    55,   206,    58,    32,   440,     4,    35,
      59,    61,    40,    71,    30,    11,    26,    75,    53,    54,
      53,    54,    32,     5,    76,    35,     5,    52,    64,    75,
      65,    59,    65,   198,    59,    63,   231,    41,    75,    75,
     234,   235,   236,    47,    48,    49,    50,    51,    52,    75,
     393,   394,    75,   368,   219,    75,   221,    57,    75,   224,
     436,   437,   240,   412,    64,    75,    75,    57,    57,    59,
     419,    57,   342,   267,   268,    64,     5,   326,    64,   328,
     395,    57,   397,   260,   399,    12,    57,    71,    64,   264,
     263,    75,   265,    64,    52,   266,    25,    75,   300,   301,
     307,    30,   309,   115,   116,   307,   308,   309,   310,   467,
     304,     6,    63,    75,   363,   430,   365,   432,   387,   388,
     389,   390,    75,    52,   473,   290,    58,   292,    57,   148,
     149,    63,    61,    62,    29,    64,    31,     5,   514,    34,
     516,    58,    58,    60,    61,    17,   311,    15,    16,   355,
     356,    59,   501,   502,   348,    23,   350,   351,   352,    26,
      27,   342,    30,     5,   342,   330,   358,   359,    35,    41,
      63,    78,    40,    15,    16,    47,    48,    49,    50,    51,
      52,    23,    63,    57,    57,    59,    53,    54,    30,    62,
      43,    44,    45,    60,    17,    63,    63,    58,    40,    60,
      61,   366,    55,    56,    71,    72,    73,    74,    75,    17,
      57,   406,    75,    52,    57,    62,   410,   411,    41,    62,
     426,    63,    17,    76,    47,    48,    49,    50,    51,    52,
      71,    37,    59,    41,    75,    41,    63,    37,   440,    47,
      48,    49,    50,    51,    52,     5,    41,    43,    44,    59,
      75,     5,    47,    48,    49,    50,    51,    52,    58,    55,
      60,    61,    75,   469,   470,   459,    42,    67,    68,    69,
      70,   180,   181,   182,   183,   184,   185,   186,   443,    18,
      76,   475,    57,    22,    59,    61,   493,    63,    75,    75,
      66,   493,    57,    59,    59,    71,    72,    73,    74,    75,
     494,    28,    41,    28,    57,   500,    59,    63,    47,    48,
      49,    50,    51,    52,    42,    71,    72,    73,    74,    75,
      75,    63,   529,   530,    46,    53,    54,   529,   530,    42,
      75,    53,    54,    61,    62,    63,     9,    57,    66,    59,
      53,    54,    75,    71,    72,    73,    74,    75,    61,    75,
      63,    64,    75,    66,    42,    77,    75,    75,    71,    72,
      73,    74,    75,    62,     5,    53,    54,   191,   192,   193,
     194,   195,   196,    61,    57,    63,    59,    42,    66,     5,
      57,   546,    59,    71,    72,    73,    74,    75,    53,    54,
      75,    57,    42,    59,     5,    57,    61,    59,    63,     5,
      57,    66,    59,    53,    54,     5,    71,    72,    73,    74,
      75,    61,     8,    63,    10,    40,    66,    13,    57,    59,
      15,    71,    72,    73,    74,    75,     7,     8,    30,    10,
      26,     7,     8,    14,    10,    28,    32,    52,     5,    35,
       9,     5,    38,    30,     5,    26,    59,    59,    28,    28,
      26,    32,    75,    52,    35,    36,    32,    38,    75,    35,
      36,    41,    38,     5,     5,    75,    58,    47,    48,    49,
      50,    51,    52,    41,    71,    72,    73,    74,    75,    47,
      48,    49,    50,    51,    52,    41,    30,    75,    59,    75,
      62,    47,    48,    49,    50,    51,    52,    65,     5,    53,
      54,    28,    75,     5,    53,    54,    28,     5,    64,    63,
      53,    54,    63,   167,    63,   163,    64,    71,    72,    73,
      74,    75,    71,    72,    73,    74,    75,    33,    71,    72,
      73,    74,    75,   161,   539,    41,   165,   443,   546,   542,
     330,    47,    48,    49,    50,    51,    52,    41,   540,   409,
     366,   348,   351,    47,    48,    49,    50,    51,    52,   472,
      41,   268,    -1,   468,    -1,    59,    47,    48,    49,    50,
      51,    52,    -1,    41,    -1,    -1,    -1,    -1,    59,    47,
      48,    49,    50,    51,    52
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    80,    81,    84,    85,    87,    75,    82,
      75,     0,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    86,   124,   138,    58,    63,    83,     5,    71,    75,
      92,    75,    93,    94,    75,   137,    75,   122,   123,   137,
      75,   139,   141,    75,   100,   101,    75,   137,     5,    12,
       5,    57,    52,    59,    94,     5,    57,   128,   123,    59,
     128,     5,    30,     3,    19,    20,    21,    24,    25,    36,
      39,    71,    75,   144,   145,   146,   147,   150,   151,   153,
     154,   155,   156,   163,   164,   165,   166,    52,   101,    64,
      84,    88,    71,    75,    42,    53,    54,    61,    63,    66,
      71,    72,    73,    74,    75,   147,   151,   168,   169,   170,
     171,   172,    26,    27,    35,    53,    54,    60,    63,    71,
      72,    73,    74,    75,    99,   102,   106,   107,   108,   109,
      75,    63,   129,   102,     5,   129,   140,    71,    75,    75,
     139,   168,    75,   151,   167,   168,   139,   168,    59,    59,
      63,    37,    58,    60,    61,    67,    68,    69,    70,   102,
      58,     8,    10,    13,    26,    32,    35,    38,    89,   172,
     170,   170,    62,   168,   173,   174,   168,   172,    58,     5,
      41,    47,    48,    49,    50,    51,    52,    46,    53,    54,
      77,    43,    44,    45,    55,    56,    76,    78,    63,   134,
     134,    75,   109,   109,    75,   137,    61,    52,     6,    29,
      31,    34,    53,    54,    65,    43,    44,    55,    76,     8,
      26,    32,    35,    64,    75,   130,   131,   137,     5,     5,
      52,   127,    59,   144,    37,    41,    23,    17,    58,    17,
      57,    28,    30,    33,    75,   145,   145,    64,   148,   149,
     168,   168,    75,   152,   168,   168,   168,   168,   168,     5,
      93,   137,    85,    75,   122,    75,   100,    65,    57,    62,
      64,    75,   169,   169,   169,   169,   169,   169,   169,   170,
     170,   170,   170,   171,   171,   171,   171,   171,   171,   172,
       8,    26,    32,    35,    64,   135,   136,   137,    59,    64,
      53,    54,    98,    99,    63,    95,   168,    28,    61,    28,
      28,   105,    63,    99,   111,   112,   111,    53,    54,   110,
     111,   112,   112,   112,   112,   137,    75,   137,    75,   137,
       5,    64,    59,     9,   133,    95,    84,    75,   168,   168,
     168,   142,   143,    75,   142,   151,   157,   142,    57,    64,
      59,    63,    57,    62,     5,    14,    36,    90,   128,   128,
     168,   174,   137,    75,   137,    75,     5,    64,    59,    75,
      99,    99,    62,    75,    95,    97,   168,     5,   102,   107,
     102,   107,   113,   114,   115,   137,   110,    43,    44,    55,
      76,   111,   111,    53,    54,    59,   134,    59,   134,    59,
     131,     6,    75,   132,     5,    75,   125,     5,     5,     5,
      18,    22,    17,   144,   110,   159,   160,   161,   162,    40,
     149,   168,   148,   168,   139,   139,    15,    91,   129,   129,
      59,   134,    59,   134,   136,   132,    59,    57,     5,    64,
      57,    62,    30,     5,    25,   119,    59,    64,   112,   112,
     112,   112,   111,   111,   132,   132,    59,   132,    28,    52,
       5,    84,     9,   133,   168,   168,   142,    65,     5,    16,
      40,   158,    57,    59,   142,    59,    64,   139,    30,     5,
      59,   132,   132,    59,    95,    95,    96,    64,    75,   107,
      28,   115,    75,   116,    52,    75,    75,   168,     5,     5,
     126,    17,    17,   110,   160,   139,   139,    30,   162,   142,
     168,    58,    75,    75,    57,    64,    59,    62,   103,    28,
      59,   102,   168,    84,   142,   142,     5,    95,    95,    28,
     104,   118,    75,     5,   102,   102,   120,   121,   161,    28,
       5,    59,   118,   121,    63,   120,   117,   113,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    80,    81,    81,    82,    82,    83,    83,
      84,    85,    85,    86,    86,    86,    86,    86,    86,    87,
      88,    88,    89,    89,    89,    89,    89,    89,    90,    90,
      90,    91,    91,    92,    92,    92,    92,    93,    93,    94,
      94,    95,    95,    95,    95,    96,    96,    97,    97,    98,
      98,    98,    99,    99,    99,    99,    99,   100,   100,   101,
     102,   102,   103,   104,   102,   102,   102,   102,   102,   105,
     102,   102,   102,   102,   106,   106,   107,   107,   107,   107,
     108,   108,   108,   108,   108,   109,   109,   109,   109,   109,
     110,   110,   110,   110,   110,   111,   111,   111,   111,   111,
     112,   112,   113,   114,   114,   116,   115,   115,   117,   118,
     119,   119,   119,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   124,   125,   124,   126,   124,   127,   124,   128,
     129,   129,   129,   130,   130,   131,   131,   131,   131,   131,
     131,   131,   131,   132,   132,   133,   133,   134,   134,   134,
     135,   135,   136,   136,   136,   136,   136,   137,   137,   138,
     140,   139,   141,   139,   143,   142,   144,   144,   144,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   146,   147,   147,   147,   148,   148,   149,   149,
     149,   150,   150,   150,   150,   150,   151,   151,   151,   151,
     152,   152,   153,   154,   154,   155,   155,   157,   156,   158,
     158,   158,   159,   159,   160,   160,   161,   161,   162,   162,
     163,   164,   165,   165,   165,   166,   167,   167,   168,   168,
     168,   168,   168,   168,   168,   168,   169,   169,   169,   169,
     169,   169,   169,   170,   170,   170,   170,   170,   170,   170,
     171,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   173,   173,   174,   174
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,    11,
       2,     0,     3,     2,     2,     2,     5,     7,     2,     2,
       0,     2,     0,     3,     3,     1,     1,     2,     1,     4,
       6,     1,     5,     3,     4,     1,     3,     3,     5,     1,
       2,     2,     1,     1,     1,     1,     1,     2,     1,     4,
       1,     2,     0,     0,     9,     4,     2,     4,     9,     0,
       5,     4,     2,     4,     1,     0,     3,     1,     4,     3,
       1,     2,     2,     3,     3,     1,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     2,     3,     1,     0,     4,     0,     0,     0,
       7,     5,     0,     3,     1,     6,     0,     2,     1,     4,
       6,     7,     9,     0,     9,     0,    11,     0,     7,     0,
       3,     2,     0,     3,     1,     3,     5,     4,     3,     4,
       5,     6,     4,     1,     3,     0,     3,     3,     2,     0,
       3,     1,     3,     4,     4,     3,     5,     3,     1,     3,
       0,     4,     0,     2,     0,     2,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     3,     1,     4,     3,     6,     3,     1,     1,     3,
       5,     3,     3,     3,     3,     3,     4,     3,     2,     1,
       3,     1,     3,     2,     2,     4,     6,     0,     7,     0,
       2,     2,     3,     1,     3,     0,     3,     1,     1,     3,
       4,     4,     8,     8,     6,     4,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     3,     2,
       2,     2,     3,     1,     3,     1,     3
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
#line 185 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1801 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 186 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1807 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 189 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1813 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 190 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1819 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 193 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1825 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 194 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 1831 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 197 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1837 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 198 "pascal.y"
     { (yyval.node) = NULL; }
#line 1843 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 201 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1849 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 204 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1855 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 205 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1861 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 208 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1867 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 209 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1873 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 210 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1879 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 211 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1885 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 212 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1891 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 213 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1897 "pascal.tab.c"
    break;

  case 19: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 216 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1903 "pascal.tab.c"
    break;

  case 20: /* intf_part_list: intf_part_list intf_part  */
#line 219 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1909 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: %empty  */
#line 220 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1915 "pascal.tab.c"
    break;

  case 22: /* intf_part: USESSY id_list SEMICOLON  */
#line 223 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1921 "pascal.tab.c"
    break;

  case 23: /* intf_part: CONSTSY const_decl_list  */
#line 224 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1927 "pascal.tab.c"
    break;

  case 24: /* intf_part: TYPESY type_decl_list  */
#line 225 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1933 "pascal.tab.c"
    break;

  case 25: /* intf_part: VARSY var_decl_list  */
#line 226 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1939 "pascal.tab.c"
    break;

  case 26: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 227 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1945 "pascal.tab.c"
    break;

  case 27: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 228 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1951 "pascal.tab.c"
    break;

  case 28: /* init_opt: INITIALIZATIONSY statement_list  */
#line 231 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1957 "pascal.tab.c"
    break;

  case 29: /* init_opt: BEGINSY statement_list  */
#line 232 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1963 "pascal.tab.c"
    break;

  case 30: /* init_opt: %empty  */
#line 233 "pascal.y"
     { (yyval.node) = NULL; }
#line 1969 "pascal.tab.c"
    break;

  case 31: /* fini_opt: FINALIZATIONSY statement_list  */
#line 236 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1975 "pascal.tab.c"
    break;

  case 32: /* fini_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 1981 "pascal.tab.c"
    break;

  case 33: /* label_list: label_list COMMA INTCONST  */
#line 240 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1987 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA IDENT  */
#line 241 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1993 "pascal.tab.c"
    break;

  case 35: /* label_list: INTCONST  */
#line 242 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1999 "pascal.tab.c"
    break;

  case 36: /* label_list: IDENT  */
#line 243 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2005 "pascal.tab.c"
    break;

  case 37: /* const_decl_list: const_decl_list const_decl  */
#line 246 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2011 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl  */
#line 247 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2017 "pascal.tab.c"
    break;

  case 39: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 250 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2023 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 251 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2029 "pascal.tab.c"
    break;

  case 41: /* typed_value: expression  */
#line 254 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2035 "pascal.tab.c"
    break;

  case 42: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 255 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2041 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT field_value_list RPARENT  */
#line 256 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2047 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 257 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2053 "pascal.tab.c"
    break;

  case 45: /* typed_value_list: typed_value  */
#line 260 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2059 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 261 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2065 "pascal.tab.c"
    break;

  case 47: /* field_value_list: IDENT COLON typed_value  */
#line 264 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2071 "pascal.tab.c"
    break;

  case 48: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 265 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2077 "pascal.tab.c"
    break;

  case 49: /* constant: scalar_constant  */
#line 268 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2083 "pascal.tab.c"
    break;

  case 50: /* constant: PLUS scalar_constant  */
#line 269 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2089 "pascal.tab.c"
    break;

  case 51: /* constant: MINUS scalar_constant  */
#line 270 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2095 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: IDENT  */
#line 273 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2101 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: INTCONST  */
#line 274 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2107 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: REALCONST  */
#line 275 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2113 "pascal.tab.c"
    break;

  case 55: /* scalar_constant: STRINGCONST  */
#line 276 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2119 "pascal.tab.c"
    break;

  case 56: /* scalar_constant: CHARCODE  */
#line 277 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2125 "pascal.tab.c"
    break;

  case 57: /* type_decl_list: type_decl_list type_decl  */
#line 280 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2131 "pascal.tab.c"
    break;

  case 58: /* type_decl_list: type_decl  */
#line 281 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2137 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 284 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2143 "pascal.tab.c"
    break;

  case 60: /* type: simple_type  */
#line 287 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2149 "pascal.tab.c"
    break;

  case 61: /* type: ARROW IDENT  */
#line 288 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2155 "pascal.tab.c"
    break;

  case 62: /* $@1: %empty  */
#line 289 "pascal.y"
                                                       { }
#line 2161 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 289 "pascal.y"
                                                           { }
#line 2167 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 289 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2173 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt ARRAYSY OFSY type  */
#line 290 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2179 "pascal.tab.c"
    break;

  case 66: /* type: PROCEDURESY pf_params  */
#line 291 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2185 "pascal.tab.c"
    break;

  case 67: /* type: FUNCTIONSY pf_params COLON IDENT  */
#line 292 "pascal.y"
                                      { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2191 "pascal.tab.c"
    break;

  case 68: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 293 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2197 "pascal.tab.c"
    break;

  case 69: /* $@3: %empty  */
#line 294 "pascal.y"
                         { }
#line 2203 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 294 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2209 "pascal.tab.c"
    break;

  case 71: /* type: packed_opt SETSY OFSY simple_type  */
#line 295 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2215 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt FILESY  */
#line 296 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2221 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt FILESY OFSY type  */
#line 297 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2227 "pascal.tab.c"
    break;

  case 74: /* packed_opt: PACKEDSY  */
#line 300 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2233 "pascal.tab.c"
    break;

  case 75: /* packed_opt: %empty  */
#line 301 "pascal.y"
     { (yyval.node) = NULL; }
#line 2239 "pascal.tab.c"
    break;

  case 76: /* simple_type: LPARENT id_list RPARENT  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2245 "pascal.tab.c"
    break;

  case 77: /* simple_type: IDENT  */
#line 305 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2251 "pascal.tab.c"
    break;

  case 78: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 306 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2257 "pascal.tab.c"
    break;

  case 79: /* simple_type: lo_bound DOTDOT b_expr  */
#line 307 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2263 "pascal.tab.c"
    break;

  case 80: /* lo_bound: lo_term  */
#line 310 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2269 "pascal.tab.c"
    break;

  case 81: /* lo_bound: PLUS lo_term  */
#line 311 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2275 "pascal.tab.c"
    break;

  case 82: /* lo_bound: MINUS lo_term  */
#line 312 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2281 "pascal.tab.c"
    break;

  case 83: /* lo_bound: lo_bound PLUS b_term  */
#line 313 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2287 "pascal.tab.c"
    break;

  case 84: /* lo_bound: lo_bound MINUS b_term  */
#line 314 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2293 "pascal.tab.c"
    break;

  case 85: /* lo_term: scalar_constant  */
#line 317 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2299 "pascal.tab.c"
    break;

  case 86: /* lo_term: lo_term MUL b_factor  */
#line 318 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2305 "pascal.tab.c"
    break;

  case 87: /* lo_term: lo_term IDIV b_factor  */
#line 319 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2311 "pascal.tab.c"
    break;

  case 88: /* lo_term: lo_term IMOD b_factor  */
#line 320 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2317 "pascal.tab.c"
    break;

  case 89: /* lo_term: lo_term SHIFTOP b_factor  */
#line 321 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2323 "pascal.tab.c"
    break;

  case 90: /* b_expr: b_term  */
#line 324 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2329 "pascal.tab.c"
    break;

  case 91: /* b_expr: PLUS b_term  */
#line 325 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2335 "pascal.tab.c"
    break;

  case 92: /* b_expr: MINUS b_term  */
#line 326 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2341 "pascal.tab.c"
    break;

  case 93: /* b_expr: b_expr PLUS b_term  */
#line 327 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2347 "pascal.tab.c"
    break;

  case 94: /* b_expr: b_expr MINUS b_term  */
#line 328 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2353 "pascal.tab.c"
    break;

  case 95: /* b_term: b_factor  */
#line 331 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2359 "pascal.tab.c"
    break;

  case 96: /* b_term: b_term MUL b_factor  */
#line 332 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2365 "pascal.tab.c"
    break;

  case 97: /* b_term: b_term IDIV b_factor  */
#line 333 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2371 "pascal.tab.c"
    break;

  case 98: /* b_term: b_term IMOD b_factor  */
#line 334 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2377 "pascal.tab.c"
    break;

  case 99: /* b_term: b_term SHIFTOP b_factor  */
#line 335 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2383 "pascal.tab.c"
    break;

  case 100: /* b_factor: scalar_constant  */
#line 338 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2389 "pascal.tab.c"
    break;

  case 101: /* b_factor: LPARENT b_expr RPARENT  */
#line 339 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2395 "pascal.tab.c"
    break;

  case 102: /* record_body: record_field_list record_case_opt  */
#line 342 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2401 "pascal.tab.c"
    break;

  case 103: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 345 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2407 "pascal.tab.c"
    break;

  case 104: /* record_field_list: record_field  */
#line 346 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2413 "pascal.tab.c"
    break;

  case 105: /* $@4: %empty  */
#line 349 "pascal.y"
                  { }
#line 2419 "pascal.tab.c"
    break;

  case 106: /* record_field: id_list COLON $@4 type  */
#line 349 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 107: /* record_field: %empty  */
#line 350 "pascal.y"
     { (yyval.node) = NULL; }
#line 2431 "pascal.tab.c"
    break;

  case 108: /* case_arm_mark: %empty  */
#line 353 "pascal.y"
    { }
#line 2437 "pascal.tab.c"
    break;

  case 109: /* case_open: %empty  */
#line 356 "pascal.y"
    { }
#line 2443 "pascal.tab.c"
    break;

  case 110: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 359 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2449 "pascal.tab.c"
    break;

  case 111: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 360 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2455 "pascal.tab.c"
    break;

  case 112: /* record_case_opt: %empty  */
#line 361 "pascal.y"
     { (yyval.node) = NULL; }
#line 2461 "pascal.tab.c"
    break;

  case 113: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 364 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2467 "pascal.tab.c"
    break;

  case 114: /* record_case_list: record_case_arm  */
#line 365 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2473 "pascal.tab.c"
    break;

  case 115: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 368 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2479 "pascal.tab.c"
    break;

  case 116: /* record_case_arm: %empty  */
#line 369 "pascal.y"
     { (yyval.node) = NULL; }
#line 2485 "pascal.tab.c"
    break;

  case 117: /* var_decl_list: var_decl_list var_decl  */
#line 372 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 118: /* var_decl_list: var_decl  */
#line 373 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2497 "pascal.tab.c"
    break;

  case 119: /* var_decl: id_list COLON type SEMICOLON  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2503 "pascal.tab.c"
    break;

  case 120: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 377 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2509 "pascal.tab.c"
    break;

  case 121: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 380 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2515 "pascal.tab.c"
    break;

  case 122: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 381 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2521 "pascal.tab.c"
    break;

  case 123: /* $@5: %empty  */
#line 382 "pascal.y"
                                                                               { }
#line 2527 "pascal.tab.c"
    break;

  case 124: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 382 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2533 "pascal.tab.c"
    break;

  case 125: /* $@6: %empty  */
#line 383 "pascal.y"
                                                                                          { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2539 "pascal.tab.c"
    break;

  case 126: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 383 "pascal.y"
                                                                                                                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 127: /* $@7: %empty  */
#line 384 "pascal.y"
                                        { }
#line 2551 "pascal.tab.c"
    break;

  case 128: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 384 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2557 "pascal.tab.c"
    break;

  case 129: /* pv_mark: %empty  */
#line 387 "pascal.y"
    { }
#line 2563 "pascal.tab.c"
    break;

  case 130: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 390 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2569 "pascal.tab.c"
    break;

  case 131: /* parameter_list_opt: LPARENT RPARENT  */
#line 391 "pascal.y"
                     { (yyval.node) = NULL; }
#line 2575 "pascal.tab.c"
    break;

  case 132: /* parameter_list_opt: %empty  */
#line 392 "pascal.y"
     { (yyval.node) = NULL; }
#line 2581 "pascal.tab.c"
    break;

  case 133: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 395 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2587 "pascal.tab.c"
    break;

  case 134: /* parameter_decl_list: parameter_decl  */
#line 396 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2593 "pascal.tab.c"
    break;

  case 135: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 399 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2599 "pascal.tab.c"
    break;

  case 136: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 400 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2605 "pascal.tab.c"
    break;

  case 137: /* parameter_decl: VARSY id_list COLON param_type  */
#line 401 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2611 "pascal.tab.c"
    break;

  case 138: /* parameter_decl: id_list COLON param_type  */
#line 402 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2617 "pascal.tab.c"
    break;

  case 139: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 403 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2623 "pascal.tab.c"
    break;

  case 140: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 404 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2629 "pascal.tab.c"
    break;

  case 141: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 405 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2635 "pascal.tab.c"
    break;

  case 142: /* parameter_decl: IDENT id_list COLON param_type  */
#line 406 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2641 "pascal.tab.c"
    break;

  case 143: /* param_type: IDENT  */
#line 409 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2647 "pascal.tab.c"
    break;

  case 144: /* param_type: ARRAYSY OFSY IDENT  */
#line 410 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 2653 "pascal.tab.c"
    break;

  case 145: /* routine_directives: %empty  */
#line 413 "pascal.y"
    { (yyval.node) = NULL; }
#line 2659 "pascal.tab.c"
    break;

  case 146: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 414 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 2672 "pascal.tab.c"
    break;

  case 147: /* pf_params: LPARENT pf_sections RPARENT  */
#line 424 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2678 "pascal.tab.c"
    break;

  case 148: /* pf_params: LPARENT RPARENT  */
#line 425 "pascal.y"
                     { (yyval.node) = NULL; }
#line 2684 "pascal.tab.c"
    break;

  case 149: /* pf_params: %empty  */
#line 426 "pascal.y"
     { (yyval.node) = NULL; }
#line 2690 "pascal.tab.c"
    break;

  case 150: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 429 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2696 "pascal.tab.c"
    break;

  case 151: /* pf_sections: pf_section  */
#line 430 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 152: /* pf_section: id_list COLON param_type  */
#line 433 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2708 "pascal.tab.c"
    break;

  case 153: /* pf_section: VARSY id_list COLON param_type  */
#line 434 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2714 "pascal.tab.c"
    break;

  case 154: /* pf_section: CONSTSY id_list COLON param_type  */
#line 435 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2720 "pascal.tab.c"
    break;

  case 155: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 436 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2726 "pascal.tab.c"
    break;

  case 156: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 437 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2732 "pascal.tab.c"
    break;

  case 157: /* id_list: id_list COMMA IDENT  */
#line 440 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2738 "pascal.tab.c"
    break;

  case 158: /* id_list: IDENT  */
#line 441 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2744 "pascal.tab.c"
    break;

  case 159: /* body: BEGINSY statement_list ENDSY  */
#line 444 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2750 "pascal.tab.c"
    break;

  case 160: /* $@8: %empty  */
#line 447 "pascal.y"
                             { pas_stmt_mark(); }
#line 2756 "pascal.tab.c"
    break;

  case 161: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 447 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2762 "pascal.tab.c"
    break;

  case 162: /* $@9: %empty  */
#line 448 "pascal.y"
     { pas_stmt_mark(); }
#line 2768 "pascal.tab.c"
    break;

  case 163: /* statement_list: $@9 statement  */
#line 448 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 164: /* $@10: %empty  */
#line 451 "pascal.y"
    { pas_stmt_mark(); }
#line 2780 "pascal.tab.c"
    break;

  case 165: /* body_stmt: $@10 statement  */
#line 451 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2786 "pascal.tab.c"
    break;

  case 166: /* statement: statement_no_label  */
#line 454 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2792 "pascal.tab.c"
    break;

  case 167: /* statement: INTCONST COLON statement_no_label  */
#line 455 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 168: /* statement: IDENT COLON statement_no_label  */
#line 456 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2804 "pascal.tab.c"
    break;

  case 169: /* statement_no_label: assignment  */
#line 459 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2810 "pascal.tab.c"
    break;

  case 170: /* statement_no_label: call  */
#line 460 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2816 "pascal.tab.c"
    break;

  case 171: /* statement_no_label: compound_statement  */
#line 461 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2822 "pascal.tab.c"
    break;

  case 172: /* statement_no_label: goto_statement  */
#line 462 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2828 "pascal.tab.c"
    break;

  case 173: /* statement_no_label: if_statement  */
#line 463 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2834 "pascal.tab.c"
    break;

  case 174: /* statement_no_label: case_statement  */
#line 464 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2840 "pascal.tab.c"
    break;

  case 175: /* statement_no_label: while_statement  */
#line 465 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2846 "pascal.tab.c"
    break;

  case 176: /* statement_no_label: repeat_statement  */
#line 466 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2852 "pascal.tab.c"
    break;

  case 177: /* statement_no_label: for_statement  */
#line 467 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2858 "pascal.tab.c"
    break;

  case 178: /* statement_no_label: with_statement  */
#line 468 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2864 "pascal.tab.c"
    break;

  case 179: /* statement_no_label: %empty  */
#line 469 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2870 "pascal.tab.c"
    break;

  case 180: /* call: IDENT  */
#line 472 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2876 "pascal.tab.c"
    break;

  case 181: /* call: selector PERIOD IDENT  */
#line 473 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2882 "pascal.tab.c"
    break;

  case 182: /* call: call_with_args  */
#line 474 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2888 "pascal.tab.c"
    break;

  case 183: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 477 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2894 "pascal.tab.c"
    break;

  case 184: /* call_with_args: IDENT LPARENT RPARENT  */
#line 478 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 2900 "pascal.tab.c"
    break;

  case 185: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 479 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2906 "pascal.tab.c"
    break;

  case 186: /* argument_list: argument_list COMMA argument  */
#line 482 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2912 "pascal.tab.c"
    break;

  case 187: /* argument_list: argument  */
#line 483 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2918 "pascal.tab.c"
    break;

  case 188: /* argument: expression  */
#line 486 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2924 "pascal.tab.c"
    break;

  case 189: /* argument: expression COLON expression  */
#line 487 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2930 "pascal.tab.c"
    break;

  case 190: /* argument: expression COLON expression COLON expression  */
#line 488 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2936 "pascal.tab.c"
    break;

  case 191: /* assignment: selector BECOMES expression  */
#line 491 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2942 "pascal.tab.c"
    break;

  case 192: /* assignment: selector ADDBECOMES expression  */
#line 492 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 2948 "pascal.tab.c"
    break;

  case 193: /* assignment: selector SUBBECOMES expression  */
#line 493 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 2954 "pascal.tab.c"
    break;

  case 194: /* assignment: selector MULBECOMES expression  */
#line 494 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 2960 "pascal.tab.c"
    break;

  case 195: /* assignment: selector DIVBECOMES expression  */
#line 495 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 2966 "pascal.tab.c"
    break;

  case 196: /* selector: selector LBRACK expression_list RBRACK  */
#line 498 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2972 "pascal.tab.c"
    break;

  case 197: /* selector: selector PERIOD IDENT  */
#line 499 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2978 "pascal.tab.c"
    break;

  case 198: /* selector: selector ARROW  */
#line 500 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2984 "pascal.tab.c"
    break;

  case 199: /* selector: IDENT  */
#line 501 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2990 "pascal.tab.c"
    break;

  case 200: /* expression_list: expression_list COMMA expression  */
#line 504 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2996 "pascal.tab.c"
    break;

  case 201: /* expression_list: expression  */
#line 505 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3002 "pascal.tab.c"
    break;

  case 202: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 508 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3008 "pascal.tab.c"
    break;

  case 203: /* goto_statement: GOTOSY INTCONST  */
#line 511 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3014 "pascal.tab.c"
    break;

  case 204: /* goto_statement: GOTOSY IDENT  */
#line 512 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3020 "pascal.tab.c"
    break;

  case 205: /* if_statement: IFSY expression THENSY body_stmt  */
#line 515 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3026 "pascal.tab.c"
    break;

  case 206: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 516 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3032 "pascal.tab.c"
    break;

  case 207: /* $@11: %empty  */
#line 519 "pascal.y"
                           { }
#line 3038 "pascal.tab.c"
    break;

  case 208: /* case_statement: CASESY expression OFSY $@11 case_list case_tail ENDSY  */
#line 519 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3044 "pascal.tab.c"
    break;

  case 209: /* case_tail: %empty  */
#line 522 "pascal.y"
    { (yyval.node) = NULL; }
#line 3050 "pascal.tab.c"
    break;

  case 210: /* case_tail: ELSESY statement_list  */
#line 523 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3056 "pascal.tab.c"
    break;

  case 211: /* case_tail: OTHERWISESY statement_list  */
#line 524 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3062 "pascal.tab.c"
    break;

  case 212: /* case_list: case_list SEMICOLON case_elem  */
#line 527 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3068 "pascal.tab.c"
    break;

  case 213: /* case_list: case_elem  */
#line 528 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3074 "pascal.tab.c"
    break;

  case 214: /* case_elem: constant_list COLON body_stmt  */
#line 531 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3080 "pascal.tab.c"
    break;

  case 215: /* case_elem: %empty  */
#line 532 "pascal.y"
     { (yyval.node) = NULL; }
#line 3086 "pascal.tab.c"
    break;

  case 216: /* constant_list: constant_list COMMA case_label  */
#line 535 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3092 "pascal.tab.c"
    break;

  case 217: /* constant_list: case_label  */
#line 536 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3098 "pascal.tab.c"
    break;

  case 218: /* case_label: b_expr  */
#line 539 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3104 "pascal.tab.c"
    break;

  case 219: /* case_label: b_expr DOTDOT b_expr  */
#line 540 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3110 "pascal.tab.c"
    break;

  case 220: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 543 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3116 "pascal.tab.c"
    break;

  case 221: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 546 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3122 "pascal.tab.c"
    break;

  case 222: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 549 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3128 "pascal.tab.c"
    break;

  case 223: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 550 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3134 "pascal.tab.c"
    break;

  case 224: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 551 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3140 "pascal.tab.c"
    break;

  case 225: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 554 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3146 "pascal.tab.c"
    break;

  case 226: /* with_open: with_open COMMA selector  */
#line 557 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3152 "pascal.tab.c"
    break;

  case 227: /* with_open: selector  */
#line 558 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3158 "pascal.tab.c"
    break;

  case 228: /* expression: simple_expression  */
#line 565 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3164 "pascal.tab.c"
    break;

  case 229: /* expression: expression INOP simple_expression  */
#line 566 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3170 "pascal.tab.c"
    break;

  case 230: /* expression: expression LTOP simple_expression  */
#line 567 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3176 "pascal.tab.c"
    break;

  case 231: /* expression: expression LEOP simple_expression  */
#line 568 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3182 "pascal.tab.c"
    break;

  case 232: /* expression: expression GTOP simple_expression  */
#line 569 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3188 "pascal.tab.c"
    break;

  case 233: /* expression: expression GEOP simple_expression  */
#line 570 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3194 "pascal.tab.c"
    break;

  case 234: /* expression: expression NEOP simple_expression  */
#line 571 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3200 "pascal.tab.c"
    break;

  case 235: /* expression: expression EQOP simple_expression  */
#line 572 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3206 "pascal.tab.c"
    break;

  case 236: /* simple_expression: term  */
#line 575 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3212 "pascal.tab.c"
    break;

  case 237: /* simple_expression: PLUS term  */
#line 576 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3218 "pascal.tab.c"
    break;

  case 238: /* simple_expression: MINUS term  */
#line 577 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3224 "pascal.tab.c"
    break;

  case 239: /* simple_expression: simple_expression PLUS term  */
#line 578 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3230 "pascal.tab.c"
    break;

  case 240: /* simple_expression: simple_expression MINUS term  */
#line 579 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3236 "pascal.tab.c"
    break;

  case 241: /* simple_expression: simple_expression OROP term  */
#line 580 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3242 "pascal.tab.c"
    break;

  case 242: /* simple_expression: simple_expression XOROP term  */
#line 581 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3248 "pascal.tab.c"
    break;

  case 243: /* term: power  */
#line 584 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3254 "pascal.tab.c"
    break;

  case 244: /* term: term MUL power  */
#line 585 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3260 "pascal.tab.c"
    break;

  case 245: /* term: term RDIV power  */
#line 586 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3266 "pascal.tab.c"
    break;

  case 246: /* term: term IDIV power  */
#line 587 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3272 "pascal.tab.c"
    break;

  case 247: /* term: term IMOD power  */
#line 588 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3278 "pascal.tab.c"
    break;

  case 248: /* term: term ANDOP power  */
#line 589 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3284 "pascal.tab.c"
    break;

  case 249: /* term: term SHIFTOP power  */
#line 590 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3290 "pascal.tab.c"
    break;

  case 250: /* power: factor  */
#line 593 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3296 "pascal.tab.c"
    break;

  case 251: /* power: power POWOP factor  */
#line 594 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3302 "pascal.tab.c"
    break;

  case 252: /* factor: selector  */
#line 597 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3308 "pascal.tab.c"
    break;

  case 253: /* factor: call_with_args  */
#line 598 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3314 "pascal.tab.c"
    break;

  case 254: /* factor: INTCONST  */
#line 599 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3320 "pascal.tab.c"
    break;

  case 255: /* factor: REALCONST  */
#line 600 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3326 "pascal.tab.c"
    break;

  case 256: /* factor: STRINGCONST  */
#line 601 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 3332 "pascal.tab.c"
    break;

  case 257: /* factor: CHARCODE  */
#line 602 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3338 "pascal.tab.c"
    break;

  case 258: /* factor: LPARENT expression RPARENT  */
#line 603 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3344 "pascal.tab.c"
    break;

  case 259: /* factor: NOTSY factor  */
#line 604 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 3350 "pascal.tab.c"
    break;

  case 260: /* factor: ATSIGN factor  */
#line 605 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 3356 "pascal.tab.c"
    break;

  case 261: /* factor: LBRACK RBRACK  */
#line 606 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 3362 "pascal.tab.c"
    break;

  case 262: /* factor: LBRACK set_member_list RBRACK  */
#line 607 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 3368 "pascal.tab.c"
    break;

  case 263: /* set_member_list: set_member  */
#line 610 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3374 "pascal.tab.c"
    break;

  case 264: /* set_member_list: set_member_list COMMA set_member  */
#line 611 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3380 "pascal.tab.c"
    break;

  case 265: /* set_member: expression  */
#line 614 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3386 "pascal.tab.c"
    break;

  case 266: /* set_member: expression DOTDOT expression  */
#line 615 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3392 "pascal.tab.c"
    break;


#line 3396 "pascal.tab.c"

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

#line 621 "pascal.y"

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
