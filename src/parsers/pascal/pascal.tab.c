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
  YYSYMBOL_YYACCEPT = 78,                  /* $accept  */
  YYSYMBOL_compilation_unit = 79,          /* compilation_unit  */
  YYSYMBOL_program = 80,                   /* program  */
  YYSYMBOL_program_name = 81,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 82,          /* file_id_list_opt  */
  YYSYMBOL_block = 83,                     /* block  */
  YYSYMBOL_decl_part_list = 84,            /* decl_part_list  */
  YYSYMBOL_decl_part = 85,                 /* decl_part  */
  YYSYMBOL_unit = 86,                      /* unit  */
  YYSYMBOL_intf_part_list = 87,            /* intf_part_list  */
  YYSYMBOL_intf_part = 88,                 /* intf_part  */
  YYSYMBOL_init_opt = 89,                  /* init_opt  */
  YYSYMBOL_fini_opt = 90,                  /* fini_opt  */
  YYSYMBOL_label_list = 91,                /* label_list  */
  YYSYMBOL_const_decl_list = 92,           /* const_decl_list  */
  YYSYMBOL_const_decl = 93,                /* const_decl  */
  YYSYMBOL_typed_value = 94,               /* typed_value  */
  YYSYMBOL_typed_value_list = 95,          /* typed_value_list  */
  YYSYMBOL_field_value_list = 96,          /* field_value_list  */
  YYSYMBOL_constant = 97,                  /* constant  */
  YYSYMBOL_scalar_constant = 98,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 99,            /* type_decl_list  */
  YYSYMBOL_type_decl = 100,                /* type_decl  */
  YYSYMBOL_type = 101,                     /* type  */
  YYSYMBOL_102_1 = 102,                    /* $@1  */
  YYSYMBOL_103_2 = 103,                    /* $@2  */
  YYSYMBOL_104_3 = 104,                    /* $@3  */
  YYSYMBOL_packed_opt = 105,               /* packed_opt  */
  YYSYMBOL_simple_type = 106,              /* simple_type  */
  YYSYMBOL_lo_bound = 107,                 /* lo_bound  */
  YYSYMBOL_lo_term = 108,                  /* lo_term  */
  YYSYMBOL_b_expr = 109,                   /* b_expr  */
  YYSYMBOL_b_term = 110,                   /* b_term  */
  YYSYMBOL_b_factor = 111,                 /* b_factor  */
  YYSYMBOL_record_body = 112,              /* record_body  */
  YYSYMBOL_record_field_list = 113,        /* record_field_list  */
  YYSYMBOL_record_field = 114,             /* record_field  */
  YYSYMBOL_115_4 = 115,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 116,            /* case_arm_mark  */
  YYSYMBOL_case_open = 117,                /* case_open  */
  YYSYMBOL_record_case_opt = 118,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 119,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 120,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 121,            /* var_decl_list  */
  YYSYMBOL_var_decl = 122,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 123,           /* procedure_decl  */
  YYSYMBOL_124_5 = 124,                    /* $@5  */
  YYSYMBOL_125_6 = 125,                    /* $@6  */
  YYSYMBOL_126_7 = 126,                    /* $@7  */
  YYSYMBOL_pv_mark = 127,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 128,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 129,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 130,           /* parameter_decl  */
  YYSYMBOL_param_type = 131,               /* param_type  */
  YYSYMBOL_pf_params = 132,                /* pf_params  */
  YYSYMBOL_pf_sections = 133,              /* pf_sections  */
  YYSYMBOL_pf_section = 134,               /* pf_section  */
  YYSYMBOL_id_list = 135,                  /* id_list  */
  YYSYMBOL_body = 136,                     /* body  */
  YYSYMBOL_statement_list = 137,           /* statement_list  */
  YYSYMBOL_138_8 = 138,                    /* $@8  */
  YYSYMBOL_139_9 = 139,                    /* $@9  */
  YYSYMBOL_body_stmt = 140,                /* body_stmt  */
  YYSYMBOL_141_10 = 141,                   /* $@10  */
  YYSYMBOL_statement = 142,                /* statement  */
  YYSYMBOL_statement_no_label = 143,       /* statement_no_label  */
  YYSYMBOL_call = 144,                     /* call  */
  YYSYMBOL_call_with_args = 145,           /* call_with_args  */
  YYSYMBOL_argument_list = 146,            /* argument_list  */
  YYSYMBOL_argument = 147,                 /* argument  */
  YYSYMBOL_assignment = 148,               /* assignment  */
  YYSYMBOL_selector = 149,                 /* selector  */
  YYSYMBOL_expression_list = 150,          /* expression_list  */
  YYSYMBOL_compound_statement = 151,       /* compound_statement  */
  YYSYMBOL_goto_statement = 152,           /* goto_statement  */
  YYSYMBOL_if_statement = 153,             /* if_statement  */
  YYSYMBOL_case_statement = 154,           /* case_statement  */
  YYSYMBOL_155_11 = 155,                   /* $@11  */
  YYSYMBOL_case_tail = 156,                /* case_tail  */
  YYSYMBOL_case_list = 157,                /* case_list  */
  YYSYMBOL_case_elem = 158,                /* case_elem  */
  YYSYMBOL_constant_list = 159,            /* constant_list  */
  YYSYMBOL_case_label = 160,               /* case_label  */
  YYSYMBOL_while_statement = 161,          /* while_statement  */
  YYSYMBOL_repeat_statement = 162,         /* repeat_statement  */
  YYSYMBOL_for_statement = 163,            /* for_statement  */
  YYSYMBOL_with_statement = 164,           /* with_statement  */
  YYSYMBOL_with_open = 165,                /* with_open  */
  YYSYMBOL_expression = 166,               /* expression  */
  YYSYMBOL_simple_expression = 167,        /* simple_expression  */
  YYSYMBOL_term = 168,                     /* term  */
  YYSYMBOL_factor = 169,                   /* factor  */
  YYSYMBOL_set_member_list = 170,          /* set_member_list  */
  YYSYMBOL_set_member = 171                /* set_member  */
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
#define YYLAST   722

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  94
/* YYNRULES -- Number of rules.  */
#define YYNRULES  254
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  530

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   332


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
      75,    76,    77
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   184,   184,   185,   188,   189,   192,   193,   196,   197,
     200,   203,   204,   207,   208,   209,   210,   211,   212,   215,
     218,   219,   222,   223,   224,   225,   226,   227,   230,   231,
     232,   235,   236,   239,   240,   243,   244,   247,   248,   251,
     252,   253,   254,   257,   258,   261,   262,   265,   266,   267,
     270,   271,   272,   273,   274,   277,   278,   281,   284,   285,
     286,   286,   286,   287,   288,   289,   290,   291,   291,   292,
     293,   294,   297,   298,   301,   302,   303,   304,   307,   308,
     309,   310,   311,   314,   315,   316,   317,   318,   321,   322,
     323,   324,   325,   328,   329,   330,   331,   332,   335,   336,
     339,   342,   343,   346,   346,   347,   350,   353,   356,   357,
     358,   361,   362,   365,   366,   369,   370,   373,   374,   377,
     378,   379,   379,   380,   380,   381,   381,   384,   387,   388,
     391,   392,   395,   396,   397,   398,   399,   400,   403,   404,
     407,   408,   411,   412,   415,   416,   417,   418,   419,   422,
     423,   426,   429,   429,   430,   430,   433,   433,   436,   437,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   453,   454,   455,   458,   459,   460,   463,   464,   467,
     468,   469,   472,   473,   474,   475,   476,   479,   480,   481,
     482,   485,   486,   489,   492,   495,   496,   499,   499,   502,
     503,   504,   507,   508,   511,   512,   515,   516,   519,   520,
     523,   526,   529,   530,   531,   534,   537,   538,   545,   546,
     547,   548,   549,   550,   551,   552,   555,   556,   557,   558,
     559,   560,   561,   564,   565,   566,   567,   568,   569,   570,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   586,   587,   590,   591
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
  "IDENT", "SHIFTOP", "XOROP", "$accept", "compilation_unit", "program",
  "program_name", "file_id_list_opt", "block", "decl_part_list",
  "decl_part", "unit", "intf_part_list", "intf_part", "init_opt",
  "fini_opt", "label_list", "const_decl_list", "const_decl", "typed_value",
  "typed_value_list", "field_value_list", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "$@1", "$@2", "$@3", "packed_opt",
  "simple_type", "lo_bound", "lo_term", "b_expr", "b_term", "b_factor",
  "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "param_type", "pf_params", "pf_sections", "pf_section",
  "id_list", "body", "statement_list", "$@8", "$@9", "body_stmt", "$@10",
  "statement", "statement_no_label", "call", "call_with_args",
  "argument_list", "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@11", "case_tail", "case_list", "case_elem", "constant_list",
  "case_label", "while_statement", "repeat_statement", "for_statement",
  "with_statement", "with_open", "expression", "simple_expression", "term",
  "factor", "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-311)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-173)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     142,   -11,    -2,    76,  -311,    92,   562,  -311,  -311,   159,
      89,  -311,  -311,    63,    83,    95,   130,    95,   162,  -311,
     181,  -311,  -311,  -311,   211,    95,   194,   275,  -311,    33,
     124,    83,  -311,  -311,    36,  -311,    95,  -311,    29,  -311,
      40,    60,   268,   181,  -311,  -311,   139,  -311,  -311,  -311,
     250,   434,   300,  -311,  -311,   256,   270,  -311,   300,    67,
    -311,  -311,   259,   261,  -311,   434,   262,   434,  -311,   434,
     291,   253,  -311,  -311,  -311,  -311,  -311,   352,  -311,  -311,
    -311,  -311,  -311,  -311,  -311,  -311,   300,  -311,  -311,   293,
     542,  -311,   488,   488,   488,   396,   434,   488,  -311,  -311,
    -311,  -311,   294,  -311,   320,   116,   198,   348,  -311,   298,
    -311,   298,   368,   368,   287,    95,  -311,  -311,  -311,  -311,
     249,  -311,   327,   274,  -311,   217,    93,  -311,    23,   385,
      35,  -311,   346,    60,  -311,    13,    55,   212,  -311,   420,
      34,   558,    52,   632,   135,   411,   434,   333,  -311,   434,
     434,   434,   434,   434,   404,  -311,    83,    95,  -311,   336,
      95,   339,   181,  -311,  -311,   348,   348,  -311,   570,   266,
    -311,   582,  -311,   354,  -311,   434,   434,   434,   434,   434,
     434,   434,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,    39,  -311,   371,  -311,    93,    93,  -311,   164,
     373,   450,    28,   409,   423,  -311,   295,   295,   584,   295,
     295,   295,   295,    95,   388,    95,   391,    95,    31,  -311,
     154,   464,  -311,   434,  -311,   415,  -311,   434,   434,   434,
    -311,   418,  -311,   262,  -311,  -311,  -311,  -311,  -311,   231,
    -311,   644,   670,   302,   267,   670,   670,   670,   670,   670,
    -311,    83,    72,   557,  -311,    95,  -311,   181,   434,   434,
    -311,  -311,   416,   198,   198,   198,   198,   198,   198,   198,
     348,   348,   348,   348,  -311,  -311,  -311,  -311,  -311,  -311,
      95,   427,    95,   435,    37,  -311,   184,   439,  -311,   368,
     368,   436,  -311,   473,   523,   670,   300,   589,   300,   589,
      95,   584,  -311,   151,  -311,   151,   295,   295,   125,   151,
    -311,  -311,  -311,  -311,   284,   298,   358,   298,   395,    23,
    -311,    17,   524,  -311,   140,   526,   527,   384,   297,   670,
    -311,    60,  -311,  -311,   420,   584,   493,   434,  -311,   434,
     434,   434,  -311,  -311,  -311,  -311,   520,   270,   270,   670,
    -311,   432,   298,   437,   298,    39,  -311,    17,  -311,  -311,
    -311,  -311,   134,   480,    41,   582,  -311,  -311,   277,  -311,
    -311,   508,   115,  -311,   442,   108,   295,   295,   295,   295,
     151,   151,   295,   295,    17,  -311,    17,   481,    17,  -311,
     513,  -311,  -311,  -311,   537,  -311,  -311,   534,   434,   434,
    -311,  -311,   219,    43,  -311,   460,  -311,  -311,  -311,   657,
     255,   670,   548,   548,  -311,   528,   551,   498,    17,  -311,
      17,   507,  -311,  -311,   450,   450,   137,  -311,   589,   545,
    -311,    95,   500,  -311,  -311,  -311,  -311,  -311,  -311,  -311,
     151,   151,  -311,  -311,   501,  -311,   503,  -311,   574,  -311,
     335,   347,  -311,   584,   584,  -311,  -311,   552,   584,  -311,
    -311,   434,  -311,   548,   529,  -311,   506,  -311,  -311,   509,
    -311,  -311,   258,  -311,   531,   539,  -311,  -311,    16,   300,
    -311,  -311,  -311,   580,  -311,  -311,   125,  -311,   548,   548,
    -311,  -311,  -311,   670,  -311,   586,  -311,   450,  -311,   450,
     568,  -311,  -311,   538,  -311,  -311,  -311,  -311,  -311,  -311,
    -311,   300,   300,   584,   575,  -311,  -311,   597,  -311,   461,
    -311,   584,   541,   584,  -311,  -311,   597,    95,   550,  -311
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,   154,
       0,    11,    17,    10,     0,     0,     0,     0,    34,     0,
       0,    14,    36,   150,     0,   127,    16,   116,     0,   127,
       0,   170,     0,    15,    56,     7,     0,    12,    21,    13,
       0,     0,    73,    35,    18,     0,   129,   115,    73,   129,
     152,   151,     0,     0,   154,     0,     0,     0,   154,     0,
       0,   190,   155,   158,   161,   173,   160,     0,   162,   163,
     164,   165,   166,   167,   168,   169,    73,    55,     8,     0,
       0,    33,     0,     0,     0,     0,     0,     0,   242,   245,
     243,   244,   190,   241,   240,     0,   218,   226,   233,   141,
      72,   141,     0,     0,     0,     0,    51,    54,    52,    53,
      50,    83,     0,     0,    58,     0,    78,   149,     0,     0,
       0,   125,     0,   170,   194,     0,     0,     0,   190,   217,
       0,     0,     0,     0,   170,     0,     0,     0,   189,     0,
       0,     0,     0,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    20,   247,   227,   228,   249,   253,     0,
     251,     0,   248,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    50,    79,    80,    59,     0,
       0,     0,     0,    70,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   150,     0,   131,
       0,   121,   117,     0,    12,     0,   153,     0,     0,     0,
     156,     0,   156,     0,   197,   193,   156,   159,   175,     0,
     178,   179,   182,   188,     0,   192,   183,   184,   185,   186,
      57,    23,     0,    30,   127,    25,   127,    24,     0,     0,
     250,   246,   188,   219,   220,   221,   222,   223,   224,   225,
     231,   229,   230,   232,   236,   237,   238,   234,   235,   239,
       0,     0,     0,     0,     0,   143,     0,     0,    74,     0,
       0,     0,    47,     0,     0,    39,    73,     0,    73,     0,
     105,     0,    98,    81,    93,    82,     0,     0,    77,    88,
      85,    86,    84,    87,     0,   141,     0,   141,     0,     0,
     128,     0,     0,    12,     0,     0,     0,     0,     0,   211,
     210,   170,   188,   215,   216,   205,   195,     0,   174,     0,
       0,     0,   187,    22,   154,   154,    32,   129,   129,   254,
     252,     0,   141,     0,   141,     0,   140,     0,    65,    48,
      49,    76,   190,     0,     0,    39,    38,    63,     0,    71,
      69,     0,   110,   102,     0,     0,     0,     0,     0,     0,
      89,    90,     0,     0,     0,   132,     0,     0,     0,   130,
       0,   138,   135,   119,     0,   118,   126,   123,     0,     0,
     156,   157,   208,   199,   203,     0,   207,   156,   177,   180,
       0,   191,    28,    29,   154,     0,     0,     0,     0,   147,
       0,     0,   142,   144,     0,     0,     0,    41,     0,     0,
      68,   105,     0,   100,   103,    99,    95,    96,    94,    97,
      91,    92,   136,   134,     0,   137,     0,   122,     0,    12,
       0,     0,   214,     0,   205,   154,   154,     0,     0,   156,
     196,     0,   176,    31,     0,    26,     0,   146,   145,     0,
      45,    43,     0,    42,     0,     0,    60,   101,     0,    73,
     133,   139,   120,     0,   156,   156,   209,   202,   201,   200,
     198,   206,   204,   181,    19,     0,   148,     0,    40,     0,
       0,    61,   107,     0,   104,   124,   213,   212,    27,    44,
      46,    73,    73,   114,     0,    66,    62,   109,   112,     0,
     107,   114,     0,   114,   111,   106,   108,   105,     0,   113
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -311,  -311,  -311,  -311,  -311,   -42,   454,  -311,  -311,  -311,
    -311,  -311,  -311,  -311,   459,   -28,  -273,  -311,  -311,  -311,
     -51,   462,   -26,   -56,  -311,  -311,  -311,  -311,  -281,  -311,
     165,  -204,  -174,  -143,    98,  -311,   185,  -311,  -311,   106,
    -311,   104,   107,   476,   -30,  -311,  -311,  -311,  -311,   -24,
     -48,  -311,   321,  -245,   -97,  -311,   286,   -15,  -311,   -55,
    -311,  -311,  -193,  -311,  -112,   495,  -311,   -22,   304,   308,
    -311,   -29,  -311,  -311,  -311,  -311,  -311,  -311,  -311,  -311,
     195,  -310,   190,  -311,  -311,  -311,  -311,  -311,   -43,   491,
     -66,   -63,  -311,   392
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    26,     5,     6,    21,     7,    90,
     163,   346,   415,    29,    31,    32,   294,   472,   364,   291,
     302,    43,    44,   122,   501,   512,   300,   123,   124,   125,
     126,   402,   309,   304,   371,   372,   373,   479,   527,   513,
     433,   517,   518,    36,    37,    22,   323,   449,   224,    56,
     129,   218,   219,   392,   193,   284,   285,    38,    23,    40,
     133,    41,   330,   331,    72,    73,    74,   103,   239,   240,
      76,   104,   244,    78,    79,    80,    81,   335,   457,   403,
     404,   519,   406,    82,    83,    84,    85,   140,   295,   106,
     107,   108,   169,   170
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,   121,   130,    53,   308,    89,    57,   121,   105,   136,
      46,   132,    77,   142,   194,    59,   368,    87,   370,    75,
     363,   226,   137,   390,   141,   405,   143,   165,   166,   164,
     154,   213,   303,   305,   172,   121,   319,   139,    49,   333,
     222,    54,   355,   336,   502,    60,   426,   280,   454,   214,
     227,   232,   168,   171,   228,   215,   296,    60,   216,   455,
      60,   121,   121,    62,     8,   281,   310,   311,   312,   313,
      61,   282,   131,    10,   283,   503,    11,   343,   229,    63,
      64,    65,   235,   456,    66,    67,    55,   223,    58,   297,
      50,   233,   391,    55,    27,   320,    68,   375,   217,    69,
     199,   356,   241,   242,    77,   427,   245,   246,   247,   248,
     249,    75,   423,   220,    33,    77,   270,   271,   272,   273,
     431,   174,    75,   274,   275,   276,   277,   278,   279,    55,
     128,    70,   380,   381,    28,    71,   209,   210,    62,   442,
     432,   443,   252,   445,   405,   395,     1,   475,   211,   292,
      12,   470,   471,     2,    63,    64,    65,   175,    30,    66,
      67,   382,   383,   176,   177,   178,   179,   180,   181,   212,
      33,    68,   435,   467,    69,   468,    51,   286,   382,   383,
     324,   175,   325,    52,   327,   328,   329,   176,   177,   178,
     179,   180,   181,   424,   376,   377,    55,   145,   314,    47,
     316,   473,   318,    88,   334,    35,   378,   452,   440,   441,
      71,    55,   474,   321,   460,   349,   168,    24,   385,   401,
     387,    55,    25,    53,   509,    57,   510,   379,   288,   230,
     347,    87,   348,   436,   437,   438,   439,    39,   359,   360,
     367,    55,   369,   357,   182,   121,   121,   121,   121,   486,
     365,   183,   184,   175,   -75,   419,    42,   421,  -171,   176,
     177,   178,   179,   180,   181,   351,   492,   353,  -171,  -171,
     206,   207,   382,   383,   -75,   185,  -171,   196,   197,   -75,
     202,   394,   208,  -171,   453,   374,    45,    48,   337,   412,
     413,   506,   507,  -171,   241,   338,   409,   241,   411,   416,
     417,   -75,    77,   203,   220,   204,   -75,  -172,   205,    75,
     200,   -75,   337,   -75,   400,   497,   145,  -172,  -172,   462,
      86,    91,   498,   259,   341,  -172,   109,   110,   260,   342,
     134,   127,  -172,   128,   428,   111,   135,   138,   175,   429,
     286,    55,  -172,   384,   176,   177,   178,   179,   180,   181,
     144,   155,   484,   112,   113,   450,   451,   145,   301,   463,
     114,   192,   198,   115,   485,   340,   116,   117,   118,   119,
     195,   116,   117,   118,   119,   120,   175,   121,   173,   201,
     148,   149,   176,   177,   178,   179,   180,   181,   175,   146,
     221,   186,   187,   188,   176,   177,   178,   179,   180,   181,
     488,   489,   398,   189,   190,   225,   399,   483,   243,   250,
     147,   254,   148,   149,   256,    55,   374,   386,   493,   150,
     151,   152,   153,   504,   191,   175,   289,   290,   121,   262,
     287,   176,   177,   178,   179,   180,   181,   298,    92,   116,
     117,   118,   119,   195,   116,   117,   118,   119,   195,    93,
      94,   299,    55,    92,   388,   515,   516,    95,   167,    96,
     121,   121,    97,   315,    93,    94,   317,    98,    99,   100,
     101,   102,    95,   322,    96,   238,    92,    97,   231,   340,
     148,   149,    98,    99,   100,   101,   102,    93,    94,    55,
     326,   418,    92,   332,    55,    95,   420,    96,   361,    55,
      97,   434,   352,    93,    94,    98,    99,   100,   101,   102,
     354,    95,   374,   293,   358,    92,    97,   458,   458,   459,
     522,    98,    99,   100,   101,   102,    93,    94,   366,   393,
      92,   396,   397,   407,    95,   414,   293,   425,   430,    97,
     444,   446,   447,   448,    98,    99,   100,   101,   362,    95,
     156,    96,   157,    60,    97,   158,   465,   466,   464,    98,
      99,   100,   101,   102,    13,    14,   469,    15,   159,    13,
      14,   344,    15,   476,   160,   478,   480,   161,   481,   482,
     162,   495,   490,    16,   496,   505,   234,   494,    16,    17,
     499,   508,    18,   345,    17,    20,   511,    18,    19,   175,
      20,   500,   521,   520,   525,   176,   177,   178,   179,   180,
     181,   175,   253,   514,   529,   251,   477,   176,   177,   178,
     179,   180,   181,   175,   257,   528,   523,   526,   524,   176,
     177,   178,   179,   180,   181,   258,   255,   306,   307,   237,
     389,   422,   112,   113,   410,   408,   261,   301,   491,   487,
       0,   350,   115,     0,     0,   116,   117,   118,   119,   195,
     116,   117,   118,   119,   120,   236,   263,   264,   265,   266,
     267,   268,   269,   175,     0,     0,     0,     0,     0,   176,
     177,   178,   179,   180,   181,   175,     0,     0,     0,     0,
       0,   176,   177,   178,   179,   180,   181,     0,   175,     0,
       0,     0,     0,   339,   176,   177,   178,   179,   180,   181,
       0,   175,     0,     0,     0,     0,   461,   176,   177,   178,
     179,   180,   181
};

static const yytype_int16 yycheck[] =
{
      15,    52,    58,    31,   208,    47,    36,    58,    51,    64,
      25,    59,    41,    68,   111,    39,   297,    43,   299,    41,
     293,   133,    65,     6,    67,   335,    69,    93,    94,    92,
      86,     8,   206,   207,    97,    86,     5,    66,     5,   232,
       5,     5,     5,   236,    28,     5,     5,     8,     5,    26,
      37,    17,    95,    96,    41,    32,    28,     5,    35,    16,
       5,   112,   113,     3,    75,    26,   209,   210,   211,   212,
      30,    32,     5,    75,    35,    59,     0,     5,    23,    19,
      20,    21,    30,    40,    24,    25,    57,    52,    59,    61,
      57,    57,    75,    57,     5,    64,    36,   301,    75,    39,
     115,    64,   145,   146,   133,    64,   149,   150,   151,   152,
     153,   133,   357,   128,    75,   144,   182,   183,   184,   185,
       5,     5,   144,   186,   187,   188,   189,   190,   191,    57,
      63,    71,   306,   307,    71,    75,    43,    44,     3,   384,
      25,   386,   157,   388,   454,     5,     4,   428,    55,   200,
      58,   424,   425,    11,    19,    20,    21,    41,    75,    24,
      25,    53,    54,    47,    48,    49,    50,    51,    52,    76,
      75,    36,    64,   418,    39,   420,    52,   192,    53,    54,
     223,    41,   224,    59,   227,   228,   229,    47,    48,    49,
      50,    51,    52,    59,    43,    44,    57,    63,   213,     5,
     215,    64,   217,    64,   233,    75,    55,   400,   382,   383,
      75,    57,    75,    59,   407,   258,   259,    58,   315,   331,
     317,    57,    63,   251,   497,   255,   499,    76,    64,    17,
     254,   257,   256,   376,   377,   378,   379,    75,   289,   290,
     296,    57,   298,    59,    46,   296,   297,   298,   299,   453,
     293,    53,    54,    41,     5,   352,    75,   354,     5,    47,
      48,    49,    50,    51,    52,   280,   459,   282,    15,    16,
      53,    54,    53,    54,    25,    77,    23,   112,   113,    30,
       6,   323,    65,    30,    65,   300,    75,    12,    57,   344,
     345,   484,   485,    40,   337,    64,   339,   340,   341,   347,
     348,    52,   331,    29,   319,    31,    57,     5,    34,   331,
      61,    62,    57,    64,    17,    57,    63,    15,    16,    64,
      52,    71,    64,    57,    57,    23,    26,    27,    62,    62,
      71,    75,    30,    63,    57,    35,    75,    75,    41,    62,
     355,    57,    40,    59,    47,    48,    49,    50,    51,    52,
      59,    58,    17,    53,    54,   398,   399,    63,    63,   414,
      60,    63,    75,    63,    17,    63,    71,    72,    73,    74,
      75,    71,    72,    73,    74,    75,    41,   428,    58,    52,
      60,    61,    47,    48,    49,    50,    51,    52,    41,    37,
       5,    43,    44,    45,    47,    48,    49,    50,    51,    52,
     455,   456,    18,    55,    56,    59,    22,   449,    75,     5,
      58,    75,    60,    61,    75,    57,   431,    59,   461,    67,
      68,    69,    70,   479,    76,    41,    53,    54,   479,    75,
      59,    47,    48,    49,    50,    51,    52,    28,    42,    71,
      72,    73,    74,    75,    71,    72,    73,    74,    75,    53,
      54,    28,    57,    42,    59,   511,   512,    61,    62,    63,
     511,   512,    66,    75,    53,    54,    75,    71,    72,    73,
      74,    75,    61,     9,    63,    64,    42,    66,    58,    63,
      60,    61,    71,    72,    73,    74,    75,    53,    54,    57,
      75,    59,    42,    75,    57,    61,    59,    63,    62,    57,
      66,    59,    75,    53,    54,    71,    72,    73,    74,    75,
      75,    61,   527,    63,    75,    42,    66,    57,    57,    59,
      59,    71,    72,    73,    74,    75,    53,    54,     5,     5,
      42,     5,     5,    40,    61,    15,    63,    57,    30,    66,
      59,    28,     5,     9,    71,    72,    73,    74,    75,    61,
       8,    63,    10,     5,    66,    13,     5,    59,    30,    71,
      72,    73,    74,    75,     7,     8,    59,    10,    26,     7,
       8,    14,    10,    28,    32,    75,    75,    35,    75,     5,
      38,    75,    30,    26,    75,     5,    28,    58,    26,    32,
      59,     5,    35,    36,    32,    38,    28,    35,    36,    41,
      38,    62,     5,    28,    63,    47,    48,    49,    50,    51,
      52,    41,   158,    75,    64,   156,   431,    47,    48,    49,
      50,    51,    52,    41,   162,   527,   520,   523,   521,    47,
      48,    49,    50,    51,    52,    65,   160,    53,    54,   144,
     319,   355,    53,    54,   340,   337,    64,    63,   458,   454,
      -1,   259,    63,    -1,    -1,    71,    72,    73,    74,    75,
      71,    72,    73,    74,    75,    33,   175,   176,   177,   178,
     179,   180,   181,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    41,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,    50,    51,    52,    -1,    41,    -1,
      -1,    -1,    -1,    59,    47,    48,    49,    50,    51,    52,
      -1,    41,    -1,    -1,    -1,    -1,    59,    47,    48,    49,
      50,    51,    52
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    79,    80,    83,    84,    86,    75,    81,
      75,     0,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    85,   123,   136,    58,    63,    82,     5,    71,    91,
      75,    92,    93,    75,   135,    75,   121,   122,   135,    75,
     137,   139,    75,    99,   100,    75,   135,     5,    12,     5,
      57,    52,    59,    93,     5,    57,   127,   122,    59,   127,
       5,    30,     3,    19,    20,    21,    24,    25,    36,    39,
      71,    75,   142,   143,   144,   145,   148,   149,   151,   152,
     153,   154,   161,   162,   163,   164,    52,   100,    64,    83,
      87,    71,    42,    53,    54,    61,    63,    66,    71,    72,
      73,    74,    75,   145,   149,   166,   167,   168,   169,    26,
      27,    35,    53,    54,    60,    63,    71,    72,    73,    74,
      75,    98,   101,   105,   106,   107,   108,    75,    63,   128,
     101,     5,   128,   138,    71,    75,   137,   166,    75,   149,
     165,   166,   137,   166,    59,    63,    37,    58,    60,    61,
      67,    68,    69,    70,   101,    58,     8,    10,    13,    26,
      32,    35,    38,    88,   169,   168,   168,    62,   166,   170,
     171,   166,   169,    58,     5,    41,    47,    48,    49,    50,
      51,    52,    46,    53,    54,    77,    43,    44,    45,    55,
      56,    76,    63,   132,   132,    75,   108,   108,    75,   135,
      61,    52,     6,    29,    31,    34,    53,    54,    65,    43,
      44,    55,    76,     8,    26,    32,    35,    75,   129,   130,
     135,     5,     5,    52,   126,    59,   142,    37,    41,    23,
      17,    58,    17,    57,    28,    30,    33,   143,    64,   146,
     147,   166,   166,    75,   150,   166,   166,   166,   166,   166,
       5,    92,   135,    84,    75,   121,    75,    99,    65,    57,
      62,    64,    75,   167,   167,   167,   167,   167,   167,   167,
     168,   168,   168,   168,   169,   169,   169,   169,   169,   169,
       8,    26,    32,    35,   133,   134,   135,    59,    64,    53,
      54,    97,    98,    63,    94,   166,    28,    61,    28,    28,
     104,    63,    98,   110,   111,   110,    53,    54,   109,   110,
     111,   111,   111,   111,   135,    75,   135,    75,   135,     5,
      64,    59,     9,   124,   166,    83,    75,   166,   166,   166,
     140,   141,    75,   140,   149,   155,   140,    57,    64,    59,
      63,    57,    62,     5,    14,    36,    89,   127,   127,   166,
     171,   135,    75,   135,    75,     5,    64,    59,    75,    98,
      98,    62,    75,    94,    96,   166,     5,   101,   106,   101,
     106,   112,   113,   114,   135,   109,    43,    44,    55,    76,
     110,   110,    53,    54,    59,   132,    59,   132,    59,   130,
       6,    75,   131,     5,    83,     5,     5,     5,    18,    22,
      17,   142,   109,   157,   158,   159,   160,    40,   147,   166,
     146,   166,   137,   137,    15,    90,   128,   128,    59,   132,
      59,   132,   134,   131,    59,    57,     5,    64,    57,    62,
      30,     5,    25,   118,    59,    64,   111,   111,   111,   111,
     110,   110,   131,   131,    59,   131,    28,     5,     9,   125,
     166,   166,   140,    65,     5,    16,    40,   156,    57,    59,
     140,    59,    64,   137,    30,     5,    59,   131,   131,    59,
      94,    94,    95,    64,    75,   106,    28,   114,    75,   115,
      75,    75,     5,    83,    17,    17,   109,   158,   137,   137,
      30,   160,   140,   166,    58,    75,    75,    57,    64,    59,
      62,   102,    28,    59,   101,     5,   140,   140,     5,    94,
      94,    28,   103,   117,    75,   101,   101,   119,   120,   159,
      28,     5,    59,   117,   120,    63,   119,   116,   112,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    80,    80,    81,    81,    82,    82,
      83,    84,    84,    85,    85,    85,    85,    85,    85,    86,
      87,    87,    88,    88,    88,    88,    88,    88,    89,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    94,    94,    95,    95,    96,    96,    97,    97,    97,
      98,    98,    98,    98,    98,    99,    99,   100,   101,   101,
     102,   103,   101,   101,   101,   101,   101,   104,   101,   101,
     101,   101,   105,   105,   106,   106,   106,   106,   107,   107,
     107,   107,   107,   108,   108,   108,   108,   108,   109,   109,
     109,   109,   109,   110,   110,   110,   110,   110,   111,   111,
     112,   113,   113,   115,   114,   114,   116,   117,   118,   118,
     118,   119,   119,   120,   120,   121,   121,   122,   122,   123,
     123,   124,   123,   125,   123,   126,   123,   127,   128,   128,
     129,   129,   130,   130,   130,   130,   130,   130,   131,   131,
     132,   132,   133,   133,   134,   134,   134,   134,   134,   135,
     135,   136,   138,   137,   139,   137,   141,   140,   142,   142,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   144,   144,   144,   145,   145,   145,   146,   146,   147,
     147,   147,   148,   148,   148,   148,   148,   149,   149,   149,
     149,   150,   150,   151,   152,   153,   153,   155,   154,   156,
     156,   156,   157,   157,   158,   158,   159,   159,   160,   160,
     161,   162,   163,   163,   163,   164,   165,   165,   166,   166,
     166,   166,   166,   166,   166,   166,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   170,   170,   171,   171
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,    11,
       2,     0,     3,     2,     2,     2,     5,     7,     2,     2,
       0,     2,     0,     3,     1,     2,     1,     4,     6,     1,
       5,     3,     4,     1,     3,     3,     5,     1,     2,     2,
       1,     1,     1,     1,     1,     2,     1,     4,     1,     2,
       0,     0,     9,     4,     2,     4,     9,     0,     5,     4,
       2,     4,     1,     0,     3,     1,     4,     3,     1,     2,
       2,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       2,     3,     1,     0,     4,     0,     0,     0,     7,     5,
       0,     3,     1,     6,     0,     2,     1,     4,     6,     7,
       9,     0,     8,     0,    10,     0,     7,     0,     3,     0,
       3,     1,     3,     5,     4,     3,     4,     4,     1,     3,
       3,     0,     3,     1,     3,     4,     4,     3,     5,     3,
       1,     3,     0,     4,     0,     2,     0,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     3,     1,     4,     3,     6,     3,     1,     1,
       3,     5,     3,     3,     3,     3,     3,     4,     3,     2,
       1,     3,     1,     3,     2,     4,     6,     0,     7,     0,
       2,     2,     3,     1,     3,     0,     3,     1,     1,     3,
       4,     4,     8,     8,     6,     4,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     3,
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
#line 184 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1786 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 185 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1792 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 188 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1798 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 189 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1804 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 192 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1810 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 193 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 1816 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 196 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1822 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 197 "pascal.y"
     { (yyval.node) = NULL; }
#line 1828 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 200 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1834 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 203 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1840 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 204 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1846 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 207 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1852 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 208 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1858 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 209 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1864 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 210 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1870 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 211 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1876 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 212 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1882 "pascal.tab.c"
    break;

  case 19: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 215 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1888 "pascal.tab.c"
    break;

  case 20: /* intf_part_list: intf_part_list intf_part  */
#line 218 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1894 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: %empty  */
#line 219 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 1900 "pascal.tab.c"
    break;

  case 22: /* intf_part: USESSY id_list SEMICOLON  */
#line 222 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 1906 "pascal.tab.c"
    break;

  case 23: /* intf_part: CONSTSY const_decl_list  */
#line 223 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1912 "pascal.tab.c"
    break;

  case 24: /* intf_part: TYPESY type_decl_list  */
#line 224 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1918 "pascal.tab.c"
    break;

  case 25: /* intf_part: VARSY var_decl_list  */
#line 225 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1924 "pascal.tab.c"
    break;

  case 26: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 226 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1930 "pascal.tab.c"
    break;

  case 27: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 227 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 1936 "pascal.tab.c"
    break;

  case 28: /* init_opt: INITIALIZATIONSY statement_list  */
#line 230 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1942 "pascal.tab.c"
    break;

  case 29: /* init_opt: BEGINSY statement_list  */
#line 231 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 1948 "pascal.tab.c"
    break;

  case 30: /* init_opt: %empty  */
#line 232 "pascal.y"
     { (yyval.node) = NULL; }
#line 1954 "pascal.tab.c"
    break;

  case 31: /* fini_opt: FINALIZATIONSY statement_list  */
#line 235 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 1960 "pascal.tab.c"
    break;

  case 32: /* fini_opt: %empty  */
#line 236 "pascal.y"
     { (yyval.node) = NULL; }
#line 1966 "pascal.tab.c"
    break;

  case 33: /* label_list: label_list COMMA INTCONST  */
#line 239 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1972 "pascal.tab.c"
    break;

  case 34: /* label_list: INTCONST  */
#line 240 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1978 "pascal.tab.c"
    break;

  case 35: /* const_decl_list: const_decl_list const_decl  */
#line 243 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1984 "pascal.tab.c"
    break;

  case 36: /* const_decl_list: const_decl  */
#line 244 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1990 "pascal.tab.c"
    break;

  case 37: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 247 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 1996 "pascal.tab.c"
    break;

  case 38: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 248 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2002 "pascal.tab.c"
    break;

  case 39: /* typed_value: expression  */
#line 251 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2008 "pascal.tab.c"
    break;

  case 40: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 252 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2014 "pascal.tab.c"
    break;

  case 41: /* typed_value: LPARENT field_value_list RPARENT  */
#line 253 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2020 "pascal.tab.c"
    break;

  case 42: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 254 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2026 "pascal.tab.c"
    break;

  case 43: /* typed_value_list: typed_value  */
#line 257 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2032 "pascal.tab.c"
    break;

  case 44: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 258 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2038 "pascal.tab.c"
    break;

  case 45: /* field_value_list: IDENT COLON typed_value  */
#line 261 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2044 "pascal.tab.c"
    break;

  case 46: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 262 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2050 "pascal.tab.c"
    break;

  case 47: /* constant: scalar_constant  */
#line 265 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2056 "pascal.tab.c"
    break;

  case 48: /* constant: PLUS scalar_constant  */
#line 266 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2062 "pascal.tab.c"
    break;

  case 49: /* constant: MINUS scalar_constant  */
#line 267 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2068 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 270 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2074 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 271 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2080 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 272 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2086 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 273 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2092 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 274 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2098 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 277 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2104 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 278 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2110 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 281 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2116 "pascal.tab.c"
    break;

  case 58: /* type: simple_type  */
#line 284 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2122 "pascal.tab.c"
    break;

  case 59: /* type: ARROW IDENT  */
#line 285 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2128 "pascal.tab.c"
    break;

  case 60: /* $@1: %empty  */
#line 286 "pascal.y"
                                                       { }
#line 2134 "pascal.tab.c"
    break;

  case 61: /* $@2: %empty  */
#line 286 "pascal.y"
                                                           { }
#line 2140 "pascal.tab.c"
    break;

  case 62: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 286 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2146 "pascal.tab.c"
    break;

  case 63: /* type: packed_opt ARRAYSY OFSY type  */
#line 287 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2152 "pascal.tab.c"
    break;

  case 64: /* type: PROCEDURESY pf_params  */
#line 288 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2158 "pascal.tab.c"
    break;

  case 65: /* type: FUNCTIONSY pf_params COLON IDENT  */
#line 289 "pascal.y"
                                      { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2164 "pascal.tab.c"
    break;

  case 66: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 290 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2170 "pascal.tab.c"
    break;

  case 67: /* $@3: %empty  */
#line 291 "pascal.y"
                         { }
#line 2176 "pascal.tab.c"
    break;

  case 68: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 291 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2182 "pascal.tab.c"
    break;

  case 69: /* type: packed_opt SETSY OFSY simple_type  */
#line 292 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2188 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt FILESY  */
#line 293 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2194 "pascal.tab.c"
    break;

  case 71: /* type: packed_opt FILESY OFSY type  */
#line 294 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2200 "pascal.tab.c"
    break;

  case 72: /* packed_opt: PACKEDSY  */
#line 297 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2206 "pascal.tab.c"
    break;

  case 73: /* packed_opt: %empty  */
#line 298 "pascal.y"
     { (yyval.node) = NULL; }
#line 2212 "pascal.tab.c"
    break;

  case 74: /* simple_type: LPARENT id_list RPARENT  */
#line 301 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2218 "pascal.tab.c"
    break;

  case 75: /* simple_type: IDENT  */
#line 302 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2224 "pascal.tab.c"
    break;

  case 76: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 303 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2230 "pascal.tab.c"
    break;

  case 77: /* simple_type: lo_bound DOTDOT b_expr  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2236 "pascal.tab.c"
    break;

  case 78: /* lo_bound: lo_term  */
#line 307 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2242 "pascal.tab.c"
    break;

  case 79: /* lo_bound: PLUS lo_term  */
#line 308 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2248 "pascal.tab.c"
    break;

  case 80: /* lo_bound: MINUS lo_term  */
#line 309 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2254 "pascal.tab.c"
    break;

  case 81: /* lo_bound: lo_bound PLUS b_term  */
#line 310 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2260 "pascal.tab.c"
    break;

  case 82: /* lo_bound: lo_bound MINUS b_term  */
#line 311 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2266 "pascal.tab.c"
    break;

  case 83: /* lo_term: scalar_constant  */
#line 314 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2272 "pascal.tab.c"
    break;

  case 84: /* lo_term: lo_term MUL b_factor  */
#line 315 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2278 "pascal.tab.c"
    break;

  case 85: /* lo_term: lo_term IDIV b_factor  */
#line 316 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2284 "pascal.tab.c"
    break;

  case 86: /* lo_term: lo_term IMOD b_factor  */
#line 317 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2290 "pascal.tab.c"
    break;

  case 87: /* lo_term: lo_term SHIFTOP b_factor  */
#line 318 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2296 "pascal.tab.c"
    break;

  case 88: /* b_expr: b_term  */
#line 321 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2302 "pascal.tab.c"
    break;

  case 89: /* b_expr: PLUS b_term  */
#line 322 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2308 "pascal.tab.c"
    break;

  case 90: /* b_expr: MINUS b_term  */
#line 323 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2314 "pascal.tab.c"
    break;

  case 91: /* b_expr: b_expr PLUS b_term  */
#line 324 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2320 "pascal.tab.c"
    break;

  case 92: /* b_expr: b_expr MINUS b_term  */
#line 325 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2326 "pascal.tab.c"
    break;

  case 93: /* b_term: b_factor  */
#line 328 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2332 "pascal.tab.c"
    break;

  case 94: /* b_term: b_term MUL b_factor  */
#line 329 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2338 "pascal.tab.c"
    break;

  case 95: /* b_term: b_term IDIV b_factor  */
#line 330 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2344 "pascal.tab.c"
    break;

  case 96: /* b_term: b_term IMOD b_factor  */
#line 331 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2350 "pascal.tab.c"
    break;

  case 97: /* b_term: b_term SHIFTOP b_factor  */
#line 332 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2356 "pascal.tab.c"
    break;

  case 98: /* b_factor: scalar_constant  */
#line 335 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2362 "pascal.tab.c"
    break;

  case 99: /* b_factor: LPARENT b_expr RPARENT  */
#line 336 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2368 "pascal.tab.c"
    break;

  case 100: /* record_body: record_field_list record_case_opt  */
#line 339 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2374 "pascal.tab.c"
    break;

  case 101: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 342 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2380 "pascal.tab.c"
    break;

  case 102: /* record_field_list: record_field  */
#line 343 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2386 "pascal.tab.c"
    break;

  case 103: /* $@4: %empty  */
#line 346 "pascal.y"
                  { }
#line 2392 "pascal.tab.c"
    break;

  case 104: /* record_field: id_list COLON $@4 type  */
#line 346 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2398 "pascal.tab.c"
    break;

  case 105: /* record_field: %empty  */
#line 347 "pascal.y"
     { (yyval.node) = NULL; }
#line 2404 "pascal.tab.c"
    break;

  case 106: /* case_arm_mark: %empty  */
#line 350 "pascal.y"
    { }
#line 2410 "pascal.tab.c"
    break;

  case 107: /* case_open: %empty  */
#line 353 "pascal.y"
    { }
#line 2416 "pascal.tab.c"
    break;

  case 108: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 356 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2422 "pascal.tab.c"
    break;

  case 109: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 357 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2428 "pascal.tab.c"
    break;

  case 110: /* record_case_opt: %empty  */
#line 358 "pascal.y"
     { (yyval.node) = NULL; }
#line 2434 "pascal.tab.c"
    break;

  case 111: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 361 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2440 "pascal.tab.c"
    break;

  case 112: /* record_case_list: record_case_arm  */
#line 362 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2446 "pascal.tab.c"
    break;

  case 113: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 365 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2452 "pascal.tab.c"
    break;

  case 114: /* record_case_arm: %empty  */
#line 366 "pascal.y"
     { (yyval.node) = NULL; }
#line 2458 "pascal.tab.c"
    break;

  case 115: /* var_decl_list: var_decl_list var_decl  */
#line 369 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2464 "pascal.tab.c"
    break;

  case 116: /* var_decl_list: var_decl  */
#line 370 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2470 "pascal.tab.c"
    break;

  case 117: /* var_decl: id_list COLON type SEMICOLON  */
#line 373 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2476 "pascal.tab.c"
    break;

  case 118: /* var_decl: id_list COLON type EQOP expression SEMICOLON  */
#line 374 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2482 "pascal.tab.c"
    break;

  case 119: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 377 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2488 "pascal.tab.c"
    break;

  case 120: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 378 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2494 "pascal.tab.c"
    break;

  case 121: /* $@5: %empty  */
#line 379 "pascal.y"
                                                            { }
#line 2500 "pascal.tab.c"
    break;

  case 122: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 379 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2506 "pascal.tab.c"
    break;

  case 123: /* $@6: %empty  */
#line 380 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); }
#line 2512 "pascal.tab.c"
    break;

  case 124: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 380 "pascal.y"
                                                                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2518 "pascal.tab.c"
    break;

  case 125: /* $@7: %empty  */
#line 381 "pascal.y"
                                        { }
#line 2524 "pascal.tab.c"
    break;

  case 126: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 381 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2530 "pascal.tab.c"
    break;

  case 127: /* pv_mark: %empty  */
#line 384 "pascal.y"
    { }
#line 2536 "pascal.tab.c"
    break;

  case 128: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 387 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2542 "pascal.tab.c"
    break;

  case 129: /* parameter_list_opt: %empty  */
#line 388 "pascal.y"
     { (yyval.node) = NULL; }
#line 2548 "pascal.tab.c"
    break;

  case 130: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 391 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2554 "pascal.tab.c"
    break;

  case 131: /* parameter_decl_list: parameter_decl  */
#line 392 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2560 "pascal.tab.c"
    break;

  case 132: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2566 "pascal.tab.c"
    break;

  case 133: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 396 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2572 "pascal.tab.c"
    break;

  case 134: /* parameter_decl: VARSY id_list COLON param_type  */
#line 397 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2578 "pascal.tab.c"
    break;

  case 135: /* parameter_decl: id_list COLON param_type  */
#line 398 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2584 "pascal.tab.c"
    break;

  case 136: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 399 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2590 "pascal.tab.c"
    break;

  case 137: /* parameter_decl: IDENT id_list COLON param_type  */
#line 400 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2596 "pascal.tab.c"
    break;

  case 138: /* param_type: IDENT  */
#line 403 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2602 "pascal.tab.c"
    break;

  case 139: /* param_type: ARRAYSY OFSY IDENT  */
#line 404 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 2608 "pascal.tab.c"
    break;

  case 140: /* pf_params: LPARENT pf_sections RPARENT  */
#line 407 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2614 "pascal.tab.c"
    break;

  case 141: /* pf_params: %empty  */
#line 408 "pascal.y"
     { (yyval.node) = NULL; }
#line 2620 "pascal.tab.c"
    break;

  case 142: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 411 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2626 "pascal.tab.c"
    break;

  case 143: /* pf_sections: pf_section  */
#line 412 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2632 "pascal.tab.c"
    break;

  case 144: /* pf_section: id_list COLON param_type  */
#line 415 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2638 "pascal.tab.c"
    break;

  case 145: /* pf_section: VARSY id_list COLON param_type  */
#line 416 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2644 "pascal.tab.c"
    break;

  case 146: /* pf_section: CONSTSY id_list COLON param_type  */
#line 417 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 2650 "pascal.tab.c"
    break;

  case 147: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 418 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2656 "pascal.tab.c"
    break;

  case 148: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 419 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2662 "pascal.tab.c"
    break;

  case 149: /* id_list: id_list COMMA IDENT  */
#line 422 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2668 "pascal.tab.c"
    break;

  case 150: /* id_list: IDENT  */
#line 423 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2674 "pascal.tab.c"
    break;

  case 151: /* body: BEGINSY statement_list ENDSY  */
#line 426 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2680 "pascal.tab.c"
    break;

  case 152: /* $@8: %empty  */
#line 429 "pascal.y"
                             { pas_stmt_mark(); }
#line 2686 "pascal.tab.c"
    break;

  case 153: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 429 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2692 "pascal.tab.c"
    break;

  case 154: /* $@9: %empty  */
#line 430 "pascal.y"
     { pas_stmt_mark(); }
#line 2698 "pascal.tab.c"
    break;

  case 155: /* statement_list: $@9 statement  */
#line 430 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2704 "pascal.tab.c"
    break;

  case 156: /* $@10: %empty  */
#line 433 "pascal.y"
    { pas_stmt_mark(); }
#line 2710 "pascal.tab.c"
    break;

  case 157: /* body_stmt: $@10 statement  */
#line 433 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2716 "pascal.tab.c"
    break;

  case 158: /* statement: statement_no_label  */
#line 436 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2722 "pascal.tab.c"
    break;

  case 159: /* statement: INTCONST COLON statement_no_label  */
#line 437 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2728 "pascal.tab.c"
    break;

  case 160: /* statement_no_label: assignment  */
#line 440 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2734 "pascal.tab.c"
    break;

  case 161: /* statement_no_label: call  */
#line 441 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2740 "pascal.tab.c"
    break;

  case 162: /* statement_no_label: compound_statement  */
#line 442 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2746 "pascal.tab.c"
    break;

  case 163: /* statement_no_label: goto_statement  */
#line 443 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2752 "pascal.tab.c"
    break;

  case 164: /* statement_no_label: if_statement  */
#line 444 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2758 "pascal.tab.c"
    break;

  case 165: /* statement_no_label: case_statement  */
#line 445 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2764 "pascal.tab.c"
    break;

  case 166: /* statement_no_label: while_statement  */
#line 446 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2770 "pascal.tab.c"
    break;

  case 167: /* statement_no_label: repeat_statement  */
#line 447 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2776 "pascal.tab.c"
    break;

  case 168: /* statement_no_label: for_statement  */
#line 448 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2782 "pascal.tab.c"
    break;

  case 169: /* statement_no_label: with_statement  */
#line 449 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2788 "pascal.tab.c"
    break;

  case 170: /* statement_no_label: %empty  */
#line 450 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2794 "pascal.tab.c"
    break;

  case 171: /* call: IDENT  */
#line 453 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2800 "pascal.tab.c"
    break;

  case 172: /* call: selector PERIOD IDENT  */
#line 454 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2806 "pascal.tab.c"
    break;

  case 173: /* call: call_with_args  */
#line 455 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2812 "pascal.tab.c"
    break;

  case 174: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 458 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2818 "pascal.tab.c"
    break;

  case 175: /* call_with_args: IDENT LPARENT RPARENT  */
#line 459 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 2824 "pascal.tab.c"
    break;

  case 176: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 460 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 2830 "pascal.tab.c"
    break;

  case 177: /* argument_list: argument_list COMMA argument  */
#line 463 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2836 "pascal.tab.c"
    break;

  case 178: /* argument_list: argument  */
#line 464 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2842 "pascal.tab.c"
    break;

  case 179: /* argument: expression  */
#line 467 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2848 "pascal.tab.c"
    break;

  case 180: /* argument: expression COLON expression  */
#line 468 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2854 "pascal.tab.c"
    break;

  case 181: /* argument: expression COLON expression COLON expression  */
#line 469 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2860 "pascal.tab.c"
    break;

  case 182: /* assignment: selector BECOMES expression  */
#line 472 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2866 "pascal.tab.c"
    break;

  case 183: /* assignment: selector ADDBECOMES expression  */
#line 473 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 2872 "pascal.tab.c"
    break;

  case 184: /* assignment: selector SUBBECOMES expression  */
#line 474 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 2878 "pascal.tab.c"
    break;

  case 185: /* assignment: selector MULBECOMES expression  */
#line 475 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 2884 "pascal.tab.c"
    break;

  case 186: /* assignment: selector DIVBECOMES expression  */
#line 476 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 2890 "pascal.tab.c"
    break;

  case 187: /* selector: selector LBRACK expression_list RBRACK  */
#line 479 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2896 "pascal.tab.c"
    break;

  case 188: /* selector: selector PERIOD IDENT  */
#line 480 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2902 "pascal.tab.c"
    break;

  case 189: /* selector: selector ARROW  */
#line 481 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2908 "pascal.tab.c"
    break;

  case 190: /* selector: IDENT  */
#line 482 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2914 "pascal.tab.c"
    break;

  case 191: /* expression_list: expression_list COMMA expression  */
#line 485 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2920 "pascal.tab.c"
    break;

  case 192: /* expression_list: expression  */
#line 486 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2926 "pascal.tab.c"
    break;

  case 193: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 489 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2932 "pascal.tab.c"
    break;

  case 194: /* goto_statement: GOTOSY INTCONST  */
#line 492 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2938 "pascal.tab.c"
    break;

  case 195: /* if_statement: IFSY expression THENSY body_stmt  */
#line 495 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2944 "pascal.tab.c"
    break;

  case 196: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 496 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2950 "pascal.tab.c"
    break;

  case 197: /* $@11: %empty  */
#line 499 "pascal.y"
                           { }
#line 2956 "pascal.tab.c"
    break;

  case 198: /* case_statement: CASESY expression OFSY $@11 case_list case_tail ENDSY  */
#line 499 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2962 "pascal.tab.c"
    break;

  case 199: /* case_tail: %empty  */
#line 502 "pascal.y"
    { (yyval.node) = NULL; }
#line 2968 "pascal.tab.c"
    break;

  case 200: /* case_tail: ELSESY statement_list  */
#line 503 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 2974 "pascal.tab.c"
    break;

  case 201: /* case_tail: OTHERWISESY statement_list  */
#line 504 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 2980 "pascal.tab.c"
    break;

  case 202: /* case_list: case_list SEMICOLON case_elem  */
#line 507 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2986 "pascal.tab.c"
    break;

  case 203: /* case_list: case_elem  */
#line 508 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2992 "pascal.tab.c"
    break;

  case 204: /* case_elem: constant_list COLON body_stmt  */
#line 511 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2998 "pascal.tab.c"
    break;

  case 205: /* case_elem: %empty  */
#line 512 "pascal.y"
     { (yyval.node) = NULL; }
#line 3004 "pascal.tab.c"
    break;

  case 206: /* constant_list: constant_list COMMA case_label  */
#line 515 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3010 "pascal.tab.c"
    break;

  case 207: /* constant_list: case_label  */
#line 516 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3016 "pascal.tab.c"
    break;

  case 208: /* case_label: b_expr  */
#line 519 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3022 "pascal.tab.c"
    break;

  case 209: /* case_label: b_expr DOTDOT b_expr  */
#line 520 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3028 "pascal.tab.c"
    break;

  case 210: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 523 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3034 "pascal.tab.c"
    break;

  case 211: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 526 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3040 "pascal.tab.c"
    break;

  case 212: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 529 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3046 "pascal.tab.c"
    break;

  case 213: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 530 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3052 "pascal.tab.c"
    break;

  case 214: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 531 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3058 "pascal.tab.c"
    break;

  case 215: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 534 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3064 "pascal.tab.c"
    break;

  case 216: /* with_open: with_open COMMA selector  */
#line 537 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3070 "pascal.tab.c"
    break;

  case 217: /* with_open: selector  */
#line 538 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3076 "pascal.tab.c"
    break;

  case 218: /* expression: simple_expression  */
#line 545 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3082 "pascal.tab.c"
    break;

  case 219: /* expression: expression INOP simple_expression  */
#line 546 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3088 "pascal.tab.c"
    break;

  case 220: /* expression: expression LTOP simple_expression  */
#line 547 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3094 "pascal.tab.c"
    break;

  case 221: /* expression: expression LEOP simple_expression  */
#line 548 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3100 "pascal.tab.c"
    break;

  case 222: /* expression: expression GTOP simple_expression  */
#line 549 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3106 "pascal.tab.c"
    break;

  case 223: /* expression: expression GEOP simple_expression  */
#line 550 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3112 "pascal.tab.c"
    break;

  case 224: /* expression: expression NEOP simple_expression  */
#line 551 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3118 "pascal.tab.c"
    break;

  case 225: /* expression: expression EQOP simple_expression  */
#line 552 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3124 "pascal.tab.c"
    break;

  case 226: /* simple_expression: term  */
#line 555 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3130 "pascal.tab.c"
    break;

  case 227: /* simple_expression: PLUS term  */
#line 556 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3136 "pascal.tab.c"
    break;

  case 228: /* simple_expression: MINUS term  */
#line 557 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3142 "pascal.tab.c"
    break;

  case 229: /* simple_expression: simple_expression PLUS term  */
#line 558 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3148 "pascal.tab.c"
    break;

  case 230: /* simple_expression: simple_expression MINUS term  */
#line 559 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3154 "pascal.tab.c"
    break;

  case 231: /* simple_expression: simple_expression OROP term  */
#line 560 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3160 "pascal.tab.c"
    break;

  case 232: /* simple_expression: simple_expression XOROP term  */
#line 561 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3166 "pascal.tab.c"
    break;

  case 233: /* term: factor  */
#line 564 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3172 "pascal.tab.c"
    break;

  case 234: /* term: term MUL factor  */
#line 565 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3178 "pascal.tab.c"
    break;

  case 235: /* term: term RDIV factor  */
#line 566 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3184 "pascal.tab.c"
    break;

  case 236: /* term: term IDIV factor  */
#line 567 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3190 "pascal.tab.c"
    break;

  case 237: /* term: term IMOD factor  */
#line 568 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3196 "pascal.tab.c"
    break;

  case 238: /* term: term ANDOP factor  */
#line 569 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3202 "pascal.tab.c"
    break;

  case 239: /* term: term SHIFTOP factor  */
#line 570 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3208 "pascal.tab.c"
    break;

  case 240: /* factor: selector  */
#line 573 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3214 "pascal.tab.c"
    break;

  case 241: /* factor: call_with_args  */
#line 574 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3220 "pascal.tab.c"
    break;

  case 242: /* factor: INTCONST  */
#line 575 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3226 "pascal.tab.c"
    break;

  case 243: /* factor: REALCONST  */
#line 576 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3232 "pascal.tab.c"
    break;

  case 244: /* factor: STRINGCONST  */
#line 577 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 3238 "pascal.tab.c"
    break;

  case 245: /* factor: CHARCODE  */
#line 578 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3244 "pascal.tab.c"
    break;

  case 246: /* factor: LPARENT expression RPARENT  */
#line 579 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3250 "pascal.tab.c"
    break;

  case 247: /* factor: NOTSY factor  */
#line 580 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 3256 "pascal.tab.c"
    break;

  case 248: /* factor: ATSIGN factor  */
#line 581 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 3262 "pascal.tab.c"
    break;

  case 249: /* factor: LBRACK RBRACK  */
#line 582 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 3268 "pascal.tab.c"
    break;

  case 250: /* factor: LBRACK set_member_list RBRACK  */
#line 583 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 3274 "pascal.tab.c"
    break;

  case 251: /* set_member_list: set_member  */
#line 586 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3280 "pascal.tab.c"
    break;

  case 252: /* set_member_list: set_member_list COMMA set_member  */
#line 587 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3286 "pascal.tab.c"
    break;

  case 253: /* set_member: expression  */
#line 590 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3292 "pascal.tab.c"
    break;

  case 254: /* set_member: expression DOTDOT expression  */
#line 591 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3298 "pascal.tab.c"
    break;


#line 3302 "pascal.tab.c"

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

#line 597 "pascal.y"

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
