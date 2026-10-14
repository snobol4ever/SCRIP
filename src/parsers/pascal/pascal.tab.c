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
#line 31 "pascal.y"

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
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; int unit_mode; int dialect; PasDefE *defs; int ndefs; int capdefs; int cond_skip; int cond_depth; int cond_taken; int cond_unknown; int macro_on; int string_declared; const char *mode_macro; unsigned sw_set; unsigned sw_on; unsigned *sw_save; int sw_n; int sw_cap; int last_tok; const char *ident_p; int ident_n; } g_pascal_cenv;
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
void pascal_cenv_reset(void) { g_pascal_cenv.last_tok = 0; g_pascal_cenv.ident_p = NULL; g_pascal_cenv.ident_n = 0;g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; g_pascal_cenv.unit_mode = 0; g_pascal_cenv.dialect = PAS_DIALECT_ISO_DEFAULT; g_pascal_cenv.ndefs = 0; g_pascal_cenv.cond_skip = 0; g_pascal_cenv.cond_depth = 0; g_pascal_cenv.cond_taken = 0; g_pascal_cenv.cond_unknown = 0; g_pascal_cenv.macro_on = 0; g_pascal_cenv.string_declared = 0; g_pascal_cenv.mode_macro = NULL; g_pascal_cenv.sw_set = 0; g_pascal_cenv.sw_on = 0; g_pascal_cenv.sw_n = 0; }
int pascal_unit_mode(void) { return g_pascal_cenv.unit_mode; }
void pascal_unit_mode_set(int m) { g_pascal_cenv.unit_mode = m; }
int pascal_dialect(void) { return g_pascal_cenv.dialect; }
int pascal_last_tok(void) { return g_pascal_cenv.last_tok; }
void pascal_last_ident_set(const char *p, int n) { g_pascal_cenv.ident_p = p; g_pascal_cenv.ident_n = n; }
tree_t *pascal_last_ident_qlit(void) {
    tree_t *q = ast_node_new(TT_QLIT);
    char *s = ct_alloc((size_t)g_pascal_cenv.ident_n + 1);
    if (g_pascal_cenv.ident_p) memcpy(s, g_pascal_cenv.ident_p, (size_t)g_pascal_cenv.ident_n);
    s[g_pascal_cenv.ident_p ? g_pascal_cenv.ident_n : 0] = '\0';
    q->v.sval = s;
    return q;
}
int pascal_lex_wrapped(void) {
#undef pascal_yylex
    int t = pascal_yylex();
#define pascal_yylex pascal_lex_wrapped
    pas_stmt_line_fill();
    int lt = g_pascal_cenv.last_tok;
    if (t == IDENT && pascal_yylval.node && pascal_yylval.node->v.sval && (lt == IDENT || lt == INTCONST || lt == REALCONST || lt == STRINGCONST || lt == CHARCODE || lt == RPARENT || lt == RBRACK)) {
        const char *w = pascal_yylval.node->v.sval;
        if (!strcmp(w, "shl") || !strcmp(w, "shr")) { pascal_flip_dialect(PAS_DIALECT_FPC); t = SHIFTOP; }
        else if (!strcmp(w, "xor")) t = XOROP;
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

#line 221 "pascal.tab.c"

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
  YYSYMBOL_CLASSSY = 79,                   /* CLASSSY  */
  YYSYMBOL_ISOP = 80,                      /* ISOP  */
  YYSYMBOL_ASOP = 81,                      /* ASOP  */
  YYSYMBOL_TRYSY = 82,                     /* TRYSY  */
  YYSYMBOL_EXCEPTSY = 83,                  /* EXCEPTSY  */
  YYSYMBOL_FINALLYSY = 84,                 /* FINALLYSY  */
  YYSYMBOL_RAISESY = 85,                   /* RAISESY  */
  YYSYMBOL_INHERITEDSY = 86,               /* INHERITEDSY  */
  YYSYMBOL_CONSTRUCTORSY = 87,             /* CONSTRUCTORSY  */
  YYSYMBOL_DESTRUCTORSY = 88,              /* DESTRUCTORSY  */
  YYSYMBOL_PROPERTYSY = 89,                /* PROPERTYSY  */
  YYSYMBOL_ONSY = 90,                      /* ONSY  */
  YYSYMBOL_YYACCEPT = 91,                  /* $accept  */
  YYSYMBOL_compilation_unit = 92,          /* compilation_unit  */
  YYSYMBOL_program = 93,                   /* program  */
  YYSYMBOL_program_name = 94,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 95,          /* file_id_list_opt  */
  YYSYMBOL_block = 96,                     /* block  */
  YYSYMBOL_decl_part_list = 97,            /* decl_part_list  */
  YYSYMBOL_decl_part = 98,                 /* decl_part  */
  YYSYMBOL_unit = 99,                      /* unit  */
  YYSYMBOL_intf_part_list = 100,           /* intf_part_list  */
  YYSYMBOL_intf_part = 101,                /* intf_part  */
  YYSYMBOL_init_opt = 102,                 /* init_opt  */
  YYSYMBOL_fini_opt = 103,                 /* fini_opt  */
  YYSYMBOL_label_list = 104,               /* label_list  */
  YYSYMBOL_const_decl_list = 105,          /* const_decl_list  */
  YYSYMBOL_const_decl = 106,               /* const_decl  */
  YYSYMBOL_typed_value = 107,              /* typed_value  */
  YYSYMBOL_typed_value_list = 108,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 109,         /* field_value_list  */
  YYSYMBOL_constant = 110,                 /* constant  */
  YYSYMBOL_scalar_constant = 111,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 112,           /* type_decl_list  */
  YYSYMBOL_type_decl = 113,                /* type_decl  */
  YYSYMBOL_type = 114,                     /* type  */
  YYSYMBOL_115_1 = 115,                    /* $@1  */
  YYSYMBOL_116_2 = 116,                    /* $@2  */
  YYSYMBOL_117_3 = 117,                    /* $@3  */
  YYSYMBOL_class_members = 118,            /* class_members  */
  YYSYMBOL_class_member = 119,             /* class_member  */
  YYSYMBOL_prop_index_opt = 120,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 121,               /* prop_specs  */
  YYSYMBOL_packed_opt = 122,               /* packed_opt  */
  YYSYMBOL_simple_type = 123,              /* simple_type  */
  YYSYMBOL_lo_bound = 124,                 /* lo_bound  */
  YYSYMBOL_lo_term = 125,                  /* lo_term  */
  YYSYMBOL_b_expr = 126,                   /* b_expr  */
  YYSYMBOL_b_term = 127,                   /* b_term  */
  YYSYMBOL_b_factor = 128,                 /* b_factor  */
  YYSYMBOL_record_body = 129,              /* record_body  */
  YYSYMBOL_record_field_list = 130,        /* record_field_list  */
  YYSYMBOL_record_field = 131,             /* record_field  */
  YYSYMBOL_132_4 = 132,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 133,            /* case_arm_mark  */
  YYSYMBOL_case_open = 134,                /* case_open  */
  YYSYMBOL_record_case_opt = 135,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 136,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 137,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 138,            /* var_decl_list  */
  YYSYMBOL_var_decl = 139,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 140,           /* procedure_decl  */
  YYSYMBOL_141_5 = 141,                    /* $@5  */
  YYSYMBOL_142_6 = 142,                    /* $@6  */
  YYSYMBOL_143_7 = 143,                    /* $@7  */
  YYSYMBOL_144_8 = 144,                    /* $@8  */
  YYSYMBOL_145_9 = 145,                    /* $@9  */
  YYSYMBOL_146_10 = 146,                   /* $@10  */
  YYSYMBOL_147_11 = 147,                   /* $@11  */
  YYSYMBOL_148_12 = 148,                   /* $@12  */
  YYSYMBOL_149_13 = 149,                   /* $@13  */
  YYSYMBOL_pv_mark = 150,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 151,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 152,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 153,           /* parameter_decl  */
  YYSYMBOL_param_type = 154,               /* param_type  */
  YYSYMBOL_routine_directives = 155,       /* routine_directives  */
  YYSYMBOL_pf_params = 156,                /* pf_params  */
  YYSYMBOL_pf_sections = 157,              /* pf_sections  */
  YYSYMBOL_pf_section = 158,               /* pf_section  */
  YYSYMBOL_id_list = 159,                  /* id_list  */
  YYSYMBOL_body = 160,                     /* body  */
  YYSYMBOL_statement_list = 161,           /* statement_list  */
  YYSYMBOL_162_14 = 162,                   /* $@14  */
  YYSYMBOL_163_15 = 163,                   /* $@15  */
  YYSYMBOL_body_stmt = 164,                /* body_stmt  */
  YYSYMBOL_165_16 = 165,                   /* $@16  */
  YYSYMBOL_statement = 166,                /* statement  */
  YYSYMBOL_statement_no_label = 167,       /* statement_no_label  */
  YYSYMBOL_try_statement = 168,            /* try_statement  */
  YYSYMBOL_handler_list = 169,             /* handler_list  */
  YYSYMBOL_handler = 170,                  /* handler  */
  YYSYMBOL_raise_statement = 171,          /* raise_statement  */
  YYSYMBOL_inherited_call = 172,           /* inherited_call  */
  YYSYMBOL_call = 173,                     /* call  */
  YYSYMBOL_call_with_args = 174,           /* call_with_args  */
  YYSYMBOL_argument_list = 175,            /* argument_list  */
  YYSYMBOL_argument = 176,                 /* argument  */
  YYSYMBOL_assignment = 177,               /* assignment  */
  YYSYMBOL_selector = 178,                 /* selector  */
  YYSYMBOL_expression_list = 179,          /* expression_list  */
  YYSYMBOL_compound_statement = 180,       /* compound_statement  */
  YYSYMBOL_goto_statement = 181,           /* goto_statement  */
  YYSYMBOL_if_statement = 182,             /* if_statement  */
  YYSYMBOL_case_statement = 183,           /* case_statement  */
  YYSYMBOL_184_17 = 184,                   /* $@17  */
  YYSYMBOL_case_tail = 185,                /* case_tail  */
  YYSYMBOL_case_list = 186,                /* case_list  */
  YYSYMBOL_case_elem = 187,                /* case_elem  */
  YYSYMBOL_constant_list = 188,            /* constant_list  */
  YYSYMBOL_case_label = 189,               /* case_label  */
  YYSYMBOL_while_statement = 190,          /* while_statement  */
  YYSYMBOL_repeat_statement = 191,         /* repeat_statement  */
  YYSYMBOL_for_statement = 192,            /* for_statement  */
  YYSYMBOL_with_statement = 193,           /* with_statement  */
  YYSYMBOL_with_open = 194,                /* with_open  */
  YYSYMBOL_expression = 195,               /* expression  */
  YYSYMBOL_simple_expression = 196,        /* simple_expression  */
  YYSYMBOL_term = 197,                     /* term  */
  YYSYMBOL_power = 198,                    /* power  */
  YYSYMBOL_factor = 199,                   /* factor  */
  YYSYMBOL_set_member_list = 200,          /* set_member_list  */
  YYSYMBOL_set_member = 201                /* set_member  */
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
#define YYLAST   1013

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  322
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  727

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   345


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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   201,   201,   202,   205,   206,   209,   210,   213,   214,
     217,   220,   221,   224,   225,   226,   227,   228,   229,   232,
     235,   236,   239,   240,   241,   242,   243,   244,   247,   248,
     249,   252,   253,   256,   257,   258,   259,   262,   263,   266,
     267,   270,   271,   272,   273,   276,   277,   280,   281,   284,
     285,   286,   289,   290,   291,   292,   293,   296,   297,   300,
     301,   302,   305,   306,   307,   307,   307,   308,   309,   310,
     311,   312,   312,   313,   314,   315,   316,   317,   320,   321,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     336,   337,   340,   341,   342,   345,   346,   349,   350,   351,
     352,   355,   356,   357,   358,   359,   362,   363,   364,   365,
     366,   369,   370,   371,   372,   373,   376,   377,   378,   379,
     380,   383,   384,   387,   390,   391,   394,   394,   395,   398,
     401,   404,   405,   406,   409,   410,   413,   414,   417,   418,
     421,   422,   425,   426,   427,   427,   428,   428,   429,   429,
     430,   430,   431,   431,   434,   434,   437,   437,   440,   440,
     443,   443,   448,   451,   452,   453,   456,   457,   460,   461,
     462,   463,   464,   465,   466,   467,   470,   471,   474,   475,
     485,   486,   487,   490,   491,   494,   495,   496,   497,   498,
     501,   502,   505,   508,   508,   509,   509,   512,   512,   515,
     516,   517,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   537,   538,   539,
     540,   543,   544,   545,   548,   549,   552,   553,   556,   557,
     558,   561,   562,   563,   566,   567,   568,   571,   572,   575,
     576,   577,   580,   581,   582,   583,   584,   587,   588,   589,
     590,   591,   592,   595,   596,   599,   602,   603,   606,   607,
     610,   610,   613,   614,   615,   618,   619,   622,   623,   626,
     627,   630,   631,   634,   637,   640,   641,   642,   645,   648,
     649,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     667,   668,   669,   670,   671,   672,   673,   676,   677,   678,
     679,   680,   681,   682,   683,   686,   687,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   704,
     705,   708,   709
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
  "IDENT", "SHIFTOP", "XOROP", "POWOP", "CLASSSY", "ISOP", "ASOP", "TRYSY",
  "EXCEPTSY", "FINALLYSY", "RAISESY", "INHERITEDSY", "CONSTRUCTORSY",
  "DESTRUCTORSY", "PROPERTYSY", "ONSY", "$accept", "compilation_unit",
  "program", "program_name", "file_id_list_opt", "block", "decl_part_list",
  "decl_part", "unit", "intf_part_list", "intf_part", "init_opt",
  "fini_opt", "label_list", "const_decl_list", "const_decl", "typed_value",
  "typed_value_list", "field_value_list", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "$@1", "$@2", "$@3",
  "class_members", "class_member", "prop_index_opt", "prop_specs",
  "packed_opt", "simple_type", "lo_bound", "lo_term", "b_expr", "b_term",
  "b_factor", "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "pv_mark",
  "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "param_type", "routine_directives", "pf_params", "pf_sections",
  "pf_section", "id_list", "body", "statement_list", "$@14", "$@15",
  "body_stmt", "$@16", "statement", "statement_no_label", "try_statement",
  "handler_list", "handler", "raise_statement", "inherited_call", "call",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@17", "case_tail", "case_list",
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

#define YYPACT_NINF (-451)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-253)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     178,    12,    70,   162,  -451,   143,   265,  -451,  -451,   134,
     189,  -451,  -451,    -1,   211,   221,   274,   221,   280,  -451,
     298,    83,   322,   343,  -451,  -451,  -451,   364,   221,   213,
     240,  -451,  -451,    35,   109,   211,  -451,  -451,    64,   250,
     221,  -451,   183,   264,    59,   384,   383,   298,  -451,   377,
     378,   409,   410,  -451,   197,  -451,  -451,  -451,   246,   714,
     580,  -451,  -451,   382,   396,   419,  -451,   580,   401,    30,
    -451,  -451,   252,   414,  -451,   714,   417,   714,  -451,   714,
     426,   875,  -451,   714,   420,  -451,  -451,  -451,  -451,  -451,
    -451,   301,  -451,   916,  -451,  -451,  -451,  -451,  -451,  -451,
    -451,  -451,   609,  -451,   438,   441,   427,   433,  -451,   463,
     554,  -451,  -451,   772,   772,   772,   645,   714,   772,  -451,
    -451,  -451,  -451,   470,   420,  -451,   301,   361,    47,   329,
     177,   451,  -451,   473,  -451,   473,   546,   546,   455,   221,
    -451,  -451,  -451,  -451,   552,   477,  -451,   491,   258,  -451,
     245,   135,  -451,  -451,   151,   543,    31,  -451,  -451,   493,
     384,  -451,  -451,   175,   141,   157,   301,   361,    74,   598,
      81,   644,   412,   412,   672,    22,   872,   492,   479,  -451,
     714,   481,  -451,   714,   714,   714,   714,   714,    50,   555,
     486,   488,  -451,  -451,  -451,   211,   221,  -451,   494,   221,
     497,   298,  -451,  -451,   177,   177,  -451,   812,   182,  -451,
     824,  -451,   498,  -451,   714,   714,   714,   714,   714,   714,
     714,   714,   772,   772,   772,   772,   772,   772,   772,   772,
     772,   772,   772,   772,   316,  -451,   507,  -451,   135,   135,
    -451,   206,   795,   221,   512,   730,    20,   540,   548,  -451,
     266,   266,   925,   266,   266,   266,   266,   419,   221,   504,
     221,   506,  -451,   221,    36,  -451,   335,   562,  -451,   730,
     419,  -451,   510,  -451,   714,   714,   714,  -451,  -451,   417,
    -451,  -451,  -451,   888,  -451,  -451,  -451,   286,  -451,   846,
     500,  -451,   688,  -451,   872,   903,   233,   872,   872,   872,
     872,   872,  -451,   221,  -451,  -451,  -451,   419,   419,   211,
      80,   496,  -451,   221,  -451,   298,   714,   714,  -451,  -451,
     525,   329,   329,   329,   329,   329,   329,   329,   329,   177,
     177,   177,   177,   451,   451,   451,   451,   451,   451,   451,
    -451,   221,   518,   221,   521,  -451,    46,  -451,   368,   522,
    -451,   546,   546,   541,  -451,   290,   530,  -451,   533,    63,
     123,   550,   553,   556,  -451,   399,   756,   622,   872,   580,
     938,   580,   938,   221,   925,  -451,   159,  -451,   159,   266,
     266,   348,   159,  -451,  -451,  -451,  -451,   627,   415,   473,
     418,   473,   429,    41,  -451,    27,   632,   566,   637,   597,
     653,   655,   284,   393,   872,  -451,   384,  -451,   361,   925,
     621,   714,  -451,   714,   589,   137,   136,  -451,   158,  -451,
     299,   714,   714,  -451,   310,   419,   419,   660,   661,  -451,
    -451,  -451,   652,   419,   419,   872,  -451,   450,   473,   460,
     473,   161,  -451,    27,  -451,  -451,  -451,  -451,  -451,  -451,
    -451,  -451,   593,   595,  -451,  -451,   610,   580,   171,   616,
      48,   824,  -451,  -451,   247,  -451,  -451,   646,   170,  -451,
     466,   360,   266,   266,   266,   266,   159,   159,   266,   266,
    -451,    27,  -451,    27,   615,    27,  -451,   647,  -451,   634,
    -451,   674,  -451,  -451,   614,  -451,   681,   714,   714,  -451,
    -451,   292,    85,  -451,   467,  -451,  -451,  -451,   859,    58,
    -451,   500,  -451,  -451,  -451,  -451,   315,   872,   692,   695,
     642,  -451,  -451,   697,   697,  -451,   673,   699,   651,    27,
    -451,    27,   654,  -451,  -451,   523,   419,   419,  -451,  -451,
     419,   419,    41,   656,   700,   730,   730,   185,  -451,   938,
     684,  -451,   221,   648,  -451,  -451,  -451,  -451,  -451,  -451,
    -451,   159,   159,   566,   669,  -451,   657,  -451,   659,   714,
    -451,   717,   722,   723,   566,   413,   464,  -451,   925,   925,
    -451,  -451,   707,   925,  -451,  -451,   714,  -451,   665,  -451,
     180,  -451,  -451,  -451,   675,   566,   566,   697,   671,  -451,
     678,  -451,  -451,   680,  -451,   734,   689,   419,   419,   752,
     759,    52,   690,  -451,  -451,  -451,   324,  -451,   710,   704,
    -451,  -451,    49,   580,  -451,   714,  -451,  -451,   872,  -451,
    -451,  -451,  -451,  -451,  -451,   348,  -451,   697,   697,  -451,
    -451,  -451,   872,  -451,   753,  -451,   566,   766,  -451,  -451,
    -451,   768,  -451,  -451,   701,   773,   720,  -451,  -451,  -451,
    -451,   730,  -451,   730,   754,  -451,  -451,   706,  -451,   785,
     872,   566,   787,  -451,  -451,  -451,  -451,  -451,   789,   790,
    -451,   792,  -451,   724,    29,  -451,  -451,   580,   580,   925,
     778,  -451,  -451,  -451,  -451,   802,   566,  -451,  -451,  -451,
     803,  -451,   314,  -451,  -451,   806,  -451,   482,  -451,   807,
    -451,  -451,  -451,  -451,  -451,   925,   750,   925,  -451,   810,
    -451,  -451,   806,  -451,   221,   757,  -451
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,   195,
       0,     0,     0,     0,    11,    17,    10,     0,     0,     0,
       0,    35,    36,     0,     0,    14,    38,   191,     0,   162,
      16,   139,     0,   162,     0,   216,     0,    15,    58,     0,
       0,     0,     0,     7,     0,    12,    21,    13,     0,     0,
      96,    37,    18,     0,     0,   165,   138,    96,     0,   165,
     193,   192,     0,     0,   195,     0,     0,     0,   195,     0,
       0,   231,   195,   226,   215,   196,   199,   212,   213,   214,
     203,   233,   202,     0,   204,   205,   206,   207,   208,   209,
     210,   211,    96,    57,     0,     0,     0,     0,     8,     0,
       0,    33,    34,     0,     0,     0,     0,     0,     0,   309,
     312,   310,   311,   252,     0,   314,   308,   307,     0,   281,
     290,   297,   305,   182,    95,   182,     0,     0,     0,     0,
      53,    56,    54,    55,    52,    78,   106,     0,     0,    62,
       0,   101,   190,   162,     0,     0,     0,   162,   148,     0,
     216,   256,   257,     0,     0,     0,     0,   280,     0,     0,
       0,     0,   216,   216,     0,     0,   227,   228,     0,   251,
       0,     0,   249,     0,     0,     0,     0,     0,    78,     0,
       0,     0,   162,   162,     4,     0,     0,    12,     0,     0,
       0,     0,    20,   315,   291,   292,   317,   321,     0,   319,
       0,   316,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    52,   102,   103,
      63,     0,     0,     0,     0,     0,     0,    74,     0,    71,
       0,     0,     0,     0,     0,     0,     0,   165,     0,     0,
       0,     0,   164,   191,     0,   167,     0,   178,   140,     0,
     165,    12,     0,   194,     0,     0,     0,   197,   197,     0,
     260,   255,   197,   231,   200,   201,   235,     0,   238,   239,
     195,   195,     0,   250,   242,   232,     0,   254,   243,   244,
     245,   246,    60,     0,    59,   162,   162,   165,   165,    23,
       0,    30,   162,    25,   162,    24,     0,     0,   318,   313,
     248,   282,   283,   284,   285,   286,   287,   288,   289,   295,
     293,   294,   296,   300,   301,   302,   298,   299,   303,   304,
     306,     0,     0,     0,     0,   181,     0,   184,     0,     0,
      97,     0,     0,     0,    49,     0,     0,    76,     0,    80,
       0,     0,     0,     0,    79,     0,     0,     0,    41,    96,
       0,    96,     0,   128,     0,   121,   104,   116,   105,     0,
       0,   100,   111,   108,   109,   107,   110,     0,     0,   182,
       0,   182,     0,     0,   163,     0,     0,   144,     0,     0,
       0,     0,     0,     0,   274,   273,   216,   278,   279,   268,
     258,     0,   234,     0,     0,     0,     0,   221,     0,   229,
       0,     0,     0,   247,     0,   165,   165,     0,     0,    22,
     195,   195,    32,   165,   165,   322,   320,     0,   182,     0,
     182,     0,   180,     0,    69,    50,    51,    99,    78,   162,
     162,    81,     0,     0,   162,   162,    90,    96,   252,     0,
       0,    41,    40,    67,     0,    75,    73,     0,   133,   125,
       0,     0,     0,     0,     0,     0,   112,   113,     0,     0,
     178,     0,   168,     0,     0,     0,   166,     0,   176,   171,
     142,     0,    12,   141,     0,   149,   178,     0,     0,   197,
     198,   271,   262,   266,     0,   270,   197,   237,   240,     0,
     217,   223,   218,   195,   220,   230,     0,   253,    78,     0,
       0,   178,   178,    28,    29,   195,     0,     0,     0,     0,
     188,     0,     0,   183,   185,     0,   165,   165,   162,   162,
     165,   165,     0,     0,     0,     0,     0,     0,    43,     0,
       0,    72,   128,     0,   123,   126,   122,   118,   119,   117,
     120,   114,   115,   150,   172,   170,     0,   175,     0,     0,
     179,     0,     0,     0,   146,     0,     0,   277,     0,   268,
     195,   195,     0,     0,   197,   259,     0,   197,     0,   222,
       0,   236,    61,   178,     0,   154,   156,    31,     0,    26,
       0,   187,   186,     0,    77,     0,     0,   165,   165,     0,
       0,     0,     0,    82,    47,    45,     0,    44,     0,     0,
      64,   124,     0,    96,    12,     0,   169,   177,   173,   145,
     178,   143,    12,   197,   197,   272,   265,   264,   263,   261,
     269,   267,   241,   225,     0,   219,   158,     0,    12,    12,
      19,     0,   189,    83,     0,     0,     0,    85,    86,    91,
      92,     0,    42,     0,     0,    65,   130,     0,   127,     0,
     174,   152,     0,   276,   275,   197,    12,   178,     0,     0,
      27,     0,    87,     0,     0,    46,    48,    96,    96,   137,
       0,   151,    12,   147,   224,     0,   160,   155,   157,    84,
       0,    89,     0,    70,    66,   132,   135,     0,   130,     0,
     159,    12,    88,    94,    93,   137,     0,   137,   153,     0,
     134,   129,   131,   161,   128,     0,   136
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -451,  -451,  -451,  -451,  -451,   -54,   623,  -451,  -451,  -451,
    -451,  -451,  -451,  -451,   628,   -28,  -263,  -451,  -451,  -451,
     -58,   617,   -30,   -64,  -451,  -451,  -451,  -411,  -451,  -451,
    -451,  -451,  -350,  -451,   313,  -248,  -232,  -195,   100,  -451,
     273,  -451,  -451,   118,  -451,   115,   119,   638,   -29,  -451,
    -451,  -451,  -451,  -451,  -451,  -451,  -451,  -451,  -451,   -38,
     -57,   294,   446,  -401,  -450,  -120,  -451,   400,   -15,  -451,
     -35,  -451,  -451,  -268,  -451,  -144,   256,  -451,  -451,   339,
    -451,   -16,  -451,   -22,  -261,   440,  -451,   -20,  -451,  -451,
    -451,  -451,  -451,  -451,  -451,  -451,   261,  -388,   269,  -451,
    -451,  -451,  -451,  -451,   -51,   664,   -52,   776,   -68,  -451,
     537
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    29,     5,     6,    24,     7,   110,
     202,   432,   526,    33,    35,    36,   367,   616,   460,   353,
     375,    47,    48,   147,   665,   688,   373,   244,   364,   543,
     684,   148,   149,   150,   151,   501,   382,   377,   467,   468,
     469,   623,   724,   689,   554,   705,   706,    40,    41,    25,
     492,   632,   271,   624,   692,   648,   649,   676,   711,    65,
     155,   264,   265,   489,   397,   235,   346,   347,    42,    26,
      44,   160,    45,   405,   406,    85,    86,    87,   416,   417,
      88,   125,    90,   126,   287,   288,    92,   127,   296,    94,
      95,    96,    97,   409,   582,   502,   503,   707,   505,    98,
      99,   100,   101,   168,   368,   129,   130,   131,   132,   208,
     209
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,   109,   146,   156,   381,    69,   398,    61,   128,   146,
     407,    66,   159,    54,   410,   236,   273,   103,   376,   378,
     464,   504,   466,    91,   165,    93,   169,    70,   171,    89,
     563,   420,   176,   487,   701,   158,   268,   535,   189,   164,
      57,   393,   534,   170,   146,   203,   574,   175,   369,   258,
     211,   441,   213,   547,   166,   302,   167,   393,   383,   384,
     385,   386,   204,   205,    70,   207,   210,   259,   451,    62,
      31,   595,   596,   260,    32,   587,   261,   666,   146,   146,
     564,   370,   565,   269,   567,   429,    70,     8,   214,    71,
     579,   278,    58,   154,   215,   216,   217,   218,   219,   220,
     394,   580,   488,   459,   702,   290,   291,   535,   667,    49,
     442,   281,   548,   303,   659,   257,   263,   588,    50,   270,
    -191,    63,  -191,   289,   241,   581,   471,   221,   601,   294,
     602,   279,   297,   298,   299,   300,   301,    63,    91,   266,
      93,   511,    70,   646,    89,    10,    70,   476,   477,   452,
      91,    91,    93,    93,   307,   308,    89,    89,   453,   258,
     516,    59,    11,    70,   276,   340,   512,   510,    60,   341,
     329,   330,   331,   332,   277,   552,   513,   259,   253,   254,
     671,   310,     1,   260,   354,    70,   261,   342,   514,     2,
     255,   504,    27,   343,    30,   553,   344,    28,   214,   619,
     387,    12,   472,   473,   215,   216,   217,   218,   219,   220,
     645,   256,   274,   399,   474,   262,   275,   400,    55,   348,
     226,   227,   228,   402,   403,   404,   263,   696,   355,   365,
     545,   577,   229,   230,   174,   475,    37,   221,   585,   317,
      63,   289,    67,   388,   318,   390,   561,   562,   392,   617,
     427,   428,    56,   231,    63,   415,   418,   166,   232,   408,
     618,   108,   500,    63,   246,   435,   207,   425,   426,   482,
     350,   484,    13,    14,   433,    15,   434,   557,   558,   559,
     560,    61,   614,   615,    66,   103,    34,   247,   424,   248,
     422,    16,   249,   445,   446,   423,    37,    17,   250,   251,
      18,    19,   497,    20,   549,   463,   498,   465,    64,   550,
     252,   146,   146,   146,   146,   461,   641,   111,   530,   643,
     532,   112,    68,   161,   341,   214,   437,   162,   439,   374,
     635,   215,   216,   217,   218,   219,   220,   140,   141,   142,
     143,   237,   342,   411,    21,   478,   479,    63,   343,    39,
     412,   344,    22,    23,   448,    43,   411,   578,   470,   178,
     289,   179,   508,   515,   221,   673,   674,    63,   519,   520,
     289,   517,   411,    46,   518,   222,   527,   528,   266,   591,
     345,   661,   223,   224,    91,   713,    93,    72,   662,   714,
      89,    37,    63,   544,   395,   523,   524,    51,   685,   146,
     686,   478,   479,    73,    74,    75,   225,   694,    76,    77,
     499,   536,   537,   478,   479,    72,   540,   541,    52,   212,
      78,   182,   183,    79,   556,    63,   348,   443,   284,   285,
     633,    73,    74,    75,   214,   102,    76,    77,   571,    53,
     215,   216,   217,   218,   219,   220,   575,   576,    78,   238,
     239,    79,   104,   105,   214,    80,    63,   152,   457,    81,
     215,   216,   217,   218,   219,   220,    82,   106,   107,    83,
      84,   153,    63,   221,   481,    63,   157,   483,   590,   605,
     606,   634,   154,   609,   610,   172,    63,   283,   485,   163,
     597,   146,   123,   221,    82,   177,   190,    83,    84,   191,
     607,   608,   192,    13,    14,   214,    15,    63,   193,   529,
     430,   215,   216,   217,   218,   219,   220,    63,   628,   531,
     365,   194,    16,    63,   583,   555,   584,   266,    17,   233,
     240,    18,   431,   174,    20,   642,   234,   470,   356,   583,
     243,   716,   357,   245,   221,   637,   638,   358,   267,   356,
     655,   656,   272,   604,   293,   292,   295,   -98,   358,   668,
     304,   305,   195,   306,   196,   146,   349,   197,   371,   312,
     669,   396,   314,   320,   670,    21,   372,   -98,   672,   389,
     198,   391,   -98,    22,    23,   401,   199,   359,   421,   200,
     414,   360,   201,   438,   678,   679,   440,   444,   359,   361,
     362,   363,   360,   447,   -98,   449,   133,   134,   450,   -98,
     361,   362,   363,   242,   -98,   135,   -98,   140,   141,   142,
     143,   237,   695,   703,   704,   454,   280,   462,   455,   146,
     146,   456,   480,   136,   137,   133,   134,   490,   709,   214,
     138,   491,   493,   139,   135,   215,   216,   217,   218,   219,
     220,   140,   141,   142,   143,   144,   494,   719,   495,   145,
     496,   506,   136,   137,   509,   521,   522,   525,   538,   138,
     539,   542,   139,   546,   566,   568,   551,   282,   221,   570,
     140,   141,   142,   143,   144,   214,   569,   113,   188,   572,
     573,   215,   216,   217,   218,   219,   220,   592,   114,   115,
     593,   594,    70,   598,   599,   613,   116,   206,   117,   470,
     600,   118,   620,   603,   113,   612,   119,   120,   121,   122,
     123,   625,   629,   622,   221,   114,   115,   630,   631,   650,
     113,   124,   626,   116,   627,   117,   286,   639,   118,   653,
     644,   114,   115,   119,   120,   121,   122,   123,   654,   116,
     647,   117,   419,   651,   118,   652,   113,   657,   124,   119,
     120,   121,   122,   123,   658,   660,   664,   114,   115,   663,
     675,   677,   113,   680,   124,   116,   681,   117,   682,   683,
     118,   690,   687,   114,   115,   119,   120,   121,   122,   123,
     691,   116,   693,   366,   697,   698,   118,   699,   113,   700,
     124,   119,   120,   121,   122,   123,   708,   710,   712,   114,
     115,   715,   718,   721,   113,   723,   124,   116,   315,   366,
     311,   726,   118,   309,   725,   621,   717,   119,   120,   121,
     122,   458,   722,   116,   720,   117,   611,   313,   118,   486,
     636,   533,   124,   119,   120,   121,   122,   123,   351,   352,
     589,   507,   640,   214,   436,     0,     0,     0,   124,   215,
     216,   217,   218,   219,   220,   214,   140,   141,   142,   143,
     237,   215,   216,   217,   218,   219,   220,   316,   321,   322,
     323,   324,   325,   326,   327,   328,     0,   214,   319,     0,
       0,     0,   221,   215,   216,   217,   218,   219,   220,     0,
     214,     0,     0,     0,   221,   413,   215,   216,   217,   218,
     219,   220,  -252,   214,     0,     0,     0,     0,   586,   215,
     216,   217,   218,   219,   220,  -252,   221,     0,     0,     0,
       0,     0,     0,  -252,   173,  -252,  -252,     0,   174,   221,
    -248,     0,  -252,  -252,  -252,  -252,  -252,     0,  -252,  -252,
       0,   174,   221,   180,     0,  -252,  -252,  -252,  -252,     0,
       0,  -248,     0,  -248,  -248,     0,   421,     0,     0,     0,
    -248,  -248,  -248,  -248,   181,     0,   182,   183,   379,   380,
       0,     0,     0,   184,   185,   186,   187,     0,   374,     0,
       0,   136,   137,     0,     0,     0,   140,   141,   142,   143,
     237,   139,   333,   334,   335,   336,   337,   338,   339,   140,
     141,   142,   143,   144
};

static const yytype_int16 yycheck[] =
{
      15,    55,    60,    67,   252,    43,   269,    35,    59,    67,
     278,    40,    69,    28,   282,   135,   160,    47,   250,   251,
     370,   409,   372,    45,    75,    45,    77,     5,    79,    45,
     480,   292,    83,     6,     5,     5,     5,   448,   102,    74,
       5,     5,   443,    78,   102,   113,   496,    82,    28,     8,
     118,     5,     5,     5,    76,     5,    76,     5,   253,   254,
     255,   256,   114,   115,     5,   116,   117,    26,     5,     5,
      71,   521,   522,    32,    75,    17,    35,    28,   136,   137,
     481,    61,   483,    52,   485,     5,     5,    75,    41,    30,
       5,    17,    57,    63,    47,    48,    49,    50,    51,    52,
      64,    16,    75,   366,    75,    83,    84,   518,    59,    26,
      64,    30,    64,    63,    62,   153,    75,    59,    35,   157,
      57,    57,    59,   174,   139,    40,   374,    80,   529,   180,
     531,    57,   183,   184,   185,   186,   187,    57,   160,   154,
     160,     5,     5,   593,   160,    75,     5,   379,   380,    26,
     172,   173,   172,   173,   192,   193,   172,   173,    35,     8,
     421,    52,     0,     5,    23,   233,    30,    30,    59,     8,
     222,   223,   224,   225,    17,     5,    40,    26,    43,    44,
     630,   196,     4,    32,   242,     5,    35,    26,    30,    11,
      55,   579,    58,    32,     5,    25,    35,    63,    41,   549,
     257,    58,    43,    44,    47,    48,    49,    50,    51,    52,
      30,    76,    37,   270,    55,    64,    41,   271,     5,   234,
      43,    44,    45,   274,   275,   276,    75,   677,   243,   244,
      59,   499,    55,    56,    63,    76,    75,    80,   506,    57,
      57,   292,    59,   258,    62,   260,   478,   479,   263,    64,
     307,   308,    12,    76,    57,   290,   291,   279,    81,   279,
      75,    64,   406,    57,     6,   316,   317,   305,   306,   389,
      64,   391,     7,     8,   312,    10,   314,   472,   473,   474,
     475,   309,   545,   546,   313,   315,    75,    29,   303,    31,
      57,    26,    34,   351,   352,    62,    75,    32,    53,    54,
      35,    36,    18,    38,    57,   369,    22,   371,    58,    62,
      65,   369,   370,   371,   372,   366,   584,    71,   438,   587,
     440,    75,    58,    71,     8,    41,   341,    75,   343,    63,
     578,    47,    48,    49,    50,    51,    52,    71,    72,    73,
      74,    75,    26,    57,    79,    53,    54,    57,    32,    75,
      64,    35,    87,    88,    64,    75,    57,    65,   373,    58,
     411,    60,   413,    64,    80,   633,   634,    57,   425,   426,
     421,   422,    57,    75,    64,    46,   433,   434,   393,    64,
      64,    57,    53,    54,   406,    71,   406,     3,    64,    75,
     406,    75,    57,   457,    59,   430,   431,    75,   661,   457,
     663,    53,    54,    19,    20,    21,    77,   675,    24,    25,
      17,   449,   450,    53,    54,     3,   454,   455,    75,    58,
      36,    60,    61,    39,    64,    57,   441,    59,   172,   173,
      17,    19,    20,    21,    41,    52,    24,    25,   492,    75,
      47,    48,    49,    50,    51,    52,   497,   498,    36,   136,
     137,    39,    75,    75,    41,    71,    57,    75,    59,    75,
      47,    48,    49,    50,    51,    52,    82,    58,    58,    85,
      86,    75,    57,    80,    59,    57,    75,    59,   513,   536,
     537,    17,    63,   540,   541,    59,    57,    75,    59,    75,
     525,   549,    75,    80,    82,    75,    58,    85,    86,    58,
     538,   539,    75,     7,     8,    41,    10,    57,    75,    59,
      14,    47,    48,    49,    50,    51,    52,    57,   569,    59,
     535,    58,    26,    57,    57,    59,    59,   542,    32,    78,
      75,    35,    36,    63,    38,   586,    63,   552,    26,    57,
      63,    59,    30,    52,    80,   580,   581,    35,     5,    26,
     607,   608,    59,    30,    75,    63,    75,     5,    35,   623,
       5,    75,     8,    75,    10,   623,    59,    13,    28,    75,
     624,     9,    75,    75,   625,    79,    28,    25,   632,    75,
      26,    75,    30,    87,    88,    75,    32,    75,    63,    35,
      90,    79,    38,    75,   648,   649,    75,    75,    75,    87,
      88,    89,    79,    62,    52,    75,    26,    27,    75,    57,
      87,    88,    89,    61,    62,    35,    64,    71,    72,    73,
      74,    75,   676,   687,   688,    75,    28,     5,    75,   687,
     688,    75,     5,    53,    54,    26,    27,     5,   692,    41,
      60,    75,     5,    63,    35,    47,    48,    49,    50,    51,
      52,    71,    72,    73,    74,    75,    59,   711,     5,    79,
       5,    40,    53,    54,    75,     5,     5,    15,    75,    60,
      75,    61,    63,    57,    59,    28,    30,    33,    80,     5,
      71,    72,    73,    74,    75,    41,    52,    42,    79,    75,
       9,    47,    48,    49,    50,    51,    52,     5,    53,    54,
       5,    59,     5,    30,     5,     5,    61,    62,    63,   724,
      59,    66,    28,    59,    42,    59,    71,    72,    73,    74,
      75,    52,     5,    75,    80,    53,    54,     5,     5,    58,
      42,    86,    75,    61,    75,    63,    64,    30,    66,     5,
      75,    53,    54,    71,    72,    73,    74,    75,    59,    61,
      75,    63,    64,    75,    66,    75,    42,     5,    86,    71,
      72,    73,    74,    75,     5,    75,    62,    53,    54,    59,
      17,     5,    42,     5,    86,    61,    75,    63,     5,    59,
      66,    75,    28,    53,    54,    71,    72,    73,    74,    75,
       5,    61,     5,    63,     5,     5,    66,     5,    42,    75,
      86,    71,    72,    73,    74,    75,    28,     5,     5,    53,
      54,     5,     5,    63,    42,     5,    86,    61,   201,    63,
     197,    64,    66,   195,   724,   552,   708,    71,    72,    73,
      74,    75,   717,    61,   715,    63,   542,   199,    66,   393,
     579,   441,    86,    71,    72,    73,    74,    75,    53,    54,
     511,   411,   583,    41,   317,    -1,    -1,    -1,    86,    47,
      48,    49,    50,    51,    52,    41,    71,    72,    73,    74,
      75,    47,    48,    49,    50,    51,    52,    65,   214,   215,
     216,   217,   218,   219,   220,   221,    -1,    41,    64,    -1,
      -1,    -1,    80,    47,    48,    49,    50,    51,    52,    -1,
      41,    -1,    -1,    -1,    80,    59,    47,    48,    49,    50,
      51,    52,    37,    41,    -1,    -1,    -1,    -1,    59,    47,
      48,    49,    50,    51,    52,    37,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    -1,    63,    80,
      37,    -1,    67,    68,    69,    70,    58,    -1,    60,    61,
      -1,    63,    80,    37,    -1,    67,    68,    69,    70,    -1,
      -1,    58,    -1,    60,    61,    -1,    63,    -1,    -1,    -1,
      67,    68,    69,    70,    58,    -1,    60,    61,    53,    54,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    63,    -1,
      -1,    53,    54,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    63,   226,   227,   228,   229,   230,   231,   232,    71,
      72,    73,    74,    75
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    92,    93,    96,    97,    99,    75,    94,
      75,     0,    58,     7,     8,    10,    26,    32,    35,    36,
      38,    79,    87,    88,    98,   140,   160,    58,    63,    95,
       5,    71,    75,   104,    75,   105,   106,    75,   159,    75,
     138,   139,   159,    75,   161,   163,    75,   112,   113,    26,
      35,    75,    75,    75,   159,     5,    12,     5,    57,    52,
      59,   106,     5,    57,    58,   150,   139,    59,    58,   150,
       5,    30,     3,    19,    20,    21,    24,    25,    36,    39,
      71,    75,    82,    85,    86,   166,   167,   168,   171,   172,
     173,   174,   177,   178,   180,   181,   182,   183,   190,   191,
     192,   193,    52,   113,    75,    75,    58,    58,    64,    96,
     100,    71,    75,    42,    53,    54,    61,    63,    66,    71,
      72,    73,    74,    75,    86,   172,   174,   178,   195,   196,
     197,   198,   199,    26,    27,    35,    53,    54,    60,    63,
      71,    72,    73,    74,    75,    79,   111,   114,   122,   123,
     124,   125,    75,    75,    63,   151,   114,    75,     5,   151,
     162,    71,    75,    75,   161,   195,   174,   178,   194,   195,
     161,   195,    59,    59,    63,   161,   195,    75,    58,    60,
      37,    58,    60,    61,    67,    68,    69,    70,    79,   114,
      58,    58,    75,    75,    58,     8,    10,    13,    26,    32,
      35,    38,   101,   199,   197,   197,    62,   195,   200,   201,
     195,   199,    58,     5,    41,    47,    48,    49,    50,    51,
      52,    80,    46,    53,    54,    77,    43,    44,    45,    55,
      56,    76,    81,    78,    63,   156,   156,    75,   125,   125,
      75,   159,    61,    63,   118,    52,     6,    29,    31,    34,
      53,    54,    65,    43,    44,    55,    76,   150,     8,    26,
      32,    35,    64,    75,   152,   153,   159,     5,     5,    52,
     150,   143,    59,   166,    37,    41,    23,    17,    17,    57,
      28,    30,    33,    75,   167,   167,    64,   175,   176,   195,
      83,    84,    63,    75,   195,    75,   179,   195,   195,   195,
     195,   195,     5,    63,     5,    75,    75,   150,   150,   105,
     159,    97,    75,   138,    75,   112,    65,    57,    62,    64,
      75,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     197,   197,   197,   198,   198,   198,   198,   198,   198,   198,
     199,     8,    26,    32,    35,    64,   157,   158,   159,    59,
      64,    53,    54,   110,   111,   159,    26,    30,    35,    75,
      79,    87,    88,    89,   119,   159,    63,   107,   195,    28,
      61,    28,    28,   117,    63,   111,   127,   128,   127,    53,
      54,   126,   127,   128,   128,   128,   128,   151,   159,    75,
     159,    75,   159,     5,    64,    59,     9,   155,   107,   151,
      96,    75,   195,   195,   195,   164,   165,   164,   178,   184,
     164,    57,    64,    59,    90,   161,   169,   170,   161,    64,
     175,    63,    57,    62,   159,   150,   150,   151,   151,     5,
      14,    36,   102,   150,   150,   195,   201,   159,    75,   159,
      75,     5,    64,    59,    75,   111,   111,    62,    64,    75,
      75,     5,    26,    35,    75,    75,    75,    59,    75,   107,
     109,   195,     5,   114,   123,   114,   123,   129,   130,   131,
     159,   126,    43,    44,    55,    76,   127,   127,    53,    54,
       5,    59,   156,    59,   156,    59,   153,     6,    75,   154,
       5,    75,   141,     5,    59,     5,     5,    18,    22,    17,
     166,   126,   186,   187,   188,   189,    40,   176,   195,    75,
      30,     5,    30,    40,    30,    64,   175,   195,    64,   151,
     151,     5,     5,   161,   161,    15,   103,   151,   151,    59,
     156,    59,   156,   158,   154,   118,   150,   150,    75,    75,
     150,   150,    61,   120,   114,    59,    57,     5,    64,    57,
      62,    30,     5,    25,   135,    59,    64,   128,   128,   128,
     128,   127,   127,   155,   154,   154,    59,   154,    28,    52,
       5,    96,    75,     9,   155,   195,   195,   164,    65,     5,
      16,    40,   185,    57,    59,   164,    59,    17,    59,   170,
     161,    64,     5,     5,    59,   155,   155,   161,    30,     5,
      59,   154,   154,    59,    30,   151,   151,   150,   150,   151,
     151,   152,    59,     5,   107,   107,   108,    64,    75,   123,
      28,   131,    75,   132,   144,    52,    75,    75,   195,     5,
       5,     5,   142,    17,    17,   126,   187,   161,   161,    30,
     189,   164,   195,   164,    75,    30,   155,    75,   146,   147,
      58,    75,    75,     5,    59,   151,   151,     5,     5,    62,
      75,    57,    64,    59,    62,   115,    28,    59,   114,    96,
     195,   155,    96,   164,   164,    17,   148,     5,    96,    96,
       5,    75,     5,    59,   121,   107,   107,    28,   116,   134,
      75,     5,   145,     5,   164,    96,   155,     5,     5,     5,
      75,     5,    75,   114,   114,   136,   137,   188,    28,    96,
       5,   149,     5,    71,    75,     5,    59,   134,     5,    96,
     137,    63,   136,     5,   133,   129,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      96,    97,    97,    98,    98,    98,    98,    98,    98,    99,
     100,   100,   101,   101,   101,   101,   101,   101,   102,   102,
     102,   103,   103,   104,   104,   104,   104,   105,   105,   106,
     106,   107,   107,   107,   107,   108,   108,   109,   109,   110,
     110,   110,   111,   111,   111,   111,   111,   112,   112,   113,
     113,   113,   114,   114,   115,   116,   114,   114,   114,   114,
     114,   117,   114,   114,   114,   114,   114,   114,   118,   118,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     120,   120,   121,   121,   121,   122,   122,   123,   123,   123,
     123,   124,   124,   124,   124,   124,   125,   125,   125,   125,
     125,   126,   126,   126,   126,   126,   127,   127,   127,   127,
     127,   128,   128,   129,   130,   130,   132,   131,   131,   133,
     134,   135,   135,   135,   136,   136,   137,   137,   138,   138,
     139,   139,   140,   140,   141,   140,   142,   140,   143,   140,
     144,   140,   145,   140,   146,   140,   147,   140,   148,   140,
     149,   140,   150,   151,   151,   151,   152,   152,   153,   153,
     153,   153,   153,   153,   153,   153,   154,   154,   155,   155,
     156,   156,   156,   157,   157,   158,   158,   158,   158,   158,
     159,   159,   160,   162,   161,   163,   161,   165,   164,   166,
     166,   166,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   168,   168,   168,
     168,   169,   169,   169,   170,   170,   171,   171,   172,   172,
     172,   173,   173,   173,   174,   174,   174,   175,   175,   176,
     176,   176,   177,   177,   177,   177,   177,   178,   178,   178,
     178,   178,   178,   179,   179,   180,   181,   181,   182,   182,
     184,   183,   185,   185,   185,   186,   186,   187,   187,   188,
     188,   189,   189,   190,   191,   192,   192,   192,   193,   194,
     194,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     196,   196,   196,   196,   196,   196,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   198,   198,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   200,
     200,   201,   201
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
       4,     7,     1,     2,     0,     0,     9,     4,     2,     4,
       9,     0,     5,     4,     2,     4,     3,     6,     0,     2,
       1,     2,     4,     5,     7,     5,     5,     6,     8,     7,
       0,     3,     0,     3,     3,     1,     0,     3,     1,     4,
       3,     1,     2,     2,     3,     3,     1,     3,     3,     3,
       3,     1,     2,     2,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     2,     3,     1,     0,     4,     0,     0,
       0,     7,     5,     0,     3,     1,     6,     0,     2,     1,
       4,     6,     7,     9,     0,     9,     0,    11,     0,     7,
       0,    11,     0,    13,     0,    11,     0,    11,     0,    12,
       0,    14,     0,     3,     2,     0,     3,     1,     3,     5,
       4,     3,     4,     5,     6,     4,     1,     3,     0,     3,
       3,     2,     0,     3,     1,     3,     4,     4,     3,     5,
       3,     1,     3,     0,     4,     0,     2,     0,     2,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     5,     5,     7,
       5,     1,     3,     2,     6,     4,     1,     2,     2,     4,
       5,     1,     3,     1,     4,     3,     6,     3,     1,     1,
       3,     5,     3,     3,     3,     3,     3,     4,     3,     2,
       3,     2,     1,     3,     1,     3,     2,     2,     4,     6,
       0,     7,     0,     2,     2,     3,     1,     3,     0,     3,
       1,     1,     3,     4,     4,     8,     8,     6,     4,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     3,     1,     2,     2,     2,     3,     1,
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
  case 2: /* compilation_unit: program  */
#line 201 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1976 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 202 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1982 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 205 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1988 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 206 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1994 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 209 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2000 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 210 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2006 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 213 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2012 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 214 "pascal.y"
     { (yyval.node) = NULL; }
#line 2018 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 217 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2024 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 220 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2030 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 221 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2036 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 224 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2042 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 225 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2048 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 226 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2054 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 227 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2060 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 228 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2066 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 229 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2072 "pascal.tab.c"
    break;

  case 19: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 232 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2078 "pascal.tab.c"
    break;

  case 20: /* intf_part_list: intf_part_list intf_part  */
#line 235 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2084 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: %empty  */
#line 236 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2090 "pascal.tab.c"
    break;

  case 22: /* intf_part: USESSY id_list SEMICOLON  */
#line 239 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2096 "pascal.tab.c"
    break;

  case 23: /* intf_part: CONSTSY const_decl_list  */
#line 240 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2102 "pascal.tab.c"
    break;

  case 24: /* intf_part: TYPESY type_decl_list  */
#line 241 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2108 "pascal.tab.c"
    break;

  case 25: /* intf_part: VARSY var_decl_list  */
#line 242 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2114 "pascal.tab.c"
    break;

  case 26: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 243 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2120 "pascal.tab.c"
    break;

  case 27: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 244 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2126 "pascal.tab.c"
    break;

  case 28: /* init_opt: INITIALIZATIONSY statement_list  */
#line 247 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2132 "pascal.tab.c"
    break;

  case 29: /* init_opt: BEGINSY statement_list  */
#line 248 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2138 "pascal.tab.c"
    break;

  case 30: /* init_opt: %empty  */
#line 249 "pascal.y"
     { (yyval.node) = NULL; }
#line 2144 "pascal.tab.c"
    break;

  case 31: /* fini_opt: FINALIZATIONSY statement_list  */
#line 252 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2150 "pascal.tab.c"
    break;

  case 32: /* fini_opt: %empty  */
#line 253 "pascal.y"
     { (yyval.node) = NULL; }
#line 2156 "pascal.tab.c"
    break;

  case 33: /* label_list: label_list COMMA INTCONST  */
#line 256 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2162 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA IDENT  */
#line 257 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2168 "pascal.tab.c"
    break;

  case 35: /* label_list: INTCONST  */
#line 258 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2174 "pascal.tab.c"
    break;

  case 36: /* label_list: IDENT  */
#line 259 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2180 "pascal.tab.c"
    break;

  case 37: /* const_decl_list: const_decl_list const_decl  */
#line 262 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2186 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl  */
#line 263 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2192 "pascal.tab.c"
    break;

  case 39: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 266 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2198 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 267 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2204 "pascal.tab.c"
    break;

  case 41: /* typed_value: expression  */
#line 270 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2210 "pascal.tab.c"
    break;

  case 42: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 271 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2216 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT field_value_list RPARENT  */
#line 272 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2222 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 273 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2228 "pascal.tab.c"
    break;

  case 45: /* typed_value_list: typed_value  */
#line 276 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2234 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 277 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2240 "pascal.tab.c"
    break;

  case 47: /* field_value_list: IDENT COLON typed_value  */
#line 280 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2246 "pascal.tab.c"
    break;

  case 48: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 281 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2252 "pascal.tab.c"
    break;

  case 49: /* constant: scalar_constant  */
#line 284 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2258 "pascal.tab.c"
    break;

  case 50: /* constant: PLUS scalar_constant  */
#line 285 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2264 "pascal.tab.c"
    break;

  case 51: /* constant: MINUS scalar_constant  */
#line 286 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2270 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: IDENT  */
#line 289 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2276 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: INTCONST  */
#line 290 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2282 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: REALCONST  */
#line 291 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2288 "pascal.tab.c"
    break;

  case 55: /* scalar_constant: STRINGCONST  */
#line 292 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2294 "pascal.tab.c"
    break;

  case 56: /* scalar_constant: CHARCODE  */
#line 293 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2300 "pascal.tab.c"
    break;

  case 57: /* type_decl_list: type_decl_list type_decl  */
#line 296 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2306 "pascal.tab.c"
    break;

  case 58: /* type_decl_list: type_decl  */
#line 297 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2312 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 300 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2318 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 301 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2324 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 302 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2330 "pascal.tab.c"
    break;

  case 62: /* type: simple_type  */
#line 305 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2336 "pascal.tab.c"
    break;

  case 63: /* type: ARROW IDENT  */
#line 306 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2342 "pascal.tab.c"
    break;

  case 64: /* $@1: %empty  */
#line 307 "pascal.y"
                                                       { }
#line 2348 "pascal.tab.c"
    break;

  case 65: /* $@2: %empty  */
#line 307 "pascal.y"
                                                           { }
#line 2354 "pascal.tab.c"
    break;

  case 66: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 307 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2360 "pascal.tab.c"
    break;

  case 67: /* type: packed_opt ARRAYSY OFSY type  */
#line 308 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2366 "pascal.tab.c"
    break;

  case 68: /* type: PROCEDURESY pf_params  */
#line 309 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2372 "pascal.tab.c"
    break;

  case 69: /* type: FUNCTIONSY pf_params COLON IDENT  */
#line 310 "pascal.y"
                                      { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2378 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 311 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2384 "pascal.tab.c"
    break;

  case 71: /* $@3: %empty  */
#line 312 "pascal.y"
                         { }
#line 2390 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 312 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2396 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt SETSY OFSY simple_type  */
#line 313 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2402 "pascal.tab.c"
    break;

  case 74: /* type: packed_opt FILESY  */
#line 314 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2408 "pascal.tab.c"
    break;

  case 75: /* type: packed_opt FILESY OFSY type  */
#line 315 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2414 "pascal.tab.c"
    break;

  case 76: /* type: CLASSSY class_members ENDSY  */
#line 316 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2420 "pascal.tab.c"
    break;

  case 77: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 317 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2426 "pascal.tab.c"
    break;

  case 78: /* class_members: %empty  */
#line 320 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2432 "pascal.tab.c"
    break;

  case 79: /* class_members: class_members class_member  */
#line 321 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2438 "pascal.tab.c"
    break;

  case 80: /* class_member: IDENT  */
#line 324 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2444 "pascal.tab.c"
    break;

  case 81: /* class_member: IDENT SEMICOLON  */
#line 325 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2450 "pascal.tab.c"
    break;

  case 82: /* class_member: id_list COLON type SEMICOLON  */
#line 326 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2456 "pascal.tab.c"
    break;

  case 83: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 327 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2462 "pascal.tab.c"
    break;

  case 84: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 328 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2468 "pascal.tab.c"
    break;

  case 85: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 329 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2474 "pascal.tab.c"
    break;

  case 86: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 330 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2480 "pascal.tab.c"
    break;

  case 87: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 331 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2486 "pascal.tab.c"
    break;

  case 88: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 332 "pascal.y"
                                                                               { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2492 "pascal.tab.c"
    break;

  case 89: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 333 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2498 "pascal.tab.c"
    break;

  case 90: /* prop_index_opt: %empty  */
#line 336 "pascal.y"
    { (yyval.node) = NULL; }
#line 2504 "pascal.tab.c"
    break;

  case 91: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 337 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2510 "pascal.tab.c"
    break;

  case 92: /* prop_specs: %empty  */
#line 340 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2516 "pascal.tab.c"
    break;

  case 93: /* prop_specs: prop_specs IDENT IDENT  */
#line 341 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2522 "pascal.tab.c"
    break;

  case 94: /* prop_specs: prop_specs IDENT INTCONST  */
#line 342 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2528 "pascal.tab.c"
    break;

  case 95: /* packed_opt: PACKEDSY  */
#line 345 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2534 "pascal.tab.c"
    break;

  case 96: /* packed_opt: %empty  */
#line 346 "pascal.y"
     { (yyval.node) = NULL; }
#line 2540 "pascal.tab.c"
    break;

  case 97: /* simple_type: LPARENT id_list RPARENT  */
#line 349 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2546 "pascal.tab.c"
    break;

  case 98: /* simple_type: IDENT  */
#line 350 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2552 "pascal.tab.c"
    break;

  case 99: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 351 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2558 "pascal.tab.c"
    break;

  case 100: /* simple_type: lo_bound DOTDOT b_expr  */
#line 352 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2564 "pascal.tab.c"
    break;

  case 101: /* lo_bound: lo_term  */
#line 355 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2570 "pascal.tab.c"
    break;

  case 102: /* lo_bound: PLUS lo_term  */
#line 356 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2576 "pascal.tab.c"
    break;

  case 103: /* lo_bound: MINUS lo_term  */
#line 357 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2582 "pascal.tab.c"
    break;

  case 104: /* lo_bound: lo_bound PLUS b_term  */
#line 358 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2588 "pascal.tab.c"
    break;

  case 105: /* lo_bound: lo_bound MINUS b_term  */
#line 359 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2594 "pascal.tab.c"
    break;

  case 106: /* lo_term: scalar_constant  */
#line 362 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2600 "pascal.tab.c"
    break;

  case 107: /* lo_term: lo_term MUL b_factor  */
#line 363 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2606 "pascal.tab.c"
    break;

  case 108: /* lo_term: lo_term IDIV b_factor  */
#line 364 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2612 "pascal.tab.c"
    break;

  case 109: /* lo_term: lo_term IMOD b_factor  */
#line 365 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2618 "pascal.tab.c"
    break;

  case 110: /* lo_term: lo_term SHIFTOP b_factor  */
#line 366 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2624 "pascal.tab.c"
    break;

  case 111: /* b_expr: b_term  */
#line 369 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2630 "pascal.tab.c"
    break;

  case 112: /* b_expr: PLUS b_term  */
#line 370 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2636 "pascal.tab.c"
    break;

  case 113: /* b_expr: MINUS b_term  */
#line 371 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2642 "pascal.tab.c"
    break;

  case 114: /* b_expr: b_expr PLUS b_term  */
#line 372 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2648 "pascal.tab.c"
    break;

  case 115: /* b_expr: b_expr MINUS b_term  */
#line 373 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2654 "pascal.tab.c"
    break;

  case 116: /* b_term: b_factor  */
#line 376 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2660 "pascal.tab.c"
    break;

  case 117: /* b_term: b_term MUL b_factor  */
#line 377 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2666 "pascal.tab.c"
    break;

  case 118: /* b_term: b_term IDIV b_factor  */
#line 378 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2672 "pascal.tab.c"
    break;

  case 119: /* b_term: b_term IMOD b_factor  */
#line 379 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2678 "pascal.tab.c"
    break;

  case 120: /* b_term: b_term SHIFTOP b_factor  */
#line 380 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2684 "pascal.tab.c"
    break;

  case 121: /* b_factor: scalar_constant  */
#line 383 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2690 "pascal.tab.c"
    break;

  case 122: /* b_factor: LPARENT b_expr RPARENT  */
#line 384 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2696 "pascal.tab.c"
    break;

  case 123: /* record_body: record_field_list record_case_opt  */
#line 387 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 124: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 390 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2708 "pascal.tab.c"
    break;

  case 125: /* record_field_list: record_field  */
#line 391 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2714 "pascal.tab.c"
    break;

  case 126: /* $@4: %empty  */
#line 394 "pascal.y"
                  { }
#line 2720 "pascal.tab.c"
    break;

  case 127: /* record_field: id_list COLON $@4 type  */
#line 394 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2726 "pascal.tab.c"
    break;

  case 128: /* record_field: %empty  */
#line 395 "pascal.y"
     { (yyval.node) = NULL; }
#line 2732 "pascal.tab.c"
    break;

  case 129: /* case_arm_mark: %empty  */
#line 398 "pascal.y"
    { }
#line 2738 "pascal.tab.c"
    break;

  case 130: /* case_open: %empty  */
#line 401 "pascal.y"
    { }
#line 2744 "pascal.tab.c"
    break;

  case 131: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2750 "pascal.tab.c"
    break;

  case 132: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 405 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2756 "pascal.tab.c"
    break;

  case 133: /* record_case_opt: %empty  */
#line 406 "pascal.y"
     { (yyval.node) = NULL; }
#line 2762 "pascal.tab.c"
    break;

  case 134: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 409 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2768 "pascal.tab.c"
    break;

  case 135: /* record_case_list: record_case_arm  */
#line 410 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 136: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 413 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2780 "pascal.tab.c"
    break;

  case 137: /* record_case_arm: %empty  */
#line 414 "pascal.y"
     { (yyval.node) = NULL; }
#line 2786 "pascal.tab.c"
    break;

  case 138: /* var_decl_list: var_decl_list var_decl  */
#line 417 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2792 "pascal.tab.c"
    break;

  case 139: /* var_decl_list: var_decl  */
#line 418 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 140: /* var_decl: id_list COLON type SEMICOLON  */
#line 421 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2804 "pascal.tab.c"
    break;

  case 141: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 422 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2810 "pascal.tab.c"
    break;

  case 142: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 425 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2816 "pascal.tab.c"
    break;

  case 143: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 426 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2822 "pascal.tab.c"
    break;

  case 144: /* $@5: %empty  */
#line 427 "pascal.y"
                                                                               { }
#line 2828 "pascal.tab.c"
    break;

  case 145: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 427 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2834 "pascal.tab.c"
    break;

  case 146: /* $@6: %empty  */
#line 428 "pascal.y"
                                                                                          { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2840 "pascal.tab.c"
    break;

  case 147: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 428 "pascal.y"
                                                                                                                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2846 "pascal.tab.c"
    break;

  case 148: /* $@7: %empty  */
#line 429 "pascal.y"
                                        { }
#line 2852 "pascal.tab.c"
    break;

  case 149: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 429 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2858 "pascal.tab.c"
    break;

  case 150: /* $@8: %empty  */
#line 430 "pascal.y"
                                                                                            { }
#line 2864 "pascal.tab.c"
    break;

  case 151: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 430 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2870 "pascal.tab.c"
    break;

  case 152: /* $@9: %empty  */
#line 431 "pascal.y"
                                                                                                       { }
#line 2876 "pascal.tab.c"
    break;

  case 153: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 431 "pascal.y"
                                                                                                                           {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 2884 "pascal.tab.c"
    break;

  case 154: /* $@10: %empty  */
#line 434 "pascal.y"
                                                                                              { }
#line 2890 "pascal.tab.c"
    break;

  case 155: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 434 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 2898 "pascal.tab.c"
    break;

  case 156: /* $@11: %empty  */
#line 437 "pascal.y"
                                                                                             { }
#line 2904 "pascal.tab.c"
    break;

  case 157: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 437 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 2912 "pascal.tab.c"
    break;

  case 158: /* $@12: %empty  */
#line 440 "pascal.y"
                                                                                                    { }
#line 2918 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 440 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 2926 "pascal.tab.c"
    break;

  case 160: /* $@13: %empty  */
#line 443 "pascal.y"
                                                                                                               { }
#line 2932 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 443 "pascal.y"
                                                                                                                                   {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 2940 "pascal.tab.c"
    break;

  case 162: /* pv_mark: %empty  */
#line 448 "pascal.y"
    { }
#line 2946 "pascal.tab.c"
    break;

  case 163: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 451 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2952 "pascal.tab.c"
    break;

  case 164: /* parameter_list_opt: LPARENT RPARENT  */
#line 452 "pascal.y"
                     { (yyval.node) = NULL; }
#line 2958 "pascal.tab.c"
    break;

  case 165: /* parameter_list_opt: %empty  */
#line 453 "pascal.y"
     { (yyval.node) = NULL; }
#line 2964 "pascal.tab.c"
    break;

  case 166: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 456 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2970 "pascal.tab.c"
    break;

  case 167: /* parameter_decl_list: parameter_decl  */
#line 457 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2976 "pascal.tab.c"
    break;

  case 168: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 460 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2982 "pascal.tab.c"
    break;

  case 169: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 461 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2988 "pascal.tab.c"
    break;

  case 170: /* parameter_decl: VARSY id_list COLON param_type  */
#line 462 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2994 "pascal.tab.c"
    break;

  case 171: /* parameter_decl: id_list COLON param_type  */
#line 463 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3000 "pascal.tab.c"
    break;

  case 172: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 464 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3006 "pascal.tab.c"
    break;

  case 173: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 465 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3012 "pascal.tab.c"
    break;

  case 174: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 466 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3018 "pascal.tab.c"
    break;

  case 175: /* parameter_decl: IDENT id_list COLON param_type  */
#line 467 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3024 "pascal.tab.c"
    break;

  case 176: /* param_type: IDENT  */
#line 470 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3030 "pascal.tab.c"
    break;

  case 177: /* param_type: ARRAYSY OFSY IDENT  */
#line 471 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3036 "pascal.tab.c"
    break;

  case 178: /* routine_directives: %empty  */
#line 474 "pascal.y"
    { (yyval.node) = NULL; }
#line 3042 "pascal.tab.c"
    break;

  case 179: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 475 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3055 "pascal.tab.c"
    break;

  case 180: /* pf_params: LPARENT pf_sections RPARENT  */
#line 485 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3061 "pascal.tab.c"
    break;

  case 181: /* pf_params: LPARENT RPARENT  */
#line 486 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3067 "pascal.tab.c"
    break;

  case 182: /* pf_params: %empty  */
#line 487 "pascal.y"
     { (yyval.node) = NULL; }
#line 3073 "pascal.tab.c"
    break;

  case 183: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 490 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3079 "pascal.tab.c"
    break;

  case 184: /* pf_sections: pf_section  */
#line 491 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3085 "pascal.tab.c"
    break;

  case 185: /* pf_section: id_list COLON param_type  */
#line 494 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3091 "pascal.tab.c"
    break;

  case 186: /* pf_section: VARSY id_list COLON param_type  */
#line 495 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3097 "pascal.tab.c"
    break;

  case 187: /* pf_section: CONSTSY id_list COLON param_type  */
#line 496 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3103 "pascal.tab.c"
    break;

  case 188: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 497 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3109 "pascal.tab.c"
    break;

  case 189: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 498 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3115 "pascal.tab.c"
    break;

  case 190: /* id_list: id_list COMMA IDENT  */
#line 501 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3121 "pascal.tab.c"
    break;

  case 191: /* id_list: IDENT  */
#line 502 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 192: /* body: BEGINSY statement_list ENDSY  */
#line 505 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3133 "pascal.tab.c"
    break;

  case 193: /* $@14: %empty  */
#line 508 "pascal.y"
                             { pas_stmt_mark(); }
#line 3139 "pascal.tab.c"
    break;

  case 194: /* statement_list: statement_list SEMICOLON $@14 statement  */
#line 508 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3145 "pascal.tab.c"
    break;

  case 195: /* $@15: %empty  */
#line 509 "pascal.y"
     { pas_stmt_mark(); }
#line 3151 "pascal.tab.c"
    break;

  case 196: /* statement_list: $@15 statement  */
#line 509 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3157 "pascal.tab.c"
    break;

  case 197: /* $@16: %empty  */
#line 512 "pascal.y"
    { pas_stmt_mark(); }
#line 3163 "pascal.tab.c"
    break;

  case 198: /* body_stmt: $@16 statement  */
#line 512 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3169 "pascal.tab.c"
    break;

  case 199: /* statement: statement_no_label  */
#line 515 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3175 "pascal.tab.c"
    break;

  case 200: /* statement: INTCONST COLON statement_no_label  */
#line 516 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3181 "pascal.tab.c"
    break;

  case 201: /* statement: IDENT COLON statement_no_label  */
#line 517 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3187 "pascal.tab.c"
    break;

  case 202: /* statement_no_label: assignment  */
#line 520 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3193 "pascal.tab.c"
    break;

  case 203: /* statement_no_label: call  */
#line 521 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3199 "pascal.tab.c"
    break;

  case 204: /* statement_no_label: compound_statement  */
#line 522 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3205 "pascal.tab.c"
    break;

  case 205: /* statement_no_label: goto_statement  */
#line 523 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3211 "pascal.tab.c"
    break;

  case 206: /* statement_no_label: if_statement  */
#line 524 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3217 "pascal.tab.c"
    break;

  case 207: /* statement_no_label: case_statement  */
#line 525 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3223 "pascal.tab.c"
    break;

  case 208: /* statement_no_label: while_statement  */
#line 526 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3229 "pascal.tab.c"
    break;

  case 209: /* statement_no_label: repeat_statement  */
#line 527 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3235 "pascal.tab.c"
    break;

  case 210: /* statement_no_label: for_statement  */
#line 528 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3241 "pascal.tab.c"
    break;

  case 211: /* statement_no_label: with_statement  */
#line 529 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3247 "pascal.tab.c"
    break;

  case 212: /* statement_no_label: try_statement  */
#line 530 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3253 "pascal.tab.c"
    break;

  case 213: /* statement_no_label: raise_statement  */
#line 531 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3259 "pascal.tab.c"
    break;

  case 214: /* statement_no_label: inherited_call  */
#line 532 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3265 "pascal.tab.c"
    break;

  case 215: /* statement_no_label: INHERITEDSY  */
#line 533 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3271 "pascal.tab.c"
    break;

  case 216: /* statement_no_label: %empty  */
#line 534 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3277 "pascal.tab.c"
    break;

  case 217: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 537 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3283 "pascal.tab.c"
    break;

  case 218: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 538 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3289 "pascal.tab.c"
    break;

  case 219: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 539 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3295 "pascal.tab.c"
    break;

  case 220: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 540 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3301 "pascal.tab.c"
    break;

  case 221: /* handler_list: handler  */
#line 543 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3307 "pascal.tab.c"
    break;

  case 222: /* handler_list: handler_list SEMICOLON handler  */
#line 544 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3313 "pascal.tab.c"
    break;

  case 223: /* handler_list: handler_list SEMICOLON  */
#line 545 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3319 "pascal.tab.c"
    break;

  case 224: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 548 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3325 "pascal.tab.c"
    break;

  case 225: /* handler: ONSY IDENT DOSY body_stmt  */
#line 549 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3331 "pascal.tab.c"
    break;

  case 226: /* raise_statement: RAISESY  */
#line 552 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3337 "pascal.tab.c"
    break;

  case 227: /* raise_statement: RAISESY expression  */
#line 553 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3343 "pascal.tab.c"
    break;

  case 228: /* inherited_call: INHERITEDSY IDENT  */
#line 556 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3349 "pascal.tab.c"
    break;

  case 229: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 557 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3355 "pascal.tab.c"
    break;

  case 230: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 558 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3361 "pascal.tab.c"
    break;

  case 231: /* call: IDENT  */
#line 561 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3367 "pascal.tab.c"
    break;

  case 232: /* call: selector PERIOD IDENT  */
#line 562 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3373 "pascal.tab.c"
    break;

  case 233: /* call: call_with_args  */
#line 563 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3379 "pascal.tab.c"
    break;

  case 234: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 566 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3385 "pascal.tab.c"
    break;

  case 235: /* call_with_args: IDENT LPARENT RPARENT  */
#line 567 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3391 "pascal.tab.c"
    break;

  case 236: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 568 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3397 "pascal.tab.c"
    break;

  case 237: /* argument_list: argument_list COMMA argument  */
#line 571 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3403 "pascal.tab.c"
    break;

  case 238: /* argument_list: argument  */
#line 572 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3409 "pascal.tab.c"
    break;

  case 239: /* argument: expression  */
#line 575 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3415 "pascal.tab.c"
    break;

  case 240: /* argument: expression COLON expression  */
#line 576 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3421 "pascal.tab.c"
    break;

  case 241: /* argument: expression COLON expression COLON expression  */
#line 577 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3427 "pascal.tab.c"
    break;

  case 242: /* assignment: selector BECOMES expression  */
#line 580 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3433 "pascal.tab.c"
    break;

  case 243: /* assignment: selector ADDBECOMES expression  */
#line 581 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3439 "pascal.tab.c"
    break;

  case 244: /* assignment: selector SUBBECOMES expression  */
#line 582 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3445 "pascal.tab.c"
    break;

  case 245: /* assignment: selector MULBECOMES expression  */
#line 583 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3451 "pascal.tab.c"
    break;

  case 246: /* assignment: selector DIVBECOMES expression  */
#line 584 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3457 "pascal.tab.c"
    break;

  case 247: /* selector: selector LBRACK expression_list RBRACK  */
#line 587 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3463 "pascal.tab.c"
    break;

  case 248: /* selector: selector PERIOD IDENT  */
#line 588 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3469 "pascal.tab.c"
    break;

  case 249: /* selector: selector ARROW  */
#line 589 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3475 "pascal.tab.c"
    break;

  case 250: /* selector: call_with_args PERIOD IDENT  */
#line 590 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3481 "pascal.tab.c"
    break;

  case 251: /* selector: call_with_args ARROW  */
#line 591 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3487 "pascal.tab.c"
    break;

  case 252: /* selector: IDENT  */
#line 592 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3493 "pascal.tab.c"
    break;

  case 253: /* expression_list: expression_list COMMA expression  */
#line 595 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3499 "pascal.tab.c"
    break;

  case 254: /* expression_list: expression  */
#line 596 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3505 "pascal.tab.c"
    break;

  case 255: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 599 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3511 "pascal.tab.c"
    break;

  case 256: /* goto_statement: GOTOSY INTCONST  */
#line 602 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3517 "pascal.tab.c"
    break;

  case 257: /* goto_statement: GOTOSY IDENT  */
#line 603 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3523 "pascal.tab.c"
    break;

  case 258: /* if_statement: IFSY expression THENSY body_stmt  */
#line 606 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3529 "pascal.tab.c"
    break;

  case 259: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 607 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3535 "pascal.tab.c"
    break;

  case 260: /* $@17: %empty  */
#line 610 "pascal.y"
                           { }
#line 3541 "pascal.tab.c"
    break;

  case 261: /* case_statement: CASESY expression OFSY $@17 case_list case_tail ENDSY  */
#line 610 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3547 "pascal.tab.c"
    break;

  case 262: /* case_tail: %empty  */
#line 613 "pascal.y"
    { (yyval.node) = NULL; }
#line 3553 "pascal.tab.c"
    break;

  case 263: /* case_tail: ELSESY statement_list  */
#line 614 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3559 "pascal.tab.c"
    break;

  case 264: /* case_tail: OTHERWISESY statement_list  */
#line 615 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3565 "pascal.tab.c"
    break;

  case 265: /* case_list: case_list SEMICOLON case_elem  */
#line 618 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3571 "pascal.tab.c"
    break;

  case 266: /* case_list: case_elem  */
#line 619 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3577 "pascal.tab.c"
    break;

  case 267: /* case_elem: constant_list COLON body_stmt  */
#line 622 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3583 "pascal.tab.c"
    break;

  case 268: /* case_elem: %empty  */
#line 623 "pascal.y"
     { (yyval.node) = NULL; }
#line 3589 "pascal.tab.c"
    break;

  case 269: /* constant_list: constant_list COMMA case_label  */
#line 626 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3595 "pascal.tab.c"
    break;

  case 270: /* constant_list: case_label  */
#line 627 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3601 "pascal.tab.c"
    break;

  case 271: /* case_label: b_expr  */
#line 630 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3607 "pascal.tab.c"
    break;

  case 272: /* case_label: b_expr DOTDOT b_expr  */
#line 631 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3613 "pascal.tab.c"
    break;

  case 273: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 634 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3619 "pascal.tab.c"
    break;

  case 274: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 637 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3625 "pascal.tab.c"
    break;

  case 275: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 640 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3631 "pascal.tab.c"
    break;

  case 276: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 641 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3637 "pascal.tab.c"
    break;

  case 277: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 642 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3643 "pascal.tab.c"
    break;

  case 278: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 645 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3649 "pascal.tab.c"
    break;

  case 279: /* with_open: with_open COMMA selector  */
#line 648 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3655 "pascal.tab.c"
    break;

  case 280: /* with_open: selector  */
#line 649 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3661 "pascal.tab.c"
    break;

  case 281: /* expression: simple_expression  */
#line 656 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3667 "pascal.tab.c"
    break;

  case 282: /* expression: expression INOP simple_expression  */
#line 657 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3673 "pascal.tab.c"
    break;

  case 283: /* expression: expression LTOP simple_expression  */
#line 658 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3679 "pascal.tab.c"
    break;

  case 284: /* expression: expression LEOP simple_expression  */
#line 659 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3685 "pascal.tab.c"
    break;

  case 285: /* expression: expression GTOP simple_expression  */
#line 660 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3691 "pascal.tab.c"
    break;

  case 286: /* expression: expression GEOP simple_expression  */
#line 661 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3697 "pascal.tab.c"
    break;

  case 287: /* expression: expression NEOP simple_expression  */
#line 662 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3703 "pascal.tab.c"
    break;

  case 288: /* expression: expression EQOP simple_expression  */
#line 663 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3709 "pascal.tab.c"
    break;

  case 289: /* expression: expression ISOP simple_expression  */
#line 664 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3715 "pascal.tab.c"
    break;

  case 290: /* simple_expression: term  */
#line 667 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3721 "pascal.tab.c"
    break;

  case 291: /* simple_expression: PLUS term  */
#line 668 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3727 "pascal.tab.c"
    break;

  case 292: /* simple_expression: MINUS term  */
#line 669 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3733 "pascal.tab.c"
    break;

  case 293: /* simple_expression: simple_expression PLUS term  */
#line 670 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3739 "pascal.tab.c"
    break;

  case 294: /* simple_expression: simple_expression MINUS term  */
#line 671 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3745 "pascal.tab.c"
    break;

  case 295: /* simple_expression: simple_expression OROP term  */
#line 672 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3751 "pascal.tab.c"
    break;

  case 296: /* simple_expression: simple_expression XOROP term  */
#line 673 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3757 "pascal.tab.c"
    break;

  case 297: /* term: power  */
#line 676 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3763 "pascal.tab.c"
    break;

  case 298: /* term: term MUL power  */
#line 677 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3769 "pascal.tab.c"
    break;

  case 299: /* term: term RDIV power  */
#line 678 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3775 "pascal.tab.c"
    break;

  case 300: /* term: term IDIV power  */
#line 679 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3781 "pascal.tab.c"
    break;

  case 301: /* term: term IMOD power  */
#line 680 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3787 "pascal.tab.c"
    break;

  case 302: /* term: term ANDOP power  */
#line 681 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3793 "pascal.tab.c"
    break;

  case 303: /* term: term SHIFTOP power  */
#line 682 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3799 "pascal.tab.c"
    break;

  case 304: /* term: term ASOP power  */
#line 683 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3805 "pascal.tab.c"
    break;

  case 305: /* power: factor  */
#line 686 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3811 "pascal.tab.c"
    break;

  case 306: /* power: power POWOP factor  */
#line 687 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3817 "pascal.tab.c"
    break;

  case 307: /* factor: selector  */
#line 690 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3823 "pascal.tab.c"
    break;

  case 308: /* factor: call_with_args  */
#line 691 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3829 "pascal.tab.c"
    break;

  case 309: /* factor: INTCONST  */
#line 692 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3835 "pascal.tab.c"
    break;

  case 310: /* factor: REALCONST  */
#line 693 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3841 "pascal.tab.c"
    break;

  case 311: /* factor: STRINGCONST  */
#line 694 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 3847 "pascal.tab.c"
    break;

  case 312: /* factor: CHARCODE  */
#line 695 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3853 "pascal.tab.c"
    break;

  case 313: /* factor: LPARENT expression RPARENT  */
#line 696 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3859 "pascal.tab.c"
    break;

  case 314: /* factor: inherited_call  */
#line 697 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3865 "pascal.tab.c"
    break;

  case 315: /* factor: NOTSY factor  */
#line 698 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 3871 "pascal.tab.c"
    break;

  case 316: /* factor: ATSIGN factor  */
#line 699 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 3877 "pascal.tab.c"
    break;

  case 317: /* factor: LBRACK RBRACK  */
#line 700 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 3883 "pascal.tab.c"
    break;

  case 318: /* factor: LBRACK set_member_list RBRACK  */
#line 701 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 3889 "pascal.tab.c"
    break;

  case 319: /* set_member_list: set_member  */
#line 704 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3895 "pascal.tab.c"
    break;

  case 320: /* set_member_list: set_member_list COMMA set_member  */
#line 705 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3901 "pascal.tab.c"
    break;

  case 321: /* set_member: expression  */
#line 708 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3907 "pascal.tab.c"
    break;

  case 322: /* set_member: expression DOTDOT expression  */
#line 709 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3913 "pascal.tab.c"
    break;


#line 3917 "pascal.tab.c"

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

#line 715 "pascal.y"

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
