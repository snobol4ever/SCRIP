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
void pascal_note_type_ident(tree_t *id) {
    if (!id || !id->v.sval) return;
    if ((!strcasecmp(id->v.sval, "string") && !g_pascal_cenv.string_declared) || !strcasecmp(id->v.sval, "ansistring") || !strcasecmp(id->v.sval, "pchar")) pascal_flip_dialect(PAS_DIALECT_FPC);
}
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

#line 224 "pascal.tab.c"

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
#define YYLAST   1038

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  325
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  736

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
       0,   204,   204,   205,   208,   209,   212,   213,   216,   217,
     220,   223,   224,   227,   228,   229,   230,   231,   232,   235,
     238,   239,   242,   243,   244,   245,   246,   247,   250,   251,
     252,   255,   256,   259,   260,   261,   262,   265,   266,   269,
     270,   273,   274,   275,   276,   279,   280,   283,   284,   287,
     288,   289,   292,   293,   294,   295,   296,   299,   300,   303,
     304,   305,   308,   309,   310,   310,   310,   311,   312,   313,
     314,   315,   315,   316,   317,   318,   319,   320,   323,   324,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     339,   340,   343,   344,   345,   348,   349,   352,   353,   354,
     355,   358,   359,   360,   361,   362,   365,   366,   367,   368,
     369,   370,   373,   374,   375,   376,   377,   380,   381,   382,
     383,   384,   387,   388,   389,   392,   395,   396,   399,   399,
     400,   403,   406,   409,   410,   411,   414,   415,   418,   419,
     422,   423,   426,   427,   430,   431,   432,   432,   433,   433,
     434,   434,   435,   435,   436,   436,   439,   439,   442,   442,
     445,   445,   448,   448,   453,   456,   457,   458,   461,   462,
     465,   466,   467,   468,   469,   470,   471,   472,   475,   476,
     479,   480,   490,   491,   492,   495,   496,   499,   500,   501,
     502,   503,   506,   507,   510,   513,   513,   514,   514,   517,
     517,   520,   521,   522,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   542,
     543,   544,   545,   548,   549,   550,   553,   554,   557,   558,
     561,   562,   563,   566,   567,   568,   571,   572,   573,   574,
     577,   578,   581,   582,   583,   586,   587,   588,   589,   590,
     593,   594,   595,   596,   597,   598,   601,   602,   605,   608,
     609,   612,   613,   616,   616,   619,   620,   621,   624,   625,
     628,   629,   632,   633,   636,   637,   640,   643,   646,   647,
     648,   651,   654,   655,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   673,   674,   675,   676,   677,   678,   679,
     682,   683,   684,   685,   686,   687,   688,   689,   692,   693,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   710,   711,   714,   715
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

#define YYPACT_NINF (-454)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-256)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     227,    42,    55,    53,  -454,   106,   512,  -454,  -454,   177,
     208,  -454,  -454,   128,   148,   186,   199,   186,   213,  -454,
     241,   224,   251,   281,  -454,  -454,  -454,   323,   186,   277,
     331,  -454,  -454,    85,   286,   148,  -454,  -454,    88,   124,
     186,  -454,   196,   318,   116,   418,   353,   241,  -454,   336,
     359,   370,   390,  -454,    54,  -454,  -454,  -454,   228,   737,
     609,  -454,  -454,   374,   381,   320,  -454,   609,   388,    32,
    -454,  -454,   279,   389,  -454,   737,   408,   737,  -454,   737,
     443,   889,  -454,   737,   435,  -454,  -454,  -454,  -454,  -454,
    -454,   358,  -454,   930,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,   638,  -454,   455,   456,   442,   446,  -454,   467,
     993,  -454,  -454,   797,   797,   797,   653,   737,   797,  -454,
    -454,  -454,  -454,   461,   435,  -454,   358,   375,    36,   237,
     341,   458,  -454,   470,  -454,   470,   498,   498,   465,   186,
    -454,  -454,  -454,  -454,   466,   478,  -454,   493,   205,  -454,
     239,   132,  -454,  -454,   153,   541,    46,  -454,  -454,   508,
     418,  -454,  -454,   354,    59,   261,   358,   375,    74,    22,
     163,   425,   426,   426,   679,    16,   886,   494,   500,  -454,
     737,   506,  -454,   737,   737,   737,   737,   737,    43,   580,
     513,   515,  -454,  -454,  -454,   148,   186,  -454,   517,   186,
     518,   241,  -454,  -454,   341,   341,  -454,   826,   214,  -454,
     838,  -454,   520,  -454,   737,   737,   737,   737,   737,   737,
     737,   737,   797,   797,   797,   797,   797,   797,   797,   797,
     797,   797,   797,   797,   157,  -454,   539,   542,   132,   132,
    -454,   222,   939,   407,   186,   314,   763,    98,   574,   598,
    -454,   604,   604,   407,   604,   604,   604,   604,   320,   186,
     552,   186,   553,  -454,   186,    17,  -454,   428,   621,  -454,
     763,   320,  -454,   559,  -454,   737,   737,   737,  -454,  -454,
     408,  -454,  -454,  -454,   902,  -454,  -454,  -454,   289,  -454,
     860,   547,  -454,   695,  -454,   886,   917,   280,   886,   886,
     886,   886,   886,  -454,   186,  -454,  -454,  -454,   320,   320,
     148,   105,   325,  -454,   186,  -454,   241,   737,   737,  -454,
    -454,   578,   237,   237,   237,   237,   237,   237,   237,   237,
     341,   341,   341,   341,   458,   458,   458,   458,   458,   458,
     458,  -454,   186,   567,   186,   568,  -454,    25,  -454,   429,
     570,  -454,   870,   870,  -454,   584,  -454,   604,   604,   407,
     586,  -454,   113,   151,  -454,   291,   575,  -454,   576,    50,
     270,   585,   593,   595,  -454,   433,   779,   648,   886,   609,
     543,   609,   543,   186,   151,   151,   247,  -454,  -454,  -454,
    -454,   656,   450,   470,   475,   470,   480,   170,  -454,    30,
     666,   599,   680,   627,   684,   685,   607,   514,   886,  -454,
     418,  -454,   375,   407,   654,   737,  -454,   737,   618,   185,
     107,  -454,   192,  -454,   295,   721,   737,  -454,   301,   320,
     320,   691,   692,  -454,  -454,  -454,   687,   320,   320,   886,
    -454,   492,   470,   499,   470,   172,  -454,    30,  -454,  -454,
    -454,  -454,   151,   151,   156,   407,   604,   604,  -454,   604,
     604,   604,   604,  -454,  -454,  -454,  -454,   624,   625,  -454,
    -454,   642,   609,   308,   647,    37,   838,  -454,  -454,   330,
    -454,  -454,   675,   103,  -454,   519,  -454,    30,  -454,    30,
     649,    30,  -454,   694,  -454,   668,  -454,   718,  -454,  -454,
     655,  -454,   720,   737,   737,  -454,  -454,   253,   109,  -454,
     523,  -454,  -454,  -454,   873,    63,  -454,   547,  -454,  -454,
    -454,  -454,  -454,   305,   886,   726,   729,   676,  -454,  -454,
     731,   731,  -454,   708,   736,   688,    30,  -454,    30,   696,
    -454,  -454,  -454,   266,   151,   151,  -454,  -454,  -454,  -454,
     380,   320,   320,  -454,  -454,   320,   320,   170,   698,   739,
     763,   763,   179,  -454,   543,   732,  -454,   186,   671,  -454,
    -454,   599,   710,  -454,   689,  -454,   697,   737,  -454,   766,
     768,   771,   599,   560,   572,  -454,   407,   407,  -454,  -454,
     747,   407,  -454,  -454,   737,  -454,   703,  -454,   207,  -454,
    -454,  -454,   705,   599,   599,   731,   725,  -454,   711,  -454,
    -454,   713,  -454,  -454,   784,   738,   320,   320,   794,   796,
      41,   727,  -454,  -454,  -454,   313,  -454,   745,   744,  -454,
    -454,    99,   609,  -454,   737,  -454,  -454,   886,  -454,  -454,
    -454,  -454,  -454,  -454,   247,  -454,   731,   731,  -454,  -454,
    -454,   886,  -454,   798,  -454,   599,   808,  -454,  -454,  -454,
     809,  -454,  -454,   743,   814,   761,  -454,  -454,  -454,  -454,
     763,  -454,   763,   799,  -454,  -454,   750,  -454,   817,   886,
     599,   823,  -454,  -454,  -454,  -454,  -454,   825,   836,  -454,
     839,  -454,   756,    29,  -454,  -454,   609,   609,   407,   815,
    -454,  -454,  -454,  -454,   841,   599,  -454,  -454,  -454,   842,
    -454,   348,  -454,  -454,   843,  -454,   527,  -454,   850,  -454,
    -454,  -454,  -454,  -454,   407,   793,   407,  -454,   852,  -454,
    -454,   843,  -454,   186,   795,  -454
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,   197,
       0,     0,     0,     0,    11,    17,    10,     0,     0,     0,
       0,    35,    36,     0,     0,    14,    38,   193,     0,   164,
      16,   141,     0,   164,     0,   218,     0,    15,    58,     0,
       0,     0,     0,     7,     0,    12,    21,    13,     0,     0,
      96,    37,    18,     0,     0,   167,   140,    96,     0,   167,
     195,   194,     0,     0,   197,     0,     0,     0,   197,     0,
       0,   233,   197,   228,   217,   198,   201,   214,   215,   216,
     205,   235,   204,     0,   206,   207,   208,   209,   210,   211,
     212,   213,    96,    57,     0,     0,     0,     0,     8,     0,
       0,    33,    34,     0,     0,     0,     0,     0,     0,   312,
     315,   313,   314,   255,     0,   317,   311,   310,     0,   284,
     293,   300,   308,   184,    95,   184,     0,     0,     0,     0,
      53,    56,    54,    55,    52,    78,   106,     0,     0,    62,
       0,   101,   192,   164,     0,     0,     0,   164,   150,     0,
     218,   259,   260,     0,     0,     0,     0,   283,     0,     0,
       0,     0,   218,   218,     0,     0,   229,   230,     0,   254,
       0,     0,   252,     0,     0,     0,     0,     0,    78,     0,
       0,     0,   164,   164,     4,     0,     0,    12,     0,     0,
       0,     0,    20,   318,   294,   295,   320,   324,     0,   322,
       0,   319,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    52,   102,   103,
      63,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      71,     0,     0,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,   166,   193,     0,   169,     0,   180,   142,
       0,   167,    12,     0,   196,     0,     0,     0,   199,   199,
       0,   263,   258,   199,   233,   202,   203,   237,     0,   241,
     242,   197,   197,     0,   253,   245,   234,     0,   257,   246,
     247,   248,   249,    60,     0,    59,   164,   164,   167,   167,
      23,     0,    30,   164,    25,   164,    24,     0,     0,   321,
     316,   251,   285,   286,   287,   288,   289,   290,   291,   292,
     298,   296,   297,   299,   303,   304,   305,   301,   302,   306,
     307,   309,     0,     0,     0,     0,   183,     0,   186,     0,
       0,    97,     0,     0,    52,     0,    49,     0,     0,     0,
      52,   122,     0,   112,   117,     0,     0,    76,     0,    80,
       0,     0,     0,     0,    79,     0,     0,     0,    41,    96,
       0,    96,     0,   130,   104,   105,   100,   109,   110,   108,
     111,     0,     0,   184,     0,   184,     0,     0,   165,     0,
       0,   146,     0,     0,     0,     0,     0,     0,   277,   276,
     218,   281,   282,   271,   261,     0,   236,     0,     0,     0,
       0,   223,     0,   231,     0,     0,     0,   250,     0,   167,
     167,     0,     0,    22,   197,   197,    32,   167,   167,   325,
     323,     0,   184,     0,   184,     0,   182,     0,    69,    50,
      51,    99,   113,   114,     0,     0,     0,     0,   107,     0,
       0,     0,     0,    78,   164,   164,    81,     0,     0,   164,
     164,    90,    96,   255,     0,     0,    41,    40,    67,     0,
      75,    73,     0,   135,   127,     0,   180,     0,   170,     0,
       0,     0,   168,     0,   178,   173,   144,     0,    12,   143,
       0,   151,   180,     0,     0,   199,   200,   274,   265,   269,
       0,   273,   199,   240,   243,     0,   219,   225,   220,   197,
     222,   232,   239,     0,   256,    78,     0,     0,   180,   180,
      28,    29,   197,     0,     0,     0,     0,   190,     0,     0,
     185,   187,   124,     0,   115,   116,   119,   120,   118,   121,
       0,   167,   167,   164,   164,   167,   167,     0,     0,     0,
       0,     0,     0,    43,     0,     0,    72,   130,     0,   125,
     128,   152,   174,   172,     0,   177,     0,     0,   181,     0,
       0,     0,   148,     0,     0,   280,     0,   271,   197,   197,
       0,     0,   199,   262,     0,   199,     0,   224,     0,   238,
      61,   180,     0,   156,   158,    31,     0,    26,     0,   189,
     188,     0,   123,    77,     0,     0,   167,   167,     0,     0,
       0,     0,    82,    47,    45,     0,    44,     0,     0,    64,
     126,     0,    96,    12,     0,   171,   179,   175,   147,   180,
     145,    12,   199,   199,   275,   268,   267,   266,   264,   272,
     270,   244,   227,     0,   221,   160,     0,    12,    12,    19,
       0,   191,    83,     0,     0,     0,    85,    86,    91,    92,
       0,    42,     0,     0,    65,   132,     0,   129,     0,   176,
     154,     0,   279,   278,   199,    12,   180,     0,     0,    27,
       0,    87,     0,     0,    46,    48,    96,    96,   139,     0,
     153,    12,   149,   226,     0,   162,   157,   159,    84,     0,
      89,     0,    70,    66,   134,   137,     0,   132,     0,   161,
      12,    88,    94,    93,   139,     0,   139,   155,     0,   136,
     131,   133,   163,   130,     0,   138
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -454,  -454,  -454,  -454,  -454,   -54,   664,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,   667,   -29,  -263,  -454,  -454,  -454,
     -58,   663,   -36,   -64,  -454,  -454,  -454,  -428,  -454,  -454,
    -454,  -454,  -364,  -454,   288,  -239,  -184,  -197,   133,  -454,
     315,  -454,  -454,   164,  -454,   154,   160,   701,   -30,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,   -38,
     -57,   346,   507,  -395,  -453,  -108,  -454,   460,   -15,  -454,
     -35,  -454,  -454,  -264,  -454,  -140,   268,  -454,  -454,   396,
    -454,   -16,  -454,   -22,  -262,   501,  -454,   -20,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,   328,  -396,   326,  -454,
    -454,  -454,  -454,  -454,   -51,   678,   -53,   806,   -73,  -454,
     610
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    29,     5,     6,    24,     7,   110,
     202,   436,   533,    33,    35,    36,   377,   625,   475,   355,
     361,    47,    48,   147,   674,   697,   383,   245,   374,   558,
     693,   148,   149,   150,   151,   507,   363,   364,   482,   483,
     484,   632,   733,   698,   569,   714,   715,    40,    41,    25,
     498,   641,   272,   633,   701,   657,   658,   685,   720,    65,
     155,   265,   266,   495,   401,   235,   347,   348,    42,    26,
      44,   160,    45,   409,   410,    85,    86,    87,   420,   421,
      88,   125,    90,   126,   288,   289,    92,   127,   297,    94,
      95,    96,    97,   413,   590,   508,   509,   716,   511,    98,
      99,   100,   101,   168,   378,   129,   130,   131,   132,   208,
     209
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,   109,   146,   156,   362,    69,    61,   402,   128,   146,
      66,   103,   159,    54,   386,   411,   479,   510,   481,   414,
     274,    70,   397,    91,   165,    93,   169,   236,   171,    89,
     445,   424,   176,   571,   710,   550,   493,   158,   189,   164,
     203,   213,   562,   170,   146,   211,   397,   175,   303,   582,
     281,   269,   541,    11,   166,   466,   167,   387,   388,   389,
     390,   204,   205,   214,    70,   207,   210,   384,   385,   215,
     216,   217,   218,   219,   220,   603,   604,   214,   146,   146,
     595,   398,   277,   215,   216,   217,   218,   219,   220,   446,
      57,   279,   572,    62,   573,   154,   575,   550,   270,   291,
     292,   563,   221,   668,   711,   494,   304,  -193,   567,  -193,
     433,    63,   517,   474,   587,   258,   221,     8,   108,   271,
     454,    70,   596,   290,   241,   588,   379,   675,   568,   295,
      10,   280,   298,   299,   300,   301,   302,   518,    91,   267,
      93,   609,    58,   610,    89,    63,    71,   519,   655,   589,
      91,    91,    93,    93,   308,   309,    89,    89,   676,   380,
     341,   259,    63,   523,    12,   342,   456,   457,    70,   330,
     331,   332,   333,   452,   453,   254,   255,   458,   259,   260,
     342,   311,    64,   343,   356,   261,   680,   256,   262,   344,
      70,   510,   345,   282,   459,   460,   260,    70,   343,    31,
     628,   391,   261,    32,   344,   262,   461,   345,   257,   456,
     457,   247,    70,    30,   403,   516,   543,   263,   404,   349,
     542,   346,   520,    34,   406,   407,   408,   462,   264,   365,
     375,     1,    37,   705,   248,    27,   249,   654,     2,   250,
      28,   585,   290,   626,   392,   264,   394,    37,   593,   396,
      49,   431,   432,    63,   627,    67,   419,   422,   166,    50,
     412,    37,   546,   547,   548,   549,   439,   207,   429,   430,
     506,   318,   544,   545,    39,   437,   319,   438,   278,    63,
     103,    61,    55,   222,    66,   488,   351,   490,    43,   428,
     223,   224,   251,   252,   449,   450,   467,   623,   624,   111,
     456,   457,   214,   112,   253,   468,   456,   457,   215,   216,
     217,   218,   219,   220,   225,   478,    46,   480,   586,   456,
     457,   146,   146,   146,   146,   476,    51,   441,   650,   443,
     612,   652,    13,    14,   537,    15,   539,   426,    59,   434,
     366,   221,   427,    56,   367,    60,   415,   644,    63,   368,
     161,    16,   415,   416,   162,   463,    52,    17,    63,   521,
      18,   435,   415,    20,   290,   525,   514,   560,   485,   599,
     670,   174,   526,   527,   290,   524,    68,   671,   682,   683,
     534,   535,   267,   154,   226,   227,   228,   564,    91,   369,
      93,   275,   565,   370,    89,   276,   229,   230,    53,   530,
     531,   371,   372,   373,    21,   102,   366,   694,   559,   695,
     613,   104,    22,    23,   146,   368,   178,   231,   179,   722,
     703,    72,   232,   723,   238,   239,   551,   552,   106,    72,
     349,   555,   556,   212,   105,   182,   183,    73,    74,    75,
     285,   286,    76,    77,   579,    73,    74,    75,   107,   152,
      76,    77,   583,   584,    78,   369,   153,    79,   283,   370,
     357,   358,    78,   157,   163,    79,   214,   371,   372,   373,
     359,   -98,   215,   216,   217,   218,   219,   220,   140,   141,
     142,   143,   360,   123,   598,    63,    63,   399,   447,    80,
      63,   -98,   472,    81,   614,   615,   -98,   605,   618,   619,
      82,   284,   172,    83,    84,   221,   146,    63,    82,   487,
     177,    83,    84,   190,   191,   616,   617,   192,   -98,    13,
      14,   193,    15,   -98,   174,   194,   637,   242,   -98,   243,
     -98,   505,    63,   234,   489,   375,   233,    63,    16,   491,
     240,   244,   267,   651,    17,   246,   268,    18,    19,    63,
      20,   536,   485,   646,   647,   214,    63,   293,   538,   664,
     665,   215,   216,   217,   218,   219,   220,   273,   677,   140,
     141,   142,   143,   237,   146,   294,    63,   642,   570,   678,
     591,   296,   592,   679,   591,   305,   725,   681,   306,   643,
     307,    21,   313,   315,   221,   321,   136,   137,   350,    22,
      23,   214,   381,   687,   688,   243,   139,   215,   216,   217,
     218,   219,   220,   214,   140,   141,   142,   143,   144,   215,
     216,   217,   218,   219,   220,   503,   382,   393,   395,   504,
     400,   704,   712,   713,   405,   133,   134,   418,   146,   146,
     221,   425,   442,   444,   135,   448,   451,   718,   214,   455,
     464,   465,   221,   477,   215,   216,   217,   218,   219,   220,
     469,   486,   136,   137,   133,   134,   728,   359,   470,   138,
     471,   496,   139,   135,   497,   140,   141,   142,   143,   360,
     140,   141,   142,   143,   144,   499,   500,   221,   145,   501,
     502,   136,   137,   515,   512,   113,   528,   529,   138,   553,
     554,   139,   532,   557,   561,   566,   114,   115,   574,   140,
     141,   142,   143,   144,   116,   206,   117,   188,   485,   118,
     577,   113,   576,   578,   119,   120,   121,   122,   123,   581,
     580,   600,   114,   115,   601,   602,    70,   113,   606,   124,
     116,   607,   117,   287,   622,   118,   631,   608,   114,   115,
     119,   120,   121,   122,   123,   611,   116,   621,   117,   423,
     629,   118,   634,   113,   635,   124,   119,   120,   121,   122,
     123,   638,   636,   639,   114,   115,   640,   648,   653,   113,
     656,   124,   116,   659,   117,   522,   660,   118,   661,   662,
     114,   115,   119,   120,   121,   122,   123,   663,   116,   666,
     117,   667,   669,   118,   672,   113,   673,   124,   119,   120,
     121,   122,   123,   686,   689,   684,   114,   115,   690,   691,
     692,   113,   700,   124,   116,   699,   376,   696,   702,   118,
     706,   709,   114,   115,   119,   120,   121,   122,   123,   113,
     116,   707,   376,   717,   708,   118,   719,   721,   724,   124,
     119,   120,   121,   122,   473,   727,   730,   732,   116,   735,
     117,   312,   310,   118,   316,   124,   734,   214,   119,   120,
     121,   122,   123,   215,   216,   217,   218,   219,   220,   214,
     731,   726,   630,   124,   729,   215,   216,   217,   218,   219,
     220,   317,   322,   323,   324,   325,   326,   327,   328,   329,
     314,   214,   320,   620,   492,   540,   221,   215,   216,   217,
     218,   219,   220,   597,   214,   645,   513,   649,   221,   417,
     215,   216,   217,   218,   219,   220,  -255,   214,   440,     0,
       0,     0,   594,   215,   216,   217,   218,   219,   220,  -255,
     221,   140,   141,   142,   143,   354,     0,  -255,   173,  -255,
    -255,     0,   174,   221,  -251,     0,  -255,  -255,  -255,  -255,
    -255,     0,  -255,  -255,     0,   174,   221,   180,     0,  -255,
    -255,  -255,  -255,     0,     0,  -251,     0,  -251,  -251,     0,
     425,     0,     0,     0,  -251,  -251,  -251,  -251,   181,     0,
     182,   183,   352,   353,     0,     0,     0,   184,   185,   186,
     187,   195,     0,   196,     0,     0,   197,     0,     0,     0,
     140,   141,   142,   143,   354,     0,     0,     0,     0,   198,
       0,     0,     0,     0,     0,   199,     0,     0,   200,     0,
       0,   201,   334,   335,   336,   337,   338,   339,   340
};

static const yytype_int16 yycheck[] =
{
      15,    55,    60,    67,   243,    43,    35,   270,    59,    67,
      40,    47,    69,    28,   253,   279,   380,   413,   382,   283,
     160,     5,     5,    45,    75,    45,    77,   135,    79,    45,
       5,   293,    83,   486,     5,   463,     6,     5,   102,    74,
     113,     5,     5,    78,   102,   118,     5,    82,     5,   502,
      28,     5,   447,     0,    76,     5,    76,   254,   255,   256,
     257,   114,   115,    41,     5,   116,   117,   251,   252,    47,
      48,    49,    50,    51,    52,   528,   529,    41,   136,   137,
      17,    64,    23,    47,    48,    49,    50,    51,    52,    64,
       5,    17,   487,     5,   489,    63,   491,   525,    52,    83,
      84,    64,    80,    62,    75,    75,    63,    57,     5,    59,
       5,    57,     5,   376,     5,   153,    80,    75,    64,   157,
     359,     5,    59,   174,   139,    16,    28,    28,    25,   180,
      75,    57,   183,   184,   185,   186,   187,    30,   160,   154,
     160,   536,    57,   538,   160,    57,    30,    40,   601,    40,
     172,   173,   172,   173,   192,   193,   172,   173,    59,    61,
     233,     8,    57,   425,    58,     8,    53,    54,     5,   222,
     223,   224,   225,   357,   358,    43,    44,    64,     8,    26,
       8,   196,    58,    26,   242,    32,   639,    55,    35,    32,
       5,   587,    35,    30,    43,    44,    26,     5,    26,    71,
     564,   258,    32,    75,    32,    35,    55,    35,    76,    53,
      54,     6,     5,     5,   271,    30,   455,    64,   272,   234,
      64,    64,    30,    75,   275,   276,   277,    76,    75,   244,
     245,     4,    75,   686,    29,    58,    31,    30,    11,    34,
      63,   505,   293,    64,   259,    75,   261,    75,   512,   264,
      26,   308,   309,    57,    75,    59,   291,   292,   280,    35,
     280,    75,   459,   460,   461,   462,   317,   318,   306,   307,
     410,    57,   456,   457,    75,   313,    62,   315,    17,    57,
     316,   310,     5,    46,   314,   393,    64,   395,    75,   304,
      53,    54,    53,    54,   352,   353,    26,   560,   561,    71,
      53,    54,    41,    75,    65,    35,    53,    54,    47,    48,
      49,    50,    51,    52,    77,   379,    75,   381,    65,    53,
      54,   379,   380,   381,   382,   376,    75,   342,   592,   344,
      64,   595,     7,     8,   442,    10,   444,    57,    52,    14,
      26,    80,    62,    12,    30,    59,    57,   586,    57,    35,
      71,    26,    57,    64,    75,    64,    75,    32,    57,    64,
      35,    36,    57,    38,   415,    64,   417,    59,   383,    64,
      57,    63,   429,   430,   425,   426,    58,    64,   642,   643,
     437,   438,   397,    63,    43,    44,    45,    57,   410,    75,
     410,    37,    62,    79,   410,    41,    55,    56,    75,   434,
     435,    87,    88,    89,    79,    52,    26,   670,   472,   672,
      30,    75,    87,    88,   472,    35,    58,    76,    60,    71,
     684,     3,    81,    75,   136,   137,   464,   465,    58,     3,
     445,   469,   470,    58,    75,    60,    61,    19,    20,    21,
     172,   173,    24,    25,   498,    19,    20,    21,    58,    75,
      24,    25,   503,   504,    36,    75,    75,    39,    33,    79,
      53,    54,    36,    75,    75,    39,    41,    87,    88,    89,
      63,     5,    47,    48,    49,    50,    51,    52,    71,    72,
      73,    74,    75,    75,   519,    57,    57,    59,    59,    71,
      57,    25,    59,    75,   551,   552,    30,   532,   555,   556,
      82,    75,    59,    85,    86,    80,   564,    57,    82,    59,
      75,    85,    86,    58,    58,   553,   554,    75,    52,     7,
       8,    75,    10,    57,    63,    58,   577,    61,    62,    63,
      64,    17,    57,    63,    59,   550,    78,    57,    26,    59,
      75,    63,   557,   594,    32,    52,     5,    35,    36,    57,
      38,    59,   567,   588,   589,    41,    57,    63,    59,   616,
     617,    47,    48,    49,    50,    51,    52,    59,   632,    71,
      72,    73,    74,    75,   632,    75,    57,    17,    59,   633,
      57,    75,    59,   634,    57,     5,    59,   641,    75,    17,
      75,    79,    75,    75,    80,    75,    53,    54,    59,    87,
      88,    41,    28,   657,   658,    63,    63,    47,    48,    49,
      50,    51,    52,    41,    71,    72,    73,    74,    75,    47,
      48,    49,    50,    51,    52,    18,    28,    75,    75,    22,
       9,   685,   696,   697,    75,    26,    27,    90,   696,   697,
      80,    63,    75,    75,    35,    75,    62,   701,    41,    63,
      75,    75,    80,     5,    47,    48,    49,    50,    51,    52,
      75,     5,    53,    54,    26,    27,   720,    63,    75,    60,
      75,     5,    63,    35,    75,    71,    72,    73,    74,    75,
      71,    72,    73,    74,    75,     5,    59,    80,    79,     5,
       5,    53,    54,    75,    40,    42,     5,     5,    60,    75,
      75,    63,    15,    61,    57,    30,    53,    54,    59,    71,
      72,    73,    74,    75,    61,    62,    63,    79,   733,    66,
      52,    42,    28,     5,    71,    72,    73,    74,    75,     9,
      75,     5,    53,    54,     5,    59,     5,    42,    30,    86,
      61,     5,    63,    64,     5,    66,    75,    59,    53,    54,
      71,    72,    73,    74,    75,    59,    61,    59,    63,    64,
      28,    66,    52,    42,    75,    86,    71,    72,    73,    74,
      75,     5,    75,     5,    53,    54,     5,    30,    75,    42,
      75,    86,    61,    58,    63,    64,    75,    66,    75,     5,
      53,    54,    71,    72,    73,    74,    75,    59,    61,     5,
      63,     5,    75,    66,    59,    42,    62,    86,    71,    72,
      73,    74,    75,     5,     5,    17,    53,    54,    75,     5,
      59,    42,     5,    86,    61,    75,    63,    28,     5,    66,
       5,    75,    53,    54,    71,    72,    73,    74,    75,    42,
      61,     5,    63,    28,     5,    66,     5,     5,     5,    86,
      71,    72,    73,    74,    75,     5,    63,     5,    61,    64,
      63,   197,   195,    66,   201,    86,   733,    41,    71,    72,
      73,    74,    75,    47,    48,    49,    50,    51,    52,    41,
     726,   717,   567,    86,   724,    47,    48,    49,    50,    51,
      52,    65,   214,   215,   216,   217,   218,   219,   220,   221,
     199,    41,    64,   557,   397,   445,    80,    47,    48,    49,
      50,    51,    52,   517,    41,   587,   415,   591,    80,    59,
      47,    48,    49,    50,    51,    52,    37,    41,   318,    -1,
      -1,    -1,    59,    47,    48,    49,    50,    51,    52,    37,
      80,    71,    72,    73,    74,    75,    -1,    58,    59,    60,
      61,    -1,    63,    80,    37,    -1,    67,    68,    69,    70,
      58,    -1,    60,    61,    -1,    63,    80,    37,    -1,    67,
      68,    69,    70,    -1,    -1,    58,    -1,    60,    61,    -1,
      63,    -1,    -1,    -1,    67,    68,    69,    70,    58,    -1,
      60,    61,    53,    54,    -1,    -1,    -1,    67,    68,    69,
      70,     8,    -1,    10,    -1,    -1,    13,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,   226,   227,   228,   229,   230,   231,   232
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
      75,   159,    61,    63,    63,   118,    52,     6,    29,    31,
      34,    53,    54,    65,    43,    44,    55,    76,   150,     8,
      26,    32,    35,    64,    75,   152,   153,   159,     5,     5,
      52,   150,   143,    59,   166,    37,    41,    23,    17,    17,
      57,    28,    30,    33,    75,   167,   167,    64,   175,   176,
     195,    83,    84,    63,    75,   195,    75,   179,   195,   195,
     195,   195,   195,     5,    63,     5,    75,    75,   150,   150,
     105,   159,    97,    75,   138,    75,   112,    65,    57,    62,
      64,    75,   196,   196,   196,   196,   196,   196,   196,   196,
     197,   197,   197,   197,   198,   198,   198,   198,   198,   198,
     198,   199,     8,    26,    32,    35,    64,   157,   158,   159,
      59,    64,    53,    54,    75,   110,   111,    53,    54,    63,
      75,   111,   126,   127,   128,   159,    26,    30,    35,    75,
      79,    87,    88,    89,   119,   159,    63,   107,   195,    28,
      61,    28,    28,   117,   127,   127,   126,   128,   128,   128,
     128,   151,   159,    75,   159,    75,   159,     5,    64,    59,
       9,   155,   107,   151,    96,    75,   195,   195,   195,   164,
     165,   164,   178,   184,   164,    57,    64,    59,    90,   161,
     169,   170,   161,    64,   175,    63,    57,    62,   159,   150,
     150,   151,   151,     5,    14,    36,   102,   150,   150,   195,
     201,   159,    75,   159,    75,     5,    64,    59,    75,   111,
     111,    62,   127,   127,   126,    63,    53,    54,    64,    43,
      44,    55,    76,    64,    75,    75,     5,    26,    35,    75,
      75,    75,    59,    75,   107,   109,   195,     5,   114,   123,
     114,   123,   129,   130,   131,   159,     5,    59,   156,    59,
     156,    59,   153,     6,    75,   154,     5,    75,   141,     5,
      59,     5,     5,    18,    22,    17,   166,   126,   186,   187,
     188,   189,    40,   176,   195,    75,    30,     5,    30,    40,
      30,    64,    64,   175,   195,    64,   151,   151,     5,     5,
     161,   161,    15,   103,   151,   151,    59,   156,    59,   156,
     158,   154,    64,   126,   127,   127,   128,   128,   128,   128,
     118,   150,   150,    75,    75,   150,   150,    61,   120,   114,
      59,    57,     5,    64,    57,    62,    30,     5,    25,   135,
      59,   155,   154,   154,    59,   154,    28,    52,     5,    96,
      75,     9,   155,   195,   195,   164,    65,     5,    16,    40,
     185,    57,    59,   164,    59,    17,    59,   170,   161,    64,
       5,     5,    59,   155,   155,   161,    30,     5,    59,   154,
     154,    59,    64,    30,   151,   151,   150,   150,   151,   151,
     152,    59,     5,   107,   107,   108,    64,    75,   123,    28,
     131,    75,   132,   144,    52,    75,    75,   195,     5,     5,
       5,   142,    17,    17,   126,   187,   161,   161,    30,   189,
     164,   195,   164,    75,    30,   155,    75,   146,   147,    58,
      75,    75,     5,    59,   151,   151,     5,     5,    62,    75,
      57,    64,    59,    62,   115,    28,    59,   114,    96,   195,
     155,    96,   164,   164,    17,   148,     5,    96,    96,     5,
      75,     5,    59,   121,   107,   107,    28,   116,   134,    75,
       5,   145,     5,   164,    96,   155,     5,     5,     5,    75,
       5,    75,   114,   114,   136,   137,   188,    28,    96,     5,
     149,     5,    71,    75,     5,    59,   134,     5,    96,   137,
      63,   136,     5,   133,   129,    64
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
     125,   125,   126,   126,   126,   126,   126,   127,   127,   127,
     127,   127,   128,   128,   128,   129,   130,   130,   132,   131,
     131,   133,   134,   135,   135,   135,   136,   136,   137,   137,
     138,   138,   139,   139,   140,   140,   141,   140,   142,   140,
     143,   140,   144,   140,   145,   140,   146,   140,   147,   140,
     148,   140,   149,   140,   150,   151,   151,   151,   152,   152,
     153,   153,   153,   153,   153,   153,   153,   153,   154,   154,
     155,   155,   156,   156,   156,   157,   157,   158,   158,   158,
     158,   158,   159,   159,   160,   162,   161,   163,   161,   165,
     164,   166,   166,   166,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   168,
     168,   168,   168,   169,   169,   169,   170,   170,   171,   171,
     172,   172,   172,   173,   173,   173,   174,   174,   174,   174,
     175,   175,   176,   176,   176,   177,   177,   177,   177,   177,
     178,   178,   178,   178,   178,   178,   179,   179,   180,   181,
     181,   182,   182,   184,   183,   185,   185,   185,   186,   186,
     187,   187,   188,   188,   189,   189,   190,   191,   192,   192,
     192,   193,   194,   194,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   196,   196,   196,   196,   196,   196,   196,
     197,   197,   197,   197,   197,   197,   197,   197,   198,   198,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   200,   200,   201,   201
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
       3,     1,     2,     2,     3,     3,     1,     4,     3,     3,
       3,     3,     1,     2,     2,     3,     3,     1,     3,     3,
       3,     3,     1,     4,     3,     2,     3,     1,     0,     4,
       0,     0,     0,     7,     5,     0,     3,     1,     6,     0,
       2,     1,     4,     6,     7,     9,     0,     9,     0,    11,
       0,     7,     0,    11,     0,    13,     0,    11,     0,    11,
       0,    12,     0,    14,     0,     3,     2,     0,     3,     1,
       3,     5,     4,     3,     4,     5,     6,     4,     1,     3,
       0,     3,     3,     2,     0,     3,     1,     3,     4,     4,
       3,     5,     3,     1,     3,     0,     4,     0,     2,     0,
       2,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       5,     7,     5,     1,     3,     2,     6,     4,     1,     2,
       2,     4,     5,     1,     3,     1,     4,     3,     6,     5,
       3,     1,     1,     3,     5,     3,     3,     3,     3,     3,
       4,     3,     2,     3,     2,     1,     3,     1,     3,     2,
       2,     4,     6,     0,     7,     0,     2,     2,     3,     1,
       3,     0,     3,     1,     1,     3,     4,     4,     8,     8,
       6,     4,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     2,     2,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     2,     2,
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
#line 204 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1986 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 205 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1992 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 208 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1998 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 209 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2004 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 212 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2010 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 213 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2016 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 216 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2022 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 217 "pascal.y"
     { (yyval.node) = NULL; }
#line 2028 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 220 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2034 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 223 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2040 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 224 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2046 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 227 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2052 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 228 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2058 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 229 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2064 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 230 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2070 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 231 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2076 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 232 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2082 "pascal.tab.c"
    break;

  case 19: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 235 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2088 "pascal.tab.c"
    break;

  case 20: /* intf_part_list: intf_part_list intf_part  */
#line 238 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2094 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: %empty  */
#line 239 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2100 "pascal.tab.c"
    break;

  case 22: /* intf_part: USESSY id_list SEMICOLON  */
#line 242 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2106 "pascal.tab.c"
    break;

  case 23: /* intf_part: CONSTSY const_decl_list  */
#line 243 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2112 "pascal.tab.c"
    break;

  case 24: /* intf_part: TYPESY type_decl_list  */
#line 244 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2118 "pascal.tab.c"
    break;

  case 25: /* intf_part: VARSY var_decl_list  */
#line 245 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2124 "pascal.tab.c"
    break;

  case 26: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 246 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2130 "pascal.tab.c"
    break;

  case 27: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 247 "pascal.y"
                                                                       { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2136 "pascal.tab.c"
    break;

  case 28: /* init_opt: INITIALIZATIONSY statement_list  */
#line 250 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2142 "pascal.tab.c"
    break;

  case 29: /* init_opt: BEGINSY statement_list  */
#line 251 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2148 "pascal.tab.c"
    break;

  case 30: /* init_opt: %empty  */
#line 252 "pascal.y"
     { (yyval.node) = NULL; }
#line 2154 "pascal.tab.c"
    break;

  case 31: /* fini_opt: FINALIZATIONSY statement_list  */
#line 255 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2160 "pascal.tab.c"
    break;

  case 32: /* fini_opt: %empty  */
#line 256 "pascal.y"
     { (yyval.node) = NULL; }
#line 2166 "pascal.tab.c"
    break;

  case 33: /* label_list: label_list COMMA INTCONST  */
#line 259 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2172 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA IDENT  */
#line 260 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2178 "pascal.tab.c"
    break;

  case 35: /* label_list: INTCONST  */
#line 261 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2184 "pascal.tab.c"
    break;

  case 36: /* label_list: IDENT  */
#line 262 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2190 "pascal.tab.c"
    break;

  case 37: /* const_decl_list: const_decl_list const_decl  */
#line 265 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2196 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl  */
#line 266 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2202 "pascal.tab.c"
    break;

  case 39: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 269 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2208 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 270 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2214 "pascal.tab.c"
    break;

  case 41: /* typed_value: expression  */
#line 273 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2220 "pascal.tab.c"
    break;

  case 42: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 274 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2226 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT field_value_list RPARENT  */
#line 275 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2232 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 276 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2238 "pascal.tab.c"
    break;

  case 45: /* typed_value_list: typed_value  */
#line 279 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2244 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 280 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2250 "pascal.tab.c"
    break;

  case 47: /* field_value_list: IDENT COLON typed_value  */
#line 283 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2256 "pascal.tab.c"
    break;

  case 48: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 284 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2262 "pascal.tab.c"
    break;

  case 49: /* constant: scalar_constant  */
#line 287 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2268 "pascal.tab.c"
    break;

  case 50: /* constant: PLUS scalar_constant  */
#line 288 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2274 "pascal.tab.c"
    break;

  case 51: /* constant: MINUS scalar_constant  */
#line 289 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2280 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: IDENT  */
#line 292 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2286 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: INTCONST  */
#line 293 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2292 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: REALCONST  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2298 "pascal.tab.c"
    break;

  case 55: /* scalar_constant: STRINGCONST  */
#line 295 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2304 "pascal.tab.c"
    break;

  case 56: /* scalar_constant: CHARCODE  */
#line 296 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2310 "pascal.tab.c"
    break;

  case 57: /* type_decl_list: type_decl_list type_decl  */
#line 299 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2316 "pascal.tab.c"
    break;

  case 58: /* type_decl_list: type_decl  */
#line 300 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2322 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 303 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2328 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 304 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2334 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 305 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2340 "pascal.tab.c"
    break;

  case 62: /* type: simple_type  */
#line 308 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2346 "pascal.tab.c"
    break;

  case 63: /* type: ARROW IDENT  */
#line 309 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2352 "pascal.tab.c"
    break;

  case 64: /* $@1: %empty  */
#line 310 "pascal.y"
                                                       { }
#line 2358 "pascal.tab.c"
    break;

  case 65: /* $@2: %empty  */
#line 310 "pascal.y"
                                                           { }
#line 2364 "pascal.tab.c"
    break;

  case 66: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 310 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2370 "pascal.tab.c"
    break;

  case 67: /* type: packed_opt ARRAYSY OFSY type  */
#line 311 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2376 "pascal.tab.c"
    break;

  case 68: /* type: PROCEDURESY pf_params  */
#line 312 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2382 "pascal.tab.c"
    break;

  case 69: /* type: FUNCTIONSY pf_params COLON IDENT  */
#line 313 "pascal.y"
                                      { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2388 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 314 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2394 "pascal.tab.c"
    break;

  case 71: /* $@3: %empty  */
#line 315 "pascal.y"
                         { }
#line 2400 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 315 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2406 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt SETSY OFSY simple_type  */
#line 316 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2412 "pascal.tab.c"
    break;

  case 74: /* type: packed_opt FILESY  */
#line 317 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2418 "pascal.tab.c"
    break;

  case 75: /* type: packed_opt FILESY OFSY type  */
#line 318 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2424 "pascal.tab.c"
    break;

  case 76: /* type: CLASSSY class_members ENDSY  */
#line 319 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2430 "pascal.tab.c"
    break;

  case 77: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 320 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2436 "pascal.tab.c"
    break;

  case 78: /* class_members: %empty  */
#line 323 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2442 "pascal.tab.c"
    break;

  case 79: /* class_members: class_members class_member  */
#line 324 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2448 "pascal.tab.c"
    break;

  case 80: /* class_member: IDENT  */
#line 327 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2454 "pascal.tab.c"
    break;

  case 81: /* class_member: IDENT SEMICOLON  */
#line 328 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2460 "pascal.tab.c"
    break;

  case 82: /* class_member: id_list COLON type SEMICOLON  */
#line 329 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2466 "pascal.tab.c"
    break;

  case 83: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 330 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2472 "pascal.tab.c"
    break;

  case 84: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 331 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2478 "pascal.tab.c"
    break;

  case 85: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 332 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2484 "pascal.tab.c"
    break;

  case 86: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 333 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2490 "pascal.tab.c"
    break;

  case 87: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 334 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2496 "pascal.tab.c"
    break;

  case 88: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON  */
#line 335 "pascal.y"
                                                                               { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2502 "pascal.tab.c"
    break;

  case 89: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 336 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2508 "pascal.tab.c"
    break;

  case 90: /* prop_index_opt: %empty  */
#line 339 "pascal.y"
    { (yyval.node) = NULL; }
#line 2514 "pascal.tab.c"
    break;

  case 91: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 340 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2520 "pascal.tab.c"
    break;

  case 92: /* prop_specs: %empty  */
#line 343 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2526 "pascal.tab.c"
    break;

  case 93: /* prop_specs: prop_specs IDENT IDENT  */
#line 344 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2532 "pascal.tab.c"
    break;

  case 94: /* prop_specs: prop_specs IDENT INTCONST  */
#line 345 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2538 "pascal.tab.c"
    break;

  case 95: /* packed_opt: PACKEDSY  */
#line 348 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2544 "pascal.tab.c"
    break;

  case 96: /* packed_opt: %empty  */
#line 349 "pascal.y"
     { (yyval.node) = NULL; }
#line 2550 "pascal.tab.c"
    break;

  case 97: /* simple_type: LPARENT id_list RPARENT  */
#line 352 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2556 "pascal.tab.c"
    break;

  case 98: /* simple_type: IDENT  */
#line 353 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2562 "pascal.tab.c"
    break;

  case 99: /* simple_type: IDENT LBRACK constant RBRACK  */
#line 354 "pascal.y"
                                  { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2568 "pascal.tab.c"
    break;

  case 100: /* simple_type: lo_bound DOTDOT b_expr  */
#line 355 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2574 "pascal.tab.c"
    break;

  case 101: /* lo_bound: lo_term  */
#line 358 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2580 "pascal.tab.c"
    break;

  case 102: /* lo_bound: PLUS lo_term  */
#line 359 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2586 "pascal.tab.c"
    break;

  case 103: /* lo_bound: MINUS lo_term  */
#line 360 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2592 "pascal.tab.c"
    break;

  case 104: /* lo_bound: lo_bound PLUS b_term  */
#line 361 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2598 "pascal.tab.c"
    break;

  case 105: /* lo_bound: lo_bound MINUS b_term  */
#line 362 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2604 "pascal.tab.c"
    break;

  case 106: /* lo_term: scalar_constant  */
#line 365 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2610 "pascal.tab.c"
    break;

  case 107: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 366 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2616 "pascal.tab.c"
    break;

  case 108: /* lo_term: lo_term MUL b_factor  */
#line 367 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2622 "pascal.tab.c"
    break;

  case 109: /* lo_term: lo_term IDIV b_factor  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2628 "pascal.tab.c"
    break;

  case 110: /* lo_term: lo_term IMOD b_factor  */
#line 369 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2634 "pascal.tab.c"
    break;

  case 111: /* lo_term: lo_term SHIFTOP b_factor  */
#line 370 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2640 "pascal.tab.c"
    break;

  case 112: /* b_expr: b_term  */
#line 373 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2646 "pascal.tab.c"
    break;

  case 113: /* b_expr: PLUS b_term  */
#line 374 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2652 "pascal.tab.c"
    break;

  case 114: /* b_expr: MINUS b_term  */
#line 375 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2658 "pascal.tab.c"
    break;

  case 115: /* b_expr: b_expr PLUS b_term  */
#line 376 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2664 "pascal.tab.c"
    break;

  case 116: /* b_expr: b_expr MINUS b_term  */
#line 377 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2670 "pascal.tab.c"
    break;

  case 117: /* b_term: b_factor  */
#line 380 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2676 "pascal.tab.c"
    break;

  case 118: /* b_term: b_term MUL b_factor  */
#line 381 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2682 "pascal.tab.c"
    break;

  case 119: /* b_term: b_term IDIV b_factor  */
#line 382 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2688 "pascal.tab.c"
    break;

  case 120: /* b_term: b_term IMOD b_factor  */
#line 383 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2694 "pascal.tab.c"
    break;

  case 121: /* b_term: b_term SHIFTOP b_factor  */
#line 384 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2700 "pascal.tab.c"
    break;

  case 122: /* b_factor: scalar_constant  */
#line 387 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2706 "pascal.tab.c"
    break;

  case 123: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 388 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2712 "pascal.tab.c"
    break;

  case 124: /* b_factor: LPARENT b_expr RPARENT  */
#line 389 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2718 "pascal.tab.c"
    break;

  case 125: /* record_body: record_field_list record_case_opt  */
#line 392 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2724 "pascal.tab.c"
    break;

  case 126: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 395 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2730 "pascal.tab.c"
    break;

  case 127: /* record_field_list: record_field  */
#line 396 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2736 "pascal.tab.c"
    break;

  case 128: /* $@4: %empty  */
#line 399 "pascal.y"
                  { }
#line 2742 "pascal.tab.c"
    break;

  case 129: /* record_field: id_list COLON $@4 type  */
#line 399 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2748 "pascal.tab.c"
    break;

  case 130: /* record_field: %empty  */
#line 400 "pascal.y"
     { (yyval.node) = NULL; }
#line 2754 "pascal.tab.c"
    break;

  case 131: /* case_arm_mark: %empty  */
#line 403 "pascal.y"
    { }
#line 2760 "pascal.tab.c"
    break;

  case 132: /* case_open: %empty  */
#line 406 "pascal.y"
    { }
#line 2766 "pascal.tab.c"
    break;

  case 133: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 409 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2772 "pascal.tab.c"
    break;

  case 134: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 410 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2778 "pascal.tab.c"
    break;

  case 135: /* record_case_opt: %empty  */
#line 411 "pascal.y"
     { (yyval.node) = NULL; }
#line 2784 "pascal.tab.c"
    break;

  case 136: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 414 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2790 "pascal.tab.c"
    break;

  case 137: /* record_case_list: record_case_arm  */
#line 415 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2796 "pascal.tab.c"
    break;

  case 138: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 418 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2802 "pascal.tab.c"
    break;

  case 139: /* record_case_arm: %empty  */
#line 419 "pascal.y"
     { (yyval.node) = NULL; }
#line 2808 "pascal.tab.c"
    break;

  case 140: /* var_decl_list: var_decl_list var_decl  */
#line 422 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2814 "pascal.tab.c"
    break;

  case 141: /* var_decl_list: var_decl  */
#line 423 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2820 "pascal.tab.c"
    break;

  case 142: /* var_decl: id_list COLON type SEMICOLON  */
#line 426 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2826 "pascal.tab.c"
    break;

  case 143: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 427 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2832 "pascal.tab.c"
    break;

  case 144: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 430 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2838 "pascal.tab.c"
    break;

  case 145: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 431 "pascal.y"
                                                                                           { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2844 "pascal.tab.c"
    break;

  case 146: /* $@5: %empty  */
#line 432 "pascal.y"
                                                                               { }
#line 2850 "pascal.tab.c"
    break;

  case 147: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 432 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2856 "pascal.tab.c"
    break;

  case 148: /* $@6: %empty  */
#line 433 "pascal.y"
                                                                                          { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2862 "pascal.tab.c"
    break;

  case 149: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 433 "pascal.y"
                                                                                                                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2868 "pascal.tab.c"
    break;

  case 150: /* $@7: %empty  */
#line 434 "pascal.y"
                                        { }
#line 2874 "pascal.tab.c"
    break;

  case 151: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 434 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2880 "pascal.tab.c"
    break;

  case 152: /* $@8: %empty  */
#line 435 "pascal.y"
                                                                                            { }
#line 2886 "pascal.tab.c"
    break;

  case 153: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 435 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2892 "pascal.tab.c"
    break;

  case 154: /* $@9: %empty  */
#line 436 "pascal.y"
                                                                                                       { }
#line 2898 "pascal.tab.c"
    break;

  case 155: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 436 "pascal.y"
                                                                                                                           {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 2906 "pascal.tab.c"
    break;

  case 156: /* $@10: %empty  */
#line 439 "pascal.y"
                                                                                              { }
#line 2912 "pascal.tab.c"
    break;

  case 157: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 439 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 2920 "pascal.tab.c"
    break;

  case 158: /* $@11: %empty  */
#line 442 "pascal.y"
                                                                                             { }
#line 2926 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 442 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 2934 "pascal.tab.c"
    break;

  case 160: /* $@12: %empty  */
#line 445 "pascal.y"
                                                                                                    { }
#line 2940 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 445 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 2948 "pascal.tab.c"
    break;

  case 162: /* $@13: %empty  */
#line 448 "pascal.y"
                                                                                                               { }
#line 2954 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 448 "pascal.y"
                                                                                                                                   {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 2962 "pascal.tab.c"
    break;

  case 164: /* pv_mark: %empty  */
#line 453 "pascal.y"
    { }
#line 2968 "pascal.tab.c"
    break;

  case 165: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 456 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2974 "pascal.tab.c"
    break;

  case 166: /* parameter_list_opt: LPARENT RPARENT  */
#line 457 "pascal.y"
                     { (yyval.node) = NULL; }
#line 2980 "pascal.tab.c"
    break;

  case 167: /* parameter_list_opt: %empty  */
#line 458 "pascal.y"
     { (yyval.node) = NULL; }
#line 2986 "pascal.tab.c"
    break;

  case 168: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 461 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2992 "pascal.tab.c"
    break;

  case 169: /* parameter_decl_list: parameter_decl  */
#line 462 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2998 "pascal.tab.c"
    break;

  case 170: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 465 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3004 "pascal.tab.c"
    break;

  case 171: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 466 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3010 "pascal.tab.c"
    break;

  case 172: /* parameter_decl: VARSY id_list COLON param_type  */
#line 467 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3016 "pascal.tab.c"
    break;

  case 173: /* parameter_decl: id_list COLON param_type  */
#line 468 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3022 "pascal.tab.c"
    break;

  case 174: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 469 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3028 "pascal.tab.c"
    break;

  case 175: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 470 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3034 "pascal.tab.c"
    break;

  case 176: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 471 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3040 "pascal.tab.c"
    break;

  case 177: /* parameter_decl: IDENT id_list COLON param_type  */
#line 472 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3046 "pascal.tab.c"
    break;

  case 178: /* param_type: IDENT  */
#line 475 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3052 "pascal.tab.c"
    break;

  case 179: /* param_type: ARRAYSY OFSY IDENT  */
#line 476 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3058 "pascal.tab.c"
    break;

  case 180: /* routine_directives: %empty  */
#line 479 "pascal.y"
    { (yyval.node) = NULL; }
#line 3064 "pascal.tab.c"
    break;

  case 181: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 480 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3077 "pascal.tab.c"
    break;

  case 182: /* pf_params: LPARENT pf_sections RPARENT  */
#line 490 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3083 "pascal.tab.c"
    break;

  case 183: /* pf_params: LPARENT RPARENT  */
#line 491 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3089 "pascal.tab.c"
    break;

  case 184: /* pf_params: %empty  */
#line 492 "pascal.y"
     { (yyval.node) = NULL; }
#line 3095 "pascal.tab.c"
    break;

  case 185: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 495 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 186: /* pf_sections: pf_section  */
#line 496 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 187: /* pf_section: id_list COLON param_type  */
#line 499 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3113 "pascal.tab.c"
    break;

  case 188: /* pf_section: VARSY id_list COLON param_type  */
#line 500 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3119 "pascal.tab.c"
    break;

  case 189: /* pf_section: CONSTSY id_list COLON param_type  */
#line 501 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3125 "pascal.tab.c"
    break;

  case 190: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 502 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3131 "pascal.tab.c"
    break;

  case 191: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 503 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3137 "pascal.tab.c"
    break;

  case 192: /* id_list: id_list COMMA IDENT  */
#line 506 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 193: /* id_list: IDENT  */
#line 507 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 194: /* body: BEGINSY statement_list ENDSY  */
#line 510 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3155 "pascal.tab.c"
    break;

  case 195: /* $@14: %empty  */
#line 513 "pascal.y"
                             { pas_stmt_mark(); }
#line 3161 "pascal.tab.c"
    break;

  case 196: /* statement_list: statement_list SEMICOLON $@14 statement  */
#line 513 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 197: /* $@15: %empty  */
#line 514 "pascal.y"
     { pas_stmt_mark(); }
#line 3173 "pascal.tab.c"
    break;

  case 198: /* statement_list: $@15 statement  */
#line 514 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 199: /* $@16: %empty  */
#line 517 "pascal.y"
    { pas_stmt_mark(); }
#line 3185 "pascal.tab.c"
    break;

  case 200: /* body_stmt: $@16 statement  */
#line 517 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3191 "pascal.tab.c"
    break;

  case 201: /* statement: statement_no_label  */
#line 520 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3197 "pascal.tab.c"
    break;

  case 202: /* statement: INTCONST COLON statement_no_label  */
#line 521 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3203 "pascal.tab.c"
    break;

  case 203: /* statement: IDENT COLON statement_no_label  */
#line 522 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 204: /* statement_no_label: assignment  */
#line 525 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3215 "pascal.tab.c"
    break;

  case 205: /* statement_no_label: call  */
#line 526 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3221 "pascal.tab.c"
    break;

  case 206: /* statement_no_label: compound_statement  */
#line 527 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3227 "pascal.tab.c"
    break;

  case 207: /* statement_no_label: goto_statement  */
#line 528 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3233 "pascal.tab.c"
    break;

  case 208: /* statement_no_label: if_statement  */
#line 529 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3239 "pascal.tab.c"
    break;

  case 209: /* statement_no_label: case_statement  */
#line 530 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3245 "pascal.tab.c"
    break;

  case 210: /* statement_no_label: while_statement  */
#line 531 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3251 "pascal.tab.c"
    break;

  case 211: /* statement_no_label: repeat_statement  */
#line 532 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3257 "pascal.tab.c"
    break;

  case 212: /* statement_no_label: for_statement  */
#line 533 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3263 "pascal.tab.c"
    break;

  case 213: /* statement_no_label: with_statement  */
#line 534 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3269 "pascal.tab.c"
    break;

  case 214: /* statement_no_label: try_statement  */
#line 535 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3275 "pascal.tab.c"
    break;

  case 215: /* statement_no_label: raise_statement  */
#line 536 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3281 "pascal.tab.c"
    break;

  case 216: /* statement_no_label: inherited_call  */
#line 537 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3287 "pascal.tab.c"
    break;

  case 217: /* statement_no_label: INHERITEDSY  */
#line 538 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3293 "pascal.tab.c"
    break;

  case 218: /* statement_no_label: %empty  */
#line 539 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3299 "pascal.tab.c"
    break;

  case 219: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 542 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3305 "pascal.tab.c"
    break;

  case 220: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 543 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3311 "pascal.tab.c"
    break;

  case 221: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 544 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3317 "pascal.tab.c"
    break;

  case 222: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 545 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3323 "pascal.tab.c"
    break;

  case 223: /* handler_list: handler  */
#line 548 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3329 "pascal.tab.c"
    break;

  case 224: /* handler_list: handler_list SEMICOLON handler  */
#line 549 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3335 "pascal.tab.c"
    break;

  case 225: /* handler_list: handler_list SEMICOLON  */
#line 550 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3341 "pascal.tab.c"
    break;

  case 226: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 553 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3347 "pascal.tab.c"
    break;

  case 227: /* handler: ONSY IDENT DOSY body_stmt  */
#line 554 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3353 "pascal.tab.c"
    break;

  case 228: /* raise_statement: RAISESY  */
#line 557 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3359 "pascal.tab.c"
    break;

  case 229: /* raise_statement: RAISESY expression  */
#line 558 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 230: /* inherited_call: INHERITEDSY IDENT  */
#line 561 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3371 "pascal.tab.c"
    break;

  case 231: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 562 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3377 "pascal.tab.c"
    break;

  case 232: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 563 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3383 "pascal.tab.c"
    break;

  case 233: /* call: IDENT  */
#line 566 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3389 "pascal.tab.c"
    break;

  case 234: /* call: selector PERIOD IDENT  */
#line 567 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3395 "pascal.tab.c"
    break;

  case 235: /* call: call_with_args  */
#line 568 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3401 "pascal.tab.c"
    break;

  case 236: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 571 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3407 "pascal.tab.c"
    break;

  case 237: /* call_with_args: IDENT LPARENT RPARENT  */
#line 572 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3413 "pascal.tab.c"
    break;

  case 238: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 573 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3419 "pascal.tab.c"
    break;

  case 239: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 574 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3425 "pascal.tab.c"
    break;

  case 240: /* argument_list: argument_list COMMA argument  */
#line 577 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3431 "pascal.tab.c"
    break;

  case 241: /* argument_list: argument  */
#line 578 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3437 "pascal.tab.c"
    break;

  case 242: /* argument: expression  */
#line 581 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3443 "pascal.tab.c"
    break;

  case 243: /* argument: expression COLON expression  */
#line 582 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3449 "pascal.tab.c"
    break;

  case 244: /* argument: expression COLON expression COLON expression  */
#line 583 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3455 "pascal.tab.c"
    break;

  case 245: /* assignment: selector BECOMES expression  */
#line 586 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3461 "pascal.tab.c"
    break;

  case 246: /* assignment: selector ADDBECOMES expression  */
#line 587 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3467 "pascal.tab.c"
    break;

  case 247: /* assignment: selector SUBBECOMES expression  */
#line 588 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3473 "pascal.tab.c"
    break;

  case 248: /* assignment: selector MULBECOMES expression  */
#line 589 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3479 "pascal.tab.c"
    break;

  case 249: /* assignment: selector DIVBECOMES expression  */
#line 590 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3485 "pascal.tab.c"
    break;

  case 250: /* selector: selector LBRACK expression_list RBRACK  */
#line 593 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3491 "pascal.tab.c"
    break;

  case 251: /* selector: selector PERIOD IDENT  */
#line 594 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3497 "pascal.tab.c"
    break;

  case 252: /* selector: selector ARROW  */
#line 595 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3503 "pascal.tab.c"
    break;

  case 253: /* selector: call_with_args PERIOD IDENT  */
#line 596 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3509 "pascal.tab.c"
    break;

  case 254: /* selector: call_with_args ARROW  */
#line 597 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3515 "pascal.tab.c"
    break;

  case 255: /* selector: IDENT  */
#line 598 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3521 "pascal.tab.c"
    break;

  case 256: /* expression_list: expression_list COMMA expression  */
#line 601 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3527 "pascal.tab.c"
    break;

  case 257: /* expression_list: expression  */
#line 602 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3533 "pascal.tab.c"
    break;

  case 258: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 605 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3539 "pascal.tab.c"
    break;

  case 259: /* goto_statement: GOTOSY INTCONST  */
#line 608 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3545 "pascal.tab.c"
    break;

  case 260: /* goto_statement: GOTOSY IDENT  */
#line 609 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3551 "pascal.tab.c"
    break;

  case 261: /* if_statement: IFSY expression THENSY body_stmt  */
#line 612 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3557 "pascal.tab.c"
    break;

  case 262: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 613 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3563 "pascal.tab.c"
    break;

  case 263: /* $@17: %empty  */
#line 616 "pascal.y"
                           { }
#line 3569 "pascal.tab.c"
    break;

  case 264: /* case_statement: CASESY expression OFSY $@17 case_list case_tail ENDSY  */
#line 616 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3575 "pascal.tab.c"
    break;

  case 265: /* case_tail: %empty  */
#line 619 "pascal.y"
    { (yyval.node) = NULL; }
#line 3581 "pascal.tab.c"
    break;

  case 266: /* case_tail: ELSESY statement_list  */
#line 620 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3587 "pascal.tab.c"
    break;

  case 267: /* case_tail: OTHERWISESY statement_list  */
#line 621 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3593 "pascal.tab.c"
    break;

  case 268: /* case_list: case_list SEMICOLON case_elem  */
#line 624 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3599 "pascal.tab.c"
    break;

  case 269: /* case_list: case_elem  */
#line 625 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3605 "pascal.tab.c"
    break;

  case 270: /* case_elem: constant_list COLON body_stmt  */
#line 628 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3611 "pascal.tab.c"
    break;

  case 271: /* case_elem: %empty  */
#line 629 "pascal.y"
     { (yyval.node) = NULL; }
#line 3617 "pascal.tab.c"
    break;

  case 272: /* constant_list: constant_list COMMA case_label  */
#line 632 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3623 "pascal.tab.c"
    break;

  case 273: /* constant_list: case_label  */
#line 633 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3629 "pascal.tab.c"
    break;

  case 274: /* case_label: b_expr  */
#line 636 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3635 "pascal.tab.c"
    break;

  case 275: /* case_label: b_expr DOTDOT b_expr  */
#line 637 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3641 "pascal.tab.c"
    break;

  case 276: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 640 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3647 "pascal.tab.c"
    break;

  case 277: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 643 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3653 "pascal.tab.c"
    break;

  case 278: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 646 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3659 "pascal.tab.c"
    break;

  case 279: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 647 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3665 "pascal.tab.c"
    break;

  case 280: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 648 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3671 "pascal.tab.c"
    break;

  case 281: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 651 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3677 "pascal.tab.c"
    break;

  case 282: /* with_open: with_open COMMA selector  */
#line 654 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3683 "pascal.tab.c"
    break;

  case 283: /* with_open: selector  */
#line 655 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3689 "pascal.tab.c"
    break;

  case 284: /* expression: simple_expression  */
#line 662 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3695 "pascal.tab.c"
    break;

  case 285: /* expression: expression INOP simple_expression  */
#line 663 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3701 "pascal.tab.c"
    break;

  case 286: /* expression: expression LTOP simple_expression  */
#line 664 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3707 "pascal.tab.c"
    break;

  case 287: /* expression: expression LEOP simple_expression  */
#line 665 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3713 "pascal.tab.c"
    break;

  case 288: /* expression: expression GTOP simple_expression  */
#line 666 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3719 "pascal.tab.c"
    break;

  case 289: /* expression: expression GEOP simple_expression  */
#line 667 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3725 "pascal.tab.c"
    break;

  case 290: /* expression: expression NEOP simple_expression  */
#line 668 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3731 "pascal.tab.c"
    break;

  case 291: /* expression: expression EQOP simple_expression  */
#line 669 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3737 "pascal.tab.c"
    break;

  case 292: /* expression: expression ISOP simple_expression  */
#line 670 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3743 "pascal.tab.c"
    break;

  case 293: /* simple_expression: term  */
#line 673 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3749 "pascal.tab.c"
    break;

  case 294: /* simple_expression: PLUS term  */
#line 674 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3755 "pascal.tab.c"
    break;

  case 295: /* simple_expression: MINUS term  */
#line 675 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3761 "pascal.tab.c"
    break;

  case 296: /* simple_expression: simple_expression PLUS term  */
#line 676 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3767 "pascal.tab.c"
    break;

  case 297: /* simple_expression: simple_expression MINUS term  */
#line 677 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3773 "pascal.tab.c"
    break;

  case 298: /* simple_expression: simple_expression OROP term  */
#line 678 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3779 "pascal.tab.c"
    break;

  case 299: /* simple_expression: simple_expression XOROP term  */
#line 679 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3785 "pascal.tab.c"
    break;

  case 300: /* term: power  */
#line 682 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3791 "pascal.tab.c"
    break;

  case 301: /* term: term MUL power  */
#line 683 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3797 "pascal.tab.c"
    break;

  case 302: /* term: term RDIV power  */
#line 684 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3803 "pascal.tab.c"
    break;

  case 303: /* term: term IDIV power  */
#line 685 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3809 "pascal.tab.c"
    break;

  case 304: /* term: term IMOD power  */
#line 686 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3815 "pascal.tab.c"
    break;

  case 305: /* term: term ANDOP power  */
#line 687 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3821 "pascal.tab.c"
    break;

  case 306: /* term: term SHIFTOP power  */
#line 688 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3827 "pascal.tab.c"
    break;

  case 307: /* term: term ASOP power  */
#line 689 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3833 "pascal.tab.c"
    break;

  case 308: /* power: factor  */
#line 692 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3839 "pascal.tab.c"
    break;

  case 309: /* power: power POWOP factor  */
#line 693 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3845 "pascal.tab.c"
    break;

  case 310: /* factor: selector  */
#line 696 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3851 "pascal.tab.c"
    break;

  case 311: /* factor: call_with_args  */
#line 697 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3857 "pascal.tab.c"
    break;

  case 312: /* factor: INTCONST  */
#line 698 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3863 "pascal.tab.c"
    break;

  case 313: /* factor: REALCONST  */
#line 699 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3869 "pascal.tab.c"
    break;

  case 314: /* factor: STRINGCONST  */
#line 700 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 3875 "pascal.tab.c"
    break;

  case 315: /* factor: CHARCODE  */
#line 701 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3881 "pascal.tab.c"
    break;

  case 316: /* factor: LPARENT expression RPARENT  */
#line 702 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3887 "pascal.tab.c"
    break;

  case 317: /* factor: inherited_call  */
#line 703 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3893 "pascal.tab.c"
    break;

  case 318: /* factor: NOTSY factor  */
#line 704 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 3899 "pascal.tab.c"
    break;

  case 319: /* factor: ATSIGN factor  */
#line 705 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 3905 "pascal.tab.c"
    break;

  case 320: /* factor: LBRACK RBRACK  */
#line 706 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 3911 "pascal.tab.c"
    break;

  case 321: /* factor: LBRACK set_member_list RBRACK  */
#line 707 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 3917 "pascal.tab.c"
    break;

  case 322: /* set_member_list: set_member  */
#line 710 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3923 "pascal.tab.c"
    break;

  case 323: /* set_member_list: set_member_list COMMA set_member  */
#line 711 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3929 "pascal.tab.c"
    break;

  case 324: /* set_member: expression  */
#line 714 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3935 "pascal.tab.c"
    break;

  case 325: /* set_member: expression DOTDOT expression  */
#line 715 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3941 "pascal.tab.c"
    break;


#line 3945 "pascal.tab.c"

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

#line 721 "pascal.y"

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
