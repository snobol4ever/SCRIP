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
static char g_pascal_ident_buf[256];
void pascal_last_ident_set(const char *p, int n) { if (n > 255) n = 255; memcpy(g_pascal_ident_buf, p, (size_t)n); g_pascal_ident_buf[n] = '\0'; g_pascal_cenv.ident_p = g_pascal_ident_buf; g_pascal_cenv.ident_n = n; }
tree_t *pascal_last_ident_qlit(void) {
    tree_t *q = ast_node_new(TT_QLIT);
    char *s = ct_alloc((size_t)g_pascal_cenv.ident_n + 1);
    if (g_pascal_cenv.ident_p) memcpy(s, g_pascal_cenv.ident_p, (size_t)g_pascal_cenv.ident_n);
    s[g_pascal_cenv.ident_p ? g_pascal_cenv.ident_n : 0] = '\0';
    q->v.sval = s;
    return q;
}
static tree_t *pas_enum_member(tree_t *id) {
    tree_t *q = pascal_last_ident_qlit();
    if (id && id->v.sval && q->v.sval && strcmp(id->v.sval, q->v.sval)) {
        tree_t *a = ast_node_new(TT_ATTR);
        a->v.sval = ct_strdup(":spelling");
        ast_push(a, q);
        ast_push(id, a);
    }
    return id;
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

#line 235 "pascal.tab.c"

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
  YYSYMBOL_EXPORTSSY = 35,                 /* EXPORTSSY  */
  YYSYMBOL_FUNCTIONSY = 36,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 37,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 38,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 39,                    /* TYPESY  */
  YYSYMBOL_IFSY = 40,                      /* IFSY  */
  YYSYMBOL_ELSESY = 41,                    /* ELSESY  */
  YYSYMBOL_INOP = 42,                      /* INOP  */
  YYSYMBOL_NOTSY = 43,                     /* NOTSY  */
  YYSYMBOL_IDIV = 44,                      /* IDIV  */
  YYSYMBOL_IMOD = 45,                      /* IMOD  */
  YYSYMBOL_ANDOP = 46,                     /* ANDOP  */
  YYSYMBOL_OROP = 47,                      /* OROP  */
  YYSYMBOL_LTOP = 48,                      /* LTOP  */
  YYSYMBOL_LEOP = 49,                      /* LEOP  */
  YYSYMBOL_GTOP = 50,                      /* GTOP  */
  YYSYMBOL_GEOP = 51,                      /* GEOP  */
  YYSYMBOL_NEOP = 52,                      /* NEOP  */
  YYSYMBOL_EQOP = 53,                      /* EQOP  */
  YYSYMBOL_PLUS = 54,                      /* PLUS  */
  YYSYMBOL_MINUS = 55,                     /* MINUS  */
  YYSYMBOL_MUL = 56,                       /* MUL  */
  YYSYMBOL_RDIV = 57,                      /* RDIV  */
  YYSYMBOL_COMMA = 58,                     /* COMMA  */
  YYSYMBOL_PERIOD = 59,                    /* PERIOD  */
  YYSYMBOL_COLON = 60,                     /* COLON  */
  YYSYMBOL_ARROW = 61,                     /* ARROW  */
  YYSYMBOL_LBRACK = 62,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 63,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 64,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 65,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 66,                    /* DOTDOT  */
  YYSYMBOL_ATSIGN = 67,                    /* ATSIGN  */
  YYSYMBOL_ADDBECOMES = 68,                /* ADDBECOMES  */
  YYSYMBOL_SUBBECOMES = 69,                /* SUBBECOMES  */
  YYSYMBOL_MULBECOMES = 70,                /* MULBECOMES  */
  YYSYMBOL_DIVBECOMES = 71,                /* DIVBECOMES  */
  YYSYMBOL_INTCONST = 72,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 73,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 74,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 75,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 76,                     /* IDENT  */
  YYSYMBOL_SHIFTOP = 77,                   /* SHIFTOP  */
  YYSYMBOL_XOROP = 78,                     /* XOROP  */
  YYSYMBOL_POWOP = 79,                     /* POWOP  */
  YYSYMBOL_CLASSSY = 80,                   /* CLASSSY  */
  YYSYMBOL_ISOP = 81,                      /* ISOP  */
  YYSYMBOL_ASOP = 82,                      /* ASOP  */
  YYSYMBOL_TRYSY = 83,                     /* TRYSY  */
  YYSYMBOL_EXCEPTSY = 84,                  /* EXCEPTSY  */
  YYSYMBOL_FINALLYSY = 85,                 /* FINALLYSY  */
  YYSYMBOL_RAISESY = 86,                   /* RAISESY  */
  YYSYMBOL_INHERITEDSY = 87,               /* INHERITEDSY  */
  YYSYMBOL_CONSTRUCTORSY = 88,             /* CONSTRUCTORSY  */
  YYSYMBOL_DESTRUCTORSY = 89,              /* DESTRUCTORSY  */
  YYSYMBOL_PROPERTYSY = 90,                /* PROPERTYSY  */
  YYSYMBOL_ONSY = 91,                      /* ONSY  */
  YYSYMBOL_YYACCEPT = 92,                  /* $accept  */
  YYSYMBOL_compilation_unit = 93,          /* compilation_unit  */
  YYSYMBOL_program = 94,                   /* program  */
  YYSYMBOL_program_name = 95,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 96,          /* file_id_list_opt  */
  YYSYMBOL_block = 97,                     /* block  */
  YYSYMBOL_decl_part_list = 98,            /* decl_part_list  */
  YYSYMBOL_decl_part = 99,                 /* decl_part  */
  YYSYMBOL_unit = 100,                     /* unit  */
  YYSYMBOL_intf_part_list = 101,           /* intf_part_list  */
  YYSYMBOL_intf_part = 102,                /* intf_part  */
  YYSYMBOL_init_opt = 103,                 /* init_opt  */
  YYSYMBOL_fini_opt = 104,                 /* fini_opt  */
  YYSYMBOL_label_list = 105,               /* label_list  */
  YYSYMBOL_const_decl_list = 106,          /* const_decl_list  */
  YYSYMBOL_const_decl = 107,               /* const_decl  */
  YYSYMBOL_typed_value = 108,              /* typed_value  */
  YYSYMBOL_typed_value_list = 109,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 110,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 111,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 112,           /* type_decl_list  */
  YYSYMBOL_type_decl = 113,                /* type_decl  */
  YYSYMBOL_type = 114,                     /* type  */
  YYSYMBOL_115_1 = 115,                    /* $@1  */
  YYSYMBOL_116_2 = 116,                    /* $@2  */
  YYSYMBOL_117_3 = 117,                    /* $@3  */
  YYSYMBOL_result_type = 118,              /* result_type  */
  YYSYMBOL_class_members = 119,            /* class_members  */
  YYSYMBOL_class_member = 120,             /* class_member  */
  YYSYMBOL_prop_index_opt = 121,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 122,               /* prop_specs  */
  YYSYMBOL_packed_opt = 123,               /* packed_opt  */
  YYSYMBOL_simple_type = 124,              /* simple_type  */
  YYSYMBOL_lo_bound = 125,                 /* lo_bound  */
  YYSYMBOL_lo_term = 126,                  /* lo_term  */
  YYSYMBOL_b_expr = 127,                   /* b_expr  */
  YYSYMBOL_b_term = 128,                   /* b_term  */
  YYSYMBOL_b_factor = 129,                 /* b_factor  */
  YYSYMBOL_record_body = 130,              /* record_body  */
  YYSYMBOL_record_field_list = 131,        /* record_field_list  */
  YYSYMBOL_record_field = 132,             /* record_field  */
  YYSYMBOL_133_4 = 133,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 134,            /* case_arm_mark  */
  YYSYMBOL_case_open = 135,                /* case_open  */
  YYSYMBOL_record_case_opt = 136,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 137,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 138,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 139,            /* var_decl_list  */
  YYSYMBOL_var_decl = 140,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 141,           /* procedure_decl  */
  YYSYMBOL_142_5 = 142,                    /* $@5  */
  YYSYMBOL_143_6 = 143,                    /* $@6  */
  YYSYMBOL_144_7 = 144,                    /* $@7  */
  YYSYMBOL_145_8 = 145,                    /* $@8  */
  YYSYMBOL_146_9 = 146,                    /* $@9  */
  YYSYMBOL_147_10 = 147,                   /* $@10  */
  YYSYMBOL_148_11 = 148,                   /* $@11  */
  YYSYMBOL_149_12 = 149,                   /* $@12  */
  YYSYMBOL_150_13 = 150,                   /* $@13  */
  YYSYMBOL_pv_mark = 151,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 152,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 153,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 154,           /* parameter_decl  */
  YYSYMBOL_param_type = 155,               /* param_type  */
  YYSYMBOL_routine_directives = 156,       /* routine_directives  */
  YYSYMBOL_pf_params = 157,                /* pf_params  */
  YYSYMBOL_pf_sections = 158,              /* pf_sections  */
  YYSYMBOL_pf_section = 159,               /* pf_section  */
  YYSYMBOL_enum_list = 160,                /* enum_list  */
  YYSYMBOL_enum_member = 161,              /* enum_member  */
  YYSYMBOL_id_list = 162,                  /* id_list  */
  YYSYMBOL_body = 163,                     /* body  */
  YYSYMBOL_statement_list = 164,           /* statement_list  */
  YYSYMBOL_165_14 = 165,                   /* $@14  */
  YYSYMBOL_166_15 = 166,                   /* $@15  */
  YYSYMBOL_body_stmt = 167,                /* body_stmt  */
  YYSYMBOL_168_16 = 168,                   /* $@16  */
  YYSYMBOL_statement = 169,                /* statement  */
  YYSYMBOL_statement_no_label = 170,       /* statement_no_label  */
  YYSYMBOL_try_statement = 171,            /* try_statement  */
  YYSYMBOL_handler_list = 172,             /* handler_list  */
  YYSYMBOL_handler = 173,                  /* handler  */
  YYSYMBOL_raise_statement = 174,          /* raise_statement  */
  YYSYMBOL_inherited_call = 175,           /* inherited_call  */
  YYSYMBOL_call = 176,                     /* call  */
  YYSYMBOL_recv_primary = 177,             /* recv_primary  */
  YYSYMBOL_call_with_args = 178,           /* call_with_args  */
  YYSYMBOL_argument_list = 179,            /* argument_list  */
  YYSYMBOL_argument = 180,                 /* argument  */
  YYSYMBOL_assignment = 181,               /* assignment  */
  YYSYMBOL_selector = 182,                 /* selector  */
  YYSYMBOL_expression_list = 183,          /* expression_list  */
  YYSYMBOL_compound_statement = 184,       /* compound_statement  */
  YYSYMBOL_goto_statement = 185,           /* goto_statement  */
  YYSYMBOL_if_statement = 186,             /* if_statement  */
  YYSYMBOL_case_statement = 187,           /* case_statement  */
  YYSYMBOL_188_17 = 188,                   /* $@17  */
  YYSYMBOL_case_tail = 189,                /* case_tail  */
  YYSYMBOL_case_list = 190,                /* case_list  */
  YYSYMBOL_case_elem = 191,                /* case_elem  */
  YYSYMBOL_constant_list = 192,            /* constant_list  */
  YYSYMBOL_case_label = 193,               /* case_label  */
  YYSYMBOL_while_statement = 194,          /* while_statement  */
  YYSYMBOL_repeat_statement = 195,         /* repeat_statement  */
  YYSYMBOL_for_statement = 196,            /* for_statement  */
  YYSYMBOL_with_statement = 197,           /* with_statement  */
  YYSYMBOL_with_open = 198,                /* with_open  */
  YYSYMBOL_expression = 199,               /* expression  */
  YYSYMBOL_simple_expression = 200,        /* simple_expression  */
  YYSYMBOL_term = 201,                     /* term  */
  YYSYMBOL_power = 202,                    /* power  */
  YYSYMBOL_factor = 203,                   /* factor  */
  YYSYMBOL_set_member_list = 204,          /* set_member_list  */
  YYSYMBOL_set_member = 205                /* set_member  */
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
#define YYLAST   1237

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  347
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  781

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   346


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
      85,    86,    87,    88,    89,    90,    91
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   215,   215,   216,   219,   220,   223,   224,   227,   228,
     231,   234,   235,   238,   239,   240,   241,   242,   243,   244,
     247,   250,   251,   254,   255,   256,   257,   258,   259,   262,
     263,   264,   267,   268,   271,   272,   273,   274,   277,   278,
     281,   282,   285,   286,   287,   288,   291,   292,   295,   296,
     299,   300,   301,   302,   303,   306,   307,   310,   311,   312,
     315,   316,   317,   317,   317,   318,   319,   320,   321,   322,
     323,   324,   324,   325,   326,   327,   328,   329,   332,   333,
     336,   337,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   352,   353,   356,   357,   358,   361,   362,   365,
     366,   367,   368,   369,   372,   373,   374,   375,   376,   379,
     380,   381,   382,   383,   384,   387,   388,   389,   390,   391,
     394,   395,   396,   397,   398,   401,   402,   403,   406,   409,
     410,   413,   413,   414,   417,   420,   423,   424,   425,   428,
     429,   432,   433,   436,   437,   440,   441,   442,   445,   446,
     447,   447,   448,   448,   449,   449,   450,   450,   451,   451,
     454,   454,   457,   457,   460,   460,   463,   463,   468,   471,
     472,   473,   476,   477,   480,   481,   482,   483,   484,   485,
     486,   487,   490,   491,   492,   495,   496,   506,   507,   508,
     511,   512,   515,   516,   517,   518,   519,   522,   523,   526,
     529,   530,   533,   536,   536,   537,   537,   540,   540,   543,
     544,   545,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   565,   566,   567,
     568,   571,   572,   573,   576,   577,   580,   581,   584,   585,
     586,   589,   590,   591,   594,   595,   598,   599,   600,   601,
     602,   603,   604,   605,   608,   609,   612,   613,   614,   617,
     618,   619,   620,   621,   622,   625,   626,   627,   628,   629,
     630,   631,   632,   635,   636,   639,   642,   643,   646,   647,
     650,   650,   653,   654,   655,   658,   659,   662,   663,   666,
     667,   670,   671,   674,   677,   680,   681,   682,   685,   688,
     689,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     707,   708,   709,   710,   711,   712,   713,   716,   717,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     731,   732,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   749,   750,   753,   754
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
  "RECORDSY", "EXPORTSSY", "FUNCTIONSY", "BEGINSY", "BECOMES", "TYPESY",
  "IFSY", "ELSESY", "INOP", "NOTSY", "IDIV", "IMOD", "ANDOP", "OROP",
  "LTOP", "LEOP", "GTOP", "GEOP", "NEOP", "EQOP", "PLUS", "MINUS", "MUL",
  "RDIV", "COMMA", "PERIOD", "COLON", "ARROW", "LBRACK", "RBRACK",
  "LPARENT", "RPARENT", "DOTDOT", "ATSIGN", "ADDBECOMES", "SUBBECOMES",
  "MULBECOMES", "DIVBECOMES", "INTCONST", "CHARCODE", "REALCONST",
  "STRINGCONST", "IDENT", "SHIFTOP", "XOROP", "POWOP", "CLASSSY", "ISOP",
  "ASOP", "TRYSY", "EXCEPTSY", "FINALLYSY", "RAISESY", "INHERITEDSY",
  "CONSTRUCTORSY", "DESTRUCTORSY", "PROPERTYSY", "ONSY", "$accept",
  "compilation_unit", "program", "program_name", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "unit", "intf_part_list",
  "intf_part", "init_opt", "fini_opt", "label_list", "const_decl_list",
  "const_decl", "typed_value", "typed_value_list", "field_value_list",
  "scalar_constant", "type_decl_list", "type_decl", "type", "$@1", "$@2",
  "$@3", "result_type", "class_members", "class_member", "prop_index_opt",
  "prop_specs", "packed_opt", "simple_type", "lo_bound", "lo_term",
  "b_expr", "b_term", "b_factor", "record_body", "record_field_list",
  "record_field", "$@4", "case_arm_mark", "case_open", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "param_type", "routine_directives", "pf_params",
  "pf_sections", "pf_section", "enum_list", "enum_member", "id_list",
  "body", "statement_list", "$@14", "$@15", "body_stmt", "$@16",
  "statement", "statement_no_label", "try_statement", "handler_list",
  "handler", "raise_statement", "inherited_call", "call", "recv_primary",
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

#define YYPACT_NINF (-477)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-273)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     118,    25,    67,   123,  -477,   -12,   347,  -477,  -477,   287,
     179,  -477,  -477,   227,   111,   151,   162,   151,   151,   199,
    -477,   211,   124,   241,   258,  -477,  -477,  -477,   261,   151,
     306,   337,  -477,  -477,    44,   201,   111,  -477,  -477,    74,
     303,   151,  -477,   395,    82,   311,    63,   289,   332,   211,
    -477,   330,   352,   329,   357,  -477,   209,  -477,  -477,  -477,
     422,   820,   665,  -477,  -477,   358,   361,   379,  -477,   665,
    -477,   385,    60,  -477,  -477,   435,   393,  -477,   820,   150,
     820,  -477,   820,   820,   418,  -477,  1093,  -477,   820,   405,
    -477,  -477,  -477,  -477,  -477,  -477,   429,   259,  -477,  1150,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,   695,  -477,
     432,   451,   472,   473,  -477,   506,   467,  -477,  -477,   196,
     196,   196,   710,   820,   196,  -477,  -477,  -477,   507,   462,
     405,  -477,   517,   521,   466,   376,  1094,   483,  -477,   509,
    -477,   509,   449,   449,   520,   525,  -477,  -477,  -477,  -477,
    1123,   543,  -477,   566,   299,  -477,   165,   168,  -477,  -477,
      27,   608,    24,  -477,  -477,   562,   289,  -477,  -477,   489,
     115,   503,   517,   521,    37,   153,   191,   662,   994,   426,
     426,   346,    13,  1084,   565,   557,   820,   559,  -477,   820,
     820,   568,  -477,   820,   820,   820,   820,   820,    62,   647,
     577,   581,  -477,  -477,  -477,   111,   151,  -477,   595,   151,
     596,   211,  -477,  -477,  1094,  1094,  -477,  1012,   349,  -477,
    1031,  -477,   604,  -477,   820,   820,   820,   820,   820,   820,
     820,   820,   196,   196,   196,   196,   878,   900,   916,   938,
     954,   196,   196,   196,   101,   637,   619,   629,   168,   168,
    -477,  -477,   254,  -477,   607,  1013,  1013,   151,   514,   836,
      66,   666,   668,  -477,   513,   513,  1013,   513,   513,   513,
     513,   379,   151,   621,   151,   622,  -477,   151,    40,  -477,
     537,   690,  -477,   836,   624,   379,  -477,   626,  -477,   820,
     820,   820,  -477,  -477,   150,  -477,  -477,  -477,  -477,  1122,
    -477,  -477,  -477,   313,  -477,  1049,   612,  -477,   736,   641,
    1084,   642,   407,  1084,  1084,  1135,   421,  1084,  1084,  1084,
    1084,  -477,   151,  -477,  -477,  -477,   379,   379,   111,   109,
     450,  -477,   151,  -477,   211,   820,   820,  -477,   648,   644,
     376,   376,   376,   376,   376,   376,   376,   376,  1094,  1094,
    1094,  1094,   196,   483,   196,   483,   196,   483,   196,   483,
     196,   483,   483,   483,  -477,   151,   640,   151,   649,  -477,
      46,  -477,   550,   651,   626,   525,  -477,  -477,   513,   513,
    1013,   654,  -477,   277,   271,  -477,   270,   316,   652,  -477,
     656,   120,   220,   657,   658,   659,  -477,   554,   862,   718,
    1084,   665,  1153,   665,  1153,   151,   271,   271,   342,  -477,
    -477,  -477,  -477,   719,   560,   509,   574,   509,   585,    81,
    -477,    20,   725,   660,   737,   739,   686,   742,   689,   746,
     636,   519,  1084,  -477,   289,  -477,   521,  1013,   711,   820,
    -477,   820,   678,   205,   177,  -477,   210,  -477,   322,   752,
     778,   820,  -477,   794,  -477,   340,   379,   379,   750,   753,
    -477,  -477,  -477,   745,   379,   379,  1084,  -477,   483,   483,
     483,   483,   483,   590,   509,   591,   509,   157,  -477,    20,
    -477,   729,  -477,   271,   271,   304,  1013,   513,   513,  -477,
     513,   513,   513,   513,  -477,  -477,  -477,  -477,  -477,   685,
     700,  -477,  -477,   704,   665,   477,   720,    47,  1031,  -477,
    -477,   434,  -477,  -477,   732,   114,  -477,   602,  -477,    20,
    -477,    20,   721,    20,  -477,   759,   730,   727,  -477,   783,
    -477,  -477,  -477,   626,  -477,   716,   784,   820,   820,  -477,
    -477,   235,    94,  -477,   603,  -477,  -477,  -477,  1065,    71,
    -477,   612,  -477,  -477,  -477,  -477,  -477,   367,  -477,   383,
    1084,  -477,   384,   789,   791,   744,  -477,  -477,   797,   797,
    -477,   769,   800,   755,    20,  -477,    20,   758,  -477,  -477,
     754,  -477,   312,   271,   271,  -477,  -477,  -477,  -477,   579,
     379,   379,  -477,  -477,   379,   379,    81,   760,   808,   836,
     836,   146,  -477,  1153,   801,  -477,   151,   762,  -477,  -477,
     660,   781,  -477,   626,  -477,   765,   768,   820,  -477,   817,
     826,  -477,   830,   660,   575,   589,  -477,  1013,  1013,  -477,
    -477,   806,  1013,  -477,  -477,   820,  -477,   770,  -477,   223,
    -477,  -477,  -477,  -477,  -477,   626,   660,   660,   797,   788,
    -477,   626,  -477,  -477,   626,  -477,  -477,  -477,   850,   802,
     379,   379,   852,   855,    73,   795,  -477,  -477,  -477,   394,
    -477,   804,   809,  -477,  -477,    56,   665,  -477,   820,  -477,
    -477,  -477,  1084,  -477,  -477,  -477,  -477,  -477,  -477,   342,
    -477,   797,   797,  -477,  -477,  -477,  1084,  -477,   856,  -477,
     660,   871,  -477,  -477,  -477,   872,  -477,  -477,   626,   873,
     823,  -477,  -477,  -477,  -477,   836,  -477,   836,   857,  -477,
    -477,   810,  -477,   875,  1084,   660,   883,  -477,  -477,  -477,
    -477,  -477,   884,   892,  -477,   894,  -477,   626,    39,  -477,
    -477,   665,   665,  1013,   874,  -477,  -477,  -477,  -477,   896,
     660,  -477,  -477,  -477,   899,  -477,   470,  -477,  -477,   901,
    -477,   606,  -477,   908,  -477,  -477,  -477,  -477,  -477,  1013,
     851,  1013,  -477,   909,  -477,  -477,   901,  -477,   151,   853,
    -477
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,     0,
     205,     0,     0,     0,     0,    11,    17,    10,     0,     0,
       0,     0,    36,    37,     0,     0,    14,    39,   201,     0,
     168,    16,   144,     0,     0,   168,     0,   226,     0,    15,
      56,     0,     0,     0,     0,     7,     0,    12,    22,    13,
       0,     0,    98,    38,    18,     0,     0,   171,   143,    98,
      19,     0,   171,   203,   202,     0,     0,   205,     0,     0,
       0,   205,     0,     0,     0,   244,   241,   205,   236,   225,
     206,   209,   222,   223,   224,   213,     0,   243,   212,     0,
     214,   215,   216,   217,   218,   219,   220,   221,    98,    55,
       0,     0,     0,     0,     8,     0,     0,    34,    35,     0,
       0,     0,     0,     0,     0,   334,   337,   335,   336,   272,
       0,   339,   333,   332,     0,   301,   310,   317,   330,   189,
      97,   189,     0,     0,     0,     0,    51,    54,    52,    53,
     100,    80,   109,     0,     0,    60,     0,   104,   200,   168,
       0,     0,     0,   168,   154,     0,   226,   276,   277,     0,
       0,     0,     0,   300,     0,     0,     0,     0,     0,   226,
     226,     0,     0,   237,   238,     0,     0,     0,   270,     0,
       0,     0,   267,     0,     0,     0,     0,     0,    80,     0,
       0,     0,   168,   168,     4,     0,     0,    12,     0,     0,
       0,     0,    21,   340,   311,   312,   342,   346,     0,   344,
       0,   341,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    50,   105,   106,
      61,   199,     0,   198,     0,     0,     0,     0,     0,     0,
       0,    74,     0,    71,     0,     0,     0,     0,     0,     0,
       0,   171,     0,     0,     0,     0,   170,   201,     0,   173,
       0,   185,   145,     0,     0,   171,    12,     0,   204,     0,
       0,     0,   207,   207,     0,   280,   275,   207,   245,   241,
     210,   211,   247,     0,   255,   256,   205,   205,     0,   271,
     264,   268,     0,   274,   259,   242,     0,   260,   261,   262,
     263,    58,     0,    57,   168,   168,   171,   171,    24,     0,
      31,   168,    26,   168,    25,     0,     0,   343,   338,   266,
     302,   303,   304,   305,   306,   307,   308,   309,   315,   313,
     314,   316,     0,   320,     0,   321,     0,   322,     0,   318,
       0,   319,   328,   329,   331,     0,     0,     0,     0,   188,
       0,   191,     0,     0,     0,     0,    99,   102,     0,     0,
       0,    50,   125,     0,   115,   120,     0,     0,     0,    76,
       0,    82,     0,     0,     0,     0,    81,     0,     0,     0,
      42,    98,     0,    98,     0,   133,   107,   108,   103,   112,
     113,   111,   114,     0,     0,   189,     0,   189,     0,     0,
     169,     0,     0,   150,     0,     0,     0,     0,    78,     0,
       0,     0,   294,   293,   226,   298,   299,   288,   278,     0,
     246,     0,     0,     0,     0,   231,     0,   239,     0,     0,
       0,     0,   269,     0,   265,     0,   171,   171,     0,     0,
      23,   205,   205,    33,   171,   171,   347,   345,   325,   326,
     327,   323,   324,     0,   189,     0,   189,     0,   187,     0,
      67,    68,   197,   116,   117,     0,     0,     0,     0,   101,
       0,     0,     0,     0,   110,    80,   168,   168,    83,     0,
       0,   168,   168,    92,    98,   272,     0,     0,    42,    41,
      65,     0,    75,    73,     0,   138,   130,     0,   185,     0,
     174,     0,     0,     0,   172,     0,   182,   177,   148,     0,
      12,   146,   147,     0,   155,     0,   185,     0,     0,   207,
     208,   291,   282,   286,     0,   290,   207,   254,   257,     0,
     227,   233,   228,   205,   230,   240,   253,     0,   251,     0,
     273,   249,     0,    80,     0,     0,   185,   185,    29,    30,
     205,     0,     0,     0,     0,   195,     0,     0,   190,   192,
       0,   127,     0,   118,   119,   122,   123,   121,   124,     0,
     171,   171,   168,   168,   171,   171,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    72,   133,     0,   128,   131,
     156,   178,   176,     0,   181,     0,     0,     0,   186,     0,
       0,    79,     0,   152,     0,     0,   297,     0,   288,   205,
     205,     0,     0,   207,   279,     0,   207,     0,   232,     0,
     252,   250,   248,    59,   185,     0,   160,   162,    32,     0,
      27,     0,   194,   193,     0,    69,   126,    77,     0,     0,
     171,   171,     0,     0,     0,     0,    84,    48,    46,     0,
      45,     0,     0,    62,   129,     0,    98,    12,     0,   175,
     184,   183,   179,   151,   185,   149,    12,   207,   207,   292,
     285,   284,   283,   281,   289,   287,   258,   235,     0,   229,
     164,     0,    12,    12,    20,     0,   196,    85,     0,     0,
       0,    87,    88,    93,    94,     0,    43,     0,     0,    63,
     135,     0,   132,     0,   180,   158,     0,   296,   295,   207,
      12,   185,     0,     0,    28,     0,    89,     0,     0,    47,
      49,    98,    98,   142,     0,   157,    12,   153,   234,     0,
     166,   161,   163,    86,     0,    91,     0,    70,    64,   137,
     140,     0,   135,     0,   165,    12,    90,    96,    95,   142,
       0,   142,   159,     0,   139,   134,   136,   167,   133,     0,
     141
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -477,  -477,  -477,  -477,  -477,   -56,   712,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,   715,   -28,  -277,  -477,  -477,   -60,
     714,   -36,   -65,  -477,  -477,  -477,  -369,  -445,  -477,  -477,
    -477,  -477,  -371,  -477,   362,  -234,  -237,  -212,   144,  -477,
     321,  -477,  -477,   166,  -477,   159,   163,   722,   -26,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,   -29,
     -62,   343,   522,  -415,  -476,  -129,  -477,   469,  -477,   569,
     -15,  -477,   -70,  -477,  -477,  -273,  -477,  -143,   355,  -477,
    -477,   396,  -477,    -8,  -477,  -477,   -17,  -274,   518,  -477,
     -10,   763,  -477,  -477,  -477,  -477,  -477,  -477,  -477,   320,
    -412,   326,  -477,  -477,  -477,  -477,  -477,   -42,  1006,   -35,
    -166,   -58,  -477,   625
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    30,     5,     6,    25,     7,   116,
     212,   463,   571,    34,    36,    37,   399,   669,   507,   382,
      49,    50,   153,   719,   742,   405,   429,   258,   396,   597,
     738,   154,   155,   156,   157,   541,   384,   385,   514,   515,
     516,   676,   778,   743,   608,   759,   760,    41,    42,    26,
     530,   686,   286,   677,   746,   702,   703,   730,   765,    67,
     161,   278,   279,   527,   423,   245,   370,   371,   252,   253,
      43,    27,    46,   166,    47,   433,   434,    90,    91,    92,
     444,   445,    93,   131,    95,    96,   132,   303,   304,    98,
     133,   312,   100,   101,   102,   103,   437,   631,   542,   543,
     761,   545,   104,   105,   106,   107,   174,   305,   135,   136,
     137,   138,   218,   219
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   115,   152,    44,   162,   481,   424,   170,    63,   152,
     165,   176,   246,   109,    56,    68,    72,   182,    73,   134,
     435,   383,   386,   288,   438,   544,   525,   406,   407,   282,
      97,   511,   408,   513,   448,   272,   171,    99,   175,    94,
     177,   178,   610,   199,   755,   419,   183,    12,   152,    59,
     589,   477,   601,   273,   293,   409,   410,   411,   412,   274,
     623,   213,   172,   275,   579,   164,   221,   321,    73,   173,
     353,   355,   357,   359,   361,   362,   363,   283,   419,    64,
     217,   220,   152,   152,   720,   214,   215,    70,   636,   272,
     646,   647,   276,    74,   401,   294,   526,   306,   307,   628,
     284,     8,    60,   277,   611,   420,   612,   273,   614,   365,
     629,   478,   602,   274,   460,   756,   721,   275,   589,   606,
      73,   506,     1,    11,   160,   498,   322,   366,   402,     2,
     271,   637,    65,   367,   285,   630,   713,   368,   291,   607,
      65,   483,   484,    10,   310,   280,   485,   313,   314,    97,
      51,   313,   317,   318,   319,   320,    99,   277,    94,   652,
      52,   653,    97,    97,   620,   365,   369,    65,   700,    99,
      99,    94,    94,   326,   327,   557,   559,    38,  -201,   562,
    -201,   295,   551,   366,    31,   364,   468,    35,   469,   367,
     470,   329,   471,   368,   472,   224,    73,   348,   349,   350,
     351,   225,   226,   227,   228,   229,   230,   552,   725,   413,
      73,   670,   267,   268,    83,    73,   544,   400,   553,   264,
     265,   296,   671,   426,   269,    85,   129,    38,    73,   372,
     427,   266,   672,    38,   231,   550,   443,   446,    40,   119,
     554,   400,   387,   397,   679,   270,   499,   430,   431,   432,
     583,   584,   582,   699,    61,   750,   500,   414,   122,   416,
     123,    62,   418,   124,   458,   459,   626,    65,   125,   126,
     127,   128,   129,   634,   114,    45,   701,   172,   585,   586,
     587,   588,   705,   130,   436,   706,   520,    48,   522,   487,
     488,   540,    75,   466,   217,   456,   457,   186,   109,    32,
      63,   627,   464,    33,   465,   260,    68,   455,    76,    77,
      78,    57,   375,    79,    80,   490,   491,    53,   187,   376,
     188,   189,   667,   668,   487,   488,    81,   492,   261,    82,
     262,   487,   488,   263,    54,   494,   510,    55,   512,   735,
     489,   152,   152,   152,   152,   575,    28,   577,   493,    58,
     473,    29,   475,    83,    13,    14,   508,    15,   487,   488,
     695,    84,    66,   697,    85,    86,   487,   488,   754,   581,
      71,   439,    87,    16,    65,    88,    89,   656,   440,    17,
     439,   495,    18,    19,    20,   108,    21,   555,   112,   119,
     517,   568,   569,   689,   564,   565,   487,   488,    65,   548,
     120,   121,   572,   573,   280,   563,   110,   336,   122,   560,
     123,   302,   337,   124,   727,   728,   113,    97,   125,   126,
     127,   128,   129,   232,    99,   439,    94,    22,   111,    75,
     233,   234,   640,   130,   158,    23,    24,   159,   739,   598,
     740,   439,   439,   160,   152,    76,    77,    78,   641,   642,
      79,    80,   715,    65,   235,    69,   748,    13,    14,   716,
      15,   163,   372,    81,   461,   451,    82,   590,   591,   169,
     452,   223,   594,   595,   619,   205,    16,   206,   179,   451,
     207,   184,    17,   639,   454,    18,    19,   462,   185,    21,
      83,   200,   603,   208,   117,   624,   625,   604,   118,   209,
     648,    85,   299,   210,   248,   249,   211,   167,   224,    87,
     201,   168,    88,    89,   225,   226,   227,   228,   229,   230,
     292,   146,   147,   148,   149,   247,   181,   289,   658,   659,
      22,   290,   662,   663,   300,   301,   539,   599,    23,    24,
     388,   181,   767,   152,   389,   224,   768,   231,   202,   203,
     390,   225,   226,   227,   228,   229,   230,   400,   400,   691,
     692,   224,   243,   660,   661,   204,  -244,   225,   226,   227,
     228,   229,   230,   244,   397,   682,   187,   380,   188,   189,
     222,   280,   192,   193,   231,   146,   147,   148,   149,   381,
     391,   517,   687,   696,   392,    65,   250,   421,   709,   710,
     231,   251,   393,   394,   395,   388,   688,   257,    65,   657,
     479,   722,    65,   281,   504,   390,   152,   224,    65,   259,
     519,   723,   287,   225,   226,   227,   228,   229,   230,   308,
     726,   224,    65,   309,   521,   311,   724,   225,   226,   227,
     228,   229,   230,    65,   315,   523,   732,   733,    65,    65,
     574,   576,   323,   324,   537,   391,   231,   325,   538,   392,
      65,   632,   609,   633,   632,   373,   770,   393,   394,   395,
     231,   331,   333,   400,   749,   400,   757,   758,   224,   374,
     339,   152,   152,   377,   225,   226,   227,   228,   229,   230,
     763,   139,   140,   256,   403,   297,   404,   415,   417,   422,
     425,   141,   428,   442,   224,   449,   450,  -245,   453,   773,
     225,   226,   227,   228,   229,   230,   474,   231,   486,   142,
     143,   139,   140,   509,   518,   476,   144,   480,   496,   145,
     528,   141,   497,   501,   502,   503,   529,   146,   147,   148,
     149,   150,   531,   231,   532,   151,   533,   534,   535,   142,
     143,   536,   546,   119,   549,   566,   144,   580,   567,   145,
     570,   592,   605,   517,   120,   121,   596,   146,   147,   148,
     149,   150,   122,   216,   123,   198,   593,   124,   600,   119,
     617,   613,   125,   126,   127,   128,   129,   615,   618,   616,
     120,   121,   621,   622,   643,   119,   644,   130,   122,   649,
     123,   447,    73,   124,   645,   650,   120,   121,   125,   126,
     127,   128,   129,   666,   122,   651,   123,   556,   654,   124,
     665,   119,   683,   130,   125,   126,   127,   128,   129,   673,
     655,   684,   120,   121,   678,   685,   693,   119,   675,   130,
     122,   680,   123,   558,   681,   124,   698,   704,   120,   121,
     125,   126,   127,   128,   129,   707,   122,   711,   123,   561,
     712,   124,   708,   119,   717,   130,   125,   126,   127,   128,
     129,   714,   718,   729,   120,   121,   731,   734,   736,   119,
     745,   130,   122,   737,   123,   741,   744,   124,   747,   751,
     120,   121,   125,   126,   127,   128,   129,   752,   122,   753,
     398,   764,   762,   124,   766,   119,   769,   130,   125,   126,
     127,   128,   129,   772,   777,   775,   120,   121,   780,   330,
     328,   119,   779,   130,   122,   334,   398,   674,   771,   124,
     776,   332,   774,   352,   125,   126,   127,   128,   505,   664,
     122,   524,   123,   119,   482,   124,   578,   638,   690,   130,
     125,   126,   127,   128,   129,   354,   316,   547,   694,   119,
       0,   467,   122,     0,   123,   130,     0,   124,     0,     0,
       0,   356,   125,   126,   127,   128,   129,     0,   122,     0,
     123,   119,     0,   124,     0,     0,     0,   130,   125,   126,
     127,   128,   129,   358,     0,     0,     0,   119,     0,     0,
     122,     0,   123,   130,     0,   124,     0,     0,     0,   360,
     125,   126,   127,   128,   129,     0,   122,     0,   123,     0,
       0,   124,     0,     0,     0,   130,   125,   126,   127,   128,
     129,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   130,   225,   226,   227,   228,   229,   230,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   298,
     225,   226,   227,   228,   229,   230,     0,   378,   379,     0,
       0,     0,     0,   224,     0,   231,     0,   380,   335,   225,
     226,   227,   228,   229,   230,   146,   147,   148,   149,   381,
       0,   224,     0,   231,     0,     0,   338,   225,   226,   227,
     228,   229,   230,     0,     0,     0,     0,   224,     0,   441,
       0,     0,   231,   225,   226,   227,   228,   229,   230,     0,
       0,     0,     0,     0,     0,   635,   224,     0,     0,     0,
     231,  -272,   225,   226,   227,   228,   229,   230,   236,   237,
     238,     0,     0,     0,     0,     0,   231,     0,     0,     0,
     239,   240,  -272,   180,  -272,  -272,     0,   181,     0,     0,
    -272,  -272,  -272,  -272,  -272,   231,     0,   -50,   -50,     0,
       0,   241,     0,  -266,     0,     0,   242,   -50,   -50,   -50,
       0,  -272,   254,  -272,  -272,   255,   181,   256,   190,   -50,
    -272,  -272,  -272,  -272,  -266,     0,  -266,  -266,     0,   453,
     -50,     0,     0,  -266,  -266,  -266,  -266,   142,   143,   191,
       0,   192,   193,     0,     0,     0,     0,   145,   194,   195,
     196,   197,     0,     0,     0,   146,   147,   148,   149,   150,
     340,   341,   342,   343,   344,   345,   346,   347
};

static const yytype_int16 yycheck[] =
{
      15,    57,    62,    18,    69,   374,   283,    77,    36,    69,
      72,    81,   141,    49,    29,    41,    45,    87,     5,    61,
     293,   255,   256,   166,   297,   437,     6,   264,   265,     5,
      47,   402,   266,   404,   308,     8,    78,    47,    80,    47,
      82,    83,   518,   108,     5,     5,    88,    59,   108,     5,
     495,     5,     5,    26,    17,   267,   268,   269,   270,    32,
     536,   119,    79,    36,   479,     5,   124,     5,     5,    79,
     236,   237,   238,   239,   240,   241,   242,    53,     5,     5,
     122,   123,   142,   143,    28,   120,   121,     5,    17,     8,
     566,   567,    65,    30,    28,    58,    76,    84,    85,     5,
      76,    76,    58,    76,   519,    65,   521,    26,   523,     8,
      16,    65,    65,    32,     5,    76,    60,    36,   563,     5,
       5,   398,     4,     0,    64,     5,    64,    26,    62,    11,
     159,    60,    58,    32,   163,    41,    63,    36,    23,    25,
      58,   378,   379,    76,   186,   160,   380,   189,   190,   166,
      26,   193,   194,   195,   196,   197,   166,    76,   166,   574,
      36,   576,   179,   180,   533,     8,    65,    58,   644,   179,
     180,   179,   180,   202,   203,   449,   450,    76,    58,   453,
      60,    28,     5,    26,     5,   243,   352,    76,   354,    32,
     356,   206,   358,    36,   360,    42,     5,   232,   233,   234,
     235,    48,    49,    50,    51,    52,    53,    30,   684,   271,
       5,    65,    44,    45,    64,     5,   628,   259,    41,    54,
      55,    30,    76,   285,    56,    75,    76,    76,     5,   244,
     286,    66,   603,    76,    81,    30,   306,   307,    76,    43,
      30,   283,   257,   258,   613,    77,    26,   289,   290,   291,
     487,   488,   486,    30,    53,   731,    36,   272,    62,   274,
      64,    60,   277,    67,   326,   327,   539,    58,    72,    73,
      74,    75,    76,   546,    65,    76,   645,   294,   490,   491,
     492,   493,   651,    87,   294,   654,   415,    76,   417,    54,
      55,   434,     3,   335,   336,   324,   325,    38,   334,    72,
     328,    66,   331,    76,   333,     6,   332,   322,    19,    20,
      21,     5,    58,    24,    25,    44,    45,    76,    59,    65,
      61,    62,   599,   600,    54,    55,    37,    56,    29,    40,
      31,    54,    55,    34,    76,    65,   401,    76,   403,   708,
      63,   401,   402,   403,   404,   474,    59,   476,    77,    12,
     365,    64,   367,    64,     7,     8,   398,    10,    54,    55,
     633,    72,    59,   636,    75,    76,    54,    55,   737,    65,
      59,    58,    83,    26,    58,    86,    87,    65,    65,    32,
      58,    65,    35,    36,    37,    53,    39,    65,    59,    43,
     405,   461,   462,   627,   456,   457,    54,    55,    58,   441,
      54,    55,   464,   465,   419,    65,    76,    58,    62,   451,
      64,    65,    63,    67,   687,   688,    59,   434,    72,    73,
      74,    75,    76,    47,   434,    58,   434,    80,    76,     3,
      54,    55,    65,    87,    76,    88,    89,    76,   715,   504,
     717,    58,    58,    64,   504,    19,    20,    21,    65,    65,
      24,    25,    58,    58,    78,    60,   729,     7,     8,    65,
      10,    76,   477,    37,    14,    58,    40,   496,   497,    76,
      63,     5,   501,   502,   530,     8,    26,    10,    60,    58,
      13,    76,    32,   553,    63,    35,    36,    37,    59,    39,
      64,    59,    58,    26,    72,   537,   538,    63,    76,    32,
     570,    75,    76,    36,   142,   143,    39,    72,    42,    83,
      59,    76,    86,    87,    48,    49,    50,    51,    52,    53,
      17,    72,    73,    74,    75,    76,    64,    38,   590,   591,
      80,    42,   594,   595,   179,   180,    17,    60,    88,    89,
      26,    64,    72,   603,    30,    42,    76,    81,    76,    76,
      36,    48,    49,    50,    51,    52,    53,   599,   600,   629,
     630,    42,    79,   592,   593,    59,    59,    48,    49,    50,
      51,    52,    53,    64,   589,   617,    59,    64,    61,    62,
      59,   596,    61,    62,    81,    72,    73,    74,    75,    76,
      76,   606,    17,   635,    80,    58,    76,    60,   660,   661,
      81,    76,    88,    89,    90,    26,    17,    64,    58,    30,
      60,   676,    58,     5,    60,    36,   676,    42,    58,    53,
      60,   677,    60,    48,    49,    50,    51,    52,    53,    64,
     686,    42,    58,    76,    60,    76,   678,    48,    49,    50,
      51,    52,    53,    58,    76,    60,   702,   703,    58,    58,
      60,    60,     5,    76,    18,    76,    81,    76,    22,    80,
      58,    58,    60,    60,    58,    28,    60,    88,    89,    90,
      81,    76,    76,   715,   730,   717,   741,   742,    42,    60,
      76,   741,   742,    76,    48,    49,    50,    51,    52,    53,
     746,    26,    27,    64,    28,    33,    28,    76,    76,     9,
      76,    36,    76,    91,    42,    64,    64,    59,    64,   765,
      48,    49,    50,    51,    52,    53,    76,    81,    64,    54,
      55,    26,    27,     5,     5,    76,    61,    76,    76,    64,
       5,    36,    76,    76,    76,    76,    76,    72,    73,    74,
      75,    76,     5,    81,     5,    80,    60,     5,    59,    54,
      55,     5,    41,    43,    76,     5,    61,    28,     5,    64,
      15,    76,    30,   778,    54,    55,    62,    72,    73,    74,
      75,    76,    62,    63,    64,    80,    76,    67,    58,    43,
      53,    60,    72,    73,    74,    75,    76,    28,     5,    59,
      54,    55,    76,     9,     5,    43,     5,    87,    62,    30,
      64,    65,     5,    67,    60,     5,    54,    55,    72,    73,
      74,    75,    76,     5,    62,    60,    64,    65,    60,    67,
      60,    43,     5,    87,    72,    73,    74,    75,    76,    28,
      76,     5,    54,    55,    53,     5,    30,    43,    76,    87,
      62,    76,    64,    65,    76,    67,    76,    59,    54,    55,
      72,    73,    74,    75,    76,     5,    62,     5,    64,    65,
       5,    67,    60,    43,    60,    87,    72,    73,    74,    75,
      76,    76,    63,    17,    54,    55,     5,     5,     5,    43,
       5,    87,    62,    60,    64,    28,    76,    67,     5,     5,
      54,    55,    72,    73,    74,    75,    76,     5,    62,     5,
      64,     5,    28,    67,     5,    43,     5,    87,    72,    73,
      74,    75,    76,     5,     5,    64,    54,    55,    65,   207,
     205,    43,   778,    87,    62,   211,    64,   606,   762,    67,
     771,   209,   769,    55,    72,    73,    74,    75,    76,   596,
      62,   419,    64,    43,   375,    67,   477,   551,   628,    87,
      72,    73,    74,    75,    76,    55,   193,   439,   632,    43,
      -1,   336,    62,    -1,    64,    87,    -1,    67,    -1,    -1,
      -1,    55,    72,    73,    74,    75,    76,    -1,    62,    -1,
      64,    43,    -1,    67,    -1,    -1,    -1,    87,    72,    73,
      74,    75,    76,    55,    -1,    -1,    -1,    43,    -1,    -1,
      62,    -1,    64,    87,    -1,    67,    -1,    -1,    -1,    55,
      72,    73,    74,    75,    76,    -1,    62,    -1,    64,    -1,
      -1,    67,    -1,    -1,    -1,    87,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    87,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    65,
      48,    49,    50,    51,    52,    53,    -1,    54,    55,    -1,
      -1,    -1,    -1,    42,    -1,    81,    -1,    64,    66,    48,
      49,    50,    51,    52,    53,    72,    73,    74,    75,    76,
      -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,
      -1,    -1,    81,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    42,    -1,    -1,    -1,
      81,    38,    48,    49,    50,    51,    52,    53,    44,    45,
      46,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      56,    57,    59,    60,    61,    62,    -1,    64,    -1,    -1,
      38,    68,    69,    70,    71,    81,    -1,    44,    45,    -1,
      -1,    77,    -1,    38,    -1,    -1,    82,    54,    55,    56,
      -1,    59,    59,    61,    62,    62,    64,    64,    38,    66,
      68,    69,    70,    71,    59,    -1,    61,    62,    -1,    64,
      77,    -1,    -1,    68,    69,    70,    71,    54,    55,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    64,    68,    69,
      70,    71,    -1,    -1,    -1,    72,    73,    74,    75,    76,
     224,   225,   226,   227,   228,   229,   230,   231
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    93,    94,    97,    98,   100,    76,    95,
      76,     0,    59,     7,     8,    10,    26,    32,    35,    36,
      37,    39,    80,    88,    89,    99,   141,   163,    59,    64,
      96,     5,    72,    76,   105,    76,   106,   107,    76,   162,
      76,   139,   140,   162,   162,    76,   164,   166,    76,   112,
     113,    26,    36,    76,    76,    76,   162,     5,    12,     5,
      58,    53,    60,   107,     5,    58,    59,   151,   140,    60,
       5,    59,   151,     5,    30,     3,    19,    20,    21,    24,
      25,    37,    40,    64,    72,    75,    76,    83,    86,    87,
     169,   170,   171,   174,   175,   176,   177,   178,   181,   182,
     184,   185,   186,   187,   194,   195,   196,   197,    53,   113,
      76,    76,    59,    59,    65,    97,   101,    72,    76,    43,
      54,    55,    62,    64,    67,    72,    73,    74,    75,    76,
      87,   175,   178,   182,   199,   200,   201,   202,   203,    26,
      27,    36,    54,    55,    61,    64,    72,    73,    74,    75,
      76,    80,   111,   114,   123,   124,   125,   126,    76,    76,
      64,   152,   114,    76,     5,   152,   165,    72,    76,    76,
     164,   199,   178,   182,   198,   199,   164,   199,   199,    60,
      60,    64,   164,   199,    76,    59,    38,    59,    61,    62,
      38,    59,    61,    62,    68,    69,    70,    71,    80,   114,
      59,    59,    76,    76,    59,     8,    10,    13,    26,    32,
      36,    39,   102,   203,   201,   201,    63,   199,   204,   205,
     199,   203,    59,     5,    42,    48,    49,    50,    51,    52,
      53,    81,    47,    54,    55,    78,    44,    45,    46,    56,
      57,    77,    82,    79,    64,   157,   157,    76,   126,   126,
      76,    76,   160,   161,    59,    62,    64,    64,   119,    53,
       6,    29,    31,    34,    54,    55,    66,    44,    45,    56,
      77,   151,     8,    26,    32,    36,    65,    76,   153,   154,
     162,     5,     5,    53,    76,   151,   144,    60,   169,    38,
      42,    23,    17,    17,    58,    28,    30,    33,    65,    76,
     170,   170,    65,   179,   180,   199,    84,    85,    64,    76,
     199,    76,   183,   199,   199,    76,   183,   199,   199,   199,
     199,     5,    64,     5,    76,    76,   151,   151,   106,   162,
      98,    76,   139,    76,   112,    66,    58,    63,    65,    76,
     200,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     201,   201,    55,   202,    55,   202,    55,   202,    55,   202,
      55,   202,   202,   202,   203,     8,    26,    32,    36,    65,
     158,   159,   162,    28,    60,    58,    65,    76,    54,    55,
      64,    76,   111,   127,   128,   129,   127,   162,    26,    30,
      36,    76,    80,    88,    89,    90,   120,   162,    64,   108,
     199,    28,    62,    28,    28,   117,   128,   128,   127,   129,
     129,   129,   129,   152,   162,    76,   162,    76,   162,     5,
      65,    60,     9,   156,   108,    76,   152,    97,    76,   118,
     199,   199,   199,   167,   168,   167,   182,   188,   167,    58,
      65,    60,    91,   164,   172,   173,   164,    65,   179,    64,
      64,    58,    63,    64,    63,   162,   151,   151,   152,   152,
       5,    14,    37,   103,   151,   151,   199,   205,   202,   202,
     202,   202,   202,   162,    76,   162,    76,     5,    65,    60,
      76,   118,   161,   128,   128,   127,    64,    54,    55,    63,
      44,    45,    56,    77,    65,    65,    76,    76,     5,    26,
      36,    76,    76,    76,    60,    76,   108,   110,   199,     5,
     114,   124,   114,   124,   130,   131,   132,   162,     5,    60,
     157,    60,   157,    60,   154,     6,    76,   155,     5,    76,
     142,     5,     5,    60,     5,    59,     5,    18,    22,    17,
     169,   127,   190,   191,   192,   193,    41,   180,   199,    76,
      30,     5,    30,    41,    30,    65,    65,   179,    65,   179,
     199,    65,   179,    65,   152,   152,     5,     5,   164,   164,
      15,   104,   152,   152,    60,   157,    60,   157,   159,   155,
      28,    65,   127,   128,   128,   129,   129,   129,   129,   119,
     151,   151,    76,    76,   151,   151,    62,   121,   114,    60,
      58,     5,    65,    58,    63,    30,     5,    25,   136,    60,
     156,   155,   155,    60,   155,    28,    59,    53,     5,    97,
     118,    76,     9,   156,   199,   199,   167,    66,     5,    16,
      41,   189,    58,    60,   167,    60,    17,    60,   173,   164,
      65,    65,    65,     5,     5,    60,   156,   156,   164,    30,
       5,    60,   155,   155,    60,    76,    65,    30,   152,   152,
     151,   151,   152,   152,   153,    60,     5,   108,   108,   109,
      65,    76,   124,    28,   132,    76,   133,   145,    53,   118,
      76,    76,   199,     5,     5,     5,   143,    17,    17,   127,
     191,   164,   164,    30,   193,   167,   199,   167,    76,    30,
     156,   118,   147,   148,    59,   118,   118,     5,    60,   152,
     152,     5,     5,    63,    76,    58,    65,    60,    63,   115,
      28,    60,   114,    97,   199,   156,    97,   167,   167,    17,
     149,     5,    97,    97,     5,   118,     5,    60,   122,   108,
     108,    28,   116,   135,    76,     5,   146,     5,   167,    97,
     156,     5,     5,     5,   118,     5,    76,   114,   114,   137,
     138,   192,    28,    97,     5,   150,     5,    72,    76,     5,
      60,   135,     5,    97,   138,    64,   137,     5,   134,   130,
      65
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    92,    93,    93,    94,    94,    95,    95,    96,    96,
      97,    98,    98,    99,    99,    99,    99,    99,    99,    99,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   103,
     103,   103,   104,   104,   105,   105,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   108,   109,   109,   110,   110,
     111,   111,   111,   111,   111,   112,   112,   113,   113,   113,
     114,   114,   115,   116,   114,   114,   114,   114,   114,   114,
     114,   117,   114,   114,   114,   114,   114,   114,   118,   118,
     119,   119,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   121,   121,   122,   122,   122,   123,   123,   124,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   126,   126,   127,   127,   127,   127,   127,
     128,   128,   128,   128,   128,   129,   129,   129,   130,   131,
     131,   133,   132,   132,   134,   135,   136,   136,   136,   137,
     137,   138,   138,   139,   139,   140,   140,   140,   141,   141,
     142,   141,   143,   141,   144,   141,   145,   141,   146,   141,
     147,   141,   148,   141,   149,   141,   150,   141,   151,   152,
     152,   152,   153,   153,   154,   154,   154,   154,   154,   154,
     154,   154,   155,   155,   155,   156,   156,   157,   157,   157,
     158,   158,   159,   159,   159,   159,   159,   160,   160,   161,
     162,   162,   163,   165,   164,   166,   164,   168,   167,   169,
     169,   169,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   171,   171,   171,
     171,   172,   172,   172,   173,   173,   174,   174,   175,   175,
     175,   176,   176,   176,   177,   177,   178,   178,   178,   178,
     178,   178,   178,   178,   179,   179,   180,   180,   180,   181,
     181,   181,   181,   181,   181,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   183,   184,   185,   185,   186,   186,
     188,   187,   189,   189,   189,   190,   190,   191,   191,   192,
     192,   193,   193,   194,   195,   196,   196,   196,   197,   198,
     198,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     200,   200,   200,   200,   200,   200,   200,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     202,   202,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   204,   204,   205,   205
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,     3,
      11,     2,     0,     3,     2,     2,     2,     5,     7,     2,
       2,     0,     2,     0,     3,     3,     1,     1,     2,     1,
       4,     6,     1,     5,     3,     4,     1,     3,     3,     5,
       1,     1,     1,     1,     1,     2,     1,     4,     4,     7,
       1,     2,     0,     0,     9,     4,     2,     4,     4,     6,
       9,     0,     5,     4,     2,     4,     3,     6,     1,     3,
       0,     2,     1,     2,     4,     5,     7,     5,     5,     6,
       8,     7,     0,     3,     0,     3,     3,     1,     0,     3,
       1,     4,     3,     3,     1,     2,     2,     3,     3,     1,
       4,     3,     3,     3,     3,     1,     2,     2,     3,     3,
       1,     3,     3,     3,     3,     1,     4,     3,     2,     3,
       1,     0,     4,     0,     0,     0,     7,     5,     0,     3,
       1,     6,     0,     2,     1,     4,     6,     6,     7,     9,
       0,     9,     0,    11,     0,     7,     0,    11,     0,    13,
       0,    11,     0,    11,     0,    12,     0,    14,     0,     3,
       2,     0,     3,     1,     3,     5,     4,     3,     4,     5,
       6,     4,     1,     3,     3,     0,     3,     3,     2,     0,
       3,     1,     3,     4,     4,     3,     5,     3,     1,     1,
       3,     1,     3,     0,     4,     0,     2,     0,     2,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     5,     5,     7,
       5,     1,     3,     2,     6,     4,     1,     2,     2,     4,
       5,     1,     3,     1,     1,     3,     4,     3,     6,     5,
       6,     5,     6,     5,     3,     1,     1,     3,     5,     3,
       3,     3,     3,     3,     3,     4,     3,     2,     3,     4,
       2,     3,     1,     3,     1,     3,     2,     2,     4,     6,
       0,     7,     0,     2,     2,     3,     1,     3,     0,     3,
       1,     1,     3,     4,     4,     8,     8,     6,     4,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     3,     1,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     4,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     3,     1,
       2,     2,     2,     3,     1,     3,     1,     3
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
#line 215 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2063 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 216 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2069 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 219 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2075 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 220 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2081 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 223 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2087 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 224 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2093 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 227 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2099 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 228 "pascal.y"
     { (yyval.node) = NULL; }
#line 2105 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 231 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2111 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 234 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2117 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 235 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2123 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 238 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2129 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 239 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2135 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 240 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2141 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 241 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2147 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 242 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2153 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 243 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2159 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 244 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2165 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 247 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2171 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 250 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2177 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 251 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2183 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY id_list SEMICOLON  */
#line 254 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2189 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 255 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2195 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 256 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2201 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 257 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2207 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 258 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2213 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 259 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2219 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 262 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2225 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 263 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2231 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 264 "pascal.y"
     { (yyval.node) = NULL; }
#line 2237 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 267 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2243 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 268 "pascal.y"
     { (yyval.node) = NULL; }
#line 2249 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 271 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2255 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2261 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 273 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2267 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 274 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2273 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 277 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2279 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 278 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2285 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 281 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2291 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 282 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2297 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 285 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2303 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 286 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2309 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 287 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2315 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 288 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2321 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 291 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2327 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 292 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2333 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 295 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2339 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 296 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2345 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 299 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2351 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 300 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2357 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 301 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2363 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 302 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2369 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 303 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2375 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 306 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2381 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 307 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2387 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 310 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2393 "pascal.tab.c"
    break;

  case 58: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 311 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2399 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 312 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2405 "pascal.tab.c"
    break;

  case 60: /* type: simple_type  */
#line 315 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2411 "pascal.tab.c"
    break;

  case 61: /* type: ARROW IDENT  */
#line 316 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 62: /* $@1: %empty  */
#line 317 "pascal.y"
                                                       { }
#line 2423 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 317 "pascal.y"
                                                           { }
#line 2429 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 317 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2435 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt ARRAYSY OFSY type  */
#line 318 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2441 "pascal.tab.c"
    break;

  case 66: /* type: PROCEDURESY pf_params  */
#line 319 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2447 "pascal.tab.c"
    break;

  case 67: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 320 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 68: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 321 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 69: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 322 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 323 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 71: /* $@3: %empty  */
#line 324 "pascal.y"
                         { }
#line 2477 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 324 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt SETSY OFSY simple_type  */
#line 325 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2489 "pascal.tab.c"
    break;

  case 74: /* type: packed_opt FILESY  */
#line 326 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2495 "pascal.tab.c"
    break;

  case 75: /* type: packed_opt FILESY OFSY type  */
#line 327 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2501 "pascal.tab.c"
    break;

  case 76: /* type: CLASSSY class_members ENDSY  */
#line 328 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2507 "pascal.tab.c"
    break;

  case 77: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 329 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 78: /* result_type: IDENT  */
#line 332 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2519 "pascal.tab.c"
    break;

  case 79: /* result_type: IDENT PERIOD IDENT  */
#line 333 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2525 "pascal.tab.c"
    break;

  case 80: /* class_members: %empty  */
#line 336 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2531 "pascal.tab.c"
    break;

  case 81: /* class_members: class_members class_member  */
#line 337 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 82: /* class_member: IDENT  */
#line 340 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2543 "pascal.tab.c"
    break;

  case 83: /* class_member: IDENT SEMICOLON  */
#line 341 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2549 "pascal.tab.c"
    break;

  case 84: /* class_member: id_list COLON type SEMICOLON  */
#line 342 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2555 "pascal.tab.c"
    break;

  case 85: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 343 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2561 "pascal.tab.c"
    break;

  case 86: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 344 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 87: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 345 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2573 "pascal.tab.c"
    break;

  case 88: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 346 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2579 "pascal.tab.c"
    break;

  case 89: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 347 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2585 "pascal.tab.c"
    break;

  case 90: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 348 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2591 "pascal.tab.c"
    break;

  case 91: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 349 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2597 "pascal.tab.c"
    break;

  case 92: /* prop_index_opt: %empty  */
#line 352 "pascal.y"
    { (yyval.node) = NULL; }
#line 2603 "pascal.tab.c"
    break;

  case 93: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 353 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2609 "pascal.tab.c"
    break;

  case 94: /* prop_specs: %empty  */
#line 356 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2615 "pascal.tab.c"
    break;

  case 95: /* prop_specs: prop_specs IDENT IDENT  */
#line 357 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2621 "pascal.tab.c"
    break;

  case 96: /* prop_specs: prop_specs IDENT INTCONST  */
#line 358 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 97: /* packed_opt: PACKEDSY  */
#line 361 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2633 "pascal.tab.c"
    break;

  case 98: /* packed_opt: %empty  */
#line 362 "pascal.y"
     { (yyval.node) = NULL; }
#line 2639 "pascal.tab.c"
    break;

  case 99: /* simple_type: LPARENT enum_list RPARENT  */
#line 365 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2645 "pascal.tab.c"
    break;

  case 100: /* simple_type: IDENT  */
#line 366 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2651 "pascal.tab.c"
    break;

  case 101: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 367 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2657 "pascal.tab.c"
    break;

  case 102: /* simple_type: IDENT PERIOD IDENT  */
#line 368 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2663 "pascal.tab.c"
    break;

  case 103: /* simple_type: lo_bound DOTDOT b_expr  */
#line 369 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2669 "pascal.tab.c"
    break;

  case 104: /* lo_bound: lo_term  */
#line 372 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2675 "pascal.tab.c"
    break;

  case 105: /* lo_bound: PLUS lo_term  */
#line 373 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2681 "pascal.tab.c"
    break;

  case 106: /* lo_bound: MINUS lo_term  */
#line 374 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2687 "pascal.tab.c"
    break;

  case 107: /* lo_bound: lo_bound PLUS b_term  */
#line 375 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 108: /* lo_bound: lo_bound MINUS b_term  */
#line 376 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 109: /* lo_term: scalar_constant  */
#line 379 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2705 "pascal.tab.c"
    break;

  case 110: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 380 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2711 "pascal.tab.c"
    break;

  case 111: /* lo_term: lo_term MUL b_factor  */
#line 381 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 112: /* lo_term: lo_term IDIV b_factor  */
#line 382 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 113: /* lo_term: lo_term IMOD b_factor  */
#line 383 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2729 "pascal.tab.c"
    break;

  case 114: /* lo_term: lo_term SHIFTOP b_factor  */
#line 384 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2735 "pascal.tab.c"
    break;

  case 115: /* b_expr: b_term  */
#line 387 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2741 "pascal.tab.c"
    break;

  case 116: /* b_expr: PLUS b_term  */
#line 388 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 117: /* b_expr: MINUS b_term  */
#line 389 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2753 "pascal.tab.c"
    break;

  case 118: /* b_expr: b_expr PLUS b_term  */
#line 390 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 119: /* b_expr: b_expr MINUS b_term  */
#line 391 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 120: /* b_term: b_factor  */
#line 394 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2771 "pascal.tab.c"
    break;

  case 121: /* b_term: b_term MUL b_factor  */
#line 395 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 122: /* b_term: b_term IDIV b_factor  */
#line 396 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2783 "pascal.tab.c"
    break;

  case 123: /* b_term: b_term IMOD b_factor  */
#line 397 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 124: /* b_term: b_term SHIFTOP b_factor  */
#line 398 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2795 "pascal.tab.c"
    break;

  case 125: /* b_factor: scalar_constant  */
#line 401 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2801 "pascal.tab.c"
    break;

  case 126: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 402 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 127: /* b_factor: LPARENT b_expr RPARENT  */
#line 403 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2813 "pascal.tab.c"
    break;

  case 128: /* record_body: record_field_list record_case_opt  */
#line 406 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 129: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 409 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 130: /* record_field_list: record_field  */
#line 410 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 131: /* $@4: %empty  */
#line 413 "pascal.y"
                  { }
#line 2837 "pascal.tab.c"
    break;

  case 132: /* record_field: id_list COLON $@4 type  */
#line 413 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2843 "pascal.tab.c"
    break;

  case 133: /* record_field: %empty  */
#line 414 "pascal.y"
     { (yyval.node) = NULL; }
#line 2849 "pascal.tab.c"
    break;

  case 134: /* case_arm_mark: %empty  */
#line 417 "pascal.y"
    { }
#line 2855 "pascal.tab.c"
    break;

  case 135: /* case_open: %empty  */
#line 420 "pascal.y"
    { }
#line 2861 "pascal.tab.c"
    break;

  case 136: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 423 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 137: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 424 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 138: /* record_case_opt: %empty  */
#line 425 "pascal.y"
     { (yyval.node) = NULL; }
#line 2879 "pascal.tab.c"
    break;

  case 139: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 428 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2885 "pascal.tab.c"
    break;

  case 140: /* record_case_list: record_case_arm  */
#line 429 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 141: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 432 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2897 "pascal.tab.c"
    break;

  case 142: /* record_case_arm: %empty  */
#line 433 "pascal.y"
     { (yyval.node) = NULL; }
#line 2903 "pascal.tab.c"
    break;

  case 143: /* var_decl_list: var_decl_list var_decl  */
#line 436 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2909 "pascal.tab.c"
    break;

  case 144: /* var_decl_list: var_decl  */
#line 437 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2915 "pascal.tab.c"
    break;

  case 145: /* var_decl: id_list COLON type SEMICOLON  */
#line 440 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2921 "pascal.tab.c"
    break;

  case 146: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 441 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2927 "pascal.tab.c"
    break;

  case 147: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 442 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 2933 "pascal.tab.c"
    break;

  case 148: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 445 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2939 "pascal.tab.c"
    break;

  case 149: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 446 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2945 "pascal.tab.c"
    break;

  case 150: /* $@5: %empty  */
#line 447 "pascal.y"
                                                                               { }
#line 2951 "pascal.tab.c"
    break;

  case 151: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 447 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2957 "pascal.tab.c"
    break;

  case 152: /* $@6: %empty  */
#line 448 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2963 "pascal.tab.c"
    break;

  case 153: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 448 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2969 "pascal.tab.c"
    break;

  case 154: /* $@7: %empty  */
#line 449 "pascal.y"
                                        { }
#line 2975 "pascal.tab.c"
    break;

  case 155: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 449 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2981 "pascal.tab.c"
    break;

  case 156: /* $@8: %empty  */
#line 450 "pascal.y"
                                                                                            { }
#line 2987 "pascal.tab.c"
    break;

  case 157: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 450 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2993 "pascal.tab.c"
    break;

  case 158: /* $@9: %empty  */
#line 451 "pascal.y"
                                                                                                             { }
#line 2999 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 451 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3007 "pascal.tab.c"
    break;

  case 160: /* $@10: %empty  */
#line 454 "pascal.y"
                                                                                              { }
#line 3013 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 454 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3021 "pascal.tab.c"
    break;

  case 162: /* $@11: %empty  */
#line 457 "pascal.y"
                                                                                             { }
#line 3027 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 457 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3035 "pascal.tab.c"
    break;

  case 164: /* $@12: %empty  */
#line 460 "pascal.y"
                                                                                                    { }
#line 3041 "pascal.tab.c"
    break;

  case 165: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 460 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3049 "pascal.tab.c"
    break;

  case 166: /* $@13: %empty  */
#line 463 "pascal.y"
                                                                                                                     { }
#line 3055 "pascal.tab.c"
    break;

  case 167: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 463 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3063 "pascal.tab.c"
    break;

  case 168: /* pv_mark: %empty  */
#line 468 "pascal.y"
    { }
#line 3069 "pascal.tab.c"
    break;

  case 169: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 471 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3075 "pascal.tab.c"
    break;

  case 170: /* parameter_list_opt: LPARENT RPARENT  */
#line 472 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3081 "pascal.tab.c"
    break;

  case 171: /* parameter_list_opt: %empty  */
#line 473 "pascal.y"
     { (yyval.node) = NULL; }
#line 3087 "pascal.tab.c"
    break;

  case 172: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 476 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3093 "pascal.tab.c"
    break;

  case 173: /* parameter_decl_list: parameter_decl  */
#line 477 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3099 "pascal.tab.c"
    break;

  case 174: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 480 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3105 "pascal.tab.c"
    break;

  case 175: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 481 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3111 "pascal.tab.c"
    break;

  case 176: /* parameter_decl: VARSY id_list COLON param_type  */
#line 482 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3117 "pascal.tab.c"
    break;

  case 177: /* parameter_decl: id_list COLON param_type  */
#line 483 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3123 "pascal.tab.c"
    break;

  case 178: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 484 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3129 "pascal.tab.c"
    break;

  case 179: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 485 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3135 "pascal.tab.c"
    break;

  case 180: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 486 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3141 "pascal.tab.c"
    break;

  case 181: /* parameter_decl: IDENT id_list COLON param_type  */
#line 487 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3147 "pascal.tab.c"
    break;

  case 182: /* param_type: IDENT  */
#line 490 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3153 "pascal.tab.c"
    break;

  case 183: /* param_type: IDENT PERIOD IDENT  */
#line 491 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3159 "pascal.tab.c"
    break;

  case 184: /* param_type: ARRAYSY OFSY IDENT  */
#line 492 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3165 "pascal.tab.c"
    break;

  case 185: /* routine_directives: %empty  */
#line 495 "pascal.y"
    { (yyval.node) = NULL; }
#line 3171 "pascal.tab.c"
    break;

  case 186: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 496 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3184 "pascal.tab.c"
    break;

  case 187: /* pf_params: LPARENT pf_sections RPARENT  */
#line 506 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3190 "pascal.tab.c"
    break;

  case 188: /* pf_params: LPARENT RPARENT  */
#line 507 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3196 "pascal.tab.c"
    break;

  case 189: /* pf_params: %empty  */
#line 508 "pascal.y"
     { (yyval.node) = NULL; }
#line 3202 "pascal.tab.c"
    break;

  case 190: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 511 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3208 "pascal.tab.c"
    break;

  case 191: /* pf_sections: pf_section  */
#line 512 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3214 "pascal.tab.c"
    break;

  case 192: /* pf_section: id_list COLON param_type  */
#line 515 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3220 "pascal.tab.c"
    break;

  case 193: /* pf_section: VARSY id_list COLON param_type  */
#line 516 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3226 "pascal.tab.c"
    break;

  case 194: /* pf_section: CONSTSY id_list COLON param_type  */
#line 517 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3232 "pascal.tab.c"
    break;

  case 195: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 518 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3238 "pascal.tab.c"
    break;

  case 196: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 519 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3244 "pascal.tab.c"
    break;

  case 197: /* enum_list: enum_list COMMA enum_member  */
#line 522 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3250 "pascal.tab.c"
    break;

  case 198: /* enum_list: enum_member  */
#line 523 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3256 "pascal.tab.c"
    break;

  case 199: /* enum_member: IDENT  */
#line 526 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3262 "pascal.tab.c"
    break;

  case 200: /* id_list: id_list COMMA IDENT  */
#line 529 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3268 "pascal.tab.c"
    break;

  case 201: /* id_list: IDENT  */
#line 530 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3274 "pascal.tab.c"
    break;

  case 202: /* body: BEGINSY statement_list ENDSY  */
#line 533 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3280 "pascal.tab.c"
    break;

  case 203: /* $@14: %empty  */
#line 536 "pascal.y"
                             { pas_stmt_mark(); }
#line 3286 "pascal.tab.c"
    break;

  case 204: /* statement_list: statement_list SEMICOLON $@14 statement  */
#line 536 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3292 "pascal.tab.c"
    break;

  case 205: /* $@15: %empty  */
#line 537 "pascal.y"
     { pas_stmt_mark(); }
#line 3298 "pascal.tab.c"
    break;

  case 206: /* statement_list: $@15 statement  */
#line 537 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3304 "pascal.tab.c"
    break;

  case 207: /* $@16: %empty  */
#line 540 "pascal.y"
    { pas_stmt_mark(); }
#line 3310 "pascal.tab.c"
    break;

  case 208: /* body_stmt: $@16 statement  */
#line 540 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3316 "pascal.tab.c"
    break;

  case 209: /* statement: statement_no_label  */
#line 543 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3322 "pascal.tab.c"
    break;

  case 210: /* statement: INTCONST COLON statement_no_label  */
#line 544 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3328 "pascal.tab.c"
    break;

  case 211: /* statement: IDENT COLON statement_no_label  */
#line 545 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3334 "pascal.tab.c"
    break;

  case 212: /* statement_no_label: assignment  */
#line 548 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3340 "pascal.tab.c"
    break;

  case 213: /* statement_no_label: call  */
#line 549 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3346 "pascal.tab.c"
    break;

  case 214: /* statement_no_label: compound_statement  */
#line 550 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3352 "pascal.tab.c"
    break;

  case 215: /* statement_no_label: goto_statement  */
#line 551 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3358 "pascal.tab.c"
    break;

  case 216: /* statement_no_label: if_statement  */
#line 552 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3364 "pascal.tab.c"
    break;

  case 217: /* statement_no_label: case_statement  */
#line 553 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3370 "pascal.tab.c"
    break;

  case 218: /* statement_no_label: while_statement  */
#line 554 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3376 "pascal.tab.c"
    break;

  case 219: /* statement_no_label: repeat_statement  */
#line 555 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3382 "pascal.tab.c"
    break;

  case 220: /* statement_no_label: for_statement  */
#line 556 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3388 "pascal.tab.c"
    break;

  case 221: /* statement_no_label: with_statement  */
#line 557 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3394 "pascal.tab.c"
    break;

  case 222: /* statement_no_label: try_statement  */
#line 558 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3400 "pascal.tab.c"
    break;

  case 223: /* statement_no_label: raise_statement  */
#line 559 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3406 "pascal.tab.c"
    break;

  case 224: /* statement_no_label: inherited_call  */
#line 560 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3412 "pascal.tab.c"
    break;

  case 225: /* statement_no_label: INHERITEDSY  */
#line 561 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3418 "pascal.tab.c"
    break;

  case 226: /* statement_no_label: %empty  */
#line 562 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3424 "pascal.tab.c"
    break;

  case 227: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 565 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3430 "pascal.tab.c"
    break;

  case 228: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 566 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3436 "pascal.tab.c"
    break;

  case 229: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 567 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3442 "pascal.tab.c"
    break;

  case 230: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 568 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3448 "pascal.tab.c"
    break;

  case 231: /* handler_list: handler  */
#line 571 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3454 "pascal.tab.c"
    break;

  case 232: /* handler_list: handler_list SEMICOLON handler  */
#line 572 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3460 "pascal.tab.c"
    break;

  case 233: /* handler_list: handler_list SEMICOLON  */
#line 573 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3466 "pascal.tab.c"
    break;

  case 234: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 576 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3472 "pascal.tab.c"
    break;

  case 235: /* handler: ONSY IDENT DOSY body_stmt  */
#line 577 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3478 "pascal.tab.c"
    break;

  case 236: /* raise_statement: RAISESY  */
#line 580 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3484 "pascal.tab.c"
    break;

  case 237: /* raise_statement: RAISESY expression  */
#line 581 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3490 "pascal.tab.c"
    break;

  case 238: /* inherited_call: INHERITEDSY IDENT  */
#line 584 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3496 "pascal.tab.c"
    break;

  case 239: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 585 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3502 "pascal.tab.c"
    break;

  case 240: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 586 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3508 "pascal.tab.c"
    break;

  case 241: /* call: IDENT  */
#line 589 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3514 "pascal.tab.c"
    break;

  case 242: /* call: selector PERIOD IDENT  */
#line 590 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3520 "pascal.tab.c"
    break;

  case 243: /* call: call_with_args  */
#line 591 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3526 "pascal.tab.c"
    break;

  case 244: /* recv_primary: STRINGCONST  */
#line 594 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3532 "pascal.tab.c"
    break;

  case 245: /* recv_primary: LPARENT expression RPARENT  */
#line 595 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3538 "pascal.tab.c"
    break;

  case 246: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 598 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3544 "pascal.tab.c"
    break;

  case 247: /* call_with_args: IDENT LPARENT RPARENT  */
#line 599 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3550 "pascal.tab.c"
    break;

  case 248: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 600 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3556 "pascal.tab.c"
    break;

  case 249: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 601 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3562 "pascal.tab.c"
    break;

  case 250: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 602 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3568 "pascal.tab.c"
    break;

  case 251: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 603 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3574 "pascal.tab.c"
    break;

  case 252: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 604 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3580 "pascal.tab.c"
    break;

  case 253: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 605 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3586 "pascal.tab.c"
    break;

  case 254: /* argument_list: argument_list COMMA argument  */
#line 608 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3592 "pascal.tab.c"
    break;

  case 255: /* argument_list: argument  */
#line 609 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3598 "pascal.tab.c"
    break;

  case 256: /* argument: expression  */
#line 612 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3604 "pascal.tab.c"
    break;

  case 257: /* argument: expression COLON expression  */
#line 613 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3610 "pascal.tab.c"
    break;

  case 258: /* argument: expression COLON expression COLON expression  */
#line 614 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3616 "pascal.tab.c"
    break;

  case 259: /* assignment: selector BECOMES expression  */
#line 617 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3622 "pascal.tab.c"
    break;

  case 260: /* assignment: selector ADDBECOMES expression  */
#line 618 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3628 "pascal.tab.c"
    break;

  case 261: /* assignment: selector SUBBECOMES expression  */
#line 619 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3634 "pascal.tab.c"
    break;

  case 262: /* assignment: selector MULBECOMES expression  */
#line 620 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3640 "pascal.tab.c"
    break;

  case 263: /* assignment: selector DIVBECOMES expression  */
#line 621 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3646 "pascal.tab.c"
    break;

  case 264: /* assignment: call_with_args BECOMES expression  */
#line 622 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3652 "pascal.tab.c"
    break;

  case 265: /* selector: selector LBRACK expression_list RBRACK  */
#line 625 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3658 "pascal.tab.c"
    break;

  case 266: /* selector: selector PERIOD IDENT  */
#line 626 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3664 "pascal.tab.c"
    break;

  case 267: /* selector: selector ARROW  */
#line 627 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3670 "pascal.tab.c"
    break;

  case 268: /* selector: call_with_args PERIOD IDENT  */
#line 628 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3676 "pascal.tab.c"
    break;

  case 269: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 629 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3682 "pascal.tab.c"
    break;

  case 270: /* selector: call_with_args ARROW  */
#line 630 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3688 "pascal.tab.c"
    break;

  case 271: /* selector: recv_primary PERIOD IDENT  */
#line 631 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3694 "pascal.tab.c"
    break;

  case 272: /* selector: IDENT  */
#line 632 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3700 "pascal.tab.c"
    break;

  case 273: /* expression_list: expression_list COMMA expression  */
#line 635 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3706 "pascal.tab.c"
    break;

  case 274: /* expression_list: expression  */
#line 636 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3712 "pascal.tab.c"
    break;

  case 275: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 639 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3718 "pascal.tab.c"
    break;

  case 276: /* goto_statement: GOTOSY INTCONST  */
#line 642 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3724 "pascal.tab.c"
    break;

  case 277: /* goto_statement: GOTOSY IDENT  */
#line 643 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3730 "pascal.tab.c"
    break;

  case 278: /* if_statement: IFSY expression THENSY body_stmt  */
#line 646 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3736 "pascal.tab.c"
    break;

  case 279: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 647 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3742 "pascal.tab.c"
    break;

  case 280: /* $@17: %empty  */
#line 650 "pascal.y"
                           { }
#line 3748 "pascal.tab.c"
    break;

  case 281: /* case_statement: CASESY expression OFSY $@17 case_list case_tail ENDSY  */
#line 650 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3754 "pascal.tab.c"
    break;

  case 282: /* case_tail: %empty  */
#line 653 "pascal.y"
    { (yyval.node) = NULL; }
#line 3760 "pascal.tab.c"
    break;

  case 283: /* case_tail: ELSESY statement_list  */
#line 654 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3766 "pascal.tab.c"
    break;

  case 284: /* case_tail: OTHERWISESY statement_list  */
#line 655 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3772 "pascal.tab.c"
    break;

  case 285: /* case_list: case_list SEMICOLON case_elem  */
#line 658 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3778 "pascal.tab.c"
    break;

  case 286: /* case_list: case_elem  */
#line 659 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3784 "pascal.tab.c"
    break;

  case 287: /* case_elem: constant_list COLON body_stmt  */
#line 662 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3790 "pascal.tab.c"
    break;

  case 288: /* case_elem: %empty  */
#line 663 "pascal.y"
     { (yyval.node) = NULL; }
#line 3796 "pascal.tab.c"
    break;

  case 289: /* constant_list: constant_list COMMA case_label  */
#line 666 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3802 "pascal.tab.c"
    break;

  case 290: /* constant_list: case_label  */
#line 667 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3808 "pascal.tab.c"
    break;

  case 291: /* case_label: b_expr  */
#line 670 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3814 "pascal.tab.c"
    break;

  case 292: /* case_label: b_expr DOTDOT b_expr  */
#line 671 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3820 "pascal.tab.c"
    break;

  case 293: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 674 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3826 "pascal.tab.c"
    break;

  case 294: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 677 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3832 "pascal.tab.c"
    break;

  case 295: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 680 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3838 "pascal.tab.c"
    break;

  case 296: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 681 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3844 "pascal.tab.c"
    break;

  case 297: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 682 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3850 "pascal.tab.c"
    break;

  case 298: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 685 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3856 "pascal.tab.c"
    break;

  case 299: /* with_open: with_open COMMA selector  */
#line 688 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3862 "pascal.tab.c"
    break;

  case 300: /* with_open: selector  */
#line 689 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3868 "pascal.tab.c"
    break;

  case 301: /* expression: simple_expression  */
#line 696 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3874 "pascal.tab.c"
    break;

  case 302: /* expression: expression INOP simple_expression  */
#line 697 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3880 "pascal.tab.c"
    break;

  case 303: /* expression: expression LTOP simple_expression  */
#line 698 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3886 "pascal.tab.c"
    break;

  case 304: /* expression: expression LEOP simple_expression  */
#line 699 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3892 "pascal.tab.c"
    break;

  case 305: /* expression: expression GTOP simple_expression  */
#line 700 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 306: /* expression: expression GEOP simple_expression  */
#line 701 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3904 "pascal.tab.c"
    break;

  case 307: /* expression: expression NEOP simple_expression  */
#line 702 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3910 "pascal.tab.c"
    break;

  case 308: /* expression: expression EQOP simple_expression  */
#line 703 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3916 "pascal.tab.c"
    break;

  case 309: /* expression: expression ISOP simple_expression  */
#line 704 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3922 "pascal.tab.c"
    break;

  case 310: /* simple_expression: term  */
#line 707 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3928 "pascal.tab.c"
    break;

  case 311: /* simple_expression: PLUS term  */
#line 708 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3934 "pascal.tab.c"
    break;

  case 312: /* simple_expression: MINUS term  */
#line 709 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3940 "pascal.tab.c"
    break;

  case 313: /* simple_expression: simple_expression PLUS term  */
#line 710 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3946 "pascal.tab.c"
    break;

  case 314: /* simple_expression: simple_expression MINUS term  */
#line 711 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3952 "pascal.tab.c"
    break;

  case 315: /* simple_expression: simple_expression OROP term  */
#line 712 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3958 "pascal.tab.c"
    break;

  case 316: /* simple_expression: simple_expression XOROP term  */
#line 713 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3964 "pascal.tab.c"
    break;

  case 317: /* term: power  */
#line 716 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3970 "pascal.tab.c"
    break;

  case 318: /* term: term MUL power  */
#line 717 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3976 "pascal.tab.c"
    break;

  case 319: /* term: term RDIV power  */
#line 718 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3982 "pascal.tab.c"
    break;

  case 320: /* term: term IDIV power  */
#line 719 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3988 "pascal.tab.c"
    break;

  case 321: /* term: term IMOD power  */
#line 720 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3994 "pascal.tab.c"
    break;

  case 322: /* term: term ANDOP power  */
#line 721 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4000 "pascal.tab.c"
    break;

  case 323: /* term: term MUL MINUS power  */
#line 722 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4006 "pascal.tab.c"
    break;

  case 324: /* term: term RDIV MINUS power  */
#line 723 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4012 "pascal.tab.c"
    break;

  case 325: /* term: term IDIV MINUS power  */
#line 724 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4018 "pascal.tab.c"
    break;

  case 326: /* term: term IMOD MINUS power  */
#line 725 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4024 "pascal.tab.c"
    break;

  case 327: /* term: term ANDOP MINUS power  */
#line 726 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4030 "pascal.tab.c"
    break;

  case 328: /* term: term SHIFTOP power  */
#line 727 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4036 "pascal.tab.c"
    break;

  case 329: /* term: term ASOP power  */
#line 728 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4042 "pascal.tab.c"
    break;

  case 330: /* power: factor  */
#line 731 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4048 "pascal.tab.c"
    break;

  case 331: /* power: power POWOP factor  */
#line 732 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4054 "pascal.tab.c"
    break;

  case 332: /* factor: selector  */
#line 735 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4060 "pascal.tab.c"
    break;

  case 333: /* factor: call_with_args  */
#line 736 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4066 "pascal.tab.c"
    break;

  case 334: /* factor: INTCONST  */
#line 737 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4072 "pascal.tab.c"
    break;

  case 335: /* factor: REALCONST  */
#line 738 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4078 "pascal.tab.c"
    break;

  case 336: /* factor: STRINGCONST  */
#line 739 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4084 "pascal.tab.c"
    break;

  case 337: /* factor: CHARCODE  */
#line 740 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4090 "pascal.tab.c"
    break;

  case 338: /* factor: LPARENT expression RPARENT  */
#line 741 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 4096 "pascal.tab.c"
    break;

  case 339: /* factor: inherited_call  */
#line 742 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4102 "pascal.tab.c"
    break;

  case 340: /* factor: NOTSY factor  */
#line 743 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4108 "pascal.tab.c"
    break;

  case 341: /* factor: ATSIGN factor  */
#line 744 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4114 "pascal.tab.c"
    break;

  case 342: /* factor: LBRACK RBRACK  */
#line 745 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4120 "pascal.tab.c"
    break;

  case 343: /* factor: LBRACK set_member_list RBRACK  */
#line 746 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 344: /* set_member_list: set_member  */
#line 749 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4132 "pascal.tab.c"
    break;

  case 345: /* set_member_list: set_member_list COMMA set_member  */
#line 750 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4138 "pascal.tab.c"
    break;

  case 346: /* set_member: expression  */
#line 753 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4144 "pascal.tab.c"
    break;

  case 347: /* set_member: expression DOTDOT expression  */
#line 754 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4150 "pascal.tab.c"
    break;


#line 4154 "pascal.tab.c"

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

#line 760 "pascal.y"

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
