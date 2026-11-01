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

#include "ir/ct_arena.h"
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
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
#define YYLAST   1270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  348
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  782

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
     630,   631,   632,   633,   636,   637,   640,   643,   644,   647,
     648,   651,   651,   654,   655,   656,   659,   660,   663,   664,
     667,   668,   671,   672,   675,   678,   681,   682,   683,   686,
     689,   690,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   708,   709,   710,   711,   712,   713,   714,   717,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,   732,   733,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   750,   751,   754,   755
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

#define YYPACT_NINF (-499)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-274)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     145,   -27,    -4,   220,  -499,    91,   531,  -499,  -499,   251,
     164,  -499,  -499,   135,   152,   160,   173,   160,   160,   176,
    -499,   188,   197,   204,   219,  -499,  -499,  -499,   236,   160,
     387,   340,  -499,  -499,    36,   187,   152,  -499,  -499,    69,
     339,   160,  -499,   280,    93,   363,    23,   360,   372,   188,
    -499,   368,   381,   408,   425,  -499,   218,  -499,  -499,  -499,
     289,   830,   626,  -499,  -499,   420,   424,   410,  -499,   626,
    -499,   428,    51,  -499,  -499,   335,   432,  -499,   830,    82,
     830,  -499,   830,   830,   468,  -499,  1113,  -499,   830,   449,
    -499,  -499,  -499,  -499,  -499,  -499,   444,   243,  -499,   451,
    -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,   658,  -499,
     481,   493,   472,   477,  -499,   505,  1231,  -499,  -499,   986,
     986,   986,   703,   830,   986,  -499,  -499,  -499,   450,   495,
     449,  -499,   484,   522,    59,   151,   290,   486,  -499,   509,
    -499,   509,   406,   406,   504,   518,  -499,  -499,  -499,  -499,
    1144,   512,  -499,   516,   244,  -499,   252,   169,  -499,  -499,
     186,   592,    37,  -499,  -499,   538,   360,  -499,  -499,   416,
     248,   275,   484,   522,    33,   675,   147,   702,  1026,   431,
     431,   730,    18,  1116,   545,   525,  -499,   830,   534,  -499,
     830,   830,   539,  -499,   830,   830,   830,   830,   830,    58,
     627,   560,   561,  -499,  -499,  -499,   152,   160,  -499,   569,
     160,   570,   188,  -499,  -499,   290,   290,  -499,  1044,   253,
    -499,  1063,  -499,   573,  -499,   830,   830,   830,   830,   830,
     830,   830,   830,   986,   986,   986,   986,   894,   910,   932,
     948,   970,   986,   986,   986,   209,   593,   590,   587,   169,
     169,  -499,  -499,   228,  -499,   578,  1045,  1045,   160,   338,
     856,    67,   632,   635,  -499,   314,   314,  1045,   314,   314,
     314,   314,   410,   160,   594,   160,   598,  -499,   160,    32,
    -499,   313,   667,  -499,   856,   603,   410,  -499,   620,  -499,
     830,   830,   830,  -499,  -499,    82,  -499,  -499,  -499,  -499,
    1154,  -499,  -499,  -499,   292,  -499,  1081,   617,  -499,   746,
     625,  1116,   640,   375,  1116,  1116,  1167,   390,  1116,  1116,
    1116,  1116,  -499,   160,  -499,  -499,  -499,   410,   410,   152,
     112,   462,  -499,   160,  -499,   188,   830,   830,  -499,   465,
     645,   151,   151,   151,   151,   151,   151,   151,   151,   290,
     290,   290,   290,   986,   486,   986,   486,   986,   486,   986,
     486,   986,   486,   486,   486,  -499,   160,   638,   160,   639,
    -499,    41,  -499,   489,   642,   620,   518,  -499,  -499,   314,
     314,  1045,   652,  -499,   278,   171,  -499,   294,   297,   644,
    -499,   653,    66,   206,   660,   661,   663,  -499,   500,   872,
     687,  1116,   626,  1178,   626,  1178,   160,   171,   171,   438,
    -499,  -499,  -499,  -499,   706,   514,   509,   533,   509,   556,
      60,  -499,    13,   716,   664,   736,   737,   683,   740,   688,
     743,   616,   537,  1116,  -499,   360,  -499,   522,  1045,   708,
     830,  -499,   830,   684,   148,   178,  -499,   179,  -499,   312,
     772,   788,   830,  -499,   814,  -499,   317,   410,   410,   754,
     756,  -499,  -499,  -499,   747,   410,   410,  1116,  -499,   486,
     486,   486,   486,   486,   597,   509,   601,   509,   106,  -499,
      13,  -499,   735,  -499,   171,   171,   347,  1045,   314,   314,
    -499,   314,   314,   314,   314,  -499,  -499,  -499,  -499,  -499,
     692,   693,  -499,  -499,   709,   626,   426,   714,    53,  1063,
    -499,  -499,   403,  -499,  -499,   744,   114,  -499,   613,  -499,
      13,  -499,    13,   720,    13,  -499,   753,   723,   733,  -499,
     782,  -499,  -499,  -499,   620,  -499,   712,   784,   830,   830,
    -499,  -499,   265,   118,  -499,   628,  -499,  -499,  -499,  1097,
      68,  -499,   617,  -499,  -499,  -499,  -499,  -499,   348,  -499,
     350,  1116,  -499,   351,   786,   791,   738,  -499,  -499,   794,
     794,  -499,   777,   804,   752,    13,  -499,    13,   763,  -499,
    -499,   748,  -499,   376,   171,   171,  -499,  -499,  -499,  -499,
     447,   410,   410,  -499,  -499,   410,   410,    60,   765,   809,
     856,   856,     8,  -499,  1178,   800,  -499,   160,   759,  -499,
    -499,   664,   776,  -499,   620,  -499,   762,   764,   830,  -499,
     811,   825,  -499,   827,   664,   554,   591,  -499,  1045,  1045,
    -499,  -499,   819,  1045,  -499,  -499,   830,  -499,   775,  -499,
     191,  -499,  -499,  -499,  -499,  -499,   620,   664,   664,   794,
     795,  -499,   620,  -499,  -499,   620,  -499,  -499,  -499,   836,
     796,   410,   410,   853,   860,    42,   790,  -499,  -499,  -499,
     384,  -499,   807,   808,  -499,  -499,   103,   626,  -499,   830,
    -499,  -499,  -499,  1116,  -499,  -499,  -499,  -499,  -499,  -499,
     438,  -499,   794,   794,  -499,  -499,  -499,  1116,  -499,   855,
    -499,   664,   865,  -499,  -499,  -499,   869,  -499,  -499,   620,
     875,   817,  -499,  -499,  -499,  -499,   856,  -499,   856,   854,
    -499,  -499,   815,  -499,   878,  1116,   664,   888,  -499,  -499,
    -499,  -499,  -499,   890,   891,  -499,   893,  -499,   620,    40,
    -499,  -499,   626,   626,  1045,   879,  -499,  -499,  -499,  -499,
     895,   664,  -499,  -499,  -499,   903,  -499,   415,  -499,  -499,
     904,  -499,   647,  -499,   907,  -499,  -499,  -499,  -499,  -499,
    1045,   849,  1045,  -499,   909,  -499,  -499,   904,  -499,   160,
     851,  -499
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
       0,     0,     0,     0,     0,   335,   338,   336,   337,   273,
       0,   340,   334,   333,     0,   302,   311,   318,   331,   189,
      97,   189,     0,     0,     0,     0,    51,    54,    52,    53,
     100,    80,   109,     0,     0,    60,     0,   104,   200,   168,
       0,     0,     0,   168,   154,     0,   226,   277,   278,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,   226,
     226,     0,     0,   237,   238,     0,   272,     0,     0,   270,
       0,     0,     0,   267,     0,     0,     0,     0,     0,    80,
       0,     0,     0,   168,   168,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   341,   312,   313,   343,   347,     0,
     345,     0,   342,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    50,   105,
     106,    61,   199,     0,   198,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    71,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,   170,   201,     0,
     173,     0,   185,   145,     0,     0,   171,    12,     0,   204,
       0,     0,     0,   207,   207,     0,   281,   276,   207,   245,
     241,   210,   211,   247,     0,   255,   256,   205,   205,     0,
     271,   264,   268,     0,   275,   259,   242,     0,   260,   261,
     262,   263,    58,     0,    57,   168,   168,   171,   171,    24,
       0,    31,   168,    26,   168,    25,     0,     0,   344,   339,
     266,   303,   304,   305,   306,   307,   308,   309,   310,   316,
     314,   315,   317,     0,   321,     0,   322,     0,   323,     0,
     319,     0,   320,   329,   330,   332,     0,     0,     0,     0,
     188,     0,   191,     0,     0,     0,     0,    99,   102,     0,
       0,     0,    50,   125,     0,   115,   120,     0,     0,     0,
      76,     0,    82,     0,     0,     0,     0,    81,     0,     0,
       0,    42,    98,     0,    98,     0,   133,   107,   108,   103,
     112,   113,   111,   114,     0,     0,   189,     0,   189,     0,
       0,   169,     0,     0,   150,     0,     0,     0,     0,    78,
       0,     0,     0,   295,   294,   226,   299,   300,   289,   279,
       0,   246,     0,     0,     0,     0,   231,     0,   239,     0,
       0,     0,     0,   269,     0,   265,     0,   171,   171,     0,
       0,    23,   205,   205,    33,   171,   171,   348,   346,   326,
     327,   328,   324,   325,     0,   189,     0,   189,     0,   187,
       0,    67,    68,   197,   116,   117,     0,     0,     0,     0,
     101,     0,     0,     0,     0,   110,    80,   168,   168,    83,
       0,     0,   168,   168,    92,    98,   273,     0,     0,    42,
      41,    65,     0,    75,    73,     0,   138,   130,     0,   185,
       0,   174,     0,     0,     0,   172,     0,   182,   177,   148,
       0,    12,   146,   147,     0,   155,     0,   185,     0,     0,
     207,   208,   292,   283,   287,     0,   291,   207,   254,   257,
       0,   227,   233,   228,   205,   230,   240,   253,     0,   251,
       0,   274,   249,     0,    80,     0,     0,   185,   185,    29,
      30,   205,     0,     0,     0,     0,   195,     0,     0,   190,
     192,     0,   127,     0,   118,   119,   122,   123,   121,   124,
       0,   171,   171,   168,   168,   171,   171,     0,     0,     0,
       0,     0,     0,    44,     0,     0,    72,   133,     0,   128,
     131,   156,   178,   176,     0,   181,     0,     0,     0,   186,
       0,     0,    79,     0,   152,     0,     0,   298,     0,   289,
     205,   205,     0,     0,   207,   280,     0,   207,     0,   232,
       0,   252,   250,   248,    59,   185,     0,   160,   162,    32,
       0,    27,     0,   194,   193,     0,    69,   126,    77,     0,
       0,   171,   171,     0,     0,     0,     0,    84,    48,    46,
       0,    45,     0,     0,    62,   129,     0,    98,    12,     0,
     175,   184,   183,   179,   151,   185,   149,    12,   207,   207,
     293,   286,   285,   284,   282,   290,   288,   258,   235,     0,
     229,   164,     0,    12,    12,    20,     0,   196,    85,     0,
       0,     0,    87,    88,    93,    94,     0,    43,     0,     0,
      63,   135,     0,   132,     0,   180,   158,     0,   297,   296,
     207,    12,   185,     0,     0,    28,     0,    89,     0,     0,
      47,    49,    98,    98,   142,     0,   157,    12,   153,   234,
       0,   166,   161,   163,    86,     0,    91,     0,    70,    64,
     137,   140,     0,   135,     0,   165,    12,    90,    96,    95,
     142,     0,   142,   159,     0,   139,   134,   136,   167,   133,
       0,   141
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -499,  -499,  -499,  -499,  -499,   -56,   711,  -499,  -499,  -499,
    -499,  -499,  -499,  -499,   715,   -20,  -279,  -499,  -499,   -60,
     710,   -32,   -65,  -499,  -499,  -499,  -355,  -460,  -499,  -499,
    -499,  -499,  -378,  -499,   389,  -205,  -199,  -237,   146,  -499,
     326,  -499,  -499,   161,  -499,   163,   168,   731,   -35,  -499,
    -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,   -38,
     -62,   343,   530,  -387,  -498,  -117,  -499,   464,  -499,   575,
     -15,  -499,   -69,  -499,  -499,  -268,  -499,  -151,   150,  -499,
    -499,   400,  -499,   -36,  -499,  -499,   -25,  -265,   515,  -499,
     -18,   760,  -499,  -499,  -499,  -499,  -499,  -499,  -499,   328,
    -425,   327,  -499,  -499,  -499,  -499,  -499,   -23,   398,   -33,
    -162,   -84,  -499,   634
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    30,     5,     6,    25,     7,   116,
     213,   464,   572,    34,    36,    37,   400,   670,   508,   383,
      49,    50,   153,   720,   743,   406,   430,   259,   397,   598,
     739,   154,   155,   156,   157,   542,   385,   386,   515,   516,
     517,   677,   779,   744,   609,   760,   761,    41,    42,    26,
     531,   687,   287,   678,   747,   703,   704,   731,   766,    67,
     161,   279,   280,   528,   424,   246,   371,   372,   253,   254,
      43,    27,    46,   166,    47,   434,   435,    90,    91,    92,
     445,   446,    93,   131,    95,    96,   132,   304,   305,    98,
     133,   313,   100,   101,   102,   103,   438,   632,   543,   544,
     762,   546,   104,   105,   106,   107,   174,   306,   135,   136,
     137,   138,   219,   220
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   115,   152,    44,   162,   425,    68,    72,   170,   152,
     165,    94,   176,   545,    56,   289,    63,   109,   182,   526,
     482,   611,    97,    73,   247,   512,   436,   514,    73,    99,
     439,   410,   411,   412,   413,   214,   590,   420,   134,   624,
     222,    59,   283,   200,   449,   756,   478,   420,   152,     8,
     294,   384,   387,    74,   172,   171,   164,   175,   602,   177,
     178,   173,   409,   322,   224,   183,   407,   408,   273,   647,
     648,   499,    10,   671,    64,   354,   356,   358,   360,   362,
     363,   364,   152,   152,   672,   637,   274,   215,   216,   527,
     284,   295,   275,   580,    60,   402,   276,   421,    70,   218,
     221,   225,   307,   308,   590,   714,   479,   226,   227,   228,
     229,   230,   231,   285,   366,   160,   757,   461,   603,   607,
     507,   272,   323,   629,  -201,   286,  -201,    65,   638,   403,
      94,   721,   367,   612,   630,   613,   278,   615,   368,   608,
     232,    97,   369,    94,    94,   281,    83,   701,    99,     1,
      12,    65,    73,    73,    97,    97,     2,    85,   129,   631,
     365,    99,    99,   722,   311,   327,   328,   314,   315,    31,
      65,   314,   318,   319,   320,   321,   486,   297,   551,   621,
     484,   485,    38,   552,    73,   558,   560,   726,   653,   563,
     654,   469,   330,   470,   273,   471,    73,   472,   233,   473,
     349,   350,   351,   352,   545,   234,   235,    32,   553,   555,
     414,    33,   274,   268,   269,   491,   492,   366,   275,   554,
      11,   700,   276,    51,   427,   270,   673,   493,    35,   236,
     373,   428,   500,    52,   751,   367,    38,   401,   444,   447,
      61,   368,   501,   388,   398,   369,   271,    62,   494,    40,
     261,   277,    45,    73,   586,   587,   588,   589,   415,   680,
     417,   401,   278,   419,    48,   459,   460,   431,   432,   433,
     172,   292,   627,   262,   370,   263,    65,   437,   264,   635,
      53,   187,   583,   114,   541,    38,   376,   457,   458,   584,
     585,   702,   293,   377,   465,    54,   466,   706,    68,   521,
     707,   523,   188,   109,   189,   190,   265,   266,   456,    63,
      28,   337,    55,   467,   218,    29,   338,   225,   267,   488,
     489,   668,   669,   226,   227,   228,   229,   230,   231,   301,
     302,   628,   488,   489,   237,   238,   239,   511,    65,   513,
      69,   490,   152,   152,   152,   152,   240,   241,   488,   489,
     440,   474,    58,   476,   736,    65,   232,   441,   576,   495,
     578,   117,   496,    75,   389,   118,   696,   242,   390,   698,
     440,    65,   243,   422,   391,    65,   509,   556,   381,    76,
      77,    78,   564,   755,    79,    80,   146,   147,   148,   149,
     382,   518,    57,   569,   570,   565,   566,    81,    66,    94,
      82,   488,   489,   573,   574,   281,   440,   167,   440,   440,
      97,   168,   582,   641,   392,   642,   643,    99,   393,   549,
     728,   729,    71,   690,    83,   108,   394,   395,   396,   561,
     488,   489,    84,   452,    75,    85,    86,   740,   453,   741,
     599,   657,   716,    87,   110,   152,    88,    89,   452,   717,
      76,    77,    78,   455,   290,    79,    80,   111,   291,   591,
     592,   604,   749,   373,   595,   596,   605,   112,    81,    13,
      14,    82,    15,   389,   160,   620,   462,   658,   146,   147,
     148,   149,   248,   391,   113,   640,   600,   768,    16,   191,
     181,   769,   488,   489,    17,    83,   158,    18,    19,   463,
     159,    21,   649,   185,   163,   186,    85,   300,   169,  -244,
     192,  -244,   193,   194,    87,   625,   626,    88,    89,   195,
     196,   197,   198,   392,  -245,   184,  -245,   393,   179,   659,
     660,   249,   250,   663,   664,   394,   395,   396,    13,    14,
     201,    15,    22,   188,   152,   189,   190,    65,   203,   480,
      23,    24,   202,   204,   540,   661,   662,    16,    65,   181,
     505,   692,   693,    17,   205,   244,    18,    19,    20,   260,
      21,   688,    65,   245,   520,   398,   258,   401,   401,   225,
     251,   223,   281,   193,   194,   226,   227,   228,   229,   230,
     231,    65,   518,   522,   252,   683,   225,   282,   288,   710,
     711,   310,   226,   227,   228,   229,   230,   231,   689,   309,
     312,    22,   723,   697,    65,   316,   524,   152,   232,    23,
      24,   374,   724,   341,   342,   343,   344,   345,   346,   347,
     348,   727,   324,   225,   538,   232,   325,   326,   539,   226,
     227,   228,   229,   230,   231,   332,   334,   733,   734,   340,
     375,   257,   139,   140,   378,    65,   725,   575,   225,    65,
     404,   577,   141,   405,   226,   227,   228,   229,   230,   231,
     416,    65,   232,   610,   418,   750,   423,   758,   759,   426,
     142,   143,   152,   152,   139,   140,   633,   144,   634,   450,
     145,   764,   510,   401,   141,   401,   429,   232,   146,   147,
     148,   149,   150,   296,   451,   633,   151,   771,   443,   454,
     774,   519,   142,   143,   475,   477,   487,   225,   481,   144,
     497,   529,   145,   226,   227,   228,   229,   230,   231,   498,
     146,   147,   148,   149,   150,   298,   502,   503,   199,   504,
     530,   532,   533,   534,   225,   535,   119,   536,   537,   547,
     226,   227,   228,   229,   230,   231,   232,   120,   121,   567,
     550,   568,   571,   581,   518,   122,   217,   123,   593,   594,
     124,   597,   601,   119,   606,   125,   126,   127,   128,   129,
     614,   616,   617,   232,   120,   121,   618,   619,   622,   119,
     130,   644,   122,   623,   123,   303,   645,   124,   646,    73,
     120,   121,   125,   126,   127,   128,   129,   650,   122,   651,
     123,   448,   652,   124,   667,   119,   684,   130,   125,   126,
     127,   128,   129,   655,   656,   666,   120,   121,   674,   679,
     685,   119,   686,   130,   122,   676,   123,   557,   681,   124,
     682,   708,   120,   121,   125,   126,   127,   128,   129,   694,
     122,   699,   123,   559,   705,   124,   709,   119,   712,   130,
     125,   126,   127,   128,   129,   713,   715,   718,   120,   121,
     732,   719,   730,   119,   735,   130,   122,   738,   123,   562,
     737,   124,   742,   746,   120,   121,   125,   126,   127,   128,
     129,   745,   122,   748,   123,   752,   753,   124,   754,   119,
     765,   130,   125,   126,   127,   128,   129,   763,   767,   770,
     120,   121,   773,   776,   778,   119,   781,   130,   122,   331,
     399,   329,   335,   124,   772,   780,   120,   121,   125,   126,
     127,   128,   129,   675,   122,   777,   399,   119,   775,   124,
     665,   333,   579,   130,   125,   126,   127,   128,   506,   353,
     525,   483,   639,   119,   317,   548,   122,   691,   123,   130,
     695,   124,     0,     0,     0,   355,   125,   126,   127,   128,
     129,   468,   122,     0,   123,   119,     0,   124,     0,     0,
       0,   130,   125,   126,   127,   128,   129,   357,     0,     0,
       0,   119,     0,     0,   122,     0,   123,   130,     0,   124,
       0,     0,     0,   359,   125,   126,   127,   128,   129,     0,
     122,     0,   123,   119,     0,   124,     0,     0,     0,   130,
     125,   126,   127,   128,   129,   361,     0,     0,     0,   119,
       0,     0,   122,     0,   123,   130,     0,   124,     0,     0,
       0,     0,   125,   126,   127,   128,   129,     0,   122,     0,
     123,     0,     0,   124,     0,     0,     0,   130,   125,   126,
     127,   128,   129,     0,     0,     0,     0,     0,   225,     0,
       0,     0,     0,   130,   226,   227,   228,   229,   230,   231,
       0,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,   299,   226,   227,   228,   229,   230,   231,     0,   379,
     380,     0,     0,     0,     0,   225,     0,   232,     0,   381,
     336,   226,   227,   228,   229,   230,   231,   146,   147,   148,
     149,   382,     0,   225,     0,   232,     0,     0,   339,   226,
     227,   228,   229,   230,   231,     0,     0,     0,     0,   225,
       0,   442,     0,     0,   232,   226,   227,   228,   229,   230,
     231,  -273,     0,     0,     0,     0,     0,   636,   225,     0,
       0,     0,   232,     0,   226,   227,   228,   229,   230,   231,
       0,     0,  -273,   180,  -273,  -273,     0,   181,   232,     0,
       0,  -273,  -273,  -273,  -273,     0,     0,     0,   -50,   -50,
       0,     0,  -273,     0,     0,     0,     0,   232,   -50,   -50,
     -50,     0,     0,   255,     0,  -266,   256,     0,   257,     0,
     -50,     0,     0,  -273,     0,  -273,  -273,     0,   181,     0,
       0,   -50,  -273,  -273,  -273,  -273,  -266,     0,  -266,  -266,
       0,   454,   142,   143,     0,  -266,  -266,  -266,  -266,   206,
       0,   207,   145,     0,   208,     0,     0,     0,     0,     0,
     146,   147,   148,   149,   150,     0,     0,   209,     0,     0,
       0,     0,     0,   210,     0,     0,     0,   211,     0,     0,
     212
};

static const yytype_int16 yycheck[] =
{
      15,    57,    62,    18,    69,   284,    41,    45,    77,    69,
      72,    47,    81,   438,    29,   166,    36,    49,    87,     6,
     375,   519,    47,     5,   141,   403,   294,   405,     5,    47,
     298,   268,   269,   270,   271,   119,   496,     5,    61,   537,
     124,     5,     5,   108,   309,     5,     5,     5,   108,    76,
      17,   256,   257,    30,    79,    78,     5,    80,     5,    82,
      83,    79,   267,     5,     5,    88,   265,   266,     8,   567,
     568,     5,    76,    65,     5,   237,   238,   239,   240,   241,
     242,   243,   142,   143,    76,    17,    26,   120,   121,    76,
      53,    58,    32,   480,    58,    28,    36,    65,     5,   122,
     123,    42,    84,    85,   564,    63,    65,    48,    49,    50,
      51,    52,    53,    76,     8,    64,    76,     5,    65,     5,
     399,   159,    64,     5,    58,   163,    60,    58,    60,    62,
     166,    28,    26,   520,    16,   522,    76,   524,    32,    25,
      81,   166,    36,   179,   180,   160,    64,   645,   166,     4,
      59,    58,     5,     5,   179,   180,    11,    75,    76,    41,
     244,   179,   180,    60,   187,   203,   204,   190,   191,     5,
      58,   194,   195,   196,   197,   198,   381,    30,    30,   534,
     379,   380,    76,     5,     5,   450,   451,   685,   575,   454,
     577,   353,   207,   355,     8,   357,     5,   359,    47,   361,
     233,   234,   235,   236,   629,    54,    55,    72,    30,    30,
     272,    76,    26,    44,    45,    44,    45,     8,    32,    41,
       0,    30,    36,    26,   286,    56,   604,    56,    76,    78,
     245,   287,    26,    36,   732,    26,    76,   260,   307,   308,
      53,    32,    36,   258,   259,    36,    77,    60,    77,    76,
       6,    65,    76,     5,   491,   492,   493,   494,   273,   614,
     275,   284,    76,   278,    76,   327,   328,   290,   291,   292,
     295,    23,   540,    29,    65,    31,    58,   295,    34,   547,
      76,    38,   487,    65,   435,    76,    58,   325,   326,   488,
     489,   646,    17,    65,   332,    76,   334,   652,   333,   416,
     655,   418,    59,   335,    61,    62,    54,    55,   323,   329,
      59,    58,    76,   336,   337,    64,    63,    42,    66,    54,
      55,   600,   601,    48,    49,    50,    51,    52,    53,   179,
     180,    66,    54,    55,    44,    45,    46,   402,    58,   404,
      60,    63,   402,   403,   404,   405,    56,    57,    54,    55,
      58,   366,    12,   368,   709,    58,    81,    65,   475,    65,
     477,    72,    65,     3,    26,    76,   634,    77,    30,   637,
      58,    58,    82,    60,    36,    58,   399,    65,    64,    19,
      20,    21,    65,   738,    24,    25,    72,    73,    74,    75,
      76,   406,     5,   462,   463,   457,   458,    37,    59,   435,
      40,    54,    55,   465,   466,   420,    58,    72,    58,    58,
     435,    76,    65,    65,    76,    65,    65,   435,    80,   442,
     688,   689,    59,   628,    64,    53,    88,    89,    90,   452,
      54,    55,    72,    58,     3,    75,    76,   716,    63,   718,
     505,    65,    58,    83,    76,   505,    86,    87,    58,    65,
      19,    20,    21,    63,    38,    24,    25,    76,    42,   497,
     498,    58,   730,   478,   502,   503,    63,    59,    37,     7,
       8,    40,    10,    26,    64,   531,    14,    30,    72,    73,
      74,    75,    76,    36,    59,   554,    60,    72,    26,    38,
      64,    76,    54,    55,    32,    64,    76,    35,    36,    37,
      76,    39,   571,    59,    76,    61,    75,    76,    76,    59,
      59,    61,    61,    62,    83,   538,   539,    86,    87,    68,
      69,    70,    71,    76,    59,    76,    61,    80,    60,   591,
     592,   142,   143,   595,   596,    88,    89,    90,     7,     8,
      59,    10,    80,    59,   604,    61,    62,    58,    76,    60,
      88,    89,    59,    76,    17,   593,   594,    26,    58,    64,
      60,   630,   631,    32,    59,    79,    35,    36,    37,    53,
      39,    17,    58,    64,    60,   590,    64,   600,   601,    42,
      76,    59,   597,    61,    62,    48,    49,    50,    51,    52,
      53,    58,   607,    60,    76,   618,    42,     5,    60,   661,
     662,    76,    48,    49,    50,    51,    52,    53,    17,    64,
      76,    80,   677,   636,    58,    76,    60,   677,    81,    88,
      89,    28,   678,   225,   226,   227,   228,   229,   230,   231,
     232,   687,     5,    42,    18,    81,    76,    76,    22,    48,
      49,    50,    51,    52,    53,    76,    76,   703,   704,    76,
      60,    64,    26,    27,    76,    58,   679,    60,    42,    58,
      28,    60,    36,    28,    48,    49,    50,    51,    52,    53,
      76,    58,    81,    60,    76,   731,     9,   742,   743,    76,
      54,    55,   742,   743,    26,    27,    58,    61,    60,    64,
      64,   747,     5,   716,    36,   718,    76,    81,    72,    73,
      74,    75,    76,    28,    64,    58,    80,    60,    91,    64,
     766,     5,    54,    55,    76,    76,    64,    42,    76,    61,
      76,     5,    64,    48,    49,    50,    51,    52,    53,    76,
      72,    73,    74,    75,    76,    33,    76,    76,    80,    76,
      76,     5,     5,    60,    42,     5,    43,    59,     5,    41,
      48,    49,    50,    51,    52,    53,    81,    54,    55,     5,
      76,     5,    15,    28,   779,    62,    63,    64,    76,    76,
      67,    62,    58,    43,    30,    72,    73,    74,    75,    76,
      60,    28,    59,    81,    54,    55,    53,     5,    76,    43,
      87,     5,    62,     9,    64,    65,     5,    67,    60,     5,
      54,    55,    72,    73,    74,    75,    76,    30,    62,     5,
      64,    65,    60,    67,     5,    43,     5,    87,    72,    73,
      74,    75,    76,    60,    76,    60,    54,    55,    28,    53,
       5,    43,     5,    87,    62,    76,    64,    65,    76,    67,
      76,     5,    54,    55,    72,    73,    74,    75,    76,    30,
      62,    76,    64,    65,    59,    67,    60,    43,     5,    87,
      72,    73,    74,    75,    76,     5,    76,    60,    54,    55,
       5,    63,    17,    43,     5,    87,    62,    60,    64,    65,
       5,    67,    28,     5,    54,    55,    72,    73,    74,    75,
      76,    76,    62,     5,    64,     5,     5,    67,     5,    43,
       5,    87,    72,    73,    74,    75,    76,    28,     5,     5,
      54,    55,     5,    64,     5,    43,    65,    87,    62,   208,
      64,   206,   212,    67,   763,   779,    54,    55,    72,    73,
      74,    75,    76,   607,    62,   772,    64,    43,   770,    67,
     597,   210,   478,    87,    72,    73,    74,    75,    76,    55,
     420,   376,   552,    43,   194,   440,    62,   629,    64,    87,
     633,    67,    -1,    -1,    -1,    55,    72,    73,    74,    75,
      76,   337,    62,    -1,    64,    43,    -1,    67,    -1,    -1,
      -1,    87,    72,    73,    74,    75,    76,    55,    -1,    -1,
      -1,    43,    -1,    -1,    62,    -1,    64,    87,    -1,    67,
      -1,    -1,    -1,    55,    72,    73,    74,    75,    76,    -1,
      62,    -1,    64,    43,    -1,    67,    -1,    -1,    -1,    87,
      72,    73,    74,    75,    76,    55,    -1,    -1,    -1,    43,
      -1,    -1,    62,    -1,    64,    87,    -1,    67,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    62,    -1,
      64,    -1,    -1,    67,    -1,    -1,    -1,    87,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    87,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    65,    48,    49,    50,    51,    52,    53,    -1,    54,
      55,    -1,    -1,    -1,    -1,    42,    -1,    81,    -1,    64,
      66,    48,    49,    50,    51,    52,    53,    72,    73,    74,
      75,    76,    -1,    42,    -1,    81,    -1,    -1,    65,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    42,
      -1,    60,    -1,    -1,    81,    48,    49,    50,    51,    52,
      53,    38,    -1,    -1,    -1,    -1,    -1,    60,    42,    -1,
      -1,    -1,    81,    -1,    48,    49,    50,    51,    52,    53,
      -1,    -1,    59,    60,    61,    62,    -1,    64,    81,    -1,
      -1,    68,    69,    70,    71,    -1,    -1,    -1,    44,    45,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    81,    54,    55,
      56,    -1,    -1,    59,    -1,    38,    62,    -1,    64,    -1,
      66,    -1,    -1,    59,    -1,    61,    62,    -1,    64,    -1,
      -1,    77,    68,    69,    70,    71,    59,    -1,    61,    62,
      -1,    64,    54,    55,    -1,    68,    69,    70,    71,     8,
      -1,    10,    64,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    36,    -1,    -1,
      39
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
      60,    64,   164,   199,    76,    59,    61,    38,    59,    61,
      62,    38,    59,    61,    62,    68,    69,    70,    71,    80,
     114,    59,    59,    76,    76,    59,     8,    10,    13,    26,
      32,    36,    39,   102,   203,   201,   201,    63,   199,   204,
     205,   199,   203,    59,     5,    42,    48,    49,    50,    51,
      52,    53,    81,    47,    54,    55,    78,    44,    45,    46,
      56,    57,    77,    82,    79,    64,   157,   157,    76,   126,
     126,    76,    76,   160,   161,    59,    62,    64,    64,   119,
      53,     6,    29,    31,    34,    54,    55,    66,    44,    45,
      56,    77,   151,     8,    26,    32,    36,    65,    76,   153,
     154,   162,     5,     5,    53,    76,   151,   144,    60,   169,
      38,    42,    23,    17,    17,    58,    28,    30,    33,    65,
      76,   170,   170,    65,   179,   180,   199,    84,    85,    64,
      76,   199,    76,   183,   199,   199,    76,   183,   199,   199,
     199,   199,     5,    64,     5,    76,    76,   151,   151,   106,
     162,    98,    76,   139,    76,   112,    66,    58,    63,    65,
      76,   200,   200,   200,   200,   200,   200,   200,   200,   201,
     201,   201,   201,    55,   202,    55,   202,    55,   202,    55,
     202,    55,   202,   202,   202,   203,     8,    26,    32,    36,
      65,   158,   159,   162,    28,    60,    58,    65,    76,    54,
      55,    64,    76,   111,   127,   128,   129,   127,   162,    26,
      30,    36,    76,    80,    88,    89,    90,   120,   162,    64,
     108,   199,    28,    62,    28,    28,   117,   128,   128,   127,
     129,   129,   129,   129,   152,   162,    76,   162,    76,   162,
       5,    65,    60,     9,   156,   108,    76,   152,    97,    76,
     118,   199,   199,   199,   167,   168,   167,   182,   188,   167,
      58,    65,    60,    91,   164,   172,   173,   164,    65,   179,
      64,    64,    58,    63,    64,    63,   162,   151,   151,   152,
     152,     5,    14,    37,   103,   151,   151,   199,   205,   202,
     202,   202,   202,   202,   162,    76,   162,    76,     5,    65,
      60,    76,   118,   161,   128,   128,   127,    64,    54,    55,
      63,    44,    45,    56,    77,    65,    65,    76,    76,     5,
      26,    36,    76,    76,    76,    60,    76,   108,   110,   199,
       5,   114,   124,   114,   124,   130,   131,   132,   162,     5,
      60,   157,    60,   157,    60,   154,     6,    76,   155,     5,
      76,   142,     5,     5,    60,     5,    59,     5,    18,    22,
      17,   169,   127,   190,   191,   192,   193,    41,   180,   199,
      76,    30,     5,    30,    41,    30,    65,    65,   179,    65,
     179,   199,    65,   179,    65,   152,   152,     5,     5,   164,
     164,    15,   104,   152,   152,    60,   157,    60,   157,   159,
     155,    28,    65,   127,   128,   128,   129,   129,   129,   129,
     119,   151,   151,    76,    76,   151,   151,    62,   121,   114,
      60,    58,     5,    65,    58,    63,    30,     5,    25,   136,
      60,   156,   155,   155,    60,   155,    28,    59,    53,     5,
      97,   118,    76,     9,   156,   199,   199,   167,    66,     5,
      16,    41,   189,    58,    60,   167,    60,    17,    60,   173,
     164,    65,    65,    65,     5,     5,    60,   156,   156,   164,
      30,     5,    60,   155,   155,    60,    76,    65,    30,   152,
     152,   151,   151,   152,   152,   153,    60,     5,   108,   108,
     109,    65,    76,   124,    28,   132,    76,   133,   145,    53,
     118,    76,    76,   199,     5,     5,     5,   143,    17,    17,
     127,   191,   164,   164,    30,   193,   167,   199,   167,    76,
      30,   156,   118,   147,   148,    59,   118,   118,     5,    60,
     152,   152,     5,     5,    63,    76,    58,    65,    60,    63,
     115,    28,    60,   114,    97,   199,   156,    97,   167,   167,
      17,   149,     5,    97,    97,     5,   118,     5,    60,   122,
     108,   108,    28,   116,   135,    76,     5,   146,     5,   167,
      97,   156,     5,     5,     5,   118,     5,    76,   114,   114,
     137,   138,   192,    28,    97,     5,   150,     5,    72,    76,
       5,    60,   135,     5,    97,   138,    64,   137,     5,   134,
     130,    65
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
     182,   182,   182,   182,   183,   183,   184,   185,   185,   186,
     186,   188,   187,   189,   189,   189,   190,   190,   191,   191,
     192,   192,   193,   193,   194,   195,   196,   196,   196,   197,
     198,   198,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   202,   202,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   204,   204,   205,   205
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
       2,     3,     2,     1,     3,     1,     3,     2,     2,     4,
       6,     0,     7,     0,     2,     2,     3,     1,     3,     0,
       3,     1,     1,     3,     4,     4,     8,     8,     6,     4,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     2,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     4,     4,     4,     4,     4,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       1,     2,     2,     2,     3,     1,     3,     1,     3
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
#line 2071 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 216 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2077 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 219 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2083 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 220 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2089 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 223 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2095 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 224 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2101 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 227 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2107 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 228 "pascal.y"
     { (yyval.node) = NULL; }
#line 2113 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 231 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2119 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 234 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2125 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 235 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2131 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 238 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2137 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 239 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2143 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 240 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2149 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 241 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2155 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 242 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2161 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 243 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2167 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 244 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2173 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 247 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2179 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 250 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2185 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 251 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2191 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY id_list SEMICOLON  */
#line 254 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2197 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 255 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2203 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 256 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2209 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 257 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2215 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 258 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2221 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 259 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2227 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 262 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2233 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 263 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2239 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 264 "pascal.y"
     { (yyval.node) = NULL; }
#line 2245 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 267 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2251 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 268 "pascal.y"
     { (yyval.node) = NULL; }
#line 2257 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 271 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2263 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2269 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 273 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2275 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 274 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2281 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 277 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2287 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 278 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2293 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 281 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2299 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 282 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2305 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 285 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2311 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 286 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2317 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 287 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2323 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 288 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2329 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 291 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2335 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 292 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2341 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 295 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2347 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 296 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2353 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 299 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2359 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 300 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2365 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 301 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2371 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 302 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2377 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 303 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2383 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 306 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2389 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 307 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2395 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 310 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2401 "pascal.tab.c"
    break;

  case 58: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 311 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2407 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 312 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2413 "pascal.tab.c"
    break;

  case 60: /* type: simple_type  */
#line 315 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2419 "pascal.tab.c"
    break;

  case 61: /* type: ARROW IDENT  */
#line 316 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 62: /* $@1: %empty  */
#line 317 "pascal.y"
                                                       { }
#line 2431 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 317 "pascal.y"
                                                           { }
#line 2437 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 317 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2443 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt ARRAYSY OFSY type  */
#line 318 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2449 "pascal.tab.c"
    break;

  case 66: /* type: PROCEDURESY pf_params  */
#line 319 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2455 "pascal.tab.c"
    break;

  case 67: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 320 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2461 "pascal.tab.c"
    break;

  case 68: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 321 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2467 "pascal.tab.c"
    break;

  case 69: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 322 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2473 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 323 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2479 "pascal.tab.c"
    break;

  case 71: /* $@3: %empty  */
#line 324 "pascal.y"
                         { }
#line 2485 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 324 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt SETSY OFSY simple_type  */
#line 325 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2497 "pascal.tab.c"
    break;

  case 74: /* type: packed_opt FILESY  */
#line 326 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2503 "pascal.tab.c"
    break;

  case 75: /* type: packed_opt FILESY OFSY type  */
#line 327 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2509 "pascal.tab.c"
    break;

  case 76: /* type: CLASSSY class_members ENDSY  */
#line 328 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2515 "pascal.tab.c"
    break;

  case 77: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 329 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2521 "pascal.tab.c"
    break;

  case 78: /* result_type: IDENT  */
#line 332 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2527 "pascal.tab.c"
    break;

  case 79: /* result_type: IDENT PERIOD IDENT  */
#line 333 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2533 "pascal.tab.c"
    break;

  case 80: /* class_members: %empty  */
#line 336 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2539 "pascal.tab.c"
    break;

  case 81: /* class_members: class_members class_member  */
#line 337 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 82: /* class_member: IDENT  */
#line 340 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2551 "pascal.tab.c"
    break;

  case 83: /* class_member: IDENT SEMICOLON  */
#line 341 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2557 "pascal.tab.c"
    break;

  case 84: /* class_member: id_list COLON type SEMICOLON  */
#line 342 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2563 "pascal.tab.c"
    break;

  case 85: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 343 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2569 "pascal.tab.c"
    break;

  case 86: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 344 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2575 "pascal.tab.c"
    break;

  case 87: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 345 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2581 "pascal.tab.c"
    break;

  case 88: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 346 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2587 "pascal.tab.c"
    break;

  case 89: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 347 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2593 "pascal.tab.c"
    break;

  case 90: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 348 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2599 "pascal.tab.c"
    break;

  case 91: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 349 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2605 "pascal.tab.c"
    break;

  case 92: /* prop_index_opt: %empty  */
#line 352 "pascal.y"
    { (yyval.node) = NULL; }
#line 2611 "pascal.tab.c"
    break;

  case 93: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 353 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2617 "pascal.tab.c"
    break;

  case 94: /* prop_specs: %empty  */
#line 356 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2623 "pascal.tab.c"
    break;

  case 95: /* prop_specs: prop_specs IDENT IDENT  */
#line 357 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2629 "pascal.tab.c"
    break;

  case 96: /* prop_specs: prop_specs IDENT INTCONST  */
#line 358 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2635 "pascal.tab.c"
    break;

  case 97: /* packed_opt: PACKEDSY  */
#line 361 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2641 "pascal.tab.c"
    break;

  case 98: /* packed_opt: %empty  */
#line 362 "pascal.y"
     { (yyval.node) = NULL; }
#line 2647 "pascal.tab.c"
    break;

  case 99: /* simple_type: LPARENT enum_list RPARENT  */
#line 365 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2653 "pascal.tab.c"
    break;

  case 100: /* simple_type: IDENT  */
#line 366 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2659 "pascal.tab.c"
    break;

  case 101: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 367 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2665 "pascal.tab.c"
    break;

  case 102: /* simple_type: IDENT PERIOD IDENT  */
#line 368 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2671 "pascal.tab.c"
    break;

  case 103: /* simple_type: lo_bound DOTDOT b_expr  */
#line 369 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2677 "pascal.tab.c"
    break;

  case 104: /* lo_bound: lo_term  */
#line 372 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2683 "pascal.tab.c"
    break;

  case 105: /* lo_bound: PLUS lo_term  */
#line 373 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2689 "pascal.tab.c"
    break;

  case 106: /* lo_bound: MINUS lo_term  */
#line 374 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2695 "pascal.tab.c"
    break;

  case 107: /* lo_bound: lo_bound PLUS b_term  */
#line 375 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2701 "pascal.tab.c"
    break;

  case 108: /* lo_bound: lo_bound MINUS b_term  */
#line 376 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2707 "pascal.tab.c"
    break;

  case 109: /* lo_term: scalar_constant  */
#line 379 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2713 "pascal.tab.c"
    break;

  case 110: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 380 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2719 "pascal.tab.c"
    break;

  case 111: /* lo_term: lo_term MUL b_factor  */
#line 381 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2725 "pascal.tab.c"
    break;

  case 112: /* lo_term: lo_term IDIV b_factor  */
#line 382 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2731 "pascal.tab.c"
    break;

  case 113: /* lo_term: lo_term IMOD b_factor  */
#line 383 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2737 "pascal.tab.c"
    break;

  case 114: /* lo_term: lo_term SHIFTOP b_factor  */
#line 384 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2743 "pascal.tab.c"
    break;

  case 115: /* b_expr: b_term  */
#line 387 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2749 "pascal.tab.c"
    break;

  case 116: /* b_expr: PLUS b_term  */
#line 388 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2755 "pascal.tab.c"
    break;

  case 117: /* b_expr: MINUS b_term  */
#line 389 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2761 "pascal.tab.c"
    break;

  case 118: /* b_expr: b_expr PLUS b_term  */
#line 390 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2767 "pascal.tab.c"
    break;

  case 119: /* b_expr: b_expr MINUS b_term  */
#line 391 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2773 "pascal.tab.c"
    break;

  case 120: /* b_term: b_factor  */
#line 394 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2779 "pascal.tab.c"
    break;

  case 121: /* b_term: b_term MUL b_factor  */
#line 395 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2785 "pascal.tab.c"
    break;

  case 122: /* b_term: b_term IDIV b_factor  */
#line 396 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2791 "pascal.tab.c"
    break;

  case 123: /* b_term: b_term IMOD b_factor  */
#line 397 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2797 "pascal.tab.c"
    break;

  case 124: /* b_term: b_term SHIFTOP b_factor  */
#line 398 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2803 "pascal.tab.c"
    break;

  case 125: /* b_factor: scalar_constant  */
#line 401 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2809 "pascal.tab.c"
    break;

  case 126: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 402 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2815 "pascal.tab.c"
    break;

  case 127: /* b_factor: LPARENT b_expr RPARENT  */
#line 403 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2821 "pascal.tab.c"
    break;

  case 128: /* record_body: record_field_list record_case_opt  */
#line 406 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2827 "pascal.tab.c"
    break;

  case 129: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 409 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2833 "pascal.tab.c"
    break;

  case 130: /* record_field_list: record_field  */
#line 410 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2839 "pascal.tab.c"
    break;

  case 131: /* $@4: %empty  */
#line 413 "pascal.y"
                  { }
#line 2845 "pascal.tab.c"
    break;

  case 132: /* record_field: id_list COLON $@4 type  */
#line 413 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2851 "pascal.tab.c"
    break;

  case 133: /* record_field: %empty  */
#line 414 "pascal.y"
     { (yyval.node) = NULL; }
#line 2857 "pascal.tab.c"
    break;

  case 134: /* case_arm_mark: %empty  */
#line 417 "pascal.y"
    { }
#line 2863 "pascal.tab.c"
    break;

  case 135: /* case_open: %empty  */
#line 420 "pascal.y"
    { }
#line 2869 "pascal.tab.c"
    break;

  case 136: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 423 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2875 "pascal.tab.c"
    break;

  case 137: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 424 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2881 "pascal.tab.c"
    break;

  case 138: /* record_case_opt: %empty  */
#line 425 "pascal.y"
     { (yyval.node) = NULL; }
#line 2887 "pascal.tab.c"
    break;

  case 139: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 428 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2893 "pascal.tab.c"
    break;

  case 140: /* record_case_list: record_case_arm  */
#line 429 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2899 "pascal.tab.c"
    break;

  case 141: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 432 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2905 "pascal.tab.c"
    break;

  case 142: /* record_case_arm: %empty  */
#line 433 "pascal.y"
     { (yyval.node) = NULL; }
#line 2911 "pascal.tab.c"
    break;

  case 143: /* var_decl_list: var_decl_list var_decl  */
#line 436 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2917 "pascal.tab.c"
    break;

  case 144: /* var_decl_list: var_decl  */
#line 437 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2923 "pascal.tab.c"
    break;

  case 145: /* var_decl: id_list COLON type SEMICOLON  */
#line 440 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2929 "pascal.tab.c"
    break;

  case 146: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 441 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2935 "pascal.tab.c"
    break;

  case 147: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 442 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 2941 "pascal.tab.c"
    break;

  case 148: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 445 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2947 "pascal.tab.c"
    break;

  case 149: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 446 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2953 "pascal.tab.c"
    break;

  case 150: /* $@5: %empty  */
#line 447 "pascal.y"
                                                                               { }
#line 2959 "pascal.tab.c"
    break;

  case 151: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 447 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2965 "pascal.tab.c"
    break;

  case 152: /* $@6: %empty  */
#line 448 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2971 "pascal.tab.c"
    break;

  case 153: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 448 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2977 "pascal.tab.c"
    break;

  case 154: /* $@7: %empty  */
#line 449 "pascal.y"
                                        { }
#line 2983 "pascal.tab.c"
    break;

  case 155: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 449 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2989 "pascal.tab.c"
    break;

  case 156: /* $@8: %empty  */
#line 450 "pascal.y"
                                                                                            { }
#line 2995 "pascal.tab.c"
    break;

  case 157: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 450 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3001 "pascal.tab.c"
    break;

  case 158: /* $@9: %empty  */
#line 451 "pascal.y"
                                                                                                             { }
#line 3007 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 451 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3015 "pascal.tab.c"
    break;

  case 160: /* $@10: %empty  */
#line 454 "pascal.y"
                                                                                              { }
#line 3021 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 454 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3029 "pascal.tab.c"
    break;

  case 162: /* $@11: %empty  */
#line 457 "pascal.y"
                                                                                             { }
#line 3035 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 457 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3043 "pascal.tab.c"
    break;

  case 164: /* $@12: %empty  */
#line 460 "pascal.y"
                                                                                                    { }
#line 3049 "pascal.tab.c"
    break;

  case 165: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 460 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3057 "pascal.tab.c"
    break;

  case 166: /* $@13: %empty  */
#line 463 "pascal.y"
                                                                                                                     { }
#line 3063 "pascal.tab.c"
    break;

  case 167: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 463 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3071 "pascal.tab.c"
    break;

  case 168: /* pv_mark: %empty  */
#line 468 "pascal.y"
    { }
#line 3077 "pascal.tab.c"
    break;

  case 169: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 471 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3083 "pascal.tab.c"
    break;

  case 170: /* parameter_list_opt: LPARENT RPARENT  */
#line 472 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3089 "pascal.tab.c"
    break;

  case 171: /* parameter_list_opt: %empty  */
#line 473 "pascal.y"
     { (yyval.node) = NULL; }
#line 3095 "pascal.tab.c"
    break;

  case 172: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 476 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 173: /* parameter_decl_list: parameter_decl  */
#line 477 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 174: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 480 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3113 "pascal.tab.c"
    break;

  case 175: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 481 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3119 "pascal.tab.c"
    break;

  case 176: /* parameter_decl: VARSY id_list COLON param_type  */
#line 482 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3125 "pascal.tab.c"
    break;

  case 177: /* parameter_decl: id_list COLON param_type  */
#line 483 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3131 "pascal.tab.c"
    break;

  case 178: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 484 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3137 "pascal.tab.c"
    break;

  case 179: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 485 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3143 "pascal.tab.c"
    break;

  case 180: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 486 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3149 "pascal.tab.c"
    break;

  case 181: /* parameter_decl: IDENT id_list COLON param_type  */
#line 487 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3155 "pascal.tab.c"
    break;

  case 182: /* param_type: IDENT  */
#line 490 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3161 "pascal.tab.c"
    break;

  case 183: /* param_type: IDENT PERIOD IDENT  */
#line 491 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 184: /* param_type: ARRAYSY OFSY IDENT  */
#line 492 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3173 "pascal.tab.c"
    break;

  case 185: /* routine_directives: %empty  */
#line 495 "pascal.y"
    { (yyval.node) = NULL; }
#line 3179 "pascal.tab.c"
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
#line 3192 "pascal.tab.c"
    break;

  case 187: /* pf_params: LPARENT pf_sections RPARENT  */
#line 506 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3198 "pascal.tab.c"
    break;

  case 188: /* pf_params: LPARENT RPARENT  */
#line 507 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3204 "pascal.tab.c"
    break;

  case 189: /* pf_params: %empty  */
#line 508 "pascal.y"
     { (yyval.node) = NULL; }
#line 3210 "pascal.tab.c"
    break;

  case 190: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 511 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3216 "pascal.tab.c"
    break;

  case 191: /* pf_sections: pf_section  */
#line 512 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3222 "pascal.tab.c"
    break;

  case 192: /* pf_section: id_list COLON param_type  */
#line 515 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3228 "pascal.tab.c"
    break;

  case 193: /* pf_section: VARSY id_list COLON param_type  */
#line 516 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3234 "pascal.tab.c"
    break;

  case 194: /* pf_section: CONSTSY id_list COLON param_type  */
#line 517 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3240 "pascal.tab.c"
    break;

  case 195: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 518 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3246 "pascal.tab.c"
    break;

  case 196: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 519 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3252 "pascal.tab.c"
    break;

  case 197: /* enum_list: enum_list COMMA enum_member  */
#line 522 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3258 "pascal.tab.c"
    break;

  case 198: /* enum_list: enum_member  */
#line 523 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3264 "pascal.tab.c"
    break;

  case 199: /* enum_member: IDENT  */
#line 526 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3270 "pascal.tab.c"
    break;

  case 200: /* id_list: id_list COMMA IDENT  */
#line 529 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3276 "pascal.tab.c"
    break;

  case 201: /* id_list: IDENT  */
#line 530 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3282 "pascal.tab.c"
    break;

  case 202: /* body: BEGINSY statement_list ENDSY  */
#line 533 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3288 "pascal.tab.c"
    break;

  case 203: /* $@14: %empty  */
#line 536 "pascal.y"
                             { pas_stmt_mark(); }
#line 3294 "pascal.tab.c"
    break;

  case 204: /* statement_list: statement_list SEMICOLON $@14 statement  */
#line 536 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3300 "pascal.tab.c"
    break;

  case 205: /* $@15: %empty  */
#line 537 "pascal.y"
     { pas_stmt_mark(); }
#line 3306 "pascal.tab.c"
    break;

  case 206: /* statement_list: $@15 statement  */
#line 537 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3312 "pascal.tab.c"
    break;

  case 207: /* $@16: %empty  */
#line 540 "pascal.y"
    { pas_stmt_mark(); }
#line 3318 "pascal.tab.c"
    break;

  case 208: /* body_stmt: $@16 statement  */
#line 540 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3324 "pascal.tab.c"
    break;

  case 209: /* statement: statement_no_label  */
#line 543 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3330 "pascal.tab.c"
    break;

  case 210: /* statement: INTCONST COLON statement_no_label  */
#line 544 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3336 "pascal.tab.c"
    break;

  case 211: /* statement: IDENT COLON statement_no_label  */
#line 545 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3342 "pascal.tab.c"
    break;

  case 212: /* statement_no_label: assignment  */
#line 548 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3348 "pascal.tab.c"
    break;

  case 213: /* statement_no_label: call  */
#line 549 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3354 "pascal.tab.c"
    break;

  case 214: /* statement_no_label: compound_statement  */
#line 550 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3360 "pascal.tab.c"
    break;

  case 215: /* statement_no_label: goto_statement  */
#line 551 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3366 "pascal.tab.c"
    break;

  case 216: /* statement_no_label: if_statement  */
#line 552 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3372 "pascal.tab.c"
    break;

  case 217: /* statement_no_label: case_statement  */
#line 553 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3378 "pascal.tab.c"
    break;

  case 218: /* statement_no_label: while_statement  */
#line 554 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3384 "pascal.tab.c"
    break;

  case 219: /* statement_no_label: repeat_statement  */
#line 555 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3390 "pascal.tab.c"
    break;

  case 220: /* statement_no_label: for_statement  */
#line 556 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3396 "pascal.tab.c"
    break;

  case 221: /* statement_no_label: with_statement  */
#line 557 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3402 "pascal.tab.c"
    break;

  case 222: /* statement_no_label: try_statement  */
#line 558 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3408 "pascal.tab.c"
    break;

  case 223: /* statement_no_label: raise_statement  */
#line 559 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3414 "pascal.tab.c"
    break;

  case 224: /* statement_no_label: inherited_call  */
#line 560 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3420 "pascal.tab.c"
    break;

  case 225: /* statement_no_label: INHERITEDSY  */
#line 561 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3426 "pascal.tab.c"
    break;

  case 226: /* statement_no_label: %empty  */
#line 562 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3432 "pascal.tab.c"
    break;

  case 227: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 565 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3438 "pascal.tab.c"
    break;

  case 228: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 566 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3444 "pascal.tab.c"
    break;

  case 229: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 567 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3450 "pascal.tab.c"
    break;

  case 230: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 568 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3456 "pascal.tab.c"
    break;

  case 231: /* handler_list: handler  */
#line 571 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3462 "pascal.tab.c"
    break;

  case 232: /* handler_list: handler_list SEMICOLON handler  */
#line 572 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3468 "pascal.tab.c"
    break;

  case 233: /* handler_list: handler_list SEMICOLON  */
#line 573 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3474 "pascal.tab.c"
    break;

  case 234: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 576 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3480 "pascal.tab.c"
    break;

  case 235: /* handler: ONSY IDENT DOSY body_stmt  */
#line 577 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3486 "pascal.tab.c"
    break;

  case 236: /* raise_statement: RAISESY  */
#line 580 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3492 "pascal.tab.c"
    break;

  case 237: /* raise_statement: RAISESY expression  */
#line 581 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3498 "pascal.tab.c"
    break;

  case 238: /* inherited_call: INHERITEDSY IDENT  */
#line 584 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3504 "pascal.tab.c"
    break;

  case 239: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 585 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3510 "pascal.tab.c"
    break;

  case 240: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 586 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3516 "pascal.tab.c"
    break;

  case 241: /* call: IDENT  */
#line 589 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3522 "pascal.tab.c"
    break;

  case 242: /* call: selector PERIOD IDENT  */
#line 590 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3528 "pascal.tab.c"
    break;

  case 243: /* call: call_with_args  */
#line 591 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3534 "pascal.tab.c"
    break;

  case 244: /* recv_primary: STRINGCONST  */
#line 594 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3540 "pascal.tab.c"
    break;

  case 245: /* recv_primary: LPARENT expression RPARENT  */
#line 595 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3546 "pascal.tab.c"
    break;

  case 246: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 598 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3552 "pascal.tab.c"
    break;

  case 247: /* call_with_args: IDENT LPARENT RPARENT  */
#line 599 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3558 "pascal.tab.c"
    break;

  case 248: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 600 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3564 "pascal.tab.c"
    break;

  case 249: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 601 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3570 "pascal.tab.c"
    break;

  case 250: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 602 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3576 "pascal.tab.c"
    break;

  case 251: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 603 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3582 "pascal.tab.c"
    break;

  case 252: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 604 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3588 "pascal.tab.c"
    break;

  case 253: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 605 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3594 "pascal.tab.c"
    break;

  case 254: /* argument_list: argument_list COMMA argument  */
#line 608 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3600 "pascal.tab.c"
    break;

  case 255: /* argument_list: argument  */
#line 609 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3606 "pascal.tab.c"
    break;

  case 256: /* argument: expression  */
#line 612 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3612 "pascal.tab.c"
    break;

  case 257: /* argument: expression COLON expression  */
#line 613 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3618 "pascal.tab.c"
    break;

  case 258: /* argument: expression COLON expression COLON expression  */
#line 614 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3624 "pascal.tab.c"
    break;

  case 259: /* assignment: selector BECOMES expression  */
#line 617 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3630 "pascal.tab.c"
    break;

  case 260: /* assignment: selector ADDBECOMES expression  */
#line 618 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3636 "pascal.tab.c"
    break;

  case 261: /* assignment: selector SUBBECOMES expression  */
#line 619 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3642 "pascal.tab.c"
    break;

  case 262: /* assignment: selector MULBECOMES expression  */
#line 620 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3648 "pascal.tab.c"
    break;

  case 263: /* assignment: selector DIVBECOMES expression  */
#line 621 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3654 "pascal.tab.c"
    break;

  case 264: /* assignment: call_with_args BECOMES expression  */
#line 622 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3660 "pascal.tab.c"
    break;

  case 265: /* selector: selector LBRACK expression_list RBRACK  */
#line 625 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3666 "pascal.tab.c"
    break;

  case 266: /* selector: selector PERIOD IDENT  */
#line 626 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3672 "pascal.tab.c"
    break;

  case 267: /* selector: selector ARROW  */
#line 627 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3678 "pascal.tab.c"
    break;

  case 268: /* selector: call_with_args PERIOD IDENT  */
#line 628 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3684 "pascal.tab.c"
    break;

  case 269: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 629 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3690 "pascal.tab.c"
    break;

  case 270: /* selector: call_with_args ARROW  */
#line 630 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3696 "pascal.tab.c"
    break;

  case 271: /* selector: recv_primary PERIOD IDENT  */
#line 631 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3702 "pascal.tab.c"
    break;

  case 272: /* selector: recv_primary ARROW  */
#line 632 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3708 "pascal.tab.c"
    break;

  case 273: /* selector: IDENT  */
#line 633 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3714 "pascal.tab.c"
    break;

  case 274: /* expression_list: expression_list COMMA expression  */
#line 636 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3720 "pascal.tab.c"
    break;

  case 275: /* expression_list: expression  */
#line 637 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3726 "pascal.tab.c"
    break;

  case 276: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 640 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3732 "pascal.tab.c"
    break;

  case 277: /* goto_statement: GOTOSY INTCONST  */
#line 643 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3738 "pascal.tab.c"
    break;

  case 278: /* goto_statement: GOTOSY IDENT  */
#line 644 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3744 "pascal.tab.c"
    break;

  case 279: /* if_statement: IFSY expression THENSY body_stmt  */
#line 647 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3750 "pascal.tab.c"
    break;

  case 280: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 648 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3756 "pascal.tab.c"
    break;

  case 281: /* $@17: %empty  */
#line 651 "pascal.y"
                           { }
#line 3762 "pascal.tab.c"
    break;

  case 282: /* case_statement: CASESY expression OFSY $@17 case_list case_tail ENDSY  */
#line 651 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3768 "pascal.tab.c"
    break;

  case 283: /* case_tail: %empty  */
#line 654 "pascal.y"
    { (yyval.node) = NULL; }
#line 3774 "pascal.tab.c"
    break;

  case 284: /* case_tail: ELSESY statement_list  */
#line 655 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3780 "pascal.tab.c"
    break;

  case 285: /* case_tail: OTHERWISESY statement_list  */
#line 656 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3786 "pascal.tab.c"
    break;

  case 286: /* case_list: case_list SEMICOLON case_elem  */
#line 659 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3792 "pascal.tab.c"
    break;

  case 287: /* case_list: case_elem  */
#line 660 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3798 "pascal.tab.c"
    break;

  case 288: /* case_elem: constant_list COLON body_stmt  */
#line 663 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3804 "pascal.tab.c"
    break;

  case 289: /* case_elem: %empty  */
#line 664 "pascal.y"
     { (yyval.node) = NULL; }
#line 3810 "pascal.tab.c"
    break;

  case 290: /* constant_list: constant_list COMMA case_label  */
#line 667 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3816 "pascal.tab.c"
    break;

  case 291: /* constant_list: case_label  */
#line 668 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3822 "pascal.tab.c"
    break;

  case 292: /* case_label: b_expr  */
#line 671 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3828 "pascal.tab.c"
    break;

  case 293: /* case_label: b_expr DOTDOT b_expr  */
#line 672 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3834 "pascal.tab.c"
    break;

  case 294: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 675 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3840 "pascal.tab.c"
    break;

  case 295: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 678 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3846 "pascal.tab.c"
    break;

  case 296: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 681 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3852 "pascal.tab.c"
    break;

  case 297: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 682 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3858 "pascal.tab.c"
    break;

  case 298: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 683 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3864 "pascal.tab.c"
    break;

  case 299: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 686 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3870 "pascal.tab.c"
    break;

  case 300: /* with_open: with_open COMMA selector  */
#line 689 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3876 "pascal.tab.c"
    break;

  case 301: /* with_open: selector  */
#line 690 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3882 "pascal.tab.c"
    break;

  case 302: /* expression: simple_expression  */
#line 697 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3888 "pascal.tab.c"
    break;

  case 303: /* expression: expression INOP simple_expression  */
#line 698 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3894 "pascal.tab.c"
    break;

  case 304: /* expression: expression LTOP simple_expression  */
#line 699 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3900 "pascal.tab.c"
    break;

  case 305: /* expression: expression LEOP simple_expression  */
#line 700 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3906 "pascal.tab.c"
    break;

  case 306: /* expression: expression GTOP simple_expression  */
#line 701 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3912 "pascal.tab.c"
    break;

  case 307: /* expression: expression GEOP simple_expression  */
#line 702 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3918 "pascal.tab.c"
    break;

  case 308: /* expression: expression NEOP simple_expression  */
#line 703 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3924 "pascal.tab.c"
    break;

  case 309: /* expression: expression EQOP simple_expression  */
#line 704 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3930 "pascal.tab.c"
    break;

  case 310: /* expression: expression ISOP simple_expression  */
#line 705 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3936 "pascal.tab.c"
    break;

  case 311: /* simple_expression: term  */
#line 708 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3942 "pascal.tab.c"
    break;

  case 312: /* simple_expression: PLUS term  */
#line 709 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3948 "pascal.tab.c"
    break;

  case 313: /* simple_expression: MINUS term  */
#line 710 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3954 "pascal.tab.c"
    break;

  case 314: /* simple_expression: simple_expression PLUS term  */
#line 711 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3960 "pascal.tab.c"
    break;

  case 315: /* simple_expression: simple_expression MINUS term  */
#line 712 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3966 "pascal.tab.c"
    break;

  case 316: /* simple_expression: simple_expression OROP term  */
#line 713 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3972 "pascal.tab.c"
    break;

  case 317: /* simple_expression: simple_expression XOROP term  */
#line 714 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3978 "pascal.tab.c"
    break;

  case 318: /* term: power  */
#line 717 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3984 "pascal.tab.c"
    break;

  case 319: /* term: term MUL power  */
#line 718 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3990 "pascal.tab.c"
    break;

  case 320: /* term: term RDIV power  */
#line 719 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3996 "pascal.tab.c"
    break;

  case 321: /* term: term IDIV power  */
#line 720 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4002 "pascal.tab.c"
    break;

  case 322: /* term: term IMOD power  */
#line 721 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4008 "pascal.tab.c"
    break;

  case 323: /* term: term ANDOP power  */
#line 722 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4014 "pascal.tab.c"
    break;

  case 324: /* term: term MUL MINUS power  */
#line 723 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4020 "pascal.tab.c"
    break;

  case 325: /* term: term RDIV MINUS power  */
#line 724 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4026 "pascal.tab.c"
    break;

  case 326: /* term: term IDIV MINUS power  */
#line 725 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4032 "pascal.tab.c"
    break;

  case 327: /* term: term IMOD MINUS power  */
#line 726 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4038 "pascal.tab.c"
    break;

  case 328: /* term: term ANDOP MINUS power  */
#line 727 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4044 "pascal.tab.c"
    break;

  case 329: /* term: term SHIFTOP power  */
#line 728 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4050 "pascal.tab.c"
    break;

  case 330: /* term: term ASOP power  */
#line 729 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4056 "pascal.tab.c"
    break;

  case 331: /* power: factor  */
#line 732 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4062 "pascal.tab.c"
    break;

  case 332: /* power: power POWOP factor  */
#line 733 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4068 "pascal.tab.c"
    break;

  case 333: /* factor: selector  */
#line 736 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4074 "pascal.tab.c"
    break;

  case 334: /* factor: call_with_args  */
#line 737 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4080 "pascal.tab.c"
    break;

  case 335: /* factor: INTCONST  */
#line 738 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4086 "pascal.tab.c"
    break;

  case 336: /* factor: REALCONST  */
#line 739 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4092 "pascal.tab.c"
    break;

  case 337: /* factor: STRINGCONST  */
#line 740 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4098 "pascal.tab.c"
    break;

  case 338: /* factor: CHARCODE  */
#line 741 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4104 "pascal.tab.c"
    break;

  case 339: /* factor: LPARENT expression RPARENT  */
#line 742 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 4110 "pascal.tab.c"
    break;

  case 340: /* factor: inherited_call  */
#line 743 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4116 "pascal.tab.c"
    break;

  case 341: /* factor: NOTSY factor  */
#line 744 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4122 "pascal.tab.c"
    break;

  case 342: /* factor: ATSIGN factor  */
#line 745 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4128 "pascal.tab.c"
    break;

  case 343: /* factor: LBRACK RBRACK  */
#line 746 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4134 "pascal.tab.c"
    break;

  case 344: /* factor: LBRACK set_member_list RBRACK  */
#line 747 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4140 "pascal.tab.c"
    break;

  case 345: /* set_member_list: set_member  */
#line 750 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4146 "pascal.tab.c"
    break;

  case 346: /* set_member_list: set_member_list COMMA set_member  */
#line 751 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4152 "pascal.tab.c"
    break;

  case 347: /* set_member: expression  */
#line 754 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4158 "pascal.tab.c"
    break;

  case 348: /* set_member: expression DOTDOT expression  */
#line 755 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4164 "pascal.tab.c"
    break;


#line 4168 "pascal.tab.c"

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

#line 761 "pascal.y"

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
