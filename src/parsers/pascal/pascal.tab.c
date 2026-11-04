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
static tree_t *pt_spec(tree_t *name, tree_t *args) { return pt_add(pt_add(pt_sval(pt_new(TT_DECL), "specialize"), name), args); }
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

#line 236 "pascal.tab.c"

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
  YYSYMBOL_GENERICSY = 92,                 /* GENERICSY  */
  YYSYMBOL_GENERICTSY = 93,                /* GENERICTSY  */
  YYSYMBOL_SPECIALIZESY = 94,              /* SPECIALIZESY  */
  YYSYMBOL_YYACCEPT = 95,                  /* $accept  */
  YYSYMBOL_compilation_unit = 96,          /* compilation_unit  */
  YYSYMBOL_program = 97,                   /* program  */
  YYSYMBOL_program_name = 98,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 99,          /* file_id_list_opt  */
  YYSYMBOL_block = 100,                    /* block  */
  YYSYMBOL_decl_part_list = 101,           /* decl_part_list  */
  YYSYMBOL_decl_part = 102,                /* decl_part  */
  YYSYMBOL_unit = 103,                     /* unit  */
  YYSYMBOL_intf_part_list = 104,           /* intf_part_list  */
  YYSYMBOL_intf_part = 105,                /* intf_part  */
  YYSYMBOL_init_opt = 106,                 /* init_opt  */
  YYSYMBOL_fini_opt = 107,                 /* fini_opt  */
  YYSYMBOL_label_list = 108,               /* label_list  */
  YYSYMBOL_const_decl_list = 109,          /* const_decl_list  */
  YYSYMBOL_const_decl = 110,               /* const_decl  */
  YYSYMBOL_typed_value = 111,              /* typed_value  */
  YYSYMBOL_typed_value_list = 112,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 113,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 114,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 115,           /* type_decl_list  */
  YYSYMBOL_type_decl = 116,                /* type_decl  */
  YYSYMBOL_class_parents = 117,            /* class_parents  */
  YYSYMBOL_parent_item = 118,              /* parent_item  */
  YYSYMBOL_spec_kw = 119,                  /* spec_kw  */
  YYSYMBOL_spec_ref = 120,                 /* spec_ref  */
  YYSYMBOL_type_arg_list = 121,            /* type_arg_list  */
  YYSYMBOL_type = 122,                     /* type  */
  YYSYMBOL_123_1 = 123,                    /* $@1  */
  YYSYMBOL_124_2 = 124,                    /* $@2  */
  YYSYMBOL_125_3 = 125,                    /* $@3  */
  YYSYMBOL_result_type = 126,              /* result_type  */
  YYSYMBOL_class_members = 127,            /* class_members  */
  YYSYMBOL_class_member = 128,             /* class_member  */
  YYSYMBOL_prop_index_opt = 129,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 130,               /* prop_specs  */
  YYSYMBOL_packed_opt = 131,               /* packed_opt  */
  YYSYMBOL_simple_type = 132,              /* simple_type  */
  YYSYMBOL_lo_bound = 133,                 /* lo_bound  */
  YYSYMBOL_lo_term = 134,                  /* lo_term  */
  YYSYMBOL_b_expr = 135,                   /* b_expr  */
  YYSYMBOL_b_term = 136,                   /* b_term  */
  YYSYMBOL_b_factor = 137,                 /* b_factor  */
  YYSYMBOL_record_body = 138,              /* record_body  */
  YYSYMBOL_record_field_list = 139,        /* record_field_list  */
  YYSYMBOL_record_field = 140,             /* record_field  */
  YYSYMBOL_141_4 = 141,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 142,            /* case_arm_mark  */
  YYSYMBOL_case_open = 143,                /* case_open  */
  YYSYMBOL_record_case_opt = 144,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 145,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 146,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 147,            /* var_decl_list  */
  YYSYMBOL_var_decl = 148,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 149,           /* procedure_decl  */
  YYSYMBOL_150_5 = 150,                    /* $@5  */
  YYSYMBOL_151_6 = 151,                    /* $@6  */
  YYSYMBOL_152_7 = 152,                    /* $@7  */
  YYSYMBOL_153_8 = 153,                    /* $@8  */
  YYSYMBOL_154_9 = 154,                    /* $@9  */
  YYSYMBOL_155_10 = 155,                   /* $@10  */
  YYSYMBOL_156_11 = 156,                   /* $@11  */
  YYSYMBOL_157_12 = 157,                   /* $@12  */
  YYSYMBOL_158_13 = 158,                   /* $@13  */
  YYSYMBOL_159_14 = 159,                   /* $@14  */
  YYSYMBOL_160_15 = 160,                   /* $@15  */
  YYSYMBOL_161_16 = 161,                   /* $@16  */
  YYSYMBOL_162_17 = 162,                   /* $@17  */
  YYSYMBOL_pv_mark = 163,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 164,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 165,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 166,           /* parameter_decl  */
  YYSYMBOL_param_type = 167,               /* param_type  */
  YYSYMBOL_routine_directives = 168,       /* routine_directives  */
  YYSYMBOL_pf_params = 169,                /* pf_params  */
  YYSYMBOL_pf_sections = 170,              /* pf_sections  */
  YYSYMBOL_pf_section = 171,               /* pf_section  */
  YYSYMBOL_enum_list = 172,                /* enum_list  */
  YYSYMBOL_enum_member = 173,              /* enum_member  */
  YYSYMBOL_id_list = 174,                  /* id_list  */
  YYSYMBOL_body = 175,                     /* body  */
  YYSYMBOL_statement_list = 176,           /* statement_list  */
  YYSYMBOL_177_18 = 177,                   /* $@18  */
  YYSYMBOL_178_19 = 178,                   /* $@19  */
  YYSYMBOL_body_stmt = 179,                /* body_stmt  */
  YYSYMBOL_180_20 = 180,                   /* $@20  */
  YYSYMBOL_statement = 181,                /* statement  */
  YYSYMBOL_statement_no_label = 182,       /* statement_no_label  */
  YYSYMBOL_try_statement = 183,            /* try_statement  */
  YYSYMBOL_handler_list = 184,             /* handler_list  */
  YYSYMBOL_handler = 185,                  /* handler  */
  YYSYMBOL_raise_statement = 186,          /* raise_statement  */
  YYSYMBOL_inherited_call = 187,           /* inherited_call  */
  YYSYMBOL_call = 188,                     /* call  */
  YYSYMBOL_recv_primary = 189,             /* recv_primary  */
  YYSYMBOL_call_with_args = 190,           /* call_with_args  */
  YYSYMBOL_argument_list = 191,            /* argument_list  */
  YYSYMBOL_argument = 192,                 /* argument  */
  YYSYMBOL_assignment = 193,               /* assignment  */
  YYSYMBOL_selector = 194,                 /* selector  */
  YYSYMBOL_expression_list = 195,          /* expression_list  */
  YYSYMBOL_compound_statement = 196,       /* compound_statement  */
  YYSYMBOL_goto_statement = 197,           /* goto_statement  */
  YYSYMBOL_if_statement = 198,             /* if_statement  */
  YYSYMBOL_case_statement = 199,           /* case_statement  */
  YYSYMBOL_200_21 = 200,                   /* $@21  */
  YYSYMBOL_case_tail = 201,                /* case_tail  */
  YYSYMBOL_case_list = 202,                /* case_list  */
  YYSYMBOL_case_elem = 203,                /* case_elem  */
  YYSYMBOL_constant_list = 204,            /* constant_list  */
  YYSYMBOL_case_label = 205,               /* case_label  */
  YYSYMBOL_while_statement = 206,          /* while_statement  */
  YYSYMBOL_repeat_statement = 207,         /* repeat_statement  */
  YYSYMBOL_for_statement = 208,            /* for_statement  */
  YYSYMBOL_with_statement = 209,           /* with_statement  */
  YYSYMBOL_with_open = 210,                /* with_open  */
  YYSYMBOL_expression = 211,               /* expression  */
  YYSYMBOL_simple_expression = 212,        /* simple_expression  */
  YYSYMBOL_term = 213,                     /* term  */
  YYSYMBOL_power = 214,                    /* power  */
  YYSYMBOL_factor = 215,                   /* factor  */
  YYSYMBOL_set_member_list = 216,          /* set_member_list  */
  YYSYMBOL_set_member = 217                /* set_member  */
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
#define YYLAST   1449

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  123
/* YYNRULES -- Number of rules.  */
#define YYNRULES  375
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  872

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   349


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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   216,   216,   217,   220,   221,   224,   225,   228,   229,
     232,   235,   236,   239,   240,   241,   242,   243,   244,   245,
     248,   251,   252,   255,   256,   257,   258,   259,   260,   263,
     264,   265,   268,   269,   272,   273,   274,   275,   278,   279,
     282,   283,   286,   287,   288,   289,   292,   293,   296,   297,
     300,   301,   302,   303,   304,   307,   308,   311,   312,   313,
     314,   315,   318,   319,   322,   323,   326,   329,   330,   333,
     334,   337,   338,   339,   340,   340,   340,   341,   342,   343,
     344,   345,   346,   347,   347,   348,   349,   350,   351,   352,
     355,   356,   357,   360,   361,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   376,   377,   380,   381,   382,
     385,   386,   389,   390,   391,   392,   393,   394,   397,   398,
     399,   400,   401,   404,   405,   406,   407,   408,   409,   412,
     413,   414,   415,   416,   419,   420,   421,   422,   423,   426,
     427,   428,   431,   434,   435,   438,   438,   439,   442,   445,
     448,   449,   450,   453,   454,   457,   458,   461,   462,   465,
     466,   467,   470,   471,   472,   472,   473,   473,   474,   474,
     475,   475,   476,   476,   477,   477,   478,   478,   479,   479,
     480,   480,   483,   483,   486,   486,   489,   489,   492,   492,
     497,   500,   501,   502,   505,   506,   509,   510,   511,   512,
     513,   514,   515,   516,   519,   520,   521,   522,   525,   526,
     536,   537,   538,   541,   542,   545,   546,   547,   548,   549,
     552,   553,   556,   559,   560,   563,   566,   566,   567,   567,
     570,   570,   573,   574,   575,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     595,   596,   597,   598,   601,   602,   603,   606,   607,   610,
     611,   614,   615,   616,   619,   620,   621,   622,   625,   626,
     627,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   642,   643,   646,   647,   648,   651,   652,   653,   654,
     655,   656,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   670,   671,   674,   677,   678,   681,   682,   685,   685,
     688,   689,   690,   693,   694,   697,   698,   701,   702,   705,
     706,   709,   712,   715,   716,   717,   720,   723,   724,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   742,   743,
     744,   745,   746,   747,   748,   751,   752,   753,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,   766,   767,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   784,   785,   788,   789
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
  "CONSTRUCTORSY", "DESTRUCTORSY", "PROPERTYSY", "ONSY", "GENERICSY",
  "GENERICTSY", "SPECIALIZESY", "$accept", "compilation_unit", "program",
  "program_name", "file_id_list_opt", "block", "decl_part_list",
  "decl_part", "unit", "intf_part_list", "intf_part", "init_opt",
  "fini_opt", "label_list", "const_decl_list", "const_decl", "typed_value",
  "typed_value_list", "field_value_list", "scalar_constant",
  "type_decl_list", "type_decl", "class_parents", "parent_item", "spec_kw",
  "spec_ref", "type_arg_list", "type", "$@1", "$@2", "$@3", "result_type",
  "class_members", "class_member", "prop_index_opt", "prop_specs",
  "packed_opt", "simple_type", "lo_bound", "lo_term", "b_expr", "b_term",
  "b_factor", "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14",
  "$@15", "$@16", "$@17", "pv_mark", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "param_type",
  "routine_directives", "pf_params", "pf_sections", "pf_section",
  "enum_list", "enum_member", "id_list", "body", "statement_list", "$@18",
  "$@19", "body_stmt", "$@20", "statement", "statement_no_label",
  "try_statement", "handler_list", "handler", "raise_statement",
  "inherited_call", "call", "recv_primary", "call_with_args",
  "argument_list", "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@21", "case_tail", "case_list", "case_elem", "constant_list",
  "case_label", "while_statement", "repeat_statement", "for_statement",
  "with_statement", "with_open", "expression", "simple_expression", "term",
  "power", "factor", "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-516)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-301)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      81,    95,   151,   243,  -516,   253,   801,  -516,  -516,   213,
     273,  -516,  -516,   227,   255,   293,   296,   293,   293,   310,
    -516,    40,    10,   313,   329,   341,  -516,  -516,  -516,   349,
     293,   423,   457,  -516,  -516,    79,   154,   255,  -516,  -516,
     117,   100,   293,  -516,   150,   145,   128,   142,   719,   473,
     405,    40,  -516,   446,   474,   472,   503,   527,   565,  -516,
     190,  -516,  -516,  -516,   331,  1011,   630,  -516,  -516,   566,
     293,   578,   501,  -516,   630,  -516,   293,   579,    72,  -516,
    -516,   434,   589,  -516,  1011,   238,  1011,  -516,  1011,  1011,
     519,  -516,  1337,  -516,  1011,   598,   606,   514,  -516,  -516,
    -516,  -516,  -516,  -516,   285,   317,  -516,  1378,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,   293,   654,   639,  -516,
     633,   640,   622,   631,   664,   668,  -516,   666,   575,  -516,
    -516,  1179,  1179,  1179,   869,  1011,  1179,  -516,  -516,  -516,
     431,   671,   598,   677,  -516,   599,   658,    75,   283,   421,
     667,  -516,   678,  -516,   678,   688,   688,   153,   700,  -516,
    -516,  -516,  -516,  1326,   714,  -516,  -516,  -516,   724,   337,
    -516,   308,   219,  -516,   418,  -516,    70,   776,    38,   425,
    -516,  -516,   726,   719,  -516,  -516,   510,   282,   487,   599,
     658,   125,   849,   204,  1226,  1243,   779,   779,   885,    34,
    1314,   720,   739,   911,   716,  -516,  1011,   717,  -516,  1011,
    1011,   721,  -516,  1011,  1011,  1011,  1011,  1011,   429,    74,
     784,   293,   731,   734,  -516,  -516,   293,   293,  -516,   255,
     293,  -516,   736,   293,   741,    40,  -516,  -516,   421,   421,
    -516,  1261,   383,  -516,  1280,  -516,   742,  -516,  1011,  1011,
    1011,  1011,  1011,  1011,  1011,  1011,  1179,  1179,  1179,  1179,
    1079,  1095,  1121,  1137,  1163,  1179,  1179,  1179,   230,   762,
     755,   732,   219,   219,   774,  -516,  -516,   326,  -516,  1208,
     747,  1262,  1262,   222,   156,  1037,    19,   796,   800,  -516,
     795,   795,  1262,   795,   795,   795,   795,  -516,   501,   293,
     753,   293,   754,  -516,   293,    39,  -516,   522,   822,  -516,
    1037,   756,  -516,   501,  -516,   233,  -516,  1011,  1011,  1011,
    -516,  -516,   238,  -516,  -516,  -516,  -516,  1351,  -516,  -516,
    -516,   379,  -516,   171,   743,  -516,   927,  1208,  -516,   393,
     771,  1314,   777,   488,  1314,  1314,  1364,   500,  1314,  1314,
    1314,  1314,   789,  -516,   222,  -516,   435,  -516,  -516,   501,
     501,   436,   441,   255,   146,   535,  -516,   293,  -516,    40,
    1011,  1011,  -516,   532,   780,   283,   283,   283,   283,   283,
     283,   283,   283,   421,   421,   421,   421,  1179,   667,  1179,
     667,  1179,   667,  1179,   667,  1179,   667,   667,   667,  -516,
     293,   775,   293,   781,  -516,    64,  -516,   562,   782,   233,
     700,  -516,   451,  -516,  -516,   795,   795,  1262,   799,  -516,
     407,   223,  -516,   286,   774,   449,  -516,  -516,   785,  -516,
     788,    85,   414,   798,   803,   804,  -516,   619,  1053,   855,
    1314,   630,  1208,   630,  1208,   293,   223,   223,   554,  -516,
    -516,  -516,  -516,   501,   867,   635,   678,   663,   678,   673,
     225,  -516,    21,   870,   812,   887,   889,   501,   835,   891,
     353,  -516,   898,   834,   547,  1314,  -516,   719,  -516,   658,
    1262,   863,  1011,  -516,  1011,   829,   212,   200,  -516,   220,
    -516,   450,   455,  -516,   953,   969,  1011,  -516,   995,  -516,
     630,   458,   853,   501,   501,   902,   903,  -516,  -516,  -516,
    -516,  -516,   894,   501,   501,  1314,  -516,   667,   667,   667,
     667,   667,   689,   678,   695,   678,   316,  -516,    21,  -516,
     882,  -516,  -516,  1208,   223,   223,   361,  1262,   795,   795,
    -516,   795,   795,   795,   795,  -516,   222,  -516,  -516,  -516,
    -516,   837,   838,  -516,  -516,   854,   630,   546,   859,    66,
    1280,  -516,  -516,   518,  -516,  -516,   881,    78,  -516,   708,
     913,  -516,    21,  -516,    21,   860,    21,  -516,   893,   360,
    -516,   866,  -516,   917,  -516,  -516,  -516,   865,   233,  -516,
     858,   920,  1011,  1011,  -516,  -516,   340,   149,  -516,   709,
    -516,  -516,  -516,  1298,    28,  -516,   743,  -516,  -516,  -516,
    -516,  -516,  -516,   460,  -516,   462,  1314,  -516,   476,   930,
     932,   630,   933,   886,  -516,  -516,   501,   501,   943,   943,
    -516,   921,   948,   895,    21,  -516,    21,   904,  -516,  -516,
     892,  -516,  -516,   366,   223,   223,  -516,  -516,  -516,  -516,
    -516,   334,   501,   501,  -516,  -516,   501,   501,   225,   907,
     957,  1037,  1037,    17,  -516,  1208,   941,  -516,   293,   901,
    -516,  -516,  -516,   812,   918,  -516,   233,  -516,   912,   914,
    1011,  -516,   975,   233,   988,  -516,   990,   812,   620,   797,
    -516,  1262,  1262,  -516,  -516,   944,  1262,  -516,  -516,  1011,
    -516,   928,  -516,   235,  -516,  -516,  -516,  -516,  -516,   992,
    -516,   233,   812,   812,  1001,   949,   943,   951,  -516,   233,
    -516,  -516,   233,  -516,  -516,  -516,  1006,   956,   501,   501,
    1008,  1014,    37,   946,  -516,  -516,  -516,   489,  -516,   970,
     972,  -516,  -516,    26,   630,   812,  -516,  1011,  -516,  -516,
    -516,  1314,  -516,  1027,  -516,  -516,  -516,  -516,  -516,   554,
    -516,   943,   943,  -516,  -516,  -516,  1314,  -516,  1020,  -516,
    -516,   812,  1034,  -516,  -516,  -516,   233,  -516,  1041,  -516,
    -516,   233,  1043,   991,  -516,  -516,  -516,  -516,  1037,  -516,
    1037,  1024,  -516,  -516,   977,  -516,  -516,  1050,  1314,  -516,
     812,  1056,  -516,  -516,  -516,  -516,  -516,  1059,  1067,   812,
    1069,  -516,  1071,  -516,   233,    36,  -516,  -516,   630,   630,
    1262,  1030,  1072,  -516,   812,  -516,  -516,  -516,  1074,   812,
    -516,  -516,  -516,  -516,  -516,  1076,  -516,   559,  -516,  -516,
    1083,  -516,   713,  -516,  -516,  -516,  1085,  -516,  -516,  1088,
     812,  -516,  -516,  -516,  1262,  1031,  1262,  1089,  -516,  1092,
    -516,  -516,  -516,  -516,  1083,  -516,  -516,  1097,   293,  -516,
    1035,  -516
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,     0,
     228,     0,     0,     0,     0,     0,    11,    17,    10,     0,
       0,     0,     0,    36,    37,     0,     0,    14,    39,   224,
       0,   190,    16,   158,     0,     0,   190,     0,   249,     0,
       0,    15,    56,     0,     0,     0,     0,     0,     0,     7,
       0,    12,    22,    13,     0,     0,   111,    38,    18,     0,
       0,     0,   193,   157,   111,    19,     0,     0,   193,   226,
     225,     0,     0,   228,     0,     0,     0,   228,     0,     0,
       0,   268,   264,   228,   259,   248,     0,   265,   229,   232,
     245,   246,   247,   236,     0,   267,   235,     0,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   111,     0,    55,
       0,     0,     0,     0,     0,     0,     8,     0,     0,    34,
      35,     0,     0,     0,     0,     0,     0,   362,   365,   363,
     364,   300,     0,   270,   367,   361,   360,     0,   329,   338,
     345,   358,   212,   110,   212,     0,     0,     0,     0,    51,
      54,    52,    53,   113,    93,   123,    67,   116,     0,     0,
      71,     0,   118,   223,     0,   190,     0,     0,     0,     0,
     190,   168,     0,   249,   304,   305,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,   249,   249,     0,     0,
     260,   261,     0,     0,     0,   299,     0,     0,   297,     0,
       0,     0,   294,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,   190,   190,     0,     0,     4,     0,
       0,    12,     0,     0,     0,     0,    21,   368,   339,   340,
     370,   374,     0,   372,     0,   369,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,    50,   119,   120,    72,    73,   222,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,    83,
       0,     0,     0,     0,     0,     0,     0,   190,   193,     0,
       0,     0,     0,   192,   224,     0,   195,     0,   208,   159,
       0,     0,   190,   193,    12,     0,   227,     0,     0,     0,
     230,   230,     0,   308,   303,   230,   269,   264,   233,   234,
     274,     0,   282,   283,   228,   228,     0,     0,   273,     0,
     298,   291,   295,     0,   302,   286,   266,     0,   287,   288,
     289,   290,     0,    60,     0,    57,     0,   190,   190,   193,
     193,     0,     0,    24,     0,    31,   190,    26,   190,    25,
       0,     0,   371,   366,   293,   330,   331,   332,   333,   334,
     335,   336,   337,   343,   341,   342,   344,     0,   348,     0,
     349,     0,   350,     0,   346,     0,   347,   356,   357,   359,
       0,     0,     0,     0,   211,     0,   214,     0,     0,     0,
       0,   112,     0,    69,   115,     0,     0,     0,    50,   139,
       0,   129,   134,     0,    64,     0,    62,    65,     0,    88,
       0,    95,     0,     0,     0,     0,    94,     0,     0,     0,
      42,   111,     0,   111,     0,   147,   121,   122,   117,   126,
     127,   125,   128,   193,     0,     0,   212,     0,   212,     0,
       0,   191,     0,     0,   164,     0,     0,   193,     0,     0,
      90,    92,     0,     0,     0,   322,   321,   249,   326,   327,
     316,   306,     0,   271,     0,     0,     0,     0,   254,     0,
     262,     0,     0,   272,     0,     0,     0,   296,     0,   292,
     111,     0,     0,   193,   193,     0,     0,   190,   190,    23,
     228,   228,    33,   193,   193,   375,   373,   353,   354,   355,
     351,   352,     0,   212,     0,   212,     0,   210,     0,    79,
      80,   220,    68,     0,   130,   131,     0,     0,     0,     0,
     114,     0,     0,     0,     0,   124,     0,    93,   190,   190,
      96,     0,     0,   190,   190,   105,   111,   300,     0,     0,
      42,    41,    77,     0,    87,    85,     0,   152,   144,     0,
       0,   208,     0,   196,     0,     0,     0,   194,     0,   204,
     207,   199,   162,     0,    12,   160,   161,     0,     0,   169,
       0,   208,     0,     0,   230,   231,   319,   310,   314,     0,
     318,   230,   281,   284,     0,   250,   256,   251,   228,   253,
     263,    66,   280,     0,   278,     0,   301,   276,     0,     0,
      93,   111,     0,     0,   208,   208,   193,   193,    29,    30,
     228,     0,     0,     0,     0,   218,     0,     0,   213,   215,
       0,    70,   141,     0,   132,   133,   136,   137,   135,   138,
      63,     0,   193,   193,   190,   190,   193,   193,     0,     0,
       0,     0,     0,     0,    44,     0,     0,    84,   147,     0,
     142,   145,   208,   178,   200,   198,     0,   203,     0,     0,
       0,   209,     0,     0,     0,    91,     0,   166,     0,     0,
     325,     0,   316,   228,   228,     0,     0,   230,   307,     0,
     230,     0,   255,     0,   279,   277,   275,    59,    61,     0,
     208,     0,   182,   184,     0,     0,    32,     0,    27,     0,
     217,   216,     0,    81,   140,    89,     0,     0,   193,   193,
       0,     0,     0,     0,    97,    48,    46,     0,    45,     0,
       0,    74,   143,     0,   111,   174,    12,     0,   197,   206,
     205,   201,   165,     0,   208,   163,    12,   230,   230,   320,
     313,   312,   311,   309,   317,   315,   285,   258,     0,   252,
      58,   186,     0,    12,    12,   208,     0,    20,     0,   219,
      98,     0,     0,     0,   100,   101,   106,   107,     0,    43,
       0,     0,    75,   149,     0,   146,    12,     0,   202,   208,
     180,     0,   324,   323,   230,    12,   208,     0,     0,   170,
       0,    28,     0,   102,     0,     0,    47,    49,   111,   111,
     156,     0,     0,   179,   176,    12,   167,   257,     0,   188,
     183,   185,    12,   208,    99,     0,   104,     0,    82,    76,
     151,   154,     0,   149,   175,    12,     0,   187,    12,     0,
     172,   103,   109,   108,   156,     0,   156,     0,   181,     0,
     171,    12,   153,   148,   150,   177,   189,     0,   147,   173,
       0,   155
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -516,  -516,  -516,  -516,  -516,   -60,   872,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,   877,   -30,  -308,  -516,  -516,   -61,
     879,   -41,   764,   570,   -44,  -117,   786,   -68,  -516,  -516,
    -516,  -372,  -515,  -516,  -516,  -516,  -516,  -248,  -516,   -35,
    -257,  -231,  -185,   251,  -516,   453,  -516,  -516,   287,  -516,
     276,   279,   906,   -31,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,   272,   -40,
     477,   676,  -502,  -484,  -135,  -516,   611,  -516,   735,   -15,
    -516,   -75,  -516,  -516,  -304,  -516,  -160,   437,  -516,  -516,
     536,  -516,   -39,  -516,  -516,   -34,  -169,   662,  -516,   -28,
     935,  -516,  -516,  -516,  -516,  -516,  -516,  -516,   464,  -464,
     465,  -516,  -516,  -516,  -516,  -516,   -36,   397,   -57,  -198,
    -103,  -516,   778
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    31,     5,     6,    26,     7,   128,
     236,   512,   631,    35,    37,    38,   439,   737,   559,   419,
      51,    52,   425,   426,   143,   167,   412,   168,   792,   819,
     445,   472,   284,   436,   659,   815,   169,   170,   171,   172,
     596,   421,   422,   566,   567,   568,   744,   868,   820,   670,
     840,   841,    42,    43,    27,   584,   756,   314,   832,   861,
     796,   845,   746,   825,   773,   774,   805,   848,    72,   177,
     305,   306,   581,   464,   269,   405,   406,   277,   278,    44,
      28,    47,   183,    48,   476,   477,    98,    99,   100,   487,
     488,   101,   144,   103,   104,   145,   331,   332,   106,   146,
     343,   108,   109,   110,   111,   480,   695,   597,   598,   842,
     600,   112,   113,   114,   115,   191,   333,   148,   149,   150,
     151,   242,   243
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,   127,   465,    45,    97,   165,   178,    67,   187,   102,
     119,    73,   193,   165,   105,    60,   599,   478,   199,   270,
     107,   481,   166,   316,   420,   423,   639,   578,   237,   147,
     166,   413,   651,   245,   339,   448,    53,   530,   182,    79,
     275,   836,   460,   309,   460,   700,    54,   441,   188,   220,
     192,   189,   194,   195,   793,   174,   165,   190,   200,   446,
     447,   179,   388,   390,   392,   394,   396,   397,   398,   526,
     674,   663,   675,   166,   677,   238,   239,   181,   299,   353,
     247,   442,   738,   668,    63,     1,   794,   673,   701,   413,
     550,   310,     2,   739,   165,   165,   300,   579,   241,   244,
     786,   218,   301,   669,   461,   651,   302,   687,   449,   450,
     451,   452,   837,   166,   311,    96,    49,   248,   334,   335,
     272,   273,    68,   249,   250,   251,   252,   253,   254,   527,
     558,   664,   720,    50,   721,   303,   176,    64,   354,    97,
     712,   713,   321,  -224,   102,  -224,   304,    79,    70,   105,
      75,   509,    97,    97,   692,   107,   255,   102,   102,    71,
     536,   307,   105,   105,   399,   693,   427,   491,   107,   107,
     341,     8,    80,   344,   345,    69,    76,   344,   348,   349,
     350,   351,   428,   322,   534,   535,   429,    77,   745,   517,
     694,   518,   430,   519,   563,   520,   565,   521,   471,   383,
     384,   385,   386,    69,    69,   606,   356,    65,    69,    79,
      74,   361,   362,   248,    66,   364,   684,    79,   165,   249,
     250,   251,   252,   253,   254,    79,   771,    10,   599,   274,
     607,   484,   431,   299,   324,   166,   432,   427,   400,   166,
      79,   608,   605,    11,   433,   434,   435,    96,    69,   440,
     609,   300,   255,   407,   469,   126,   401,   301,   454,   486,
     489,   302,   402,   293,   294,   769,   403,   541,   542,   437,
     800,   166,    29,   468,   440,   295,   165,    30,    32,   543,
     643,   473,   474,   475,   455,   641,   457,    79,   189,   459,
     690,   809,   471,   166,   479,   404,   296,   698,   424,    33,
     544,   304,    89,    34,   748,   319,    39,   644,   645,   470,
     166,   753,    12,    91,   141,   824,    96,   595,    78,   505,
     506,   573,   829,   575,   400,   613,   615,    96,   119,   618,
     256,    36,    96,    67,   515,   241,    73,   257,   258,   772,
     538,   539,   401,   286,   204,   580,   205,   778,   402,   850,
     779,   545,   403,   735,   736,   206,   646,   647,   648,   649,
     428,   259,   290,   291,   725,   166,   287,    57,   288,    39,
     430,   289,    41,   562,   292,   564,   207,    58,   208,   209,
     165,   165,   165,   165,   410,   522,    46,   524,   635,    55,
     637,   411,    39,   765,   538,   539,   767,   166,   166,   166,
     166,   279,   560,   129,   810,    56,   691,   130,   279,   812,
     431,   580,   590,   570,   432,   538,   539,   740,   166,   679,
     538,   539,   433,   434,   435,    59,   642,   587,    61,   427,
     569,   724,   619,    97,   759,   628,   629,   482,   102,   165,
     551,   371,   835,   105,   483,   307,   372,   298,   603,   107,
     552,   482,   313,   802,   803,   580,   166,   580,   493,   580,
     616,   538,   539,   622,   623,   260,   261,   262,   297,    62,
     540,   471,   165,   632,   633,   312,    69,   263,   264,   352,
     816,   118,   817,    69,   166,   502,   507,    69,   660,   166,
    -268,   508,  -268,    69,    69,   165,   359,   360,   265,    69,
     827,   532,   166,   266,   320,   611,   184,   546,   482,   533,
     185,   407,   166,   533,   547,   610,   546,   580,   482,   580,
     482,   116,   120,   620,   682,   704,   117,   705,   166,   248,
     166,   122,   166,   703,   482,   249,   250,   251,   252,   253,
     254,   706,    13,    14,   166,    15,   496,   788,   317,   510,
     121,   497,   318,   709,   789,   716,   688,   689,   496,   471,
     165,    16,   123,   499,   594,   176,   471,    17,   255,   453,
      18,    19,   511,  -270,    21,  -270,   665,   166,   203,   196,
      69,   666,   462,   229,   467,   230,   714,   715,   231,   248,
     166,  -269,   166,  -269,   471,   249,   250,   251,   252,   253,
     254,   232,   471,   124,   165,   471,   661,   233,   538,   539,
     198,   234,   726,   727,   235,    22,   730,   731,   761,   762,
      69,   166,   528,    23,    24,   440,   440,    25,   255,   503,
     504,   852,   166,   328,   329,   853,   437,   757,   513,   166,
     514,   125,   173,   307,   751,   375,   376,   377,   378,   379,
     380,   381,   382,   569,   175,   180,   152,   153,   207,   471,
     208,   209,   248,   766,   471,   186,   154,   166,   249,   250,
     251,   252,   253,   254,   201,   166,   795,    69,   166,   556,
     152,   153,   202,   165,   155,   156,   797,   221,   782,   783,
     154,   157,   222,    69,   158,   572,   801,   471,   224,   223,
     166,   255,   159,   160,   161,   162,   163,   225,   155,   156,
     164,   798,   226,   807,   808,   157,   227,   246,   158,   212,
     213,    69,    81,   574,    96,   228,   159,   160,   161,   162,
     163,    69,   166,   576,   219,   198,   822,   166,    82,    83,
      84,   203,   268,    85,    86,   828,   267,    69,    96,   634,
     838,   839,   440,    69,   440,   636,    87,   165,   165,    88,
     159,   160,   161,   162,   271,   846,    69,   696,   671,   697,
     166,   696,   849,   855,   166,   166,   276,   285,   283,   626,
     627,   308,    81,    89,   336,   857,   315,   337,   859,   355,
     408,    90,   340,   342,    91,    92,   282,   346,    82,    83,
      84,   867,    93,    85,    86,    94,    95,   357,    13,    14,
     358,    15,   366,    96,   758,   409,    87,   368,   374,    88,
     652,   653,   279,   414,   443,   656,   657,    16,   444,   456,
     458,   463,   466,    17,   485,   494,    18,    19,    20,   248,
      21,   495,   500,    89,   498,   249,   250,   251,   252,   253,
     254,   523,   592,   569,    91,   327,   593,   525,   529,   417,
     561,   548,    93,   537,   549,    94,    95,   159,   160,   161,
     162,   418,   571,    96,   553,   582,   248,   323,   255,   554,
     555,    22,   249,   250,   251,   252,   253,   254,   583,    23,
      24,   248,   585,    25,   586,   588,   589,   249,   250,   251,
     252,   253,   254,   591,   601,   604,   621,   624,   625,   630,
     640,   667,   131,   654,   655,   255,   658,   662,   672,   680,
     676,   678,   681,   132,   133,   683,   728,   729,   131,   686,
     255,   134,   240,   135,   685,   707,   136,   708,   710,   132,
     133,   137,   138,   139,   140,   141,   711,   134,    79,   135,
     330,   717,   136,   718,   131,   719,   142,   137,   138,   139,
     140,   141,   734,    96,   722,   132,   133,   733,   723,   741,
     131,   747,   142,   134,   763,   135,   338,   743,   136,    96,
     752,   132,   133,   137,   138,   139,   140,   141,   749,   134,
     750,   135,   490,   754,   136,   755,   131,   770,   142,   137,
     138,   139,   140,   141,   768,    96,   775,   132,   133,   776,
     777,   780,   131,   784,   142,   134,   781,   135,   612,   785,
     136,    96,   787,   132,   133,   137,   138,   139,   140,   141,
     790,   134,   799,   135,   614,   791,   136,   804,   131,   806,
     142,   137,   138,   139,   140,   141,   811,    96,   813,   132,
     133,   814,   818,   821,   131,   823,   142,   134,   843,   135,
     617,   826,   136,    96,   830,   132,   133,   137,   138,   139,
     140,   141,   831,   134,   833,   135,   834,   844,   136,   847,
     131,   851,   142,   137,   138,   139,   140,   141,   854,    96,
     858,   132,   133,   860,   865,   863,   131,   866,   142,   134,
     871,   438,   869,   365,   136,    96,   363,   132,   133,   137,
     138,   139,   140,   141,   369,   134,   650,   438,   501,   870,
     136,   742,   131,   492,   142,   137,   138,   139,   140,   557,
     856,    96,   864,   862,   387,   732,   577,   638,   131,   367,
     142,   134,   702,   135,   602,   531,   136,    96,   347,   516,
     389,   137,   138,   139,   140,   141,   760,   134,     0,   135,
       0,   764,   136,     0,   131,     0,   142,   137,   138,   139,
     140,   141,     0,    96,     0,     0,   391,     0,     0,     0,
     131,     0,   142,   134,     0,   135,     0,     0,   136,    96,
       0,     0,   393,   137,   138,   139,   140,   141,     0,   134,
       0,   135,     0,     0,   136,     0,   131,     0,   142,   137,
     138,   139,   140,   141,     0,    96,     0,     0,   395,     0,
       0,     0,   131,     0,   142,   134,     0,   135,     0,     0,
     136,    96,     0,     0,     0,   137,   138,   139,   140,   141,
       0,   134,     0,   135,     0,     0,   136,     0,     0,     0,
     142,   137,   138,   139,   140,   141,     0,    96,     0,   325,
       0,     0,   155,   156,     0,     0,   142,     0,   248,     0,
       0,     0,   158,    96,   249,   250,   251,   252,   253,   254,
     159,   160,   161,   162,   163,   248,     0,     0,     0,     0,
       0,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,    96,   248,     0,     0,     0,   255,   326,   249,
     250,   251,   252,   253,   254,     0,   415,   416,     0,     0,
       0,     0,   248,     0,   255,     0,   417,   370,   249,   250,
     251,   252,   253,   254,   159,   160,   161,   162,   418,     0,
     248,     0,   255,     0,     0,   373,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,   248,     0,   699,     0,
       0,   255,   249,   250,   251,   252,   253,   254,     0,     0,
     -50,   -50,     0,     0,   279,  -300,     0,     0,     0,   255,
     -50,   -50,   -50,     0,     0,   280,     0,     0,   281,  -300,
     282,     0,   -50,     0,     0,   255,  -300,   197,  -300,  -300,
       0,   198,  -293,   -50,     0,  -300,  -300,  -300,  -300,     0,
    -300,     0,  -300,  -300,     0,   198,   210,     0,     0,  -300,
    -300,  -300,  -300,  -293,     0,  -293,  -293,     0,   498,     0,
       0,     0,  -293,  -293,  -293,  -293,     0,   211,     0,   212,
     213,     0,     0,     0,     0,     0,   214,   215,   216,   217
};

static const yytype_int16 yycheck[] =
{
      15,    61,   310,    18,    48,    66,    74,    37,    83,    48,
      51,    42,    87,    74,    48,    30,   480,   321,    93,   154,
      48,   325,    66,   183,   281,   282,   528,     6,   131,    65,
      74,   279,   547,   136,   203,   292,    26,   409,    78,     5,
     157,     5,     5,     5,     5,    17,    36,    28,    84,   117,
      86,    85,    88,    89,    28,    70,   117,    85,    94,   290,
     291,    76,   260,   261,   262,   263,   264,   265,   266,     5,
     572,     5,   574,   117,   576,   132,   133,     5,     8,     5,
       5,    62,    65,     5,     5,     4,    60,   571,    60,   337,
       5,    53,    11,    76,   155,   156,    26,    76,   134,   135,
      63,   116,    32,    25,    65,   620,    36,   591,   293,   294,
     295,   296,    76,   157,    76,    94,    76,    42,    84,    85,
     155,   156,     5,    48,    49,    50,    51,    52,    53,    65,
     438,    65,   634,    93,   636,    65,    64,    58,    64,   183,
     624,   625,    17,    58,   183,    60,    76,     5,    48,   183,
       5,     5,   196,   197,     5,   183,    81,   196,   197,    59,
     417,   176,   196,   197,   267,    16,   283,   336,   196,   197,
     206,    76,    30,   209,   210,    58,    48,   213,   214,   215,
     216,   217,    26,    58,   415,   416,    30,    59,   672,   387,
      41,   389,    36,   391,   442,   393,   444,   395,   315,   256,
     257,   258,   259,    58,    58,     5,   221,    53,    58,     5,
      60,   226,   227,    42,    60,   230,   588,     5,   279,    48,
      49,    50,    51,    52,    53,     5,   710,    76,   692,    76,
      30,    60,    76,     8,    30,   279,    80,   354,     8,   283,
       5,    41,    30,     0,    88,    89,    90,    94,    58,   285,
      30,    26,    81,   268,   314,    65,    26,    32,   298,   334,
     335,    36,    32,    44,    45,    30,    36,    44,    45,   284,
     754,   315,    59,   313,   310,    56,   337,    64,     5,    56,
     537,   317,   318,   319,   299,   533,   301,     5,   322,   304,
     594,   775,   409,   337,   322,    65,    77,   601,    76,    72,
      77,    76,    64,    76,   676,    23,    76,   538,   539,    76,
     354,   683,    59,    75,    76,   799,    94,   477,    46,   359,
     360,   456,   806,   458,     8,   494,   495,    94,   369,   498,
      47,    76,    94,   363,   370,   371,   367,    54,    55,   711,
      54,    55,    26,     6,    59,   462,    61,   719,    32,   833,
     722,    65,    36,   661,   662,    38,   541,   542,   543,   544,
      26,    78,    54,    55,    30,   409,    29,    26,    31,    76,
      36,    34,    76,   441,    66,   443,    59,    36,    61,    62,
     441,   442,   443,   444,    58,   400,    76,   402,   523,    76,
     525,    65,    76,   697,    54,    55,   700,   441,   442,   443,
     444,    48,   438,    72,   776,    76,    66,    76,    48,   781,
      76,   528,    59,   453,    80,    54,    55,   665,   462,    59,
      54,    55,    88,    89,    90,    76,    65,   467,     5,   546,
     445,    65,   500,   477,   691,   510,   511,    58,   477,   500,
      26,    58,   814,   477,    65,   460,    63,   175,   484,   477,
      36,    58,   180,   757,   758,   572,   500,   574,    65,   576,
     496,    54,    55,   503,   504,    44,    45,    46,    50,    12,
      63,   588,   533,   513,   514,    50,    58,    56,    57,    50,
     788,    76,   790,    58,   528,    50,    50,    58,   556,   533,
      59,    50,    61,    58,    58,   556,   224,   225,    77,    58,
     804,    50,   546,    82,    17,    50,    72,    58,    58,    58,
      76,   526,   556,    58,    65,    65,    58,   634,    58,   636,
      58,    48,    76,    65,   584,    65,    53,    65,   572,    42,
     574,    59,   576,   608,    58,    48,    49,    50,    51,    52,
      53,    65,     7,     8,   588,    10,    58,    58,    38,    14,
      76,    63,    42,   621,    65,   630,   592,   593,    58,   676,
     621,    26,    59,    63,    17,    64,   683,    32,    81,   297,
      35,    36,    37,    59,    39,    61,    58,   621,    64,    60,
      58,    63,    60,     8,   312,    10,   626,   627,    13,    42,
     634,    59,   636,    61,   711,    48,    49,    50,    51,    52,
      53,    26,   719,    76,   665,   722,    60,    32,    54,    55,
      64,    36,   652,   653,    39,    80,   656,   657,   693,   694,
      58,   665,    60,    88,    89,   661,   662,    92,    81,   357,
     358,    72,   676,   196,   197,    76,   651,    17,   366,   683,
     368,    76,    76,   658,   680,   248,   249,   250,   251,   252,
     253,   254,   255,   668,    76,    76,    26,    27,    59,   776,
      61,    62,    42,   699,   781,    76,    36,   711,    48,    49,
      50,    51,    52,    53,    76,   719,   744,    58,   722,    60,
      26,    27,    76,   744,    54,    55,   746,    48,   728,   729,
      36,    61,    59,    58,    64,    60,   756,   814,    76,    59,
     744,    81,    72,    73,    74,    75,    76,    76,    54,    55,
      80,   747,    48,   773,   774,    61,    48,    59,    64,    61,
      62,    58,     3,    60,    94,    59,    72,    73,    74,    75,
      76,    58,   776,    60,    80,    64,   796,   781,    19,    20,
      21,    64,    64,    24,    25,   805,    79,    58,    94,    60,
     818,   819,   788,    58,   790,    60,    37,   818,   819,    40,
      72,    73,    74,    75,    76,   825,    58,    58,    60,    60,
     814,    58,   832,    60,   818,   819,    76,    53,    64,   507,
     508,     5,     3,    64,    64,   845,    60,    48,   848,     5,
      28,    72,    76,    76,    75,    76,    64,    76,    19,    20,
      21,   861,    83,    24,    25,    86,    87,    76,     7,     8,
      76,    10,    76,    94,    17,    60,    37,    76,    76,    40,
     548,   549,    48,    76,    28,   553,   554,    26,    28,    76,
      76,     9,    76,    32,    91,    64,    35,    36,    37,    42,
      39,    64,    53,    64,    64,    48,    49,    50,    51,    52,
      53,    76,    18,   868,    75,    76,    22,    76,    76,    64,
       5,    76,    83,    64,    76,    86,    87,    72,    73,    74,
      75,    76,     5,    94,    76,     5,    42,    28,    81,    76,
      76,    80,    48,    49,    50,    51,    52,    53,    76,    88,
      89,    42,     5,    92,     5,    60,     5,    48,    49,    50,
      51,    52,    53,     5,    41,    76,    53,     5,     5,    15,
      28,    30,    43,    76,    76,    81,    62,    58,     5,    53,
      60,    28,     5,    54,    55,    60,   654,   655,    43,     9,
      81,    62,    63,    64,    76,     5,    67,     5,     5,    54,
      55,    72,    73,    74,    75,    76,    60,    62,     5,    64,
      65,    30,    67,     5,    43,    60,    87,    72,    73,    74,
      75,    76,     5,    94,    60,    54,    55,    60,    76,    28,
      43,    53,    87,    62,    30,    64,    65,    76,    67,    94,
       5,    54,    55,    72,    73,    74,    75,    76,    76,    62,
      76,    64,    65,     5,    67,     5,    43,     5,    87,    72,
      73,    74,    75,    76,    76,    94,     5,    54,    55,    60,
      59,     5,    43,     5,    87,    62,    60,    64,    65,     5,
      67,    94,    76,    54,    55,    72,    73,    74,    75,    76,
      60,    62,     5,    64,    65,    63,    67,    17,    43,     5,
      87,    72,    73,    74,    75,    76,     5,    94,     5,    54,
      55,    60,    28,    76,    43,     5,    87,    62,    28,    64,
      65,     5,    67,    94,     5,    54,    55,    72,    73,    74,
      75,    76,     5,    62,     5,    64,     5,     5,    67,     5,
      43,     5,    87,    72,    73,    74,    75,    76,     5,    94,
       5,    54,    55,     5,     5,    64,    43,     5,    87,    62,
      65,    64,     5,   231,    67,    94,   229,    54,    55,    72,
      73,    74,    75,    76,   235,    62,   546,    64,   354,   868,
      67,   668,    43,   337,    87,    72,    73,    74,    75,    76,
     843,    94,   856,   854,    55,   658,   460,   526,    43,   233,
      87,    62,   606,    64,   482,   410,    67,    94,   213,   371,
      55,    72,    73,    74,    75,    76,   692,    62,    -1,    64,
      -1,   696,    67,    -1,    43,    -1,    87,    72,    73,    74,
      75,    76,    -1,    94,    -1,    -1,    55,    -1,    -1,    -1,
      43,    -1,    87,    62,    -1,    64,    -1,    -1,    67,    94,
      -1,    -1,    55,    72,    73,    74,    75,    76,    -1,    62,
      -1,    64,    -1,    -1,    67,    -1,    43,    -1,    87,    72,
      73,    74,    75,    76,    -1,    94,    -1,    -1,    55,    -1,
      -1,    -1,    43,    -1,    87,    62,    -1,    64,    -1,    -1,
      67,    94,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,
      87,    72,    73,    74,    75,    76,    -1,    94,    -1,    33,
      -1,    -1,    54,    55,    -1,    -1,    87,    -1,    42,    -1,
      -1,    -1,    64,    94,    48,    49,    50,    51,    52,    53,
      72,    73,    74,    75,    76,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    94,    42,    -1,    -1,    -1,    81,    65,    48,
      49,    50,    51,    52,    53,    -1,    54,    55,    -1,    -1,
      -1,    -1,    42,    -1,    81,    -1,    64,    66,    48,    49,
      50,    51,    52,    53,    72,    73,    74,    75,    76,    -1,
      42,    -1,    81,    -1,    -1,    65,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    -1,    -1,
      44,    45,    -1,    -1,    48,    38,    -1,    -1,    -1,    81,
      54,    55,    56,    -1,    -1,    59,    -1,    -1,    62,    38,
      64,    -1,    66,    -1,    -1,    81,    59,    60,    61,    62,
      -1,    64,    38,    77,    -1,    68,    69,    70,    71,    -1,
      59,    -1,    61,    62,    -1,    64,    38,    -1,    -1,    68,
      69,    70,    71,    59,    -1,    61,    62,    -1,    64,    -1,
      -1,    -1,    68,    69,    70,    71,    -1,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    96,    97,   100,   101,   103,    76,    98,
      76,     0,    59,     7,     8,    10,    26,    32,    35,    36,
      37,    39,    80,    88,    89,    92,   102,   149,   175,    59,
      64,    99,     5,    72,    76,   108,    76,   109,   110,    76,
     174,    76,   147,   148,   174,   174,    76,   176,   178,    76,
      93,   115,   116,    26,    36,    76,    76,    26,    36,    76,
     174,     5,    12,     5,    58,    53,    60,   110,     5,    58,
      48,    59,   163,   148,    60,     5,    48,    59,   163,     5,
      30,     3,    19,    20,    21,    24,    25,    37,    40,    64,
      72,    75,    76,    83,    86,    87,    94,   119,   181,   182,
     183,   186,   187,   188,   189,   190,   193,   194,   196,   197,
     198,   199,   206,   207,   208,   209,    48,    53,    76,   116,
      76,    76,    59,    59,    76,    76,    65,   100,   104,    72,
      76,    43,    54,    55,    62,    64,    67,    72,    73,    74,
      75,    76,    87,   119,   187,   190,   194,   211,   212,   213,
     214,   215,    26,    27,    36,    54,    55,    61,    64,    72,
      73,    74,    75,    76,    80,   114,   119,   120,   122,   131,
     132,   133,   134,    76,   174,    76,    64,   164,   122,   174,
      76,     5,   164,   177,    72,    76,    76,   176,   211,   190,
     194,   210,   211,   176,   211,   211,    60,    60,    64,   176,
     211,    76,    76,    64,    59,    61,    38,    59,    61,    62,
      38,    59,    61,    62,    68,    69,    70,    71,   174,    80,
     122,    48,    59,    59,    76,    76,    48,    48,    59,     8,
      10,    13,    26,    32,    36,    39,   105,   215,   213,   213,
      63,   211,   216,   217,   211,   215,    59,     5,    42,    48,
      49,    50,    51,    52,    53,    81,    47,    54,    55,    78,
      44,    45,    46,    56,    57,    77,    82,    79,    64,   169,
     169,    76,   134,   134,    76,   120,    76,   172,   173,    48,
      59,    62,    64,    64,   127,    53,     6,    29,    31,    34,
      54,    55,    66,    44,    45,    56,    77,    50,   163,     8,
      26,    32,    36,    65,    76,   165,   166,   174,     5,     5,
      53,    76,    50,   163,   152,    60,   181,    38,    42,    23,
      17,    17,    58,    28,    30,    33,    65,    76,   182,   182,
      65,   191,   192,   211,    84,    85,    64,    48,    65,   191,
      76,   211,    76,   195,   211,   211,    76,   195,   211,   211,
     211,   211,    50,     5,    64,     5,   174,    76,    76,   163,
     163,   174,   174,   109,   174,   101,    76,   147,    76,   115,
      66,    58,    63,    65,    76,   212,   212,   212,   212,   212,
     212,   212,   212,   213,   213,   213,   213,    55,   214,    55,
     214,    55,   214,    55,   214,    55,   214,   214,   214,   215,
       8,    26,    32,    36,    65,   170,   171,   174,    28,    60,
      58,    65,   121,   132,    76,    54,    55,    64,    76,   114,
     135,   136,   137,   135,    76,   117,   118,   120,    26,    30,
      36,    76,    80,    88,    89,    90,   128,   174,    64,   111,
     211,    28,    62,    28,    28,   125,   136,   136,   135,   137,
     137,   137,   137,   163,   164,   174,    76,   174,    76,   174,
       5,    65,    60,     9,   168,   111,    76,   163,   164,   100,
      76,   120,   126,   211,   211,   211,   179,   180,   179,   194,
     200,   179,    58,    65,    60,    91,   176,   184,   185,   176,
      65,   191,   121,    65,    64,    64,    58,    63,    64,    63,
      53,   117,    50,   163,   163,   164,   164,    50,    50,     5,
      14,    37,   106,   163,   163,   211,   217,   214,   214,   214,
     214,   214,   174,    76,   174,    76,     5,    65,    60,    76,
     126,   173,    50,    58,   136,   136,   135,    64,    54,    55,
      63,    44,    45,    56,    77,    65,    58,    65,    76,    76,
       5,    26,    36,    76,    76,    76,    60,    76,   111,   113,
     211,     5,   122,   132,   122,   132,   138,   139,   140,   174,
     164,     5,    60,   169,    60,   169,    60,   166,     6,    76,
     120,   167,     5,    76,   150,     5,     5,   164,    60,     5,
      59,     5,    18,    22,    17,   181,   135,   202,   203,   204,
     205,    41,   192,   211,    76,    30,     5,    30,    41,    30,
      65,    50,    65,   191,    65,   191,   211,    65,   191,   122,
      65,    53,   164,   164,     5,     5,   163,   163,   176,   176,
      15,   107,   164,   164,    60,   169,    60,   169,   171,   167,
      28,   132,    65,   135,   136,   136,   137,   137,   137,   137,
     118,   127,   163,   163,    76,    76,   163,   163,    62,   129,
     122,    60,    58,     5,    65,    58,    63,    30,     5,    25,
     144,    60,     5,   168,   167,   167,    60,   167,    28,    59,
      53,     5,   100,    60,   126,    76,     9,   168,   211,   211,
     179,    66,     5,    16,    41,   201,    58,    60,   179,    60,
      17,    60,   185,   176,    65,    65,    65,     5,     5,   122,
       5,    60,   168,   168,   164,   164,   176,    30,     5,    60,
     167,   167,    60,    76,    65,    30,   164,   164,   163,   163,
     164,   164,   165,    60,     5,   111,   111,   112,    65,    76,
     132,    28,   140,    76,   141,   168,   157,    53,   126,    76,
      76,   211,     5,   126,     5,     5,   151,    17,    17,   135,
     203,   176,   176,    30,   205,   179,   211,   179,    76,    30,
       5,   168,   126,   159,   160,     5,    60,    59,   126,   126,
       5,    60,   164,   164,     5,     5,    63,    76,    58,    65,
      60,    63,   123,    28,    60,   122,   155,   100,   211,     5,
     168,   100,   179,   179,    17,   161,     5,   100,   100,   168,
     126,     5,   126,     5,    60,   130,   111,   111,    28,   124,
     143,    76,   100,     5,   168,   158,     5,   179,   100,   168,
       5,     5,   153,     5,     5,   126,     5,    76,   122,   122,
     145,   146,   204,    28,     5,   156,   100,     5,   162,   100,
     168,     5,    72,    76,     5,    60,   143,   100,     5,   100,
       5,   154,   146,    64,   145,     5,     5,   100,   142,     5,
     138,    65
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    95,    96,    96,    97,    97,    98,    98,    99,    99,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   102,
     103,   104,   104,   105,   105,   105,   105,   105,   105,   106,
     106,   106,   107,   107,   108,   108,   108,   108,   109,   109,
     110,   110,   111,   111,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   114,   114,   115,   115,   116,   116,   116,
     116,   116,   117,   117,   118,   118,   119,   120,   120,   121,
     121,   122,   122,   122,   123,   124,   122,   122,   122,   122,
     122,   122,   122,   125,   122,   122,   122,   122,   122,   122,
     126,   126,   126,   127,   127,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   129,   129,   130,   130,   130,
     131,   131,   132,   132,   132,   132,   132,   132,   133,   133,
     133,   133,   133,   134,   134,   134,   134,   134,   134,   135,
     135,   135,   135,   135,   136,   136,   136,   136,   136,   137,
     137,   137,   138,   139,   139,   141,   140,   140,   142,   143,
     144,   144,   144,   145,   145,   146,   146,   147,   147,   148,
     148,   148,   149,   149,   150,   149,   151,   149,   152,   149,
     153,   149,   154,   149,   155,   149,   156,   149,   157,   149,
     158,   149,   159,   149,   160,   149,   161,   149,   162,   149,
     163,   164,   164,   164,   165,   165,   166,   166,   166,   166,
     166,   166,   166,   166,   167,   167,   167,   167,   168,   168,
     169,   169,   169,   170,   170,   171,   171,   171,   171,   171,
     172,   172,   173,   174,   174,   175,   177,   176,   178,   176,
     180,   179,   181,   181,   181,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     183,   183,   183,   183,   184,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   188,   188,   188,   188,   189,   189,
     189,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   191,   191,   192,   192,   192,   193,   193,   193,   193,
     193,   193,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   195,   195,   196,   197,   197,   198,   198,   200,   199,
     201,   201,   201,   202,   202,   203,   203,   204,   204,   205,
     205,   206,   207,   208,   208,   208,   209,   210,   210,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   212,   212,
     212,   212,   212,   212,   212,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   214,   214,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   217,   217
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,     3,
      11,     2,     0,     3,     2,     2,     2,     5,     7,     2,
       2,     0,     2,     0,     3,     3,     1,     1,     2,     1,
       4,     6,     1,     5,     3,     4,     1,     3,     3,     5,
       1,     1,     1,     1,     1,     2,     1,     4,     8,     7,
       4,     7,     1,     3,     1,     1,     5,     1,     4,     1,
       3,     1,     2,     2,     0,     0,     9,     4,     2,     4,
       4,     6,     9,     0,     5,     4,     2,     4,     3,     6,
       1,     3,     1,     0,     2,     1,     2,     4,     5,     7,
       5,     5,     6,     8,     7,     0,     3,     0,     3,     3,
       1,     0,     3,     1,     4,     3,     1,     3,     1,     2,
       2,     3,     3,     1,     4,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     1,     3,     3,     3,     3,     1,
       4,     3,     2,     3,     1,     0,     4,     0,     0,     0,
       7,     5,     0,     3,     1,     6,     0,     2,     1,     4,
       6,     6,     7,     9,     0,     9,     0,    11,     0,     7,
       0,    13,     0,    15,     0,    12,     0,    14,     0,    11,
       0,    13,     0,    11,     0,    11,     0,    12,     0,    14,
       0,     3,     2,     0,     3,     1,     3,     5,     4,     3,
       4,     5,     6,     4,     1,     3,     3,     1,     0,     3,
       3,     2,     0,     3,     1,     3,     4,     4,     3,     5,
       3,     1,     1,     3,     1,     3,     0,     4,     0,     2,
       0,     2,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       5,     5,     7,     5,     1,     3,     2,     6,     4,     1,
       2,     2,     4,     5,     1,     1,     3,     1,     1,     3,
       1,     4,     4,     3,     3,     6,     5,     6,     5,     6,
       5,     3,     1,     1,     3,     5,     3,     3,     3,     3,
       3,     3,     4,     3,     2,     3,     4,     2,     3,     2,
       1,     3,     1,     3,     2,     2,     4,     6,     0,     7,
       0,     2,     2,     3,     1,     3,     0,     3,     1,     1,
       3,     4,     4,     8,     8,     6,     4,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     4,     3,     3,     1,     3,
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
#line 216 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2158 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 217 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2164 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 220 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2170 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 221 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2176 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 224 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2182 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 225 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2188 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 228 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2194 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 229 "pascal.y"
     { (yyval.node) = NULL; }
#line 2200 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 232 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2206 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 235 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2212 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 236 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2218 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 239 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2224 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 240 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2230 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 241 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2236 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 242 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2242 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 243 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2248 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 244 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2254 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 245 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2260 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 248 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2266 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 251 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2272 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 252 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2278 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY id_list SEMICOLON  */
#line 255 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2284 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 256 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2290 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 257 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2296 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 258 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2302 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 259 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2308 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 260 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2314 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 263 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2320 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 264 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2326 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 265 "pascal.y"
     { (yyval.node) = NULL; }
#line 2332 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 268 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2338 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 269 "pascal.y"
     { (yyval.node) = NULL; }
#line 2344 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 272 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2350 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 273 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2356 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 274 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2362 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 275 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2368 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 278 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2374 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 279 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2380 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 282 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2386 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 283 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2392 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 286 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2398 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 287 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2404 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 288 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2410 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 289 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2416 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 292 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2422 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 293 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2428 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 296 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2434 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 297 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2440 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 300 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2446 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 301 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2452 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 302 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2458 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 303 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2464 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 304 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2470 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 307 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2476 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 308 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2482 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 311 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2488 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 312 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2494 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 313 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2500 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 314 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2506 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 315 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2512 "pascal.tab.c"
    break;

  case 62: /* class_parents: parent_item  */
#line 318 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2518 "pascal.tab.c"
    break;

  case 63: /* class_parents: class_parents COMMA parent_item  */
#line 319 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2524 "pascal.tab.c"
    break;

  case 64: /* parent_item: IDENT  */
#line 322 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2530 "pascal.tab.c"
    break;

  case 65: /* parent_item: spec_ref  */
#line 323 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2536 "pascal.tab.c"
    break;

  case 66: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 326 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2542 "pascal.tab.c"
    break;

  case 67: /* spec_ref: spec_kw  */
#line 329 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2548 "pascal.tab.c"
    break;

  case 68: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 330 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2554 "pascal.tab.c"
    break;

  case 69: /* type_arg_list: simple_type  */
#line 333 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2560 "pascal.tab.c"
    break;

  case 70: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 334 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2566 "pascal.tab.c"
    break;

  case 71: /* type: simple_type  */
#line 337 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2572 "pascal.tab.c"
    break;

  case 72: /* type: ARROW IDENT  */
#line 338 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2578 "pascal.tab.c"
    break;

  case 73: /* type: ARROW spec_ref  */
#line 339 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2584 "pascal.tab.c"
    break;

  case 74: /* $@1: %empty  */
#line 340 "pascal.y"
                                                       { }
#line 2590 "pascal.tab.c"
    break;

  case 75: /* $@2: %empty  */
#line 340 "pascal.y"
                                                           { }
#line 2596 "pascal.tab.c"
    break;

  case 76: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 340 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2602 "pascal.tab.c"
    break;

  case 77: /* type: packed_opt ARRAYSY OFSY type  */
#line 341 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2608 "pascal.tab.c"
    break;

  case 78: /* type: PROCEDURESY pf_params  */
#line 342 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2614 "pascal.tab.c"
    break;

  case 79: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 343 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2620 "pascal.tab.c"
    break;

  case 80: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 344 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2626 "pascal.tab.c"
    break;

  case 81: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 345 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2632 "pascal.tab.c"
    break;

  case 82: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 346 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2638 "pascal.tab.c"
    break;

  case 83: /* $@3: %empty  */
#line 347 "pascal.y"
                         { }
#line 2644 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 347 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2650 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt SETSY OFSY simple_type  */
#line 348 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2656 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt FILESY  */
#line 349 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2662 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt FILESY OFSY type  */
#line 350 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2668 "pascal.tab.c"
    break;

  case 88: /* type: CLASSSY class_members ENDSY  */
#line 351 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2674 "pascal.tab.c"
    break;

  case 89: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 352 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2680 "pascal.tab.c"
    break;

  case 90: /* result_type: IDENT  */
#line 355 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2686 "pascal.tab.c"
    break;

  case 91: /* result_type: IDENT PERIOD IDENT  */
#line 356 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2692 "pascal.tab.c"
    break;

  case 92: /* result_type: spec_ref  */
#line 357 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2698 "pascal.tab.c"
    break;

  case 93: /* class_members: %empty  */
#line 360 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2704 "pascal.tab.c"
    break;

  case 94: /* class_members: class_members class_member  */
#line 361 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2710 "pascal.tab.c"
    break;

  case 95: /* class_member: IDENT  */
#line 364 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2716 "pascal.tab.c"
    break;

  case 96: /* class_member: IDENT SEMICOLON  */
#line 365 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2722 "pascal.tab.c"
    break;

  case 97: /* class_member: id_list COLON type SEMICOLON  */
#line 366 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2728 "pascal.tab.c"
    break;

  case 98: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 367 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2734 "pascal.tab.c"
    break;

  case 99: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 368 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2740 "pascal.tab.c"
    break;

  case 100: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 369 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2746 "pascal.tab.c"
    break;

  case 101: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 370 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2752 "pascal.tab.c"
    break;

  case 102: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 371 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2758 "pascal.tab.c"
    break;

  case 103: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 372 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2764 "pascal.tab.c"
    break;

  case 104: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 373 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2770 "pascal.tab.c"
    break;

  case 105: /* prop_index_opt: %empty  */
#line 376 "pascal.y"
    { (yyval.node) = NULL; }
#line 2776 "pascal.tab.c"
    break;

  case 106: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 377 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2782 "pascal.tab.c"
    break;

  case 107: /* prop_specs: %empty  */
#line 380 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2788 "pascal.tab.c"
    break;

  case 108: /* prop_specs: prop_specs IDENT IDENT  */
#line 381 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2794 "pascal.tab.c"
    break;

  case 109: /* prop_specs: prop_specs IDENT INTCONST  */
#line 382 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2800 "pascal.tab.c"
    break;

  case 110: /* packed_opt: PACKEDSY  */
#line 385 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2806 "pascal.tab.c"
    break;

  case 111: /* packed_opt: %empty  */
#line 386 "pascal.y"
     { (yyval.node) = NULL; }
#line 2812 "pascal.tab.c"
    break;

  case 112: /* simple_type: LPARENT enum_list RPARENT  */
#line 389 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2818 "pascal.tab.c"
    break;

  case 113: /* simple_type: IDENT  */
#line 390 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2824 "pascal.tab.c"
    break;

  case 114: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 391 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2830 "pascal.tab.c"
    break;

  case 115: /* simple_type: IDENT PERIOD IDENT  */
#line 392 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2836 "pascal.tab.c"
    break;

  case 116: /* simple_type: spec_ref  */
#line 393 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2842 "pascal.tab.c"
    break;

  case 117: /* simple_type: lo_bound DOTDOT b_expr  */
#line 394 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2848 "pascal.tab.c"
    break;

  case 118: /* lo_bound: lo_term  */
#line 397 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2854 "pascal.tab.c"
    break;

  case 119: /* lo_bound: PLUS lo_term  */
#line 398 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2860 "pascal.tab.c"
    break;

  case 120: /* lo_bound: MINUS lo_term  */
#line 399 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2866 "pascal.tab.c"
    break;

  case 121: /* lo_bound: lo_bound PLUS b_term  */
#line 400 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2872 "pascal.tab.c"
    break;

  case 122: /* lo_bound: lo_bound MINUS b_term  */
#line 401 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2878 "pascal.tab.c"
    break;

  case 123: /* lo_term: scalar_constant  */
#line 404 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2884 "pascal.tab.c"
    break;

  case 124: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 405 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2890 "pascal.tab.c"
    break;

  case 125: /* lo_term: lo_term MUL b_factor  */
#line 406 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2896 "pascal.tab.c"
    break;

  case 126: /* lo_term: lo_term IDIV b_factor  */
#line 407 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2902 "pascal.tab.c"
    break;

  case 127: /* lo_term: lo_term IMOD b_factor  */
#line 408 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2908 "pascal.tab.c"
    break;

  case 128: /* lo_term: lo_term SHIFTOP b_factor  */
#line 409 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2914 "pascal.tab.c"
    break;

  case 129: /* b_expr: b_term  */
#line 412 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2920 "pascal.tab.c"
    break;

  case 130: /* b_expr: PLUS b_term  */
#line 413 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2926 "pascal.tab.c"
    break;

  case 131: /* b_expr: MINUS b_term  */
#line 414 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2932 "pascal.tab.c"
    break;

  case 132: /* b_expr: b_expr PLUS b_term  */
#line 415 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2938 "pascal.tab.c"
    break;

  case 133: /* b_expr: b_expr MINUS b_term  */
#line 416 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2944 "pascal.tab.c"
    break;

  case 134: /* b_term: b_factor  */
#line 419 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2950 "pascal.tab.c"
    break;

  case 135: /* b_term: b_term MUL b_factor  */
#line 420 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2956 "pascal.tab.c"
    break;

  case 136: /* b_term: b_term IDIV b_factor  */
#line 421 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2962 "pascal.tab.c"
    break;

  case 137: /* b_term: b_term IMOD b_factor  */
#line 422 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2968 "pascal.tab.c"
    break;

  case 138: /* b_term: b_term SHIFTOP b_factor  */
#line 423 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2974 "pascal.tab.c"
    break;

  case 139: /* b_factor: scalar_constant  */
#line 426 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2980 "pascal.tab.c"
    break;

  case 140: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 427 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2986 "pascal.tab.c"
    break;

  case 141: /* b_factor: LPARENT b_expr RPARENT  */
#line 428 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2992 "pascal.tab.c"
    break;

  case 142: /* record_body: record_field_list record_case_opt  */
#line 431 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2998 "pascal.tab.c"
    break;

  case 143: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 434 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3004 "pascal.tab.c"
    break;

  case 144: /* record_field_list: record_field  */
#line 435 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3010 "pascal.tab.c"
    break;

  case 145: /* $@4: %empty  */
#line 438 "pascal.y"
                  { }
#line 3016 "pascal.tab.c"
    break;

  case 146: /* record_field: id_list COLON $@4 type  */
#line 438 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3022 "pascal.tab.c"
    break;

  case 147: /* record_field: %empty  */
#line 439 "pascal.y"
     { (yyval.node) = NULL; }
#line 3028 "pascal.tab.c"
    break;

  case 148: /* case_arm_mark: %empty  */
#line 442 "pascal.y"
    { }
#line 3034 "pascal.tab.c"
    break;

  case 149: /* case_open: %empty  */
#line 445 "pascal.y"
    { }
#line 3040 "pascal.tab.c"
    break;

  case 150: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 448 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3046 "pascal.tab.c"
    break;

  case 151: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 449 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3052 "pascal.tab.c"
    break;

  case 152: /* record_case_opt: %empty  */
#line 450 "pascal.y"
     { (yyval.node) = NULL; }
#line 3058 "pascal.tab.c"
    break;

  case 153: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 453 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3064 "pascal.tab.c"
    break;

  case 154: /* record_case_list: record_case_arm  */
#line 454 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3070 "pascal.tab.c"
    break;

  case 155: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 457 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3076 "pascal.tab.c"
    break;

  case 156: /* record_case_arm: %empty  */
#line 458 "pascal.y"
     { (yyval.node) = NULL; }
#line 3082 "pascal.tab.c"
    break;

  case 157: /* var_decl_list: var_decl_list var_decl  */
#line 461 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3088 "pascal.tab.c"
    break;

  case 158: /* var_decl_list: var_decl  */
#line 462 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3094 "pascal.tab.c"
    break;

  case 159: /* var_decl: id_list COLON type SEMICOLON  */
#line 465 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3100 "pascal.tab.c"
    break;

  case 160: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 466 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3106 "pascal.tab.c"
    break;

  case 161: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 467 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3112 "pascal.tab.c"
    break;

  case 162: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 470 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3118 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 471 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3124 "pascal.tab.c"
    break;

  case 164: /* $@5: %empty  */
#line 472 "pascal.y"
                                                                               { }
#line 3130 "pascal.tab.c"
    break;

  case 165: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 472 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3136 "pascal.tab.c"
    break;

  case 166: /* $@6: %empty  */
#line 473 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3142 "pascal.tab.c"
    break;

  case 167: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 473 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3148 "pascal.tab.c"
    break;

  case 168: /* $@7: %empty  */
#line 474 "pascal.y"
                                        { }
#line 3154 "pascal.tab.c"
    break;

  case 169: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 474 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3160 "pascal.tab.c"
    break;

  case 170: /* $@8: %empty  */
#line 475 "pascal.y"
                                                                                                           { }
#line 3166 "pascal.tab.c"
    break;

  case 171: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 475 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3172 "pascal.tab.c"
    break;

  case 172: /* $@9: %empty  */
#line 476 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3178 "pascal.tab.c"
    break;

  case 173: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 476 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3184 "pascal.tab.c"
    break;

  case 174: /* $@10: %empty  */
#line 477 "pascal.y"
                                                                                                 { }
#line 3190 "pascal.tab.c"
    break;

  case 175: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 477 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3196 "pascal.tab.c"
    break;

  case 176: /* $@11: %empty  */
#line 478 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3202 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 478 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3208 "pascal.tab.c"
    break;

  case 178: /* $@12: %empty  */
#line 479 "pascal.y"
                                                                                            { }
#line 3214 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 479 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3220 "pascal.tab.c"
    break;

  case 180: /* $@13: %empty  */
#line 480 "pascal.y"
                                                                                                             { }
#line 3226 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 480 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3234 "pascal.tab.c"
    break;

  case 182: /* $@14: %empty  */
#line 483 "pascal.y"
                                                                                              { }
#line 3240 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 483 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3248 "pascal.tab.c"
    break;

  case 184: /* $@15: %empty  */
#line 486 "pascal.y"
                                                                                             { }
#line 3254 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 486 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3262 "pascal.tab.c"
    break;

  case 186: /* $@16: %empty  */
#line 489 "pascal.y"
                                                                                                    { }
#line 3268 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 489 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3276 "pascal.tab.c"
    break;

  case 188: /* $@17: %empty  */
#line 492 "pascal.y"
                                                                                                                     { }
#line 3282 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 492 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3290 "pascal.tab.c"
    break;

  case 190: /* pv_mark: %empty  */
#line 497 "pascal.y"
    { }
#line 3296 "pascal.tab.c"
    break;

  case 191: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 500 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3302 "pascal.tab.c"
    break;

  case 192: /* parameter_list_opt: LPARENT RPARENT  */
#line 501 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3308 "pascal.tab.c"
    break;

  case 193: /* parameter_list_opt: %empty  */
#line 502 "pascal.y"
     { (yyval.node) = NULL; }
#line 3314 "pascal.tab.c"
    break;

  case 194: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 505 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3320 "pascal.tab.c"
    break;

  case 195: /* parameter_decl_list: parameter_decl  */
#line 506 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3326 "pascal.tab.c"
    break;

  case 196: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 509 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3332 "pascal.tab.c"
    break;

  case 197: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 510 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3338 "pascal.tab.c"
    break;

  case 198: /* parameter_decl: VARSY id_list COLON param_type  */
#line 511 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3344 "pascal.tab.c"
    break;

  case 199: /* parameter_decl: id_list COLON param_type  */
#line 512 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3350 "pascal.tab.c"
    break;

  case 200: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 513 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3356 "pascal.tab.c"
    break;

  case 201: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 514 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3362 "pascal.tab.c"
    break;

  case 202: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 515 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3368 "pascal.tab.c"
    break;

  case 203: /* parameter_decl: IDENT id_list COLON param_type  */
#line 516 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3374 "pascal.tab.c"
    break;

  case 204: /* param_type: IDENT  */
#line 519 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3380 "pascal.tab.c"
    break;

  case 205: /* param_type: IDENT PERIOD IDENT  */
#line 520 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3386 "pascal.tab.c"
    break;

  case 206: /* param_type: ARRAYSY OFSY IDENT  */
#line 521 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3392 "pascal.tab.c"
    break;

  case 207: /* param_type: spec_ref  */
#line 522 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3398 "pascal.tab.c"
    break;

  case 208: /* routine_directives: %empty  */
#line 525 "pascal.y"
    { (yyval.node) = NULL; }
#line 3404 "pascal.tab.c"
    break;

  case 209: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 526 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3417 "pascal.tab.c"
    break;

  case 210: /* pf_params: LPARENT pf_sections RPARENT  */
#line 536 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3423 "pascal.tab.c"
    break;

  case 211: /* pf_params: LPARENT RPARENT  */
#line 537 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3429 "pascal.tab.c"
    break;

  case 212: /* pf_params: %empty  */
#line 538 "pascal.y"
     { (yyval.node) = NULL; }
#line 3435 "pascal.tab.c"
    break;

  case 213: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 541 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3441 "pascal.tab.c"
    break;

  case 214: /* pf_sections: pf_section  */
#line 542 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3447 "pascal.tab.c"
    break;

  case 215: /* pf_section: id_list COLON param_type  */
#line 545 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3453 "pascal.tab.c"
    break;

  case 216: /* pf_section: VARSY id_list COLON param_type  */
#line 546 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3459 "pascal.tab.c"
    break;

  case 217: /* pf_section: CONSTSY id_list COLON param_type  */
#line 547 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3465 "pascal.tab.c"
    break;

  case 218: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 548 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3471 "pascal.tab.c"
    break;

  case 219: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 549 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3477 "pascal.tab.c"
    break;

  case 220: /* enum_list: enum_list COMMA enum_member  */
#line 552 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3483 "pascal.tab.c"
    break;

  case 221: /* enum_list: enum_member  */
#line 553 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3489 "pascal.tab.c"
    break;

  case 222: /* enum_member: IDENT  */
#line 556 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3495 "pascal.tab.c"
    break;

  case 223: /* id_list: id_list COMMA IDENT  */
#line 559 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3501 "pascal.tab.c"
    break;

  case 224: /* id_list: IDENT  */
#line 560 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3507 "pascal.tab.c"
    break;

  case 225: /* body: BEGINSY statement_list ENDSY  */
#line 563 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3513 "pascal.tab.c"
    break;

  case 226: /* $@18: %empty  */
#line 566 "pascal.y"
                             { pas_stmt_mark(); }
#line 3519 "pascal.tab.c"
    break;

  case 227: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 566 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3525 "pascal.tab.c"
    break;

  case 228: /* $@19: %empty  */
#line 567 "pascal.y"
     { pas_stmt_mark(); }
#line 3531 "pascal.tab.c"
    break;

  case 229: /* statement_list: $@19 statement  */
#line 567 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3537 "pascal.tab.c"
    break;

  case 230: /* $@20: %empty  */
#line 570 "pascal.y"
    { pas_stmt_mark(); }
#line 3543 "pascal.tab.c"
    break;

  case 231: /* body_stmt: $@20 statement  */
#line 570 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3549 "pascal.tab.c"
    break;

  case 232: /* statement: statement_no_label  */
#line 573 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3555 "pascal.tab.c"
    break;

  case 233: /* statement: INTCONST COLON statement_no_label  */
#line 574 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3561 "pascal.tab.c"
    break;

  case 234: /* statement: IDENT COLON statement_no_label  */
#line 575 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3567 "pascal.tab.c"
    break;

  case 235: /* statement_no_label: assignment  */
#line 578 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3573 "pascal.tab.c"
    break;

  case 236: /* statement_no_label: call  */
#line 579 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3579 "pascal.tab.c"
    break;

  case 237: /* statement_no_label: compound_statement  */
#line 580 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3585 "pascal.tab.c"
    break;

  case 238: /* statement_no_label: goto_statement  */
#line 581 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3591 "pascal.tab.c"
    break;

  case 239: /* statement_no_label: if_statement  */
#line 582 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3597 "pascal.tab.c"
    break;

  case 240: /* statement_no_label: case_statement  */
#line 583 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3603 "pascal.tab.c"
    break;

  case 241: /* statement_no_label: while_statement  */
#line 584 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3609 "pascal.tab.c"
    break;

  case 242: /* statement_no_label: repeat_statement  */
#line 585 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3615 "pascal.tab.c"
    break;

  case 243: /* statement_no_label: for_statement  */
#line 586 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3621 "pascal.tab.c"
    break;

  case 244: /* statement_no_label: with_statement  */
#line 587 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3627 "pascal.tab.c"
    break;

  case 245: /* statement_no_label: try_statement  */
#line 588 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3633 "pascal.tab.c"
    break;

  case 246: /* statement_no_label: raise_statement  */
#line 589 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3639 "pascal.tab.c"
    break;

  case 247: /* statement_no_label: inherited_call  */
#line 590 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3645 "pascal.tab.c"
    break;

  case 248: /* statement_no_label: INHERITEDSY  */
#line 591 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3651 "pascal.tab.c"
    break;

  case 249: /* statement_no_label: %empty  */
#line 592 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3657 "pascal.tab.c"
    break;

  case 250: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 595 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3663 "pascal.tab.c"
    break;

  case 251: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 596 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3669 "pascal.tab.c"
    break;

  case 252: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 597 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3675 "pascal.tab.c"
    break;

  case 253: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 598 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3681 "pascal.tab.c"
    break;

  case 254: /* handler_list: handler  */
#line 601 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3687 "pascal.tab.c"
    break;

  case 255: /* handler_list: handler_list SEMICOLON handler  */
#line 602 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3693 "pascal.tab.c"
    break;

  case 256: /* handler_list: handler_list SEMICOLON  */
#line 603 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3699 "pascal.tab.c"
    break;

  case 257: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 606 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3705 "pascal.tab.c"
    break;

  case 258: /* handler: ONSY IDENT DOSY body_stmt  */
#line 607 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3711 "pascal.tab.c"
    break;

  case 259: /* raise_statement: RAISESY  */
#line 610 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3717 "pascal.tab.c"
    break;

  case 260: /* raise_statement: RAISESY expression  */
#line 611 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3723 "pascal.tab.c"
    break;

  case 261: /* inherited_call: INHERITEDSY IDENT  */
#line 614 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3729 "pascal.tab.c"
    break;

  case 262: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 615 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3735 "pascal.tab.c"
    break;

  case 263: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 616 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3741 "pascal.tab.c"
    break;

  case 264: /* call: IDENT  */
#line 619 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3747 "pascal.tab.c"
    break;

  case 265: /* call: spec_kw  */
#line 620 "pascal.y"
             { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3753 "pascal.tab.c"
    break;

  case 266: /* call: selector PERIOD IDENT  */
#line 621 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3759 "pascal.tab.c"
    break;

  case 267: /* call: call_with_args  */
#line 622 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3765 "pascal.tab.c"
    break;

  case 268: /* recv_primary: STRINGCONST  */
#line 625 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3771 "pascal.tab.c"
    break;

  case 269: /* recv_primary: LPARENT expression RPARENT  */
#line 626 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3777 "pascal.tab.c"
    break;

  case 270: /* recv_primary: spec_kw  */
#line 627 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3783 "pascal.tab.c"
    break;

  case 271: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 630 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3789 "pascal.tab.c"
    break;

  case 272: /* call_with_args: spec_kw LPARENT argument_list RPARENT  */
#line 631 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3795 "pascal.tab.c"
    break;

  case 273: /* call_with_args: spec_kw LPARENT RPARENT  */
#line 632 "pascal.y"
                             { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3801 "pascal.tab.c"
    break;

  case 274: /* call_with_args: IDENT LPARENT RPARENT  */
#line 633 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3807 "pascal.tab.c"
    break;

  case 275: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 634 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3813 "pascal.tab.c"
    break;

  case 276: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 635 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3819 "pascal.tab.c"
    break;

  case 277: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 636 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3825 "pascal.tab.c"
    break;

  case 278: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 637 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3831 "pascal.tab.c"
    break;

  case 279: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 638 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3837 "pascal.tab.c"
    break;

  case 280: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 639 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3843 "pascal.tab.c"
    break;

  case 281: /* argument_list: argument_list COMMA argument  */
#line 642 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3849 "pascal.tab.c"
    break;

  case 282: /* argument_list: argument  */
#line 643 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3855 "pascal.tab.c"
    break;

  case 283: /* argument: expression  */
#line 646 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3861 "pascal.tab.c"
    break;

  case 284: /* argument: expression COLON expression  */
#line 647 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3867 "pascal.tab.c"
    break;

  case 285: /* argument: expression COLON expression COLON expression  */
#line 648 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3873 "pascal.tab.c"
    break;

  case 286: /* assignment: selector BECOMES expression  */
#line 651 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3879 "pascal.tab.c"
    break;

  case 287: /* assignment: selector ADDBECOMES expression  */
#line 652 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3885 "pascal.tab.c"
    break;

  case 288: /* assignment: selector SUBBECOMES expression  */
#line 653 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3891 "pascal.tab.c"
    break;

  case 289: /* assignment: selector MULBECOMES expression  */
#line 654 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3897 "pascal.tab.c"
    break;

  case 290: /* assignment: selector DIVBECOMES expression  */
#line 655 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3903 "pascal.tab.c"
    break;

  case 291: /* assignment: call_with_args BECOMES expression  */
#line 656 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3909 "pascal.tab.c"
    break;

  case 292: /* selector: selector LBRACK expression_list RBRACK  */
#line 659 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3915 "pascal.tab.c"
    break;

  case 293: /* selector: selector PERIOD IDENT  */
#line 660 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3921 "pascal.tab.c"
    break;

  case 294: /* selector: selector ARROW  */
#line 661 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3927 "pascal.tab.c"
    break;

  case 295: /* selector: call_with_args PERIOD IDENT  */
#line 662 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3933 "pascal.tab.c"
    break;

  case 296: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 663 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3939 "pascal.tab.c"
    break;

  case 297: /* selector: call_with_args ARROW  */
#line 664 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3945 "pascal.tab.c"
    break;

  case 298: /* selector: recv_primary PERIOD IDENT  */
#line 665 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3951 "pascal.tab.c"
    break;

  case 299: /* selector: recv_primary ARROW  */
#line 666 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3957 "pascal.tab.c"
    break;

  case 300: /* selector: IDENT  */
#line 667 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3963 "pascal.tab.c"
    break;

  case 301: /* expression_list: expression_list COMMA expression  */
#line 670 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3969 "pascal.tab.c"
    break;

  case 302: /* expression_list: expression  */
#line 671 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3975 "pascal.tab.c"
    break;

  case 303: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 674 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3981 "pascal.tab.c"
    break;

  case 304: /* goto_statement: GOTOSY INTCONST  */
#line 677 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3987 "pascal.tab.c"
    break;

  case 305: /* goto_statement: GOTOSY IDENT  */
#line 678 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3993 "pascal.tab.c"
    break;

  case 306: /* if_statement: IFSY expression THENSY body_stmt  */
#line 681 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3999 "pascal.tab.c"
    break;

  case 307: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 682 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4005 "pascal.tab.c"
    break;

  case 308: /* $@21: %empty  */
#line 685 "pascal.y"
                           { }
#line 4011 "pascal.tab.c"
    break;

  case 309: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 685 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4017 "pascal.tab.c"
    break;

  case 310: /* case_tail: %empty  */
#line 688 "pascal.y"
    { (yyval.node) = NULL; }
#line 4023 "pascal.tab.c"
    break;

  case 311: /* case_tail: ELSESY statement_list  */
#line 689 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4029 "pascal.tab.c"
    break;

  case 312: /* case_tail: OTHERWISESY statement_list  */
#line 690 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4035 "pascal.tab.c"
    break;

  case 313: /* case_list: case_list SEMICOLON case_elem  */
#line 693 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4041 "pascal.tab.c"
    break;

  case 314: /* case_list: case_elem  */
#line 694 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4047 "pascal.tab.c"
    break;

  case 315: /* case_elem: constant_list COLON body_stmt  */
#line 697 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4053 "pascal.tab.c"
    break;

  case 316: /* case_elem: %empty  */
#line 698 "pascal.y"
     { (yyval.node) = NULL; }
#line 4059 "pascal.tab.c"
    break;

  case 317: /* constant_list: constant_list COMMA case_label  */
#line 701 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4065 "pascal.tab.c"
    break;

  case 318: /* constant_list: case_label  */
#line 702 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4071 "pascal.tab.c"
    break;

  case 319: /* case_label: b_expr  */
#line 705 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4077 "pascal.tab.c"
    break;

  case 320: /* case_label: b_expr DOTDOT b_expr  */
#line 706 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4083 "pascal.tab.c"
    break;

  case 321: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 709 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4089 "pascal.tab.c"
    break;

  case 322: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 712 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4095 "pascal.tab.c"
    break;

  case 323: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 715 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4101 "pascal.tab.c"
    break;

  case 324: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 716 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4107 "pascal.tab.c"
    break;

  case 325: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 717 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4113 "pascal.tab.c"
    break;

  case 326: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 720 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4119 "pascal.tab.c"
    break;

  case 327: /* with_open: with_open COMMA selector  */
#line 723 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4125 "pascal.tab.c"
    break;

  case 328: /* with_open: selector  */
#line 724 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4131 "pascal.tab.c"
    break;

  case 329: /* expression: simple_expression  */
#line 731 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4137 "pascal.tab.c"
    break;

  case 330: /* expression: expression INOP simple_expression  */
#line 732 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4143 "pascal.tab.c"
    break;

  case 331: /* expression: expression LTOP simple_expression  */
#line 733 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4149 "pascal.tab.c"
    break;

  case 332: /* expression: expression LEOP simple_expression  */
#line 734 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4155 "pascal.tab.c"
    break;

  case 333: /* expression: expression GTOP simple_expression  */
#line 735 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4161 "pascal.tab.c"
    break;

  case 334: /* expression: expression GEOP simple_expression  */
#line 736 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4167 "pascal.tab.c"
    break;

  case 335: /* expression: expression NEOP simple_expression  */
#line 737 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4173 "pascal.tab.c"
    break;

  case 336: /* expression: expression EQOP simple_expression  */
#line 738 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4179 "pascal.tab.c"
    break;

  case 337: /* expression: expression ISOP simple_expression  */
#line 739 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4185 "pascal.tab.c"
    break;

  case 338: /* simple_expression: term  */
#line 742 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4191 "pascal.tab.c"
    break;

  case 339: /* simple_expression: PLUS term  */
#line 743 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4197 "pascal.tab.c"
    break;

  case 340: /* simple_expression: MINUS term  */
#line 744 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4203 "pascal.tab.c"
    break;

  case 341: /* simple_expression: simple_expression PLUS term  */
#line 745 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4209 "pascal.tab.c"
    break;

  case 342: /* simple_expression: simple_expression MINUS term  */
#line 746 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4215 "pascal.tab.c"
    break;

  case 343: /* simple_expression: simple_expression OROP term  */
#line 747 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4221 "pascal.tab.c"
    break;

  case 344: /* simple_expression: simple_expression XOROP term  */
#line 748 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4227 "pascal.tab.c"
    break;

  case 345: /* term: power  */
#line 751 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4233 "pascal.tab.c"
    break;

  case 346: /* term: term MUL power  */
#line 752 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4239 "pascal.tab.c"
    break;

  case 347: /* term: term RDIV power  */
#line 753 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4245 "pascal.tab.c"
    break;

  case 348: /* term: term IDIV power  */
#line 754 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4251 "pascal.tab.c"
    break;

  case 349: /* term: term IMOD power  */
#line 755 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4257 "pascal.tab.c"
    break;

  case 350: /* term: term ANDOP power  */
#line 756 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4263 "pascal.tab.c"
    break;

  case 351: /* term: term MUL MINUS power  */
#line 757 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4269 "pascal.tab.c"
    break;

  case 352: /* term: term RDIV MINUS power  */
#line 758 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4275 "pascal.tab.c"
    break;

  case 353: /* term: term IDIV MINUS power  */
#line 759 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4281 "pascal.tab.c"
    break;

  case 354: /* term: term IMOD MINUS power  */
#line 760 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4287 "pascal.tab.c"
    break;

  case 355: /* term: term ANDOP MINUS power  */
#line 761 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4293 "pascal.tab.c"
    break;

  case 356: /* term: term SHIFTOP power  */
#line 762 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4299 "pascal.tab.c"
    break;

  case 357: /* term: term ASOP power  */
#line 763 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4305 "pascal.tab.c"
    break;

  case 358: /* power: factor  */
#line 766 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4311 "pascal.tab.c"
    break;

  case 359: /* power: power POWOP factor  */
#line 767 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4317 "pascal.tab.c"
    break;

  case 360: /* factor: selector  */
#line 770 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4323 "pascal.tab.c"
    break;

  case 361: /* factor: call_with_args  */
#line 771 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4329 "pascal.tab.c"
    break;

  case 362: /* factor: INTCONST  */
#line 772 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4335 "pascal.tab.c"
    break;

  case 363: /* factor: REALCONST  */
#line 773 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4341 "pascal.tab.c"
    break;

  case 364: /* factor: STRINGCONST  */
#line 774 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4347 "pascal.tab.c"
    break;

  case 365: /* factor: CHARCODE  */
#line 775 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4353 "pascal.tab.c"
    break;

  case 366: /* factor: LPARENT expression RPARENT  */
#line 776 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 4359 "pascal.tab.c"
    break;

  case 367: /* factor: inherited_call  */
#line 777 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4365 "pascal.tab.c"
    break;

  case 368: /* factor: NOTSY factor  */
#line 778 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4371 "pascal.tab.c"
    break;

  case 369: /* factor: ATSIGN factor  */
#line 779 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4377 "pascal.tab.c"
    break;

  case 370: /* factor: LBRACK RBRACK  */
#line 780 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4383 "pascal.tab.c"
    break;

  case 371: /* factor: LBRACK set_member_list RBRACK  */
#line 781 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4389 "pascal.tab.c"
    break;

  case 372: /* set_member_list: set_member  */
#line 784 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4395 "pascal.tab.c"
    break;

  case 373: /* set_member_list: set_member_list COMMA set_member  */
#line 785 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4401 "pascal.tab.c"
    break;

  case 374: /* set_member: expression  */
#line 788 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4407 "pascal.tab.c"
    break;

  case 375: /* set_member: expression DOTDOT expression  */
#line 789 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4413 "pascal.tab.c"
    break;


#line 4417 "pascal.tab.c"

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

#line 795 "pascal.y"

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
